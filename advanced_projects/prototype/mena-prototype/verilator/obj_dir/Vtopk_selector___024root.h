// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtopk_selector.h for the primary calling header

#ifndef VERILATED_VTOPK_SELECTOR___024ROOT_H_
#define VERILATED_VTOPK_SELECTOR___024ROOT_H_  // guard

#include "verilated.h"


class Vtopk_selector__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtopk_selector___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_OUT8(topk_idx,5,0);
    CData/*2:0*/ topk_selector__DOT__max1_idx;
    CData/*2:0*/ topk_selector__DOT__max2_idx;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __VactContinue;
    SData/*15:0*/ topk_selector__DOT__max1_val;
    SData/*15:0*/ topk_selector__DOT__max2_val;
    VL_INW(scores_in,127,0,4);
    VL_OUT(topk_score,31,0);
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<SData/*15:0*/, 8> topk_selector__DOT__scores;
    VlUnpacked<CData/*0:0*/, 2> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<0> __VactTriggered;
    VlTriggerVec<0> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtopk_selector__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtopk_selector___024root(Vtopk_selector__Syms* symsp, const char* v__name);
    ~Vtopk_selector___024root();
    VL_UNCOPYABLE(Vtopk_selector___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
