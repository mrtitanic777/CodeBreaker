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
* Description:  Produce Watcom style debugging information in load file.
*
****************************************************************************/


#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string_view>
#include "walloca.h"
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/debuginfo/dbginfo.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/objects/objomf.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/debuginfo/dbgwat.h"
#include "wdbginfo.h"
#include "felang.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


// these two are seen through objstruc.h (ODBIMODINFO), so the struct stays at global scope

struct demanddata {
    unsigned_32         offset;
    unsigned_16         num;
    unsigned_16         size;
};

struct odbimodinfo {
    unsigned_32         linelinksize;
    demanddata          types;
    demanddata          locals;
    demanddata          lines;
    unsigned_32         modnum;
    unsigned            dbisourceoffset;
};

namespace {

constexpr unsigned      DEMAND_INFO_SPLIT = _16K;

constexpr unsigned_16   NON_SECT_INFO = 0x8000;

struct snamelist {                      // source name list
    snamelist           *next;
    size_t              len;            // length of the name
    char                name[1];        // stored WITH a nullchar
};

// what dbginfo.cpp keeps for one run of the linker
struct DbgInfoState {
    unsigned_32                         dbiSize = 0;
    std::array<uint_8, sizeof( master_dbg_header )> master{};    // rest depend on .obj files.
    snamelist                           *dbiSourceLang = nullptr;   // list of source languages
#ifdef DEVBUILD
    struct {
        offset   sizeadded;
        offset   sizegenned;
    } traceInfo{};
#endif
};

DbgInfoState &dbgInfoState()
{
    return cb_wlink::RunState::part<DbgInfoState>();
}

inline debug_info *dbgInfo( section *sect )
{
    return static_cast<debug_info *>( sect->dbg_info );
}

constexpr std::size_t MASTER_OBJ_MAJOR = offsetof( master_dbg_header, obj_major_ver );
constexpr std::size_t MASTER_OBJ_MINOR = offsetof( master_dbg_header, obj_minor_ver );

snamelist *LangAlloc( std::string_view name )
/*******************************************/
{
    snamelist *node;

    node = static_cast<snamelist *>( _PermAlloc( sizeof( snamelist ) + name.size() ) );
    node->len = name.size();
    std::memcpy( node->name, name.data(), name.size() );
    node->name[name.size()] = '\0';
    return( node );
}

}

void ODBIInit( section *sect )
/****************************/
{
    DbgInfoState    &st = dbgInfoState();

    st.dbiSize = sizeof( master_dbg_header );
    MPUT_LE_16( st.master.data() + offsetof( master_dbg_header, signature ), WAT_DBG_SIGNATURE );
    st.master[offsetof( master_dbg_header, exe_major_ver )] = EXE_MAJOR_VERSION;
    st.master[offsetof( master_dbg_header, exe_minor_ver )] = EXE_MINOR_VERSION;
    st.master[offsetof( master_dbg_header, obj_major_ver )] = 0;
    st.master[offsetof( master_dbg_header, obj_minor_ver )] = 0;
    st.dbiSourceLang = LangAlloc( std::string_view( FE_LANG_C, sizeof( FE_LANG_C ) - 1 ) );
    st.dbiSourceLang->next = nullptr;
    sect->dbg_info = _PermAlloc( sizeof( debug_info ) );
    std::memset( sect->dbg_info, 0, sizeof( debug_info ) );  //assumes NULL == 0
#ifdef DEVBUILD
    st.traceInfo = {};
#endif
}

void ODBIInitModule(Link& link,  mod_entry *mod )
/***********************************/
{
    if( link.CurrSect->dbg_info == nullptr )
        return;
    mod->u3.o = static_cast<ODBIMODINFO *>( _PermAlloc( sizeof( odbimodinfo ) ) );
    std::memset( mod->u3.o, 0, sizeof( odbimodinfo ) );
}

static void DumpInfo( debug_info *dinfo, const void *data, size_t len )
/*********************************************************************/
{
    PutInfo( dinfo->dump_addr, data, len );
    dinfo->dump_addr += len;
}

static bool FindMatch( size_t len, const char *buff, unsigned *off )
/******************************************************************/
// returns false if not found
{
    snamelist   *node;

    *off = 0;
    for( node = dbgInfoState().dbiSourceLang; node != nullptr; node = node->next ) {
        if( node->len == len ) {
            if( strnicmp( buff, node->name, len ) == 0 ) {
                return( true );
            }
        }
        *off += node->len + 1;     // +1 for NULLCHAR
    }
    return( false );
}

