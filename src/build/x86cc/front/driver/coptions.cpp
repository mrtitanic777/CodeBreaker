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
* Description:  C compiler command line option processing.
*
****************************************************************************/


/*****************************************************************************
*                                                                            *
*       If you add an option, don't forget to change bld/cc/gml/options.gml  *
*       Also, don't forget to add a case in MacroDefs                        *
*       to predefine a __SW_xx macro                                         *
*                                                                            *
*****************************************************************************/


#include "cvars.h"
#include <ctype.h>
#include "wio.h"
#include "watcom.h"
#include "pdefn2.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "iopath.h"
#include "pathlist.h"
#include "cmdlnprs.gh"
#include "cmdlnprs.h"
#include "cmdscan.h"
#include "cmdlnsys.h"
#include "toggles.h"
#include "feprotos.h"
#include "frontrun.hpp"


    #define DbgNever()

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct CoptionsState {
    const char *switch_start = "";
};
namespace {

}  // namespace



static void BadCmdLine( FrontRun& front, int error_code )
/***************************************
 * SIGNAL CMD-LINE ERROR
 */
{
    char        buffer[128];
    size_t      len;

    for( CmdScanChar(front); !CmdScanSwEnd(front); CmdScanChar(front) ) {
        ;
    }
    len = CmdScanAddr(front) - front.coptions->switch_start;
    if( len > sizeof( buffer ) - 1 )
        len = sizeof( buffer ) - 1;
    strncpy( buffer, front.coptions->switch_start, len );
    buffer[len] = '\0';
    CErr2p( front, static_cast<msg_codes>(error_code), buffer );
}

// BAD CHAR DETECTED
void BadCmdLineChar( FrontRun& front )
{
    BadCmdLine( front, ERR_INVALID_OPTION );
}
// BAD ID DETECTED
void BadCmdLineId( FrontRun& front )
{
    BadCmdLine( front, ERR_INVALID_OPTION );
}
// BAD NUMBER DETECTED
void BadCmdLineNumber( FrontRun& front )
{
    BadCmdLine( front, ERR_INVALID_OPTION );
}
// BAD PATH DETECTED
void BadCmdLinePath( FrontRun& front )
{
    BadCmdLine( front, ERR_INVALID_OPTION );
}
// BAD FILE DETECTED
void BadCmdLineFile( FrontRun& front )
{
    BadCmdLine( front, ERR_INVALID_OPTION );
}
// BAD TEXT DETECTED
void BadCmdLineOption( FrontRun& front )
{
    BadCmdLine( front, ERR_INVALID_OPTION );
}

static bool debugOptionAfterOptOption( OPT_STORAGE *data )
{
    if( data->debug_info_timestamp > data->opt_level_timestamp ) {
        if( data->debug_info_timestamp > data->opt_size_time_timestamp ) {
            return( true );
        }
    }
    return( false );
}

char *SetStringOption( FrontRun& front, char **o, OPT_STRING **h )
/***********************************************/
{
    OPT_STRING *s;
    char *p;

    s = *h;
    p = NULL;
    if( s != NULL ) {
        if( s->data[0] != '\0' ) {
            p = CMemStrdup( front, s->data );
        }
        OPT_CLEAN_STRING( front, h );
    }
    if( o != NULL ) {
        CMemFree( front, *o );
        *o = p;
    }
    return( p );
}

static void reverseList( OPT_STRING **h )
{
    OPT_STRING *s;
    OPT_STRING *p;
    OPT_STRING *n;

    s = *h;
    *h = NULL;
    for( p = s; p != NULL; p = n ) {
        n = p->next;
        p->next = *h;
        *h = p;
    }
}

static void checkPacking( unsigned *p )
{
    *p = VerifyPackAmount( *p );
}

static void checkDataThreshold( unsigned *p )
{
    if( *p > TARGET_INT_MAX ) {
        *p = 256;
    }
}

static void checkOENumber( unsigned *p )
{
    /* unused parameters */ (void)p;
}

