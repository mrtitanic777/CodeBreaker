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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "stackcg.h"
#include "data.h"
#include "makeblk.h"
#include "insutil.h"
#include "flograph.h"
#include "flood.h"
#include "backrun.hpp"


static void             NewInterval( BackRun& back, block *blk, level_depth level );


static  void    Irreducable( BackRun& back )
/*********************************/
{
    block       *blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        _MarkBlkAttrClr( blk, BLK_ITERATIONS_KNOWN | BLK_LOOP_HEADER );
        blk->loop_head = NULL;
        blk->depth = 1;
    }
}


static  void    NoBlocksToSelf( BackRun& back )
/************************************/
{
    block       *blk;
    block       *new_blk;
    block_edge  *edge;
    block_edge  *new_edge;
    block_edge  *curr;
    block_edge  **owner;
    block_edge  *next;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( edge = blk->input_edges; edge != NULL; edge = next ) {
            next = edge->next_source;
            if( edge->source == blk ) {
                new_blk = MakeBlockInit( back, 1 );
                /*
                 * set up new block to look like it was generated after blk
                 */
                _SetBlkAttr( new_blk, BLK_JUMP );
                new_blk->label = AskForNewLabel(back);
                new_blk->edge[0].flags = BEF_BLOCK_LABEL_DIES;
                new_blk->gen_blk_id = blk->gen_blk_id;
                new_blk->ins.head.line_num = blk->ins.head.line_num;
                new_blk->next_block = blk->next_block;
                new_blk->prev_block = blk;
                /*
                 * set new block to jump from new_blk to blk
                 */
                new_blk->targets = 1;
                new_edge = &new_blk->edge[0];
                new_edge->flags |= BEF_DEST_IS_BLOCK;
                new_edge->destination.u.blk = blk;
                new_edge->source = new_blk;

                new_blk->inputs = 1;
                new_blk->input_edges = edge;
                /*
                 * link it after blk
                 */
                blk->next_block = new_blk;
                if( new_blk->next_block != NULL ) {
                    new_blk->next_block->prev_block = new_blk;
                }
                /*
                 * retarget edge to point to new block
                 */
                edge->destination.u.blk = new_blk;
                edge->flags &= ~BEF_DEST_LABEL_DIES;
                /*
                 * replace edge with new_edge in blk's input edge list
                 */
                owner = &blk->input_edges;
                for( ;; ) {
                    curr = *owner;
                    if( curr == edge )
                        break;
                    owner = &(*owner)->next_source;
                }
                *owner = new_edge;
                new_edge->next_source = curr->next_source;
                /*
                 * edge is now the only input to new_blk
                 */
                edge->next_source = NULL;
            }
        }
    }
}


static  void    ReturnsToBottom( BackRun& back )
/**************************************
 * moving return blocks to the bottom
 */
{
    block       *curr;
    block       *prev;
    block       *last;

    for( last = back.data.HeadBlock; last->next_block != NULL; ) {
        last = last->next_block;
    }
    for( curr = last->prev_block; curr != NULL; curr = prev ) {
        prev = curr->prev_block;
        if( _IsBlkAttr( curr, BLK_RETURN ) ) {
            if( prev != NULL ) {
                prev->next_block = curr->next_block;
            }
            curr->next_block->prev_block = prev;
            last->next_block = curr;
            curr->prev_block = last;
            curr->next_block = NULL;
            last = curr;
        }
    }
    back.data.BlockList = last;
}

static  void *MarkVisited( BackRun& back, block *blk )
/*************************************/
{
    block_num   i;

    _MarkBlkVisited( blk );
    for( i = 0; i < blk->targets; ++i ) {
        if( _IsBlkVisited( blk->edge[i].destination.u.blk ) )
            continue;
        SafeRecurseCG( back, (func_sr)MarkVisited, blk->edge[i].destination.u.blk );
    }
    blk->prev_block = back.data.BlockList;
    back.data.BlockList = blk;
    return NULL;
}

static  bool    DepthFirstSearch( BackRun& back )
/**************************************/
{
    block       *blk;
    bool        reducible;

    back.data.BlockList = NULL;
    MarkVisited( back, back.data.HeadBlock );
    reducible = true;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !_IsBlkVisited( blk ) ) {
            blk->prev_block = back.data.BlockList;
            back.data.BlockList = blk;
            reducible = false;
        }
        _MarkBlkUnVisited( blk );
    }
    return( reducible );
}


static  void    RestoreLinks( BackRun& back )
/**********************************/
{
    block       *blk;
    block       *prev;

    prev = NULL;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        blk->prev_block = prev;
        prev = blk;
    }
    back.data.BlockList = prev;
}


