/* optrel_state.h -- the variables of optrel.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "optwif.h"
#include "inslist.h"
#include "block.h"
#include "data.h"
#include "objout.h"
#include "optmain.h"
#include "encode.h"
#include "optutil.h"
#include "optmkins.h"
#include "optrel.h"

struct OptrelState {
    label_handle Handle;
};
