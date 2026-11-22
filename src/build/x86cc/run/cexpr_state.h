/* cexpr_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include <ctype.h>
#include "cvars.h"
#include "cmacadd.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "i64.h"
#include "caux.h"
#include "cfeinfo.h"

/* the variables of cexpr_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct CexprState {
    call_list *CallNodeList;
};
inline CexprState& cexpr_state() { return cbrun_part<CexprState>(); }
