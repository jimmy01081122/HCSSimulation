// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtopk_selector__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vtopk_selector::Vtopk_selector(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtopk_selector__Syms(contextp(), _vcname__, this)}
    , topk_idx{vlSymsp->TOP.topk_idx}
    , scores_in{vlSymsp->TOP.scores_in}
    , topk_score{vlSymsp->TOP.topk_score}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtopk_selector::Vtopk_selector(const char* _vcname__)
    : Vtopk_selector(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtopk_selector::~Vtopk_selector() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtopk_selector___024root___eval_debug_assertions(Vtopk_selector___024root* vlSelf);
#endif  // VL_DEBUG
void Vtopk_selector___024root___eval_static(Vtopk_selector___024root* vlSelf);
void Vtopk_selector___024root___eval_initial(Vtopk_selector___024root* vlSelf);
void Vtopk_selector___024root___eval_settle(Vtopk_selector___024root* vlSelf);
void Vtopk_selector___024root___eval(Vtopk_selector___024root* vlSelf);

void Vtopk_selector::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtopk_selector::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtopk_selector___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtopk_selector___024root___eval_static(&(vlSymsp->TOP));
        Vtopk_selector___024root___eval_initial(&(vlSymsp->TOP));
        Vtopk_selector___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtopk_selector___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtopk_selector::eventsPending() { return false; }

uint64_t Vtopk_selector::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vtopk_selector::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtopk_selector___024root___eval_final(Vtopk_selector___024root* vlSelf);

VL_ATTR_COLD void Vtopk_selector::final() {
    Vtopk_selector___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtopk_selector::hierName() const { return vlSymsp->name(); }
const char* Vtopk_selector::modelName() const { return "Vtopk_selector"; }
unsigned Vtopk_selector::threads() const { return 1; }
void Vtopk_selector::prepareClone() const { contextp()->prepareClone(); }
void Vtopk_selector::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vtopk_selector::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vtopk_selector___024root__trace_decl_types(VerilatedVcd* tracep);

void Vtopk_selector___024root__trace_init_top(Vtopk_selector___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vtopk_selector___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtopk_selector___024root*>(voidSelf);
    Vtopk_selector__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vtopk_selector___024root__trace_decl_types(tracep);
    Vtopk_selector___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtopk_selector___024root__trace_register(Vtopk_selector___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vtopk_selector::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vtopk_selector::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vtopk_selector___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
