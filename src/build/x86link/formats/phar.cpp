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
* Description:  LOADPHAR : routines for creating phar lap load files.
*
****************************************************************************/

#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "exephar.h"
#include "build/x86link/formats/phar.h"
#include "build/x86link/fixups/reloc.h"
#include "proc386.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/directives/state.hpp"   /* CodeBreaker: `runtime realbreak` given */


#ifdef _PHARLAP

namespace {

constexpr unsigned  HEAD_SIZE = 0x180;
constexpr unsigned  RTP_SIZE = 0x080;

enum class DescFlags : unsigned {
    none    = 0,
    IS_CODE = 0x1,
    IS_APP  = 0x2,
    IS_USER = 0x4,
    BASE    = 0x8,
    TSS     = 0x10
};

constexpr DescFlags operator|( DescFlags a, DescFlags b )
{
    return static_cast<DescFlags>( static_cast<unsigned>( a ) | static_cast<unsigned>( b ) );
}

constexpr DescFlags &operator|=( DescFlags &a, DescFlags b )
{
    return a = a | b;
}

constexpr bool has( DescFlags flags, DescFlags f )
{
    return ( static_cast<unsigned>( flags ) & static_cast<unsigned>( f ) ) != 0;
}

}


static unsigned_32 WritePharData(Link& link,  unsigned file_pos )
/***************************************************/
{
    group_entry         *group;
    outfilelist *       fnode;
    bool                repos;

    DEBUG(( DBG_BASE, "Writing data" ));
    OrderGroups(link,  CompareOffsets );
    link.CurrSect = link.Root;        // needed for WriteInfo.
    fnode = link.Root->outfile;
    fnode->file_loc = file_pos;
    link.Root->u.file_loc = file_pos;
    link.Root->sect_addr = link.Groups->addr;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        repos = WriteGroup(link,  group );
        if( repos ) {
            SeekLoad(link,  fnode->file_loc );
        }
    }
    return( fnode->file_loc - file_pos );
}

static unsigned_32 WritePharRelocs(Link& link)
/****************************************/
// write the relocations.
{
    reloc_info  reloclist;

    reloclist = link.Root->reloclist;             // don't want to modify original
    return( DumpMaxRelocList(link,  &reloclist, 0 ) );
}

static void WritePharSimple(Link& link,  unsigned_32 start )
/**********************************************/
{
    simple_header   header;
    unsigned_32     file_size;
    unsigned_32     header_size;
    unsigned_32     extra;
    unsigned_32     temp;

    if( link.FmtData.type & MK_PHAR_REX ) {
        SeekLoad(link,  start + sizeof( simple_header ) );
        extra = start + sizeof( simple_header ) + WritePharRelocs(link);
        header_size = __ROUND_UP_SIZE_PARA( extra );
        PadLoad(link,  header_size - extra );
    } else {
        SeekLoad(link,  start + __ROUND_UP_SIZE_PARA( sizeof( simple_header ) ) );
        header_size = __ROUND_UP_SIZE_PARA( sizeof( simple_header ) );    // para align.
    }
    file_size = header_size + WritePharData(link,  start + header_size );
    DBIWrite(link);
    if( link.FmtData.type & MK_PHAR_SIMPLE ) {
        _HostU16toTarg( SIMPLE_SIGNATURE, header.signature );
    } else {
        _HostU16toTarg( EXESIGN_REX, header.signature );
    }
    _HostU16toTarg( file_size % 512U, header.mod_size );
    _HostU16toTarg( (file_size + 511U) / 512U, header.file_size );
    _HostU16toTarg( link.Root->relocs, header.num_relocs );
    _HostU16toTarg( header_size / 16, header.hdr_size );
    extra = MemorySize(link) - file_size + header_size + 0xfff;
    temp = link.FmtData.u.phar.mindata + extra;
    if( temp < link.FmtData.u.phar.mindata )
        temp = 0xffffffff;
    _HostU16toTarg( temp >> 12, header.min_data );
    temp = link.FmtData.u.phar.maxdata + extra;
    if( temp < link.FmtData.u.phar.maxdata )
        temp = 0xffffffff;
    _HostU16toTarg( temp >> 12, header.max_data );
    _HostU32toTarg( link.StackAddr.off, header.ESP );
    _HostU16toTarg( 0, header.checksum );
    _HostU32toTarg( link.StartInfo.addr.off, header.EIP );
    _HostU16toTarg( 0x1E, header.reloc_offset );
    _HostU16toTarg( 0, header.overlay_num );
    /* allow header size to exceed 1M */
    _HostU16toTarg( (header_size / _1M) + 1, header.always_one );
    SeekLoad(link,  start );
    WriteLoad(link,  &header, sizeof( simple_header ) );
    if( link.FmtData.type & MK_PHAR_SIMPLE ) {
        PadLoad(link,  2 );                   // header occupies a full paragraph.
    }
}

