// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vweight_buffer_ctrl.h for the primary calling header

#include "Vweight_buffer_ctrl__pch.h"
#include "Vweight_buffer_ctrl___024root.h"

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___eval_static(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___eval_initial(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = vlSelf->rst_n;
}

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___eval_final(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__stl(Vweight_buffer_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vweight_buffer_ctrl___024root___eval_phase__stl(Vweight_buffer_ctrl___024root* vlSelf);

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___eval_settle(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_settle\n"); );
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
            Vweight_buffer_ctrl___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("../rtl/weight_buffer_ctrl.sv", 5, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vweight_buffer_ctrl___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__stl(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___stl_sequent__TOP__0(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___stl_sequent__TOP__0\n"); );
    // Body
    vlSelf->swap_event = 0U;
    vlSelf->active_sel = vlSelf->weight_buffer_ctrl__DOT__active_sel_reg;
    vlSelf->dma_sel = (1U & (~ (IData)(vlSelf->weight_buffer_ctrl__DOT__active_sel_reg)));
    vlSelf->shadow_valid = vlSelf->weight_buffer_ctrl__DOT__shadow_valid_reg;
    vlSelf->active_valid = vlSelf->weight_buffer_ctrl__DOT__active_valid_reg;
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

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___eval_stl(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vweight_buffer_ctrl___024root___stl_sequent__TOP__0(vlSelf);
        vlSelf->__Vm_traceActivity[1U] = 1U;
        vlSelf->__Vm_traceActivity[0U] = 1U;
    }
}

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___eval_triggers__stl(Vweight_buffer_ctrl___024root* vlSelf);

VL_ATTR_COLD bool Vweight_buffer_ctrl___024root___eval_phase__stl(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vweight_buffer_ctrl___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vweight_buffer_ctrl___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__ico(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___dump_triggers__ico\n"); );
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
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__act(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___dump_triggers__act\n"); );
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
VL_ATTR_COLD void Vweight_buffer_ctrl___024root___dump_triggers__nba(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk or negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vweight_buffer_ctrl___024root___ctor_var_reset(Vweight_buffer_ctrl___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vweight_buffer_ctrl___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst_n = VL_RAND_RESET_I(1);
    vlSelf->dma_start = VL_RAND_RESET_I(1);
    vlSelf->shadow_done = VL_RAND_RESET_I(1);
    vlSelf->exec_start = VL_RAND_RESET_I(1);
    vlSelf->exec_done = VL_RAND_RESET_I(1);
    vlSelf->active_sel = VL_RAND_RESET_I(1);
    vlSelf->dma_sel = VL_RAND_RESET_I(1);
    vlSelf->stall = VL_RAND_RESET_I(1);
    vlSelf->swap_event = VL_RAND_RESET_I(1);
    vlSelf->shadow_valid = VL_RAND_RESET_I(1);
    vlSelf->active_valid = VL_RAND_RESET_I(1);
    vlSelf->weight_buffer_ctrl__DOT__state_reg = VL_RAND_RESET_I(2);
    vlSelf->weight_buffer_ctrl__DOT__state_next = VL_RAND_RESET_I(2);
    vlSelf->weight_buffer_ctrl__DOT__active_sel_reg = VL_RAND_RESET_I(1);
    vlSelf->weight_buffer_ctrl__DOT__active_sel_next = VL_RAND_RESET_I(1);
    vlSelf->weight_buffer_ctrl__DOT__shadow_valid_reg = VL_RAND_RESET_I(1);
    vlSelf->weight_buffer_ctrl__DOT__shadow_valid_next = VL_RAND_RESET_I(1);
    vlSelf->weight_buffer_ctrl__DOT__active_valid_reg = VL_RAND_RESET_I(1);
    vlSelf->weight_buffer_ctrl__DOT__active_valid_next = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
