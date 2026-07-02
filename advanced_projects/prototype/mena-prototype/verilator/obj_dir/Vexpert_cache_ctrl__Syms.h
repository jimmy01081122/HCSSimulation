// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VEXPERT_CACHE_CTRL__SYMS_H_
#define VERILATED_VEXPERT_CACHE_CTRL__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vexpert_cache_ctrl.h"

// INCLUDE MODULE CLASSES
#include "Vexpert_cache_ctrl___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vexpert_cache_ctrl__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vexpert_cache_ctrl* const __Vm_modelp;
    bool __Vm_activity = false;  ///< Used by trace routines to determine change occurred
    uint32_t __Vm_baseCode = 0;  ///< Used by trace routines when tracing multiple models
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vexpert_cache_ctrl___024root   TOP;

    // CONSTRUCTORS
    Vexpert_cache_ctrl__Syms(VerilatedContext* contextp, const char* namep, Vexpert_cache_ctrl* modelp);
    ~Vexpert_cache_ctrl__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
