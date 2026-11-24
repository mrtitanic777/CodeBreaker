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
* Description:  C lexical analyzer. Closely coupled with preprocessor.
*
****************************************************************************/


#include "cvars.h"
#include "scan.h"
#include "escchars.h"
#include "asciiout.h"
#include "i64.h"
#include "kwhash.h"
#include "unicode.h"
#include "cmacadd.h"
#include "cscanbuf.h"
#include "frontrun.hpp"


static inline bool diagnose_lex_error( FrontRun& front )
{
    return ( front.cvars.SkipLevel == front.cvars.NestLevel ) && ( front.cvars.PPControl & PPCTL_NO_LEX_ERRORS ) == 0;
}

static inline int HexBin( FrontRun& front, int c ) { return ( front.scan.CharSet[c] & C_HX ) ? HexToBin( c ) : DecToBin( c ); }
inline bool IsOctal( int c ) { return ((c) >= '0' && (c) <= '7'); }

enum scan_class {
    #define pick(e,p) e,
    #include "_scnclas.h"
    #undef pick
};

extern const unsigned char  TokValue[];


static struct {
    unsigned short  chr;
    unsigned char   cls;
} const InitClassTable[] = {
    '\r',       SCAN_CR,
    '\n',       SCAN_NEWLINE,
    ' ',        SCAN_WHITESPACE,
    '\f',       SCAN_WHITESPACE,
    '\t',       SCAN_WHITESPACE,
    '\v',       SCAN_WHITESPACE,
    '\'',       SCAN_CHARCONST,
    '"',        SCAN_STRING,
    '(',        SCAN_DELIM1,
    ')',        SCAN_DELIM1,
    ',',        SCAN_DELIM1,
    ';',        SCAN_DELIM1,
    '?',        SCAN_DELIM1,
    '/',        SCAN_SLASH,
    '-',        SCAN_DELIM2,        // -, -=, --, ->
    '=',        SCAN_DELIM2,        // =, ==
    ':',        SCAN_DELIM2,        // :, :>
    '*',        SCAN_DELIM2,        // *, *=
    '[',        SCAN_DELIM1,
    ']',        SCAN_DELIM1,
    '{',        SCAN_DELIM1,
    '}',        SCAN_DELIM1,
    '~',        SCAN_DELIM1,
    '.',        SCAN_DOT,
    '!',        SCAN_DELIM2,        // !, !=
    '#',        SCAN_DELIM2,        // #, ##
    '%',        SCAN_DELIM2,        // %, %=
    '&',        SCAN_DELIM2,        // &, &=, &&
    '+',        SCAN_DELIM2,        // +, +=, ++
    '<',        SCAN_DELIM2,        // <, <=, <<, <<=, <:
    '>',        SCAN_DELIM2,        // >, >=, >>, >>=
    '^',        SCAN_DELIM2,        // ^, ^=
    '|',        SCAN_DELIM2,        // |, |=, ||
    '_',        SCAN_NAME,
    'L',        SCAN_WIDE,
    LCHR_EOF,   SCAN_EOF,
    '\0',       0
};


void NewLineStartPos( FCB *srcfile )
/**********************************/
{
    srcfile->src_line_cnt++;
    srcfile->src_loc.line++;
    srcfile->src_loc.column = 0;
}

#define pick(e,p) static TOKEN p( FrontRun& front );
#include "_scnclas.h"
#undef pick

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CscanState: cscan_state.h) */

}  // namespace

/* (the routines for each class of character: the run starts with these) */
void cscan_start( FrontRun& front )
{
    TOKEN (* const initial[14])( FrontRun& ) = {
    #define pick(e,p) p,
    #include "_scnclas.h"
    #undef pick
    };

    for( int i = 0; i < 14; ++i ) {
        front.cscan.ScanFunc[i] = initial[i];
    }
}

namespace {

}  // namespace


void ReScanInit( FrontRun& front, const char *ptr )
/********************************/
{
    front.cscan.rescan_fcb.src_ptr = reinterpret_cast<const unsigned char *>(ptr);
}

const char *ReScanPos( FrontRun& front )
/***************************/
{
    return( reinterpret_cast<const char *>(front.cscan.rescan_fcb.src_ptr) );
}

static int reScanGetNextChar( FrontRun& front )
/**********************************/
{
    front.cvars.CurrChar = *front.cscan.rescan_fcb.src_ptr++;
    if( front.cvars.CurrChar == '\0' ) {
        front.cvars.CompFlags.rescan_buffer_done = true;
    }
    return( front.cvars.CurrChar );
}

static int reScanGetCharCheck( FrontRun& front, int c )
/************************************/
{
    if( c == '\0' ) {
        front.cvars.CompFlags.rescan_buffer_done = true;
    }
    return( c );
}

static void reScanGetNextCharUndo( FrontRun& front, int c )
/****************************************/
{
    /* unused parameters */ (void)c;

    front.cscan.rescan_fcb.src_ptr--;
    front.cvars.CompFlags.rescan_buffer_done = false;
}

static unsigned hashpjw( const char *s, size_t len )
/**************************************************/
{
    unsigned        h;
    size_t          i;

    h = *reinterpret_cast<const unsigned char *>(s++);
    if( len > 1 ) {
        h = ( ( h << 4 ) + *reinterpret_cast<const unsigned char *>(s++) ) & 0x0fff;
        for( i = 2; i < len; i++) {
            h = ( h << 4 ) + *reinterpret_cast<const unsigned char *>(s++);
            h = ( h ^ ( h >> 12 ) ) & 0x0fff;
        }
    }
    return( h );
}

id_hash_idx CalcHashID( const char *id )
/**************************************/
{
    unsigned    hash;

    if( *id == '\0' )
        return( 0 );
    hash = hashpjw( id, strlen( id ) );
    return( static_cast<id_hash_idx>( hash % ID_HASH_SIZE ) );
}

mac_hash_idx CalcHashMacro( const char *id )
/******************************************/
{
    unsigned    hash;

    if( *id == '\0' )
        return( 0 );
    hash = hashpjw( id, strlen( id ) );
    if( hash >= MACRO_HASH_SIZE ) {
        hash -= MACRO_HASH_SIZE;
    }
    return( static_cast<mac_hash_idx>(hash) );
}

str_hash_idx CalcStringHash( STR_HANDLE lit )
/*******************************************/
{
    if( lit->length == 0 )
        return( 0 );
    return( static_cast<str_hash_idx>( hashpjw( lit->literal, lit->length ) % STRING_HASH_SIZE ) );
}

TOKEN KwLookup( FrontRun& front, const char *buf, size_t len )
/*******************************************/
{
    const char  *keyword;
    TOKEN       token;

    token = static_cast<TOKEN>(keyword_hash( buf, TokValue, len ) + FIRST_KEYWORD);
    /*
     * look up id in keyword table
     */
    switch( token ) {
    case T__BOOL:
    case T_INLINE:
        if( front.cvars.CompVars.cstd < STD_C99
          && !front.cvars.CompFlags.extensions_enabled )
            return( T_ID );
        break;
    case T__COMPLEX:
    case T_RESTRICT:
    case T__IMAGINARY:
    case T___OW_IMAGINARY_UNIT:
        if( front.cvars.CompVars.cstd < STD_C99 ) {
            return( T_ID );
        }
        break;
    case T__NORETURN:
        if( front.cvars.CompVars.cstd >= STD_C23
          || front.cvars.CompVars.cstd < STD_C11
          && !front.cvars.CompFlags.extensions_enabled )
            return( T_ID );
        break;
    }
    keyword = TokenString[token];
    if( *keyword == buf[0] ) {
        if( strcmp( keyword, buf ) == 0 ) {
            return( token );
        }
    }
    /*
     * not in keyword table, so must be just an identifier
     */
    return( T_ID );
}

