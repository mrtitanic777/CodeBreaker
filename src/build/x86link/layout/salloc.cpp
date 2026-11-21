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
* Description:  Utilities to compute the starting address of segments
*               includes rounding up to alignment and paragraph boundaries
*
****************************************************************************/


#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/layout/salloc.h"
#include "build/x86link/link/run_state.hpp"


namespace {

// the running addresses of the ID split format, one run of the linker's
struct AllocState {
    addr_type   DataLoc;
    addr_type   CodeLoc;
};

AllocState &alloc_state()
{
    return cb_wlink::RunState::part<AllocState>();
}

} // namespace

void ResetAddr( void )
/***************************/
{
    wlink().CurrLoc.seg = 0;
    wlink().CurrLoc.off = 0;
    wlink().CurrentSeg = nullptr;
}

void NormalizeAddr( void )
/*******************************/
{
    unsigned_32     new_seg;

    DbgAssert( (wlink().FmtData.type & MK_PROT_MODE) == 0 );

    if( wlink().CurrSect == wlink().NonSect
      || !wlink().FmtData.u.dos.ovl_short ) {
        new_seg = ( wlink().CurrLoc.off >> wlink().FmtData.SegShift ) + wlink().CurrLoc.seg;
        if( new_seg > 0xFFFF ) {
            LnkMsg( ERR+MSG_APP_TOO_BIG_FOR_DOS, nullptr );
        }
        wlink().CurrLoc.seg = static_cast<segment>( new_seg );
        wlink().CurrLoc.off &= wlink().FmtData.SegMask;
    }
}


static offset BumpUp( offset ptr, offset size )
/*********************************************/
{
    ptr += size;
    if( wlink().CurrentSeg != nullptr
      && (wlink().CurrentSeg->info & SEGINF_USE_32) == 0
      && (wlink().FmtData.type & MK_RAW) == 0
      && ptr > 0x10000 ) {
        LnkMsg( ERR+MSG_SEG_TOO_BIG, "sl", wlink().CurrentSeg->segname, static_cast<unsigned long>( ptr - 0x10000 ) );
    }
    return( ptr );
}


void AddSize( offset size )
/********************************/
{
    wlink().CurrSect->size += size;
    wlink().CurrLoc.off = BumpUp( wlink().CurrLoc.off, size );
}

offset CAlign( offset off, unsigned align )
/************************************************/
/* this aligns to 2^align */
{
    unsigned        part;

    if( align > 0 ) {
        align = 1U << align;
        part = off & ( align - 1 );
        if( part != 0 ) {
            off = BumpUp( off, align - part );
        }
    }
    return( off );
}

void Align( byte align )
/*****************************/
{
    offset          off;

    off = CAlign( wlink().CurrLoc.off, align );
    AddSize( off - wlink().CurrLoc.off );
}

void MAlign( byte align )
/******************************/
{
    wlink().CurrLoc.off = CAlign( wlink().CurrLoc.off, align );
}

void StartMemMap( void )
/*****************************/
{
    if( wlink().FmtData.type & MK_ID_SPLIT ) {
        AllocState &st = alloc_state();

        st.CodeLoc.seg = CODE_SEGMENT;
        st.CodeLoc.off = 0;
        st.DataLoc.seg = DATA_SEGMENT;
        st.DataLoc.off = 0;
    } else {
        wlink().CurrLoc.seg = ( wlink().FmtData.type & MK_PROT_MODE ) ? 1 : 0;
        wlink().CurrLoc.off = ( wlink().FmtData.type & MK_FLAT_OFFS ) ? wlink().FmtData.base : 0;
    }
}

static addr_type *GetIDLoc( group_entry *group )
/***********************************************
 * return a pointer to the current address for ID split format
 */
{
    AllocState      &st = alloc_state();

    if( group->segflags & SEG_DATA ) {
        return( &st.DataLoc );
    }
    return( &st.CodeLoc );
}

void ChkLocated( addr_type *seg_addr, bool fixed )
/***********************************************
 * If segment has been given a fixed address, use it
 * unless location counter is already past it
 * This should only be called from real mode
 */
{
    if( fixed ) {
        if( ( wlink().CurrLoc.seg << wlink().FmtData.SegShift ) + wlink().CurrLoc.off > ( seg_addr->seg << wlink().FmtData.SegShift ) + seg_addr->off ) {
            LnkMsg( ERR + MSG_FIXED_LOC_BEFORE_CUR_LOC, "a", seg_addr );
        } else {
            wlink().CurrLoc = *seg_addr;
        }
    } else {
        *seg_addr = wlink().CurrLoc;
    }
}

void NewSegment( seg_leader *seg )
/********************************/
{
    group_entry     *group;
    addr_type       *loc_addr;
    offset          off;
    bool            auto_group;

    group = seg->group;
    if( IS_DBG_INFO( seg ) ) {
        wlink().CurrentSeg = nullptr;
        Align( seg->align );
        ChkLocated( &seg->seg_addr, ( (seg->info & SEGINF_FIXED) != 0 ) );
        AddSize( seg->size );
    } else if( wlink().FmtData.type & MK_REAL_MODE ) {
        if( group->isautogrp
          && Ring2First( group->leaders ) != seg ) {
            auto_group = true; /* auto-group segment, but not first one */
        } else {
            auto_group = false;
        }
        if( !auto_group ) {
            NormalizeAddr();    /*  to prevent overflows on the alignment */
        }
        off = CAlign( wlink().CurrLoc.off, seg->align );
        group->totalsize += off - wlink().CurrLoc.off;
        AddSize( off - wlink().CurrLoc.off );
        if( !auto_group ) {
            NormalizeAddr();    /*  to normalize address of segment */
        }
        ChkLocated( &(seg->seg_addr), ( (seg->info & SEGINF_FIXED) != 0 ) );
        AddSize( seg->size );
        group->totalsize += seg->size;
    } else if( wlink().FmtData.type & (MK_FLAT_OFFS | MK_ID_SPLIT) ) {
        if( wlink().FmtData.type & MK_ID_SPLIT ) {
            loc_addr = GetIDLoc( group );
            wlink().CurrLoc = *loc_addr;
        }
        off = CAlign( wlink().CurrLoc.off, seg->align );
        group->totalsize += off - wlink().CurrLoc.off;
        AddSize( off - wlink().CurrLoc.off );
        seg->seg_addr = wlink().CurrLoc;
        AddSize( seg->size );
        group->totalsize += seg->size;
        if( wlink().FmtData.type & MK_ID_SPLIT ) {
            loc_addr = GetIDLoc( group );
            *loc_addr = wlink().CurrLoc;
        }
    } else {
        wlink().CurrLoc.seg = group->addr.seg;
        seg->seg_addr.seg = wlink().CurrLoc.seg;
        wlink().CurrLoc.off = group->totalsize;
        if( seg == wlink().FmtData.dgroupsplitseg ) {
            wlink().FmtData.bsspad = __ROUND_UP_SIZE( wlink().CurrLoc.off, wlink().FmtData.objalign ) - wlink().CurrLoc.off;
            AddSize( wlink().FmtData.bsspad );
        }
        Align( seg->align );
        seg->seg_addr.off = wlink().CurrLoc.off;
        if( seg->size != 0
          || !group->isautogrp ) {
            AddSize( seg->size );
            group->totalsize = wlink().CurrLoc.off;
        }
    }
}
