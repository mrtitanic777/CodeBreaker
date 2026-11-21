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
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include <exerdos.h>
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/formats/rdos.h"
#include "build/x86link/link/run_state.hpp"


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

static void WriteBinData(Link& link)
/*******************************
 * copy code from extra memory to loadfile
 */
{
    group_entry         *group;

    DEBUG(( DBG_BASE, "Writing data" ));
    OrderGroups(link,  bind_link( link, CompareDosSegments ) );

    link.Root->outfile->file_loc = link.Root->u.file_loc;
    link.Root->sect_addr = link.Groups->addr;

    /* write groups and relocations */
    for( group = link.Groups; group != nullptr; group = group->next ) {
        link.CurrSect = group->section;  // needed for WriteInfo.
        if( group->totalsize ) {
            WriteGroup(link,  group );
            state().CodeSize += group->totalsize;
        }
    }
}

static void WriteRDOSCode(Link& link)
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
    OrderGroups(link,  bind_link( link, CompareDosSegments ) );

    link.Root->outfile->file_loc = link.Root->u.file_loc;
    link.Root->sect_addr = link.Groups->addr;
    leader = nullptr;

    /* write groups and relocations */
    iscode = false;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( leader != group->leaders ) {
            iscode = false;
            leader = group->leaders;
            if( leader != nullptr
              && leader->size ) {
                piece = leader->pieces;
                if( piece != nullptr ) {
                    if( piece->iscode
                      && ( leader->seg_addr.seg == link.FmtData.u.rdos.code_seg ) ) {
                        iscode = true;
                    }
                }
            }
        }
        link.CurrSect = group->section;  // needed for WriteInfo.
        if( iscode ) {
            link.CurrSect->u.file_loc = state().HeaderSize + state().CodeSize;
            WriteGroup(link,  group );
            if( group->totalsize > group->size ) {
                PadLoad(link,  group->totalsize - group->size );
            }
            state().CodeSize += group->totalsize;
        }
    }
}

static void WriteRDOSData(Link& link)
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
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( leader != group->leaders ) {
            isdata = false;
            leader = group->leaders;
            if( leader != nullptr
              && leader->size ) {
                piece = leader->pieces;
                if( piece != nullptr ) {
                    if( ( piece->isidata
                      || piece->isuninit )
                      && ( leader->seg_addr.seg == link.FmtData.u.rdos.data_seg ) ) {
                        isdata = true;
                    }
                }
            }
        }
        link.CurrSect = group->section;  // needed for WriteInfo.
        if( isdata ) {
            link.CurrSect->u.file_loc = state().HeaderSize + state().CodeSize + state().DataSize;
            if( link.StackSegPtr != nullptr ) {
                if( group->totalsize - group->size < link.StackSize ) {
                    link.StackSize = group->totalsize - group->size;
                    group->totalsize = group->size;
                } else {
                    group->totalsize -= link.StackSize;
                }
            }
            WriteGroup(link,  group );
            if( group->totalsize > group->size ) {
                PadLoad(link,  group->totalsize - group->size );
            }
            state().DataSize += group->totalsize;
        }
    }
}

void GetRdosSegs(Link& link)
/***********************
 * resolve RDOS code & data segments
 */
{
    group_entry         *group;
    seg_leader         *leader;
    SEGDATA             *piece;

    leader = nullptr;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( leader != group->leaders ) {
            leader = group->leaders;
            if( leader != nullptr
              && leader->size ) {
                piece = leader->pieces;
                if( piece != nullptr ) {
                    if( piece->iscode )
                        link.FmtData.u.rdos.code_seg = leader->seg_addr.seg;
                    if( piece->isidata
                      || piece->isuninit ) {
                        link.FmtData.u.rdos.data_seg = leader->seg_addr.seg;
                    }
                }
            }
        }
    }
}