static int getIDName( FrontRun& front, int c )
/***************************/
{
    while( front.scan.CharSet[c] & (C_AL | C_DI) ) {
        while( front.scan.CharSet[c] & (C_AL | C_DI) ) {
            WriteBufferChar( front, c );
            c = *front.cvars.SrcFiles->src_ptr++;
        }
        if( (front.scan.CharSet[c] & C_EX) == 0 )
            break;
        c = front.cvars.GetCharCheck( front, c );
    }
    front.cvars.CurrChar = c;
    return( c );
}

static TOKEN doScanName( FrontRun& front )
/*****************************/
{
    TOKEN       token;
    MEPTR       mentry;

    getIDName( front, front.cvars.CurrChar );
    WriteBufferNullChar(front);
    if( front.cvars.CompFlags.doing_macro_expansion
      || (front.cvars.PPControl & PPCTL_NO_EXPAND) ) {
        return( T_ID );
    }
    mentry = MacroLookup( front, front.cvars.Buffer );
    if( mentry == NULL ) {
        if( IsPPOperatorPragma( front, front.cvars.Buffer, front.cvars.TokenLen ) ) {
            token = Process_Pragma(front);
        } else {
            token = KwLookup( front, front.cvars.Buffer, front.cvars.TokenLen );
        }
    } else {
        /*
         * this is a macro
         */
        if( MacroIsSpecial( mentry ) ) {
            return( SpecialMacro( front, mentry ) );
        }
        mentry->macro_flags |= MFLAG_REFERENCED;
        /*
         * if macro requires parameters and next char is not a '('
         * then this is not a macro
         */
        if( MacroWithParenthesis( mentry ) ) {
            SkipAhead(front);
            if( front.cvars.CurrChar != '(' ) {
                if( front.cvars.CompFlags.cpp_mode ) {
                    InsertToken( front, T_WHITE_SPACE, " " );
                    token = T_ID;
                } else {
                    if( IsPPOperatorPragma( front, front.cvars.Buffer, front.cvars.TokenLen ) ) {
                        token = Process_Pragma(front);
                    } else {
                        token = KwLookup( front, front.cvars.Buffer, front.cvars.TokenLen );
                    }
                }
                return( token );
            }
        }
        /*
         * start macro expansion
         */
        DoMacroExpansion( front, mentry );
        token = GetMacroToken(front);
        if( token == T_NULL ) {
            token = T_WHITE_SPACE;
        }
    }
    return( token );
}

static TOKEN ScanName( FrontRun& front )
/***************************/
{
    front.cvars.Buffer[0] = front.cvars.CurrChar;
    front.cvars.TokenLen = 1;
    front.cvars.NextChar(front);
    return( doScanName(front) );
}

static TOKEN doScanDotSomething( FrontRun& front, int c )
/***************************************
 * on entry TokenLen = 1
 *
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN   token;

    token = T_DOT;
    if( c == '.' ) {
        c = front.cvars.NextChar(front);
        if( c == '.' ) {
            front.cvars.Buffer[front.cvars.TokenLen++] = '.';
            front.cvars.Buffer[front.cvars.TokenLen++] = '.';
            front.cvars.NextChar(front);
            token = T_DOT_DOT_DOT;
        } else {
            front.cvars.CurrChar = '.';
            front.cvars.UnGetChar( front, c );
        }
    }
    front.cvars.Buffer[front.cvars.TokenLen] = '\0';
    return( token );
}

static TOKEN doScanFloat( FrontRun& front, bool hex )
/**********************************/
{
    int             c;
    TOKEN           token;
    charset_flags   flags;

    front.cvars.BadTokenInfo = ERR_NONE;
    c = front.cvars.CurrChar;
    if( c == '.' ) {
        flags = ( hex ) ? C_HX | C_DI : C_DI;
        c = WriteBufferCharNextChar( front, c );
        while( front.scan.CharSet[c] & flags ) {
            c = WriteBufferCharNextChar( front, c );
        }
        if( front.cvars.TokenLen == 1 ) {   /* .? */
            return( doScanDotSomething( front, c ) );
        }
    }
    token = T_CONSTANT;
    if( OneCaseEqual( c, 'E' )
      || hex && OneCaseEqual( c, 'P' ) ) {
        c = WriteBufferCharNextChar( front, c );
        if( c == '+'
          || c == '-' ) {
            c = WriteBufferCharNextChar( front, c );
        }
        if( (front.scan.CharSet[c] & C_DI) == 0 ) {
            token = T_BAD_TOKEN;
            front.cvars.BadTokenInfo = ERR_INVALID_FLOATING_POINT_CONSTANT;
        }
        while( front.scan.CharSet[c] & C_DI ) {
            c = WriteBufferCharNextChar( front, c );
        }
    }
    if( OneCaseEqual( c, 'F' ) ) {
        WriteBufferCharNextChar( front, c );
        front.cvars.ConstType = TYP_FLOAT;
    } else if( OneCaseEqual( c, 'L' ) ) {
        WriteBufferCharNextChar( front, c );
        if( front.cvars.CompFlags.use_long_double ) {
            front.cvars.ConstType = TYP_LONG_DOUBLE;
        } else {
            front.cvars.ConstType = TYP_DOUBLE;
        }
    } else {
        front.cvars.ConstType = TYP_DOUBLE;
    }
    WriteBufferNullChar(front);
    return( token );
}

static TOKEN doScanAsm( FrontRun& front )
/****************************/
{
    front.cvars.BadTokenInfo = ERR_NONE;
    front.cvars.NextChar(front);
    while( getIDName( front, front.cvars.CurrChar ) == '.' ) {
        WriteBufferCharNextChar( front, '.' );
    }
    WriteBufferNullChar(front);
    return( T_ID );
}

static TOKEN ScanDot( FrontRun& front )
/**************************/
{
    if( front.cvars.PPControl & PPCTL_ASM ) {
        front.cvars.Buffer[0] = '.';
        front.cvars.TokenLen = 1;
        return( doScanAsm(front) );
    } else {
        front.cvars.TokenLen = 0;
        return( doScanFloat( front, false ) );
    }
}

