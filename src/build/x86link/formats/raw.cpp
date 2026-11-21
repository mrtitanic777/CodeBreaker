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
* Description:  Functions for creating binary and hex load files.
*
****************************************************************************/


#include <string.h>
#include <stdio.h>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/base/ring.h"
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/formats/raw.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/link/run_state.hpp"
#include <array>


#ifdef _RAW

typedef offset  hex_offset;

typedef segment hex_segment;

typedef struct {
    hex_offset      off;
    hex_segment     seg;
} hex_addr;

static bool WriteBinSegGroup( group_entry *group )
/*********************************************/
/* write the data for group to the loadfile */
/* returns true if the file should be repositioned */
{
    unsigned long       file_loc;
    section             *sect;
    bool                repos;
    outfilelist         *finfo;

    repos = false;
    if( group->size != 0
      || (group->leaders->class_->flags & CLASS_COPY) ) {
        sect = group->section;
        wlink().CurrSect = sect;
        finfo = sect->outfile;
        file_loc = GROUP_FILE_LOC( group );
        // Offset may make this go negative for NOEMIT classes or segments
        if( (long)file_loc >= 0 ) {
            if( file_loc > finfo->file_loc ) {
                PadLoad( file_loc - finfo->file_loc );
            } else if( file_loc != finfo->file_loc ) {
                SeekLoad( file_loc );
                repos = true;
            }
            DEBUG((DBG_LOADDOS, "group %a section %d to %l in %s",
                &group->addr, sect->ovlref, file_loc, finfo->fname ));
            file_loc += WriteGroupLoad( group, repos );
            if( file_loc > finfo->file_loc ) {
                finfo->file_loc = file_loc;
            }
        }
    }
    return( repos );
}


void BinOutput( void )
/********************/
{
    outfilelist         *fnode;
    group_entry         *group;
    bool                repos;
#if defined( _OS2 ) || defined( _QNX ) || defined( _ELF )
    unsigned_32         size;
    signed_32           diff;
#endif

#if defined( _OS2 ) || defined( _QNX ) || defined( _ELF )
    if( wlink().FmtData.type & (MK_PE | MK_QNX_FLAT | MK_OS2_FLAT | MK_WIN_VXD | MK_ELF) ) {
        wlink().CurrSect = wlink().Root;        // needed for WriteInfo.
        wlink().Root->sect_addr = wlink().Groups->addr;
        fnode = wlink().Root->outfile;
        fnode->file_loc = 0;
        wlink().Root->u.file_loc = wlink().Root->sect_addr.off - wlink().FmtData.output_offset;
        /* write groups */
        for( group = wlink().Groups; group != nullptr; group = group->next ) {
            if( group->leaders->class_->flags & CLASS_COPY ) {
                size = CalcGroupSize( group->leaders->class_->DupClass->segs->group );
            } else {
                size = CalcGroupSize( group );
            }
            if( size ) {
                diff = ( wlink().FmtData.base + group->addr.off + group->linear - wlink().FmtData.output_offset ) - PosLoad();
                if( diff < 0 ) {
                    LnkMsg( ERR + MSG_FIXED_LOC_BEFORE_CUR_LOC, "a", &(group->addr));
                } else if( diff > 0 ) {
                    PadLoad( static_cast<size_t>( diff ) );
                }
                WriteGroupLoad( group, false );
            }
        }
    } else {
#endif
        OrderGroups( CompareDosSegments );
        wlink().CurrSect = wlink().Root;        // needed for WriteInfo.
        wlink().Root->sect_addr = wlink().Groups->addr;
        fnode = wlink().Root->outfile;
        fnode->file_loc = 0;
        wlink().Root->u.file_loc = ( wlink().Root->sect_addr.seg << wlink().FmtData.SegShift ) + wlink().Root->sect_addr.off - wlink().FmtData.output_offset;
        /* write groups */
        for( group = wlink().Groups; group != nullptr; group = group->next ) {
            repos = WriteBinSegGroup( group );
            if( repos ) {
                SeekLoad( fnode->file_loc );
            }
        }
#if defined( _OS2 ) || defined( _QNX ) || defined( _ELF )
    }
#endif
    DBIWrite();
}

