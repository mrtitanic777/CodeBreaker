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
* Description:  routines for producing HLL debug information in the load file
*
****************************************************************************/


#include <array>
#include <cstdint>
#include <cstring>
#include <string_view>
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "alloc.h"
#include "hll.h"
#include "virtmem.h"
#include "loadfile.h"
#include "objnode.h"
#include "objcalc.h"
#include "dbgcomm.h"
#include "ring.h"
#include "dbghll.h"
#include "specials.h"
#include "run_state.hpp"

// The memory of the debug information is filled, before anything is written in it, with the digit
// of its section ('0' + the section's number): the bytes nothing writes show it.
constexpr bool kHllFillSections = true;


// What wlink keeps of a module's HLL information (objstruc.h names it HLLMODINFO).
struct hllmodinfo {
    unsigned_32 pubsize;
    /* used when writing segment info.
     * Initialized by HllAddModule. */
    virt_mem    segloc;
    /* Used for advancing from the first to the second seginfo
     * structure. The first seginfo structure is at the head
     * of the module record, while the rest follows the name. */
    virt_mem    next_segloc;
    unsigned_16 numlines;               /* number of lines */
    unsigned_16 numsegs;                /* number of segments */
    unsigned_16 modidx;                 /* the module index */
};

namespace {

// split hll up into a number of different "sections" to keep track
// of where we write bits of the information.

enum class HllSect {
    module,
    misc,
    directory,
};
constexpr std::size_t kNumHllSects = 3;

constexpr std::size_t index_of( HllSect s )
{
    return static_cast<std::size_t>( s );
}

// The HLL information is just one big memory block (hll_base is the start of it, hll_size the
// length of it).

struct HllState {
    // The sizes of the sections hll we've defined above.
    // Collected during the early link phases and used to calculate the
    // actual size of the debug info and where to put things (sect_offsets).
    std::array<unsigned, kNumHllSects>  sect_sizes;
    // Where to write bits bits of information. This is as
    // Calculated by HllAddrStart.
    std::array<virt_mem, kNumHllSects>  sect_offsets;
    virt_mem        hll_base;
    unsigned        hll_size;
    hll_dirinfo     *hll_dir_hdr;
    // The current module index.
    unsigned        mod_index;
};

HllState& state()
{
    return cb_wlink::RunState::part<HllState>();
}

}  // namespace

static void     hllSrcModHeader( void );

void HllInit( void )
/******************/
// called just after command file parsing
{
    DEBUG(( DBG_HLL, "HllInit:" ));

    state() = HllState{};
}

void HllInitModule( mod_entry *obj )
/**********************************/
// called before pass 1 is done on the module
{
    DEBUG(( DBG_HLL, "HllInitModule: obj=%p %s", obj, obj->name.u.ptr ));

    obj->u3.hll = static_cast<HLLMODINFO *>( _PermAlloc( sizeof( hllmodinfo ) ) );
    std::memset( obj->u3.hll, 0, sizeof( hllmodinfo ) );
}

static void hllDumpInfo( HllSect sect, const void *data, unsigned len )
/*********************************************************************/
{
    virt_mem &offset_of_sect = state().sect_offsets[index_of( sect )];

    PutInfo( offset_of_sect, data, len );
    offset_of_sect += len;
}

static segment hllGetSegment( seg_leader *seg )
/*********************************************/
{
    segment         segm_index;
    group_entry     *group;

    if( ( seg != nullptr )
      && ( seg->group != nullptr ) ) {
        if( wlink().FmtData.type & (MK_REAL_MODE | MK_FLAT_OFFS | MK_ID_SPLIT) ) {
            segm_index = 1;
            for( group = wlink().Groups; group != nullptr; group = group->next ) {
                if( group == seg->group )
                    return( segm_index );
                segm_index++;
            }
        } else {
            return( seg->seg_addr.seg );
        }
    }
    return( 0 );
}

static void hllAddSubSection( void )
/**********************************/
// Add a subsection to section directory.
{
    state().sect_sizes[index_of( HllSect::directory )] += sizeof( hll_dir_entry );
}

