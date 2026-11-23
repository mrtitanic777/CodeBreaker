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
* Description:  Macro processing, part 2.
*
****************************************************************************/


#include "cvars.h"
#include <stddef.h>
#include "i64.h"
#include "scan.h"
#include "cgmisc.h"
#include "cmacadd.h"
#include "cscanbuf.h"
#include "ppexpn.h"
#include "frontrun.hpp"


inline bool HasVarArgs( macro_flags m ) { return (((m) & MFLAG_HAS_VAR_ARGS) != 0); }


static void    CSkip( FrontRun& front );
static void    CSkipIf( FrontRun& front );
static void    CDefine( FrontRun& front );
static void    CInclude( FrontRun& front );
static void    CIfDef( FrontRun& front );
static void    CIfNDef( FrontRun& front );
static void    CIf( FrontRun& front );
static void    CElif( FrontRun& front );
static void    CElse( FrontRun& front );
static void    CEndif( FrontRun& front );
static void    CUnDef( FrontRun& front );
static void    CLine( FrontRun& front );
static void    CError( FrontRun& front );
static void    CIdent( FrontRun& front );
static void    CWarning( FrontRun& front );



static unsigned char const PreProcWeights[] = {
/* a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z */
   2, 0, 0, 9, 1, 4, 3, 0, 5, 0, 0, 7, 0, 0, 0,10, 0,11, 0,13,12, 0,14, 0, 0, 0
};

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct Cmac2State: cmac2_state.h) */

}  // namespace

/* (the directives of the preprocessor: the run starts with these) */
void cmac2_start( FrontRun& front )
{
    const struct preproc directives[16] = {
        { const_cast<char *>("define"),     CDefine,    CSkip },
        { const_cast<char *>("error"),      CError,     CSkip },
        { const_cast<char *>("pragma"),     CPragma,    CSkip },
        { const_cast<char *>(""),           NULL,       NULL },
        { const_cast<char *>(""),           NULL,       NULL },
        { const_cast<char *>("undef"),      CUnDef,     CSkip },
        { const_cast<char *>("else"),       CElse,      CElse },
        { const_cast<char *>("ident"),      CIdent,     CSkip },
        { const_cast<char *>("warning"),    CWarning,   CSkip },
        { const_cast<char *>("elif"),       CElif,      CElif },
        { const_cast<char *>("endif"),      CEndif,     CEndif },
        { const_cast<char *>("if"),         CIf,        CSkipIf },
        { const_cast<char *>("line"),       CLine,      CSkip },
        { const_cast<char *>("include"),    CInclude,   CSkip },
        { const_cast<char *>("ifdef"),      CIfDef,     CSkipIf },
        { const_cast<char *>("ifndef"),     CIfNDef,    CSkipIf },
    };

    for( int i = 0; i < 16; ++i ) {
        front.cmac2.PreProcTable[i] = directives[i];
    }
}

namespace {

}  // namespace



enum    pp_types {
    PRE_IF,
    PRE_ELIF,
    PRE_ELSE
};

struct  cpp_info {              /* C Pre-processor information */
    struct cpp_info *prev_cpp;
    source_loc      src_loc;
    FNAMEPTR        flist;
    enum pp_types   cpp_type;
    bool            processing;
};


static void PPFlush2EOL( FrontRun& front )
{
    while( front.cvars.CurToken != T_NULL && front.cvars.CurToken != T_EOF ) {
        PPNextToken(front);
        if( front.cvars.CurToken == T_BAD_TOKEN && front.cvars.BadTokenInfo == ERR_MISSING_QUOTE ) {
            CErr1( front, front.cvars.BadTokenInfo );
        }
    }
}


static void CheckEOL( FrontRun& front )
{
    if( front.cvars.CurToken != T_EOF ) {
        ExpectingToken( front, T_NULL );
    }
}