static void WriteHeader16(Link& link)
/********************************
 * write 16-bit device header
 */
{
    rdos_dev16_header   exe_head;

    SeekLoad(link,  0 );
    _HostU16toTarg( RDOS_SIGNATURE_16, exe_head.signature );
    _HostU16toTarg( link.StartInfo.addr.off, exe_head.IP );
    _HostU16toTarg( state().CodeSize, exe_head.code_size );
    _HostU16toTarg( link.FmtData.u.rdos.code_sel, exe_head.code_sel );
    _HostU16toTarg( state().DataSize, exe_head.data_size );
    _HostU16toTarg( link.FmtData.u.rdos.data_sel, exe_head.data_sel );
    WriteLoad(link,  &exe_head, sizeof( rdos_dev16_header ) );
}

static void WriteHeader32(Link& link)
/********************************
 * write 32-bit device header
 */
{
    rdos_dev32_header   exe_head;

    SeekLoad(link,  0 );
    _HostU16toTarg( RDOS_SIGNATURE_32, exe_head.signature );
    _HostU32toTarg( link.StartInfo.addr.off, exe_head.EIP );
    _HostU32toTarg( state().CodeSize, exe_head.code_size );
    _HostU16toTarg( link.FmtData.u.rdos.code_sel, exe_head.code_sel );
    _HostU32toTarg( state().DataSize, exe_head.data_size );
    _HostU16toTarg( link.FmtData.u.rdos.data_sel, exe_head.data_sel );
    WriteLoad(link,  &exe_head, sizeof( rdos_dev32_header ) );
}

static void WriteMbootHeader(Link& link)
/***********************************
 * write multiboot header
 */
{
    mb_header          mb_head;
    unsigned_32        temp32;
    unsigned_32        linear;

    SeekLoad(link,  0 );
    _HostU32toTarg( 0x1BADB002, mb_head.mb_magic );
    _HostU32toTarg( 0x00010003, mb_head.mb_flags );
    _HostU32toTarg( 0xE4514FFB, mb_head.mb_checksum );
    _HostU32toTarg( MB_BASE, mb_head.mb_header_addr );
    _HostU32toTarg( MB_BASE, mb_head.mb_load_addr );
    linear = MB_BASE + state().CodeSize + sizeof( mb_header );
    _HostU32toTarg( linear, mb_head.mb_load_end_addr );
    _HostU32toTarg( linear, mb_head.mb_bss_end_addr );
    temp32 = MB_BASE + link.StartInfo.addr.off + sizeof( mb_header );
    _HostU32toTarg( temp32, mb_head.mb_entry_addr );
    WriteLoad(link,  &mb_head, sizeof( mb_header ) );
}

static size_t getHeaderSize(Link& link)
/**********************************
 * get header size for appropriate
 * RDOS format
 */
{
    size_t  size;

    if( link.FmtData.u.rdos.mboot ) {
        size = sizeof( mb_header );
    } else if( link.FmtData.u.rdos.driver ) {
        if( link.LinkState & LS_FMT_SEEN_32BIT ) {
            size = sizeof( rdos_dev32_header );
        } else {
            size = sizeof( rdos_dev16_header );
        }
    } else {
        size = 0;
    }
    return( size );
}

static void writeHeader(Link& link)
/**********************************
 * if required then write header for
 * appropriate RDOS format
 */
{
    if( link.FmtData.u.rdos.mboot ) {
        WriteMbootHeader(link);
    } else if( link.FmtData.u.rdos.driver ) {
        if( link.LinkState & LS_FMT_SEEN_32BIT ) {
            state().CodeSize += 0x10;   // this is a fix to make offsets into data segment correct
            WriteHeader32(link);
        } else {
            WriteHeader16(link);
        }
    } else {
        /* nothing to write */
    }
}

void FiniRdosLoadFile(Link& link)
/****************************
 * finish writing of load file
 */
{
    if( (link.FmtData.type & MK_RDOS_16)
      && (link.LinkState & LS_FMT_SEEN_32BIT) ) {
        LnkMsg(link,  WRN+LOC+MSG_CANNOT_HAVE_16_AND_32, nullptr );
    }
    state().HeaderSize = getHeaderSize(link);
    SeekLoad(link,  state().HeaderSize );
    link.Root->u.file_loc = state().HeaderSize;
    if( link.FmtData.u.rdos.driver ) {
        WriteRDOSCode(link);
        WriteRDOSData(link);
    } else {
        WriteBinData(link);
    }
    DBIWrite(link);
    writeHeader(link);
}

#endif
