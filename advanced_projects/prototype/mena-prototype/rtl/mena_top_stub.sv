module mena_top_stub #(
    parameter int NUM_EXPERTS = 8,
    parameter int NUM_TOKENS = 32,
    parameter int TOP_K = 2,
    parameter int SCORE_WIDTH = 16,
    parameter int IDX_WIDTH = $clog2(NUM_EXPERTS),
    parameter int TOK_WIDTH = $clog2(NUM_TOKENS)
) (
    input  logic                             clk,
    input  logic                             rst_n,
    
    // Input interface: Write token scores
    input  logic                             score_valid,
    input  logic [TOK_WIDTH-1:0]             score_token_id,
    input  logic [NUM_EXPERTS*SCORE_WIDTH-1:0] score_vector,
    
    // Dispatch control
    input  logic                             start_dispatch,
    
    // Outputs
    output logic [NUM_EXPERTS*6-1:0]         expert_count,
    output logic                             dispatch_valid,
    output logic [IDX_WIDTH-1:0]             dispatch_expert_id,
    output logic [TOK_WIDTH-1:0]             dispatch_token_id,
    output logic                             dispatch_done
);

    // Internal wires
    logic [TOP_K*IDX_WIDTH-1:0] selector_topk_idx;
    logic [TOP_K*SCORE_WIDTH-1:0] selector_topk_score;

    // Instantiate Top-k Selector
    topk_selector #(
        .NUM_EXPERTS(NUM_EXPERTS),
        .SCORE_WIDTH(SCORE_WIDTH),
        .TOP_K(TOP_K)
    ) u_topk_selector (
        .scores_in(score_vector),
        .topk_idx(selector_topk_idx),
        .topk_score(selector_topk_score)
    );

    // Instantiate Token Dispatcher
    // We write the select index of topk_selector into token_dispatcher.
    // The load is triggered when score_valid is high.
    token_dispatcher #(
        .NUM_EXPERTS(NUM_EXPERTS),
        .NUM_TOKENS(NUM_TOKENS),
        .TOP_K(TOP_K)
    ) u_token_dispatcher (
        .clk(clk),
        .rst_n(rst_n),
        .wr_en(score_valid),
        .wr_token_id(score_token_id),
        .wr_expert_ids(selector_topk_idx),
        .start_dispatch(start_dispatch),
        .expert_count(expert_count),
        .dispatch_valid(dispatch_valid),
        .dispatch_expert_id(dispatch_expert_id),
        .dispatch_token_id(dispatch_token_id),
        .dispatch_done(dispatch_done)
    );

endmodule
