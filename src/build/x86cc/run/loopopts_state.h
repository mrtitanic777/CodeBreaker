/* loopopts_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "indvars.h"
#include "cgmem.h"
#include "stackcg.h"
#include "zoiks.h"
#include "i64.h"
#include "data.h"
#include "fpu.h"
#include "makeins.h"
#include "utils.h"
#include "namelist.h"
#include "propind.h"
#include "blips.h"
#include "edge.h"
#include "redefby.h"
#include "makeblk.h"
#include "loopopts.h"
#include "split.h"
#include "insutil.h"
#include "insdead.h"
#include "blktrim.h"
#include "unroll.h"
#include "optimize.h"
#include "tree.h"
#include "treefold.h"
#include "splitvar.h"
#include "cachecon.h"
#include "feprotos.h"
#include "revcond.h"

/* the variables of loopopts_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct LoopoptsState {
    block *Head;
    block *PreHead;
    induction *IndVarList;
    block *Loop;
};
inline LoopoptsState& loopopts_state() { return cbrun_part<LoopoptsState>(); }
