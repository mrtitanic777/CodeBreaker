/* callinfo_state.h -- the variables of callinfo.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <string.h>
#include <stdio.h>
#include "callinfo.h"

struct CallinfoFileState {
#if _INTEL_CPU
    hw_reg_set WatcallParms[2] = {
  #if _CPU == 8086
        HW_D_4( HW_AX, HW_BX, HW_CX, HW_DX )  /*+HW_ST1+HW_ST2+HW_ST3+HW_ST4*/,
  #else /* _CPU == 386 */
        HW_D_4( HW_EAX,HW_EBX,HW_ECX,HW_EDX ) /*+HW_ST1+HW_ST2+HW_ST3+HW_ST4*/,
  #endif
        HW_D( HW_EMPTY )
    };
#endif
#if _INTEL_CPU
    hw_reg_set StackParms[1] = {
        HW_D( HW_EMPTY )
    };
#endif
#if _INTEL_CPU
    #if _CPU == 386
    hw_reg_set FastcallParms[3] = {
  #if _CPU == 8086
        HW_D( HW_AX ), HW_D( HW_DX ), HW_D( HW_BX ),
  #else /* _CPU == 386 */
        HW_D( HW_ECX ), HW_D( HW_EDX ),
  #endif
        HW_D( HW_EMPTY )
    };
#else
    hw_reg_set FastcallParms[4] = {
  #if _CPU == 8086
        HW_D( HW_AX ), HW_D( HW_DX ), HW_D( HW_BX ),
  #else /* _CPU == 386 */
        HW_D( HW_ECX ), HW_D( HW_EDX ),
  #endif
        HW_D( HW_EMPTY )
    };
#endif
#endif
#if _INTEL_CPU
#if _CPU == 386
    hw_reg_set MetaWareParms[1] = {
        HW_D( HW_EMPTY )
    };
#endif
#endif
#if _INTEL_CPU
#if _CPU == 386
    hw_reg_set OptlinkParms[2] = {
        HW_D_4( HW_EAX, HW_ECX, HW_EDX, HW_FLTS ),
        HW_D( HW_EMPTY )
    };
#endif
#endif
};
