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
* Description:  Macro processing, part one.
*
****************************************************************************/


#include "cvars.h"
#include "i64.h"
#include "scan.h"
#include "cfeinfo.h"
#include "cmacadd.h"
#include "ppexpn.h"
#include "cscanbuf.h"
#include "dumpapi.h"

#include "clibext.h"
#include "frontrun.hpp"


#define MACRO_END_CHAR          'Z'
#define MACRO_END_STRING        "Z-<end of macro>"
#define MACRO_T_NULL            ""











extern void         DumpMTokens( MACRO_TOKEN *mtok );


static MACRO_TOKEN  *ExpandNestedMacros( FrontRun& front, MACRO_TOKEN *mtok_head, bool rescanning );

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct Cmac1State: cmac1_state.h) */

}  // namespace



void InitTokenBuf( FrontRun& front, size_t size )
/******************************/
{
    front.cvars.TokenBuf = static_cast<char *>(CMemAlloc( front, size ));
    front.cmac1.TokenBufSize = size;
}

static void EnlargeTokenBuf( FrontRun& front, size_t size )
/****************************************/
{
    char    *newBuffer;

//    size += 32;     /* TokenBuf size margin */
    if( size > front.cmac1.TokenBufSize ) {
        size = RoundUp( size, BUF_SIZE );
        newBuffer = static_cast<char *>(CMemAlloc( front, size ));
        memcpy( newBuffer, front.cvars.TokenBuf, front.cmac1.TokenBufSize );
        CMemFree( front, (void *)front.cvars.TokenBuf );
        front.cvars.TokenBuf = newBuffer;
        front.cmac1.TokenBufSize = size;
    }
}

static size_t WriteTokenBufToken( FrontRun& front, size_t i, TOKEN t )
/***************************************************/
{
    EnlargeTokenBuf( front, i + SIZE_MTOKEN );
    SetMtoken( front.cvars.TokenBuf + i, t );
    return( i + SIZE_MTOKEN );
}

static size_t WriteTokenBufChar( FrontRun& front, size_t i, char c )
/*************************************************/
{
    EnlargeTokenBuf( front, i + 1 );
    front.cvars.TokenBuf[i++] = c;
    return( i );
}

static size_t WriteTokenBufMem( FrontRun& front, size_t i, const char *buf, size_t len )
/*********************************************************************/
{
    EnlargeTokenBuf( front, i + len );
    memcpy( front.cvars.TokenBuf + i, buf, len );
    return( i + len );
}

static size_t TokenBufRemoveWSToken( size_t i )
/*********************************************/
{
    if( i >= SIZE_MTOKEN )
        return( i - SIZE_MTOKEN );
    return( i );
}

static void SpecialMacroAdd( FrontRun& front, special_macro_names *mac )
/*****************************************************/
{
    MEPTR           mentry;

    mentry = CreateMEntry( front, mac->name, 0 );
    mentry->u.special_macro = mac->value;
    MacroDefine( front, mentry->macro_len, mac->flags );
}

void MacroInit( FrontRun& front )
/********************/
{
    mac_hash_idx    hash;
    int             i;

    front.cvars.MacroCount = 0;
    front.cvars.MacroPtr = NULL;
    CppStackInit(front);
    InitPPexpn(front);
    front.cmac1.NestedMacros = NULL;
    front.cmac1.MTokenList = NULL;
    front.cvars.UndefMacroList = NULL;
    front.cvars.InitialMacroFlags = MFLAG_DEFINED_BEFORE_FIRST_INCLUDE;
    front.cvars.MacHash = static_cast<MEPTR *>(PermMemAlloc( front, MACRO_HASH_SIZE * sizeof( MEPTR ) ));
    for( hash = 0; hash < MACRO_HASH_SIZE; hash++ ) {
        front.cvars.MacHash[hash] = NULL;
    }
    for( i = MACRO_FIRST; i <= MACRO_LAST; i++ ) {
        SpecialMacroAdd( front, &front.cmac1.SpcMacros[i] );
    }
    /*
     * grab time and date for __TIME__ and __DATE__
     */
    TimeInit(front);
}

void MacroAddComp( FrontRun& front )
/***********************/
{
    int             i;

    for( i = MACRO_COMP_FIRST; i <= MACRO_COMP_LAST; i++ ) {
        SpecialMacroAdd( front, &front.cmac1.SpcMacros[i] );
    }
}

void MacroFini( FrontRun& front )
/********************/
{
    FiniPPexpn(front);
    CppStackFini(front);
    MacroPurge();
}


void MacroPurge( void )
/*********************/
{
}


static void DumpMacroDefn( FrontRun& front, const char *p )
/****************************************/
{
    unsigned char   c;
    TOKEN           token;

    while( (token = static_cast<TOKEN>(GetMtoken( p ))) != T_NULL ) {
        p += SIZE_MTOKEN;
        switch( token ) {
        case T_CONSTANT:
        case T_PPNUMBER:
        case T_BAD_TOKEN:
        case T_ID:
        case T_UNEXPANDABLE_ID:
            for( ; (c = *p++) != '\0'; ) {
                fputc( c, front.cvars.CppFile );
            }
            break;
        case T_LSTRING:
            fputc( 'L', front.cvars.CppFile );
            /* fall through */
        case T_STRING:
            fputc( '"', front.cvars.CppFile );
            for( ; (c = *p++) != '\0'; ) {
                fputc( c, front.cvars.CppFile );
            }
            fputc( '"', front.cvars.CppFile );
            break;
        case T_WHITE_SPACE:
            fputc( ' ', front.cvars.CppFile );
            break;
        case T_BAD_CHAR:
            fputc( *p++, front.cvars.CppFile );
            break;
        case T_MACRO_PARM:
        case T_MACRO_VAR_PARM:
            p += SIZE_MPARM;
            break;
        default:
            fprintf( front.cvars.CppFile, "%s", TokenString[token] );
            break;
        }
    }
}


