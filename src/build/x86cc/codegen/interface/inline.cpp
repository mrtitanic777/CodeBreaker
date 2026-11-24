/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "makeins.h"
#include "data.h"
#include "types.h"
#include "makeaddr.h"
#include "makeblk.h"
#include "namelist.h"
#include "typemap.h"
#include "inline.h"
#include "bldins.h"
#include "feprotos.h"
#include "backrun.hpp"






namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct InlineState: inline_state.h) */

}  // namespace



void    BGStartInline( BackRun& back, cg_sym_handle proc_sym )
/*********************************************/
{
    inline_stack        *stk;

    stk = static_cast<inline_stack *>(CGAlloc( back, sizeof( *stk ) ));
    stk->parms = NULL;
    stk->tipe = NULL;
    stk->addr = NULL;
    stk->proc_sym = proc_sym;
    stk->next = back.inline_part.InlineStack;
    back.inline_part.InlineStack = stk;
}


void    BGAddInlineParm( BackRun& back, an addr )
/********************************/
{
    inline_parm *parm;

    parm = static_cast<inline_parm *>(CGAlloc( back, sizeof( *parm ) ));
    parm->addr = addr;
    parm->next = back.inline_part.InlineStack->parms;
    back.inline_part.InlineStack->parms = parm;
}


an      BGStopInline( BackRun& back, call_handle call, const type_def *tipe )
/************************************************************/
{
    // works if we're in the middle of a conditional???

    label_handle    lbl;
    inline_stack    *junk;
    an              retv;

    if( !back.data.HaveCurrBlock ) {
        EnLink( back, AskForNewLabel(back) );
        back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
        back.data.HaveCurrBlock = true;
    }
    lbl = AskForNewLabel(back);
    GenBlock( back, BLK_JUMP, 1 );
    AddTarget( back, lbl, false );
    EnLink( back, lbl );
    back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    back.inline_part.InlineStack->tipe = tipe;
    FEGenProc( *back.front, back.inline_part.InlineStack->proc_sym, call );
    retv = back.inline_part.InlineStack->addr;
    junk = back.inline_part.InlineStack;
    back.inline_part.InlineStack = back.inline_part.InlineStack->next;
    CGFree( back, junk );
    return( retv );
}


static bool NotEquiv( BackRun& back, const type_def *a, const type_def *b )
/**********************************************************/
{
    if( TypeClass( back, a ) != TypeClass( back, b ) )
        return( true );
    if( a->length != b->length )
        return( true );
    return( false );
}


void    BGProcInline( BackRun& back, cg_sym_handle proc_sym, const type_def *tipe )
/******************************************************************/
{
    if( back.inline_part.InlineStack->proc_sym != proc_sym || NotEquiv( back, back.inline_part.InlineStack->tipe, tipe ) ) {
        Zoiks( back, ZOIKS_072 );
    }
}


void    BGParmInline( BackRun& back, cg_sym_handle sym, const type_def *tipe )
/*************************************************************/
{
    name                *temp;
    name                *parm_value;
    inline_parm         *parm;

    parm = back.inline_part.InlineStack->parms;
    back.inline_part.InlineStack->parms = back.inline_part.InlineStack->parms->next;
//  if( NotEquiv( parm->addr->tipe, tipe ) ) {
//      _Zoiks( ZOIKS_070 );
//  }
    temp = SAllocUserTemp( back, sym, TypeClass( back, tipe ), tipe->length );
    temp->v.usage |= USE_IN_ANOTHER_BLOCK;
    parm_value = GenIns( back, parm->addr );
    BGDone( back, parm->addr );
    AddIns( back, MakeMove( back, parm_value, temp, temp->n.type_class ) );
    CGFree( back, parm );
}


void    BGRetInline( BackRun& back, an addr, const type_def *tipe )
/**************************************************/
{
    an  tempaddr;

    if( NotEquiv( back, tipe, back.inline_part.InlineStack->tipe ) ) {
        Zoiks( back, ZOIKS_070 );
    }
    if( addr == NULL ) {
        back.inline_part.InlineStack->addr = MakeTempAddr( back, BGNewTemp(back, back.data.TypeInteger) );
    } else {
        tempaddr = MakeTempAddr( back, BGNewTemp( back, tipe ) );
        back.inline_part.InlineStack->addr = BGCopy( back, tempaddr );
        BGDone( back, BGAssign( back, tempaddr, addr, tipe ) );
    }
}


bool    BGInInline( BackRun& back )
/************************/
{
    return( back.inline_part.InlineStack != NULL );
}

int     BGInlineDepth( BackRun& back )
/***************************/
{
    int                 depth;
    inline_stack        *curr;

    depth = 0;
    for( curr = back.inline_part.InlineStack; curr != NULL; curr = curr->next ) {
        depth++;
    }
    return( depth );
}
