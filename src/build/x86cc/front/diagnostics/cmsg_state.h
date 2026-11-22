/* cmsg_state.h -- the variables of cmsg.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <stddef.h>
#include "cvars.h"
#include "target.h"
#include "intlload.h"

struct CmsgState {
    IntlData *internationalData;   // translated messages
};
