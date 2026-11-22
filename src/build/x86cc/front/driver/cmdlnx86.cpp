/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "cvars.h"
#include "cmdlnprs.gh"
#include "cmdlnprs.h"
#include "cmdscan.h"
#include "toggles.h"
#include "cmdlnsys.h"
#include "cgdefs.h"
#include "feprotos.h"

#include "clibext.h"
#include "frontrun.hpp"


#if !_INTEL_CPU
    #error cmdlx86.c - unsupported architecture
#endif

#if _CPU == 8086
    #define MX86    "M_I86"
    #define UMX86   "_M_I86"
    #define IMX86   "M_I86"
    #define UIMX86  "_M_I86"
#else
    #define MX86    "M_386"
    #define UMX86   "_M_386"
    #define IMX86   "M_I386"
    #define UIMX86  "_M_I386"
#endif

#ifdef DEVBUILD
    #define DbgNever()  (FEMessage( FEMSG_FATAL, "should never execute this" __location ))
#else
    #define DbgNever()
#endif

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct Cmdlnx86State {
    bool nd_used = false;
};
namespace {

}  // namespace



void CmdSysInit( FrontRun& front )
{
    SetAuxWatcallInfo(front);

    front.cvars.GenSwitches = CGSW_GEN_MEMORY_LOW_FAILS;
#if _CPU == 8086
    front.cvars.TargetSwitches = static_cast<cg_target_switches>(0);
    front.cvars.PackAmount = TARGET_INT;                /* pack structs on word boundaries */
#else
    front.cvars.TargetSwitches = CGSW_X86_USE_32;
    front.cvars.PackAmount = 8;
#endif
    front.cvars.GblPackAmount = front.cvars.PackAmount;
    front.cvars.DataThreshold = TARGET_INT_MAX;
    front.cvars.Inline_Threshold = 20;
    front.cvars.TextSegName = const_cast<char *>("");
    front.cvars.DataSegName = const_cast<char *>("");
    front.cvars.GenCodeGroup = const_cast<char *>("");
}

static char *setTargetSystem( FrontRun& front, OPT_STORAGE *data )
{
    char *target_name = NULL;

    /*
     * -bt specify a build target name
     */
    if( data->bt ) {
        SetTargetNameBT( front, &target_name, &(data->bt_value) );
    }
    /*
     * -zw overrides a build target name and setting
     */
    switch( data->win ) {
    case OPT_ENUM_win_zw:
        SetTargetName( front, &target_name, "WINDOWS" );
        break;
#if _CPU == 8086
    case OPT_ENUM_win_zW:
        SetTargetName( front, &target_name, "CHEAP_WINDOWS" );
        break;
    case OPT_ENUM_win_zws:
        SetTargetName( front, &target_name, "WINDOWS" );
        front.cvars.TargetSwitches |= CGSW_X86_SMART_WINDOWS;
        break;
    case OPT_ENUM_win_zWs:
        SetTargetName( front, &target_name, "CHEAP_WINDOWS" );
        front.cvars.TargetSwitches |= CGSW_X86_SMART_WINDOWS;
        break;
#endif
    case OPT_ENUM_win_default:
        break;
    default:
        DbgNever();
        break;
    }

    if( target_name == NULL ) {
#if defined( __NOVELL__ )
        SetTargetName( &target_name, "NETWARE" );
#elif defined( __QNX__ )
        SetTargetName( &target_name, "QNX" );
#elif defined( __LINUX__ )
        SetTargetName( &target_name, "LINUX" );
#elif defined( __HAIKU__ )
        SetTargetName( &target_name, "HAIKU" );
#elif defined( __SOLARIS__ ) || defined( __sun__ )
        SetTargetName( &target_name, "SOLARIS" );
#elif defined( __OSX__ ) || defined( __APPLE__ )
        SetTargetName( &target_name, "OSX" );
#elif defined( __OS2__ )
        SetTargetName( &target_name, "OS2" );
#elif defined( __NT__ )
        SetTargetName( front, &target_name, "NT" );
#elif defined( __DOS__ )
        SetTargetName( &target_name, "DOS" );
#elif defined( __BSD__ )
        SetTargetName( &target_name, "BSD" );
#elif defined( __RDOS__ )
        SetTargetName( &target_name, "RDOS" );
#else
        #error cmdlx86.c - Target OS not_ defined
#endif
    }

    if( target_name != NULL ) {
        if( strcmp( target_name, "DOS" ) == 0 ) {
            front.cvars.TargetSystem = TS_DOS;
        } else if( strcmp( target_name, "NETWARE" ) == 0 ) {
            front.cvars.TargetSystem = TS_NETWARE;
        } else if( strcmp( target_name, "NETWARE5" ) == 0 ) {
            front.cvars.TargetSystem = TS_NETWARE5;
            SetTargetName( front, &target_name, "NETWARE" );
        } else if( strcmp( target_name, "WINDOWS" ) == 0 ) {
            front.cvars.TargetSystem = TS_WINDOWS;
        } else if( strcmp( target_name, "CHEAP_WINDOWS" ) == 0 ) {
#if _CPU == 8086
            front.cvars.TargetSystem = TS_CHEAP_WINDOWS;
#else
            front.cvars.TargetSystem = TS_WINDOWS;
#endif
            SetTargetName( front, &target_name, "WINDOWS" );
        } else if( strcmp( target_name, "NT" ) == 0 ) {
            front.cvars.TargetSystem = TS_NT;
        } else if( strcmp( target_name, "LINUX" ) == 0 ) {
            front.cvars.TargetSystem = TS_LINUX;
        } else if( strcmp( target_name, "QNX" ) == 0 ) {
            front.cvars.TargetSystem = TS_QNX;
        } else if( strcmp( target_name, "OS2" ) == 0 ) {
            front.cvars.TargetSystem = TS_OS2;
        } else if( strcmp( target_name, "RDOS" ) == 0 ) {
            front.cvars.TargetSystem = TS_RDOS;
        } else if( strcmp( target_name, "HAIKU" ) == 0
                || strcmp( target_name, "OSX" ) == 0
                || strcmp( target_name, "SOLARIS" ) == 0
                || strcmp( target_name, "BSD" ) == 0 ) {
            front.cvars.TargetSystem = TS_UNIX;
        }
    }
    return( target_name );
}

