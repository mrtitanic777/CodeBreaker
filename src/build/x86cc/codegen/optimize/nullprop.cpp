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
* Description:  Propagate null pointer comparisons.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "data.h"
#include "stackcg.h"
#include "redefby.h"
#include "nullprop.h"
#include "insdead.h"
#include "blktrim.h"
#include "backrun.hpp"


void    ClearBlocksBitsMask( BackRun& back, block_class mask )
/*********************************************/
{
    block               *blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        blk->class_ &= mask;
    }
}

static  instruction     *CompareIns( block *blk )
/***********************************************/
{
    instruction         *last;

    if( _IsBlkAttr( blk, BLK_CONDITIONAL ) ) {
        for( last = blk->ins.head.prev; last->head.opcode != OP_BLOCK; last = last->head.prev ) {
            if( _OpIsCompare( last->head.opcode ) ) {
                return( last );
            }
        }
    }
    return( NULL );
}

static  bool            IsZero( name *op )
/****************************************/
{
    return( op->n.class_ == N_CONSTANT && op->c.const_type == CONS_ABSOLUTE && op->c.lo.u.int_value == 0 );
}


typedef struct null_edge_entry {
    struct null_edge_entry   *next;
    block_edge          *edge;
} null_edge_entry;

typedef struct null_edge_stack {
    null_edge_entry          *top;
} null_edge_stack;

static  null_edge_stack      *InitStack( BackRun& back )
/****************************************/
{
    null_edge_stack          *stk;

    stk = static_cast<null_edge_stack *>(CGAlloc( back, sizeof( null_edge_stack ) ));
    stk->top = NULL;
    return( stk );
}

static  bool            Empty( null_edge_stack *stk )
/**********************************************/
{
    return( stk->top == NULL );
}

static  void            Push( BackRun& back, null_edge_stack *stk, block_edge *edge )
/***************************************************************/
{
    null_edge_entry          *new_entry;

    new_entry = static_cast<null_edge_entry *>(CGAlloc( back, sizeof( null_edge_entry ) ));
    new_entry->edge = edge;
    new_entry->next = stk->top;
    stk->top = new_entry;
}

static  block_edge      *Pop( BackRun& back, null_edge_stack *stk )
/*********************************************/
{
    null_edge_entry          *top;
    block_edge          *edge;

    if( stk->top != NULL ) {
        top = stk->top;
        edge = top->edge;
        stk->top = top->next;
        CGFree( back, top );
        return( edge );
    }
    return( NULL );
}

static  void            FiniStack( BackRun& back, null_edge_stack *stk )
/**************************************************/
{
    while( !Empty( stk ) ) {
        Pop( back, stk );
    }
    CGFree( back, stk );
}

static  bool            DereferencedBy( instruction *ins, name *ptr )
/********************************************************************
    Return true if the instruction dereferences the given name. Any use as
    an index (with op as index), providing the base is NULL, is considered a deref.
*/
{
    opcnt               i;
    name                *op;

    op = ins->result;
    for( i = 0; i <= ins->num_operands; i++ ) {
        if( op != NULL && op->n.class_ == N_INDEXED ) {
            if( ptr == op->i.index ) {
                if( op->i.base == NULL ) {
                    return( true );
                }
            }
        }
        op = ins->operands[i];
    }
    return( false );
}

static  void            PushTargets( BackRun& back, null_edge_stack *stk, block *blk, bool forward )
/******************************************************************************/
{
    block_num           i;
    block_edge          *edge;

    if( forward ) {
        for( i = 0; i < blk->targets; i++ ) {
            Push( back, stk, &blk->edge[i] );
        }
    } else {
        for( edge = blk->input_edges; edge != NULL; edge = edge->next_source ) {
            Push( back, stk, edge );
        }
    }
}

static  instruction     *NextIns( instruction *ins, bool forward )
/****************************************************************/
{
    instruction         *next;

    if( forward ) {
        next = ins->head.next;
    } else {
        next = ins->head.prev;
    }
    return( next );
}

static  instruction     *FirstIns( block *blk, bool forward )
/***********************************************************/
{
    instruction         *first;

    if( forward ) {
        first = blk->ins.head.next;
    } else {
        first = blk->ins.head.prev;
    }
    return( first );
}

static  block           *EdgeBlock( block_edge *edge, bool forward )
/******************************************************************/
{
    if( forward ) {
        return( edge->destination.u.blk );
    } else {
        return( edge->source );
    }
}

