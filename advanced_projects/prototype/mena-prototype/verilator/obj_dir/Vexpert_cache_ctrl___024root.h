// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vexpert_cache_ctrl.h for the primary calling header

#ifndef VERILATED_VEXPERT_CACHE_CTRL___024ROOT_H_
#define VERILATED_VEXPERT_CACHE_CTRL___024ROOT_H_  // guard

#include "verilated.h"


class Vexpert_cache_ctrl__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vexpert_cache_ctrl___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(lookup_valid,0,0);
    VL_IN8(lookup_expert,2,0);
    VL_OUT8(lookup_hit,0,0);
    VL_OUT8(lookup_ready,0,0);
    VL_IN8(prefetch_valid,0,0);
    VL_IN8(prefetch_expert,2,0);
    VL_OUT8(mem_req_valid,0,0);
    VL_OUT8(mem_req_expert,2,0);
    VL_OUT8(mem_req_is_pf,0,0);
    VL_IN8(mem_req_ready,0,0);
    VL_IN8(mem_fill_valid,0,0);
    VL_IN8(mem_fill_expert,2,0);
    CData/*2:0*/ expert_cache_ctrl__DOT__state_reg;
    CData/*2:0*/ expert_cache_ctrl__DOT__state_next;
    CData/*2:0*/ expert_cache_ctrl__DOT__req_expert_reg;
    CData/*2:0*/ expert_cache_ctrl__DOT__req_expert_next;
    CData/*0:0*/ expert_cache_ctrl__DOT__req_is_pf_reg;
    CData/*0:0*/ expert_cache_ctrl__DOT__req_is_pf_next;
    CData/*1:0*/ expert_cache_ctrl__DOT__alloc_slot_reg;
    CData/*1:0*/ expert_cache_ctrl__DOT__alloc_slot_next;
    CData/*0:0*/ expert_cache_ctrl__DOT__hit_comb;
    CData/*1:0*/ expert_cache_ctrl__DOT__hit_slot;
    CData/*1:0*/ expert_cache_ctrl__DOT__lru_slot;
    CData/*0:0*/ expert_cache_ctrl__DOT__pf_hit;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ expert_cache_ctrl__DOT__unnamedblk4__DOT__i;
    IData/*31:0*/ expert_cache_ctrl__DOT__unnamedblk5__DOT__i;
    IData/*31:0*/ expert_cache_ctrl__DOT__unnamedblk3__DOT__i;
    IData/*31:0*/ expert_cache_ctrl__DOT__unnamedblk6__DOT__i;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<CData/*2:0*/, 4> expert_cache_ctrl__DOT__tags;
    VlUnpacked<CData/*0:0*/, 4> expert_cache_ctrl__DOT__valids;
    VlUnpacked<CData/*1:0*/, 4> expert_cache_ctrl__DOT__age;
    VlUnpacked<CData/*0:0*/, 3> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vexpert_cache_ctrl__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vexpert_cache_ctrl___024root(Vexpert_cache_ctrl__Syms* symsp, const char* v__name);
    ~Vexpert_cache_ctrl___024root();
    VL_UNCOPYABLE(Vexpert_cache_ctrl___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
