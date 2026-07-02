// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtopk_selector.h for the primary calling header

#include "Vtopk_selector__pch.h"
#include "Vtopk_selector___024root.h"

VL_ATTR_COLD void Vtopk_selector___024root___eval_static(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vtopk_selector___024root___eval_initial(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_initial\n"); );
}

VL_ATTR_COLD void Vtopk_selector___024root___eval_final(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__stl(Vtopk_selector___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtopk_selector___024root___eval_phase__stl(Vtopk_selector___024root* vlSelf);

VL_ATTR_COLD void Vtopk_selector___024root___eval_settle(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_settle\n"); );
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelf->__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY((0x64U < __VstlIterCount))) {
#ifdef VL_DEBUG
            Vtopk_selector___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("../rtl/topk_selector.sv", 1, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtopk_selector___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__stl(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

void Vtopk_selector___024root___ico_sequent__TOP__0(Vtopk_selector___024root* vlSelf);

VL_ATTR_COLD void Vtopk_selector___024root___eval_stl(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vtopk_selector___024root___ico_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
        vlSelf->__Vm_traceActivity[0U] = 1U;
    }
}

VL_ATTR_COLD void Vtopk_selector___024root___eval_triggers__stl(Vtopk_selector___024root* vlSelf);

VL_ATTR_COLD bool Vtopk_selector___024root___eval_phase__stl(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtopk_selector___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vtopk_selector___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__ico(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VicoTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__act(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__nba(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtopk_selector___024root___ctor_var_reset(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___ctor_var_reset\n"); );
    // Body
    VL_RAND_RESET_W(128, vlSelf->scores_in);
    vlSelf->topk_idx = VL_RAND_RESET_I(6);
    vlSelf->topk_score = VL_RAND_RESET_I(32);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->topk_selector__DOT__scores[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->topk_selector__DOT__max1_val = VL_RAND_RESET_I(16);
    vlSelf->topk_selector__DOT__max1_idx = VL_RAND_RESET_I(3);
    vlSelf->topk_selector__DOT__max2_val = VL_RAND_RESET_I(16);
    vlSelf->topk_selector__DOT__max2_idx = VL_RAND_RESET_I(3);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
