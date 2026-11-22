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
struct Cmdlnx86State {
    bool nd_used = false;
};

}  // namespace

static inline Cmdlnx86State& cmdlnx86_state() { return cbrun_part<Cmdlnx86State>(); }


void CmdSysInit( void )
{
    SetAuxWatcallInfo();

    cvars_state().GenSwitches = CGSW_GEN_MEMORY_LOW_FAILS;
#if _CPU == 8086
    cvars_state().TargetSwitches = static_cast<cg_target_switches>(0);
    cvars_state().PackAmount = TARGET_INT;                /* pack structs on word boundaries */
#else
    cvars_state().TargetSwitches = CGSW_X86_USE_32;
    cvars_state().PackAmount = 8;
#endif
    cvars_state().GblPackAmount = cvars_state().PackAmount;
    cvars_state().DataThreshold = TARGET_INT_MAX;
    cvars_state().Inline_Threshold = 20;
    cvars_state().TextSegName = const_cast<char *>("");
    cvars_state().DataSegName = const_cast<char *>("");
    cvars_state().GenCodeGroup = const_cast<char *>("");
}

static char *setTargetSystem( OPT_STORAGE *data )
{
    char *target_name = NULL;

    /*
     * -bt specify a build target name
     */
    if( data->bt ) {
        SetTargetNameBT( &target_name, &(data->bt_value) );
    }
    /*
     * -zw overrides a build target name and setting
     */
    switch( data->win ) {
    case OPT_ENUM_win_zw:
        SetTargetName( &target_name, "WINDOWS" );
        break;
#if _CPU == 8086
    case OPT_ENUM_win_zW:
        SetTargetName( &target_name, "CHEAP_WINDOWS" );
        break;
    case OPT_ENUM_win_zws:
        SetTargetName( &target_name, "WINDOWS" );
        cvars_state().TargetSwitches |= CGSW_X86_SMART_WINDOWS;
        break;
    case OPT_ENUM_win_zWs:
        SetTargetName( &target_name, "CHEAP_WINDOWS" );
        cvars_state().TargetSwitches |= CGSW_X86_SMART_WINDOWS;
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
        SetTargetName( &target_name, "NT" );
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
            cvars_state().TargetSystem = TS_DOS;
        } else if( strcmp( target_name, "NETWARE" ) == 0 ) {
            cvars_state().TargetSystem = TS_NETWARE;
        } else if( strcmp( target_name, "NETWARE5" ) == 0 ) {
            cvars_state().TargetSystem = TS_NETWARE5;
            SetTargetName( &target_name, "NETWARE" );
        } else if( strcmp( target_name, "WINDOWS" ) == 0 ) {
            cvars_state().TargetSystem = TS_WINDOWS;
        } else if( strcmp( target_name, "CHEAP_WINDOWS" ) == 0 ) {
#if _CPU == 8086
            cvars_state().TargetSystem = TS_CHEAP_WINDOWS;
#else
            cvars_state().TargetSystem = TS_WINDOWS;
#endif
            SetTargetName( &target_name, "WINDOWS" );
        } else if( strcmp( target_name, "NT" ) == 0 ) {
            cvars_state().TargetSystem = TS_NT;
        } else if( strcmp( target_name, "LINUX" ) == 0 ) {
            cvars_state().TargetSystem = TS_LINUX;
        } else if( strcmp( target_name, "QNX" ) == 0 ) {
            cvars_state().TargetSystem = TS_QNX;
        } else if( strcmp( target_name, "OS2" ) == 0 ) {
            cvars_state().TargetSystem = TS_OS2;
        } else if( strcmp( target_name, "RDOS" ) == 0 ) {
            cvars_state().TargetSystem = TS_RDOS;
        } else if( strcmp( target_name, "HAIKU" ) == 0
                || strcmp( target_name, "OSX" ) == 0
                || strcmp( target_name, "SOLARIS" ) == 0
                || strcmp( target_name, "BSD" ) == 0 ) {
            cvars_state().TargetSystem = TS_UNIX;
        }
    }
    return( target_name );
}

