/* exprtree_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include "cvars.h"

/* the variables of exprtree_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct ExprtreeState {
    TREEPTR ExprNodeList;
    unsigned NodeCount;
};
inline ExprtreeState& exprtree_state() { return cbrun_part<ExprtreeState>(); }
