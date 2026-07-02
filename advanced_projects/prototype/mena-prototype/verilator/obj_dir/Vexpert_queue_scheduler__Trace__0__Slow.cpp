// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vexpert_queue_scheduler__Syms.h"


VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_init_sub__TOP__0(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+40,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+41,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declQuad(c+42,0,"expert_counts",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 47,0);
    tracep->declBit(c+44,0,"valid_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+45,0,"next_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+46,0,"sched_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("expert_queue_scheduler", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+47,0,"NUM_EXPERTS",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+48,0,"QUEUE_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+49,0,"SCHED_POLICY",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBit(c+40,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+41,0,"rst_n",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declQuad(c+42,0,"expert_counts",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 47,0);
    tracep->declBit(c+44,0,"valid_in",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+45,0,"next_expert",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+46,0,"sched_valid",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+50,0,"EXP_W",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+51,0,"HOT_W",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+51,0,"AGE_W",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("cnt", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 8; ++i) {
        tracep->declBus(c+1+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 5,0);
    }
    tracep->popPrefix();
    tracep->declBus(c+20,0,"rr_ptr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->pushPrefix("hot_score", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 8; ++i) {
        tracep->declBus(c+21+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 15,0);
    }
    tracep->popPrefix();
    tracep->pushPrefix("age_counter", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 8; ++i) {
        tracep->declBus(c+29+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 15,0);
    }
    tracep->popPrefix();
    tracep->declBit(c+9,0,"any_nonempty",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+12,0,"sel_rr",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBit(c+13,0,"rr_found",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+10,0,"sel_lqf",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+11,0,"lqf_best",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 5,0);
    tracep->declBus(c+14,0,"sel_hef",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+15,0,"hef_best",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBit(c+16,0,"hef_found",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+17,0,"sel_age",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+18,0,"age_best",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 15,0);
    tracep->declBit(c+19,0,"age_found",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("p_age", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk5", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+52,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_any", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+52,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_hef", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk4", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+52,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_lqf", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk3", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+52,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_rr", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk2", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+52,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("p_seq", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->pushPrefix("unnamedblk6", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+37,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk7", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+38,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk8", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+39,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_init_top(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_init_top\n"); );
    // Body
    Vexpert_queue_scheduler___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vexpert_queue_scheduler___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void Vexpert_queue_scheduler___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_register(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_register\n"); );
    // Body
    tracep->addConstCb(&Vexpert_queue_scheduler___024root__trace_const_0, 0U, vlSelf);
    tracep->addFullCb(&Vexpert_queue_scheduler___024root__trace_full_0, 0U, vlSelf);
    tracep->addChgCb(&Vexpert_queue_scheduler___024root__trace_chg_0, 0U, vlSelf);
    tracep->addCleanupCb(&Vexpert_queue_scheduler___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_const_0_sub_0(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_const_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_const_0\n"); );
    // Init
    Vexpert_queue_scheduler___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_queue_scheduler___024root*>(voidSelf);
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vexpert_queue_scheduler___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_const_0_sub_0(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_const_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+47,(8U),32);
    bufp->fullIData(oldp+48,(6U),32);
    bufp->fullIData(oldp+49,(0U),32);
    bufp->fullIData(oldp+50,(3U),32);
    bufp->fullIData(oldp+51,(0x10U),32);
    bufp->fullIData(oldp+52,(8U),32);
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_full_0_sub_0(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_full_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_full_0\n"); );
    // Init
    Vexpert_queue_scheduler___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_queue_scheduler___024root*>(voidSelf);
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vexpert_queue_scheduler___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_full_0_sub_0(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vexpert_queue_scheduler___024root__trace_full_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullCData(oldp+1,(vlSelf->expert_queue_scheduler__DOT__cnt[0]),6);
    bufp->fullCData(oldp+2,(vlSelf->expert_queue_scheduler__DOT__cnt[1]),6);
    bufp->fullCData(oldp+3,(vlSelf->expert_queue_scheduler__DOT__cnt[2]),6);
    bufp->fullCData(oldp+4,(vlSelf->expert_queue_scheduler__DOT__cnt[3]),6);
    bufp->fullCData(oldp+5,(vlSelf->expert_queue_scheduler__DOT__cnt[4]),6);
    bufp->fullCData(oldp+6,(vlSelf->expert_queue_scheduler__DOT__cnt[5]),6);
    bufp->fullCData(oldp+7,(vlSelf->expert_queue_scheduler__DOT__cnt[6]),6);
    bufp->fullCData(oldp+8,(vlSelf->expert_queue_scheduler__DOT__cnt[7]),6);
    bufp->fullBit(oldp+9,(vlSelf->expert_queue_scheduler__DOT__any_nonempty));
    bufp->fullCData(oldp+10,(vlSelf->expert_queue_scheduler__DOT__sel_lqf),3);
    bufp->fullCData(oldp+11,(vlSelf->expert_queue_scheduler__DOT__lqf_best),6);
    bufp->fullCData(oldp+12,(vlSelf->expert_queue_scheduler__DOT__sel_rr),3);
    bufp->fullBit(oldp+13,(vlSelf->expert_queue_scheduler__DOT__rr_found));
    bufp->fullCData(oldp+14,(vlSelf->expert_queue_scheduler__DOT__sel_hef),3);
    bufp->fullSData(oldp+15,(vlSelf->expert_queue_scheduler__DOT__hef_best),16);
    bufp->fullBit(oldp+16,(vlSelf->expert_queue_scheduler__DOT__hef_found));
    bufp->fullCData(oldp+17,(vlSelf->expert_queue_scheduler__DOT__sel_age),3);
    bufp->fullSData(oldp+18,(vlSelf->expert_queue_scheduler__DOT__age_best),16);
    bufp->fullBit(oldp+19,(vlSelf->expert_queue_scheduler__DOT__age_found));
    bufp->fullCData(oldp+20,(vlSelf->expert_queue_scheduler__DOT__rr_ptr),3);
    bufp->fullSData(oldp+21,(vlSelf->expert_queue_scheduler__DOT__hot_score[0]),16);
    bufp->fullSData(oldp+22,(vlSelf->expert_queue_scheduler__DOT__hot_score[1]),16);
    bufp->fullSData(oldp+23,(vlSelf->expert_queue_scheduler__DOT__hot_score[2]),16);
    bufp->fullSData(oldp+24,(vlSelf->expert_queue_scheduler__DOT__hot_score[3]),16);
    bufp->fullSData(oldp+25,(vlSelf->expert_queue_scheduler__DOT__hot_score[4]),16);
    bufp->fullSData(oldp+26,(vlSelf->expert_queue_scheduler__DOT__hot_score[5]),16);
    bufp->fullSData(oldp+27,(vlSelf->expert_queue_scheduler__DOT__hot_score[6]),16);
    bufp->fullSData(oldp+28,(vlSelf->expert_queue_scheduler__DOT__hot_score[7]),16);
    bufp->fullSData(oldp+29,(vlSelf->expert_queue_scheduler__DOT__age_counter[0]),16);
    bufp->fullSData(oldp+30,(vlSelf->expert_queue_scheduler__DOT__age_counter[1]),16);
    bufp->fullSData(oldp+31,(vlSelf->expert_queue_scheduler__DOT__age_counter[2]),16);
    bufp->fullSData(oldp+32,(vlSelf->expert_queue_scheduler__DOT__age_counter[3]),16);
    bufp->fullSData(oldp+33,(vlSelf->expert_queue_scheduler__DOT__age_counter[4]),16);
    bufp->fullSData(oldp+34,(vlSelf->expert_queue_scheduler__DOT__age_counter[5]),16);
    bufp->fullSData(oldp+35,(vlSelf->expert_queue_scheduler__DOT__age_counter[6]),16);
    bufp->fullSData(oldp+36,(vlSelf->expert_queue_scheduler__DOT__age_counter[7]),16);
    bufp->fullIData(oldp+37,(vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i),32);
    bufp->fullIData(oldp+38,(vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i),32);
    bufp->fullIData(oldp+39,(vlSelf->expert_queue_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i),32);
    bufp->fullBit(oldp+40,(vlSelf->clk));
    bufp->fullBit(oldp+41,(vlSelf->rst_n));
    bufp->fullQData(oldp+42,(vlSelf->expert_counts),48);
    bufp->fullBit(oldp+44,(vlSelf->valid_in));
    bufp->fullCData(oldp+45,(vlSelf->next_expert),3);
    bufp->fullBit(oldp+46,(vlSelf->sched_valid));
}