static void setIntelArchitecture( OPT_STORAGE *data )
{
#if _CPU == 8086
    /*
     * Set CPU for 16-bit architecture, default is 8086
     */
    switch( data->arch_i86 ) {
    case OPT_ENUM_arch_i86_default:
    case OPT_ENUM_arch_i86__0:
        SET_CPU( cvars_state().ProcRevision, CPU_86 );
        break;
    case OPT_ENUM_arch_i86__1:
        SET_CPU( cvars_state().ProcRevision, CPU_186 );
        break;
    case OPT_ENUM_arch_i86__2:
        SET_CPU( cvars_state().ProcRevision, CPU_286 );
        break;
    case OPT_ENUM_arch_i86__3:
        SET_CPU( cvars_state().ProcRevision, CPU_386 );
        break;
    case OPT_ENUM_arch_i86__4:
        SET_CPU( cvars_state().ProcRevision, CPU_486 );
        break;
    case OPT_ENUM_arch_i86__5:
        SET_CPU( cvars_state().ProcRevision, CPU_586 );
        break;
    case OPT_ENUM_arch_i86__6:
        SET_CPU( cvars_state().ProcRevision, CPU_686 );
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
        SET_CPU( cvars_state().ProcRevision, CPU_386 );
        break;
    case OPT_ENUM_arch_386__3r:
        cvars_state().CompFlags.register_conventions = true;
        SET_CPU( cvars_state().ProcRevision, CPU_386 );
        break;
    case OPT_ENUM_arch_386__4s:
        SET_CPU( cvars_state().ProcRevision, CPU_486 );
        break;
    case OPT_ENUM_arch_386__4r:
        cvars_state().CompFlags.register_conventions = true;
        SET_CPU( cvars_state().ProcRevision, CPU_486 );
        break;
    case OPT_ENUM_arch_386__5s:
        SET_CPU( cvars_state().ProcRevision, CPU_586 );
        break;
    case OPT_ENUM_arch_386__5r:
        cvars_state().CompFlags.register_conventions = true;
        SET_CPU( cvars_state().ProcRevision, CPU_586 );
        break;
    case OPT_ENUM_arch_386__6s:
        SET_CPU( cvars_state().ProcRevision, CPU_686 );
        break;
    case OPT_ENUM_arch_386_default:
    case OPT_ENUM_arch_386__6r:
        cvars_state().CompFlags.register_conventions = true;
        SET_CPU( cvars_state().ProcRevision, CPU_686 );
        break;
    default:
        DbgNever();
        break;
    }
    /*
     * NETWARE uses stack based calling conventions
     * by default - silly people.
     */
    if( cvars_state().TargetSystem == TS_NETWARE
      || cvars_state().TargetSystem == TS_NETWARE5 ) {
        cvars_state().CompFlags.register_conventions = false;
    }
#endif

    /*
     * Set FPU version, default is 8087/80387
     */
    switch( data->intel_fpu_level ) {
    case OPT_ENUM_intel_fpu_level_fp6:
        SET_FPU_LEVEL( cvars_state().ProcRevision, FPU_686 );
        break;
    case OPT_ENUM_intel_fpu_level_fp5:
        SET_FPU_LEVEL( cvars_state().ProcRevision, FPU_586 );
        break;
#if _CPU == 386
    case OPT_ENUM_intel_fpu_level_default:
#endif
    case OPT_ENUM_intel_fpu_level_fp3:
        SET_FPU_LEVEL( cvars_state().ProcRevision, FPU_387 );
        break;
#if _CPU == 8086
    case OPT_ENUM_intel_fpu_level_default:
#endif
    case OPT_ENUM_intel_fpu_level_fp2:
        SET_FPU_LEVEL( cvars_state().ProcRevision, FPU_87 );
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
        if( cvars_state().TargetSystem != TS_WINDOWS ) {
#endif
            SET_FPU_EMU( cvars_state().ProcRevision );
            break;
#if _CPU == 386
        }
        /* fall through */
#endif
    case OPT_ENUM_intel_fpu_model_fpi87:
        SET_FPU_INLINE( cvars_state().ProcRevision );
        break;
    case OPT_ENUM_intel_fpu_model_fpc:
        SET_FPU_FPC( cvars_state().ProcRevision );
        cvars_state().CompFlags.op_switch_used = false;
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
        cvars_state().Stack87 = 4;
    } else if( cvars_state().TargetSystem == TS_NETWARE ) {
        cvars_state().Stack87 = 4;
    } else {
        cvars_state().Stack87 = 8;
    }

    if( data->fld ) {
        cvars_state().CompFlags.use_long_double = true;
    }
    if( data->fpd ) {
        cvars_state().TargetSwitches |= CGSW_X86_P5_DIVIDE_CHECK;
    }
    if( data->zfw ) {
        cvars_state().TargetSwitches |= CGSW_X86_GEN_FWAIT_386;
    }
#if _CPU == 8086
    if( data->zmsc ) {
        cvars_state().TargetSwitches |= CGSW_X86_MSC8;
    }
#endif
    switch( data->fp_rounding ) {
    case OPT_ENUM_fp_rounding_zri:
        cvars_state().GenSwitches |= CGSW_GEN_FPU_ROUNDING_INLINE;
        break;
    case OPT_ENUM_fp_rounding_zro:
        cvars_state().GenSwitches |= CGSW_GEN_FPU_ROUNDING_OMIT;
        break;
    case OPT_ENUM_fp_rounding_default:
        break;
    default:
        DbgNever();
        break;
    }
}

