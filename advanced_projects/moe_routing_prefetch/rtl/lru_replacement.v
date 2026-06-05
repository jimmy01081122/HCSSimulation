// lru_replacement.v
// LRU replacement policy pointer using age counters.

module lru_replacement #(
    parameter NUM_WAYS = 4,
    parameter WAY_W    = 2
)(
    input  wire             clk,
    input  wire             rst_n,
    input  wire             access_valid,
    input  wire [WAY_W-1:0] access_way,
    output reg  [WAY_W-1:0] victim_way,
    output wire             victim_valid
);

    reg [WAY_W-1:0] age [0:NUM_WAYS-1];
    integer i;

    assign victim_valid = 1'b1;

    // Find the way with the maximum age (NUM_WAYS - 1)
    integer j;
    always @(*) begin
        victim_way = {WAY_W{1'b0}};
        for (j = 0; j < NUM_WAYS; j = j + 1) begin
            if (age[j] == NUM_WAYS - 1) begin
                victim_way = j[WAY_W-1:0];
            end
        end
    end

    // Sequential update of age registers on access
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            for (i = 0; i < NUM_WAYS; i = i + 1) begin
                age[i] <= i[WAY_W-1:0];
            end
        end else begin
            if (access_valid) begin
                for (i = 0; i < NUM_WAYS; i = i + 1) begin
                    if (i[WAY_W-1:0] == access_way) begin
                        // Accessed way becomes MRU (age = 0)
                        age[i] <= {WAY_W{1'b0}};
                    end else begin
                        // Other ways get older if they were younger than the accessed way
                        if (age[i] < age[access_way]) begin
                            age[i] <= age[i] + 1'b1;
                        end
                    end
                end
            end
        end
    end

endmodule
