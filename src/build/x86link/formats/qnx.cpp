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
* Description:  Routines for creating QNX load files.
*
****************************************************************************/


#include <cstring>
#include <span>
#include <vector>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/base/ring.h"
#include "exeqnx.h"
#include "build/x86link/formats/qnx.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/link/run_state.hpp"


#ifdef _QNX

namespace {

constexpr std::size_t   QNX_MAX_DATA_SIZE = QNX_MAX_REC_SIZE - sizeof( lmf_data );
constexpr std::size_t   VERIFY_END = VERIFY_OFFSET + sizeof( lmf_rw_end::verify );

// where the load file's current group has got to, for one link
struct QnxState {
    offset          AmountWritten;
    bool            InVerifySegment;
    lmf_rw_end      RWEndRec;
    group_entry     *CurrGroup;
};

QnxState &state()
{
    return cb_wlink::RunState::part<QnxState>();
}

using LoadWriter = FunctionRef<void( virt_mem, unsigned long )>;
using VerifyWriter = void (*)( char *, virt_mem, unsigned long );

}

static void WriteLoadRec(Link& link)
/******************************/
{
    QnxState        &st = state();
    lmf_record      record;
    lmf_data        data;

    record.rec_type = LMF_LOAD_REC;
    record.reserved = record.spare = 0;
    if( st.CurrGroup->size - st.AmountWritten > QNX_MAX_DATA_SIZE ){
        record.data_nbytes = QNX_MAX_REC_SIZE;
    } else {
        record.data_nbytes = st.CurrGroup->size - st.AmountWritten + sizeof( lmf_data );
    }
    WriteLoad(link,  &record, sizeof( lmf_record ) );
    data.segment = QNX_SEL_NUM( st.CurrGroup->addr.seg );
    data.offset = st.AmountWritten;
    WriteLoad(link,  &data, sizeof( lmf_data ) );
}

static void ZeroLoad(Link& link,  virt_mem dummy, unsigned long length )
/**********************************************************/
{
    /* unused parameters */ (void)dummy;

    PadLoad(link,  length );
}

static void CopyLoad(Link& link,  virt_mem data, unsigned long length )
/*********************************************************/
{
    WriteInfoLoad( data, length );
}

static void WriteQNXInfo(Link& link,  LoadWriter wrfn, virt_mem data, unsigned long length )
/*****************************************************************************/
{
    QnxState    &st = state();
    unsigned    bufmod;
    unsigned    adjust;

    bufmod = st.AmountWritten % QNX_MAX_DATA_SIZE;
    while(( bufmod + length ) >= QNX_MAX_DATA_SIZE ) {
        adjust = QNX_MAX_DATA_SIZE - bufmod;
        wrfn( data, adjust );
        st.AmountWritten += adjust;
        length -= adjust;
        data += adjust;
        bufmod = 0;
        WriteLoadRec(link);
    }
    if( length > 0 ) {
        wrfn( data, length );
        st.AmountWritten += length;
    }
}

static void CopyVerify( char *targ, virt_mem src, unsigned long size )
/********************************************************************/
{
    ReadInfo( src, targ, size );
}

static void ZeroVerify( char *targ, virt_mem dummy, unsigned long size )
/**********************************************************************/
{
    /* unused parameters */ (void)dummy;

    std::memset( targ, 0, size );
}

static void SetVerifyInfo( VerifyWriter wrfn, virt_mem data, unsigned long length )
/*********************************************************************************/
{
    QnxState    &st = state();
    offset      newstart;
    char *      targ;

    if( st.AmountWritten + length <= VERIFY_OFFSET )
        return;
    if( st.AmountWritten >= VERIFY_END )
        return;
    newstart = st.AmountWritten;
    if( st.AmountWritten < VERIFY_OFFSET ) {
        data += VERIFY_OFFSET - st.AmountWritten;
        newstart = VERIFY_OFFSET;
    }
    if( newstart + length >= VERIFY_END ) {
        length = VERIFY_END - newstart;
        st.InVerifySegment = false;
    }
    targ = reinterpret_cast<char *>( &st.RWEndRec.verify );
    if( st.AmountWritten > VERIFY_OFFSET ) {
        targ += st.AmountWritten - VERIFY_OFFSET;
    }
    wrfn( targ, data, length );
}

