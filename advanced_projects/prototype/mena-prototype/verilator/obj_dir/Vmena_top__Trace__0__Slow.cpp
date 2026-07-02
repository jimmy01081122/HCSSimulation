// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vmena_top__Syms.h"


VL_ATTR_COLD void Vmena_top___024root__trace_init_sub__TOP__0(Vmena_top___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+143,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+144,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+145,0,"reg_write_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+146,0,"reg_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+147,0,"reg_write_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+148,0,"reg_read_data",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+149,0,"mem_req_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+150,0,"mem_req_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+151,0,"mem_req_is_pf",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+152,0,"mem_req_ready",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+153,0,"mem_fill_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+154,0,"mem_fill_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+155,0,"token_wr_en",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+156,0,"token_wr_id",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+157,0,"token_wr_experts",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->pushPrefix("mena_top", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+163,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+164,0,"CACHE_SIZE",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+165,0,"QUEUE_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+166,0,"ADDR_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+143,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+144,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+145,0,"reg_write_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+146,0,"reg_addr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+147,0,"reg_write_data",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+148,0,"reg_read_data",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+149,0,"mem_req_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+150,0,"mem_req_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+151,0,"mem_req_is_pf",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+152,0,"mem_req_ready",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+153,0,"mem_fill_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+154,0,"mem_fill_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+155,0,"token_wr_en",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+156,0,"token_wr_id",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+157,0,"token_wr_experts",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+127,0,"reg_ctrl",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+128,0,"reg_status",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+129,0,"reg_cache_hit",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+130,0,"reg_cache_miss",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+131,0,"reg_dma_cycles",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+132,0,"reg_total_cycles",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+1,0,"start_pulse",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+2,0,"reset_pulse",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+133,0,"busy_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+134,0,"done_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declQuad(c+13,0,"expert_counts",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 47,0);
    tracep->declBit(c+15,0,"dispatch_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+16,0,"dispatch_expert_id",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+17,0,"dispatch_token_id",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+158,0,"dispatch_done",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+135,0,"next_sched_expert",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+136,0,"sched_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+137,0,"cache_lookup_hit",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+159,0,"cache_lookup_ready",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+18,0,"wbuf_active_sel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+19,0,"wbuf_dma_sel",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"wbuf_stall",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+20,0,"wbuf_swap_event",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+21,0,"wbuf_shadow_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+22,0,"wbuf_active_valid",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("u_cache", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+164,0,"CACHE_SIZE",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+163,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+166,0,"ADDR_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+143,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+3,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+136,0,"lookup_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+135,0,"lookup_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+137,0,"lookup_hit",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+159,0,"lookup_ready",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+167,0,"prefetch_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+168,0,"prefetch_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+149,0,"mem_req_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+150,0,"mem_req_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+151,0,"mem_req_is_pf",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+152,0,"mem_req_ready",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+153,0,"mem_fill_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+154,0,"mem_fill_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->pushPrefix("tags", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 4; ++i) {
        tracep->declBus(c+23+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 2,0);
    }
    tracep->popPrefix();
    tracep->pushPrefix("valids", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 4; ++i) {
        tracep->declBit(c+27+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0));
    }
    tracep->popPrefix();
    tracep->pushPrefix("age", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 4; ++i) {
        tracep->declBus(c+31+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 1,0);
    }
    tracep->popPrefix();
    tracep->declBus(c+35,0,"state_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+160,0,"state_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+36,0,"req_expert_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+138,0,"req_expert_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+37,0,"req_is_pf_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+139,0,"req_is_pf_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+38,0,"alloc_slot_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+140,0,"alloc_slot_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+141,0,"hit_comb",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+142,0,"hit_slot",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+39,0,"lru_slot",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+167,0,"pf_hit",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+169,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk2", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+169,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk3", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+40,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk4", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+41,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk5", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+42,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk6", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+170,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_dispatcher", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+163,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+171,0,"NUM_TOKENS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+172,0,"TOP_K",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+166,0,"IDX_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+173,0,"TOK_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+143,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+3,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+155,0,"wr_en",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+161,0,"wr_token_id",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+157,0,"wr_expert_ids",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBit(c+1,0,"start_dispatch",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declQuad(c+13,0,"expert_count",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 47,0);
    tracep->declBit(c+15,0,"dispatch_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+16,0,"dispatch_expert_id",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+17,0,"dispatch_token_id",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+158,0,"dispatch_done",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("token_loaded", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 32; ++i) {
        tracep->declBit(c+43+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0));
    }
    tracep->popPrefix();
    tracep->pushPrefix("wr_expert_decoded", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 2; ++i) {
        tracep->declBus(c+4+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 2,0);
    }
    tracep->popPrefix();
    tracep->declBus(c+75,0,"state",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+7,0,"next_state",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+76,0,"curr_expert",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+8,0,"next_expert",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+77,0,"curr_token",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+9,0,"next_token",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBit(c+78,0,"dispatch_match",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+174,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk10", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+79,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk2", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+80,0,"t",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("unnamedblk3", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+81,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk4", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+82,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk5", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+175,0,"e",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("unnamedblk6", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+83,0,"count",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->pushPrefix("unnamedblk7", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+176,0,"t",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("unnamedblk8", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+84,0,"match_count",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("unnamedblk9", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+85,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_scheduler", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+163,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+165,0,"QUEUE_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+177,0,"SCHED_POLICY",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+143,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+3,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declQuad(c+13,0,"expert_counts",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 47,0);
    tracep->declBit(c+133,0,"valid_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+135,0,"next_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+136,0,"sched_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+166,0,"EXP_W",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+178,0,"HOT_W",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+178,0,"AGE_W",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("cnt", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 8; ++i) {
        tracep->declBus(c+86+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 5,0);
    }
    tracep->popPrefix();
    tracep->declBus(c+94,0,"rr_ptr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->pushPrefix("hot_score", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 8; ++i) {
        tracep->declBus(c+95+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 15,0);
    }
    tracep->popPrefix();
    tracep->pushPrefix("age_counter", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 8; ++i) {
        tracep->declBus(c+103+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 15,0);
    }
    tracep->popPrefix();
    tracep->declBit(c+111,0,"any_nonempty",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+112,0,"sel_rr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+113,0,"rr_found",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+114,0,"sel_lqf",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+115,0,"lqf_best",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+116,0,"sel_hef",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+117,0,"hef_best",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBit(c+118,0,"hef_found",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+119,0,"sel_age",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+120,0,"age_best",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBit(c+121,0,"age_found",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("p_age", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk5", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+175,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_any", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+175,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_hef", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk4", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+175,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_lqf", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk3", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+175,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_rr", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk2", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+175,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_seq", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk6", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+122,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk7", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+123,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk8", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+124,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_wbuf_ctrl", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+163,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+179,0,"BUF_ID_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+143,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+3,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+162,0,"dma_start",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+153,0,"shadow_done",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+15,0,"exec_start",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+158,0,"exec_done",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+18,0,"active_sel",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+19,0,"dma_sel",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+6,0,"stall",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+20,0,"swap_event",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+21,0,"shadow_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+22,0,"active_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+125,0,"state_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+10,0,"state_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+18,0,"active_sel_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+126,0,"active_sel_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+21,0,"shadow_valid_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+11,0,"shadow_valid_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+22,0,"active_valid_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+12,0,"active_valid_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vmena_top___024root__trace_init_top(Vmena_top___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_init_top\n"); );
    // Body
    Vmena_top___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vmena_top___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void Vmena_top___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vmena_top___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vmena_top___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vmena_top___024root__trace_register(Vmena_top___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_register\n"); );
    // Body
    tracep->addConstCb(&Vmena_top___024root__trace_const_0, 0U, vlSelf);
    tracep->addFullCb(&Vmena_top___024root__trace_full_0, 0U, vlSelf);
    tracep->addChgCb(&Vmena_top___024root__trace_chg_0, 0U, vlSelf);
    tracep->addCleanupCb(&Vmena_top___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vmena_top___024root__trace_const_0_sub_0(Vmena_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vmena_top___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_const_0\n"); );
    // Init
    Vmena_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vmena_top___024root*>(voidSelf);
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vmena_top___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vmena_top___024root__trace_const_0_sub_0(Vmena_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_const_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+163,(8U),32);
    bufp->fullIData(oldp+164,(4U),32);
    bufp->fullIData(oldp+165,(6U),32);
    bufp->fullIData(oldp+166,(3U),32);
    bufp->fullBit(oldp+167,(0U));
    bufp->fullCData(oldp+168,(0U),3);
    bufp->fullIData(oldp+169,(4U),32);
    bufp->fullIData(oldp+170,(vlSelf->mena_top__DOT__u_cache__DOT__unnamedblk6__DOT__i),32);
    bufp->fullIData(oldp+171,(0x20U),32);
    bufp->fullIData(oldp+172,(2U),32);
    bufp->fullIData(oldp+173,(5U),32);
    bufp->fullIData(oldp+174,(2U),32);
    bufp->fullIData(oldp+175,(8U),32);
    bufp->fullIData(oldp+176,(0x20U),32);
    bufp->fullIData(oldp+177,(0U),32);
    bufp->fullIData(oldp+178,(0x10U),32);
    bufp->fullIData(oldp+179,(1U),32);
}

VL_ATTR_COLD void Vmena_top___024root__trace_full_0_sub_0(Vmena_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vmena_top___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_full_0\n"); );
    // Init
    Vmena_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vmena_top___024root*>(voidSelf);
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vmena_top___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vmena_top___024root__trace_full_0_sub_0(Vmena_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_full_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->mena_top__DOT__start_pulse));
    bufp->fullBit(oldp+2,(vlSelf->mena_top__DOT__reset_pulse));
    bufp->fullBit(oldp+3,(vlSelf->mena_top__DOT____Vcellinp__u_dispatcher__rst_n));
    bufp->fullCData(oldp+4,(vlSelf->mena_top__DOT__u_dispatcher__DOT__wr_expert_decoded[0]),3);
    bufp->fullCData(oldp+5,(vlSelf->mena_top__DOT__u_dispatcher__DOT__wr_expert_decoded[1]),3);
    bufp->fullBit(oldp+6,(vlSelf->mena_top__DOT__wbuf_stall));
    bufp->fullCData(oldp+7,(vlSelf->mena_top__DOT__u_dispatcher__DOT__next_state),2);
    bufp->fullCData(oldp+8,(vlSelf->mena_top__DOT__u_dispatcher__DOT__next_expert),3);
    bufp->fullCData(oldp+9,(vlSelf->mena_top__DOT__u_dispatcher__DOT__next_token),6);
    bufp->fullCData(oldp+10,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__state_next),2);
    bufp->fullBit(oldp+11,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__shadow_valid_next));
    bufp->fullBit(oldp+12,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_valid_next));
    bufp->fullQData(oldp+13,(vlSelf->mena_top__DOT__expert_counts),48);
    bufp->fullBit(oldp+15,(vlSelf->mena_top__DOT__dispatch_valid));
    bufp->fullCData(oldp+16,(vlSelf->mena_top__DOT__dispatch_expert_id),3);
    bufp->fullCData(oldp+17,(vlSelf->mena_top__DOT__dispatch_token_id),5);
    bufp->fullBit(oldp+18,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_reg));
    bufp->fullBit(oldp+19,((1U & (~ (IData)(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_reg)))));
    bufp->fullBit(oldp+20,(vlSelf->mena_top__DOT__wbuf_swap_event));
    bufp->fullBit(oldp+21,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__shadow_valid_reg));
    bufp->fullBit(oldp+22,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_valid_reg));
    bufp->fullCData(oldp+23,(vlSelf->mena_top__DOT__u_cache__DOT__tags[0]),3);
    bufp->fullCData(oldp+24,(vlSelf->mena_top__DOT__u_cache__DOT__tags[1]),3);
    bufp->fullCData(oldp+25,(vlSelf->mena_top__DOT__u_cache__DOT__tags[2]),3);
    bufp->fullCData(oldp+26,(vlSelf->mena_top__DOT__u_cache__DOT__tags[3]),3);
    bufp->fullBit(oldp+27,(vlSelf->mena_top__DOT__u_cache__DOT__valids[0]));
    bufp->fullBit(oldp+28,(vlSelf->mena_top__DOT__u_cache__DOT__valids[1]));
    bufp->fullBit(oldp+29,(vlSelf->mena_top__DOT__u_cache__DOT__valids[2]));
    bufp->fullBit(oldp+30,(vlSelf->mena_top__DOT__u_cache__DOT__valids[3]));
    bufp->fullCData(oldp+31,(vlSelf->mena_top__DOT__u_cache__DOT__age[0]),2);
    bufp->fullCData(oldp+32,(vlSelf->mena_top__DOT__u_cache__DOT__age[1]),2);
    bufp->fullCData(oldp+33,(vlSelf->mena_top__DOT__u_cache__DOT__age[2]),2);
    bufp->fullCData(oldp+34,(vlSelf->mena_top__DOT__u_cache__DOT__age[3]),2);
    bufp->fullCData(oldp+35,(vlSelf->mena_top__DOT__u_cache__DOT__state_reg),3);
    bufp->fullCData(oldp+36,(vlSelf->mena_top__DOT__u_cache__DOT__req_expert_reg),3);
    bufp->fullBit(oldp+37,(vlSelf->mena_top__DOT__u_cache__DOT__req_is_pf_reg));
    bufp->fullCData(oldp+38,(vlSelf->mena_top__DOT__u_cache__DOT__alloc_slot_reg),2);
    bufp->fullCData(oldp+39,(vlSelf->mena_top__DOT__u_cache__DOT__lru_slot),2);
    bufp->fullIData(oldp+40,(vlSelf->mena_top__DOT__u_cache__DOT__unnamedblk3__DOT__i),32);
    bufp->fullIData(oldp+41,(vlSelf->mena_top__DOT__u_cache__DOT__unnamedblk4__DOT__i),32);
    bufp->fullIData(oldp+42,(vlSelf->mena_top__DOT__u_cache__DOT__unnamedblk5__DOT__i),32);
    bufp->fullBit(oldp+43,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[0]));
    bufp->fullBit(oldp+44,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[1]));
    bufp->fullBit(oldp+45,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[2]));
    bufp->fullBit(oldp+46,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[3]));
    bufp->fullBit(oldp+47,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[4]));
    bufp->fullBit(oldp+48,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[5]));
    bufp->fullBit(oldp+49,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[6]));
    bufp->fullBit(oldp+50,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[7]));
    bufp->fullBit(oldp+51,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[8]));
    bufp->fullBit(oldp+52,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[9]));
    bufp->fullBit(oldp+53,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[10]));
    bufp->fullBit(oldp+54,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[11]));
    bufp->fullBit(oldp+55,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[12]));
    bufp->fullBit(oldp+56,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[13]));
    bufp->fullBit(oldp+57,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[14]));
    bufp->fullBit(oldp+58,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[15]));
    bufp->fullBit(oldp+59,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[16]));
    bufp->fullBit(oldp+60,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[17]));
    bufp->fullBit(oldp+61,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[18]));
    bufp->fullBit(oldp+62,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[19]));
    bufp->fullBit(oldp+63,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[20]));
    bufp->fullBit(oldp+64,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[21]));
    bufp->fullBit(oldp+65,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[22]));
    bufp->fullBit(oldp+66,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[23]));
    bufp->fullBit(oldp+67,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[24]));
    bufp->fullBit(oldp+68,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[25]));
    bufp->fullBit(oldp+69,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[26]));
    bufp->fullBit(oldp+70,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[27]));
    bufp->fullBit(oldp+71,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[28]));
    bufp->fullBit(oldp+72,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[29]));
    bufp->fullBit(oldp+73,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[30]));
    bufp->fullBit(oldp+74,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[31]));
    bufp->fullCData(oldp+75,(vlSelf->mena_top__DOT__u_dispatcher__DOT__state),2);
    bufp->fullCData(oldp+76,(vlSelf->mena_top__DOT__u_dispatcher__DOT__curr_expert),3);
    bufp->fullCData(oldp+77,(vlSelf->mena_top__DOT__u_dispatcher__DOT__curr_token),6);
    bufp->fullBit(oldp+78,(vlSelf->mena_top__DOT__u_dispatcher__DOT__dispatch_match));
    bufp->fullIData(oldp+79,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk10__DOT__k),32);
    bufp->fullIData(oldp+80,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk2__DOT__t),32);
    bufp->fullIData(oldp+81,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk2__DOT__unnamedblk3__DOT__k),32);
    bufp->fullIData(oldp+82,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk4__DOT__k),32);
    bufp->fullCData(oldp+83,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__count),6);
    bufp->fullBit(oldp+84,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__match_count));
    bufp->fullIData(oldp+85,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__unnamedblk9__DOT__k),32);
    bufp->fullCData(oldp+86,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[0]),6);
    bufp->fullCData(oldp+87,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[1]),6);
    bufp->fullCData(oldp+88,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[2]),6);
    bufp->fullCData(oldp+89,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[3]),6);
    bufp->fullCData(oldp+90,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[4]),6);
    bufp->fullCData(oldp+91,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[5]),6);
    bufp->fullCData(oldp+92,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[6]),6);
    bufp->fullCData(oldp+93,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[7]),6);
    bufp->fullCData(oldp+94,(vlSelf->mena_top__DOT__u_scheduler__DOT__rr_ptr),3);
    bufp->fullSData(oldp+95,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[0]),16);
    bufp->fullSData(oldp+96,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[1]),16);
    bufp->fullSData(oldp+97,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[2]),16);
    bufp->fullSData(oldp+98,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[3]),16);
    bufp->fullSData(oldp+99,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[4]),16);
    bufp->fullSData(oldp+100,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[5]),16);
    bufp->fullSData(oldp+101,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[6]),16);
    bufp->fullSData(oldp+102,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[7]),16);
    bufp->fullSData(oldp+103,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[0]),16);
    bufp->fullSData(oldp+104,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[1]),16);
    bufp->fullSData(oldp+105,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[2]),16);
    bufp->fullSData(oldp+106,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[3]),16);
    bufp->fullSData(oldp+107,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[4]),16);
    bufp->fullSData(oldp+108,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[5]),16);
    bufp->fullSData(oldp+109,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[6]),16);
    bufp->fullSData(oldp+110,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[7]),16);
    bufp->fullBit(oldp+111,(vlSelf->mena_top__DOT__u_scheduler__DOT__any_nonempty));
    bufp->fullCData(oldp+112,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_rr),3);
    bufp->fullBit(oldp+113,(vlSelf->mena_top__DOT__u_scheduler__DOT__rr_found));
    bufp->fullCData(oldp+114,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_lqf),3);
    bufp->fullCData(oldp+115,(vlSelf->mena_top__DOT__u_scheduler__DOT__lqf_best),6);
    bufp->fullCData(oldp+116,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_hef),3);
    bufp->fullSData(oldp+117,(vlSelf->mena_top__DOT__u_scheduler__DOT__hef_best),16);
    bufp->fullBit(oldp+118,(vlSelf->mena_top__DOT__u_scheduler__DOT__hef_found));
    bufp->fullCData(oldp+119,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_age),3);
    bufp->fullSData(oldp+120,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_best),16);
    bufp->fullBit(oldp+121,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_found));
    bufp->fullIData(oldp+122,(vlSelf->mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i),32);
    bufp->fullIData(oldp+123,(vlSelf->mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i),32);
    bufp->fullIData(oldp+124,(vlSelf->mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i),32);
    bufp->fullCData(oldp+125,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__state_reg),2);
    bufp->fullBit(oldp+126,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_next));
    bufp->fullIData(oldp+127,(vlSelf->mena_top__DOT__reg_ctrl),32);
    bufp->fullIData(oldp+128,((((IData)(vlSelf->mena_top__DOT__busy_reg) 
                                << 1U) | (IData)(vlSelf->mena_top__DOT__done_reg))),32);
    bufp->fullIData(oldp+129,(vlSelf->mena_top__DOT__reg_cache_hit),32);
    bufp->fullIData(oldp+130,(vlSelf->mena_top__DOT__reg_cache_miss),32);
    bufp->fullIData(oldp+131,(vlSelf->mena_top__DOT__reg_dma_cycles),32);
    bufp->fullIData(oldp+132,(vlSelf->mena_top__DOT__reg_total_cycles),32);
    bufp->fullBit(oldp+133,(vlSelf->mena_top__DOT__busy_reg));
    bufp->fullBit(oldp+134,(vlSelf->mena_top__DOT__done_reg));
    bufp->fullCData(oldp+135,(vlSelf->mena_top__DOT__next_sched_expert),3);
    bufp->fullBit(oldp+136,(vlSelf->mena_top__DOT__sched_valid));
    bufp->fullBit(oldp+137,(vlSelf->mena_top__DOT__cache_lookup_hit));
    bufp->fullCData(oldp+138,(vlSelf->mena_top__DOT__u_cache__DOT__req_expert_next),3);
    bufp->fullBit(oldp+139,(vlSelf->mena_top__DOT__u_cache__DOT__req_is_pf_next));
    bufp->fullCData(oldp+140,(vlSelf->mena_top__DOT__u_cache__DOT__alloc_slot_next),2);
    bufp->fullBit(oldp+141,(vlSelf->mena_top__DOT__u_cache__DOT__hit_comb));
    bufp->fullCData(oldp+142,(vlSelf->mena_top__DOT__u_cache__DOT__hit_slot),2);
    bufp->fullBit(oldp+143,(vlSelf->clk));
    bufp->fullBit(oldp+144,(vlSelf->rst_n));
    bufp->fullBit(oldp+145,(vlSelf->reg_write_valid));
    bufp->fullCData(oldp+146,(vlSelf->reg_addr),6);
    bufp->fullIData(oldp+147,(vlSelf->reg_write_data),32);
    bufp->fullIData(oldp+148,(vlSelf->reg_read_data),32);
    bufp->fullBit(oldp+149,(vlSelf->mem_req_valid));
    bufp->fullCData(oldp+150,(vlSelf->mem_req_expert),3);
    bufp->fullBit(oldp+151,(vlSelf->mem_req_is_pf));
    bufp->fullBit(oldp+152,(vlSelf->mem_req_ready));
    bufp->fullBit(oldp+153,(vlSelf->mem_fill_valid));
    bufp->fullCData(oldp+154,(vlSelf->mem_fill_expert),3);
    bufp->fullBit(oldp+155,(vlSelf->token_wr_en));
    bufp->fullCData(oldp+156,(vlSelf->token_wr_id),6);
    bufp->fullCData(oldp+157,(vlSelf->token_wr_experts),6);
    bufp->fullBit(oldp+158,(vlSelf->mena_top__DOT__dispatch_done));
    bufp->fullBit(oldp+159,(vlSelf->mena_top__DOT__cache_lookup_ready));
    bufp->fullCData(oldp+160,(vlSelf->mena_top__DOT__u_cache__DOT__state_next),3);
    bufp->fullCData(oldp+161,((0x1fU & (IData)(vlSelf->token_wr_id))),5);
    bufp->fullBit(oldp+162,(((IData)(vlSelf->mem_req_ready) 
                             & (IData)(vlSelf->mem_req_valid))));
}
