/* splitvar_state.h -- the variables of splitvar.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "stackcg.h"
#include "data.h"
#include "namelist.h"
#include "regalloc.h"
#include "insdead.h"
#include "conflict.h"
#include "dataflo.h"
#include "varusage.h"
#include "splitvar.h"

struct SplitvarState {
    block_id Instance;
    global_bit_set Id;
};