// The following routine generates Intel Hex records using data buffered in LineBuf.
// A full record is 16 bytes of data.  Partial records are allowed when needed
// because of gaps in the loadfile data.  Initially segmented addressing is assumed
// with a starting segment value of 0 (Original 64K Intel Hex records).  If data
// is encountered above 64K, the routine outputs a segment record, seamlessly
// switching to segmented mode.  Segmented mode can use either the standard 4 bit
// shift (default) or the shift defined in FmtData.SegShift, or if set, the
// value in FmtData.HexSegShift, which is uniquely used by this function.  The
// purpose for this is that 24 bit addressing can be fully handled using segmented
// addressing (rather than extended linear addressing) if SegShift is 8.
// If data is encountered above what Segmented addressing can handle (1 M for default)
// the routine outputs an extended linear address record, seamlessly switching
// to a full 32 bit address range.  This approach provides full backward compatibility
// for systems which cannot read the newer formats when creating files that don't
// need the extra range.

#define HEXLEN 16   // number of bytes of data in a full record
namespace {

// The hex writer's line buffer and address, fresh for each link.
struct HexState {
    hex_addr            next_addr {};
    bool                linear = false;
    std::array<char, HEXLEN> lineBuf {};
    size_t              buf_offset = 0;
};

HexState& hexState()
{
    return cb_wlink::RunState::part<HexState>();
}

}  // namespace

static void WriteHexLine( void )
/******************************/
{
    HexState         &hs = hexState();
    std::array<char, 2 * HEXLEN + 15> str_arr;
    char             *str_buf = str_arr.data();
    unsigned_16      checksum;
    unsigned_16      i;
    unsigned_16      seg_shift;
    unsigned_16      curr_offs;

    if( wlink().FmtData.output_hshift ) {
        seg_shift = wlink().FmtData.HexSegShift;
    } else {
        seg_shift = wlink().FmtData.SegShift;
    }
    if( ( hs.next_addr.off + hs.buf_offset >= ( 0x10000UL << seg_shift ) )
      && !hs.linear ) {
        hs.next_addr.seg = 0;
        hs.linear = true;
    }

    if( !hs.linear ) {
        if( hs.next_addr.off - ( hs.next_addr.seg << seg_shift ) + hs.buf_offset > 0x10000L ) { // See if we need to output a segment record
            hs.next_addr.seg = static_cast<hex_segment>( hs.next_addr.off >> seg_shift );
            checksum = 4 + (( hs.next_addr.seg >> 8 ) & 0xFF) + (hs.next_addr.seg & 0xFF);
            sprintf( str_buf, ":02000002%04x%02x\r\n", hs.next_addr.seg, (-checksum) & 0xFF );
            WriteLoad( str_buf, 17 );
        }
        curr_offs = static_cast<unsigned_16>( hs.next_addr.off - ( hs.next_addr.seg << seg_shift ) );
    } else {
        if( hs.next_addr.off - ( hs.next_addr.seg << 16 ) + hs.buf_offset > 0x10000L ) { // See if we need to output
            hs.next_addr.seg = static_cast<hex_segment>( hs.next_addr.off >> 16 );               //   an extended linear record
            checksum = 6 + (( hs.next_addr.seg >> 8 ) & 0xFF) + (hs.next_addr.seg & 0xFF);
            sprintf( str_buf, ":02000004%04x%02x\r\n", hs.next_addr.seg, (-checksum) & 0xFF );
            WriteLoad( str_buf, 17);
        }
        curr_offs = static_cast<unsigned_16>( hs.next_addr.off - ( hs.next_addr.seg << 16 ) );
    }

    sprintf( str_buf, ":%02x%04x00", (unsigned)hs.buf_offset, curr_offs );         // Intel Hex header
    checksum = hs.buf_offset + (( curr_offs >> 8 ) & 0xFF) + (curr_offs & 0xFF);   // Start checksum using above elements

    i = 0;
    for( ; hs.buf_offset > 0; --hs.buf_offset ) { // Do data bytes, leaving hs.buf_offset 0 at end
        sprintf( str_buf + 9 + 2 * i, "%02x", hs.lineBuf[i] );
        checksum += hs.lineBuf[i++];
    }
    sprintf( str_buf + 9 + 2 * i, "%02x\r\n", (-checksum) & 0xFF );
    WriteLoad( str_buf, 13 + 2 * i );
}

