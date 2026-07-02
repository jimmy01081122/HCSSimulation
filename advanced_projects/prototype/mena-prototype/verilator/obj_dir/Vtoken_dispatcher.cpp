// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtoken_dispatcher__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vtoken_dispatcher::Vtoken_dispatcher(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtoken_dispatcher__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , wr_en{vlSymsp->TOP.wr_en}
    , wr_token_id{vlSymsp->TOP.wr_token_id}
    , wr_expert_ids{vlSymsp->TOP.wr_expert_ids}
    , start_dispatch{vlSymsp->TOP.start_dispatch}
    , dispatch_valid{vlSymsp->TOP.dispatch_valid}
    , dispatch_expert_id{vlSymsp->TOP.dispatch_expert_id}
    , dispatch_token_id{vlSymsp->TOP.dispatch_token_id}
    , dispatch_done{vlSymsp->TOP.dispatch_done}
    , expert_count{vlSymsp->TOP.expert_count}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtoken_dispatcher::Vtoken_dispatcher(const char* _vcname__)
    : Vtoken_dispatcher(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtoken_dispatcher::~Vtoken_dispatcher() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtoken_dispatcher___024root___eval_debug_assertions(Vtoken_dispatcher___024root* vlSelf);
#endif  // VL_DEBUG
void Vtoken_dispatcher___024root___eval_static(Vtoken_dispatcher___024root* vlSelf);
void Vtoken_dispatcher___024root___eval_initial(Vtoken_dispatcher___024root* vlSelf);
void Vtoken_dispatcher___024root___eval_settle(Vtoken_dispatcher___024root* vlSelf);
void Vtoken_dispatcher___024root___eval(Vtoken_dispatcher___024root* vlSelf);

void Vtoken_dispatcher::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtoken_dispatcher::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtoken_dispatcher___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtoken_dispatcher___024root___eval_static(&(vlSymsp->TOP));
        Vtoken_dispatcher___024root___eval_initial(&(vlSymsp->TOP));
        Vtoken_dispatcher___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtoken_dispatcher___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtoken_dispatcher::eventsPending() { return false; }

uint64_t Vtoken_dispatcher::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vtoken_dispatcher::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtoken_dispatcher___024root___eval_final(Vtoken_dispatcher___024root* vlSelf);

VL_ATTR_COLD void Vtoken_dispatcher::final() {
    Vtoken_dispatcher___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtoken_dispatcher::hierName() const { return vlSymsp->name(); }
const char* Vtoken_dispatcher::modelName() const { return "Vtoken_dispatcher"; }
unsigned Vtoken_dispatcher::threads() const { return 1; }
void Vtoken_dispatcher::prepareClone() const { contextp()->prepareClone(); }
void Vtoken_dispatcher::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vtoken_dispatcher::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vtoken_dispatcher___024root__trace_decl_types(VerilatedVcd* tracep);

void Vtoken_dispatcher___024root__trace_init_top(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vtoken_dispatcher___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtoken_dispatcher___024root*>(voidSelf);
    Vtoken_dispatcher__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vtoken_dispatcher___024root__trace_decl_types(tracep);
    Vtoken_dispatcher___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtoken_dispatcher___024root__trace_register(Vtoken_dispatcher___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vtoken_dispatcher::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vtoken_dispatcher::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vtoken_dispatcher___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
