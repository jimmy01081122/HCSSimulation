// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vmena_top__Syms.h"


void Vmena_top___024root__trace_chg_0_sub_0(Vmena_top___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vmena_top___024root__trace_chg_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_chg_0\n"); );
    // Init
    Vmena_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vmena_top___024root*>(voidSelf);
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vmena_top___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vmena_top___024root__trace_chg_0_sub_0(Vmena_top___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_chg_0_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
        bufp->chgBit(oldp+0,(vlSelf->mena_top__DOT__start_pulse));
        bufp->chgBit(oldp+1,(vlSelf->mena_top__DOT__reset_pulse));
        bufp->chgBit(oldp+2,(vlSelf->mena_top__DOT____Vcellinp__u_dispatcher__rst_n));
        bufp->chgCData(oldp+3,(vlSelf->mena_top__DOT__u_dispatcher__DOT__wr_expert_decoded[0]),3);
        bufp->chgCData(oldp+4,(vlSelf->mena_top__DOT__u_dispatcher__DOT__wr_expert_decoded[1]),3);
    }
    if (VL_UNLIKELY((vlSelf->__Vm_traceActivity[1U] 
                     | vlSelf->__Vm_traceActivity[4U]))) {
        bufp->chgBit(oldp+5,(vlSelf->mena_top__DOT__wbuf_stall));
        bufp->chgCData(oldp+6,(vlSelf->mena_top__DOT__u_dispatcher__DOT__next_state),2);
        bufp->chgCData(oldp+7,(vlSelf->mena_top__DOT__u_dispatcher__DOT__next_expert),3);
        bufp->chgCData(oldp+8,(vlSelf->mena_top__DOT__u_dispatcher__DOT__next_token),6);
        bufp->chgCData(oldp+9,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__state_next),2);
        bufp->chgBit(oldp+10,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__shadow_valid_next));
        bufp->chgBit(oldp+11,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_valid_next));
    }
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[2U])) {
        bufp->chgQData(oldp+12,(vlSelf->mena_top__DOT__expert_counts),48);
        bufp->chgBit(oldp+14,(vlSelf->mena_top__DOT__dispatch_valid));
        bufp->chgCData(oldp+15,(vlSelf->mena_top__DOT__dispatch_expert_id),3);
        bufp->chgCData(oldp+16,(vlSelf->mena_top__DOT__dispatch_token_id),5);
        bufp->chgBit(oldp+17,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_reg));
        bufp->chgBit(oldp+18,((1U & (~ (IData)(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_reg)))));
        bufp->chgBit(oldp+19,(vlSelf->mena_top__DOT__wbuf_swap_event));
        bufp->chgBit(oldp+20,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__shadow_valid_reg));
        bufp->chgBit(oldp+21,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_valid_reg));
        bufp->chgCData(oldp+22,(vlSelf->mena_top__DOT__u_cache__DOT__tags[0]),3);
        bufp->chgCData(oldp+23,(vlSelf->mena_top__DOT__u_cache__DOT__tags[1]),3);
        bufp->chgCData(oldp+24,(vlSelf->mena_top__DOT__u_cache__DOT__tags[2]),3);
        bufp->chgCData(oldp+25,(vlSelf->mena_top__DOT__u_cache__DOT__tags[3]),3);
        bufp->chgBit(oldp+26,(vlSelf->mena_top__DOT__u_cache__DOT__valids[0]));
        bufp->chgBit(oldp+27,(vlSelf->mena_top__DOT__u_cache__DOT__valids[1]));
        bufp->chgBit(oldp+28,(vlSelf->mena_top__DOT__u_cache__DOT__valids[2]));
        bufp->chgBit(oldp+29,(vlSelf->mena_top__DOT__u_cache__DOT__valids[3]));
        bufp->chgCData(oldp+30,(vlSelf->mena_top__DOT__u_cache__DOT__age[0]),2);
        bufp->chgCData(oldp+31,(vlSelf->mena_top__DOT__u_cache__DOT__age[1]),2);
        bufp->chgCData(oldp+32,(vlSelf->mena_top__DOT__u_cache__DOT__age[2]),2);
        bufp->chgCData(oldp+33,(vlSelf->mena_top__DOT__u_cache__DOT__age[3]),2);
        bufp->chgCData(oldp+34,(vlSelf->mena_top__DOT__u_cache__DOT__state_reg),3);
        bufp->chgCData(oldp+35,(vlSelf->mena_top__DOT__u_cache__DOT__req_expert_reg),3);
        bufp->chgBit(oldp+36,(vlSelf->mena_top__DOT__u_cache__DOT__req_is_pf_reg));
        bufp->chgCData(oldp+37,(vlSelf->mena_top__DOT__u_cache__DOT__alloc_slot_reg),2);
        bufp->chgCData(oldp+38,(vlSelf->mena_top__DOT__u_cache__DOT__lru_slot),2);
        bufp->chgIData(oldp+39,(vlSelf->mena_top__DOT__u_cache__DOT__unnamedblk3__DOT__i),32);
        bufp->chgIData(oldp+40,(vlSelf->mena_top__DOT__u_cache__DOT__unnamedblk4__DOT__i),32);
        bufp->chgIData(oldp+41,(vlSelf->mena_top__DOT__u_cache__DOT__unnamedblk5__DOT__i),32);
        bufp->chgBit(oldp+42,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[0]));
        bufp->chgBit(oldp+43,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[1]));
        bufp->chgBit(oldp+44,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[2]));
        bufp->chgBit(oldp+45,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[3]));
        bufp->chgBit(oldp+46,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[4]));
        bufp->chgBit(oldp+47,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[5]));
        bufp->chgBit(oldp+48,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[6]));
        bufp->chgBit(oldp+49,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[7]));
        bufp->chgBit(oldp+50,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[8]));
        bufp->chgBit(oldp+51,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[9]));
        bufp->chgBit(oldp+52,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[10]));
        bufp->chgBit(oldp+53,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[11]));
        bufp->chgBit(oldp+54,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[12]));
        bufp->chgBit(oldp+55,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[13]));
        bufp->chgBit(oldp+56,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[14]));
        bufp->chgBit(oldp+57,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[15]));
        bufp->chgBit(oldp+58,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[16]));
        bufp->chgBit(oldp+59,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[17]));
        bufp->chgBit(oldp+60,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[18]));
        bufp->chgBit(oldp+61,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[19]));
        bufp->chgBit(oldp+62,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[20]));
        bufp->chgBit(oldp+63,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[21]));
        bufp->chgBit(oldp+64,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[22]));
        bufp->chgBit(oldp+65,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[23]));
        bufp->chgBit(oldp+66,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[24]));
        bufp->chgBit(oldp+67,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[25]));
        bufp->chgBit(oldp+68,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[26]));
        bufp->chgBit(oldp+69,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[27]));
        bufp->chgBit(oldp+70,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[28]));
        bufp->chgBit(oldp+71,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[29]));
        bufp->chgBit(oldp+72,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[30]));
        bufp->chgBit(oldp+73,(vlSelf->mena_top__DOT__u_dispatcher__DOT__token_loaded[31]));
        bufp->chgCData(oldp+74,(vlSelf->mena_top__DOT__u_dispatcher__DOT__state),2);
        bufp->chgCData(oldp+75,(vlSelf->mena_top__DOT__u_dispatcher__DOT__curr_expert),3);
        bufp->chgCData(oldp+76,(vlSelf->mena_top__DOT__u_dispatcher__DOT__curr_token),6);
        bufp->chgBit(oldp+77,(vlSelf->mena_top__DOT__u_dispatcher__DOT__dispatch_match));
        bufp->chgIData(oldp+78,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk10__DOT__k),32);
        bufp->chgIData(oldp+79,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk2__DOT__t),32);
        bufp->chgIData(oldp+80,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk2__DOT__unnamedblk3__DOT__k),32);
        bufp->chgIData(oldp+81,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk4__DOT__k),32);
        bufp->chgCData(oldp+82,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__count),6);
        bufp->chgBit(oldp+83,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__match_count));
        bufp->chgIData(oldp+84,(vlSelf->mena_top__DOT__u_dispatcher__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__unnamedblk8__DOT__unnamedblk9__DOT__k),32);
        bufp->chgCData(oldp+85,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[0]),6);
        bufp->chgCData(oldp+86,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[1]),6);
        bufp->chgCData(oldp+87,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[2]),6);
        bufp->chgCData(oldp+88,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[3]),6);
        bufp->chgCData(oldp+89,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[4]),6);
        bufp->chgCData(oldp+90,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[5]),6);
        bufp->chgCData(oldp+91,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[6]),6);
        bufp->chgCData(oldp+92,(vlSelf->mena_top__DOT__u_scheduler__DOT__cnt[7]),6);
        bufp->chgCData(oldp+93,(vlSelf->mena_top__DOT__u_scheduler__DOT__rr_ptr),3);
        bufp->chgSData(oldp+94,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[0]),16);
        bufp->chgSData(oldp+95,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[1]),16);
        bufp->chgSData(oldp+96,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[2]),16);
        bufp->chgSData(oldp+97,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[3]),16);
        bufp->chgSData(oldp+98,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[4]),16);
        bufp->chgSData(oldp+99,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[5]),16);
        bufp->chgSData(oldp+100,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[6]),16);
        bufp->chgSData(oldp+101,(vlSelf->mena_top__DOT__u_scheduler__DOT__hot_score[7]),16);
        bufp->chgSData(oldp+102,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[0]),16);
        bufp->chgSData(oldp+103,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[1]),16);
        bufp->chgSData(oldp+104,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[2]),16);
        bufp->chgSData(oldp+105,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[3]),16);
        bufp->chgSData(oldp+106,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[4]),16);
        bufp->chgSData(oldp+107,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[5]),16);
        bufp->chgSData(oldp+108,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[6]),16);
        bufp->chgSData(oldp+109,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_counter[7]),16);
        bufp->chgBit(oldp+110,(vlSelf->mena_top__DOT__u_scheduler__DOT__any_nonempty));
        bufp->chgCData(oldp+111,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_rr),3);
        bufp->chgBit(oldp+112,(vlSelf->mena_top__DOT__u_scheduler__DOT__rr_found));
        bufp->chgCData(oldp+113,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_lqf),3);
        bufp->chgCData(oldp+114,(vlSelf->mena_top__DOT__u_scheduler__DOT__lqf_best),6);
        bufp->chgCData(oldp+115,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_hef),3);
        bufp->chgSData(oldp+116,(vlSelf->mena_top__DOT__u_scheduler__DOT__hef_best),16);
        bufp->chgBit(oldp+117,(vlSelf->mena_top__DOT__u_scheduler__DOT__hef_found));
        bufp->chgCData(oldp+118,(vlSelf->mena_top__DOT__u_scheduler__DOT__sel_age),3);
        bufp->chgSData(oldp+119,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_best),16);
        bufp->chgBit(oldp+120,(vlSelf->mena_top__DOT__u_scheduler__DOT__age_found));
        bufp->chgIData(oldp+121,(vlSelf->mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk6__DOT__i),32);
        bufp->chgIData(oldp+122,(vlSelf->mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk7__DOT__i),32);
        bufp->chgIData(oldp+123,(vlSelf->mena_top__DOT__u_scheduler__DOT__p_seq__DOT__unnamedblk8__DOT__i),32);
        bufp->chgCData(oldp+124,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__state_reg),2);
        bufp->chgBit(oldp+125,(vlSelf->mena_top__DOT__u_wbuf_ctrl__DOT__active_sel_next));
    }
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[3U])) {
        bufp->chgIData(oldp+126,(vlSelf->mena_top__DOT__reg_ctrl),32);
        bufp->chgIData(oldp+127,((((IData)(vlSelf->mena_top__DOT__busy_reg) 
                                   << 1U) | (IData)(vlSelf->mena_top__DOT__done_reg))),32);
        bufp->chgIData(oldp+128,(vlSelf->mena_top__DOT__reg_cache_hit),32);
        bufp->chgIData(oldp+129,(vlSelf->mena_top__DOT__reg_cache_miss),32);
        bufp->chgIData(oldp+130,(vlSelf->mena_top__DOT__reg_dma_cycles),32);
        bufp->chgIData(oldp+131,(vlSelf->mena_top__DOT__reg_total_cycles),32);
        bufp->chgBit(oldp+132,(vlSelf->mena_top__DOT__busy_reg));
        bufp->chgBit(oldp+133,(vlSelf->mena_top__DOT__done_reg));
    }
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[5U])) {
        bufp->chgCData(oldp+134,(vlSelf->mena_top__DOT__next_sched_expert),3);
        bufp->chgBit(oldp+135,(vlSelf->mena_top__DOT__sched_valid));
        bufp->chgBit(oldp+136,(vlSelf->mena_top__DOT__cache_lookup_hit));
        bufp->chgCData(oldp+137,(vlSelf->mena_top__DOT__u_cache__DOT__req_expert_next),3);
        bufp->chgBit(oldp+138,(vlSelf->mena_top__DOT__u_cache__DOT__req_is_pf_next));
        bufp->chgCData(oldp+139,(vlSelf->mena_top__DOT__u_cache__DOT__alloc_slot_next),2);
        bufp->chgBit(oldp+140,(vlSelf->mena_top__DOT__u_cache__DOT__hit_comb));
        bufp->chgCData(oldp+141,(vlSelf->mena_top__DOT__u_cache__DOT__hit_slot),2);
    }
    bufp->chgBit(oldp+142,(vlSelf->clk));
    bufp->chgBit(oldp+143,(vlSelf->rst_n));
    bufp->chgBit(oldp+144,(vlSelf->reg_write_valid));
    bufp->chgCData(oldp+145,(vlSelf->reg_addr),6);
    bufp->chgIData(oldp+146,(vlSelf->reg_write_data),32);
    bufp->chgIData(oldp+147,(vlSelf->reg_read_data),32);
    bufp->chgBit(oldp+148,(vlSelf->mem_req_valid));
    bufp->chgCData(oldp+149,(vlSelf->mem_req_expert),3);
    bufp->chgBit(oldp+150,(vlSelf->mem_req_is_pf));
    bufp->chgBit(oldp+151,(vlSelf->mem_req_ready));
    bufp->chgBit(oldp+152,(vlSelf->mem_fill_valid));
    bufp->chgCData(oldp+153,(vlSelf->mem_fill_expert),3);
    bufp->chgBit(oldp+154,(vlSelf->token_wr_en));
    bufp->chgCData(oldp+155,(vlSelf->token_wr_id),6);
    bufp->chgCData(oldp+156,(vlSelf->token_wr_experts),6);
    bufp->chgBit(oldp+157,(vlSelf->mena_top__DOT__dispatch_done));
    bufp->chgBit(oldp+158,(vlSelf->mena_top__DOT__cache_lookup_ready));
    bufp->chgCData(oldp+159,(vlSelf->mena_top__DOT__u_cache__DOT__state_next),3);
    bufp->chgCData(oldp+160,((0x1fU & (IData)(vlSelf->token_wr_id))),5);
    bufp->chgBit(oldp+161,(((IData)(vlSelf->mem_req_ready) 
                            & (IData)(vlSelf->mem_req_valid))));
}

void Vmena_top___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vmena_top___024root__trace_cleanup\n"); );
    // Init
    Vmena_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vmena_top___024root*>(voidSelf);
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[3U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[4U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[5U] = 0U;
}