void DumpAllMacros( FrontRun& front )
/************************/
{
    mac_hash_idx    hash;
    MEPTR           mentry;
    const char      *p;

    for( hash = 0; hash < MACRO_HASH_SIZE; hash++ ) {
        for( mentry = front.cvars.MacHash[hash]; mentry != NULL; mentry = mentry->next_macro ) {
            if( mentry->macro_flags & MFLAG_HIDDEN )
                continue;
            if( MacroIsSpecial( mentry ) )
                continue;
            fprintf( front.cvars.CppFile, "#define %s", mentry->macro_name );
            p = (char *)mentry + mentry->macro_defn;
            if( GetMtoken( p ) != T_NULL ) {
                fputc( ' ', front.cvars.CppFile );
                DumpMacroDefn( front, p );
            }
            fputc( '\n', front.cvars.CppFile );
        }
    }
    fflush( front.cvars.CppFile );
}


static void DeleteNestedMacro( FrontRun& front )
/***********************************/
{
    MEPTR           mentry;
    NESTED_MACRO    *nested;
    MACRO_ARG       *macro_parms;
    int             parm_idx;

    nested = front.cmac1.NestedMacros;
    if( nested != NULL ) {
        front.cmac1.NestedMacros = nested->next;
        macro_parms =  nested->macro_parms;
        mentry = nested->mentry;
        CMemFree( front, nested );
        if( macro_parms != NULL ) {
            for( parm_idx = 0; parm_idx < GetMacroParmCount( mentry ); ++parm_idx ) {
                CMemFree( front, macro_parms[parm_idx].arg );
            }
            CMemFree( front, macro_parms );
        }
    }
}

TOKEN GetMacroToken( FrontRun& front )
/*************************/
{
    MACRO_TOKEN     *mtok;
    bool            keep_token;
    TOKEN           token;

    token = T_NULL;
    for( ; (mtok = front.cmac1.MTokenList) != NULL; ) {
        if( (token = mtok->token) != T_NULL ) {
            break;
        }
        if( mtok->data[0] == MACRO_END_CHAR ) {     // if end of macro
            DeleteNestedMacro(front);
        }
        front.cmac1.MTokenList = mtok->next;
        CMemFree( front, mtok );
    }
    front.cvars.TokenLen = 0;
    if( mtok == NULL ) {
        front.cvars.MacroPtr = NULL;
        token = T_NULL;
        front.cvars.Buffer[0] = '\0';
    } else {
        /*
         * size of Buffer is OK, token data was processed in Buffer before
         */
        while( (front.cvars.Buffer[front.cvars.TokenLen] = mtok->data[front.cvars.TokenLen]) != '\0' ) {
            front.cvars.TokenLen++;
        }
        keep_token = false;
        switch( token ) {
        case T_SAVED_ID:
            if( front.cvars.CompFlags.doing_macro_expansion ) {
                token = T_ID;
            }
            /* fall through */
        case T_ID:
        case T_UNEXPANDABLE_ID:
            if( !front.cvars.CompFlags.doing_macro_expansion ) {
                if( IsPPOperatorPragma( front, front.cvars.Buffer, front.cvars.TokenLen ) ) {
                    front.cmac1.MTokenList = mtok->next;
                    CMemFree( front, mtok );
                    token = Process_Pragma(front);
                    mtok = front.cmac1.MTokenList;
                    keep_token = true;
                } else {
                    token = KwLookup( front, front.cvars.Buffer, front.cvars.TokenLen );
                }
            }
            break;
        case T_BAD_TOKEN:
        case T_CONSTANT:
            ReScanInit( front, mtok->data );
            token = ReScanToken(front);
            break;
        case T_PPNUMBER:
            ReScanInit( front, mtok->data );
            token = ReScanToken(front);
            if( !front.cvars.CompFlags.rescan_buffer_done ) {   // didn't finish string bad boy
                const char  *tcur;
                char        *tbeg;

                tbeg = mtok->data;
                tcur = ReScanPos(front);
                /*
                 * ppnumber is quite general so it may absorb multiple tokens
                 * overlapping src & dst so do our own copy;
                 */
                for( ; (*tbeg = *tcur) != '\0'; ) {
                    ++tcur;
                    ++tbeg;
                }
                keep_token = true;
            }
            break;
        case T_LSTRING:
            token = T_STRING;
            front.cvars.CompFlags.wide_char_string = true;
            break;
        case T_STRING:
            front.cvars.CompFlags.wide_char_string = false;
            break;
        }
        if( !keep_token ) {
            front.cmac1.MTokenList = mtok->next;
            CMemFree( front, mtok );
        }
    }
    return( token );
}

static char *expandMacroToken( FrontRun& front, MACRO_TOKEN *mtok )
/*************************************************
 * returns Dynamically allocated buffer with expanded macro
 */
{
    size_t      len;
    const char  *src;
    char        *dst;
    char        *buf;

    len = 0;
    switch( mtok->token ) {
    case T_NULL:
        return( NULL );
    case T_CONSTANT:
    case T_PPNUMBER:
    case T_ID:
    case T_UNEXPANDABLE_ID:
    case T_SAVED_ID:
    case T_BAD_TOKEN:
        return( CMemStrdup( front, mtok->data ) );
    case T_LSTRING:
        len = 1;
        /* fall through */
    case T_STRING:
        src = mtok->data;
        dst = buf = static_cast<char *>(CMemAlloc( front, len + strlen( src ) + 3 ));
        if( mtok->token == T_LSTRING )
            *dst++ = 'L';
        *dst++ = '"';
        while( (*dst = *src++) != '\0' )
            dst++;
        *dst++ = '"';
        *dst = '\0';
        return( buf );
    default:
        return( CMemStrdup( front, TokenString[mtok->token] ) );
    }
}