static bool checkSTD( FrontRun& front, unsigned *value )
{
    cstd_ver    cstd;

    cstd = STD_NONE;
    *value = 0;
    CmdRecogEquals(front);
    while( !CmdScanSwEnd(front) ) {
        if( CmdRecogChar( front, 'c' ) ) {
            if( CmdRecogChar( front, '8' ) && CmdRecogChar( front, '9' ) ) {
                cstd = STD_C89;
            } else if( CmdRecogChar( front, '9' ) && CmdRecogChar( front, '9' ) ) {
                cstd = STD_C99;
            } else if( CmdRecogChar( front, '1' ) ) {
                if( CmdRecogChar( front, '1' ) ) {
                    cstd = STD_C11;
                } else if( CmdRecogChar( front, '7' ) ) {
                    cstd = STD_C17;
                } else {
                    BadCmdLineChar(front);
                }
            } else if( CmdRecogChar( front, '2' ) && CmdRecogChar( front, '3' ) ) {
                cstd = STD_C23;
            } else {
                BadCmdLineChar(front);
            }
        } else {
            BadCmdLineChar(front);
        }
    }
    if( cstd != STD_NONE ) {
        *value = cstd;
        return( true );
    }
    return( false );
}

static void checkPrologSize( unsigned *p )
{
    *p = _RoundUp( *p, TARGET_UINT );
}

static void checkErrorLimit( unsigned *p )
{
    /* unused parameters */ (void)p;
}

static bool scanDefine( FrontRun& front, OPT_STRING **p )
{
    /* unused parameters */ (void)p;

    CmdScanLineInit( front, Define_UserMacro( front, CmdScanAddr(front), front.cvars.CompFlags.extended_defines ) );
    front.coptions->switch_start = CmdScanAddr(front);
    return( true );
}

static bool scanDefinePlus( FrontRun& front, OPT_STRING **p )
{
    /* unused parameters */ (void)p;

    if( CmdScanSwEnd(front) ) {
        front.cvars.CompFlags.extended_defines = true;
    } else {
        CmdScanLineInit( front, Define_UserMacro( front, CmdScanAddr(front), true ) );
        front.coptions->switch_start = CmdScanAddr(front);
    }
    return( true );
}

static bool scanUndefine( FrontRun& front, OPT_STRING **p )
{
    const char  *name;
    size_t      len;
    char        *mname;

    /* unused parameters */ (void)p;

    len = CmdScanId( front, &name );
    mname = ToStringDup( front, name, len );
    AddUndefMacro( front, mname );
    CMemFree( front, mname );
    return( true );
}

static void handleOptionEQ( FrontRun& front, OPT_STORAGE *data, bool value )
{
    /* unused parameters */ (void)data;

    front.cvars.CompFlags.eq_switch_used = value;
}

static unsigned verifyWarnLevel( unsigned level )
{
    if( level > WLEVEL_MAX ) {
        level = WLEVEL_MAX;
    }
    return( level );
}

static void checkWarnLevel( unsigned *p )
{
    *p = verifyWarnLevel( *p );
}

static unsigned verifyPPWidth( unsigned width )
{
    if( width != 0 ) {
        if( width < 20 ) {
            width = 20;
        } else if( width > 10000 ) {
            width = 10000;
        }
    }
    return( width );
}

static void checkPPWidth( unsigned *p )
{
    *p = verifyPPWidth( *p );
}

static void setMessageStatus( FrontRun& front, OPT_STRING *s, bool state )
{
    msg_codes   msgnum;

    while( s != NULL ) {
        if( GetMsgNum( s->data, &msgnum ) ) {
            WarnEnableDisable( front, state, msgnum );
        }
        s = s->next;
    }
}

void DefSwitchMacro( FrontRun& front, const char *str )
{
    char buff[64];

    PreDefine_Macro( front, strcpy( strcpy( buff, "__SW_" ) + 5, str ) - 5 );
}

