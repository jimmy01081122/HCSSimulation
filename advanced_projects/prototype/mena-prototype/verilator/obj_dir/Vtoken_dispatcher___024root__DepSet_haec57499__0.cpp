// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtoken_dispatcher.h for the primary calling header

#include "Vtoken_dispatcher__pch.h"
#include "Vtoken_dispatcher__Syms.h"
#include "Vtoken_dispatcher___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtoken_dispatcher___024root___dump_triggers__ico(Vtoken_dispatcher___024root* vlSelf);
#endif  // VL_DEBUG

void Vtoken_dispatcher___024root___eval_triggers__ico(Vtoken_dispatcher___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root___eval_triggers__ico\n"); );
    // Body
    vlSelf->__VicoTriggered.set(0U, (IData)(vlSelf->__VicoFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtoken_dispatcher___024root___dump_triggers__ico(vlSelf);
    }
#endif
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtoken_dispatcher___024root___dump_triggers__act(Vtoken_dispatcher___024root* vlSelf);
#endif  // VL_DEBUG

void Vtoken_dispatcher___024root___eval_triggers__act(Vtoken_dispatcher___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root___eval_triggers__act\n"); );
    // Body
    vlSelf->__VactTriggered.set(0U, (((IData)(vlSelf->clk) 
                                      & (~ (IData)(vlSelf->__Vtrigprevexpr___TOP__clk__0))) 
                                     | ((~ (IData)(vlSelf->rst_n)) 
                                        & (IData)(vlSelf->__Vtrigprevexpr___TOP__rst_n__0))));
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtoken_dispatcher___024root___dump_triggers__act(vlSelf);
    }
#endif
}
