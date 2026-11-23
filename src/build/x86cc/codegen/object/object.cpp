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
* Description:  Generate object code from symbolic instructions.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgauxinf.h"
#include "zoiks.h"
#include "data.h"
#include "objout.h"
#include "objprof.h"
#include "object.h"
#include "encode.h"
#include "edge.h"
#include "redefby.h"
#include "targetin.h"
#include "opttell.h"
#include "makeblk.h"
#include "nullprop.h"
#include "feprotos.h"
#include "backrun.hpp"


/*
 * block flag usage
 *
 * BLK_BLOCK_VISITED is used in the sense of placed
 */

#define NOT_TAKEN       -1
#define DNA             0
#define TAKEN           1

typedef struct {
    block       *first;
    block       *last;
} block_queue;

typedef enum {
    ABORT = 0,          /* Abort this particular path */
    CONTINUE,           /* Continue on as normal      */
    STOP,               /* Stop the entire flood down */
} flood_decision;

typedef struct flood_info {
    bool        post_dominates;
    block       *dominator;
} flood_info;

typedef flood_decision (*flood_down_func)( BackRun&, block *, flood_info * );

typedef int (*bp_heuristic)( BackRun&, block *, instruction * );

static  source_line_number      DumpLineNum( BackRun& back, source_line_number n,
                                             source_line_number last,
                                             bool label_line )
/*******************************************************************/
{
    if( IsModel( back, CGSW_GEN_DBG_NUMBERS ) ) {
        if( n > 0 && n != last ) {
            last = n;
            CodeLineNumber( back, n, label_line );
        }
    }
    return( last );
}


void    GenObject( BackRun& back )
/***********************/
{
    block               *blk;
    block               *next_block;
    instruction         *ins;
    source_line_number  last_line;
    block_num           j;
    block_num           i;
    label_handle        lbl;
    unsigned            align;
    fe_attr             attr;

    { segment_id old_segid = ChangeOP( back, AskCodeSeg(back) );
        InitZeroPage(back);
        last_line = 0;
        attr = FEAttr( *back.front, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ) );
        for( blk = back.data.HeadBlock; blk != NULL; blk = next_block ) {
            next_block = blk->next_block;
            if( blk->label != back.data.CurrProc->label && blk->label != NULL ) {
                last_line = DumpLineNum( back, blk->ins.head.line_num, last_line, true );
                if( _IsBlkAttr( blk, BLK_ITERATIONS_KNOWN ) && blk->iterations >= 10 ) {
                    align = DepthAlign( back, DEEP_LOOP_ALIGN );
                } else {
                    align = DepthAlign( back, blk->depth );
                }
                CodeLabel( back, blk->label, align );
                if( (blk->edge[0].flags & BEF_BLOCK_LABEL_DIES) && back.data.BlocksUnTrimmed ) {
                    TellCondemnedLabel( back, blk->label );
                }
            }
            StartBlockProfiling( back, blk );
            InitStackDepth( back, blk );
            for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
                if( ins->head.opcode == OP_NOP
                  && ( (ins->flags.u.nop_flags & NOP_SOURCE_QUEUE)
                  || ins->flags.u.nop_flags == NOP_DBGINFO )) { // an end block
                    last_line = DumpLineNum(back, ins->head.line_num, last_line, true);
                } else {
                    last_line = DumpLineNum(back, ins->head.line_num, last_line, false);
                }
                if( attr & FE_NAKED ) {
                    /*
                     * don't want to generate anything except calls to pragma's for
                     * naked functions
                     */
                    if( ins->head.opcode == OP_CALL ) {
                        if( FindAuxInfo( back, ins->operands[CALL_OP_ADDR], FEINF_CALL_BYTES ) != NULL ) {
                            GenObjCode( back, ins );
                        }
                    }
                } else {
                    GenObjCode( back, ins );
                }
            }
            EndBlockProfiling();
            if( _IsBlkAttr( blk, BLK_JUMP | BLK_BIG_JUMP ) ) {
                if( back.data.BlockByBlock
                 || next_block == NULL
                 || blk->edge[0].destination.u.lbl != next_block->label ) {
                    /*
                     * watch out for orphan blocks (no inputs/targets)
                     */
                    if( blk->targets > 0 ) {
                        GenJumpLabel( back, blk->edge[0].destination.u.lbl );
                    }
                }
            } else if( _IsBlkAttr( blk, BLK_RETURN ) ) {
                FiniZeroPage(back);
                GenEpilog(back);
            } else if( _IsBlkAttr( blk, BLK_CALL_LABEL ) ) {
                GenCallLabel( back, blk->edge[0].destination.u.blk );
                if( back.data.BlockByBlock ) {
                    if( next_block == NULL ) {
                        GenJumpLabel( back, blk->u2.next->label );
                    } else {
                        GenJumpLabel( back, next_block->label );
                    }
                }
            } else if( _IsBlkAttr( blk, BLK_LABEL_RETURN ) ) {
                GenLabelReturn(back);
            }
            if( !_IsBlkAttr( blk, BLK_LABEL_RETURN ) ) { /* maybe pointer to dead label */
                for( j = blk->targets; j-- > 0; ) {
                    lbl = blk->edge[j].destination.u.lbl;
                    TellReachedLabel( back, lbl );
                    if( (blk->edge[j].flags & BEF_DEST_LABEL_DIES) && back.data.BlocksUnTrimmed ) {
                        TellCondemnedLabel( back, lbl );
                        for( i = j; i-- > 0; ) {
                            if( blk->edge[i].destination.u.lbl == lbl ) {
                                blk->edge[i].flags &= ~BEF_DEST_LABEL_DIES;
                            }
                        }
                    }
                }
            }
            if( !back.data.BlocksUnTrimmed && blk->label != back.data.CurrProc->label && blk->label != NULL ) {
                TellCondemnedLabel( back, blk->label );
            }
            back.data.CurrBlock = blk;
            FreeBlock(back);
        }
        back.data.HeadBlock = blk;
        back.data.BlockList = blk;
    SetOP( back, old_segid ); }
}