static void AddIncList( FrontRun& front, const char *path_list )
{
    size_t      old_len;
    size_t      len;
    char        *old_list;
    char        *p;

    if( path_list != NULL
      && *path_list != '\0' ) {
        len = strlen( path_list );
        old_list = front.cvars.IncPathList;
        old_len = strlen( old_list );
        front.cvars.IncPathList = static_cast<char *>(CMemAlloc( front, old_len + 1 + len + 1 ));
        strcpy( front.cvars.IncPathList, old_list );
        CMemFree( front, old_list );
        p = front.cvars.IncPathList + old_len;
        while( *path_list != '\0' ) {
            if( p != front.cvars.IncPathList )
                *p++ = PATH_LIST_SEP;
            path_list = GetPathElement( path_list, NULL, &p );
        }
        *p = '\0';
    }
}

static void SetCharacterEncoding( FrontRun& front, OPT_STORAGE *data )
{
    front.cvars.CompFlags.jis_to_unicode = false;

    switch( data->char_set ) {
    case OPT_ENUM_char_set_zku:
        LoadUnicodeTable( front, data->zku_value );
        break;
    case OPT_ENUM_char_set_zk0u:
        front.cvars.CompFlags.jis_to_unicode = true;
        /* fall through */
    case OPT_ENUM_char_set_zk0:
        front.cvars.CompFlags.use_double_byte = true;
        SetDBChar( front, 0 );
        break;
    case OPT_ENUM_char_set_zk1:
        front.cvars.CompFlags.use_double_byte = true;
        SetDBChar( front, 1 );
        break;
    case OPT_ENUM_char_set_zk2:
        front.cvars.CompFlags.use_double_byte = true;
        SetDBChar( front, 2 );
        break;
    case OPT_ENUM_char_set_zk3:
        front.cvars.CompFlags.use_double_byte = true;
        SetDBChar( front, 3 );
        break;
    case OPT_ENUM_char_set_zkl:
        front.cvars.CompFlags.use_double_byte = true;
        SetDBChar( front, -1 );
        break;
    case OPT_ENUM_char_set_default:
        break;
    default:
        DbgNever();
        break;
    }
}

void SetTargetName( FrontRun& front, char **target_name, const char *name )
/********************************************************/
{
    if( *target_name != NULL ) {
        CMemFree( front, *target_name );
    }
    *target_name = CMemStrdup( front, name );
}

void SetTargetNameBT( FrontRun& front, char **target_name, OPT_STRING **bt_value )
/***************************************************************/
{
    char            *p;
    int             c;

    if( *target_name != NULL ) {
        CMemFree( front, *target_name );
    }
    *target_name = p = SetStringOption( front, NULL, bt_value );
    while( (c = *(unsigned char *)p) != '\0' ) {
        *p++ = (char)toupper( c );
    }
}