static void setIntelArchitecture( FrontRun& front, OPT_STORAGE *data )
{
#if _CPU == 8086
    /*
     * Set CPU for 16-bit architecture, default is 8086
     */
    switch( data->arch_i86 ) {
    case OPT_ENUM_arch_i86_default:
    case OPT_ENUM_arch_i86__0:
        SET_CPU( front.cvars.ProcRevision, CPU_86 );
        break;
    case OPT_ENUM_arch_i86__1:
        SET_CPU( front.cvars.ProcRevision, CPU_186 );
        break;
    case OPT_ENUM_arch_i86__2:
        SET_CPU( front.cvars.ProcRevision, CPU_286 );
        break;
    case OPT_ENUM_arch_i86__3:
        SET_CPU( front.cvars.ProcRevision, CPU_386 );
        break;
    case OPT_ENUM_arch_i86__4:
        SET_CPU( front.cvars.ProcRevision, CPU_486 );
        break;
    case OPT_ENUM_arch_i86__5:
        SET_CPU( front.cvars.ProcRevision, CPU_586 );
        break;
    case OPT_ENUM_arch_i86__6:
        SET_CPU( front.cvars.ProcRevision, CPU_686 );
        break;
    default:
        DbgNever();
        break;
    }
#else
    /*
     * Set CPU for 32-bit architecture, default is 80686
     */
    switch( data->arch_386 ) {
    case OPT_ENUM_arch_386__3s:
        SET_CPU( front.cvars.ProcRevision, CPU_386 );
        break;
    case OPT_ENUM_arch_386__3r:
        front.cvars.CompFlags.register_conventions = true;
        SET_CPU( front.cvars.ProcRevision, CPU_386 );
        break;
    case OPT_ENUM_arch_386__4s:
        SET_CPU( front.cvars.ProcRevision, CPU_486 );
        break;
    case OPT_ENUM_arch_386__4r:
        front.cvars.CompFlags.register_conventions = true;
        SET_CPU( front.cvars.ProcRevision, CPU_486 );
        break;
    case OPT_ENUM_arch_386__5s:
        SET_CPU( front.cvars.ProcRevision, CPU_586 );
        break;
    case OPT_ENUM_arch_386__5r:
        front.cvars.CompFlags.register_conventions = true;
        SET_CPU( front.cvars.ProcRevision, CPU_586 );
        break;
    case OPT_ENUM_arch_386__6s:
        SET_CPU( front.cvars.ProcRevision, CPU_686 );
        break;
    case OPT_ENUM_arch_386_default:
    case OPT_ENUM_arch_386__6r:
        front.cvars.CompFlags.register_conventions = true;
        SET_CPU( front.cvars.ProcRevision, CPU_686 );
        break;
    default:
        DbgNever();
        break;
    }
    /*
     * NETWARE uses stack based calling conventions
     * by default - silly people.
     */
    if( front.cvars.TargetSystem == TS_NETWARE
      || front.cvars.TargetSystem == TS_NETWARE5 ) {
        front.cvars.CompFlags.register_conventions = false;
    }
#endif

    /*
     * Set FPU version, default is 8087/80387
     */
    switch( data->intel_fpu_level ) {
    case OPT_ENUM_intel_fpu_level_fp6:
        SET_FPU_LEVEL( front.cvars.ProcRevision, FPU_686 );
        break;
    case OPT_ENUM_intel_fpu_level_fp5:
        SET_FPU_LEVEL( front.cvars.ProcRevision, FPU_586 );
        break;
#if _CPU == 386
    case OPT_ENUM_intel_fpu_level_default:
#endif
    case OPT_ENUM_intel_fpu_level_fp3:
        SET_FPU_LEVEL( front.cvars.ProcRevision, FPU_387 );
        break;
#if _CPU == 8086
    case OPT_ENUM_intel_fpu_level_default:
#endif
    case OPT_ENUM_intel_fpu_level_fp2:
        SET_FPU_LEVEL( front.cvars.ProcRevision, FPU_87 );
        break;
    default:
        DbgNever();
        break;
    }
    /*
     * Set FPU model, default is fpi
     */
    switch( data->intel_fpu_model ) {
    case OPT_ENUM_intel_fpu_model_default:
    case OPT_ENUM_intel_fpu_model_fpi:
#if _CPU == 386
        if( front.cvars.TargetSystem != TS_WINDOWS ) {
#endif
            SET_FPU_EMU( front.cvars.ProcRevision );
            break;
#if _CPU == 386
        }
        /* fall through */
#endif
    case OPT_ENUM_intel_fpu_model_fpi87:
        SET_FPU_INLINE( front.cvars.ProcRevision );
        break;
    case OPT_ENUM_intel_fpu_model_fpc:
        SET_FPU_FPC( front.cvars.ProcRevision );
        front.cvars.CompFlags.op_switch_used = false;
        break;
    default:
        DbgNever();
        break;
    }
    /*
     * If using NetWare, set Stack87 unless the target
     * is NetWare 5 or higher.
     */
    if( data->fpr ) {
        front.cvars.Stack87 = 4;
    } else if( front.cvars.TargetSystem == TS_NETWARE ) {
        front.cvars.Stack87 = 4;
    } else {
        front.cvars.Stack87 = 8;
    }

    if( data->fld ) {
        front.cvars.CompFlags.use_long_double = true;
    }
    if( data->fpd ) {
        front.cvars.TargetSwitches |= CGSW_X86_P5_DIVIDE_CHECK;
    }
    if( data->zfw ) {
        front.cvars.TargetSwitches |= CGSW_X86_GEN_FWAIT_386;
    }
#if _CPU == 8086
    if( data->zmsc ) {
        front.cvars.TargetSwitches |= CGSW_X86_MSC8;
    }
#endif
    switch( data->fp_rounding ) {
    case OPT_ENUM_fp_rounding_zri:
        front.cvars.GenSwitches |= CGSW_GEN_FPU_ROUNDING_INLINE;
        break;
    case OPT_ENUM_fp_rounding_zro:
        front.cvars.GenSwitches |= CGSW_GEN_FPU_ROUNDING_OMIT;
        break;
    case OPT_ENUM_fp_rounding_default:
        break;
    default:
        DbgNever();
        break;
    }
}

