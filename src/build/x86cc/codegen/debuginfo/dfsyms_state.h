/* dfsyms_state.h -- the variables of dfsyms.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "dw.h"
#include "dwarf.h"
#include "dfdbg.h"
#include "data.h"
#include "types.h"
#include "objout.h"
#include "dbsyms.h"
#include "dfsyms.h"
#include "dfsupp.h"
#include "dftypes.h"
#include "targetdb.h"
#include "namelist.h"
#include "felang.h"
#include "cgprotos.h"
#include "feprotos.h"

typedef struct {
    segment_id  segid;
    long_offset offset;
} big_patch_handle;
typedef struct {
    back_handle  bck;
    int_32       disp;
} loc_range;
struct dwarf_lang_map {
     dw_langnum dwarf_lang;
     const char *name;
};

struct DfsymsFileState {
    short CurrFNo;
    bool CcuDef;
    back_handle Pc_High;
    back_handle Pc_Low;
    back_handle Comp_High;
    back_handle ARange;
    sect_info DwarfSegs[DW_DEBUG_MAX];
    big_patch_handle UnitSize[1];
};
