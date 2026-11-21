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
* Description:  OBJFREE : free linker structures
*
****************************************************************************/

#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/symbols/library.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/formats/os2.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/symbols/wcomdef.h"
#include "build/x86link/objects/objorl.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/objects/objfree.h"


void FiniLinkStruct(Link& link)
/********************************/
/* Free object processing data structures. */
{
    DEBUG(( DBG_OLD, "FiniLinkStruct()" ));
    BurnNodes(link);
    FreeTokBuffs(link);
    CacheFini();
    FiniORLObj();
}

static void FreeClasses(Link& link,  class_entry *class_ )
/*******************************************/
{
    class_entry     *next;

    for( ; class_ != nullptr; class_ = next ) {
        next = class_->next;
        RingWalk( class_->segs, FreeLeader );
        CarveFree( link.CarveClass, class_ );
    }
}

void FreeAMod(Link& link,  mod_entry *mod )
/************************************/
{
    FreeObjCache( mod->u1.source );
    FreeModEntry(link,  mod );
}

static void FreeMods(Link& link,  mod_entry *head )
/*************************************/
{
    void        *next;

    for( ; head != nullptr; head = static_cast<mod_entry *>( next ) ) {
        next = head->u.next;
        FreeAMod(link,  head );
    }
}

static void FreeFiles( file_list *file )
/**************************************/
{
    void        *next;

    for( ; file != nullptr; file = static_cast<file_list *>( next ) ) {
        next = file->next;
        CacheClose( file, 3 );
        if( (file->flags_file & STAT_HAS_MEMBER) && file->u.member != nullptr ) {
            FreeList( file->u.member );
        }
        _PermFree( file );
    }
}

static void FreeAreas(Link& link,  AREASECT *area );

static void FreeSections(Link& link,  section *sect )
/****************************************
 * Free sections & classes.
 */
{
    section             *next;
    ORDER_CLASS         *Class;
    ORDER_CLASS         *NextClass;
    ORDER_SEGMENT       *Seg;
    ORDER_SEGMENT       *NextSeg;

    for( ; sect != nullptr; sect = next ) {
        next = sect->next;
        FreeFiles( sect->files );
        if( (link.LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
            FreeMods(link,  sect->mods );
            FreeClasses(link,  sect->classes );
        }
        DBISectCleanup(link,  sect );
        FreeAreas(link,  sect->areas );
        ZapHTable( sect->modFilesHashed, MemFree );
        // Free up any Order Class entries
        for( Class = sect->orderlist; Class != nullptr; Class = NextClass ) {
            NextClass = Class->NextClass;
            if( Class->Name != nullptr ) {   // Including members and sucessors
                MemFree( Class->Name );
            }
            if( Class->Copy ) {
                MemFree( Class->SrcName );
            }
            // Order Seg entries can also have members and sucessors
            for( Seg = Class->SegList; Seg != nullptr; Seg = NextSeg ) {
                NextSeg = Seg->NextSeg;
                if( Seg->Name != nullptr ) {
                    MemFree( Seg->Name );
                }
                MemFree( Seg );
            }
            MemFree( Class );
        }
        MemFree( sect );
    }
}

static void FreeAreas(Link& link,  AREASECT *area )
/*************************************/
{
    AREASECT    *next;

    for( ; area != nullptr; area = next ) {
        next = area->next;
        FreeSections(link,  area->sections );
    }
}


void CleanLinkStruct(Link& link)
/*********************************/
/* free all structures */
{
    if( link.Root == nullptr )
        return;  /* haven't finished initializing */
    BurnLibs(link);
    FreeFiles( link.ObjLibFiles );
    FreeFiles( link.Root->files );
    link.ObjLibFiles = nullptr;
    link.Root->files = nullptr;
    if( (link.LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        FreeMods(link,  link.LibModules );
    }
    if( link.SymFileName != nullptr ) {
        MemFree( link.SymFileName );
    }
    if( link.FmtData.osname != nullptr ) {
        MemFree( link.FmtData.osname );
    }
    if( link.FmtData.resource != nullptr ) {
        MemFree( link.FmtData.resource );
    }
    FreeRelocInfo(link);
    FreeGroups(link,  link.Groups );
    FreeGroups(link,  link.AbsGroups );
    link.Groups = nullptr;
    link.AbsGroups = nullptr;
    FreeSections(link,  link.Root );
    DBICleanup(link);
    link.Root = nullptr;
#ifdef _EXE
    if( link.FmtData.type & MK_DOS ) {
        FreeOverlaySupp(link);
    }
#endif
}

#if defined( _OS2 ) || defined( _QNX )
void FreeSegFlags( seg_flags_struct *curr )
/*****************************************/
{
    seg_flags_struct    *next;

    for( ; curr != nullptr; curr = next ) {
        next = curr->next;
        MemFree( curr->name );
        MemFree( curr );
    }
}
#endif

void FreeObjInfo(Link& link)
/*****************************/
{
    FreeNodes( link.ExtNodes );
    FreeNodes( link.SegNodes );
    FreeNodes( link.GrpNodes );
}

static void FreeAGroup(Link& link,  group_entry *group )
/******************************************/
/* free all memory associated with group */
{
    CarveFree( link.CarveGroup, group );
}

void FreeGroups(Link& link,  group_entry *group )
/***********************************/
{
    group_entry *next;

    for( ; group != nullptr; group = next ) {
        next = group->next;
        FreeAGroup(link,  group );
    }
}
