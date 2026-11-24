/* bldsel_state.h -- the variables of bldsel.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "seldef.h"
#include "tree.h"
#include "zoiks.h"
#include "data.h"
#include "types.h"
#include "bldins.h"
#include "makeins.h"
#include "makeaddr.h"
#include "bldsel.h"
#include "bldselco.h"
#include "makeblk.h"
#include "cgsrtlst.h"
#include "generate.h"
#include "i64.h"

struct BldselState {
    const type_def *SortTipe;
};
