// SystemVerilog MoE Accelerator Control Frontend and Memory Scheduler
// Features: Expert Cache (LRU, 2 experts/layer, 6 layers), Prefetch Queue, KV Block Table, Bandwidth Arbiter, Perf Counters

`timescale 1ns / 1ps

// Event types
localparam [1:0] EVENT_EXPERT_ACCESS    = 2'b00;
localparam [1:0] EVENT_EXPERT_PREFETCH  = 2'b01;
localparam [1:0] EVENT_KV_ACCESS        = 2'b10;
localparam [1:0] EVENT_COMPUTE          = 2'b11;

// Top level module
module moe_accelerator_frontend (
    input  logic        clk,
    input  logic        rst_n,
    
    // Trace Replay Interface
    input  logic        event_valid,
    input  logic [1:0]  event_type,
    input  logic [7:0]  event_req_id,
    input  logic [7:0]  event_layer_id,
    input  logic [7:0]  event_expert_id,
    input  logic [7:0]  event_kv_block_id,
    
    output logic        ready,
    output logic        stall,
    output logic        expert_hit,
    output logic        expert_miss,
    output logic        kv_hit,
    output logic        kv_miss,
    
    // Performance Counters
    output logic [31:0] perf_cycles,
    output logic [31:0] perf_stalls,
    output logic [31:0] perf_expert_hits,
    output logic [31:0] perf_expert_misses,
    output logic [31:0] perf_prefetches,
    output logic [31:0] perf_kv_hits,
    output logic [31:0] perf_kv_misses
);

    // Params
    localparam NUM_LAYERS = 6;
    localparam EXPERT_CAPACITY_PER_LAYER = 2;
    localparam TRANSFER_CYCLES_EXPERT = 16;
    localparam TRANSFER_CYCLES_KV = 2;
    localparam PREFETCH_FIFO_DEPTH = 8;
    localparam KV_BLOCK_CAPACITY = 16;

    // ----------------------------------------------------
    // 1. Expert Cache State
    // ----------------------------------------------------
    logic [7:0] expert_cache_tags [0:NUM_LAYERS-1][0:EXPERT_CAPACITY_PER_LAYER-1];
    logic       expert_cache_valid [0:NUM_LAYERS-1][0:EXPERT_CAPACITY_PER_LAYER-1];
    logic       expert_cache_lru [0:NUM_LAYERS-1]; // LRU way

    // ----------------------------------------------------
    // 2. KV Cache Block Table State
    // ----------------------------------------------------
    logic [7:0] kv_block_tags [0:KV_BLOCK_CAPACITY-1];
    logic       kv_block_valid [0:KV_BLOCK_CAPACITY-1];
    logic [3:0] kv_block_lru [0:KV_BLOCK_CAPACITY-1];

    // ----------------------------------------------------
    // 3. Prefetch Queue (FIFO)
    // ----------------------------------------------------
    logic [7:0] prefetch_fifo_layer [0:PREFETCH_FIFO_DEPTH-1];
    logic [7:0] prefetch_fifo_expert [0:PREFETCH_FIFO_DEPTH-1];
    logic [3:0] prefetch_rptr, prefetch_wptr;
    logic       prefetch_fifo_full, prefetch_fifo_empty;

    assign prefetch_fifo_empty = (prefetch_rptr == prefetch_wptr);
    assign prefetch_fifo_full  = ((prefetch_wptr[2:0] == prefetch_rptr[2:0]) && (prefetch_wptr[3] != prefetch_rptr[3]));

    // ----------------------------------------------------
    // 4. Bandwidth Arbiter State
    // ----------------------------------------------------
    logic        bus_busy;
    logic [7:0]  bus_timer;
    logic [1:0]  bus_owner; // 00: Idle, 01: KV Load, 10: Expert Load, 11: Prefetch Load
    
    logic        req_exp_load;
    logic [7:0]  req_exp_layer;
    logic [7:0]  req_exp_id;
    
    logic        req_kv_load;
    logic [7:0]  req_kv_block;
    
    logic        req_prefetch_load;
    logic [7:0]  req_prefetch_layer;
    logic [7:0]  req_prefetch_id;

    // ----------------------------------------------------
    // Control and Stall Logic
    // ----------------------------------------------------
    logic is_expert_access, is_expert_prefetch, is_kv_access, is_compute;
    assign is_expert_access   = event_valid && (event_type == EVENT_EXPERT_ACCESS);
    assign is_expert_prefetch = event_valid && (event_type == EVENT_EXPERT_PREFETCH);
    assign is_kv_access       = event_valid && (event_type == EVENT_KV_ACCESS);
    assign is_compute         = event_valid && (event_type == EVENT_COMPUTE);

    assign ready = !stall;
    assign stall = (bus_busy && (bus_owner != 2'b11)) || req_exp_load || req_kv_load;

    // Helper variables for loop execution - declared at module scope to avoid compiler limitations
    logic       lru_way;
    logic       p_already_cached;
    logic       hit_found;
    logic [7:0] l_id, e_id;
    logic       kv_hit_found;
    logic [7:0] kv_block;
    logic       found_invalid;
    integer     victim_idx;
    logic [3:0] max_lru_val;
    logic [7:0] p_layer, p_exp;

    // Loop indices declared as integers for standard Verilog parser support
    integer l, w, b, i, other_b;

    // ----------------------------------------------------
    // Main Sequential Block
    // ----------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            // Reset Expert Cache
            for (l = 0; l < NUM_LAYERS; l = l + 1) begin
                expert_cache_lru[l] <= 1'b0;
                for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                    expert_cache_tags[l][w] <= 8'hFF;
                    expert_cache_valid[l][w] <= 1'b0;
                end
            end
            
            // Reset KV Cache Block Table
            for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                kv_block_tags[b] <= 8'hFF;
                kv_block_valid[b] <= 1'b0;
                kv_block_lru[b] <= b[3:0];
            end
            
            // Reset Prefetch Queue
            prefetch_rptr <= 4'd0;
            prefetch_wptr <= 4'd0;
            for (i = 0; i < PREFETCH_FIFO_DEPTH; i = i + 1) begin
                prefetch_fifo_layer[i] <= 8'd0;
                prefetch_fifo_expert[i] <= 8'd0;
            end
            
            // Reset Arbiter
            bus_busy <= 1'b0;
            bus_timer <= 8'd0;
            bus_owner <= 2'b00;
            
            req_exp_load <= 1'b0;
            req_exp_layer <= 8'd0;
            req_exp_id <= 8'd0;
            req_kv_load <= 1'b0;
            req_kv_block <= 8'd0;
            req_prefetch_load <= 1'b0;
            req_prefetch_layer <= 8'd0;
            req_prefetch_id <= 8'd0;
            
            // Reset Perf Counters
            perf_cycles <= 32'd0;
            perf_stalls <= 32'd0;
            perf_expert_hits <= 32'd0;
            perf_expert_misses <= 32'd0;
            perf_prefetches <= 32'd0;
            perf_kv_hits <= 32'd0;
            perf_kv_misses <= 32'd0;
            
            expert_hit <= 1'b0;
            expert_miss <= 1'b0;
            kv_hit <= 1'b0;
            kv_miss <= 1'b0;
            
        end else begin
            perf_cycles <= perf_cycles + 1;
            if (stall) begin
                perf_stalls <= perf_stalls + 1;
            end
            
            expert_hit <= 1'b0;
            expert_miss <= 1'b0;
            kv_hit <= 1'b0;
            kv_miss <= 1'b0;

            // ------------------------------------------------
            // Memory Bus Management (Arbiter Timer)
            // ------------------------------------------------
            if (bus_busy) begin
                if (bus_timer > 1) begin
                    bus_timer <= bus_timer - 1;
                end else begin
                    bus_busy <= 1'b0;
                    bus_timer <= 8'd0;
                    
                    if (bus_owner == 2'b10) begin // Expert Load Done
                        lru_way = expert_cache_lru[req_exp_layer];
                        expert_cache_tags[req_exp_layer][lru_way] <= req_exp_id;
                        expert_cache_valid[req_exp_layer][lru_way] <= 1'b1;
                        expert_cache_lru[req_exp_layer] <= ~lru_way;
                    end else if (bus_owner == 2'b11) begin // Prefetch Load Done
                        lru_way = expert_cache_lru[req_prefetch_layer];
                        expert_cache_tags[req_prefetch_layer][lru_way] <= req_prefetch_id;
                        expert_cache_valid[req_prefetch_layer][lru_way] <= 1'b1;
                        expert_cache_lru[req_prefetch_layer] <= ~lru_way;
                    end else if (bus_owner == 2'b01) begin // KV Block Load Done
                        victim_idx = 0;
                        max_lru_val = 4'd0;
                        found_invalid = 1'b0;
                        for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                            if (!found_invalid && !kv_block_valid[b]) begin
                                victim_idx = b;
                                found_invalid = 1'b1;
                            end else if (!found_invalid && (kv_block_lru[b] > max_lru_val)) begin
                                max_lru_val = kv_block_lru[b];
                                victim_idx = b;
                            end
                        end
                        kv_block_tags[victim_idx] <= req_kv_block;
                        kv_block_valid[victim_idx] <= 1'b1;
                        for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                            if (kv_block_valid[b] && b != victim_idx) begin
                                kv_block_lru[b] <= kv_block_lru[b] + 1;
                            end
                        end
                        kv_block_lru[victim_idx] <= 4'd0;
                    end
                    bus_owner <= 2'b00;
                end
            end

            // ------------------------------------------------
            // Trace Replay Event Processing
            // ------------------------------------------------
            if (event_valid && !stall) begin
                if (is_expert_access) begin
                    l_id = event_layer_id;
                    e_id = event_expert_id;
                    hit_found = 1'b0;
                    for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                        if (expert_cache_valid[l_id][w] && (expert_cache_tags[l_id][w] == e_id)) begin
                            hit_found = 1'b1;
                            expert_cache_lru[l_id] <= ~w[0];
                        end
                    end
                    
                    if (hit_found) begin
                        expert_hit <= 1'b1;
                        perf_expert_hits <= perf_expert_hits + 1;
                    end else begin
                        expert_miss <= 1'b1;
                        perf_expert_misses <= perf_expert_misses + 1;
                        req_exp_load <= 1'b1;
                        req_exp_layer <= l_id;
                        req_exp_id <= e_id;
                    end
                end
                
                else if (is_expert_prefetch) begin
                    if (!prefetch_fifo_full) begin
                        prefetch_fifo_layer[prefetch_wptr[2:0]] <= event_layer_id;
                        prefetch_fifo_expert[prefetch_wptr[2:0]] <= event_expert_id;
                        prefetch_wptr <= prefetch_wptr + 1;
                        perf_prefetches <= perf_prefetches + 1;
                    end
                end
                
                else if (is_kv_access) begin
                    kv_block = event_kv_block_id;
                    kv_hit_found = 1'b0;
                    for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                        if (kv_block_valid[b] && (kv_block_tags[b] == kv_block)) begin
                            kv_hit_found = 1'b1;
                            kv_block_lru[b] <= 4'd0;
                            for (other_b = 0; other_b < KV_BLOCK_CAPACITY; other_b = other_b + 1) begin
                                if (other_b != b && kv_block_valid[other_b]) begin
                                    kv_block_lru[other_b] <= kv_block_lru[other_b] + 1;
                                end
                            end
                        end
                    end
                    
                    if (kv_hit_found) begin
                        kv_hit <= 1'b1;
                        perf_kv_hits <= perf_kv_hits + 1;
                    end else begin
                        kv_miss <= 1'b1;
                        perf_kv_misses <= perf_kv_misses + 1;
                        req_kv_load <= 1'b1;
                        req_kv_block <= kv_block;
                    end
                end
            end

            // ------------------------------------------------
            // Bandwidth Arbiter & Scheduling
            // ------------------------------------------------
            if (!bus_busy) begin
                if (req_kv_load) begin
                    bus_busy <= 1'b1;
                    bus_timer <= TRANSFER_CYCLES_KV;
                    bus_owner <= 2'b01;
                    req_kv_load <= 1'b0;
                end else if (req_exp_load) begin
                    bus_busy <= 1'b1;
                    bus_timer <= TRANSFER_CYCLES_EXPERT;
                    bus_owner <= 2'b10;
                    req_exp_load <= 1'b0;
                end else if (!prefetch_fifo_empty) begin
                    p_layer = prefetch_fifo_layer[prefetch_rptr[2:0]];
                    p_exp = prefetch_fifo_expert[prefetch_rptr[2:0]];
                    
                    p_already_cached = 1'b0;
                    for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                        if (expert_cache_valid[p_layer][w] && (expert_cache_tags[p_layer][w] == p_exp)) begin
                            p_already_cached = 1'b1;
                        end
                    end
                    
                    prefetch_rptr <= prefetch_rptr + 1;
                    
                    if (!p_already_cached) begin
                        bus_busy <= 1'b1;
                        bus_timer <= TRANSFER_CYCLES_EXPERT;
                        bus_owner <= 2'b11;
                        req_prefetch_layer <= p_layer;
                        req_prefetch_id <= p_exp;
                    end
                end
            end
        end
    end

endmodule
