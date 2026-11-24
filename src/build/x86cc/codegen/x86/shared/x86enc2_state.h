/* x86enc2_state.h -- the variables of x86enc2.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "optmain.h"
#include "cgmem.h"
#include "system.h"
#include "x86objd.h"
#include "pcencode.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "seldef.h"
#include "pccode.h"
#include "encode.h"
#include "data.h"
#include "types.h"
#include "utils.h"
#include "objout.h"
#include "object.h"
#include "intrface.h"
#include "targetin.h"
#include "targetdb.h"
#include "opttell.h"
#include "x87.h"
#include "x86esc.h"
#include "rgtbl.h"
#include "split.h"
#include "namelist.h"
#include "i64.h"
#include "x86enc.h"
#include "x86enc2.h"
#include "feprotos.h"

typedef enum {
    UNSIGNED,               /* always an unsigned jump */
    SIGNED_86,              /* signed if 8086 instruction, else unsigned */
    SIGNED_87,              /* signed if 8087 instruction, else unsigned */
    SIGNED_BOTH             /* always signed */
} issigned;

struct X86enc2State {
    unsigned char DepthAlign_AlignArray[10] = { 0 };
};
