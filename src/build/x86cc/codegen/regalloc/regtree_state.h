/* regtree_state.h -- the variables of regtree.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "confldef.h"
#include "regset.h"
#include "freelist.h"
#include "zoiks.h"
#include "rgtbl.h"
#include "conflict.h"
#include "regtree.h"

struct RegtreeState {
    pointer *RegFrl;
    pointer *TreeFrl;
};
