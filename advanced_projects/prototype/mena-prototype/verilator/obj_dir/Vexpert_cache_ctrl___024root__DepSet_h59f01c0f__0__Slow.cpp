// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vexpert_cache_ctrl.h for the primary calling header

#include "Vexpert_cache_ctrl__pch.h"
#include "Vexpert_cache_ctrl__Syms.h"
#include "Vexpert_cache_ctrl___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__stl(Vexpert_cache_ctrl___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___eval_triggers__stl(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (IData)(vlSelf->__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vexpert_cache_ctrl___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
