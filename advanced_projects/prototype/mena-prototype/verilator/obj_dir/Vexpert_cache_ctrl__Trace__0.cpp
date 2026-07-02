// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vexpert_cache_ctrl__Syms.h"


void Vexpert_cache_ctrl___024root__trace_chg_0_sub_0(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vexpert_cache_ctrl___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_chg_0\n"); );
    // Init
    Vexpert_cache_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_cache_ctrl___024root*>(voidSelf);
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vexpert_cache_ctrl___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vexpert_cache_ctrl___024root__trace_chg_0_sub_0(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_chg_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY((vlSelf->__Vm_traceActivity[1U] 
                     | vlSelf->__Vm_traceActivity[2U]))) {
        bufp->chgCData(oldp+0,(vlSelf->expert_cache_ctrl__DOT__state_next),3);
        bufp->chgCData(oldp+1,(vlSelf->expert_cache_ctrl__DOT__req_expert_next),3);
        bufp->chgBit(oldp+2,(vlSelf->expert_cache_ctrl__DOT__req_is_pf_next));
        bufp->chgCData(oldp+3,(vlSelf->expert_cache_ctrl__DOT__alloc_slot_next),2);
        bufp->chgBit(oldp+4,(vlSelf->expert_cache_ctrl__DOT__hit_comb));
        bufp->chgCData(oldp+5,(vlSelf->expert_cache_ctrl__DOT__hit_slot),2);
        bufp->chgBit(oldp+6,(vlSelf->expert_cache_ctrl__DOT__pf_hit));
        bufp->chgIData(oldp+7,(vlSelf->expert_cache_ctrl__DOT__unnamedblk6__DOT__i),32);
    }
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[2U])) {
        bufp->chgCData(oldp+8,(vlSelf->expert_cache_ctrl__DOT__tags[0]),3);
        bufp->chgCData(oldp+9,(vlSelf->expert_cache_ctrl__DOT__tags[1]),3);
        bufp->chgCData(oldp+10,(vlSelf->expert_cache_ctrl__DOT__tags[2]),3);
        bufp->chgCData(oldp+11,(vlSelf->expert_cache_ctrl__DOT__tags[3]),3);
        bufp->chgBit(oldp+12,(vlSelf->expert_cache_ctrl__DOT__valids[0]));
        bufp->chgBit(oldp+13,(vlSelf->expert_cache_ctrl__DOT__valids[1]));
        bufp->chgBit(oldp+14,(vlSelf->expert_cache_ctrl__DOT__valids[2]));
        bufp->chgBit(oldp+15,(vlSelf->expert_cache_ctrl__DOT__valids[3]));
        bufp->chgCData(oldp+16,(vlSelf->expert_cache_ctrl__DOT__age[0]),2);
        bufp->chgCData(oldp+17,(vlSelf->expert_cache_ctrl__DOT__age[1]),2);
        bufp->chgCData(oldp+18,(vlSelf->expert_cache_ctrl__DOT__age[2]),2);
        bufp->chgCData(oldp+19,(vlSelf->expert_cache_ctrl__DOT__age[3]),2);
        bufp->chgCData(oldp+20,(vlSelf->expert_cache_ctrl__DOT__state_reg),3);
        bufp->chgCData(oldp+21,(vlSelf->expert_cache_ctrl__DOT__req_expert_reg),3);
        bufp->chgBit(oldp+22,(vlSelf->expert_cache_ctrl__DOT__req_is_pf_reg));
        bufp->chgCData(oldp+23,(vlSelf->expert_cache_ctrl__DOT__alloc_slot_reg),2);
        bufp->chgCData(oldp+24,(vlSelf->expert_cache_ctrl__DOT__lru_slot),2);
        bufp->chgIData(oldp+25,(vlSelf->expert_cache_ctrl__DOT__unnamedblk3__DOT__i),32);
        bufp->chgIData(oldp+26,(vlSelf->expert_cache_ctrl__DOT__unnamedblk4__DOT__i),32);
        bufp->chgIData(oldp+27,(vlSelf->expert_cache_ctrl__DOT__unnamedblk5__DOT__i),32);
    }
    bufp->chgBit(oldp+28,(vlSelf->clk));
    bufp->chgBit(oldp+29,(vlSelf->rst_n));
    bufp->chgBit(oldp+30,(vlSelf->lookup_valid));
    bufp->chgCData(oldp+31,(vlSelf->lookup_expert),3);
    bufp->chgBit(oldp+32,(vlSelf->lookup_hit));
    bufp->chgBit(oldp+33,(vlSelf->lookup_ready));
    bufp->chgBit(oldp+34,(vlSelf->prefetch_valid));
    bufp->chgCData(oldp+35,(vlSelf->prefetch_expert),3);
    bufp->chgBit(oldp+36,(vlSelf->mem_req_valid));
    bufp->chgCData(oldp+37,(vlSelf->mem_req_expert),3);
    bufp->chgBit(oldp+38,(vlSelf->mem_req_is_pf));
    bufp->chgBit(oldp+39,(vlSelf->mem_req_ready));
    bufp->chgBit(oldp+40,(vlSelf->mem_fill_valid));
    bufp->chgCData(oldp+41,(vlSelf->mem_fill_expert),3);
}

void Vexpert_cache_ctrl___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_cache_ctrl___024root__trace_cleanup\n"); );
    // Init
    Vexpert_cache_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_cache_ctrl___024root*>(voidSelf);
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
}
