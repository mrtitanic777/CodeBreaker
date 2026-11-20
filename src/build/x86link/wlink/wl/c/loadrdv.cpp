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
* Description:  Routines for creation of RDOS device driver files
*
****************************************************************************/

#include <string.h>
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include <exerdos.h>
#include "pcobj.h"
#include "newmem.h"
#include "reloc.h"
#include "virtmem.h"
#include "fileio.h"
#include "overlays.h"
#include "loadfile.h"
#include "objcalc.h"
#include "ring.h"
#include "dbgall.h"
#include "loadrdv.h"
#include "run_state.hpp"


#ifdef _RDOS

static constexpr unsigned_32 MB_BASE = 0x120000;

namespace {

// The sizes of the parts written so far, fresh for each link.
struct RdosState {
    unsigned_32     CodeSize = 0;
    unsigned_32     DataSize = 0;
    unsigned_32     HeaderSize = 0;
};

RdosState& state()
{
    return cb_wlink::RunState::part<RdosState>();
}

}  // namespace

struct mb_header {
    unsigned_32 mb_magic;
    unsigned_32 mb_flags;
    unsigned_32 mb_checksum;
    unsigned_32 mb_header_addr;
    unsigned_32 mb_load_addr;
    unsigned_32 mb_load_end_addr;
    unsigned_32 mb_bss_end_addr;
    unsigned_32 mb_entry_addr;
};

static void WriteBinData( void )
/*******************************
 * copy code from extra memory to loadfile
 */
{
    group_entry         *group;

    DEBUG(( DBG_BASE, "Writing data" ));
    OrderGroups( CompareDosSegments );

    wlink().Root->outfile->file_loc = wlink().Root->u.file_loc;
    wlink().Root->sect_addr = wlink().Groups->addr;

    /* write groups and relocations */
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        wlink().CurrSect = group->section;  // needed for WriteInfo.
        if( group->totalsize ) {
            WriteGroup( group );
            state().CodeSize += group->totalsize;
        }
    }
}

static void WriteRDOSCode( void )
/********************************
 * copy driver code from extra memory
 * to loadfile
 */
{
    group_entry         *group;
    seg_leader         *leader;
    SEGDATA             *piece;
    bool                iscode;

    DEBUG(( DBG_BASE, "Writing code" ));
    OrderGroups( CompareDosSegments );

    wlink().Root->outfile->file_loc = wlink().Root->u.file_loc;
    wlink().Root->sect_addr = wlink().Groups->addr;
    leader = nullptr;

    /* write groups and relocations */
    iscode = false;
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( leader != group->leaders ) {
            iscode = false;
            leader = group->leaders;
            if( leader != nullptr
              && leader->size ) {
                piece = leader->pieces;
                if( piece != nullptr ) {
                    if( piece->iscode
                      && ( leader->seg_addr.seg == wlink().FmtData.u.rdos.code_seg ) ) {
                        iscode = true;
                    }
                }
            }
        }
        wlink().CurrSect = group->section;  // needed for WriteInfo.
        if( iscode ) {
            wlink().CurrSect->u.file_loc = state().HeaderSize + state().CodeSize;
            WriteGroup( group );
            if( group->totalsize > group->size ) {
                PadLoad( group->totalsize - group->size );
            }
            state().CodeSize += group->totalsize;
        }
    }
}

static void WriteRDOSData( void )
/********************************
 * copy driver data from extra memory
 * to loadfile
 */
{
    group_entry         *group;
    seg_leader         *leader;
    SEGDATA             *piece;
    bool                isdata;

    DEBUG(( DBG_BASE, "Writing data" ));

    /* write groups and relocations */
    leader = nullptr;
    isdata = false;
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( leader != group->leaders ) {
            isdata = false;
            leader = group->leaders;
            if( leader != nullptr
              && leader->size ) {
                piece = leader->pieces;
                if( piece != nullptr ) {
                    if( ( piece->isidata
                      || piece->isuninit )
                      && ( leader->seg_addr.seg == wlink().FmtData.u.rdos.data_seg ) ) {
                        isdata = true;
                    }
                }
            }
        }
        wlink().CurrSect = group->section;  // needed for WriteInfo.
        if( isdata ) {
            wlink().CurrSect->u.file_loc = state().HeaderSize + state().CodeSize + state().DataSize;
            if( wlink().StackSegPtr != nullptr ) {
                if( group->totalsize - group->size < wlink().StackSize ) {
                    wlink().StackSize = group->totalsize - group->size;
                    group->totalsize = group->size;
                } else {
                    group->totalsize -= wlink().StackSize;
                }
            }
            WriteGroup( group );
            if( group->totalsize > group->size ) {
                PadLoad( group->totalsize - group->size );
            }
            state().DataSize += group->totalsize;
        }
    }
}