static void SetDftCallConv( FrontRun& front, OPT_STORAGE *data )
{
    /*
     * default is Watcom calling convention
     */
    switch( data->intel_call_conv ) {
    case OPT_ENUM_intel_call_conv_ecc:
        front.callinfo.DftCallConv = &CdeclInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecd:
        front.callinfo.DftCallConv = &StdcallInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecf:
        front.callinfo.DftCallConv = &FastcallInfo;
        break;
    case OPT_ENUM_intel_call_conv_eco:
        front.callinfo.DftCallConv = &OptlinkInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecp:
        front.callinfo.DftCallConv = &PascalInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecr:
        front.callinfo.DftCallConv = &FortranInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecs:
        front.callinfo.DftCallConv = &SyscallInfo;
        break;
    case OPT_ENUM_intel_call_conv_default:
    case OPT_ENUM_intel_call_conv_ecw:
        front.callinfo.DftCallConv = &WatcallInfo;
        break;
    default:
        DbgNever();
        break;
    }
}

static void SetFinalTargetSystem( FrontRun& front, OPT_STORAGE *data, char *target_name )
{
    char buff[128];

    /* unused parameters */ (void)data;

    if( target_name != NULL ) {
        sprintf( buff, "__%s__", target_name );
        PreDefine_Macro( front, buff );
    }

    PreDefine_Macro( front, UIMX86 );
    if( front.cvars.CompFlags.non_iso_compliant_names_enabled ) {
        PreDefine_Macro( front, IMX86 );
    }
    PreDefine_Macro( front, "__X86__" );
    PreDefine_Macro( front, "_X86_" );
#if _CPU == 8086
    PreDefine_Macro( front, "__I86__" );
#else
    PreDefine_Macro( front, "__386__" );
    PreDefine_Macro( front, "_STDCALL_SUPPORTED" );
#endif
    PreDefine_Macro( front, "__WATCOM_INT64__" );
    PreDefine_Macro( front, "_INTEGRAL_MAX_BITS=64" );

    switch( front.cvars.TargetSystem ) {
    case TS_DOS:
        PreDefine_Macro( front, "_DOS" );
        if( front.cvars.CompFlags.non_iso_compliant_names_enabled ) {
            PreDefine_Macro( front, "MSDOS" );
        }
        break;
    case TS_QNX:
        PreDefine_Macro( front, "__UNIX__" );
        break;
    case TS_CHEAP_WINDOWS:
        PreDefine_Macro( front, "__CHEAP_WINDOWS__" );
        front.cvars.TargetSwitches |= CGSW_X86_CHEAP_WINDOWS;
        /* fall through */
    case TS_WINDOWS:
#if _CPU == 8086
        PreDefine_Macro( front, "_WINDOWS" );
        front.cvars.TargetSwitches |= CGSW_X86_WINDOWS;
#else
        PreDefine_Macro( front, "__WINDOWS_386__" );
#endif
        break;
#if _CPU == 386
    case TS_NETWARE5:
        PreDefine_Macro( front, "__NETWARE5__" );
        /* fall through */
    case TS_NETWARE:
        PreDefine_Macro( front, "__NETWARE_386__" );
        break;
    case TS_RDOS:
        PreDefine_Macro( front, "_RDOS" );
        break;
    case TS_NT:
        PreDefine_Macro( front, "_WIN32" );
        break;
    case TS_LINUX:
    case TS_UNIX:
        PreDefine_Macro( front, "__UNIX__" );
        break;
#endif
    default:
        break;
    }

    if( GET_CPU( front.cvars.ProcRevision ) < CPU_386 ) {
        /*
         * issue warning message if /zf[f|p] or /zg[f|p] spec'd?
         */
        front.cvars.TargetSwitches &= ~(CGSW_X86_FLOATING_FS | CGSW_X86_FLOATING_GS);
    }
    if( !front.cvars.CompFlags.save_restore_segregs ) {
        if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_DS ) {
            HW_CTurnOff( WatcallInfo.save, HW_DS );
        }
        if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_ES ) {
            HW_CTurnOff( WatcallInfo.save, HW_ES );
        }
        if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_FS ) {
            HW_CTurnOff( WatcallInfo.save, HW_FS );
        }
        if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_GS ) {
            HW_CTurnOff( WatcallInfo.save, HW_GS );
        }
    }
    if( data->of_plus ) {
        WatcallInfo.cclass_target |= FECALL_X86_GENERATE_STACK_FRAME;
    }
  #if _CPU == 386
    if( !front.cvars.CompFlags.register_conventions ) {
        SetAuxStackConventions(front);
    }
  #endif

    if( target_name != NULL ) {
        strcpy( buff, target_name );
        strcat( buff, "_INCLUDE" );
        MergeIncludeFromEnv( front, buff );
    }
