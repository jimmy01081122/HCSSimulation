// cache_eviction_accelerator.v
// M3 Expert Cache Admission/Eviction Accelerator
// Computes admission scores in parallel to identify the optimal cache eviction candidate.

module cache_eviction_accelerator (
    input wire clk,
    input wire rst_n,
    
    // Weight configurations for Score = w_rec*Rec + w_freq*Freq + w_pred*Pred
    input wire [3:0] w_recency,
    input wire [3:0] w_frequency,
    input wire [3:0] w_prediction,
    
    // Cache slot update interface
    input wire update_en,
    input wire [1:0] update_slot_idx,
    input wire [5:0] update_expert_id,
    input wire [7:0] update_recency,
    input wire [7:0] update_frequency,
    input wire [7:0] update_prediction,
    input wire update_valid,
    
    // Eviction output (combinational score comparison output)
    output reg [1:0] evict_slot_idx,
    output reg [5:0] evict_expert_id,
    output reg evict_valid
);

    parameter NUM_SLOTS = 4;

    // Cache slot registers
    reg [5:0]  slot_expert_id [0:NUM_SLOTS-1];
    reg [7:0]  slot_recency   [0:NUM_SLOTS-1];
    reg [7:0]  slot_frequency [0:NUM_SLOTS-1];
    reg [7:0]  slot_prediction[0:NUM_SLOTS-1];
    reg        slot_valid     [0:NUM_SLOTS-1];

    // Score calculations
    reg [15:0] slot_score [0:NUM_SLOTS-1];
    integer i;

    always @(*) begin
        for (i = 0; i < NUM_SLOTS; i = i + 1) begin
            if (slot_valid[i]) begin
                slot_score[i] = (slot_recency[i] * w_recency) + 
                                (slot_frequency[i] * w_frequency) + 
                                (slot_prediction[i] * w_prediction);
            end else begin
                slot_score[i] = 16'hFFFF; // max score for invalid slots so they aren't evicted
            end
        end
    end

    // Parallel comparator tree for Min Score Reduction
    reg [15:0] min_score;
    reg [1:0]  min_idx;
    reg        has_valid_evict;

    always @(*) begin
        min_score = 16'hFFFF;
        min_idx = 2'd0;
        has_valid_evict = 1'b0;
        for (i = 0; i < NUM_SLOTS; i = i + 1) begin
            if (slot_valid[i]) begin
                if (!has_valid_evict || (slot_score[i] < min_score)) begin
                    min_score = slot_score[i];
                    min_idx = i[1:0];
                    has_valid_evict = 1'b1;
                end
            end
        end
    end

    always @(*) begin
        evict_slot_idx = min_idx;
        evict_expert_id = slot_expert_id[min_idx];
        evict_valid = has_valid_evict;
    end

    // Sequential slot register updates
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (i = 0; i < NUM_SLOTS; i = i + 1) begin
                slot_expert_id[i] <= 6'd0;
                slot_recency[i]   <= 8'd0;
                slot_frequency[i] <= 8'd0;
                slot_prediction[i]<= 8'd0;
                slot_valid[i]     <= 1'b0;
            end
        end else begin
            if (update_en) begin
                slot_expert_id[update_slot_idx] <= update_expert_id;
                slot_recency[update_slot_idx]   <= update_recency;
                slot_frequency[update_slot_idx] <= update_frequency;
                slot_prediction[update_slot_idx]<= update_prediction;
                slot_valid[update_slot_idx]     <= update_valid;
            end
        end
    end

endmodule