static void WantEOL( FrontRun& front )
{
    if( front.cvars.CompFlags.extensions_enabled ) {
        if( front.cvars.CurToken != T_NULL && front.cvars.CurToken != T_EOF ) {
            if( front.cvars.SkipLevel == front.cvars.NestLevel ) {
                CWarn1( front, ERR_JUNK_FOLLOWS_DIRECTIVE );
            }
        }
    } else {
        CheckEOL(front);
    }
}


static void IncLevel( FrontRun& front, bool value )
{
    struct cpp_info *cpp;

    cpp = (struct cpp_info *)CMemAlloc( front, sizeof( struct cpp_info ) );
    cpp->prev_cpp = front.cvars.CppStack;
    cpp->src_loc = front.cvars.TokenLoc;
    cpp->flist = front.cvars.SrcFiles->src_flist;
    cpp->cpp_type = PRE_IF;
    cpp->processing = false;
    front.cvars.CppStack = cpp;
    if( front.cvars.SkipLevel == front.cvars.NestLevel ) {
        if( value ) {
            ++front.cvars.SkipLevel;
            cpp->processing = true;
        }
    }
    ++front.cvars.NestLevel;
}


static void CUnknown( FrontRun& front )
{
    if( front.cvars.SkipLevel == front.cvars.NestLevel ) {
        CErr2p( front, ERR_UNKNOWN_DIRECTIVE, front.cvars.Buffer );
    }
}


static void PreProcStmt( FrontRun& front )
{
    struct preproc      *pp;
    unsigned            hash;
    int                 c1;
    int                 c2;

    front.cvars.NextChar(front);                 /* skip over '#' */
    PPNextToken(front);
    if( front.cvars.CurToken == T_ID ) {
        c1 = IndexLower( ((unsigned char *)front.cvars.Buffer)[0] );
        c2 = IndexLower( ((unsigned char *)front.cvars.Buffer)[front.cvars.TokenLen - 1] );
        if( c1 >= 0 && c2 >= 0 ) {
            hash = (front.cvars.TokenLen + PreProcWeights[c1] + PreProcWeights[c2]) & 0x0f;
            pp = &front.cmac2.PreProcTable[hash];
            if( strcmp( pp->directive, front.cvars.Buffer ) == 0 ) {
                if( front.cvars.SkipLevel == front.cvars.NestLevel ) {
                    pp->samelevel(front);
                } else {
                    pp->skipfunc(front);
                }
            } else {
                CUnknown(front);
            }
        } else {
            CUnknown(front);
        }
    } else if( front.cvars.CurToken != T_NULL ) {
        CUnknown(front);
    }
}

TOKEN CheckControl( FrontRun& front )
{
    bool        lines_skipped;
    ppctl_t     old_ppctl;
    TOKEN       token;

    if( !front.cvars.CompFlags.doing_macro_expansion ) {
        if( front.cvars.CompFlags.cpp_mode ) {
            front.cvars.PrintWhiteSpace = false;
        }
    }
    while( front.cvars.CurrChar == '\n' ) {
        if( TBreak(*front.back) ) {
            CErr1( front, ERR_BREAK_KEY_HIT );
            CSuicide(front);
        }
        lines_skipped = false;
        old_ppctl = front.cvars.PPControl;
        for( ; front.cvars.CurrChar != LCHR_EOF; ) {
            if( front.cvars.CompFlags.cpp_mode ) {
                CppPrtChar( front, '\n' );
            }
            front.cvars.NextChar(front);
            if( front.cvars.CurrChar != front.cvars.PreProcChar ) {
                SkipAhead(front);
            }
            if( front.cvars.CurrChar == LCHR_EOF )
                break;
            if( front.cvars.CurrChar == front.cvars.PreProcChar ) { /* start of comp control line */
                PPCtlEnableEol( front );
                PPCtlDisableMacros( front );
                PPCtlDisableLexErrors( front );
                PreProcStmt(front);
                PPFlush2EOL(front);
                front.cvars.PPControl = old_ppctl;
            } else if( front.cvars.SkipLevel != front.cvars.NestLevel ) {
                PPCtlEnableEol( front );
                PPCtlDisableMacros( front );
                PPCtlDisableLexErrors( front );
                PPNextToken(front);              /* get into token mode */
                PPFlush2EOL(front);
                front.cvars.PPControl = old_ppctl;
            }
            if( front.cvars.SkipLevel == front.cvars.NestLevel )
                break;
            if( front.cvars.CurrChar == '\n' ) {
                lines_skipped = true;
            }
        }
        if( front.cvars.CompFlags.cpp_mode ) {
            if( lines_skipped ) {
                if( front.cvars.SrcFiles != NULL ) {
                    CppEmitPoundLine( front, front.cvars.SrcFiles->src_loc.line, front.cvars.SrcFiles->src_name, false );
                }
            }
        }
    }
    /*
     * we have already skipped past all white space at the start of the line
     */
    token = T_WHITE_SPACE;
//    token = ScanToken();
    return( token );
}