void ODBIP1Source(Link& link,  byte major, byte minor, const char *name, size_t len )
/***********************************************************************/
{
    DbgInfoState    &st = dbgInfoState();
    snamelist       *node;

    if( st.master[MASTER_OBJ_MAJOR] == 0 )
        st.master[MASTER_OBJ_MAJOR] = major;
    if( major != st.master[MASTER_OBJ_MAJOR] ) {
        LnkMsg( WRN+LOC+MSG_CANT_USE_LOCALS, nullptr );
        link.CurrMod->flags_dbi &= ~( DBI_TYPE | DBI_LOCAL );
    }
    if( st.master[MASTER_OBJ_MINOR] < minor ) {
        st.master[MASTER_OBJ_MINOR] = minor;
    }
    if( !FindMatch( len, name, &link.CurrMod->u3.o->dbisourceoffset ) ) {
        node = LangAlloc( std::string_view( name, len ) );
        node->next = st.dbiSourceLang->next;
        st.dbiSourceLang->next = node;        // keep "C" the first entry.
    }
}

static void DoAddLocal( dbi_section *dbi, offset length )
/*******************************************************/
{
    if( ( dbi->size == 0 )
      || ( dbi->size + length > DEMAND_INFO_SPLIT ) ) {
        dbi->curr.u.vm_offs += sizeof( unsigned_32 );
        dbi->size = 0;
    }
    dbi->size += length;
#ifdef DEVBUILD
    dbgInfoState().traceInfo.sizeadded += length;
#endif
}

void ODBIAddLocal(Link& link,  seg_leader *seg, offset length )
/*************************************************/
{
    debug_info          *dinfo;

    dinfo = dbgInfo( link.CurrSect );
    if( dinfo == nullptr )
        return;
    if( seg->dbgtype == MS_TYPE ) {
        DEBUG(( DBG_DBGINFO, "adding type info %h", length ));
        DoAddLocal( &dinfo->typelinks, length );
    } else if( seg->dbgtype == MS_LOCAL ) {
        DEBUG(( DBG_DBGINFO, "adding local info %h", length ));
        DoAddLocal( &dinfo->locallinks, length );
    }
}

void ODBIP1ModuleScanned(Link& link)
/******************************/
{
    debug_info          *dinfo;

    dinfo = dbgInfo( link.CurrSect );
    if( dinfo == nullptr )
        return;
    dinfo->typelinks.size = 0;
    dinfo->locallinks.size = 0;
}

static void DoGenLocal( dbi_section *dsect, dbi_section *dlink, demanddata *dmod, offset length )
/***********************************************************************************************/
{
    unsigned_32 spot;

    if( ( dmod->size == 0 )
      || ( dmod->size + length > DEMAND_INFO_SPLIT ) ) {
        spot = dsect->start + ( dsect->curr.u.vm_offs - dsect->init.u.vm_offs );
        PutInfo( dlink->curr.u.vm_ptr, &spot, sizeof( unsigned_32 ) );
        dmod->num++;
        if( dmod->size == 0 ) {
            dmod->offset = dlink->start + ( dlink->curr.u.vm_ptr - dlink->init.u.vm_ptr );
        } else {
            dmod->size = 0;
        }
        dlink->curr.u.vm_ptr += sizeof( unsigned_32 );
    }
    dsect->curr.u.vm_ptr += length;
    dmod->size += length;
#ifdef DEVBUILD
    dbgInfoState().traceInfo.sizegenned += length;
#endif
}

void ODBIGenLocal(Link& link,  segdata *sdata )
/*********************************/
{
    debug_info          *dinfo;
    odbimodinfo         *minfo;

    dinfo = dbgInfo( link.CurrSect );
    if( dinfo == nullptr )
        return;
    if( sdata->isdead )
        return;
    minfo = link.CurrMod->u3.o;
    if( sdata->u.leader->dbgtype == MS_TYPE ) {
        DEBUG(( DBG_DBGINFO, "genning type info %h", sdata->length ));
        DoGenLocal( &dinfo->type, &dinfo->typelinks, &minfo->types, sdata->length );
    } else if( sdata->u.leader->dbgtype == MS_LOCAL ) {
        DEBUG(( DBG_DBGINFO, "genning local info %h", sdata->length ));
        DoGenLocal( &dinfo->local, &dinfo->locallinks, &minfo->locals, sdata->length );
    }
}

