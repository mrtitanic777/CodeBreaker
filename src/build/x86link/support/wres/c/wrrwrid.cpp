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

#include <cstring>
#include "layer0.h"
#include "read.h"
#include "reserr.h"
#include "util.h"
#include "wresrtns.h"


void *ResReadWResID( FILE *fp, unsigned offs, uint_16 ver )
/*********************************************************/
{
    bool        error = false;
    size_t      numchars = 0;
    uint_8      isname = ResReadUint8( &error, fp );
    char        *ptr;

    if( error )
        return( nullptr );
    if( isname ) {
        isname = true;  /* normalize value to boolean type */
        if( ver < 3 ) {
            numchars = ResReadUint8( &error, fp );
        } else {
            numchars = ResReadUint16( &error, fp );
        }
        if( error )
            return( nullptr );
        ptr = static_cast<char *>( AllocWResIDName( offs + offsetof( WResID, ID.Name ), numchars ) );
    } else {
        ptr = static_cast<char *>( AllocWResIDNum( offs + offsetof( WResID, ID.Num ) ) );
    }
    if( ptr == nullptr ) {
        WRES_ERROR( WRS_MALLOC_FAILED );
        return( nullptr );
    }
    auto *id = reinterpret_cast<WResID *>( ptr + offs );

    id->IsName = isname;
    if( isname ) {
        id->ID.Name.NumChars = static_cast<uint_16>( numchars );
        const size_t numread = WRESREAD( fp, id->ID.Name.Name, numchars );

        if( numread != numchars ) {
            error = WRES_ERROR( WRESIOERR( fp, numread ) ? WRS_READ_FAILED : WRS_READ_INCOMPLETE );
        }
    } else {
        id->ID.Num = ResReadUint16( &error, fp );
    }
    if( error ) {
        WRESFREE( ptr );
        return( nullptr );
    }
    return( ptr );
}

WResID *WResReadWResID( FILE *fp )
/********************************/
{
    return( static_cast<WResID *>( ResReadWResID( fp, 0, WRESVERSION ) ) );
}