static void CSkip( FrontRun& front )
{
}


static void CSkipIf( FrontRun& front )
{
    IncLevel( front, false );
}


static void CIdent( FrontRun& front )
{
    if( !front.cvars.CompFlags.extensions_enabled ) {
        CUnknown(front);
    }
}


void CInclude( FrontRun& front )
{
    bool        in_macro;
    char        buf[82];

    if( front.cvars.PCH_FileName != NULL && !front.cvars.CompFlags.make_precompiled_header ) {
        if( front.cvars.CompFlags.ok_to_use_precompiled_hdr ) {
            front.cvars.CompFlags.use_precompiled_header = true;
        }
    }
    if( front.cvars.CompFlags.use_precompiled_header ) {
        InitBuildPreCompiledHeader(front);
    }
    front.cvars.InitialMacroFlags = MFLAG_NONE;
    in_macro = ( front.cvars.MacroPtr != NULL );
    PPCtlEnableMacros( front );
    PPNextToken(front);
    PPCtlDisableMacros( front );
    if( front.cvars.CurToken == T_STRING ) {
        if( !OpenSrcFile( front, front.cvars.Buffer, FT_HEADER ) ) {
            PrtfFilenameErr( front, front.cvars.Buffer, FT_HEADER, true );
        }
    } else if( front.cvars.CurToken == T_LT ) {
        if( in_macro ) {
            PPCtlEnableMacros( front );
        }
        buf[0] = '\0';
        for( ;; ) {
            PPNextToken(front);
            if( front.cvars.CurToken == T_GT ) {
                if( !OpenSrcFile( front, buf, FT_LIBRARY ) ) {
                    PrtfFilenameErr( front, buf, FT_LIBRARY, true );
                }
                break;
            }
            strncat( buf, front.cvars.Buffer, sizeof( buf ) - 2 );
            if( in_macro && front.cvars.MacroPtr == NULL || front.cvars.CurToken == T_NULL || front.cvars.CurToken == T_EOF ) {
                CErr1( front, ERR_INVALID_INCLUDE );
                break;
            }
        }
        if( in_macro ) {
            PPCtlDisableMacros( front );
        }
    } else {
        CErr1( front, ERR_INVALID_INCLUDE );
    }
    if( front.cvars.CurToken != T_EOF ) {
        PPNextToken(front);
    }
    front.cvars.CompFlags.use_precompiled_header = false;
}


static int FormalParm( FrontRun& front, MPPTR formal_parms )
{
    int     parm_num;

    parm_num = 1;
    for( ; formal_parms != NULL; formal_parms = formal_parms->next ) {
        if( strcmp( formal_parms->parm, front.cvars.Buffer ) == 0 ) {
            return( parm_num );
        }
        ++parm_num;
    }
    return( 0 );
}


