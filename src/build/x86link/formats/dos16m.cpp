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
* Description:  Routines for creating DOS/16M load files.
*
****************************************************************************/


#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "exedos.h"
#include "exe16m.h"
#include "build/x86link/formats/dos16m.h"
#include "pcobj.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


#ifdef _DOS16M

/* Notes:
 *  - Currently we only support RSI-1 style relocations (2 segments, one containing
 * fixup segment values and the other offsets, presumably limited to 32K relocs).
 * Newer tools create RSI-2 style relocations in slightly more complex format - a word
 * containing segment value, a word containing count, and specified number of word-sized
 * offsets, repeated for each segment to be fixed up. RSI-2 style relocs may span multiple
 * segments (huge seg). Support for RSI-2 could be added relatively easily.
 *   - It might be good to default to 'AUTO' (ie. relocatable) modules, since
 * non-relocatable DOS/16M modules only run on VCPI or raw systems, not DPMI.
 */

namespace {

using reloc_addr = dos_addr;

struct context {
    reloc_addr      *reloc_data;
};

// what the DOS/16M writer keeps for one run
struct D16MState {
    unsigned_32     LastSel = 0;
    unsigned        reloc_fmt = 0;
};

D16MState &state()
{
    return cb_wlink::RunState::part<D16MState>();
}

}   // namespace

static void WriteGDT(Link& link,  unsigned_32 reloc_size )
/********************************************/
// write out the GDTs
{
    gdt_info        gdt;
    group_entry     *group;
    const unsigned  reloc_fmt = state().reloc_fmt;

// write program gdt's.
    gdt.gdtaddr_hi = 0;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->size > 0 ) {
            gdt.gdtlen = __ROUND_UP_SIZE_PARA( group->size ) - 1; // para align.
        } else {
            gdt.gdtlen = 0;
        }
        gdt.gdtaddr = 0; // group->u.dos_segment;
        gdt.gdtreserved = __ROUND_UP_SIZE_TO_PARA( (unsigned_32)group->totalsize );   // mem size in paras
        if( gdt.gdtreserved == 0 && group->size == 0 )
            gdt.gdtreserved |= 0x2000;
        if( group->segflags & SEG_DATA ) {
            gdt.gdtaccess = D16M_ACC_DATA;
            if( group == link.DataGroup ) {
                gdt.gdtreserved = link.FmtData.u.d16m.datasize;
                if( link.FmtData.u.d16m.flags & TRANS_STACK ) {
                    gdt.gdtreserved |= TRANSPARENT;
                }
            }
            if( link.FmtData.u.d16m.flags & TRANS_DATA ) {
                gdt.gdtreserved |= TRANSPARENT;
            }
        } else {
            gdt.gdtaccess = D16M_ACC_CODE;
        }
        WriteLoad(link,  &gdt, sizeof( gdt_info ) );
    }
// write out gdt for reloc info
    if( reloc_fmt == 1 ) {
        reloc_size /= 2;
        gdt.gdtreserved = 0;
        gdt.gdtaddr = 0;
        gdt.gdtaccess = D16M_ACC_DATA;
        gdt.gdtlen = reloc_size - 1;
        gdt.gdtreserved = reloc_size >> 4;   // mem size in paras
        WriteLoad(link,  &gdt, sizeof( gdt_info ) );
        WriteLoad(link,  &gdt, sizeof( gdt_info ) );
    } else if( reloc_fmt == 2 ) {
        gdt.gdtreserved = 0;
        gdt.gdtaddr = 0;
        gdt.gdtaccess = D16M_ACC_DATA;
        gdt.gdtlen = reloc_size - 1;
        gdt.gdtreserved = reloc_size >> 4;   // mem size in paras
        WriteLoad(link,  &gdt, sizeof( gdt_info ) );
    }
}

static unsigned Write16MData(Link& link,  unsigned hdr_size )
/***********************************************/
{
    group_entry     *group;

    DEBUG(( DBG_BASE, "Writing DOS/16M data" ));

    SeekLoad(link,  hdr_size );
    for( group = link.Groups; group != nullptr; group = group->next ) {
        WriteGroupLoad(link,  group, false );
        NullAlign(link,  0x10 );          // paragraph alignment.
    }
    return( PosLoad(link) );
}

