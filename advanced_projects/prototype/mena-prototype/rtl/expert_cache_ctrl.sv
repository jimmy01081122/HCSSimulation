// expert_cache_ctrl.sv
// Parameterizable Expert Weight Cache Controller.
// Manages lookup, hit/miss detection, LRU eviction, and prefetch requests.

module expert_cache_ctrl #(
    parameter int CACHE_SIZE  = 4,
    parameter int NUM_EXPERTS = 8,
    parameter int ADDR_WIDTH  = 3  // $clog2(NUM_EXPERTS)
) (
    input  logic                   clk,
    input  logic                   rst_n,

    // CPU / Dispatcher Interface
    input  logic                   lookup_valid,  // Request to lookup an expert
    input  logic [ADDR_WIDTH-1:0]  lookup_expert, // Expert ID to lookup
    output logic                   lookup_hit,    // 1 if hit, 0 if miss
    output logic                   lookup_ready,  // Ready to accept next lookup

    // Prefetch Interface
    input  logic                   prefetch_valid,  // Request to prefetch an expert
    input  logic [ADDR_WIDTH-1:0]  prefetch_expert, // Expert ID to prefetch

    // DRAM Request Interface
    output logic                   mem_req_valid,   // Request to load expert from DRAM
    output logic [ADDR_WIDTH-1:0]  mem_req_expert,  // Expert ID to load
    output logic                   mem_req_is_pf,   // 1 if prefetch, 0 if demand load
    input  logic                   mem_req_ready,   // DRAM interface accepted request

    // DRAM Fill Input
    input  logic                   mem_fill_valid,  // DRAM completed loading expert
    input  logic [ADDR_WIDTH-1:0]  mem_fill_expert  // Expert ID loaded
);

    // Cache Tag and Valid Arrays
    logic [ADDR_WIDTH-1:0] tags [CACHE_SIZE];
    logic                  valids [CACHE_SIZE];

    // Silence unused parameter/signal warnings
    logic _unused;
    assign _unused = &{1'b0, NUM_EXPERTS};
    
    // LRU Tracker: age[i] holds the age of slot i.
    // 0 = Most Recently Used (MRU), CACHE_SIZE-1 = Least Recently Used (LRU)
    logic [$clog2(CACHE_SIZE)-1:0] age [CACHE_SIZE];

    // State Machine
    typedef enum logic [2:0] {
        ST_READY,
        ST_MISS_REQ,
        ST_MISS_WAIT,
        ST_PF_REQ,
        ST_PF_WAIT
    } state_t;

    state_t state_reg, state_next;

    // Registers to hold current transaction state
    logic [ADDR_WIDTH-1:0] req_expert_reg, req_expert_next;
    logic                  req_is_pf_reg, req_is_pf_next;
    logic [$clog2(CACHE_SIZE)-1:0] alloc_slot_reg, alloc_slot_next;

    // Hit / Miss logic
    logic hit_comb;
    logic [$clog2(CACHE_SIZE)-1:0] hit_slot;
    logic [$clog2(CACHE_SIZE)-1:0] lru_slot;

    always_comb begin
        hit_comb = 1'b0;
        hit_slot = '0;
        for (int i = 0; i < CACHE_SIZE; i++) begin
            if (valids[i] && (tags[i] == lookup_expert)) begin
                hit_comb = 1'b1;
                hit_slot = i[$clog2(CACHE_SIZE)-1:0];
            end
        end

        // Find the slot with max age (LRU slot)
        lru_slot = '0;
        for (int i = 0; i < CACHE_SIZE; i++) begin
            if (age[i] > age[lru_slot]) begin
                lru_slot = i[$clog2(CACHE_SIZE)-1:0];
            end
        end
    end

    // Sequential update of cache contents and age tracking
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state_reg      <= ST_READY;
            req_expert_reg <= '0;
            req_is_pf_reg  <= 1'b0;
            alloc_slot_reg <= '0;

            for (int i = 0; i < CACHE_SIZE; i++) begin
                tags[i]   <= '0;
                valids[i] <= 1'b0;
                age[i]    <= i[$clog2(CACHE_SIZE)-1:0];
            end
        end else begin
            state_reg      <= state_next;
            req_expert_reg <= req_expert_next;
            req_is_pf_reg  <= req_is_pf_next;
            alloc_slot_reg <= alloc_slot_next;

            // Handle Hit Age Update
            if (state_reg == ST_READY && lookup_valid && hit_comb) begin
                // Move hit slot to age 0, increment others that were younger
                for (int i = 0; i < CACHE_SIZE; i++) begin
                    if (i[$clog2(CACHE_SIZE)-1:0] == hit_slot) begin
                        age[i] <= '0;
                    end else if (age[i] < age[hit_slot]) begin
                        age[i] <= age[i] + 1'b1;
                    end
                end
            end

            // Handle memory fill completion
            if (mem_fill_valid) begin
                tags[alloc_slot_reg]   <= mem_fill_expert;
                valids[alloc_slot_reg] <= 1'b1;
                
                // Set filled slot to MRU (age 0), increment all others
                for (int i = 0; i < CACHE_SIZE; i++) begin
                    if (i[$clog2(CACHE_SIZE)-1:0] == alloc_slot_reg) begin
                        age[i] <= '0;
                    end else if (age[i] < age[alloc_slot_reg]) begin
                        age[i] <= age[i] + 1'b1;
                    end
                end
            end
        end
    end

    // Combinational Next State Logic
    logic pf_hit;
    always_comb begin
        state_next      = state_reg;
        req_expert_next = req_expert_reg;
        req_is_pf_next  = req_is_pf_reg;
        alloc_slot_next = alloc_slot_reg;
        pf_hit          = 1'b0;

        lookup_hit      = 1'b0;
        lookup_ready    = 1'b0;
        mem_req_valid   = 1'b0;
        mem_req_expert  = req_expert_reg;
        mem_req_is_pf   = req_is_pf_reg;

        case (state_reg)
            ST_READY: begin
                lookup_ready = 1'b1;
                if (lookup_valid) begin
                    if (hit_comb) begin
                        lookup_hit = 1'b1;
                    end else begin
                        // Cache miss: initiate demand load
                        lookup_hit      = 1'b0;
                        req_expert_next = lookup_expert;
                        req_is_pf_next  = 1'b0;
                        alloc_slot_next = lru_slot;
                        state_next      = ST_MISS_REQ;
                    end
                end else if (prefetch_valid) begin
                    // Check if prefetch target is already present
                    pf_hit = 1'b0;
                    for (int i = 0; i < CACHE_SIZE; i++) begin
                        if (valids[i] && (tags[i] == prefetch_expert)) begin
                            pf_hit = 1'b1;
                        end
                    end

                    if (!pf_hit) begin
                        // Initiate prefetch request into LRU slot
                        req_expert_next = prefetch_expert;
                        req_is_pf_next  = 1'b1;
                        alloc_slot_next = lru_slot;
                        state_next      = ST_PF_REQ;
                    end
                end
            end

            ST_MISS_REQ: begin
                mem_req_valid = 1'b1;
                if (mem_req_ready) begin
                    state_next = ST_MISS_WAIT;
                end
            end

            ST_MISS_WAIT: begin
                if (mem_fill_valid && (mem_fill_expert == req_expert_reg)) begin
                    state_next = ST_READY;
                end
            end

            ST_PF_REQ: begin
                mem_req_valid = 1'b1;
                if (mem_req_ready) begin
                    state_next = ST_PF_WAIT;
                end
            end

            ST_PF_WAIT: begin
                if (mem_fill_valid && (mem_fill_expert == req_expert_reg)) begin
                    state_next = ST_READY;
                end
                // Allow demand request to preempt wait?
                // For simplicity, wait until prefetch completes before accepting next demand lookup
            end

            default: state_next = ST_READY;
        endcase
    end

endmodule
