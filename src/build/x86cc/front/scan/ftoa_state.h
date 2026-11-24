/* ftoa_state.h -- the variables of ftoa.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <stdio.h>
#include <ctype.h>
#include "cvars.h"

struct FtoaState {
    char buf[80];
};
