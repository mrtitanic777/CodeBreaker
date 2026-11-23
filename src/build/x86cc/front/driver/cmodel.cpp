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
#include "cmdscan.h"
#include "scan.h"
#include "pdefn2.h"
#include <ctype.h>
#include "cmacadd.h"
#include "toggles.h"
#include "frontrun.hpp"


extern  char    const CompilerID[];

/*
 * COMMAND LINE PARSING OF MACRO DEFINITIONS
 */

static bool EqualChar( int c )
{
    return( c == '#'
        || c == '=' );
}

static size_t get_token_len( const char *start )
{
    int         ch;
    const char  *src;

    for( src = start; (ch = *(unsigned char *)src) != '\0'; ++src ) {
        if( EqualChar( ch ) )
            break;
        if( ch == ' ' )
            break;
        if( CmdScanSwitchChar( ch ) ) {
            break;
        }
    }
    return( src - start );
}


static char *Def_Macro_Tokens( FrontRun& front, const char *str, bool multiple_tokens, macro_flags mflags )
{
    size_t      mlen;
    MEPTR       mentry;
    bool        ppscan_mode;
    TOKEN       token;

    mlen = get_token_len( str );
    if( mlen == 0 ) {
        CErr1( front, ERR_NO_MACRO_ID_COMMAND_LINE );
        return( (char *)str );
    }
    mentry = CreateMEntry( front, str, mlen );
    str += mlen;
    mlen = mentry->macro_len;
    mentry->macro_defn = mlen;
    if( !EqualChar( *str ) ) {
        MacroSegmentAddToken( front, &mlen, T_PPNUMBER );
        MacroSegmentAddChar( front, &mlen, '1' );
        MacroSegmentAddChar( front, &mlen, '\0' );
    } else {
        ppscan_mode = InitPPScan(front);
        ReScanInit( front, ++str );
        for( ; *str != '\0'; ) {
            token = ReScanToken(front);
            if( ReScanPos(front) == str )
                break;
            if( token == T_WHITE_SPACE )
                break;
            if( token == T_BAD_CHAR && !multiple_tokens )
                break;
            MacroSegmentAddToken( front, &mlen, token );
            switch( token ) {
            case T_BAD_CHAR:
                MacroSegmentAddChar( front, &mlen, front.cvars.Buffer[0] );
                break;
            case T_CONSTANT:
            case T_PPNUMBER:
            case T_ID:
            case T_LSTRING:
            case T_STRING:
                MacroSegmentAddMem( front, &mlen, front.cvars.Buffer, front.cvars.TokenLen + 1 );
                break;
            default:
                break;
            }
            str = ReScanPos(front);
            if( !multiple_tokens ) {
                break;
            }
        }
        FiniPPScan( front, ppscan_mode );
    }
    MacroSegmentAddToken( front, &mlen, T_NULL );
    if( IS_PPOPERATOR_DEFINED( mentry->macro_name ) ) {
        CErr1( front, ERR_CANT_DEFINE_DEFINED );
    } else {
        MacroDefine( front, mlen, mflags );
    }
    return( (char *)str );
}

char *Define_Macro( FrontRun& front, const char *str )
{
    return( Def_Macro_Tokens( front, str, front.cvars.CompFlags.extended_defines, MFLAG_NONE ) );
}

char *Define_UserMacro( FrontRun& front, const char *str, bool multiple_tokens )
{
    return( Def_Macro_Tokens( front, str, multiple_tokens, MFLAG_USER_DEFINED ) );
}

void PreDefine_Macro( FrontRun& front, const char *str )
{
    undef_names     *uname;
    size_t          len;
    const char      *p;

    if( !front.cvars.CompFlags.undefine_all_macros ) {
        if( front.cvars.UndefNames != NULL ) {
            len = 0;
            for( p = str; *p != '\0'; ++p ) {
                if( EqualChar( *p ) )
                    break;
                ++len;
            }
            for( uname = front.cvars.UndefNames; uname != NULL; uname = uname->next ) {
                if( strncmp( uname->name, str, len ) == 0 ) {
                    if( uname->name[len] == '\0' ) {
                        return;
                    }
                }
            }
        }
        Def_Macro_Tokens( front, str, true, MFLAG_CAN_BE_REDEFINED );
    }
}


void AddUndefMacro( FrontRun& front, const char *str )
{
    size_t          len;
    undef_names     *uname;

    len = strlen( str );
    if( len == 0 ) {
        front.cvars.CompFlags.undefine_all_macros = true;
    } else {
        if( !MacroDel( front, str ) ) {
            uname = (undef_names *)CMemAlloc( front, sizeof( undef_names ) );
            uname->next = front.cvars.UndefNames;
            uname->name = CMemStrdup( front, str );
            front.cvars.UndefNames = uname;
        }
    }
}


static void FreeUndefNames( FrontRun& front )
{
    undef_names     *uname;

    for( ; (uname = front.cvars.UndefNames) != NULL; ) {
        front.cvars.UndefNames = uname->next;
        CMemFree( front, uname->name );
        CMemFree( front, uname );
    }
}


