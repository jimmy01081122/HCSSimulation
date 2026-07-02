// expert_queue_scheduler.sv
// Expert Queue Scheduler with 4 scheduling policies:
//   0 = Round Robin  (RR)
//   1 = Longest Queue First (LQF)
//   2 = Hot Expert First (HEF)
//   3 = Age-based scheduling

module expert_queue_scheduler #(
    parameter int NUM_EXPERTS  = 8,
    parameter int QUEUE_WIDTH  = 6,
    parameter int SCHED_POLICY = 0   // 0=RR, 1=LQF, 2=HEF, 3=Age
) (
    input  logic                                     clk,
    input  logic                                     rst_n,
    input  logic [NUM_EXPERTS*QUEUE_WIDTH-1:0]        expert_counts,
    input  logic                                     valid_in,
    output logic [$clog2(NUM_EXPERTS)-1:0]            next_expert,
    output logic                                     sched_valid
);

    // -----------------------------------------------------------------------
    // Local parameters
    // -----------------------------------------------------------------------
    localparam int EXP_W  = $clog2(NUM_EXPERTS);
    localparam int HOT_W  = 16;
    localparam int AGE_W  = 16;

    // -----------------------------------------------------------------------
    // Unpack expert_counts into an array for readability
    // -----------------------------------------------------------------------
    logic [QUEUE_WIDTH-1:0] cnt [NUM_EXPERTS];
    genvar gi;
    generate
        for (gi = 0; gi < NUM_EXPERTS; gi++) begin : g_unpack
            assign cnt[gi] = expert_counts[gi*QUEUE_WIDTH +: QUEUE_WIDTH];
        end
    endgenerate

    // -----------------------------------------------------------------------
    // Policy 0: Round-Robin pointer register
    // -----------------------------------------------------------------------
    logic [EXP_W-1:0] rr_ptr;

    // -----------------------------------------------------------------------
    // Policy 2: Hot Expert First – per-expert dispatch counter
    // -----------------------------------------------------------------------
    logic [HOT_W-1:0] hot_score [NUM_EXPERTS];

    // -----------------------------------------------------------------------
    // Policy 3: Age-based – per-expert age counter
    // -----------------------------------------------------------------------
    logic [AGE_W-1:0] age_counter [NUM_EXPERTS];

    // -----------------------------------------------------------------------
    // Combinational: any_nonempty flag
    // -----------------------------------------------------------------------
    logic any_nonempty;
    always_comb begin : p_any
        any_nonempty = 1'b0;
        for (int i = 0; i < NUM_EXPERTS; i++) begin
            if (cnt[i] != '0) any_nonempty = 1'b1;
        end
    end

    // -----------------------------------------------------------------------
    // Policy 0: Round Robin
    // Walk forward from rr_ptr until we find a non-empty expert.
    // -----------------------------------------------------------------------
    logic [EXP_W-1:0] sel_rr;
    logic              rr_found;

    always_comb begin : p_rr
        sel_rr  = '0;
        rr_found = 1'b0;
        for (int i = 0; i < NUM_EXPERTS; i++) begin
            if (!rr_found) begin
                if (cnt[(int'(rr_ptr) + i) % NUM_EXPERTS] != '0) begin
                    sel_rr   = EXP_W'((int'(rr_ptr) + i) % NUM_EXPERTS);
                    rr_found = 1'b1;
                end
            end
        end
    end

    // -----------------------------------------------------------------------
    // Policy 1: Longest Queue First
    // -----------------------------------------------------------------------
    logic [EXP_W-1:0]      sel_lqf;
    logic [QUEUE_WIDTH-1:0] lqf_best;

    always_comb begin : p_lqf
        sel_lqf  = '0;
        lqf_best = '0;
        for (int i = 0; i < NUM_EXPERTS; i++) begin
            if (cnt[i] > lqf_best) begin
                lqf_best = cnt[i];
                sel_lqf  = EXP_W'(i);
            end
        end
    end

    // -----------------------------------------------------------------------
    // Policy 2: Hot Expert First
    // -----------------------------------------------------------------------
    logic [EXP_W-1:0]  sel_hef;
    logic [HOT_W-1:0]  hef_best;
    logic               hef_found;

    always_comb begin : p_hef
        sel_hef   = '0;
        hef_best  = '0;
        hef_found = 1'b0;
        for (int i = 0; i < NUM_EXPERTS; i++) begin
            if (cnt[i] != '0) begin
                if (!hef_found || hot_score[i] > hef_best) begin
                    hef_best  = hot_score[i];
                    sel_hef   = EXP_W'(i);
                    hef_found = 1'b1;
                end
            end
        end
        // No non-empty expert with a hot score → fall back to RR result
        if (!hef_found) sel_hef = sel_rr;
    end

    // -----------------------------------------------------------------------
    // Policy 3: Age-based
    // -----------------------------------------------------------------------
    logic [EXP_W-1:0]  sel_age;
    logic [AGE_W-1:0]  age_best;
    logic               age_found;

    always_comb begin : p_age
        sel_age   = '0;
        age_best  = '0;
        age_found = 1'b0;
        for (int i = 0; i < NUM_EXPERTS; i++) begin
            if (cnt[i] != '0) begin
                if (!age_found || age_counter[i] > age_best) begin
                    age_best  = age_counter[i];
                    sel_age   = EXP_W'(i);
                    age_found = 1'b1;
                end
            end
        end
        if (!age_found) sel_age = sel_rr;
    end

    // -----------------------------------------------------------------------
    // Output mux – fully initialized to avoid latch warnings
    // -----------------------------------------------------------------------
    always_comb begin : p_out
        next_expert = '0;
        sched_valid = 1'b0;

        if (valid_in && any_nonempty) begin
            sched_valid = 1'b1;
            unique case (SCHED_POLICY)
                0:       next_expert = sel_rr;
                1:       next_expert = sel_lqf;
                2:       next_expert = sel_hef;
                3:       next_expert = sel_age;
                default: next_expert = sel_rr;
            endcase
        end
    end

    // -----------------------------------------------------------------------
    // Sequential updates
    // -----------------------------------------------------------------------
    always_ff @(posedge clk or negedge rst_n) begin : p_seq
        if (!rst_n) begin
            rr_ptr <= '0;
            for (int i = 0; i < NUM_EXPERTS; i++) begin
                hot_score  [i] <= '0;
                age_counter[i] <= '0;
            end
        end else begin
            if (valid_in && any_nonempty) begin
                // RR: advance pointer past the just-scheduled expert
                rr_ptr <= EXP_W'((int'(rr_ptr) + 1) % NUM_EXPERTS);

                // HEF: increment hot_score of dispatched expert
                hot_score[next_expert] <= hot_score[next_expert] + HOT_W'(1);

                // Age: reset dispatched expert; increment non-empty non-dispatched
                for (int i = 0; i < NUM_EXPERTS; i++) begin
                    if (EXP_W'(i) == next_expert) begin
                        age_counter[i] <= '0;
                    end else if (cnt[i] != '0) begin
                        age_counter[i] <= age_counter[i] + AGE_W'(1);
                    end
                end
            end else begin
                // No dispatch – accumulate age for all non-empty experts
                for (int i = 0; i < NUM_EXPERTS; i++) begin
                    if (cnt[i] != '0) begin
                        age_counter[i] <= age_counter[i] + AGE_W'(1);
                    end
                end
            end
        end
    end

endmodule
