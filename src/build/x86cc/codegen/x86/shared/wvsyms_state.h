/* wvsyms_state.h -- the variables of wvsyms.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "wvdbg.h"
#include "data.h"
#include "types.h"
#include "objout.h"
#include "wvsyms.h"
#include "objio.h"
#include "x86dbsup.h"
#include "dbsupp.h"
#include "wvtypes.h"
#include "x86objd.h"
#include "x86obj.h"
#include "wvsupp.h"
#include "feprotos.h"
#include "cgprotos.h"

struct WvsymsState {
    offset CodeOffset;
};
