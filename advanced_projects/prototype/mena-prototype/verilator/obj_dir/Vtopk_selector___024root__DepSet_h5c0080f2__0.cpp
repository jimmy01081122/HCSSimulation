// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtopk_selector.h for the primary calling header

#include "Vtopk_selector__pch.h"
#include "Vtopk_selector___024root.h"

VL_INLINE_OPT void Vtopk_selector___024root___ico_sequent__TOP__0(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->topk_selector__DOT__scores[0U] = (0xffffU 
                                              & vlSelf->scores_in[0U]);
    vlSelf->topk_selector__DOT__scores[1U] = (vlSelf->scores_in[0U] 
                                              >> 0x10U);
    vlSelf->topk_selector__DOT__scores[2U] = (0xffffU 
                                              & vlSelf->scores_in[1U]);
    vlSelf->topk_selector__DOT__scores[3U] = (vlSelf->scores_in[1U] 
                                              >> 0x10U);
    vlSelf->topk_selector__DOT__scores[4U] = (0xffffU 
                                              & vlSelf->scores_in[2U]);
    vlSelf->topk_selector__DOT__scores[5U] = (vlSelf->scores_in[2U] 
                                              >> 0x10U);
    vlSelf->topk_selector__DOT__scores[6U] = (0xffffU 
                                              & vlSelf->scores_in[3U]);
    vlSelf->topk_selector__DOT__scores[7U] = (vlSelf->scores_in[3U] 
                                              >> 0x10U);
    vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
        [0U];
    vlSelf->topk_selector__DOT__max1_idx = 0U;
    if ((vlSelf->topk_selector__DOT__scores[1U] > (IData)(vlSelf->topk_selector__DOT__max1_val))) {
        vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
            [1U];
        vlSelf->topk_selector__DOT__max1_idx = 1U;
    }
    if ((vlSelf->topk_selector__DOT__scores[2U] > (IData)(vlSelf->topk_selector__DOT__max1_val))) {
        vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
            [2U];
        vlSelf->topk_selector__DOT__max1_idx = 2U;
    }
    if ((vlSelf->topk_selector__DOT__scores[3U] > (IData)(vlSelf->topk_selector__DOT__max1_val))) {
        vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
            [3U];
        vlSelf->topk_selector__DOT__max1_idx = 3U;
    }
    if ((vlSelf->topk_selector__DOT__scores[4U] > (IData)(vlSelf->topk_selector__DOT__max1_val))) {
        vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
            [4U];
        vlSelf->topk_selector__DOT__max1_idx = 4U;
    }
    if ((vlSelf->topk_selector__DOT__scores[5U] > (IData)(vlSelf->topk_selector__DOT__max1_val))) {
        vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
            [5U];
        vlSelf->topk_selector__DOT__max1_idx = 5U;
    }
    if ((vlSelf->topk_selector__DOT__scores[6U] > (IData)(vlSelf->topk_selector__DOT__max1_val))) {
        vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
            [6U];
        vlSelf->topk_selector__DOT__max1_idx = 6U;
    }
    if ((vlSelf->topk_selector__DOT__scores[7U] > (IData)(vlSelf->topk_selector__DOT__max1_val))) {
        vlSelf->topk_selector__DOT__max1_val = vlSelf->topk_selector__DOT__scores
            [7U];
        vlSelf->topk_selector__DOT__max1_idx = 7U;
    }
    if ((0U == (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        vlSelf->topk_selector__DOT__max2_val = vlSelf->topk_selector__DOT__scores
            [1U];
        vlSelf->topk_selector__DOT__max2_idx = 1U;
    } else {
        vlSelf->topk_selector__DOT__max2_val = vlSelf->topk_selector__DOT__scores
            [0U];
        vlSelf->topk_selector__DOT__max2_idx = 0U;
    }
    if ((0U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[0U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[0U];
            vlSelf->topk_selector__DOT__max2_idx = 0U;
        }
    }
    if ((1U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[1U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[1U];
            vlSelf->topk_selector__DOT__max2_idx = 1U;
        }
    }
    if ((2U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[2U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[2U];
            vlSelf->topk_selector__DOT__max2_idx = 2U;
        }
    }
    if ((3U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[3U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[3U];
            vlSelf->topk_selector__DOT__max2_idx = 3U;
        }
    }
    if ((4U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[4U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[4U];
            vlSelf->topk_selector__DOT__max2_idx = 4U;
        }
    }
    if ((5U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[5U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[5U];
            vlSelf->topk_selector__DOT__max2_idx = 5U;
        }
    }
    if ((6U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[6U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[6U];
            vlSelf->topk_selector__DOT__max2_idx = 6U;
        }
    }
    if ((7U != (IData)(vlSelf->topk_selector__DOT__max1_idx))) {
        if ((vlSelf->topk_selector__DOT__scores[7U] 
             > (IData)(vlSelf->topk_selector__DOT__max2_val))) {
            vlSelf->topk_selector__DOT__max2_val = 
                vlSelf->topk_selector__DOT__scores[7U];
            vlSelf->topk_selector__DOT__max2_idx = 7U;
        }
    }
    vlSelf->topk_idx = (((IData)(vlSelf->topk_selector__DOT__max2_idx) 
                         << 3U) | (IData)(vlSelf->topk_selector__DOT__max1_idx));
    vlSelf->topk_score = (((IData)(vlSelf->topk_selector__DOT__max2_val) 
                           << 0x10U) | (IData)(vlSelf->topk_selector__DOT__max1_val));
}

void Vtopk_selector___024root___eval_ico(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_ico\n"); );
    // Body
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        Vtopk_selector___024root___ico_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
}

void Vtopk_selector___024root___eval_triggers__ico(Vtopk_selector___024root* vlSelf);

bool Vtopk_selector___024root___eval_phase__ico(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_phase__ico\n"); );
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vtopk_selector___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelf->__VicoTriggered.any();
    if (__VicoExecute) {
        Vtopk_selector___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vtopk_selector___024root___eval_act(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_act\n"); );
}

void Vtopk_selector___024root___eval_nba(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_nba\n"); );
}

void Vtopk_selector___024root___eval_triggers__act(Vtopk_selector___024root* vlSelf);

bool Vtopk_selector___024root___eval_phase__act(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<0> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtopk_selector___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vtopk_selector___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtopk_selector___024root___eval_phase__nba(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vtopk_selector___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__ico(Vtopk_selector___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__nba(Vtopk_selector___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtopk_selector___024root___dump_triggers__act(Vtopk_selector___024root* vlSelf);
#endif  // VL_DEBUG

void Vtopk_selector___024root___eval(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VicoIterCount;
    CData/*0:0*/ __VicoContinue;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VicoIterCount = 0U;
    vlSelf->__VicoFirstIteration = 1U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        if (VL_UNLIKELY((0x64U < __VicoIterCount))) {
#ifdef VL_DEBUG
            Vtopk_selector___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("../rtl/topk_selector.sv", 1, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vtopk_selector___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelf->__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vtopk_selector___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("../rtl/topk_selector.sv", 1, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vtopk_selector___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("../rtl/topk_selector.sv", 1, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vtopk_selector___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vtopk_selector___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vtopk_selector___024root___eval_debug_assertions(Vtopk_selector___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtopk_selector___024root___eval_debug_assertions\n"); );
}
#endif  // VL_DEBUG