static bool WriteSegData(Link& link,  void *_sdata, void *_start )
/****************************************************/
{
    QnxState &st = state();
    segdata *sdata = static_cast<segdata *>( _sdata );
    unsigned long *start = static_cast<unsigned long *>( _start );
    unsigned long newpos;
    long pad;

    if( !sdata->isuninit && !sdata->isdead && sdata->length > 0 ) {
        newpos = *start + sdata->a.delta;
        pad = newpos - st.AmountWritten;
        DbgAssert( pad >= 0 );
        if( pad > 0 ) {
            if( st.InVerifySegment ) {
                SetVerifyInfo( ZeroVerify, 0, pad );
            }
            WriteQNXInfo(link,  bind_link( link, ZeroLoad ), 0, pad );
        }
        if( st.InVerifySegment ) {
            SetVerifyInfo( CopyVerify, sdata->u1.vm_ptr, sdata->length );
        }
        WriteQNXInfo(link,  bind_link( link, CopyLoad ), sdata->u1.vm_ptr, sdata->length );
    }
    return( false );
}

static void DoGroupLeader(Link& link,  void *_seg )
/*************************************/
{
    offset      start;
    seg_leader *seg = static_cast<seg_leader *>( _seg );

    start = SEG_GROUP_DELTA( seg );
    RingLookup( seg->pieces, bind_link( link, WriteSegData ), &start );
}

static void WriteQNXGroup(Link& link,  group_entry *group, std::span<unsigned_32> segments )
/*****************************************************************************/
{
    QnxState    &st = state();
    segment     seg;

    seg = QNX_SEL_NUM( group->addr.seg );
    segments[seg] = group->totalsize | ( static_cast<unsigned_32>( group->u.qnxflags ) << 28 );
    if( link.StackSegPtr != nullptr && group == link.StackSegPtr->group ) {
        segments[seg] -= link.StackSize;      // stack size gets
    }                                    // added on by QNX loader.
    if( group->size == 0 )
        return;
    st.AmountWritten = 0;
    st.CurrGroup = group;
    WriteLoadRec(link);
    Ring2Walk( group->leaders, bind_link( link, DoGroupLeader ) );
}

static void WriteQNXRelocs(Link& link,  reloc_info reloclist, unsigned lmf_type, unsigned_16 seg )
/************************************************************************************/
{
    lmf_record          record;
    unsigned_32         pos;
    unsigned_32         size;
    bool                islinear;
    unsigned            adjust;

    adjust = 0;
    record.reserved = record.spare = 0;
    record.rec_type = lmf_type;
    islinear = (lmf_type == LMF_LINEAR_FIXUP_REC);
    if( islinear ) {
        adjust = 2;
    }
    pos = PosLoad(link);    /* get current position */
    while( reloclist != nullptr ) {
        SeekLoad(link,  pos + sizeof( lmf_record ) );
        if( islinear ) {
            WriteLoad(link,  &seg, sizeof( unsigned_16 ) );
        }
        size = DumpMaxRelocList(link,  &reloclist, QNX_MAX_FIXUPS - adjust ) + adjust;
        SeekLoad(link,  pos );
        record.data_nbytes = size;
        WriteLoad(link,  &record, sizeof( lmf_record ) );
        pos += size + sizeof( lmf_record );
    }
    SeekLoad(link,  pos );
}

