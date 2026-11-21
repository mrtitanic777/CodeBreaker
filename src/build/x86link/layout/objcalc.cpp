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
* Description:  Address calculation for pass 2 of wlink.
*
****************************************************************************/


#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <array>
#include <cstddef>
#include <string_view>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/formats/os2.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/formats/qnx.h"
#include "build/x86link/formats/elf.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/map/map.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/formats/dos16m.h"
#include "build/x86link/symbols/objpass1.h"

#include "clibext.h"


namespace {

// The order of the classes under DOSSEG.
enum class ClassOrder : int {
    RealMode,
    BegCode,
    Code,
    OvlMgr,
    Other,
    BegDGroup,
    Data,
    InitDGroup,
    UninitDGroup,
    Bss,
    Stack,
    Count
};

constexpr std::size_t ORD_COUNT = static_cast<std::size_t>( ClassOrder::Count );

struct grpaddrinfo {
    offset      start_offs;
    offset      end_offs;
    group_entry *group;
    group_entry *last_group;       // used only for copy classes
    boolbit     first_time  : 1;
};

struct FloatPatch {
    const char      *name;
    fix_fpp_type    win_fpp;
    fix_fpp_type    fpp;
};

constexpr FloatPatch FloatPatches[] = {
    #define pick_fp(enum,name,alt_name,win,alt_win,others,alt_others) {name, static_cast<fix_fpp_type>(win), static_cast<fix_fpp_type>(others)},
    #include "fppatche.h"
    #undef pick_fp
    #define pick_fp(enum,name,alt_name,win,alt_win,others,alt_others) {alt_name, static_cast<fix_fpp_type>(alt_win), static_cast<fix_fpp_type>(alt_others)},
    #include "fppatche.h"
    #undef pick_fp
};

} // namespace

static bool CheckLxdataSeen( void *_seg, void *dummy )
/****************************************************/
{
    seg_leader  *seg = static_cast<seg_leader *>( _seg );

    /* unused parameters */ (void)dummy;

    if( seg->info & SEGINF_LXDATA_SEEN ) {
        seg->class_->flags |= CLASS_LXDATA_SEEN;
        return( true );
    }
    return( false );
}

static void CheckClassUninitialized( class_entry *class_ )
/*******************************************************/
{
    RingLookup( class_->segs, CheckLxdataSeen, nullptr );
}

static void InsertPrevRing( seg_leader **list, seg_leader *curr, seg_leader *prev )
/*********************************************************************************/
{
    if( prev == nullptr ) {
        RingPush( list, curr );
    } else {
        RingInsert( list, curr, prev );
    }
}

#define CODECL_SIZE ( sizeof( CodeClassName ) - 1 )

bool IsCodeClass( const char *name, size_t namelen )
/**************************************************/
{
    return( ( namelen >= CODECL_SIZE )
        && ( strnicmp( name + namelen - CODECL_SIZE, CodeClassName, CODECL_SIZE ) == 0 ) );
}

#define CONSTCL_SIZE ( sizeof( ConstClassName ) - 1 )

bool IsConstClass( const char *name, size_t namelen )
/***************************************************/
{
    return( ( namelen >= CONSTCL_SIZE )
        && ( strnicmp( name + namelen - CONSTCL_SIZE, ConstClassName, CONSTCL_SIZE ) == 0 ) );
}

#define STACKCL_SIZE ( sizeof( StackClassName ) - 1 )

bool IsStackClass( const char *name, size_t namelen )
/***************************************************/
{
    return( ( namelen >= STACKCL_SIZE )
        && ( stricmp( name, StackClassName ) == 0 ) );
}

/* -----------------------Allocating Segments-------------------------------- */

static void AddUpSegData( void *_sdata )
/**************************************/
{
    Link& link = wlink();
    segdata     *sdata = static_cast<segdata *>( _sdata );
    seg_leader  *leader;

    if( sdata->isdead )
        return;
    leader = sdata->u.leader;
    if( leader->info & SEGINF_ABSOLUTE ) {
        leader->vsize = sdata->a.delta = 0;
        if( leader->size < sdata->length ) {
            leader->vsize = leader->size = sdata->length;
        }
    } else {
        leader->vsize = sdata->a.delta = CAlign(link,  leader->vsize, sdata->align );
        if( sdata->length > 0 ) {
            leader->vsize = leader->size = sdata->a.delta + sdata->length;
        }
    }
    if( leader->align < sdata->align ) {
        leader->align = sdata->align;
    }
}

static void CalcSeg( seg_leader *seg )
/************************************/
{
    RingWalk( seg->pieces, AddUpSegData );
}

void CalcSegSizes(Link& link)
/******************************/
{
    WalkLeaders(link,  CalcSeg );
}

static bool SetGroupInitSize( void *_sdata, void *_delta )
/********************************************************/
{
    segdata     *sdata = static_cast<segdata *>( _sdata );
    offset      *delta = static_cast<offset *>( _delta );

    if( !sdata->isuninit
      && ( sdata->length > 0 )
      && !sdata->isdead ) {
        sdata->u.leader->group->size = *delta + sdata->a.delta + sdata->length;
    }
    return( false );
}