TOKEN SpecialMacro( FrontRun& front, MEPTR mentry )
/********************************/
{
    const char      *p;

    front.cvars.TokenLen = 0;
    front.cvars.CompFlags.wide_char_string = false;
    switch( mentry->u.special_macro ) {
    case MACRO_LINE:
        front.cvars.TokenLen = sprintf( front.cvars.Buffer, "%u", front.cvars.TokenLoc.line );
        Set64ValU32( front.cvars.Constant64, front.cvars.TokenLoc.line );
        front.cvars.ConstType = TYP_INT;
        return( T_CONSTANT );
    case MACRO_FILE:
        p = FileIndexToFName( front, front.cvars.TokenLoc.fno )->name;
        WriteBufferEscStr( front, &p, false );
        return( T_STRING );
    case MACRO_DATE:
        WriteBufferString( front, front.cvars.__Date );
        return( T_STRING );
    case MACRO_TIME:
        WriteBufferString( front, front.cvars.__Time );
        return( T_STRING );
    case MACRO_STDC:
    case MACRO_STDC_HOSTED:
        WriteBufferString( front, "1" );
        Set64Val1p( front.cvars.Constant64 );
        front.cvars.ConstType = TYP_INT;
        return( T_CONSTANT );
    case MACRO_STDC_VERSION:
        if( front.cvars.CompVars.cstd < STD_C99 ) {
            WriteBufferString( front, "199409L" );
            Set64ValU32( front.cvars.Constant64, 199409 );
        } else {
            WriteBufferString( front, "199901L" ); /* C99 */
//            WriteBufferString( "201112L" ); /* C11 */
//            WriteBufferString( "201710L" ); /* C17 */
//            WriteBufferString( "202311L" ); /* C23 */
            Set64ValU32( front.cvars.Constant64, 199901 );  /* C99 */
//            Set64ValU32( Constant64, 201112 );  /* C11 */
//            Set64ValU32( Constant64, 201710 );  /* C17 */
//            Set64ValU32( Constant64, 202311 );  /* C23 */
        }
        front.cvars.ConstType = TYP_LONG;
        return( T_CONSTANT );
    case MACRO_FUNCTION:
    case MACRO_FUNC:
        front.cvars.Buffer[0] = '\0';
        if( front.cvars.CurFunc != NULL && front.cvars.CurFunc->name != NULL ) {
            WriteBufferString( front, front.cvars.CurFunc->name );
        }
        return( T_STRING );
    default:
        front.cvars.Buffer[0] = '\0';
        return( T_NULL );   // shut up the compiler
    }
}


static TOKEN NextMToken( FrontRun& front )
/*****************************/
{
    TOKEN   token;

    front.cvars.CompFlags.doing_macro_expansion = true;
    token = GetMacroToken(front);
    if( token == T_NULL ) {
        token = ScanToken(front);
    }
    front.cvars.CompFlags.doing_macro_expansion = false;
    return( token );
}

static void SaveParm( FrontRun& front, MEPTR mentry, size_t len, int parm_idx, MACRO_ARG *macro_parms )
/************************************************************************************/
{
    char            *p;

    len = WriteTokenBufToken( front, len, T_NULL );
    if( parm_idx < GetMacroParmCount( mentry ) ) {
        p = static_cast<char *>(CMemAlloc( front, len ));
        macro_parms[parm_idx].arg = p;
        if( p != NULL ) {
            memcpy( p, front.cvars.TokenBuf, len );
        }
    }
}

