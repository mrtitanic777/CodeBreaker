/* peepopt_state.h -- the variables of peepopt.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "indvars.h"
#include "score.h"
#include "zoiks.h"
#include "data.h"
#include "makeins.h"
#include "namelist.h"
#include "peepopt.h"
#include "redefby.h"
#include "insdead.h"
#include "optimize.h"
#include "inssched.h"

typedef bool    (ONE_OP)(BackRun&, instruction *,instruction *);
typedef bool    (TWO_OP)(BackRun&, instruction *,instruction *);
typedef bool    (THREE_OP)(BackRun&, instruction *,instruction *);

struct PeepoptState {
    bool AfterRegAlloc;
};
