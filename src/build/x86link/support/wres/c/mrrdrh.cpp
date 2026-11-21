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

#include "layer0.h"
#include "read.h"
#include "reserr.h"
#include "wresrtns.h"


namespace {

/* the Win32 form of the header: DWORD aligned, 32-bit sizes */
bool readHeader32(Link& link,  MResResourceHeader *msheader, FILE *fp )
{
    bool    error;

    error = ResReadPadDWord(link,  fp );
    if( !error ) {
        msheader->Size = ResReadUint32( &error, fp );
    }
    if( !error ) {
        msheader->HdrSize = ResReadUint32( &error, fp );
    }
    if( !error ) {
        msheader->Type = ResRead32NameOrOrdinal(link,  fp );
        error = ( msheader->Type == nullptr );
    }
    if( !error ) {
        msheader->Name = ResRead32NameOrOrdinal(link,  fp );
        error = ( msheader->Name == nullptr );
    }
    if( !error ) {
        error = ResReadPadDWord(link,  fp );
    }
    if( !error ) {
        msheader->DataVersion = ResReadUint32( &error, fp );
    }
    if( !error ) {
        msheader->MemoryFlags = ResReadUint16( &error, fp );
    }
    if( !error ) {
        msheader->LanguageId = ResReadUint16( &error, fp );
    }
    if( !error ) {
        msheader->Version = ResReadUint32( &error, fp );
    }
    if( !error ) {
        msheader->Characteristics = ResReadUint32( &error, fp );
    }
    return( error );
}

/* the Win16 form of the header */
bool readHeader16(Link& link,  MResResourceHeader *msheader, FILE *fp )
{
    bool    error;

    msheader->Type = ResReadNameOrOrdinal(link,  fp );
    error = ( msheader->Type == nullptr );
    if( !error ) {
        msheader->Name = ResReadNameOrOrdinal(link,  fp );
        error = ( msheader->Name == nullptr );
    }
    if( !error ) {
        msheader->MemoryFlags = ResReadUint16( &error, fp );
    }
    if( !error ) {
        msheader->Size = ResReadUint32( &error, fp );
    }
    return( error );
}

} // namespace

MResResourceHeader *MResReadResourceHeader(Link& link,  FILE *fp, bool iswin32 )
/******************************************************************/
{
    auto *msheader = static_cast<MResResourceHeader *>( WRESALLOC(link,  sizeof( MResResourceHeader ) ) );

    if( msheader == nullptr ) {
        WRES_ERROR( WRS_MALLOC_FAILED );
        return( nullptr );
    }
    const bool error = iswin32 ? readHeader32(link,  msheader, fp ) : readHeader16(link,  msheader, fp );
    if( error ) {
        WRESFREE( msheader );
        msheader = nullptr;
    }
    return( msheader );
}
