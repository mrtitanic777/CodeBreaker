/* inline_state.h -- the variables of inline.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "makeins.h"
#include "data.h"
#include "types.h"
#include "makeaddr.h"
#include "makeblk.h"
#include "namelist.h"
#include "typemap.h"
#include "inline.h"
#include "bldins.h"
#include "feprotos.h"

typedef struct inline_parm {
        struct inline_parm      *next;
        an                      addr;
} inline_parm;
typedef struct inline_stack {
        struct inline_stack     *next;
        struct inline_parm      *parms;
        cg_sym_handle           proc_sym;
        const type_def          *tipe;
        an                      addr;
} inline_stack;

struct InlineState {
    inline_stack *InlineStack = NULL;   // fix this!
};