static  bool            LastBlock( BackRun& back, block *blk, bool forward )
/***********************************************************/
{
    if( forward ) {
        if( _IsBlkAttr( blk, BLK_RETURN ) ) {
            return( true );
        }
    } else {
        if( blk == back.data.HeadBlock ) {
            return( true );
        }
    }
    return( false );
}

enum {
    BLOCK_DEREFS,
    BLOCK_REDEFS,
    BLOCK_NOTHING
};

typedef struct goofy_struct_so_we_can_use_saferecurse {
    block       *blk;
    instruction *ins;
    name        *op;
    bool        forward;
} parm_struct;

static  void            *DominatingDeref( BackRun& back, parm_struct *parms );

static  int             BlockSearch( BackRun& back, block *blk, instruction *ins, name *op, bool forward )
/******************************************************************************************
    Search the block for something interesting - either a dereference or a
    redefinition of the given operand.
*/
{
    instruction         *curr;

    /* unused parameters */ (void)blk;

    for( curr = ins; curr->head.opcode != OP_BLOCK; curr = NextIns( curr, forward ) ) {
        // if we are going forward, the dereferences take precedence over the
        // redefinitions, the reverse is true when backpeddling
        if( forward ) {
            if( DereferencedBy( curr, op ) )
                return( BLOCK_DEREFS );
            if( IsReDefinedBy( back, curr, op ) )
                return( BLOCK_REDEFS );
        } else {
            if( IsReDefinedBy( back, curr, op ) )
                return( BLOCK_REDEFS );
            if( DereferencedBy( curr, op ) ) {
                return( BLOCK_DEREFS );
            }
        }
    }
    return( BLOCK_NOTHING );
}

static  void            *DominatingDeref( BackRun& back, parm_struct *parms )
/*************************************************************
    Return true if the given instruction is dominated by a dereference of op. This is not a true
    dominator in the sense of the dragon book, but a dominator in the sense that every path from
    the instruction given to the return encounters a dereference of op (if forward) or every
    path from the first instruction in the function to the instruction given encounters a deref
    of op (if backwards or forwards = false ).
*/
{
    block_edge          *edge;
    null_edge_stack          *stk;
    int                 result;
    bool                dominated;
    block               *blk;


    // check instructions from ins to end of block
    // also check that op is USE_IN_ANOTHER_BLOCK
    result = BlockSearch( back, parms->blk, NextIns( parms->ins, parms->forward ), parms->op, parms->forward );
    switch( result ) {
    case BLOCK_DEREFS:
        return( TO_SR_VALUE( true ) );
    case BLOCK_REDEFS:
        return( TO_SR_VALUE( false ) );
    }
    if( LastBlock( back, parms->blk, parms->forward ) || (parms->op->v.usage & USE_IN_ANOTHER_BLOCK) == 0 )
        return( TO_SR_VALUE( false ) );
    stk = InitStack(back);
    PushTargets( back, stk, parms->blk, parms->forward );
    for( dominated = true; dominated; ) {
        if( Empty( stk ) )
            break;
        edge = Pop( back, stk );
        blk = EdgeBlock( edge, parms->forward );
        if( _IsBlkVisited( blk ) )
            continue;
        result = BlockSearch( back, blk, FirstIns( blk, parms->forward ), parms->op, parms->forward );
        switch( result ) {
        case BLOCK_DEREFS:
            // continue on with the next block in the old depth-first search
            // this path is ok and we do not need to push any targets
            break;
        case BLOCK_REDEFS:
            // one path encountered a redefinition before it got a deref, so
            // the compare is not dominated by a dereference
            dominated = false;
            break;
        default:
            PushTargets( back, stk, blk, parms->forward );
            if( LastBlock( back, blk, parms->forward ) ) {
                // we have hit either the return or head of the function without
                // finding a deref along this particular path, so the compare is
                // not dominated by a dereference.
                dominated = false;
            }
        }
        _MarkBlkVisited( blk );
    }
    FiniStack( back, stk );
    return( TO_SR_VALUE( dominated ) );
}

