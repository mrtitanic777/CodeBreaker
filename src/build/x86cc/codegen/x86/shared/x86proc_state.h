/* x86proc_state.h -- the variables of x86proc.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <stdio.h>
#include "_cgstd.h"
#include "coderep.h"
#include "cgaux.h"
#include "stackcg.h"
#include "zoiks.h"
#include "data.h"
#include "display.h"
#include "rtrtn.h"
#include "utils.h"
#include "objout.h"
#include "dbsyms.h"
#include "flowsave.h"
#include "object.h"
#include "x86enc2.h"
#include "encode.h"
#include "targetin.h"
#include "targetdb.h"
#include "x86proc.h"
#include "opttell.h"
#include "i87data.h"
#include "x86objd.h"
#include "x86obj.h"
#include "optab.h"
#include "rgtbl.h"
#include "x86base.h"
#include "pccode.h"
#include "pcencode.h"
#include "x86enc.h"
#include "x86temps.h"
#include "feprotos.h"

struct X86procFileState {
    bool ParmsReferenced = true;
};
