/* cvsyms_state.h -- the variables of cvsyms.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "cvdbg.h"
#include "data.h"
#include "types.h"
#include "objout.h"
#include "targetdb.h"
#include "cvsyms.h"
#include "rgtbl.h"
#include "namelist.h"
#include "dbsupp.h"
#include "cvtypes.h"
#include "felang.h"
#include "feprotos.h"
#include "cgprotos.h"

struct sf_info {
    char       size;
    s_values   code;
};
struct cv_lang_map {
     int        cv_lang;
     const char *name;
};

struct CvsymsState {
    struct sf_info sf_info;
    dbg_patch RtnPatch[1];
};
