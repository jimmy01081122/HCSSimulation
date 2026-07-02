// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vexpert_cache_ctrl__Syms.h"


VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_init_sub__TOP__0(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+29,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+30,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+31,0,"lookup_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+32,0,"lookup_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+33,0,"lookup_hit",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+34,0,"lookup_ready",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+35,0,"prefetch_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+36,0,"prefetch_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+37,0,"mem_req_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+38,0,"mem_req_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+39,0,"mem_req_is_pf",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+40,0,"mem_req_ready",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+41,0,"mem_fill_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+42,0,"mem_fill_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->pushPrefix("expert_cache_ctrl", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+43,0,"CACHE_SIZE",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+44,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+45,0,"ADDR_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+29,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+30,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+31,0,"lookup_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+32,0,"lookup_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+33,0,"lookup_hit",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+34,0,"lookup_ready",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+35,0,"prefetch_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+36,0,"prefetch_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+37,0,"mem_req_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+38,0,"mem_req_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+39,0,"mem_req_is_pf",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+40,0,"mem_req_ready",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+41,0,"mem_fill_valid",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+42,0,"mem_fill_expert",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->pushPrefix("tags", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 4; ++i) {
        tracep->declBus(c+9+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 2,0);
    }
    tracep->popPrefix();
    tracep->pushPrefix("valids", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 4; ++i) {
        tracep->declBit(c+13+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0));
    }
    tracep->popPrefix();
    tracep->pushPrefix("age", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 4; ++i) {
        tracep->declBus(c+17+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 1,0);
    }
    tracep->popPrefix();
    tracep->declBus(c+21,0,"state_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+1,0,"state_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+22,0,"req_expert_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+2,0,"req_expert_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+23,0,"req_is_pf_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+3,0,"req_is_pf_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+24,0,"alloc_slot_reg",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+4,0,"alloc_slot_next",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+5,0,"hit_comb",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+6,0,"hit_slot",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+25,0,"lru_slot",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBit(c+7,0,"pf_hit",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+46,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk2", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+46,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk3", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+26,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk4", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+27,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk5", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+28,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk6", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+8,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_init_top(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_init_top\n"); );
    // Body
    Vexpert_cache_ctrl___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vexpert_cache_ctrl___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vexpert_cache_ctrl___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_register(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_register\n"); );
    // Body
    tracep->addConstCb(&Vexpert_cache_ctrl___024root__trace_const_0, 0U, vlSelf);
    tracep->addFullCb(&Vexpert_cache_ctrl___024root__trace_full_0, 0U, vlSelf);
    tracep->addChgCb(&Vexpert_cache_ctrl___024root__trace_chg_0, 0U, vlSelf);
    tracep->addCleanupCb(&Vexpert_cache_ctrl___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_const_0_sub_0(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_const_0\n"); );
    // Init
    Vexpert_cache_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_cache_ctrl___024root*>(voidSelf);
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vexpert_cache_ctrl___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_const_0_sub_0(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_const_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+43,(4U),32);
    bufp->fullIData(oldp+44,(8U),32);
    bufp->fullIData(oldp+45,(3U),32);
    bufp->fullIData(oldp+46,(4U),32);
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_full_0_sub_0(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_full_0\n"); );
    // Init
    Vexpert_cache_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_cache_ctrl___024root*>(voidSelf);
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vexpert_cache_ctrl___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_full_0_sub_0(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_full_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullCData(oldp+1,(vlSelf->expert_cache_ctrl__DOT__state_next),3);
    bufp->fullCData(oldp+2,(vlSelf->expert_cache_ctrl__DOT__req_expert_next),3);
    bufp->fullBit(oldp+3,(vlSelf->expert_cache_ctrl__DOT__req_is_pf_next));
    bufp->fullCData(oldp+4,(vlSelf->expert_cache_ctrl__DOT__alloc_slot_next),2);
    bufp->fullBit(oldp+5,(vlSelf->expert_cache_ctrl__DOT__hit_comb));
    bufp->fullCData(oldp+6,(vlSelf->expert_cache_ctrl__DOT__hit_slot),2);
    bufp->fullBit(oldp+7,(vlSelf->expert_cache_ctrl__DOT__pf_hit));
    bufp->fullIData(oldp+8,(vlSelf->expert_cache_ctrl__DOT__unnamedblk6__DOT__i),32);
    bufp->fullCData(oldp+9,(vlSelf->expert_cache_ctrl__DOT__tags[0]),3);
    bufp->fullCData(oldp+10,(vlSelf->expert_cache_ctrl__DOT__tags[1]),3);
    bufp->fullCData(oldp+11,(vlSelf->expert_cache_ctrl__DOT__tags[2]),3);
    bufp->fullCData(oldp+12,(vlSelf->expert_cache_ctrl__DOT__tags[3]),3);
    bufp->fullBit(oldp+13,(vlSelf->expert_cache_ctrl__DOT__valids[0]));
    bufp->fullBit(oldp+14,(vlSelf->expert_cache_ctrl__DOT__valids[1]));
    bufp->fullBit(oldp+15,(vlSelf->expert_cache_ctrl__DOT__valids[2]));
    bufp->fullBit(oldp+16,(vlSelf->expert_cache_ctrl__DOT__valids[3]));
    bufp->fullCData(oldp+17,(vlSelf->expert_cache_ctrl__DOT__age[0]),2);
    bufp->fullCData(oldp+18,(vlSelf->expert_cache_ctrl__DOT__age[1]),2);
    bufp->fullCData(oldp+19,(vlSelf->expert_cache_ctrl__DOT__age[2]),2);
    bufp->fullCData(oldp+20,(vlSelf->expert_cache_ctrl__DOT__age[3]),2);
    bufp->fullCData(oldp+21,(vlSelf->expert_cache_ctrl__DOT__state_reg),3);
    bufp->fullCData(oldp+22,(vlSelf->expert_cache_ctrl__DOT__req_expert_reg),3);
    bufp->fullBit(oldp+23,(vlSelf->expert_cache_ctrl__DOT__req_is_pf_reg));
    bufp->fullCData(oldp+24,(vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg),2);
    bufp->fullCData(oldp+25,(vlSelf->expert_cache_ctrl__DOT__lru_slot),2);
    bufp->fullIData(oldp+26,(vlSelf->expert_cache_ctrl__DOT__unnamedblk3__DOT__i),32);
    bufp->fullIData(oldp+27,(vlSelf->expert_cache_ctrl__DOT__unnamedblk4__DOT__i),32);
    bufp->fullIData(oldp+28,(vlSelf->expert_cache_ctrl__DOT__unnamedblk5__DOT__i),32);
    bufp->fullBit(oldp+29,(vlSelf->clk));
    bufp->fullBit(oldp+30,(vlSelf->rst_n));
    bufp->fullBit(oldp+31,(vlSelf->lookup_valid));
    bufp->fullCData(oldp+32,(vlSelf->lookup_expert),3);
    bufp->fullBit(oldp+33,(vlSelf->lookup_hit));
    bufp->fullBit(oldp+34,(vlSelf->lookup_ready));
    bufp->fullBit(oldp+35,(vlSelf->prefetch_valid));
    bufp->fullCData(oldp+36,(vlSelf->prefetch_expert),3);
    bufp->fullBit(oldp+37,(vlSelf->mem_req_valid));
    bufp->fullCData(oldp+38,(vlSelf->mem_req_expert),3);
    bufp->fullBit(oldp+39,(vlSelf->mem_req_is_pf));
    bufp->fullBit(oldp+40,(vlSelf->mem_req_ready));
    bufp->fullBit(oldp+41,(vlSelf->mem_fill_valid));
    bufp->fullCData(oldp+42,(vlSelf->mem_fill_expert),3);
}