static void AnalyseAnyTargetOptions( FrontRun& front, OPT_STORAGE *data )
/******************************************************/
{
    /*
     * quickly do the quiet option so the banner can be printed
     */
    if( data->q
      || data->zq ) {
        front.cvars.CompFlags.quiet_mode = true;
    }
    switch( data->warn_level ) {
    case OPT_ENUM_warn_level_w:
        front.cvars.WngLevel = data->w_value;
        break;
    case OPT_ENUM_warn_level_wx:
        front.cvars.WngLevel = WLEVEL_WX;
        break;
    case OPT_ENUM_warn_level_default:
        front.cvars.WngLevel = WLEVEL_DEFAULT;
        break;
    default:
        DbgNever();
        break;
    }
    switch( data->file_83 ) {
    case OPT_ENUM_file_83_fx:
        front.cvars.CompFlags.check_truncated_fnames = false;
        break;
    case OPT_ENUM_file_83_ft:
    case OPT_ENUM_file_83_default:
        front.cvars.CompFlags.check_truncated_fnames = true;
        break;
    default:
        DbgNever();
        break;
    }
    switch( data->opt_level ) {
    case OPT_ENUM_opt_level_ox:  /* -ox => -obmiler -s */
        front.cvars.GenSwitches &= ~CGSW_GEN_NO_OPTIMIZATION;
        front.cvars.GenSwitches |= CGSW_GEN_BRANCH_PREDICTION;  // -ob
        front.cvars.GenSwitches |= CGSW_GEN_I_MATH_INLINE;      // -om
        data->oi = true;                            // -oi
        front.cvars.GenSwitches |= CGSW_GEN_LOOP_OPTIMIZATION;  // -ol
        front.cvars.Inline_Threshold = 20;                      // -oe=20
        Toggles( front ).tog_inline = true;                    // ...
        front.cvars.GenSwitches |= CGSW_GEN_INS_SCHEDULING;     // -or
        Toggles( front ).tog_check_stack = false;              // -s
        break;
    case OPT_ENUM_opt_level_od:
        front.cvars.GenSwitches |= CGSW_GEN_NO_OPTIMIZATION;
        break;
    case OPT_ENUM_opt_level_default:
        break;
    default:
        DbgNever();
        break;
    }
    switch( data->opt_size_time ) {
    case OPT_ENUM_opt_size_time_ot:
        front.cvars.OptSize = 0;
        front.cvars.GenSwitches &= ~CGSW_GEN_NO_OPTIMIZATION;
        break;
    case OPT_ENUM_opt_size_time_os:
        front.cvars.OptSize = 100;
        front.cvars.GenSwitches &= ~CGSW_GEN_NO_OPTIMIZATION;
        break;
    case OPT_ENUM_opt_size_time_default:
        front.cvars.OptSize = 50;
        break;
    default:
        DbgNever();
        break;
    }
    switch( data->cstd ) {
    case OPT_ENUM_cstd_za99:
        front.cvars.CompVars.cstd = STD_C99;
        break;
    case OPT_ENUM_cstd_zastd:
        if( data->zastd_value != STD_NONE )
            front.cvars.CompVars.cstd = static_cast<cstd_ver>(data->zastd_value);
        break;
    case OPT_ENUM_cstd_default:
        break;
    default:
        DbgNever();
        break;
    }
    switch( data->iso ) {
    case OPT_ENUM_iso_zA:
        front.cvars.CompFlags.strict_ANSI = true;
        /* fall through */
    case OPT_ENUM_iso_za:
        front.cvars.CompFlags.extensions_enabled = false;
        front.cvars.CompFlags.non_iso_compliant_names_enabled = false;
        front.cvars.CompFlags.unique_functions = true;
        break;
    case OPT_ENUM_iso_ze:
    case OPT_ENUM_iso_default:
        front.cvars.CompFlags.extensions_enabled = true;
        front.cvars.CompFlags.non_iso_compliant_names_enabled = true;
        break;
    default:
        DbgNever();
        break;
    }
    // following must precede processing of data->oe
    switch( data->debug_info ) {
    case OPT_ENUM_debug_info_d9:
        front.cvars.CompFlags.use_full_codegen_od = true;
        break;
    case OPT_ENUM_debug_info_d3:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_NUMBERS | CGSW_GEN_DBG_TYPES | CGSW_GEN_DBG_LOCALS;
        front.cvars.CompFlags.dump_types_with_names = true;
        if( debugOptionAfterOptOption( data ) ) {
            front.cvars.GenSwitches |= CGSW_GEN_NO_OPTIMIZATION;
//            data->oe = false;
//            TOGGLE( inline ) = false;
        }
        front.cvars.GenSwitches |= CGSW_GEN_NO_OPTIMIZATION;
        data->oi = false;
        break;
    case OPT_ENUM_debug_info_d2_tilde:
        front.cvars.CompFlags.no_debug_type_names = true;
        /* fall through */
    case OPT_ENUM_debug_info_d2:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_NUMBERS | CGSW_GEN_DBG_TYPES | CGSW_GEN_DBG_LOCALS;
        if( debugOptionAfterOptOption( data ) ) {
//            data->oe = false;
//            TOGGLE( inline ) = false;
//            data->oi = false;
        }
        front.cvars.GenSwitches |= CGSW_GEN_NO_OPTIMIZATION;
        data->oi = false;
        break;
    case OPT_ENUM_debug_info_d1_plus:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_NUMBERS | CGSW_GEN_DBG_TYPES | CGSW_GEN_DBG_LOCALS;
        front.cvars.CompFlags.debug_info_some = true;
        break;
    case OPT_ENUM_debug_info_d1:
        front.cvars.GenSwitches |= CGSW_GEN_DBG_NUMBERS;
        break;
    case OPT_ENUM_debug_info_d0:
        break;
    case OPT_ENUM_debug_info_default:
        break;
    default:
        DbgNever();
        break;
    }

    switch( data->enum_size ) {
    case OPT_ENUM_enum_size_ei:
        front.cvars.CompFlags.make_enums_an_int = true;
        front.cvars.CompFlags.original_enum_setting = true;
        break;
    case OPT_ENUM_enum_size_em:
        front.cvars.CompFlags.make_enums_an_int = false;
        front.cvars.CompFlags.original_enum_setting = false;
        break;
    case OPT_ENUM_enum_size_default:
        /* default set in CmdSysInit() */
        break;
    default:
        DbgNever();
        break;
    }

    if( data->aa ) {
        front.cvars.CompFlags.auto_agg_inits = true;
    }
    if( data->ad ) {
        SetStringOption( front, &front.cvars.DependFileName, &(data->ad_value) );
        front.cvars.CompFlags.generate_auto_depend = true;
    }
    if( data->adbs ) {
        front.cvars.DependForceSlash = '\\';
    }
    if( data->add ) {
        SetStringOption( front, &front.cvars.SrcDepName, &(data->add_value) );
        front.cvars.CompFlags.generate_auto_depend = true;
    }
    if( data->adfs ) {
        front.cvars.DependForceSlash = '/';
    }
    if( data->adhp ) {
        SetStringOption( front, &front.cvars.DependHeaderPath, &(data->adhp_value) );
        front.cvars.CompFlags.generate_auto_depend = true;
    }
    if( data->adt ) {
        SetStringOption( front, &front.cvars.DependTarget, &(data->adt_value) );
        front.cvars.CompFlags.generate_auto_depend = true;
    }
    if( data->ai ) {
        front.cvars.CompFlags.no_check_inits = true;
    }
    if( data->aq ) {
        front.cvars.CompFlags.no_check_qualifiers = true;
    }
    if( data->bc ) {
        front.cvars.CompFlags.bc_switch_used = true;
    }
    if( data->bd ) {
        front.cvars.CompFlags.bd_switch_used = true;
        front.cvars.GenSwitches |= CGSW_GEN_DLL_RESIDENT_CODE;
    }
    if( data->bg ) {
        front.cvars.CompFlags.bg_switch_used = true;
    }
    if( data->bm ) {
        front.cvars.CompFlags.bm_switch_used = true;
    }
    if( data->bw ) {
        front.cvars.CompFlags.bw_switch_used = true;
    }
    if( data->db ) {
        front.cvars.CompFlags.emit_browser_info = true;
    }
    if( data->e ) {
        front.cvars.ErrLimit = data->e_value;
    }
    if( data->ef ) {
        front.cvars.CompFlags.ef_switch_used = true;
    }
    if( data->en ) {
        front.cvars.CompFlags.emit_names = true;
    }
    if( data->ep ) {
        front.cvars.CompFlags.ep_switch_used = true;
        front.cvars.ProEpiDataSize = data->ep_value;
    }
    if( data->fh || data->fhq ) {
        if( data->fhq ) {
            front.cvars.CompFlags.no_pch_warnings = true;
        }
        if( data->fh && ( !data->fhq || data->fh_timestamp > data->fhq_timestamp ) ) {
            SetStringOption( front, &front.cvars.PCH_FileName, &(data->fh_value) );
            OPT_CLEAN_STRING( front, &(data->fhq_value) );
        } else if( data->fhq && ( !data->fh || data->fhq_timestamp > data->fh_timestamp ) ) {
            SetStringOption( front, &front.cvars.PCH_FileName, &(data->fhq_value) );
            OPT_CLEAN_STRING( front, &(data->fh_value) );
        }
    }
    if( data->fi ) {
        SetStringOption( front, &front.cvars.ForceInclude, &(data->fi_value) );
    }
    if( data->fip ) {
        SetStringOption( front, &front.cvars.ForcePreInclude, &(data->fip_value) );
    }
    if( data->fo ) {
        SetStringOption( front, &front.cvars.ObjectFileName, &(data->fo_value) );
        front.cvars.CompFlags.cpp_output_to_file = true;    /* in case '-p' option */
    }
    if( data->fr ) {
        SetStringOption( front, &front.cvars.ErrorFileName, &(data->fr_value) );
    }
    if( data->fti ) {
        front.cvars.CompFlags.track_includes = true;
    }
    if( data->i ) {
        OPT_STRING *s;
        reverseList( &(data->i_value) );
        for( s = data->i_value; s != NULL; s = s->next ) {
            if( s->data[0] != '\0' ) {
                AddIncList( front, s->data );
            }
        }
    }
    if( data->j ) {
        SetSignedChar(front);
        front.cvars.CompFlags.signed_char = true;
    }
    if( data->oa ) {
        front.cvars.GenSwitches |= CGSW_GEN_RELAX_ALIAS;
    }
    if( data->ob ) {
        front.cvars.GenSwitches |= CGSW_GEN_BRANCH_PREDICTION;
    }
    // following must follow processing of debug options
    if( data->oe ) {
        front.cvars.Inline_Threshold = data->oe_value;
        Toggles( front ).tog_inline = true;
    }
    if( data->oh ) {
        front.cvars.GenSwitches |= CGSW_GEN_SUPER_OPTIMAL;
    }
    if( data->oi ) {
        front.cvars.CompFlags.inline_functions = true;
    }
    if( data->ok ) {
        front.cvars.GenSwitches |= CGSW_GEN_FLOW_REG_SAVES;
    }
    if( data->ol ) {
        front.cvars.GenSwitches |= CGSW_GEN_LOOP_OPTIMIZATION;
    }
    if( data->ol_plus ) {
        front.cvars.GenSwitches |= CGSW_GEN_LOOP_OPTIMIZATION | CGSW_GEN_LOOP_UNROLLING;
    }
    if( data->on ) {
        front.cvars.GenSwitches |= CGSW_GEN_FP_UNSTABLE_OPTIMIZATION;
    }
    if( data->oo ) {
        front.cvars.GenSwitches &= ~CGSW_GEN_MEMORY_LOW_FAILS;
    }
    if( data->or_ ) {
        front.cvars.GenSwitches |= CGSW_GEN_INS_SCHEDULING;
    }
    if( data->ou ) {
        front.cvars.CompFlags.unique_functions = true;
    }
    if( data->oz ) {
        front.cvars.GenSwitches |= CGSW_GEN_NULL_DEREF_OK;
    }
    if( data->p ) {
        front.cvars.CompFlags.cpp_mode = true;
    }
    if( data->p_sharp ) {
        front.cvars.PreProcChar = (char)data->p_sharp_value;
    }
    if( data->pil ) {
        front.cvars.CompFlags.cpp_ignore_line = true;
    }
    if( data->pl ) {
        front.cvars.CompFlags.cpp_mode = true;
        front.cvars.CompFlags.cpp_line_wanted = true;
    }
    if( data->pc ) {
        front.cvars.CompFlags.cpp_mode = true;
        front.cvars.CompFlags.cpp_keep_comments = true;
    }
    if( data->pw ) {
        front.cvars.CompFlags.cpp_mode = true;
        SetPPWidth( front, data->pw_value );
    } else {
        /*
         * #line directives get screwed by wrapped lines but we don't want
         * to interfere with a user's setting of the width
         */
        if( data->pl ) {
            SetPPWidth( front, 0 );
        }
    }
    if( front.cvars.CompFlags.cpp_mode ) {
        front.cvars.CompFlags.cpp_output = true;
        front.cvars.CompFlags.quiet_mode = true;
    }
    if( data->pm ) {
        front.cvars.CompFlags.cpp_dump_macros = true;
        front.cvars.CompFlags.quiet_mode = true;
    }
    if( data->rod ) {
        OPT_STRING *s;
        for( s = data->rod_value; s != NULL; s = s->next ) {
            SrcFileReadOnlyDir( front, s->data );
        }
    }
    if( data->s ) {
        Toggles( front ).tog_check_stack = false;
    }
    if( data->v ) {
        front.cvars.CompFlags.dump_prototypes = true;
    }
    if( data->wcd ) {
        setMessageStatus( front, data->wcd_value, false );
    }
    if( data->wce ) {
        setMessageStatus( front, data->wce_value, true );
    }
    if( data->we ) {
        front.cvars.CompFlags.warnings_cause_bad_exit = true;
    }
    if( data->wpx ) {
        front.cvars.Check_global_prototype = true;
    }
    if( data->x ) {
        front.cvars.CompFlags.cpp_ignore_env = true;
    }
    if( data->xbsa ) {
        front.cvars.CompFlags.unaligned_segs = true;
    }
    if( data->xx ) {
        front.cvars.CompFlags.ignore_default_dirs = true;
    }
    if( data->zam ) {
        front.cvars.CompFlags.non_iso_compliant_names_enabled = false;
    }
    if( data->zev ) {
        front.cvars.CompFlags.unix_ext = true;
    }
    if( data->zg ) {
        front.cvars.CompFlags.generate_prototypes = true;
        front.cvars.CompFlags.dump_prototypes = true;
    }
    if( data->zl ) {
        front.cvars.CompFlags.emit_library_names = false;
    }
    if( data->zld ) {
        front.cvars.CompFlags.emit_dependencies = false;
    }
    if( data->zlf ) {
        front.cvars.CompFlags.emit_all_default_libs = true;
    }
    if( data->zls ) {
        front.cvars.CompFlags.emit_targimp_symbols = false;
    }
    if( data->zp ) {
        front.cvars.PackAmount = data->zp_value;
        front.cvars.GblPackAmount = front.cvars.PackAmount;
    }
    if( data->zpw ) {
        front.cvars.CompFlags.slack_byte_warning = true;
    }
    if( data->zs ) {
        front.cvars.CompFlags.check_syntax = true;
    }
    SetCharacterEncoding( front, data );
    CBanner(front);
}

