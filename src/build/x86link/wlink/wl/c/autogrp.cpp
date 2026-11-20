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
* Description:  Routines for creating auto groups and packing segments
*
****************************************************************************/


#include <string.h>
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "ring.h"
#include "pcobj.h"
#include "specials.h"
#include "overlays.h"
#include "strtab.h"
#include "carve.h"
#include "permdata.h"
#include "objcalc.h"
#include "objnode.h"
#include "objpass1.h"
#include "symmem.h"

#include "run_state.hpp"
#include "clibext.h"



namespace {

struct AutoGrpState {
    group_entry     *curr_group = nullptr;
};

AutoGrpState &state()
{
    return cb_wlink::RunState::part<AutoGrpState>();
}

}  // namespace

static offset GetSegGroupPackLimit( seg_leader *seg )
/***************************************************/
// set packlimit if necessary.
{
    offset      limit;

    if( seg->info & SEGINF_CODE ) {
        if( wlink().LinkFlags & LF_PACKCODE_FLAG ) {
            return( wlink().PackCodeLimit );
        }
    } else {
        if( wlink().LinkFlags & LF_PACKDATA_FLAG ) {
            return( wlink().PackDataLimit );
        }
    }
    if( seg->info & SEGINF_USE_32 ) {
        limit = 0xFFFFFFFF;
    } else if( wlink().FmtData.type & MK_WIN_NE ) { /* windows doesn't like */
        limit = 0xFFF0;                     /* large code segments */
    } else {
        limit = 0xFFFF;
    }
    return( limit );
}

static seg_leader *GetNextSeg( section *sect, seg_leader *seg )
/*************************************************************/
/*
 * if seg == nullptr then get first segment in section
 * if seg != nullptr then get next segment after seg
 *
 * segments with debug info are skiped
 *
 * TODO!
 * now it goes through sorted class list
 * it can not handle DOSSEG segment order properly if class contains some segments
 * which are and are not member of DGROUP
 * it should be replaced by sorted segment list
 */
{
    class_entry *class_;

    if( seg == nullptr ) {
        for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
            if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) {
                break;
            }
        }
        if( class_ == nullptr ) {
            return( nullptr );
        }
    } else {
        class_ = seg->class_;
    }
    while( (seg = static_cast<seg_leader *>( RingStep( class_->segs, seg ) )) == nullptr ) {
        for( class_ = class_->next; class_ != nullptr; class_ = class_->next ) {
            if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) {
                break;
            }
        }
        if( class_ == nullptr ) {
            return( nullptr );
        }
    }
    return( seg );
}

static bool CanPack( seg_leader *seg1, seg_leader *seg2 )
/*******************************************************/
{
    if( wlink().FmtData.type & MK_WIN_VXD ) {
        // A VxD's LE objects are grouped BY CLASS, exactly as MS LINK does: the
        // DDK gives every locked segment (_LTEXT/_LDATA/_TEXT/_DATA/_BSS/CONST)
        // the class 'LCODE', so they merge into one locked object (obj1, holding
        // the code + DDB + service table), while INIT ('ICODE') and each pageable
        // message segment get their own class and stay separate. Retail VKD =
        // 10 objects that way. Packing UNRELATED flat segments (as the OS/2-flat
        // default does) collapsed VKD to 2 objects and diverged VMM's device-init;
        // never-packing over-split it to 11 (DDB/table in their own object). Pack
        // iff same class -> reproduces the retail per-class object layout.
        return( seg1->class_ == seg2->class_ );
    }
    if( seg1->info & SEGINF_CODE ) {
        if( seg2->combine == COMBINE_INVALID ) {
            return( false );
        }
    } else {
        if( seg1->align != seg2->align ) {
            return( false );
        }
    }
    if( (seg1->info & (SEGINF_USE_32 | SEGINF_CODE)) != (seg2->info & (SEGINF_USE_32 | SEGINF_CODE)) )
        return( false );
    if( seg1->segflags != seg2->segflags
      || (seg2->info & SEGINF_FIXED) )
        return( false );
    if( seg1->class_ != seg2->class_
      && (seg2->class_->flags & CLASS_FIXED) )
        return( false );
    if( seg1->group != seg2->group )
        return( false );
    return( true );
}

