/* i87sched_state.h -- the variables of i87sched.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "cgmem.h"
#include "i87sched.h"
#include "data.h"
#include "edge.h"
#include "redefby.h"
#include "cgsrtlst.h"
#include "indvars.h"
#include "loopopts.h"
#include "fpu.h"
#include "x87.h"
#include "namelist.h"
#include "optab.h"
#include "fixindex.h"
#include "revcond.h"
#include "x86regn.h"

struct I87schedFileState {
    block *Entry;
    block *Exit;
};