static void ODBIAddLines( lineinfo *info )
/****************************************/
{
    Link& link = wlink();
    unsigned            lineqty;
    unsigned_32         linesize;
    debug_info          *dinfo;

    dinfo = dbgInfo( link.CurrSect );
    lineqty = DBICalcLineQty( info );
    linesize = lineqty * sizeof( ln_off_386 ) + sizeof( lineseg );
    dinfo->line.curr.u.vm_offs += linesize;
    DoAddLocal( &dinfo->linelinks, linesize );
}

void ODBIP1ModuleFinished( mod_entry *obj )
/*****************************************/
{
    Link& link = wlink();
    debug_info          *dinfo;

    dinfo = dbgInfo( link.CurrSect );
    if( ( dinfo == nullptr )
      || (obj->flags_dbi & DBI_ALL) == 0 )
        return;
    if( MOD_NOT_DEBUGGABLE( obj ) )
        return;
    link.CurrMod = obj;
    if( link.CurrMod->flags_dbi & DBI_LINE ) {
        DBILineWalk( reinterpret_cast<lineinfo *>( obj->lines ), ODBIAddLines );
    }
    Ring2Walk( obj->publist, DBIModGlobal );
    dinfo->mod.curr.u.vm_offs += std::string_view( obj->name.u.ptr ).size() + sizeof( mod_dbg_info );
    dinfo->linelinks.size = 0;
}

void ODBIDefClass(Link& link,  class_entry *class_, unsigned_32 size )
/*******************************************************/
{
    debug_info *dinfo;

    if( class_->flags & CLASS_DWARF )
        return;
    dinfo = dbgInfo( link.CurrSect );
    if( dinfo == nullptr )
        return;
    if( class_->flags & CLASS_MS_TYPE ) {
        dinfo->type.curr.u.vm_offs += size;
        dinfo->TypeClass = class_;
    } else if( class_->flags & CLASS_MS_LOCAL ) {
        dinfo->local.curr.u.vm_offs += size;
        dinfo->LocalClass = class_;
    }
}

static int ODBISymIsForGlobalDebugging( symbol *sym, mod_entry *currMod )
/***********************************************************************/
{
    return( (currMod->flags_dbi & DBI_ONLY_EXPORTS) == 0 && ( (currMod->flags_dbi & DBI_STATICS) || (sym->info & SYM_STATIC) == 0 ) );
}

void ODBIAddGlobal(Link& link,  symbol *sym )
/*******************************/
{
    debug_info      *dinfo;
    size_t          len;

    dinfo = dbgInfo( link.CurrSect );
    if( dinfo == nullptr )
        return;
    if( ODBISymIsForGlobalDebugging( sym, link.CurrMod ) ) {
        len = std::strlen( sym->name.u.ptr );
        if( len > 255 ) {
            LnkMsg( WRN+MSG_SYMBOL_NAME_TOO_LONG, "s", sym->name.u.ptr );
            len = 255;
        }
        dinfo->global.curr.u.vm_offs += sizeof( v3_gbl_info ) + len;
    }
}

static bool AllocASeg( void *leader, void *group )
/************************************************/
{
    static_cast<seg_leader *>( leader )->group = static_cast<group_entry *>( group );
    return( false );
}

static void AllocDBIClasses(Link& link,  class_entry *class_ )
/***********************************************/
/* Allocate all classes in the list */
{
    group_entry *group;

    for( ; class_ != nullptr; class_ = class_->next ) {
        if( class_->flags & CLASS_DEBUG_INFO ) {
            group = AllocGroup(link,  AutoGrpName, &link.DBIGroups );
            group->addr.seg = 0;
            RingLookup( class_->segs, AllocASeg, group );
        }
    }
}

