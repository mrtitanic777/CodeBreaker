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
* Description:  routines for producing codeview information in the load file
*
****************************************************************************/


#include <array>
#include <cstring>
#include <string_view>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "exepe.h"
#include "cv4w.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/formats/qnx.h"    // for ToQNXIndex
#include "build/x86link/base/ring.h"
#include "build/x86link/debuginfo/dbgcv.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/link/run_state.hpp"


// What wlink keeps of a module's CodeView information (objstruc.h names it CVMODINFO).
struct cvmodinfo {
    unsigned_32         pubsize;
    virt_mem            segloc;
    unsigned_16         numlines;
    unsigned_16         modidx;
    unsigned_16         numsegs;
};

#include "pushpck1.h"

// Cheesy implementation of sstSrcModule structures.
// Only allow one file and one segment.
struct offset_range {
    unsigned_32         start;
    unsigned_32         end;
};

struct cheesy_module_header {
    unsigned_16         cFile;
    unsigned_16         cSeg;
    unsigned_32         baseSrcFile[1];
    offset_range        range[1];
    unsigned_16         seg[1];
    unsigned_16         pad;
};

struct cheesy_file_table {
    unsigned_16         cSeg;
    unsigned_16         pad;
    unsigned_32         baseSrcLn[1];
    offset_range        range[1];
    unsigned_8          name[1];        /* variable sized */
};

struct cheesy_mapping_table {
    unsigned_16         Seg;
    unsigned_16         cPair;
//  unsigned_32         offset[];
//  unsigned_32         linenumber[];
};

#include "poppck.h"

namespace {

// global information needed to keep track of line number information

struct cvlineinfo {
    virt_mem            linestart;
    virt_mem            offbase;
    virt_mem            numbase;
    offset_range        range;
    unsigned_32         prevaddr;
    unsigned_16         seg;
    bool                needsort;
};

// split codeview up into a number of different "sections" to keep track
// of where we write bits of the information.

enum class CvSect {
    module,
    misc,
    moddir,
    directory,
};
constexpr std::size_t kNumCvSects = 4;

// the codeview information is just one big memory block (cv_base is the start of it); the sections
// keep track of where to write bits of information, and are used in pass 1 to collect the sizes of
// the different blocks.

struct CvState {
    std::array<virt_mem_ptr, kNumCvSects> sect_addrs;
    virt_mem        cv_base;
    unsigned        temp_index;
    cvlineinfo      line_info;

    virt_mem_ptr& at( CvSect s ) { return sect_addrs[static_cast<std::size_t>( s )]; }
};

CvState& state()
{
    return cb_wlink::RunState::part<CvState>();
}

}  // namespace



void CVInit( void )
/*****************/
// called just after command file parsing
{
    state() = CvState{};
}

void CVInitModule( mod_entry *obj )
/*********************************/
// called before pass 1 is done on the module
{
    obj->u3.cv = static_cast<CVMODINFO *>( _PermAlloc( sizeof( cvmodinfo ) ) );
    std::memset( obj->u3.cv, 0, sizeof( cvmodinfo ) );
}

static void DumpInfo( CvSect sect, const void *data, size_t len )
/***************************************************************/
{
    virt_mem_ptr &addr = state().at( sect );

    PutInfo( addr.vm_ptr, data, len );
    addr.vm_ptr += len;
}

static void DumpInfoU8( CvSect sect, unsigned_8 data )
/****************************************************/
{
    DumpInfo( sect, &data, sizeof( data ) );
}

static void DumpInfoU32( CvSect sect, unsigned_32 data )
/******************************************************/
{
    DumpInfo( sect, &data, sizeof( data ) );
}

static unsigned_16 GetCVSegment(Link& link,  seg_leader *seg )
/************************************************/
{
    unsigned_16     index;
    group_entry     *group;

    if( ( seg == nullptr )
      || ( seg->group == nullptr ) ) {
        return( 0 );
    }
    if( link.FmtData.type & (MK_REAL_MODE | MK_FLAT_OFFS | MK_ID_SPLIT) ) {
        for( index = 1, group = link.Groups; group != nullptr; group = group->next, ++index ) {
            if( group == seg->group ) {
                return( index );
            }
        }
        return( 0 );
    }
#ifdef _QNX
    if( link.FmtData.type & MK_QNX ) {
        return( ToQNXIndex( seg->seg_addr.seg ) );
    }
#endif
    return( seg->seg_addr.seg );
}