static void CalcInitSize( seg_leader *seg )
/*****************************************/
{
    Link& link = wlink();
    offset      delta;

    if( seg->group != nullptr ) {
        delta = SEG_GROUP_DELTA( seg );
        RingLookup( seg->pieces, SetGroupInitSize, &delta );
    }
}

static bool setGroupSeg(Link& link,  group_entry *group, unsigned seg_num )
/****************************************************************
 * return false if segment number is fixed
 * return true if segment number should be incremented
 */
{
#ifdef _DOS16M
    if( link.FmtData.type & MK_DOS16M ) {
        group->addr.seg = ToD16MSel(link,  seg_num );
        return( true );
    }
#endif
#ifdef _NOVELL
    if( link.FmtData.type & MK_ID_SPLIT ) {
        if( group->segflags & SEG_DATA ) {
            group->addr.seg = DATA_SEGMENT;
        } else {
            group->addr.seg = CODE_SEGMENT;
        }
        return( false );
    }
#endif
#ifdef _QNX
    if( link.FmtData.type & MK_QNX ) {
        group->addr.seg = ToQNXSel(link,  seg_num );
        return( true );
    }
#endif
#if defined( _PHARLAP ) || defined( _RAW )
    if( link.FmtData.type & MK_FLAT_OFFS ) {
        group->addr.seg = seg_num;    // only segment 1 in flat mem. model
        return( false );
    }
#endif
#ifdef _PHARLAP
    if( link.FmtData.type & MK_PHAR_MULTISEG ) {
        group->addr.seg = ( seg_num << 3 ) | 4;
        return( true );
    }
#endif
    group->addr.seg = seg_num;
    return( true );
}

static void AllocFileSegs(Link& link)
/*******************************/
{
    group_entry         *group;
    unsigned            seg_num;

    seg_num = 1;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        group->addr.off = 0;
        if( setGroupSeg(link,  group, seg_num ) ) {
            seg_num++;
        }
    }
}

static void SetLeaderSeg( void *_seg )
/*****************************************/
{
    seg_leader      *seg = static_cast<seg_leader *>( _seg );

    if( (seg->info & SEGINF_ABSOLUTE) == 0 ) {
        seg->seg_addr.seg = seg->group->addr.seg;
    }
}

static void ReallocFileSegs(Link& link)
/*********************************/
/* In many cases we can't have any size 0 physical segments, so after we have
 * calculated the size of everything we have to go back and reallocate the
 * segment numbers so there aren't any "gaps" where the 0 size physical
 * segments (groups) are. */
{
    group_entry         *group;
    class_entry         *class_;
    unsigned            seg_num;

    seg_num = 1;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        /*
         * segment number is also set for zero length group to be
         * segments in map file sorted properly even if they are not emited
         * into load file
         */
        if( setGroupSeg(link,  group, seg_num ) ) {
            // increment segment if possible for target and group is not zero length
            if( group->totalsize != 0 ) {
                seg_num++;
            }
        }
        if( group->totalsize == 0 ) {
            /* to make life easier in loadxxx remove zero length group from groups count */
            link.NumGroups--;
        }
    }
    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) {
            RingWalk( class_->segs, SetLeaderSeg );
        }
    }
}

static void FindUninitDataStart(Link& link)
/*************************************/
/* for some formats we have to split the uninitialized data off of the rest of
 * DGROUP. So - this finds the start of the uninitialized portion of DGROUP */
{
    class_entry         *class_;
    bool                setnext;

    setnext = true;
    link.FmtData.dgroupsplitseg = nullptr;
    link.FmtData.bsspad = 0;
    if( (link.LinkState & LS_DOSSEG_FLAG) == 0 )
        return;
    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) {
            if( class_->flags & CLASS_LXDATA_SEEN ) {
                setnext = true;
            } else if( setnext ) {
                link.FmtData.dgroupsplitseg = static_cast<seg_leader *>( RingFirst( class_->segs ) );
                setnext = false;
            }
        }
    }
    if( setnext ) {             //last one was had an LXDATA or no segs.
        link.FmtData.dgroupsplitseg = nullptr;
    }
}

static bool FindEndAddr( void *_seg, void *_info )
/**************************************************/
{
    Link& link = wlink();
    seg_leader  *seg  = static_cast<seg_leader *>( _seg );
    grpaddrinfo *info = static_cast<grpaddrinfo *>( _info );
    offset      seg_offs;

    if( link.FmtData.type & MK_REAL_MODE ) {
        seg_offs = MK_REAL_ADDR( seg->seg_addr.seg, seg->seg_addr.off );
    } else {
        seg_offs = seg->seg_addr.off;
    }
    if( info->first_time ) {
        info->group->addr = seg->seg_addr;
        info->start_offs = seg_offs;
        info->end_offs = seg_offs + seg->size;
        info->first_time = false;
    } else {
        if( info->start_offs > seg_offs ) {
            info->group->addr = seg->seg_addr;
            info->start_offs = seg_offs;
        }
        if( info->end_offs < seg_offs + seg->size ) {
            info->end_offs = seg_offs + seg->size;
        }
    }
    return( false );
}

