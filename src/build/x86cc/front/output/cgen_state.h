/* cgen_state.h -- the variables of cgen.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "langenv.h"
#include "jmpbuf.h"
#include "i64.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgcli.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "cgen.h"
#include "cfeinfo.h"
#include "tryblock.h"
#include "togglesd.h"

typedef struct local_vars {
    struct local_vars   *next;
    SYM_HANDLE          sym_list;
} local_vars;
typedef struct  try_table_back_handles {
    struct try_table_back_handles   *next;
    BACK_HANDLE                     back_handle;
} try_table_back_handles;

struct CgenState {
    local_vars *LocalVarList;
    label_handle *CGLabelHandles;
    TREEPTR FirstNode;
    TREEPTR LastNode;
    OPNODE *FuncNodePtr;
    int Refno;
    temp_handle SSVar;
    SYM_HANDLE Saved_CurFunc;
    int InLineDepth;
#ifdef __SEH__
    int TryRefno;
#endif
#ifdef __SEH__
    try_table_back_handles *TryTableBackHandles;
#endif
};
