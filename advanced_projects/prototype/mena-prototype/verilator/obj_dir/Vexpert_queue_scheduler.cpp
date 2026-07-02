// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vexpert_queue_scheduler__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vexpert_queue_scheduler::Vexpert_queue_scheduler(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vexpert_queue_scheduler__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , valid_in{vlSymsp->TOP.valid_in}
    , next_expert{vlSymsp->TOP.next_expert}
    , sched_valid{vlSymsp->TOP.sched_valid}
    , expert_counts{vlSymsp->TOP.expert_counts}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vexpert_queue_scheduler::Vexpert_queue_scheduler(const char* _vcname__)
    : Vexpert_queue_scheduler(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vexpert_queue_scheduler::~Vexpert_queue_scheduler() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vexpert_queue_scheduler___024root___eval_debug_assertions(Vexpert_queue_scheduler___024root* vlSelf);
#endif  // VL_DEBUG
void Vexpert_queue_scheduler___024root___eval_static(Vexpert_queue_scheduler___024root* vlSelf);
void Vexpert_queue_scheduler___024root___eval_initial(Vexpert_queue_scheduler___024root* vlSelf);
void Vexpert_queue_scheduler___024root___eval_settle(Vexpert_queue_scheduler___024root* vlSelf);
void Vexpert_queue_scheduler___024root___eval(Vexpert_queue_scheduler___024root* vlSelf);

void Vexpert_queue_scheduler::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vexpert_queue_scheduler::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vexpert_queue_scheduler___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vexpert_queue_scheduler___024root___eval_static(&(vlSymsp->TOP));
        Vexpert_queue_scheduler___024root___eval_initial(&(vlSymsp->TOP));
        Vexpert_queue_scheduler___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vexpert_queue_scheduler___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vexpert_queue_scheduler::eventsPending() { return false; }

uint64_t Vexpert_queue_scheduler::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vexpert_queue_scheduler::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vexpert_queue_scheduler___024root___eval_final(Vexpert_queue_scheduler___024root* vlSelf);

VL_ATTR_COLD void Vexpert_queue_scheduler::final() {
    Vexpert_queue_scheduler___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vexpert_queue_scheduler::hierName() const { return vlSymsp->name(); }
const char* Vexpert_queue_scheduler::modelName() const { return "Vexpert_queue_scheduler"; }
unsigned Vexpert_queue_scheduler::threads() const { return 1; }
void Vexpert_queue_scheduler::prepareClone() const { contextp()->prepareClone(); }
void Vexpert_queue_scheduler::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vexpert_queue_scheduler::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vexpert_queue_scheduler___024root__trace_decl_types(VerilatedVcd* tracep);

void Vexpert_queue_scheduler___024root__trace_init_top(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vexpert_queue_scheduler___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vexpert_queue_scheduler___024root*>(voidSelf);
    Vexpert_queue_scheduler__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vexpert_queue_scheduler___024root__trace_decl_types(tracep);
    Vexpert_queue_scheduler___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vexpert_queue_scheduler___024root__trace_register(Vexpert_queue_scheduler___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vexpert_queue_scheduler::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vexpert_queue_scheduler::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vexpert_queue_scheduler___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
