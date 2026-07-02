// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vexpert_queue_scheduler__Syms.h"


void Vexpert_queue_scheduler___024root__trace_chg_0_sub_0(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vexpert_queue_scheduler___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_chg_0\n"); );
    // Init
    Vexpert_queue_scheduler___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_queue_scheduler___024root*>(voidSelf);
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vexpert_queue_scheduler___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vexpert_queue_scheduler___024root__trace_chg_0_sub_0(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_chg_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
        bufp->chgCData(oldp+0,(vlSelf->expert_queue_scheduler__DOT__cnt[0]),6);
        bufp->chgCData(oldp+1,(vlSelf->expert_queue_scheduler__DOT__cnt[1]),6);
        bufp->chgCData(oldp+2,(vlSelf->expert_queue_scheduler__DOT__cnt[2]),6);
        bufp->chgCData(oldp+3,(vlSelf->expert_queue_scheduler__DOT__cnt[3]),6);
        bufp->chgCData(oldp+4,(vlSelf->expert_queue_scheduler__DOT__cnt[4]),6);
        bufp->chgCData(oldp+5,(vlSelf->expert_queue_scheduler__DOT__cnt[5]),6);
        bufp->chgCData(oldp+6,(vlSelf->expert_queue_scheduler__DOT__cnt[6]),6);
        bufp->chgCData(oldp+7,(vlSelf->expert_queue_scheduler__DOT__cnt[7]),6);
        bufp->chgBit(oldp+8,(vlSelf->expert_queue_scheduler__DOT__any_nonempty));
        bufp->chgCData(oldp+9,(vlSelf->expert_queue_scheduler__DOT__sel_lqf),3);
        bufp->chgCData(oldp+10,(vlSelf->expert_queue_scheduler__DOT__lqf_best),6);
    }
    if (VL_UNLIKELY((vlSelf->__Vm_traceActivity[1U] 
                     | vlSelf->__Vm_traceActivity[2U]))) {
        bufp->chgCData(oldp+11,(vlSelf->expert_queue_scheduler__DOT__sel_rr),3);
        bufp->chgBit(oldp+12,(vlSelf->expert_queue_scheduler__DOT__rr_found));
        bufp->chgCData(oldp+13,(vlSelf->expert_queue_scheduler__DOT__sel_hef),3);
        bufp->chgSData(oldp+14,(vlSelf->expert_queue_scheduler__DOT__hef_best),16);
        bufp->chgBit(oldp+15,(vlSelf->expert_queue_scheduler__DOT__hef_found));
        bufp->chgCData(oldp+16,(vlSelf->expert_queue_scheduler__DOT__sel_age),3);
        bufp->chgSData(oldp+17,(vlSelf->expert_queue_scheduler__DOT__age_best),16);
        bufp->chgBit(oldp+18,(vlSelf->expert_queue_scheduler__DOT__age_found));
    }
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[2U])) {
        bufp->chgCData(oldp+19,(vlSelf->expert_queue_scheduler__DOT__rr_ptr),3);
        bufp->chgSData(oldp+20,(vlSelf->expert_queue_scheduler__DOT__hot_score[0]),16);
        bufp->chgSData(oldp+21,(vlSelf->expert_queue_scheduler__DOT__hot_score[1]),16);
        bufp->chgSData(oldp+22,(vlSelf->expert_queue_scheduler__DOT__hot_score[2]),16);
        bufp->chgSData(oldp+23,(vlSelf->expert_queue_scheduler__DOT__hot_score[3]),16);
        bufp->chgSData(oldp+24,(vlSelf->expert_queue_scheduler__DOT__hot_score[4]),16);
        bufp->chgSData(oldp+25,(vlSelf->expert_queue_scheduler__DOT__hot_score[5]),16);
        bufp->chgSData(oldp+26,(vlSelf->expert_queue_scheduler__DOT__hot_score[6]),16);
        bufp->chgSData(oldp+27,(vlSelf->expert_queue_scheduler__DOT__hot_score[7]),16);
        bufp->chgSData(oldp+28,(vlSelf->expert_queue_scheduler__DOT__age_counter[0]),16);
        bufp->chgSData(oldp+29,(vlSelf->expert_queue_scheduler__DOT__age_counter[1]),16);
        bufp->chgSData(oldp+30,(vlSelf->expert_queue_scheduler__DOT__age_counter[2]),16);
        bufp->chgSData(oldp+31,(vlSelf->expert_queue_scheduler__DOT__age_counter[3]),16);
        bufp->chgSData(oldp+32,(vlSelf->expert_queue_scheduler__DOT__age_counter[4]),16);
        bufp->chgSData(oldp+33,(vlSelf->expert_queue_scheduler__DOT__age_counter[5]),16);
        bufp->chgSData(oldp+34,(vlSelf->expert_queue_scheduler__DOT__age_counter[6]),16);
        bufp->chgSData(oldp+35,(vlSelf->expert_queue_scheduler__DOT__age_counter[7]),16);
        bufp->chgIData(oldp+36,(vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i),32);
        bufp->chgIData(oldp+37,(vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i),32);
        bufp->chgIData(oldp+38,(vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i),32);
    }
    bufp->chgBit(oldp+39,(vlSelf->clk));
    bufp->chgBit(oldp+40,(vlSelf->rst_n));
    bufp->chgQData(oldp+41,(vlSelf->expert_counts),48);
    bufp->chgBit(oldp+43,(vlSelf->valid_in));
    bufp->chgCData(oldp+44,(vlSelf->next_expert),3);
    bufp->chgBit(oldp+45,(vlSelf->sched_valid));
}

void Vexpert_queue_scheduler___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_cleanup\n"); );
    // Init
    Vexpert_queue_scheduler___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_queue_scheduler___024root*>(voidSelf);
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
}