static void WriteQNXData(Link& link,  std::span<unsigned_32> segments )
/*********************************************************/
{
    QnxState &      st = state();
    group_entry *   group;
    lmf_record      record;
    bool            first;

    first = true;
    st.InVerifySegment = false;
    /* write all read/write segment data */
    st.RWEndRec.signature = 0;
    for( group = link.Groups; group != nullptr; group = group->next ){
        if( group->u.qnxflags == QNX_READ_WRITE && group->totalsize != 0 ) {
            if( first && group->size >= VERIFY_END ) {
                st.InVerifySegment = true;
                first = false;
            }
            WriteQNXGroup(link,  group, segments );
        }
        st.RWEndRec.signature += group->totalsize;
        st.RWEndRec.signature <<= 3;
        st.RWEndRec.signature ^= 0x32476235;
    }
    record.reserved = record.spare = 0;
    record.rec_type = LMF_RW_END_REC;
    record.data_nbytes = sizeof( st.RWEndRec );
    WriteLoad(link,  &record, sizeof( lmf_record ) );
    st.RWEndRec.verify ^= static_cast<unsigned_16>( ~0 );
    WriteLoad(link,  &st.RWEndRec, sizeof( st.RWEndRec ) );
    /* write all read only segment data */
    for( group = link.Groups; group != nullptr; group = group->next ){
        if( group->u.qnxflags != QNX_READ_WRITE && group->totalsize != 0 ) {
            WriteQNXGroup(link,  group, segments );
        }
        WriteQNXRelocs(link,  group->g.reloclist, LMF_LINEAR_FIXUP_REC, QNX_SEL_NUM( group->addr.seg ) );
    }
}


static bool checkGroupFlags( void *_seg, void *_group )
/*****************************************************/
{
    seg_leader      *seg = static_cast<seg_leader *>( _seg );
    group_entry     *group = static_cast<group_entry *>( _group );
    seg_flags_type  sflags;

    sflags = seg->segflags;

    // the default value for segflags is set to SEG_PMODE_DPL_3 (0x0C00) for OS/2
    // and SEG_MOVABLE (0x10) for windows. Since the highest value that can be
    // specified for a QNX seg flag is 4, if sflags >= 0x10, there was no QNX value
    // specified.

    if( sflags < 0x10 ) {
        if( (sflags & QNX_READ_ONLY) == 0 ) {       // if can read/write or exec/read
            group->u.qnxflags &= ~QNX_READ_ONLY;    // can for all segments.
            return( true );                         // no need to check others
        }
    } else {
        // make segments read/write or exec/read unless every segment is specifically
        // set otherwise.

        group->u.qnxflags &= ~QNX_READ_ONLY;
        return( true );
    }
    return( false );
}

void SetQNXGroupFlags(Link& link)
/***************************/
// This goes through the groups, setting the flag word to be compatible with
// the flag words that are specified in the segments.
{
    group_entry *   group;

    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->segflags & SEG_DATA ) {
            group->u.qnxflags = QNX_READ_ONLY;      // 1
        } else {
            group->u.qnxflags = QNX_EXEC_ONLY;      // 3
        }
        Ring2Lookup( group->leaders, checkGroupFlags, group );
    }
}

void SetQNXSegFlags(Link& link)
/*************************/
{
    SetSegFlags(link,  link.FmtData.u.qnx.seg_flags );
    link.FmtData.u.qnx.seg_flags = nullptr;         // segsegflags frees the list.
}

static void WriteQNXResource(Link& link)
/**********************************/
{
    unsigned long   len;
    lmf_record      rec;
    f_handle        file;
    lmf_resource    resource;

    if( link.FmtData.resource != nullptr ) {
        rec.reserved = 0;
        rec.spare = 0;
        rec.rec_type = LMF_RESOURCE_REC;
        resource = lmf_resource{};
        if( link.FmtData.res_name_only ) {
            file = QObjOpen( link.FmtData.resource );
            if( file == NIL_FHANDLE ) {
                PrintIOError( WRN+MSG_CANT_OPEN_NO_REASON, "s", link.FmtData.resource );
                return;
            }
            len = QFileSize( file );
            if( len + sizeof( lmf_resource ) > QNX_MAX_REC_SIZE ) {
                LnkMsg( WRN+MSG_RESOURCE_TOO_BIG, "s", link.FmtData.resource );
                return;
            }
            std::vector<char> buf( len );
            rec.data_nbytes = len + sizeof( lmf_resource );
            WriteLoad(link,  &rec, sizeof( lmf_record ) );
            WriteLoad(link,  &resource, sizeof( lmf_resource ) );
            QRead( file, buf.data(), len, link.FmtData.resource );
            WriteLoad(link,  buf.data(), len );
            QClose( file, link.FmtData.resource );
        } else {
            len = std::strlen( link.FmtData.resource );
            link.FmtData.resource[len] = '\n';
            len++;
            rec.data_nbytes = len + sizeof( lmf_resource );
            WriteLoad(link,  &rec, sizeof( lmf_record ) );
            WriteLoad(link,  &resource, sizeof( lmf_resource ) );
            WriteLoad(link,  link.FmtData.resource, len );
        }
    }
}