static  bool    GenId( BackRun& back, block *blk, block *next )
/**********************************************/
{
    return( blk->gen_blk_id > next->gen_blk_id );
}


static  void    BlocksSortedBy( BackRun& back, bool (*bigger)( BackRun&, block *, block * ) )
/******************************************************************/
{
    block       *blk;
    block       *next;
    bool        change;

    for( change = ( back.data.HeadBlock->next_block != NULL ); change; ) {
        change = false;
        for( blk = back.data.HeadBlock->next_block; (next = blk->next_block) != NULL; blk = next ) {
            if( bigger( back, blk, next ) ) {
                blk->prev_block->next_block = next;
                if( next->next_block != NULL ) {
                    next->next_block->prev_block = blk;
                }
                blk->next_block = next->next_block;
                if( blk->prev_block == back.data.HeadBlock ) {
                    back.data.HeadBlock->next_block = next;
                }
                next->prev_block = blk->prev_block;
                blk->prev_block = next;
                next->next_block = blk;
                if( back.data.BlockList == next ) {
                    back.data.BlockList = blk;
                }
                change = true;
            }
        }
    }
}


static  void    BQInit( block_queue *q )
/**************************************/
{
    q->first = NULL;
    q->last = NULL;
}

static  bool    BQEmpty( block_queue *q )
/***************************************/
{
    return( q->first == NULL );
}

static  void    BQAdd( block_queue *q, block *element )
/*****************************************************/
{
    if( BQEmpty( q ) ) {
        element->next_block = NULL;
        element->prev_block = NULL;
        q->first = element;
        q->last = element;
    } else {
        element->prev_block = q->last;
        element->next_block = NULL;
        q->last->next_block = element;
        q->last = element;
    }
}

static block    *BQFirst( block_queue *q )
/****************************************/
{
    return( q->first );
}

static block    *BQNext( block_queue *q, block *curr )
/****************************************************/
{
    /* unused parameters */ (void)q;

    return( curr->next_block );
}

