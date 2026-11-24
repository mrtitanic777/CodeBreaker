/* rtrtn_state.h -- the variables of rtrtn.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "data.h"
#include "rtrtn.h"

struct RtrtnState {
    import_handle RTHdls[RTSIZE];
    label_handle RTLbls[RTSIZE];
};
