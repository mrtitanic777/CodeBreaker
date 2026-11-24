/* cdecl2_state.h -- the variables of cdecl2.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "cgswitch.h"
#include "i64.h"
#include "toggles.h"

struct Cdecl2State {
    segment_id thread_segid = SEG_NULL;
};
