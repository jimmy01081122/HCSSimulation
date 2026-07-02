// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vweight_buffer_ctrl.h for the primary calling header

#include "Vweight_buffer_ctrl__pch.h"
#include "Vweight_buffer_ctrl___024root.h"

VL_INLINE_OPT void Vweight_buffer_ctrl___024root___ico_sequent__TOP__0(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->weight_buffer_ctrl__DOT__state_next = vlSelf->weight_buffer_ctrl__DOT__state_reg;
    vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next 
        = vlSelf->weight_buffer_ctrl__DOT__shadow_valid_reg;
    vlSelf->weight_buffer_ctrl__DOT__active_valid_next 
        = vlSelf->weight_buffer_ctrl__DOT__active_valid_reg;
    vlSelf->stall = 0U;
    if (vlSelf->shadow_done) {
        vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next = 1U;
    }
    if (vlSelf->exec_done) {
        vlSelf->weight_buffer_ctrl__DOT__active_valid_next = 0U;
    }
    if ((2U & (IData)(vlSelf->weight_buffer_ctrl__DOT__state_reg))) {
        if ((1U & (IData)(vlSelf->weight_buffer_ctrl__DOT__state_reg))) {
            vlSelf->weight_buffer_ctrl__DOT__active_valid_next = 1U;
            vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next = 0U;
            vlSelf->weight_buffer_ctrl__DOT__state_next = 2U;
        } else {
            if (((IData)(vlSelf->exec_start) & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)))) {
                vlSelf->stall = 1U;
            }
            if ((1U & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)))) {
                vlSelf->weight_buffer_ctrl__DOT__state_next 
                    = ((IData)(vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next)
                        ? 3U : 1U);
            }
        }
    } else if ((1U & (IData)(vlSelf->weight_buffer_ctrl__DOT__state_reg))) {
        if (((IData)(vlSelf->exec_start) & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)))) {
            vlSelf->stall = 1U;
        }
        if (((~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)) 
             & (IData)(vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next))) {
            vlSelf->weight_buffer_ctrl__DOT__state_next = 3U;
        }
    } else if (vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next) {
        vlSelf->weight_buffer_ctrl__DOT__state_next = 3U;
    } else if (vlSelf->exec_start) {
        vlSelf->stall = 1U;
    }
}

void Vweight_buffer_ctrl___024root___eval_ico(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_ico\n"); );
    // Body
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        Vweight_buffer_ctrl___024root___ico_sequent__TOP__0(vlSelf);
    }
}

void Vweight_buffer_ctrl___024root___eval_triggers__ico(Vweight_buffer_ctrl___024root* vlSelf);

