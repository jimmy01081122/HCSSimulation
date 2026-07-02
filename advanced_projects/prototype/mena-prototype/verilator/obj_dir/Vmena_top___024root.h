// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vmena_top.h for the primary calling header

#ifndef VERILATED_VMENA_TOP___024ROOT_H_
#define VERILATED_VMENA_TOP___024ROOT_H_  // guard

#include "verilated.h"


class Vmena_top__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vmena_top___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(rst_n,0,0);
        CData/*0:0*/ mena_top__DOT____Vcellinp__u_dispatcher__rst_n;
        VL_IN8(reg_write_valid,0,0);
        VL_IN8(reg_addr,5,0);
        VL_OUT8(mem_req_valid,0,0);
        VL_OUT8(mem_req_expert,2,0);
        VL_OUT8(mem_req_is_pf,0,0);
        VL_IN8(mem_req_ready,0,0);
        VL_IN8(mem_fill_valid,0,0);
        VL_IN8(mem_fill_expert,2,0);
        VL_IN8(token_wr_en,0,0);
        VL_IN8(token_wr_id,5,0);
        VL_IN8(token_wr_experts,5,0);
        CData/*0:0*/ mena_top__DOT__start_pulse;
        CData/*0:0*/ mena_top__DOT__reset_pulse;
        CData/*0:0*/ mena_top__DOT__busy_reg;
        CData/*0:0*/ mena_top__DOT__done_reg;
        CData/*0:0*/ mena_top__DOT__dispatch_valid;
        CData/*2:0*/ mena_top__DOT__dispatch_expert_id;
        CData/*4:0*/ mena_top__DOT__dispatch_token_id;
        CData/*0:0*/ mena_top__DOT__dispatch_done;
        CData/*2:0*/ mena_top__DOT__next_sched_expert;
        CData/*0:0*/ mena_top__DOT__sched_valid;
        CData/*0:0*/ mena_top__DOT__cache_lookup_hit;
        CData/*0:0*/ mena_top__DOT__cache_lookup_ready;
        CData/*0:0*/ mena_top__DOT__wbuf_stall;
        CData/*0:0*/ mena_top__DOT__wbuf_swap_event;
        CData/*1:0*/ mena_top__DOT__u_dispatcher__DOT__state;
        CData/*1:0*/ mena_top__DOT__u_dispatcher__DOT__next_state;
        CData/*2:0*/ mena_top__DOT__u_dispatcher__DOT__curr_expert;
        CData/*2:0*/ mena_top__DOT__u_dispatcher__DOT__next_expert;
        CData/*5:0*/ mena_top__DOT__u_dispatcher__DOT__curr_token;
        CData/*5:0*/ mena_top__DOT__u_dispatcher__DOT__next_token;
        CData/*0:0*/ mena_top__DOT__u_dispatcher__DOT__dispatch_match;
        CData/*5:0*/ mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__count;
        CData/*0:0*/ mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__match_count;
        CData/*2:0*/ mena_top__DOT__u_scheduler__DOT__rr_ptr;
        CData/*0:0*/ mena_top__DOT__u_scheduler__DOT__any_nonempty;
        CData/*2:0*/ mena_top__DOT__u_scheduler__DOT__sel_rr;
        CData/*0:0*/ mena_top__DOT__u_scheduler__DOT__rr_found;
        CData/*2:0*/ mena_top__DOT__u_scheduler__DOT__sel_lqf;
        CData/*5:0*/ mena_top__DOT__u_scheduler__DOT__lqf_best;
        CData/*2:0*/ mena_top__DOT__u_scheduler__DOT__sel_hef;
        CData/*0:0*/ mena_top__DOT__u_scheduler__DOT__hef_found;
        CData/*2:0*/ mena_top__DOT__u_scheduler__DOT__sel_age;
        CData/*0:0*/ mena_top__DOT__u_scheduler__DOT__age_found;
        CData/*2:0*/ mena_top__DOT__u_cache__DOT__state_reg;
        CData/*2:0*/ mena_top__DOT__u_cache__DOT__state_next;
        CData/*2:0*/ mena_top__DOT__u_cache__DOT__req_expert_reg;
        CData/*2:0*/ mena_top__DOT__u_cache__DOT__req_expert_next;
        CData/*0:0*/ mena_top__DOT__u_cache__DOT__req_is_pf_reg;
        CData/*0:0*/ mena_top__DOT__u_cache__DOT__req_is_pf_next;
        CData/*1:0*/ mena_top__DOT__u_cache__DOT__alloc_slot_reg;
        CData/*1:0*/ mena_top__DOT__u_cache__DOT__alloc_slot_next;
        CData/*0:0*/ mena_top__DOT__u_cache__DOT__hit_comb;
        CData/*1:0*/ mena_top__DOT__u_cache__DOT__hit_slot;
        CData/*1:0*/ mena_top__DOT__u_cache__DOT__lru_slot;
        CData/*1:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__state_reg;
        CData/*1:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__state_next;
        CData/*0:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_reg;
        CData/*0:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_next;
        CData/*0:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__shadow_valid_reg;
        CData/*0:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__shadow_valid_next;
    };
    struct {
        CData/*0:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__active_valid_reg;
        CData/*0:0*/ mena_top__DOT__u_wbuf_ctrl__DOT__active_valid_next;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VicoFirstIteration;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__mena_top__DOT____Vcellinp__u_dispatcher__rst_n__0;
        CData/*0:0*/ __VactContinue;
        SData/*15:0*/ mena_top__DOT__u_scheduler__DOT__hef_best;
        SData/*15:0*/ mena_top__DOT__u_scheduler__DOT__age_best;
        VL_IN(reg_write_data,31,0);
        VL_OUT(reg_read_data,31,0);
        IData/*31:0*/ mena_top__DOT__reg_ctrl;
        IData/*31:0*/ mena_top__DOT__reg_cache_hit;
        IData/*31:0*/ mena_top__DOT__reg_cache_miss;
        IData/*31:0*/ mena_top__DOT__reg_dma_cycles;
        IData/*31:0*/ mena_top__DOT__reg_total_cycles;
        IData/*31:0*/ mena_top__DOT__u_dispatcher__DOT__unnamedblk4__DOT__k;
        IData/*31:0*/ mena_top__DOT__u_dispatcher__DOT__unnamedblk2__DOT__t;
        IData/*31:0*/ mena_top__DOT__u_dispatcher__DOT__unnamedblk2__DOT__unnamedblk3__DOT__k;
        IData/*31:0*/ mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__unnamedblk9__DOT__k;
        IData/*31:0*/ mena_top__DOT__u_dispatcher__DOT__unnamedblk10__DOT__k;
        IData/*31:0*/ mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i;
        IData/*31:0*/ mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i;
        IData/*31:0*/ mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i;
        IData/*31:0*/ mena_top__DOT__u_cache__DOT__unnamedblk4__DOT__i;
        IData/*31:0*/ mena_top__DOT__u_cache__DOT__unnamedblk5__DOT__i;
        IData/*31:0*/ mena_top__DOT__u_cache__DOT__unnamedblk3__DOT__i;
        IData/*31:0*/ mena_top__DOT__u_cache__DOT__unnamedblk6__DOT__i;
        IData/*31:0*/ __VactIterCount;
        QData/*47:0*/ mena_top__DOT__expert_counts;
        VlUnpacked<VlUnpacked<CData/*2:0*/, 2>, 32> mena_top__DOT__u_dispatcher__DOT__token_experts;
        VlUnpacked<CData/*0:0*/, 32> mena_top__DOT__u_dispatcher__DOT__token_loaded;
        VlUnpacked<CData/*2:0*/, 2> mena_top__DOT__u_dispatcher__DOT__wr_expert_decoded;
        VlUnpacked<CData/*5:0*/, 8> mena_top__DOT__u_scheduler__DOT__cnt;
        VlUnpacked<SData/*15:0*/, 8> mena_top__DOT__u_scheduler__DOT__hot_score;
        VlUnpacked<SData/*15:0*/, 8> mena_top__DOT__u_scheduler__DOT__age_counter;
        VlUnpacked<CData/*2:0*/, 4> mena_top__DOT__u_cache__DOT__tags;
        VlUnpacked<CData/*0:0*/, 4> mena_top__DOT__u_cache__DOT__valids;
        VlUnpacked<CData/*1:0*/, 4> mena_top__DOT__u_cache__DOT__age;
        VlUnpacked<CData/*0:0*/, 6> __Vm_traceActivity;
    };
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vmena_top__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vmena_top___024root(Vmena_top__Syms* symsp, const char* v__name);
    ~Vmena_top___024root();
    VL_UNCOPYABLE(Vmena_top___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
