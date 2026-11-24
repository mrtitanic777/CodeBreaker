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
* Description:  Clean up basic blocks in a routine.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "data.h"
#include "makeins.h"
#include "edge.h"
#include "opttell.h"
#include "makeblk.h"
#include "nullprop.h"
#include "insutil.h"
#include "blktrim.h"
#include "backrun.hpp"


static  instruction     *FindOneCond( block *blk )
/************************************************/
{
    instruction *ins;
    instruction *cond;

    cond = NULL;
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( OpIsCondition( ins->head.opcode ) ) {
            if( cond != NULL )
                return( NULL );
            cond = ins;
        }
    }
    return( cond );
}


static  void    MarkReachableBlocks( BackRun& back )
/******************************************
 * Written NON-Recursively for a very good reason. (stack blew up)
 */
{
    block       *blk;
    block       *son;
    bool        change;
    block_num   i;

    do {
        change = false;
        for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
            if( IsBlkVisited( blk )
              || IsBlkAttr( blk, BLK_BIG_LABEL | BLK_SELECT ) ) {
                MarkBlkVisited( blk );
                for( i = blk->targets; i-- > 0; ) {
                    son = blk->edge[i].destination.u.blk;
                    if( !IsBlkVisited( son ) ) {
                        MarkBlkVisited( son );
                        change = true;
                    }
                }
            }
        }
    } while( change );
}


int     CountIns( block *blk )
/****************************/
{
    int         num_instrs;
    instruction *ins;

    num_instrs = 0;
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        num_instrs++;
    }
    return( num_instrs );
}


static  bool    FindBlock( BackRun& back, block *blk )
/*************************************/
{
    block       *curr;

    for( curr = back.data.HeadBlock; curr != NULL; curr = curr->next_block ) {
        if( curr == blk ) {
            return( true );
        }
    }
    return( false );
}


void    RemoveBlock( BackRun& back, block *blk )
/*******************************/
{
    block_num   i;
    unsigned    last_line;
    block       *chk;
    block       *next;
    instruction *next_ins;

    if( blk->prev_block != NULL ) {
        blk->prev_block->next_block = blk->next_block;
    }
    if( blk->next_block != NULL ) {
        blk->next_block->prev_block = blk->prev_block;
    }
    for( i = 0; i < blk->targets; ++i ) {
        /*
         * block may have already been removed by dead code removal
         */
        if( FindBlock( back, blk->edge[i].destination.u.blk ) ) {
            RemoveInputEdge( &blk->edge[i] );
        }
    }
    last_line = blk->ins.head.line_num;
    for( ;; ) {
        next_ins = blk->ins.head.next;
        if( next_ins == reinterpret_cast<instruction *>(&blk->ins) )
            break;
        if( next_ins->head.line_num != 0 ) {
            last_line = next_ins->head.line_num;
        }
        FreeIns( back, next_ins );
    }
    /*
     * Move the last line number from the block being deleted to the head
     * of the next block in source order, if that block doesn't already
     * have a line number on it.
     */
    if( blk->next_block != NULL
      && blk->next_block->gen_blk_id == (blk->gen_blk_id + 1) ) {
        /*
         * quick check to see if following block is next one in src order
         */
        next = blk->next_block;
    } else {
        next = NULL;
        for( chk = back.data.HeadBlock; chk != NULL; chk = chk->next_block ) {
            if( (chk != blk)
              && (chk->gen_blk_id > blk->gen_blk_id)
              && (next == NULL
              || next->gen_blk_id > chk->gen_blk_id) ) {
                next = chk;
            }
        }
    }
    if( next != NULL
      && next->ins.head.line_num == 0 ) {
        next->ins.head.line_num = last_line;
    }
    if( back.data.HeadBlock == blk ) {
        back.data.HeadBlock = blk->next_block;
    }
    if( back.data.BlockList == blk ) {
        back.data.BlockList = blk->prev_block;
        if( back.data.BlockList == NULL ) {
            back.data.BlockList = back.data.HeadBlock;
        }
    }
    TellScrapLabel( back, blk->label );
    if( blk->dataflow != NULL ) {
        CGFree( back, blk->dataflow );
    }
    FreeABlock( back, blk );
}


void    RemoveInputEdge( block_edge *edge )
/*****************************************/
{
    block       *dest;
    block_edge  *prev;

    if( (edge->flags & BEF_DEST_IS_BLOCK) == 0 )
        return;
    dest = edge->destination.u.blk;
    dest->inputs--;
    prev = dest->input_edges;
    if( prev == edge ) {
        dest->input_edges = edge->next_source;
    } else {
        while( prev->next_source != edge ) {
            prev = prev->next_source;
        }
        prev->next_source = edge->next_source;
    }
}


