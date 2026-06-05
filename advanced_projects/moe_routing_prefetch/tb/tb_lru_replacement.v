// tb_lru_replacement.v
`timescale 1ns/1ps

module tb_lru_replacement;

    reg clk;
    reg rst_n;
    reg access_valid;
    reg [1:0] access_way;
    wire [1:0] victim_way;
    wire victim_valid;

    lru_replacement #(
        .NUM_WAYS(4),
        .WAY_W(2)
    ) uut (
        .clk(clk),
        .rst_n(rst_n),
        .access_valid(access_valid),
        .access_way(access_way),
        .victim_way(victim_way),
        .victim_valid(victim_valid)
    );

    // Clock generator (10ns cycle)
    always #5 clk = ~clk;

    initial begin
        clk = 0;
        rst_n = 0;
        access_valid = 0;
        access_way = 0;

        #20;
        rst_n = 1;
        #10; // Align to clock edge

        // Test 1: Check initial state. Since age[3] is initialized to 3 (LRU), victim_way should be 3.
        #1; // Sample combinational output
        if (victim_way != 2'd3 || !victim_valid) begin
            $display("FAIL: Test 1 failed. Initial victim should be 3, got %d", victim_way);
            $finish;
        end
        #9;

        // Test 2: Access way 3 (becomes MRU), next LRU should be way 2 (since age[2] was 2, now becomes 3)
        access_valid = 1;
        access_way = 2'd3;
        #10;
        access_valid = 0;
        #1;
        if (victim_way != 2'd2) begin
            $display("FAIL: Test 2 failed. Victim should be 2, got %d", victim_way);
            $finish;
        end
        #9;

        // Test 3: Access ways in sequence: 0, then 1, then 2, then 3.
        // Let's access 0:
        access_valid = 1; access_way = 2'd0; #10;
        // Access 1:
        access_way = 2'd1; #10;
        // Access 2:
        access_way = 2'd2; #10;
        // Access 3:
        access_way = 2'd3; #10;
        access_valid = 0;
        
        // After sequence 0, 1, 2, 3:
        // way 3 is MRU (age=0), way 2 is age=1, way 1 is age=2, way 0 is LRU (age=3)
        #1;
        if (victim_way != 2'd0) begin
            $display("FAIL: Test 3 failed. After accessing 0,1,2,3, LRU victim should be 0, got %d", victim_way);
            $finish;
        end
        #9;

        // Test 4: Hit (access) way 0 (becomes MRU).
        // The next LRU should become way 1.
        access_valid = 1;
        access_way = 2'd0;
        #10;
        access_valid = 0;
        #1;
        if (victim_way != 2'd1) begin
            $display("FAIL: Test 4 failed. After hitting way 0, LRU victim should be 1, got %d", victim_way);
            $finish;
        end
        #9;

        $display("PASS tb_lru_replacement");
        $finish;
    end

endmodule