static  block   *BQRemove( block_queue *q, block *blk )
/*****************************************************/
{
    if( blk == NULL )
        blk = q->first;
    if( blk->prev_block != NULL )
        blk->prev_block->next_block = blk->next_block;
    if( blk->next_block != NULL )
        blk->next_block->prev_block = blk->prev_block;
    if( q->first == blk )
        q->first = blk->next_block;
    if( q->last == blk )
        q->last = blk->prev_block;
    return( blk );
}

static  block_edge *FindLoopBackEdge( block *blk )
/************************************************/
{
    block_edge          *edge;
    block_num           i;

    for( i = 0; i < blk->targets; i++ ) {
        edge = &blk->edge[i];
        if( edge->destination.u.blk == blk->loop_head )
            return( edge );
        if( edge->destination.u.blk == blk ) {
            return( edge );
        }
    }
    return( NULL );
}

static  instruction     *FindCondition( block *blk )
/**************************************************/
{
    instruction         *cond;

    for( cond = blk->ins.head.prev; cond->head.opcode != OP_BLOCK; cond = cond->head.prev ) {
        if( _OpIsCondition( cond->head.opcode ) ) {
            return( cond );
        }
    }
    /* _Zoiks( ZOIKS_XXX ); */
    return( NULL );
}

static  int     PointerHeuristic( BackRun& back, block *blk, instruction *cond )
/***************************************************************/
{
    int                 prediction;

    /* unused parameters */ (void)blk;

    prediction = DNA;
    if( _IsPointer( cond->type_class ) ) {
        switch( cond->head.opcode ) {
        case OP_CMP_EQUAL:
            prediction = NOT_TAKEN;
            break;
        case OP_CMP_NOT_EQUAL:
            prediction = TAKEN;
            break;
        }
    }
    return( prediction );
}

static  int     OpcodeHeuristic( BackRun& back, block *blk, instruction *cond )
/**************************************************************/
{
    int                 prediction;
//    name                *op1;
    name                *op2;

    /* unused parameters */ (void)blk;

    prediction = DNA;
//    op1 = cond->operands[0];
    op2 = cond->operands[1];
    switch( cond->head.opcode ) {
    case OP_CMP_NOT_EQUAL:
        if( _IsFloating( cond->type_class ) ) {
            prediction = TAKEN;
        }
        break;
    case OP_CMP_EQUAL:
        if( _IsFloating( cond->type_class ) ) {
            prediction = NOT_TAKEN;
        }
        break;
    case OP_CMP_LESS:
    case OP_CMP_LESS_EQUAL:
        if( op2->n.class_ == N_CONSTANT ) {
            if( op2->c.const_type == CONS_ABSOLUTE ) {
                if( op2->c.lo.u.int_value == 0 ) {
                    prediction = NOT_TAKEN;
                }
            }
        }
        break;
    case OP_CMP_GREATER:
    case OP_CMP_GREATER_EQUAL:
        if( op2->n.class_ == N_CONSTANT ) {
            if( op2->c.const_type == CONS_ABSOLUTE ) {
                if( op2->c.lo.u.int_value == 0 ) {
                    prediction = TAKEN;
                }
            }
        }
        break;
    default:
        prediction = DNA;
    }
    return( prediction );
}

static  int     Want( instruction *cond, int index )
/*****************************************************
 * We know which block we want, or rather its index in the
 * list of edges of the basic block, so this tells us
 * if the conditional will be TAKEN or NOT_TAKEN to get
 * to that block.
 */
{
    int         prediction;

    prediction = TAKEN;
    if( _TrueIndex( cond ) == 0 ) {
        if( index == 1 ) {
            prediction = NOT_TAKEN;
        }
    } else {
        if( index == 0 ) {
            prediction = NOT_TAKEN;
        }
    }
    return( prediction );
}

static  bool    BlockContainsCall( block *blk )
/*********************************************/
{
    instruction         *ins;

    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( _OpIsCall( ins->head.opcode ) ) {
            return( true );
        }
    }
    return( false );
}

