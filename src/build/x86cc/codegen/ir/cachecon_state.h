/* cachecon_state.h -- the variables of cachecon.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "stackcg.h"
#include "makeins.h"
#include "foldins.h"
#include "convins.h"
#include "data.h"
#include "namelist.h"
#include "regalloc.h"
#include "rgtbl.h"
#include "split.h"
#include "insutil.h"
#include "optab.h"
#include "cachecon.h"

struct CacheconState {
    block *Head;
    block *(*Next)(BackRun&, block*);
};
