// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Precompiled header
//
// Internal details; most user sources do not need this header,
// unless using verilator public meta comments.
// Suggest use Vexpert_queue_scheduler.h instead.


#ifndef VERILATED_VEXPERT_QUEUE_SCHEDULER__PCH_H_
#define VERILATED_VEXPERT_QUEUE_SCHEDULER__PCH_H_  // guard

// GCC and Clang only will precompile headers (PCH) for the first header.
// So, make sure this is the one and only PCH.
// If multiple module's includes are needed, use individual includes.
#ifdef VL_PCH_INCLUDED
# error "Including multiple precompiled header files"
#endif
#define VL_PCH_INCLUDED


#include "verilated.h"

#include "Vexpert_queue_scheduler__Syms.h"
#include "Vexpert_queue_scheduler.h"

#endif  // guard
