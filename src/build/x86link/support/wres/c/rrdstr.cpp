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
#include <string>
#include "layer0.h"
#include "varstr.h"
#include "read.h"
#include "reserr.h"
#include "wresrtns.h"


namespace {

/* hand the collected text to the caller in a block wres allocated (none when the read failed) */
char *finishString(Link& link,  const std::string &text, bool error, size_t *len )
{
    auto *str = static_cast<char *>( WRESALLOC(link,  text.size() + 1 ) );

    if( str == nullptr ) {
        WRES_ERROR( WRS_MALLOC_FAILED );
        return( nullptr );
    }
    std::memcpy( str, text.data(), text.size() );
    str[text.size()] = '\0';
    if( len != nullptr ) {
        *len = text.size();
    }
    if( error ) {
        WRESFREE( str );
        str = nullptr;
    }
    return( str );
}

} // namespace

char *ResReadString(Link& link,  FILE *fp, size_t *len )
/******************************************/
{
    std::string text;
    bool        error = false;
    uint_8      nextchar = ResReadUint8( &error, fp );

    while( !error && nextchar != '\0' ) {
        text.push_back( static_cast<char>( nextchar ) );
        nextchar = ResReadUint8( &error, fp );
    }
    return( finishString(link,  text, error, len ) );
}

char *ResRead32String(Link& link,  FILE *fp, size_t *len )
/********************************************/
{
    std::string text;
    bool        error = false;
    uint_16     nextchar = ResReadUint16( &error, fp );

    while( !error && nextchar != 0x0000 ) {
        text.push_back( UNI2ASCII( nextchar ) );
        nextchar = ResReadUint16( &error, fp );
    }
    return( finishString(link,  text, error, len ) );
}
