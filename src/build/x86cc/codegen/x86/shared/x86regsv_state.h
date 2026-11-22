/* x86regsv_state.h -- the variables of x86regsv.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "data.h"
#include "savings.h"
#include "rgtbl.h"
#include "conflict.h"
#include "regsave.h"

struct X86regsvState {
    save_def MaxConstSave;
};
