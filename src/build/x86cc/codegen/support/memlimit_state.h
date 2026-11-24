/* memlimit_state.h -- the variables of memlimit.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "data.h"
#include "objout.h"
#include "memcheck.h"
#include "memlimit.h"
#include "optmain.h"
#include "namelist.h"
#include "optmkins.h"
#include "makeins.h"
#include "conflict.h"
#include "inssched.h"
#include "memsydep.h"
#include "regtree.h"
#include "score.h"
#include "makeaddr.h"
#include "tree.h"
#include "treeprot.h"
#include "feprotos.h"

struct MemlimitState {
    pointer_uint MemLimit;
    bool IckyWicky;
};
