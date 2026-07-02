// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtoken_dispatcher.h for the primary calling header

#ifndef VERILATED_VTOKEN_DISPATCHER___024ROOT_H_
#define VERILATED_VTOKEN_DISPATCHER___024ROOT_H_  // guard

#include "verilated.h"


class Vtoken_dispatcher__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtoken_dispatcher___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(wr_en,0,0);
    VL_IN8(wr_token_id,4,0);
    VL_IN8(wr_expert_ids,5,0);
    VL_IN8(start_dispatch,0,0);
    VL_OUT8(dispatch_valid,0,0);
    VL_OUT8(dispatch_expert_id,2,0);
    VL_OUT8(dispatch_token_id,4,0);
    VL_OUT8(dispatch_done,0,0);
    CData/*1:0*/ token_dispatcher__DOT__state;
    CData/*1:0*/ token_dispatcher__DOT__next_state;
    CData/*2:0*/ token_dispatcher__DOT__curr_expert;
    CData/*2:0*/ token_dispatcher__DOT__next_expert;
    CData/*5:0*/ token_dispatcher__DOT__curr_token;
    CData/*5:0*/ token_dispatcher__DOT__next_token;
    CData/*0:0*/ token_dispatcher__DOT__dispatch_match;
    CData/*5:0*/ token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__count;
    CData/*0:0*/ token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__match_count;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ token_dispatcher__DOT__unnamedblk4__DOT__k;
    IData/*31:0*/ token_dispatcher__DOT__unnamedblk2__DOT__t;
    IData/*31:0*/ token_dispatcher__DOT__unnamedblk2__DOT__unnamedblk3__DOT__k;
    IData/*31:0*/ token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__unnamedblk9__DOT__k;
    IData/*31:0*/ token_dispatcher__DOT__unnamedblk10__DOT__k;
    IData/*31:0*/ __VactIterCount;
    VL_OUT64(expert_count,47,0);
    VlUnpacked<VlUnpacked<CData/*2:0*/, 2>, 32> token_dispatcher__DOT__token_experts;
    VlUnpacked<CData/*0:0*/, 32> token_dispatcher__DOT__token_loaded;
    VlUnpacked<CData/*2:0*/, 2> token_dispatcher__DOT__wr_expert_decoded;
    VlUnpacked<CData/*0:0*/, 2> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtoken_dispatcher__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtoken_dispatcher___024root(Vtoken_dispatcher__Syms* symsp, const char* v__name);
    ~Vtoken_dispatcher___024root();
    VL_UNCOPYABLE(Vtoken_dispatcher___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
