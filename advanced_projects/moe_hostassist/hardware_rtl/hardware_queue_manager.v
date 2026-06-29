// hardware_queue_manager.v
// M2 Hardware Queue Manager (HQM)
// Schedules Mixture-of-Experts inference tasks based on Earliest Deadline First (EDF).

module hardware_queue_manager (
    input wire clk,
    input wire rst_n,
    
    // Enqueue interface
    input wire enqueue_en,
    input wire [3:0] in_stream_id,
    input wire [5:0] in_expert_id,
    input wire [15:0] in_deadline,
    output reg enqueue_ready,
    
    // Dispatch interface
    input wire dispatch_req,
    output reg dispatch_valid,
    output reg [3:0] dispatch_stream_id,
    output reg [5:0] dispatch_expert_id
);

    parameter MAX_TASKS = 8;

    // Task entry registers
    reg [3:0]  task_stream_id [0:MAX_TASKS-1];
    reg [5:0]  task_expert_id [0:MAX_TASKS-1];
    reg [15:0] task_deadline  [0:MAX_TASKS-1];
    reg        task_valid     [0:MAX_TASKS-1];

    integer i;
    reg [2:0] free_slot;
    reg       has_free_slot;

    // Find first free slot for enqueuing
    always @(*) begin
        free_slot = 3'd0;
        has_free_slot = 1'b0;
        for (i = 0; i < MAX_TASKS; i = i + 1) begin
            if (!task_valid[i] && !has_free_slot) begin
                free_slot = i[2:0];
                has_free_slot = 1'b1;
            end
        end
    end

    always @(*) begin
        enqueue_ready = has_free_slot;
    end

    // Scheduling logic: Earliest Deadline First (EDF)
    reg [2:0]  best_slot;
    reg        has_valid_task;
    reg [15:0] min_deadline;

    always @(*) begin
        best_slot = 3'd0;
        has_valid_task = 1'b0;
        min_deadline = 16'hFFFF;
        for (i = 0; i < MAX_TASKS; i = i + 1) begin
            if (task_valid[i]) begin
                if (!has_valid_task || (task_deadline[i] < min_deadline)) begin
                    min_deadline = task_deadline[i];
                    best_slot = i[2:0];
                    has_valid_task = 1'b1;
                end
            end
        end
    end

    // Sequential State Update
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            dispatch_valid <= 1'b0;
            dispatch_stream_id <= 4'd0;
            dispatch_expert_id <= 6'd0;
            for (i = 0; i < MAX_TASKS; i = i + 1) begin
                task_valid[i] <= 1'b0;
                task_stream_id[i] <= 4'd0;
                task_expert_id[i] <= 6'd0;
                task_deadline[i] <= 16'd0;
            end
        end else begin
            // Default outputs
            dispatch_valid <= 1'b0;

            // Handle dispatch request
            if (dispatch_req && has_valid_task) begin
                dispatch_valid <= 1'b1;
                dispatch_stream_id <= task_stream_id[best_slot];
                dispatch_expert_id <= task_expert_id[best_slot];
                task_valid[best_slot] <= 1'b0; // remove task from queue
            end

            // Handle enqueue request
            if (enqueue_en && enqueue_ready) begin
                // Avoid overwriting a slot that is being dispatched at the same cycle
                // if we try to enqueue to the same slot
                task_stream_id[free_slot] <= in_stream_id;
                task_expert_id[free_slot] <= in_expert_id;
                task_deadline[free_slot]  <= in_deadline;
                task_valid[free_slot]     <= 1'b1;
            end
        end
    end

endmodule
