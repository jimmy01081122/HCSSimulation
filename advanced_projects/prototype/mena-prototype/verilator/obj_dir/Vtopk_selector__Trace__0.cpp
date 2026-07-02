// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtopk_selector__Syms.h"


void Vtopk_selector___024root__trace_chg_0_sub_0(Vtopk_selector___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vtopk_selector___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_chg_0\n"); );
    // Init
    Vtopk_selector___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtopk_selector___024root*>(voidSelf);
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vtopk_selector___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vtopk_selector___024root__trace_chg_0_sub_0(Vtopk_selector___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_chg_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
        bufp->chgSData(oldp+0,(vlSelf->topk_selector__DOT__scores[0]),16);
        bufp->chgSData(oldp+1,(vlSelf->topk_selector__DOT__scores[1]),16);
        bufp->chgSData(oldp+2,(vlSelf->topk_selector__DOT__scores[2]),16);
        bufp->chgSData(oldp+3,(vlSelf->topk_selector__DOT__scores[3]),16);
        bufp->chgSData(oldp+4,(vlSelf->topk_selector__DOT__scores[4]),16);
        bufp->chgSData(oldp+5,(vlSelf->topk_selector__DOT__scores[5]),16);
        bufp->chgSData(oldp+6,(vlSelf->topk_selector__DOT__scores[6]),16);
        bufp->chgSData(oldp+7,(vlSelf->topk_selector__DOT__scores[7]),16);
        bufp->chgSData(oldp+8,(vlSelf->topk_selector__DOT__max1_val),16);
        bufp->chgCData(oldp+9,(vlSelf->topk_selector__DOT__max1_idx),3);
        bufp->chgSData(oldp+10,(vlSelf->topk_selector__DOT__max2_val),16);
        bufp->chgCData(oldp+11,(vlSelf->topk_selector__DOT__max2_idx),3);
    }
    bufp->chgWData(oldp+12,(vlSelf->scores_in),128);
    bufp->chgCData(oldp+16,(vlSelf->topk_idx),6);
    bufp->chgIData(oldp+17,(vlSelf->topk_score),32);
}

void Vtopk_selector___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root__trace_cleanup\n"); );
    // Init
    Vtopk_selector___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtopk_selector___024root*>(voidSelf);
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
}
