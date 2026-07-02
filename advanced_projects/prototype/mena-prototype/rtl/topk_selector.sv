module topk_selector #(
    parameter int NUM_EXPERTS = 8,
    parameter int SCORE_WIDTH = 16,
    parameter int TOP_K = 2,
    parameter int IDX_WIDTH = $clog2(NUM_EXPERTS)
) (
    input  logic [NUM_EXPERTS*SCORE_WIDTH-1:0] scores_in,
    output logic [TOP_K*IDX_WIDTH-1:0]        topk_idx,
    output logic [TOP_K*SCORE_WIDTH-1:0]      topk_score
);

    logic [SCORE_WIDTH-1:0] scores [NUM_EXPERTS];
    
    // Unpack inputs
    always_comb begin
        for (int i = 0; i < NUM_EXPERTS; i++) begin
            scores[i] = scores_in[i*SCORE_WIDTH +: SCORE_WIDTH];
        end
    end

    // Combinational search logic
    logic [SCORE_WIDTH-1:0] max1_val;
    logic [IDX_WIDTH-1:0]   max1_idx;
    logic [SCORE_WIDTH-1:0] max2_val;
    logic [IDX_WIDTH-1:0]   max2_idx;

    always_comb begin
        // Find absolute maximum (first expert)
        max1_val = scores[0];
        max1_idx = '0;
        for (int i = 1; i < NUM_EXPERTS; i++) begin
            if (scores[i] > max1_val) begin
                max1_val = scores[i];
                max1_idx = IDX_WIDTH'(i);
            end
        end

        // Find second maximum (exclude max1_idx)
        // Initialize max2 to the first available element that is not max1
        if (max1_idx == '0) begin
            max2_val = scores[1];
            max2_idx = IDX_WIDTH'(1);
        end else begin
            max2_val = scores[0];
            max2_idx = '0;
        end

        for (int i = 0; i < NUM_EXPERTS; i++) begin
            if (IDX_WIDTH'(i) != max1_idx) begin
                if (scores[i] > max2_val) begin
                    max2_val = scores[i];
                    max2_idx = IDX_WIDTH'(i);
                end
            end
        end

        // Pack outputs
        topk_idx[0*IDX_WIDTH +: IDX_WIDTH] = max1_idx;
        topk_idx[1*IDX_WIDTH +: IDX_WIDTH] = max2_idx;
        topk_score[0*SCORE_WIDTH +: SCORE_WIDTH] = max1_val;
        topk_score[1*SCORE_WIDTH +: SCORE_WIDTH] = max2_val;
    end

endmodule
