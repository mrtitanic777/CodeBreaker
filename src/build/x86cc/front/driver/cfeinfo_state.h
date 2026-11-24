/* cfeinfo_state.h -- the variables of cfeinfo.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <ctype.h>
#include "cgdefs.h"
#include "cg.h"
#include "cgswitch.h"
#include "pdefn2.h"
#include "compcfg.h"
#include <sys/stat.h>
#include "autodept.h"
#include "langenv.h"
#include "feprotos.h"
#include "cfeinfo.h"
#include "caux.h"
#include "dwarfid.h"
#include "felang.h"
#include "scan.h"

struct CfeinfoState {
#if _CPU == 386
    hw_reg_set TryFiniParms[2] = {
        HW_D( HW_EAX ),
        HW_D( HW_EMPTY )
    };
#endif
    hw_reg_set FEAuxInfo_save_set;
    unsigned char FEAuxInfo_Alignment[3] = { 2, 1, 1 };
};
