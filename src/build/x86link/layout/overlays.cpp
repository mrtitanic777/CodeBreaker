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
* Description:  overlay support routines which can't be put in an overlay
*
****************************************************************************/


#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/base/newmem.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/debuginfo/dbgwat.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/layout/distrib.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/layout/distrib.h"


#ifdef _EXE

static void OvlNumASect(Link& link,  section *sect )
/**************************************/
{
    sect->ovlref = link.OvlSectNum++;
}

void OvlNumberSections(Link& link)
/****************************/
{
    if( link.FmtData.u.dos.distribute ) {
        DistribNumberSections(link);
    }
    /* OvlSectNum value 0 is reserved for Root */
    /* Overlayed sections start at 1 */
    link.OvlSectNum = 1;
    WalkAreas( link.Root->areas, bind_link( link, OvlNumASect ) );
}

void OvlSetSegments(Link& link)
/*************************/
{
    DistribSetSegments(link);
}

static void FillOutPtr(Link& link,  section *sect )
/*************************************/
{
    if( sect->outfile == nullptr ) {
        if( sect->parent != nullptr ) {
            sect->outfile = sect->parent->outfile;  //same file as ancestor.
        } else {
            sect->outfile = link.Root->outfile;
        }
    }
}

void OvlFillOutFilePtrs(Link& link)
/*****************************/
{
    WalkAreas( link.Root->areas, bind_link( link, FillOutPtr ) );
}

void OvlTryDefVector(Link& link,  symbol *sym )
/*********************************/
{
    if( sym->info & SYM_1_DISTRIB ) {
        DefDistribSym(link,  sym );
    } else {
        OvlDefVector(link,  sym );
    }
}

void OvlTryUseVector(Link& link,  symbol *sym, extnode *newnode )
/***************************************************/
{
    if( newnode != nullptr ) {
        newnode->ovlref = 0;
        OvlUseVector(link,  sym, newnode );
    }
}

static void PSection(Link& link,  section *sect )
/***********************************/
{
    link.CurrSect = sect;
    PModList(link,  sect->mods );
}

void OvlPass2(Link& link)
/*******************/
{
    OvlEmitVectors(link);
    WalkAreas( link.Root->areas, bind_link( link, PSection ) );
}

#endif
