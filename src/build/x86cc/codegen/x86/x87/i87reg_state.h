/* i87reg_state.h -- the variables of i87reg.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "data.h"
#include "makeins.h"
#include "namelist.h"
#include "redefby.h"
#include "regalloc.h"
#include "fpu.h"
#include "x87.h"
#include "insutil.h"
#include "rgtbl.h"
#include "inssegs.h"
#include "fixindex.h"
#include "conflict.h"
#include "escape.h"
#include "pcencode.h"
#include "liveinfo.h"
#include "x86regn.h"

struct I87regState {
    fp_patches FPPatchType;
    byte StackReq387[LAST_IFUNC - FIRST_IFUNC + 1] = {
    /********************************************************
        how much stack over and above the parm
        does the operation require? NB: this number + number parms must be <= 4
    */
        2,        /* OP_POW */
        0,        /* OP_ATAN2 */
        0,        /* OP_FMOD */
        0,        /* OP_NEGATE */
        0,        /* OP_COMPLEMENT */
        1,        /* OP_LOG */
        1,        /* OP_COS */
        1,        /* OP_SIN */
        1,        /* OP_TAN */
        0,        /* OP_SQRT */
        0,        /* OP_FABS */
        2,        /* OP_ACOS */
        2,        /* OP_ASIN */
        1,        /* OP_ATAN */
        2,        /* OP_COSH */
        2,        /* OP_EXP */
        1,        /* OP_LOG10 */
        2,        /* OP_SINH */
        2         /* OP_TANH */
    };
};
