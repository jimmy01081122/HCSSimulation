// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vexpert_cache_ctrl.h for the primary calling header

#include "Vexpert_cache_ctrl__pch.h"
#include "Vexpert_cache_ctrl___024root.h"

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___eval_static(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___eval_initial(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___eval_final(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__stl(Vexpert_cache_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vexpert_cache_ctrl___024root___eval_phase__stl(Vexpert_cache_ctrl___024root* vlSelf);

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___eval_settle(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_settle\n"); );
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
            Vexpert_cache_ctrl___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("../rtl/expert_cache_ctrl.sv", 5, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vexpert_cache_ctrl___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__stl(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___stl_sequent__TOP__0(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___stl_sequent__TOP__0\n"); );
    // Body
    vlSelf->mem_req_is_pf = vlSelf->expert_cache_ctrl__DOT__req_is_pf_reg;
    vlSelf->mem_req_expert = vlSelf->expert_cache_ctrl__DOT__req_expert_reg;
    vlSelf->mem_req_valid = 0U;
    vlSelf->lookup_ready = 0U;
    vlSelf->expert_cache_ctrl__DOT__hit_slot = 0U;
    if ((vlSelf->expert_cache_ctrl__DOT__valids[0U] 
         & (vlSelf->expert_cache_ctrl__DOT__tags[0U] 
            == (IData)(vlSelf->lookup_expert)))) {
        vlSelf->expert_cache_ctrl__DOT__hit_slot = 0U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__valids[1U] 
         & (vlSelf->expert_cache_ctrl__DOT__tags[1U] 
            == (IData)(vlSelf->lookup_expert)))) {
        vlSelf->expert_cache_ctrl__DOT__hit_slot = 1U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__valids[2U] 
         & (vlSelf->expert_cache_ctrl__DOT__tags[2U] 
            == (IData)(vlSelf->lookup_expert)))) {
        vlSelf->expert_cache_ctrl__DOT__hit_slot = 2U;
    }
    vlSelf->expert_cache_ctrl__DOT__lru_slot = 0U;
    if ((vlSelf->expert_cache_ctrl__DOT__age[0U] > 
         vlSelf->expert_cache_ctrl__DOT__age[0U])) {
        vlSelf->expert_cache_ctrl__DOT__lru_slot = 0U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__age[1U] > 
         vlSelf->expert_cache_ctrl__DOT__age[vlSelf->expert_cache_ctrl__DOT__lru_slot])) {
        vlSelf->expert_cache_ctrl__DOT__lru_slot = 1U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__age[2U] > 
         vlSelf->expert_cache_ctrl__DOT__age[vlSelf->expert_cache_ctrl__DOT__lru_slot])) {
        vlSelf->expert_cache_ctrl__DOT__lru_slot = 2U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__age[3U] > 
         vlSelf->expert_cache_ctrl__DOT__age[vlSelf->expert_cache_ctrl__DOT__lru_slot])) {
        vlSelf->expert_cache_ctrl__DOT__lru_slot = 3U;
    }
    vlSelf->expert_cache_ctrl__DOT__hit_comb = 0U;
    if ((vlSelf->expert_cache_ctrl__DOT__valids[0U] 
         & (vlSelf->expert_cache_ctrl__DOT__tags[0U] 
            == (IData)(vlSelf->lookup_expert)))) {
        vlSelf->expert_cache_ctrl__DOT__hit_comb = 1U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__valids[1U] 
         & (vlSelf->expert_cache_ctrl__DOT__tags[1U] 
            == (IData)(vlSelf->lookup_expert)))) {
        vlSelf->expert_cache_ctrl__DOT__hit_comb = 1U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__valids[2U] 
         & (vlSelf->expert_cache_ctrl__DOT__tags[2U] 
            == (IData)(vlSelf->lookup_expert)))) {
        vlSelf->expert_cache_ctrl__DOT__hit_comb = 1U;
    }
    if ((vlSelf->expert_cache_ctrl__DOT__valids[3U] 
         & (vlSelf->expert_cache_ctrl__DOT__tags[3U] 
            == (IData)(vlSelf->lookup_expert)))) {
        vlSelf->expert_cache_ctrl__DOT__hit_slot = 3U;
        vlSelf->expert_cache_ctrl__DOT__hit_comb = 1U;
    }
    vlSelf->lookup_hit = 0U;
    if ((1U & (~ ((IData)(vlSelf->expert_cache_ctrl__DOT__state_reg) 
                  >> 2U)))) {
        if ((2U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
            if ((1U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
                vlSelf->mem_req_valid = 1U;
            }
        } else if ((1U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
            vlSelf->mem_req_valid = 1U;
        }
        if ((1U & (~ ((IData)(vlSelf->expert_cache_ctrl__DOT__state_reg) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg)))) {
                vlSelf->lookup_ready = 1U;
                if ((1U & (~ (IData)(vlSelf->lookup_valid)))) {
                    if (vlSelf->prefetch_valid) {
                        vlSelf->expert_cache_ctrl__DOT__unnamedblk6__DOT__i = 4U;
                    }
                }
                if (vlSelf->lookup_valid) {
                    vlSelf->lookup_hit = vlSelf->expert_cache_ctrl__DOT__hit_comb;
                }
            }
        }
    }
    vlSelf->expert_cache_ctrl__DOT__state_next = vlSelf->expert_cache_ctrl__DOT__state_reg;
    vlSelf->expert_cache_ctrl__DOT__req_expert_next 
        = vlSelf->expert_cache_ctrl__DOT__req_expert_reg;
    vlSelf->expert_cache_ctrl__DOT__req_is_pf_next 
        = vlSelf->expert_cache_ctrl__DOT__req_is_pf_reg;
    vlSelf->expert_cache_ctrl__DOT__alloc_slot_next 
        = vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg;
    vlSelf->expert_cache_ctrl__DOT__pf_hit = 0U;
    if ((4U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
        if ((2U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
            vlSelf->expert_cache_ctrl__DOT__state_next = 0U;
        } else if ((1U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
            vlSelf->expert_cache_ctrl__DOT__state_next = 0U;
        } else if (((IData)(vlSelf->mem_fill_valid) 
                    & ((IData)(vlSelf->mem_fill_expert) 
                       == (IData)(vlSelf->expert_cache_ctrl__DOT__req_expert_reg)))) {
            vlSelf->expert_cache_ctrl__DOT__state_next = 0U;
        }
    } else if ((2U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
        if ((1U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
            if (vlSelf->mem_req_ready) {
                vlSelf->expert_cache_ctrl__DOT__state_next = 4U;
            }
        } else if (((IData)(vlSelf->mem_fill_valid) 
                    & ((IData)(vlSelf->mem_fill_expert) 
                       == (IData)(vlSelf->expert_cache_ctrl__DOT__req_expert_reg)))) {
            vlSelf->expert_cache_ctrl__DOT__state_next = 0U;
        }
    } else if ((1U & (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg))) {
        if (vlSelf->mem_req_ready) {
            vlSelf->expert_cache_ctrl__DOT__state_next = 2U;
        }
    } else if (vlSelf->lookup_valid) {
        if ((1U & (~ (IData)(vlSelf->expert_cache_ctrl__DOT__hit_comb)))) {
            vlSelf->expert_cache_ctrl__DOT__req_expert_next 
                = vlSelf->lookup_expert;
            vlSelf->expert_cache_ctrl__DOT__req_is_pf_next = 0U;
            vlSelf->expert_cache_ctrl__DOT__alloc_slot_next 
                = vlSelf->expert_cache_ctrl__DOT__lru_slot;
            vlSelf->expert_cache_ctrl__DOT__state_next = 1U;
        }
    } else if (vlSelf->prefetch_valid) {
        vlSelf->expert_cache_ctrl__DOT__pf_hit = 0U;
        if ((vlSelf->expert_cache_ctrl__DOT__valids
             [0U] & (vlSelf->expert_cache_ctrl__DOT__tags
                     [0U] == (IData)(vlSelf->prefetch_expert)))) {
            vlSelf->expert_cache_ctrl__DOT__pf_hit = 1U;
        }
        if ((vlSelf->expert_cache_ctrl__DOT__valids
             [1U] & (vlSelf->expert_cache_ctrl__DOT__tags
                     [1U] == (IData)(vlSelf->prefetch_expert)))) {
            vlSelf->expert_cache_ctrl__DOT__pf_hit = 1U;
        }
        if ((vlSelf->expert_cache_ctrl__DOT__valids
             [2U] & (vlSelf->expert_cache_ctrl__DOT__tags
                     [2U] == (IData)(vlSelf->prefetch_expert)))) {
            vlSelf->expert_cache_ctrl__DOT__pf_hit = 1U;
        }
        if ((vlSelf->expert_cache_ctrl__DOT__valids
             [3U] & (vlSelf->expert_cache_ctrl__DOT__tags
                     [3U] == (IData)(vlSelf->prefetch_expert)))) {
            vlSelf->expert_cache_ctrl__DOT__pf_hit = 1U;
        }
        if ((1U & (~ (IData)(vlSelf->expert_cache_ctrl__DOT__pf_hit)))) {
            vlSelf->expert_cache_ctrl__DOT__req_expert_next 
                = vlSelf->prefetch_expert;
            vlSelf->expert_cache_ctrl__DOT__req_is_pf_next = 1U;
            vlSelf->expert_cache_ctrl__DOT__alloc_slot_next 
                = vlSelf->expert_cache_ctrl__DOT__lru_slot;
            vlSelf->expert_cache_ctrl__DOT__state_next = 3U;
        }
    }
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___eval_stl(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vexpert_cache_ctrl___024root___stl_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[2U] = 1U;
        vlSelf->__Vm_traceActivity[1U] = 1U;
        vlSelf->__Vm_traceActivity[0U] = 1U;
    }
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___eval_triggers__stl(Vexpert_cache_ctrl___024root* vlSelf);

VL_ATTR_COLD bool Vexpert_cache_ctrl___024root___eval_phase__stl(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vexpert_cache_ctrl___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vexpert_cache_ctrl___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__ico(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___dump_triggers__ico\n"); );
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
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__act(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___dump_triggers__act\n"); );
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
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__nba(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vexpert_cache_ctrl___024root___ctor_var_reset(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst_n = VL_RAND_RESET_I(1);
    vlSelf->lookup_valid = VL_RAND_RESET_I(1);
    vlSelf->lookup_expert = VL_RAND_RESET_I(3);
    vlSelf->lookup_hit = VL_RAND_RESET_I(1);
    vlSelf->lookup_ready = VL_RAND_RESET_I(1);
    vlSelf->prefetch_valid = VL_RAND_RESET_I(1);
    vlSelf->prefetch_expert = VL_RAND_RESET_I(3);
    vlSelf->mem_req_valid = VL_RAND_RESET_I(1);
    vlSelf->mem_req_expert = VL_RAND_RESET_I(3);
    vlSelf->mem_req_is_pf = VL_RAND_RESET_I(1);
    vlSelf->mem_req_ready = VL_RAND_RESET_I(1);
    vlSelf->mem_fill_valid = VL_RAND_RESET_I(1);
    vlSelf->mem_fill_expert = VL_RAND_RESET_I(3);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->expert_cache_ctrl__DOT__tags[__Vi0] = VL_RAND_RESET_I(3);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->expert_cache_ctrl__DOT__valids[__Vi0] = VL_RAND_RESET_I(1);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->expert_cache_ctrl__DOT__age[__Vi0] = VL_RAND_RESET_I(2);
    }
    vlSelf->expert_cache_ctrl__DOT__state_reg = VL_RAND_RESET_I(3);
    vlSelf->expert_cache_ctrl__DOT__state_next = VL_RAND_RESET_I(3);
    vlSelf->expert_cache_ctrl__DOT__req_expert_reg = VL_RAND_RESET_I(3);
    vlSelf->expert_cache_ctrl__DOT__req_expert_next = VL_RAND_RESET_I(3);
    vlSelf->expert_cache_ctrl__DOT__req_is_pf_reg = VL_RAND_RESET_I(1);
    vlSelf->expert_cache_ctrl__DOT__req_is_pf_next = VL_RAND_RESET_I(1);
    vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg = VL_RAND_RESET_I(2);
    vlSelf->expert_cache_ctrl__DOT__alloc_slot_next = VL_RAND_RESET_I(2);
    vlSelf->expert_cache_ctrl__DOT__hit_comb = VL_RAND_RESET_I(1);
    vlSelf->expert_cache_ctrl__DOT__hit_slot = VL_RAND_RESET_I(2);
    vlSelf->expert_cache_ctrl__DOT__lru_slot = VL_RAND_RESET_I(2);
    vlSelf->expert_cache_ctrl__DOT__pf_hit = VL_RAND_RESET_I(1);
    vlSelf->expert_cache_ctrl__DOT__unnamedblk4__DOT__i = 0;
    vlSelf->expert_cache_ctrl__DOT__unnamedblk5__DOT__i = 0;
    vlSelf->expert_cache_ctrl__DOT__unnamedblk3__DOT__i = 0;
    vlSelf->expert_cache_ctrl__DOT__unnamedblk6__DOT__i = 0;
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 3; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
