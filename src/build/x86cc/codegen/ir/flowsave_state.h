/* flowsave_state.h -- the variables of flowsave.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "cgmem.h"
#include "data.h"
#include "makeins.h"
#include "namelist.h"
#include "nullprop.h"
#include "flowsave.h"
#include "rgtbl.h"
#include "score.h"
#include "insutil.h"
#include "utils.h"

typedef struct {
    hw_reg_set          reg;                    // actual register bit
    dom_bit_set         dom_usage;              // dominator set for all uses/references
    dom_bit_set         post_dom_usage;         // post-dominator "
    block               *save;                  // block we have picked to save reg in
    block               *restore;               // ditto for restore
} reg_flow_info;

struct FlowsaveState {
    block *blockArray[_DBit_SIZE];
    int bitCount;
    hw_reg_set flowedRegs;
};
