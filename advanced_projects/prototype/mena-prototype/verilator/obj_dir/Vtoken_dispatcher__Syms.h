// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VTOKEN_DISPATCHER__SYMS_H_
#define VERILATED_VTOKEN_DISPATCHER__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vtoken_dispatcher.h"

// INCLUDE MODULE CLASSES
#include "Vtoken_dispatcher___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vtoken_dispatcher__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtoken_dispatcher* const __Vm_modelp;
    bool __Vm_activity = false;  ///< Used by trace routines to determine change occurred
    uint32_t __Vm_baseCode = 0;  ///< Used by trace routines when tracing multiple models
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtoken_dispatcher___024root    TOP;

    // CONSTRUCTORS
    Vtoken_dispatcher__Syms(VerilatedContext* contextp, const char* namep, Vtoken_dispatcher* modelp);
    ~Vtoken_dispatcher__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