static MEPTR GrabTokens( FrontRun& front, int parm_count, macro_flags mflags, MPPTR formal_parms, const char *mac_name, source_loc *loc )
{
    MEPTR           mentry;
    MEPTR           new_mentry;
    TOKEN           prev_token;
    TOKEN           prev_non_ws_token;
    size_t          mlen;
    int             parm_num;

    mentry = CreateMEntry( front, mac_name, 0 );
    mentry->u.parm_count = parm_count;
    mentry->src_loc = *loc;
    mlen = mentry->macro_len;
    mentry->macro_defn = mlen;
    prev_token = T_NULL;
    prev_non_ws_token = T_NULL;
    if( front.cvars.CurToken != T_NULL ) {
        do {
            front.cvars.CurToken = ScanToken(front);
        } while( front.cvars.CurToken == T_WHITE_SPACE );
        if( front.cvars.CurToken == T_SHARP_SHARP ) {
            CErr1( front, ERR_MISPLACED_SHARP_SHARP );
            PPNextToken(front);
        }
    }
    for( ; front.cvars.CurToken != T_NULL && front.cvars.CurToken != T_EOF ; ) {
        switch( front.cvars.CurToken ) {
        case T_SHARP:
            /*
             * if it is a function-like macro definition
             */
            if( parm_count != 0 ) {
                front.cvars.CurToken = T_MACRO_SHARP;
            }
            MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            break;
        case T_SHARP_SHARP:
            front.cvars.CurToken = T_MACRO_SHARP_SHARP;
            MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            break;
        case T_WHITE_SPACE:
            if( prev_token != T_WHITE_SPACE ) {
                MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            }
            break;
        case T_ID:
            parm_num = FormalParm( front, formal_parms );
            if( parm_num != 0 ) {
                if( HasVarArgs( mflags ) && (parm_num == parm_count - 1) ) {
                    front.cvars.CurToken = T_MACRO_VAR_PARM;
                } else {
                    front.cvars.CurToken = T_MACRO_PARM;
                }
            }
            MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            if( parm_num != 0 ) {
                MacroSegmentAddChar( front, &mlen, parm_num - 1 );
            } else {
                MacroSegmentAddMem( front, &mlen, front.cvars.Buffer, front.cvars.TokenLen + 1 );
            }
            break;
        case T_BAD_CHAR:
            MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            MacroSegmentAddChar( front, &mlen, front.cvars.Buffer[0] );
            if( front.cvars.Buffer[1] != '\0' ) {
                front.cvars.CurToken = T_WHITE_SPACE;
                MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            }
            break;
        case T_STRING:
            if( front.cvars.CompFlags.wide_char_string ) {
                front.cvars.CurToken = T_LSTRING;
            }
            /* fall through */
        case T_LSTRING:
        case T_CONSTANT:
        case T_BAD_TOKEN:
        case T_PPNUMBER:
            MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            MacroSegmentAddMem( front, &mlen, front.cvars.Buffer, front.cvars.TokenLen + 1 );
            break;
        default:
            MacroSegmentAddToken( front, &mlen, front.cvars.CurToken );
            break;
        }
        if( front.cvars.CurToken != T_WHITE_SPACE ) {
            if( prev_non_ws_token == T_MACRO_SHARP && front.cvars.CurToken != T_MACRO_PARM && front.cvars.CurToken != T_MACRO_VAR_PARM ) {
                CErr1( front, ERR_MUST_BE_MACRO_PARM );
//                SetMtoken( MacroOffset + mlen - SIZE_MTOKEN, T_SHARP );
            }
            prev_non_ws_token = front.cvars.CurToken;
        }
        prev_token = front.cvars.CurToken;
        front.cvars.CurToken = ScanToken(front);
    }
    if( prev_non_ws_token == T_MACRO_SHARP ) {
        CErr1( front, ERR_MUST_BE_MACRO_PARM );
    }
    if( prev_token == T_WHITE_SPACE ) {
        mlen -= SIZE_MTOKEN;
    }
    MacroSegmentAddToken( front, &mlen, T_NULL );
    if( prev_non_ws_token == T_MACRO_SHARP_SHARP ) {
        CErr1( front, ERR_MISPLACED_SHARP_SHARP );
    }
    new_mentry = MacroDefine( front, mlen, mflags );
    if( new_mentry != NULL ) {
        front.cvars.MacroSize += mlen;
    }
    return( new_mentry );
}


