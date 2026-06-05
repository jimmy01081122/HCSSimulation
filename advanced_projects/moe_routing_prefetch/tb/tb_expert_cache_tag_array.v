// tb_expert_cache_tag_array.v
`timescale 1ns/1ps

module tb_expert_cache_tag_array;

    reg clk;
    reg rst_n;

    reg                   lookup_valid;
    reg [3:0]             lookup_expert_id;
    wire                  lookup_ready;
    wire                  lookup_result_valid;
    wire                  lookup_hit;
    wire                  lookup_miss;
    wire [1:0]            lookup_hit_way;
    wire                  lookup_hit_prefetched;

    reg                   fill_valid;
    wire                  fill_ready;
    reg [3:0]             fill_expert_id;
    reg [1:0]             fill_way;
    reg                   fill_is_prefetch;

    wire                  evict_valid;
    wire                  evict_prefetched;

    expert_cache_tag_array #(
        .EXPERT_ID_W(4),
        .NUM_WAYS(4),
        .WAY_W(2)
    ) uut (
        .clk(clk),
        .rst_n(rst_n),
        .lookup_valid(lookup_valid),
        .lookup_expert_id(lookup_expert_id),
        .lookup_ready(lookup_ready),
        .lookup_result_valid(lookup_result_valid),
        .lookup_hit(lookup_hit),
        .lookup_miss(lookup_miss),
        .lookup_hit_way(lookup_hit_way),
        .lookup_hit_prefetched(lookup_hit_prefetched),
        .fill_valid(fill_valid),
        .fill_ready(fill_ready),
        .fill_expert_id(fill_expert_id),
        .fill_way(fill_way),
        .fill_is_prefetch(fill_is_prefetch),
        .evict_valid(evict_valid),
        .evict_prefetched(evict_prefetched)
    );

    // Clock generator (10ns cycle)
    always #5 clk = ~clk;

    initial begin
        clk = 0;
        rst_n = 0;
        lookup_valid = 0;
        lookup_expert_id = 0;
        fill_valid = 0;
        fill_expert_id = 0;
        fill_way = 0;
        fill_is_prefetch = 0;

        #20;
        rst_n = 1;
        #10; // Alignment with clock edge

        // Test 1: Lookup after reset (should miss)
        lookup_valid = 1;
        lookup_expert_id = 4'd3;
        #1; // Sample before clock edge
        if (lookup_hit || !lookup_miss) begin
            $display("FAIL: Test 1 failed, hit on empty cache");
            $finish;
        end
        #9;
        lookup_valid = 0;
        #10;

        // Test 2: Fill expert A (not prefetch) then lookup
        fill_valid = 1;
        fill_expert_id = 4'd3;
        fill_way = 2'd0;
        fill_is_prefetch = 0;
        #10;
        fill_valid = 0;
        #10;

        lookup_valid = 1;
        lookup_expert_id = 4'd3;
        #1; // Sample before clock edge
        if (!lookup_hit || lookup_hit_prefetched || lookup_hit_way != 2'd0) begin
            $display("FAIL: Test 2 failed, lookup hit mismatch");
            $finish;
        end
        #9;
        lookup_valid = 0;
        #10;

        // Test 3: Fill expert B (prefetch) then lookup
        fill_valid = 1;
        fill_expert_id = 4'd5;
        fill_way = 2'd1;
        fill_is_prefetch = 1;
        #10;
        fill_valid = 0;
        #10;

        lookup_valid = 1;
        lookup_expert_id = 4'd5;
        #1; // Sample before clock edge (prefetched is still 1)
        if (!lookup_hit || !lookup_hit_prefetched || lookup_hit_way != 2'd1) begin
            $display("FAIL: Test 3 failed, lookup hit prefetched mismatch");
            $finish;
        end
        #9; // Rising edge occurs here, clearing prefetched bit to 0
        lookup_valid = 0;
        #10;

        // Test 4: Fill expert D (prefetch) in way 2, but do NOT lookup (useless prefetch)
        fill_valid = 1;
        fill_expert_id = 4'd9;
        fill_way = 2'd2;
        fill_is_prefetch = 1;
        #10;
        fill_valid = 0;
        #10;

        // Test 5: Overwrite way 2 with expert C (trigger useless prefetch eviction)
        fill_valid = 1;
        fill_expert_id = 4'd7;
        fill_way = 2'd2; // Overwriting way 2 which has valid prefetched=1 and not accessed
        fill_is_prefetch = 0;
        #1; // Wait small delay to check combinational outputs
        if (!evict_valid || !evict_prefetched) begin
            $display("FAIL: Test 5 failed, eviction not triggered");
            $finish;
        end
        #9;
        fill_valid = 0;
        #10;

        $display("PASS tb_expert_cache_tag_array");
        $finish;
    end

endmodule
