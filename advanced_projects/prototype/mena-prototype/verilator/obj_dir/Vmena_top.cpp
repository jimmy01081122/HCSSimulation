// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vmena_top__pch.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

Vmena_top::Vmena_top(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vmena_top__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , reg_write_valid{vlSymsp->TOP.reg_write_valid}
    , reg_addr{vlSymsp->TOP.reg_addr}
    , mem_req_valid{vlSymsp->TOP.mem_req_valid}
    , mem_req_expert{vlSymsp->TOP.mem_req_expert}
    , mem_req_is_pf{vlSymsp->TOP.mem_req_is_pf}
    , mem_req_ready{vlSymsp->TOP.mem_req_ready}
    , mem_fill_valid{vlSymsp->TOP.mem_fill_valid}
    , mem_fill_expert{vlSymsp->TOP.mem_fill_expert}
    , token_wr_en{vlSymsp->TOP.token_wr_en}
    , token_wr_id{vlSymsp->TOP.token_wr_id}
    , token_wr_experts{vlSymsp->TOP.token_wr_experts}
    , reg_write_data{vlSymsp->TOP.reg_write_data}
    , reg_read_data{vlSymsp->TOP.reg_read_data}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vmena_top::Vmena_top(const char* _vcname__)
    : Vmena_top(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vmena_top::~Vmena_top() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vmena_top___024root___eval_debug_assertions(Vmena_top___024root* vlSelf);
#endif  // VL_DEBUG
void Vmena_top___024root___eval_static(Vmena_top___024root* vlSelf);
void Vmena_top___024root___eval_initial(Vmena_top___024root* vlSelf);
void Vmena_top___024root___eval_settle(Vmena_top___024root* vlSelf);
void Vmena_top___024root___eval(Vmena_top___024root* vlSelf);

void Vmena_top::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vmena_top::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vmena_top___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vmena_top___024root___eval_static(&(vlSymsp->TOP));
        Vmena_top___024root___eval_initial(&(vlSymsp->TOP));
        Vmena_top___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vmena_top___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vmena_top::eventsPending() { return false; }

uint64_t Vmena_top::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vmena_top::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vmena_top___024root___eval_final(Vmena_top___024root* vlSelf);

VL_ATTR_COLD void Vmena_top::final() {
    Vmena_top___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vmena_top::hierName() const { return vlSymsp->name(); }
const char* Vmena_top::modelName() const { return "Vmena_top"; }
unsigned Vmena_top::threads() const { return 1; }
void Vmena_top::prepareClone() const { contextp()->prepareClone(); }
void Vmena_top::atClone() const {
    contextp()->threadPoolpOnClone();
}
std::unique_ptr<VerilatedTraceConfig> Vmena_top::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void Vmena_top___024root__trace_decl_types(VerilatedVcd* tracep);

void Vmena_top___024root__trace_init_top(Vmena_top___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    Vmena_top___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vmena_top___024root*>(voidSelf);
    Vmena_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->pushPrefix(std::string{vlSymsp->name()}, VerilatedTracePrefixType::SCOPE_MODULE);
    Vmena_top___024root__trace_decl_types(tracep);
    Vmena_top___024root__trace_init_top(vlSelf, tracep);
    tracep->popPrefix();
}

VL_ATTR_COLD void Vmena_top___024root__trace_register(Vmena_top___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void Vmena_top::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'Vmena_top::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    Vmena_top___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
