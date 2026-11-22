/* scinfo_state.h -- the variables of scinfo.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "score.h"
#include "zoiks.h"
#include "data.h"
#include "utils.h"

struct ScinfoState {
    pointer SegConstSymbol;
    pointer HighAddrSymbol;
    pointer HighAddrConst;
};