static MACRO_ARG *CollectParms( FrontRun& front, MEPTR mentry )
/********************************************/
{
    size_t          len;
    int             bracket;
    TOKEN           token;
    TOKEN           token_prev;
    int             parm_idx;
    int             parm_count_reqd;
    bool            ppscan_mode;
    MACRO_ARG       *macro_parms;

    macro_parms = NULL;
    if( MacroWithParenthesis( mentry ) ) {     /* if () expected */
        ppscan_mode = InitPPScan(front);     // enable T_PPNUMBER tokens
        parm_count_reqd = GetMacroParmCount( mentry );
        if( parm_count_reqd > 0 ) {
            macro_parms = (MACRO_ARG *)CMemAlloc( front, parm_count_reqd * sizeof( MACRO_ARG ) );
        }
        parm_idx = 0;
        do {
            token = NextMToken(front);
        } while( token == T_WHITE_SPACE );
        /*
         * token will now be a '('
         */
        bracket = 0;
        len = 0;
        for( ;; ) {
            token_prev = token;
            do {
                token = NextMToken(front);
            } while( token == T_WHITE_SPACE && len == 0 );
            if( token == T_EOF || token == T_NULL ) {
                CErr2p( front, ERR_INCOMPLETE_MACRO, mentry->macro_name );
                break;
            }
            if( token == T_BAD_TOKEN && front.cvars.BadTokenInfo == ERR_MISSING_QUOTE ) {
                CErr1( front, ERR_MISSING_QUOTE );
                token = T_RIGHT_PAREN;
            }
            if( token == T_LEFT_PAREN ) {
                ++bracket;
            } else if( token == T_RIGHT_PAREN ) {
                if( bracket == 0 )
                    break;
                --bracket;
            } else if( token == T_STRING ) {
                if( front.cvars.CompFlags.wide_char_string ) {
                    token = T_LSTRING;
                }
            } else if( token == T_COMMA && bracket == 0
              && ( !MacroHasVarArgs( mentry ) || parm_idx != parm_count_reqd - 1 ) ) {
                if( token_prev == T_WHITE_SPACE ) {
                    len = TokenBufRemoveWSToken( len );
                }
                if( macro_parms != NULL ) {     // if expecting parms
                    SaveParm( front, mentry, len, parm_idx, macro_parms );
                }
                ++parm_idx;
                len = 0;
                continue;
            }
            len = WriteTokenBufToken( front, len, token );
            switch( token ) {
            case T_WHITE_SPACE:
                if( token_prev == T_WHITE_SPACE ) {
                    len = TokenBufRemoveWSToken( len );
                }
                break;
            case T_BAD_CHAR:
                len = WriteTokenBufChar( front, len, front.cvars.Buffer[0] );
                if( front.cvars.Buffer[1] != '\0' ) {
                    len = WriteTokenBufToken( front, len, T_WHITE_SPACE );
                }
                break;
            case T_CONSTANT:
            case T_PPNUMBER:
            case T_ID:
            case T_UNEXPANDABLE_ID:
            case T_LSTRING:
            case T_STRING:
            case T_BAD_TOKEN:
                len = WriteTokenBufMem( front, len, front.cvars.Buffer, front.cvars.TokenLen + 1 );
                break;
            default:
                break;
            }
        }
        if( token_prev == T_WHITE_SPACE ) {
            len = TokenBufRemoveWSToken( len );
        }
        if( macro_parms != NULL ) {     // if expecting parms
            SaveParm( front, mentry, len, parm_idx, macro_parms );
            ++parm_idx;
        } else if( len != 0 ) {
            ++parm_idx;                   // will cause "too many parms" error
        }
        if( MacroHasVarArgs( mentry ) && parm_idx < ( parm_count_reqd - 1 )
          || !MacroHasVarArgs( mentry ) && parm_idx < parm_count_reqd ) {
            CErr2p( front, ERR_TOO_FEW_MACRO_PARMS, mentry->macro_name );
        } else if( !MacroHasVarArgs( mentry ) && parm_idx > parm_count_reqd ) {
            if( parm_count_reqd > 0 ) {
                CWarn2p( front, ERR_TOO_MANY_MACRO_PARMS, mentry->macro_name  );
            }
        } else if( strcmp( mentry->macro_name, "va_start" ) == 0 ) {
            if( front.cvars.SymLevel != 0 && !VarParm( front.cvars.CurFunc ) ) {
                CErr1( front, ERR_MUST_BE_VAR_PARM_FUNC );
            }
        }
        FiniPPScan( front, ppscan_mode );      // disable T_PPNUMBER tokens
    }
    return( macro_parms );
}




static MACRO_TOKEN *BuildAToken( FrontRun& front, TOKEN token, const char *data )
/**************************************************************/
{
    MACRO_TOKEN *mtok;

    mtok = (MACRO_TOKEN *)CMemAlloc( front, sizeof( MACRO_TOKEN ) + strlen( data ) );
    strcpy( mtok->data, data );
    mtok->token = token;
    mtok->next = NULL;
    return( mtok );
}


static MACRO_TOKEN **BuildATokenOnEnd( FrontRun& front, MACRO_TOKEN **pmtok_tail, TOKEN token, const char *data )
/**********************************************************************************************/
{
    *pmtok_tail = BuildAToken( front, token, data );
    return( &(*pmtok_tail)->next );
}


static MACRO_TOKEN *AppendToken( FrontRun& front, MACRO_TOKEN *mtok_head, TOKEN token, const char *data )
/**************************************************************************************/
{
    MACRO_TOKEN *mtok_tail;
    MACRO_TOKEN *mtok;

    mtok = BuildAToken( front, token, data );
    if( mtok_head == NULL ) {
        mtok_head = mtok;
    } else {
        mtok_tail = mtok_head;
        while( mtok_tail->next != NULL ) {
            mtok_tail = mtok_tail->next;
        }
        mtok_tail->next = mtok;
    }
    return( mtok_head );
}

static bool MacroBeingExpanded( FrontRun& front, MEPTR mentry )
/********************************************/
{
    NESTED_MACRO    *nested;

    for( nested = front.cmac1.NestedMacros; nested != NULL; nested = nested->next ) {
        if( nested->mentry == mentry )
            return( true );
        if( !nested->rescanning ) {
            break;
        }
    }
    return( false );
}

static exp_state isExpandable( FrontRun& front, MEPTR mentry, MACRO_TOKEN *mtok, bool macro_parm )
/*******************************************************************************/
{
    int         lparen;

    if( MacroIsSpecial( mentry ) ) {            /* if special macro */
        return( EXPANDABLE_YES );
    }
    if( !MacroWithParenthesis( mentry ) ) {     /* if () not expected */
        if( macro_parm ) {
            if( MacroBeingExpanded( front, mentry ) ) {
                return( EXPANDABLE_NO );
            }
        }
        return( EXPANDABLE_YES );
    }
    for( ; mtok != NULL; mtok = mtok->next ) {
        if( mtok->token != T_WHITE_SPACE && mtok->token != T_NULL ) {
            break;
        }
    }
    if( mtok != NULL ) {
        if( mtok->token == T_LEFT_PAREN ) {
            if( front.cvars.MacroDepth == 1 && !macro_parm )
                return( EXPANDABLE_YES );
            lparen = 0;
            for( ; (mtok = mtok->next) != NULL; ) {
                if( mtok->token == T_LEFT_PAREN ) {
                    ++lparen;
                } else if( mtok->token == T_RIGHT_PAREN ) {
                    if( lparen == 0 )
                        return( EXPANDABLE_YES );
                    --lparen;
                }
            }
        }
    } else if( !macro_parm ) {
        SkipAhead(front);
        if( front.cvars.CurrChar == '(' ) {
            return( EXPANDABLE_YES );
        } else if( front.cvars.CompFlags.cpp_mode ) {
            return( EXPANDABLE_WSSKIP );
        }
    }
    return( EXPANDABLE_NO );
}