static  void    FixLinks( BackRun& back )
/******************************/
{
    block       *blk;
    block       *prev;
    block_id    blk_id;

    prev = NULL;
    blk_id = 1;
    for( blk = back.data.BlockList; blk != NULL; blk = blk->next_block ) {
        blk->next_block = blk->prev_block;
        blk->prev_block = prev;
        blk->blk_id = blk_id++;
        prev = blk;
    }
    back.data.HeadBlock = back.data.BlockList;
    back.data.BlockList = prev;
}


static  interval_def    *IntervalNo( block *blk, level_depth level )
/******************************************************************/
{
    interval_def        *curr;

    curr = blk->u.interval;
    while( level-- > 0 ) {
        curr = curr->parent;
    }
    return( curr );
}


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct FlographState: flograph_state.h) */

}  // namespace


static  bool    FindIntervals( BackRun& back )
/***********************************/
{
    block               *blk;
    block_edge          *edge;
    interval_def        *curr;
    interval_def        *prev_int;
    interval_def        *test;
    level_depth         level;
    int                 num;
    int                 prev_num;
    bool                add;

    num = 0;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        ++num;
        curr = static_cast<interval_def *>(CGAlloc( back, sizeof( interval_def ) ));
        curr->link = back.flograph.Intervals;
        back.flograph.Intervals = curr;
        curr->parent = NULL;
        curr->sub_int = NULL;
        curr->next_sub_int = NULL;
        curr->first_block = blk;
        curr->last_block = blk;
        curr->level = 0;
        blk->depth = 0;
        blk->u.interval = curr;
    }
    level = 1;
    for( ;; ) {
        prev_num = num;
        /*
         * at least one node at new level
         */
        num = 1;
        NewInterval( back, back.data.HeadBlock, level );
        for( blk = back.data.HeadBlock->next_block; blk != NULL; blk = blk->next_block ) {
            curr = IntervalNo( blk, level - 1 );
            if( curr->parent == NULL ) {
                prev_int = NULL;
                edge = blk->input_edges;
                add = false;
                for( ;; ) {
                    test = IntervalNo( edge->source, level - 1 );
                    /*
                     * guess - internal edge
                     */
                    if( test != curr ) {
                        /*
                         * guess - lower level head
                         */
                        test = test->parent;
                        if( test == NULL ) {
                            add = true;
                            break;
                        }
                        /*
                         * guess - no other predecessor
                         */
                        if( prev_int == NULL ) {
                            prev_int = test;
                        } else {
                            /*
                             * guess - different predecessor
                             */
                            if( test != prev_int ) {
                                add = true;
                                break;
                            }
                        }
                    }
                    edge = edge->next_source;
                    if( edge == NULL ) {
                        break;
                    }
                }
                if( !add ) {
                    curr->parent = prev_int;
                    prev_int->last_block = curr->last_block;
                    for( test = prev_int->sub_int; test->next_sub_int != NULL; ) {
                        test = test->next_sub_int;
                    }
                    test->next_sub_int = curr;
                } else {
                    /*
                     * admit - create a new interval
                     */
                    NewInterval( back, blk, level );
                    num ++;
                }
            }
        }
        ++level;
        if( num == prev_num || num == 1 ) {
            break;
        }
    }
    return( num == 1 );
}


static  void    ReorderBlocks( BackRun& back )
/************************************
 * Reorder blocks according to the interval ordering
 * This allows each interval to be identified as a continuous range of blocks
 */
{
    interval_def        *curr;
    block               *last_block;
    block               *next_block;

    last_block = back.data.HeadBlock;
    curr = back.data.HeadBlock->u.interval;
    for( ;; ) {
        for( ;; ) {
            if( curr->next_sub_int != NULL )
                break;
            curr = curr->parent;
            if( curr == NULL )
                break;
            curr->last_block = last_block;
        }
        if( curr == NULL )
            break;
        for( curr = curr->next_sub_int; curr->level > 0; ) {
            curr = curr->sub_int;
        }
        next_block = curr->first_block;
        next_block->prev_block = last_block;
        last_block->next_block = next_block;
        next_block->blk_id = last_block->blk_id + 1;
        last_block = next_block;
    }
    last_block->next_block = NULL;
    back.data.BlockList = last_block;
}


static  void    EdgeLevels( BackRun& back )
/********************************/
{
    block               *blk;
    block_edge          *edge;
    interval_def        *interval;
    block_id            blk_id;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( edge = blk->input_edges; edge != NULL; edge = edge->next_source ) {
            blk_id = edge->source->blk_id;
            interval = blk->u.interval;
            for( ;; ) {
                if( blk_id >= interval->first_block->blk_id && blk_id <= interval->last_block->blk_id )
                    break;
                edge->join_level = interval->level;
                interval = interval->parent;
                if( interval == NULL ) {
                    break;
                }
            }
        }
    }
}


