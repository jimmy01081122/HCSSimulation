// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vexpert_cache_ctrl__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vexpert_cache_ctrl::Vexpert_cache_ctrl(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vexpert_cache_ctrl__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , lookup_valid{vlSymsp->TOP.lookup_valid}
    , lookup_expert{vlSymsp->TOP.lookup_expert}
    , lookup_hit{vlSymsp->TOP.lookup_hit}
    , lookup_ready{vlSymsp->TOP.lookup_ready}
    , prefetch_valid{vlSymsp->TOP.prefetch_valid}
    , prefetch_expert{vlSymsp->TOP.prefetch_expert}
    , mem_req_valid{vlSymsp->TOP.mem_req_valid}
    , mem_req_expert{vlSymsp->TOP.mem_req_expert}
    , mem_req_is_pf{vlSymsp->TOP.mem_req_is_pf}
    , mem_req_ready{vlSymsp->TOP.mem_req_ready}
    , mem_fill_valid{vlSymsp->TOP.mem_fill_valid}
    , mem_fill_expert{vlSymsp->TOP.mem_fill_expert}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vexpert_cache_ctrl::Vexpert_cache_ctrl(const char* _vcname__)
    : Vexpert_cache_ctrl(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vexpert_cache_ctrl::~Vexpert_cache_ctrl() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vexpert_cache_ctrl___024root___eval_debug_assertions(Vexpert_cache_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
void Vexpert_cache_ctrl___024root___eval_static(Vexpert_cache_ctrl___024root* vlSelf);
void Vexpert_cache_ctrl___024root___eval_initial(Vexpert_cache_ctrl___024root* vlSelf);
void Vexpert_cache_ctrl___024root___eval_settle(Vexpert_cache_ctrl___024root* vlSelf);
void Vexpert_cache_ctrl___024root___eval(Vexpert_cache_ctrl___024root* vlSelf);

void Vexpert_cache_ctrl::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vexpert_cache_ctrl::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vexpert_cache_ctrl___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vexpert_cache_ctrl___024root___eval_static(&(vlSymsp->TOP));
        Vexpert_cache_ctrl___024root___eval_initial(&(vlSymsp->TOP));
        Vexpert_cache_ctrl___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vexpert_cache_ctrl___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vexpert_cache_ctrl::eventsPending() { return false; }

uint64_t Vexpert_cache_ctrl::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vexpert_cache_ctrl::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vexpert_cache_ctrl___024root___eval_final(Vexpert_cache_ctrl___024root* vlSelf);

VL_ATTR_COLD void Vexpert_cache_ctrl::final() {
    Vexpert_cache_ctrl___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vexpert_cache_ctrl::hierName() const { return vlSymsp->name(); }
const char* Vexpert_cache_ctrl::modelName() const { return "Vexpert_cache_ctrl"; }
unsigned Vexpert_cache_ctrl::threads() const { return 1; }
void Vexpert_cache_ctrl::prepareClone() const { contextp()->prepareClone(); }
void Vexpert_cache_ctrl::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vexpert_cache_ctrl::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vexpert_cache_ctrl___024root__trace_decl_types(VerilatedVcd* tracep);

void Vexpert_cache_ctrl___024root__trace_init_top(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vexpert_cache_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_cache_ctrl___024root*>(voidSelf);
    Vexpert_cache_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vexpert_cache_ctrl___024root__trace_decl_types(tracep);
    Vexpert_cache_ctrl___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vexpert_cache_ctrl___024root__trace_register(Vexpert_cache_ctrl___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vexpert_cache_ctrl::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vexpert_cache_ctrl::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vexpert_cache_ctrl___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