static char *GlueTokenToBuffer( FrontRun& front, MACRO_TOKEN *mtok, char *gluebuf )
/****************************************************************/
{
    size_t      gluelen;
    size_t      tokenlen;
    char        *buf;

    buf = NULL;
    if( mtok != NULL ) {
        buf = expandMacroToken( front, mtok );
    }
    if( buf == NULL ) {
        buf = gluebuf;
    } else if( gluebuf != NULL ) {
        /*
         * now do a "strcat( gluebuf, buf )"
         */
        gluelen = strlen( gluebuf );
        tokenlen = strlen( buf );
        gluebuf = static_cast<char *>(CMemRealloc( front, gluebuf, gluelen + tokenlen + 1 ));
        strcpy( gluebuf + gluelen, buf );
        CMemFree( front, buf );
        buf = gluebuf;
    }
    return( buf );
}

static MACRO_TOKEN *ReTokenBuffer( FrontRun& front, const char *buffer )
/******************************************************
 * retokenize starting at buffer
 */
{
    MACRO_TOKEN *mtok_head;
    MACRO_TOKEN **pmtok_tail;
    bool        ppscan_mode;

    ppscan_mode = InitPPScan(front);
    ReScanInit( front, buffer );
    mtok_head = NULL;
    pmtok_tail = &mtok_head;
    for( front.cvars.CompFlags.rescan_buffer_done = false; !front.cvars.CompFlags.rescan_buffer_done; ) {
        pmtok_tail = BuildATokenOnEnd( front, pmtok_tail, ReScanToken(front), front.cvars.Buffer );
    }
    FiniPPScan( front, ppscan_mode );
    return( mtok_head );
}


static MACRO_TOKEN *GlueTokens( FrontRun& front, MACRO_TOKEN *mtok_head )
/******************************************************/
{
    MACRO_TOKEN *mtok;
    MACRO_TOKEN **pmtok_tail;
    MACRO_TOKEN **pmtok_tail_prev;   // prior pmtok_tail
    MACRO_TOKEN *mtok_next;
    char        *buf;
    char        *gluebuf;

    gluebuf = NULL;
    pmtok_tail_prev = NULL;
    pmtok_tail = &mtok_head;
    mtok = *pmtok_tail;
    buf = front.cvars.Buffer;
    for( ; mtok != NULL; ) {
        if( mtok->token == T_WHITE_SPACE ) {
            pmtok_tail_prev = pmtok_tail;
            pmtok_tail = &mtok->next;
            mtok = *pmtok_tail;
        } else if( mtok->next == NULL ) {
            break;
        } else {
            mtok_next = mtok->next;
            if( mtok_next->token == T_WHITE_SPACE )
                mtok_next = mtok_next->next;
            if( mtok_next == NULL )
                break;
            if( mtok_next->token != T_MACRO_SHARP_SHARP ) {
                pmtok_tail_prev = pmtok_tail;
                pmtok_tail = &mtok->next;
                mtok = *pmtok_tail;
            } else {    // T_MACRO_SHARP_SHARP
                int         pos;
                MACRO_TOKEN *mtok_save;

                mtok_next = mtok_next->next;
                pos = 0;
                /*
                 * glue mtok->token with mtok_next->token to make one token
                 */
                if( ( mtok->token == T_COMMA && mtok_next->token == T_MACRO_EMPTY_VAR_PARM )
                  || ( mtok->token == T_MACRO_EMPTY_VAR_PARM && mtok_next->token == T_COMMA ) ) {
                    /*
                     * delete [mtoken(a comma),##,empty __VA_ARGS__]
                     * delete [empty __VA_ARGS__,##,mtoken(a comma)]
                     */
                    buf[10] = '\0';
                    pos = -1;
                } else {
                    if( mtok->token == T_MACRO_EMPTY_VAR_PARM ) {
                        /*
                         * well should never be in this state if no mtok_next - since ## cannot
                         * appear at the end of a macro
                         */
                        gluebuf = GlueTokenToBuffer( front, mtok_next, NULL );
                    } else {
                        gluebuf = GlueTokenToBuffer( front, mtok, NULL );
                        if( mtok_next != NULL && mtok_next->token != T_MACRO_EMPTY_VAR_PARM ) {
                            gluebuf = GlueTokenToBuffer( front, mtok_next, gluebuf ); //paste in mtok_next
                        }
                    }
                }
                if( mtok_next != NULL ) {
                    mtok_save = mtok_next->next;   // save unseen stuff
                    mtok_next->next = NULL;  // break link;
                } else {
                    mtok_save = NULL;         // happens if no arg after comma
                }
                while( mtok != NULL ) { // free old stuff [mtoken,##,{atok,} mtok_next]
                    mtok_next = mtok->next;
                    CMemFree( front, mtok );
                    mtok = mtok_next;
                }
                if( pos >= 0 ) {
                    if( gluebuf != NULL ) {
                        mtok = ReTokenBuffer( front, gluebuf );
                        CMemFree( front, gluebuf );
                    } else {
                        /*
                         * Both ends of ## were empty
                         */
                        mtok = BuildAToken( front, T_NULL, "P-<placemarker>" );
                    }
                    *pmtok_tail = mtok;  // link in new mtok to mtok's link
                    while( mtok != NULL && mtok->next != NULL ) {   //position mtok & pmtok_tail to last token
                        pmtok_tail_prev = pmtok_tail;
                        pmtok_tail = &mtok->next;
                        mtok = *pmtok_tail;
                        if( mtok == mtok->next ) {
                            return( mtok_head );
                        }
                    }
                    /*
                     * mtok == last token of retokenizing
                     * pmtok_tail == the pointer which references mtok
                     */
                    mtok->next = mtok_save;
                } else {
                    if( pmtok_tail_prev != NULL ) {
                        *pmtok_tail = mtok_save;
                        pmtok_tail = pmtok_tail_prev;
                        mtok = *pmtok_tail;
                    } else {
                        *pmtok_tail = mtok_save;
                        mtok = mtok_head;
                    }
                }
            }
        }
    }
    return( mtok_head );
}

