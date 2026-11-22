/* flograph_state.h -- the variables of flograph.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "stackcg.h"
#include "data.h"
#include "makeblk.h"
#include "insutil.h"
#include "flograph.h"
#include "flood.h"

struct FlographState {
    interval_def *Intervals;
    bool functionDegenerate;
};