bool MergeIncludeFromEnv( FrontRun& front, const char *env )
/*****************************************/
{
    const char  *env_value;

    if( front.cvars.CompFlags.cpp_ignore_env )
        return( false );
    env_value = FEGetEnv( front, env );
    if( env_value != NULL ) {
        AddIncList( front, env_value );
        return( true );
    }
    return( false );
}

static char *ReadIndirectFile( FrontRun& front, const char *fname )
{
    char        *env;
    char        *str;
    FILE        *fp;
    size_t      len;
    char        ch;

    env = NULL;
    fp = front.held->fopen( fname, "rb" );
    if( fp != NULL ) {
        fseek( fp, 0, SEEK_END );
        len = (size_t)ftell( fp );
        env = static_cast<char *>(CMemAlloc( front, len + 1 ));
        rewind( fp );
        len = fread( env, 1, len, fp );
        env[len] = '\0';
        front.held->fclose( fp );
        /*
         * zip through characters changing \r, \n etc into ' '
         */
        for( str = env; (ch = *str) != '\0'; str++ ) {
            if( ch == '\r'
              || ch == '\n' ) {
                *str = ' ';
            }
            /*
             * if DOS end of file (^Z) -> mark end of str
             */
            if( ch == DOS_EOF_CHAR ) {
                *str = '\0';
                break;
            }
        }
    }
    return( env );
}

