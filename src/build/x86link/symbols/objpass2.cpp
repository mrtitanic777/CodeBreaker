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
* Description:  Linker pass 2 routines
*
****************************************************************************/


#include <string.h>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/base/newmem.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/objects/omfreloc.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/map/map.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/layout/distrib.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/symbols/objpass2.h"



void ObjPass2(Link& link)
/**************************/
/* Pass 2 of 8086 linker. */
{
    DEBUG(( DBG_BASE, "ObjPass2()" ));
    IncP2Start(link);
    WalkAllSects(link,  bind_link( link, DBIP2Start ) );
    link.CurrSect = link.Root;/*  TAI */
    PModList(link,  link.Root->mods );
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        OvlPass2(link);
    }
    if( (link.FmtData.type & MK_OVERLAYS)
      && link.FmtData.u.dos.distribute ) {
        DistribProcMods(link);
    } else {
#endif
        link.CurrSect = link.Root;
        PModList(link,  link.LibModules );
#ifdef _EXE
    }
#endif
    if( link.MapFile != nullptr ) {
        WriteMapTracedSyms(link);
    }
    PrintUndefinedSyms(link);
    if( link.LinkState & LS_UNDEFED_SYM_ERROR ) {
        link.LinkState |= LS_LINK_ERROR;
    }
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        OvlSetStartAddr(link);
    }
#endif
    WalkAllSects(link,  bind_link( link, DBIFini ) );
}

void PModList(Link& link,  mod_entry *head )
/*************************************/
{
    mod_entry           *obj;

    for( obj = head; obj != nullptr; obj = obj->u.next ) {
        PModule(link,  obj );
    }
}

void PModule(Link& link,  mod_entry *obj )
/***********************************/
{
    if( (obj->flags_mod & MOD_NEED_PASS_2) == 0 )
        return;
    DEBUG(( DBG_BASE, "2 : processing module %s", obj->name ));
    link.CurrMod = obj;
    IterateModRelocs( link.CurrMod->relocs, link.CurrMod->sizerelocs, bind_link( link, IncExecRelocs ) );
    DoBakPats();
    DBIGenModule(link);
    CheckStop(link);
}

bool LoadObj(Link& link,  segdata *seg )
/*********************************/
{
    seg_leader *leader;

    leader = seg->u.leader;
    if( ( leader == nullptr )
      || DBISkip(link,  leader ) )
        return( false );
    link.CurrRec.seg = seg;
    if( leader->group == nullptr ) {
        link.CurrRec.addr = leader->seg_addr;
#ifdef DEVBUILD
        LnkMsg( WRN+MSG_INTERNAL, "s", "null leader group found" );
#endif
    } else {
        link.CurrRec.addr = leader->group->addr;
        link.CurrRec.addr.off += SEG_GROUP_DELTA( leader );
    }
    link.CurrRec.addr.off += seg->a.delta;
    return( true );
}
