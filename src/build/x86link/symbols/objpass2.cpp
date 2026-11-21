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
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
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



void ObjPass2( void )
/**************************/
/* Pass 2 of 8086 linker. */
{
    DEBUG(( DBG_BASE, "ObjPass2()" ));
    IncP2Start();
    WalkAllSects( DBIP2Start );
    wlink().CurrSect = wlink().Root;/*  TAI */
    PModList( wlink().Root->mods );
#ifdef _EXE
    if( wlink().FmtData.type & MK_OVERLAYS ) {
        OvlPass2();
    }
    if( (wlink().FmtData.type & MK_OVERLAYS)
      && wlink().FmtData.u.dos.distribute ) {
        DistribProcMods();
    } else {
#endif
        wlink().CurrSect = wlink().Root;
        PModList( wlink().LibModules );
#ifdef _EXE
    }
#endif
    if( wlink().MapFile != nullptr ) {
        WriteMapTracedSyms();
    }
    PrintUndefinedSyms();
    if( wlink().LinkState & LS_UNDEFED_SYM_ERROR ) {
        wlink().LinkState |= LS_LINK_ERROR;
    }
#ifdef _EXE
    if( wlink().FmtData.type & MK_OVERLAYS ) {
        OvlSetStartAddr();
    }
#endif
    WalkAllSects( DBIFini );
}

void PModList( mod_entry *head )
/*************************************/
{
    mod_entry           *obj;

    for( obj = head; obj != nullptr; obj = obj->u.next ) {
        PModule( obj );
    }
}

void PModule( mod_entry *obj )
/***********************************/
{
    if( (obj->flags_mod & MOD_NEED_PASS_2) == 0 )
        return;
    DEBUG(( DBG_BASE, "2 : processing module %s", obj->name ));
    wlink().CurrMod = obj;
    IterateModRelocs( wlink().CurrMod->relocs, wlink().CurrMod->sizerelocs, IncExecRelocs );
    DoBakPats();
    DBIGenModule();
    CheckStop();
}

bool LoadObj( segdata *seg )
/*********************************/
{
    seg_leader *leader;

    leader = seg->u.leader;
    if( ( leader == nullptr )
      || DBISkip( leader ) )
        return( false );
    wlink().CurrRec.seg = seg;
    if( leader->group == nullptr ) {
        wlink().CurrRec.addr = leader->seg_addr;
#ifdef DEVBUILD
        LnkMsg( WRN+MSG_INTERNAL, "s", "null leader group found" );
#endif
    } else {
        wlink().CurrRec.addr = leader->group->addr;
        wlink().CurrRec.addr.off += SEG_GROUP_DELTA( leader );
    }
    wlink().CurrRec.addr.off += seg->a.delta;
    return( true );
}
