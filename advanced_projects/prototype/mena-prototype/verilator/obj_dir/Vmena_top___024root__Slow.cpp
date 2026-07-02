// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vmena_top.h for the primary calling header

#include "Vmena_top__pch.h"
#include "Vmena_top__Syms.h"
#include "Vmena_top___024root.h"

void Vmena_top___024root___ctor_var_reset(Vmena_top___024root* vlSelf);

Vmena_top___024root::Vmena_top___024root(Vmena_top__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vmena_top___024root___ctor_var_reset(this);
}

void Vmena_top___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

Vmena_top___024root::~Vmena_top___024root() {
}