static void AddSubSection( bool ismodule )
/****************************************/
{
    CvSect sect;

    if( ismodule ) {
        sect = CvSect::moddir;
    } else {
        sect = CvSect::directory;
    }
    state().at( sect ).vm_offs += sizeof( cv_directory_entry );
}

static void GenSubSection(Link& link,  sst sect, unsigned_32 size )
/*****************************************************/
// generate a subsection entry
{
    CvState             &st = state();
    cv_directory_entry  entry;

    entry.subsection = sect;
    if( link.CurrMod == nullptr ) {
        entry.iMod = 0xFFFF;
    } else {
        entry.iMod = link.CurrMod->u3.cv->modidx;
    }
    entry.cb = size;
    if( sect == sstModule ) {
        entry.lfo = st.at( CvSect::module ).vm_ptr - st.cv_base;
        DumpInfo( CvSect::moddir, &entry, sizeof( cv_directory_entry ) );
    } else {
        entry.lfo = st.at( CvSect::misc ).vm_ptr - st.cv_base;
        DumpInfo( CvSect::directory, &entry, sizeof( cv_directory_entry ) );
    }
}

void CVP1ModuleScanned( void )
/****************************/
{
}

static void CVAddLines(Link& link,  lineinfo *info )
/**************************************/
// called during pass 1 linnum processing
{
    link.CurrMod->u3.cv->numlines += DBICalcLineQty( info );
}

void CVP1ModuleFinished( mod_entry *obj )
/***************************************/
// calculate size of the sstModule
{
    Link& link = wlink();
    CvState     &st = state();
    size_t      namelen;
    unsigned_32 temp;
    unsigned_32 size;

    if( MOD_NOT_DEBUGGABLE( obj ) )
        return;
    st.temp_index++;
    link.CurrMod = obj;
    obj->u3.cv->modidx = st.temp_index;
    if( link.CurrMod->flags_dbi & DBI_LINE ) {
        DBILineWalk( reinterpret_cast<lineinfo *>( obj->lines ), bind_link( link, CVAddLines ) );
    }
    Ring2Walk( obj->publist, bind_link( link, DBIModGlobal ) );
    namelen = std::string_view( obj->name.u.ptr ).size();
    //  required alignment ???
    size = sizeof( cv_sst_module ) + namelen + 1 - sizeof( cv_seginfo );
    size = __ROUND_UP_SIZE_DWORD( size );
    st.at( CvSect::module ).vm_offs += size;
    //  required alignment ???
    AddSubSection( true );
    if( obj->u3.cv->pubsize > 0 ) {
        AddSubSection( false );
        st.at( CvSect::misc ).vm_offs += sizeof( unsigned_32 );
        obj->u3.cv->pubsize += sizeof( unsigned_32 );
    }
    if( obj->u3.cv->numlines > 0 ) {
        AddSubSection( false );
        temp = sizeof( cheesy_module_header );
        temp += __ROUND_UP_SIZE_DWORD( sizeof( cheesy_file_table ) + namelen );
        temp += sizeof( cheesy_mapping_table );
        temp += __ROUND_UP_SIZE_DWORD( obj->u3.cv->numlines * ( sizeof( unsigned_32 ) + sizeof( unsigned_16 ) ) );
        st.at( CvSect::misc ).vm_offs += temp;
    }
}