static MACRO_TOKEN *BuildString( FrontRun& front, const char *p )
/**********************************************/
{
    MACRO_TOKEN     *mtok_head;
    MACRO_TOKEN     **pmtok_tail;
    char            c;
    TOKEN           token;

    mtok_head = NULL;
    if( p != NULL ) {
        pmtok_tail = &mtok_head;
        front.cvars.TokenLen = 0;
        while( GetMtoken( p ) == T_WHITE_SPACE ) {
            p += SIZE_MTOKEN;   //eat leading white space
        }
        while( (token = static_cast<TOKEN>(GetMtoken( p ))) != T_NULL ) {
            p += SIZE_MTOKEN;
            switch( token ) {
            case T_CONSTANT:
            case T_PPNUMBER:
            case T_ID:
            case T_UNEXPANDABLE_ID:
            case T_BAD_TOKEN:
                for( ; (c = *p++) != '\0'; ) {
                    if( c == '\\' )
                        WriteBufferChar( front, c );
                    WriteBufferChar( front, c );
                }
                break;
            case T_LSTRING:
                WriteBufferChar( front, 'L' );
                /* fall through */
            case T_STRING:
                WriteBufferChar( front, '\\' );
                WriteBufferChar( front, '"' );
                for( ; (c = *p++) != '\0'; ) {
                    if( c == '\\' || c == '"' )
                        WriteBufferChar( front, '\\' );
                    WriteBufferChar( front, c );
                }
                WriteBufferChar( front, '\\' );
                WriteBufferChar( front, '"' );
                break;
            case T_WHITE_SPACE:
                while( (token = static_cast<TOKEN>(GetMtoken( p ))) == T_WHITE_SPACE ) {
                    p += SIZE_MTOKEN;
                }
                if( token != T_NULL ) {
                    WriteBufferChar( front, ' ' );
                }
                break;
            case T_BAD_CHAR:
                if( *p == '\\' && GetMtoken( p + 1 ) == T_NULL ) {
                    CErr1( front, ERR_INVALID_STRING_LITERAL );
                }
                WriteBufferChar( front, *p++ );
                break;
            default:
                WriteBufferString( front, TokenString[token] );
                break;
            }
        }
        if( front.cvars.TokenLen > 0 ) {
            WriteBufferNullChar(front);
            pmtok_tail = BuildATokenOnEnd( front, pmtok_tail, T_STRING, front.cvars.Buffer );
        }
    }
    return( mtok_head );
}


static MACRO_TOKEN *BuildMTokenList( FrontRun& front, const char *p, MACRO_ARG *macro_parms )
/**************************************************************************/
{
    MACRO_TOKEN     *mtok;
    MACRO_TOKEN     *mtok_head;
    MACRO_TOKEN     **pmtok_tail;
    NESTED_MACRO    *nested;
    const char      *p2;
    char            buf[2];
    TOKEN           token_prev;
    TOKEN           token;
    int             parm_idx;

    mtok_head = NULL;
    pmtok_tail = &mtok_head;
    nested = front.cmac1.NestedMacros;
    buf[1] = '\0';
    token_prev = T_NULL;
    if( p == NULL )
        return( NULL );
    while( (token = static_cast<TOKEN>(GetMtoken( p ))) != T_NULL ) {
        p += SIZE_MTOKEN;
        switch( token ) {
        case T_CONSTANT:
        case T_PPNUMBER:
        case T_ID:
        case T_UNEXPANDABLE_ID:
        case T_BAD_TOKEN:
        case T_LSTRING:
        case T_STRING:
            mtok = BuildAToken( front, token, p );
            while( *p++ != '\0' )
                ;
            break;
        case T_WHITE_SPACE:
            if( token_prev == T_MACRO_SHARP_SHARP )
                continue;
            mtok = BuildAToken( front, T_WHITE_SPACE, " " );
            break;
        case T_BAD_CHAR:
            buf[0] = *p++;
            mtok = BuildAToken( front, T_BAD_CHAR, buf );
            break;
        case T_MACRO_SHARP:
            while( GetMtoken( p ) == T_WHITE_SPACE ) {
                p += SIZE_MTOKEN;
            }
            p += SIZE_MTOKEN;   // skip over T_MACRO_PARM
            /*
             * If no macro arg given, result must be "", not empty
             */
            parm_idx = GetMparm( p );
            p += SIZE_MPARM;
            if( macro_parms != NULL && macro_parms[parm_idx].arg != NULL && macro_parms[parm_idx].arg[0] != '\0' ) {
                mtok = BuildString( front, macro_parms[parm_idx].arg );
            } else {
                mtok = BuildAToken( front, T_STRING, "" );
            }
            break;
        case T_MACRO_PARM:
            parm_idx = GetMparm( p );
            p += SIZE_MPARM;
            p2 = p;
            while( GetMtoken( p2 ) == T_WHITE_SPACE ) {
                p2 += SIZE_MTOKEN;
            }
            nested->substituting_parms = true;
            if( macro_parms != NULL ) {
                mtok = BuildMTokenList( front, macro_parms[parm_idx].arg, NULL );
                /*
                 * NB: mtok is now NULL if macro arg was empty
                 */
            } else {
                mtok = BuildAToken( front, T_WHITE_SPACE, "" );
            }
            if( GetMtoken( p2 ) != T_MACRO_SHARP_SHARP && token_prev != T_MACRO_SHARP_SHARP ) {
                if( mtok != NULL ) {
                    mtok = AppendToken( front, mtok, T_NULL, "P-<end of parm>" );
                    mtok = ExpandNestedMacros( front, mtok, false );
                }
            } else if( mtok == NULL ) {
                mtok = BuildAToken( front, T_NULL, "P-<placemarker>" );
            }
            nested->substituting_parms = false;
            break;
        case T_MACRO_VAR_PARM:
            parm_idx = GetMparm( p );
            p += SIZE_MPARM;
            p2 = p;
            while( GetMtoken( p2 ) == T_WHITE_SPACE )
                p2 += SIZE_MTOKEN;
            nested->substituting_parms = true;
            if( macro_parms != NULL ) {
                if( macro_parms[parm_idx].arg != NULL ) {
                    mtok = BuildMTokenList( front, macro_parms[parm_idx].arg, NULL );
                } else {
                    if( token_prev == T_MACRO_SHARP_SHARP || GetMtoken( p2 ) == T_MACRO_SHARP_SHARP ) {
                        mtok = BuildAToken( front, T_MACRO_EMPTY_VAR_PARM, "" );
                    } else {
                        mtok = BuildAToken( front, T_WHITE_SPACE, "" );
                    }
                }
            } else {
                mtok = BuildAToken( front, T_WHITE_SPACE, "" );
            }
            if( GetMtoken( p2 ) != T_MACRO_SHARP_SHARP && token_prev != T_MACRO_SHARP_SHARP ) {
                if( mtok != NULL ) {
                    mtok = AppendToken( front, mtok, T_NULL, "P-<end of parm>" );
                    mtok = ExpandNestedMacros( front, mtok, false );
                }
            }
            nested->substituting_parms = false;
            break;
        default:
            mtok = BuildAToken( front, token, TokenString[token] );
            break;
        }
        if( mtok != NULL ) {
            if( mtok->token != T_WHITE_SPACE ) {
                token_prev = mtok->token;
            }
            *pmtok_tail = mtok;
            while( *pmtok_tail != NULL ) {
                pmtok_tail = &(*pmtok_tail)->next;
            }
        }
    }
    mtok_head = GlueTokens( front, mtok_head );
    return( mtok_head );
}