static void     FloodDown( BackRun& back, block *blk, block_class bits )
/*******************************************************/
{
    null_edge_stack          *stk;
    block_edge          *edge;

    stk = InitStack(back);
    for( ;; ) {
        if( _IsBlkAttr( blk, bits ) ) {
            _MarkBlkAttrSet( blk, bits );
            PushTargets( back, stk, blk, true );
        }
        if( Empty( stk ) )
            break;
        edge = Pop( back, stk );
        blk = edge->destination.u.blk;
    }
    FiniStack( back, stk );
}

static  bool            BlockSideEffect( BackRun& back, block *blk )
/***************************************************/
{
    instruction         *ins;

    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( _OpIsCall( ins->head.opcode ) )
            return( true );
        if( SideEffect( back, ins ) ) {
            return( true );
        }
    }
    return( false );
}

static  bool            EdgeHasSideEffect( BackRun& back, block *blk, instruction *cmp, bool cmp_result )
/****************************************************************************************/
{
    null_edge_stack          *stk;
    block               *elim;
    block               *taken;
    bool                side_effect;
    block_edge          *edge;

    if( cmp_result ) {
        taken = blk->edge[_TrueIndex( cmp )].destination.u.blk;
        elim = blk->edge[_FalseIndex( cmp )].destination.u.blk;
    } else {
        taken = blk->edge[_FalseIndex( cmp )].destination.u.blk;
        elim = blk->edge[_TrueIndex( cmp )].destination.u.blk;
    }
    MarkBlkAllUnVisited( back );
    FloodDown( back, taken, BLK_BLOCK_VISITED );
    stk = InitStack(back);

    /*
     * Flood fill forward looking for a path which has a side effect on it.
     * Paths are killed when we hit a block already seen or which is in
     * the other edge of the graph (the edge which is taken).
     */
    for( side_effect = false; !side_effect; ) {
        if( !_IsBlkVisited( elim ) ) {
            if( BlockSideEffect( back, elim ) ) {
                side_effect = true;
                break;
            }
            // should add something here which cuts out if we see a
            // deref of the temp we are searching for - a call to
            // BlockSearch with the appropriate parms should do
            _MarkBlkVisited( elim );
            PushTargets( back, stk, elim, true );
        }
        if( Empty( stk ) )
            break;
        edge = Pop( back, stk );
        elim = edge->destination.u.blk;
    }
    FiniStack( back, stk );
    MarkBlkAllUnVisited( back );
    return( side_effect );
}

static  bool            NullProp( BackRun& back, block *blk )
/********************************************/
{
    instruction         *cmp;
    name                **ptr;
    parm_struct         parms;
    cond_dst_idx        dest_idx;

    cmp = CompareIns( blk );
    if( cmp == NULL )
        return( false );
    switch( cmp->head.opcode ) {
    case OP_CMP_EQUAL:
        dest_idx = _FalseIndex( cmp );
        break;
    case OP_CMP_NOT_EQUAL:
        dest_idx = _TrueIndex( cmp );
        break;
    default:
        return( false );
    }
    if( IsZero( cmp->operands[0] ) ) {
        ptr = &cmp->operands[1];
    } else if( IsZero( cmp->operands[1] ) ) {
        ptr = &cmp->operands[0];
    } else {
        return( false );
    }
    parms.blk = blk;
    parms.ins = cmp;
    parms.op = *ptr;
    parms.forward = true;
    MarkBlkAllUnVisited( back );
    if( FROM_SR_VALUE( DominatingDeref( back, &parms ), bool ) ) {
        if( !EdgeHasSideEffect( back, blk, cmp, cmp->head.opcode == OP_CMP_NOT_EQUAL ) ) {
            // only nuke the edge if the code we are removing
            // does not have a side effect or if the dominators are before the compare
            KillCondBlk( back, blk, cmp, dest_idx );
            return( true );
        }
    }
    parms.forward = false;
    MarkBlkAllUnVisited( back );
    if( FROM_SR_VALUE( DominatingDeref( back, &parms ), bool ) ) {
        KillCondBlk( back, blk, cmp, dest_idx );
        return( true );
    }
    return( false );
}

void            PropNullInfo( BackRun& back )
/***********************************
    Use pointer dereferences as information to enable folding of
    pointer comparisons versus NULL.
*/
{
    block               *blk;
    bool                change;

    if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION ) )
        return;
    if( IsModel( back, CGSW_GEN_NULL_DEREF_OK ) )
        return;
    change = false;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        change |= NullProp( back, blk );
    }
    MarkBlkAllUnVisited( back );
    if( change ) {
        BlockTrim(back);
    }
}
