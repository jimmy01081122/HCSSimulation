// tb_top_moe_prefetch_system.v
`timescale 1ns/1ps

module tb_top_moe_prefetch_system;

    reg clk;
    reg rst_n;

    // Demand Request Interface
    reg                   req_valid;
    reg  [3:0]            req_expert_id;
    wire                  req_ready;

    // Prefetch Hint Interface
    reg                   prefetch_hint_valid;
    reg  [3:0]            prefetch_hint_expert_id;
    reg  [7:0]            prefetch_hint_score;
    reg  [7:0]            score_threshold;
    reg                   prefetch_enable;
    reg                   replacement_policy_sel;

    // Response Interface
    wire                  resp_valid;
    wire [3:0]            resp_expert_id;

    // Counters
    wire [15:0]           cnt_total_req;
    wire [15:0]           cnt_hit;
    wire [15:0]           cnt_miss;
    wire [15:0]           cnt_prefetch_issued;
    wire [15:0]           cnt_prefetch_hit;
    wire [15:0]           cnt_prefetch_useless;
    wire [15:0]           cnt_prefetch_dropped;
    wire [15:0]           cnt_prefetch_filtered;

    // Instantiate UUT
    top_moe_prefetch_system #(
        .EXPERT_ID_W(4),
        .NUM_WAYS(4),
        .WAY_W(2),
        .SCORE_W(8),
        .CNT_W(16),
        .DMA_LATENCY(10)
    ) uut (
        .clk(clk),
        .rst_n(rst_n),
        .req_valid(req_valid),
        .req_expert_id(req_expert_id),
        .req_ready(req_ready),
        .prefetch_hint_valid(prefetch_hint_valid),
        .prefetch_hint_expert_id(prefetch_hint_expert_id),
        .prefetch_hint_score(prefetch_hint_score),
        .score_threshold(score_threshold),
        .prefetch_enable(prefetch_enable),
        .replacement_policy_sel(replacement_policy_sel),
        .resp_valid(resp_valid),
        .resp_expert_id(resp_expert_id),
        .cnt_total_req(cnt_total_req),
        .cnt_hit(cnt_hit),
        .cnt_miss(cnt_miss),
        .cnt_prefetch_issued(cnt_prefetch_issued),
        .cnt_prefetch_hit(cnt_prefetch_hit),
        .cnt_prefetch_useless(cnt_prefetch_useless),
        .cnt_prefetch_dropped(cnt_prefetch_dropped),
        .cnt_prefetch_filtered(cnt_prefetch_filtered)
    );

`ifndef STIM_LEN
`define STIM_LEN 10000
`endif

    // Stimulus memory
    reg [31:0] req_mem [0:`STIM_LEN-1];
    reg [31:0] hint_mem [0:`STIM_LEN-1];
    
    reg [31:0] cycle;
    integer total_stim_reqs;
    integer idx;

    // Clock generator
    always #5 clk = ~clk;

    // Watchdog timer (increased to 200,000ns to allow full stall cycles)
    initial begin
        #200000;
        $display("FAIL: tb_top_moe_prefetch_system timeout");
        $finish;
    end

    // Load stimulus and initialize
    initial begin
        // Initialize memory with zeros to avoid warnings about uninitialized locations
        for (idx = 0; idx < `STIM_LEN; idx = idx + 1) begin
            req_mem[idx]  = 32'd0;
            hint_mem[idx] = 32'd0;
        end

        $readmemh("tb/stimulus_req.hex", req_mem);
        $readmemh("tb/stimulus_hint.hex", hint_mem);

        // Count total valid requests in stimulus
        total_stim_reqs = 0;
        for (idx = 0; idx < `STIM_LEN; idx = idx + 1) begin
            if (req_mem[idx][31]) begin
                total_stim_reqs = total_stim_reqs + 1;
            end
        end
        $display("Loaded stimulus. Total requests to process: %d", total_stim_reqs);

        clk = 0;
        rst_n = 0;
        cycle = 0;
        req_valid = 0;
        req_expert_id = 0;
        prefetch_hint_valid = 0;
        prefetch_hint_expert_id = 0;
        prefetch_hint_score = 0;
        
        // Fixed settings
        score_threshold = 8'd25; // 0.1 * 255 = 25.5
        prefetch_enable = 1;
`ifndef REPL_POLICY_SEL
        replacement_policy_sel = 0; // FIFO
`else
        replacement_policy_sel = `REPL_POLICY_SEL;
`endif

        #20;
        rst_n = 1;
    end

    // Stimulus feeding logic with valid/ready handshake
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            req_valid <= 0;
            req_expert_id <= 0;
            prefetch_hint_valid <= 0;
            prefetch_hint_expert_id <= 0;
            prefetch_hint_score <= 0;
            cycle <= 0;
        end else begin
            if (req_valid && !req_ready) begin
                // Hold request, do not advance cycle
            end else begin
                if (cycle < `STIM_LEN) begin
                    req_valid <= req_mem[cycle][31];
                    req_expert_id <= req_mem[cycle][3:0];

                    prefetch_hint_valid <= hint_mem[cycle][31];
                    prefetch_hint_score <= hint_mem[cycle][15:8];
                    prefetch_hint_expert_id <= hint_mem[cycle][3:0];

                    cycle <= cycle + 1;
                end else begin
                    req_valid <= 0;
                    prefetch_hint_valid <= 0;
                end
            end
        end
    end

    // Termination check
    always @(posedge clk) begin
        if (rst_n && cnt_total_req == total_stim_reqs && total_stim_reqs > 0) begin
            #100; // Let final completions propagate
            $display("--- RTL Simulation Counters ---");
            $display("cnt_total_req:         %d", cnt_total_req);
            $display("cnt_hit:               %d", cnt_hit);
            $display("cnt_miss:              %d", cnt_miss);
            $display("cnt_prefetch_issued:   %d", cnt_prefetch_issued);
            $display("cnt_prefetch_hit:      %d", cnt_prefetch_hit);
            $display("cnt_prefetch_useless:  %d", cnt_prefetch_useless);
            $display("cnt_prefetch_dropped:  %d", cnt_prefetch_dropped);
            $display("cnt_prefetch_filtered: %d", cnt_prefetch_filtered);
            $display("--------------------------------");
            if (cnt_hit + cnt_miss == cnt_total_req) begin
                $display("PASS tb_top_moe_prefetch_system");
            end else begin
                $display("FAIL: cnt_hit + cnt_miss != cnt_total_req");
                $display("FAIL tb_top_moe_prefetch_system");
            end
            $finish;
        end
    end

endmodule

