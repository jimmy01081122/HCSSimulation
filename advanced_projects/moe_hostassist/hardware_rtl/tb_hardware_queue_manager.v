// tb_hardware_queue_manager.v
// Testbench for M2 Hardware Queue Manager.

`timescale 1ns/1ps

module tb_hardware_queue_manager;

    reg clk;
    reg rst_n;
    reg enqueue_en;
    reg [3:0] in_stream_id;
    reg [5:0] in_expert_id;
    reg [15:0] in_deadline;
    wire enqueue_ready;
    reg dispatch_req;
    wire dispatch_valid;
    wire [3:0] dispatch_stream_id;
    wire [5:0] dispatch_expert_id;

    // Instantiate unit under test
    hardware_queue_manager uut (
        .clk(clk),
        .rst_n(rst_n),
        .enqueue_en(enqueue_en),
        .in_stream_id(in_stream_id),
        .in_expert_id(in_expert_id),
        .in_deadline(in_deadline),
        .enqueue_ready(enqueue_ready),
        .dispatch_req(dispatch_req),
        .dispatch_valid(dispatch_valid),
        .dispatch_stream_id(dispatch_stream_id),
        .dispatch_expert_id(dispatch_expert_id)
    );

    // Clock generation (100MHz)
    always #5 clk = ~clk;

    initial begin
        clk = 0;
        rst_n = 0;
        enqueue_en = 0;
        in_stream_id = 0;
        in_expert_id = 0;
        in_deadline = 0;
        dispatch_req = 0;

        #20;
        rst_n = 1;
        #10;

        // Verify ready is true
        if (enqueue_ready !== 1'b1) begin
            $display("ERROR: Queue should be ready initially");
            $finish;
        end

        // Enqueue Task 1: Stream 1, Expert 10, Deadline 500
        $display("[TB] Enqueuing Task 1 (Deadline 500)");
        enqueue_en = 1;
        in_stream_id = 4'd1;
        in_expert_id = 6'd10;
        in_deadline = 16'd500;
        #10;

        // Enqueue Task 2: Stream 2, Expert 20, Deadline 100
        $display("[TB] Enqueuing Task 2 (Deadline 100)");
        in_stream_id = 4'd2;
        in_expert_id = 6'd20;
        in_deadline = 16'd100;
        #10;

        // Enqueue Task 3: Stream 3, Expert 30, Deadline 250
        $display("[TB] Enqueuing Task 3 (Deadline 250)");
        in_stream_id = 4'd3;
        in_expert_id = 6'd30;
        in_deadline = 16'd250;
        #10;

        enqueue_en = 0;
        #20;

        // Dispatch 1: Should be Task 2 (Earliest Deadline = 100)
        $display("[TB] Requesting Dispatch 1...");
        dispatch_req = 1;
        #10;
        dispatch_req = 0;
        if (dispatch_valid && dispatch_stream_id == 4'd2 && dispatch_expert_id == 6'd20) begin
            $display("[SUCCESS] Dispatch 1: Stream %d, Expert %d (Earliest Deadline 100)", dispatch_stream_id, dispatch_expert_id);
        end else begin
            $display("[FAIL] Dispatch 1 incorrect! Valid: %d, Stream: %d, Expert: %d", dispatch_valid, dispatch_stream_id, dispatch_expert_id);
            $finish;
        end
        #20;

        // Dispatch 2: Should be Task 3 (Deadline = 250)
        $display("[TB] Requesting Dispatch 2...");
        dispatch_req = 1;
        #10;
        dispatch_req = 0;
        if (dispatch_valid && dispatch_stream_id == 4'd3 && dispatch_expert_id == 6'd30) begin
            $display("[SUCCESS] Dispatch 2: Stream %d, Expert %d (Deadline 250)", dispatch_stream_id, dispatch_expert_id);
        end else begin
            $display("[FAIL] Dispatch 2 incorrect! Valid: %d, Stream: %d, Expert: %d", dispatch_valid, dispatch_stream_id, dispatch_expert_id);
            $finish;
        end
        #20;

        // Dispatch 3: Should be Task 1 (Deadline = 500)
        $display("[TB] Requesting Dispatch 3...");
        dispatch_req = 1;
        #10;
        dispatch_req = 0;
        if (dispatch_valid && dispatch_stream_id == 4'd1 && dispatch_expert_id == 6'd10) begin
            $display("[SUCCESS] Dispatch 3: Stream %d, Expert %d (Deadline 500)", dispatch_stream_id, dispatch_expert_id);
        end else begin
            $display("[FAIL] Dispatch 3 incorrect! Valid: %d, Stream: %d, Expert: %d", dispatch_valid, dispatch_stream_id, dispatch_expert_id);
            $finish;
        end
        #20;

        $display("[TB] HQM scheduling verification complete. Latency = 1 clock cycle (10ns).");
        $finish;
    end

endmodule