void ODBIAddrSectStart(Link& link,  section *sect )
/*************************************/
{
    debug_info      *dptr;

    dptr = dbgInfo( sect );
    if( dptr == nullptr )
        return;
    if( ( dptr->local.curr.u.vm_offs > 0 )
      || ( dptr->type.curr.u.vm_offs > 0 )
      || ( dptr->line.curr.u.vm_offs > 0 ) ) {
        dptr->linelinks.curr.u.vm_offs += sizeof( unsigned_32 );
    }
    dptr->locallinks.start = sizeof( section_dbg_header );
    dptr->locallinks.size = dptr->locallinks.curr.u.vm_offs;
    dptr->locallinks.curr.u.vm_ptr = DBIAlloc( dptr->locallinks.curr.u.vm_offs );
    dptr->locallinks.init.u.vm_ptr = dptr->locallinks.curr.u.vm_ptr;

    dptr->typelinks.start = dptr->locallinks.start + dptr->locallinks.size;
    dptr->typelinks.size = dptr->typelinks.curr.u.vm_offs;
    dptr->typelinks.curr.u.vm_ptr = DBIAlloc( dptr->typelinks.curr.u.vm_offs );
    dptr->typelinks.init.u.vm_ptr = dptr->typelinks.curr.u.vm_ptr;

    dptr->linelinks.start = dptr->typelinks.start + dptr->typelinks.size;
    dptr->linelinks.size = dptr->linelinks.curr.u.vm_offs;
    dptr->linelinks.curr.u.vm_ptr = DBIAlloc( dptr->linelinks.curr.u.vm_offs );
    dptr->linelinks.init.u.vm_ptr = dptr->linelinks.curr.u.vm_ptr;

    dptr->local.start = dptr->linelinks.start + dptr->linelinks.size;
    dptr->local.size = dptr->local.curr.u.vm_offs;
    dptr->local.init.u.vm_offs = dptr->local.curr.u.vm_offs;

    dptr->type.start = dptr->local.start + dptr->local.size;
    dptr->type.size = dptr->type.curr.u.vm_offs;
    dptr->type.init.u.vm_offs = dptr->type.curr.u.vm_offs;

    dptr->line.start = dptr->type.start + dptr->type.size;
    dptr->line.size = dptr->line.curr.u.vm_offs;
    dptr->line.curr.u.vm_ptr = DBIAlloc( dptr->line.curr.u.vm_offs );
    dptr->line.init.u.vm_ptr = dptr->line.curr.u.vm_ptr;

    dptr->mod.start = dptr->line.start + dptr->line.size;
    dptr->mod.size = dptr->mod.curr.u.vm_offs;
    dptr->mod.curr.u.vm_ptr = DBIAlloc( dptr->mod.curr.u.vm_offs );
    dptr->mod.init.u.vm_ptr = dptr->mod.curr.u.vm_ptr;

    dptr->global.start = dptr->mod.start + dptr->mod.size;
    dptr->addr.start = dptr->global.start + dptr->global.curr.u.vm_offs;
    dptr->global.size = 0;
    dptr->global.curr.u.vm_ptr = DBIAlloc( dptr->global.curr.u.vm_offs );
    dptr->global.init.u.vm_ptr = dptr->global.curr.u.vm_ptr;

    dptr->addr.curr.u.vm_ptr = DBIAlloc( dptr->addr.size );
    dptr->addr.init.u.vm_ptr = dptr->addr.curr.u.vm_ptr;

    dptr->dump_addr = dptr->global.curr.u.vm_ptr;
    dptr->modnum = -1;

    AllocDBIClasses(link,  sect->classes );
}

// a name as the debugger reads it: its length, then its characters
static void DoName( const char *cname, char *intelname, size_t len )
/******************************************************************/
{
    intelname[0] = static_cast<char>( len );
    std::memcpy( intelname + 1, cname, len );
}

void ODBIGenGlobal(Link& link,  symbol *sym, section *sect )
/**********************************************/
{
    size_t      len;
    v3_gbl_info *data;
    debug_info  *dptr;

    dptr = dbgInfo( sect );
    if( dptr == nullptr )
        return;
    if( ODBISymIsForGlobalDebugging( sym, link.CurrMod ) ) {
        len = std::strlen( sym->name.u.ptr );
        if( len > 255 ) {
            len = 255;
        }
        alignas( v3_gbl_info ) std::array<unsigned char, sizeof( v3_gbl_info ) + 255> storage;
        data = reinterpret_cast<v3_gbl_info *>( storage.data() );
        _HostU32toTarg( sym->addr.off, data->addr.offset );
        _HostU16toTarg( sym->addr.seg, data->addr.segment );
        _HostU16toTarg( dptr->modnum, data->mod );
        data->kind = 0;
        if( sym->info & SYM_STATIC ) {
            data->kind |= GBL_KIND_STATIC;
        }
        if( sym->p.seg != nullptr ) {
            if( sym->p.seg->u.leader->info & SEGINF_CODE ) {
                data->kind |= GBL_KIND_CODE;
            } else {
                data->kind |= GBL_KIND_DATA;
            }
        }
        DoName( sym->name.u.ptr, data->name, len );
        len += sizeof( v3_gbl_info );
        DumpInfo( dptr, data, len );
        dptr->global.size += len;
    }
}

