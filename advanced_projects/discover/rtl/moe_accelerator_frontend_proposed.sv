// SystemVerilog MoE Accelerator Proposed Joint Scheduler Frontend
// Features: Criticality-aware Arbiter, Prefetch Drop Controller, 
//           Prefetch Confidence Filter, Deadline Tracker, Performance Counters
// Compatible with Yosys 0.9 syntax.

`timescale 1ns / 1ps

// Event types
localparam [1:0] EVENT_EXPERT_DEMAND    = 2'b00;
localparam [1:0] EVENT_EXPERT_PREFETCH  = 2'b01;
localparam [1:0] EVENT_KV_ACCESS        = 2'b10;
localparam [1:0] EVENT_COMPUTE          = 2'b11;

module moe_accelerator_frontend_proposed (
    input  logic        clk,
    input  logic        rst_n,
    
    // Trace Replay Interface
    input  logic        event_valid,
    input  logic [1:0]  event_type,
    input  logic [7:0]  event_req_id,
    input  logic [7:0]  event_layer_id,
    input  logic [7:0]  event_expert_id,
    input  logic [7:0]  event_kv_block_id,
    input  logic [31:0] event_size_bytes,
    input  logic [31:0] event_deadline,
    input  logic [7:0]  event_confidence, // 0 to 100
    input  logic        event_criticality,
    
    output logic        ready,
    output logic        stall,
    output logic        expert_hit,
    output logic        expert_miss,
    output logic        kv_hit,
    output logic        kv_miss,
    
    // Performance Counters
    output logic [31:0] perf_cycles,
    output logic [31:0] perf_expert_demand_served,
    output logic [31:0] perf_expert_prefetch_served,
    output logic [31:0] perf_expert_prefetch_dropped,
    output logic [31:0] perf_kv_served,
    output logic [31:0] perf_deadline_misses,
    output logic [31:0] perf_memory_idle_cycles,
    output logic [31:0] perf_queue_occupancy_sum,
    output logic [31:0] perf_arbitration_decision_count
);

    // Params
    localparam NUM_LAYERS = 24;
    localparam EXPERT_CAPACITY_PER_LAYER = 8;
    localparam TRANSFER_CYCLES_EXPERT = 16;
    localparam TRANSFER_CYCLES_KV = 2;
    localparam DEMAND_QUEUE_DEPTH = 8;
    localparam PREFETCH_QUEUE_DEPTH = 8;
    localparam KV_BLOCK_CAPACITY = 16;
    
    // ----------------------------------------------------
    // 1. Expert Cache State
    // ----------------------------------------------------
    logic [7:0] expert_cache_tags [0:NUM_LAYERS-1][0:EXPERT_CAPACITY_PER_LAYER-1];
    logic       expert_cache_valid [0:NUM_LAYERS-1][0:EXPERT_CAPACITY_PER_LAYER-1];
    logic [2:0] expert_cache_lru [0:NUM_LAYERS-1][0:EXPERT_CAPACITY_PER_LAYER-1];

    // ----------------------------------------------------
    // 2. KV Cache Block Table State
    // ----------------------------------------------------
    logic [7:0] kv_block_tags [0:KV_BLOCK_CAPACITY-1];
    logic       kv_block_valid [0:KV_BLOCK_CAPACITY-1];
    logic [3:0] kv_block_lru [0:KV_BLOCK_CAPACITY-1];

    // ----------------------------------------------------
    // 3. Demand Queue (FIFO)
    // ----------------------------------------------------
    logic [7:0]  demand_q_req_id [0:DEMAND_QUEUE_DEPTH-1];
    logic [7:0]  demand_q_layer_id [0:DEMAND_QUEUE_DEPTH-1];
    logic [7:0]  demand_q_expert_id [0:DEMAND_QUEUE_DEPTH-1];
    logic [7:0]  demand_q_kv_block [0:DEMAND_QUEUE_DEPTH-1];
    logic [31:0] demand_q_deadline [0:DEMAND_QUEUE_DEPTH-1];
    logic        demand_q_is_kv [0:DEMAND_QUEUE_DEPTH-1];
    logic        demand_q_criticality [0:DEMAND_QUEUE_DEPTH-1];
    logic        demand_q_valid [0:DEMAND_QUEUE_DEPTH-1];
    logic [3:0]  demand_q_count;

    // ----------------------------------------------------
    // 4. Prefetch Queue (FIFO)
    // ----------------------------------------------------
    logic [7:0]  prefetch_q_layer_id [0:PREFETCH_QUEUE_DEPTH-1];
    logic [7:0]  prefetch_q_expert_id [0:PREFETCH_QUEUE_DEPTH-1];
    logic [7:0]  prefetch_q_confidence [0:PREFETCH_QUEUE_DEPTH-1];
    logic        prefetch_q_valid [0:PREFETCH_QUEUE_DEPTH-1];
    logic [3:0]  prefetch_q_count;

    // ----------------------------------------------------
    // 5. Memory Bus and Arbiter State
    // ----------------------------------------------------
    logic        bus_busy;
    logic [7:0]  bus_timer;
    logic [1:0]  bus_owner; // 2'b00: Idle, 2'b01: KV Demand, 2'b10: Expert Demand, 2'b11: Prefetch
    
    // Active transaction details
    logic [7:0]  active_req_id;
    logic [7:0]  active_layer_id;
    logic [7:0]  active_expert_id;
    logic [7:0]  active_kv_block;
    logic        active_is_kv;

    // Helper loop variables declared at module scope for Yosys 0.9 compatibility
    integer l, w, b, i, q, other_w, other_b;
    logic       hit_found;
    logic       kv_hit_found;
    logic [2:0] victim_way;
    logic [2:0] max_cache_lru;
    logic [3:0] victim_kv;
    logic [3:0] max_kv_lru;
    logic       found_invalid;
    
    // Queue insert indicators
    logic       demand_queued;
    logic       prefetch_queued;

    // Arbiter internal variables declared at module scope
    logic [3:0]  sel_idx;
    logic        found_sel;
    logic [31:0] min_deadline;
    logic [3:0]  p_idx;
    logic        found_p;

    // Control and Stall logic
    assign ready = !stall;
    // Stall if demand queue is full
    assign stall = (demand_q_count >= DEMAND_QUEUE_DEPTH - 1);

    // ----------------------------------------------------
    // Main Sequential Clock Block
    // ----------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            // Reset cache arrays
            for (l = 0; l < NUM_LAYERS; l = l + 1) begin
                for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                    expert_cache_tags[l][w] <= 8'hFF;
                    expert_cache_valid[l][w] <= 1'b0;
                    expert_cache_lru[l][w] <= w[2:0];
                end
            end
            
            for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                kv_block_tags[b] <= 8'hFF;
                kv_block_valid[b] <= 1'b0;
                kv_block_lru[b] <= b[3:0];
            end
            
            // Reset queues
            for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                demand_q_req_id[q] <= 8'd0;
                demand_q_layer_id[q] <= 8'd0;
                demand_q_expert_id[q] <= 8'd0;
                demand_q_kv_block[q] <= 8'd0;
                demand_q_deadline[q] <= 32'd0;
                demand_q_is_kv[q] <= 1'b0;
                demand_q_criticality[q] <= 1'b0;
                demand_q_valid[q] <= 1'b0;
            end
            demand_q_count <= 4'd0;
            
            for (q = 0; q < PREFETCH_QUEUE_DEPTH; q = q + 1) begin
                prefetch_q_layer_id[q] <= 8'd0;
                prefetch_q_expert_id[q] <= 8'd0;
                prefetch_q_confidence[q] <= 8'd0;
                prefetch_q_valid[q] <= 1'b0;
            end
            prefetch_q_count <= 4'd0;
            
            // Reset Bus
            bus_busy <= 1'b0;
            bus_timer <= 8'd0;
            bus_owner <= 2'b00;
            active_req_id <= 8'd0;
            active_layer_id <= 8'd0;
            active_expert_id <= 8'd0;
            active_kv_block <= 8'd0;
            active_is_kv <= 1'b0;
            
            // Reset Perf Counters
            perf_cycles <= 32'd0;
            perf_expert_demand_served <= 32'd0;
            perf_expert_prefetch_served <= 32'd0;
            perf_expert_prefetch_dropped <= 32'd0;
            perf_kv_served <= 32'd0;
            perf_deadline_misses <= 32'd0;
            perf_memory_idle_cycles <= 32'd0;
            perf_queue_occupancy_sum <= 32'd0;
            perf_arbitration_decision_count <= 32'd0;
            
            expert_hit <= 1'b0;
            expert_miss <= 1'b0;
            kv_hit <= 1'b0;
            kv_miss <= 1'b0;
            
        end else begin
            perf_cycles <= perf_cycles + 1;
            perf_queue_occupancy_sum <= perf_queue_occupancy_sum + demand_q_count + prefetch_q_count;
            
            expert_hit <= 1'b0;
            expert_miss <= 1'b0;
            kv_hit <= 1'b0;
            kv_miss <= 1'b0;
            
            if (!bus_busy) begin
                perf_memory_idle_cycles <= perf_memory_idle_cycles + 1;
            end

            // ----------------------------------------------------
            // 1. Deadline Tracking (Decrement waiting queue items)
            // ----------------------------------------------------
            for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                if (demand_q_valid[q] && demand_q_deadline[q] > 0) begin
                    if (demand_q_deadline[q] == 1) begin
                        perf_deadline_misses <= perf_deadline_misses + 1;
                        demand_q_deadline[q] <= 32'd0;
                    end else begin
                        demand_q_deadline[q] <= demand_q_deadline[q] - 1;
                    end
                end
            end

            // ----------------------------------------------------
            // 2. Memory Bus Processing (Serve current transfer)
            // ----------------------------------------------------
            if (bus_busy) begin
                if (bus_timer > 1) begin
                    bus_timer <= bus_timer - 1;
                end else begin
                    // Transfer Complete: Commit weights to caches
                    bus_busy <= 1'b0;
                    bus_timer <= 8'd0;
                    
                    if (bus_owner == 2'b01) begin
                        // KV block load complete: insert into block table
                        victim_kv = 4'd0;
                        max_kv_lru = 4'd0;
                        found_invalid = 1'b0;
                        for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                            if (!found_invalid && !kv_block_valid[b]) begin
                                victim_kv = b[3:0];
                                found_invalid = 1'b1;
                            end else if (!found_invalid && (kv_block_lru[b] > max_kv_lru)) begin
                                max_kv_lru = kv_block_lru[b];
                                victim_kv = b[3:0];
                            end
                        end
                        kv_block_tags[victim_kv] <= active_kv_block;
                        kv_block_valid[victim_kv] <= 1'b1;
                        for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                            if (kv_block_valid[b] && b != victim_kv) begin
                                kv_block_lru[b] <= kv_block_lru[b] + 1;
                            end
                        end
                        kv_block_lru[victim_kv] <= 4'd0;
                        perf_kv_served <= perf_kv_served + 1;
                        
                    end else if (bus_owner == 2'b10 || bus_owner == 2'b11) begin
                        // Expert load complete: insert to cache
                        victim_way = 3'd0;
                        max_cache_lru = 3'd0;
                        found_invalid = 1'b0;
                        for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                            if (!found_invalid && !expert_cache_valid[active_layer_id][w]) begin
                                victim_way = w[2:0];
                                found_invalid = 1'b1;
                            end else if (!found_invalid && (expert_cache_lru[active_layer_id][w] > max_cache_lru)) begin
                                max_cache_lru = expert_cache_lru[active_layer_id][w];
                                victim_way = w[2:0];
                            end
                        end
                        expert_cache_tags[active_layer_id][victim_way] <= active_expert_id;
                        expert_cache_valid[active_layer_id][victim_way] <= 1'b1;
                        for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                            if (expert_cache_valid[active_layer_id][w] && w != victim_way) begin
                                expert_cache_lru[active_layer_id][w] <= expert_cache_lru[active_layer_id][w] + 1;
                            end
                        end
                        expert_cache_lru[active_layer_id][victim_way] <= 3'd0;
                        
                        if (bus_owner == 2'b10) begin
                            perf_expert_demand_served <= perf_expert_demand_served + 1;
                        end else begin
                            perf_expert_prefetch_served <= perf_expert_prefetch_served + 1;
                        end
                    end
                    bus_owner <= 2'b00;
                end
            end

            // ----------------------------------------------------
            // 3. Input Event Parsing & Queueing
            // ----------------------------------------------------
            if (event_valid && !stall) begin
                if (event_type == EVENT_EXPERT_DEMAND) begin
                    // Check expert cache hit
                    hit_found = 1'b0;
                    for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                        if (expert_cache_valid[event_layer_id][w] && (expert_cache_tags[event_layer_id][w] == event_expert_id)) begin
                            hit_found = 1'b1;
                            expert_cache_lru[event_layer_id][w] <= 3'd0;
                            for (other_w = 0; other_w < EXPERT_CAPACITY_PER_LAYER; other_w = other_w + 1) begin
                                if (other_w != w && expert_cache_valid[event_layer_id][other_w]) begin
                                    expert_cache_lru[event_layer_id][other_w] <= expert_cache_lru[event_layer_id][other_w] + 1;
                                end
                            end
                        end
                    end
                    
                    if (hit_found) begin
                        expert_hit <= 1'b1;
                    end else begin
                        expert_miss <= 1'b1;
                        // Queue the demand miss request
                        demand_queued = 1'b0;
                        for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                            if (!demand_queued && !demand_q_valid[q]) begin
                                demand_q_req_id[q] <= event_req_id;
                                demand_q_layer_id[q] <= event_layer_id;
                                demand_q_expert_id[q] <= event_expert_id;
                                demand_q_kv_block[q] <= 8'd0;
                                demand_q_deadline[q] <= event_deadline;
                                demand_q_is_kv[q] <= 1'b0;
                                demand_q_criticality[q] <= event_criticality;
                                demand_q_valid[q] <= 1'b1;
                                demand_queued = 1'b1;
                            end
                        end
                        demand_q_count <= demand_q_count + 1;
                    end
                    
                end else if (event_type == EVENT_KV_ACCESS) begin
                    // Check KV block hit
                    kv_hit_found = 1'b0;
                    for (b = 0; b < KV_BLOCK_CAPACITY; b = b + 1) begin
                        if (kv_block_valid[b] && (kv_block_tags[b] == event_kv_block_id)) begin
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
                    end else begin
                        kv_miss <= 1'b1;
                        // Queue the KV demand miss
                        demand_queued = 1'b0;
                        for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                            if (!demand_queued && !demand_q_valid[q]) begin
                                demand_q_req_id[q] <= event_req_id;
                                demand_q_layer_id[q] <= event_layer_id;
                                demand_q_expert_id[q] <= 8'd0;
                                demand_q_kv_block[q] <= event_kv_block_id;
                                demand_q_deadline[q] <= event_deadline;
                                demand_q_is_kv[q] <= 1'b1;
                                demand_q_criticality[q] <= event_criticality;
                                demand_q_valid[q] <= 1'b1;
                                demand_queued = 1'b1;
                            end
                        end
                        demand_q_count <= demand_q_count + 1;
                    end
                    
                end else if (event_type == EVENT_EXPERT_PREFETCH) begin
                    // Prefetch Confidence Filter and Drop Controller
                    // Drop prefetch if confidence < 15, or if demand queue is congested and confidence < 70
                    if (event_confidence < 8'd15 || (demand_q_count > 4'd2 && event_confidence < 8'd70)) begin
                        perf_expert_prefetch_dropped <= perf_expert_prefetch_dropped + 1;
                    end else begin
                        // Check if already cached
                        hit_found = 1'b0;
                        for (w = 0; w < EXPERT_CAPACITY_PER_LAYER; w = w + 1) begin
                            if (expert_cache_valid[event_layer_id][w] && (expert_cache_tags[event_layer_id][w] == event_expert_id)) begin
                                hit_found = 1'b1;
                            end
                        end
                        
                        if (!hit_found) begin
                            // Queue the prefetch request
                            prefetch_queued = 1'b0;
                            for (q = 0; q < PREFETCH_QUEUE_DEPTH; q = q + 1) begin
                                if (!prefetch_queued && !prefetch_q_valid[q]) begin
                                    prefetch_q_layer_id[q] <= event_layer_id;
                                    prefetch_q_expert_id[q] <= event_expert_id;
                                    prefetch_q_confidence[q] <= event_confidence;
                                    prefetch_q_valid[q] <= 1'b1;
                                    prefetch_queued = 1'b1;
                                end
                            end
                            if (prefetch_queued) begin
                                prefetch_q_count <= prefetch_q_count + 1;
                            end
                        end
                    end
                end
            end

            // ----------------------------------------------------
            // 4. Criticality & Deadline-Aware Bandwidth Arbitration
            // ----------------------------------------------------
            if (!bus_busy) begin
                sel_idx = 4'd0;
                found_sel = 1'b0;
                min_deadline = 32'hFFFFFFFF;
                
                // Priority Level 1: Highly Critical KV Demand Requests
                for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                    if (demand_q_valid[q] && demand_q_is_kv[q] && demand_q_criticality[q]) begin
                        if (!found_sel || (demand_q_deadline[q] < min_deadline)) begin
                            min_deadline = demand_q_deadline[q];
                            sel_idx = q[3:0];
                            found_sel = 1'b1;
                        end
                    end
                end
                
                // Priority Level 2: Critical Expert Demand Requests
                if (!found_sel) begin
                    for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                        if (demand_q_valid[q] && !demand_q_is_kv[q] && demand_q_criticality[q]) begin
                            if (!found_sel || (demand_q_deadline[q] < min_deadline)) begin
                                min_deadline = demand_q_deadline[q];
                                sel_idx = q[3:0];
                                found_sel = 1'b1;
                            end
                        end
                    end
                end
                
                // Priority Level 3: Non-critical KV Demand Requests
                if (!found_sel) begin
                    for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                        if (demand_q_valid[q] && demand_q_is_kv[q]) begin
                            if (!found_sel || (demand_q_deadline[q] < min_deadline)) begin
                                min_deadline = demand_q_deadline[q];
                                sel_idx = q[3:0];
                                found_sel = 1'b1;
                            end
                        end
                    end
                end
                
                // Priority Level 4: Non-critical Expert Demand Requests
                if (!found_sel) begin
                    for (q = 0; q < DEMAND_QUEUE_DEPTH; q = q + 1) begin
                        if (demand_q_valid[q] && !demand_q_is_kv[q]) begin
                            if (!found_sel || (demand_q_deadline[q] < min_deadline)) begin
                                min_deadline = demand_q_deadline[q];
                                sel_idx = q[3:0];
                                found_sel = 1'b1;
                            end
                        end
                    end
                end
                
                // Issue selected Demand Load
                if (found_sel) begin
                    bus_busy <= 1'b1;
                    bus_timer <= demand_q_is_kv[sel_idx] ? TRANSFER_CYCLES_KV : TRANSFER_CYCLES_EXPERT;
                    bus_owner <= demand_q_is_kv[sel_idx] ? 2'b01 : 2'b10;
                    
                    active_req_id <= demand_q_req_id[sel_idx];
                    active_layer_id <= demand_q_layer_id[sel_idx];
                    active_expert_id <= demand_q_expert_id[sel_idx];
                    active_kv_block <= demand_q_kv_block[sel_idx];
                    active_is_kv <= demand_q_is_kv[sel_idx];
                    
                    demand_q_valid[sel_idx] <= 1'b0;
                    demand_q_count <= demand_q_count - 1;
                    perf_arbitration_decision_count <= perf_arbitration_decision_count + 1;
                    
                end else if (prefetch_q_count > 0) begin
                    // Priority Level 5: Prefetch Requests (when no demand misses)
                    // Issue prefetch request
                    p_idx = 4'd0;
                    found_p = 1'b0;
                    for (q = 0; q < PREFETCH_QUEUE_DEPTH; q = q + 1) begin
                        if (!found_p && prefetch_q_valid[q]) begin
                            p_idx = q[3:0];
                            found_p = 1'b1;
                        end
                    end
                    
                    if (found_p) begin
                        bus_busy <= 1'b1;
                        bus_timer <= TRANSFER_CYCLES_EXPERT;
                        bus_owner <= 2'b11;
                        
                        active_req_id <= 8'd0;
                        active_layer_id <= prefetch_q_layer_id[p_idx];
                        active_expert_id <= prefetch_q_expert_id[p_idx];
                        active_kv_block <= 8'd0;
                        active_is_kv <= 1'b0;
                        
                        prefetch_q_valid[p_idx] <= 1'b0;
                        prefetch_q_count <= prefetch_q_count - 1;
                        perf_arbitration_decision_count <= perf_arbitration_decision_count + 1;
                    end
                end
            end
        end
    end

endmodule