#if _CPU == 8086
    MergeIncludeFromEnv( front, "INCLUDE" );
#else
    if( !MergeIncludeFromEnv( front, "INC386" ) ) {
        MergeIncludeFromEnv( front, "INCLUDE" );
    }
#endif

    CMemFree( front, target_name );
}

static void SetDebugInfoFormat( FrontRun& front, OPT_STORAGE *data )
{
    front.cvars.GenSwitches &= ~(CGSW_GEN_DBG_CV | CGSW_GEN_DBG_DF | CGSW_GEN_DBG_PREDEF);
    switch( data->dbg_output ) {
    case OPT_ENUM_dbg_output_hc:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_CV;
        break;
    case OPT_ENUM_dbg_output_hw:
        break;
    case OPT_ENUM_dbg_output_hda:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_DF | CGSW_GEN_DBG_PREDEF;
        front.cvars.SymDFAbbr = SpcSymbol( front, "__DFABBREV", GetType( front, TYP_USHORT ), SC_EXTERN );
        break;
    case OPT_ENUM_dbg_output_hdg:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_DF | CGSW_GEN_DBG_PREDEF;
        front.cvars.SymDFAbbr = SpcSymbol( front, "__DFABBREV", GetType( front, TYP_USHORT ), SC_NONE );
        break;
    case OPT_ENUM_dbg_output_hd:
    case OPT_ENUM_dbg_output_default:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_DF;
        break;
    default:
        DbgNever();
        break;
    }
}

static void SetGenSwitches( FrontRun& front, OPT_STORAGE *data )
{
#if _CPU == 386
    if( data->br ) {
        front.cvars.CompFlags.br_switch_used = true;
    }
#endif
    if( data->ec ) {
        front.cvars.CompFlags.ec_switch_used = true;
    }
    if( data->ee ) {
        front.cvars.CompFlags.ee_switch_used = true;
    }
    if( data->eoo ) {
        front.cvars.GenSwitches &= ~(CGSW_GEN_OBJ_ELF | CGSW_GEN_OBJ_COFF);
    }
#if _CPU == 386
    if( data->eoe ) {
        front.cvars.GenSwitches = (front.cvars.GenSwitches & ~CGSW_GEN_OBJ_COFF) | CGSW_GEN_OBJ_ELF;
    }
    if( data->eoc ) {
        front.cvars.GenSwitches = (front.cvars.GenSwitches & ~CGSW_GEN_OBJ_ELF) | CGSW_GEN_OBJ_COFF;
    }
#endif
    if( data->eq ) {
        front.cvars.CompFlags.eq_switch_used = true;
    }
#if _CPU == 386
    if( data->et ) {
        front.cvars.TargetSwitches |= CGSW_X86_P5_PROFILING;
    }
    if( data->esp ) {
        front.cvars.TargetSwitches |= CGSW_X86_STATEMENT_COUNTING;
    }
    if( data->et0 ) {
        front.cvars.TargetSwitches |= CGSW_X86_P5_PROFILING | CGSW_X86_P5_PROFILING_CTR0;
    }
    if( data->etp ) {
        front.cvars.TargetSwitches |= CGSW_X86_NEW_P5_PROFILING;
    }
    if( data->ez ) {
        front.cvars.TargetSwitches |= CGSW_X86_EZ_OMF;
    }
#endif
    if( data->g ) {
        SetStringOption( front, &front.cvars.GenCodeGroup, &(data->g_value) );
    }
    if( data->nc ) {
        SetStringOption( front, &front.cvars.CodeClassName, &(data->nc_value) );
    }
    if( data->nd ) {
        front.cmdlnx86->nd_used = true;
        SetStringOption( front, &front.cvars.DataSegName, &(data->nd_value) );
        ImportNearSegIdInit(front);
        if( *front.cvars.DataSegName == '\0' ) {
            CMemFree( front, front.cvars.DataSegName );
            front.cvars.DataSegName = NULL;
        }
    }
    if( data->nm ) {
        SetStringOption( front, &front.cvars.ModuleName, &(data->nm_value) );
    }
    if( data->nt ) {
        SetStringOption( front, &front.cvars.TextSegName, &(data->nt_value) );
    }
    if( data->oc ) {
        front.cvars.GenSwitches |= CGSW_GEN_NO_CALL_RET_TRANSFORM;
    }
    if( data->of ) {
        front.cvars.TargetSwitches |= CGSW_X86_NEED_STACK_FRAME;
    }
    if( data->of_plus ) {
        front.cvars.TargetSwitches |= CGSW_X86_NEED_STACK_FRAME;
    }
    if( data->om ) {
        front.cvars.GenSwitches |= CGSW_GEN_I_MATH_INLINE;
    }
    if( data->op ) {
        front.cvars.CompFlags.op_switch_used = true;
    }
    if( data->r ) {
        front.cvars.CompFlags.save_restore_segregs = true;
    }
#if _CPU == 386
    if( data->re ) {
        front.cvars.CompFlags.rent = true;
    }
#endif
    if( data->ri ) {
        front.cvars.CompFlags.returns_promoted = true;
    }
    if( data->sg ) {
        front.cvars.CompFlags.sg_switch_used = true;
    }
    if( data->st ) {
        front.cvars.CompFlags.st_switch_used = true;
    }
#if _CPU == 8086
    if( data->wo ) {
        front.cvars.CompFlags.using_overlays = true;
    }
#endif
#if _CPU == 386
    if( data->xgv ) {
        front.cvars.TargetSwitches |= CGSW_X86_INDEXED_GLOBALS;
    }
#endif
    if( data->zc ) {
        front.cvars.CompFlags.strings_in_code_segment = true;
        front.cvars.CompFlags.zc_switch_used = true;
        front.cvars.TargetSwitches |= CGSW_X86_CONST_IN_CODE;
    }
    if( data->zm ) {
        front.cvars.CompFlags.multiple_code_segments = true;
        front.cvars.CompFlags.zm_switch_used = true;
    }
    if( data->zt ) {
        front.cvars.DataThreshold = data->zt_value;
    }
    if( data->zu ) {
        front.cvars.CompFlags.zu_switch_used = true;
    }
#if _CPU == 386
    if( data->zz ) {
        front.cvars.CompFlags.use_stdcall_at_number = false;
    }
#endif
}

