// weight_buffer_ctrl.sv
// Double-buffering weight buffer controller.
// Manages the ping-pong selection between Weight Buffer A and B for DMA and NPU.

module weight_buffer_ctrl #(
    parameter int NUM_EXPERTS  = 8,
    parameter int BUF_ID_WIDTH = 1   // 1 bit: 0=BufA, 1=BufB
) (
    input  logic clk,
    input  logic rst_n,

    // Handshake from DMA engine
    input  logic dma_start,      // DMA just started loading next expert
    input  logic shadow_done,    // DMA finished loading into shadow buffer

    // Handshake from NPU
    input  logic exec_start,     // NPU started executing active buffer
    input  logic exec_done,      // NPU finished executing active buffer

    // Control outputs
    output logic active_sel,     // 0=BufA is active (NPU reads), 1=BufB is active
    output logic dma_sel,        // 0=DMA writes BufA, 1=DMA writes BufB
    output logic stall,          // 1 = NPU must wait, active not valid or swap pending
    output logic swap_event,     // 1-cycle pulse when buffers swap
    
    // Status
    output logic shadow_valid,   // shadow buffer has valid data
    output logic active_valid    // active buffer has valid data
);

    // Silence unused parameter/signal warnings
    logic _unused;
    assign _unused = &{1'b0, NUM_EXPERTS, BUF_ID_WIDTH, dma_start};

    // States
    typedef enum logic [1:0] {
        ST_IDLE,
        ST_LOADING,
        ST_RUNNING,
        ST_SWAP
    } state_t;

    state_t state_reg, state_next;

    // Registers to hold buffer mappings
    logic active_sel_reg, active_sel_next;
    logic shadow_valid_reg, shadow_valid_next;
    logic active_valid_reg, active_valid_next;

    // Output assignments
    assign active_sel = active_sel_reg;
    assign dma_sel    = ~active_sel_reg; // DMA writes to the shadow buffer (opposite of NPU)
    assign shadow_valid = shadow_valid_reg;
    assign active_valid = active_valid_reg;

    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state_reg        <= ST_IDLE;
            active_sel_reg   <= 1'b0; // Start with BufA as active
            shadow_valid_reg <= 1'b0;
            active_valid_reg <= 1'b0;
        end else begin
            state_reg        <= state_next;
            active_sel_reg   <= active_sel_next;
            shadow_valid_reg <= shadow_valid_next;
            active_valid_reg <= active_valid_next;
        end
    end

    always_comb begin
        state_next        = state_reg;
        active_sel_next   = active_sel_reg;
        shadow_valid_next = shadow_valid_reg;
        active_valid_next = active_valid_reg;
        stall             = 1'b0;
        swap_event        = 1'b0;

        // Process DMA shadow_done
        if (shadow_done) begin
            shadow_valid_next = 1'b1;
        end

        // Process NPU exec_done
        if (exec_done) begin
            active_valid_next = 1'b0;
        end

        case (state_reg)
            ST_IDLE: begin
                // Waiting for first DMA transfer to finish
                if (shadow_valid_next) begin
                    // Trigger swap to make it active
                    state_next = ST_SWAP;
                end else if (exec_start) begin
                    stall = 1'b1; // NPU wants to run but no weights loaded
                end
            end

            ST_LOADING: begin
                // DMA is loading, NPU is waiting or idle
                if (exec_start && !active_valid_next) begin
                    stall = 1'b1;
                end
                
                // If NPU is idle and shadow is valid, swap
                if (!active_valid_next && shadow_valid_next) begin
                    state_next = ST_SWAP;
                end
            end

            ST_RUNNING: begin
                // NPU is executing active buffer, DMA might be loading shadow
                if (exec_start && !active_valid_next) begin
                    stall = 1'b1;
                end

                // If NPU finishes, and shadow is ready, we swap
                if (!active_valid_next) begin
                    if (shadow_valid_next) begin
                        state_next = ST_SWAP;
                    end else begin
                        state_next = ST_LOADING; // Wait for DMA to finish
                    end
                end
            end

            ST_SWAP: begin
                // Swap active and shadow buffers
                active_sel_next   = ~active_sel_reg;
                active_valid_next = 1'b1;
                shadow_valid_next = 1'b0;
                swap_event        = 1'b1;
                
                state_next = ST_RUNNING;
            end

            default: state_next = ST_IDLE;
        endcase
    end

endmodule