static bool FindInitEndAddr( void *_seg, void *_info )
/******************************************************/
// Only use initialized data segments.  Copy doesn't need uninitialized segments
// This is really only advantageous if uninitialized segments are at the end
{
    Link& link = wlink();
    seg_leader  *seg  = static_cast<seg_leader *>( _seg );
    grpaddrinfo *info = static_cast<grpaddrinfo *>( _info );
    offset      seg_offs;

    if( link.FmtData.type & MK_REAL_MODE ) {
        seg_offs = MK_REAL_ADDR( seg->seg_addr.seg, seg->seg_addr.off );
    } else {
        seg_offs = seg->seg_addr.off;
    }
    if( seg->info & SEGINF_LXDATA_SEEN ) {
        if( info->first_time ) { // First time, use seg_offs values
            info->start_offs = seg_offs;
            info->end_offs = seg_offs + seg->size;
            info->first_time = false;
        } else {  // If more segs found, use lowest start address and highest end address;
            if( info->start_offs > seg_offs ) {
                info->start_offs = seg_offs;
            }
            if( info->end_offs < seg_offs + seg->size ) {
                info->end_offs = seg_offs + seg->size;
            }
        }
    }
    return( false );
}

static bool FindCopyGroups( void *_seg, void *_info )
/************************************************/
{
    // This is called by the outer level iteration looking for classes
    //  that have more than one group in them
    seg_leader  *seg = static_cast<seg_leader *>( _seg );
    grpaddrinfo *info = static_cast<grpaddrinfo *>( _info );

    if( info->last_group != seg->group ) {   // Only interate new groups
        info->last_group = seg->group;
        // Check each initialized segment in group
        Ring2Lookup( seg->group->leaders, FindInitEndAddr, info );
    }
    return( false );
}


static void AllocSeg( void *_seg )
/********************************/
/* Allocate a segment (process all segments in a given class) */
{
    Link& link = wlink();
    seg_leader  *seg = static_cast<seg_leader *>( _seg );

    if( (seg->info & SEGINF_ABSOLUTE) == 0 ) {
        if( IS_DBG_DWARF( seg ) ) {
            link.CurrLoc.off = 0;
        }
        link.CurrentSeg = seg;
        NewSegment(link,  seg );
        DEBUG(( DBG_OLD, "- segment %s allocated", seg->segname.u.ptr ));
    }
}


static void CalcGrpAddr(Link& link,  group_entry *group )
/********************************************
 * Find lowest segment within group (the group's address)
 * not useful for OS/2 16-bit mode.
 */
{
    grpaddrinfo     info;
    seg_leader      *seg;
    class_entry     *class_;
    offset          addr;
    addr_type       save_addr;

    for( ; group != nullptr; group = group->next ) {
        info.group = group;
        info.first_time = true;
        seg = group->leaders;
        class_ = seg->class_;
        if( class_->flags & CLASS_COPY ) {
            group->addr = seg->seg_addr; // Get address of real segment (there's only one)
            // For copy classes must check eash segment to see if it is in a new group
            // this could be the case with FAR_DATA class in large model
            info.last_group = nullptr; // so it will use the first group
            RingLookup( class_->DupClass->segs, FindCopyGroups, &info );
            group->size = info.end_offs - info.start_offs;
            group->totalsize = group->size;
            // for copy classes put it in class size, also, so map file can find it.
            seg->size = group->totalsize;
            // Now must recompute addresses for all segments in all classes beyond this
            addr = (group->addr.seg << link.FmtData.SegShift) +
                   group->addr.off + group->totalsize;
            link.CurrLoc.seg = addr >> link.FmtData.SegShift;
            link.CurrLoc.off = addr & link.FmtData.SegMask;
            while( (class_ = class_->next) != nullptr ) {
                if( class_->flags & CLASS_FIXED ) {
                    save_addr = class_->BaseAddr;     // If class is fixed, can stop
                    ChkLocated(link,  &save_addr, true );  //   after making sure address
                    break;                      //   isn't already past here
                }
                if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) { // skip Debug classes, they've already been done
                    RingWalk( class_->segs, AllocSeg );
                }
            }
        } else {
            Ring2Lookup( seg, FindEndAddr, &info );
            if( (link.FmtData.type & MK_REAL_MODE)
              && (seg->info & SEGINF_USE_32) == 0
              && (info.end_offs - info.start_offs > _64K) ) {
                LnkMsg( ERR+MSG_GROUP_TOO_BIG, "sl", group->sym->name,
                        info.end_offs - info.start_offs - _64K );
            }
            group->totalsize = info.end_offs - info.start_offs;
        }
    }
}

void AllocClasses(Link& link,  section *sect )
/********************************/
/* Allocate all classes in the list */
{
    addr_type       save_addr;
    unsigned_32     size;
    class_entry     *class_;

    for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
        DEBUG(( DBG_OLD, "Allocating class %s", class_->name.u.ptr ));
        if( class_->flags & CLASS_DEBUG_INFO ) {
            /* don't *really* allocate room for these guys */
            save_addr = link.CurrLoc;
            link.CurrLoc.off = 0;
            link.CurrLoc.seg = 0;
            size = link.CurrSect->size;
            RingWalk( class_->segs, AllocSeg );
            DBIDefClass(link,  class_, link.CurrSect->size - size );
            link.CurrSect->size = size;
            link.CurrLoc = save_addr;
        } else {
            if( link.FmtData.type & (MK_PE | MK_QNX_FLAT | MK_OS2_FLAT | MK_WIN_VXD | MK_ELF) ) {
                // flat addresses
                if( class_->flags & CLASS_FIXED ) {
                    class_->segs->group->addr.off = class_->BaseAddr.off;
                    // Group inherits fixed address from class (only useful if it is first thing in group)
                }
            } else {
                // segmented
                save_addr = class_->BaseAddr;
                ChkLocated(link,  &save_addr, ( (class_->flags & CLASS_FIXED) != 0 ) );    // Process fixed locations if any
            }
            RingWalk( class_->segs, AllocSeg );
        }
    }
}