void    MoveHead( BackRun& back, block *old, block *new_ )
/*****************************************
 * We're eliminating a loop header, so move it the the new
 * block and point all loop_head pointers to the new block.
 */
{
    block       *blk;

    if( !IsBlkAttr( old, BLK_LOOP_HEADER ) )
        return;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( blk->loop_head == old ) {
            blk->loop_head = new_;
        }
    }
    if( IsBlkAttr( old, BLK_LOOP_HEADER ) )
        MarkBlkAttrSet( new_, BLK_LOOP_HEADER );
    if( IsBlkAttr( old, BLK_ITERATIONS_KNOWN ) )
        MarkBlkAttrSet( new_, BLK_ITERATIONS_KNOWN );
    MarkBlkAttrClr( old, BLK_LOOP_HEADER | BLK_ITERATIONS_KNOWN );
    new_->iterations = old->iterations;
    new_->loop_head = old->loop_head;
    old->loop_head = new_;
}


static  bool    Retarget( BackRun& back, block *blk )
/************************************/
{
    block_edge  *edge;
    block_edge  *next;
    block       *target;
    bool        success;

    success = true;                    /* assume can get rid of block*/
    target = blk->edge[0].destination.u.blk;
    edge = blk->input_edges;
    blk->input_edges = NULL;
    for( ; edge != NULL; edge = next ) {
        next = edge->next_source;
        if( IsBlkAttr( edge->source, BLK_SELECT | BLK_LABEL_RETURN ) ) {
            success = false;   /* let the optimizer do it later on*/
            edge->next_source = blk->input_edges;
            blk->input_edges = edge;
        } else {
            PointEdge( edge, target );
            blk->inputs--;
        }
    }
    if( success ) {
        MoveHead( back, blk, target );
        RemoveBlock( back, blk );
    }
    return( success );
}


static  void    JoinBlocks( BackRun& back, block *jump, block *target )
/******************************************************/
{
    block_edge          *edge;
    source_line_number  line_num;
    label_handle        label;
    instruction         *nop;

    /*
     * To get here, 'target' is only entered from 'jump'
     * Thus, the only input edge to 'target' is from jump, and can be tossed
     *
     * keep the label from jump in case it's referenced in a SELECT block
     */
    label = target->label;
    target->label = jump->label;
    if( IsBlkAttr( jump, BLK_BIG_LABEL ) )
        MarkBlkAttrSet( target, BLK_BIG_LABEL );
    jump->label = label;
    line_num = target->ins.head.line_num;
    target->ins.head.line_num = jump->ins.head.line_num;
    /*
     * Move the inputs to 'jump' to be inputs to 'target'
     */
    target->inputs = jump->inputs;
    edge = jump->input_edges;
    target->input_edges = edge;
    for( ; edge != NULL; edge = edge->next_source ) {
        edge->destination.u.blk = target;    /* was 'jump' before*/
    }
    /*
     * Now join the instruction streams
     */
    nop = jump->ins.head.prev;
    if( nop->head.opcode == OP_NOP ) {
        if( nop->flags.u.nop_flags & NOP_SOURCE_QUEUE ) {
            /*
             * this nop is only here to hold source info so we just
             * attach the source info to the next instruction and
             * nuke this nop so that it can't inhibit optimization
             */
            if( target->ins.head.next->head.line_num == 0 ) {
                target->ins.head.next->head.line_num = nop->head.line_num;
            }
            FreeIns( back, nop );
        }
    }

    if( jump->ins.head.next != reinterpret_cast<instruction *>(&jump->ins) ) {
        if( line_num != 0 ) {
            jump->ins.head.prev->head.line_num = line_num;
        }
        jump->ins.head.prev->head.next = target->ins.head.next;
        target->ins.head.next->head.prev = jump->ins.head.prev;
        target->ins.head.next = jump->ins.head.next;
        target->ins.head.next->head.prev = reinterpret_cast<instruction *>(&target->ins);
        /*
         * so RemoveBlock won't free the instr list
         */
        jump->ins.head.next = reinterpret_cast<instruction *>(&jump->ins);
        jump->ins.head.prev = reinterpret_cast<instruction *>(&jump->ins);
    }

    jump->inputs = 0;
    jump->targets = 0;
    MoveHead( back, jump, target );
    RemoveBlock( back, jump );
}


static  bool    SameTarget( BackRun& back, block *blk )
/**************************************/
{
    instruction *ins;
    block       *targ1, *targ2;

    targ1 = blk->edge[0].destination.u.blk;
    targ2 = blk->edge[1].destination.u.blk;
    if( targ1 != targ2 )
        return( false );
    if( IsBlkAttr( targ1, BLK_UNKNOWN_DESTINATION )
      || IsBlkAttr( targ2, BLK_UNKNOWN_DESTINATION ) ) {
        return( false );
    }
    RemoveEdge( &blk->edge[1] );
    ins = blk->ins.head.prev;
    while( !OpIsCondition( ins->head.opcode ) ) {
        ins = ins->head.prev;
    }
    FreeIns( back, ins );
    return( true );
}