void ODBIAddModule( mod_entry *obj, section *sect )
/*************************************************/
{
    debug_info          *dptr;

    dptr = dbgInfo( sect );
    if( ( dptr == nullptr )
      || (obj->flags_dbi & DBI_ALL) == 0 )
        return;
    dptr->modnum++;
    obj->u3.o->modnum = dptr->modnum;
}

static void ODBIGenAddrInit( segdata *sdata, void *_dinfo )
/*********************************************************/
{
    Link& link = wlink();
    seg_dbg_info    seghdr;
    seg_leader      *seg;
    debug_info      *dptr = static_cast<debug_info *>( _dinfo );

    seg = sdata->u.leader;
    if( seg->group == nullptr ) {
        seghdr.base.offset = seg->seg_addr.off;
        seghdr.base.segment = seg->seg_addr.seg;
    } else {
        seghdr.base.offset = seg->group->addr.off + SEG_GROUP_DELTA( seg );
        seghdr.base.segment = seg->group->addr.seg;
    }
    seghdr.count = seg->num;
    if( link.CurrSect == link.NonSect )
        seghdr.count |= NON_SECT_INFO;
    DumpInfo( dptr, &seghdr, sizeof( seg_dbg_info ) - sizeof( addr_dbg_info ) );
    dptr->addr.curr.u.vm_ptr += sizeof( seg_dbg_info ) - sizeof( addr_dbg_info );
}

static void ODBIGenAddrAdd( segdata *sdata, offset delta, offset size, void *_dinfo, bool isnewmod )
/**************************************************************************************************/
{
    addr_dbg_info   addr;
    debug_info      *dptr = static_cast<debug_info *>( _dinfo );

    /* unused parameters */ (void)delta;

    if( isnewmod ) {
        addr.size = size;
        addr.mod = sdata->o.mod->u3.o->modnum;
        DumpInfo( dptr, &addr, sizeof( addr_dbg_info ) );
        sdata->addrinfo = dptr->addr.curr.u.vm_ptr - dptr->addr.init.u.vm_ptr;
        dptr->addr.curr.u.vm_ptr += sizeof( addr_dbg_info );
    } else {
        sdata->addrinfo = dptr->addr.curr.u.vm_ptr - dptr->addr.init.u.vm_ptr;
    }
}

static void ODBIGenAddrInfo( seg_leader *seg )
/********************************************/
{
    Link& link = wlink();
    debug_info  *dptr;

    if( link.CurrSect == link.NonSect ) {
        dptr = dbgInfo( link.Root );
    } else {
        dptr = dbgInfo( link.CurrSect );
    }
    if( ( dptr == nullptr )
      || ( seg->num == 0 ) )
        return;
    DBIAddrInfoScan(link,  seg, ODBIGenAddrInit, ODBIGenAddrAdd, dptr );
}

static void WriteBogusAddrInfo( debug_info *dptr )
/************************************************/
{
    seg_dbg_info    header;

    dptr->addr.size = sizeof( seg_dbg_info );
    dptr->addr.curr.u.vm_ptr = DBIAlloc( dptr->addr.size );
    dptr->addr.init.u.vm_ptr = dptr->addr.curr.u.vm_ptr;
    dptr->dump_addr = dptr->addr.curr.u.vm_ptr;
    header.base.offset = 0;
    header.base.segment = 0;
    header.count = 1;
    header.addr[0].size = 0;
    header.addr[0].mod = 0;
    DumpInfo( dptr, &header, sizeof( seg_dbg_info ) );
    dptr->addr.curr.u.vm_ptr += sizeof( seg_dbg_info );
}

void ODBIP2Start(Link& link,  section *sect )
/*******************************/
/* initialize pointers for pass 2 processing */

{
    debug_info          *dptr;

    if( sect == link.NonSect ) {
        dptr = dbgInfo( link.Root );
    } else {
        dptr = dbgInfo( sect );
    }
    if( dptr != nullptr ) {
        // if section has no info then write bogus address info
        if( dptr->addr.curr.u.vm_ptr == 0 ) {
            WriteBogusAddrInfo( dptr );
        } else {
            dptr->dump_addr = dptr->addr.curr.u.vm_ptr;
            SectWalkClass(link,  sect, ODBIGenAddrInfo );
        }
        dptr->dump_addr = dptr->line.curr.u.vm_ptr;
        dptr->modnum = 0;
    }
}