void ConvertToFrame(Link& link,  addr_type *frame_addr, segment frame, bool check_16bit )
/***************************************************************************/
{
    unsigned long   off;

    if( link.FmtData.type & MK_REAL_MODE ) {
        off = MK_REAL_ADDR( static_cast<int>( frame_addr->seg - frame ), frame_addr->off );
        if( check_16bit
          && ( off >= 0x10000 )) {
            LnkMsg( ERR+LOC+MSG_FRAME_INVALID, "Ax", frame_addr, frame );
        }
        frame_addr->off = off;
    }
    frame_addr->seg = frame;
}

/* -------------------------Defining Publics--------------------------------- */


static void FindFloatSyms(Link& link)
/*******************************/
// this finds the floating point patch symbols and marks them.
{
    symbol      *sym;

    for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
        SET_SYM_2_FPP( sym, FPP_NONE );
    }
    for( const FloatPatch &patch : FloatPatches ) {
        if( patch.name != nullptr ) {
            sym = FindISymbol(link,  patch.name );
            if( sym != nullptr ) {
                if( link.FmtData.type & MK_WIN_NE ) {
                    SET_SYM_2_FPP( sym, patch.win_fpp );
                } else {
                    SET_SYM_2_FPP( sym, patch.fpp );
                }
            }
        }
    }
}


static void DefinePublics(Link& link)
/*******************************/
/* Define public symbols. */

{
    DEBUG(( DBG_OLD, "DefinePublics()" ));
    if( link.MapFlags & MAP_FLAG ) {
        LnkMsg( INF+MSG_CREATE_MAP, nullptr );
    }
    if( link.MapFile != nullptr ) {
        WriteMapHead(link,  link.Root );
        WriteMapPubStart();
    }
    ProcPubsSect(link,  link.Root->mods, link.Root );
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        OvlProcPubsSect(link,  link.Root );
    }
#endif
    ProcPubsSect(link,  link.LibModules, link.Root );
    if( link.MapFile != nullptr ) {
        WriteMapPubEnd(link);
    }
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        OvlProcPubs(link);
    }
#endif
    if( link.MapFile != nullptr ) {
        if( link.MapFlags & MAP_VERBOSE ) {
            WriteMapModulesSegments(link);
        }
    }
    FindFloatSyms(link);
}

void ProcPubsSect(Link& link,  mod_entry *head, section *sect )
/****************************************************/
{
    for( link.CurrMod = head; link.CurrMod != nullptr; link.CurrMod = link.CurrMod->u.next ) {
        DoPubsSect(link,  sect );
    }
}

static bool DefPubSym( void *_pub, void *_info )
/**********************************************/
{
    Link& link = wlink();
    symbol      *sym = static_cast<symbol *>( _pub );
    pubdefinfo  *info = static_cast<pubdefinfo *>( _info );
    segdata     *seg;
    seg_leader  *leader;
    offset      temp;

    if( sym->info & (SYM_DEAD | SYM_IS_ALTDEF) )
        return( false );
    if( IS_SYM_ALIAS( sym ) )
        return( false );
    if( IS_SYM_IMPORTED( sym ) )
        return( false );
    seg = sym->p.seg;
    if( seg != nullptr ) {
        leader = seg->u.leader;
        /* address in symbol table is actually signed_32 offset
           from segdata zero */
        if( seg->isabs || IS_DBG_INFO( leader ) ) {
            SET_SYM_ADDR( sym, sym->addr.off + seg->a.delta + leader->seg_addr.off, leader->seg_addr.seg );
        } else {
            temp = sym->addr.off;
            temp += seg->a.delta;
            temp += SEG_GROUP_DELTA( leader );
            SET_SYM_ADDR( sym, temp + leader->group->addr.off, leader->group->addr.seg );
            DBIGenGlobal(link,  sym, info->sect );
        }
    }
    if( link.MapFile != nullptr ) {
        WriteMapPubEntry(link,  info, sym );
    }
    return( false );
}

void DoPubsSect(Link& link,  section *sect )
/*********************************/
/* Process public definitions for an object file. */
{
    pubdefinfo  info;

    if( !MOD_NOT_DEBUGGABLE( link.CurrMod ) ) {
        DBIAddModule(link,  link.CurrMod, sect );
    }
    info.num = 0;
    info.first = true;
    info.sect = sect;
    if( link.MapFile != nullptr ) {
        WriteMapPubSortStart(link,  &info );
    }
    Ring2Lookup( link.CurrMod->publist, DefPubSym, &info );
    if( link.MapFile != nullptr ) {
        WriteMapPubSortEnd(link,  &info );
    }
}

