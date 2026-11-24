/* cmac2_state.h -- the variables of cmac2.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <stddef.h>
#include "i64.h"
#include "scan.h"
#include "cgmisc.h"
#include "cmacadd.h"
#include "cscanbuf.h"
#include "ppexpn.h"

struct preproc {
    char  *directive;
    void  (*samelevel)( FrontRun& ); /* func to call when SkipLevel == NestLevel */
    void  (*skipfunc)( FrontRun& );  /* func to call when SkipLevel != NestLevel */
};

struct Cmac2State {
    struct preproc PreProcTable[16];
};
