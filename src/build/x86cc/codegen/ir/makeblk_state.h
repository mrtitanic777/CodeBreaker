/* makeblk_state.h -- the variables of makeblk.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "cgauxinf.h"
#include "zoiks.h"
#include "makeins.h"
#include "data.h"
#include "utils.h"
#include "stackcg.h"
#include "makeaddr.h"
#include "namelist.h"
#include "targetin.h"
#include "opttell.h"
#include "makeblk.h"
#include "typemap.h"
#include "blktrim.h"
#include "bgcall.h"
#include "edge.h"
#include "feprotos.h"

struct MakeblkState {
    label_handle LinkReturnsParms[2];
};