static void SetReadOnly( void *_seg )
/***********************************/
{
    seg_leader      *seg = static_cast<seg_leader *>( _seg );

    if( seg->class_->flags & CLASS_READ_ONLY ) {
        seg->segflags |= SEG_READ_ONLY;
    }
}

static void setDefBase(Link& link)
/****************************/
{
#ifdef _OS2
    if( link.FmtData.type & MK_PE ) {
        link.FmtData.base = PE_DEFAULT_BASE;
        return;
    }
    if( link.FmtData.type & MK_WIN_VXD ) {
        // A VxD is a position-independent flat module: VMM loads it at a linear
        // address it chooses and applies the LE fixups, so its objects must be
        // based at 0 (retail VxD objects all have reloc base 0). Using
        // FLAT_GRANULARITY here based every object at 0x10000, which shifted every
        // relocated pointer -- including the DDB service-table handlers -- by 64K,
        // so a peer's first VxDCall into the driver jumped to garbage.
        link.FmtData.base = 0;
        return;
    }
    if( link.FmtData.type & MK_OS2_FLAT ) {
        link.FmtData.base = FLAT_GRANULARITY;
        return;
    }
#endif
#ifdef _QNX
    if( link.FmtData.type & MK_QNX_FLAT ) {
        link.FmtData.base = __ROUND_UP_SIZE_4K( link.StackSize + QNX_DEFAULT_BASE );
        return;
    }
#endif
#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        switch( link.LinkState & LS_HAVE_MACHTYPE_MASK ) {
        case LS_HAVE_ALPHA_CODE:
            break;
        case LS_HAVE_MIPS_CODE:
            link.FmtData.base = 0x00400000;
            break;
        case LS_HAVE_PPC_CODE:
            link.FmtData.base = 0x10000000;
            break;
        case LS_HAVE_X64_CODE:
            // TODO
            link.FmtData.base = 0x08048000;
            break;
        case LS_HAVE_X86_CODE:
            link.FmtData.base = 0x08048000;
            break;
        default:
            break;
        }
        return;
    }
#endif
    link.FmtData.base = 0;
}

static void setDefObjAlign(Link& link)
/********************************/
{
#ifdef _OS2
    if( link.FmtData.type & MK_PE ) {
        switch( link.LinkState & LS_HAVE_MACHTYPE_MASK ) {
        case LS_HAVE_ALPHA_CODE:
        case LS_HAVE_MIPS_CODE: // TODO
        case LS_HAVE_PPC_CODE:
            link.FmtData.objalign = _64K;
            break;
        case LS_HAVE_X86_CODE:
            link.FmtData.objalign = _4K;
            break;
        case LS_HAVE_X64_CODE:
            // TODO
            link.FmtData.objalign = _64K;
            break;
        default:
            link.FmtData.objalign = _64K;
            break;
        }
        return;
    } else if( link.FmtData.type & MK_WIN_VXD ) {
        link.FmtData.objalign = _4K;
        return;
    } else if( link.FmtData.type & (MK_NE | MK_OS2_FLAT) ) {
#if 0
        if( (link.LinkState & LS_HAVE_PPC_CODE) ) {
            // Development temporarly on hold:
            link.FmtData.objalign = _1K;
            return;
        }
#endif
        link.FmtData.objalign = FLAT_GRANULARITY;
        return;
    }
#endif
#ifdef _QNX
    if( link.FmtData.type & MK_QNX ) {
        link.FmtData.objalign = QNX_GROUP_ALIGN;
        return;
    }
#endif
#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        link.FmtData.objalign = _4K;
        return;
    }
#endif
    link.FmtData.objalign = _64K;
}

static offset getFlatOffset(Link& link)
/*********************************/
{
    /****************************************************
     * this must be always first to override target value
     */
#ifdef _RAW
    if( link.FmtData.output_raw || link.FmtData.output_hex ) {
        return( 0 );
    }
#endif
    /****************************************************/
#ifdef _OS2
    if( link.FmtData.type & MK_PE ) {
        return( GetPEHeaderSize(link) );
    }
#endif
#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        return( GetElfHeaderSize(link) );
    }
#endif
    return( link.FmtData.base );
}