static group_entry *GetAutoGroup( bool abs_seg )
/***********************************************/
{
    group_entry      *group;
    group_entry **   grp_list;

    if( abs_seg ) {
        grp_list = &wlink().AbsGroups;
    } else {
        grp_list = &wlink().Groups;
    }
    group = AllocGroup( AutoGrpName, grp_list );
    if( !abs_seg ) {
        state().curr_group = group;
    }
    return( group );
}

static void PackSegs( seg_leader *seg, unsigned num_segs )
/********************************************************/
{
    group_entry         *group;

    if( num_segs == 0 )
        return;

    if( seg->group != nullptr ) {
        group = seg->group;
    } else {
        group = GetAutoGroup( seg->info & SEGINF_ABSOLUTE );
    }
    group->section = seg->class_->section;
    while( num_segs != 0 ) {
        if( seg->group == nullptr
          || seg->group == group ) {
            if( (seg->info & SEGINF_CODE) == 0 ) {
                group->segflags |= SEG_DATA;
            }
            if( (seg->class_->flags & CLASS_READ_ONLY) == 0 ) {
                group->segflags &= ~SEG_READ_ONLY;
            }
            if( seg->class_->flags & CLASS_COPY ) {  // If class is copied, mark group accordingly
                group->isdup = true;
            }
            if( seg->group == nullptr ) {              // if its not in a group add it to this one
                seg->group = group;
                Ring2Append( &group->leaders, seg );
            }
            --num_segs;
        }
        seg = GetNextSeg( group->section, seg );
    }
}

static void AutoGroupSect( section *sect )
/****************************************/
{
    seg_leader      *seg;
    seg_leader      *packstart;
    offset          size;
    offset          new_size;
    offset          align_size;
    unsigned        num_segs;
    bool            lastseg = false;    // true iff this should be last seg in group.
    offset          limit = 0;

    wlink().CurrentSeg = nullptr;
    size = 0;
    num_segs = 0;
    packstart = nullptr;
    for( seg = nullptr; (seg = GetNextSeg( sect, seg )) != nullptr; ) {
        if( seg->info & SEGINF_ABSOLUTE ) {
            PackSegs( seg, 1 );
        } else {
            if( packstart == nullptr ) {
                limit = GetSegGroupPackLimit( seg );
                packstart = seg;
            }
            align_size = CAlign( size, seg->align );
            new_size = align_size + seg->size;
            if( ( new_size >= limit )      // group overflow 16/32-bit
              || ( new_size < size )     // group overflow 32-bit
              || lastseg
              || !CanPack( packstart, seg ) ) {
                PackSegs( packstart, num_segs );
                packstart = seg;
                num_segs = 1;
                if( wlink().FmtData.type & MK_REAL_MODE ) {
                    size = ( align_size & 0xF ) + seg->size;
                } else {
                    size = seg->size;
                }
                lastseg = false;
                limit = GetSegGroupPackLimit( seg );
            } else {
                size = new_size;
                ++num_segs;
            }
            if( seg->info & SEGINF_LAST_SEGMENT ) {
                lastseg = true;
            }
        }
    }
    PackSegs( packstart, num_segs );
}

static void InitGroup( group_entry *group )
/*****************************************/
{
    SET_ADDR_UNDEFINED( group->addr );
    group->totalsize = 0;
    group->size = 0;
    group->segflags = DEFAULT_GRP_FLAGS;
    group->u.os2flags = 0;
    group->isfree = false;
    group->isautogrp = false;
    group->isdup = false;
    group->g.reloclist = nullptr;
}

