/* trecurse_state.h -- the variables of trecurse.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "data.h"
#include "makeins.h"
#include "stackcg.h"
#include "namelist.h"
#include "edge.h"
#include "insutil.h"
#include "insdead.h"
#include "blktrim.h"
#include "trecurse.h"

struct TrecurseState {
    name *ReturnValue;
};
