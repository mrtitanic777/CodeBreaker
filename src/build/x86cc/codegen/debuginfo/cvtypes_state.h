/* cvtypes_state.h -- the variables of cvtypes.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "cvdbg.h"
#include "data.h"
#include "types.h"
#include "utils.h"
#include "objout.h"
#include "cvsyms.h"
#include "cvtypes.h"
#include "dbsupp.h"
#include "i64.h"
#include "feprotos.h"
#include "cgprotos.h"

struct lf_info {
     char       size;
     lf_values  code;
};

struct CvtypesState {
    struct lf_info lf_info;
};
