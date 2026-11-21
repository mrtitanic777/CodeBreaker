/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2025-2026 The Open Watcom Contributors. All Rights Reserved.
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
#include "wresrtns.h"


namespace {

/* a name or ordinal whose first unit (flags) is read already; 'wide' units are the Win32 form */
ResNameOrOrdinal *finishNameOrOrdinal( FILE *fp, bool wide, unsigned flags )
{
    const unsigned  ordinal_flag = wide ? 0xffffu : 0xffu;
    bool            error = false;
    uint_16         ord = 0;
    char            *restofstr = nullptr;
    size_t          stringlen = 0;

    /* read the rest of the Name or Ordinal */
    if( flags == ordinal_flag ) {
        ord = ResReadUint16( &error, fp );
        if( error ) {
            return( nullptr );
        }
    } else if( flags != 0 ) {
        restofstr = wide ? ResRead32String( fp, &stringlen ) : ResReadString( fp, &stringlen );
        if( restofstr == nullptr ) {
            return( nullptr );
        }
        stringlen++; /* for the '\0' */
    }
    /* allocate space for the new Name or Ordinal */
    auto *name = static_cast<ResNameOrOrdinal *>( WRESALLOC( sizeof( ResNameOrOrdinal ) + stringlen ) );

    if( name == nullptr ) {
        WRES_ERROR( WRS_MALLOC_FAILED );
    } else if( flags == ordinal_flag ) {
        /* copy the new Name or Ordinal into the correct place */
        name->ord.fFlag = 0xff;
        name->ord.wOrdinalID = ord;
    } else if( flags == 0 ) {
        name->name[0] = '\0';
    } else {
        name->name[0] = wide ? UNI2ASCII( flags ) : static_cast<char>( flags );
        std::memcpy( name->name + 1, restofstr, stringlen );
    }
    if( restofstr != nullptr ) {
        WRESFREE( restofstr );
    }
    return( name );
}

} // namespace

ResNameOrOrdinal *ResReadNameOrOrdinal( FILE *fp )
/************************************************/
{
    bool            error = false;
    const uint_8    flags = ResReadUint8( &error, fp );

    if( error )
        return( nullptr );
    return( finishNameOrOrdinal( fp, false, flags ) );
}

ResNameOrOrdinal *ResRead32NameOrOrdinal( FILE *fp )
/**************************************************/
{
    bool            error = false;
    const uint_16   flags = ResReadUint16( &error, fp );

    if( error )
        return( nullptr );
    return( finishNameOrOrdinal( fp, true, flags ) );
}
