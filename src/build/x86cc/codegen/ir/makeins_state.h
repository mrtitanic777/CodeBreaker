/* makeins_state.h -- the variables of makeins.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "freelist.h"
#include "data.h"
#include "makeins.h"
#include "namelist.h"
#include "optab.h"

struct MakeinsState {
    pointer *InsFrl;
};