static void WriteDescriptor(Link& link,  unsigned_32 base, unsigned_32 limit,
                             DescFlags flags )
/****************************************************************/
{
    descriptor  desc;

    SET_DESC_BASE( desc, base );
    desc.u1.val = 0;
    if( has( flags, DescFlags::BASE ) ) {
        desc.u1.flags_data.writeable = 1;
        desc.u1.flags.present = 1;
    }
    if( has( flags, DescFlags::TSS ) ) {
        desc.u1.flags_sys.type = 1;
//        desc.u1.flags_sys.gate = 0;
        desc.u1.flags_sys.use32 = 1;
        desc.u1.flags.present = 1;
    }
    if( has( flags, DescFlags::IS_CODE ) ) {
        desc.u1.flags.execute = 1;
    }
    if( has( flags, DescFlags::IS_APP ) ) {
        desc.u1.flags.nonsystem = 1;
    }
    desc.u2.val = 0;
    if( has( flags, DescFlags::IS_USER ) ) {
        desc.u2.flags.use32 = 1;
    }
    if( limit > 0 )
        limit--;
    if( limit >= _1M ) {
        limit >>= 12;
        desc.u2.flags.page_granular = 1;
    }
    SET_DESC_LIMIT( desc, limit );
    WriteLoad(link,  &desc, sizeof( descriptor ) );
}

static unsigned_32 WritePharSegData(Link& link)
/*****************************************/
{
    TSS                 tss;
    group_entry *       group;
    unsigned            pos;
    unsigned            size;
    DescFlags           flags;

    DEBUG(( DBG_BASE, "Writing data" ));
    OrderGroups(link,  CompareProtSegments );
    tss = TSS{};
    tss.eip = link.StartInfo.addr.off;       // NYI: what about backlink ss0-2 & esp0-2 ?
    tss.esp = link.StackAddr.off;
    tss.cs = link.StartInfo.addr.seg;
    tss.ss = link.StackAddr.seg;
    tss.ds = link.DataGroup->addr.seg;
    tss.ldt = 0x28;
    WriteLoad(link,  &tss, sizeof( TSS ) );
    WriteDescriptor(link,  0, 0, DescFlags::none );                                // nullptr GDT entry;
    WriteDescriptor(link,  0, sizeof( TSS ), DescFlags::TSS );  // TSS
    WriteDescriptor(link,  0, sizeof( TSS ), DescFlags::BASE | DescFlags::IS_APP ); // TSS alias
    pos = sizeof( TSS );
    size = NUM_GDT_DESCRIPTORS * sizeof( descriptor );
    WriteDescriptor(link,  pos, size, DescFlags::BASE | DescFlags::IS_APP );      // GDT
    pos += size;
    size = NUM_IDT_DESCRIPTORS * sizeof( descriptor );
    WriteDescriptor(link,  pos, size, DescFlags::BASE | DescFlags::IS_APP );      // IDT
    pos += size;
    size = ( link.NumGroups + 1 ) * sizeof( descriptor );
    WriteDescriptor(link,  pos, size, DescFlags::BASE );                  // LDT
    WriteDescriptor(link,  pos, size, DescFlags::BASE | DescFlags::IS_APP );      // LDT alias
    WriteDescriptor(link,  0, 0, DescFlags::none );                                // nullptr IDT entry;
    WriteDescriptor(link,  0, 0, DescFlags::none );                                // nullptr LDT entry;
    pos += size;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        flags = DescFlags::BASE | DescFlags::IS_APP | DescFlags::IS_USER;
        if( (group->segflags & SEG_DATA) == 0 ) {
            flags |= DescFlags::IS_CODE;
        }
        WriteDescriptor(link,  pos, group->totalsize, flags );
        pos += group->totalsize;
    }
    for( group = link.Groups; group != nullptr; group = group->next ) {
        WriteGroupLoad(link,  group, false );
        if( group->totalsize > group->size ) {  // phar lap is stupid
            PadLoad(link,  group->totalsize - group->size );
        }
    }
    return( pos );
}

