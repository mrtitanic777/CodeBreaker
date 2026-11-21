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
* Description:  linker spill file i/o  routines
*
****************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <array>
#include <cstddef>

#include "build/x86link/link/linkstd.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/driver/ideentry.h"
#include "build/x86link/base/spillio.h"
#include "build/x86link/link/run_state.hpp"


namespace {

// what the spill file keeps for one run of the linker
struct SpillState {
    virt_mem_size   filesize = 0;
    FILE            *fp = nullptr;
};

SpillState &spill_state()
{
    return cb_wlink::RunState::part<SpillState>();
}

void tmp_IOerror(Link& link,  const char *str )
{
    if( str == nullptr )
        str = strerror( errno );
    LnkMsg(link,  ERR+MSG_IO_PROBLEM, "12", "Spill temporary file", str );
}

int tmp_seek(Link& link,  SpillState &st, long offset )
{
    int     rc;

    rc = fseek( st.fp, offset, SEEK_SET );
    if( rc ) {
        tmp_IOerror(link,  nullptr );
    }
    return( rc );
}

size_t tmp_read(Link& link,  SpillState &st, void *buffer, size_t len )
{
    size_t  rc;

    if( len == 0 )
        return( 0 );

    CheckBreak(link);
    rc = fread( buffer, 1, len, st.fp );
    if( rc != len ) {
        if( ferror( st.fp ) ) {
            tmp_IOerror(link,  nullptr );
        }
    }
    return( rc );
}

size_t tmp_write(Link& link,  SpillState &st, const void *buffer, size_t len )
{
    size_t  rc;

    if( len == 0 )
        return( 0 );
    CheckBreak(link);
    rc = fwrite( buffer, 1, len, st.fp );
    if( rc != len ) {
        if( ferror( st.fp ) ) {
            tmp_IOerror(link,  nullptr );
        } else {
            std::array<char, RESOURCE_MAX_SIZE> rc_buff;

            Msg_Get( MSG_IOERRLIST_7, rc_buff.data() );
            tmp_IOerror(link,  rc_buff.data() );
        }
    }
    return( rc );
}

void tmp_write_nulls(Link& link,  SpillState &st, size_t len )
{
    static constexpr std::array<unsigned, 16> NullArray{};

    for( ; len > sizeof( NullArray ); len -= sizeof( NullArray ) ) {
        tmp_write(link,  st, NullArray.data(), sizeof( NullArray ) );
    }
    if( len > 0 ) {
        tmp_write(link,  st, NullArray.data(), len );
    }
}

} // namespace

void InitSpillFile( void )
/************************/
{
    SpillState &st = spill_state();

    st.fp = nullptr;
    st.filesize = 0;
    SetBreak();
}

virt_mem_size SpillAlloc(Link& link,  virt_mem_size amt )
/*******************************************/
{
    SpillState              &st = spill_state();
    virt_mem_size           stg;

    if( st.fp == nullptr ) {
        st.fp = tmpfile();
        if( st.fp == nullptr ) {
            LnkMsg(link,  FTL+MSG_CANT_OPEN_SPILL, nullptr );
        }
        LnkMsg(link,  INF+MSG_USING_SPILL, nullptr );
    }
    /* round up storage start to a disk sector boundry -- assumed power of 2 */
    st.filesize = __ROUND_UP_SIZE_SECTOR( st.filesize );
    stg = st.filesize;
    st.filesize += amt;
    return( stg );
}

void SpillNull(Link& link,  virt_mem_size base, size_t off, size_t size )
/***********************************************************/
{
    SpillState &st = spill_state();

    tmp_seek(link,  st, base + static_cast<unsigned long>( off ) - 1 );
    tmp_write_nulls(link,  st, size );
}

void SpillWrite(Link& link,  virt_mem_size base, size_t off, const void *mem, size_t size )
/*****************************************************************************/
{
    SpillState &st = spill_state();

    tmp_seek(link,  st, base + static_cast<unsigned long>( off ) - 1 );
    tmp_write(link,  st, mem, size );
}

void SpillRead(Link& link,  virt_mem_size base, size_t off, void *mem, size_t size )
/**********************************************************************/
{
    SpillState &st = spill_state();

    tmp_seek(link,  st, base + static_cast<unsigned long>( off ) - 1 );
    tmp_read(link,  st, mem, size );
}

void CloseSpillFile( void )
/*************************/
/*  Close temporary file.  */
{
    SpillState &st = spill_state();

    if( st.fp != nullptr ) {
        RestoreBreak();
        fclose( st.fp );
        st.fp = nullptr;
    }
}
