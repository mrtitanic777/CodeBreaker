/* namelist_state.h -- the variables of namelist.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "hwreg.h"
#include "freelist.h"
#include "zoiks.h"
#include "namelist.h"
#include "data.h"
#include "seldef.h"
#include "bldsel.h"
#include "intrface.h"
#include "rgtbl.h"
#include "typemap.h"
#include "i64.h"

struct NamelistState {
    pointer *FrlHead[N_CLASS_MAX];
    pointer *ConstDefnFrl;
    name *NullReg;
    name *ConstZero;
    name *ConstOne;
};