MEPTR MacroScan( FrontRun& front )
{
    MPPTR           mp;
    MPPTR           prev_mp;
    MPPTR           formal_parms;
    int             parm_count;
    macro_flags     mflags;
    bool            ppscan_mode;
    char            *token_buf;
    source_loc      macro_loc;
    MEPTR           mentry;

    PPNextToken(front);
    if( front.cvars.CurToken != T_ID ) {
        ExpectIdentifier(front);
        return( NULL );
    }
    if( IsPPOperatorDefined( front.cvars.Buffer ) ) {
        CErr1( front, ERR_CANT_DEFINE_DEFINED );
        return( NULL );
    }
    token_buf = CMemStrdup( front, front.cvars.Buffer );
    formal_parms = NULL;
    macro_loc = front.cvars.SrcFileLoc;
    parm_count = 0;             /* 0 ==> no () following */
    mflags = MFLAG_NONE;
    if( front.cvars.CurrChar == '(' ) {     /* parms present */
        PPNextToken(front);          /* grab the '(' */
        PPNextToken(front);
        parm_count = 1;         /* 1 ==> () following */
        prev_mp = NULL;
        for( ; front.cvars.CurToken != T_RIGHT_PAREN; ) {
            if( HasVarArgs( mflags ) ) {
                ExpectingAfter( front, T_RIGHT_PAREN, T_DOT_DOT_DOT );
                return( NULL );
            }
            if( front.cvars.CurToken != T_DOT_DOT_DOT && !ExpectingToken( front, T_ID ) ) {
                return( NULL );
            }
            ++parm_count;
            if( front.cvars.CurToken == T_DOT_DOT_DOT ) {
                mflags |= MFLAG_HAS_VAR_ARGS;   /* can have no more parms after this. */
            }
            mp = (MPPTR)CMemAlloc( front, sizeof( MPDEFN ) );
            if( formal_parms == NULL ) {
                formal_parms = mp;
            } else {
                if( FormalParm( front, formal_parms ) ) {
                    CErr2p( front, ERR_DUPLICATE_MACRO_PARM, front.cvars.Buffer );
                }
                prev_mp->next = mp;
            }
            if( front.cvars.CurToken == T_DOT_DOT_DOT ) {
                mp->parm = CMemStrdup( front, "__VA_ARGS__" );
            } else {
                mp->parm = CMemStrdup( front, front.cvars.Buffer );
            }
            prev_mp = mp;
            PPNextToken(front);
            if( front.cvars.CurToken == T_RIGHT_PAREN )
                break;
            if( front.cvars.CurToken == T_NULL ) {
                CErr1( front, ERR_INVALID_MACRO_DEFN );
                break;
            }
            if( HasVarArgs( mflags ) ) {
                ExpectingAfter( front, T_RIGHT_PAREN, T_DOT_DOT_DOT );
                return( NULL );
            }
            MustRecog( front, T_COMMA );
            if( front.cvars.CurToken != T_DOT_DOT_DOT && !ExpectingToken( front, T_ID ) ) {
                return( NULL );
            }
        }
    }
    /*
     * grab replacement tokens
     */
    ppscan_mode = InitPPScan(front);         // enable T_PPNUMBER tokens
    mentry = GrabTokens( front, parm_count, mflags, formal_parms, token_buf, &macro_loc );
    FiniPPScan( front, ppscan_mode );          // disable T_PPNUMBER tokens
    for( ; (mp = formal_parms) != NULL; ) {
        formal_parms = mp->next;
        CMemFree( front, mp->parm );
        CMemFree( front, mp );
    }
    CMemFree( front, token_buf );
    return( mentry );
}

