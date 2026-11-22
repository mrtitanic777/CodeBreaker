/* onexit_state.h -- the variables of onexit.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include <stdlib.h>
#include <signal.h>
#include "cgmisc.h"
#include "coderep.h"
#include "objio.h"
#include "onexit.h"
#include "qtimer.h"
#include "feprotos.h"

struct OnexitState {
    bool volatile BrkFlag;
    bool OnExitFlag = true;
};
