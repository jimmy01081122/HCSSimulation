// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vexpert_queue_scheduler.h for the primary calling header

#include "Vexpert_queue_scheduler__pch.h"
#include "Vexpert_queue_scheduler___024root.h"

VL_INLINE_OPT void Vexpert_queue_scheduler___024root___ico_sequent__TOP__0(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->expert_queue_scheduler__DOT__cnt[0U] = 
        (0x3fU & (IData)(vlSelf->expert_counts));
    vlSelf->expert_queue_scheduler__DOT__cnt[1U] = 
        (0x3fU & (IData)((vlSelf->expert_counts >> 6U)));
    vlSelf->expert_queue_scheduler__DOT__cnt[2U] = 
        (0x3fU & (IData)((vlSelf->expert_counts >> 0xcU)));
    vlSelf->expert_queue_scheduler__DOT__cnt[3U] = 
        (0x3fU & (IData)((vlSelf->expert_counts >> 0x12U)));
    vlSelf->expert_queue_scheduler__DOT__cnt[4U] = 
        (0x3fU & (IData)((vlSelf->expert_counts >> 0x18U)));
    vlSelf->expert_queue_scheduler__DOT__cnt[5U] = 
        (0x3fU & (IData)((vlSelf->expert_counts >> 0x1eU)));
    vlSelf->expert_queue_scheduler__DOT__cnt[6U] = 
        (0x3fU & (IData)((vlSelf->expert_counts >> 0x24U)));
    vlSelf->expert_queue_scheduler__DOT__cnt[7U] = 
        (0x3fU & (IData)((vlSelf->expert_counts >> 0x2aU)));
    vlSelf->expert_queue_scheduler__DOT__sel_lqf = 0U;
    vlSelf->expert_queue_scheduler__DOT__lqf_best = 0U;
    if ((0U < vlSelf->expert_queue_scheduler__DOT__cnt
         [0U])) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [0U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 0U;
    }
    if ((vlSelf->expert_queue_scheduler__DOT__cnt[1U] 
         > (IData)(vlSelf->expert_queue_scheduler__DOT__lqf_best))) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [1U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 1U;
    }
    if ((vlSelf->expert_queue_scheduler__DOT__cnt[2U] 
         > (IData)(vlSelf->expert_queue_scheduler__DOT__lqf_best))) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [2U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 2U;
    }
    if ((vlSelf->expert_queue_scheduler__DOT__cnt[3U] 
         > (IData)(vlSelf->expert_queue_scheduler__DOT__lqf_best))) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [3U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 3U;
    }
    if ((vlSelf->expert_queue_scheduler__DOT__cnt[4U] 
         > (IData)(vlSelf->expert_queue_scheduler__DOT__lqf_best))) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [4U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 4U;
    }
    if ((vlSelf->expert_queue_scheduler__DOT__cnt[5U] 
         > (IData)(vlSelf->expert_queue_scheduler__DOT__lqf_best))) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [5U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 5U;
    }
    if ((vlSelf->expert_queue_scheduler__DOT__cnt[6U] 
         > (IData)(vlSelf->expert_queue_scheduler__DOT__lqf_best))) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [6U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 6U;
    }
    if ((vlSelf->expert_queue_scheduler__DOT__cnt[7U] 
         > (IData)(vlSelf->expert_queue_scheduler__DOT__lqf_best))) {
        vlSelf->expert_queue_scheduler__DOT__lqf_best 
            = vlSelf->expert_queue_scheduler__DOT__cnt
            [7U];
        vlSelf->expert_queue_scheduler__DOT__sel_lqf = 7U;
    }
    vlSelf->expert_queue_scheduler__DOT__any_nonempty = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [0U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [1U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [2U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [3U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [4U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [5U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    vlSelf->expert_queue_scheduler__DOT__sel_rr = 0U;
    vlSelf->expert_queue_scheduler__DOT__rr_found = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [(7U & VL_MODDIVS_III(32, (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr), (IData)(8U)))])) {
        vlSelf->expert_queue_scheduler__DOT__sel_rr 
            = (7U & VL_MODDIVS_III(32, (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr), (IData)(8U)));
        vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(1U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(1U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(2U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(2U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(3U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(3U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(4U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(4U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(5U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(5U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(6U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(6U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(7U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(7U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    vlSelf->sched_valid = 0U;
    vlSelf->next_expert = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [6U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [7U])) {
        vlSelf->expert_queue_scheduler__DOT__any_nonempty = 1U;
    }
    if (((IData)(vlSelf->valid_in) & (IData)(vlSelf->expert_queue_scheduler__DOT__any_nonempty))) {
        vlSelf->sched_valid = 1U;
        vlSelf->next_expert = vlSelf->expert_queue_scheduler__DOT__sel_rr;
    }
    vlSelf->expert_queue_scheduler__DOT__sel_hef = 0U;
    vlSelf->expert_queue_scheduler__DOT__hef_best = 0U;
    vlSelf->expert_queue_scheduler__DOT__hef_found = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [0U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [0U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [0U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 0U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [1U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [1U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [1U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 1U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [2U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [2U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [2U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 2U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [3U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [3U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [3U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 3U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [4U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [4U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [4U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 4U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [5U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [5U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [5U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 5U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [6U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [6U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [6U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 6U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [7U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [7U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [7U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 7U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)))) {
        vlSelf->expert_queue_scheduler__DOT__sel_hef 
            = vlSelf->expert_queue_scheduler__DOT__sel_rr;
    }
    vlSelf->expert_queue_scheduler__DOT__sel_age = 0U;
    vlSelf->expert_queue_scheduler__DOT__age_best = 0U;
    vlSelf->expert_queue_scheduler__DOT__age_found = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [0U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [0U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [0U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 0U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [1U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [1U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [1U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 1U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [2U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [2U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [2U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 2U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [3U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [3U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [3U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 3U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [4U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [4U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [4U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 4U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [5U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [5U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [5U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 5U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [6U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [6U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [6U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 6U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [7U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [7U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [7U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 7U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)))) {
        vlSelf->expert_queue_scheduler__DOT__sel_age 
            = vlSelf->expert_queue_scheduler__DOT__sel_rr;
    }
}

void Vexpert_queue_scheduler___024root___eval_ico(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_ico\n"); );
    // Body
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        Vexpert_queue_scheduler___024root___ico_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
}

void Vexpert_queue_scheduler___024root___eval_triggers__ico(Vexpert_queue_scheduler___024root* vlSelf);

bool Vexpert_queue_scheduler___024root___eval_phase__ico(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_phase__ico\n"); );
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vexpert_queue_scheduler___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelf->__VicoTriggered.any();
    if (__VicoExecute) {
        Vexpert_queue_scheduler___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vexpert_queue_scheduler___024root___eval_act(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vexpert_queue_scheduler___024root___nba_sequent__TOP__0(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v0;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v0 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v1;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v1 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v1;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v1 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v2;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v2 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v3;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v3 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v3;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v3 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v4;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v4 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v5;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v5 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v5;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v5 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v6;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v6 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v7;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v7 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v7;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v7 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v8;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v8 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v9;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v9 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v9;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v9 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v10;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v10 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v11;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v11 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v11;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v11 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v12;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v12 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v13;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v13 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v13;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v13 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v14;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v14 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v15;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v15 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v15;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v15 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v16;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v16 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v16;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v16 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v17;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v17 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v17;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v17 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v18;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v18 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v18;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v18 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v19;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v19 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v19;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v19 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v20;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v20 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v20;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v20 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v21;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v21 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v21;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v21 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v22;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v22 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v22;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v22 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v23;
    __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v23 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v23;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v23 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v24;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v24 = 0;
    CData/*2:0*/ __Vdlyvdim0__expert_queue_scheduler__DOT__hot_score__v0;
    __Vdlyvdim0__expert_queue_scheduler__DOT__hot_score__v0 = 0;
    SData/*15:0*/ __Vdlyvval__expert_queue_scheduler__DOT__hot_score__v0;
    __Vdlyvval__expert_queue_scheduler__DOT__hot_score__v0 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v0;
    __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v0 = 0;
    CData/*0:0*/ __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v1;
    __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v1 = 0;
    // Body
    __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v0 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v1 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v0 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v1 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v2 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v3 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v4 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v5 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v6 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v7 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v8 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v9 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v10 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v11 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v12 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v13 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v14 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v15 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v16 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v17 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v18 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v19 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v20 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v21 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v22 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v23 = 0U;
    __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v24 = 0U;
    if ((1U & (~ (IData)(vlSelf->rst_n)))) {
        vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i = 8U;
    }
    if (vlSelf->rst_n) {
        if (((IData)(vlSelf->valid_in) & (IData)(vlSelf->expert_queue_scheduler__DOT__any_nonempty))) {
            vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i = 8U;
            vlSelf->expert_queue_scheduler__DOT__rr_ptr 
                = (7U & VL_MODDIVS_III(32, ((IData)(1U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            __Vdlyvval__expert_queue_scheduler__DOT__hot_score__v0 
                = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__hot_score
                              [vlSelf->next_expert]));
            __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v0 = 1U;
            __Vdlyvdim0__expert_queue_scheduler__DOT__hot_score__v0 
                = vlSelf->next_expert;
            if ((0U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v0 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [0U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v1 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [0U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v1 = 1U;
            }
            if ((1U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v2 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [1U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v3 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [1U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v3 = 1U;
            }
            if ((2U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v4 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [2U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v5 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [2U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v5 = 1U;
            }
            if ((3U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v6 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [3U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v7 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [3U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v7 = 1U;
            }
            if ((4U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v8 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [4U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v9 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [4U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v9 = 1U;
            }
            if ((5U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v10 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [5U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v11 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [5U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v11 = 1U;
            }
            if ((6U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v12 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [6U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v13 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [6U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v13 = 1U;
            }
            if ((7U == (IData)(vlSelf->next_expert))) {
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v14 = 1U;
            } else if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                        [7U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v15 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [7U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v15 = 1U;
            }
        } else {
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [0U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v16 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [0U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v16 = 1U;
            }
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [1U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v17 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [1U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v17 = 1U;
            }
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [2U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v18 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [2U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v18 = 1U;
            }
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [3U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v19 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [3U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v19 = 1U;
            }
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [4U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v20 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [4U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v20 = 1U;
            }
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [5U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v21 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [5U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v21 = 1U;
            }
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [6U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v22 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [6U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v22 = 1U;
            }
            if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
                 [7U])) {
                __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v23 
                    = (0xffffU & ((IData)(1U) + vlSelf->expert_queue_scheduler__DOT__age_counter
                                  [7U]));
                __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v23 = 1U;
            }
        }
        if ((1U & (~ ((IData)(vlSelf->valid_in) & (IData)(vlSelf->expert_queue_scheduler__DOT__any_nonempty))))) {
            vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i = 8U;
        }
    } else {
        vlSelf->expert_queue_scheduler__DOT__rr_ptr = 0U;
        __Vdlyvset__expert_queue_scheduler__DOT__hot_score__v1 = 1U;
        __Vdlyvset__expert_queue_scheduler__DOT__age_counter__v24 = 1U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__hot_score__v0) {
        vlSelf->expert_queue_scheduler__DOT__hot_score[__Vdlyvdim0__expert_queue_scheduler__DOT__hot_score__v0] 
            = __Vdlyvval__expert_queue_scheduler__DOT__hot_score__v0;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__hot_score__v1) {
        vlSelf->expert_queue_scheduler__DOT__hot_score[0U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__hot_score[1U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__hot_score[2U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__hot_score[3U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__hot_score[4U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__hot_score[5U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__hot_score[6U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__hot_score[7U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v0) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[0U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v1) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[0U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v1;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v2) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[1U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v3) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[1U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v3;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v4) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[2U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v5) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[2U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v5;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v6) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[3U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v7) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[3U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v7;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v8) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[4U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v9) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[4U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v9;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v10) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[5U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v11) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[5U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v11;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v12) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[6U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v13) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[6U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v13;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v14) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[7U] = 0U;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v15) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[7U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v15;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v16) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[0U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v16;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v17) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[1U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v17;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v18) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[2U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v18;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v19) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[3U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v19;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v20) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[4U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v20;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v21) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[5U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v21;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v22) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[6U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v22;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v23) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[7U] 
            = __Vdlyvval__expert_queue_scheduler__DOT__age_counter__v23;
    }
    if (__Vdlyvset__expert_queue_scheduler__DOT__age_counter__v24) {
        vlSelf->expert_queue_scheduler__DOT__age_counter[0U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__age_counter[1U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__age_counter[2U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__age_counter[3U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__age_counter[4U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__age_counter[5U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__age_counter[6U] = 0U;
        vlSelf->expert_queue_scheduler__DOT__age_counter[7U] = 0U;
    }
}

VL_INLINE_OPT void Vexpert_queue_scheduler___024root___nba_sequent__TOP__1(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___nba_sequent__TOP__1\n"); );
    // Body
    vlSelf->expert_queue_scheduler__DOT__sel_rr = 0U;
    vlSelf->expert_queue_scheduler__DOT__rr_found = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [(7U & VL_MODDIVS_III(32, (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr), (IData)(8U)))])) {
        vlSelf->expert_queue_scheduler__DOT__sel_rr 
            = (7U & VL_MODDIVS_III(32, (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr), (IData)(8U)));
        vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(1U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(1U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(2U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(2U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(3U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(3U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(4U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(4U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(5U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(5U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(6U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(6U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__rr_found)))) {
        if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
             [(7U & VL_MODDIVS_III(32, ((IData)(7U) 
                                        + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)))])) {
            vlSelf->expert_queue_scheduler__DOT__sel_rr 
                = (7U & VL_MODDIVS_III(32, ((IData)(7U) 
                                            + (IData)(vlSelf->expert_queue_scheduler__DOT__rr_ptr)), (IData)(8U)));
            vlSelf->expert_queue_scheduler__DOT__rr_found = 1U;
        }
    }
    vlSelf->next_expert = 0U;
    if (((IData)(vlSelf->valid_in) & (IData)(vlSelf->expert_queue_scheduler__DOT__any_nonempty))) {
        vlSelf->next_expert = vlSelf->expert_queue_scheduler__DOT__sel_rr;
    }
    vlSelf->expert_queue_scheduler__DOT__sel_hef = 0U;
    vlSelf->expert_queue_scheduler__DOT__hef_best = 0U;
    vlSelf->expert_queue_scheduler__DOT__hef_found = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [0U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [0U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [0U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 0U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [1U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [1U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [1U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 1U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [2U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [2U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [2U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 2U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [3U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [3U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [3U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 3U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [4U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [4U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [4U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 4U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [5U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [5U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [5U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 5U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [6U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [6U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [6U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 6U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [7U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__hot_score
                      [7U] > (IData)(vlSelf->expert_queue_scheduler__DOT__hef_best))))) {
            vlSelf->expert_queue_scheduler__DOT__hef_best 
                = vlSelf->expert_queue_scheduler__DOT__hot_score
                [7U];
            vlSelf->expert_queue_scheduler__DOT__sel_hef = 7U;
            vlSelf->expert_queue_scheduler__DOT__hef_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__hef_found)))) {
        vlSelf->expert_queue_scheduler__DOT__sel_hef 
            = vlSelf->expert_queue_scheduler__DOT__sel_rr;
    }
    vlSelf->expert_queue_scheduler__DOT__sel_age = 0U;
    vlSelf->expert_queue_scheduler__DOT__age_best = 0U;
    vlSelf->expert_queue_scheduler__DOT__age_found = 0U;
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [0U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [0U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [0U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 0U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [1U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [1U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [1U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 1U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [2U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [2U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [2U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 2U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [3U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [3U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [3U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 3U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [4U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [4U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [4U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 4U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [5U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [5U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [5U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 5U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [6U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [6U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [6U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 6U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((0U != vlSelf->expert_queue_scheduler__DOT__cnt
         [7U])) {
        if ((1U & ((~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)) 
                   | (vlSelf->expert_queue_scheduler__DOT__age_counter
                      [7U] > (IData)(vlSelf->expert_queue_scheduler__DOT__age_best))))) {
            vlSelf->expert_queue_scheduler__DOT__age_best 
                = vlSelf->expert_queue_scheduler__DOT__age_counter
                [7U];
            vlSelf->expert_queue_scheduler__DOT__sel_age = 7U;
            vlSelf->expert_queue_scheduler__DOT__age_found = 1U;
        }
    }
    if ((1U & (~ (IData)(vlSelf->expert_queue_scheduler__DOT__age_found)))) {
        vlSelf->expert_queue_scheduler__DOT__sel_age 
            = vlSelf->expert_queue_scheduler__DOT__sel_rr;
    }
}

void Vexpert_queue_scheduler___024root___eval_nba(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vexpert_queue_scheduler___024root___nba_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[2U] = 1U;
        Vexpert_queue_scheduler___024root___nba_sequent__TOP__1(vlSelf);
    }
}

void Vexpert_queue_scheduler___024root___eval_triggers__act(Vexpert_queue_scheduler___024root* vlSelf);

bool Vexpert_queue_scheduler___024root___eval_phase__act(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vexpert_queue_scheduler___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vexpert_queue_scheduler___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vexpert_queue_scheduler___024root___eval_phase__nba(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vexpert_queue_scheduler___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__ico(Vexpert_queue_scheduler___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__nba(Vexpert_queue_scheduler___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_queue_scheduler___024root___dump_triggers__act(Vexpert_queue_scheduler___024root* vlSelf);
#endif  // VL_DEBUG

void Vexpert_queue_scheduler___024root___eval(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval\n"); );
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
            Vexpert_queue_scheduler___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("../rtl/expert_queue_scheduler.sv", 8, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vexpert_queue_scheduler___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelf->__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vexpert_queue_scheduler___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("../rtl/expert_queue_scheduler.sv", 8, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vexpert_queue_scheduler___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("../rtl/expert_queue_scheduler.sv", 8, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vexpert_queue_scheduler___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vexpert_queue_scheduler___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vexpert_queue_scheduler___024root___eval_debug_assertions(Vexpert_queue_scheduler___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst_n & 0xfeU))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY((vlSelf->expert_counts & 0ULL))) {
        Verilated::overWidthError("expert_counts");}
    if (VL_UNLIKELY((vlSelf->valid_in & 0xfeU))) {
        Verilated::overWidthError("valid_in");}
}
#endif  // VL_DEBUG