void CVAddModule(Link& link,  mod_entry *obj, section *sect )
/***********************************************/
// called just before publics have been assigned addresses between p1 & p2
{
    CvState             &st = state();
    cv_sst_module       mod;
    unsigned_32         size;
    size_t              namelen;

    /* unused parameters */ (void)sect;

    if( obj->u3.cv->pubsize > 0 ) {
        GenSubSection(link,  sstPublicSym, obj->u3.cv->pubsize );
        DumpInfoU32( CvSect::misc, 1 );
    }
    namelen = std::string_view( obj->name.u.ptr ).size();
    size = sizeof( cv_sst_module ) + namelen + 1 + ( obj->u3.cv->numsegs - 1 ) * sizeof( cv_seginfo );
    //  begin padding required ???
    size = __ROUND_UP_SIZE_DWORD( size );
    //  end padding required ???
    GenSubSection(link,  sstModule, size );
    mod.ovlNumber = 0;
    mod.iLib = 0;
    mod.cSeg = obj->u3.cv->numsegs;
    mod.Style = CV_DEBUG_STYLE;
    DumpInfo( CvSect::module, &mod, sizeof( cv_sst_module ) - sizeof( cv_seginfo ) );
    obj->u3.cv->segloc = st.at( CvSect::module ).vm_ptr;
    st.at( CvSect::module ).vm_ptr += sizeof( cv_seginfo ) * obj->u3.cv->numsegs;
    DumpInfoU8( CvSect::module, static_cast<unsigned_8>( namelen ) );
    DumpInfo( CvSect::module, obj->name.u.ptr, namelen );
}

static int RelocCompare( virt_mem a, virt_mem b )
/***********************************************/
{
    unsigned_32 a32;
    unsigned_32 b32;

    GET32INFO( a, a32 );
    GET32INFO( b, b32 );
    return( static_cast<int>( static_cast<unsigned_32>( static_cast<signed_32>( a32 ) - b32 ) ) );
}

static void SwapRelocs( virt_mem a, virt_mem b )
/**********************************************/
{
    const cvlineinfo &line_info = state().line_info;
    virt_mem    diffa;
    virt_mem    diffb;
    unsigned_16 a16;
    unsigned_16 b16;
    unsigned_32 a32;
    unsigned_32 b32;

    GET32INFO( a, a32 );
    GET32INFO( b, b32 );
    PUT32INFO( a, b32 );
    PUT32INFO( b, a32 );
    diffa = line_info.numbase + static_cast<unsigned_32>( ( ( a - line_info.offbase ) / sizeof( unsigned_32 ) ) * sizeof( unsigned_16 ) );
    diffb = line_info.numbase + static_cast<unsigned_32>( ( ( b - line_info.offbase ) / sizeof( unsigned_32 ) ) * sizeof( unsigned_16 ) );
    GET16INFO( diffa, a16 );
    GET16INFO( diffb, b16 );
    PUT16INFO( diffa, b16 );
    PUT16INFO( diffb, a16 );
}

static void SortRelocs(Link& link)
/****************************/
{
    cvlineinfo &line_info = state().line_info;

    line_info.offbase -= link.CurrMod->u3.cv->numlines * sizeof( unsigned_32 );
    line_info.numbase -= link.CurrMod->u3.cv->numlines * sizeof( unsigned_16 );
    VMemQSort( line_info.offbase, link.CurrMod->u3.cv->numlines, sizeof( unsigned_32 ), SwapRelocs, RelocCompare );
}

static void GenSrcModHeader(Link& link)
/*********************************/
// emit header for line number information now that we know where everything
// is.
{
    cvlineinfo                  &line_info = state().line_info;
    cheesy_module_header        mod_hdr;
    cheesy_file_table           file_tbl;
    cheesy_mapping_table        map_tbl;
    unsigned                    adjust;

    if( line_info.linestart == 0 )
        return;
    std::memset( &mod_hdr, 0, sizeof( mod_hdr ) );
    mod_hdr.cFile = 1;
    mod_hdr.cSeg = 1;
    mod_hdr.range[0] = line_info.range;
    mod_hdr.baseSrcFile[0] = sizeof( mod_hdr );
    mod_hdr.seg[0] = line_info.seg;
    mod_hdr.pad = 0;
    PutInfo( line_info.linestart, &mod_hdr, sizeof( mod_hdr ) );
    line_info.linestart += sizeof( mod_hdr );
    file_tbl.cSeg = 1;
    file_tbl.pad = 0;
    file_tbl.range[0] = line_info.range;
    file_tbl.name[0] = static_cast<unsigned_8>( std::string_view( link.CurrMod->name.u.ptr ).size() );
    file_tbl.baseSrcLn[0] = sizeof( mod_hdr ) + __ROUND_UP_SIZE_DWORD( sizeof( file_tbl ) + file_tbl.name[0] );
    PutInfo( line_info.linestart, &file_tbl, sizeof( file_tbl ) );
    line_info.linestart += sizeof( file_tbl );
    PutInfo( line_info.linestart, link.CurrMod->name.u.ptr, file_tbl.name[0] );
    line_info.linestart += file_tbl.name[0];
    adjust = file_tbl.baseSrcLn[0] - sizeof( mod_hdr ) - sizeof( file_tbl ) - file_tbl.name[0];
    if( adjust != 0 ) {
        PutInfoNulls( line_info.linestart, adjust );
        line_info.linestart += adjust;
    }
    map_tbl.Seg = mod_hdr.seg[0];
    map_tbl.cPair = link.CurrMod->u3.cv->numlines;
    PutInfo( line_info.linestart, &map_tbl, sizeof( map_tbl ) );
    line_info = cvlineinfo{};
}