static  void    PushTargets( BackRun& back, void *stack, block *blk )
/****************************************************/
{
    block_num   i;

    for( i = 0; i < blk->targets; i++ ) {
        EdgeStackPush( back, static_cast<edge_stack *>(stack), &blk->edge[i] );
    }
}

static  void    FloodDown( BackRun& back, block *from, flood_down_func func, void *parm )
/************************************************************************/
{
    void                *stack;
    block               *dest;
    flood_decision      decision;
    block_edge          *edge;

    MarkBlkAllAttrClr( back, BLK_FLOODED );
    stack = EdgeStackInit(back);
    PushTargets( back, stack, from );
    _MarkBlkAttrSet( from, BLK_FLOODED );
    while( !EdgeStackEmpty( static_cast<edge_stack *>(stack) ) ) {
        edge = EdgeStackPop( back, static_cast<edge_stack *>(stack) );
        dest = edge->destination.u.blk;
        if( _IsBlkAttr( dest, BLK_FLOODED ) )
            continue;
        decision = func( back, dest, static_cast<flood_info *>(parm) );
        if( decision == ABORT )
            continue;
        if( decision == STOP )
            break;
        _MarkBlkAttrSet( dest, BLK_FLOODED );
        PushTargets( back, stack, dest );
    }
    EdgeStackFini( back, static_cast<edge_stack *>(stack) );
}

static  flood_decision PDFloodFunc( BackRun& back, block *blk, flood_info *info )
/****************************************************************/
{
    if( blk == info->dominator )
        return( ABORT );
    if( _IsBlkAttr( blk, BLK_RETURN ) ) {
        info->post_dominates = false;
        return( STOP );
    }
    return( CONTINUE );
}

static  bool    PostDominates( BackRun& back, block *dominator, block *blk )
/**************************************************************
 * Return true if dominator post-dominates blk.
 * To determine this, we just flood down aborting whenever we hit
 * previously encountered blocks or the dominator. If we hit a
 * RETURN block, we return false.
 */
{
    flood_info          info;

    info.post_dominates = true;
    info.dominator = dominator;
    FloodDown( back, blk, PDFloodFunc, (void *)&info );
    return( info.post_dominates );
}

static  bool    CallApplies( BackRun& back, block *src, block *dst )
/***************************************************/
{
    src = dst;
    if( BlockContainsCall( dst ) ) {
        if( !PostDominates( back, dst, src ) ) {
            return( true );
        }
    }
    return( false );
}

static  int     CallHeuristic( BackRun& back, block *blk, instruction *cond )
/************************************************************/
{
    int         prediction;

    prediction = DNA;
    if( CallApplies( back, blk, blk->edge[0].destination.u.blk ) ) {
        if( !CallApplies( back, blk, blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 1 );
        }
    } else {
        if( CallApplies( back, blk, blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 0 );
        }
    }
    return( prediction );
}

static  bool    LoopApplies( block *blk )
/***************************************/
{
    if( _IsBlkAttr( blk, BLK_LOOP_HEADER ) )
        return( true );
    if( _IsBlkAttr( blk, BLK_JUMP ) ) {
        if( _IsBlkAttr( blk->edge[0].destination.u.blk, BLK_LOOP_HEADER ) ) {
            return( true );
        }
    }
    return( false );
}

static  int     LoopHeuristic( BackRun& back, block *blk, instruction *cond )
/************************************************************/
{
    int         prediction;

    prediction = DNA;
    if( LoopApplies( blk->edge[0].destination.u.blk ) ) {
        if( !LoopApplies( blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 0 );
        }
    } else {
        if( LoopApplies( blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 1 );
        }
    }
    return( prediction );
}

static  bool    GuardApplies( BackRun& back, block *blk, block *dst, name *reg )
/***************************************************************/
{
    instruction *ins;
    opcnt       i;

    for( ins = dst->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        for( i = 0; i < ins->num_operands; i++ ) {
            /*
             * this could be beefed up to take into account aggragates
             */
            if( ins->operands[i] == reg ) {
                return( !PostDominates( back, dst, blk ) );
            }
            if( IsReDefinedBy( back, ins, reg ) ) {
                return( false );
            }
        }
    }
    return( false );
}