static void CDefine( FrontRun& front )
{
    MEPTR           mentry;

    mentry = MacroScan(front);
    if( mentry != NULL ) {
        mentry->macro_flags |= MFLAG_USER_DEFINED;
    }
}

static void CIfDef( FrontRun& front )
{
    MEPTR       mentry;

    PPNextToken(front);
    if( front.cvars.CurToken != T_ID ) {
        ExpectIdentifier(front);
        IncLevel( front, false );
        return;
    }
    mentry = MacroLookup( front, front.cvars.Buffer );
    if( mentry != NULL ) {
        mentry->macro_flags |= MFLAG_REFERENCED;
        IncLevel( front, true );
    } else {
        IncLevel( front, false );
    }
    PPNextToken(front);
    CheckEOL(front);
}


static void CIfNDef( FrontRun& front )
{
    MEPTR       mentry;

    PPNextToken(front);
    if( !ExpectingToken( front, T_ID ) ) {
        IncLevel( front, false );
        return;
    }
    mentry = MacroLookup( front, front.cvars.Buffer );
    if( mentry != NULL ) {
        mentry->macro_flags |= MFLAG_REFERENCED;
        IncLevel( front, false );
    } else {
        IncLevel( front, true );
    }
    PPNextToken(front);
    CheckEOL(front);
}


static void CIf( FrontRun& front )
{
    bool    value;

    PPCtlEnableLexErrors( front );
    PPCtlEnableMacros( front );
    PPNextToken(front);
    value = PpConstExpr(front);
    IncLevel( front, value );
    CheckEOL(front);
    PPCtlDisableMacros( front );
    PPCtlDisableLexErrors( front );
}


static void CElif( FrontRun& front )
{
    bool    value;

    PPCtlEnableLexErrors( front );
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ( front.cvars.NestLevel == 0 ) || ( front.cvars.CppStack->cpp_type == PRE_ELSE ) ) {
        CErr1( front, ERR_MISPLACED_ELIF );
    } else {
        if( front.cvars.SkipLevel == front.cvars.NestLevel ) {
            --front.cvars.SkipLevel;                /* start skipping else part */
            front.cvars.CppStack->processing = false;
            front.cvars.CppStack->cpp_type = PRE_ELIF;
        } else if( front.cvars.SkipLevel + 1 == front.cvars.NestLevel ) {
            /*
             * only evaluate the expression when required
             */
            if( front.cvars.CppStack->cpp_type == PRE_IF ) {
                value = PpConstExpr(front);
                CheckEOL(front);
                if( value ) {
                    front.cvars.SkipLevel = front.cvars.NestLevel; /* start including else part */
                    front.cvars.CppStack->processing = true;
                    front.cvars.CppStack->cpp_type = PRE_ELIF;
                }
            }
        }
    }
    PPCtlDisableMacros( front );
    PPCtlDisableLexErrors( front );
}


static void CElse( FrontRun& front )
{
    if( ( front.cvars.NestLevel == 0 ) || ( front.cvars.CppStack->cpp_type == PRE_ELSE ) ) {
        CErr1( front, ERR_MISPLACED_ELSE );
    } else {
        if( front.cvars.SkipLevel == front.cvars.NestLevel ) {
            --front.cvars.SkipLevel;                /* start skipping else part */
            front.cvars.CppStack->processing = false;
        } else if( front.cvars.SkipLevel + 1 == front.cvars.NestLevel ) {
            /*
             * cpp_type will be PRE_ELIF if an elif was true
             */
            if( front.cvars.CppStack->cpp_type == PRE_IF ) {
                front.cvars.SkipLevel = front.cvars.NestLevel;  /* start including else part */
                front.cvars.CppStack->processing = true;
            }
        }
        front.cvars.CppStack->cpp_type = PRE_ELSE;
    }
    PPNextToken(front);
    WantEOL(front);
}