static void hllGenSubSection( hll_sst sect, unsigned size )
/*********************************************************/
// generate a subsection entry
{
    HllState        &st = state();
    hll_dir_entry   entry;

    entry.subsection = sect;
    entry.iMod = wlink().CurrMod ? wlink().CurrMod->u3.hll->modidx : 0;
    entry.cb = size;
    switch( sect ) {
    case hll_sstModules:
        entry.lfo = st.sect_offsets[index_of( HllSect::module )] - st.hll_base;
        break;
    default:
        entry.lfo = st.sect_offsets[index_of( HllSect::misc )] - st.hll_base;
        break;
    }
    DEBUG(( DBG_HLL, "hllGenSubSection: %h: sst=%h mod=%d cb=%h lfo=%h",
            st.sect_offsets[sect] - st.hll_base, sect, entry.iMod, size, entry.lfo ));
    hllDumpInfo( HllSect::directory, &entry, sizeof( entry ) );
}

void HllP1ModuleScanned( void )
/*****************************/
{
    DEBUG(( DBG_HLL, "HllP1ModuleScanned:" ));
}

void HllP1ModuleFinished( mod_entry *obj )
/*****************************************
 * calculate size of the sstModule
 */
{
    HllState    &st = state();
    byte        namelen;

    if( MOD_NOT_DEBUGGABLE( obj ) )
        return;
    wlink().CurrMod = obj;

    /* assign module index */
    obj->u3.hll->modidx = ++st.mod_index;
    DEBUG(( DBG_HLL, "HllP1ModuleFinished: obj=%p %s modidx=%d offset=%h",
            obj, obj->name.u.ptr, st.mod_index, st.sect_sizes[index_of( HllSect::module )] ));

    /* add the module subsection */
    namelen = static_cast<byte>( std::string_view( obj->name.u.ptr ).size() );
    st.sect_sizes[index_of( HllSect::module )] += sizeof( hll_module ) + namelen;
    hllAddSubSection();

    /* add public symbols subsection */
    Ring2Walk( obj->publist, DBIModGlobal );
    if( obj->u3.hll->pubsize > 0 ) {
        hllAddSubSection();
    }

    // FIXME: Implement support for non-HLL linenumbers (the old-style linenumber subsection).
}

void HllAddModule( mod_entry *obj, section *sect )
/************************************************/
// called just before publics have been assigned addresses between p1 & p2
// We generate the module entry and some other bits now.
{
    HllState        &st = state();
    hll_module      mod;
    unsigned        size;
    byte            namelen;

    DEBUG(( DBG_HLL, "HllAddModule: obj=%p %s sect=%p implib=%d pubsize=%h",
            obj, obj->name.u.ptr, sect, obj->modinfo & MOD_IMPORT_LIB,
            obj->u3.hll->pubsize ));

    wlink().CurrMod = obj; // paranoia

    /* generate the module entry */
    namelen = static_cast<byte>( std::string_view( obj->name.u.ptr ).size() );
    size = sizeof( mod ) + namelen;
    if( obj->u3.hll->numsegs ) {
        size += ( obj->u3.hll->numsegs - 1 ) * sizeof( hll_seginfo );
    }
    hllGenSubSection( hll_sstModules, size );

    obj->u3.hll->segloc = st.sect_offsets[index_of( HllSect::module )];
    mod.SegInfo.Seg     = 0;        /* corrected later if any code */
    mod.SegInfo.offset  = 0;
    mod.SegInfo.cbSeg   = 0;
    mod.ovlNumber       = 0;
    mod.iLib            = 0; /// @todo add library table and indexes into it. (not important)
    mod.cSeg            = obj->u3.hll->numsegs;
    switch( obj->omfdbg ) {
    case OMF_DBG_HLL_03:
        mod.Version = 0x300;
        mod.Style   = HLL_DEBUG_STYLE_HL;
        break;
    case OMF_DBG_HLL_04:
        mod.Version = 0x400;
        mod.Style   = HLL_DEBUG_STYLE_HL;
        break;
    case OMF_DBG_HLL_06:
        mod.Version = 0x600;
        mod.Style   = HLL_DEBUG_STYLE_HL;
        break;
    default:
        mod.Version = 0;
        mod.Style   = 0;
        break;
    }
    mod.name_len = namelen;
    hllDumpInfo( HllSect::module, &mod, sizeof( mod ) );
    hllDumpInfo( HllSect::module, obj->name.u.ptr, mod.name_len );

    /* next comes extra segment info structures. reserve space for them. */
    obj->u3.hll->next_segloc = st.sect_offsets[index_of( HllSect::module )];
    if( obj->u3.hll->numsegs ) {
        st.sect_sizes[index_of( HllSect::module )] += ( obj->u3.hll->numsegs - 1 ) * sizeof( hll_seginfo );
    }

    /* publics (the callers caller will define public afterwards). */
    if( obj->u3.hll->pubsize > 0 ) {
        hllGenSubSection( hll_sstPublics, obj->u3.hll->pubsize );
    }
    (void)sect; // komh: make compiler happy
}

