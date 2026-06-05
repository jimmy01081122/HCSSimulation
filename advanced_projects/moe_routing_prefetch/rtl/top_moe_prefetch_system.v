// top_moe_prefetch_system.v
// Top-level integration module for the MoE routing-aware prefetch cache system.

module top_moe_prefetch_system #(
    parameter EXPERT_ID_W = 4,
    parameter NUM_WAYS    = 4,
    parameter WAY_W       = 2,
    parameter SCORE_W     = 8,
    parameter CNT_W       = 16,
    parameter DMA_LATENCY = 10
)(
    input  wire                   clk,
    input  wire                   rst_n,

    // Demand Request Interface
    input  wire                   req_valid,
    input  wire [EXPERT_ID_W-1:0] req_expert_id,
    output wire                   req_ready,

    // Prefetch Hint Interface
    input  wire                   prefetch_hint_valid,
    input  wire [EXPERT_ID_W-1:0] prefetch_hint_expert_id,
    input  wire [SCORE_W-1:0]     prefetch_hint_score,
    input  wire [SCORE_W-1:0]     score_threshold,
    input  wire                   prefetch_enable,
    input  wire                   replacement_policy_sel, // 0=FIFO, 1=LRU (LRU is TODO for Phase B)

    // Response Interface
    output reg                    resp_valid,
    output reg  [EXPERT_ID_W-1:0] resp_expert_id,

    // Counters
    output reg  [CNT_W-1:0]       cnt_total_req,
    output reg  [CNT_W-1:0]       cnt_hit,
    output reg  [CNT_W-1:0]       cnt_miss,
    output reg  [CNT_W-1:0]       cnt_prefetch_issued,
    output reg  [CNT_W-1:0]       cnt_prefetch_hit,
    output reg  [CNT_W-1:0]       cnt_prefetch_useless,
    output reg  [CNT_W-1:0]       cnt_prefetch_dropped,
    output reg  [CNT_W-1:0]       cnt_prefetch_filtered
);

    // 1. Internal FIFO Queue for Demand Requests
    reg [EXPERT_ID_W-1:0] fifo_mem [0:7];
    reg [2:0] fifo_head;
    reg [2:0] fifo_tail;
    reg [3:0] fifo_count;

    wire fifo_empty = (fifo_count == 4'd0);
    wire fifo_full  = (fifo_count == 4'd8);

    assign req_ready = !fifo_full;

    wire fifo_push = req_valid && req_ready;
    wire fifo_pop  = !fifo_empty && lookup_hit;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            fifo_head  <= 3'd0;
            fifo_tail  <= 3'd0;
            fifo_count <= 4'd0;
        end else begin
            if (fifo_push && fifo_pop) begin
                fifo_mem[fifo_tail] <= req_expert_id;
                fifo_tail           <= fifo_tail + 1'b1;
                fifo_head           <= fifo_head + 1'b1;
            end else if (fifo_push) begin
                fifo_mem[fifo_tail] <= req_expert_id;
                fifo_tail           <= fifo_tail + 1'b1;
                fifo_count          <= fifo_count + 1'b1;
            end else if (fifo_pop) begin
                fifo_head  <= fifo_head + 1'b1;
                fifo_count <= fifo_count - 1'b1;
            end
        end
    end

    wire [EXPERT_ID_W-1:0] out_expert_id = fifo_mem[fifo_head];
    wire out_valid = !fifo_empty;

    // 2. Cache Tag Array Instantiation
    wire                   lookup_ready;
    wire                   lookup_result_valid;
    wire                   lookup_hit;
    wire                   lookup_miss;
    wire [WAY_W-1:0]       lookup_hit_way;
    wire                   lookup_hit_prefetched;
    wire                   fill_ready;
    wire                   evict_valid;
    wire                   evict_prefetched;

    expert_cache_tag_array #(
        .EXPERT_ID_W(EXPERT_ID_W),
        .NUM_WAYS(NUM_WAYS),
        .WAY_W(WAY_W)
    ) tag_array_inst (
        .clk(clk),
        .rst_n(rst_n),
        .lookup_valid(out_valid),
        .lookup_expert_id(out_expert_id),
        .lookup_ready(lookup_ready),
        .lookup_result_valid(lookup_result_valid),
        .lookup_hit(lookup_hit),
        .lookup_miss(lookup_miss),
        .lookup_hit_way(lookup_hit_way),
        .lookup_hit_prefetched(lookup_hit_prefetched),
        .fill_valid(dma_done),
        .fill_ready(fill_ready),
        .fill_expert_id(dma_done_expert_id),
        .fill_way(victim_way),
        .fill_is_prefetch(dma_done_is_prefetch),
        .evict_valid(evict_valid),
        .evict_prefetched(evict_prefetched)
    );

    // 3. Replacement Policies Instantiation
    wire [WAY_W-1:0] fifo_victim_way;
    wire             fifo_victim_valid;
    wire [WAY_W-1:0] lru_victim_way;
    wire             lru_victim_valid;

    fifo_replacement #(
        .NUM_WAYS(NUM_WAYS),
        .WAY_W(WAY_W)
    ) fifo_repl_inst (
        .clk(clk),
        .rst_n(rst_n),
        .request_victim(dma_done),
        .victim_way(fifo_victim_way),
        .victim_valid(fifo_victim_valid),
        .fill_done(dma_done)
    );

    // For LRU, access occurs on a cache hit (fifo_pop) or cache fill (dma_done)
    wire lru_access_valid = (fifo_pop && lookup_hit) || dma_done;
    wire [WAY_W-1:0] lru_access_way = (fifo_pop && lookup_hit) ? lookup_hit_way : victim_way;

    lru_replacement #(
        .NUM_WAYS(NUM_WAYS),
        .WAY_W(WAY_W)
    ) lru_repl_inst (
        .clk(clk),
        .rst_n(rst_n),
        .access_valid(lru_access_valid),
        .access_way(lru_access_way),
        .victim_way(lru_victim_way),
        .victim_valid(lru_victim_valid)
    );

    // Mux victim selection based on replacement_policy_sel (0=FIFO, 1=LRU)
    wire [WAY_W-1:0] victim_way = replacement_policy_sel ? lru_victim_way : fifo_victim_way;
    wire             victim_valid = replacement_policy_sel ? lru_victim_valid : fifo_victim_valid;

    // 4. DMA Model Instantiation
    wire                   dma_req_valid;
    wire                   dma_req_ready;
    wire [EXPERT_ID_W-1:0] dma_req_expert_id;
    wire                   dma_req_is_prefetch;
    wire                   dma_done;
    wire [EXPERT_ID_W-1:0] dma_done_expert_id;
    wire                   dma_done_is_prefetch;
    wire                   dma_busy;

    simple_dma_model #(
        .EXPERT_ID_W(EXPERT_ID_W),
        .DMA_LATENCY(DMA_LATENCY)
    ) dma_inst (
        .clk(clk),
        .rst_n(rst_n),
        .dma_req_valid(dma_req_valid),
        .dma_req_ready(dma_req_ready),
        .dma_req_expert_id(dma_req_expert_id),
        .dma_req_is_prefetch(dma_req_is_prefetch),
        .dma_done(dma_done),
        .dma_done_expert_id(dma_done_expert_id),
        .dma_done_is_prefetch(dma_done_is_prefetch),
        .dma_busy(dma_busy)
    );

    // 5. In-flight Tracker for Single Channel DMA
    reg                   in_flight_valid;
    reg [EXPERT_ID_W-1:0] in_flight_expert_id;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            in_flight_valid     <= 1'b0;
            in_flight_expert_id <= {EXPERT_ID_W{1'b0}};
        end else begin
            if (dma_req_valid && dma_req_ready) begin
                in_flight_valid     <= 1'b1;
                in_flight_expert_id <= dma_req_expert_id;
            end else if (dma_done) begin
                in_flight_valid     <= 1'b0;
                in_flight_expert_id <= {EXPERT_ID_W{1'b0}};
            end
        end
    end

    // 6. Combinational Cache Lookup for Prefetch Hint Target
    reg hint_in_cache;
    integer k;
    always @(*) begin
        hint_in_cache = 1'b0;
        for (k = 0; k < NUM_WAYS; k = k + 1) begin
            if (tag_array_inst.valids[k] && (tag_array_inst.tags[k] == prefetch_hint_expert_id)) begin
                hint_in_cache = 1'b1;
            end
        end
    end

    // 7. DMA Request Arbitration Logic
    wire demand_wants_dma = out_valid && !lookup_hit && !(in_flight_valid && (in_flight_expert_id == out_expert_id)) && !dma_busy;
    
    wire score_ok = (prefetch_hint_score >= score_threshold);
    wire hint_in_flight = (in_flight_valid && (in_flight_expert_id == prefetch_hint_expert_id));
    wire prefetch_wants_to_issue = prefetch_hint_valid && prefetch_enable && score_ok && !hint_in_cache;
    wire prefetch_can_issue = prefetch_wants_to_issue && !dma_busy && !hint_in_flight && !demand_wants_dma;
    wire prefetch_dropped = prefetch_wants_to_issue && !prefetch_can_issue;

    assign dma_req_valid = demand_wants_dma || prefetch_can_issue;
    assign dma_req_expert_id = demand_wants_dma ? out_expert_id : prefetch_hint_expert_id;
    assign dma_req_is_prefetch = demand_wants_dma ? 1'b0 : 1'b1;

    // Response signals mapping
    always @(*) begin
        resp_valid = fifo_pop;
        resp_expert_id = out_expert_id;
    end

    // 8. Performance Counters Update Logic
    reg out_has_missed;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            out_has_missed        <= 1'b0;
            cnt_total_req         <= {CNT_W{1'b0}};
            cnt_hit               <= {CNT_W{1'b0}};
            cnt_miss              <= {CNT_W{1'b0}};
            cnt_prefetch_issued   <= {CNT_W{1'b0}};
            cnt_prefetch_hit      <= {CNT_W{1'b0}};
            cnt_prefetch_useless  <= {CNT_W{1'b0}};
            cnt_prefetch_dropped  <= {CNT_W{1'b0}};
            cnt_prefetch_filtered <= {CNT_W{1'b0}};
        end else begin
            // Clear or reset out_has_missed on pop
            if (fifo_pop) begin
                out_has_missed <= 1'b0;
            end
            
            // Hit / Miss counter updates
            if (dma_req_valid && dma_req_ready && !dma_req_is_prefetch) begin
                out_has_missed <= 1'b1;
                cnt_miss       <= cnt_miss + 1'b1;
                cnt_total_req  <= cnt_total_req + 1'b1;
            end
            
            if (fifo_pop) begin
                if (!out_has_missed) begin
                    cnt_hit       <= cnt_hit + 1'b1;
                    cnt_total_req <= cnt_total_req + 1'b1;
                    if (lookup_hit_prefetched) begin
                        cnt_prefetch_hit <= cnt_prefetch_hit + 1'b1;
                    end
                end
            end

            // Prefetch statistics
            if (dma_req_valid && dma_req_ready && dma_req_is_prefetch) begin
                cnt_prefetch_issued <= cnt_prefetch_issued + 1'b1;
            end

            if (prefetch_hint_valid && prefetch_enable && !score_ok) begin
                cnt_prefetch_filtered <= cnt_prefetch_filtered + 1'b1;
            end

            if (prefetch_dropped) begin
                cnt_prefetch_dropped <= cnt_prefetch_dropped + 1'b1;
            end

            if (dma_done && evict_valid && evict_prefetched) begin
                cnt_prefetch_useless <= cnt_prefetch_useless + 1'b1;
            end
        end
    end

endmodule