static void SetDftCallConv( OPT_STORAGE *data )
{
    /*
     * default is Watcom calling convention
     */
    switch( data->intel_call_conv ) {
    case OPT_ENUM_intel_call_conv_ecc:
        callinfo_state().DftCallConv = &CdeclInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecd:
        callinfo_state().DftCallConv = &StdcallInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecf:
        callinfo_state().DftCallConv = &FastcallInfo;
        break;
    case OPT_ENUM_intel_call_conv_eco:
        callinfo_state().DftCallConv = &OptlinkInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecp:
        callinfo_state().DftCallConv = &PascalInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecr:
        callinfo_state().DftCallConv = &FortranInfo;
        break;
    case OPT_ENUM_intel_call_conv_ecs:
        callinfo_state().DftCallConv = &SyscallInfo;
        break;
    case OPT_ENUM_intel_call_conv_default:
    case OPT_ENUM_intel_call_conv_ecw:
        callinfo_state().DftCallConv = &WatcallInfo;
        break;
    default:
        DbgNever();
        break;
    }
}

static void SetFinalTargetSystem( OPT_STORAGE *data, char *target_name )
{
    char buff[128];

    /* unused parameters */ (void)data;

    if( target_name != NULL ) {
        sprintf( buff, "__%s__", target_name );
        PreDefine_Macro( buff );
    }

    PreDefine_Macro( UIMX86 );
    if( cvars_state().CompFlags.non_iso_compliant_names_enabled ) {
        PreDefine_Macro( IMX86 );
    }
    PreDefine_Macro( "__X86__" );
    PreDefine_Macro( "_X86_" );
#if _CPU == 8086
    PreDefine_Macro( "__I86__" );
#else
    PreDefine_Macro( "__386__" );
    PreDefine_Macro( "_STDCALL_SUPPORTED" );
#endif
    PreDefine_Macro( "__WATCOM_INT64__" );
    PreDefine_Macro( "_INTEGRAL_MAX_BITS=64" );

    switch( cvars_state().TargetSystem ) {
    case TS_DOS:
        PreDefine_Macro( "_DOS" );
        if( cvars_state().CompFlags.non_iso_compliant_names_enabled ) {
            PreDefine_Macro( "MSDOS" );
        }
        break;
    case TS_QNX:
        PreDefine_Macro( "__UNIX__" );
        break;
    case TS_CHEAP_WINDOWS:
        PreDefine_Macro( "__CHEAP_WINDOWS__" );
        cvars_state().TargetSwitches |= CGSW_X86_CHEAP_WINDOWS;
        /* fall through */
    case TS_WINDOWS:
#if _CPU == 8086
        PreDefine_Macro( "_WINDOWS" );
        cvars_state().TargetSwitches |= CGSW_X86_WINDOWS;
#else
        PreDefine_Macro( "__WINDOWS_386__" );
#endif
        break;
#if _CPU == 386
    case TS_NETWARE5:
        PreDefine_Macro( "__NETWARE5__" );
        /* fall through */
    case TS_NETWARE:
        PreDefine_Macro( "__NETWARE_386__" );
        break;
    case TS_RDOS:
        PreDefine_Macro( "_RDOS" );
        break;
    case TS_NT:
        PreDefine_Macro( "_WIN32" );
        break;
    case TS_LINUX:
    case TS_UNIX:
        PreDefine_Macro( "__UNIX__" );
        break;
#endif
    default:
        break;
    }

    if( GET_CPU( cvars_state().ProcRevision ) < CPU_386 ) {
        /*
         * issue warning message if /zf[f|p] or /zg[f|p] spec'd?
         */
        cvars_state().TargetSwitches &= ~(CGSW_X86_FLOATING_FS | CGSW_X86_FLOATING_GS);
    }
    if( !cvars_state().CompFlags.save_restore_segregs ) {
        if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_DS ) {
            HW_CTurnOff( WatcallInfo.save, HW_DS );
        }
        if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_ES ) {
            HW_CTurnOff( WatcallInfo.save, HW_ES );
        }
        if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_FS ) {
            HW_CTurnOff( WatcallInfo.save, HW_FS );
        }
        if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_GS ) {
            HW_CTurnOff( WatcallInfo.save, HW_GS );
        }
    }
    if( data->of_plus ) {
        WatcallInfo.cclass_target |= FECALL_X86_GENERATE_STACK_FRAME;
    }
  #if _CPU == 386
    if( !cvars_state().CompFlags.register_conventions ) {
        SetAuxStackConventions();
    }
  #endif

    if( target_name != NULL ) {
        strcpy( buff, target_name );
        strcat( buff, "_INCLUDE" );
        MergeIncludeFromEnv( buff );
    }
#if _CPU == 8086
    MergeIncludeFromEnv( "INCLUDE" );
#else
    if( !MergeIncludeFromEnv( "INC386" ) ) {
        MergeIncludeFromEnv( "INCLUDE" );
    }
#endif

    CMemFree( target_name );
}