void GetRdosSegs( void )
/***********************
 * resolve RDOS code & data segments
 */
{
    group_entry         *group;
    seg_leader         *leader;
    SEGDATA             *piece;

    leader = nullptr;
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( leader != group->leaders ) {
            leader = group->leaders;
            if( leader != nullptr
              && leader->size ) {
                piece = leader->pieces;
                if( piece != nullptr ) {
                    if( piece->iscode )
                        wlink().FmtData.u.rdos.code_seg = leader->seg_addr.seg;
                    if( piece->isidata
                      || piece->isuninit ) {
                        wlink().FmtData.u.rdos.data_seg = leader->seg_addr.seg;
                    }
                }
            }
        }
    }
}

static void WriteHeader16( void )
/********************************
 * write 16-bit device header
 */
{
    rdos_dev16_header   exe_head;

    SeekLoad( 0 );
    _HostU16toTarg( RDOS_SIGNATURE_16, exe_head.signature );
    _HostU16toTarg( wlink().StartInfo.addr.off, exe_head.IP );
    _HostU16toTarg( state().CodeSize, exe_head.code_size );
    _HostU16toTarg( wlink().FmtData.u.rdos.code_sel, exe_head.code_sel );
    _HostU16toTarg( state().DataSize, exe_head.data_size );
    _HostU16toTarg( wlink().FmtData.u.rdos.data_sel, exe_head.data_sel );
    WriteLoad( &exe_head, sizeof( rdos_dev16_header ) );
}

static void WriteHeader32( void )
/********************************
 * write 32-bit device header
 */
{
    rdos_dev32_header   exe_head;

    SeekLoad( 0 );
    _HostU16toTarg( RDOS_SIGNATURE_32, exe_head.signature );
    _HostU32toTarg( wlink().StartInfo.addr.off, exe_head.EIP );
    _HostU32toTarg( state().CodeSize, exe_head.code_size );
    _HostU16toTarg( wlink().FmtData.u.rdos.code_sel, exe_head.code_sel );
    _HostU32toTarg( state().DataSize, exe_head.data_size );
    _HostU16toTarg( wlink().FmtData.u.rdos.data_sel, exe_head.data_sel );
    WriteLoad( &exe_head, sizeof( rdos_dev32_header ) );
}

static void WriteMbootHeader( void )
/***********************************
 * write multiboot header
 */
{
    mb_header          mb_head;
    unsigned_32        temp32;
    unsigned_32        linear;

    SeekLoad( 0 );
    _HostU32toTarg( 0x1BADB002, mb_head.mb_magic );
    _HostU32toTarg( 0x00010003, mb_head.mb_flags );
    _HostU32toTarg( 0xE4514FFB, mb_head.mb_checksum );
    _HostU32toTarg( MB_BASE, mb_head.mb_header_addr );
    _HostU32toTarg( MB_BASE, mb_head.mb_load_addr );
    linear = MB_BASE + state().CodeSize + sizeof( mb_header );
    _HostU32toTarg( linear, mb_head.mb_load_end_addr );
    _HostU32toTarg( linear, mb_head.mb_bss_end_addr );
    temp32 = MB_BASE + wlink().StartInfo.addr.off + sizeof( mb_header );
    _HostU32toTarg( temp32, mb_head.mb_entry_addr );
    WriteLoad( &mb_head, sizeof( mb_header ) );
}

static size_t getHeaderSize( void )
/**********************************
 * get header size for appropriate
 * RDOS format
 */
{
    size_t  size;

    if( wlink().FmtData.u.rdos.mboot ) {
        size = sizeof( mb_header );
    } else if( wlink().FmtData.u.rdos.driver ) {
        if( wlink().LinkState & LS_FMT_SEEN_32BIT ) {
            size = sizeof( rdos_dev32_header );
        } else {
            size = sizeof( rdos_dev16_header );
        }
    } else {
        size = 0;
    }
    return( size );
}

static void writeHeader( void )
/**********************************
 * if required then write header for
 * appropriate RDOS format
 */
{
    if( wlink().FmtData.u.rdos.mboot ) {
        WriteMbootHeader();
    } else if( wlink().FmtData.u.rdos.driver ) {
        if( wlink().LinkState & LS_FMT_SEEN_32BIT ) {
            state().CodeSize += 0x10;   // this is a fix to make offsets into data segment correct
            WriteHeader32();
        } else {
            WriteHeader16();
        }
    } else {
        /* nothing to write */
    }
}

void FiniRdosLoadFile( void )
/****************************
 * finish writing of load file
 */
{
    if( (wlink().FmtData.type & MK_RDOS_16)
      && (wlink().LinkState & LS_FMT_SEEN_32BIT) ) {
        LnkMsg( WRN+LOC+MSG_CANNOT_HAVE_16_AND_32, nullptr );
    }
    state().HeaderSize = getHeaderSize();
    SeekLoad( state().HeaderSize );
    wlink().Root->u.file_loc = state().HeaderSize;
    if( wlink().FmtData.u.rdos.driver ) {
        WriteRDOSCode();
        WriteRDOSData();
    } else {
        WriteBinData();
    }
    DBIWrite();
    writeHeader();
}

#endif
