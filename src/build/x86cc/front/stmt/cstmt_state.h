/* cstmt_state.h -- the variables of cstmt.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "i64.h"
#include "cfeinfo.h"
#include "toggles.h"

typedef struct block_entry {
    struct block_entry  *prev_block;
    struct block_entry  *prev_loop;
    TOKEN       block_type;
    LABEL_INDEX top_label;
    LABEL_INDEX break_label;
    LABEL_INDEX continue_label;
    tryindex_t  try_index;      /* TRY: current index */
    tryindex_t  parent_index;   /* TRY: parent index */
    TREEPTR     inc_var;        /* variable used in FOR statement */
    SYM_HANDLE  sym_list;       /* symbols defined in this block */
    bool        gen_endblock;   /* set if OPR_ENDBLOCK needed */
} BLOCKDEFN, *BLOCKPTR;
enum return_with {
    RETURN_WITH_NONE = 0,
    RETURN_WITH_NO_EXPR = 1,
    RETURN_WITH_EXPR = 2,
};
struct return_info {
    enum return_with    with;
    bool                with_expr;
};

struct CstmtState {
    TREEPTR LastStmt;
    BLOCKPTR BlockStack;
    BLOCKPTR LoopStack;
    SWITCHPTR SwitchStack;
};