void CVGenModule(Link& link)
/**********************/
// generate an sstSrcModule
{
    if( state().line_info.needsort ) {
        SortRelocs(link);
    }
    GenSrcModHeader(link);
}

void CVAddLocal( seg_leader *seg, offset length )
/***********************************************/
// called during pass 1 final segment processing.
{
    /* unused parameters */ (void)length;

    if( IS_DBG_INFO( seg ) ) {
        AddSubSection( false );
    }
}

void CVAddGlobal(Link& link,  symbol *sym )
/*****************************/
// called during pass 1 symbol definition
{
    size_t  size;

    if( (sym->info & SYM_STATIC) == 0 ) {
        if( ( sym->p.seg == nullptr )
          || IS_SYM_IMPORTED( sym )
          || ( sym->p.seg->bits == BITS_32 ) ) {
            size = sizeof( s_pub32 );
        } else {
            size = sizeof( s_pub16 );
        }
        size = __ROUND_UP_SIZE_DWORD( size + std::string_view( sym->name.u.ptr ).size() + 1 );
        link.CurrMod->u3.cv->pubsize += size;
        state().at( CvSect::misc ).vm_offs += size;
    }
}

void CVGenGlobal(Link& link,  symbol *sym, section *sect )
/********************************************/
// called during symbol address calculation (between pass 1 & pass 2)
// also called by loadpe between passes
{
    s_pub16     pub16;
    s_pub32     pub32;
    unsigned    size;
    unsigned    pad;
    unsigned_32 buf;
    size_t      namelen;

    /* unused parameters */ (void)sect;

    if( sym->info & SYM_STATIC )
        return;
    namelen = std::string_view( sym->name.u.ptr ).size();
    size = namelen + 1;

    if( ( sym->p.seg == nullptr )
      || IS_SYM_IMPORTED( sym )
      || ( sym->p.seg->bits == BITS_32 ) ) {
        size += sizeof( s_pub32 );
        pub32.common.length = __ROUND_UP_SIZE_DWORD( size );
        pad = pub32.common.length - size;
        pub32.common.length -= 2;
        pub32.common.code = S_PUB32;
        pub32.f.offset = sym->addr.off;
        pub32.f.segment = GetCVSegment(link,  sym->p.seg->u.leader );
        pub32.f.type = 0;
        DumpInfo( CvSect::misc, &pub32, sizeof( s_pub32 ) );
    } else {
        size += sizeof( s_pub16 );
        pub16.common.length = __ROUND_UP_SIZE_DWORD( size );
        pad = pub16.common.length - size;
        pub16.common.length -= 2;
        pub16.common.code = S_PUB16;
        pub16.f.offset = sym->addr.off;
        pub16.f.segment = GetCVSegment(link,  sym->p.seg->u.leader );
        pub16.f.type = 0;
        DumpInfo( CvSect::misc, &pub16, sizeof( s_pub16 ) );
    }
    DumpInfoU8( CvSect::misc, static_cast<unsigned_8>( namelen ) );
    DumpInfo( CvSect::misc, sym->name.u.ptr, namelen );
    if( pad > 0 ) {
        buf = 0;
        DumpInfo( CvSect::misc, &buf, pad );
    }
}

