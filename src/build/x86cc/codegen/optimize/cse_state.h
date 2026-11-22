/* cse_state.h -- the variables of cse.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "tree.h"
#include "data.h"
#include "fpu.h"
#include "makeins.h"
#include "foldins.h"
#include "utils.h"
#include "namelist.h"
#include "peepopt.h"
#include "blips.h"
#include "redefby.h"
#include "indvars.h"
#include "loopopts.h"
#include "rgtbl.h"
#include "split.h"
#include "insutil.h"
#include "insdead.h"
#include "typemap.h"
#include "blktrim.h"
#include "treefold.h"
#include "fixindex.h"
#include "generate.h"
#include "flograph.h"
#include "cse.h"
#include "varusage.h"
#include "edge.h"
#include "feprotos.h"

typedef enum {
        ALL_LIVE,
        OP_DIES,
        RESULT_DIES
} who_dies;

struct CseState {
    instruction *ExprHeads[LAST_CSE_OP + 1];
    bool LeaveIndVars;
};
