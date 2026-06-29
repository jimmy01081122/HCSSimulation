// tb_cache_eviction_accelerator.v
// Testbench for M3 Expert Cache Eviction Accelerator.

`timescale 1ns/1ps

module tb_cache_eviction_accelerator;

    reg clk;
    reg rst_n;
    reg [3:0] w_recency;
    reg [3:0] w_frequency;
    reg [3:0] w_prediction;
    reg update_en;
    reg [1:0] update_slot_idx;
    reg [5:0] update_expert_id;
    reg [7:0] update_recency;
    reg [7:0] update_frequency;
    reg [7:0] update_prediction;
    reg update_valid;
    
    wire [1:0] evict_slot_idx;
    wire [5:0] evict_expert_id;
    wire evict_valid;

    // Instantiate unit under test
    cache_eviction_accelerator uut (
        .clk(clk),
        .rst_n(rst_n),
        .w_recency(w_recency),
        .w_frequency(w_frequency),
        .w_prediction(w_prediction),
        .update_en(update_en),
        .update_slot_idx(update_slot_idx),
        .update_expert_id(update_expert_id),
        .update_recency(update_recency),
        .update_frequency(update_frequency),
        .update_prediction(update_prediction),
        .update_valid(update_valid),
        .evict_slot_idx(evict_slot_idx),
        .evict_expert_id(evict_expert_id),
        .evict_valid(evict_valid)
    );

    // Clock generation
    always #5 clk = ~clk;

    initial begin
        clk = 0;
        rst_n = 0;
        w_recency = 4'd1;
        w_frequency = 4'd1;
        w_prediction = 4'd2; // high weight on prediction
        update_en = 0;
        update_slot_idx = 0;
        update_expert_id = 0;
        update_recency = 0;
        update_frequency = 0;
        update_prediction = 0;
        update_valid = 0;

        #20;
        rst_n = 1;
        #10;

        // Populating Slot 0: Expert 5, recency=100, frequency=10, prediction=5 -> Score=120
        $display("[TB] Updating Slot 0 (Expert 5)");
        update_en = 1;
        update_slot_idx = 2'd0;
        update_expert_id = 6'd5;
        update_recency = 8'd100;
        update_frequency = 8'd10;
        update_prediction = 8'd5;
        update_valid = 1;
        #10;

        // Populating Slot 1: Expert 15, recency=20, frequency=40, prediction=8 -> Score=76
        $display("[TB] Updating Slot 1 (Expert 15)");
        update_slot_idx = 2'd1;
        update_expert_id = 6'd15;
        update_recency = 8'd20;
        update_frequency = 8'd40;
        update_prediction = 8'd8;
        #10;

        // Populating Slot 2: Expert 25, recency=10, frequency=10, prediction=20 -> Score=60
        $display("[TB] Updating Slot 2 (Expert 25)");
        update_slot_idx = 2'd2;
        update_expert_id = 6'd25;
        update_recency = 8'd10;
        update_frequency = 8'd10;
        update_prediction = 8'd20;
        #10;

        update_en = 0;
        #10;

        // Verify output (combinational)
        // Eviction candidate should be Slot 2 (Score 60 < 76 < 120)
        if (evict_valid && evict_slot_idx == 2'd2 && evict_expert_id == 6'd25) begin
            $display("[SUCCESS] Eviction candidate: Slot %d (Expert %d) is selected (Lowest Score 60)", evict_slot_idx, evict_expert_id);
        end else begin
            $display("[FAIL] Eviction candidate incorrect! Valid: %d, Slot: %d, Expert: %d", evict_valid, evict_slot_idx, evict_expert_id);
            $finish;
        end

        // Change weights to see dynamic re-scoring
        // Score = w_rec*Rec + w_freq*Freq + w_pred*Pred
        // If w_recency=1, w_frequency=4, w_prediction=1:
        // Slot 0 (Expert 5): 100*1 + 10*4 + 5*1 = 145
        // Slot 1 (Expert 15): 20*1 + 40*4 + 8*1 = 188
        // Slot 2 (Expert 25): 10*1 + 10*4 + 20*1 = 70
        // Slot 2 is still lowest score (70). Let's make Slot 0 the lowest score:
        // If w_recency=0, w_frequency=1, w_prediction=1:
        // Slot 0: 0 + 10 + 5 = 15
        // Slot 1: 0 + 40 + 8 = 48
        // Slot 2: 0 + 10 + 20 = 30
        // Now Slot 0 should be the eviction candidate (Score 15 < 30 < 48)
        $display("[TB] Dynamically changing weights to w_recency=0, w_frequency=1, w_prediction=1");
        w_recency = 4'd0;
        w_frequency = 4'd1;
        w_prediction = 4'd1;
        #5; // small combinational settling delay

        if (evict_valid && evict_slot_idx == 2'd0 && evict_expert_id == 6'd5) begin
            $display("[SUCCESS] New Eviction candidate: Slot %d (Expert %d) is selected (Lowest Score 15)", evict_slot_idx, evict_expert_id);
        end else begin
            $display("[FAIL] Dynamic eviction candidate incorrect! Valid: %d, Slot: %d, Expert: %d", evict_valid, evict_slot_idx, evict_expert_id);
            $finish;
        end

        $display("[TB] Cache Eviction Accelerator verification complete. Latency = Combinational (Immediate).");
        $finish;
    end

endmodule
