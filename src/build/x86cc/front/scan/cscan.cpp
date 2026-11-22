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


#define diagnose_lex_error()    ((cvars_state().SkipLevel == cvars_state().NestLevel) && (cvars_state().PPControl & PPCTL_NO_LEX_ERRORS) == 0)

#define HEXBIN(c)               ((scan_state().CharSet[c] & C_HX) ? HEX2BIN((c)) : DEC2BIN((c)))
#define OCTAL(c)                ((c) >= '0' && (c) <= '7')

enum scan_class {
    #define pick(e,p) e,
    #include "_scnclas.h"
    #undef pick
};

extern const unsigned char  TokValue[];

#ifdef CHAR_MACRO
static int              SavedCurrChar;      /* used when get tokens from macro */
#endif

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
#ifdef CHAR_MACRO
    LCHR_MACRO, SCAN_MACRO,
#endif
    '\0',       0
};


void NewLineStartPos( FCB *srcfile )
/**********************************/
{
    srcfile->src_line_cnt++;
    srcfile->src_loc.line++;
    srcfile->src_loc.column = 0;
}

#define pick(e,p) static TOKEN p( void );
#include "_scnclas.h"
#undef pick

namespace {

/* the statics of this file, one set for each run of the compiler */
struct CscanState {
    FCB rescan_fcb;
    unsigned char ClassTable[LCHR_MAX];
    TOKEN (*ScanFunc[14])( void ) = {
    #define pick(e,p) p,
    #include "_scnclas.h"
    #undef pick
    };
};

}  // namespace

static inline CscanState& cscan_state() { return cbrun_part<CscanState>(); }

void ReScanInit( const char *ptr )
/********************************/
{
    cscan_state().rescan_fcb.src_ptr = (const unsigned char *)ptr;
}

const char *ReScanPos( void )
/***************************/
{
    return( (const char *)cscan_state().rescan_fcb.src_ptr );
}

static int reScanGetNextChar( void )
/**********************************/
{
    cvars_state().CurrChar = *cscan_state().rescan_fcb.src_ptr++;
    if( cvars_state().CurrChar == '\0' ) {
        cvars_state().CompFlags.rescan_buffer_done = true;
    }
    return( cvars_state().CurrChar );
}

static int reScanGetCharCheck( int c )
/************************************/
{
    if( c == '\0' ) {
        cvars_state().CompFlags.rescan_buffer_done = true;
    }
    return( c );
}

static void reScanGetNextCharUndo( int c )
/****************************************/
{
    /* unused parameters */ (void)c;

    cscan_state().rescan_fcb.src_ptr--;
    cvars_state().CompFlags.rescan_buffer_done = false;
}

static unsigned hashpjw( const char *s, size_t len )
/**************************************************/
{
    unsigned        h;
    size_t          i;

    h = *(const unsigned char *)s++;
    if( len > 1 ) {
        h = ( ( h << 4 ) + *(const unsigned char *)s++ ) & 0x0fff;
        for( i = 2; i < len; i++) {
            h = ( h << 4 ) + *(const unsigned char *)s++;
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
    return( (id_hash_idx)( hash % ID_HASH_SIZE ) );
}

mac_hash_idx CalcHashMacro( const char *id )
/******************************************/
{
    unsigned    hash;

    if( *id == '\0' )
        return( 0 );
    hash = hashpjw( id, strlen( id ) );
#if ( MACRO_HASH_SIZE > 0x0ff0 ) && ( MACRO_HASH_SIZE < 0x0fff )
    if( hash >= MACRO_HASH_SIZE ) {
        hash -= MACRO_HASH_SIZE;
    }
#else
    hash = hash % MACRO_HASH_SIZE;
#endif
    return( (mac_hash_idx)hash );
}

str_hash_idx CalcStringHash( STR_HANDLE lit )
/*******************************************/
{
    if( lit->length == 0 )
        return( 0 );
    return( (str_hash_idx)( hashpjw( lit->literal, lit->length ) % STRING_HASH_SIZE ) );
}

TOKEN KwLookup( const char *buf, size_t len )
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
        if( cvars_state().CompVars.cstd < STD_C99
          && !cvars_state().CompFlags.extensions_enabled )
            return( T_ID );
        break;
    case T__COMPLEX:
    case T_RESTRICT:
    case T__IMAGINARY:
    case T___OW_IMAGINARY_UNIT:
        if( cvars_state().CompVars.cstd < STD_C99 ) {
            return( T_ID );
        }
        break;
    case T__NORETURN:
        if( cvars_state().CompVars.cstd >= STD_C23
          || cvars_state().CompVars.cstd < STD_C11
          && !cvars_state().CompFlags.extensions_enabled )
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

static int getIDName( int c )
/***************************/
{
    while( scan_state().CharSet[c] & (C_AL | C_DI) ) {
        while( scan_state().CharSet[c] & (C_AL | C_DI) ) {
            WriteBufferChar( c );
            c = *cvars_state().SrcFiles->src_ptr++;
        }
        if( (scan_state().CharSet[c] & C_EX) == 0 )
            break;
        c = cvars_state().GetCharCheck( c );
    }
    cvars_state().CurrChar = c;
    return( c );
}

static TOKEN doScanName( void )
/*****************************/
{
    TOKEN       token;
    MEPTR       mentry;

    getIDName( cvars_state().CurrChar );
    WriteBufferNullChar();
    if( cvars_state().CompFlags.doing_macro_expansion
      || (cvars_state().PPControl & PPCTL_NO_EXPAND) ) {
        return( T_ID );
    }
    mentry = MacroLookup( cvars_state().Buffer );
    if( mentry == NULL ) {
        if( IS_PPOPERATOR_PRAGMA( cvars_state().Buffer, cvars_state().TokenLen ) ) {
            token = Process_Pragma();
        } else {
            token = KwLookup( cvars_state().Buffer, cvars_state().TokenLen );
        }
    } else {
        /*
         * this is a macro
         */
        if( MacroIsSpecial( mentry ) ) {
            return( SpecialMacro( mentry ) );
        }
        mentry->macro_flags |= MFLAG_REFERENCED;
        /*
         * if macro requires parameters and next char is not a '('
         * then this is not a macro
         */
        if( MacroWithParenthesis( mentry ) ) {
            SkipAhead();
            if( cvars_state().CurrChar != '(' ) {
                if( cvars_state().CompFlags.cpp_mode ) {
                    InsertToken( T_WHITE_SPACE, " " );
                    token = T_ID;
                } else {
                    if( IS_PPOPERATOR_PRAGMA( cvars_state().Buffer, cvars_state().TokenLen ) ) {
                        token = Process_Pragma();
                    } else {
                        token = KwLookup( cvars_state().Buffer, cvars_state().TokenLen );
                    }
                }
                return( token );
            }
        }
        /*
         * start macro expansion
         */
        DoMacroExpansion( mentry );
        token = GetMacroToken();
#if 0
        if( MacroPtr != NULL ) {
            SavedCurrChar = CurrChar;
            CurrChar = LCHR_MACRO;
        }
#endif
        if( token == T_NULL ) {
            token = T_WHITE_SPACE;
        }
    }
    return( token );
}

static TOKEN ScanName( void )
/***************************/
{
    cvars_state().Buffer[0] = cvars_state().CurrChar;
    cvars_state().TokenLen = 1;
    cvars_state().NextChar();
    return( doScanName() );
}

static TOKEN doScanDotSomething( int c )
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
        c = cvars_state().NextChar();
        if( c == '.' ) {
            cvars_state().Buffer[cvars_state().TokenLen++] = '.';
            cvars_state().Buffer[cvars_state().TokenLen++] = '.';
            cvars_state().NextChar();
            token = T_DOT_DOT_DOT;
        } else {
            cvars_state().CurrChar = '.';
            cvars_state().UnGetChar( c );
        }
    }
    cvars_state().Buffer[cvars_state().TokenLen] = '\0';
    return( token );
}

