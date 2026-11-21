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
* Description:  String resource management.
*
****************************************************************************/


#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include "wresall.h"
#include "rcstrblk.h"
#include "rcstr.h"
#include "build/x86link/formats/rcrtns.h"
#include "memfuncs.h"


#if !defined( INSIDE_WLINK ) || defined( _OS2 )

namespace {

using CompareFn = int (*)( const void *, const void * );

/*
 * strlist is a SORTED array of name_ptr's of size num; the elements that compare equal to the one before
 * are dropped (the first of each run is kept) and the number left is returned
 */
std::size_t RemoveRedundantStrings( name_ptr *strlist, std::size_t num, CompareFn compare )
{
    std::size_t     curr = 0;       /* element being examined */
    std::size_t     last = 0;       /* number of unique elements kept so far */

    while( curr < num ) {
        strlist[last] = strlist[curr];      /* this works on the first iteration */
        while( curr < num && compare( &strlist[curr], &strlist[last] ) == 0 ) {
            curr++;
        }
        last++;
    }
    return last;
}

/*
 * strlist is an array of name_ptr's of size num
 */
std::size_t SortAndRemoveRedundantStrings( name_ptr *strlist, std::size_t num, CompareFn compare )
{
    std::qsort( strlist, num, sizeof( void * ), compare );
    return RemoveRedundantStrings( strlist, num, compare );
}

/*
 * The list will be a list of pointers to WResIDName's
 */
std::size_t InitStringIDNamesList( WResDir dir, name_ptr *list, std::size_t len )
{
    name_ptr        *element = list;

    for( WResDirWindow wind = WResFirstResource( dir ); !WResIsEmptyWindow( wind ); wind = WResNextResource( wind, dir ) ) {
        if( WResIsFirstResOfType( wind ) ) {
            WResTypeInfo *typeinfo = WResGetTypeInfo( wind );
            if( typeinfo->TypeName.IsName ) {
                if( len == 0 )
                    return 0;       /* should never occur */
                element->u.name_id = &(typeinfo->TypeName.ID.Name);
                element++;
                len--;
            }
        }
        if( WResIsFirstLangOfRes( wind ) ) {
            WResResInfo *resinfo = WResGetResInfo( wind );
            if( resinfo->ResName.IsName ) {
                if( len == 0 )
                    return 0;       /* should never occur */
                element->u.name_id = &(resinfo->ResName.ID.Name);
                element++;
                len--;
            }
        }
    }

    return static_cast<std::size_t>( element - list );
}

// the name with its length in front, upper-cased and without a NUL terminator (the 16-bit form)
char *StrUprCpy( char *dst, const char *src, uint_16 length )
{
    /* output length */
    *dst++ = static_cast<char>( length );
    /* output string without NULL terminator*/
    while( length-- > 0 ) {
        *dst++ = static_cast<char>( std::toupper( static_cast<unsigned char>( *src ) ) );
        src++;
    }
    return dst;
}

// the same, each character a 16-bit one (the 32-bit form)
char *StrUprCpyToUni( char *dst, const char *src, uint_16 length )
{
    /* output length */
    MPUT_16( dst, length );
    dst += sizeof( uint_16 );
    /* output string without NULL terminator*/
    while( length-- > 0 ) {
        *dst++ = static_cast<char>( std::toupper( static_cast<unsigned char>( *src ) ) );
        *dst++ = '\0';
        src++;
    }
    return dst;
}

/*
 * the string list should be filled in when this is called
 */
void ConstructStringIDNamesBlock(Link& link,  StringsBlock *str )
{
    /*
     * calculate the size of the block needed
     */
    str->StringBlockSize = 0;
    unsigned cnt = str->StringListLen;
    for( unsigned i = 0; i < cnt; i++ ) {
        const uint_16 len = str->StringList[i].u.name_id->NumChars + 1;
        if( str->UseUnicode ) {
            str->StringBlockSize += 2 * len;
        } else {
            str->StringBlockSize += len;
        }
    }
    /*
     * allocate the block for the strings
     */
    str->StringBlock = static_cast<char *>( MemAllocSafe(link,  str->StringBlockSize ) );
    /*
     * copy the strings into the block
     */
    char *nextstring = str->StringBlock;
    cnt = str->StringListLen;
    for( unsigned i = 0; i < cnt; i++ ) {
        WResIDName *name_id = str->StringList[i].u.name_id;
        str->StringList[i].u.str = nextstring;
        if( str->UseUnicode ) {
            nextstring = StrUprCpyToUni( nextstring, name_id->Name, name_id->NumChars );
        } else {
            nextstring = StrUprCpy( nextstring, name_id->Name, name_id->NumChars );
        }
    }
}

int CompareWResIDNames( const void *n1, const void *n2 )
{
    return WResIDNameCmp( static_cast<const name_ptr *>( n1 )->u.name_id, static_cast<const name_ptr *>( n2 )->u.name_id );
}

} // namespace