static bool RelocWalkFn( void *data, size_t size, void *ctx )
/***********************************************************/
{
    context     *info = static_cast<context *>( ctx );

    memcpy( info->reloc_data, data, size );
    info->reloc_data += size / sizeof( reloc_addr );
    return( false );   /* don't stop walking */
}

static void write_sel_reloc(Link& link,  unsigned_16 sel, reloc_addr *block_start, reloc_addr *block_end )
/********************************************************************************************/
{
    unsigned_16     block_cnt;

    if( sel != 0 ) {
        WriteLoadU16(link,  sel );
        block_cnt = static_cast<unsigned_16>( block_end - block_start );
        WriteLoadU16(link,  block_cnt );
        for( ; block_start < block_end; ++block_start ) {
            WriteLoadU16(link,  block_start->off );
        }
    }
}

static bool reloc_entry_less( const reloc_addr &a, const reloc_addr &b )
/*********************************************************************/
{
    int retval;

    retval = a.seg - b.seg;
    if( retval == 0 ) {
        retval = a.off - b.off;
    }
    return( retval < 0 );
}

static unsigned GetRelocBlock(Link& link,  reloc_addr **reloc_data )
/******************************************************/
{
    reloc_info      reloclist;
    size_t          num_relocs;
    context         info;

    *reloc_data = nullptr;
    if( (link.LinkState & LS_MAKE_RELOCS) && link.Root->relocs ) {
        reloclist = link.Root->reloclist;
        num_relocs = link.Root->relocs;
        *reloc_data = static_cast<reloc_addr *>( MemAllocSafe( sizeof( reloc_addr ) * num_relocs ) );
        if( *reloc_data != nullptr ) {
            info.reloc_data = *reloc_data;
            WalkRelocList(link,  &reloclist, RelocWalkFn, &info );
            if( state().reloc_fmt == 1 ) {
                return( 2 );
            } else {
                std::sort( *reloc_data, *reloc_data + num_relocs, reloc_entry_less );
                // TODO: number of extra reloc segments should be
                // properly calculated, now only one is set
                // (max. relocs are nearly 2^15 items)
                return( 1 );
            }
        }
    }
    state().reloc_fmt = 0;
    return( 0 );
}

// There are two types of relocations for DOS/16M. Old style or RSI-1 relocs
// are stored internally in one virtual memory block, with the segment selectors
// being stored in the first half of the memory block, and the segment offsets
// being stored in the second half. These are limited to 32K relocs.
// Newer style or "huge" RSI-2 relocs are stored in one or more consecutive
// segments. First is the selector, then a count word, followed by offsets within
// the selector. This sequence is repeated for other selectors. The last selector
// in the list has bit 1 set (selector numbers are multiples of 8 so the
// low four bits are unused).

static unsigned_32 Write16MRelocs(Link& link,  reloc_addr *reloc_data )
/*********************************************************/
{
    unsigned        num_relocs;
    unsigned_32     pos;
    unsigned        i;
    unsigned_16     sel;
    unsigned        block_start;
    const unsigned  reloc_fmt = state().reloc_fmt;

    pos = PosLoad(link);
    num_relocs = link.Root->relocs;
    if( reloc_fmt == 1 ) {
        // RSI-1 reloc format
        for( i = 0; i < num_relocs; ++ i ) {
            WriteLoadU16(link,  reloc_data[i].seg );
        }
        NullAlign(link,  0x10 );
        for( i = 0; i < num_relocs; ++ i ) {
            WriteLoadU16(link,  reloc_data[i].off );
        }
    } else if( reloc_fmt == 2 ) {
        // RSI-2 reloc format
        sel = 0;
        block_start = 0;
        for( i = 0; i < num_relocs; ++ i ) {
            if( sel != reloc_data[i].seg ) {
                write_sel_reloc(link,  sel, reloc_data + block_start, reloc_data + i );
                block_start = i;
                sel = reloc_data[i].seg;
            }
        }
        write_sel_reloc(link,  sel | 0x02, reloc_data + block_start, reloc_data + i );
    }
    return( NullAlign(link,  0x10 ) - pos );
}

