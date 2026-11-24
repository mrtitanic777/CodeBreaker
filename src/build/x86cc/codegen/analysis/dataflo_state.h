/* dataflo_state.h -- the variables of dataflo.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "coderep.h"
#include "data.h"
#include "cgsrtlst.h"
#include "namelist.h"
#include "conflict.h"
#include "dataflo.h"
#include "savings.h"
#include "regsave.h"
#include "varusage.h"
#include "feprotos.h"

struct DatafloState {
    bool MoreUseInOtherTemps;
};