void CalcAddresses(Link& link)
/*******************************/
/* Calculate the starting address in the file of each segment. */
{
    offset          size;
    group_entry     *group;
    offset          flat;

    DEBUG(( DBG_OLD, "CalcAddresses()" ));
    if( link.FmtData.base == NO_BASE_SPEC ) {
        setDefBase(link);
    }
    DBIPreAddrCalc(link);
    link.CurrSect = link.Root;
    if( link.FmtData.type & MK_PROT_MODE ) {
        AllocFileSegs(link);
        if( link.FmtData.objalign == NO_BASE_SPEC ) {
            setDefObjAlign(link);
        }
        if( link.FmtData.type & MK_SPLIT_DATA ) {
            FindUninitDataStart(link);
        }
    }
    StartMemMap(link);
    AllocClasses(link,  link.Root );
    if( link.FmtData.type & (MK_REAL_MODE | MK_FLAT_OFFS | MK_ID_SPLIT) ) {
#ifdef _EXE
        if( link.FmtData.type & MK_OVERLAYS ) {
            OvlCalc(link);
        }
#endif
        CalcGrpAddr(link,  link.Groups );
        CalcGrpAddr(link,  link.AbsGroups );
#ifdef _DOS16M
    } else if( link.FmtData.type & MK_DOS16M ) {
        CalcGrpSegs(link);
#endif
    } else if( link.FmtData.type & (MK_PE | MK_OS2_FLAT | MK_WIN_VXD | MK_QNX_FLAT | MK_ELF) ) {
        flat = getFlatOffset(link);
        for( group = link.Groups; group != nullptr; group = group->next ) {
            size = group->totalsize;
            if( group->addr.off > flat + link.FmtData.base) {
               // ORDER CLNAME name OFFSET option sets start_offs,
               //   retrieve this information here and wrap into linear address
               flat = group->addr.off - link.FmtData.base;
               group->addr.off = 0;
            }
            group->linear = flat;
            if(( group == link.DataGroup )
              && ( link.FmtData.dgroupsplitseg != nullptr )) {
                if( link.StackSegPtr != nullptr ) {
                    size -= link.StackSize;
                }
            }
            flat = __ROUND_UP_SIZE( flat + size, link.FmtData.objalign );
        }
        ReallocFileSegs(link);
    } else if( link.FmtData.type & (MK_QNX_16 | MK_NE) ) {
        ReallocFileSegs(link);
    }
    DBIAddrStart(link);
    WalkLeaders(link,  CalcInitSize );
    DefinePublics(link);
}

static bool SetClassFlag( void *seg, void *flags )
/************************************************/
{
    static_cast<seg_leader *>( seg )->segflags = static_cast<seg_flags_type>( *static_cast<unsigned_16 *>( flags ) );
    return( false );
}

static void FillClassFlags(Link& link,  char *name, unsigned_16 flags )
/*********************************************************/
{
    class_entry     *class_;

    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( stricmp( class_->name.u.ptr, name ) == 0 ) {
            RingLookup( class_->segs, SetClassFlag, &flags );
            return;
        }
    }
    // if it has made it our here, no class has been found.
    LnkMsg( WRN + MSG_CLASS_NAME_NOT_FOUND, "s", name );
}

static void FillTypeFlags(Link& link,  unsigned_16 flags, segflag_type type )
/***************************************************************/
{
    class_entry     *class_;
    class_status    class_flags;

    class_flags = ( type == SEGFLAG_CODE ) ? CLASS_NONE : CLASS_CODE;
    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( class_flags == (class_->flags & CLASS_CODE) ) {
            RingLookup( class_->segs, SetClassFlag, &flags );
        }
    }
}


void SetSegFlags(Link& link,  seg_flags_struct *start )
/*****************************************/
{
    seg_flags_struct    *next;
    seg_leader          *leader;
    seg_flags_struct    *seg_flags;
    class_entry         *class_;

    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
        RingWalk( class_->segs, SetReadOnly );
    }
    /*
     * process all class type def'ns first.
     */
    for( seg_flags = start; seg_flags != nullptr; seg_flags = seg_flags->next ) {
        if( ( seg_flags->type == SEGFLAG_CODE )
          || ( seg_flags->type == SEGFLAG_DATA ) ) {
            FillTypeFlags(link,  seg_flags->flags, seg_flags->type );
        }
    }
    /*
     * process all class def'ns second.
     */
    for( seg_flags = start; seg_flags != nullptr; seg_flags = seg_flags->next ) {
        if( seg_flags->type == SEGFLAG_CLASS ) {
            FillClassFlags(link,  seg_flags->name, seg_flags->flags );
        }
    }
    /*
     * now process individual segments
     */
    for( seg_flags = start; seg_flags != nullptr; seg_flags = next ) {
        next = seg_flags->next;
        if( seg_flags->type == SEGFLAG_SEGMENT ) {
            leader = FindSegment( link.Root, seg_flags->name );
            if( leader == nullptr ) {
                LnkMsg( WRN + MSG_SEG_NAME_NOT_FOUND, "s", seg_flags->name );
            } else {
                leader->segflags = seg_flags->flags;
            }
        }
        MemFree( seg_flags->name );
        MemFree( seg_flags );
    }
}

static void SplitDGroupClasses(Link& link,  section *sect )
/*********************************************/
// split classes in DGROUP
{
    class_entry         *class_;
    class_entry         *newclass;
    seg_leader          *curr;
    seg_leader          *newlist;
    seg_leader          *oldlist;

    for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
        newlist = nullptr;
        oldlist = nullptr;
        while( (curr = static_cast<seg_leader *>( RingPop( &class_->segs ) )) != nullptr ) {
            if( curr->group == link.DataGroup ) {
                RingAppend( &newlist, curr );
            } else {
                RingAppend( &oldlist, curr );
            }
        }
        if( oldlist != nullptr ) {
            class_->segs = oldlist;
            if( newlist != nullptr ) {
                newclass = DuplicateClass(link,  class_ );
                newclass->segs = newlist;
                class_->next = newclass;
                class_ = newclass;
                for( curr = nullptr; (curr = static_cast<seg_leader *>( RingStep( newlist, curr ) )) != nullptr; ) {
                    curr->class_ = newclass;
                }
            }
        } else if( newlist != nullptr ) {
            class_->segs = newlist;
        }
    }
}

