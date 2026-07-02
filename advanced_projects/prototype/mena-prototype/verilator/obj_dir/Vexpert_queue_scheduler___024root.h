// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vexpert_queue_scheduler.h for the primary calling header

#ifndef VERILATED_VEXPERT_QUEUE_SCHEDULER___024ROOT_H_
#define VERILATED_VEXPERT_QUEUE_SCHEDULER___024ROOT_H_  // guard

#include "verilated.h"


class Vexpert_queue_scheduler__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vexpert_queue_scheduler___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst_n,0,0);
    VL_IN8(valid_in,0,0);
    VL_OUT8(next_expert,2,0);
    VL_OUT8(sched_valid,0,0);
    CData/*2:0*/ expert_queue_scheduler__DOT__rr_ptr;
    CData/*0:0*/ expert_queue_scheduler__DOT__any_nonempty;
    CData/*2:0*/ expert_queue_scheduler__DOT__sel_rr;
    CData/*0:0*/ expert_queue_scheduler__DOT__rr_found;
    CData/*2:0*/ expert_queue_scheduler__DOT__sel_lqf;
    CData/*5:0*/ expert_queue_scheduler__DOT__lqf_best;
    CData/*2:0*/ expert_queue_scheduler__DOT__sel_hef;
    CData/*0:0*/ expert_queue_scheduler__DOT__hef_found;
    CData/*2:0*/ expert_queue_scheduler__DOT__sel_age;
    CData/*0:0*/ expert_queue_scheduler__DOT__age_found;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __VicoFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
    CData/*0:0*/ __VactContinue;
    SData/*15:0*/ expert_queue_scheduler__DOT__hef_best;
    SData/*15:0*/ expert_queue_scheduler__DOT__age_best;
    IData/*31:0*/ expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i;
    IData/*31:0*/ expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i;
    IData/*31:0*/ expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i;
    IData/*31:0*/ __VactIterCount;
    VL_IN64(expert_counts,47,0);
    VlUnpacked<CData/*5:0*/, 8> expert_queue_scheduler__DOT__cnt;
    VlUnpacked<SData/*15:0*/, 8> expert_queue_scheduler__DOT__hot_score;
    VlUnpacked<SData/*15:0*/, 8> expert_queue_scheduler__DOT__age_counter;
    VlUnpacked<CData/*0:0*/, 3> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vexpert_queue_scheduler__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vexpert_queue_scheduler___024root(Vexpert_queue_scheduler__Syms* symsp, const char* v__name);
    ~Vexpert_queue_scheduler___024root();
    VL_UNCOPYABLE(Vexpert_queue_scheduler___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