static int CmpLn386( const void *a, const void *b )
/*************************************************/
{
    return( static_cast<int>( static_cast<const ln_off_386 _WCUNALIGNED *>( a )->off - static_cast<const ln_off_386 _WCUNALIGNED *>( b )->off ) );
}

static int CmpLn286( const void *a, const void *b )
/*************************************************/
{
    return( static_cast<int>( static_cast<const ln_off_286 *>( a )->off - static_cast<const ln_off_286 *>( b )->off ) );
}

static bool CheckFirst( void *_seg, void *_firstseg )
/***************************************************/
{
    segdata *seg = static_cast<segdata *>( _seg );
    segdata **firstseg = static_cast<segdata **>( _firstseg );

    if( ( seg->a.delta < (*firstseg)->a.delta )
            && ( seg->addrinfo == (*firstseg)->addrinfo ) ) {
        *firstseg = seg;
    }
    return( false );
}

void ODBIGenLines( lineinfo *info )
/*********************************/
{
    Link& link = wlink();
    size_t              linelen;
    ln_off_pair _WCUNALIGNED *pair;
    ln_off_386          tmp_ln;
    unsigned_32         temp;
    size_t              lineqty;
    debug_info          *dinfo;
    lineseg             lseg;
    segdata             *firstseg;
    unsigned_32         prevoff;
    offset              adjust;
    bool                needsort;
    size_t              size;
    size_t              item_size;
    segdata             *seg;
    int                 (*CmpLn)( const void *a, const void *b );

    seg = info->seg;

    dinfo = dbgInfo( link.CurrSect );
    if( ( dinfo == nullptr )
      || (link.CurrMod->flags_dbi & DBI_LINE) == 0 )
        return;
    lineqty = DBICalcLineQty( info );
    DoGenLocal( &dinfo->line, &dinfo->linelinks, &link.CurrMod->u3.o->lines, lineqty * sizeof( ln_off_386 ) + sizeof( lineseg ) );
    lseg.segment = seg->addrinfo;
    lseg.num = lineqty;
    DumpInfo( dinfo, &lseg, sizeof( lineseg ) );
/*
    fix the offset so that, together with modinfo.seg, it
    represents the offset of that line in the image
    also when we have multiple segdefs for the same segment, we collapse the
    addr infos, so we have to adjust the line # offset to account for this.
*/
    firstseg = static_cast<segdata *>( Ring2Step( link.CurrMod->segs, nullptr ) );
    Ring2Lookup( link.CurrMod->segs, CheckFirst, &firstseg );
    adjust = seg->a.delta - firstseg->a.delta;
    pair = reinterpret_cast<ln_off_pair *>( info->data );
    prevoff = 0;
    needsort = false;
    if( info->size & LINE_IS_32BIT ) {
        item_size = sizeof( ln_off_386 );
        CmpLn = CmpLn386;
    } else {
        item_size = sizeof( ln_off_286 );
        CmpLn = CmpLn286;
    }
    linelen = info->size & ~LINE_IS_32BIT;
    for( size = linelen; size > 0; size -= item_size ) {
        if( info->size & LINE_IS_32BIT ) {
            pair->_386.off += adjust;
            if( prevoff > pair->_386.off ) {
                needsort = true;
            }
            prevoff = pair->_386.off;
        } else {
            _TargU16toHost( pair->_286.off, temp );
            if( prevoff > temp ) {
                needsort = true;
            }
            prevoff = temp;
        }
        pair = reinterpret_cast<ln_off_pair *>( reinterpret_cast<char *>( pair ) + item_size );
    }
    if( needsort ) {
        qsort( info->data, lineqty, item_size, CmpLn );
    }
    if( info->size & LINE_IS_32BIT ) {
        DumpInfo( dinfo, info->data, linelen );
    } else {
        pair = reinterpret_cast<ln_off_pair *>( info->data );
        for( size = linelen; size > 0; size -= item_size ) {
            _TargU16toHost( pair->_286.off, temp );
            _HostU32toTarg( temp + adjust, tmp_ln.off );
            tmp_ln.linnum = pair->_286.linnum;
            // NYI: might have to do some buffering here
            DumpInfo( dinfo, &tmp_ln, sizeof( ln_off_386 ) );
            pair = reinterpret_cast<ln_off_pair *>( reinterpret_cast<char *>( pair ) + item_size );
        }
    }
}

