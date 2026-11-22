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


#define HasVarArgs(m)       (((m) & MFLAG_HAS_VAR_ARGS) != 0)


static void    CSkip( void );
static void    CSkipIf( void );
static void    CDefine( void );
static void    CInclude( void );
static void    CIfDef( void );
static void    CIfNDef( void );
static void    CIf( void );
static void    CElif( void );
static void    CElse( void );
static void    CEndif( void );
static void    CUnDef( void );
static void    CLine( void );
static void    CError( void );
static void    CIdent( void );
static void    CWarning( void );

struct preproc {
    char  *directive;
    void  (*samelevel)( void ); /* func to call when SkipLevel == NestLevel */
    void  (*skipfunc)( void );  /* func to call when SkipLevel != NestLevel */
};

static unsigned char const PreProcWeights[] = {
/* a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z */
   2, 0, 0, 9, 1, 4, 3, 0, 5, 0, 0, 7, 0, 0, 0,10, 0,11, 0,13,12, 0,14, 0, 0, 0
};

namespace {

/* the statics of this file, one set for each run of the compiler */
struct Cmac2State {
    struct preproc PreProcTable[16] = {
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
};

}  // namespace

static inline Cmac2State& cmac2_state() { return cbrun_part<Cmac2State>(); }


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


static void PPFlush2EOL( void )
{
    while( cvars_state().CurToken != T_NULL && cvars_state().CurToken != T_EOF ) {
        PPNextToken();
        if( cvars_state().CurToken == T_BAD_TOKEN && cvars_state().BadTokenInfo == ERR_MISSING_QUOTE ) {
            CErr1( cvars_state().BadTokenInfo );
        }
    }
}


static void CheckEOL( void )
{
    if( cvars_state().CurToken != T_EOF ) {
        ExpectingToken( T_NULL );
    }
}


static void WantEOL( void )
{
    if( cvars_state().CompFlags.extensions_enabled ) {
        if( cvars_state().CurToken != T_NULL && cvars_state().CurToken != T_EOF ) {
            if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
                CWarn1( ERR_JUNK_FOLLOWS_DIRECTIVE );
            }
        }
    } else {
        CheckEOL();
    }
}


static void IncLevel( bool value )
{
    struct cpp_info *cpp;

    cpp = (struct cpp_info *)CMemAlloc( sizeof( struct cpp_info ) );
    cpp->prev_cpp = cvars_state().CppStack;
    cpp->src_loc = cvars_state().TokenLoc;
    cpp->flist = cvars_state().SrcFiles->src_flist;
    cpp->cpp_type = PRE_IF;
    cpp->processing = false;
    cvars_state().CppStack = cpp;
    if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
        if( value ) {
            ++cvars_state().SkipLevel;
            cpp->processing = true;
        }
    }
    ++cvars_state().NestLevel;
}


static void CUnknown( void )
{
    if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
        CErr2p( ERR_UNKNOWN_DIRECTIVE, cvars_state().Buffer );
    }
}


static void PreProcStmt( void )
{
    struct preproc      *pp;
    unsigned            hash;
    int                 c1;
    int                 c2;

    cvars_state().NextChar();                 /* skip over '#' */
    PPNextToken();
    if( cvars_state().CurToken == T_ID ) {
        c1 = IndexLower( ((unsigned char *)cvars_state().Buffer)[0] );
        c2 = IndexLower( ((unsigned char *)cvars_state().Buffer)[cvars_state().TokenLen - 1] );
        if( c1 >= 0 && c2 >= 0 ) {
            hash = (cvars_state().TokenLen + PreProcWeights[c1] + PreProcWeights[c2]) & 0x0f;
            pp = &cmac2_state().PreProcTable[hash];
            if( strcmp( pp->directive, cvars_state().Buffer ) == 0 ) {
                if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
                    pp->samelevel();
                } else {
                    pp->skipfunc();
                }
            } else {
                CUnknown();
            }
        } else {
            CUnknown();
        }
    } else if( cvars_state().CurToken != T_NULL ) {
        CUnknown();
    }
}