static  int     TryGuard( BackRun& back, block *blk, instruction *cond, name *reg )
/******************************************************************/
{
    int         prediction;

    prediction = DNA;
    if( GuardApplies( back, blk, blk->edge[0].destination.u.blk, reg ) ) {
        if( !GuardApplies( back, blk, blk->edge[1].destination.u.blk, reg ) ) {
            prediction = Want( cond, 0 );
        }
    } else {
        if( GuardApplies( back, blk, blk->edge[1].destination.u.blk, reg ) ) {
            prediction = Want( cond, 1 );
        }
    }
    return( prediction );
}

static  int     GuardHeuristic( BackRun& back, block *blk, instruction *cond )
/*************************************************************/
{
    name        *op1;
    name        *op2;
    int         prediction;

    op1 = cond->operands[0];
    op2 = cond->operands[1];
    prediction = DNA;
    if( op1->n.class_ == N_REGISTER ) {
        prediction = TryGuard( back, blk, cond, op1 );
    }
    if( prediction == DNA ) {
        if( op2->n.class_ == N_REGISTER ) {
            prediction = TryGuard( back, blk, cond, op2 );
        }
    }
    return( DNA );
}

static  bool    StoreApplies( BackRun& back, block *blk, block *next )
/*****************************************************/
{
    instruction *ins;

    if( PostDominates( back, next, blk ) )
        return( false );
    for( ins = next->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( ins->result == NULL )
            continue;
        switch( ins->result->n.class_ ) {
        case N_MEMORY:
        case N_INDEXED:
//        case N_TEMP:
            return( true );
        }
    }
    return( false );
}

static  int     StoreHeuristic( BackRun& back, block *blk, instruction *cond )
/*************************************************************/
{
    int         prediction;

    prediction = DNA;
    if( StoreApplies( back, blk, blk->edge[0].destination.u.blk ) ) {
        if( !StoreApplies( back, blk, blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 1 );
        }
    } else {
        if( StoreApplies( back, blk, blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 0 );
        }
    }
    return( prediction );
}

static  bool    ReturnApplies( block *blk )
/*****************************************/
{
    if( _IsBlkAttr( blk, BLK_RETURN ) )
        return( true );
    if( _IsBlkAttr( blk, BLK_JUMP ) ) {
        if( _IsBlkAttr( blk->edge[0].destination.u.blk, BLK_RETURN ) ) {
            return( true );
        }
    }
    return( false );
}

static  int     ReturnHeuristic( BackRun& back, block *blk, instruction *cond )
/**************************************************************/
{
    int         prediction;

    prediction = DNA;
    if( ReturnApplies( blk->edge[0].destination.u.blk ) ) {
        if( !ReturnApplies( blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 1 );
        }
    } else {
        if( ReturnApplies( blk->edge[1].destination.u.blk ) ) {
            prediction = Want( cond, 0 );
        }
    }
    return( prediction );
}

static bp_heuristic const Heuristics[] = {
    PointerHeuristic,
    CallHeuristic,
    OpcodeHeuristic,
    ReturnHeuristic,
    StoreHeuristic,
    LoopHeuristic,
    GuardHeuristic,
};

static  block   *Predictor( BackRun& back, block *blk )
/*****************************************
 * Given a conditional block, return the most likely
 * successor of this block.
 */
{
    block_edge          *edge;
    bp_heuristic        ptr;
    int                 i;
    int                 prediction;
    instruction         *cond;

    edge = FindLoopBackEdge( blk );
    if( edge == NULL ) {
        cond = FindCondition( blk );
        for( i = 0; i < sizeof(Heuristics) / sizeof(Heuristics[0]); i++ ) {
            ptr = Heuristics[i];
            prediction = ptr( back, blk, cond );
            switch( prediction ) {
            case TAKEN:
                return( blk->edge[_TrueIndex( cond )].destination.u.blk );
            case NOT_TAKEN:
                return( blk->edge[_FalseIndex( cond )].destination.u.blk );
            }
        }
    }
    /*
     * what the hell, pick one at random
     */
    return( blk->edge[0].destination.u.blk );
}

