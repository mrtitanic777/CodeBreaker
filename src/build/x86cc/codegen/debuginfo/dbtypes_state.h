/* dbtypes_state.h -- the variables of dbtypes.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "symdbg.h"
#include "model.h"
#include "types.h"
#include "zoiks.h"
#include "i64.h"
#include "utils.h"
#include "wvtypes.h"
#include "dw.h"
#include "dftypes.h"
#include "cvdbg.h"
#include "cvtypes.h"
#include "dbsupp.h"
#include "cgprotos.h"

struct DbtypesState {
    bool Nested;   /* set when types are nested by others */
};