static unsigned_32  WriteRTPBlock(Link& link)
/***************************************/
{
    symbol          *sym;
    rtpblock        rtpblk;
    unsigned_32     offset;

    if( link.FmtData.u.phar.breaksym != nullptr ) {
        sym = FindISymbol(link,  link.FmtData.u.phar.breaksym );
        if( sym == nullptr ) {
            LnkMsg(link,  WRN+MSG_BREAKSYM_NOT_FOUND, "s", link.FmtData.u.phar.breaksym );
            offset = 0;
        } else {
            offset = sym->addr.off;
        }
    } else {
        offset = link.FmtData.u.phar.realbreak;
    }
    _HostU16toTarg( RTP_SIGNATURE, rtpblk.signature );
    _HostU16toTarg( link.FmtData.u.phar.minreal, rtpblk.minreal );
    _HostU16toTarg( link.FmtData.u.phar.maxreal, rtpblk.maxreal );
    _HostU16toTarg( link.FmtData.u.phar.minibuf, rtpblk.minibuf );
    _HostU16toTarg( link.FmtData.u.phar.maxibuf, rtpblk.maxibuf );
    _HostU16toTarg( link.FmtData.u.phar.nistack, rtpblk.nistack );
    _HostU16toTarg( link.FmtData.u.phar.istksize, rtpblk.istksize );
    _HostU16toTarg( link.FmtData.u.phar.callbufs, rtpblk.callbufs );
    _HostU32toTarg( offset, rtpblk.realbreak );
    _HostU16toTarg( link.FmtData.u.phar.extender_flags, rtpblk.extender_flags );  /* for undocumented "runtime flags" */
    _HostU16toTarg( link.FmtData.u.phar.unpriv, rtpblk.unpriv );
    WriteLoad(link,  &rtpblk, sizeof( rtpblk ) );
    PadLoad(link,  RTP_SIZE - sizeof( rtpblk ) );
    return( RTP_SIZE );
}

static unsigned_32 WriteSIT(Link& link)
/*********************************/
{
    group_entry *       group;
    seg_info_table      sit;
    unsigned_32         size;

    size = 0;
    sit.flags = 0;
    sit.base = 0;
    sit.extra = 0;
    sit.base = link.FmtData.base;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        sit.selector = group->addr.seg;
        sit.extra = group->totalsize - group->size;
        WriteLoad(link,  &sit, sizeof( seg_info_table ) );
        size += sizeof( seg_info_table );
    }
    return( size );
}