static bool WriteHexData( void *_sdata, void *_offs )
/***************************************************/
{
    HexState         &hs = hexState();
    segdata          *sdata = static_cast<segdata *>( _sdata );
    hex_offset       offs = *static_cast<hex_offset *>( _offs ) + sdata->a.delta;
    offset           len = sdata->length;
    offset           src_offs;
    offset           piece;

    if( !sdata->isuninit
      && !sdata->isdead
      && len > 0 ) {
        if( offs != hs.next_addr.off + hs.buf_offset ) {  // Must start new record if address not contiguous
            if( offs < hs.next_addr.off + hs.buf_offset ) {
                addr_type   addr;

                addr.off = offs;
                addr.seg = 0;
                LnkMsg( ERR + MSG_FIXED_LOC_BEFORE_CUR_LOC, "a", &addr );
            }
            if( hs.buf_offset > 0 ) {          // If partial record in buffer, flush
                WriteHexLine();
            }
            hs.next_addr.off = offs;
        }
        src_offs = 0;
        for( ; len > 0; len -= piece ) {    // Now lob out records
            piece = HEXLEN - hs.buf_offset;    // Handle partially filled buffer
            if( piece > len ) {             // Handle not enough to fill buffer
                piece = len;
            }
            ReadInfo( sdata->u1.vm_ptr + src_offs, hs.lineBuf.data() + hs.buf_offset, piece );
            hs.buf_offset += piece;
            if( hs.buf_offset == HEXLEN ) {
                WriteHexLine();             // Only write full buffers
                hs.next_addr.off += HEXLEN;    // hs.next_addr.off reflects start of line
            }                               // Partial records will be written later
            src_offs += piece;              //   if address is not contiguous
        }
    }
    return( false );
}

static bool DoHexLeader( void *_seg, void *_grp_offs )
/***************************************************/
{
    seg_leader      *seg = static_cast<seg_leader *>( _seg );

    if( EMIT_CLASS( seg->class_ )
      && EMIT_SEG( seg ) ) {
        hex_offset   offs = *static_cast<hex_offset *>( _grp_offs ) + SEG_GROUP_DELTA( seg );
        RingLookup( seg->pieces, WriteHexData, &offs );
    }
    return( false );
}

static bool DoHexDupLeader( void *seg, void *_grp_offs )
/******************************************************/
{
    hex_offset offs = *static_cast<hex_offset *>( _grp_offs ) + SEG_GROUP_DELTA( static_cast<seg_leader *>( seg ) );

    RingLookup( static_cast<seg_leader *>( seg )->pieces, WriteHexData, &offs );
    return( false );
}

static void WriteStart( void )
/****************************/
{
    char        str_buf[22];
    unsigned_16 checksum;

    if( wlink().StartInfo.addr.off > 0xffff ) {
        checksum = 9 + ((wlink().StartInfo.addr.off >> 24) & 0xFF) + ((wlink().StartInfo.addr.off >> 16) & 0xFF) + ((wlink().StartInfo.addr.off >> 8) & 0xFF) + (wlink().StartInfo.addr.off & 0xFF);
        sprintf( str_buf, ":04000005%08lx%02x\r\n", (unsigned long)wlink().StartInfo.addr.off, (-checksum) & 0xFF );
    } else {
        checksum = 7 + ((wlink().StartInfo.addr.seg >> 8) & 0xFF) + (wlink().StartInfo.addr.seg & 0xFF) + ((wlink().StartInfo.addr.off >> 8) & 0xFF) + (wlink().StartInfo.addr.off & 0xFF);
        sprintf( str_buf, ":04000003%04x%04x%02x\r\n", (unsigned)wlink().StartInfo.addr.seg, (unsigned)wlink().StartInfo.addr.off, (-checksum) & 0xFF );
    }
    WriteLoad( str_buf, 21 );
}

