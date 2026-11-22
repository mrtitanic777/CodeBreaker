/* bldins_state.h -- the variables of bldins.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "memcheck.h"
#include "makeins.h"
#include "data.h"
#include "bldins.h"
#include "addrfold.h"
#include "display.h"
#include "utils.h"
#include "tree.h"
#include "treeprot.h"
#include "makeaddr.h"
#include "namelist.h"
#include "makeblk.h"
#include "encode.h"
#include "rgtbl.h"
#include "typemap.h"
#include "generate.h"
#include "types.h"
#include "i64.h"
#include "feprotos.h"

struct BldinsState {
    const type_def *LastCmpType;
    uint_32 UnrollValue = 0;
};