bool Vweight_buffer_ctrl___024root___eval_phase__ico(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_phase__ico\n"); );
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vweight_buffer_ctrl___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelf->__VicoTriggered.any();
    if (__VicoExecute) {
        Vweight_buffer_ctrl___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vweight_buffer_ctrl___024root___eval_act(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_act\n"); );
}

VL_INLINE_OPT void Vweight_buffer_ctrl___024root___nba_sequent__TOP__0(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___nba_sequent__TOP__0\n"); );
    // Body
    vlSelf->weight_buffer_ctrl__DOT__active_sel_reg 
        = ((IData)(vlSelf->rst_n) && (IData)(vlSelf->weight_buffer_ctrl__DOT__active_sel_next));
    vlSelf->weight_buffer_ctrl__DOT__active_valid_reg 
        = ((IData)(vlSelf->rst_n) && (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next));
    vlSelf->weight_buffer_ctrl__DOT__shadow_valid_reg 
        = ((IData)(vlSelf->rst_n) && (IData)(vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next));
    vlSelf->weight_buffer_ctrl__DOT__state_reg = ((IData)(vlSelf->rst_n)
                                                   ? (IData)(vlSelf->weight_buffer_ctrl__DOT__state_next)
                                                   : 0U);
    vlSelf->active_sel = vlSelf->weight_buffer_ctrl__DOT__active_sel_reg;
    vlSelf->dma_sel = (1U & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_sel_reg)));
    vlSelf->active_valid = vlSelf->weight_buffer_ctrl__DOT__active_valid_reg;
    vlSelf->shadow_valid = vlSelf->weight_buffer_ctrl__DOT__shadow_valid_reg;
    vlSelf->swap_event = 0U;
    vlSelf->weight_buffer_ctrl__DOT__active_sel_next 
        = vlSelf->weight_buffer_ctrl__DOT__active_sel_reg;
    vlSelf->weight_buffer_ctrl__DOT__state_next = vlSelf->weight_buffer_ctrl__DOT__state_reg;
    vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next 
        = vlSelf->weight_buffer_ctrl__DOT__shadow_valid_reg;
    vlSelf->weight_buffer_ctrl__DOT__active_valid_next 
        = vlSelf->weight_buffer_ctrl__DOT__active_valid_reg;
    vlSelf->stall = 0U;
    if (vlSelf->shadow_done) {
        vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next = 1U;
    }
    if (vlSelf->exec_done) {
        vlSelf->weight_buffer_ctrl__DOT__active_valid_next = 0U;
    }
    if ((2U & (IData)(vlSelf->weight_buffer_ctrl__DOT__state_reg))) {
        if ((1U & (IData)(vlSelf->weight_buffer_ctrl__DOT__state_reg))) {
            vlSelf->swap_event = 1U;
            vlSelf->weight_buffer_ctrl__DOT__active_sel_next 
                = (1U & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_sel_reg)));
            vlSelf->weight_buffer_ctrl__DOT__active_valid_next = 1U;
            vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next = 0U;
            vlSelf->weight_buffer_ctrl__DOT__state_next = 2U;
        } else {
            if (((IData)(vlSelf->exec_start) & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)))) {
                vlSelf->stall = 1U;
            }
            if ((1U & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)))) {
                vlSelf->weight_buffer_ctrl__DOT__state_next 
                    = ((IData)(vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next)
                        ? 3U : 1U);
            }
        }
    } else if ((1U & (IData)(vlSelf->weight_buffer_ctrl__DOT__state_reg))) {
        if (((IData)(vlSelf->exec_start) & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)))) {
            vlSelf->stall = 1U;
        }
        if (((~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_valid_next)) 
             & (IData)(vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next))) {
            vlSelf->weight_buffer_ctrl__DOT__state_next = 3U;
        }
    } else if (vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next) {
        vlSelf->weight_buffer_ctrl__DOT__state_next = 3U;
    } else if (vlSelf->exec_start) {
        vlSelf->stall = 1U;
    }
}

void Vweight_buffer_ctrl___024root___eval_nba(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vweight_buffer_ctrl___024root___nba_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
    }
}

void Vweight_buffer_ctrl___024root___eval_triggers__act(Vweight_buffer_ctrl___024root* vlSelf);

bool Vweight_buffer_ctrl___024root___eval_phase__act(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vweight_buffer_ctrl___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vweight_buffer_ctrl___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vweight_buffer_ctrl___024root___eval_phase__nba(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vweight_buffer_ctrl___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__ico(Vweight_buffer_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__nba(Vweight_buffer_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__act(Vweight_buffer_ctrl___024root* vlSelf);
#endif  // VL_DEBUG

void Vweight_buffer_ctrl___024root___eval(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval\n"); );
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
            Vweight_buffer_ctrl___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("../rtl/weight_buffer_ctrl.sv", 5, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vweight_buffer_ctrl___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelf->__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vweight_buffer_ctrl___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("../rtl/weight_buffer_ctrl.sv", 5, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vweight_buffer_ctrl___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("../rtl/weight_buffer_ctrl.sv", 5, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vweight_buffer_ctrl___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vweight_buffer_ctrl___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vweight_buffer_ctrl___024root___eval_debug_assertions(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst_n & 0xfeU))) {
        Verilated::overWidthError("rst_n");}
    if (VL_UNLIKELY((vlSelf->dma_start & 0xfeU))) {
        Verilated::overWidthError("dma_start");}
    if (VL_UNLIKELY((vlSelf->shadow_done & 0xfeU))) {
        Verilated::overWidthError("shadow_done");}
    if (VL_UNLIKELY((vlSelf->exec_start & 0xfeU))) {
        Verilated::overWidthError("exec_start");}
    if (VL_UNLIKELY((vlSelf->exec_done & 0xfeU))) {
        Verilated::overWidthError("exec_done");}
}
#endif  // VL_DEBUG
