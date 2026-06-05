// fifo_replacement.v
// FIFO replacement policy pointer.

module fifo_replacement #(
    parameter NUM_WAYS = 4,
    parameter WAY_W    = 2
)(
    input  wire             clk,
    input  wire             rst_n,
    input  wire             request_victim,
    output wire [WAY_W-1:0] victim_way,
    output wire             victim_valid,
    input  wire             fill_done
);

    reg [WAY_W-1:0] ptr;

    // The victim way is always the one pointed to by ptr
    assign victim_way = ptr;
    assign victim_valid = 1'b1; // In a simple fully associative cache, victim is always valid to request

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            ptr <= {WAY_W{1'b0}};
        end else begin
            if (fill_done) begin
                if (ptr == NUM_WAYS - 1) begin
                    ptr <= {WAY_W{1'b0}};
                end else begin
                    ptr <= ptr + 1'b1;
                end
            end
        end
    end

endmodule