static  block   *BestFollower( BackRun& back, block_queue *unplaced, block *blk )
/****************************************************************/
{
    block       *best;
    block       *curr;
    block_num   i;

    best = NULL;
    switch( blk->class_ & (BLK_RETURN | BLK_JUMP | BLK_CONDITIONAL | BLK_SELECT | BLK_CALL_LABEL) ) {
    case BLK_RETURN:
    case BLK_JUMP:
    case BLK_SELECT:
        for( i = 0; i < blk->targets; i++ ) {
            best = blk->edge[i].destination.u.blk;
            if( !_IsBlkVisited( best ) ) {
                return( best );
            }
        }
        best = NULL;
        break;
    case BLK_CONDITIONAL:
        /*
         * If exactly one of the followers has already been placed,
         * then the other one is obviously the best candidate. Otherwise,
         * we run branch prediction if neither is placed, or return NULL
         * if both have been placed.
         */
        #define _Munge( a, b )  ( ( (a) << 8 ) + (b) )
        switch( _Munge( _IsBlkVisited( blk->edge[0].destination.u.blk ), _IsBlkVisited( blk->edge[1].destination.u.blk ) ) ) {
        case _Munge( 0, 0 ):
            /*
             * get some branch prediction going here
             */
            best = Predictor( back, blk );
            break;
        case _Munge( 1, 0 ):
            best = blk->edge[1].destination.u.blk;
            break;
        case _Munge( 0, 1 ):
            best = blk->edge[0].destination.u.blk;
            break;
        case _Munge( 1, 1 ):
            best = NULL;
            break;
        }
        #undef _Munge
        break;
    case BLK_CALL_LABEL:
        for( curr = BQFirst( unplaced ); curr != NULL; curr = BQNext( unplaced, curr ) ) {
            if( curr->gen_blk_id == ( blk->gen_blk_id + 1 ) ) {
                best = curr;
                break;
            }
        }
        assert( best != NULL );
        assert( _IsBlkAttr( best, BLK_RETURNED_TO ) );
        break;
    }
    return( best );
}

void    SortBlocks( BackRun& back )
/************************/
{
    block_queue unplaced;
    block_queue placed;
    block       *curr;
    block       *next;
//    block       *ret_block;

    MarkBlkAllUnVisited( back );
    BlocksSortedBy( back, GenId );
    if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION ) )
        return;
    if( IsntModel( back, CGSW_GEN_BRANCH_PREDICTION ) )
        return;
    if( back.data.OptForSize > 50 )
        return;
    /*
     * we can't screw about with the placement of the return
     * block when we are outputting records which mark the start
     * of the epilog etc...
     */
    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) )
        return;
    BQInit( &unplaced );
    BQInit( &placed );
    for( curr = back.data.HeadBlock; curr != NULL; curr = next ) {
        next = curr->next_block;
        BQAdd( &unplaced, curr );
        if( _IsBlkAttr( curr, BLK_RETURNED_TO ) ) {
            /*
             * blocks which are returned to by a call_label routine
             * should not be placed because they are special cased in
             * BestFollower to come out directly after the CALL_LABEL
             * block
             */
            _MarkBlkVisited( curr );
        }
//        ret_block = curr;
    }
    while( !BQEmpty( &unplaced ) ) {
        curr = BQRemove( &unplaced, NULL );
        if( _IsBlkVisited( curr ) )
            continue;
        for( ;; ) {
            BQAdd( &placed, curr );
            _MarkBlkVisited( curr );
            curr = BestFollower( back, &unplaced, curr );
            if( curr == NULL )
                break;
            BQRemove( &unplaced, curr );
        }
    }
    back.data.HeadBlock = placed.first;
    MarkBlkAllUnVisited( back );
}
