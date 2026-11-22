/* conflict_state.h -- the variables of conflict.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "freelist.h"
#include "zoiks.h"
#include "data.h"
#include "rgtbl.h"
#include "namelist.h"
#include "conflict.h"
#include "feprotos.h"

struct ConflictVarsState {
    pointer *ConfFrl;
    pointer *ConfAliasVarsFrl;
};
