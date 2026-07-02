// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vtoken_dispatcher__Syms.h"


VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_init_sub__TOP__0(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+44,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+45,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+46,0,"wr_en",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+47,0,"wr_token_id",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+48,0,"wr_expert_ids",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBit(c+49,0,"start_dispatch",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declQuad(c+50,0,"expert_count",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 47,0);
    tracep->declBit(c+52,0,"dispatch_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+53,0,"dispatch_expert_id",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+54,0,"dispatch_token_id",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+55,0,"dispatch_done",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("token_dispatcher", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+61,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+62,0,"NUM_TOKENS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+63,0,"TOP_K",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+64,0,"IDX_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+65,0,"TOK_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+44,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+45,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+46,0,"wr_en",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+47,0,"wr_token_id",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+48,0,"wr_expert_ids",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBit(c+49,0,"start_dispatch",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declQuad(c+50,0,"expert_count",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 47,0);
    tracep->declBit(c+52,0,"dispatch_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+53,0,"dispatch_expert_id",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+54,0,"dispatch_token_id",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBit(c+55,0,"dispatch_done",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("token_loaded", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 32; ++i) {
        tracep->declBit(c+1+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0));
    }
    tracep->popPrefix();
    tracep->pushPrefix("wr_expert_decoded", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 2; ++i) {
        tracep->declBus(c+56+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 2,0);
    }
    tracep->popPrefix();
    tracep->declBus(c+33,0,"state",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+58,0,"next_state",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 1,0);
    tracep->declBus(c+34,0,"curr_expert",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+59,0,"next_expert",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+35,0,"curr_token",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+60,0,"next_token",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBit(c+36,0,"dispatch_match",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+66,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk10", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+37,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk2", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+38,0,"t",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("unnamedblk3", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+39,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk4", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+40,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk5", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+67,0,"e",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("unnamedblk6", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+41,0,"count",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->pushPrefix("unnamedblk7", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+68,0,"t",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("unnamedblk8", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+42,0,"match_count",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("unnamedblk9", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+43,0,"k",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_init_top(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_init_top\n"); );
    // Body
    Vtoken_dispatcher___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtoken_dispatcher___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vtoken_dispatcher___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_register(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_register\n"); );
    // Body
    tracep->addConstCb(&Vtoken_dispatcher___024root__trace_const_0, 0U, vlSelf);
    tracep->addFullCb(&Vtoken_dispatcher___024root__trace_full_0, 0U, vlSelf);
    tracep->addChgCb(&Vtoken_dispatcher___024root__trace_chg_0, 0U, vlSelf);
    tracep->addCleanupCb(&Vtoken_dispatcher___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_const_0_sub_0(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_const_0\n"); );
    // Init
    Vtoken_dispatcher___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtoken_dispatcher___024root*>(voidSelf);
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vtoken_dispatcher___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_const_0_sub_0(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_const_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+61,(8U),32);
    bufp->fullIData(oldp+62,(0x20U),32);
    bufp->fullIData(oldp+63,(2U),32);
    bufp->fullIData(oldp+64,(3U),32);
    bufp->fullIData(oldp+65,(5U),32);
    bufp->fullIData(oldp+66,(2U),32);
    bufp->fullIData(oldp+67,(8U),32);
    bufp->fullIData(oldp+68,(0x20U),32);
}

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_full_0_sub_0(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_full_0\n"); );
    // Init
    Vtoken_dispatcher___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtoken_dispatcher___024root*>(voidSelf);
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vtoken_dispatcher___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_full_0_sub_0(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtoken_dispatcher___024root__trace_full_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->token_dispatcher__DOT__token_loaded[0]));
    bufp->fullBit(oldp+2,(vlSelf->token_dispatcher__DOT__token_loaded[1]));
    bufp->fullBit(oldp+3,(vlSelf->token_dispatcher__DOT__token_loaded[2]));
    bufp->fullBit(oldp+4,(vlSelf->token_dispatcher__DOT__token_loaded[3]));
    bufp->fullBit(oldp+5,(vlSelf->token_dispatcher__DOT__token_loaded[4]));
    bufp->fullBit(oldp+6,(vlSelf->token_dispatcher__DOT__token_loaded[5]));
    bufp->fullBit(oldp+7,(vlSelf->token_dispatcher__DOT__token_loaded[6]));
    bufp->fullBit(oldp+8,(vlSelf->token_dispatcher__DOT__token_loaded[7]));
    bufp->fullBit(oldp+9,(vlSelf->token_dispatcher__DOT__token_loaded[8]));
    bufp->fullBit(oldp+10,(vlSelf->token_dispatcher__DOT__token_loaded[9]));
    bufp->fullBit(oldp+11,(vlSelf->token_dispatcher__DOT__token_loaded[10]));
    bufp->fullBit(oldp+12,(vlSelf->token_dispatcher__DOT__token_loaded[11]));
    bufp->fullBit(oldp+13,(vlSelf->token_dispatcher__DOT__token_loaded[12]));
    bufp->fullBit(oldp+14,(vlSelf->token_dispatcher__DOT__token_loaded[13]));
    bufp->fullBit(oldp+15,(vlSelf->token_dispatcher__DOT__token_loaded[14]));
    bufp->fullBit(oldp+16,(vlSelf->token_dispatcher__DOT__token_loaded[15]));
    bufp->fullBit(oldp+17,(vlSelf->token_dispatcher__DOT__token_loaded[16]));
    bufp->fullBit(oldp+18,(vlSelf->token_dispatcher__DOT__token_loaded[17]));
    bufp->fullBit(oldp+19,(vlSelf->token_dispatcher__DOT__token_loaded[18]));
    bufp->fullBit(oldp+20,(vlSelf->token_dispatcher__DOT__token_loaded[19]));
    bufp->fullBit(oldp+21,(vlSelf->token_dispatcher__DOT__token_loaded[20]));
    bufp->fullBit(oldp+22,(vlSelf->token_dispatcher__DOT__token_loaded[21]));
    bufp->fullBit(oldp+23,(vlSelf->token_dispatcher__DOT__token_loaded[22]));
    bufp->fullBit(oldp+24,(vlSelf->token_dispatcher__DOT__token_loaded[23]));
    bufp->fullBit(oldp+25,(vlSelf->token_dispatcher__DOT__token_loaded[24]));
    bufp->fullBit(oldp+26,(vlSelf->token_dispatcher__DOT__token_loaded[25]));
    bufp->fullBit(oldp+27,(vlSelf->token_dispatcher__DOT__token_loaded[26]));
    bufp->fullBit(oldp+28,(vlSelf->token_dispatcher__DOT__token_loaded[27]));
    bufp->fullBit(oldp+29,(vlSelf->token_dispatcher__DOT__token_loaded[28]));
    bufp->fullBit(oldp+30,(vlSelf->token_dispatcher__DOT__token_loaded[29]));
    bufp->fullBit(oldp+31,(vlSelf->token_dispatcher__DOT__token_loaded[30]));
    bufp->fullBit(oldp+32,(vlSelf->token_dispatcher__DOT__token_loaded[31]));
    bufp->fullCData(oldp+33,(vlSelf->token_dispatcher__DOT__state),2);
    bufp->fullCData(oldp+34,(vlSelf->token_dispatcher__DOT__curr_expert),3);
    bufp->fullCData(oldp+35,(vlSelf->token_dispatcher__DOT__curr_token),6);
    bufp->fullBit(oldp+36,(vlSelf->token_dispatcher__DOT__dispatch_match));
    bufp->fullIData(oldp+37,(vlSelf->token_dispatcher__DOT__unnamedblk10__DOT__k),32);
    bufp->fullIData(oldp+38,(vlSelf->token_dispatcher__DOT__unnamedblk2__DOT__t),32);
    bufp->fullIData(oldp+39,(vlSelf->token_dispatcher__DOT__unnamedblk2__DOT__unnamedblk3__DOT__k),32);
    bufp->fullIData(oldp+40,(vlSelf->token_dispatcher__DOT__unnamedblk4__DOT__k),32);
    bufp->fullCData(oldp+41,(vlSelf->token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__count),6);
    bufp->fullBit(oldp+42,(vlSelf->token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__match_count));
    bufp->fullIData(oldp+43,(vlSelf->token_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__unnamedblk9__DOT__k),32);
    bufp->fullBit(oldp+44,(vlSelf->clk));
    bufp->fullBit(oldp+45,(vlSelf->rst_n));
    bufp->fullBit(oldp+46,(vlSelf->wr_en));
    bufp->fullCData(oldp+47,(vlSelf->wr_token_id),5);
    bufp->fullCData(oldp+48,(vlSelf->wr_expert_ids),6);
    bufp->fullBit(oldp+49,(vlSelf->start_dispatch));
    bufp->fullQData(oldp+50,(vlSelf->expert_count),48);
    bufp->fullBit(oldp+52,(vlSelf->dispatch_valid));
    bufp->fullCData(oldp+53,(vlSelf->dispatch_expert_id),3);
    bufp->fullCData(oldp+54,(vlSelf->dispatch_token_id),5);
    bufp->fullBit(oldp+55,(vlSelf->dispatch_done));
    bufp->fullCData(oldp+56,(vlSelf->token_dispatcher__DOT__wr_expert_decoded[0]),3);
    bufp->fullCData(oldp+57,(vlSelf->token_dispatcher__DOT__wr_expert_decoded[1]),3);
    bufp->fullCData(oldp+58,(vlSelf->token_dispatcher__DOT__next_state),2);
    bufp->fullCData(oldp+59,(vlSelf->token_dispatcher__DOT__next_expert),3);
    bufp->fullCData(oldp+60,(vlSelf->token_dispatcher__DOT__next_token),6);
}