static void ReOrderClasses( section *sect )
/******************************************
 * rebuild the class list using the Microsoft DOS segment ordering.
 * This builds various classes into separate rings, and then joins them
 * together.
 */
{
    Link& link = wlink();
    std::array<class_entry *, ORD_COUNT> rings{};   // all nullptr
    class_entry         *nextclass;
    class_entry         *class_;
    class_entry         **owner;
    seg_leader          *currseg;
    seg_leader          *prevseg;
    char                *name;
    ClassOrder          ord;

    SplitDGroupClasses(link,  sect );
    for( class_ = sect->classes; class_ != nullptr; class_ = nextclass ) {
        nextclass = class_->next;  // Take class out of original ring
        class_->next = nullptr;
        CheckClassUninitialized( class_ );
        if( (link.FmtData.type & (MK_NOVELL | MK_PHAR_LAP | MK_OS2_LX))
          && ( class_->bits == BITS_16 ) ) {
            ord = ClassOrder::RealMode;
        } else {
            name = class_->name.u.ptr;
            if( class_->flags & CLASS_CODE ) {
                if( stricmp( name, OvlMgrClass ) == 0 ) {
                    ord = ClassOrder::OvlMgr;
                } else if( stricmp( name, BegCodeClassName ) == 0 ) {
                    ord = ClassOrder::BegCode;
                } else {
                    ord = ClassOrder::Code;
                }
            } else if( ( class_->segs == nullptr )
                || ( class_->segs->group == nullptr )
                || ( class_->segs->group != link.DataGroup ) ) {
                ord = ClassOrder::Other;
            } else {     // it's the DGROUP segments.
                if( stricmp( name, BSSClassName ) == 0 ) {
                    ord = ClassOrder::Bss;
                } else if( stricmp( name, StackClassName ) == 0 ) {
                    ord = ClassOrder::Stack;
                } else if( stricmp( name, BegDataClassName ) == 0 ) {
                    ord = ClassOrder::BegDGroup;
                } else if( stricmp( name, DataClassName ) == 0 ) {
                    ord = ClassOrder::Data;
                } else if( class_->flags & CLASS_LXDATA_SEEN ) {
                    ord = ClassOrder::InitDGroup;
                } else {
                    ord = ClassOrder::UninitDGroup;
                }
            }
        }
        /* add the class to the appropriate ring */
        /* 'ring' points to the _last_ element of the ring */
        class_entry *&ring = rings[static_cast<std::size_t>( ord )];
        if( ring == nullptr ) {
            class_->next = class_;
        } else {
            class_->next = ring->next;
            ring->next = class_;
        }
        ring = class_;
    }

    /* move the BEGTEXT segment in CODE class to front of segment list */
    const class_entry *const code_ring = rings[static_cast<std::size_t>( ClassOrder::Code )];
    class_ = rings[static_cast<std::size_t>( ClassOrder::Code )];
    if( class_ != nullptr ) {
        do {
            prevseg = class_->segs;
            if( prevseg == nullptr )
                break;
            currseg = prevseg->next;
            for( ;; ) {
                if( stricmp( currseg->segname.u.ptr, BegTextSegName ) == 0 ) {
                    RingPromote( &class_->segs, currseg, prevseg );
                    break;
                }
                if( currseg == class_->segs )
                    break;
                prevseg = currseg;
                currseg = currseg->next;
            }
            class_ = class_->next;
        } while( class_ != code_ring );
    }

    /* now construct list out of the collected parts. */
    owner = &sect->classes;
    for( class_entry *ring : rings ) {
        if( ring != nullptr ) {
            *owner = ring->next;
            owner = &ring->next;
        }
    }
    *owner = nullptr;
}