TOKEN CheckControl( void )
{
    bool        lines_skipped;
    ppctl_t     old_ppctl;
    TOKEN       token;

    if( !cvars_state().CompFlags.doing_macro_expansion ) {
        if( cvars_state().CompFlags.cpp_mode ) {
            cvars_state().PrintWhiteSpace = false;
        }
    }
    while( cvars_state().CurrChar == '\n' ) {
        if( TBreak() ) {
            CErr1( ERR_BREAK_KEY_HIT );
            CSuicide();
        }
        lines_skipped = false;
        old_ppctl = cvars_state().PPControl;
        for( ; cvars_state().CurrChar != LCHR_EOF; ) {
            if( cvars_state().CompFlags.cpp_mode ) {
                CppPrtChar( '\n' );
            }
            cvars_state().NextChar();
            if( cvars_state().CurrChar != cvars_state().PreProcChar ) {
                SkipAhead();
            }
            if( cvars_state().CurrChar == LCHR_EOF )
                break;
            if( cvars_state().CurrChar == cvars_state().PreProcChar ) { /* start of comp control line */
                PPCTL_ENABLE_EOL();
                PPCTL_DISABLE_MACROS();
                PPCTL_DISABLE_LEX_ERRORS();
                PreProcStmt();
                PPFlush2EOL();
                cvars_state().PPControl = old_ppctl;
            } else if( cvars_state().SkipLevel != cvars_state().NestLevel ) {
                PPCTL_ENABLE_EOL();
                PPCTL_DISABLE_MACROS();
                PPCTL_DISABLE_LEX_ERRORS();
                PPNextToken();              /* get into token mode */
                PPFlush2EOL();
                cvars_state().PPControl = old_ppctl;
            }
            if( cvars_state().SkipLevel == cvars_state().NestLevel )
                break;
            if( cvars_state().CurrChar == '\n' ) {
                lines_skipped = true;
            }
        }
        if( cvars_state().CompFlags.cpp_mode ) {
            if( lines_skipped ) {
                if( cvars_state().SrcFiles != NULL ) {
                    CppEmitPoundLine( cvars_state().SrcFiles->src_loc.line, cvars_state().SrcFiles->src_name, false );
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


static void CSkip( void )
{
}


static void CSkipIf( void )
{
    IncLevel( false );
}


static void CIdent( void )
{
    if( !cvars_state().CompFlags.extensions_enabled ) {
        CUnknown();
    }
}


void CInclude( void )
{
    bool        in_macro;
    char        buf[82];

    if( cvars_state().PCH_FileName != NULL && !cvars_state().CompFlags.make_precompiled_header ) {
        if( cvars_state().CompFlags.ok_to_use_precompiled_hdr ) {
            cvars_state().CompFlags.use_precompiled_header = true;
        }
    }
    if( cvars_state().CompFlags.use_precompiled_header ) {
        InitBuildPreCompiledHeader();
    }
    cvars_state().InitialMacroFlags = MFLAG_NONE;
    in_macro = ( cvars_state().MacroPtr != NULL );
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    PPCTL_DISABLE_MACROS();
    if( cvars_state().CurToken == T_STRING ) {
        if( !OpenSrcFile( cvars_state().Buffer, FT_HEADER ) ) {
            PrtfFilenameErr( cvars_state().Buffer, FT_HEADER, true );
        }
#if _CPU == 370
        if( !CompFlags.use_precompiled_header ) {
            SrcFiles->colum = Column;    /* do trunc and col on  */
            SrcFiles->trunc = Trunc;     /* on user source files */
        }
#endif
    } else if( cvars_state().CurToken == T_LT ) {
        if( in_macro ) {
            PPCTL_ENABLE_MACROS();
        }
        buf[0] = '\0';
        for( ;; ) {
            PPNextToken();
            if( cvars_state().CurToken == T_GT ) {
                if( !OpenSrcFile( buf, FT_LIBRARY ) ) {
                    PrtfFilenameErr( buf, FT_LIBRARY, true );
                }
                break;
            }
            strncat( buf, cvars_state().Buffer, sizeof( buf ) - 2 );
            if( in_macro && cvars_state().MacroPtr == NULL || cvars_state().CurToken == T_NULL || cvars_state().CurToken == T_EOF ) {
                CErr1( ERR_INVALID_INCLUDE );
                break;
            }
        }
        if( in_macro ) {
            PPCTL_DISABLE_MACROS();
        }
    } else {
        CErr1( ERR_INVALID_INCLUDE );
    }
    if( cvars_state().CurToken != T_EOF ) {
        PPNextToken();
    }
    cvars_state().CompFlags.use_precompiled_header = false;
}


static int FormalParm( MPPTR formal_parms )
{
    int     parm_num;

    parm_num = 1;
    for( ; formal_parms != NULL; formal_parms = formal_parms->next ) {
        if( strcmp( formal_parms->parm, cvars_state().Buffer ) == 0 ) {
            return( parm_num );
        }
        ++parm_num;
    }
    return( 0 );
}


static MEPTR GrabTokens( int parm_count, macro_flags mflags, MPPTR formal_parms, const char *mac_name, source_loc *loc )
{
    MEPTR           mentry;
    MEPTR           new_mentry;
    TOKEN           prev_token;
    TOKEN           prev_non_ws_token;
    size_t          mlen;
    int             parm_num;

    mentry = CreateMEntry( mac_name, 0 );
    mentry->u.parm_count = parm_count;
    mentry->src_loc = *loc;
    mlen = mentry->macro_len;
    mentry->macro_defn = mlen;
    prev_token = T_NULL;
    prev_non_ws_token = T_NULL;
    if( cvars_state().CurToken != T_NULL ) {
        do {
            cvars_state().CurToken = ScanToken();
        } while( cvars_state().CurToken == T_WHITE_SPACE );
        if( cvars_state().CurToken == T_SHARP_SHARP ) {
            CErr1( ERR_MISPLACED_SHARP_SHARP );
            PPNextToken();
        }
    }
    for( ; cvars_state().CurToken != T_NULL && cvars_state().CurToken != T_EOF ; ) {
        switch( cvars_state().CurToken ) {
        case T_SHARP:
            /*
             * if it is a function-like macro definition
             */
            if( parm_count != 0 ) {
                cvars_state().CurToken = T_MACRO_SHARP;
            }
            MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            break;
        case T_SHARP_SHARP:
            cvars_state().CurToken = T_MACRO_SHARP_SHARP;
            MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            break;
        case T_WHITE_SPACE:
            if( prev_token != T_WHITE_SPACE ) {
                MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            }
            break;
        case T_ID:
            parm_num = FormalParm( formal_parms );
            if( parm_num != 0 ) {
                if( HasVarArgs( mflags ) && (parm_num == parm_count - 1) ) {
                    cvars_state().CurToken = T_MACRO_VAR_PARM;
                } else {
                    cvars_state().CurToken = T_MACRO_PARM;
                }
            }
            MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            if( parm_num != 0 ) {
                MacroSegmentAddChar( &mlen, parm_num - 1 );
            } else {
                MacroSegmentAddMem( &mlen, cvars_state().Buffer, cvars_state().TokenLen + 1 );
            }
            break;
        case T_BAD_CHAR:
            MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            MacroSegmentAddChar( &mlen, cvars_state().Buffer[0] );
            if( cvars_state().Buffer[1] != '\0' ) {
                cvars_state().CurToken = T_WHITE_SPACE;
                MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            }
            break;
        case T_STRING:
            if( cvars_state().CompFlags.wide_char_string ) {
                cvars_state().CurToken = T_LSTRING;
            }
            /* fall through */
        case T_LSTRING:
        case T_CONSTANT:
        case T_BAD_TOKEN:
        case T_PPNUMBER:
            MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            MacroSegmentAddMem( &mlen, cvars_state().Buffer, cvars_state().TokenLen + 1 );
            break;
        default:
            MacroSegmentAddToken( &mlen, cvars_state().CurToken );
            break;
        }
        if( cvars_state().CurToken != T_WHITE_SPACE ) {
            if( prev_non_ws_token == T_MACRO_SHARP && cvars_state().CurToken != T_MACRO_PARM && cvars_state().CurToken != T_MACRO_VAR_PARM ) {
                CErr1( ERR_MUST_BE_MACRO_PARM );
//                SET_MTOKEN( MacroOffset + mlen - SIZE_MTOKEN, T_SHARP );
            }
            prev_non_ws_token = cvars_state().CurToken;
        }
        prev_token = cvars_state().CurToken;
        cvars_state().CurToken = ScanToken();
    }
    if( prev_non_ws_token == T_MACRO_SHARP ) {
        CErr1( ERR_MUST_BE_MACRO_PARM );
    }
    if( prev_token == T_WHITE_SPACE ) {
        mlen -= SIZE_MTOKEN;
    }
    MacroSegmentAddToken( &mlen, T_NULL );
    if( prev_non_ws_token == T_MACRO_SHARP_SHARP ) {
        CErr1( ERR_MISPLACED_SHARP_SHARP );
    }
    new_mentry = MacroDefine( mlen, mflags );
    if( new_mentry != NULL ) {
        cvars_state().MacroSize += mlen;
    }
    return( new_mentry );
}


MEPTR MacroScan( void )
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

    PPNextToken();
    if( cvars_state().CurToken != T_ID ) {
        ExpectIdentifier();
        return( NULL );
    }
    if( IS_PPOPERATOR_DEFINED( cvars_state().Buffer ) ) {
        CErr1( ERR_CANT_DEFINE_DEFINED );
        return( NULL );
    }
    token_buf = CMemStrdup( cvars_state().Buffer );
    formal_parms = NULL;
    macro_loc = cvars_state().SrcFileLoc;
    parm_count = 0;             /* 0 ==> no () following */
    mflags = MFLAG_NONE;
    if( cvars_state().CurrChar == '(' ) {     /* parms present */
        PPNextToken();          /* grab the '(' */
        PPNextToken();
        parm_count = 1;         /* 1 ==> () following */
        prev_mp = NULL;
        for( ; cvars_state().CurToken != T_RIGHT_PAREN; ) {
            if( HasVarArgs( mflags ) ) {
                ExpectingAfter( T_RIGHT_PAREN, T_DOT_DOT_DOT );
                return( NULL );
            }
            if( cvars_state().CurToken != T_DOT_DOT_DOT && !ExpectingToken( T_ID ) ) {
                return( NULL );
            }
            ++parm_count;
            if( cvars_state().CurToken == T_DOT_DOT_DOT ) {
                mflags |= MFLAG_HAS_VAR_ARGS;   /* can have no more parms after this. */
            }
            mp = (MPPTR)CMemAlloc( sizeof( MPDEFN ) );
            if( formal_parms == NULL ) {
                formal_parms = mp;
            } else {
                if( FormalParm( formal_parms ) ) {
                    CErr2p( ERR_DUPLICATE_MACRO_PARM, cvars_state().Buffer );
                }
                prev_mp->next = mp;
            }
            if( cvars_state().CurToken == T_DOT_DOT_DOT ) {
                mp->parm = CMemStrdup( "__VA_ARGS__" );
            } else {
                mp->parm = CMemStrdup( cvars_state().Buffer );
            }
            prev_mp = mp;
            PPNextToken();
            if( cvars_state().CurToken == T_RIGHT_PAREN )
                break;
            if( cvars_state().CurToken == T_NULL ) {
                CErr1( ERR_INVALID_MACRO_DEFN );
                break;
            }
            if( HasVarArgs( mflags ) ) {
                ExpectingAfter( T_RIGHT_PAREN, T_DOT_DOT_DOT );
                return( NULL );
            }
            MustRecog( T_COMMA );
            if( cvars_state().CurToken != T_DOT_DOT_DOT && !ExpectingToken( T_ID ) ) {
                return( NULL );
            }
        }
    }
    /*
     * grab replacement tokens
     */
    ppscan_mode = InitPPScan();         // enable T_PPNUMBER tokens
    mentry = GrabTokens( parm_count, mflags, formal_parms, token_buf, &macro_loc );
    FiniPPScan( ppscan_mode );          // disable T_PPNUMBER tokens
    for( ; (mp = formal_parms) != NULL; ) {
        formal_parms = mp->next;
        CMemFree( mp->parm );
        CMemFree( mp );
    }
    CMemFree( token_buf );
    return( mentry );
}

static void CDefine( void )
{
    MEPTR           mentry;

    mentry = MacroScan();
    if( mentry != NULL ) {
        mentry->macro_flags |= MFLAG_USER_DEFINED;
    }
}

static void CIfDef( void )
{
    MEPTR       mentry;

    PPNextToken();
    if( cvars_state().CurToken != T_ID ) {
        ExpectIdentifier();
        IncLevel( false );
        return;
    }
    mentry = MacroLookup( cvars_state().Buffer );
    if( mentry != NULL ) {
        mentry->macro_flags |= MFLAG_REFERENCED;
        IncLevel( true );
    } else {
        IncLevel( false );
    }
    PPNextToken();
    CheckEOL();
}


static void CIfNDef( void )
{
    MEPTR       mentry;

    PPNextToken();
    if( !ExpectingToken( T_ID ) ) {
        IncLevel( false );
        return;
    }
    mentry = MacroLookup( cvars_state().Buffer );
    if( mentry != NULL ) {
        mentry->macro_flags |= MFLAG_REFERENCED;
        IncLevel( false );
    } else {
        IncLevel( true );
    }
    PPNextToken();
    CheckEOL();
}


static void CIf( void )
{
    bool    value;

    PPCTL_ENABLE_LEX_ERRORS();
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    value = PpConstExpr();
    IncLevel( value );
    CheckEOL();
    PPCTL_DISABLE_MACROS();
    PPCTL_DISABLE_LEX_ERRORS();
}


static void CElif( void )
{
    bool    value;

    PPCTL_ENABLE_LEX_ERRORS();
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ( cvars_state().NestLevel == 0 ) || ( cvars_state().CppStack->cpp_type == PRE_ELSE ) ) {
        CErr1( ERR_MISPLACED_ELIF );
    } else {
        if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
            --cvars_state().SkipLevel;                /* start skipping else part */
            cvars_state().CppStack->processing = false;
            cvars_state().CppStack->cpp_type = PRE_ELIF;
        } else if( cvars_state().SkipLevel + 1 == cvars_state().NestLevel ) {
            /*
             * only evaluate the expression when required
             */
            if( cvars_state().CppStack->cpp_type == PRE_IF ) {
                value = PpConstExpr();
                CheckEOL();
                if( value ) {
                    cvars_state().SkipLevel = cvars_state().NestLevel; /* start including else part */
                    cvars_state().CppStack->processing = true;
                    cvars_state().CppStack->cpp_type = PRE_ELIF;
                }
            }
        }
    }
    PPCTL_DISABLE_MACROS();
    PPCTL_DISABLE_LEX_ERRORS();
}


static void CElse( void )
{
    if( ( cvars_state().NestLevel == 0 ) || ( cvars_state().CppStack->cpp_type == PRE_ELSE ) ) {
        CErr1( ERR_MISPLACED_ELSE );
    } else {
        if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
            --cvars_state().SkipLevel;                /* start skipping else part */
            cvars_state().CppStack->processing = false;
        } else if( cvars_state().SkipLevel + 1 == cvars_state().NestLevel ) {
            /*
             * cpp_type will be PRE_ELIF if an elif was true
             */
            if( cvars_state().CppStack->cpp_type == PRE_IF ) {
                cvars_state().SkipLevel = cvars_state().NestLevel;  /* start including else part */
                cvars_state().CppStack->processing = true;
            }
        }
        cvars_state().CppStack->cpp_type = PRE_ELSE;
    }
    PPNextToken();
    WantEOL();
}


static void CEndif( void )
{
    if( cvars_state().NestLevel == 0 ) {
        CErr1( ERR_MISPLACED_ENDIF );
    } else {
        struct cpp_info *cpp;

        --cvars_state().NestLevel;
        cpp = cvars_state().CppStack;
        if( cvars_state().SrcFiles != NULL && cpp->flist != cvars_state().SrcFiles->src_flist ) {
             CWarn2p( ERR_WEIRD_ENDIF_ENCOUNTER, FileIndexToCorrectName( cpp->src_loc.fno ) );
        }
        cvars_state().CppStack = cpp->prev_cpp;
        CMemFree( cpp );
    }
    if( cvars_state().SkipLevel > cvars_state().NestLevel ) {
        cvars_state().SkipLevel = cvars_state().NestLevel;
    }
    PPNextToken();
    WantEOL();
}

bool MacroDel( const char *name )
/*******************************/
{
    MEPTR           mentry;
    MEPTR           prev_mentry;
    bool            ret;
    mac_hash_idx    hash;

    ret = false;
    if( IS_PPOPERATOR_DEFINED( name ) ) {
        CErr2p( ERR_CANT_UNDEF_THESE_NAMES, name  );
        return( ret );
    }
    hash = CalcHashMacro( name );
    prev_mentry = NULL;
    for( mentry = cvars_state().MacHash[hash]; mentry != NULL; mentry = mentry->next_macro ) {
        if( strcmp( mentry->macro_name, name ) == 0 )
            break;
        prev_mentry = mentry;
    }
    if( mentry != NULL ) {
        if( MacroIsSpecial( mentry ) ) {
            CErr2p( ERR_CANT_UNDEF_THESE_NAMES, name );
        } else {
            if( prev_mentry != NULL ) {
                prev_mentry->next_macro = mentry->next_macro;
            } else {
                cvars_state().MacHash[hash] = mentry->next_macro;
            }
            if( (cvars_state().InitialMacroFlags & MFLAG_DEFINED_BEFORE_FIRST_INCLUDE) == 0 ) {
                /*
                 * remember macros that were defined before first include
                 */
                if( mentry->macro_flags & MFLAG_DEFINED_BEFORE_FIRST_INCLUDE ) {
                    mentry->next_macro = cvars_state().UndefMacroList;
                    cvars_state().UndefMacroList = mentry;
                }
            }
            ret = true;
        }
    }
    return( ret );
}


static void CUnDef( void )
{

    PPNextToken();
    if( ExpectingToken( T_ID ) ) {
        MacroDel( cvars_state().Buffer );
        PPNextToken();
        CheckEOL();
    }
}


static void CLine( void )
{
    FNAMEPTR        flist;
    unsigned        src_line;

    src_line = 0;
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingConstant() ) {
        if( !cvars_state().CompFlags.cpp_ignore_line ) {
            src_line = U32FetchTrunc( cvars_state().Constant64 ); // stash in case of side effects
            cvars_state().SrcFiles->src_loc.line = src_line - 1; /* don't count this line */
        }
        PPNextToken();
        if( cvars_state().CurToken == T_NULL ) {
            if( !cvars_state().CompFlags.cpp_ignore_line ) {
                if( cvars_state().CompFlags.cpp_mode ) {
                    CppEmitPoundLine( src_line, cvars_state().SrcFiles->src_name, false );
                }
            }
        } else {
            if( ExpectingToken( T_STRING ) ) {
                if( cvars_state().CompFlags.wide_char_string ) {
                    /*
                     * wide char string not allowed
                     */
                    ExpectString();
                } else {
                    if( !cvars_state().CompFlags.cpp_ignore_line ) {
//                        RemoveEscapes( Buffer );
                        flist = AddFlist( cvars_state().Buffer );
                        flist->rwflag = false;  // not a real file so no autodep
                        cvars_state().SrcFiles->src_name = flist->name;
                        cvars_state().SrcFiles->src_loc.fno = flist->index;
                        if( cvars_state().CompFlags.cpp_mode ) {
                            CppEmitPoundLine( src_line, cvars_state().SrcFiles->src_name, false );
                        }
                    }
                }
            }
            PPNextToken();
            CheckEOL();
        }
    }
    PPCTL_DISABLE_MACROS();
}

