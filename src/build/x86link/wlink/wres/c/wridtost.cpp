/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2026      The Open Watcom Contributors. All Rights Reserved.
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

#include <array>
#include <cstring>
#include "layer0.h"
#include "util.h"
#include "reserr.h"
#include "wresrtns.h"


namespace {

constexpr size_t UINT16_MAXDIGITS = 5;
constexpr size_t UINT32_MAXDIGITS = 9;

/* the decimal digits of num at buf; returns the end of them (not NUL terminated) */
char *u32tostr( char *buf, uint_32 num )
{
    std::array<char, 10>    digits;
    size_t                  count = 0;

    do {
        digits[count++] = static_cast<char>( '0' + num % 10 );
        num /= 10;
    } while( num != 0 );
    while( count > 0 ) {
        *buf++ = digits[--count];
    }
    return( buf );
}

/* the name or the decimal number of an ID as an allocated string */
template <typename Id>
char *idToStr( const Id *id, size_t maxdigits )
{
    char    *str;

    if( id->IsName ) {
        /* alloc space for the string and a \0 char at the end */
        str = static_cast<char *>( WRESALLOC( id->ID.Name.NumChars + 1 ) );
        if( str == nullptr ) {
            WRES_ERROR( WRS_MALLOC_FAILED );
        } else {
            /* copy the string */
            std::memcpy( str, id->ID.Name.Name, id->ID.Name.NumChars );
            str[id->ID.Name.NumChars] = '\0';
        }
    } else {
        str = static_cast<char *>( WRESALLOC( maxdigits + 1 ) );
        if( str == nullptr ) {
            WRES_ERROR( WRS_MALLOC_FAILED );
        } else {
            *u32tostr( str, id->ID.Num ) = '\0';
        }
    }
    return( str );
}

} // namespace

char *WResIDToStr( const WResID *id )
/************************************
 * return the value in an ID if it is a string, NULL otherwise
 */
{
    return( idToStr( id, UINT16_MAXDIGITS ) );
}

char *WResHelpIDToStr( const WResHelpID *help_id )
/*************************************************
 * return the value in a Help ID if it is a string, NULL otherwise
 */
{
    return( idToStr( help_id, UINT32_MAXDIGITS ) );
}
