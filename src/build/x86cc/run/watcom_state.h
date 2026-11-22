/* watcom_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include "cvars.h"
#include <banner.h>

/* the variables of watcom_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct WatcomState {
    char CompilerID[17] = { "__WATCOMC__=" _MACROSTR( _BLDVER ) };
};
inline WatcomState& watcom_state() { return cbrun_part<WatcomState>(); }