static TOKEN doScanPPNumber( FrontRun& front )
/**********************************
 *
 * 3.1.8 pp-number (C99 §6.4.8 adds 'p'/'P')
 * pp-number:
 *          digit           (checked by caller)
 *          . digit         (checked by caller)
 *          pp-number digit
 *          pp-number identifier-nondigit
 *          pp-number e sign
 *          pp-number E sign
 *          pp-number p sign
 *          pp-number P sign
 *          pp-number .
 */
{
    int         c;
    int         prevc;

    c = 0;
    for( ;; ) {
        prevc = c;
        c = front.cvars.NextChar(front);
        if( (front.scan.CharSet[c] & (C_AL | C_DI))
          || c == '.' ) {
            WriteBufferChar( front, c );
        } else if( ( OneCaseEqual( prevc, 'E' )
          || ( front.cvars.CompVars.cstd > STD_C89 )
          && OneCaseEqual( prevc, 'P' ) )
          && ( c == '+' || c == '-' ) ) {
            WriteBufferChar( front, c );
            if( front.cvars.CompFlags.extensions_enabled ) {
                /*
                 * concession to existing practice...
                 *  #define A2 0x02
                 *  #define A3 0xaa0e+A2
                 *  // users want: 0xaa0e + 0x02
                 *  // not: 0xaa0e + A2 (but, this is what ISO C requires!)
                 */
                /*
                 * advance to next
                 */
                prevc = c;
                c = front.cvars.NextChar(front);
                if( (front.scan.CharSet[c] & C_DI) == 0 ) {
                    /*
                     * allow e+<digit>
                     */
                    break;
                }
                WriteBufferChar( front, c );
            }
        } else {
            break;
        }
    }
    WriteBufferNullChar(front);
    return( T_PPNUMBER );
}

static TOKEN ScanPPDigit( FrontRun& front )
/******************************/
{
    front.cvars.Buffer[0] = front.cvars.CurrChar;
    front.cvars.TokenLen = 1;
    return( doScanPPNumber(front) );
}

static TOKEN ScanPPDot( FrontRun& front )
/****************************/
{
    int         c;

    front.cvars.Buffer[0] = '.';
    front.cvars.TokenLen = 1;
    c = front.cvars.NextChar(front);
    if( front.scan.CharSet[c] & C_DI ) {
        front.cvars.Buffer[front.cvars.TokenLen++] = c;
        return( doScanPPNumber(front) );
    } else {
        return( doScanDotSomething( front, c ) );
    }
}

static TOKEN doScanNum( FrontRun& front )
/****************************/
{
    int             c;
    msg_codes       bad_token_type;
    TOKEN           token;
    const char      *curr;
    size_t          len;
    bool            overflow;
    bool            ssuffix;
    bool            usuffix;
    enum {
        SUFF_NONE,
        SUFF_8    = 0x01, /* 8-bit */
        SUFF_16   = 0x02, /* 16-bit */
        SUFF_L    = 0x03, /* 32-bit */
        SUFF_LL   = 0x04, /* 64-bit */
        SUFF_U    = 0x08, /* unsigned */
        SUFF_MS   = 0x10, /* MS ixx */
        SUFF_MASK = SUFF_8 | SUFF_16 | SUFF_L | SUFF_LL,
    } suffix;

    front.cvars.ConstType = TYP_UNDEFINED;
    front.cvars.BadTokenInfo = ERR_NONE;
    overflow = false;
    usuffix = true;
    Set64ValZero( front.cvars.Constant64 );
    if( front.cvars.CurrChar == '0' ) {
        c = front.cvars.NextChar(front);
        if( OneCaseEqual( c, 'X' ) ) {
            bad_token_type = ERR_INVALID_HEX_CONSTANT;
            c = WriteBufferCharNextChar( front, c );
            while( front.scan.CharSet[c] & (C_HX | C_DI) ) {
                c = WriteBufferCharNextChar( front, c );
            }

            if( front.cvars.CompVars.cstd > STD_C89 ) {
                if( c == '.'
                  || OneCaseEqual( c, 'P' ) ) {
                    return( doScanFloat( front, true ) );
                }
            }

            if( front.cvars.TokenLen == 2 ) {
                /*
                 * just collected a 0x
                 */
                front.cvars.BadTokenInfo = ERR_INVALID_HEX_CONSTANT;
                if( diagnose_lex_error( front ) ) {
                    CErr1( front, ERR_INVALID_HEX_CONSTANT );
                }
            } else {
                /*
                 * skip 0x thing
                 */
                curr = front.cvars.Buffer + 2;
                len = front.cvars.TokenLen - 2;
                while( len-- > 0 ) {
                    unsigned char   ch;

                    ch = *reinterpret_cast<const unsigned char *>(curr++);
                    if( U64Cnv16( &front.cvars.Constant64, HexBin( front, ch ) ) ) {
                        overflow = true;
                    }
                }
            }
        } else if( OneCaseEqual( c, 'B' )
          && ( front.cvars.CompFlags.extensions_enabled
          || ( front.cvars.CompVars.cstd >= STD_C23 ) ) ) {
            bad_token_type = ERR_INVALID_BINARY_CONSTANT;
            c = WriteBufferCharNextChar( front, c );
            while( c == '0' || c == '1' ) {
                c = WriteBufferCharNextChar( front, c );
            }

            if( front.cvars.TokenLen == 2 ) {
                /*
                 * just collected a 0b
                 */
                front.cvars.BadTokenInfo = ERR_INVALID_BINARY_CONSTANT;
                if( diagnose_lex_error( front ) ) {
                    CErr1( front, ERR_INVALID_BINARY_CONSTANT );
                }
            } else {
                /*
                 * skip the 0b start of the binary number
                 */
                curr = front.cvars.Buffer + 2;
                len = front.cvars.TokenLen - 2;
                while( len-- > 0 ) {
                    unsigned char   ch;

                    ch = *reinterpret_cast<const unsigned char *>(curr++);
                    if( U64Cnv2( &front.cvars.Constant64, DecToBin( ch ) ) ) {
                        overflow = true;
                    }
                }
            }
        } else {
            /*
             * scan octal number
             */
            bool    digit89;

            bad_token_type = ERR_INVALID_OCTAL_CONSTANT;
            /*
             * if collecting tokens for macro preprocessor, allow 8 and 9
             * since the argument may be used in with # or ##.
             */
            digit89 = false;
            while( front.scan.CharSet[c] & C_DI ) {
                digit89 |= ( c > '7' );
                c = WriteBufferCharNextChar( front, c );
            }
            if( c == '.'
              || OneCaseEqual( c, 'E' ) ) {
                return( doScanFloat( front, false ) );
            }
            if( digit89 ) {
                /*
                 * if digit 8 or 9 somewhere
                 */
                front.cvars.BadTokenInfo = ERR_INVALID_OCTAL_CONSTANT;
                if( diagnose_lex_error( front ) ) {
                    CErr1( front, ERR_INVALID_OCTAL_CONSTANT );
                }
            } else {
                curr = front.cvars.Buffer;
                len = front.cvars.TokenLen;
                while( len-- > 0 ) {
                    unsigned char   ch;

                    ch = *reinterpret_cast<const unsigned char *>(curr++);
                    if( U64Cnv8( &front.cvars.Constant64, DecToBin( ch ) ) ) {
                        overflow = true;
                    }
                }
            }
        }
    } else {
        /*
         * scan decimal number
         */
        bad_token_type = ERR_INVALID_CONSTANT;
        usuffix = false;
        c = front.cvars.NextChar(front);
        while( front.scan.CharSet[c] & C_DI ) {
            c = WriteBufferCharNextChar( front, c );
        }
        if( c == '.'
          || OneCaseEqual( c, 'E' ) ) {
            return( doScanFloat( front, false ) );
        }
        curr = front.cvars.Buffer;
        len = front.cvars.TokenLen;
        while( len-- > 0 ) {
            unsigned char   ch;

            ch = *reinterpret_cast<const unsigned char *>(curr++);
            if( U64Cnv10( &front.cvars.Constant64, DecToBin( ch ) ) ) {
                overflow = true;
            }
        }
    }
    /*
     * collect suffix
     */
    suffix = SUFF_NONE;
    if( OneCaseEqual( c, 'U' ) ) {
        suffix |= SUFF_U;
        c = WriteBufferCharNextChar( front, c );
    }
    if( OneCaseEqual( c, 'L' ) ) {
        c = WriteBufferCharNextChar( front, c );
        if( OneCaseEqual( c, 'L' ) ) {
            suffix |= SUFF_LL;
            c = WriteBufferCharNextChar( front, c );
        } else {
            suffix |= SUFF_L;
        }
        if( OneCaseEqual( c, 'U' ) ) {
            suffix |= SUFF_U;
            c = WriteBufferCharNextChar( front, c );
        }
    } else if( OneCaseEqual( c, 'I' ) ) {
        c = WriteBufferCharNextChar( front, c );
        if( c == '6' ) {
            c = WriteBufferCharNextChar( front, c );
            if( c == '4' ) {
                suffix |= SUFF_LL | SUFF_MS;
                c = WriteBufferCharNextChar( front, c );
            } else if( diagnose_lex_error( front ) ) {
                CErr1( front, ERR_INVALID_CONSTANT );
            }
        } else if( c == '3' ) {
            c = WriteBufferCharNextChar( front, c );
            if( c == '2' ) {
                suffix |= SUFF_L | SUFF_MS;
                c = WriteBufferCharNextChar( front, c );
            } else if( diagnose_lex_error( front ) ) {
                CErr1( front, ERR_INVALID_CONSTANT );
            }
        } else if( c == '1' ) {
            c = WriteBufferCharNextChar( front, c );
            if( c == '6' ) {
                suffix |= SUFF_16 | SUFF_MS;
                c = WriteBufferCharNextChar( front, c );
            } else if( diagnose_lex_error( front ) ) {
                CErr1( front, ERR_INVALID_CONSTANT );
            }
        } else if( c == '8' ) {
            suffix |= SUFF_8 | SUFF_MS;
            c = WriteBufferCharNextChar( front, c );
        } else if( diagnose_lex_error( front ) ) {
            CErr1( front, ERR_INVALID_CONSTANT );
        }
    }
    /*
     * process constant size
     */
    ssuffix = ( (suffix & SUFF_U) == 0 );
    usuffix = ( !ssuffix || usuffix );
    switch( suffix & SUFF_MASK ) {
    case SUFF_NONE:
    case SUFF_8:
    case SUFF_16:
        if( ssuffix ) {
            if( U64CmpU32( front.cvars.Constant64, TARGET_INT_MAX ) <= 0 ) {
                front.cvars.ConstType = TYP_INT;
                break;
            }
        }
        if( usuffix ) {
            if( U64CmpU32( front.cvars.Constant64, TARGET_UINT_MAX ) <= 0 ) {
                front.cvars.ConstType = TYP_UINT;
                break;
            }
        }
        /* fall through */
    case SUFF_L:
        if( ssuffix ) {
            if( U64CmpU32( front.cvars.Constant64, TARGET_LONG_MAX ) <= 0 ) {
                front.cvars.ConstType = TYP_LONG;
                break;
            }
        }
        if( usuffix ) {
            if( U64CmpU32( front.cvars.Constant64, TARGET_ULONG_MAX ) <= 0 ) {
                front.cvars.ConstType = TYP_ULONG;
                break;
            }
        }
        /* fall through */
    case SUFF_LL:
        if( ssuffix ) {
            if( front.cvars.Constant64.u.sign.v == 0 ) {
                front.cvars.ConstType = TYP_LONG64;
                break;
            }
        }
        if( usuffix ) {
            front.cvars.ConstType = TYP_ULONG64;
        } else if( front.cvars.ConstType == TYP_UNDEFINED ) {
            /*
             * C99 say that signed long long decimal number is undefined type
             * if it cannot be represented by any standard or extended signed type
             * signed integer over LLONG_MAX cannot be represented by OW signed type
             * that it should be undefined type.
             * We report this issue at Warning level 1 and setup it as
             * unsigned long long type
             */
            front.cvars.ConstType = TYP_ULONG64;
            if( diagnose_lex_error( front ) ) {
                CWarn1( front, ERR_INT_IS_UNSIGNED );
            }
        }
        break;
    }

    token = T_CONSTANT;
    if( front.scan.CharSet[c] & (C_AL | C_DI) ) {
        token = T_BAD_TOKEN;
        while( front.scan.CharSet[c] & (C_AL | C_DI) ) {
            c = WriteBufferCharNextChar( front, c );
        }
    }
    WriteBufferNullChar(front);
    if( overflow ) {
        front.cvars.BadTokenInfo = ERR_CONSTANT_TOO_BIG;
        if( diagnose_lex_error( front ) ) {
            CWarn1( front, ERR_CONSTANT_TOO_BIG );
        }
    }
    if( token == T_BAD_TOKEN ) {
        front.cvars.BadTokenInfo = bad_token_type;
    }
    return( token );
}

