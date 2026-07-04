// SystemVerilog Testbench for MoE Accelerator Proposed Joint Scheduler Frontend
`timescale 1ns / 1ps

module tb_moe_accelerator_proposed;

    logic        clk;
    logic        rst_n;
    logic        event_valid;
    logic [1:0]  event_type;
    logic [7:0]  event_req_id;
    logic [7:0]  event_layer_id;
    logic [7:0]  event_expert_id;
    logic [7:0]  event_kv_block_id;
    logic [31:0] event_size_bytes;
    logic [31:0] event_deadline;
    logic [7:0]  event_confidence;
    logic        event_criticality;
    
    logic        ready;
    logic        stall;
    logic        expert_hit;
    logic        expert_miss;
    logic        kv_hit;
    logic        kv_miss;
    
    logic [31:0] perf_cycles;
    logic [31:0] perf_expert_demand_served;
    logic [31:0] perf_expert_prefetch_served;
    logic [31:0] perf_expert_prefetch_dropped;
    logic [31:0] perf_kv_served;
    logic [31:0] perf_deadline_misses;
    logic [31:0] perf_memory_idle_cycles;
    logic [31:0] perf_queue_occupancy_sum;
    logic [31:0] perf_arbitration_decision_count;

    // Instantiate UUT
    moe_accelerator_frontend_proposed uut (
        .clk,
        .rst_n,
        .event_valid,
        .event_type,
        .event_req_id,
        .event_layer_id,
        .event_expert_id,
        .event_kv_block_id,
        .event_size_bytes,
        .event_deadline,
        .event_confidence,
        .event_criticality,
        .ready,
        .stall,
        .expert_hit,
        .expert_miss,
        .kv_hit,
        .kv_miss,
        .perf_cycles,
        .perf_expert_demand_served,
        .perf_expert_prefetch_served,
        .perf_expert_prefetch_dropped,
        .perf_kv_served,
        .perf_deadline_misses,
        .perf_memory_idle_cycles,
        .perf_queue_occupancy_sum,
        .perf_arbitration_decision_count
    );

    // Clock Generator
    always #5 clk = ~clk;

    // Test variables
    int file_h;
    int log_file;
    int scan_r;
    int ev_type_val;
    int req_id_val;
    int layer_id_val;
    int exp_id_val;
    int kv_block_val;
    int size_val;
    int deadline_val;
    int conf_val;
    int crit_val;
    int count = 0;

    // Cycle-by-cycle logging block
    always @(posedge clk) begin
        if (rst_n && log_file != 0) begin
            $fdisplay(log_file, "%d %d %d %d %d %d %d %d %d %d %d %d", 
                      perf_cycles, event_valid, event_type, event_req_id, 
                      event_layer_id, event_expert_id, event_kv_block_id, 
                      expert_hit, expert_miss, kv_hit, kv_miss, stall);
        end
    end

    initial begin
        clk = 0;
        rst_n = 0;
        event_valid = 0;
        event_type = 2'b00;
        event_req_id = 8'd0;
        event_layer_id = 8'd0;
        event_expert_id = 8'd0;
        event_kv_block_id = 8'd0;
        event_size_bytes = 32'd0;
        event_deadline = 32'd0;
        event_confidence = 8'd0;
        event_criticality = 1'b0;
        
        log_file = $fopen("/home/a/discover/rtl/rtl_cycle_log.txt", "w");
        if (log_file == 0) begin
            $display("ERROR: Failed to open rtl_cycle_log.txt");
            $finish;
        end

        #20;
        rst_n = 1;
        #10;
        
        // Open the trace file
        file_h = $fopen("/home/a/discover/rtl/sim_trace_proposed.txt", "r");
        if (file_h == 0) begin
            $display("ERROR: Failed to open sim_trace_proposed.txt");
            $finish;
        end
        
        $display("--- Proposed RTL Simulation Started ---");
        
        // Read file line by line
        while (!$feof(file_h)) begin
            @(posedge clk);
            if (ready) begin
                // Format: event_type req_id layer_id expert_id kv_block_id size deadline confidence criticality
                scan_r = $fscanf(file_h, "%d %d %d %d %d %d %d %d %d\n", 
                                 ev_type_val, req_id_val, layer_id_val, exp_id_val, 
                                 kv_block_val, size_val, deadline_val, conf_val, crit_val);
                if (scan_r == 9) begin
                    event_valid <= 1'b1;
                    event_type <= ev_type_val[1:0];
                    event_req_id <= req_id_val[7:0];
                    event_layer_id <= layer_id_val[7:0];
                    event_expert_id <= exp_id_val[7:0];
                    event_kv_block_id <= kv_block_val[7:0];
                    event_size_bytes <= size_val;
                    event_deadline <= deadline_val;
                    event_confidence <= conf_val[7:0];
                    event_criticality <= crit_val[0];
                    count <= count + 1;
                end else begin
                    event_valid <= 1'b0;
                end
            end else begin
                event_valid <= 1'b0;
            end
        end
        
        // Final flush
        @(posedge clk);
        event_valid <= 1'b0;
        
        // Wait for transfers to complete
        repeat (100) @(posedge clk);
        
        $fclose(file_h);
        if (log_file != 0) begin
            $fclose(log_file);
        end
        
        // Print Summary Report
        $display("\n================ PROPOSED RTL SIMULATION SUMMARY ================");
        $display("Total Cycles:                 %d", perf_cycles);
        $display("Expert Demand Served:         %d", perf_expert_demand_served);
        $display("Expert Prefetch Served:       %d", perf_expert_prefetch_served);
        $display("Expert Prefetch Dropped:      %d", perf_expert_prefetch_dropped);
        $display("KV Blocks Served:             %d", perf_kv_served);
        $display("Deadline Misses:              %d", perf_deadline_misses);
        $display("Memory Idle Cycles:           %d", perf_memory_idle_cycles);
        $display("Arbitration Decisions:        %d", perf_arbitration_decision_count);
        $display("Avg Queue Occupancy:          %2.2f", real'(perf_queue_occupancy_sum) / real'(perf_cycles));
        $display("=================================================================\n");
        
        // Save to file
        file_h = $fopen("/home/a/discover/rtl/rtl_simulation_proposed.log", "w");
        if (file_h != 0) begin
            $fdisplay(file_h, "RTL Proposed Simulation Log");
            $fdisplay(file_h, "Total Cycles: %d", perf_cycles);
            $fdisplay(file_h, "Expert Demand Served: %d", perf_expert_demand_served);
            $fdisplay(file_h, "Expert Prefetch Served: %d", perf_expert_prefetch_served);
            $fdisplay(file_h, "Expert Prefetch Dropped: %d", perf_expert_prefetch_dropped);
            $fdisplay(file_h, "KV Blocks Served: %d", perf_kv_served);
            $fdisplay(file_h, "Deadline Misses: %d", perf_deadline_misses);
            $fdisplay(file_h, "Memory Idle Cycles: %d", perf_memory_idle_cycles);
            $fdisplay(file_h, "Arbitration Decisions: %d", perf_arbitration_decision_count);
            $fclose(file_h);
        end
        
        $finish;
    end

endmodule
