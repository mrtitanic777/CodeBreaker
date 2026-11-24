/* exprtree_state.h -- the variables of exprtree.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"

struct ExprtreeState {
    TREEPTR ExprNodeList;
};
