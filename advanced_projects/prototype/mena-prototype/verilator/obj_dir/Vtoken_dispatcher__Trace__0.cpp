// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtoken_dispatcher__Syms.h"


void Vtoken_dispatcher___024root__trace_chg_0_sub_0(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vtoken_dispatcher___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_chg_0\n"); );
    // Init
    Vtoken_dispatcher___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtoken_dispatcher___024root*>(voidSelf);
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vtoken_dispatcher___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vtoken_dispatcher___024root__trace_chg_0_sub_0(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_chg_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
        bufp->chgBit(oldp+0,(vlSelf->token_dispatcher__DOT__token_loaded[0]));
        bufp->chgBit(oldp+1,(vlSelf->token_dispatcher__DOT__token_loaded[1]));
        bufp->chgBit(oldp+2,(vlSelf->token_dispatcher__DOT__token_loaded[2]));
        bufp->chgBit(oldp+3,(vlSelf->token_dispatcher__DOT__token_loaded[3]));
        bufp->chgBit(oldp+4,(vlSelf->token_dispatcher__DOT__token_loaded[4]));
        bufp->chgBit(oldp+5,(vlSelf->token_dispatcher__DOT__token_loaded[5]));
        bufp->chgBit(oldp+6,(vlSelf->token_dispatcher__DOT__token_loaded[6]));
        bufp->chgBit(oldp+7,(vlSelf->token_dispatcher__DOT__token_loaded[7]));
        bufp->chgBit(oldp+8,(vlSelf->token_dispatcher__DOT__token_loaded[8]));
        bufp->chgBit(oldp+9,(vlSelf->token_dispatcher__DOT__token_loaded[9]));
        bufp->chgBit(oldp+10,(vlSelf->token_dispatcher__DOT__token_loaded[10]));
        bufp->chgBit(oldp+11,(vlSelf->token_dispatcher__DOT__token_loaded[11]));
        bufp->chgBit(oldp+12,(vlSelf->token_dispatcher__DOT__token_loaded[12]));
        bufp->chgBit(oldp+13,(vlSelf->token_dispatcher__DOT__token_loaded[13]));
        bufp->chgBit(oldp+14,(vlSelf->token_dispatcher__DOT__token_loaded[14]));
        bufp->chgBit(oldp+15,(vlSelf->token_dispatcher__DOT__token_loaded[15]));
        bufp->chgBit(oldp+16,(vlSelf->token_dispatcher__DOT__token_loaded[16]));
        bufp->chgBit(oldp+17,(vlSelf->token_dispatcher__DOT__token_loaded[17]));
        bufp->chgBit(oldp+18,(vlSelf->token_dispatcher__DOT__token_loaded[18]));
        bufp->chgBit(oldp+19,(vlSelf->token_dispatcher__DOT__token_loaded[19]));
        bufp->chgBit(oldp+20,(vlSelf->token_dispatcher__DOT__token_loaded[20]));
        bufp->chgBit(oldp+21,(vlSelf->token_dispatcher__DOT__token_loaded[21]));
        bufp->chgBit(oldp+22,(vlSelf->token_dispatcher__DOT__token_loaded[22]));
        bufp->chgBit(oldp+23,(vlSelf->token_dispatcher__DOT__token_loaded[23]));
        bufp->chgBit(oldp+24,(vlSelf->token_dispatcher__DOT__token_loaded[24]));
        bufp->chgBit(oldp+25,(vlSelf->token_dispatcher__DOT__token_loaded[25]));
        bufp->chgBit(oldp+26,(vlSelf->token_dispatcher__DOT__token_loaded[26]));
        bufp->chgBit(oldp+27,(vlSelf->token_dispatcher__DOT__token_loaded[27]));
        bufp->chgBit(oldp+28,(vlSelf->token_dispatcher__DOT__token_loaded[28]));
        bufp->chgBit(oldp+29,(vlSelf->token_dispatcher__DOT__token_loaded[29]));
        bufp->chgBit(oldp+30,(vlSelf->token_dispatcher__DOT__token_loaded[30]));
        bufp->chgBit(oldp+31,(vlSelf->token_dispatcher__DOT__token_loaded[31]));
        bufp->chgCData(oldp+32,(vlSelf->token_dispatcher__DOT__state),2);
        bufp->chgCData(oldp+33,(vlSelf->token_dispatcher__DOT__curr_expert),3);
        bufp->chgCData(oldp+34,(vlSelf->token_dispatcher__DOT__curr_token),6);
        bufp->chgBit(oldp+35,(vlSelf->token_dispatcher__DOT__dispatch_match));
        bufp->chgIData(oldp+36,(vlSelf->token_dispatcher__DOT__unnamedblk10__DOT__k),32);
        bufp->chgIData(oldp+37,(vlSelf->token_dispatcher__DOT__unnamedblk2__DOT__t),32);
        bufp->chgIData(oldp+38,(vlSelf->token_dispatcher__DOT__unnamedblk2__DOT__unnamedblk3__DOT__k),32);
        bufp->chgIData(oldp+39,(vlSelf->token_dispatcher__DOT__unnamedblk4__DOT__k),32);
        bufp->chgCData(oldp+40,(vlSelf->token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__count),6);
        bufp->chgBit(oldp+41,(vlSelf->token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__match_count));
        bufp->chgIData(oldp+42,(vlSelf->token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__unnamedblk9__DOT__k),32);
    }
    bufp->chgBit(oldp+43,(vlSelf->clk));
    bufp->chgBit(oldp+44,(vlSelf->rst_n));
    bufp->chgBit(oldp+45,(vlSelf->wr_en));
    bufp->chgCData(oldp+46,(vlSelf->wr_token_id),5);
    bufp->chgCData(oldp+47,(vlSelf->wr_expert_ids),6);
    bufp->chgBit(oldp+48,(vlSelf->start_dispatch));
    bufp->chgQData(oldp+49,(vlSelf->expert_count),48);
    bufp->chgBit(oldp+51,(vlSelf->dispatch_valid));
    bufp->chgCData(oldp+52,(vlSelf->dispatch_expert_id),3);
    bufp->chgCData(oldp+53,(vlSelf->dispatch_token_id),5);
    bufp->chgBit(oldp+54,(vlSelf->dispatch_done));
    bufp->chgCData(oldp+55,(vlSelf->token_dispatcher__DOT__wr_expert_decoded[0]),3);
    bufp->chgCData(oldp+56,(vlSelf->token_dispatcher__DOT__wr_expert_decoded[1]),3);
    bufp->chgCData(oldp+57,(vlSelf->token_dispatcher__DOT__next_state),2);
    bufp->chgCData(oldp+58,(vlSelf->token_dispatcher__DOT__next_expert),3);
    bufp->chgCData(oldp+59,(vlSelf->token_dispatcher__DOT__next_token),6);
}

void Vtoken_dispatcher___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_cleanup\n"); );
    // Init
    Vtoken_dispatcher___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtoken_dispatcher___024root*>(voidSelf);
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
}