void HllGenModule( void )
/***********************/
// generate an sstSrcModule
{
    DEBUG(( DBG_HLL, "HllGenModule: CurrMod=%p %s", wlink().CurrMod, wlink().CurrMod->name));
    // FIXME: sort the old-style linenumbers (not supported yet)
    hllSrcModHeader();
}

void HllAddLocal( seg_leader *seg, offset length )
/************************************************/
// called during pass 1 final segment processing.
{
    switch( seg->dbgtype) {
    case MS_TYPE:
    case MS_LOCAL:
    case HLL_LINE:
        DEBUG(( DBG_HLL, "HllAddLocal: seg=%h length=%d", seg, length));
        hllAddSubSection();
        break;
    }
    (void)length; // komh: make compiler happy
}

void HllAddGlobal( symbol *sym )
/******************************/
// called during pass 1 symbol definition
{
    if( (sym->info & SYM_STATIC) == 0 ) {
        unsigned size;

        size = static_cast<byte>( std::string_view( sym->name.u.ptr ).size() ) + sizeof( hll_public );
        wlink().CurrMod->u3.hll->pubsize += size;
        state().sect_sizes[index_of( HllSect::misc )] += size;
        DEBUG(( DBG_HLL, "HllAddGlobal: size=%h sym=%p %s",
                size, sym, sym->name ));
    }
}

void HllGenGlobal( symbol *sym, section *sect )
/*********************************************/
// called during symbol address calculation (between pass 1 & pass 2)
// also called by loadpe between passes
{
    if( (sym->info & SYM_STATIC) == 0 ) {
        hll_public  pub;

        DEBUG(( DBG_HLL, "HllGenGlobal: %h: sym=%p %s sect=%p",
                state().sect_offsets[index_of( HllSect::misc )] - state().hll_base, sym, sym->name, sect ));

        pub.offset  = sym->addr.off;
        pub.seg     = hllGetSegment( sym->p.seg->u.leader );
        pub.type    = 0; /** @todo fix the public type index! */
        pub.name_len = static_cast<unsigned_8>( std::string_view( sym->name.u.ptr ).size() );
        hllDumpInfo( HllSect::misc, &pub, sizeof( pub ) );
        hllDumpInfo( HllSect::misc, sym->name.u.ptr, pub.name_len );
    }
    (void)sect;
}

static void hllSrcModHeader( void )
/*********************************/
// emit header for line number information now that we know where everything
// is.
{
    // FIXME: Implement support for non-HLL linenumbers.
}

void HllGenLines( lineinfo *info )
/********************************/
// called during pass 2 linnum processing
{
    // FIXME: Implement support for non-HLL linenumbers.
    segdata             *seg;
    bool                is32bit;
    unsigned            size;

    seg = info->seg;
    // lines = lines;
    size = info->size;
    is32bit = seg->bits == BITS_32;
    DEBUG(( DBG_HLL, "HllGenLines: info=%p seg=%p size=%d is32bit %d",
            info, seg, size, is32bit ));
    (void)is32bit;  // komh: make compiler happy
    (void)size;     // komh: make compiler happy
}

static void hllAddAddrInit( segdata *sdata, void *cookie )
/********************************************************/
{
    (void)sdata; (void)cookie;
}

static void hllAddAddrAdd( segdata *sdata, offset delta, offset size,
                           void *cookie, bool isnewmod )
/*****************************************************/
{
    (void)delta; (void)size; (void)cookie;
    if( !isnewmod )
        return;

    sdata->o.mod->u3.hll->numsegs++;
}

void HllAddAddrInfo( seg_leader *seg )
/************************************/
// We take this opportunity to count seginfo structures.
{
    if( (seg->info & SEGINF_CODE) == 0 )
        return;
    DEBUG(( DBG_HLL, "HllAddAddrInfo: seg=%p %s", seg, seg->segname ));
    DBIAddrInfoScan( seg, hllAddAddrInit, hllAddAddrAdd, nullptr );
}

