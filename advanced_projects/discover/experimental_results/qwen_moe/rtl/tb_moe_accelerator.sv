// SystemVerilog Testbench for MoE Accelerator Control Frontend
`timescale 1ns / 1ps

module tb_moe_accelerator;

    logic        clk;
    logic        rst_n;
    logic        event_valid;
    logic [1:0]  event_type;
    logic [7:0]  event_req_id;
    logic [7:0]  event_layer_id;
    logic [7:0]  event_expert_id;
    logic [7:0]  event_kv_block_id;
    
    logic        ready;
    logic        stall;
    logic        expert_hit;
    logic        expert_miss;
    logic        kv_hit;
    logic        kv_miss;
    
    logic [31:0] perf_cycles;
    logic [31:0] perf_stalls;
    logic [31:0] perf_expert_hits;
    logic [31:0] perf_expert_misses;
    logic [31:0] perf_prefetches;
    logic [31:0] perf_kv_hits;
    logic [31:0] perf_kv_misses;

    // Instantiate UUT
    moe_accelerator_frontend uut (
        .clk,
        .rst_n,
        .event_valid,
        .event_type,
        .event_req_id,
        .event_layer_id,
        .event_expert_id,
        .event_kv_block_id,
        .ready,
        .stall,
        .expert_hit,
        .expert_miss,
        .kv_hit,
        .kv_miss,
        .perf_cycles,
        .perf_stalls,
        .perf_expert_hits,
        .perf_expert_misses,
        .perf_prefetches,
        .perf_kv_hits,
        .perf_kv_misses
    );

    // Clock Generator
    always #5 clk = ~clk;

    // Test variables
    int file_h;
    int scan_r;
    int ev_type_val;
    int layer_id_val;
    int exp_id_val;
    int kv_block_val;
    int count = 0;

    initial begin
        clk = 0;
        rst_n = 0;
        event_valid = 0;
        event_type = 2'b00;
        event_req_id = 8'd0;
        event_layer_id = 8'd0;
        event_expert_id = 8'd0;
        event_kv_block_id = 8'd0;
        
        #20;
        rst_n = 1;
        #10;
        
        // Open the trace file
        file_h = $fopen("/home/a/discover/rtl/sim_trace.txt", "r");
        if (file_h == 0) begin
            $display("ERROR: Failed to open sim_trace.txt");
            $finish;
        end
        
        $display("--- RTL Simulation Started ---");
        
        // Read file line by line
        while (!$feof(file_h)) begin
            @(posedge clk);
            if (ready) begin
                // Format: event_type layer_id expert_id kv_block_id
                scan_r = $fscanf(file_h, "%d %d %d %d\n", ev_type_val, layer_id_val, exp_id_val, kv_block_val);
                if (scan_r == 4) begin
                    event_valid <= 1'b1;
                    event_type <= ev_type_val[1:0];
                    event_layer_id <= layer_id_val[7:0];
                    event_expert_id <= exp_id_val[7:0];
                    event_kv_block_id <= kv_block_val[7:0];
                    event_req_id <= 8'd0;
                    count <= count + 1;
                end else begin
                    event_valid <= 1'b0;
                end
            end else begin
                // Hold input values, but set valid to 0 if we are stalling
                // actually in a real CPU the request remains pending, but for trace replay
                // we disable event_valid while stalling so the same event is not re-driven
                event_valid <= 1'b0;
            end
        end
        
        // Final flush
        @(posedge clk);
        event_valid <= 1'b0;
        
        // Wait for memory transfers to finish
        repeat (100) @(posedge clk);
        
        $fclose(file_h);
        
        // Print Summary Report
        $display("\n================ RTL SIMULATION SUMMARY ================");
        $display("Total Cycles:          %d", perf_cycles);
        $display("Stall Cycles:          %d", perf_stalls);
        $display("Stall Ratio:           %2.2f%%", (real'(perf_stalls) / real'(perf_cycles)) * 100.0);
        $display("Expert Hits:           %d", perf_expert_hits);
        $display("Expert Misses:         %d", perf_expert_misses);
        $display("Expert Hit Rate:       %2.2f%%", (real'(perf_expert_hits) / (real'(perf_expert_hits) + real'(perf_expert_misses))) * 100.0);
        $display("KV Hits:               %d", perf_kv_hits);
        $display("KV Misses:             %d", perf_kv_misses);
        $display("KV Hit Rate:           %2.2f%%", (real'(perf_kv_hits) / (real'(perf_kv_hits) + real'(perf_kv_misses))) * 100.0);
        $display("Prefetches Triggered:  %d", perf_prefetches);
        $display("========================================================\n");
        
        // Save to file
        file_h = $fopen("/home/a/discover/rtl/rtl_simulation.log", "w");
        if (file_h != 0) begin
            $fdisplay(file_h, "RTL Simulation Log");
            $fdisplay(file_h, "Total Cycles: %d", perf_cycles);
            $fdisplay(file_h, "Stall Cycles: %d", perf_stalls);
            $fdisplay(file_h, "Expert Hits: %d", perf_expert_hits);
            $fdisplay(file_h, "Expert Misses: %d", perf_expert_misses);
            $fdisplay(file_h, "KV Hits: %d", perf_kv_hits);
            $fdisplay(file_h, "KV Misses: %d", perf_kv_misses);
            $fdisplay(file_h, "Prefetches: %d", perf_prefetches);
            $fclose(file_h);
        end
        
        $finish;
    end

endmodule
