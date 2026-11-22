/* memmgt_state.h -- the variables of memmgt.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include <stdio.h>
#include "memsydep.h"
#include "memcheck.h"
#include "zoiks.h"
#include "spawn.h"
#include "memout.h"
#include "cgmem.h"
#include "optmac.h"
#include "memlimit.h"
#include "onexit.h"
#include "dumpio.h"
#include "memmgt.h"
#include "trmem.h"

struct MemmgtState {
    mem_out_action MemOut;
};
