/* cexpr_state.h -- the variables of cexpr.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <ctype.h>
#include "cvars.h"
#include "cmacadd.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "i64.h"
#include "caux.h"
#include "cfeinfo.h"

struct CexprState {
    call_list **LastCallLnk;
    call_list **FirstCallLnk;
};