static void hllGenAddrInit( segdata *sdata, void *_info )
/*******************************************************/
{
    hll_seginfo *info = static_cast<hll_seginfo *>( _info );
    info->Seg = hllGetSegment( sdata->u.leader );
    info->offset = sdata->u.leader->seg_addr.off + sdata->a.delta;
}

static void hllGenAddrAdd( segdata *sdata, offset delta, offset size,
                           void *_info, bool isnewmod )
/*****************************************************/
{
    hll_seginfo *info = static_cast<hll_seginfo *>( _info );
    if( !isnewmod )
        return;
    info->cbSeg = size;
    PutInfo( sdata->o.mod->u3.hll->segloc, info, sizeof( hll_seginfo ) );

    sdata->o.mod->u3.hll->segloc += sizeof( hll_seginfo );
    info->offset = sdata->u.leader->seg_addr.off + delta;
}

void HllGenAddrInfo( seg_leader *seg )
/************************************/
// called for each section just before pass 2 starts
// Fill in seginfo.
{
    hll_seginfo info;

    if( (seg->info & SEGINF_CODE) == 0 )
        return;
    DEBUG(( DBG_HLL, "HllGenAddrInfo: seg=%p %s", seg, seg->segname ));
    DBIAddrInfoScan( seg, hllGenAddrInit, hllGenAddrAdd, &info );
}

void HllDefClass( class_entry *class_, unsigned size )
/*****************************************************
 * called during address calculation for the purpose of catching
 * debug info segment classes.
 */
{
    group_entry *group;
    DEBUG(( DBG_HLL, "HllDefClass: class=%p flags=%h size=%h %s", class_, class_->flags, size, class_->name ));

    switch( class_->flags & CLASS_DEBUG_INFO ) {
    case CLASS_MS_TYPE:
    case CLASS_MS_LOCAL:
    case CLASS_HLL_LINE:
        /* add the class to the DBI groups (we'll traverse them later) */
        group = AllocGroup( AutoGrpName, &wlink().DBIGroups );
        group->g.class_ = class_;
        group->addr.seg = 0;

        DEBUG(( DBG_HLL, "HllDefClass: class=%p %s group=%p flags=%h "
                "offset=%h size=%h", class_, class_->name, group,
                class_->flags, state().sect_sizes[index_of( HllSect::misc )], size ));
        state().sect_sizes[index_of( HllSect::misc )] += size;
        break;
    }
}

static void hllDefSegData( void *_sdata )
/***************************************/
// Creates a Type or Symbol subsection for a module and copies the data into it.
/* NOTE: this assumes that codeview segments are byte aligned! */
{
    segdata    *sdata = static_cast<segdata *>( _sdata );
    seg_leader *leader = sdata->u.leader;

    DEBUG(( DBG_HLL, "hllDefSegData: iMod=%d length=%h align=2**%d isdead=%d dbgtype=%h",
            sdata->o.mod->u3.hll->modidx, sdata->length, sdata->align, sdata->isdead, leader->dbgtype ));

    if( !sdata->isdead ) {
        virt_mem &misc = state().sect_offsets[index_of( HllSect::misc )];
        hll_sst sect;

        switch ( leader->dbgtype ) {
        case MS_TYPE:   sect = hll_sstTypes;   break;
        case MS_LOCAL:  sect = hll_sstSymbols; break;
        case HLL_LINE:  sect = hll_sstHLLSrc;  break;
        default:
            return;
        }
        wlink().CurrMod = sdata->o.mod;
        DEBUG(( DBG_HLL, "hllDefSegData: %h: iMod=%d sect=%d length=%h align=2**%d %s",
                misc - state().hll_base, wlink().CurrMod->u3.hll->modidx, sect, sdata->length,
                sdata->align, wlink().CurrMod->name ));

        hllGenSubSection( sect, sdata->length );
        CopyInfo( misc, sdata->u1.vm_ptr, sdata->length );
        sdata->u1.vm_ptr = misc;   // FIXME: inefficient
        misc += sdata->length;
    }
}