static TOKEN doScanFloat( bool hex )
/**********************************/
{
    int             c;
    TOKEN           token;
    charset_flags   flags;

    cvars_state().BadTokenInfo = ERR_NONE;
    c = cvars_state().CurrChar;
    if( c == '.' ) {
        flags = ( hex ) ? C_HX | C_DI : C_DI;
        c = WriteBufferCharNextChar( c );
        while( scan_state().CharSet[c] & flags ) {
            c = WriteBufferCharNextChar( c );
        }
        if( cvars_state().TokenLen == 1 ) {   /* .? */
            return( doScanDotSomething( c ) );
        }
    }
    token = T_CONSTANT;
    if( ONE_CASE_EQUAL( c, 'E' )
      || hex && ONE_CASE_EQUAL( c, 'P' ) ) {
        c = WriteBufferCharNextChar( c );
        if( c == '+'
          || c == '-' ) {
            c = WriteBufferCharNextChar( c );
        }
        if( (scan_state().CharSet[c] & C_DI) == 0 ) {
            token = T_BAD_TOKEN;
            cvars_state().BadTokenInfo = ERR_INVALID_FLOATING_POINT_CONSTANT;
        }
        while( scan_state().CharSet[c] & C_DI ) {
            c = WriteBufferCharNextChar( c );
        }
    }
    if( ONE_CASE_EQUAL( c, 'F' ) ) {
        WriteBufferCharNextChar( c );
        cvars_state().ConstType = TYP_FLOAT;
    } else if( ONE_CASE_EQUAL( c, 'L' ) ) {
        WriteBufferCharNextChar( c );
        if( cvars_state().CompFlags.use_long_double ) {
            cvars_state().ConstType = TYP_LONG_DOUBLE;
        } else {
            cvars_state().ConstType = TYP_DOUBLE;
        }
    } else {
        cvars_state().ConstType = TYP_DOUBLE;
    }
    WriteBufferNullChar();
    return( token );
}

static TOKEN doScanAsm( void )
/****************************/
{
    cvars_state().BadTokenInfo = ERR_NONE;
    cvars_state().NextChar();
    while( getIDName( cvars_state().CurrChar ) == '.' ) {
        WriteBufferCharNextChar( '.' );
    }
    WriteBufferNullChar();
    return( T_ID );
}

static TOKEN ScanDot( void )
/**************************/
{
    if( cvars_state().PPControl & PPCTL_ASM ) {
        cvars_state().Buffer[0] = '.';
        cvars_state().TokenLen = 1;
        return( doScanAsm() );
    } else {
        cvars_state().TokenLen = 0;
        return( doScanFloat( false ) );
    }
}

static TOKEN doScanPPNumber( void )
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
        c = cvars_state().NextChar();
        if( (scan_state().CharSet[c] & (C_AL | C_DI))
          || c == '.' ) {
            WriteBufferChar( c );
        } else if( ( ONE_CASE_EQUAL( prevc, 'E' )
          || ( cvars_state().CompVars.cstd > STD_C89 )
          && ONE_CASE_EQUAL( prevc, 'P' ) )
          && ( c == '+' || c == '-' ) ) {
            WriteBufferChar( c );
            if( cvars_state().CompFlags.extensions_enabled ) {
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
                c = cvars_state().NextChar();
                if( (scan_state().CharSet[c] & C_DI) == 0 ) {
                    /*
                     * allow e+<digit>
                     */
                    break;
                }
                WriteBufferChar( c );
            }
        } else {
            break;
        }
    }
    WriteBufferNullChar();
    return( T_PPNUMBER );
}