static  void    NewInterval( BackRun& back, block *blk, level_depth level )
/**********************************************************/
{
    interval_def        *prev;
    interval_def        *new_interval;

    prev = IntervalNo( blk, level - 1 );
    new_interval = static_cast<interval_def *>(CGAlloc( back, sizeof( interval_def ) ));
    new_interval->link = back.flograph.Intervals;
    back.flograph.Intervals = new_interval;
    new_interval->sub_int = prev;
    new_interval->next_sub_int = NULL;
    new_interval->level = level;
    new_interval->parent = NULL;
    new_interval->first_block = blk;
    new_interval->last_block = blk;
    prev->parent = new_interval;
}


static  void    NestingDepth( BackRun& back )
/**********************************/
{
    block               *blk;
    level_depth         level;
    interval_def        *interval;
    block_edge          *edge;
    block               *target;
    block_num           i;
    bool                change;

    for( interval = back.data.BlockList->u.interval->parent; interval->parent != NULL; interval = interval->parent ) {
        level = interval->level - 1;
        /*
         * borrow 'next_block'
         * identify all back edges at this level
         */
        for( blk = back.data.BlockList; blk != NULL; blk = blk->prev_block ) {
            blk->next_block = NULL;
            for( i = blk->targets; i-- > 0; ) {
                edge = &blk->edge[i];
                target = edge->destination.u.blk;
                if( target->blk_id <= blk->blk_id ) {
                    /*
                     * back edge
                     */
                    if( edge->join_level == level ) {
                        blk->next_block = target;
                        if( blk->loop_head == NULL ) {
                            blk->loop_head = target;
                        }
                        blk->depth++;
                        break;
                    }
                }
            }
        }
        do {
            change = false;
            for( blk = back.data.BlockList; blk != NULL; blk = blk->prev_block ) {
                if( blk->next_block == NULL ) {
                    for( i = blk->targets; i-- > 0; ) {
                        edge = &blk->edge[i];
                        if( edge->join_level <= level ) {
                            target = edge->destination.u.blk->next_block;
                            if( target != NULL ) {
                                blk->depth++;
                                blk->next_block = target;  /* store head in node */
                                if( blk->loop_head == NULL && blk != target ) {
                                    blk->loop_head = target;
                                }
                                _MarkBlkAttrSet( target, BLK_LOOP_HEADER );
                                change = true;
                                break;
                            }
                        }
                    }
                }
            }
        } while( change );
    }
    /*
     * Restore 'next_block'
     */
    back.data.HeadBlock = NULL;
    for( blk = back.data.BlockList; blk != NULL; blk = blk->prev_block ) {
        blk->next_block = back.data.HeadBlock;
        back.data.HeadBlock = blk;
    }
}

static  void    KillIntervals( BackRun& back )
/***********************************/
{
    interval_def        *next;
    block               *blk;

    for( ; back.flograph.Intervals != NULL; back.flograph.Intervals = next ) {
        next = back.flograph.Intervals->link;
        CGFree( back, back.flograph.Intervals );
    }
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        blk->u.interval = NULL;
    }
}


static  bool    FlowDone( BackRun& back, block *curr, void *parm )
/*************************************************/
{
    if( curr == (block *)parm ) {
        back.flograph.functionDegenerate = true;
        return( false );
    }
    return( true );
}

static  bool    Degenerate( BackRun& back )
/********************************/
{
    block       *blk;
    block_edge  *edge;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !_IsBlkAttr( blk, BLK_LOOP_HEADER ) )
            continue;
        for( edge = blk->input_edges; edge != NULL; edge = edge->next_source ) {
            if( _IsBlkAttr( edge->source, BLK_SELECT ) ) {
                back.flograph.functionDegenerate = false;
                FloodForward( back, edge->source, FlowDone, blk );
                if( back.flograph.functionDegenerate ) {
                    return( true );
                }
            }
        }
    }
    return( false );
}

void    MakeFlowGraph( BackRun& back )
/***************************/
{
    Irreducable(back);
    if( back.data.CurrProc->state.attr & ROUTINE_WANTS_DEBUGGING ) {
        return;
    }
    NoBlocksToSelf(back);
    back.flograph.Intervals = NULL;
    if( back.data.HeadBlock != NULL ) {
        if( !DepthFirstSearch(back) ) {
            Irreducable(back);
            RestoreLinks(back);
            Renumber(back);
            return;
        }
        FixLinks(back);
        if( FindIntervals(back) ) {
            ReorderBlocks(back);
            EdgeLevels(back);
            NestingDepth(back);
            if( Degenerate(back) ) {
                Irreducable(back);
            }
       } else {
            /*
             * irreducable flow graph. repair prev_block links
             */
            Irreducable(back);
        }
        KillIntervals(back);
        ReturnsToBottom(back);
        Renumber(back);
    }
}