static void ODBIAddAddrInit( segdata *sdata, void *cookie )
/*********************************************************/
{
    /* unused parameters */ (void)cookie;

    sdata->u.leader->num = 0;
}

static void ODBIAddAddrAdd( segdata *sdata, offset delta, offset size,
                            void *_dinfo, bool isnewmod )
/*******************************************************/
{
    /* unused parameters */ (void)delta; (void)size;

    if( !isnewmod )
        return;
    static_cast<debug_info *>( _dinfo )->addr.size += sizeof( addr_dbg_info );
    sdata->u.leader->num++;
}

void ODBIAddAddrInfo( seg_leader *seg )
/*************************************/
{
    Link& link = wlink();
    debug_info *dptr;

    if( link.CurrSect == link.NonSect ) {
        dptr = dbgInfo( link.Root );
    } else {
        dptr = dbgInfo( link.CurrSect );
    }
    if( dptr == nullptr )
        return;
    DBIAddrInfoScan(link,  seg, ODBIAddAddrInit, ODBIAddAddrAdd, dptr );
    if( seg->num > 0 ) {
        dptr->addr.size += sizeof( seg_dbg_info ) - sizeof( addr_dbg_info );
    }
}

void ODBIFini(Link& link,  section *sect )
/****************************/
// write out the final links in the link tables.
{
    debug_info          *dptr;
    unsigned_32         spot;

    if( sect == link.NonSect ) {
        dptr = dbgInfo( link.Root );
    } else {
        dptr = dbgInfo( sect );
    }
    if( dptr != nullptr ) {
        if( ( dptr->local.size > 0 )
          || ( dptr->type.size > 0 )
          || ( dptr->line.size > 0 ) ) {
            spot = dptr->line.start + ( dptr->line.curr.u.vm_ptr - dptr->line.init.u.vm_ptr );
            PutInfo( dptr->linelinks.curr.u.vm_ptr, &spot, sizeof( unsigned_32 ) );
        }
    }
}

void ODBIGenModule(Link& link)
/************************/
{
    odbimodinfo         *rec;
    mod_dbg_info        *info;
    size_t              len;
    debug_info          *dptr;

    dptr = dbgInfo( link.CurrSect );
    if( ( dptr == nullptr )
      || (link.CurrMod->flags_dbi & DBI_ALL) == 0 )
        return;
    rec = link.CurrMod->u3.o;
    len = std::strlen( link.CurrMod->name.u.ptr );
    if( len > 255 )
        len = 255;
    alignas( mod_dbg_info ) std::array<unsigned char, sizeof( mod_dbg_info ) + 255> storage;
    info = reinterpret_cast<mod_dbg_info *>( storage.data() );
    _HostU16toTarg( rec->types.num, info->di[DMND_TYPES].u.entries );
    _HostU32toTarg( rec->types.offset, info->di[DMND_TYPES].info_off );
    _HostU16toTarg( rec->locals.num, info->di[DMND_LOCALS].u.entries );
    _HostU32toTarg( rec->locals.offset, info->di[DMND_LOCALS].info_off );
    _HostU16toTarg( rec->lines.num, info->di[DMND_LINES].u.entries );
    _HostU32toTarg( rec->lines.offset, info->di[DMND_LINES].info_off );
    DoName( link.CurrMod->name.u.ptr, info->name, len );
    info->language = rec->dbisourceoffset;
    len += sizeof( mod_dbg_info );
    PutInfo( dptr->mod.curr.u.vm_ptr, reinterpret_cast<char *>( info ), len );
    dptr->mod.curr.u.vm_ptr += len;
    dptr->modnum++;
}

void ODBISectCleanup( section *sect )
/***********************************/
{
    /* unused parameters */ (void)sect;

    _PermFree( sect->dbg_info );
}

static void DBIWriteInfo(Link& link,  virt_mem stg, unsigned long len )
/*********************************************************/
{
    if( len > 0 ) {
        dbgInfoState().dbiSize += len;
        WriteInfoLoad( stg, len );
    }
}

static void DBIWriteLocal(Link& link,  const void *buff, size_t len )
/*******************************************************/
{
    if( len > 0 ) {
        dbgInfoState().dbiSize += len;
        WriteLoad(link,  buff, len );
    }
}