static bool hllDefLeader( void *_leader, void *group )
/****************************************************/
{
    seg_leader *leader = static_cast<seg_leader *>( _leader );
    DEBUG((DBG_HLL, "hllDefLeader: leader=%p %s", leader, leader->segname ));

    leader->group = static_cast<group_entry *>( group );
    RingWalk( leader->pieces, hllDefSegData );
    return( false );
}

void HllAddrStart( void )
/***********************/
// called after address calculation is done.
{
    HllState            &st = state();
    hll_dirinfo         dirhdr;
    hll_trailer         start;
    virt_mem            currpos;
    group_entry         *group;
    DEBUG((DBG_HLL, "HllAddrStart" ));

    /* calc the debug info size */
    st.sect_sizes[index_of( HllSect::directory )] += sizeof( dirhdr );
    st.hll_size = 2 * sizeof( start );
    for( unsigned sect_size : st.sect_sizes ) {
        st.hll_size += sect_size;
    }

    /* Allocate memory for the debug info. */
    st.hll_base = DBIAlloc( st.hll_size );

    /* calc the position of the section. */
    currpos = st.hll_base + sizeof( start );
    for( std::size_t index = 0; index < kNumHllSects; index++ ) {
        DEBUG (( DBG_HLL, "HllAddrStart: %d: off=%h size=%h",
                 index, currpos - st.hll_base, st.sect_sizes[index] ));
        st.sect_offsets[index] = currpos;
        currpos += st.sect_sizes[index];
        if( kHllFillSections ) {
            std::memset( reinterpret_cast<void *>( st.sect_offsets[index] ),
                         static_cast<int>( '0' + index ), st.sect_sizes[index] );    /* fill */
        }
    }

    /* write the NB04 signatures */
    std::memcpy( start.sig, HLL_NB04, sizeof( start.sig ) );
    start.offset = st.sect_offsets[index_of( HllSect::directory )] - st.hll_base;
    PutInfo( st.hll_base, &start, sizeof( start ) );
    start.offset = st.hll_size;
    PutInfo( st.hll_base + st.hll_size - sizeof( start ), &start, sizeof( start ) );

    /* write the subsection directory header (also called directory info) */
    st.hll_dir_hdr = reinterpret_cast<hll_dirinfo *>( st.sect_offsets[index_of( HllSect::directory )] );
    dirhdr.cbDirHeader = sizeof( dirhdr );
    dirhdr.cbDirEntry = sizeof( hll_dir_entry );
    dirhdr.cDir = ( st.sect_sizes[index_of( HllSect::directory )] - sizeof( dirhdr ) ) / sizeof( hll_dir_entry );
    hllDumpInfo( HllSect::directory, &dirhdr, sizeof( dirhdr ) );

    /* Walk the debug info groups and dump the data in those segments. */
    for( group = wlink().DBIGroups; group != nullptr; group = group->next ) {
        RingLookup( group->g.class_->segs, hllDefLeader, group );
        group->g.class_ = nullptr;
    }
}

void HllFini( section *sect )
/***************************/
// called after pass 2 is finished, but before load file generation
{
    DEBUG(( DBG_HLL, "HllFini: sect=%p", sect ));
    (void)sect; // komh: make compiler happy
}

void HllWrite( void )
/*******************/
// called during load file generation.  It is assumed that the loadfile is
// positioned to the right spot.
{
    HllState        &st = state();
    unsigned        i, j;
    hll_dir_entry   *dir_entries;

    DEBUG(( DBG_HLL, "HllWrite: HllBase=%h HllSize=%h", st.hll_base, st.hll_size ));

    /* before writing it sort the directory by module. */
    dir_entries = reinterpret_cast<hll_dir_entry *>( st.hll_dir_hdr + 1 );
    for( i = 0; i < st.hll_dir_hdr->cDir; i++ ) {
        for( j = i + 1; j < st.hll_dir_hdr->cDir; j++ ) {
            if( dir_entries[j].iMod < dir_entries[i].iMod
              || ( dir_entries[j].iMod == dir_entries[i].iMod
              && dir_entries[j].subsection < dir_entries[i].subsection ) ) {
                hll_dir_entry tmp = dir_entries[j];
                dir_entries[j] = dir_entries[i];
                dir_entries[i] = tmp;
            }
        }
    }

    /* write the debug section */
    WriteInfoLoad( st.hll_base, st.hll_size );
}
