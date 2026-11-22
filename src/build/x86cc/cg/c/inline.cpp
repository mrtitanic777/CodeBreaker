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


typedef struct inline_parm {
        struct inline_parm      *next;
        an                      addr;
} inline_parm;

typedef struct inline_stack {
        struct inline_stack     *next;
        struct inline_parm      *parms;
        cg_sym_handle           proc_sym;
        const type_def          *tipe;
        an                      addr;
} inline_stack;

namespace {

/* the statics of this file, one set for each run of the compiler */
struct InlineState {
    inline_stack *InlineStack = NULL;   // fix this!
};

}  // namespace

static inline InlineState& inline_state() { return cbrun_part<InlineState>(); }


void    BGStartInline( cg_sym_handle proc_sym )
/*********************************************/
{
    inline_stack        *stk;

    stk = static_cast<inline_stack *>(CGAlloc( sizeof( *stk ) ));
    stk->parms = NULL;
    stk->tipe = NULL;
    stk->addr = NULL;
    stk->proc_sym = proc_sym;
    stk->next = inline_state().InlineStack;
    inline_state().InlineStack = stk;
}


void    BGAddInlineParm( an addr )
/********************************/
{
    inline_parm *parm;

    parm = static_cast<inline_parm *>(CGAlloc( sizeof( *parm ) ));
    parm->addr = addr;
    parm->next = inline_state().InlineStack->parms;
    inline_state().InlineStack->parms = parm;
}


an      BGStopInline( call_handle call, const type_def *tipe )
/************************************************************/
{
    // works if we're in the middle of a conditional???

    label_handle    lbl;
    inline_stack    *junk;
    an              retv;

    if( !data_state().HaveCurrBlock ) {
        EnLink( AskForNewLabel() );
        data_state().CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
        data_state().HaveCurrBlock = true;
    }
    lbl = AskForNewLabel();
    GenBlock( BLK_JUMP, 1 );
    AddTarget( lbl, false );
    EnLink( lbl );
    data_state().CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    inline_state().InlineStack->tipe = tipe;
    FEGenProc( inline_state().InlineStack->proc_sym, call );
    retv = inline_state().InlineStack->addr;
    junk = inline_state().InlineStack;
    inline_state().InlineStack = inline_state().InlineStack->next;
    CGFree( junk );
    return( retv );
}


static bool NotEquiv( const type_def *a, const type_def *b )
/**********************************************************/
{
    if( TypeClass( a ) != TypeClass( b ) )
        return( true );
    if( a->length != b->length )
        return( true );
    return( false );
}


void    BGProcInline( cg_sym_handle proc_sym, const type_def *tipe )
/******************************************************************/
{
    if( inline_state().InlineStack->proc_sym != proc_sym || NotEquiv( inline_state().InlineStack->tipe, tipe ) ) {
        _Zoiks( ZOIKS_072 );
    }
}


void    BGParmInline( cg_sym_handle sym, const type_def *tipe )
/*************************************************************/
{
    name                *temp;
    name                *parm_value;
    inline_parm         *parm;

    parm = inline_state().InlineStack->parms;
    inline_state().InlineStack->parms = inline_state().InlineStack->parms->next;
//  if( NotEquiv( parm->addr->tipe, tipe ) ) {
//      _Zoiks( ZOIKS_070 );
//  }
    temp = SAllocUserTemp( sym, TypeClass( tipe ), tipe->length );
    temp->v.usage |= USE_IN_ANOTHER_BLOCK;
    parm_value = GenIns( parm->addr );
    BGDone( parm->addr );
    AddIns( MakeMove( parm_value, temp, temp->n.type_class ) );
    CGFree( parm );
}


void    BGRetInline( an addr, const type_def *tipe )
/**************************************************/
{
    an  tempaddr;

    if( NotEquiv( tipe, inline_state().InlineStack->tipe ) ) {
        _Zoiks( ZOIKS_070 );
    }
    if( addr == NULL ) {
        inline_state().InlineStack->addr = MakeTempAddr( BGNewTemp(data_state().TypeInteger) );
    } else {
        tempaddr = MakeTempAddr( BGNewTemp( tipe ) );
        inline_state().InlineStack->addr = BGCopy( tempaddr );
        BGDone( BGAssign( tempaddr, addr, tipe ) );
    }
}


bool    BGInInline( void )
/************************/
{
    return( inline_state().InlineStack != NULL );
}

int     BGInlineDepth( void )
/***************************/
{
    int                 depth;
    inline_stack        *curr;

    depth = 0;
    for( curr = inline_state().InlineStack; curr != NULL; curr = curr->next ) {
        depth++;
    }
    return( depth );
}