#include "cmdlnprs.gc"

static void ProcOptions( FrontRun& front, OPT_STORAGE *data, const char *str )
{
#define MAX_NESTING 32
    int         level;
    const char  *save[MAX_NESTING];
    char        *buffers[MAX_NESTING];
    const char  *penv;
    char        *ptr;
    int         ch;                      // - next character
    OPT_STRING  *fname;

    if( str != NULL ) {
        level = -1;
        CmdScanLineInit( front, str );
        for( ;; ) {
            CmdScanSkipWhiteSpace(front);
            ch = CmdScanChar(front);
            if( ch == '@' ) {
                front.coptions->switch_start = CmdScanAddr(front) - 1;
                CmdScanSkipWhiteSpace(front);
                fname = NULL;
                if( OPT_GET_FILE( front, &fname ) ) {
                    penv = NULL;
                    level++;
                    if( level < MAX_NESTING ) {
                        ptr = NULL;
                        penv = FEGetEnv( front, fname->data );
                        if( penv == NULL ) {
                            ptr = ReadIndirectFile( front, fname->data );
                            penv = ptr;
                        }
                        if( penv != NULL ) {
                            save[level] = CmdScanLineInit( front, penv );
                            buffers[level] = ptr;
                        }
                    }
                    if( penv == NULL ) {
                        level--;
                    }
                    OPT_CLEAN_STRING( front, &fname );
                }
                continue;
            }
            if( ch == '\0' ) {
                if( level < 0 )
                    break;
                CMemFree( front, buffers[level] );
                CmdScanLineInit( front, save[level] );
                level--;
                continue;
            }
            if( CmdScanSwitchChar( ch ) ) {
                front.coptions->switch_start = CmdScanAddr(front) - 1;
                if( OPT_PROCESS( front, data ) ) {
                    /*
                     * incorrect option
                     */
                    CBanner(front);
                    BadCmdLineOption(front);
                }
            } else {  /* collect  file name */
                CmdScanUngetChar(front);
                front.coptions->switch_start = CmdScanAddr(front);
                fname = NULL;
                if( OPT_GET_FILE( front, &fname ) ) {
                    if( front.cvars.WholeFName != NULL ) {
                        /*
                         * more than one file to compile ?
                         */
                        CBanner(front);
                        CErr1( front, ERR_CAN_ONLY_COMPILE_ONE_FILE );
                    }
                    SetStringOption( front, &front.cvars.WholeFName, &fname );
                    OPT_CLEAN_STRING( front, &fname );
                }
            }
        }
    }
#undef MAX_NESTING
}

