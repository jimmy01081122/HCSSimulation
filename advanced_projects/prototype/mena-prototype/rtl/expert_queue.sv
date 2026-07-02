module expert_queue #(
    parameter int DATA_WIDTH = 5,   // E.g., token ID width
    parameter int DEPTH = 32,
    parameter int ADDR_WIDTH = $clog2(DEPTH)
) (
    input  logic                  clk,
    input  logic                  rst_n,
    
    // Write interface
    input  logic                  push,
    input  logic [DATA_WIDTH-1:0] data_in,
    
    // Read interface
    input  logic                  pop,
    output logic [DATA_WIDTH-1:0] data_out,
    
    // Status
    output logic                  empty,
    output logic                  full,
    output logic [ADDR_WIDTH:0]   count
);

    logic [DATA_WIDTH-1:0] ram [DEPTH];
    logic [ADDR_WIDTH-1:0] wr_ptr;
    logic [ADDR_WIDTH-1:0] rd_ptr;
    logic [ADDR_WIDTH:0]   cnt_reg;

    assign empty = (cnt_reg == 0);
    assign full  = (cnt_reg == DEPTH);
    assign count = cnt_reg;
    assign data_out = ram[rd_ptr];

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr  <= '0;
            rd_ptr  <= '0;
            cnt_reg <= '0;
        end else begin
            case ({push, pop})
                2'b10: begin // Push only
                    if (cnt_reg < DEPTH) begin
                        ram[wr_ptr] <= data_in;
                        wr_ptr      <= wr_ptr + 1'b1;
                        cnt_reg     <= cnt_reg + 1'b1;
                    end
                end
                2'b01: begin // Pop only
                    if (cnt_reg > 0) begin
                        rd_ptr  <= rd_ptr + 1'b1;
                        cnt_reg <= cnt_reg - 1'b1;
                    end
                end
                2'b11: begin // Push & Pop
                    if (cnt_reg > 0 && cnt_reg < DEPTH) begin
                        ram[wr_ptr] <= data_in;
                        wr_ptr      <= wr_ptr + 1'b1;
                        rd_ptr      <= rd_ptr + 1'b1;
                    end else if (cnt_reg == 0) begin // Push to empty
                        ram[wr_ptr] <= data_in;
                        wr_ptr      <= wr_ptr + 1'b1;
                        cnt_reg     <= cnt_reg + 1'b1;
                    end else if (cnt_reg == DEPTH) begin // Pop from full
                        rd_ptr      <= rd_ptr + 1'b1;
                        cnt_reg     <= cnt_reg - 1'b1;
                    end
                end
                default: ; // Do nothing
            endcase
        end
    end

endmodule