static void WritePharExtended(Link& link,  unsigned_32 start )
/************************************************/
{
    extended_header header;
    unsigned_32     file_size;
    unsigned_32     image_offset;
    unsigned_32     temp;
    unsigned_32     extra;

    SeekLoad(link,  start + HEAD_SIZE );
    file_size = HEAD_SIZE;
    if( link.FmtData.type & MK_PHAR_MULTISEG ) {
        /*
         * write sit info
         */
        _HostU32toTarg( file_size, header.sit_offset );
        temp = WriteSIT(link);
        _HostU32toTarg( temp, header.sit_size );
        file_size += temp;
        _HostU16toTarg( sizeof( seg_info_table ), header.sit_entry_size );
        /*
         * write reloc info
         */
        _HostU32toTarg( file_size, header.reloc_offset );
        temp = WritePharRelocs(link);
        _HostU32toTarg( temp, header.reloc_size );
        file_size += temp;
        /*
         * write rtp block
         */
        _HostU32toTarg( file_size, header.rtp_offset );
        temp = WriteRTPBlock(link);
        _HostU32toTarg( temp, header.rtp_size );
        file_size += temp;
    } else {
        /*
         * write sit info
         */
        _HostU32toTarg( 0, header.sit_offset );
        _HostU32toTarg( 0, header.sit_size );
        _HostU16toTarg( 0, header.sit_entry_size );
        /*
         * write rtp block
         */
        _HostU32toTarg( file_size, header.rtp_offset );
        temp = WriteRTPBlock(link);
        _HostU32toTarg( temp, header.rtp_size );
        file_size += temp;
        /*
         * write reloc info
         */
        _HostU32toTarg( file_size, header.reloc_offset );
        _HostU32toTarg( 0, header.reloc_size );
//        file_size += 0;
    }
    /*
     * write image data
     */
    image_offset = file_size;
    if( link.FmtData.type & MK_PHAR_MULTISEG ) {
        file_size += WritePharSegData(link);
    } else {
        file_size += WritePharData(link,  start + image_offset );
    }
    DBIWrite(link);
    _HostU16toTarg( EXTENDED_SIGNATURE, header.signature );
    if( link.FmtData.type & MK_PHAR_MULTISEG ) {
        _HostU16toTarg( PHAR_FORMAT_SEGMENTED, header.format_level );
    } else {
        _HostU16toTarg( PHAR_FORMAT_FLAT, header.format_level );
    }
    _HostU16toTarg( HEAD_SIZE, header.header_size );
    _HostU32toTarg( file_size, header.file_size );
    _HostU16toTarg( 0, header.checksum );
    _HostU32toTarg( image_offset, header.load_offset );
    _HostU32toTarg( file_size - image_offset, header.load_size );
    _HostU32toTarg( 0, header.sym_offset );
    _HostU32toTarg( 0, header.sym_size );
    if( link.FmtData.type & MK_PHAR_MULTISEG ) {
        temp = sizeof( TSS );
        _HostU32toTarg( temp, header.gdt_offset );
        extra = NUM_GDT_DESCRIPTORS * sizeof( descriptor );
        _HostU32toTarg( extra, header.gdt_size );
        temp += extra;
        extra = NUM_IDT_DESCRIPTORS * sizeof( descriptor );
        _HostU32toTarg( temp, header.idt_offset );
        _HostU32toTarg( extra, header.idt_size );
        temp += extra;
        extra = (link.NumGroups + 1) * sizeof( descriptor );
        _HostU32toTarg( temp, header.ldt_offset );
        _HostU32toTarg( extra, header.ldt_size );
        _HostU32toTarg( 0, header.tss_offset );
        _HostU32toTarg( sizeof( TSS ), header.tss_size );
        _HostU32toTarg( 0, header.min_extra );
        _HostU32toTarg( 0, header.max_extra );
        _HostU16toTarg( link.StackAddr.seg, header.SS );
        _HostU16toTarg( link.StartInfo.addr.seg, header.CS );
        _HostU16toTarg( 0x28, header.LDT );
        _HostU16toTarg( 0x8, header.TSS );
    } else {
        _HostU32toTarg( 0, header.gdt_offset );
        _HostU32toTarg( 0, header.gdt_size );
        _HostU32toTarg( 0, header.idt_offset );
        _HostU32toTarg( 0, header.idt_size );
        _HostU32toTarg( 0, header.ldt_offset );
        _HostU32toTarg( 0, header.ldt_size );
        _HostU32toTarg( 0, header.tss_offset );
        _HostU32toTarg( 0, header.tss_size );
        extra = MemorySize(link) - file_size + image_offset;
        temp = link.FmtData.u.phar.mindata + extra;
        if( temp < link.FmtData.u.phar.mindata )
            temp = 0xffffffff;
        _HostU32toTarg( temp, header.min_extra );
        temp = link.FmtData.u.phar.maxdata + extra;
        if( temp < link.FmtData.u.phar.maxdata )
            temp = 0xffffffff;
        _HostU32toTarg( temp, header.max_extra );
        _HostU16toTarg( 0, header.SS );
        _HostU16toTarg( 0, header.CS );
        _HostU16toTarg( 0, header.LDT );
        _HostU16toTarg( 0, header.TSS );
    }
    _HostU32toTarg( link.FmtData.base, header.offset );
    _HostU32toTarg( link.StackAddr.off, header.ESP );
    _HostU32toTarg( link.StartInfo.addr.off, header.EIP );
    _HostU16toTarg( 0, header.flags );    // packing not yet implemented.
    _HostU32toTarg( 0, header.reserved1 );
    _HostU32toTarg( link.StackSize, header.stack_size );
    header.mem_req = header.load_size;
    SeekLoad(link,  start );
    WriteLoad(link,  &header, sizeof( extended_header ) );
    PadLoad(link,  HEAD_SIZE - sizeof( extended_header ) );
}

void FiniPharLapLoadFile(Link& link)
/*************************************/
{
    unsigned_32 start;

    start = AppendToLoadFile(link,  link.FmtData.u.phar.stub );
    if( link.FmtData.type & (MK_PHAR_FLAT | MK_PHAR_MULTISEG) ) {
        WritePharExtended(link,  start );
    } else {
        WritePharSimple(link,  start );
    }
}

void CheckPharLapData(Link& link)
/***************************/
{
    if( (link.FmtData.type & MK_PHAR_FLAT) && (link.LinkState & LS_HAVE_16BIT_CODE) && !DirectivesHaveRealBreak() ) {
        LnkMsg(link,  WRN+MSG_NO_REALBREAK_WITH_16BIT, nullptr );
    }
}

#endif