static void SortClasses( section *sect )
/***************************************
 * rebuild the class list in the order specified by the ORDER directive
 * This builds various classes into separate rings, and then joins them
 * together.  Pass class and segment address and output information
 * to the class and segment structures.  Sort segments in a class if
 * information is provided.
 */
{
    Link& link = wlink();
    class_entry         *DefaultRing;   // Where to put classes that don't match anything
    class_entry         *nextclass;
    class_entry         *class_;
    class_entry         **NewRing;
    class_entry         **owner;
    seg_leader          *currseg;
    seg_leader          *prevseg;
    ORDER_CLASS         *MatchClass;
    ORDER_SEGMENT       *MatchSeg;

    DefaultRing = nullptr;

    for( class_ = sect->classes; class_ != nullptr; class_ = nextclass ) {
        nextclass = class_->next;  // Take class out of original ring
        class_->next = nullptr;
        CheckClassUninitialized( class_ );
        NewRing = &DefaultRing;  // In case no special class is found
        for( MatchClass = sect->orderlist; MatchClass != nullptr; MatchClass = MatchClass->NextClass ) {
            if( stricmp( class_->name.u.ptr, MatchClass->Name ) == 0 ) { // search order list for name match
                NewRing = &(MatchClass->Ring);   // if found save ptr to instance
                if( MatchClass->FixedAddr ) {    // and copy any flags or address from it
                    class_->flags |= CLASS_FIXED;
                    class_->BaseAddr = MatchClass->Base;
                    link.FmtData.base = 0;  // Otherwise PE will use default and blow up
                }
                if( MatchClass->NoEmit ) {
                    class_->flags |= CLASS_NOEMIT;
                }
                break;
            }
        }
        // add the class to front of ring attached to order class, or to default ring
        if( *NewRing == nullptr ) {
            class_->next = class_;
        } else {
            class_->next = (*NewRing)->next;
            (*NewRing)->next = class_;
        }
        *NewRing = class_;
    }
    // Now re-arrange segments in any order classes for which we have segments specified
    for( MatchClass = sect->orderlist; MatchClass != nullptr; MatchClass = MatchClass->NextClass ) {
        for( MatchSeg = MatchClass->SegList; MatchSeg != nullptr; MatchSeg = MatchSeg->NextSeg ) {
            class_ = MatchClass->Ring;
            do {
                prevseg = class_->segs;
                if( prevseg == nullptr )
                    break;
                currseg = prevseg->next;

                for( ;; ) {
                    if( stricmp( currseg->segname.u.ptr, MatchSeg->Name ) == 0 ) {
                        if( MatchSeg->FixedAddr ) {    // and copy any flags or address from it
                            currseg->info |= SEGINF_FIXED;
                            currseg->seg_addr = MatchSeg->Base;
                        }
                        if( MatchSeg->NoEmit ) {
                            currseg->info |= SEGINF_NOEMIT;
                        }
                        RingPromote( &class_->segs, currseg, prevseg );
                        break;
                    }
                    if( currseg == class_->segs ) {
                        break;
                    }
                    prevseg = currseg;
                    currseg = currseg->next;
                }
                class_ = class_->next;
            } while( class_ != MatchClass->Ring );
        }
    }
    /* now construct list out of the collected parts. */
    owner = &sect->classes;
    for( MatchClass = sect->orderlist; MatchClass != nullptr; MatchClass = MatchClass->NextClass ) {
        if( MatchClass->Ring != nullptr ) {
            *owner = MatchClass->Ring->next;
            owner = &(MatchClass->Ring->next);
        }
    }
    if( DefaultRing != nullptr ) { // Finish with unmatched ones
        *owner = DefaultRing->next;
        owner = &(DefaultRing->next);
    }
    *owner = nullptr;
    // This has to happen after the class list is rebuilt, so it can be searched
    for( MatchClass = sect->orderlist; MatchClass != nullptr; MatchClass = MatchClass->NextClass ) {
         if( MatchClass->Copy
           && MatchClass->Ring != nullptr ) {   // If this is a duplicate destination, find the source
             for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
                if( stricmp( MatchClass->SrcName, class_->name.u.ptr ) == 0 ) {
                    MatchClass->Ring->DupClass = class_;
                    MatchClass->Ring->flags |= CLASS_COPY;
                    break;
                }
            }
        }
    }

}

static void SortSegments(Link& link)
/******************************/
// reorder segments based on COFF name grouping
{
    class_entry         *class_;
    seg_leader          *prev;
    seg_leader          *curr;
    seg_leader          *comp;
    seg_leader          *newlist;
    bool                foundgroup;     // so we don't search for OMF only.
    bool                foundmatch;
    bool                added;


    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
        foundgroup = false;
        newlist = nullptr;
        for( curr = static_cast<seg_leader *>( RingPop( &class_->segs ) ); curr != nullptr; curr = static_cast<seg_leader *>( RingPop( &class_->segs ) ) ) {
            const std::string_view currname( curr->segname.u.ptr );
            const std::size_t currlen = currname.substr( 0, currname.find( '$' ) ).size();
            if( currlen != currname.size() ) {
                foundgroup = true;
            }
            added = false;
            if( foundgroup ) {
                prev = nullptr;
                foundmatch = false;
                for( comp = nullptr; (comp = static_cast<seg_leader *>( RingStep( newlist, comp ) )) != nullptr; ) {
                    const std::string_view compname( comp->segname.u.ptr );
                    const std::size_t complen = compname.substr( 0, compname.find( '$' ) ).size();
                    if( ( complen == currlen )
                      && ( compname.substr( 0, complen ) == currname.substr( 0, currlen ) ) ) {
                        foundmatch = true;
                        if( compname.substr( complen ).compare( currname.substr( currlen ) ) > 0 ) {
                            InsertPrevRing( &newlist, curr, prev );
                            added = true;
                            break;
                        }
                    } else if( foundmatch ) {
                        InsertPrevRing( &newlist, curr, prev );
                        added = true;
                        break;
                    }
                    prev = comp;
                }
            }
            if( !added ) {
                RingAppend( &newlist, curr );
            }
        }
        class_->segs = newlist;
    }
}

void CheckClassOrder(Link& link)
/*********************************/
/* Reorder the classes if DOSSEG flag set or ORDER directive given */
{
    SortSegments(link);
    if( link.LinkState & LS_SPEC_ORDER_FLAG ) {
       WalkAllSects(link,  SortClasses );
    } else if( link.LinkState & LS_DOSSEG_FLAG ) {
       WalkAllSects(link,  ReOrderClasses );
    }
}
