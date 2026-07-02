// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vweight_buffer_ctrl.h for the primary calling header

#ifndef VERILATED_VWEIGHT_BUFFER_CTRL___024ROOT_H_
#define VERILATED_VWEIGHT_BUFFER_CTRL___024ROOT_H_  // guard

#include "verilated.h"


class Vweight_buffer_ctrl__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vweight_buffer_ctrl___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(dma_start,0,0);
    VL_IN8(shadow_done,0,0);
    VL_IN8(exec_start,0,0);
    VL_IN8(exec_done,0,0);
    VL_OUT8(active_sel,0,0);
    VL_OUT8(dma_sel,0,0);
    VL_OUT8(stall,0,0);
    VL_OUT8(swap_event,0,0);
    VL_OUT8(shadow_valid,0,0);
    VL_OUT8(active_valid,0,0);
    CData/*1:0*/ weight_buffer_ctrl__DOT__state_reg;
    CData/*1:0*/ weight_buffer_ctrl__DOT__state_next;
    CData/*0:0*/ weight_buffer_ctrl__DOT__active_sel_reg;
    CData/*0:0*/ weight_buffer_ctrl__DOT__active_sel_next;
    CData/*0:0*/ weight_buffer_ctrl__DOT__shadow_valid_reg;
    CData/*0:0*/ weight_buffer_ctrl__DOT__shadow_valid_next;
    CData/*0:0*/ weight_buffer_ctrl__DOT__active_valid_reg;
    CData/*0:0*/ weight_buffer_ctrl__DOT__active_valid_next;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<CData/*0:0*/, 2> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vweight_buffer_ctrl__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vweight_buffer_ctrl___024root(Vweight_buffer_ctrl__Syms* symsp, const char* v__name);
    ~Vweight_buffer_ctrl___024root();
    VL_UNCOPYABLE(Vweight_buffer_ctrl___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