static void CEndif( FrontRun& front )
{
    if( front.cvars.NestLevel == 0 ) {
        CErr1( front, ERR_MISPLACED_ENDIF );
    } else {
        struct cpp_info *cpp;

        --front.cvars.NestLevel;
        cpp = front.cvars.CppStack;
        if( front.cvars.SrcFiles != NULL && cpp->flist != front.cvars.SrcFiles->src_flist ) {
             CWarn2p( front, ERR_WEIRD_ENDIF_ENCOUNTER, FileIndexToCorrectName( front, cpp->src_loc.fno ) );
        }
        front.cvars.CppStack = cpp->prev_cpp;
        CMemFree( front, cpp );
    }
    if( front.cvars.SkipLevel > front.cvars.NestLevel ) {
        front.cvars.SkipLevel = front.cvars.NestLevel;
    }
    PPNextToken(front);
    WantEOL(front);
}

bool MacroDel( FrontRun& front, const char *name )
/*******************************/
{
    MEPTR           mentry;
    MEPTR           prev_mentry;
    bool            ret;
    mac_hash_idx    hash;

    ret = false;
    if( IsPPOperatorDefined( name ) ) {
        CErr2p( front, ERR_CANT_UNDEF_THESE_NAMES, name  );
        return( ret );
    }
    hash = CalcHashMacro( name );
    prev_mentry = NULL;
    for( mentry = front.cvars.MacHash[hash]; mentry != NULL; mentry = mentry->next_macro ) {
        if( strcmp( mentry->macro_name, name ) == 0 )
            break;
        prev_mentry = mentry;
    }
    if( mentry != NULL ) {
        if( MacroIsSpecial( mentry ) ) {
            CErr2p( front, ERR_CANT_UNDEF_THESE_NAMES, name );
        } else {
            if( prev_mentry != NULL ) {
                prev_mentry->next_macro = mentry->next_macro;
            } else {
                front.cvars.MacHash[hash] = mentry->next_macro;
            }
            if( (front.cvars.InitialMacroFlags & MFLAG_DEFINED_BEFORE_FIRST_INCLUDE) == 0 ) {
                /*
                 * remember macros that were defined before first include
                 */
                if( mentry->macro_flags & MFLAG_DEFINED_BEFORE_FIRST_INCLUDE ) {
                    mentry->next_macro = front.cvars.UndefMacroList;
                    front.cvars.UndefMacroList = mentry;
                }
            }
            ret = true;
        }
    }
    return( ret );
}


static void CUnDef( FrontRun& front )
{

    PPNextToken(front);
    if( ExpectingToken( front, T_ID ) ) {
        MacroDel( front, front.cvars.Buffer );
        PPNextToken(front);
        CheckEOL(front);
    }
}


static void CLine( FrontRun& front )
{
    FNAMEPTR        flist;
    unsigned        src_line;

    src_line = 0;
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingConstant(front) ) {
        if( !front.cvars.CompFlags.cpp_ignore_line ) {
            src_line = U32FetchTrunc( front.cvars.Constant64 ); // stash in case of side effects
            front.cvars.SrcFiles->src_loc.line = src_line - 1; /* don't count this line */
        }
        PPNextToken(front);
        if( front.cvars.CurToken == T_NULL ) {
            if( !front.cvars.CompFlags.cpp_ignore_line ) {
                if( front.cvars.CompFlags.cpp_mode ) {
                    CppEmitPoundLine( front, src_line, front.cvars.SrcFiles->src_name, false );
                }
            }
        } else {
            if( ExpectingToken( front, T_STRING ) ) {
                if( front.cvars.CompFlags.wide_char_string ) {
                    /*
                     * wide char string not allowed
                     */
                    ExpectString(front);
                } else {
                    if( !front.cvars.CompFlags.cpp_ignore_line ) {
//                        RemoveEscapes( Buffer );
                        flist = AddFlist( front, front.cvars.Buffer );
                        flist->rwflag = false;  // not a real file so no autodep
                        front.cvars.SrcFiles->src_name = flist->name;
                        front.cvars.SrcFiles->src_loc.fno = flist->index;
                        if( front.cvars.CompFlags.cpp_mode ) {
                            CppEmitPoundLine( front, src_line, front.cvars.SrcFiles->src_name, false );
                        }
                    }
                }
            }
            PPNextToken(front);
            CheckEOL(front);
        }
    }
    PPCtlDisableMacros( front );
}

