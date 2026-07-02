// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vweight_buffer_ctrl__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vweight_buffer_ctrl::Vweight_buffer_ctrl(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vweight_buffer_ctrl__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , dma_start{vlSymsp->TOP.dma_start}
    , shadow_done{vlSymsp->TOP.shadow_done}
    , exec_start{vlSymsp->TOP.exec_start}
    , exec_done{vlSymsp->TOP.exec_done}
    , active_sel{vlSymsp->TOP.active_sel}
    , dma_sel{vlSymsp->TOP.dma_sel}
    , stall{vlSymsp->TOP.stall}
    , swap_event{vlSymsp->TOP.swap_event}
    , shadow_valid{vlSymsp->TOP.shadow_valid}
    , active_valid{vlSymsp->TOP.active_valid}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vweight_buffer_ctrl::Vweight_buffer_ctrl(const char* _vcname__)
    : Vweight_buffer_ctrl(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vweight_buffer_ctrl::~Vweight_buffer_ctrl() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vweight_buffer_ctrl___024root___eval_debug_assertions(Vweight_buffer_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
void Vweight_buffer_ctrl___024root___eval_static(Vweight_buffer_ctrl___024root* vlSelf);
void Vweight_buffer_ctrl___024root___eval_initial(Vweight_buffer_ctrl___024root* vlSelf);
void Vweight_buffer_ctrl___024root___eval_settle(Vweight_buffer_ctrl___024root* vlSelf);
void Vweight_buffer_ctrl___024root___eval(Vweight_buffer_ctrl___024root* vlSelf);

void Vweight_buffer_ctrl::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vweight_buffer_ctrl::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vweight_buffer_ctrl___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vweight_buffer_ctrl___024root___eval_static(&(vlSymsp->TOP));
        Vweight_buffer_ctrl___024root___eval_initial(&(vlSymsp->TOP));
        Vweight_buffer_ctrl___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vweight_buffer_ctrl___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vweight_buffer_ctrl::eventsPending() { return false; }

uint64_t Vweight_buffer_ctrl::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vweight_buffer_ctrl::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vweight_buffer_ctrl___024root___eval_final(Vweight_buffer_ctrl___024root* vlSelf);

VL_ATTR_COLD void Vweight_buffer_ctrl::final() {
    Vweight_buffer_ctrl___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vweight_buffer_ctrl::hierName() const { return vlSymsp->name(); }
const char* Vweight_buffer_ctrl::modelName() const { return "Vweight_buffer_ctrl"; }
unsigned Vweight_buffer_ctrl::threads() const { return 1; }
void Vweight_buffer_ctrl::prepareClone() const { contextp()->prepareClone(); }
void Vweight_buffer_ctrl::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vweight_buffer_ctrl::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vweight_buffer_ctrl___024root__trace_decl_types(VerilatedVcd* tracep);

void Vweight_buffer_ctrl___024root__trace_init_top(Vweight_buffer_ctrl___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vweight_buffer_ctrl___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vweight_buffer_ctrl___024root*>(voidSelf);
    Vweight_buffer_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vweight_buffer_ctrl___024root__trace_decl_types(tracep);
    Vweight_buffer_ctrl___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vweight_buffer_ctrl___024root__trace_register(Vweight_buffer_ctrl___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vweight_buffer_ctrl::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vweight_buffer_ctrl::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vweight_buffer_ctrl___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
