/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Variable splitting - find cases where a var is used for
*               separate and independent purposes.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "stackcg.h"
#include "data.h"
#include "namelist.h"
#include "regalloc.h"
#include "insdead.h"
#include "conflict.h"
#include "dataflo.h"
#include "varusage.h"
#include "splitvar.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct SplitvarState: splitvar_state.h) */

}  // namespace


static  void            *MarkInstance(BackRun& back, block *blk);

static  block   *FindUnMarkedInstance( BackRun& back )
/*******************************************/
{
    block               *blk;
    data_flow_def       *flow;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( IsBlkVisited( blk ) )
            continue;
        flow = blk->dataflow;
        if( GBitOverlap( back.splitvar.Id, flow->in )
          || GBitOverlap( back.splitvar.Id, flow->out )
          || GBitOverlap( back.splitvar.Id, flow->def )
          || GBitOverlap( back.splitvar.Id, flow->use ) ) {
            break;
        }
    }
    return( blk );
}


static  void    NotVisited( BackRun& back )
/********************************/
{
    block       *blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        MarkBlkUnVisited( blk );
        blk->blk_id = 0;
    }
}


bool    RepOp( BackRun& back, name **pop, name *of, name *with )
/***********************************************/
{
    name        *op;
    name        *base;
    name        *index;
    bool        change;

    op = *pop;
    change = false;
    if( op == of ) {
        *pop = with;
        change = true;
    } else if( op->n.class_ == N_INDEXED ) {
        base = op->i.base;
        if( HasTrueBase( op ) && base == of ) {
            base = with;
            change = true;
        }
        index = op->i.index;
        if( index == of ) {
            index = with;
            change = true;
        }
        if( change ) {
            *pop = ScaleIndex( back, index, base, op->i.constant,
                                op->n.type_class, op->n.size,
                                op->i.scale, op->i.index_flags );
        }
    }
    return( change );
}


static  void    ReplaceInstances( BackRun& back, name *of, name *with )
/******************************************************/
{
    block       *blk;
    instruction *ins;
    opcnt       i;
//    block       *replaced;

//    replaced = NULL;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( blk->blk_id == back.splitvar.Instance ) {
            for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
                for( i = ins->num_operands; i-- > 0; ) {
                    RepOp( back, &ins->operands[i], of, with );
                }
                if( ins->result != NULL ) {
                    RepOp( back, &ins->result, of, with );
                }
            }
        }
    }
}


static  bool    Split1Var( BackRun& back, conflict_node *conf )
/**********************************************/
{
    block       *unlabeled;
    bool        change;
    name        *op;

    NotVisited(back);
    back.splitvar.Instance = 0;
    GBitAssign( back.splitvar.Id, conf->id.out_of_block );
    for( ;; ) {
        unlabeled = FindUnMarkedInstance(back);
        if( unlabeled == NULL )
            break;
        ++back.splitvar.Instance;
        MarkInstance( back, unlabeled );
    }
    op = conf->name;
    change = false;
    while( back.splitvar.Instance > 1 ) {
        change = true;
        ReplaceInstances( back, op, SAllocTemp( back, op->n.type_class, op->n.size ) );
        --back.splitvar.Instance;
    }
    return( change );
}


static  void    CleanUp( BackRun& back )
/*****************************/
{
    block       *blk;
    block_id    blk_id;

    blk_id = 1;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        MarkBlkUnVisited( blk );
        blk->blk_id = blk_id++;
    }
}


static  void *MarkInstance( BackRun& back, block *blk )
/**************************************/
{
    block_edge          *edge;
    block_num           i;
    data_flow_def       *flow;
    global_bit_set      *bitp;

    if( IsBlkVisited( blk ) )
        return( NULL );
    MarkBlkVisited( blk );
    blk->blk_id = back.splitvar.Instance;
    flow = blk->dataflow;
    if( GBitOverlap( flow->in, back.splitvar.Id ) ) {
        for( edge = blk->input_edges; edge != NULL; edge = edge->next_source ) {
            bitp = &edge->source->dataflow->out;
            if( GBitOverlap( *bitp, back.splitvar.Id ) ) {
                SafeRecurseCG( back, (func_sr)MarkInstance, edge->source );
            }
        }
    }
    if( GBitOverlap( flow->out, back.splitvar.Id ) ) {
        edge = &blk->edge[0];
        for( i = blk->targets; i > 0; --i ) {
            bitp = &edge->destination.u.blk->dataflow->in;
            if( GBitOverlap( *bitp, back.splitvar.Id ) ) {
                SafeRecurseCG( back, (func_sr)MarkInstance, edge->destination.u.blk );
            }
            ++edge;
        }
    }
    return( NULL );
}


 void    SplitVars( BackRun& back )
/************************/
/* For each variable, find out if it can be split into two separate variables.*/
/* This often happens when programmers re-use variables rather than defining*/
/* a new one.*/
{
    name                *op;
    conflict_node       *conf;

    for( ;; ) {
        for( conf = back.data.ConfList; conf != NULL; conf = conf->next_conflict ) {
            op = conf->name;
            if( (op->v.usage & USE_IN_ANOTHER_BLOCK) == 0 )
                continue;
            if( op->n.class_ != N_TEMP )
                continue;
            if( HasConflictState( conf, CST_CONF_VISITED ) )
                continue;
            SetFalse( conf, CST_CONFLICT_ON_HOLD );
            if( GBitEmpty( conf->id.out_of_block ) )
                continue;
            if( op->t.alias == op )
                Split1Var( back, conf );
            GBitInit( conf->id.out_of_block, EMPTY );
            SetTrue( conf, CST_CONFLICT_ON_HOLD | CST_CONF_VISITED );
        }
        if( !MoreConflicts(back) ) {
            break;
        }
    }
    CleanUp(back);
    FreeConflicts(back);
    InsDead(back);
    NullConflicts( back, static_cast<var_usage>(EMPTY) );
    FindReferences(back);
    MakeConflicts(back);
}
