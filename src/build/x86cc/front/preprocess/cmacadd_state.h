/* cmacadd_state.h -- the variables of cmacadd.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <stddef.h>
#include "cmacadd.h"

typedef char        *MACADDR_T;
typedef struct macro_seg_list {
    struct macro_seg_list   *next;
    MACADDR_T               macro_seg;
} macro_seg_list;

struct CmacaddState {
    MACADDR_T MacroSegment;   /* segment for macro definitions */
    size_t MacroSegmentLimit;   /* remaining free bytes in MacroSegment */
    MACADDR_T MacroOffset;   /* first free byte in MacroSegment */
    macro_seg_list *MacSegList;   /* pointer to list of macro segments */
};
