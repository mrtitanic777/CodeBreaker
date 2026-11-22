/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Source code string literal processing.
*
****************************************************************************/


#include "cvars.h"
#include "wio.h"
#include "watcom.h"
#include "cgswitch.h"
#include "scan.h"
#include "asciiout.h"
#include "unicode.h"
#include "escchars.h"
#include "toggles.h"

#include "clibext.h"


static FILE *OpenUnicodeFile( const char *filename )
{
    FILE        *fp;
    char        fullpath[_MAX_PATH];

#if defined(__QNX__)
    _searchenv( filename, "ETC_PATH", fullpath );
    if( fullpath[0] == '\0' ) {
        #define ETC "/etc/"
        strcpy( fullpath, ETC );
        strcpy( fullpath + LENLIT( ETC ), filename );
    }
#else
    _searchenv( filename, "PATH", fullpath );
#endif
    fp = NULL;
    if( fullpath[0] != '\0' ) {
        fp = fopen( fullpath, "r" );
    }
    return( fp );
}

static void ReadUnicodeFile( FILE *fp )
{
    size_t          i;
    size_t          len;
    unicode_type    unicode_table[256];

    len = fread( unicode_table, sizeof( unicode_type ), 256, fp );
    /*
     * UniCode might be a FAR table
     */
    for( i = 0; i < 256; i++ ) {
        unicode_state().UniCode[i] = ( i < len ) ? unicode_table[i] : 0;
    }
}

void LoadUnicodeTable( unsigned codePage )
{
    FILE        *fp;
    char        filename[20];

    sprintf( filename, "unicode.%3.3u", codePage );
    if( filename[11] != '\0' ) {
        filename[7] = filename[8];
        filename[8] = '.';
    }
    fp = OpenUnicodeFile( filename );
    if( fp != NULL ) {
        ReadUnicodeFile( fp );
        fclose( fp );
    } else {
        CBanner();
        CErr2p( ERR_CANT_OPEN_FILE, filename );
    }
    return;
}

void StringInit( void )
{
    str_hash_idx    hash;

    for( hash = 0; hash < STRING_HASH_SIZE; hash++ ) {
        cvars_state().StringHash[hash] = 0;
    }
}

void FreeLiteral( STR_HANDLE str_lit )
{
    CMemFree( str_lit->literal );
    CMemFree( str_lit );
}

namespace {

/* the statics of this file, one set for each run of the compiler */
struct CstringState {
    target_size CLitLength;   /* length of string literal */
    const unsigned char *pbuf = NULL;
};

}  // namespace

static inline CstringState& cstring_state() { return cbrun_part<CstringState>(); }


static int read_inp( void )
{
    return( *cstring_state().pbuf++ );
}

#define WRITE_BYTE(x) if( buf != NULL ) buf[olen] = x; ++olen

static target_size RemoveEscapes( char *buf, const char *inbuf, target_size ilen )
/*********************************************************************************
 * check mode if buf == NULL, only check required output size
 */
{
    int                 c;
    target_size         olen;
    const unsigned char *pend;

    olen = 0;
    cvars_state().BadTokenInfo = ERR_NONE;
    cstring_state().pbuf = (const unsigned char *)inbuf;
    pend = cstring_state().pbuf + ilen;
    while( cstring_state().pbuf < pend ) {
        c = read_inp();
        if( c == '\\' ) {
            c = ESCChar( read_inp, NULL, &cvars_state().BadTokenInfo );
            if( buf != NULL ) {
                if( !cvars_state().CompFlags.cpp_mode ) {
                    if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
                        if( cvars_state().BadTokenInfo == ERR_INVALID_HEX_CONSTANT ) {
                            CErr1( ERR_INVALID_HEX_CONSTANT );
                        } else if( cvars_state().BadTokenInfo == ERR_CONSTANT_TOO_BIG
                          || cvars_state().BadTokenInfo == ERR_INV_CHAR_CONSTANT ) {
                            CWarn1( cvars_state().BadTokenInfo );
                        }
                    }
                }
            }
            cstring_state().pbuf--; // move pointer to first character after Escape sequence
            if( cvars_state().CompFlags.wide_char_string ) {
                WRITE_BYTE( c );
                c = c >> 8;
            }
        } else {
            if( scan_state().CharSet[c] & C_DB ) {       /* if double-byte character */
                if( cvars_state().CompFlags.jis_to_unicode && cvars_state().CompFlags.wide_char_string ) {
                    c = (c << 8) + read_inp();
                    c = JIS2Unicode( c );
                    WRITE_BYTE( c );
                    c = c >> 8;
                } else {
                    WRITE_BYTE( c );
                    c = read_inp();
                }
            } else if( cvars_state().CompFlags.wide_char_string ) {
                c = EncodeWchar( c );
                WRITE_BYTE( c );
                c = c >> 8;
#if _CPU == 370
            } else {
                _ASCIIOUT( c );
#endif
            }
        }
        WRITE_BYTE( c );
    }
    return( olen );
}

