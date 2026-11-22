/* cpragx86_state.h -- the variables of cpragx86.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <ctype.h>
#include "cvars.h"
#include "i64.h"
#include "cgswitch.h"
#include "pdefn2.h"
#include "asmstmt.h"

struct Cpragx86State {
    aux_info AuxInfo;
#if _CPU == 386
    hw_reg_set STOSBParms[4] = {
        HW_D( HW_EAX ), HW_D( HW_EDX ), HW_D( HW_ECX ),
        HW_D( HW_EMPTY )
    };
#endif
    struct {
        bool     f_near          : 1;
        bool     f_routine_pops  : 1;
        bool     f_caller_return : 1;
        bool     f_8087_returns  : 1;
    } AuxInfoFlg;
};
