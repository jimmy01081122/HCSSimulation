// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vweight_buffer_ctrl__Syms.h"


void Vweight_buffer_ctrl___024root__trace_chg_0_sub_0(Vweight_buffer_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vweight_buffer_ctrl___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root__trace_chg_0\n"); );
    // Init
    Vweight_buffer_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vweight_buffer_ctrl___024root*>(voidSelf);
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vweight_buffer_ctrl___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vweight_buffer_ctrl___024root__trace_chg_0_sub_0(Vweight_buffer_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root__trace_chg_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
        bufp->chgCData(oldp+0,(vlSelf->weight_buffer_ctrl__DOT__state_reg),2);
        bufp->chgBit(oldp+1,(vlSelf->weight_buffer_ctrl__DOT__active_sel_reg));
        bufp->chgBit(oldp+2,(vlSelf->weight_buffer_ctrl__DOT__active_sel_next));
        bufp->chgBit(oldp+3,(vlSelf->weight_buffer_ctrl__DOT__shadow_valid_reg));
        bufp->chgBit(oldp+4,(vlSelf->weight_buffer_ctrl__DOT__active_valid_reg));
    }
    bufp->chgBit(oldp+5,(vlSelf->clk));
    bufp->chgBit(oldp+6,(vlSelf->rst_n));
    bufp->chgBit(oldp+7,(vlSelf->dma_start));
    bufp->chgBit(oldp+8,(vlSelf->shadow_done));
    bufp->chgBit(oldp+9,(vlSelf->exec_start));
    bufp->chgBit(oldp+10,(vlSelf->exec_done));
    bufp->chgBit(oldp+11,(vlSelf->active_sel));
    bufp->chgBit(oldp+12,(vlSelf->dma_sel));
    bufp->chgBit(oldp+13,(vlSelf->stall));
    bufp->chgBit(oldp+14,(vlSelf->swap_event));
    bufp->chgBit(oldp+15,(vlSelf->shadow_valid));
    bufp->chgBit(oldp+16,(vlSelf->active_valid));
    bufp->chgCData(oldp+17,(vlSelf->weight_buffer_ctrl__DOT__state_next),2);
    bufp->chgBit(oldp+18,(vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next));
    bufp->chgBit(oldp+19,(vlSelf->weight_buffer_ctrl__DOT__active_valid_next));
}

void Vweight_buffer_ctrl___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root__trace_cleanup\n"); );
    // Init
    Vweight_buffer_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vweight_buffer_ctrl___024root*>(voidSelf);
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
}