STR_HANDLE GetLiteral( void )
{
    target_size         len;
    target_size         len2;
    char                *s;
    STR_HANDLE          str_lit;
    STR_HANDLE          p;
    STR_HANDLE          q;
    bool_                is_wide;

    /*
     * first we store the whole string in a linked list to see if
     * the end result is wide or not wide
     */
    p = str_lit = static_cast<STR_HANDLE>(CMemAlloc( sizeof( STRING_LITERAL ) ));
    q = NULL;
    is_wide = false;
    do {
        /*
         * if one component is wide then the whole string is wide
         */
        if( cvars_state().CompFlags.wide_char_string )
            is_wide = true;
        if( q != NULL ) {
            p = static_cast<STR_HANDLE>(CMemAlloc( sizeof( STRING_LITERAL ) ));
            q->next_string = p;
        }
        q = p;
        p->next_string = NULL;
        p->length = cvars_state().TokenLen + 1;
        p->literal = ToStringDup( cvars_state().Buffer, cvars_state().TokenLen );
    } while( NextToken() == T_STRING );
    cvars_state().CompFlags.wide_char_string = is_wide;
    /*
     * then remove escapes (C99: translation phase 5), and only then
     * concatenate (translation phase 6), not the other way around!
     */
    len = 1;
    s = NULL;
    q = str_lit;
    do {
        len2 = RemoveEscapes( NULL, q->literal, q->length );
        --len;
        if( is_wide && len != 0 ) {
            --len;
        }
        s = static_cast<char *>(CMemRealloc( s, len + len2 + 1 ));
        RemoveEscapes( &s[len], q->literal, q->length );
        len += len2;
        p = q->next_string;
        if( q != str_lit )
            FreeLiteral( q );
        q = p;
    } while ( q != NULL );
    cstring_state().CLitLength = len;
    CMemFree( str_lit->literal );
    str_lit->literal = s;
    str_lit->length = len;
    str_lit->flags = static_cast<string_flags>(0);
    str_lit->back_handle = NULL;
    str_lit->ref_count = 0;
    return( str_lit );
}

static TYPEPTR StringLeafType( void )
{
    TYPEPTR     typ;

    if( cvars_state().CompFlags.wide_char_string ) {
        typ = ArrayNode( GetType( TYP_USHORT ) );
        typ->u.array->dimension = cstring_state().CLitLength >> 1;
    } else if( cvars_state().StringArrayType != NULL ) {
        typ = cvars_state().StringArrayType;
        cvars_state().StringArrayType->u.array->dimension = cstring_state().CLitLength;
        cvars_state().StringArrayType = NULL;
    } else {
        typ = ArrayNode( GetType( TYP_PLAIN_CHAR ) );
        typ->u.array->dimension = cstring_state().CLitLength;
    }
    return( typ );
}

TREEPTR StringLeaf( string_flags flags )
{
    STR_HANDLE          new_lit;
    STR_HANDLE          strlit;
    TREEPTR             leaf_index;
    str_hash_idx        hash;

    strlit = NULL;
    new_lit = GetLiteral();
#if _INTEL_CPU
    if( cvars_state().TargetSwitches & CGSW_X86_BIG_DATA ) {
        if( !cvars_state().CompFlags.strings_in_code_segment ) {
            if( new_lit->length > cvars_state().DataThreshold ) {
                flags |= STRLIT_FAR;
            }
        }
    }
#endif
    if( cvars_state().CompFlags.wide_char_string )
        flags |= STRLIT_WIDE;
#if _INTEL_CPU
    if( flags & STRLIT_FAR )
        cvars_state().CompFlags.far_strings = true;
#endif
    hash = CalcStringHash( new_lit );
    if( TOGGLE( reuse_duplicate_strings ) ) {
        for( strlit = cvars_state().StringHash[hash]; strlit != NULL; strlit = strlit->next_string ) {
            if( strlit->length == new_lit->length && strlit->flags == flags ) {
                /*
                 * it contains wide characters with 0 bytes
                 * must be memory compare
                 */
                if( memcmp( strlit->literal, new_lit->literal, new_lit->length ) == 0 ) {
                    break;
                }
            }
        }
    }
    if( strlit == NULL ) {
        new_lit->flags = flags;
        ++cvars_state().LitCount;
        cvars_state().LitPoolSize += cstring_state().CLitLength;
        new_lit->next_string = cvars_state().StringHash[hash];
        cvars_state().StringHash[hash] = new_lit;
    } else {            // we found a duplicate
        FreeLiteral( new_lit );
        new_lit = strlit;
    }

    leaf_index = LeafNode( OPR_PUSHSTRING );
    leaf_index->op.u2.string_handle = new_lit;
    // set op.flags field
    leaf_index->u.expr_type = StringLeafType();

    if( cvars_state().CurFunc != NULL ) {
        cvars_state().CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
    }
    return( leaf_index );
}