static  bool    DoBlockTrim( BackRun& back )
/*********************************/
{
    block       *blk;
    block       *next;
    block       *target;
    instruction *ins;
    bool        change;
    bool        any_change;
    block_id    blk_id;

    MarkBlkAllUnVisited( back );
    any_change = false;
    do {
        change = false;
        MarkReachableBlocks(back);
        for( blk = back.data.HeadBlock->next_block; blk != NULL; blk = next ) {
            next = blk->next_block;
            if( !IsBlkVisited( blk )
              && !IsBlkAttr( blk, BLK_UNKNOWN_DESTINATION ) ) {
                while( blk->input_edges != NULL ) {
                    RemoveInputEdge( blk->input_edges );
                }
                RemoveBlock( back, blk );
                change = true;
            } else if( IsBlkAttr( blk, BLK_CONDITIONAL ) ) {
                if( SameTarget( back, blk ) ) {
                    MarkBlkAttrClr( blk, BLK_CONDITIONAL );
                    MarkBlkAttrSet( blk, BLK_JUMP );
                    change = true;
                }
            } else if( IsBlkAttr( blk, BLK_JUMP ) ) {
                target = blk->edge[0].destination.u.blk;
                if( target != blk
                  && !IsBlkAttr( target, BLK_UNKNOWN_DESTINATION ) ) {
                    for( ins = blk->ins.head.next; ins->head.opcode == OP_NOP; ins = ins->head.next ) {
                        if( ins->flags.u.nop_flags & (NOP_DBGINFO | NOP_DBGINFO_START) ) {
                            break;
                        }
                    }
                    if( ins->head.opcode == OP_BLOCK ) { /* was an empty block*/
                        if( !IsBlkAttr( blk, BLK_BIG_LABEL ) ) {
                            change |= Retarget( back, blk );
                        }
                    } else if( target->inputs == 1
                      && !IsBlkAttr( target, BLK_BIG_LABEL )
                      && ( CountIns( blk ) + CountIns( target ) ) <= INS_PER_BLOCK ) {
                        if( !IsBlkAttr( blk, BLK_RETURNED_TO | BLK_BIG_LABEL )
                          && !IsBlkAttr( target, BLK_CALL_LABEL ) ) {
                            JoinBlocks( back, blk, target );
                            change = true;
                        }
                    }
                }
            }
        }
        MarkBlkAllUnVisited( back );
        if( change ) {
            back.data.BlocksUnTrimmed = false;
            any_change = true;
        }
    } while( change );
    blk_id = 1;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        blk->blk_id = blk_id++;
    }
    return( any_change );
}

void KillCondBlk( BackRun& back, block *blk, instruction *ins, cond_dst_idx dest_idx )
/**********************************************************************
 * Assume blk is a conditional with compare ins
 * Make dest the destination and delete the unused edge
 * Change blk to a JMP to dest edge
 */
{
    block_edge  *edge;
    block       *dest_blk;

    RemoveInputEdge( &blk->edge[0] );
    RemoveInputEdge( &blk->edge[1] );
    MarkBlkAttrClr( blk, BLK_CONDITIONAL );
    MarkBlkAttrSet( blk, BLK_JUMP );
    blk->targets = 1;
    dest_blk = blk->edge[dest_idx].destination.u.blk;
    edge = &blk->edge[0];
    edge->flags = blk->edge[dest_idx].flags;
    edge->source = blk;
    PointEdge( edge, dest_blk );
    FreeIns( back, ins );
}

bool    DeadBlocks( BackRun& back )
/************************/
{
    block           *blk;
    instruction     *ins;
    cond_dst_idx    dest_idx;
    bool            change;

    change = false;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( IsBlkAttr( blk, BLK_CONDITIONAL ) ) {
            ins = FindOneCond( blk );
            if( ins == NULL )
                continue;
            if( ins->result != NULL )
                continue;
            dest_idx = TrueIndex( ins );
            if( dest_idx != FalseIndex( ins ) )
                continue;
            KillCondBlk( back, blk, ins, dest_idx );
            change = true;
        }
    }
    if( change ) {
        BlockTrim(back);
        return( true );
    }
    return( false );
}


bool    BlockTrim( BackRun& back )
/***********************/
{
    bool    change = false;

    if( ( back.data.CurrProc->state.attr & ROUTINE_WANTS_DEBUGGING ) == 0 ) {
        change = DoBlockTrim(back);
        Renumber(back);
    }
    return( change );
}