void CVGenLines(Link& link,  lineinfo *info )
/*******************************/
// called during pass 2 linnum processing
{
    CvState                     &st = state();
    cvlineinfo                  &line_info = st.line_info;
    ln_off_pair _WCUNALIGNED    *pair;
    unsigned_32                 temp_off;
    unsigned_16                 temp_num;
    offset                      adjust;
    unsigned long               cvsize;
    unsigned                    size;
    unsigned                    item_size;
    segdata                     *seg;

    seg = info->seg;

    if( (link.CurrMod->flags_dbi & DBI_LINE) == 0 )
        return;
    adjust = seg->a.delta + seg->u.leader->seg_addr.off;
    if( line_info.offbase == 0 ) { // this is our first time through.
        virt_mem_ptr &misc = st.at( CvSect::misc );

        line_info.seg = GetCVSegment(link,  seg->u.leader );
        line_info.linestart = misc.vm_ptr;
        cvsize = sizeof( cheesy_module_header ) + sizeof( cheesy_mapping_table )
            + __ROUND_UP_SIZE_DWORD( sizeof( cheesy_file_table ) + std::string_view( link.CurrMod->name.u.ptr ).size() );
        line_info.offbase = misc.vm_ptr + cvsize;
        line_info.numbase = line_info.offbase + link.CurrMod->u3.cv->numlines * sizeof( unsigned_32 );
        cvsize += link.CurrMod->u3.cv->numlines * sizeof( unsigned_32 );
        cvsize += __ROUND_UP_SIZE_DWORD( link.CurrMod->u3.cv->numlines * sizeof( unsigned_16 ) );
        GenSubSection(link,  sstSrcModule, cvsize );
        misc.vm_ptr += cvsize;
        line_info.range.start = adjust;
        line_info.range.end = adjust + seg->length;
    } else {
        if( line_info.range.start > adjust ) {
            line_info.range.start = adjust;
        }
        if( line_info.range.end < adjust + seg->length ) {
            line_info.range.end = adjust + seg->length;
        }
    }
    if( info->size & LINE_IS_32BIT ) {
        item_size = sizeof( ln_off_386 );
    } else {
        item_size = sizeof( ln_off_286 );
    }
    pair = reinterpret_cast<ln_off_pair *>( info->data );
    for( size = info->size & ~LINE_IS_32BIT; size > 0; size -= item_size ) {
        if( info->size & LINE_IS_32BIT ) {
            pair->_386.off += adjust;
            if( pair->_386.off < line_info.prevaddr ) {
                line_info.needsort = true;
            }
            line_info.prevaddr = pair->_386.off;
            _HostU32toTarg( pair->_386.off, temp_off );
            _HostU16toTarg( pair->_386.linnum, temp_num );
        } else {
            pair->_286.off += adjust;
            if( pair->_286.off < line_info.prevaddr ) {
                line_info.needsort = true;
            }
            line_info.prevaddr = pair->_286.off;
            _HostU16toTarg( pair->_286.off, temp_off );
            _HostU16toTarg( pair->_286.linnum, temp_num );
        }
        PutInfo( line_info.offbase, &temp_off, sizeof( unsigned_32 ) );
        PutInfo( line_info.numbase, &temp_num, sizeof( unsigned_16 ) );
        line_info.offbase += sizeof( unsigned_32 );
        line_info.numbase += sizeof( unsigned_16 );
        pair = reinterpret_cast<ln_off_pair *>( reinterpret_cast<char *>( pair ) + item_size );
    }
}

static void CVAddAddrInit( segdata *sdata, void *cookie )
/*******************************************************/
{
    /* unused parameters */ (void)sdata; (void)cookie;
}

static void CVAddAddrAdd( segdata *sdata, offset delta, offset size, void *cookie, bool isnewmod )
/************************************************************************************************/
{
    /* unused parameters */ (void)delta; (void)size; (void)cookie;

    if( isnewmod ) {
        sdata->o.mod->u3.cv->numsegs++;
        state().at( CvSect::module ).vm_offs += sizeof( cv_seginfo );
    }
}

void CVAddAddrInfo( seg_leader *seg )
/***********************************/
{
    Link& link = wlink();
    if( seg->info & SEGINF_CODE ) {
        DBIAddrInfoScan(link,  seg, CVAddAddrInit, CVAddAddrAdd, nullptr );
    }
}