static void SetDebugInfoFormat( OPT_STORAGE *data )
{
    cvars_state().GenSwitches &= ~(CGSW_GEN_DBG_CV | CGSW_GEN_DBG_DF | CGSW_GEN_DBG_PREDEF);
    switch( data->dbg_output ) {
    case OPT_ENUM_dbg_output_hc:
        cvars_state().GenSwitches |= CGSW_GEN_DBG_CV;
        break;
    case OPT_ENUM_dbg_output_hw:
        break;
    case OPT_ENUM_dbg_output_hda:
        cvars_state().GenSwitches |= CGSW_GEN_DBG_DF | CGSW_GEN_DBG_PREDEF;
        cvars_state().SymDFAbbr = SpcSymbol( "__DFABBREV", GetType( TYP_USHORT ), SC_EXTERN );
        break;
    case OPT_ENUM_dbg_output_hdg:
        cvars_state().GenSwitches |= CGSW_GEN_DBG_DF | CGSW_GEN_DBG_PREDEF;
        cvars_state().SymDFAbbr = SpcSymbol( "__DFABBREV", GetType( TYP_USHORT ), SC_NONE );
        break;
    case OPT_ENUM_dbg_output_hd:
    case OPT_ENUM_dbg_output_default:
        cvars_state().GenSwitches |= CGSW_GEN_DBG_DF;
        break;
    default:
        DbgNever();
        break;
    }
}

static void SetGenSwitches( OPT_STORAGE *data )
{
#if _CPU == 386
    if( data->br ) {
        cvars_state().CompFlags.br_switch_used = true;
    }
#endif
    if( data->ec ) {
        cvars_state().CompFlags.ec_switch_used = true;
    }
    if( data->ee ) {
        cvars_state().CompFlags.ee_switch_used = true;
    }
    if( data->eoo ) {
        cvars_state().GenSwitches &= ~(CGSW_GEN_OBJ_ELF | CGSW_GEN_OBJ_COFF);
    }
#if _CPU == 386
    if( data->eoe ) {
        cvars_state().GenSwitches = (cvars_state().GenSwitches & ~CGSW_GEN_OBJ_COFF) | CGSW_GEN_OBJ_ELF;
    }
    if( data->eoc ) {
        cvars_state().GenSwitches = (cvars_state().GenSwitches & ~CGSW_GEN_OBJ_ELF) | CGSW_GEN_OBJ_COFF;
    }
#endif
    if( data->eq ) {
        cvars_state().CompFlags.eq_switch_used = true;
    }
#if _CPU == 386
    if( data->et ) {
        cvars_state().TargetSwitches |= CGSW_X86_P5_PROFILING;
    }
    if( data->esp ) {
        cvars_state().TargetSwitches |= CGSW_X86_STATEMENT_COUNTING;
    }
    if( data->et0 ) {
        cvars_state().TargetSwitches |= CGSW_X86_P5_PROFILING | CGSW_X86_P5_PROFILING_CTR0;
    }
    if( data->etp ) {
        cvars_state().TargetSwitches |= CGSW_X86_NEW_P5_PROFILING;
    }
    if( data->ez ) {
        cvars_state().TargetSwitches |= CGSW_X86_EZ_OMF;
    }
#endif
    if( data->g ) {
        SetStringOption( &cvars_state().GenCodeGroup, &(data->g_value) );
    }
    if( data->nc ) {
        SetStringOption( &cvars_state().CodeClassName, &(data->nc_value) );
    }
    if( data->nd ) {
        cmdlnx86_state().nd_used = true;
        SetStringOption( &cvars_state().DataSegName, &(data->nd_value) );
        ImportNearSegIdInit();
        if( *cvars_state().DataSegName == '\0' ) {
            CMemFree( cvars_state().DataSegName );
            cvars_state().DataSegName = NULL;
        }
    }
    if( data->nm ) {
        SetStringOption( &cvars_state().ModuleName, &(data->nm_value) );
    }
    if( data->nt ) {
        SetStringOption( &cvars_state().TextSegName, &(data->nt_value) );
    }
    if( data->oc ) {
        cvars_state().GenSwitches |= CGSW_GEN_NO_CALL_RET_TRANSFORM;
    }
    if( data->of ) {
        cvars_state().TargetSwitches |= CGSW_X86_NEED_STACK_FRAME;
    }
    if( data->of_plus ) {
        cvars_state().TargetSwitches |= CGSW_X86_NEED_STACK_FRAME;
    }
    if( data->om ) {
        cvars_state().GenSwitches |= CGSW_GEN_I_MATH_INLINE;
    }
    if( data->op ) {
        cvars_state().CompFlags.op_switch_used = true;
    }
    if( data->r ) {
        cvars_state().CompFlags.save_restore_segregs = true;
    }
#if _CPU == 386
    if( data->re ) {
        cvars_state().CompFlags.rent = true;
    }
#endif
    if( data->ri ) {
        cvars_state().CompFlags.returns_promoted = true;
    }
    if( data->sg ) {
        cvars_state().CompFlags.sg_switch_used = true;
    }
    if( data->st ) {
        cvars_state().CompFlags.st_switch_used = true;
    }
#if _CPU == 8086
    if( data->wo ) {
        cvars_state().CompFlags.using_overlays = true;
    }
#endif
#if _CPU == 386
    if( data->xgv ) {
        cvars_state().TargetSwitches |= CGSW_X86_INDEXED_GLOBALS;
    }
#endif
    if( data->zc ) {
        cvars_state().CompFlags.strings_in_code_segment = true;
        cvars_state().CompFlags.zc_switch_used = true;
        cvars_state().TargetSwitches |= CGSW_X86_CONST_IN_CODE;
    }
    if( data->zm ) {
        cvars_state().CompFlags.multiple_code_segments = true;
        cvars_state().CompFlags.zm_switch_used = true;
    }
    if( data->zt ) {
        cvars_state().DataThreshold = data->zt_value;
    }
    if( data->zu ) {
        cvars_state().CompFlags.zu_switch_used = true;
    }
#if _CPU == 386
    if( data->zz ) {
        cvars_state().CompFlags.use_stdcall_at_number = false;
    }
#endif
}