static unsigned_32 WriteStubProg(Link& link)
/**************************************/
{
    unsigned_32 size;
    f_handle    fhandle;
    std::array<char, PATH_MAX> fullname;

    size = 0;
    if( link.FmtData.u.d16m.stub != nullptr ) {
        fhandle = FindPath(link,  link.FmtData.u.d16m.stub, fullname.data() );
        if( fhandle == NIL_FHANDLE ) {
            LnkMsg( WRN+MSG_CANT_OPEN_NO_REASON, "s", link.FmtData.u.d16m.stub );
        } else {
            MemFree( link.FmtData.u.d16m.stub );
            link.FmtData.u.d16m.stub = MemStrdupSafe(link,  fullname.data() );
            size = CopyToLoad(link,  fhandle, link.FmtData.u.d16m.stub );
            QClose( fhandle, link.FmtData.u.d16m.stub );
        }
    }
    SetOriginLoad(link,  size );
    return( size );
}

void Fini16MLoadFile(Link& link)
/**************************/
{
    unsigned            hdr_size;
    unsigned            temp;
    unsigned            i;
    unsigned            exe_size;
    dos16m_exe_header   exe_head;
    unsigned            extra_sels;
    D16MState           &st = state();
    unsigned            stub_size;
    unsigned            reloc_size;
    reloc_addr          *reloc_data;

    memset( &exe_head, 0, sizeof( exe_head ) );
    // TODO: add some parameter for reloc format switching
    // for now it is setup to RSI-2 reloc format
    st.reloc_fmt = 2;
    extra_sels = GetRelocBlock(link,  &reloc_data );
    stub_size = WriteStubProg(link);
    hdr_size = sizeof( exe_head ) + (link.NumGroups + extra_sels) * sizeof( gdt_info );
    hdr_size = __ROUND_UP_SIZE_PARA( hdr_size );
    reloc_size = 0;
    exe_size = Write16MData(link,  hdr_size );
    if( extra_sels ) {
        reloc_size = Write16MRelocs(link,  reloc_data );
        exe_size += reloc_size;
        MemFree( reloc_data );
    }
    DBIWrite(link);

    SeekLoad(link,  0 );
    _HostU16toTarg( EXESIGN_DOS16M, exe_head.signature );
    temp = hdr_size / 16U;
    _HostU16toTarg( exe_size % 512U, exe_head.last_page_bytes );
    temp = ( exe_size + 511U ) / 512U;
    _HostU16toTarg( temp, exe_head.pages_in_file );
    _HostU16toTarg( link.FmtData.u.d16m.extended, exe_head.max_alloc );
    _HostU16toTarg( 0, exe_head.min_alloc );
    _HostU16toTarg( link.StartInfo.addr.off, exe_head.init_ip );
    _HostU16toTarg( link.StartInfo.addr.seg, exe_head.code_seg );
    if( st.reloc_fmt == 2 ) {
        temp = link.FmtData.u.d16m.selstart + link.NumGroups * sizeof( gdt_info );
    } else {
        temp = 0;
    }
    _HostU16toTarg( temp, exe_head.first_reloc_sel );
    _HostU16toTarg( link.StackAddr.seg, exe_head.stack_seg );
    _HostU16toTarg( link.StackAddr.off, exe_head.stack_ptr );
    _HostU16toTarg( link.FmtData.u.d16m.gdtsize, exe_head.runtime_gdt_size );
    _HostU16toTarg( 2000, exe_head.MAKEPM_version );   // pretend GLU version 2.0
    _HostU32toTarg( exe_size + stub_size, exe_head.next_header_pos );
    _HostU32toTarg( 0, exe_head.cv_info_offset );
    if( extra_sels ) {
        _HostU16toTarg( ef_auto, exe_head.exp_flags );
    } else {
        link.FmtData.u.d16m.options &= ~OPT_AUTO;
    }
    _HostU16toTarg( link.FmtData.u.d16m.options, exe_head.options );
    temp = NUM_RESERVED_SELS + link.NumGroups + extra_sels;
    _HostU16toTarg( __ROUND_UP_SIZE_PARA( temp * sizeof( gdt_info ) ) - 1, exe_head.gdtimage_size );
    exe_head.reserved5 = D16M_ACC_DATA;
    _HostU16toTarg( link.FmtData.u.d16m.selstart, exe_head.first_selector );
    _HostU16toTarg( link.FmtData.u.d16m.buffer, exe_head.transfer_buffer_size );
    _HostU16toTarg( link.FmtData.u.d16m.strategy, exe_head.default_mem_strategy );
    while( extra_sels-- ) {
        st.LastSel += sizeof( gdt_info );
        if( st.LastSel > 0xFFFF ) {
            LnkMsg( FTL + MSG_TOO_MANY_SELECTORS, nullptr );
        }
        if( st.LastSel >= 0xA000 && (st.LastSel & 0x1FF) == 0 ) {   // check for reserved selector
            st.LastSel += sizeof( gdt_info );
        }
    }
    _HostU16toTarg( st.LastSel, exe_head.last_sel_used );
    /*
     * add original EXP name
     */
    temp = strlen( link.FmtData.u.d16m.exp_name );
    if( temp > sizeof( exe_head.EXP_path ) - 1 )
        temp = sizeof( exe_head.EXP_path ) - 1;
    for( i = 0; i < temp; i++ ) {
        exe_head.EXP_path[i] = toupper( reinterpret_cast<unsigned char *>( link.FmtData.u.d16m.exp_name )[i] );
    }
    exe_head.EXP_path[temp] = '\0';

    WriteLoad(link,  &exe_head, sizeof( exe_head ) );
    WriteGDT(link,  reloc_size );
}