static void PreDefine_Macros_Extension( FrontRun& front )
{
    PreDefine_Macro( front, "_far16=__far16" );
    PreDefine_Macro( front, "near=__near" );
    PreDefine_Macro( front, "far=__far" );
    PreDefine_Macro( front, "huge=__huge" );
    PreDefine_Macro( front, "cdecl=__cdecl" );
    PreDefine_Macro( front, "pascal=__pascal" );
    PreDefine_Macro( front, "fortran=__fortran" );
    PreDefine_Macro( front, "interrupt=__interrupt" );
    PreDefine_Macro( front, "_near=__near" );
    PreDefine_Macro( front, "_far=__far" );
    PreDefine_Macro( front, "_huge=__huge" );
    PreDefine_Macro( front, "_cdecl=__cdecl" );
    PreDefine_Macro( front, "_pascal=__pascal" );
    PreDefine_Macro( front, "_fortran=__fortran" );
    PreDefine_Macro( front, "_inline=__inline" );
    PreDefine_Macro( front, "_fastcall=__fastcall" );
    PreDefine_Macro( front, "_interrupt=__interrupt" );
    PreDefine_Macro( front, "_export=__export" );
    PreDefine_Macro( front, "_loadds=__loadds" );
    PreDefine_Macro( front, "_saveregs=__saveregs" );
    PreDefine_Macro( front, "_stdcall=__stdcall" );
    PreDefine_Macro( front, "_syscall=__syscall" );
    PreDefine_Macro( front, "_based=__based" );
    PreDefine_Macro( front, "_self=__self" );
    PreDefine_Macro( front, "_segname=__segname" );
    PreDefine_Macro( front, "_segment=__segment" );
    PreDefine_Macro( front, "_try=__try");
    PreDefine_Macro( front, "_except=__except");
    PreDefine_Macro( front, "_finally=__finally");
    PreDefine_Macro( front, "_leave=__leave");
    PreDefine_Macro( front, "_asm=__asm");
#if _CPU == 8086
    /*
     * SOM for Windows macros
     */
    PreDefine_Macro( front, "SOMLINK=__cdecl" );
    PreDefine_Macro( front, "SOMDLINK=__far" );
#else
    PreDefine_Macro( front, "SOMLINK=_Syscall" );
    PreDefine_Macro( front, "SOMDLINK=_Syscall" );
#endif
}


void MiscMacroDefs( FrontRun& front )
{
    if( front.cvars.CompFlags.inline_functions ) {
        Define_Macro( front, "__INLINE_FUNCTIONS__" );
    }
    if( !front.cvars.CompFlags.extensions_enabled ) {
        Define_Macro( front, "_NO_EXT_KEYS" );
        if( front.cvars.CompFlags.non_iso_compliant_names_enabled ) {
            Define_Macro( front, "NO_EXT_KEYS" );
        }
    }
    if( front.cvars.CompFlags.non_iso_compliant_names_enabled ) {
        PreDefine_Macros_Extension(front);
    }
    if( front.cvars.CompFlags.signed_char ) {
        Define_Macro( front, "__CHAR_SIGNED__" );
    }
    if( front.cvars.CompFlags.rent ) {
        Define_Macro( front, "__RENT__" );
    }
    PreDefine_Macro( front, "_PUSHPOP_SUPPORTED" );
    Define_Macro( front, "__STDC_NO_VLA__" );
    PreDefine_Macro( front, CompilerID );
    FreeUndefNames(front);
}

void InitModInfo( FrontRun& front )
{
    front.cvars.UndefNames = NULL;
    front.cvars.WholeFName = NULL;
    front.cvars.ObjectFileName = NULL;
    front.cvars.ErrorFileName = CMemStrdup( front, "*" );
    front.cvars.DependFileName = NULL;
    front.cvars.DependForceSlash = 0;
    front.cvars.ModuleName = NULL;
    front.cvars.CodeClassName = NULL;
    front.cvars.PCH_FileName = NULL;
    front.cvars.ErrLimit = 20;
    front.cvars.WngLevel = WLEVEL_DEFAULT;

    front.cvars.PreProcChar = '#';
    front.cvars.CompVars.cstd = STD_C89;

    front.cvars.CompFlags.check_syntax                      = false;
    front.cvars.CompFlags.signed_char                       = false;
    front.cvars.CompFlags.use_full_codegen_od               = false;
    front.cvars.CompFlags.inline_functions                  = false;
    front.cvars.CompFlags.dump_prototypes                   = false;
    front.cvars.CompFlags.generate_prototypes               = false;
    front.cvars.CompFlags.bss_segment_used                  = false;
    front.cvars.CompFlags.undefine_all_macros               = false;
    front.cvars.CompFlags.extensions_enabled                = true;
    front.cvars.CompFlags.non_iso_compliant_names_enabled   = true;
    front.cvars.CompFlags.unix_ext                          = false;
    front.cvars.CompFlags.slack_byte_warning                = false;
    front.cvars.CompFlags.errfile_written                   = false;
    front.cvars.CompFlags.zu_switch_used                    = false;
    front.cvars.CompFlags.pragma_library                    = false;
    front.cvars.CompFlags.emit_all_default_libs             = false;
    front.cvars.CompFlags.emit_library_names                = true;
    front.cvars.CompFlags.emit_dependencies                 = true;
    front.cvars.CompFlags.emit_targimp_symbols              = true;
    front.cvars.CompFlags.no_debug_type_names               = false;
    front.cvars.CompFlags.auto_agg_inits                    = false;
    front.cvars.CompFlags.no_check_inits                    = false;
    front.cvars.CompFlags.no_check_qualifiers               = false;
    front.cvars.CompFlags.ignore_default_dirs               = false;
    front.cvars.CompFlags.use_stdcall_at_number             = true;
    front.cvars.CompFlags.rent                              = false;
    front.cvars.CompFlags.check_truncated_fnames            = true;

    memset( &front.toggles.PragmaToggles, 0, sizeof( front.toggles.PragmaToggles ) );
    Toggles( front ).tog_check_stack = true;
    Toggles( front ).tog_unreferenced = true;
    Toggles( front ).tog_reuse_duplicate_strings = true;
}
