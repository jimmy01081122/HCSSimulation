// mena_top.sv
// Top-level MiMoE Neural Accelerator (MENA) Subsystem.
// Integrates TopK Selector, Token Dispatcher, Expert Queue Scheduler,
// Expert Cache Controller, and Weight Buffer Controller with AXI-lite registers
// and performance counters.

module mena_top #(
    parameter int NUM_EXPERTS = 8,
    parameter int CACHE_SIZE  = 4,
    parameter int QUEUE_WIDTH = 6,
    parameter int ADDR_WIDTH  = 3  // $clog2(NUM_EXPERTS)
) (
    input  logic                   clk,
    input  logic                   rst_n,

    // AXI-lite Register Interface (Simple MMIO ports)
    input  logic                   reg_write_valid,
    input  logic [5:0]             reg_addr, // 0x00 to 0x14
    input  logic [31:0]            reg_write_data,
    output logic [31:0]            reg_read_data,

    // Mock DRAM memory interface
    output logic                   mem_req_valid,
    output logic [ADDR_WIDTH-1:0]  mem_req_expert,
    output logic                   mem_req_is_pf,
    input  logic                   mem_req_ready,
    input  logic                   mem_fill_valid,
    input  logic [ADDR_WIDTH-1:0]  mem_fill_expert,

    // Dispatch input to load tokens (from routing layer)
    input  logic                   token_wr_en,
    input  logic [QUEUE_WIDTH-1:0] token_wr_id,
    input  logic [2*ADDR_WIDTH-1:0]token_wr_experts // packed top-2 expert IDs
);

    // ----------------------------------------------------
    // Registers (AXI-lite compatible Register Map)
    // ----------------------------------------------------
    logic [31:0] reg_ctrl;          // 0x00: CTRL
    logic [31:0] reg_status;        // 0x04: STATUS (bit0=DONE, bit1=BUSY)
    logic [31:0] reg_cache_hit;     // 0x08: CACHE_HIT
    logic [31:0] reg_cache_miss;    // 0x0C: CACHE_MISS
    logic [31:0] reg_dma_cycles;    // 0x10: DMA_CYCLES
    logic [31:0] reg_total_cycles;  // 0x14: TOTAL_CYCLES

    // Control bits
    logic start_pulse;
    logic reset_pulse;
    assign start_pulse = reg_write_valid && (reg_addr == 6'h00) && reg_write_data[0];
    assign reset_pulse = reg_write_valid && (reg_addr == 6'h00) && reg_write_data[1];

    // Status bits
    logic busy_reg, done_reg;
    assign reg_status = {30'd0, busy_reg, done_reg};

    // ----------------------------------------------------
    // Internal Signals & Interconnect
    // ----------------------------------------------------
    // Token Dispatcher outputs
    logic [NUM_EXPERTS*QUEUE_WIDTH-1:0] expert_counts;
    logic                               dispatch_valid;
    logic [ADDR_WIDTH-1:0]              dispatch_expert_id;
    logic [4:0]                         dispatch_token_id;
    logic                               dispatch_done;

    // Scheduler outputs
    logic [ADDR_WIDTH-1:0]              next_sched_expert;
    logic                               sched_valid;

    // Cache Controller outputs
    logic                               cache_lookup_hit;
    logic                               cache_lookup_ready;

    // Weight Buffer Controller outputs
    logic                               wbuf_active_sel;
    logic                               wbuf_dma_sel;
    logic                               wbuf_stall;
    logic                               wbuf_swap_event;
    logic                               wbuf_shadow_valid;
    logic                               wbuf_active_valid;

    // ----------------------------------------------------
    // Performance Counters Logic
    // ----------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n || reset_pulse) begin
            reg_ctrl         <= 32'h0;
            busy_reg         <= 1'b0;
            done_reg         <= 1'b0;
            reg_cache_hit    <= 32'h0;
            reg_cache_miss   <= 32'h0;
            reg_dma_cycles   <= 32'h0;
            reg_total_cycles <= 32'h0;
        end else begin
            // Handle Start
            if (start_pulse) begin
                busy_reg         <= 1'b1;
                done_reg         <= 1'b0;
                reg_cache_hit    <= 32'h0;
                reg_cache_miss   <= 32'h0;
                reg_dma_cycles   <= 32'h0;
                reg_total_cycles <= 32'h0;
            end

            // Total Cycles Counter (active when busy)
            if (busy_reg) begin
                reg_total_cycles <= reg_total_cycles + 1'b1;
                if (dispatch_done) begin
                    busy_reg <= 1'b0;
                    done_reg <= 1'b1;
                end
            end

            // DMA Cycles Counter (active when loading from memory)
            if (busy_reg && mem_req_valid && !mem_req_ready) begin
                reg_dma_cycles <= reg_dma_cycles + 1'b1;
            end

            // Cache Hits / Misses Performance counters
            if (busy_reg && sched_valid && cache_lookup_ready) begin
                if (cache_lookup_hit) begin
                    reg_cache_hit <= reg_cache_hit + 1'b1;
                end else begin
                    reg_cache_miss <= reg_cache_miss + 1'b1;
                end
            end
        end
    end

    // MMIO Read Interface
    always_comb begin
        case (reg_addr)
            6'h00:   reg_read_data = reg_ctrl;
            6'h04:   reg_read_data = reg_status;
            6'h08:   reg_read_data = reg_cache_hit;
            6'h0C:   reg_read_data = reg_cache_miss;
            6'h10:   reg_read_data = reg_dma_cycles;
            6'h14:   reg_read_data = reg_total_cycles;
            default: reg_read_data = 32'h0;
        endcase
    end

    // ----------------------------------------------------
    // Instantiations of Submodules
    // ----------------------------------------------------

    // 1. Token Dispatcher
    token_dispatcher #(
        .NUM_EXPERTS(NUM_EXPERTS),
        .NUM_TOKENS(32),
        .TOP_K(2)
    ) u_dispatcher (
        .clk(clk),
        .rst_n(rst_n && !reset_pulse),
        .wr_en(token_wr_en),
        .wr_token_id(token_wr_id[4:0]),
        .wr_expert_ids(token_wr_experts),
        .start_dispatch(start_pulse),
        .expert_count(expert_counts),
        .dispatch_valid(dispatch_valid),
        .dispatch_expert_id(dispatch_expert_id),
        .dispatch_token_id(dispatch_token_id),
        .dispatch_done(dispatch_done)
    );

    // 2. Expert Queue Scheduler
    expert_queue_scheduler #(
        .NUM_EXPERTS(NUM_EXPERTS),
        .QUEUE_WIDTH(QUEUE_WIDTH),
        .SCHED_POLICY(0) // Round Robin by default
    ) u_scheduler (
        .clk(clk),
        .rst_n(rst_n && !reset_pulse),
        .expert_counts(expert_counts),
        .valid_in(busy_reg),
        .next_expert(next_sched_expert),
        .sched_valid(sched_valid)
    );

    // 3. Expert Cache Controller
    expert_cache_ctrl #(
        .CACHE_SIZE(CACHE_SIZE),
        .NUM_EXPERTS(NUM_EXPERTS),
        .ADDR_WIDTH(ADDR_WIDTH)
    ) u_cache (
        .clk(clk),
        .rst_n(rst_n && !reset_pulse),
        .lookup_valid(sched_valid),
        .lookup_expert(next_sched_expert),
        .lookup_hit(cache_lookup_hit),
        .lookup_ready(cache_lookup_ready),
        .prefetch_valid(1'b0), // prefetches disabled by default in RTL
        .prefetch_expert('0),
        .mem_req_valid(mem_req_valid),
        .mem_req_expert(mem_req_expert),
        .mem_req_is_pf(mem_req_is_pf),
        .mem_req_ready(mem_req_ready),
        .mem_fill_valid(mem_fill_valid),
        .mem_fill_expert(mem_fill_expert)
    );

    // 4. Weight Buffer Controller (Ping-pong double buffer manager)
    weight_buffer_ctrl #(
        .NUM_EXPERTS(NUM_EXPERTS),
        .BUF_ID_WIDTH(1)
    ) u_wbuf_ctrl (
        .clk(clk),
        .rst_n(rst_n && !reset_pulse),
        .dma_start(mem_req_valid && mem_req_ready),
        .shadow_done(mem_fill_valid),
        .exec_start(dispatch_valid),
        .exec_done(dispatch_done),
        .active_sel(wbuf_active_sel),
        .dma_sel(wbuf_dma_sel),
        .stall(wbuf_stall),
        .swap_event(wbuf_swap_event),
        .shadow_valid(wbuf_shadow_valid),
        .active_valid(wbuf_active_valid)
    );

    // Silence unused internal signals/ports
    logic _unused_top;
    assign _unused_top = &{1'b0, wbuf_active_sel, wbuf_dma_sel, wbuf_stall, 
                           wbuf_swap_event, wbuf_shadow_valid, wbuf_active_valid,
                           dispatch_expert_id, dispatch_token_id, dispatch_valid,
                           reg_write_data[31:2], token_wr_id[5]};

endmodule
