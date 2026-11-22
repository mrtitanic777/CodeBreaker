/* wvtypes_state.h -- the variables of wvtypes.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "wvdbg.h"
#include "data.h"
#include "types.h"
#include "objout.h"
#include "wvtypes.h"
#include "dbsyms.h"
#include "x86dbsup.h"
#include "dbsupp.h"
#include "wvsupp.h"
#include "i64.h"

struct WvtypesState {
    dbg_patch CueInfoOffset;
};
