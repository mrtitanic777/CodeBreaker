/* dominate_state.h -- the variables of dominate.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "coderep.h"
#include "zoiks.h"
#include "edge.h"
#include "data.h"
#include "dominate.h"

struct DominateState {
    block *ReturnBlock;
};
