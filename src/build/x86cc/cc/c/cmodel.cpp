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
#ifdef DEVBUILD
    #include "togglesd.h"
#endif


extern  char    const CompilerID[];

/*
 * COMMAND LINE PARSING OF MACRO DEFINITIONS
 */

static bool_ EqualChar( int c )
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


static char *Def_Macro_Tokens( const char *str, bool_ multiple_tokens, macro_flags mflags )
{
    size_t      mlen;
    MEPTR       mentry;
    bool_        ppscan_mode;
    TOKEN       token;

    mlen = get_token_len( str );
    if( mlen == 0 ) {
        CErr1( ERR_NO_MACRO_ID_COMMAND_LINE );
        return( (char *)str );
    }
    mentry = CreateMEntry( str, mlen );
    str += mlen;
    mlen = mentry->macro_len;
    mentry->macro_defn = mlen;
    if( !EqualChar( *str ) ) {
        MacroSegmentAddToken( &mlen, T_PPNUMBER );
        MacroSegmentAddChar( &mlen, '1' );
        MacroSegmentAddChar( &mlen, '\0' );
    } else {
        ppscan_mode = InitPPScan();
        ReScanInit( ++str );
        for( ; *str != '\0'; ) {
            token = ReScanToken();
            if( ReScanPos() == str )
                break;
            if( token == T_WHITE_SPACE )
                break;
            if( token == T_BAD_CHAR && !multiple_tokens )
                break;
            MacroSegmentAddToken( &mlen, token );
            switch( token ) {
            case T_BAD_CHAR:
                MacroSegmentAddChar( &mlen, cvars_state().Buffer[0] );
                break;
            case T_CONSTANT:
            case T_PPNUMBER:
            case T_ID:
            case T_LSTRING:
            case T_STRING:
                MacroSegmentAddMem( &mlen, cvars_state().Buffer, cvars_state().TokenLen + 1 );
                break;
            default:
                break;
            }
            str = ReScanPos();
            if( !multiple_tokens ) {
                break;
            }
        }
        FiniPPScan( ppscan_mode );
    }
    MacroSegmentAddToken( &mlen, T_NULL );
    if( IS_PPOPERATOR_DEFINED( mentry->macro_name ) ) {
        CErr1( ERR_CANT_DEFINE_DEFINED );
    } else {
        MacroDefine( mlen, mflags );
    }
    return( (char *)str );
}

char *Define_Macro( const char *str )
{
    return( Def_Macro_Tokens( str, cvars_state().CompFlags.extended_defines, MFLAG_NONE ) );
}

char *Define_UserMacro( const char *str, bool_ multiple_tokens )
{
    return( Def_Macro_Tokens( str, multiple_tokens, MFLAG_USER_DEFINED ) );
}

void PreDefine_Macro( const char *str )
{
    undef_names     *uname;
    size_t          len;
    const char      *p;

    if( !cvars_state().CompFlags.undefine_all_macros ) {
        if( cvars_state().UndefNames != NULL ) {
            len = 0;
            for( p = str; *p != '\0'; ++p ) {
                if( EqualChar( *p ) )
                    break;
                ++len;
            }
            for( uname = cvars_state().UndefNames; uname != NULL; uname = uname->next ) {
                if( strncmp( uname->name, str, len ) == 0 ) {
                    if( uname->name[len] == '\0' ) {
                        return;
                    }
                }
            }
        }
        Def_Macro_Tokens( str, true, MFLAG_CAN_BE_REDEFINED );
    }
}


void AddUndefMacro( const char *str )
{
    size_t          len;
    undef_names     *uname;

    len = strlen( str );
    if( len == 0 ) {
        cvars_state().CompFlags.undefine_all_macros = true;
    } else {
        if( !MacroDel( str ) ) {
            uname = (undef_names *)CMemAlloc( sizeof( undef_names ) );
            uname->next = cvars_state().UndefNames;
            uname->name = CMemStrdup( str );
            cvars_state().UndefNames = uname;
        }
    }
}


static void FreeUndefNames( void )
{
    undef_names     *uname;

    for( ; (uname = cvars_state().UndefNames) != NULL; ) {
        cvars_state().UndefNames = uname->next;
        CMemFree( uname->name );
        CMemFree( uname );
    }
}


