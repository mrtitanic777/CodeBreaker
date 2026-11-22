/* x86ldstr_state.h -- the variables of x86ldstr.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "score.h"
#include "makeins.h"
#include "data.h"
#include "namelist.h"
#include "redefby.h"
#include "rgtbl.h"
#include "split.h"
#include "expand.h"
#include "insutil.h"
#include "optab.h"
#include "inssegs.h"
#include "optimize.h"
#include "fixindex.h"
#include "generate.h"
#include "inssched.h"
#include "liveinfo.h"

struct X86ldstrState {
    bool PreferSize;
    const hw_reg_set *RoverByte;
    const hw_reg_set *RoverWord;
    const hw_reg_set *RoverDouble;
};