static unsigned_16 WriteSegValues(Link& link)
/***************************************/
// write out all possible group segment values
{
    std::array<unsigned_16, 2>  segarray;
    group_entry     *group;
    unsigned_16     *buffer;
    unsigned_16     buflen;

    if( link.FmtData.type & MK_FLAT_OFFS ) {
        segarray[0] = 1;
        DBIWriteLocal(link,  segarray.data(), sizeof( unsigned_16 ) );
        return( sizeof( unsigned_16 ) );
    } else if( link.FmtData.type & MK_ID_SPLIT ) {
        segarray[0] = CODE_SEGMENT;
        segarray[1] = DATA_SEGMENT;
        DBIWriteLocal(link,  segarray.data(), sizeof( unsigned_16 ) * 2 );
        return( sizeof( unsigned_16 ) * 2 );
    } else {
        buffer = reinterpret_cast<unsigned_16 *>( link.TokBuff );
        buflen = 0;
        for( group = link.Groups; group != nullptr; group = group->next ) {
            *buffer++ = group->addr.seg;
            buflen += sizeof( unsigned_16 );
        }
        DBIWriteLocal(link,  link.TokBuff, buflen );
        return( buflen );
    }
}

static void WriteDBISecs( section *sect )
/***************************************/
{
    Link& link = wlink();
    debug_info          *dptr;
    section_dbg_header  header;
    unsigned long       pos;

    dptr = dbgInfo( sect );
    if( dptr != nullptr ) {
        header.section_size = dptr->addr.start + dptr->addr.size;
        header.mod_offset = dptr->mod.start;
        header.gbl_offset = dptr->global.start;
        header.addr_offset = dptr->addr.start;
        header.section_id = sect->ovlref;
        DBIWriteLocal(link,  &header, sizeof( section_dbg_header ) );
        DBIWriteInfo(link,  dptr->locallinks.init.u.vm_ptr, dptr->locallinks.size );
        DBIWriteInfo(link,  dptr->typelinks.init.u.vm_ptr, dptr->typelinks.size );
        DBIWriteInfo(link,  dptr->linelinks.init.u.vm_ptr, dptr->linelinks.size );
        pos = PosLoad(link);
        if( dptr->LocalClass != nullptr ) {
            RingWalk( dptr->LocalClass->segs, WriteLeaderLoad );
        }
        if( dptr->TypeClass != nullptr ) {
            RingWalk( dptr->TypeClass->segs, WriteLeaderLoad );
        }
        dbgInfoState().dbiSize += PosLoad(link) - pos;
        DBIWriteInfo(link,  dptr->line.init.u.vm_ptr, dptr->line.size );
        DBIWriteInfo(link,  dptr->mod.init.u.vm_ptr, dptr->mod.size );
        DBIWriteInfo(link,  dptr->global.init.u.vm_ptr, dptr->global.size );
        DBIWriteInfo(link,  dptr->addr.init.u.vm_ptr, dptr->addr.size );
    }
}

void ODBIWrite(Link& link)
/********************/
/* copy debugging info from extra memory to loadfile */
{
    DbgInfoState &st = dbgInfoState();
    snamelist   *node;
    snamelist   *next;
    uint_16     langsize;

    link.CurrSect = link.Root;
    langsize = 0;
    for( node = st.dbiSourceLang; node != nullptr; node = next ) {
        next = node->next;
        langsize += node->len + 1;
        DBIWriteLocal(link,  node->name, node->len + 1 );  // +1 for nullchar
        _PermFree( node );
    }
    MPUT_LE_16( st.master.data() + offsetof( master_dbg_header, lang_size ), langsize );
    st.dbiSourceLang = nullptr;
    MPUT_LE_16( st.master.data() + offsetof( master_dbg_header, segment_size ), WriteSegValues(link) );
    WalkAllSects(link,  WriteDBISecs );
    MPUT_LE_32_UN( st.master.data() + offsetof( master_dbg_header, debug_size ), st.dbiSize );
    if( st.master[MASTER_OBJ_MAJOR] == 0 )
        st.master[MASTER_OBJ_MAJOR] = 1;
    WriteLoad(link,  st.master.data(), st.master.size() );
#ifdef DEVBUILD
    if( st.traceInfo.sizeadded != st.traceInfo.sizegenned ) {
        LnkMsg( WRN+MSG_INTERNAL, "s", "size mismatch in watcom dbi" );
    }
#endif
}
