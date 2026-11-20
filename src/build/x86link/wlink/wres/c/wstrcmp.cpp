/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2016-2021 The Open Watcom Contributors. All Rights Reserved.
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

#include <stddef.h>
#include <ctype.h>
#include "bool.h"
#include "wstrcmp.h"


namespace {

/* a character folded to lower case when it is an ASCII letter */
struct FoldedChar {
    char    ch;
    bool    alpha;
};

FoldedChar foldChar( char ch )
{
    if( ch >= 'A' && ch <= 'Z' ) {
        return { static_cast<char>( ch + ( 'a' - 'A' ) ), true };
    }
    return { ch, ch >= 'a' && ch <= 'z' };
}

} // namespace

int WresStrnicmp( const void *p1, const void *p2, size_t len )
/*************************************************************
 * comparision is handled specialy
 * if compare two alphabet characters then compare lower cased values
 * if compare non-alphabet character with alphabet character then
 * original values are used for comparision
 */
{
    const char  *s1 = static_cast<const char *>( p1 );
    const char  *s2 = static_cast<const char *>( p2 );

    for( size_t i = 0; i < len; i++ ) {
        FoldedChar  c1 = foldChar( s1[i] );
        FoldedChar  c2 = foldChar( s2[i] );

        if( c1.ch != c2.ch ) {
            if( c1.alpha != c2.alpha ) {
                if( c1.alpha ) {
                    c1.ch = s1[i];
                } else {
                    c2.ch = s2[i];
                }
            }
            return( ( c1.ch < c2.ch ) ? -1 : 1 );
        }
        if( c1.ch == 0 ) {
            break;      /* equal to 0, end of string */
        }
    }
    return( 0 );
}