static TOKEN ScanNum( FrontRun& front )
/**************************/
{
    front.cvars.Buffer[0] = front.cvars.CurrChar;
    front.cvars.TokenLen = 1;
    if( front.cvars.PPControl & PPCTL_ASM ) {
        return( doScanAsm(front) );
    } else {
        return( doScanNum(front) );
    }
}

static TOKEN ScanDelim1( FrontRun& front )
/******************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN       token;

    token = static_cast<TOKEN>(TokValue[front.cvars.CurrChar]);
    front.cvars.Buffer[0] = front.cvars.CurrChar;
    front.cvars.Buffer[1] = '\0';
    front.cvars.TokenLen = 1;
    front.cvars.NextChar(front);
    return( token );
}

static bool checkDelim2( TOKEN *token, TOKEN last )
/*************************************************/
{
    switch( *token ) {
    case T_AND:
        if( last == T_AND ) {           /* && */
            *token = T_AND_AND;
            break;
        }
        if( last == T_EQUAL ) {         /* &= */
            *token = T_AND_EQUAL;
            break;
        }
        return( false );
    case T_PLUS:
        if( last == T_PLUS ) {          /* ++ */
            *token = T_PLUS_PLUS;
            break;
        }
        if( last == T_EQUAL ) {         /* += */
            *token = T_PLUS_EQUAL;
            break;
        }
        return( false );
    case T_MINUS:
        if( last == T_MINUS ) {         /* -- */
            *token = T_MINUS_MINUS;
            break;
        }
        if( last == T_EQUAL ) {         /* -= */
            *token = T_MINUS_EQUAL;
            break;
        }
        if( last == T_GT ) {            /* -> */
            *token = T_ARROW;
            break;
        }
        return( false );
    case T_OR:
        if( last == T_OR ) {            /* || */
            *token = T_OR_OR;
            break;
        }
        if( last == T_EQUAL ) {         /* |= */
            *token = T_OR_EQUAL;
            break;
        }
        return( false );
    case T_LT:
        if( last == T_LT ) {            /* << */
            *token = T_LSHIFT;
            break;
        }
        if( last == T_EQUAL ) {         /* <= */
            *token = T_LE;
            break;
        }
        if( last == T_COLON ) {         /* <: */
            *token = T_LEFT_BRACKET;
            break;
        }
        return( false );
    case T_GT:
        if( last == T_GT ) {            /* >> */
            *token = T_RSHIFT;
            break;
        }
        if( last == T_EQUAL ) {         /* >= */
            *token = T_GE;
            break;
        }
        return( false );
    case T_SHARP:
        if( last == T_SHARP ) {         /* ## */
            *token = T_SHARP_SHARP;
            break;
        }
        return( false );
    case T_EQUAL:
        if( last == T_EQUAL ) {         /* == */
            *token = T_EQ;
            break;
        }
        return( false );
    case T_EXCLAMATION:
        if( last == T_EQUAL ) {         /* != */
            *token = T_NE;
            break;
        }
        return( false );
    case T_PERCENT:
        if( last == T_EQUAL ) {         /* %= */
            *token = T_PERCENT_EQUAL;
            break;
        }
        return( false );
    case T_TIMES:
        if( last == T_EQUAL ) {         /* *= */
            *token = T_TIMES_EQUAL;
            break;
        }
        return( false );
    case T_DIV:
        if( last == T_EQUAL ) {         /* /= */
            *token = T_DIV_EQUAL;
            break;
        }
        return( false );
    case T_XOR:
        if( last == T_EQUAL ) {         /* ^= */
            *token = T_XOR_EQUAL;
            break;
        }
        return( false );
    case T_COLON:
        if( last == T_GT ) {            /* :> */
            /*
             * TODO: according to the standard, ":>" should be an
             * alternative token (digraph) for "]"
             * *token = T_RIGHT_BRACKET;   -> ]
             */
            *token = T_SEG_OP;
            break;
        }
        return( false );
    case T_LSHIFT:
        if( last == T_EQUAL ) {         /* <<= */
            *token = T_LSHIFT_EQUAL;
            break;
        }
        return( false );
    case T_RSHIFT:
        if( last == T_EQUAL ) {         /* >>= */
            *token = T_RSHIFT_EQUAL;
            break;
        }
        return( false );
    default:
        return( false );
    }
    return( true );
}