unsigned ToD16MSel(Link& link,  unsigned seg_num )
/************************************/
{
    unsigned_32    x;
    unsigned_32    &LastSel = state().LastSel;

    LastSel = link.FmtData.u.d16m.selstart + ( seg_num - 1 ) * sizeof( gdt_info );
    for( x = 0xA000; LastSel >= x; x += 0x200 ) {   // check for reserved selector
        LastSel += sizeof( gdt_info );
        if( LastSel > 0xFFFF ) {
            LnkMsg( FTL + MSG_TOO_MANY_SELECTORS, nullptr );
        }
    }
    return( LastSel );
}

segment Find16MSeg(Link& link,  segment selector )
/************************************/
// this finds the dos_segment value which corresponds to the given selector.
{
    group_entry     *group;
    segment         result;

    result = 0;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->addr.seg == selector ) {
            result = group->u.dos_segment;
            break;
        }
    }
    return( result );
}

void CalcGrpSegs(Link& link)
/**********************/
// go through group list & calculate what the segment would be if we were
// running under DOS.
{
    group_entry     *group;
    offset          addr;

    addr = 0;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        addr = __ROUND_UP_SIZE_PARA( addr );       // addr is paragraph aligned.
        group->u.dos_segment = addr >> 4;
        addr += group->totalsize;
    }
}

static void SetSegDataAbs( void *_sdata )
/***************************************/
{
    segdata     *sdata = static_cast<segdata *>( _sdata );

    if( !sdata->isdead ) {
        sdata->isabs = true;
        sdata->combine = COMBINE_INVALID;
        sdata->isuninit = true;
    }
}

static void RemapAliasSels( void *_leader )
/*****************************************/
{
    Link& link = wlink();
    seg_leader  *leader = static_cast<seg_leader *>( _leader );
    seg_leader  *seg;
    section     *sect;

    if( leader->info & SEGINF_ABSOLUTE ) {
        sect = link.Root;
        while( (seg = FindSegment( sect, leader->segname.u.ptr )) != nullptr ) {
            sect = nullptr;
            if( (seg->info & SEGINF_ABSOLUTE) == 0 ) {
                seg->info |= SEGINF_ABSOLUTE;
                seg->combine = COMBINE_INVALID;
                seg->seg_addr = leader->seg_addr;
                RingWalk( seg->pieces, SetSegDataAbs );
            }
        }
    }
}

void MakeDos16PM(Link& link)
/**********************/
{
    class_entry *class_;

    /*
     * remap dos16m kernel selectors reference
     */
    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) {
            RingWalk( class_->segs, RemapAliasSels );
        }
    }
    /*
     * change startup point
     */
    if( FindISymbol(link,  "_cstart_" ) != nullptr ) {
        link.StartInfo.type = START_UNDEFED;
        SetStartSym(link,  "_cstart_" );
    }
}

#endif