static void markUnexpandableIds( FrontRun& front, MACRO_TOKEN *mtok_head )
/*******************************************************/
{
    NESTED_MACRO    *nested;
    MACRO_TOKEN     *mtok;

    for( mtok = mtok_head; mtok != NULL; mtok = mtok->next ) {
        if( mtok->token == T_ID ) {
            for( nested = front.cmac1.NestedMacros; nested != NULL; nested = nested->next ) {
                if( strcmp( nested->mentry->macro_name, mtok->data ) == 0 ) {
                    if( !nested->substituting_parms ) {
                        /*
                         * change token so it won't be considered a
                         * candidate as a macro
                         */
                        mtok->token = T_UNEXPANDABLE_ID;
                        break;
                    }
                }
            }
        }
    }
}

static MACRO_TOKEN *MacroExpansion( FrontRun& front, bool rescanning, MEPTR mentry )
/*****************************************************************/
{
    MACRO_ARG       *macro_parms;
    MACRO_TOKEN     *mtok_head;
    NESTED_MACRO    *nested;

    nested = (NESTED_MACRO *)CMemAlloc( front, sizeof( NESTED_MACRO ) );
    nested->mentry = mentry;
    nested->rescanning = rescanning;
    nested->substituting_parms = false;
    nested->macro_parms = NULL;
    if( MacroIsSpecial( mentry ) ) {    /* if special macro */
        mtok_head = BuildAToken( front, SpecialMacro( front, mentry ), front.cvars.Buffer );
        nested->next = front.cmac1.NestedMacros;
        front.cmac1.NestedMacros = nested;
    } else {
        macro_parms = CollectParms( front, mentry );
        nested->next = front.cmac1.NestedMacros;
        front.cmac1.NestedMacros = nested;
        nested->macro_parms = macro_parms;
        mtok_head = BuildMTokenList( front, (char *)mentry + mentry->macro_defn, macro_parms );
        markUnexpandableIds( front, mtok_head );
    }
    mtok_head = AppendToken( front, mtok_head, T_NULL, MACRO_END_STRING );
    return( mtok_head );
}