static TOKEN ScanDelim2( FrontRun& front )
/******************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN           token;

    token = static_cast<TOKEN>(TokValue[front.cvars.CurrChar]);
    front.cvars.Buffer[0] = front.cvars.CurrChar;
    front.cvars.TokenLen = 1;
    if( (front.scan.CharSet[front.cvars.NextChar(front)] & C_DE)
      && checkDelim2( &token, static_cast<TOKEN>(TokValue[front.cvars.CurrChar]) ) ) {
        front.cvars.Buffer[front.cvars.TokenLen++] = front.cvars.CurrChar;
        if( (front.scan.CharSet[front.cvars.NextChar(front)] & C_DE)
          && checkDelim2( &token, static_cast<TOKEN>(TokValue[front.cvars.CurrChar]) ) ) {
            front.cvars.Buffer[front.cvars.TokenLen++] = front.cvars.CurrChar;
            front.cvars.NextChar(front);
        }
    }
    front.cvars.Buffer[front.cvars.TokenLen] = '\0';
    return( token );
}

static void doScanComment( FrontRun& front )
/*******************************/
{
    int         c;
    int         prev_char;

    front.cvars.CommentLoc = front.cvars.TokenLoc;
    front.cvars.CompFlags.scanning_comment = true;
    if( front.cvars.CompFlags.cpp_mode ) {
        CppComment( front, '*' );
        c = front.cvars.NextChar(front);
        for( ;; ) {
            if( c == '*' ) {
                c = front.cvars.NextChar(front);
                if( c == '/' )
                    break;
                if( front.cvars.CompFlags.cpp_keep_comments ) {
                    CppPrtChar( front, '*' );
                }
                continue; // could be **/
            }
            if( c == LCHR_EOF ) {
                break;
            }
            if( c == '\n' ) {
                CppPrtChar( front, c );
                NewLineStartPos( front.cvars.SrcFiles );
            } else if( c != '\r'
              && front.cvars.CompFlags.cpp_keep_comments ) {
                CppPrtChar( front, c );
            }
            c = front.cvars.NextChar(front);
        }
        CppComment( front, '\0' );
    } else {
        /*
         * make '/' anf '\n' a special characters so that we only have
         * to do one test for each character inside the main loop
         */
        front.scan.CharSet['/'] |= C_EX;   /* make '/' special character */
        front.scan.CharSet['\n'] |= C_EX;  /* make '\n' special character */
        c = '\0';
        for( ; c != LCHR_EOF; ) {
            if( c == '\n' ) {
                NewLineStartPos( front.cvars.SrcFiles );
                front.cvars.TokenLoc = front.cvars.SrcFileLoc = front.cvars.SrcFiles->src_loc;
            }
            do {
                do {
                    prev_char = c;
                    c = *front.cvars.SrcFiles->src_ptr++;
                } while( (front.scan.CharSet[c] & C_EX) == 0 );
                c = front.cvars.GetCharCheck( front, c );
                if( c == LCHR_EOF ) {
                    break;
                }
            } while( (front.scan.CharSet[c] & C_EX) == 0 );
            if( c == '/' ) {
                if( prev_char == '*' )
                    break;
                /*
                 * get next character and see if it is '*' for nested comment
                 */
                c = front.cvars.NextChar(front);
                if( c == '*' ) {
                    c = front.cvars.NextChar(front);
                    if( c == '/' )
                        break;
                    CWarn2( front, ERR_NESTED_COMMENT, front.cvars.CommentLoc.line );
                }
            }
            /*
             * NextChar might not be pointing to GetNextChar at this point
             */
            while( c != LCHR_EOF && front.cvars.NextChar != GetNextChar ) {
                c = front.cvars.NextChar(front);
            }
        }
        front.scan.CharSet['\n'] &= ~C_EX;     /* undo '\n' special character */
        front.scan.CharSet['/'] &= ~C_EX;      /* undo '/' special character */
    }
    if( c != LCHR_EOF ) {
        front.cvars.CompFlags.scanning_comment = false;
        front.cvars.NextChar(front);
    }
}

static TOKEN ScanSlash( FrontRun& front )
/*****************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN   token;

    token = T_DIV;
    front.cvars.Buffer[0] = '/';
    front.cvars.TokenLen = 1;
    front.cvars.NextChar(front);             /* can't inline this copy of NextChar */
    if( front.cvars.CurrChar == '=' ) { /* if second char is an = */
        front.cvars.Buffer[front.cvars.TokenLen++] = '=';
        front.cvars.NextChar(front);
        token = T_DIV_EQUAL;
    } else if( front.cvars.CurrChar == '/'
      && !front.cvars.CompFlags.strict_ANSI ) {   /* if C++ // style comment */
        if( front.cvars.CompFlags.cpp_mode ) {
            CppComment( front, '/' );
        }
        front.cvars.CompFlags.scanning_cpp_comment = true;
        for( ;; ) {
            if( front.cvars.CurrChar == '\r' ) {
                /*
                 * some editors don't put linefeeds on end of lines
                 */
                front.cvars.NextChar(front);
                break;
            }
            front.cvars.NextChar(front);
            if( front.cvars.CurrChar == '\n' )
                break;
            if( front.cvars.CurrChar == LCHR_EOF )
                break;
            if( front.cvars.CurrChar == '\0' )
                break;
            if( front.cvars.CompFlags.cpp_mode
              && front.cvars.CompFlags.cpp_keep_comments
              && front.cvars.CurrChar != '\r' ) {
                CppPrtChar( front, front.cvars.CurrChar );
            }
        }
        if( front.cvars.CompFlags.cpp_mode ) {
            CppComment( front, '\0' );
        }
        front.cvars.CompFlags.scanning_cpp_comment = false;
        front.cvars.Buffer[0] = ' ';
        token = T_WHITE_SPACE;
    } else if( front.cvars.CurrChar == '*' ) {
        doScanComment(front);
        front.cvars.Buffer[0] = ' ';
        token = T_WHITE_SPACE;
    }
    front.cvars.Buffer[front.cvars.TokenLen] = '\0';
    return( token );
}

