module token_dispatcher #(
    parameter int NUM_EXPERTS = 8,
    parameter int NUM_TOKENS = 32,
    parameter int TOP_K = 2,
    parameter int IDX_WIDTH = $clog2(NUM_EXPERTS),
    parameter int TOK_WIDTH = $clog2(NUM_TOKENS)
) (
    input  logic                      clk,
    input  logic                      rst_n,
    
    // Write/Load Interface
    input  logic                      wr_en,
    input  logic [TOK_WIDTH-1:0]      wr_token_id,
    input  logic [TOP_K*IDX_WIDTH-1:0] wr_expert_ids, // Packed top-k experts
    
    // Control
    input  logic                      start_dispatch,
    
    // Outputs
    output logic [NUM_EXPERTS*6-1:0]  expert_count, // 6 bits per expert
    output logic                      dispatch_valid,
    output logic [IDX_WIDTH-1:0]      dispatch_expert_id,
    output logic [TOK_WIDTH-1:0]      dispatch_token_id,
    output logic                      dispatch_done
);

    // Internal storage: routing table for each token
    logic [IDX_WIDTH-1:0] token_experts [NUM_TOKENS][TOP_K];
    logic                 token_loaded [NUM_TOKENS];

    // Decode wr_expert_ids
    logic [IDX_WIDTH-1:0] wr_expert_decoded [TOP_K];
    always_comb begin
        for (int k = 0; k < TOP_K; k++) begin
            wr_expert_decoded[k] = wr_expert_ids[k*IDX_WIDTH +: IDX_WIDTH];
        end
    end

    // Load logic
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (int t = 0; t < NUM_TOKENS; t++) begin
                token_loaded[t] <= 1'b0;
                for (int k = 0; k < TOP_K; k++) begin
                    token_experts[t][k] <= '0;
                end
            end
        end else if (wr_en) begin
            token_loaded[wr_token_id] <= 1'b1;
            for (int k = 0; k < TOP_K; k++) begin
                token_experts[wr_token_id][k] <= wr_expert_decoded[k];
            end
        end
    end

    // Compute expert_count combinational
    always_comb begin
        expert_count = '0;
        for (int e = 0; e < NUM_EXPERTS; e++) begin
            logic [5:0] count;
            count = '0;
            for (int t = 0; t < NUM_TOKENS; t++) begin
                logic match_count;
                match_count = 1'b0;
                if (token_loaded[t]) begin
                    for (int k = 0; k < TOP_K; k++) begin
                        if (token_experts[t][k] == IDX_WIDTH'(e)) begin
                            match_count = 1'b1;
                        end
                    end
                end
                if (match_count) begin
                    count = count + 1;
                end
            end
            expert_count[e*6 +: 6] = count;
        end
    end

    // Dispatcher State Machine
    typedef enum logic [1:0] {
        ST_IDLE,
        ST_DISPATCH,
        ST_DONE
    } state_t;

    state_t state, next_state;
    logic [IDX_WIDTH-1:0] curr_expert, next_expert;
    logic [TOK_WIDTH:0]   curr_token, next_token; // 1 bit extra to count up to NUM_TOKENS

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state       <= ST_IDLE;
            curr_expert <= '0;
            curr_token  <= '0;
        end else begin
            state       <= next_state;
            curr_expert <= next_expert;
            curr_token  <= next_token;
        end
    end

    logic dispatch_match;

    always_comb begin
        dispatch_match = 1'b0;
        
        next_state  = state;
        next_expert = curr_expert;
        next_token  = curr_token;
        
        dispatch_valid     = 1'b0;
        dispatch_expert_id = '0;
        dispatch_token_id  = '0;
        dispatch_done      = 1'b0;

        case (state)
            ST_IDLE: begin
                if (start_dispatch) begin
                    next_state  = ST_DISPATCH;
                    next_expert = '0;
                    next_token  = '0;
                end
            end

            ST_DISPATCH: begin
                if (curr_token < (TOK_WIDTH+1)'(NUM_TOKENS)) begin
                    // Check if current token matches current expert
                    if (token_loaded[curr_token[TOK_WIDTH-1:0]]) begin
                        for (int k = 0; k < TOP_K; k++) begin
                            if (token_experts[curr_token[TOK_WIDTH-1:0]][k] == curr_expert) begin
                                dispatch_match = 1'b1;
                            end
                        end
                    end

                    if (dispatch_match) begin
                        dispatch_valid     = 1'b1;
                        dispatch_expert_id = curr_expert;
                        dispatch_token_id  = curr_token[TOK_WIDTH-1:0];
                    end
                    
                    next_token = curr_token + 1;
                end else begin
                    // Finished scanning all tokens for curr_expert
                    if (curr_expert == IDX_WIDTH'(NUM_EXPERTS - 1)) begin
                        next_state = ST_DONE;
                    end else begin
                        next_expert = curr_expert + 1;
                        next_token  = '0;
                    end
                end
            end

            ST_DONE: begin
                dispatch_done = 1'b1;
                if (!start_dispatch) begin
                    next_state = ST_IDLE;
                end
            end
            
            default: next_state = ST_IDLE;
        endcase
    end

endmodule
