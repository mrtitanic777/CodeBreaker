/* x86dbsup_state.h -- the variables of x86dbsup.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "procdef.h"
#include "model.h"
#include "zoiks.h"
#include "types.h"
#include "x86objd.h"
#include "objout.h"
#include "x86data.h"
#include "x86dbsup.h"

struct X86dbsupState {
    temp_buff *CurrBuff;
};