static TOKEN ScanPPDigit( void )
/******************************/
{
    cvars_state().Buffer[0] = cvars_state().CurrChar;
    cvars_state().TokenLen = 1;
    return( doScanPPNumber() );
}

static TOKEN ScanPPDot( void )
/****************************/
{
    int         c;

    cvars_state().Buffer[0] = '.';
    cvars_state().TokenLen = 1;
    c = cvars_state().NextChar();
    if( scan_state().CharSet[c] & C_DI ) {
        cvars_state().Buffer[cvars_state().TokenLen++] = c;
        return( doScanPPNumber() );
    } else {
        return( doScanDotSomething( c ) );
    }
}

static TOKEN doScanNum( void )
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

    cvars_state().ConstType = TYP_UNDEFINED;
    cvars_state().BadTokenInfo = ERR_NONE;
    overflow = false;
    usuffix = true;
    Set64ValZero( cvars_state().Constant64 );
    if( cvars_state().CurrChar == '0' ) {
        c = cvars_state().NextChar();
        if( ONE_CASE_EQUAL( c, 'X' ) ) {
            bad_token_type = ERR_INVALID_HEX_CONSTANT;
            c = WriteBufferCharNextChar( c );
            while( scan_state().CharSet[c] & (C_HX | C_DI) ) {
                c = WriteBufferCharNextChar( c );
            }

            if( cvars_state().CompVars.cstd > STD_C89 ) {
                if( c == '.'
                  || ONE_CASE_EQUAL( c, 'P' ) ) {
                    return( doScanFloat( true ) );
                }
            }

            if( cvars_state().TokenLen == 2 ) {
                /*
                 * just collected a 0x
                 */
                cvars_state().BadTokenInfo = ERR_INVALID_HEX_CONSTANT;
                if( diagnose_lex_error() ) {
                    CErr1( ERR_INVALID_HEX_CONSTANT );
                }
            } else {
                /*
                 * skip 0x thing
                 */
                curr = cvars_state().Buffer + 2;
                len = cvars_state().TokenLen - 2;
                while( len-- > 0 ) {
                    unsigned char   ch;

                    ch = *(unsigned char *)curr++;
                    if( U64Cnv16( &cvars_state().Constant64, HEXBIN( ch ) ) ) {
                        overflow = true;
                    }
                }
            }
        } else if( ONE_CASE_EQUAL( c, 'B' )
          && ( cvars_state().CompFlags.extensions_enabled
          || ( cvars_state().CompVars.cstd >= STD_C23 ) ) ) {
            bad_token_type = ERR_INVALID_BINARY_CONSTANT;
            c = WriteBufferCharNextChar( c );
            while( c == '0' || c == '1' ) {
                c = WriteBufferCharNextChar( c );
            }

            if( cvars_state().TokenLen == 2 ) {
                /*
                 * just collected a 0b
                 */
                cvars_state().BadTokenInfo = ERR_INVALID_BINARY_CONSTANT;
                if( diagnose_lex_error() ) {
                    CErr1( ERR_INVALID_BINARY_CONSTANT );
                }
            } else {
                /*
                 * skip the 0b start of the binary number
                 */
                curr = cvars_state().Buffer + 2;
                len = cvars_state().TokenLen - 2;
                while( len-- > 0 ) {
                    unsigned char   ch;

                    ch = *(unsigned char *)curr++;
                    if( U64Cnv2( &cvars_state().Constant64, DEC2BIN( ch ) ) ) {
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
            while( scan_state().CharSet[c] & C_DI ) {
                digit89 |= ( c > '7' );
                c = WriteBufferCharNextChar( c );
            }
            if( c == '.'
              || ONE_CASE_EQUAL( c, 'E' ) ) {
                return( doScanFloat( false ) );
            }
            if( digit89 ) {
                /*
                 * if digit 8 or 9 somewhere
                 */
                cvars_state().BadTokenInfo = ERR_INVALID_OCTAL_CONSTANT;
                if( diagnose_lex_error() ) {
                    CErr1( ERR_INVALID_OCTAL_CONSTANT );
                }
            } else {
                curr = cvars_state().Buffer;
                len = cvars_state().TokenLen;
                while( len-- > 0 ) {
                    unsigned char   ch;

                    ch = *(unsigned char *)curr++;
                    if( U64Cnv8( &cvars_state().Constant64, DEC2BIN( ch ) ) ) {
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
        c = cvars_state().NextChar();
        while( scan_state().CharSet[c] & C_DI ) {
            c = WriteBufferCharNextChar( c );
        }
        if( c == '.'
          || ONE_CASE_EQUAL( c, 'E' ) ) {
            return( doScanFloat( false ) );
        }
        curr = cvars_state().Buffer;
        len = cvars_state().TokenLen;
        while( len-- > 0 ) {
            unsigned char   ch;

            ch = *(unsigned char *)curr++;
            if( U64Cnv10( &cvars_state().Constant64, DEC2BIN( ch ) ) ) {
                overflow = true;
            }
        }
    }
    /*
     * collect suffix
     */
    suffix = SUFF_NONE;
    if( ONE_CASE_EQUAL( c, 'U' ) ) {
        suffix |= SUFF_U;
        c = WriteBufferCharNextChar( c );
    }
    if( ONE_CASE_EQUAL( c, 'L' ) ) {
        c = WriteBufferCharNextChar( c );
        if( ONE_CASE_EQUAL( c, 'L' ) ) {
            suffix |= SUFF_LL;
            c = WriteBufferCharNextChar( c );
        } else {
            suffix |= SUFF_L;
        }
        if( ONE_CASE_EQUAL( c, 'U' ) ) {
            suffix |= SUFF_U;
            c = WriteBufferCharNextChar( c );
        }
    } else if( ONE_CASE_EQUAL( c, 'I' ) ) {
        c = WriteBufferCharNextChar( c );
        if( c == '6' ) {
            c = WriteBufferCharNextChar( c );
            if( c == '4' ) {
                suffix |= SUFF_LL | SUFF_MS;
                c = WriteBufferCharNextChar( c );
            } else if( diagnose_lex_error() ) {
                CErr1( ERR_INVALID_CONSTANT );
            }
        } else if( c == '3' ) {
            c = WriteBufferCharNextChar( c );
            if( c == '2' ) {
                suffix |= SUFF_L | SUFF_MS;
                c = WriteBufferCharNextChar( c );
            } else if( diagnose_lex_error() ) {
                CErr1( ERR_INVALID_CONSTANT );
            }
        } else if( c == '1' ) {
            c = WriteBufferCharNextChar( c );
            if( c == '6' ) {
                suffix |= SUFF_16 | SUFF_MS;
                c = WriteBufferCharNextChar( c );
            } else if( diagnose_lex_error() ) {
                CErr1( ERR_INVALID_CONSTANT );
            }
        } else if( c == '8' ) {
            suffix |= SUFF_8 | SUFF_MS;
            c = WriteBufferCharNextChar( c );
        } else if( diagnose_lex_error() ) {
            CErr1( ERR_INVALID_CONSTANT );
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
            if( U64CmpU32( cvars_state().Constant64, TARGET_INT_MAX ) <= 0 ) {
                cvars_state().ConstType = TYP_INT;
                break;
            }
        }
        if( usuffix ) {
            if( U64CmpU32( cvars_state().Constant64, TARGET_UINT_MAX ) <= 0 ) {
                cvars_state().ConstType = TYP_UINT;
                break;
            }
        }
        /* fall through */
    case SUFF_L:
        if( ssuffix ) {
            if( U64CmpU32( cvars_state().Constant64, TARGET_LONG_MAX ) <= 0 ) {
                cvars_state().ConstType = TYP_LONG;
                break;
            }
        }
        if( usuffix ) {
            if( U64CmpU32( cvars_state().Constant64, TARGET_ULONG_MAX ) <= 0 ) {
                cvars_state().ConstType = TYP_ULONG;
                break;
            }
        }
        /* fall through */
    case SUFF_LL:
        if( ssuffix ) {
            if( cvars_state().Constant64.u.sign.v == 0 ) {
                cvars_state().ConstType = TYP_LONG64;
                break;
            }
        }
        if( usuffix ) {
            cvars_state().ConstType = TYP_ULONG64;
        } else if( cvars_state().ConstType == TYP_UNDEFINED ) {
            /*
             * C99 say that signed long long decimal number is undefined type
             * if it cannot be represented by any standard or extended signed type
             * signed integer over LLONG_MAX cannot be represented by OW signed type
             * that it should be undefined type.
             * We report this issue at Warning level 1 and setup it as
             * unsigned long long type
             */
            cvars_state().ConstType = TYP_ULONG64;
            if( diagnose_lex_error() ) {
                CWarn1( ERR_INT_IS_UNSIGNED );
            }
        }
        break;
    }

    token = T_CONSTANT;
    if( scan_state().CharSet[c] & (C_AL | C_DI) ) {
        token = T_BAD_TOKEN;
        while( scan_state().CharSet[c] & (C_AL | C_DI) ) {
            c = WriteBufferCharNextChar( c );
        }
    }
    WriteBufferNullChar();
    if( overflow ) {
        cvars_state().BadTokenInfo = ERR_CONSTANT_TOO_BIG;
        if( diagnose_lex_error() ) {
            CWarn1( ERR_CONSTANT_TOO_BIG );
        }
    }
    if( token == T_BAD_TOKEN ) {
        cvars_state().BadTokenInfo = bad_token_type;
    }
    return( token );
}

static TOKEN ScanNum( void )
/**************************/
{
    cvars_state().Buffer[0] = cvars_state().CurrChar;
    cvars_state().TokenLen = 1;
    if( cvars_state().PPControl & PPCTL_ASM ) {
        return( doScanAsm() );
    } else {
        return( doScanNum() );
    }
}

static TOKEN ScanDelim1( void )
/******************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN       token;

    token = static_cast<TOKEN>(TokValue[cvars_state().CurrChar]);
    cvars_state().Buffer[0] = cvars_state().CurrChar;
    cvars_state().Buffer[1] = '\0';
    cvars_state().TokenLen = 1;
    cvars_state().NextChar();
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

static TOKEN ScanDelim2( void )
/******************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN           token;

    token = static_cast<TOKEN>(TokValue[cvars_state().CurrChar]);
    cvars_state().Buffer[0] = cvars_state().CurrChar;
    cvars_state().TokenLen = 1;
    if( (scan_state().CharSet[cvars_state().NextChar()] & C_DE)
      && checkDelim2( &token, static_cast<TOKEN>(TokValue[cvars_state().CurrChar]) ) ) {
        cvars_state().Buffer[cvars_state().TokenLen++] = cvars_state().CurrChar;
        if( (scan_state().CharSet[cvars_state().NextChar()] & C_DE)
          && checkDelim2( &token, static_cast<TOKEN>(TokValue[cvars_state().CurrChar]) ) ) {
            cvars_state().Buffer[cvars_state().TokenLen++] = cvars_state().CurrChar;
            cvars_state().NextChar();
        }
    }
    cvars_state().Buffer[cvars_state().TokenLen] = '\0';
    return( token );
}

static void doScanComment( void )
/*******************************/
{
    int         c;
    int         prev_char;

    cvars_state().CommentLoc = cvars_state().TokenLoc;
    cvars_state().CompFlags.scanning_comment = true;
    if( cvars_state().CompFlags.cpp_mode ) {
        CppComment( '*' );
        c = cvars_state().NextChar();
        for( ;; ) {
            if( c == '*' ) {
                c = cvars_state().NextChar();
                if( c == '/' )
                    break;
                if( cvars_state().CompFlags.cpp_keep_comments ) {
                    CppPrtChar( '*' );
                }
                continue; // could be **/
            }
            if( c == LCHR_EOF ) {
                break;
            }
            if( c == '\n' ) {
                CppPrtChar( c );
                NewLineStartPos( cvars_state().SrcFiles );
            } else if( c != '\r'
              && cvars_state().CompFlags.cpp_keep_comments ) {
                CppPrtChar( c );
            }
            c = cvars_state().NextChar();
        }
        CppComment( '\0' );
    } else {
        /*
         * make '/' anf '\n' a special characters so that we only have
         * to do one test for each character inside the main loop
         */
        scan_state().CharSet['/'] |= C_EX;   /* make '/' special character */
        scan_state().CharSet['\n'] |= C_EX;  /* make '\n' special character */
        c = '\0';
        for( ; c != LCHR_EOF; ) {
            if( c == '\n' ) {
                NewLineStartPos( cvars_state().SrcFiles );
                cvars_state().TokenLoc = cvars_state().SrcFileLoc = cvars_state().SrcFiles->src_loc;
            }
            do {
                do {
                    prev_char = c;
                    c = *cvars_state().SrcFiles->src_ptr++;
                } while( (scan_state().CharSet[c] & C_EX) == 0 );
                c = cvars_state().GetCharCheck( c );
                if( c == LCHR_EOF ) {
                    break;
                }
            } while( (scan_state().CharSet[c] & C_EX) == 0 );
            if( c == '/' ) {
                if( prev_char == '*' )
                    break;
                /*
                 * get next character and see if it is '*' for nested comment
                 */
                c = cvars_state().NextChar();
                if( c == '*' ) {
                    c = cvars_state().NextChar();
                    if( c == '/' )
                        break;
                    CWarn2( ERR_NESTED_COMMENT, cvars_state().CommentLoc.line );
                }
            }
            /*
             * NextChar might not be pointing to GetNextChar at this point
             */
            while( c != LCHR_EOF && cvars_state().NextChar != GetNextChar ) {
                c = cvars_state().NextChar();
            }
        }
        scan_state().CharSet['\n'] &= ~C_EX;     /* undo '\n' special character */
        scan_state().CharSet['/'] &= ~C_EX;      /* undo '/' special character */
    }
    if( c != LCHR_EOF ) {
        cvars_state().CompFlags.scanning_comment = false;
        cvars_state().NextChar();
    }
}

static TOKEN ScanSlash( void )
/*****************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN   token;

    token = T_DIV;
    cvars_state().Buffer[0] = '/';
    cvars_state().TokenLen = 1;
    cvars_state().NextChar();             /* can't inline this copy of NextChar */
    if( cvars_state().CurrChar == '=' ) { /* if second char is an = */
        cvars_state().Buffer[cvars_state().TokenLen++] = '=';
        cvars_state().NextChar();
        token = T_DIV_EQUAL;
    } else if( cvars_state().CurrChar == '/'
      && !cvars_state().CompFlags.strict_ANSI ) {   /* if C++ // style comment */
        if( cvars_state().CompFlags.cpp_mode ) {
            CppComment( '/' );
        }
        cvars_state().CompFlags.scanning_cpp_comment = true;
        for( ;; ) {
            if( cvars_state().CurrChar == '\r' ) {
                /*
                 * some editors don't put linefeeds on end of lines
                 */
                cvars_state().NextChar();
                break;
            }
            cvars_state().NextChar();
            if( cvars_state().CurrChar == '\n' )
                break;
            if( cvars_state().CurrChar == LCHR_EOF )
                break;
            if( cvars_state().CurrChar == '\0' )
                break;
            if( cvars_state().CompFlags.cpp_mode
              && cvars_state().CompFlags.cpp_keep_comments
              && cvars_state().CurrChar != '\r' ) {
                CppPrtChar( cvars_state().CurrChar );
            }
        }
        if( cvars_state().CompFlags.cpp_mode ) {
            CppComment( '\0' );
        }
        cvars_state().CompFlags.scanning_cpp_comment = false;
        cvars_state().Buffer[0] = ' ';
        token = T_WHITE_SPACE;
    } else if( cvars_state().CurrChar == '*' ) {
        doScanComment();
        cvars_state().Buffer[0] = ' ';
        token = T_WHITE_SPACE;
    }
    cvars_state().Buffer[cvars_state().TokenLen] = '\0';
    return( token );
}

static msg_codes doScanHex( int max, unsigned_64 *pval64, escinp_fn ifn, escout_fn ofn )
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
        c = ifn();
        if( max == 0 )
            break;
        if( (scan_state().CharSet[c] & (C_HX | C_DI)) == 0 )
            break;
        if( ofn != NULL )
            ofn( c );
        if( U64Cnv16( pval64, HEXBIN( c ) ) ) {
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

int ESCChar( escinp_fn ifn, escout_fn ofn, msg_codes *perr_msg )
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

    c = ifn();
    if( OCTAL( c ) ) {
        /*
         * get octal escape sequence
         */
        n = 0;
        i = 3;
        while( i-- > 0 && OCTAL( c ) ) {
            if( ofn != NULL )
                ofn( c );
            n = n * 8 + DEC2BIN( c );
            c = ifn();
        }
    } else if( c == 'x' ) {
        unsigned_64 val64;

        /*
         * get hex escape sequence
         */
        if( ofn != NULL )
            ofn( c );
        err_msg = doScanHex( 127, &val64, ifn, ofn );
        if( err_msg != ERR_NONE )
            *perr_msg = err_msg;
        n = U32FetchTrunc( val64 );
    } else {
        if( ofn != NULL )
            ofn( c );
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
#ifdef __QNX__
        case 'l':
            /*
             * for lazy QNX programmers
             */
            if( CompFlags.extensions_enabled ) {
                c = ESCAPE_n;
            }
            break;
#endif
        case '\\':
        case '\"':
        case '\'':
        case '?':
            break;
        default:
            *perr_msg = ERR_INV_CHAR_CONSTANT;
            break;
        }
#if _CPU == 370
        _ASCIIOUT( c );
#endif
        n = c;
        ifn();
    }
    return( n );
}

int EncodeWchar( int c )
/**********************/
{
    if( cvars_state().CompFlags.use_double_byte ) {
        if( cvars_state().CompFlags.jis_to_unicode ) {
            c = JIS2Unicode( c );
        }
    } else {
        c = unicode_state().UniCode[c];
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

static int read_inp( void )
{
    return( cvars_state().NextChar() );
}

static TOKEN doScanCharConst( DATA_TYPE char_type )
/**************************************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    int         c;
    int         i;
    TOKEN       token;

    Set64ValZero( cvars_state().Constant64 );
    c = cvars_state().NextChar();
    if( c == '\'' ) {
        cvars_state().Buffer[cvars_state().TokenLen++] = '\'';
        cvars_state().NextChar();
        token = T_BAD_TOKEN;
    } else {
        cvars_state().BadTokenInfo = ERR_NONE;
        token = T_CONSTANT;
        i = 0;
        for( ;; ) {
            if( c == '\r'
              || c == '\n' ) {
                token = T_BAD_TOKEN;
                break;
            }
            if( c == '\\' ) {
                cvars_state().Buffer[cvars_state().TokenLen++] = '\\';
                c = ESCChar( read_inp, WriteBufferChar, &cvars_state().BadTokenInfo );
                if( cvars_state().BadTokenInfo == ERR_INVALID_HEX_CONSTANT ) {
                    if( diagnose_lex_error() ) {
                        CErr1( ERR_INVALID_HEX_CONSTANT );
                    }
                } else if( cvars_state().BadTokenInfo == ERR_CONSTANT_TOO_BIG
                  || cvars_state().BadTokenInfo == ERR_INV_CHAR_CONSTANT ) {
                    if( diagnose_lex_error() ) {
                        CWarn1( cvars_state().BadTokenInfo );
                    }
                }
                if( char_type == TYP_WCHAR ) {
                    ++i;
                    /* value = (value << 8) + ((c & 0xFF00) >> 8); */
                    U64ShiftLEq( &cvars_state().Constant64, 8 );
                    U64AddI32( &cvars_state().Constant64, ((c & 0xFF00) >> 8) );
                    c &= 0x00FF;
                }
            } else {
                cvars_state().Buffer[cvars_state().TokenLen++] = c;
                cvars_state().NextChar();
                if( scan_state().CharSet[c] & C_DB ) {
                    /*
                     * if double-byte char
                     */
                    c = (c << 8) + (cvars_state().CurrChar & 0x00FF);
                    if( char_type == TYP_WCHAR ) {
                        if( cvars_state().CompFlags.jis_to_unicode ) {
                            c = JIS2Unicode( c );
                        }
                    }
                    ++i;
                    /* value = (value << 8) + ((c & 0xFF00) >> 8); */
                    U64ShiftLEq( &cvars_state().Constant64, 8 );
                    U64AddI32( &cvars_state().Constant64, ((c & 0xFF00) >> 8) );
                    c &= 0x00FF;
                    cvars_state().Buffer[cvars_state().TokenLen++] = cvars_state().CurrChar;
                    cvars_state().NextChar();
                } else if( char_type == TYP_WCHAR ) {
                    c = EncodeWchar( c );
                    ++i;
                    /* value = (value << 8) + ((c & 0xFF00) >> 8); */
                    U64ShiftLEq( &cvars_state().Constant64, 8 );
                    U64AddI32( &cvars_state().Constant64, ((c & 0xFF00) >> 8) );
                    c &= 0x00FF;
#if _CPU == 370
                } else {
                    _ASCIIOUT( c );
#endif
                }
            }
            ++i;
            /* value = (value << 8) + c; */
            U64ShiftLEq( &cvars_state().Constant64, 8 );
            U64AddI32( &cvars_state().Constant64, c );
            /*
             * handle case where user wants a \ but doesn't escape it
             */
            if( c == '\''
              && cvars_state().CurrChar != '\'' ) {
                token = T_BAD_TOKEN;
                break;
            }
            c = cvars_state().CurrChar;
            if( c == '\'' )
                break;
            if( i >= 4 ) {
                token = T_BAD_TOKEN;
                break;
            }
        }
    }
    if( token == T_BAD_TOKEN ) {
        cvars_state().BadTokenInfo = ERR_INV_CHAR_CONSTANT;
    } else {
        cvars_state().Buffer[cvars_state().TokenLen++] = c;
        cvars_state().NextChar();
        if( cvars_state().BadTokenInfo == ERR_INVALID_HEX_CONSTANT ) {
            token = T_BAD_TOKEN;
        }
    }
    cvars_state().Buffer[cvars_state().TokenLen] = '\0';
    cvars_state().ConstType = char_type;
    if( char_type == TYP_CHAR ) {
        /*
         * character constant has plain character type
         * ConstType must be setup to appropriate character type
         */
        if( cvars_state().CompFlags.signed_char ) {
            /*
             * check if it is single character constant
             * it means value in range 128 ... 255
             * then convert into signed char range -128 ... 127
             */
            if( U64CmpU32( cvars_state().Constant64, 127 ) > 0
              && U64CmpU32( cvars_state().Constant64, 256 ) < 0 ) {
                U64AddI32( &cvars_state().Constant64, -256 );
            }
        } else {
            cvars_state().ConstType = TYP_UCHAR;
        }
    }
    return( token );
}

static TOKEN ScanCharConst( void )
/********************************/
{
    cvars_state().Buffer[0] = '\'';
    cvars_state().TokenLen = 1;
    return( doScanCharConst( TYP_CHAR ) );
}

static TOKEN doScanString( bool wide )
/************************************/
{
    int         c;
    bool        ok;

    ok = false;
    cvars_state().BadTokenInfo = ERR_NONE;
    cvars_state().CompFlags.wide_char_string = false;
    cvars_state().CompFlags.trigraph_alert = false;
    cvars_state().TokenLen = 0;
    c = cvars_state().NextChar();
    for( ;; ) {
        if( c == '\n' ) {
            if( cvars_state().SkipLevel != cvars_state().NestLevel ) {
                if( cvars_state().CompFlags.extensions_enabled ) {
                    CWarn1( ERR_MISSING_QUOTE );
                    ok = true;
                }
            }
            break;
        }
        if( c == LCHR_EOF )
            break;
        if( c == '"' ) {
            cvars_state().NextChar();
            ok = true;
            break;
        }

        if( c == '\\' ) {
            WriteBufferChar( c );
            ESCChar( read_inp, WriteBufferChar, &cvars_state().BadTokenInfo );
            if( cvars_state().BadTokenInfo == ERR_INVALID_HEX_CONSTANT ) {
                if( diagnose_lex_error() ) {
                    CErr1( ERR_INVALID_HEX_CONSTANT );
                }
            } else if( cvars_state().BadTokenInfo == ERR_CONSTANT_TOO_BIG
              || cvars_state().BadTokenInfo == ERR_INV_CHAR_CONSTANT ) {
                if( diagnose_lex_error() ) {
                    CWarn1( cvars_state().BadTokenInfo );
                }
            }
            c = cvars_state().CurrChar;
        } else {
            /*
             * if first character of a double-byte character, then
             * save it and get the next one.
             */
            if( scan_state().CharSet[c] & C_DB ) {
                c = WriteBufferCharNextChar( c );
            }
            c = WriteBufferCharNextChar( c );
        }
    }
    WriteBufferNullChar();
    if( cvars_state().CompFlags.trigraph_alert ) {
        CWarn1( ERR_EXPANDED_TRIGRAPH );
    }
    if( wide )
        cvars_state().CompFlags.wide_char_string = wide;
    if( ok )
        return( T_STRING );
    cvars_state().BadTokenInfo = ERR_MISSING_QUOTE;
    return( T_BAD_TOKEN );
}

static TOKEN ScanString( void )
/*****************************/
{
    return( doScanString( false ) );
}

static TOKEN ScanWide( void )
/****************************
 * scan something that starts with L
 */
{
    int         c;
    TOKEN       token;

    c = cvars_state().NextChar();
    if( c == '"' ) {
        /*
         * L"abc"
         */
        token = doScanString( true );
    } else {
        /*
         * regular identifier
         */
        cvars_state().Buffer[0] = 'L';
        cvars_state().TokenLen = 1;
        if( c == '\'' ) {
            /*
             * L'a'
             */
            cvars_state().Buffer[cvars_state().TokenLen++] = '\'';
            token = doScanCharConst( TYP_WCHAR );
        } else {
            /*
             * regular identifier
             */
            token = doScanName();
        }
    }
    return( token );
}

static TOKEN ScanWhiteSpace( void )
/*********************************/
{
    int         c;

    if( cvars_state().NextChar == getCharAfterBackSlash ) {
        do {
            c = cvars_state().NextChar();
        } while( scan_state().CharSet[c] & C_WS );
    } else {
        do {
            do {
                c = *cvars_state().SrcFiles->src_ptr++;
            } while( scan_state().CharSet[c] & C_WS );
            if( (scan_state().CharSet[c] & C_EX) == 0 )
                break;
            c = cvars_state().GetCharCheck( c );
        } while( scan_state().CharSet[c] & C_WS );
        cvars_state().CurrChar = c;
    }
    return( T_WHITE_SPACE );
}

static void SkipWhiteSpace( int c )
/*********************************/
{
    if( !cvars_state().CompFlags.cpp_mode ) {
        ScanWhiteSpace();
    } else {
        while( scan_state().CharSet[c] & C_WS ) {
            if( c != '\r'
              && IS_PPCTL_NORMAL() ) {
                CppPrtChar( c );
            }
            c = cvars_state().NextChar();
        }
    }
}


void SkipAhead( void )
/********************/
{
    for( ;; ) {
        for( ;; ) {
            if( scan_state().CharSet[cvars_state().CurrChar] & C_WS ) {
                SkipWhiteSpace( cvars_state().CurrChar );
            }
            if( cvars_state().CurrChar != '\n' )
                break;
            if( cvars_state().CompFlags.cpp_mode
              && IS_PPCTL_NORMAL() ) {
                CppPrtChar( '\n' );
            }
            NewLineStartPos( cvars_state().SrcFiles );
            cvars_state().SrcFileLoc = cvars_state().SrcFiles->src_loc;
            cvars_state().NextChar();
        }
        if( cvars_state().CurrChar != '/' )
            break;
        cvars_state().NextChar();
        if( cvars_state().CurrChar == '*' ) {
            cvars_state().TokenLoc = cvars_state().SrcFileLoc;
            doScanComment();
        } else {
            cvars_state().UnGetChar( cvars_state().CurrChar );
            cvars_state().CurrChar = '/';
            break;
        }
    }
}

static TOKEN ScanNewline( void )
/******************************/
{
    NewLineStartPos( cvars_state().SrcFiles );
    cvars_state().SrcFileLoc = cvars_state().SrcFiles->src_loc;
    if( cvars_state().PPControl & PPCTL_EOL )
        return( T_NULL );
    return( CheckControl() );
}

static TOKEN ScanCarriageReturn( void )
/*************************************/
{
    if( cvars_state().NextChar() == '\n' ) {
        return( ScanNewline() );
    } else {
        return( ScanWhiteSpace() );
    }
}

static TOKEN ScanInvalid( void )
/*******************************
 * TokenLen is alway lower then BUF_SIZE that
 * direct access to Buffer array is used
 */
{
    TOKEN   token;

    token = T_BAD_CHAR;
    cvars_state().Buffer[0] = cvars_state().CurrChar;
    cvars_state().Buffer[1] = '\0';
    cvars_state().TokenLen = 1;
    cvars_state().NextChar();
    return( token );
}

#ifdef CHAR_MACRO
static TOKEN ScanMacroToken( void )
/*********************************/
{
    TOKEN   token;

    token = GetMacroToken();
    if( token == T_NULL ) {
        if( CompFlags.cpp_mode ) {
            CppPrtChar( ' ' );
        }
        CurrChar = SavedCurrChar;
        token = ScanToken();
    }
    return( token );
}
#endif

static TOKEN ScanEof( void )
/**************************/
{
    return( T_EOF );
}


TOKEN ScanToken( void )
/*********************/
{
    /*
     * remember line token starts on
     */
    cvars_state().TokenLoc = cvars_state().SrcFileLoc;
//    TokenLen = 1;
//    Buffer[0] = CurrChar;
    return( (*cscan_state().ScanFunc[cscan_state().ClassTable[cvars_state().CurrChar]])() );
}

TOKEN NextToken( void )
/*********************/
{
    do {
        if( cvars_state().MacroPtr == NULL ) {
            cvars_state().CurToken = ScanToken();
        } else {
            cvars_state().CurToken = GetMacroToken();
            if( cvars_state().CurToken == T_NULL ) {
                cvars_state().CurToken = ScanToken();
            }
        }
    } while( cvars_state().CurToken == T_WHITE_SPACE );
#ifdef FDEBUG
    DumpToken();
#endif
    return( cvars_state().CurToken );
}

TOKEN PPNextToken( void )
/************************
 * called from macro pre-processor
 */
{
    do {
        if( cvars_state().MacroPtr == NULL ) {
            cvars_state().CurToken = ScanToken();
        } else {
            cvars_state().CurToken = GetMacroToken();
            if( cvars_state().CurToken == T_NULL ) {
                if( cvars_state().CompFlags.cpp_mode ) {
                    CppPrtChar( ' ' );
                }
                cvars_state().CurToken = ScanToken();
            }
        }
    } while( cvars_state().CurToken == T_WHITE_SPACE );
    return( cvars_state().CurToken );
}

TOKEN ReScanToken( void )
/***********************/
{
    FCB             *oldSrcFile;
    int             saved_currchar;
    int             (*saved_nextchar)( void );
    void            (*saved_ungetchar)( int );
    int             (*saved_getcharcheck)( int );
    TOKEN           token;

    /*
     * save current status
     */
    saved_currchar = cvars_state().CurrChar;
    saved_nextchar = cvars_state().NextChar;
    saved_ungetchar = cvars_state().UnGetChar;
    saved_getcharcheck = cvars_state().GetCharCheck;
    oldSrcFile = cvars_state().SrcFiles;

    cvars_state().SrcFiles = &cscan_state().rescan_fcb;
    cvars_state().NextChar = reScanGetNextChar;
    cvars_state().UnGetChar = reScanGetNextCharUndo;
    cvars_state().GetCharCheck = reScanGetCharCheck;
    cvars_state().CompFlags.rescan_buffer_done = false;

    cvars_state().CurrChar = cvars_state().NextChar();
    cvars_state().CompFlags.doing_macro_expansion = true; /* return macros as ID's */
    token = ScanToken();
    cvars_state().CompFlags.doing_macro_expansion = false;
    if( token == T_STRING
      && cvars_state().CompFlags.wide_char_string ) {
        token = T_LSTRING;
    }
    cvars_state().SrcFiles->src_ptr--;

    cvars_state().SrcFiles = oldSrcFile;
    cvars_state().CurrChar = saved_currchar;
    cvars_state().NextChar = saved_nextchar;
    cvars_state().UnGetChar = saved_ungetchar;
    cvars_state().GetCharCheck = saved_getcharcheck;

    return( token );
}

void ScanInit( void )
/*******************/
{
    int         i;
    int         c;

    memset( &cscan_state().ClassTable[0],   SCAN_INVALID, 256 );
    memset( &cscan_state().ClassTable['A'], SCAN_NAME,    26 );
    memset( &cscan_state().ClassTable['a'], SCAN_NAME,    26 );
    memset( &cscan_state().ClassTable['0'], SCAN_NUM,     10 );
    for( i = 0; (c = InitClassTable[i].chr) != '\0'; i++ ) {
        cscan_state().ClassTable[c] = InitClassTable[i].cls;
    }
    cvars_state().CurrChar = '\n';
    cvars_state().PPControl = PPCTL_NORMAL;
    cvars_state().CompFlags.scanning_comment = false;
    cvars_state().SizeOfCount = 0;
    cvars_state().NextChar = GetNextChar;
    cvars_state().UnGetChar = GetNextCharUndo;
    cvars_state().GetCharCheck = GetCharCheckFile;
}

bool InitPPScan( void )
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
    if( cscan_state().ScanFunc[SCAN_NUM] == ScanNum ) {
        cscan_state().ScanFunc[SCAN_NUM] = ScanPPDigit;
        cscan_state().ScanFunc[SCAN_DOT] = ScanPPDot;
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

void FiniPPScan( bool ppscan_mode )
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
        cscan_state().ScanFunc[SCAN_NUM] = ScanNum;
        cscan_state().ScanFunc[SCAN_DOT] = ScanDot;
    }
}
