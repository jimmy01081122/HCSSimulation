// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vexpert_cache_ctrl.h for the primary calling header

#include "Vexpert_cache_ctrl__pch.h"
#include "Vexpert_cache_ctrl___024root.h"

VL_INLINE_OPT void Vexpert_cache_ctrl___024root___ico_sequent__TOP__0(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___ico_sequent__TOP__0\n"); );
    // Body
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
        if ((1U & (~ ((IData)(vlSelf->expert_cache_ctrl__DOT__state_reg) 
                      >> 1U)))) {
            if ((1U & (~ (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg)))) {
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

void Vexpert_cache_ctrl___024root___eval_ico(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_ico\n"); );
    // Body
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        Vexpert_cache_ctrl___024root___ico_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
}

void Vexpert_cache_ctrl___024root___eval_triggers__ico(Vexpert_cache_ctrl___024root* vlSelf);

bool Vexpert_cache_ctrl___024root___eval_phase__ico(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_phase__ico\n"); );
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vexpert_cache_ctrl___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelf->__VicoTriggered.any();
    if (__VicoExecute) {
        Vexpert_cache_ctrl___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vexpert_cache_ctrl___024root___eval_act(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vexpert_cache_ctrl___024root___nba_sequent__TOP__0(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v0;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v0 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v1;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v1 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v1;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v1 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v2;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v2 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v3;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v3 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v3;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v3 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v4;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v4 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v5;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v5 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v5;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v5 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v6;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v6 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v7;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v7 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v7;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v7 = 0;
    CData/*1:0*/ __Vdlyvdim0__expert_cache_ctrl__DOT__tags__v0;
    __Vdlyvdim0__expert_cache_ctrl__DOT__tags__v0 = 0;
    CData/*2:0*/ __Vdlyvval__expert_cache_ctrl__DOT__tags__v0;
    __Vdlyvval__expert_cache_ctrl__DOT__tags__v0 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__tags__v0;
    __Vdlyvset__expert_cache_ctrl__DOT__tags__v0 = 0;
    CData/*1:0*/ __Vdlyvdim0__expert_cache_ctrl__DOT__valids__v0;
    __Vdlyvdim0__expert_cache_ctrl__DOT__valids__v0 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v8;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v8 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v9;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v9 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v9;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v9 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v10;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v10 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v11;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v11 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v11;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v11 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v12;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v12 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v13;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v13 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v13;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v13 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v14;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v14 = 0;
    CData/*1:0*/ __Vdlyvval__expert_cache_ctrl__DOT__age__v15;
    __Vdlyvval__expert_cache_ctrl__DOT__age__v15 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__age__v15;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v15 = 0;
    CData/*0:0*/ __Vdlyvset__expert_cache_ctrl__DOT__tags__v1;
    __Vdlyvset__expert_cache_ctrl__DOT__tags__v1 = 0;
    // Body
    __Vdlyvset__expert_cache_ctrl__DOT__age__v0 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v1 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v2 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v3 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v4 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v5 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v6 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v7 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v8 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v9 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v10 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v11 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v12 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v13 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v14 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__age__v15 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__tags__v0 = 0U;
    __Vdlyvset__expert_cache_ctrl__DOT__tags__v1 = 0U;
    if ((1U & (~ (IData)(vlSelf->rst_n)))) {
        vlSelf->expert_cache_ctrl__DOT__unnamedblk3__DOT__i = 4U;
    }
    if (vlSelf->rst_n) {
        if (vlSelf->mem_fill_valid) {
            vlSelf->expert_cache_ctrl__DOT__unnamedblk5__DOT__i = 4U;
            __Vdlyvval__expert_cache_ctrl__DOT__tags__v0 
                = vlSelf->mem_fill_expert;
            __Vdlyvset__expert_cache_ctrl__DOT__tags__v0 = 1U;
            __Vdlyvdim0__expert_cache_ctrl__DOT__tags__v0 
                = vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg;
            __Vdlyvdim0__expert_cache_ctrl__DOT__valids__v0 
                = vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg;
            if ((0U == (IData)(vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v8 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [0U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v9 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [0U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v9 = 1U;
            }
            if ((1U == (IData)(vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v10 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [1U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v11 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [1U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v11 = 1U;
            }
            if ((2U == (IData)(vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v12 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [2U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v13 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [2U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v13 = 1U;
            }
            if ((3U == (IData)(vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v14 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [3U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v15 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [3U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v15 = 1U;
            }
        }
        if ((((0U == (IData)(vlSelf->expert_cache_ctrl__DOT__state_reg)) 
              & (IData)(vlSelf->lookup_valid)) & (IData)(vlSelf->expert_cache_ctrl__DOT__hit_comb))) {
            if ((0U == (IData)(vlSelf->expert_cache_ctrl__DOT__hit_slot))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v0 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [0U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__hit_slot])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v1 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [0U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v1 = 1U;
            }
            vlSelf->expert_cache_ctrl__DOT__unnamedblk4__DOT__i = 4U;
            if ((1U == (IData)(vlSelf->expert_cache_ctrl__DOT__hit_slot))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v2 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [1U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__hit_slot])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v3 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [1U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v3 = 1U;
            }
            if ((2U == (IData)(vlSelf->expert_cache_ctrl__DOT__hit_slot))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v4 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [2U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__hit_slot])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v5 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [2U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v5 = 1U;
            }
            if ((3U == (IData)(vlSelf->expert_cache_ctrl__DOT__hit_slot))) {
                __Vdlyvset__expert_cache_ctrl__DOT__age__v6 = 1U;
            } else if ((vlSelf->expert_cache_ctrl__DOT__age
                        [3U] < vlSelf->expert_cache_ctrl__DOT__age
                        [vlSelf->expert_cache_ctrl__DOT__hit_slot])) {
                __Vdlyvval__expert_cache_ctrl__DOT__age__v7 
                    = (3U & ((IData)(1U) + vlSelf->expert_cache_ctrl__DOT__age
                             [3U]));
                __Vdlyvset__expert_cache_ctrl__DOT__age__v7 = 1U;
            }
        }
        vlSelf->expert_cache_ctrl__DOT__req_expert_reg 
            = vlSelf->expert_cache_ctrl__DOT__req_expert_next;
        vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg 
            = vlSelf->expert_cache_ctrl__DOT__alloc_slot_next;
        vlSelf->expert_cache_ctrl__DOT__state_reg = vlSelf->expert_cache_ctrl__DOT__state_next;
    } else {
        __Vdlyvset__expert_cache_ctrl__DOT__tags__v1 = 1U;
        vlSelf->expert_cache_ctrl__DOT__req_expert_reg = 0U;
        vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg = 0U;
        vlSelf->expert_cache_ctrl__DOT__state_reg = 0U;
    }
    vlSelf->expert_cache_ctrl__DOT__req_is_pf_reg = 
        ((IData)(vlSelf->rst_n) && (IData)(vlSelf->expert_cache_ctrl__DOT__req_is_pf_next));
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v0) {
        vlSelf->expert_cache_ctrl__DOT__age[0U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v1) {
        vlSelf->expert_cache_ctrl__DOT__age[0U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v1;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v2) {
        vlSelf->expert_cache_ctrl__DOT__age[1U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v3) {
        vlSelf->expert_cache_ctrl__DOT__age[1U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v3;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v4) {
        vlSelf->expert_cache_ctrl__DOT__age[2U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v5) {
        vlSelf->expert_cache_ctrl__DOT__age[2U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v5;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v6) {
        vlSelf->expert_cache_ctrl__DOT__age[3U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v7) {
        vlSelf->expert_cache_ctrl__DOT__age[3U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v7;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v8) {
        vlSelf->expert_cache_ctrl__DOT__age[0U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v9) {
        vlSelf->expert_cache_ctrl__DOT__age[0U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v9;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v10) {
        vlSelf->expert_cache_ctrl__DOT__age[1U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v11) {
        vlSelf->expert_cache_ctrl__DOT__age[1U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v11;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v12) {
        vlSelf->expert_cache_ctrl__DOT__age[2U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v13) {
        vlSelf->expert_cache_ctrl__DOT__age[2U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v13;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v14) {
        vlSelf->expert_cache_ctrl__DOT__age[3U] = 0U;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__age__v15) {
        vlSelf->expert_cache_ctrl__DOT__age[3U] = __Vdlyvval__expert_cache_ctrl__DOT__age__v15;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__tags__v0) {
        vlSelf->expert_cache_ctrl__DOT__valids[__Vdlyvdim0__expert_cache_ctrl__DOT__valids__v0] = 1U;
        vlSelf->expert_cache_ctrl__DOT__tags[__Vdlyvdim0__expert_cache_ctrl__DOT__tags__v0] 
            = __Vdlyvval__expert_cache_ctrl__DOT__tags__v0;
    }
    if (__Vdlyvset__expert_cache_ctrl__DOT__tags__v1) {
        vlSelf->expert_cache_ctrl__DOT__age[0U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__age[1U] = 1U;
        vlSelf->expert_cache_ctrl__DOT__age[2U] = 2U;
        vlSelf->expert_cache_ctrl__DOT__age[3U] = 3U;
        vlSelf->expert_cache_ctrl__DOT__valids[0U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__valids[1U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__valids[2U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__valids[3U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__tags[0U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__tags[1U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__tags[2U] = 0U;
        vlSelf->expert_cache_ctrl__DOT__tags[3U] = 0U;
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
    vlSelf->mem_req_is_pf = vlSelf->expert_cache_ctrl__DOT__req_is_pf_reg;
    vlSelf->mem_req_expert = vlSelf->expert_cache_ctrl__DOT__req_expert_reg;
    vlSelf->mem_req_valid = 0U;
    vlSelf->lookup_ready = 0U;
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
                if (vlSelf->lookup_valid) {
                    vlSelf->lookup_hit = vlSelf->expert_cache_ctrl__DOT__hit_comb;
                }
                if ((1U & (~ (IData)(vlSelf->lookup_valid)))) {
                    if (vlSelf->prefetch_valid) {
                        vlSelf->expert_cache_ctrl__DOT__unnamedblk6__DOT__i = 4U;
                    }
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

void Vexpert_cache_ctrl___024root___eval_nba(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vexpert_cache_ctrl___024root___nba_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[2U] = 1U;
    }
}

void Vexpert_cache_ctrl___024root___eval_triggers__act(Vexpert_cache_ctrl___024root* vlSelf);

bool Vexpert_cache_ctrl___024root___eval_phase__act(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vexpert_cache_ctrl___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vexpert_cache_ctrl___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vexpert_cache_ctrl___024root___eval_phase__nba(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vexpert_cache_ctrl___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__ico(Vexpert_cache_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__nba(Vexpert_cache_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vexpert_cache_ctrl___024root___dump_triggers__act(Vexpert_cache_ctrl___024root* vlSelf);
#endif  // VL_DEBUG

void Vexpert_cache_ctrl___024root___eval(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval\n"); );
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
            Vexpert_cache_ctrl___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("../rtl/expert_cache_ctrl.sv", 5, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vexpert_cache_ctrl___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelf->__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vexpert_cache_ctrl___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("../rtl/expert_cache_ctrl.sv", 5, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vexpert_cache_ctrl___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("../rtl/expert_cache_ctrl.sv", 5, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vexpert_cache_ctrl___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vexpert_cache_ctrl___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vexpert_cache_ctrl___024root___eval_debug_assertions(Vexpert_cache_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst_n & 0xfeU))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY((vlSelf->lookup_valid & 0xfeU))) {
        Verilated::overWidthError("lookup_valid");}
    if (VL_UNLIKELY((vlSelf->lookup_expert & 0xf8U))) {
        Verilated::overWidthError("lookup_expert");}
    if (VL_UNLIKELY((vlSelf->prefetch_valid & 0xfeU))) {
        Verilated::overWidthError("prefetch_valid");}
    if (VL_UNLIKELY((vlSelf->prefetch_expert & 0xf8U))) {
        Verilated::overWidthError("prefetch_expert");}
    if (VL_UNLIKELY((vlSelf->mem_req_ready & 0xfeU))) {
        Verilated::overWidthError("mem_req_ready");}
    if (VL_UNLIKELY((vlSelf->mem_fill_valid & 0xfeU))) {
        Verilated::overWidthError("mem_fill_valid");}
    if (VL_UNLIKELY((vlSelf->mem_fill_expert & 0xf8U))) {
        Verilated::overWidthError("mem_fill_expert");}
}
#endif  // VL_DEBUG