static void PreDefine_UMIX86_Macro( void )
{
    unsigned    cpu;
    char        buff[32];

    switch( GET_CPU( cvars_state().ProcRevision ) ) {
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
    PreDefine_Macro( buff );
}

static void MacroDefs( OPT_STORAGE *data )
{
    if( cvars_state().GenSwitches & CGSW_GEN_I_MATH_INLINE ) {
        DefSwitchMacro( "OM" );
    }
    switch( data->mem_model ) {
    case OPT_ENUM_mem_model_ms:
        DefSwitchMacro( "MS" );
        PreDefine_Macro( UMX86 "SM" );
        PreDefine_Macro( "__SMALL__" );
        break;
    case OPT_ENUM_mem_model_mm:
        DefSwitchMacro( "MM" );
        PreDefine_Macro( UMX86 "MM" );
        PreDefine_Macro( "__MEDIUM__" );
        break;
    case OPT_ENUM_mem_model_mc:
        DefSwitchMacro( "MC" );
        PreDefine_Macro( UMX86 "CM" );
        PreDefine_Macro( "__COMPACT__" );
        break;
    case OPT_ENUM_mem_model_ml:
        DefSwitchMacro( "ML" );
        PreDefine_Macro( UMX86 "LM" );
        PreDefine_Macro( "__LARGE__" );
        break;
#if _CPU == 8086
    case OPT_ENUM_mem_model_mh:
        DefSwitchMacro( "MH" );
        PreDefine_Macro( UMX86 "HM" );
        PreDefine_Macro( "__HUGE__" );
        break;
#else
    case OPT_ENUM_mem_model_mf:
        DefSwitchMacro( "MF" );
        PreDefine_Macro( UMX86 "FM" );
        PreDefine_Macro( "__FLAT__" );
        break;
#endif
    default:
        break;
    }
    if( cvars_state().CompFlags.non_iso_compliant_names_enabled ) {
        switch( data->mem_model ) {
        case OPT_ENUM_mem_model_ms:
            PreDefine_Macro( MX86 "SM" );
            break;
        case OPT_ENUM_mem_model_mm:
            PreDefine_Macro( MX86 "MM" );
            break;
        case OPT_ENUM_mem_model_mc:
            PreDefine_Macro( MX86 "CM" );
            break;
        case OPT_ENUM_mem_model_ml:
            PreDefine_Macro( MX86 "LM" );
            break;
#if _CPU == 8086
        case OPT_ENUM_mem_model_mh:
            PreDefine_Macro( MX86 "HM" );
            break;
#else
        case OPT_ENUM_mem_model_mf:
            PreDefine_Macro( MX86 "FM" );
            break;
#endif
        default:
            break;
        }
    }

    if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_FS ) {
        DefSwitchMacro( "ZFF" );
    } else {
        DefSwitchMacro( "ZFP" );
    }
    if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_GS ) {
        DefSwitchMacro( "ZGF" );
    } else {
        DefSwitchMacro( "ZGP" );
    }
    if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_DS ) {
        DefSwitchMacro( "ZDF" );
    } else {
        DefSwitchMacro( "ZDP" );
    }
    if( cvars_state().TargetSwitches & CGSW_X86_FLOATING_SS ) {
        DefSwitchMacro( "ZU" );
    }
    if( cvars_state().TargetSwitches & CGSW_X86_INDEXED_GLOBALS ) {
        DefSwitchMacro( "XGV" );
    }
    if( cvars_state().TargetSwitches & CGSW_X86_WINDOWS ) {
        DefSwitchMacro( "ZW" );
    }
    if( cvars_state().TargetSwitches & CGSW_X86_NEED_STACK_FRAME ) {
        DefSwitchMacro( "OF" );
    }
    if( cvars_state().TargetSwitches & CGSW_X86_GEN_FWAIT_386 ) {
        DefSwitchMacro( "ZFW" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_NO_CALL_RET_TRANSFORM ) {
        DefSwitchMacro( "OC" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_SUPER_OPTIMAL ) {
        DefSwitchMacro( "OH" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_FLOW_REG_SAVES ) {
        DefSwitchMacro( "OK" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_NO_OPTIMIZATION ) {
        DefSwitchMacro( "OD" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_RELAX_ALIAS ) {
        DefSwitchMacro( "OA" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_LOOP_OPTIMIZATION ) {
        DefSwitchMacro( "OL" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_INS_SCHEDULING ) {
        DefSwitchMacro( "OR" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_FP_UNSTABLE_OPTIMIZATION ) {
        DefSwitchMacro( "ON" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_FPU_ROUNDING_OMIT ) {
        DefSwitchMacro( "ZRO" );
    }
    if( cvars_state().GenSwitches & CGSW_GEN_FPU_ROUNDING_INLINE ) {
        DefSwitchMacro( "ZRI" );
    }
    if( cvars_state().CompFlags.use_long_double ) {
        DefSwitchMacro( "FLD" );
    }
    if( cvars_state().CompFlags.signed_char ) {
        DefSwitchMacro( "J" );
    }
    if( cvars_state().PCH_FileName != NULL ) {
        DefSwitchMacro( "FH" );
    }
    if( cvars_state().CompFlags.no_pch_warnings ) {
        DefSwitchMacro( "FHQ" );
    }
    if( cvars_state().CompFlags.inline_functions ) {
        DefSwitchMacro( "OI" );
    }
    if( cvars_state().CompFlags.unique_functions ) {
        DefSwitchMacro( "OU" );
    }
#if _CPU == 386
    if( cvars_state().CompFlags.register_conventions ) {
        DefSwitchMacro( "3R" );
    } else {
        DefSwitchMacro( "3S" );
    }
#endif
    if( cvars_state().CompFlags.emit_names ) {
        DefSwitchMacro( "EN" );
    }
    if( cvars_state().CompFlags.make_enums_an_int ) {
        DefSwitchMacro( "EI" );
    }
    if( cvars_state().CompFlags.zc_switch_used ) {
        DefSwitchMacro( "ZC" );
    }
    if( cvars_state().CompFlags.use_double_byte ) {
        DefSwitchMacro( "ZK" );
    }
    if( cvars_state().CompFlags.save_restore_segregs ) {
        DefSwitchMacro( "R" );
    }
    if( cvars_state().CompFlags.sg_switch_used ) {
        DefSwitchMacro( "SG" );
    }
    if( cvars_state().CompFlags.st_switch_used ) {
        DefSwitchMacro( "ST" );
    }
    if( cvars_state().CompFlags.zu_switch_used ) {
        DefSwitchMacro( "ZU" );
    }
    if( cvars_state().CompFlags.bm_switch_used ) {
        DefSwitchMacro( "BM" );
        Define_Macro( "_MT" );
    }
    if( cvars_state().CompFlags.bd_switch_used ) {
        DefSwitchMacro( "BD" );
    }
    if( cvars_state().CompFlags.bc_switch_used ) {
        DefSwitchMacro( "BC" );
    }
    if( cvars_state().CompFlags.bg_switch_used ) {
        DefSwitchMacro( "BG" );
    }
    if( cvars_state().CompFlags.br_switch_used ) {
        DefSwitchMacro( "BR" );
        Define_Macro( "_DLL" );
    }
    if( cvars_state().CompFlags.bw_switch_used ) {
        DefSwitchMacro( "BW" );
    }
    if( cvars_state().CompFlags.zm_switch_used ) {
        DefSwitchMacro( "ZM" );
    }
    if( cvars_state().CompFlags.ep_switch_used ) {
        DefSwitchMacro( "EP" );
    }
    if( cvars_state().CompFlags.ee_switch_used ) {
        DefSwitchMacro( "EE" );
    }
    if( cvars_state().CompFlags.ec_switch_used ) {
        DefSwitchMacro( "EC" );
    }

    switch( GET_CPU( cvars_state().ProcRevision ) ) {
    case CPU_86:
        DefSwitchMacro( "0" );
        break;
    case CPU_186:
        DefSwitchMacro( "1" );
        break;
    case CPU_286:
        DefSwitchMacro( "2" );
        break;
    case CPU_386:
        DefSwitchMacro( "3" );
        break;
    case CPU_486:
        DefSwitchMacro( "4" );
        break;
    case CPU_586:
        DefSwitchMacro( "5" );
        break;
    case CPU_686:
        DefSwitchMacro( "6" );
        break;
    default:
        break;
    }

    PreDefine_UMIX86_Macro();

    switch( GET_FPU_LEVEL( cvars_state().ProcRevision ) ) {
    case FPU_87:
        DefSwitchMacro( "FP2" );
        break;
    case FPU_387:
        DefSwitchMacro( "FP3" );
        break;
    case FPU_586:
        DefSwitchMacro( "FP5" );
        break;
    case FPU_686:
        DefSwitchMacro( "FP6" );
        break;
    default:
        break;
    }
    if( GET_FPU_FPC( cvars_state().ProcRevision ) ) {
        DefSwitchMacro( "FPC" );
    } else {
        if( GET_FPU_EMU( cvars_state().ProcRevision ) ) {
            DefSwitchMacro( "FPI" );
        } else {
            DefSwitchMacro( "FPI87" );
        }
        PreDefine_Macro( "__FPI__" );
    }

    if( cmdlnx86_state().nd_used ) {
        DefSwitchMacro( "ND" );
    }
    if( cvars_state().CompFlags.op_switch_used ) {
        DefSwitchMacro( "OP" );
    }
    if( !TOGGLE( check_stack ) ) {
        DefSwitchMacro( "S" );
    }
}

static void Define_Memory_Model( OPT_STORAGE *data )
{
    char lib_model = '\0';
    unsigned bit;

    cvars_state().DataPtrSize = TARGET_POINTER;
    cvars_state().CodePtrSize = TARGET_POINTER;
    bit = 0;
    if( data->mem_model == OPT_ENUM_mem_model_default ) {
#if _CPU == 8086
        data->mem_model = OPT_ENUM_mem_model_ms;
#else
        if( cvars_state().TargetSystem == TS_NETWARE
          || cvars_state().TargetSystem == TS_NETWARE5 ) {
            data->mem_model = OPT_ENUM_mem_model_ms;
        } else {
            data->mem_model = OPT_ENUM_mem_model_mf;
        }
#endif
    }
    switch( data->mem_model ) {
    case OPT_ENUM_mem_model_ms:
        lib_model = 's';
        cvars_state().CompFlags.strings_in_code_segment = false;
        cvars_state().TargetSwitches &= ~CGSW_X86_CONST_IN_CODE;
        bit |= CGSW_X86_CHEAP_POINTER;
        break;
    case OPT_ENUM_mem_model_mm:
        lib_model = 'm';
        WatcallInfo.cclass_target |= FECALL_X86_FAR_CALL;
        cvars_state().CodePtrSize = TARGET_FAR_POINTER;
        cvars_state().CompFlags.strings_in_code_segment = false;
        cvars_state().TargetSwitches &= ~CGSW_X86_CONST_IN_CODE;
        bit |= CGSW_X86_BIG_CODE | CGSW_X86_CHEAP_POINTER;
        break;
    case OPT_ENUM_mem_model_mc:
        lib_model = 'c';
        cvars_state().DataPtrSize = TARGET_FAR_POINTER;
        bit |= CGSW_X86_BIG_DATA | CGSW_X86_CHEAP_POINTER;
        break;
    case OPT_ENUM_mem_model_ml:
        lib_model = 'l';
        WatcallInfo.cclass_target |= FECALL_X86_FAR_CALL;
        cvars_state().CodePtrSize = TARGET_FAR_POINTER;
        cvars_state().DataPtrSize = TARGET_FAR_POINTER;
        bit |= CGSW_X86_BIG_CODE | CGSW_X86_BIG_DATA | CGSW_X86_CHEAP_POINTER;
        break;
#if _CPU == 8086
    case OPT_ENUM_mem_model_mh:
        lib_model = 'h';
        WatcallInfo.cclass_target |= FECALL_X86_FAR_CALL;
        cvars_state().CodePtrSize = TARGET_FAR_POINTER;
        cvars_state().DataPtrSize = TARGET_FAR_POINTER;
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
        cvars_state().TargetSwitches |= CGSW_X86_LOAD_DS_DIRECTLY;
        bit &= ~CGSW_X86_FLOATING_DS;
    }
#endif
#if _CPU == 8086
    /*
     * any model of 16-bit Windows has DS pegged
     */
    if( cvars_state().TargetSystem == TS_WINDOWS
      || cvars_state().TargetSystem == TS_CHEAP_WINDOWS ) {
        bit &= ~CGSW_X86_FLOATING_DS;
    }
#endif
    switch( data->ds_peg ) {
    case OPT_ENUM_ds_peg_zdp:
        cvars_state().TargetSwitches &= ~CGSW_X86_FLOATING_DS;
        bit &= ~CGSW_X86_FLOATING_DS;
        break;
    case OPT_ENUM_ds_peg_zdf:
        cvars_state().TargetSwitches |= CGSW_X86_FLOATING_DS;
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
    if( GET_CPU( cvars_state().ProcRevision ) >= CPU_386 ) {
#if _CPU == 8086
        bit |= CGSW_X86_FLOATING_FS;
#else
        if( (bit & CGSW_X86_FLAT_MODEL) == 0
          || cvars_state().TargetSystem == TS_WINDOWS ) {
            bit |= CGSW_X86_FLOATING_FS;
        }
#endif
    }
    switch( data->fs_peg ) {
    case OPT_ENUM_fs_peg_zfp:
        cvars_state().TargetSwitches &= ~CGSW_X86_FLOATING_FS;
        bit &= ~CGSW_X86_FLOATING_FS;
        break;
    case OPT_ENUM_fs_peg_zff:
        cvars_state().TargetSwitches |= CGSW_X86_FLOATING_FS;
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
    if( GET_CPU( cvars_state().ProcRevision ) >= CPU_386 ) {
        bit |= CGSW_X86_FLOATING_GS;
    }
    switch( data->gs_peg ) {
    case OPT_ENUM_gs_peg_zgp:
        cvars_state().TargetSwitches &= ~CGSW_X86_FLOATING_GS;
        bit &= ~CGSW_X86_FLOATING_GS;
        break;
    case OPT_ENUM_gs_peg_zgf:
        cvars_state().TargetSwitches |= CGSW_X86_FLOATING_GS;
        bit &= ~CGSW_X86_FLOATING_GS;
        break;
    case OPT_ENUM_gs_peg_default:
        break;
    default:
        DbgNever();
        break;
    }

    cvars_state().TargetSwitches &= ~(CGSW_X86_FLAT_MODEL | CGSW_X86_BIG_CODE | CGSW_X86_BIG_DATA | CGSW_X86_CHEAP_POINTER);
    cvars_state().TargetSwitches |= static_cast<cg_target_switches>(bit);

    if( GET_CPU( cvars_state().ProcRevision ) < CPU_386 ) {
        /*
         * issue warning message if /zf[f|p] or /zg[f|p] spec'd?
         */
        cvars_state().TargetSwitches &= ~(CGSW_X86_FLOATING_FS | CGSW_X86_FLOATING_GS);
    }

#if _CPU == 8086
    strcpy( cvars_state().CLIB_Name, "1clib?" );
    if( cvars_state().CompFlags.bm_switch_used ) {
        strcpy( cvars_state().CLIB_Name, "1clibmt?" );
    }
    if( cvars_state().CompFlags.bd_switch_used ) {
        if( cvars_state().TargetSystem == TS_WINDOWS || cvars_state().TargetSystem == TS_CHEAP_WINDOWS ) {
            strcpy( cvars_state().CLIB_Name, "1clib?" );
        } else {
            strcpy( cvars_state().CLIB_Name, "1clibdl?" );
        }
    }
    if( GET_FPU_FPC( cvars_state().ProcRevision ) ) {
        strcpy( cvars_state().MATHLIB_Name, "5math?" );
        cvars_state().EmuLib_Name = NULL;
    } else {
        strcpy( cvars_state().MATHLIB_Name, "7math87?" );
        if( GET_FPU_EMU( cvars_state().ProcRevision ) ) {
            cvars_state().EmuLib_Name = const_cast<char *>("8emu87");
        } else {
            cvars_state().EmuLib_Name = const_cast<char *>("8noemu87");
        }
    }
#else
    if( cvars_state().CompFlags.register_conventions ) {
        lib_model = 'r';
        PreDefine_Macro( "__3R__" );
    } else {
        lib_model = 's';
        PreDefine_Macro( "__3S__" );
    }
    if( cvars_state().CompFlags.br_switch_used ) {
        strcpy( cvars_state().CLIB_Name, "1clb?dll" );
    } else {
        strcpy( cvars_state().CLIB_Name, "1clib3?" );     /* There is only 1 CLIB now! */
    }
    if( GET_FPU_FPC( cvars_state().ProcRevision ) ) {
        if( cvars_state().CompFlags.br_switch_used ) {
            strcpy( cvars_state().MATHLIB_Name, "5mth?dll" );
        } else {
            strcpy( cvars_state().MATHLIB_Name, "5math3?" );
        }
        cvars_state().EmuLib_Name = NULL;
    } else {
        if( cvars_state().CompFlags.br_switch_used ) {
            strcpy( cvars_state().MATHLIB_Name, "7mt7?dll" );
        } else {
            strcpy( cvars_state().MATHLIB_Name, "7math387?" );
        }
        if( GET_FPU_EMU( cvars_state().ProcRevision ) ) {
            cvars_state().EmuLib_Name = const_cast<char *>("8emu387");
        } else {
            cvars_state().EmuLib_Name = const_cast<char *>("8noemu387");
        }
    }
#endif
    *strchr( cvars_state().CLIB_Name, '?' ) = lib_model;
    *strchr( cvars_state().MATHLIB_Name, '?' ) = lib_model;
}

void CmdSysAnalyse( OPT_STORAGE *data )
{
    char        *target_name;

    target_name = setTargetSystem( data );
    setIntelArchitecture( data );
    SetDebugInfoFormat( data );
    SetGenSwitches( data );
    Define_Memory_Model( data );
    SetDftCallConv( data );
    SetFinalTargetSystem( data, target_name );
    MacroDefs( data );
}

void CmdSysSetMaxOptimization( void )
{
    cvars_state().GenSwitches |= CGSW_GEN_I_MATH_INLINE;
}
