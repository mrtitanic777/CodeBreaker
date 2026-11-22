/* csym_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include "cvars.h"
#include "cgswitch.h"
#include "caux.h"
#include "cmacadd.h"
#include "roundmac.h"

/* the variables of csym_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct CsymState {
    unsigned SymTypedef;
};
inline CsymState& csym_state() { return cbrun_part<CsymState>(); }
