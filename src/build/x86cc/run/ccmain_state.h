/* ccmain_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include "cvars.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "iopath.h"
#include "scan.h"
#include "jmpbuf.h"
#include "cgdefs.h"
#include "feprotos.h"
#include "swchar.h"
#include "cfeinfo.h"
#include "pathlist.h"
#include "cmacadd.h"
#include "pathgrp2.h"
#include "cscanbuf.h"
#include "clibint.h"
#include "clibext.h"

/* the variables of ccmain_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct CcmainState {
    bool_ PrintWhiteSpace;   // also refered from cmac2.c
};
inline CcmainState& ccmain_state() { return cbrun_part<CcmainState>(); }