static void get_arg_message( void )
{
    while( cvars_state().CurrChar == ' ' )
        cvars_state().NextChar();
    cvars_state().TokenLen = 0;
    while( cvars_state().CurrChar != '\n' && cvars_state().CurrChar != '\r' && cvars_state().CurrChar != LCHR_EOF ) {
        WriteBufferChar( cvars_state().CurrChar );
        cvars_state().NextChar();
    }
    WriteBufferNullChar();
}

static void CError( void )
{
    get_arg_message();
    /*
     * Force #error output to be reported, even with preprocessor
     */
    CErr2p( ERR_USER_ERROR_MSG, cvars_state().Buffer );
}


static void CWarning( void )
{
    if( cvars_state().CompFlags.extensions_enabled
      || ( cvars_state().CompVars.cstd >= STD_C23 ) ) {
        get_arg_message();
        /*
         * Force #error output to be reported, even with preprocessor
         */
        CWarn2p( ERR_USER_WARNING_MSG, cvars_state().Buffer );
    } else {
        CUnknown();
    }
}


void CppStackInit( void )
{
    cvars_state().NestLevel = 0;
    cvars_state().SkipLevel = 0;
    cvars_state().CppStack = NULL;
}


void CppStackFini( void )
{
    struct cpp_info *cpp;

    while( (cpp = cvars_state().CppStack) != NULL ) {
        SetErrLoc( &cpp->src_loc );
        CErr1( ERR_MISSING_CENDIF );
        InitErrLoc();
        cvars_state().CppStack = cpp->prev_cpp;
        CMemFree( cpp );
    }
    cvars_state().CppStack = NULL;
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

TOKEN Process_Pragma( void )
{
    PPNextToken();
    if( cvars_state().CurToken == T_LEFT_PAREN ) {
        PPNextToken();
        if( cvars_state().CurToken == T_STRING ) {
            char        *token_buf;

            token_buf = CMemStrdup( cvars_state().Buffer );
            PPNextToken();
            if( cvars_state().CurToken == T_RIGHT_PAREN ) {
                ppctl_t old_ppctl;

                stringize( token_buf );
                InsertReScanPragmaTokens( token_buf );
                /*
                 * call CPragma()
                 */
                old_ppctl = cvars_state().PPControl;
                PPCTL_ENABLE_EOL();
                CPragma();
                cvars_state().PPControl = old_ppctl;
            } else {
                /*
                 * error, incorrect syntax of the operator _Pragma()
                 */
            }
            CMemFree( token_buf );
            PPNextToken();
        } else {
            /*
             * error, incorrect syntax of the operator _Pragma()
             */
        }
    } else {
        InsertToken( cvars_state().CurToken, cvars_state().Buffer );
        strcpy( cvars_state().Buffer, PPOPERATOR_PRAGMA );
        cvars_state().TokenLen = LENLIT( PPOPERATOR_PRAGMA );
        cvars_state().CurToken = T_ID;
    }
    return( cvars_state().CurToken );
}