group_entry *AllocGroup( const char *name, group_entry **grp_list )
/*****************************************************************/
{
    group_entry *group;
    symbol      *sym;

    group = static_cast<group_entry *>( CarveAlloc( wlink().CarveGroup ) );
    group->leaders = nullptr;
    sym = static_cast<symbol *>( _PermAlloc( sizeof( *sym ) ) ); // second class slave citizen
    BasicInitSym( sym );
    sym->namelen_cmp = strlen( name );
    sym->name.u.ptr = AddBufferStringTable( &wlink().PermStrings, name, sym->namelen_cmp + 1 );
    SET_SYM_TYPE( sym, SYM_GROUP );
    sym->info |= SYM_STATIC;
    group->next = nullptr;
    InitGroup( group );
    group->sym = sym;
    AppendList( reinterpret_cast<void **>( grp_list ), group );
    if( stricmp( name, DataGrpName ) == 0 ) {
        wlink().DataGroup = group;
    } else if( name == AutoGrpName ) {
        group->isautogrp = true;
    }
    return( group );
}

static void SortGroup( seg_leader *seg )
/**************************************/
// Go through the classes & segments, and rebuild the group list in sorted form
{
    if( seg->group == nullptr )
        return;
    if( seg->info & SEGINF_ABSOLUTE )
        return;
    Ring2Append( &seg->group->leaders, seg );
    if( seg->group->next == nullptr ) { // not in the list yet
        group_entry *&curr = state().curr_group;
        if( curr == nullptr ) {
            wlink().Groups = curr = seg->group;
        } else {
            curr->next = seg->group;
            curr = curr->next;
        }
        // Make the list circular so we have an easy way of telling if a node
        // is in the list.
        curr->next = wlink().Groups;
        wlink().NumGroups--;
        DbgAssert( wlink().NumGroups >= 0 );
    }
}

static void SortGroupList( void )
/*******************************/
// Sort the group list by segments within classes.
{
    group_entry     *group;
    unsigned        number;

    wlink().NumGroups = 0;
    if( wlink().Groups == nullptr )
        return;

    // first, set all of the links in the group list to nullptr
    while( (group = wlink().Groups) != nullptr ) {
        wlink().Groups = wlink().Groups->next;  // Take group out of original ring
        group->next = nullptr;
        group->leaders = nullptr;
        wlink().NumGroups++;
    }
    number = wlink().NumGroups;
    state().curr_group = nullptr;
    WalkLeaders( SortGroup );
    state().curr_group->next = nullptr;  // break the circular list.
    wlink().NumGroups = number;            // save # of groups.
}

#ifdef _EXE
static bool CheckGroupSplit( void *leader, void *sect )
/*****************************************************/
{
    return( static_cast<seg_leader *>( leader )->class_->section != static_cast<section *>( sect ) );
}

static void FindSplitGroups( void )
/*********************************/
// a group can be split by accidentally putting parts of it into different
// overlays. This causes all hell to break loose, so this checks to make sure
// that this doesn't happen.
{
    group_entry     *group;

    if( wlink().FmtData.type & MK_OVERLAYS ) {
        for( group = wlink().Groups; group != nullptr; group = group->next ) {
            if( Ring2Lookup( group->leaders, CheckGroupSplit, group->section ) ) {
                LnkMsg( ERR+MSG_OVL_GROUP_SPLIT, "s", group->sym->name );
            }
        }
    }
}
#endif

static void NumberNonAutos( void )
/********************************/
{
    group_entry         *group;
    unsigned            num;

    num = 0;
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( group->isautogrp ) {
            group->num = 0;
        } else {
            num++;
            group->num = num;
        }
    }
}

void AutoGroup( void )
/********************/
{
    WalkAllSects( &AutoGroupSect );
    SortGroupList();
#ifdef _EXE
    FindSplitGroups();
#endif
    if( wlink().NumGroups == 0 ) {
        LnkMsg( FTL+MSG_NO_CODE_OR_DATA, nullptr );
    }
    NumberNonAutos();
}
