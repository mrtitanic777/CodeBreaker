/* cstmt_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include "cvars.h"
#include "i64.h"
#include "cfeinfo.h"
#include "toggles.h"

/* the variables of cstmt_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct CstmtState {
    LABEL_INDEX LabelIndex;
    SYM_LIST_HEADS *SymListHeads;
};
inline CstmtState& cstmt_state() { return cbrun_part<CstmtState>(); }
