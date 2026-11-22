/* makeaddr_state.h -- the variables of makeaddr.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "freelist.h"
#include "makeins.h"
#include "data.h"
#include "types.h"
#include "addrfold.h"
#include "display.h"
#include "utils.h"
#include "makeaddr.h"
#include "namelist.h"
#include "makeblk.h"
#include "typemap.h"
#include "temps.h"
#include "bldins.h"
#include "feprotos.h"

struct MakeaddrState {
    pointer *AddrNameFrl;
};