static void CVGenAddrInit(Link& link,  segdata *sdata, void *_info )
/******************************************************/
{
    cv_seginfo *info = static_cast<cv_seginfo *>( _info );
    info->Seg = GetCVSegment(link,  sdata->u.leader );
    info->pad = 0;
    info->offset = sdata->u.leader->seg_addr.off + sdata->a.delta;
}

static void CVGenAddrAdd( segdata *sdata, offset delta, offset size, void *_info, bool isnewmod )
/***********************************************************************************************/
{
    cv_seginfo *info = static_cast<cv_seginfo *>( _info );
    if( isnewmod ) {
        info->cbSeg = size;
        PutInfo( sdata->o.mod->u3.cv->segloc, info, sizeof( cv_seginfo ) );
        sdata->o.mod->u3.cv->segloc += sizeof( cv_seginfo );
        info->offset = sdata->u.leader->seg_addr.off + delta;
    }
}

void CVGenAddrInfo(Link& link,  seg_leader *seg )
/***********************************/
{
    cv_seginfo          info;

    if( seg->info & SEGINF_CODE ) {
        DBIAddrInfoScan(link,  seg, bind_link( link, CVGenAddrInit ), CVGenAddrAdd, &info );
    }
}

void CVDefClass(Link& link,  class_entry *class_, unsigned_32 size )
/*****************************************************/
{
    group_entry *group;

    if( class_->flags & (CLASS_DWARF | CLASS_HLL_LINE) )
        return;
    state().at( CvSect::misc ).vm_offs += size;
    group = AllocGroup(link,  AutoGrpName, &link.DBIGroups );
    group->g.class_ = class_;
    group->addr.seg = 0;
}

static void DefLocal(Link& link,  void *_sdata )
/**********************************/
/* NOTE: this assumes that codeview segments are byte aligned! */
{
    virt_mem_ptr &misc = state().at( CvSect::misc );
    segdata    *sdata = static_cast<segdata *>( _sdata );
    seg_leader *leader;
    sst         sect;

    leader = sdata->u.leader;
    if( !sdata->isdead
      && IS_DBG_INFO( leader ) ) {
        if( leader->dbgtype == MS_TYPE ) {
            sect = sstTypes;
        } else {
            sect = sstSymbols;
        }
        link.CurrMod = sdata->o.mod;
        GenSubSection(link,  sect, sdata->length );
        CopyInfo( misc.vm_ptr, sdata->u1.vm_ptr, sdata->length );
        sdata->u1.vm_ptr = misc.vm_ptr;   // FIXME: inefficient
        misc.vm_ptr += sdata->length;
    }
}

static bool DefLeader( void *_leader, void *group )
/*************************************************/
{
    Link& link = wlink();
    seg_leader *leader = static_cast<seg_leader *>( _leader );

    leader->group = static_cast<group_entry *>( group );
    RingWalk( leader->pieces, bind_link( link, DefLocal ) );
    return( false );
}

void CVAddrStart(Link& link)
/**********************/
{
    CvState                 &st = state();
    cv_subsection_directory dir;
    cv_trailer              start;
    virt_mem                currpos;
    unsigned_32             size;
    unsigned_32             numentries;
    group_entry             *group;

    AddSubSection( false );     // for sstSegMap
    numentries = ( st.at( CvSect::moddir ).vm_offs + st.at( CvSect::directory ).vm_offs ) / sizeof( cv_directory_entry );
    st.at( CvSect::misc ).vm_offs += sizeof( cv_sst_seg_map ) + ( link.NumGroups - 1 ) * sizeof( seg_desc );
    st.at( CvSect::moddir ).vm_offs += sizeof( cv_subsection_directory );
    link.CVSize = 2 * sizeof( cv_trailer );
    for( virt_mem_ptr &addr : st.sect_addrs ) {
        link.CVSize += addr.vm_offs;
    }
    st.cv_base = DBIAlloc( link.CVSize );
    // The block is not cleared by its allocator, and some of it is never written: the padding
    // that pass 1 reserves after each module name (dword rounding) and after an odd number of
    // line numbers (the 16-bit numbers rounded up to a dword). wlink.exe leaves it zero.
    PutInfoNulls( st.cv_base, link.CVSize );
    currpos = st.cv_base + sizeof( cv_trailer );
    for( virt_mem_ptr &addr : st.sect_addrs ) {
        size = addr.vm_offs;
        addr.vm_ptr = currpos;
        currpos += size;
    }
    std::memcpy( start.sig, CV4_NB05, CV_SIG_SIZE );
    start.offset = st.at( CvSect::moddir ).vm_ptr - st.cv_base;
    PutInfo( st.cv_base, &start, sizeof( cv_trailer ) );
    start.offset = link.CVSize;
    PutInfo( st.cv_base + link.CVSize - sizeof( cv_trailer ), &start, sizeof( cv_trailer ) );
    dir.cbDirHeader = sizeof( cv_subsection_directory );
    dir.cbDirEntry = sizeof( cv_directory_entry );
    dir.cDir = numentries;
    dir.lfoNextDir = 0;
    dir.flags = 0;
    DumpInfo( CvSect::moddir, &dir, sizeof( cv_subsection_directory ) );
    for( group = link.DBIGroups; group != nullptr; group = group->next ) {
        RingLookup( group->g.class_->segs, DefLeader, group );
        group->g.class_ = nullptr;
    }
}