static void PreDefine_Macros_Extension( void )
{
    PreDefine_Macro( "_far16=__far16" );
    PreDefine_Macro( "near=__near" );
    PreDefine_Macro( "far=__far" );
    PreDefine_Macro( "huge=__huge" );
    PreDefine_Macro( "cdecl=__cdecl" );
    PreDefine_Macro( "pascal=__pascal" );
    PreDefine_Macro( "fortran=__fortran" );
    PreDefine_Macro( "interrupt=__interrupt" );
    PreDefine_Macro( "_near=__near" );
    PreDefine_Macro( "_far=__far" );
    PreDefine_Macro( "_huge=__huge" );
    PreDefine_Macro( "_cdecl=__cdecl" );
    PreDefine_Macro( "_pascal=__pascal" );
    PreDefine_Macro( "_fortran=__fortran" );
    PreDefine_Macro( "_inline=__inline" );
    PreDefine_Macro( "_fastcall=__fastcall" );
    PreDefine_Macro( "_interrupt=__interrupt" );
    PreDefine_Macro( "_export=__export" );
    PreDefine_Macro( "_loadds=__loadds" );
    PreDefine_Macro( "_saveregs=__saveregs" );
    PreDefine_Macro( "_stdcall=__stdcall" );
    PreDefine_Macro( "_syscall=__syscall" );
    PreDefine_Macro( "_based=__based" );
    PreDefine_Macro( "_self=__self" );
    PreDefine_Macro( "_segname=__segname" );
    PreDefine_Macro( "_segment=__segment" );
    PreDefine_Macro( "_try=__try");
    PreDefine_Macro( "_except=__except");
    PreDefine_Macro( "_finally=__finally");
    PreDefine_Macro( "_leave=__leave");
    PreDefine_Macro( "_asm=__asm");
#if _CPU == 8086
    /*
     * SOM for Windows macros
     */
    PreDefine_Macro( "SOMLINK=__cdecl" );
    PreDefine_Macro( "SOMDLINK=__far" );
#else
    PreDefine_Macro( "SOMLINK=_Syscall" );
    PreDefine_Macro( "SOMDLINK=_Syscall" );
#endif
}


void MiscMacroDefs( void )
{
    if( cvars_state().CompFlags.inline_functions ) {
        Define_Macro( "__INLINE_FUNCTIONS__" );
    }
    if( !cvars_state().CompFlags.extensions_enabled ) {
        Define_Macro( "_NO_EXT_KEYS" );
        if( cvars_state().CompFlags.non_iso_compliant_names_enabled ) {
            Define_Macro( "NO_EXT_KEYS" );
        }
    }
    if( cvars_state().CompFlags.non_iso_compliant_names_enabled ) {
        PreDefine_Macros_Extension();
    }
    if( cvars_state().CompFlags.signed_char ) {
        Define_Macro( "__CHAR_SIGNED__" );
    }
    if( cvars_state().CompFlags.rent ) {
        Define_Macro( "__RENT__" );
    }
    PreDefine_Macro( "_PUSHPOP_SUPPORTED" );
    Define_Macro( "__STDC_NO_VLA__" );
    PreDefine_Macro( CompilerID );
    FreeUndefNames();
}

void InitModInfo( void )
{
    cvars_state().UndefNames = NULL;
    cvars_state().WholeFName = NULL;
    cvars_state().ObjectFileName = NULL;
    cvars_state().ErrorFileName = CMemStrdup( "*" );
    cvars_state().DependFileName = NULL;
    cvars_state().DependForceSlash = 0;
    cvars_state().ModuleName = NULL;
    cvars_state().CodeClassName = NULL;
    cvars_state().PCH_FileName = NULL;
    cvars_state().ErrLimit = 20;
    cvars_state().WngLevel = WLEVEL_DEFAULT;

    cvars_state().PreProcChar = '#';
    cvars_state().CompVars.cstd = STD_C89;

    cvars_state().CompFlags.check_syntax                      = false;
    cvars_state().CompFlags.signed_char                       = false;
    cvars_state().CompFlags.use_full_codegen_od               = false;
    cvars_state().CompFlags.inline_functions                  = false;
    cvars_state().CompFlags.dump_prototypes                   = false;
    cvars_state().CompFlags.generate_prototypes               = false;
    cvars_state().CompFlags.bss_segment_used                  = false;
    cvars_state().CompFlags.undefine_all_macros               = false;
    cvars_state().CompFlags.extensions_enabled                = true;
    cvars_state().CompFlags.non_iso_compliant_names_enabled   = true;
    cvars_state().CompFlags.unix_ext                          = false;
    cvars_state().CompFlags.slack_byte_warning                = false;
    cvars_state().CompFlags.errfile_written                   = false;
    cvars_state().CompFlags.zu_switch_used                    = false;
    cvars_state().CompFlags.pragma_library                    = false;
    cvars_state().CompFlags.emit_all_default_libs             = false;
    cvars_state().CompFlags.emit_library_names                = true;
    cvars_state().CompFlags.emit_dependencies                 = true;
    cvars_state().CompFlags.emit_targimp_symbols              = true;
    cvars_state().CompFlags.no_debug_type_names               = false;
    cvars_state().CompFlags.auto_agg_inits                    = false;
    cvars_state().CompFlags.no_check_inits                    = false;
    cvars_state().CompFlags.no_check_qualifiers               = false;
    cvars_state().CompFlags.ignore_default_dirs               = false;
    cvars_state().CompFlags.use_stdcall_at_number             = true;
    cvars_state().CompFlags.rent                              = false;
    cvars_state().CompFlags.check_truncated_fnames            = true;

    memset( &toggles_state().PragmaToggles, 0, sizeof( toggles_state().PragmaToggles ) );
#ifdef DEVBUILD
    memset( &PragmaDbgToggles, 0, sizeof( PragmaDbgToggles ) );
#endif
    TOGGLE( check_stack ) = true;
    TOGGLE( unreferenced ) = true;
    TOGGLE( reuse_duplicate_strings ) = true;
}
