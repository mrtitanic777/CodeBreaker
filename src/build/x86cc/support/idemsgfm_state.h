/* idemsgfm_state.h -- the variables of idemsgfm.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "idedll.h"

struct IdemsgfmState {
    char contextual_file[ 144 ];   // name of contextual file
};
