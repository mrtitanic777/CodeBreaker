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

#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "reloc.h"
#include "objnode.h"
#include "overlays.h"
#include "library.h"
#include "virtmem.h"
#include "objcache.h"
#include "loados2.h"
#include "dbgall.h"
#include "objio.h"
#include "wcomdef.h"
#include "objorl.h"
#include "loadfile.h"
#include "ring.h"
#include "carve.h"
#include "strtab.h"
#include "permdata.h"
#include "objpass1.h"
#include "objpass2.h"
#include "objfree.h"


void FiniLinkStruct( void )
/********************************/
/* Free object processing data structures. */
{
    DEBUG(( DBG_OLD, "FiniLinkStruct()" ));
    BurnNodes();
    FreeTokBuffs();
    CacheFini();
    FiniORLObj();
}

static void FreeClasses( class_entry *class_ )
/*******************************************/
{
    class_entry     *next;

    for( ; class_ != nullptr; class_ = next ) {
        next = class_->next;
        RingWalk( class_->segs, FreeLeader );
        CarveFree( wlink().CarveClass, class_ );
    }
}

void FreeAMod( mod_entry *mod )
/************************************/
{
    FreeObjCache( mod->u1.source );
    FreeModEntry( mod );
}

static void FreeMods( mod_entry *head )
/*************************************/
{
    void        *next;

    for( ; head != nullptr; head = static_cast<mod_entry *>( next ) ) {
        next = head->u.next;
        FreeAMod( head );
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

static void FreeAreas( AREASECT *area );

static void FreeSections( section *sect )
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
        if( (wlink().LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
            FreeMods( sect->mods );
            FreeClasses( sect->classes );
        }
        DBISectCleanup( sect );
        FreeAreas( sect->areas );
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

static void FreeAreas( AREASECT *area )
/*************************************/
{
    AREASECT    *next;

    for( ; area != nullptr; area = next ) {
        next = area->next;
        FreeSections( area->sections );
    }
}


void CleanLinkStruct( void )
/*********************************/
/* free all structures */
{
    if( wlink().Root == nullptr )
        return;  /* haven't finished initializing */
    BurnLibs();
    FreeFiles( wlink().ObjLibFiles );
    FreeFiles( wlink().Root->files );
    wlink().ObjLibFiles = nullptr;
    wlink().Root->files = nullptr;
    if( (wlink().LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        FreeMods( wlink().LibModules );
    }
    if( wlink().SymFileName != nullptr ) {
        MemFree( wlink().SymFileName );
    }
    if( wlink().FmtData.osname != nullptr ) {
        MemFree( wlink().FmtData.osname );
    }
    if( wlink().FmtData.resource != nullptr ) {
        MemFree( wlink().FmtData.resource );
    }
    FreeRelocInfo();
    FreeGroups( wlink().Groups );
    FreeGroups( wlink().AbsGroups );
    wlink().Groups = nullptr;
    wlink().AbsGroups = nullptr;
    FreeSections( wlink().Root );
    DBICleanup();
    wlink().Root = nullptr;
#ifdef _EXE
    if( wlink().FmtData.type & MK_DOS ) {
        FreeOverlaySupp();
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

void FreeObjInfo( void )
/*****************************/
{
    FreeNodes( wlink().ExtNodes );
    FreeNodes( wlink().SegNodes );
    FreeNodes( wlink().GrpNodes );
}

static void FreeAGroup( group_entry *group )
/******************************************/
/* free all memory associated with group */
{
    CarveFree( wlink().CarveGroup, group );
}

void FreeGroups( group_entry *group )
/***********************************/
{
    group_entry *next;

    for( ; group != nullptr; group = next ) {
        next = group->next;
        FreeAGroup( group );
    }
}
