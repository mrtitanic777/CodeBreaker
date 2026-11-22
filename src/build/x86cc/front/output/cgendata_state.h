/* cgendata_state.h -- the variables of cgendata.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "cgen.h"

struct CgendataState {
    segment_id EmitDQuad_segid = SEG_NULL;
    target_size EmitDQuad_size = 0;
};
