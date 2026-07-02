// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtopk_selector__Syms.h"


VL_ATTR_COLD void Vtopk_selector___024root__trace_init_sub__TOP__0(Vtopk_selector___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declArray(c+13,0,"scores_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 127,0);
    tracep->declBus(c+17,0,"topk_idx",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+18,0,"topk_score",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("topk_selector", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+19,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+20,0,"SCORE_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+21,0,"TOP_K",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+22,0,"IDX_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declArray(c+13,0,"scores_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 127,0);
    tracep->declBus(c+17,0,"topk_idx",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+18,0,"topk_score",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("scores", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 8; ++i) {
        tracep->declBus(c+1+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 15,0);
    }
    tracep->popPrefix();
    tracep->declBus(c+9,0,"max1_val",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBus(c+10,0,"max1_idx",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+11,0,"max2_val",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBus(c+12,0,"max2_idx",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+23,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk2", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+23,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk3", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+23,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtopk_selector___024root__trace_init_top(Vtopk_selector___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_init_top\n"); );
    // Body
    Vtopk_selector___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vtopk_selector___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void Vtopk_selector___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtopk_selector___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtopk_selector___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vtopk_selector___024root__trace_register(Vtopk_selector___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_register\n"); );
    // Body
    tracep->addConstCb(&Vtopk_selector___024root__trace_const_0, 0U, vlSelf);
    tracep->addFullCb(&Vtopk_selector___024root__trace_full_0, 0U, vlSelf);
    tracep->addChgCb(&Vtopk_selector___024root__trace_chg_0, 0U, vlSelf);
    tracep->addCleanupCb(&Vtopk_selector___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vtopk_selector___024root__trace_const_0_sub_0(Vtopk_selector___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vtopk_selector___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_const_0\n"); );
    // Init
    Vtopk_selector___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtopk_selector___024root*>(voidSelf);
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vtopk_selector___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtopk_selector___024root__trace_const_0_sub_0(Vtopk_selector___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_const_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+19,(8U),32);
    bufp->fullIData(oldp+20,(0x10U),32);
    bufp->fullIData(oldp+21,(2U),32);
    bufp->fullIData(oldp+22,(3U),32);
    bufp->fullIData(oldp+23,(8U),32);
}

VL_ATTR_COLD void Vtopk_selector___024root__trace_full_0_sub_0(Vtopk_selector___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vtopk_selector___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_full_0\n"); );
    // Init
    Vtopk_selector___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtopk_selector___024root*>(voidSelf);
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vtopk_selector___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtopk_selector___024root__trace_full_0_sub_0(Vtopk_selector___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_full_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullSData(oldp+1,(vlSelf->topk_selector__DOT__scores[0]),16);
    bufp->fullSData(oldp+2,(vlSelf->topk_selector__DOT__scores[1]),16);
    bufp->fullSData(oldp+3,(vlSelf->topk_selector__DOT__scores[2]),16);
    bufp->fullSData(oldp+4,(vlSelf->topk_selector__DOT__scores[3]),16);
    bufp->fullSData(oldp+5,(vlSelf->topk_selector__DOT__scores[4]),16);
    bufp->fullSData(oldp+6,(vlSelf->topk_selector__DOT__scores[5]),16);
    bufp->fullSData(oldp+7,(vlSelf->topk_selector__DOT__scores[6]),16);
    bufp->fullSData(oldp+8,(vlSelf->topk_selector__DOT__scores[7]),16);
    bufp->fullSData(oldp+9,(vlSelf->topk_selector__DOT__max1_val),16);
    bufp->fullCData(oldp+10,(vlSelf->topk_selector__DOT__max1_idx),3);
    bufp->fullSData(oldp+11,(vlSelf->topk_selector__DOT__max2_val),16);
    bufp->fullCData(oldp+12,(vlSelf->topk_selector__DOT__max2_idx),3);
    bufp->fullWData(oldp+13,(vlSelf->scores_in),128);
    bufp->fullCData(oldp+17,(vlSelf->topk_idx),6);
    bufp->fullIData(oldp+18,(vlSelf->topk_score),32);
}