static void get_arg_message( FrontRun& front )
{
    while( front.cvars.CurrChar == ' ' )
        front.cvars.NextChar(front);
    front.cvars.TokenLen = 0;
    while( front.cvars.CurrChar != '\n' && front.cvars.CurrChar != '\r' && front.cvars.CurrChar != LCHR_EOF ) {
        WriteBufferChar( front, front.cvars.CurrChar );
        front.cvars.NextChar(front);
    }
    WriteBufferNullChar(front);
}

static void CError( FrontRun& front )
{
    get_arg_message(front);
    /*
     * Force #error output to be reported, even with preprocessor
     */
    CErr2p( front, ERR_USER_ERROR_MSG, front.cvars.Buffer );
}


static void CWarning( FrontRun& front )
{
    if( front.cvars.CompFlags.extensions_enabled
      || ( front.cvars.CompVars.cstd >= STD_C23 ) ) {
        get_arg_message(front);
        /*
         * Force #error output to be reported, even with preprocessor
         */
        CWarn2p( front, ERR_USER_WARNING_MSG, front.cvars.Buffer );
    } else {
        CUnknown(front);
    }
}


void CppStackInit( FrontRun& front )
{
    front.cvars.NestLevel = 0;
    front.cvars.SkipLevel = 0;
    front.cvars.CppStack = NULL;
}


void CppStackFini( FrontRun& front )
{
    struct cpp_info *cpp;

    while( (cpp = front.cvars.CppStack) != NULL ) {
        SetErrLoc( front, &cpp->src_loc );
        CErr1( front, ERR_MISSING_CENDIF );
        InitErrLoc(front);
        front.cvars.CppStack = cpp->prev_cpp;
        CMemFree( front, cpp );
    }
    front.cvars.CppStack = NULL;
}

static void stringize( char *s )
{
    char    *d;

    d = s;
    while( *s != '\0' ) {
        if( s[0] == '\\' ) {
            if( s[1] == '\\' || s[1] == '\"' ) {
                s++;
            }
        }
        *d++ = *s++;
    }
    *d = '\0';
}

TOKEN Process_Pragma( FrontRun& front )
{
    PPNextToken(front);
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        PPNextToken(front);
        if( front.cvars.CurToken == T_STRING ) {
            char        *token_buf;

            token_buf = CMemStrdup( front, front.cvars.Buffer );
            PPNextToken(front);
            if( front.cvars.CurToken == T_RIGHT_PAREN ) {
                ppctl_t old_ppctl;

                stringize( token_buf );
                InsertReScanPragmaTokens( front, token_buf );
                /*
                 * call CPragma()
                 */
                old_ppctl = front.cvars.PPControl;
                PPCtlEnableEol( front );
                CPragma(front);
                front.cvars.PPControl = old_ppctl;
            } else {
                /*
                 * error, incorrect syntax of the operator _Pragma()
                 */
            }
            CMemFree( front, token_buf );
            PPNextToken(front);
        } else {
            /*
             * error, incorrect syntax of the operator _Pragma()
             */
        }
    } else {
        InsertToken( front, front.cvars.CurToken, front.cvars.Buffer );
        strcpy( front.cvars.Buffer, PPOPERATOR_PRAGMA );
        front.cvars.TokenLen = LiteralLength( PPOPERATOR_PRAGMA );
        front.cvars.CurToken = T_ID;
    }
    return( front.cvars.CurToken );
}