void GenCOptions( FrontRun& front, char **cmdline )
{
    OPT_STORAGE data;

    /*
     * Add precision warning but disabled by default
     */
    WarnEnableDisable( front, false, ERR_LOSE_PRECISION );
    /*
     * Warning about non-prototype declarations is disabled by default
     * because Windows and OS/2 API headers use it
     */
    WarnEnableDisable( front, false, ERR_OBSOLETE_FUNC_DECL );
    /*
     * Warning about pointer truncation during cast is disabled by
     * default because it would cause too many build breaks right now
     * by correctly diagnosing broken code.
     */
    WarnEnableDisable( front, false, ERR_CAST_POINTER_TRUNCATION );
    InitModInfo(front);
    CmdSysInit(front);
    OPT_INIT( &data );
#if _CPU == 8086
    ProcOptions( front, &data, FEGetEnv( front, "WCC" ) );
#elif _CPU == 386
    ProcOptions( front, &data, FEGetEnv( front, "WCC386" ) );
#endif
    for( ;*cmdline != NULL; ++cmdline ) {
        ProcOptions( front, &data, *cmdline );
    }
    if( front.cvars.CompFlags.cpp_mode ) {
        front.cvars.CompFlags.cpp_output = true;
        front.cvars.CompFlags.quiet_mode = true;
    }
    /*
     * print banner if -zq not specified
     */
    AnalyseAnyTargetOptions( front, &data );
    CmdSysAnalyse( front, &data );
    OPT_FINI( front, &data );
    MiscMacroDefs(front);
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
CoptionsState* coptions_make() { return new CoptionsState(); }
void coptions_free(CoptionsState* part) { delete part; }
