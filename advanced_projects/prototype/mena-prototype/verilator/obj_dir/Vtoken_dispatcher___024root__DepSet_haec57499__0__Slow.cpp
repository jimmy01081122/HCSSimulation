// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtoken_dispatcher.h for the primary calling header

#include "Vtoken_dispatcher__pch.h"
#include "Vtoken_dispatcher__Syms.h"
#include "Vtoken_dispatcher___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtoken_dispatcher___024root___dump_triggers__stl(Vtoken_dispatcher___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtoken_dispatcher___024root___eval_triggers__stl(Vtoken_dispatcher___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (IData)(vlSelf->__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtoken_dispatcher___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
