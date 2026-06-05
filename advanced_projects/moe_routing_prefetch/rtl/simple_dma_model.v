// simple_dma_model.v
// Fixed latency single-channel DMA model.

module simple_dma_model #(
    parameter EXPERT_ID_W = 4,
    parameter DMA_LATENCY = 10
)(
    input  wire                   clk,
    input  wire                   rst_n,

    // Request Interface
    input  wire                   dma_req_valid,
    output wire                   dma_req_ready,
    input  wire [EXPERT_ID_W-1:0] dma_req_expert_id,
    input  wire                   dma_req_is_prefetch,

    // Done Notification
    output reg                    dma_done,
    output reg  [EXPERT_ID_W-1:0] dma_done_expert_id,
    output reg                    dma_done_is_prefetch,
    output reg                    dma_busy
);

    reg [31:0] count;
    reg [EXPERT_ID_W-1:0] active_expert_id;
    reg                   active_is_prefetch;

    assign dma_req_ready = !dma_busy;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            dma_done             <= 1'b0;
            dma_done_expert_id   <= {EXPERT_ID_W{1'b0}};
            dma_done_is_prefetch <= 1'b0;
            dma_busy             <= 1'b0;
            count                <= 32'd0;
            active_expert_id     <= {EXPERT_ID_W{1'b0}};
            active_is_prefetch   <= 1'b0;
        end else begin
            dma_done <= 1'b0; // Default: 1-cycle pulse
            
            if (!dma_busy) begin
                if (dma_req_valid && dma_req_ready) begin
                    dma_busy             <= 1'b1;
                    active_expert_id     <= dma_req_expert_id;
                    active_is_prefetch   <= dma_req_is_prefetch;
                    if (DMA_LATENCY <= 1) begin
                        dma_done             <= 1'b1;
                        dma_done_expert_id   <= dma_req_expert_id;
                        dma_done_is_prefetch <= dma_req_is_prefetch;
                        dma_busy             <= 1'b0;
                    end else begin
                        count <= DMA_LATENCY - 1;
                    end
                end
            end else begin
                if (count == 32'd0) begin
                    dma_done             <= 1'b1;
                    dma_done_expert_id   <= active_expert_id;
                    dma_done_is_prefetch <= active_is_prefetch;
                    dma_busy             <= 1'b0;
                end else begin
                    count <= count - 1'b1;
                end
            end
        end
    end

endmodule
