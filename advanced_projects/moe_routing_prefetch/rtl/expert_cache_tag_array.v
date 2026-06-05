// expert_cache_tag_array.v
// Metadata-only Fully Associative Cache Tag Array with Prefetched Flag Tracking.

module expert_cache_tag_array #(
    parameter EXPERT_ID_W = 4,
    parameter NUM_WAYS    = 4,
    parameter WAY_W       = 2
)(
    input  wire                   clk,
    input  wire                   rst_n,

    // Lookup Interface
    input  wire                   lookup_valid,
    input  wire [EXPERT_ID_W-1:0] lookup_expert_id,
    output wire                   lookup_ready,
    output wire                   lookup_result_valid,
    output wire                   lookup_hit,
    output wire                   lookup_miss,
    output wire [WAY_W-1:0]       lookup_hit_way,
    output wire                   lookup_hit_prefetched,

    // Fill Interface
    input  wire                   fill_valid,
    output wire                   fill_ready,
    input  wire [EXPERT_ID_W-1:0] fill_expert_id,
    input  wire [WAY_W-1:0]       fill_way,
    input  wire                   fill_is_prefetch,

    // Eviction Interface (Combinational outputs during fill cycle)
    output wire                   evict_valid,
    output wire                   evict_prefetched
);

    // Registers for Tag, Valid, and Prefetched flag per way
    reg [EXPERT_ID_W-1:0] tags [0:NUM_WAYS-1];
    reg                   valids [0:NUM_WAYS-1];
    reg                   prefetched [0:NUM_WAYS-1];

    // Lookup combinational logic
    assign lookup_ready = 1'b1; // Always ready for lookup in metadata model
    assign lookup_result_valid = lookup_valid;

    // Hit detection logic
    reg                    hit_detected;
    reg [WAY_W-1:0]        hit_way_idx;
    reg                    hit_prefetched_flag;

    integer i;
    always @(*) begin
        hit_detected = 1'b0;
        hit_way_idx  = {WAY_W{1'b0}};
        hit_prefetched_flag = 1'b0;
        for (i = 0; i < NUM_WAYS; i = i + 1) begin
            if (valids[i] && (tags[i] == lookup_expert_id)) begin
                hit_detected = 1'b1;
                hit_way_idx  = i[WAY_W-1:0];
                hit_prefetched_flag = prefetched[i];
            end
        end
    end

    assign lookup_hit = lookup_valid && hit_detected;
    assign lookup_miss = lookup_valid && !hit_detected;
    assign lookup_hit_way = hit_way_idx;
    assign lookup_hit_prefetched = lookup_hit && hit_prefetched_flag;

    // Fill logic
    assign fill_ready = 1'b1; // Always ready for fill in 1st version
    
    // Eviction combinational outputs
    assign evict_valid = fill_valid && valids[fill_way];
    assign evict_prefetched = fill_valid && valids[fill_way] && prefetched[fill_way];

    // Sequential tag updates and prefetch bit clearing
    integer j;
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (j = 0; j < NUM_WAYS; j = j + 1) begin
                tags[j]       <= {EXPERT_ID_W{1'b0}};
                valids[j]     <= 1'b0;
                prefetched[j] <= 1'b0;
            end
        end else begin
            // 1. Clear prefetched bit on demand access lookup hit
            if (lookup_valid && hit_detected && prefetched[hit_way_idx]) begin
                prefetched[hit_way_idx] <= 1'b0;
            end
            
            // 2. Perform Fill
            if (fill_valid && fill_ready) begin
                tags[fill_way]       <= fill_expert_id;
                valids[fill_way]     <= 1'b1;
                prefetched[fill_way] <= fill_is_prefetch;
            end
        end
    end

endmodule