typedef struct {
    hex_offset      offs;
    group_entry     *last_group;  // used only for copy classes
} grpwriteinfo;

static bool WriteHexCopyGroups( void *_seg, void *_info )
/*******************************************************/
{
    // This is called by the outer level iteration looking for classes
    //  that have more than one group in them
    seg_leader *seg = static_cast<seg_leader *>( _seg );
    grpwriteinfo *info = static_cast<grpwriteinfo *>( _info );

    if( info->last_group != seg->group ) {   // Only interate new groups
        info->last_group = seg->group;
        // Check each initialized segment in group
        Ring2Lookup( seg->group->leaders, DoHexDupLeader, &info->offs );
        info->offs += seg->group->totalsize;
    }
    return( false );
}


void HexOutput( void )
/********************/
{
    HexState            &hs = hexState();
    outfilelist         *fnode;
    group_entry         *group;
    section             *sect;
#ifdef DEVBUILD
    outfilelist         *finfo;
#endif
#if defined( _OS2 ) || defined( _QNX ) || defined( _ELF )
    group_entry         *wrkgrp;
    unsigned_32         size;
#endif
    class_entry         *class_;
    grpwriteinfo        info;
//    unsigned long     file_loc;

    hs.next_addr.off = 0;      // Start at absolute linear address 0
    hs.next_addr.seg = 0;
    hs.linear        = false;  //       in segmented mode
    hs.buf_offset    = 0;

#if defined( _OS2 ) || defined( _QNX ) || defined( _ELF )
    if( wlink().FmtData.type & (MK_PE | MK_QNX_FLAT | MK_OS2_FLAT | MK_WIN_VXD | MK_ELF) ) {
        wlink().CurrSect = wlink().Root;    // needed for WriteInfo.
        wlink().Root->sect_addr = wlink().Groups->addr;
        fnode = wlink().Root->outfile;
        fnode->file_loc = 0;
        wlink().Root->u.file_loc = wlink().Root->sect_addr.off - wlink().FmtData.output_offset;
        /* write groups */
        for( group = wlink().Groups; group != nullptr; group = group->next ) {
            class_ = group->leaders->class_;
            if( class_->flags & CLASS_COPY ) {
                wrkgrp = class_->DupClass->segs->group;
            } else {
                wrkgrp = group;
            }
            size = CalcGroupSize( wrkgrp );
            if( size != 0 ) {
                info.offs = (group->addr.off + group->linear - wlink().FmtData.output_offset);
                sect = wrkgrp->section;
                wlink().CurrSect = sect;
    #ifdef DEVBUILD
                finfo = sect->outfile;
                DEBUG((DBG_LOADDOS, "group %a section %d to %l in %s",
                    &group->addr, sect->ovlref, info.offs, finfo->fname ));
    #endif
                if( group->leaders->class_->flags & CLASS_COPY ) {
                    Ring2Lookup( wrkgrp->leaders, DoHexDupLeader, &info.offs );
                } else {
                    Ring2Lookup( wrkgrp->leaders, DoHexLeader, &info.offs );
                }
            }
        }
    } else {
#endif
        OrderGroups( CompareDosSegments );
        wlink().CurrSect = wlink().Root;    // needed for WriteInfo.
        wlink().Root->sect_addr = wlink().Groups->addr;
        fnode = wlink().Root->outfile;
        fnode->file_loc = 0;
        wlink().Root->u.file_loc = (wlink().Root->sect_addr.seg << wlink().FmtData.SegShift) + wlink().Root->sect_addr.off - wlink().FmtData.output_offset;
        /* write groups */
        for( group = wlink().Groups; group != nullptr; group = group->next ) {
            class_ = group->leaders->class_;
            if( class_->flags & CLASS_COPY ) {
                sect = group->section;
                wlink().CurrSect = sect;
                info.offs = GROUP_SECTION_DELTA( group );
#ifdef DEVBUILD
                finfo = sect->outfile;
                DEBUG((DBG_LOADDOS, "group %a section %d to %l in %s",
                    &group->addr, sect->ovlref, info.offs, finfo->fname ));
#endif
                info.last_group = nullptr;
                RingLookup( class_->DupClass->segs->group->leaders, WriteHexCopyGroups, &info);
            } else {
                if( group->size != 0 ) {
                    sect = group->section;
                    wlink().CurrSect = sect;
                    info.offs = GROUP_SECTION_DELTA( group );
#ifdef DEVBUILD
                    finfo = sect->outfile;
                    DEBUG((DBG_LOADDOS, "group %a section %d to %l in %s",
                        &group->addr, sect->ovlref, info.offs, finfo->fname ));
#endif
                    Ring2Lookup( group->leaders, DoHexLeader, &info.offs );
                }
            }
        }
#if defined( _OS2 ) || defined( _QNX ) || defined( _ELF )
    }
#endif
    if( hs.buf_offset > 0 ) {          // If partial record in buffer, flush
        WriteHexLine();
    }
    if( wlink().FmtData.output_start ) {
        WriteStart();
    }
    WriteLoad( ":00000001ff\r\n", 13 );
    DBIWrite();
}