static msg_codes doScanHex( FrontRun& front, int max, unsigned_64 *pval64, escinp_fn ifn, escout_fn ofn )
/***************************************************************************************
 * Warning! this function is also used from cstring.c
 * cannot use Buffer array or NextChar function in any way
 * input and output is done using ifn or ofn functions
 */
{
    int             c;
    int             count;
    bool            too_big;

    too_big = false;
    count = max;
    Set64ValZero( *pval64 );
    for( ;; ) {
        c = ifn(front);
        if( max == 0 )
            break;
        if( (front.scan.CharSet[c] & (C_HX | C_DI)) == 0 )
            break;
        if( ofn != NULL )
            ofn( front, c );
        if( U64Cnv16( pval64, HexBin( front, c ) ) ) {
            too_big = true;
        }
        --max;
    }
    if( count == max ) {
        /*
         * indicate no characters matched
         */
        return( ERR_INVALID_HEX_CONSTANT );
    }
    if( too_big ) {
        return( ERR_CONSTANT_TOO_BIG );
    }
    /*
     * indicate characters were matched
     */
    return( ERR_NONE );
}

int ESCChar( FrontRun& front, escinp_fn ifn, escout_fn ofn, msg_codes *perr_msg )
/***************************************************************
 * Warning! this function is also used from cstring.c
 * cannot use Buffer array or NextChar function in any way
 * input and output is done using ifn or ofn functions
 */
{
    int         n;
    int         i;
    msg_codes   err_msg;
    int         c;

    c = ifn(front);
    if( IsOctal( c ) ) {
        /*
         * get octal escape sequence
         */
        n = 0;
        i = 3;
        while( i-- > 0 && IsOctal( c ) ) {
            if( ofn != NULL )
                ofn( front, c );
            n = n * 8 + DecToBin( c );
            c = ifn(front);
        }
    } else if( c == 'x' ) {
        unsigned_64 val64;

        /*
         * get hex escape sequence
         */
        if( ofn != NULL )
            ofn( front, c );
        err_msg = doScanHex( front, 127, &val64, ifn, ofn );
        if( err_msg != ERR_NONE )
            *perr_msg = err_msg;
        n = U32FetchTrunc( val64 );
    } else {
        if( ofn != NULL )
            ofn( front, c );
        switch( c ) {
        case 'a':
            c = ESCAPE_a;
            break;
        case 'b':
            c = ESCAPE_b;
            break;
        case 'f':
            c = ESCAPE_f;
            break;
        case 'n':
            c = ESCAPE_n;
            break;
        case 'r':
            c = ESCAPE_r;
            break;
        case 't':
            c = ESCAPE_t;
            break;
        case 'v':
            c = ESCAPE_v;
            break;
        case '\\':
        case '\"':
        case '\'':
        case '?':
            break;
        default:
            *perr_msg = ERR_INV_CHAR_CONSTANT;
            break;
        }
        n = c;
        ifn(front);
    }
    return( n );
}

int EncodeWchar( FrontRun& front, int c )
/**********************/
{
    if( front.cvars.CompFlags.use_double_byte ) {
        if( front.cvars.CompFlags.jis_to_unicode ) {
            c = JIS2Unicode( c );
        }
    } else {
        c = front.unicode.UniCode[c];
    }
    return( c );
}

int IndexLower( int c )
{
    switch( c ) {
    case 'z':
        return( 25 );
    case 'y':
        return( 24 );
    case 'x':
        return( 23 );
    case 'w':
        return( 22 );
    case 'v':
        return( 21 );
    case 'u':
        return( 20 );
    case 't':
        return( 19 );
    case 's':
        return( 18 );
    case 'r':
        return( 17 );
    case 'q':
        return( 16 );
    case 'p':
        return( 15 );
    case 'o':
        return( 14 );
    case 'n':
        return( 13 );
    case 'm':
        return( 12 );
    case 'l':
        return( 11 );
    case 'k':
        return( 10 );
    case 'j':
        return( 9 );
    case 'i':
        return( 8 );
    case 'h':
        return( 7 );
    case 'g':
        return( 6 );
    case 'f':
        return( 5 );
    case 'e':
        return( 4 );
    case 'd':
        return( 3 );
    case 'c':
        return( 2 );
    case 'b':
        return( 1 );
    case 'a':
        return( 0 );
    }
    return( -1 );
}

static int read_inp( FrontRun& front )
{
    return( front.cvars.NextChar(front) );
}