void CVFini(Link& link,  section *sect )
/**************************/
// called after pass 2 is finished, but before load file generation
{
    cv_sst_seg_map      map;
    seg_desc            desc;
    group_entry         *group;
    seg_leader          *leader;

    if( sect != link.Root )
        return;
    link.CurrMod = nullptr;
    GenSubSection(link,  sstSegMap, sizeof( cv_sst_seg_map ) + ( link.NumGroups - 1 ) * sizeof( seg_desc ) );
    map.cSeg = link.NumGroups;
    map.cSegLog = link.NumGroups;
    DumpInfo( CvSect::misc, &map, sizeof( cv_sst_seg_map ) - sizeof( seg_desc ) );
    std::memset( &desc, 0, sizeof( seg_desc ) );
    desc.u.b.fSel = true;
    desc.u.b.fRead = true;
    desc.iSegName = 0xFFFF;
    desc.iClassName = 0xFFFF;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        desc.frame = group->addr.seg;
        desc.offset = group->addr.off;
        desc.cbseg = group->totalsize;
        if( (group->segflags & SEG_DATA) == 0 ) {
            desc.u.b.fExecute = true;
        } else {
            desc.u.b.fExecute = false;
        }
        if( (group->segflags & (SEG_DATA | SEG_READ_ONLY)) == SEG_DATA ) {
            desc.u.b.fWrite = true;
        } else {
            desc.u.b.fWrite = false;
        }
        leader = static_cast<seg_leader *>( Ring2First( group->leaders ) );
        if( leader->info & SEGINF_USE_32 ) {
            desc.u.b.f32Bit = true;
        }
//        desc.u.b.fSel = true;
        DumpInfo( CvSect::misc, &desc, sizeof( seg_desc ) );
    }
}

void CVWriteDebugTypeMisc(Link& link,  const char *filename )
/***********************************************/
// called during load file generation.  It is assumed that the loadfile is
// positioned to the right spot.
{
    size_t                  namelen;
    size_t                  bufspace;
    debug_misc_dbgdata      dbg_exename;

    std::memset( &dbg_exename, 0, sizeof( dbg_exename ) );
    dbg_exename.data_type = 1;
    dbg_exename.length = sizeof( dbg_exename );
    dbg_exename.unicode = 0;

    if( filename != nullptr ) {
        namelen = std::string_view( filename ).size() + 1;
        bufspace = sizeof( dbg_exename.data ) - 4;
        if( bufspace >= namelen ) {
            std::memcpy( dbg_exename.data, filename, namelen );
        } else {
            LnkMsg( WRN+LOC+MSG_INTERNAL, "s", "filename too long in CVWriteDebugTypeMisc()" );
        }
    }
    dbg_exename.special_purpose = link.CVDebugDirEntryPos;
    WriteLoad(link,  &dbg_exename, sizeof( dbg_exename ) );
}

void CVWrite(Link& link)
/******************/
// called during load file generation.  It is assumed that the loadfile is
// positioned to the right spot.
{
    // write DEBUG_TYPE_CODEVIEW data: CodeView NB05 data
    WriteInfoLoad( state().cv_base, link.CVSize );
}