void StringIDNamesBlockBuild(Link& link,  StringsBlock *str, WResDir dir, bool use_unicode )
{
    if( WResIsEmpty( dir ) ) {
        /*
         * Empty directories are allowed since windows exe's may contain
         * no resources
         */
    } else {
        /*
         * set the initial list_len to be the max possible
         */
        std::size_t list_len = dir->NumTypes + dir->NumResources;
        str->UseUnicode = use_unicode;
        str->StringList = static_cast<name_ptr *>( MemAllocSafe(link,  list_len * sizeof( *str->StringList ) ) );

        list_len = InitStringIDNamesList( dir, str->StringList, list_len );
        list_len = SortAndRemoveRedundantStrings( str->StringList, list_len, CompareWResIDNames );
        str->StringListLen = static_cast<uint_16>( list_len );

        if( list_len == 0 ) {
            MemFree( str->StringList );
            str->StringList = nullptr;
            str->StringBlock = nullptr;
            str->StringBlockSize = 0;
        } else {
            str->StringList = static_cast<name_ptr *>( MemReallocSafe(link,  str->StringList, list_len * sizeof( *str->StringList ) ) );
            ConstructStringIDNamesBlock(link,  str );
        }
    }
}

namespace {

// a character (a 16-bit one in the 32-bit form: only its low byte takes part), upper-cased
inline int upper_byte( unsigned value )
{
    return std::toupper( static_cast<unsigned char>( value ) );
}

int genericCompare( const char *name1, uint_16 len1, const char *name2, uint_16 len2, bool use_unicode )
{
    const uint_16   min_chars = std::min( len1, len2 );

    if( !use_unicode ) {
        for( int char_num = 0; char_num < min_chars; char_num++ ) {
            const int ch1 = upper_byte( static_cast<unsigned char>( name1[char_num] ) );
            const int ch2 = upper_byte( static_cast<unsigned char>( name2[char_num] ) );
            if( ch1 < ch2 ) {
                return -1;
            } else if( ch1 > ch2 ) {
                return 1;
            }
        }
    } else {
        const uint_16 *char2u = reinterpret_cast<const uint_16 *>( name2 );
        for( int char_num = 0; char_num < min_chars; char_num++ ) {
            const int ch1 = upper_byte( static_cast<unsigned char>( name1[char_num] ) );
            const int ch2 = upper_byte( char2u[char_num] );
            if( ch1 < ch2 ) {
                return -1;
            } else if( ch1 > ch2 ) {
                return 1;
            }
        }
    }
    /*
     * Longer names with the same prefix are greater
     */
    if( len1 < len2 ) {
        return -1;
    } else if( len1 > len2 ) {
        return 1;
    } else {
        return 0;
    }
}

// bsearch's comparators: the key is the name looked for, an element a string of the block (16-bit form)
int compareStrings16( const void *key, const void *item )
{
    const WResIDName     *name = static_cast<const WResIDName *>( key );
    const StringItem16   *str = reinterpret_cast<const StringItem16 *>( static_cast<const name_ptr *>( item )->u.str );
    return genericCompare( name->Name, name->NumChars, str->Name, str->NumChars, false );
}

// ... and the 32-bit form
int compareStrings32( const void *key, const void *item )
{
    const WResIDName     *name = static_cast<const WResIDName *>( key );
    const StringItem32   *str = reinterpret_cast<const StringItem32 *>( static_cast<const name_ptr *>( item )->u.str );
    return genericCompare( name->Name, name->NumChars, str->Name, str->NumChars, true );
}

} // namespace

int CompareStringItems32( const StringItem32 *item1, const StringItem32 *item2 )
{
    const uint_16   *ptr1 = reinterpret_cast<const uint_16 *>( item1->Name );
    const uint_16   *ptr2 = reinterpret_cast<const uint_16 *>( item2->Name );
    const uint_16   min_chars = std::min( item1->NumChars, item2->NumChars );

    for( int char_num = 0; char_num < min_chars; char_num++ ) {
        const int ch1 = upper_byte( ptr1[char_num] );
        const int ch2 = upper_byte( ptr2[char_num] );
        if( ch1 < ch2 ) {
            return -1;
        } else if( ch1 > ch2 ) {
            return 1;
        }
    }
    /*
     * Longer names with the same prefix are greater
     */
    if( item1->NumChars < item2->NumChars ) {
        return -1;
    } else if( item1->NumChars > item2->NumChars ) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * if the return code is -1 the name was not found, otherwise it is the
 * number of bytes into the StringBlock at which to find the name.
 */
int_32 StringBlockFind( StringsBlock *str, WResIDName *name_id )
{
    const void *location = std::bsearch( name_id, str->StringList, str->StringListLen, sizeof( *str->StringList ),
                                         str->UseUnicode ? compareStrings32 : compareStrings16 );

    if( location == nullptr ) {
        return -1;
    } else {
        return static_cast<int_32>( static_cast<const name_ptr *>( location )->u.str - str->StringBlock );
    }
}
#endif