static TOKEN doScanCharConst( FrontRun& front, DATA_TYPE char_type )
/**************************************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    int         c;
    int         i;
    TOKEN       token;

    Set64ValZero( front.cvars.Constant64 );
    c = front.cvars.NextChar(front);
    if( c == '\'' ) {
        front.cvars.Buffer[front.cvars.TokenLen++] = '\'';
        front.cvars.NextChar(front);
        token = T_BAD_TOKEN;
    } else {
        front.cvars.BadTokenInfo = ERR_NONE;
        token = T_CONSTANT;
        i = 0;
        for( ;; ) {
            if( c == '\r'
              || c == '\n' ) {
                token = T_BAD_TOKEN;
                break;
            }
            if( c == '\\' ) {
                front.cvars.Buffer[front.cvars.TokenLen++] = '\\';
                c = ESCChar( front, read_inp, WriteBufferChar, &front.cvars.BadTokenInfo );
                if( front.cvars.BadTokenInfo == ERR_INVALID_HEX_CONSTANT ) {
                    if( diagnose_lex_error( front ) ) {
                        CErr1( front, ERR_INVALID_HEX_CONSTANT );
                    }
                } else if( front.cvars.BadTokenInfo == ERR_CONSTANT_TOO_BIG
                  || front.cvars.BadTokenInfo == ERR_INV_CHAR_CONSTANT ) {
                    if( diagnose_lex_error( front ) ) {
                        CWarn1( front, front.cvars.BadTokenInfo );
                    }
                }
                if( char_type == TYP_WCHAR ) {
                    ++i;
                    /* value = (value << 8) + ((c & 0xFF00) >> 8); */
                    U64ShiftLEq( &front.cvars.Constant64, 8 );
                    U64AddI32( &front.cvars.Constant64, ((c & 0xFF00) >> 8) );
                    c &= 0x00FF;
                }
            } else {
                front.cvars.Buffer[front.cvars.TokenLen++] = c;
                front.cvars.NextChar(front);
                if( front.scan.CharSet[c] & C_DB ) {
                    /*
                     * if double-byte char
                     */
                    c = (c << 8) + (front.cvars.CurrChar & 0x00FF);
                    if( char_type == TYP_WCHAR ) {
                        if( front.cvars.CompFlags.jis_to_unicode ) {
                            c = JIS2Unicode( c );
                        }
                    }
                    ++i;
                    /* value = (value << 8) + ((c & 0xFF00) >> 8); */
                    U64ShiftLEq( &front.cvars.Constant64, 8 );
                    U64AddI32( &front.cvars.Constant64, ((c & 0xFF00) >> 8) );
                    c &= 0x00FF;
                    front.cvars.Buffer[front.cvars.TokenLen++] = front.cvars.CurrChar;
                    front.cvars.NextChar(front);
                } else if( char_type == TYP_WCHAR ) {
                    c = EncodeWchar( front, c );
                    ++i;
                    /* value = (value << 8) + ((c & 0xFF00) >> 8); */
                    U64ShiftLEq( &front.cvars.Constant64, 8 );
                    U64AddI32( &front.cvars.Constant64, ((c & 0xFF00) >> 8) );
                    c &= 0x00FF;
                }
            }
            ++i;
            /* value = (value << 8) + c; */
            U64ShiftLEq( &front.cvars.Constant64, 8 );
            U64AddI32( &front.cvars.Constant64, c );
            /*
             * handle case where user wants a \ but doesn't escape it
             */
            if( c == '\''
              && front.cvars.CurrChar != '\'' ) {
                token = T_BAD_TOKEN;
                break;
            }
            c = front.cvars.CurrChar;
            if( c == '\'' )
                break;
            if( i >= 4 ) {
                token = T_BAD_TOKEN;
                break;
            }
        }
    }
    if( token == T_BAD_TOKEN ) {
        front.cvars.BadTokenInfo = ERR_INV_CHAR_CONSTANT;
    } else {
        front.cvars.Buffer[front.cvars.TokenLen++] = c;
        front.cvars.NextChar(front);
        if( front.cvars.BadTokenInfo == ERR_INVALID_HEX_CONSTANT ) {
            token = T_BAD_TOKEN;
        }
    }
    front.cvars.Buffer[front.cvars.TokenLen] = '\0';
    front.cvars.ConstType = char_type;
    if( char_type == TYP_CHAR ) {
        /*
         * character constant has plain character type
         * ConstType must be setup to appropriate character type
         */
        if( front.cvars.CompFlags.signed_char ) {
            /*
             * check if it is single character constant
             * it means value in range 128 ... 255
             * then convert into signed char range -128 ... 127
             */
            if( U64CmpU32( front.cvars.Constant64, 127 ) > 0
              && U64CmpU32( front.cvars.Constant64, 256 ) < 0 ) {
                U64AddI32( &front.cvars.Constant64, -256 );
            }
        } else {
            front.cvars.ConstType = TYP_UCHAR;
        }
    }
    return( token );
}

static TOKEN ScanCharConst( FrontRun& front )
/********************************/
{
    front.cvars.Buffer[0] = '\'';
    front.cvars.TokenLen = 1;
    return( doScanCharConst( front, TYP_CHAR ) );
}

static TOKEN doScanString( FrontRun& front, bool wide )
/************************************/
{
    int         c;
    bool        ok;

    ok = false;
    front.cvars.BadTokenInfo = ERR_NONE;
    front.cvars.CompFlags.wide_char_string = false;
    front.cvars.CompFlags.trigraph_alert = false;
    front.cvars.TokenLen = 0;
    c = front.cvars.NextChar(front);
    for( ;; ) {
        if( c == '\n' ) {
            if( front.cvars.SkipLevel != front.cvars.NestLevel ) {
                if( front.cvars.CompFlags.extensions_enabled ) {
                    CWarn1( front, ERR_MISSING_QUOTE );
                    ok = true;
                }
            }
            break;
        }
        if( c == LCHR_EOF )
            break;
        if( c == '"' ) {
            front.cvars.NextChar(front);
            ok = true;
            break;
        }

        if( c == '\\' ) {
            WriteBufferChar( front, c );
            ESCChar( front, read_inp, WriteBufferChar, &front.cvars.BadTokenInfo );
            if( front.cvars.BadTokenInfo == ERR_INVALID_HEX_CONSTANT ) {
                if( diagnose_lex_error( front ) ) {
                    CErr1( front, ERR_INVALID_HEX_CONSTANT );
                }
            } else if( front.cvars.BadTokenInfo == ERR_CONSTANT_TOO_BIG
              || front.cvars.BadTokenInfo == ERR_INV_CHAR_CONSTANT ) {
                if( diagnose_lex_error( front ) ) {
                    CWarn1( front, front.cvars.BadTokenInfo );
                }
            }
            c = front.cvars.CurrChar;
        } else {
            /*
             * if first character of a double-byte character, then
             * save it and get the next one.
             */
            if( front.scan.CharSet[c] & C_DB ) {
                c = WriteBufferCharNextChar( front, c );
            }
            c = WriteBufferCharNextChar( front, c );
        }
    }
    WriteBufferNullChar(front);
    if( front.cvars.CompFlags.trigraph_alert ) {
        CWarn1( front, ERR_EXPANDED_TRIGRAPH );
    }
    if( wide )
        front.cvars.CompFlags.wide_char_string = wide;
    if( ok )
        return( T_STRING );
    front.cvars.BadTokenInfo = ERR_MISSING_QUOTE;
    return( T_BAD_TOKEN );
}

static TOKEN ScanString( FrontRun& front )
/*****************************/
{
    return( doScanString( front, false ) );
}

static TOKEN ScanWide( FrontRun& front )
/****************************
 * scan something that starts with L
 */
{
    int         c;
    TOKEN       token;

    c = front.cvars.NextChar(front);
    if( c == '"' ) {
        /*
         * L"abc"
         */
        token = doScanString( front, true );
    } else {
        /*
         * regular identifier
         */
        front.cvars.Buffer[0] = 'L';
        front.cvars.TokenLen = 1;
        if( c == '\'' ) {
            /*
             * L'a'
             */
            front.cvars.Buffer[front.cvars.TokenLen++] = '\'';
            token = doScanCharConst( front, TYP_WCHAR );
        } else {
            /*
             * regular identifier
             */
            token = doScanName(front);
        }
    }
    return( token );
}

static TOKEN ScanWhiteSpace( FrontRun& front )
/*********************************/
{
    int         c;

    if( front.cvars.NextChar == getCharAfterBackSlash ) {
        do {
            c = front.cvars.NextChar(front);
        } while( front.scan.CharSet[c] & C_WS );
    } else {
        do {
            do {
                c = *front.cvars.SrcFiles->src_ptr++;
            } while( front.scan.CharSet[c] & C_WS );
            if( (front.scan.CharSet[c] & C_EX) == 0 )
                break;
            c = front.cvars.GetCharCheck( front, c );
        } while( front.scan.CharSet[c] & C_WS );
        front.cvars.CurrChar = c;
    }
    return( T_WHITE_SPACE );
}

static void SkipWhiteSpace( FrontRun& front, int c )
/*********************************/
{
    if( !front.cvars.CompFlags.cpp_mode ) {
        ScanWhiteSpace(front);
    } else {
        while( front.scan.CharSet[c] & C_WS ) {
            if( c != '\r'
              && IsPPCtlNormal( front ) ) {
                CppPrtChar( front, c );
            }
            c = front.cvars.NextChar(front);
        }
    }
}