static void PreDefine_UMIX86_Macro( FrontRun& front )
{
    unsigned    cpu;
    char        buff[32];

    switch( GET_CPU( front.cvars.ProcRevision ) ) {
    case CPU_86:        cpu = 0;   break;
    case CPU_186:       cpu = 100; break;
    case CPU_286:       cpu = 200; break;
    case CPU_386:       cpu = 300; break;
    case CPU_486:       cpu = 400; break;
    case CPU_586:       cpu = 500; break;
    case CPU_686:       cpu = 600; break;
    default:            cpu = 0;   break;
    }
    sprintf( buff, "_M_IX86=%u", cpu );
    PreDefine_Macro( front, buff );
}

static void MacroDefs( FrontRun& front, OPT_STORAGE *data )
{
    if( front.cvars.GenSwitches & CGSW_GEN_I_MATH_INLINE ) {
        DefSwitchMacro( front, "OM" );
    }
    switch( data->mem_model ) {
    case OPT_ENUM_mem_model_ms:
        DefSwitchMacro( front, "MS" );
        PreDefine_Macro( front, UMX86 "SM" );
        PreDefine_Macro( front, "__SMALL__" );
        break;
    case OPT_ENUM_mem_model_mm:
        DefSwitchMacro( front, "MM" );
        PreDefine_Macro( front, UMX86 "MM" );
        PreDefine_Macro( front, "__MEDIUM__" );
        break;
    case OPT_ENUM_mem_model_mc:
        DefSwitchMacro( front, "MC" );
        PreDefine_Macro( front, UMX86 "CM" );
        PreDefine_Macro( front, "__COMPACT__" );
        break;
    case OPT_ENUM_mem_model_ml:
        DefSwitchMacro( front, "ML" );
        PreDefine_Macro( front, UMX86 "LM" );
        PreDefine_Macro( front, "__LARGE__" );
        break;
#if _CPU == 8086
    case OPT_ENUM_mem_model_mh:
        DefSwitchMacro( front, "MH" );
        PreDefine_Macro( front, UMX86 "HM" );
        PreDefine_Macro( front, "__HUGE__" );
        break;
#else
    case OPT_ENUM_mem_model_mf:
        DefSwitchMacro( front, "MF" );
        PreDefine_Macro( front, UMX86 "FM" );
        PreDefine_Macro( front, "__FLAT__" );
        break;
#endif
    default:
        break;
    }
    if( front.cvars.CompFlags.non_iso_compliant_names_enabled ) {
        switch( data->mem_model ) {
        case OPT_ENUM_mem_model_ms:
            PreDefine_Macro( front, MX86 "SM" );
            break;
        case OPT_ENUM_mem_model_mm:
            PreDefine_Macro( front, MX86 "MM" );
            break;
        case OPT_ENUM_mem_model_mc:
            PreDefine_Macro( front, MX86 "CM" );
            break;
        case OPT_ENUM_mem_model_ml:
            PreDefine_Macro( front, MX86 "LM" );
            break;
#if _CPU == 8086
        case OPT_ENUM_mem_model_mh:
            PreDefine_Macro( front, MX86 "HM" );
            break;
#else
        case OPT_ENUM_mem_model_mf:
            PreDefine_Macro( front, MX86 "FM" );
            break;
#endif
        default:
            break;
        }
    }

    if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_FS ) {
        DefSwitchMacro( front, "ZFF" );
    } else {
        DefSwitchMacro( front, "ZFP" );
    }
    if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_GS ) {
        DefSwitchMacro( front, "ZGF" );
    } else {
        DefSwitchMacro( front, "ZGP" );
    }
    if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_DS ) {
        DefSwitchMacro( front, "ZDF" );
    } else {
        DefSwitchMacro( front, "ZDP" );
    }
    if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_SS ) {
        DefSwitchMacro( front, "ZU" );
    }
    if( front.cvars.TargetSwitches & CGSW_X86_INDEXED_GLOBALS ) {
        DefSwitchMacro( front, "XGV" );
    }
    if( front.cvars.TargetSwitches & CGSW_X86_WINDOWS ) {
        DefSwitchMacro( front, "ZW" );
    }
    if( front.cvars.TargetSwitches & CGSW_X86_NEED_STACK_FRAME ) {
        DefSwitchMacro( front, "OF" );
    }
    if( front.cvars.TargetSwitches & CGSW_X86_GEN_FWAIT_386 ) {
        DefSwitchMacro( front, "ZFW" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_NO_CALL_RET_TRANSFORM ) {
        DefSwitchMacro( front, "OC" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_SUPER_OPTIMAL ) {
        DefSwitchMacro( front, "OH" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_FLOW_REG_SAVES ) {
        DefSwitchMacro( front, "OK" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION ) {
        DefSwitchMacro( front, "OD" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_RELAX_ALIAS ) {
        DefSwitchMacro( front, "OA" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_LOOP_OPTIMIZATION ) {
        DefSwitchMacro( front, "OL" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_INS_SCHEDULING ) {
        DefSwitchMacro( front, "OR" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_FP_UNSTABLE_OPTIMIZATION ) {
        DefSwitchMacro( front, "ON" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_FPU_ROUNDING_OMIT ) {
        DefSwitchMacro( front, "ZRO" );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_FPU_ROUNDING_INLINE ) {
        DefSwitchMacro( front, "ZRI" );
    }
    if( front.cvars.CompFlags.use_long_double ) {
        DefSwitchMacro( front, "FLD" );
    }
    if( front.cvars.CompFlags.signed_char ) {
        DefSwitchMacro( front, "J" );
    }
    if( front.cvars.PCH_FileName != NULL ) {
        DefSwitchMacro( front, "FH" );
    }
    if( front.cvars.CompFlags.no_pch_warnings ) {
        DefSwitchMacro( front, "FHQ" );
    }
    if( front.cvars.CompFlags.inline_functions ) {
        DefSwitchMacro( front, "OI" );
    }
    if( front.cvars.CompFlags.unique_functions ) {
        DefSwitchMacro( front, "OU" );
    }
#if _CPU == 386
    if( front.cvars.CompFlags.register_conventions ) {
        DefSwitchMacro( front, "3R" );
    } else {
        DefSwitchMacro( front, "3S" );
    }
#endif
    if( front.cvars.CompFlags.emit_names ) {
        DefSwitchMacro( front, "EN" );
    }
    if( front.cvars.CompFlags.make_enums_an_int ) {
        DefSwitchMacro( front, "EI" );
    }
    if( front.cvars.CompFlags.zc_switch_used ) {
        DefSwitchMacro( front, "ZC" );
    }
    if( front.cvars.CompFlags.use_double_byte ) {
        DefSwitchMacro( front, "ZK" );
    }
    if( front.cvars.CompFlags.save_restore_segregs ) {
        DefSwitchMacro( front, "R" );
    }
    if( front.cvars.CompFlags.sg_switch_used ) {
        DefSwitchMacro( front, "SG" );
    }
    if( front.cvars.CompFlags.st_switch_used ) {
        DefSwitchMacro( front, "ST" );
    }
    if( front.cvars.CompFlags.zu_switch_used ) {
        DefSwitchMacro( front, "ZU" );
    }
    if( front.cvars.CompFlags.bm_switch_used ) {
        DefSwitchMacro( front, "BM" );
        Define_Macro( front, "_MT" );
    }
    if( front.cvars.CompFlags.bd_switch_used ) {
        DefSwitchMacro( front, "BD" );
    }
    if( front.cvars.CompFlags.bc_switch_used ) {
        DefSwitchMacro( front, "BC" );
    }
    if( front.cvars.CompFlags.bg_switch_used ) {
        DefSwitchMacro( front, "BG" );
    }
    if( front.cvars.CompFlags.br_switch_used ) {
        DefSwitchMacro( front, "BR" );
        Define_Macro( front, "_DLL" );
    }
    if( front.cvars.CompFlags.bw_switch_used ) {
        DefSwitchMacro( front, "BW" );
    }
    if( front.cvars.CompFlags.zm_switch_used ) {
        DefSwitchMacro( front, "ZM" );
    }
    if( front.cvars.CompFlags.ep_switch_used ) {
        DefSwitchMacro( front, "EP" );
    }
    if( front.cvars.CompFlags.ee_switch_used ) {
        DefSwitchMacro( front, "EE" );
    }
    if( front.cvars.CompFlags.ec_switch_used ) {
        DefSwitchMacro( front, "EC" );
    }

    switch( GET_CPU( front.cvars.ProcRevision ) ) {
    case CPU_86:
        DefSwitchMacro( front, "0" );
        break;
    case CPU_186:
        DefSwitchMacro( front, "1" );
        break;
    case CPU_286:
        DefSwitchMacro( front, "2" );
        break;
    case CPU_386:
        DefSwitchMacro( front, "3" );
        break;
    case CPU_486:
        DefSwitchMacro( front, "4" );
        break;
    case CPU_586:
        DefSwitchMacro( front, "5" );
        break;
    case CPU_686:
        DefSwitchMacro( front, "6" );
        break;
    default:
        break;
    }

    PreDefine_UMIX86_Macro(front);

    switch( GET_FPU_LEVEL( front.cvars.ProcRevision ) ) {
    case FPU_87:
        DefSwitchMacro( front, "FP2" );
        break;
    case FPU_387:
        DefSwitchMacro( front, "FP3" );
        break;
    case FPU_586:
        DefSwitchMacro( front, "FP5" );
        break;
    case FPU_686:
        DefSwitchMacro( front, "FP6" );
        break;
    default:
        break;
    }
    if( GET_FPU_FPC( front.cvars.ProcRevision ) ) {
        DefSwitchMacro( front, "FPC" );
    } else {
        if( GET_FPU_EMU( front.cvars.ProcRevision ) ) {
            DefSwitchMacro( front, "FPI" );
        } else {
            DefSwitchMacro( front, "FPI87" );
        }
        PreDefine_Macro( front, "__FPI__" );
    }

    if( front.cmdlnx86->nd_used ) {
        DefSwitchMacro( front, "ND" );
    }
    if( front.cvars.CompFlags.op_switch_used ) {
        DefSwitchMacro( front, "OP" );
    }
    if( !TOGGLE( check_stack ) ) {
        DefSwitchMacro( front, "S" );
    }
}

static void Define_Memory_Model( FrontRun& front, OPT_STORAGE *data )
{
    char lib_model = '\0';
    unsigned bit;

    front.cvars.DataPtrSize = TARGET_POINTER;
    front.cvars.CodePtrSize = TARGET_POINTER;
    bit = 0;
    if( data->mem_model == OPT_ENUM_mem_model_default ) {
#if _CPU == 8086
        data->mem_model = OPT_ENUM_mem_model_ms;
#else
        if( front.cvars.TargetSystem == TS_NETWARE
          || front.cvars.TargetSystem == TS_NETWARE5 ) {
            data->mem_model = OPT_ENUM_mem_model_ms;
        } else {
            data->mem_model = OPT_ENUM_mem_model_mf;
        }
#endif
    }
    switch( data->mem_model ) {
    case OPT_ENUM_mem_model_ms:
        lib_model = 's';
        front.cvars.CompFlags.strings_in_code_segment = false;
        front.cvars.TargetSwitches &= ~CGSW_X86_CONST_IN_CODE;
        bit |= CGSW_X86_CHEAP_POINTER;
        break;
    case OPT_ENUM_mem_model_mm:
        lib_model = 'm';
        WatcallInfo.cclass_target |= FECALL_X86_FAR_CALL;
        front.cvars.CodePtrSize = TARGET_FAR_POINTER;
        front.cvars.CompFlags.strings_in_code_segment = false;
        front.cvars.TargetSwitches &= ~CGSW_X86_CONST_IN_CODE;
        bit |= CGSW_X86_BIG_CODE | CGSW_X86_CHEAP_POINTER;
        break;
    case OPT_ENUM_mem_model_mc:
        lib_model = 'c';
        front.cvars.DataPtrSize = TARGET_FAR_POINTER;
        bit |= CGSW_X86_BIG_DATA | CGSW_X86_CHEAP_POINTER;
        break;
    case OPT_ENUM_mem_model_ml:
        lib_model = 'l';
        WatcallInfo.cclass_target |= FECALL_X86_FAR_CALL;
        front.cvars.CodePtrSize = TARGET_FAR_POINTER;
        front.cvars.DataPtrSize = TARGET_FAR_POINTER;
        bit |= CGSW_X86_BIG_CODE | CGSW_X86_BIG_DATA | CGSW_X86_CHEAP_POINTER;
        break;
#if _CPU == 8086
    case OPT_ENUM_mem_model_mh:
        lib_model = 'h';
        WatcallInfo.cclass_target |= FECALL_X86_FAR_CALL;
        front.cvars.CodePtrSize = TARGET_FAR_POINTER;
        front.cvars.DataPtrSize = TARGET_FAR_POINTER;
        bit |= CGSW_X86_BIG_CODE | CGSW_X86_BIG_DATA;
        break;
#else
    case OPT_ENUM_mem_model_mf:
        lib_model = 's';
        bit |= CGSW_X86_FLAT_MODEL | CGSW_X86_CHEAP_POINTER;
        break;
#endif
    default:
        DbgNever();
    }

    /*
     * setup "floating" segment registers
     * any memory model needs at least one floating segment register
     * by default it is ES segment register for 16-bit code and
     * GS segment register for 32-bit code
     */

    /*
     * setup SS segment register floating
     */
    if( data->zu ) {
        bit |= CGSW_X86_FLOATING_SS;
    }
    /*
     * setup ES segment register floating
     */
    if( (bit & CGSW_X86_FLAT_MODEL) == 0 ) {
        bit |= CGSW_X86_FLOATING_ES;
    }
    /*
     * setup DS segment register floating
     */
    if( bit & CGSW_X86_BIG_DATA ) {
        bit |= CGSW_X86_FLOATING_DS;
    }
#if _CPU == 386
    /*
     * 32-bit code with zdl option has DS pegged
     */
    if( data->zdl ) {
        front.cvars.TargetSwitches |= CGSW_X86_LOAD_DS_DIRECTLY;
        bit &= ~CGSW_X86_FLOATING_DS;
    }
#endif
#if _CPU == 8086
    /*
     * any model of 16-bit Windows has DS pegged
     */
    if( front.cvars.TargetSystem == TS_WINDOWS
      || front.cvars.TargetSystem == TS_CHEAP_WINDOWS ) {
        bit &= ~CGSW_X86_FLOATING_DS;
    }
#endif
    switch( data->ds_peg ) {
    case OPT_ENUM_ds_peg_zdp:
        front.cvars.TargetSwitches &= ~CGSW_X86_FLOATING_DS;
        bit &= ~CGSW_X86_FLOATING_DS;
        break;
    case OPT_ENUM_ds_peg_zdf:
        front.cvars.TargetSwitches |= CGSW_X86_FLOATING_DS;
        bit &= ~CGSW_X86_FLOATING_DS;
        break;
    case OPT_ENUM_ds_peg_default:
        break;
    default:
        DbgNever();
        break;
    }
    /*
     * setup FS segment register floating
     *
     * FS segment register is used by 32-bit FLAT memory model
     * except Windows 32-bit Extender
     */
    if( GET_CPU( front.cvars.ProcRevision ) >= CPU_386 ) {
#if _CPU == 8086
        bit |= CGSW_X86_FLOATING_FS;
#else
        if( (bit & CGSW_X86_FLAT_MODEL) == 0
          || front.cvars.TargetSystem == TS_WINDOWS ) {
            bit |= CGSW_X86_FLOATING_FS;
        }
#endif
    }
    switch( data->fs_peg ) {
    case OPT_ENUM_fs_peg_zfp:
        front.cvars.TargetSwitches &= ~CGSW_X86_FLOATING_FS;
        bit &= ~CGSW_X86_FLOATING_FS;
        break;
    case OPT_ENUM_fs_peg_zff:
        front.cvars.TargetSwitches |= CGSW_X86_FLOATING_FS;
        bit &= ~CGSW_X86_FLOATING_FS;
        break;
    case OPT_ENUM_fs_peg_default:
        break;
    default:
        DbgNever();
        break;
    }
    /*
     * setup GS segment register floating
     */
    if( GET_CPU( front.cvars.ProcRevision ) >= CPU_386 ) {
        bit |= CGSW_X86_FLOATING_GS;
    }
    switch( data->gs_peg ) {
    case OPT_ENUM_gs_peg_zgp:
        front.cvars.TargetSwitches &= ~CGSW_X86_FLOATING_GS;
        bit &= ~CGSW_X86_FLOATING_GS;
        break;
    case OPT_ENUM_gs_peg_zgf:
        front.cvars.TargetSwitches |= CGSW_X86_FLOATING_GS;
        bit &= ~CGSW_X86_FLOATING_GS;
        break;
    case OPT_ENUM_gs_peg_default:
        break;
    default:
        DbgNever();
        break;
    }

    front.cvars.TargetSwitches &= ~(CGSW_X86_FLAT_MODEL | CGSW_X86_BIG_CODE | CGSW_X86_BIG_DATA | CGSW_X86_CHEAP_POINTER);
    front.cvars.TargetSwitches |= static_cast<cg_target_switches>(bit);

    if( GET_CPU( front.cvars.ProcRevision ) < CPU_386 ) {
        /*
         * issue warning message if /zf[f|p] or /zg[f|p] spec'd?
         */
        front.cvars.TargetSwitches &= ~(CGSW_X86_FLOATING_FS | CGSW_X86_FLOATING_GS);
    }

#if _CPU == 8086
    strcpy( front.cvars.CLIB_Name, "1clib?" );
    if( front.cvars.CompFlags.bm_switch_used ) {
        strcpy( front.cvars.CLIB_Name, "1clibmt?" );
    }
    if( front.cvars.CompFlags.bd_switch_used ) {
        if( front.cvars.TargetSystem == TS_WINDOWS || front.cvars.TargetSystem == TS_CHEAP_WINDOWS ) {
            strcpy( front.cvars.CLIB_Name, "1clib?" );
        } else {
            strcpy( front.cvars.CLIB_Name, "1clibdl?" );
        }
    }
    if( GET_FPU_FPC( front.cvars.ProcRevision ) ) {
        strcpy( front.cvars.MATHLIB_Name, "5math?" );
        front.cvars.EmuLib_Name = NULL;
    } else {
        strcpy( front.cvars.MATHLIB_Name, "7math87?" );
        if( GET_FPU_EMU( front.cvars.ProcRevision ) ) {
            front.cvars.EmuLib_Name = const_cast<char *>("8emu87");
        } else {
            front.cvars.EmuLib_Name = const_cast<char *>("8noemu87");
        }
    }
#else
    if( front.cvars.CompFlags.register_conventions ) {
        lib_model = 'r';
        PreDefine_Macro( front, "__3R__" );
    } else {
        lib_model = 's';
        PreDefine_Macro( front, "__3S__" );
    }
    if( front.cvars.CompFlags.br_switch_used ) {
        strcpy( front.cvars.CLIB_Name, "1clb?dll" );
    } else {
        strcpy( front.cvars.CLIB_Name, "1clib3?" );     /* There is only 1 CLIB now! */
    }
    if( GET_FPU_FPC( front.cvars.ProcRevision ) ) {
        if( front.cvars.CompFlags.br_switch_used ) {
            strcpy( front.cvars.MATHLIB_Name, "5mth?dll" );
        } else {
            strcpy( front.cvars.MATHLIB_Name, "5math3?" );
        }
        front.cvars.EmuLib_Name = NULL;
    } else {
        if( front.cvars.CompFlags.br_switch_used ) {
            strcpy( front.cvars.MATHLIB_Name, "7mt7?dll" );
        } else {
            strcpy( front.cvars.MATHLIB_Name, "7math387?" );
        }
        if( GET_FPU_EMU( front.cvars.ProcRevision ) ) {
            front.cvars.EmuLib_Name = const_cast<char *>("8emu387");
        } else {
            front.cvars.EmuLib_Name = const_cast<char *>("8noemu387");
        }
    }
#endif
    *strchr( front.cvars.CLIB_Name, '?' ) = lib_model;
    *strchr( front.cvars.MATHLIB_Name, '?' ) = lib_model;
}

void CmdSysAnalyse( FrontRun& front, OPT_STORAGE *data )
{
    char        *target_name;

    target_name = setTargetSystem( front, data );
    setIntelArchitecture( front, data );
    SetDebugInfoFormat( front, data );
    SetGenSwitches( front, data );
    Define_Memory_Model( front, data );
    SetDftCallConv( front, data );
    SetFinalTargetSystem( front, data, target_name );
    MacroDefs( front, data );
}

void CmdSysSetMaxOptimization( FrontRun& front )
{
    front.cvars.GenSwitches |= CGSW_GEN_I_MATH_INLINE;
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
Cmdlnx86State* cmdlnx86_make() { return new Cmdlnx86State(); }
void cmdlnx86_free(Cmdlnx86State* part) { delete part; }