static unsigned long WriteGroupLoadHex( group_entry *group )
{
    grpwriteinfo    info;
    class_entry     *class_;
    unsigned long   file_loc;

    class_ = group->leaders->class_;
    file_loc = PosLoad();
    info.offs = GROUP_SECTION_DELTA( group );
    // If group is a copy group, substitute source group(s) here
    if( class_->flags & CLASS_COPY ) {
        info.last_group = nullptr; // so it will use the first group
        RingLookup( class_->DupClass->segs->group->leaders, WriteHexCopyGroups, &info );
    } else {
        Ring2Lookup( group->leaders, DoHexLeader, &info.offs );
    }
    return( PosLoad() - file_loc );
}

void FiniRawLoadFile( void )
/**************************/
{
    HexState        &hs = hexState();
    group_entry     *group;
    outfilelist     *fnode;
    unsigned long   file_loc;
    section         *sect;
    outfilelist     *finfo;
    signed_32       diff;

    DEBUG(( DBG_BASE, "Writing data" ));
    OrderGroups( CompareOffsets );
    wlink().CurrSect = wlink().Root;    // needed for WriteInfo.
    fnode = wlink().Root->outfile;
    fnode->file_loc = 0;
    wlink().Root->u.file_loc = 0;
    wlink().Root->sect_addr = wlink().Groups->addr;
    if( wlink().FmtData.raw_hex_output ) {
        hs.next_addr.off = 0;      // Start at absolute linear address 0
        hs.next_addr.seg = 0;
        hs.linear        = true;   // Linear mode
        hs.buf_offset    = 0;
    }
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( group->size ) {
            sect = group->section;
            wlink().CurrSect = sect;
            finfo = sect->outfile;
            file_loc = GROUP_FILE_LOC( group );
            if( wlink().FmtData.raw_hex_output ) {
                file_loc += WriteGroupLoadHex( group );
            } else {
                diff = file_loc - PosLoad();
                if( diff < 0 ) {
                    LnkMsg( ERR + MSG_FIXED_LOC_BEFORE_CUR_LOC, "a", &(group->addr));
                } else if( diff > 0 ) {
                    PadLoad( static_cast<size_t>( diff ) );
                }
                file_loc += WriteGroupLoad( group, false );
            }
            if( file_loc > finfo->file_loc ) {
                finfo->file_loc = file_loc;
            }
        }
    }
    DBIWrite();
}

#endif