void SkipAhead( FrontRun& front )
/********************/
{
    for( ;; ) {
        for( ;; ) {
            if( front.scan.CharSet[front.cvars.CurrChar] & C_WS ) {
                SkipWhiteSpace( front, front.cvars.CurrChar );
            }
            if( front.cvars.CurrChar != '\n' )
                break;
            if( front.cvars.CompFlags.cpp_mode
              && IsPPCtlNormal( front ) ) {
                CppPrtChar( front, '\n' );
            }
            NewLineStartPos( front.cvars.SrcFiles );
            front.cvars.SrcFileLoc = front.cvars.SrcFiles->src_loc;
            front.cvars.NextChar(front);
        }
        if( front.cvars.CurrChar != '/' )
            break;
        front.cvars.NextChar(front);
        if( front.cvars.CurrChar == '*' ) {
            front.cvars.TokenLoc = front.cvars.SrcFileLoc;
            doScanComment(front);
        } else {
            front.cvars.UnGetChar( front, front.cvars.CurrChar );
            front.cvars.CurrChar = '/';
            break;
        }
    }
}

static TOKEN ScanNewline( FrontRun& front )
/******************************/
{
    NewLineStartPos( front.cvars.SrcFiles );
    front.cvars.SrcFileLoc = front.cvars.SrcFiles->src_loc;
    if( front.cvars.PPControl & PPCTL_EOL )
        return( T_NULL );
    return( CheckControl(front) );
}

static TOKEN ScanCarriageReturn( FrontRun& front )
/*************************************/
{
    if( front.cvars.NextChar(front) == '\n' ) {
        return( ScanNewline(front) );
    } else {
        return( ScanWhiteSpace(front) );
    }
}

static TOKEN ScanInvalid( FrontRun& front )
/*******************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN   token;

    token = T_BAD_CHAR;
    front.cvars.Buffer[0] = front.cvars.CurrChar;
    front.cvars.Buffer[1] = '\0';
    front.cvars.TokenLen = 1;
    front.cvars.NextChar(front);
    return( token );
}


static TOKEN ScanEof( FrontRun& front )
/**************************/
{
    return( T_EOF );
}


TOKEN ScanToken( FrontRun& front )
/*********************/
{
    /*
     * remember line token starts on
     */
    front.cvars.TokenLoc = front.cvars.SrcFileLoc;
//    TokenLen = 1;
//    Buffer[0] = CurrChar;
    return( (*front.cscan.ScanFunc[front.cscan.ClassTable[front.cvars.CurrChar]])(front) );
}

TOKEN NextToken( FrontRun& front )
/*********************/
{
    do {
        if( front.cvars.MacroPtr == NULL ) {
            front.cvars.CurToken = ScanToken(front);
        } else {
            front.cvars.CurToken = GetMacroToken(front);
            if( front.cvars.CurToken == T_NULL ) {
                front.cvars.CurToken = ScanToken(front);
            }
        }
    } while( front.cvars.CurToken == T_WHITE_SPACE );
    return( front.cvars.CurToken );
}

TOKEN PPNextToken( FrontRun& front )
/************************
 * called from macro pre-processor
 */
{
    do {
        if( front.cvars.MacroPtr == NULL ) {
            front.cvars.CurToken = ScanToken(front);
        } else {
            front.cvars.CurToken = GetMacroToken(front);
            if( front.cvars.CurToken == T_NULL ) {
                if( front.cvars.CompFlags.cpp_mode ) {
                    CppPrtChar( front, ' ' );
                }
                front.cvars.CurToken = ScanToken(front);
            }
        }
    } while( front.cvars.CurToken == T_WHITE_SPACE );
    return( front.cvars.CurToken );
}

TOKEN ReScanToken( FrontRun& front )
/***********************/
{
    FCB             *oldSrcFile;
    int             saved_currchar;
    int             (*saved_nextchar)( FrontRun& );
    void            (*saved_ungetchar)( FrontRun&, int );
    int             (*saved_getcharcheck)( FrontRun&, int );
    TOKEN           token;

    /*
     * save current status
     */
    saved_currchar = front.cvars.CurrChar;
    saved_nextchar = front.cvars.NextChar;
    saved_ungetchar = front.cvars.UnGetChar;
    saved_getcharcheck = front.cvars.GetCharCheck;
    oldSrcFile = front.cvars.SrcFiles;

    front.cvars.SrcFiles = &front.cscan.rescan_fcb;
    front.cvars.NextChar = reScanGetNextChar;
    front.cvars.UnGetChar = reScanGetNextCharUndo;
    front.cvars.GetCharCheck = reScanGetCharCheck;
    front.cvars.CompFlags.rescan_buffer_done = false;

    front.cvars.CurrChar = front.cvars.NextChar(front);
    front.cvars.CompFlags.doing_macro_expansion = true; /* return macros as ID's */
    token = ScanToken(front);
    front.cvars.CompFlags.doing_macro_expansion = false;
    if( token == T_STRING
      && front.cvars.CompFlags.wide_char_string ) {
        token = T_LSTRING;
    }
    front.cvars.SrcFiles->src_ptr--;

    front.cvars.SrcFiles = oldSrcFile;
    front.cvars.CurrChar = saved_currchar;
    front.cvars.NextChar = saved_nextchar;
    front.cvars.UnGetChar = saved_ungetchar;
    front.cvars.GetCharCheck = saved_getcharcheck;

    return( token );
}

void ScanInit( FrontRun& front )
/*******************/
{
    int         i;
    int         c;

    memset( &front.cscan.ClassTable[0],   SCAN_INVALID, 256 );
    memset( &front.cscan.ClassTable['A'], SCAN_NAME,    26 );
    memset( &front.cscan.ClassTable['a'], SCAN_NAME,    26 );
    memset( &front.cscan.ClassTable['0'], SCAN_NUM,     10 );
    for( i = 0; (c = InitClassTable[i].chr) != '\0'; i++ ) {
        front.cscan.ClassTable[c] = InitClassTable[i].cls;
    }
    front.cvars.CurrChar = '\n';
    front.cvars.PPControl = PPCTL_NORMAL;
    front.cvars.CompFlags.scanning_comment = false;
    front.cvars.SizeOfCount = 0;
    front.cvars.NextChar = GetNextChar;
    front.cvars.UnGetChar = GetNextCharUndo;
    front.cvars.GetCharCheck = GetCharCheckFile;
}

bool InitPPScan( FrontRun& front )
/**********************
 * called by CollectParms() to gather tokens for macro parms
 * and CDefine() to gather tokens for macro definition
 * example usage:
 *      bool ppscan_mode;
 *      ppscan_mode = InitPPScan();
 *      CollectParms();
 *      FiniPPScan( ppscan_mode );
 */
{
    if( front.cscan.ScanFunc[SCAN_NUM] == ScanNum ) {
        front.cscan.ScanFunc[SCAN_NUM] = ScanPPDigit;
        front.cscan.ScanFunc[SCAN_DOT] = ScanPPDot;
        /*
         * indicate changed to PP mode
         */
        return( true );
    }
    /*
     * indicate already in PP mode
     */
    return( false );
}

void FiniPPScan( FrontRun& front, bool ppscan_mode )
/**********************************
 * called when CollectParms() and
 * CDefine() are finished gathering tokens
 * if InitPPScan() changed into PP mode
 * reset back to normal mode
 */
{
    if( ppscan_mode ) {
        /*
         * reset back to normal mode
         */
        front.cscan.ScanFunc[SCAN_NUM] = ScanNum;
        front.cscan.ScanFunc[SCAN_DOT] = ScanDot;
    }
}