static MACRO_TOKEN *ExpandNestedMacros( FrontRun& front, MACRO_TOKEN *mtok_head, bool rescanning )
/*******************************************************************************/
{
    MACRO_TOKEN *mtok1;
    MACRO_TOKEN *mtok2;
    MACRO_TOKEN *mtok_prev;
    MACRO_TOKEN *mtok_save;
    exp_state   i;
    size_t      len;
    char        *buf;
    MEPTR       mentry;

    mtok1 = mtok_head;
    ++front.cvars.MacroDepth;
    mtok_prev = NULL;
    for( ; mtok1 != NULL; ) {
        mtok2 = NULL;
        if( mtok1->token == T_ID ) {
            /*
             * if macro and not being expanded, then expand it
             * only tokens available for expansion are those in mtok list
             */
            buf = front.cvars.Buffer;
            len = 0;
            while( (buf[len] = mtok1->data[len]) != '\0' )
                len++;
            mentry = MacroLookup( front, buf );
            if( mentry != NULL ) {
                /*
                 * this is a macro
                 */
                if( rescanning ) {
                    if( MacroBeingExpanded( front, mentry ) ) {
                        mtok1->token = T_UNEXPANDABLE_ID;
                    } else {
                        mtok2 = mtok1;
                        while( mtok2->next != NULL ) {
                            mtok2 = mtok2->next;
                        }
                        mtok2->next = front.cmac1.MTokenList;
                        i = isExpandable( front, mentry, mtok1->next, false );
                        switch( i ) {
                        case EXPANDABLE_NO:     // macro is currently not expandable
                            mtok1->token = T_MACRO;
                            mtok2->next = NULL;
                            mtok2 = NULL;
                            break;
                        case EXPANDABLE_YES:    // macro is expandable
                            front.cmac1.MTokenList = mtok1->next;
                            if( mtok_head == mtok1 ) {
                                mtok_head = NULL;
                                mtok_prev = NULL;
                            }
                            CMemFree( front, mtok1 );
                            mtok2 = MacroExpansion( front, rescanning, mentry );
                            mtok1 = front.cmac1.MTokenList;
                            front.cmac1.MTokenList = NULL;
                            break;
                        case EXPANDABLE_WSSKIP: // we skipped over some white space
                            mtok1->token = T_UNEXPANDABLE_ID;
                            mtok2->next = NULL;
                            mtok2 = BuildAToken( front, T_WHITE_SPACE, " " );
                            mtok2->next = mtok1->next;
                            mtok1->next = mtok2;
                            mtok2 = NULL;
                            break;
                        }
                    }
                } else {                        // expanding a macro parm
                    i = isExpandable( front, mentry, mtok1->next, true );
                    switch( i ) {
                    case EXPANDABLE_NO:         // macro is currently not expandable
                        mtok_prev = mtok1;
                        mtok1 = mtok1->next;    // advance onto next token
                        break;
                    case EXPANDABLE_YES:        // macro is expandable
                    case EXPANDABLE_WSSKIP:     // we skipped over some white space
                        mtok_save = front.cmac1.MTokenList;
                        front.cmac1.MTokenList = mtok1->next;
                        if( mtok_head == mtok1 ) {
                            mtok_head = NULL;
                            mtok_prev = NULL;
                        }
                        CMemFree( front, mtok1 );
                        mtok2 = ExpandNestedMacros( front, MacroExpansion( front, false, mentry ), rescanning );
                        mtok1 = front.cmac1.MTokenList;
                        front.cmac1.MTokenList = mtok_save;
                        break;
                    }
                }
            } else {
                mtok1->token = T_SAVED_ID;      // avoid rechecking this ID
                mtok_prev = mtok1;
                mtok1 = mtok1->next;            // advance onto next token
            }
        } else if( mtok1->token == T_NULL ) {
            mtok2 = mtok1->next;
            if( mtok1->data[0] == MACRO_END_CHAR ) { // end of a macro
                rescanning = front.cmac1.NestedMacros->rescanning;
                DeleteNestedMacro(front);
                CMemFree( front, mtok1 );
                mtok1 = mtok2;
            } else {                            // end of a macro parm
                if( mtok2 != NULL ) {
                    front.cmac1.MTokenList = mtok2;
                }
                CMemFree( front, mtok1 );
                mtok1 = NULL;                   // will cause us to exit
            }
            mtok2 = NULL;
        } else {                                // advance onto next token
            mtok_prev = mtok1;
            mtok1 = mtok1->next;
        }
        if( mtok2 != NULL ) {                   // new tokens to insert
            if( mtok_prev == NULL ) {
                mtok_head = mtok2;
            } else {
                mtok_prev->next = mtok2;
            }
            if( mtok1 != NULL ) {
                while( mtok2->next != NULL )
                    mtok2 = mtok2->next;
                mtok2->next = mtok1;
            }
            if( mtok_prev == NULL ) {
                mtok1 = mtok_head;
            } else {
                mtok1 = mtok_prev->next;
            }
        } else {
            /*
             * either no change, or tokens were deleted
             */
            if( mtok_prev == NULL ) {
                mtok_head = mtok1;
            } else {
                mtok_prev->next = mtok1;
            }
        }
    }
    for( mtok1 = mtok_head; mtok1 != NULL; mtok1 = mtok1->next ) {
        /*
         * change a temporarily unexpandable ID into an ID because it
         * could become expandable in a later rescanning phase
         */
        if( mtok1->token == T_MACRO ) {
            mtok1->token = T_ID;
        }
    }
    --front.cvars.MacroDepth;
    return( mtok_head );
}

void DoMacroExpansion( FrontRun& front, MEPTR mentry )           // called from cscan
/***********************************/
{
    front.cvars.MacroDepth = 0;
    front.cmac1.MTokenList = ExpandNestedMacros( front, MacroExpansion( front, false, mentry ), true );
    /*
     * GetMacroToken will feed back tokens from the MTokenList
     * when the MTokenList is exhausted, then revert back to normal scanning
     */
    if( front.cmac1.MTokenList == NULL ) {
        front.cvars.MacroPtr = NULL;
    } else {
        front.cvars.MacroPtr = const_cast<char *>(MACRO_T_NULL);
    }
}


void InsertReScanPragmaTokens( FrontRun& front, const char *pragma )
/*************************************************/
{
    MACRO_TOKEN *mtok;

    mtok = ReTokenBuffer( front, pragma );
    if( mtok != NULL ) {
        MACRO_TOKEN *mtok_save;

        mtok_save = front.cmac1.MTokenList;
        front.cmac1.MTokenList = mtok;
        while( mtok->next != NULL ) {
            mtok = mtok->next;
        }
        mtok->next = BuildAToken( front, static_cast<TOKEN>(T_PRAGMA_END), "" );
        mtok = mtok->next;
        mtok->next = mtok_save;
        front.cvars.MacroPtr = const_cast<char *>(MACRO_T_NULL);
    }
}

void InsertToken( FrontRun& front, TOKEN token, const char *str )
/**********************************************/
{
    MACRO_TOKEN *mtok;

    mtok = BuildAToken( front, token, str );
    if( mtok != NULL ) {
        mtok->next = front.cmac1.MTokenList;
        front.cmac1.MTokenList = mtok;
        front.cvars.MacroPtr = const_cast<char *>(MACRO_T_NULL);
    }
}
