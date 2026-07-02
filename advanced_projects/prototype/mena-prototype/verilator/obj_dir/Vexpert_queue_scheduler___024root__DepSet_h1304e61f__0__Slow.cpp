// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vexpert_queue_scheduler.h for the primary calling header

#include "Vexpert_queue_scheduler__pch.h"
#include "Vexpert_queue_scheduler___024root.h"

VL_ATTR_COLD void Vexpert_queue_scheduler___024root___eval_static(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root___eval_initial(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root___eval_final(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__stl(Vexpert_queue_scheduler___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vexpert_queue_scheduler___024root___eval_phase__stl(Vexpert_queue_scheduler___024root* vlSelf);

VL_ATTR_COLD void Vexpert_queue_scheduler___024root___eval_settle(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_settle\n"); );
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
            Vexpert_queue_scheduler___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("../rtl/expert_queue_scheduler.sv", 8, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vexpert_queue_scheduler___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__stl(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

void Vexpert_queue_scheduler___024root___ico_sequent__TOP__0(Vexpert_queue_scheduler___024root* vlSelf);

VL_ATTR_COLD void Vexpert_queue_scheduler___024root___eval_stl(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vexpert_queue_scheduler___024root___ico_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[2U] = 1U;
        vlSelf->__Vm_traceActivity[1U] = 1U;
        vlSelf->__Vm_traceActivity[0U] = 1U;
    }
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root___eval_triggers__stl(Vexpert_queue_scheduler___024root* vlSelf);

VL_ATTR_COLD bool Vexpert_queue_scheduler___024root___eval_phase__stl(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vexpert_queue_scheduler___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vexpert_queue_scheduler___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__ico(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___dump_triggers__ico\n"); );
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
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__act(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__nba(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vexpert_queue_scheduler___024root___ctor_var_reset(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst_n = VL_RAND_RESET_I(1);
    vlSelf->expert_counts = VL_RAND_RESET_Q(48);
    vlSelf->valid_in = VL_RAND_RESET_I(1);
    vlSelf->next_expert = VL_RAND_RESET_I(3);
    vlSelf->sched_valid = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->expert_queue_scheduler__DOT__cnt[__Vi0] = VL_RAND_RESET_I(6);
    }
    vlSelf->expert_queue_scheduler__DOT__rr_ptr = VL_RAND_RESET_I(3);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->expert_queue_scheduler__DOT__hot_score[__Vi0] = VL_RAND_RESET_I(16);
    }
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[__Vi0] = VL_RAND_RESET_I(16);
    }
    vlSelf->expert_queue_scheduler__DOT__any_nonempty = VL_RAND_RESET_I(1);
    vlSelf->expert_queue_scheduler__DOT__sel_rr = VL_RAND_RESET_I(3);
    vlSelf->expert_queue_scheduler__DOT__rr_found = VL_RAND_RESET_I(1);
    vlSelf->expert_queue_scheduler__DOT__sel_lqf = VL_RAND_RESET_I(3);
    vlSelf->expert_queue_scheduler__DOT__lqf_best = VL_RAND_RESET_I(6);
    vlSelf->expert_queue_scheduler__DOT__sel_hef = VL_RAND_RESET_I(3);
    vlSelf->expert_queue_scheduler__DOT__hef_best = VL_RAND_RESET_I(16);
    vlSelf->expert_queue_scheduler__DOT__hef_found = VL_RAND_RESET_I(1);
    vlSelf->expert_queue_scheduler__DOT__sel_age = VL_RAND_RESET_I(3);
    vlSelf->expert_queue_scheduler__DOT__age_best = VL_RAND_RESET_I(16);
    vlSelf->expert_queue_scheduler__DOT__age_found = VL_RAND_RESET_I(1);
    vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i = 0;
    vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i = 0;
    vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i = 0;
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
