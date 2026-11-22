/* breakrtn_state.h -- the variables of breakrtn.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "data.h"
#include "makeblk.h"
#include "nullprop.h"
#include "blktrim.h"
#include "breakrtn.h"
#include "edge.h"

typedef struct  edge_list {
        block_edge              *edge;
        struct edge_list        *next;
        label_handle            lbl;
        block_id                gen_blk_id;
} edge_list;

struct BreakrtnState {
    block *Curr;
    block *Break;
    edge_list *BranchOuts;
    block *Tail = { NULL };
    bool HaveBreak = { false };
};