void FiniQNXLoadFile(Link& link)
/*********************************/
{
    unsigned_16     nbytes;
    lmf_record      record;
    lmf_header      header;
    lmf_eof         trailer;
    unsigned        adseg;

    link.CurrSect = link.Root;
    if( link.FmtData.type & MK_QNX_FLAT ) {
        if( link.FmtData.base < link.StackSize ) {
            LnkMsg( WRN+MSG_QNX_BASE_LT_STACK, nullptr );
        }
    }
    nbytes = link.NumGroups * sizeof( unsigned_32 );
    std::vector<unsigned_32> segments( link.NumGroups );
    SeekLoad(link,  sizeof( lmf_header ) + nbytes + sizeof( lmf_record ) );
    WriteQNXResource(link);
    WriteQNXData(link,  segments );
    WriteQNXRelocs(link,  link.Root->reloclist, LMF_FIXUP_REC, 0 );
    WriteQNXRelocs(link,  link.FloatFixups, LMF_8087_FIXUP_REC, 0 );
    record.reserved = record.spare = 0;
    record.rec_type = LMF_IMAGE_END_REC;
    record.data_nbytes = sizeof( trailer );
    WriteLoad(link,  &record, sizeof( lmf_record ) );
    trailer = lmf_eof{};
    WriteLoad(link,  &trailer, sizeof( trailer ) );
    DBIWrite(link);

    SeekLoad(link,  0 );
    record.rec_type = LMF_HEADER_REC;
    record.data_nbytes = sizeof( lmf_header ) + nbytes;
    WriteLoad(link,  &record, sizeof( lmf_record ) );
    header = lmf_header{};
    header.version = QNX_VERSION;
    header.cflags = (link.FmtData.u.qnx.flags | (link.FmtData.u.qnx.priv_level << QNX_PRIV_SHIFT)) & QNX_FLAG_MASK;
    if( link.LinkState & LS_FMT_SEEN_32BIT ) {
        header.cflags |= _TCF_32BIT;
    }
    if( link.FmtData.type & MK_QNX_FLAT ) {
        header.cflags |= _TCF_FLAT;
    }
    header.fpu = 0;     //NYI: need to set 87, 287, 387
    if( link.FmtData.cpu_type == 0 ) {        // no model specified, so assume...
        if( link.LinkState & LS_FMT_SEEN_32BIT ) {
            header.cpu = 386;
        } else {
            header.cpu = 86;
        }
    } else {
        header.cpu = (link.FmtData.cpu_type * 100) + 86;
    }
    header.code_index = QNX_SEL_NUM( link.StartInfo.addr.seg );
    header.stack_index = QNX_SEL_NUM( link.StackAddr.seg );
    adseg = header.stack_index;
    if( link.DataGroup != nullptr ) {
        adseg = QNX_SEL_NUM( link.DataGroup->addr.seg );
    }
    header.heap_index = adseg;  // all in DGROUP.
    header.argv_index = adseg;
    header.code_offset = link.StartInfo.addr.off;
    header.stack_nbytes = link.StackSize;
    header.heap_nbytes = link.FmtData.u.qnx.heapsize;
    header.image_base = link.FmtData.base;
    WriteLoad(link,  &header, sizeof( lmf_header ) );
    WriteLoad(link,  segments.data(), nbytes );
}

unsigned_16 ToQNXSel(Link& link,  unsigned_16 seg )
/********************************************/
{
    return( QNX_SELECTOR( seg - 1 ) );
}

unsigned_16 ToQNXIndex( unsigned_16 sel )
/**********************************************/
{
    return( QNX_SEL_NUM( sel ) );
}

#endif
