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
* Description:  Loop optimizations.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "indvars.h"
#include "cgmem.h"
#include "stackcg.h"
#include "zoiks.h"
#include "i64.h"
#include "data.h"
#include "fpu.h"
#include "makeins.h"
#include "utils.h"
#include "namelist.h"
#include "propind.h"
#include "blips.h"
#include "edge.h"
#include "redefby.h"
#include "makeblk.h"
#include "loopopts.h"
#include "split.h"
#include "insutil.h"
#include "insdead.h"
#include "blktrim.h"
#include "unroll.h"
#include "optimize.h"
#include "tree.h"
#include "treefold.h"
#include "splitvar.h"
#include "cachecon.h"
#include "feprotos.h"
#include "revcond.h"
#include "backrun.hpp"


/*
 * block flag usage
 *
 * BLK_BLOCK_MARKED is used in the sense of will execute
 *
 *
 * target specific. Can a pointer get anywhere near the boundary
 * pointer values? For 8086, yes. 0000 and FFFF are quite possible
 * as pointer values
 */


typedef enum {
    CMP_TRUE,
    CMP_FALSE,
    CMP_UNKNOWN
} value;

typedef struct block_list {
    block                   *blk;
    struct block_list       *next;
} block_list;


namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct LoopoptsFileState {
    int NumIndVars;
    bool LoopProtected;
    bool MemChangedInLoop;
};
namespace {

}  // namespace



static void     InitIndVars( BackRun& back )
/**********************************
 * Initialize for induction variable processing
 */
{
    back.loopopts.IndVarList = NULL;  /* initialize */
}

static level_depth      MaxDepth( BackRun& back )
/***************************************
 * return the depth of the deepest nested loop in the procedure
 */
{
    level_depth     depth;
    block           *blk;

    depth = 0;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( depth < blk->depth ) {
            depth = blk->depth;
        }
    }
    return( depth );
}


static void     ReplaceAllOccurences( BackRun& back, name *of, name *with )
/***********************************************************
 * Replace all occurences of "of" with "with" in the program.
 */
{
    block       *blk;
    instruction *ins;
    opcnt       i;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
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


static bool     InLoop( BackRun& back, block *blk )
/***********************************
 * return true if blk is in the loop defined by Head.
 */
{
    if( blk == back.loopopts.Head )
        return( true );
    for( ; blk != NULL; blk = blk->loop_head ) {
        if( blk->loop_head == back.loopopts.Head ) {
            return( true );
        }
    }
    return( false );
}


block    *AddPreBlock( BackRun& back, block *postblk )
/**************************************
 * There is no preheader for this loop (loop has no initialization) so
 * add a block right in front of the loop header and move any branch
 * that goes from outside the loop to the header, go through the
 * preheader first.
 */
{
    block_edge  *edge;
    block       *preblk;

    preblk = MakeBlockInit( back, 1 );
    /*
     * set up new block to look like it was generated after postblk
     */
    SetBlkAttr( preblk, BLK_JUMP );
    preblk->label = postblk->label;
    preblk->gen_blk_id = postblk->gen_blk_id;
    preblk->ins.head.line_num = postblk->ins.head.line_num;
    preblk->loop_head = postblk->loop_head; /**/
    preblk->depth = postblk->depth;
    if( IsBlkAttr( postblk, BLK_LOOP_HEADER ) ) {
        /*
         * we don't always add this block before a loop header
         * for instance, in the floating point scheduling stuff
         * we use this routine to add a safe 'decache' block
         * after a loop
         */
        preblk->depth -= 1;
    }
    preblk->next_block = postblk;
    preblk->prev_block = postblk->prev_block;
    postblk->prev_block = preblk;
    if( preblk->prev_block != NULL ) {
        preblk->prev_block->next_block = preblk;
    }
    /*
     * make preblk go to postblk
     */
    preblk->targets = 1;
    edge = &preblk->edge[0];
    edge->source = preblk;
    edge->flags = BEF_SOURCE_IS_PREHEADER | BEF_DEST_IS_BLOCK;
    PointEdge( edge, postblk );
    postblk->ins.head.line_num = 0;
    postblk->label = AskForNewLabel(back);
    FixBlockIds(back);
    return( preblk );
}


static bool     IsPreHeader( BackRun& back, block *test )
/*****************************************
 * return true if block "test" will serve as a preheader for "Loop".  A
 * preheader must branch directly to the loop head, and no other block
 * that is not in the loop may branch into the loop.
 */
{
    block_num   i;
    block       *other;

    /*
     * check that test only goes to the loop head
     */
    if( test->targets != 1 )
        return( false );
    if( test->edge[0].destination.u.blk != back.loopopts.Head )
        return( false );
    if( IsBlkAttr( test, BLK_IN_LOOP ) )
        return( false );
    /*
     * check that no other block outside the loop branches into the loop
     */
    for( other = back.data.HeadBlock; other != NULL; other = other->next_block ) {
        if( other != test
          && !IsBlkAttr( other, BLK_IN_LOOP ) ) {
            for( i = other->targets; i-- > 0; ) {
                if( IsBlkAttr( other->edge[i].destination.u.blk, BLK_IN_LOOP ) ) {
                    return( false );
                }
            }
        }
    }
    test->edge[0].flags |= BEF_SOURCE_IS_PREHEADER;
    return( true );
}


static block    *FindPreHeader( BackRun& back )
/*************************************
 * See if there is a basic block that will suffice as a pre-header for
 * "Loop".
 */
{
    block       *preheader;
    block_edge  *edge;

    for( edge = back.loopopts.Head->input_edges; edge != NULL; edge = edge->next_source ) {
        if( edge->flags & BEF_SOURCE_IS_PREHEADER ) {
            return( edge->source );
        }
    }
    /*
     * maybe there is a 'user defined' preheader
     */
    for( preheader = back.data.HeadBlock; preheader != NULL; preheader = preheader->next_block ) {
        if( IsPreHeader( back, preheader ) ) {
            return( preheader );
        }
    }
    return( NULL );
}


static void     PreHeader( BackRun& back )
/********************************
 * Make sure that "Loop" has a preheader "PreHead"
 */
{
    block_edge  *edge;
    block_edge  *next;

    back.loopopts.PreHead = FindPreHeader(back);
    if( back.loopopts.PreHead == NULL ) {
        back.loopopts.PreHead = AddPreBlock( back, back.loopopts.Head );
        for( edge = back.loopopts.Head->input_edges; edge != NULL; edge = next ) {
            next = edge->next_source;
            if( edge->source != back.loopopts.PreHead
              && !IsBlkAttr( edge->source, BLK_IN_LOOP ) ) {
                MoveEdge( edge, back.loopopts.PreHead );
            }
        }
    }
}


void     MarkLoop( BackRun& back )
/*******************************
 * Mark the current loop (defined by Head) as IN_LOOP.  Also mark any
 * blocks in the loop containing a branch out of the loop as LOOP_EXIT.
 */
{
    block       *other_blk;
    block_num   i;
    block_edge  *edge;

    back.loopopts.Loop = NULL;
    for( other_blk = back.data.HeadBlock; other_blk != NULL; other_blk = other_blk->next_block ) {
        if( InLoop( back, other_blk ) ) {
            MarkBlkAttrSet( other_blk, BLK_IN_LOOP );
            other_blk->u.loop = back.loopopts.Loop;
            back.loopopts.Loop = other_blk;
        }
    }
    for( other_blk = back.loopopts.Loop; other_blk != NULL; other_blk = other_blk->u.loop ) {
        edge = &other_blk->edge[0];
        for( i = other_blk->targets; i > 0; --i ) {
            if( !IsBlkAttr( edge->destination.u.blk, BLK_IN_LOOP ) ) {
                MarkBlkAttrSet( other_blk, BLK_LOOP_EXIT );
            }
            ++edge;
        }
    }
    PreHeader(back);
}


void     UnMarkLoop( BackRun& back )
/**************************
 * Turn off the loop marking bits for the current loop.
 */
{
    block       *blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        MarkBlkAttrClr( blk, BLK_IN_LOOP | BLK_LOOP_EXIT );
        blk->u.loop = NULL;
    }
}

block    *NextInLoop( BackRun& back, block *blk )
/********************************/
{
    return( blk->u.loop );
}


block    *NextInProg( block *blk )
/********************************/
{
    return( blk->next_block );
}


void     MakeJumpBlock( block *cond_blk, block_edge *exit_edge )
/***************************************************************
 * Turn the loop condition exit block into one that just transfers out
 * of the loop.
 */
{
    block_edge  *edge;

    RemoveInputEdge( &cond_blk->edge[0] );
    RemoveInputEdge( &cond_blk->edge[1] );
    MarkBlkAttrClr( cond_blk, BLK_CONDITIONAL );
    MarkBlkAttrSet( cond_blk, BLK_JUMP );
    cond_blk->targets = 1;
    edge = &cond_blk->edge[0];
    edge->flags = exit_edge->flags;
    edge->source = cond_blk;
    PointEdge( edge, exit_edge->destination.u.blk );
}


static bool     KillOneTrippers( BackRun& back )
/**************************************
 * Nuke the loops that are only going to go around one time.
 */
{
    block       *blk;
    block       *curr;
    block       *loop_head;
    instruction *ins;
    instruction *next;
    block_num   i;
    bool        change;

    change = false;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( i = blk->targets; i-- > 0; ) {
            if( blk->edge[i].flags & BEF_ONE_ITER_EXIT ) {
                for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
                    next = ins->head.next;
                    if( OpIsCondition( ins->head.opcode )
                      && ins->result == NULL ) {
                        FreeIns( back, ins );
                    }
                }
                if( IsBlkAttr( blk, BLK_LOOP_HEADER ) ) {
                    MarkBlkAttrClr( blk, BLK_LOOP_HEADER );
                    loop_head = blk;
                } else {
                    MarkBlkAttrClr( blk->loop_head, BLK_LOOP_HEADER );
                    loop_head = blk->loop_head;
                }
                for( curr = back.data.HeadBlock; curr != NULL; curr = curr->next_block ) {
                    if( curr->loop_head == loop_head ) {
                        curr->loop_head = loop_head->loop_head;
                    }
                }
                MakeJumpBlock( blk, &blk->edge[i] );
                change = true;
                break;
            }
        }
    }
    return( change );
}


void     UnMarkInvariants( BackRun& back )
/********************************
 * Turn off the bits which indicate that a name is loop invariant
 */
{
    name        *op;

    for( op = back.data.Names[N_TEMP]; op != NULL; op = op->n.next_name ) {
        SetLoopUsage( op, 0 );
    }
    for( op = back.data.Names[N_MEMORY]; op != NULL; op = op->n.next_name ) {
        SetLoopUsage( op, 0 );
    }
}


static void     ZapRegister( BackRun& back, hw_reg_set regs )
/*********************************************
 * Flip bits required given that N_REGISTER "regs" has been changed
 */
{
    name        *other;

    for( other = back.data.Names[N_REGISTER]; other != NULL; other=other->n.next_name ) {
        if( HwOvlap( other->r.reg, regs ) ) {
            other->r.reg_index++;
        }
    }
}


static void     ZapTemp( name *op )
/**********************************
 * Flip bits required given that N_TEMP "op" has been changed
 */
{
    name        *alias;

    alias = op;
    for( ;; ) {
        if( TempsOverlap( alias, op ) ) {
            if( ChkLoopUsage( alias, VU_INVARIANT ) ) {
                SetLoopUsage( alias, VU_VARIANT | VU_VARIED_ONCE );
            } else {
                SetLoopUsage( alias, VU_VARIANT );
            }
            /*
             * if temp is defined once within multiple loops, we want to make
             * sure MULT_DEFINITION gets set (so that we don't assume ONE_DEFINITION
             * applies across the entire function)
             */
            if( alias->t.temp_flags & LOOP_DEFINITION ) {
                alias->t.temp_flags |= MULT_DEFINITION;
            }
            alias->t.temp_flags |= LOOP_DEFINITION;
        }
        alias = alias->t.alias;
        if( alias == op ) {
            break;
        }
    }
}


static void     ZapMemory( BackRun& back, name *op )
/************************************
 * Flip bits required given that N_MEMORY "op" has been changed
 */
{
    name        *other;

    for( other = back.data.Names[N_MEMORY]; other != NULL; other=other->n.next_name ) {
        if( other->v.symbol == op->v.symbol ) {
            if( ChkLoopUsage( other, VU_INVARIANT ) ) {
                SetLoopUsage( other, VU_VARIANT | VU_VARIED_ONCE );
            } else {
                SetLoopUsage( other, VU_VARIANT );
            }
        }
    }
}


void     MarkInvariants( BackRun& back )
/******************************
 * Mark all N_TEMP/N_MEMORY names as INVARIANT with respect to the
 * current loop "Loop", then traverse the loop and turn off the
 * INVARIANT bit for anything that could be modified by an instruction
 * withing the loop.  We borrow the block_usage field for the marking
 * procedure since the block_usage field is only used during the live
 * information calculation.  We also distinguish names which are varied
 * once (VARIED_ONCE) from names which are varied more than once within
 * the loop.
 */
{
    name        *op;
    block       *blk;
    instruction *ins;
    bool        free_index;
    bool        have_call;


    for( op = back.data.Names[N_TEMP]; op != NULL; op = op->n.next_name ) {
        if( op->v.usage & VAR_VOLATILE ) {
            SetLoopUsage( op, 0 );
        } else {
            SetLoopUsage( op, VU_INVARIANT );
        }
    }
    for( op = back.data.Names[N_MEMORY]; op != NULL; op = op->n.next_name ) {
        if( op->v.usage & VAR_VOLATILE ) {
            SetLoopUsage( op, 0 );
        } else {
            SetLoopUsage( op, VU_INVARIANT );
        }
    }
    for( op = back.data.Names[N_REGISTER]; op != NULL; op = op->n.next_name ) {
        op->r.reg_index = 0;
    }
    have_call = false;
    free_index = false;
    back.loopopts_file->MemChangedInLoop = false;
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( OpIsCall( ins->head.opcode )
              && (ins->flags.u.call_flags & CALL_WRITES_NO_MEMORY) == 0 ) {
                back.loopopts_file->MemChangedInLoop = true;
                have_call = true;
            }
            ZapRegister( back, ins->zap->reg );
            if( ins->result != NULL ) {
                op = ins->result;
                switch( op->n.class_ ) {
                case N_REGISTER:
                    ZapRegister( back, ins->result->r.reg );
                    break;
                case N_INDEXED:
                    if( op->i.base == NULL ) {
                        if( IsntModel( back, CGSW_GEN_FORTRAN_ALIASING ) ) {
                            free_index = true;
                        }
                    } else if( op->i.base->n.class_ == N_TEMP ) {
                        ZapTemp( op->i.base );
                    } else if( !free_index ) {
                        ZapMemory( back, op->i.base );
                    }
                    back.loopopts_file->MemChangedInLoop = true;
                    break;
                case N_TEMP:
                    if( op->v.usage & USE_ADDRESS ) {
                        back.loopopts_file->MemChangedInLoop = true;
                    }
                    ZapTemp( op );
                    break;
                case N_MEMORY:
                    back.loopopts_file->MemChangedInLoop = true;
                    if( !free_index ) {
                        ZapMemory( back, op );
                    }
                    break;
                }
            }
        }
    }
    /*
     * Now check to see if the destination of an instruction is live on
     * entry to the loop before we consider hoisting it in FindRegInvar...
     */
//    ZapRegister( Head->ins.head.next->head.live.regs );
    if( have_call
      || free_index ) {
        if( IsntModel( back, CGSW_GEN_FORTRAN_ALIASING ) ) {
            for( op = back.data.Names[N_TEMP]; op != NULL; op = op->n.next_name ) {
                if( op->v.usage & USE_ADDRESS ) {
                    SetLoopUsage( op, VU_VARIANT );
                }
            }
            for( op = back.data.Names[N_MEMORY]; op != NULL; op = op->n.next_name ) {
                if( !NameIsConstant( back, op ) ) {
                    SetLoopUsage( op, VU_VARIANT );
                }
            }
        } else { /* could only be have_call */
            for( op = back.data.Names[N_MEMORY]; op != NULL; op = op->n.next_name ) {
                if( NameIsConstant( back, op ) )
                    continue;
                switch( op->m.memory_type ) {
                case CG_FE:
                    if( FEAttr( *back.front, static_cast<cg_sym_handle>(op->v.symbol) ) & FE_VISIBLE ) {
                        SetLoopUsage( op, VU_VARIANT );
                    }
                    break;
                case CG_BACK:
                case CG_VTB:
                case CG_TBL:
                    break;
                default:
                    SetLoopUsage( op, VU_VARIANT );
                    break;
                }
            }
        }
    }
}


static bool     InvariantReg( name *op )
/**************************************/
{
    if( op->n.class_ != N_REGISTER )
        return( false );
    if( op->r.reg_index != 0 )
        return( false );
    return( true );
}


bool     InvariantOp( BackRun& back, name *op )
/*******************************
 * return true if "op" is invariant with respect to "Loop"
 */
{
    switch( op->n.class_ ) {
    case N_CONSTANT:
        return( true );
    case N_MEMORY:
    case N_TEMP:
        return( ChkLoopUsage( op, VU_INVARIANT ) );
    case N_INDEXED:
        if( op->i.index_flags & X_VOLATILE )
            return( false );
        if( !InvariantOp( back, op->i.index ) )
            return( false );
        if( op->i.base == NULL )
            return( !back.loopopts_file->MemChangedInLoop );
        return( InvariantOp( back, op->i.base ) );
    default:
        return( false );
    }
}


static value    OpLTZero( name *op, bool fp )
{
    if( op->n.class_ != N_CONSTANT )
        return( CMP_UNKNOWN );
    /*
     * relocatable constants et all are always > 0
     */
    if( op->c.const_type != CONS_ABSOLUTE )
        return( CMP_FALSE );
    if( fp
      && CFTest( op->c.u.cfval ) < 0 )
        return( CMP_TRUE );
    if( !fp
      && op->c.lo.u.int_value < 0 )
        return( CMP_TRUE );
    return( CMP_FALSE );
}

static value    OpEQZero( name *op, bool fp )
{
    if( op->n.class_ != N_CONSTANT )
        return( CMP_UNKNOWN );
    /*
     * relocatable constants et all are always != 0
     */
    if( op->c.const_type != CONS_ABSOLUTE )
        return( CMP_FALSE );
    if( fp
      && CFTest( op->c.u.cfval ) == 0 )
        return( CMP_TRUE );
    if( !fp
      && op->c.lo.u.int_value == 0 )
        return( CMP_TRUE );
    return( CMP_FALSE );
}


bool     Hoistable( BackRun& back, instruction *ins, block *blk )
/*************************************************
 * Is it safe to hoist instruction "ins" out of the loop (or if)?
 */
{
    bool        will_execute;
    bool        dangerous;
    opcnt       i;
    value       v;
    bool        big_const;

    will_execute = false;
    dangerous = false;
    big_const = false;
    if( blk != NULL
      && back.loopopts_file->LoopProtected
      && IsBlkMarked( blk ) ) {
        will_execute = true;
    }
    if( IsFloating( ins->type_class )
      && IsntModel( back, CGSW_GEN_I_MATH_INLINE ) ) {
        dangerous = true;
    } else {
        for( i = 0; i < ins->num_operands; ++i ) {
            if( ins->operands[i]->n.class_ == N_INDEXED ) {
                dangerous = true;
            }
        }
        if( ins->result != NULL ) {
            if( ins->result->n.class_ == N_INDEXED ) {
                dangerous = true;
            }
        }
    }
    if( IsFloating( ins->type_class )
      || IsI64( ins->type_class ) ) {
        big_const = true;
    }
    switch( ins->head.opcode ) {
    case OP_DIV:
    case OP_MOD:
    case OP_FMOD:
        v = OpEQZero( ins->operands[1], big_const );
        if( v == CMP_TRUE )
            return( false );
        if( v == CMP_UNKNOWN )
            dangerous = true;
        break;
//    case OP_TAN: dangerous for Pi/2 which is an impossible float
    case OP_SQRT:
        v = OpLTZero( ins->operands[0], big_const );
        if( v == CMP_TRUE )
            return( false );
        if( v == CMP_UNKNOWN )
            dangerous = true;
        break;
    case OP_LOG10:
    case OP_LOG:
        v = OpLTZero( ins->operands[0], big_const );
        if( v == CMP_TRUE )
            return( false );
        v = OpEQZero( ins->operands[0], big_const );
        if( v == CMP_TRUE )
            return( false );
        if( v == CMP_UNKNOWN )
            dangerous = true;
        break;
    case OP_POW:
        /*
         * bad for x ** y where x <= 0 and -1 < y < 1 and y != 0
         */
        dangerous = true;
        break;
    case OP_MOV:
    case OP_CONVERT:
    case OP_ROUND:
        if( ins->operands[0]->n.class_ != N_INDEXED )
            return( false );
        break;
    default:
        if( ins->num_operands != 2 )
            return( false );
        if( ins->result == NULL )
            return( false );
        break;
    }
    if( dangerous
      && !will_execute )
        return( false );
    return( true );
}


static bool     InvariantExpr( BackRun& back, instruction *ins, block *blk )
/************************************************************
 * Return true if the operands of instruction "ins" are invariant with
 * respect to "Loop", and "ins" could be safely brought out of the
 * loop.
 */
{
    opcnt       i;

    if( !Hoistable( back, ins, blk ) )
        return( false );
    /*
     * For OP_LA, operand need not be invariant, only its address.
     */
    if( (ins->head.opcode == OP_LA)
      && (ins->operands[0]->n.class_ == N_MEMORY) ) {
        return( true );
    }
    for( i = ins->num_operands; i-- > 0; ) {
        if( !InvariantOp( back, ins->operands[i] ) ) {
            return( false );
        }
    }
    return( true );
}


static void     UpdateLoopLiveInfo( BackRun& back, instruction *ins )
/****************************************************/
{
    hw_reg_set  reg;
    block       *blk;
    opcnt       i;

    HwAsgn( reg, ins->result->r.reg );
    for( i = 0; i < ins->num_operands; ++i ) {
        if( ins->operands[i]->n.class_ == N_REGISTER ) {
            HwTurnOn( reg, ins->operands[i]->r.reg );
        }
    }
    for( ; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        HwTurnOn( ins->head.live.regs, reg );
    }
    HwTurnOn( ins->head.live.regs, reg );
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        ins = (instruction *)&blk->ins;
        do {
            HwTurnOn( ins->head.live.regs, reg );
            ins = ins->head.next;
        } while( ins->head.opcode != OP_BLOCK );
    }
}

induction    *FindIndVar( BackRun& back, name *op )
/******************************************
 * Find an existing induction variable entry for "op".
 */
{
    induction   *var;

    for( var = back.loopopts.IndVarList; var != NULL; var = var->next ) {
        if( var->name == op ) {
            break;
        }
    }
    return( var );
}


static invariant    *CopyInvariant( BackRun& back, invariant *invar )
/*****************************************************
 * Return a copy of invariant list "invar"
 */
{
    invariant   *new_inv;

    if( invar == NULL ) {
        new_inv = NULL;
    } else {
        new_inv = static_cast<invariant *>(CGAlloc( back, sizeof( invariant ) ));
        new_inv->name = invar->name;
        new_inv->times = invar->times;
        new_inv->id = invar->id;
        new_inv->next = static_cast<struct invariant *>(SafeRecurseCG( back, (func_sr)CopyInvariant, invar->next ));
    }
    return( new_inv );
}


static void     MulInvariant( invariant *invar, int_32 by )
/**********************************************************
 * Multiply all entries in invariant list "invar" by "by"
 */
{
    for( ; invar != NULL; invar = invar->next ) {
        invar->times *= by;
    }
}


static invariant    *NewInvariant( BackRun& back, name *op, int times )
/*******************************************************
 * bag a new "invariant"
 */
{
    invariant   *new_inv;

    new_inv = static_cast<invariant *>(CGAlloc( back, sizeof( invariant ) ));
    new_inv->name = op;
    new_inv->times = times;
    new_inv->next = NULL;
    new_inv->id = 0;
    return( new_inv );
}


static void     FreeInvariant( BackRun& back, invariant *invar )
/************************************************
 * Free an invariant list.
 */
{
    invariant   *next;

    for( ; invar != NULL; invar = next ) {
        next = invar->next;
        CGFree( back, invar );
    }
}


static bool     SameInvariant( invariant *i1, invariant *i2 )
/************************************************************
 * return true if "i1" and "i2" are idential invariant lists.
 */
{
    for( ;; ) {
        if( i1 == NULL
          && i2 == NULL )
            return( true );
        if( i1 == NULL )
            return( false );
        if( i2 == NULL )
            return( false );
        if( i1->name != i2->name )
            return( false );
        if( i1->times != i2->times )
            return( false );
        i1 = i1->next;
        i2 = i2->next;
    }
}

static bool     DifferentClasses( type_class_def type_class1, type_class_def type_class2 )
/***********************************************************************
 * return true if "type_class1" and "type_class2" are different, ignoring signed v.s.
 * unsigned differences.
 */
{
    return( Unsigned[type_class1] != Unsigned[type_class2] );
}


void     CommonInvariant( BackRun& back )
/**************************************
 * find moves from one ONE_DEFINITION temp to another. These are
 * loop invariant expressions that got hauled out of the loop
 * from different places, then got caught as common subexpressions
 * in the loop header. We can replace all occurences of the destination
 * with the source.
 */
{
    block               *blk;
    instruction         *ins;
    name                *op;
    name                *res;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            op = ins->operands[0];
            res = ins->result;
            if( ( ins->head.opcode == OP_MOV )
              && ( op->n.class_ == N_TEMP )
              && (op->t.temp_flags & ONE_DEFINITION)
              && ( res->n.class_ == N_TEMP )
              && (res->t.temp_flags & ONE_DEFINITION)
              && ( (res->t.temp_flags & MULT_DEFINITION) == 0 )
              && !back.data.BlockByBlock ) {
                ReplaceAllOccurences( back, res, op );
                op->t.temp_flags |= CROSSES_BLOCKS;
            }
        }
    }
    for( op = back.data.Names[N_TEMP]; op != NULL; op = op->n.next_name ) {
        op->t.temp_flags &= ~ONE_DEFINITION;
    }
}


static bool_maybe   DifferentIV( induction *alias, induction *new_ind )
/*********************************************************************/
{
    if( alias->basic != new_ind->basic )
        return( MB_TRUE );
    if( alias->plus2 != new_ind->plus2 )
        return( MB_TRUE );
    if( alias->times != new_ind->times )
        return( MB_TRUE );
    if( alias->ivtimes != new_ind->ivtimes )
        return( MB_TRUE );
    if( alias->lasttimes != new_ind->lasttimes )
        return( MB_TRUE );
    if( !SameInvariant( alias->invar, new_ind->invar ) )
        return( MB_TRUE );
    if( DifferentClasses( alias->name->n.type_class, new_ind->name->n.type_class ) ) {
        return( MB_TRUE );
    }
    if( alias->plus != new_ind->plus )
        return( MB_MAYBE );
    return( MB_FALSE );
}


static induction    *AddIndVar( BackRun& back, instruction *ins,
                                name *op,
                                induction *prev,
                                invariant *invar,
                                name *ivtimes,
                                invar_id lasttimes,
                                int_32 times,
                                int_32 plus,
                                int_32 plus2,
                                int_32 iv_mult,
                                type_class_def type_class )
/****************************************************************
 * Add an induction variable to our list of induction variables for the
 * current loop. Two idendical induction variables are put on a
 * list of aliases. See INDVARS.H for description of the parameters
 * used to fill in the fields.
 */
{
    induction   *new_ind;
    induction   *alias;

    new_ind = FindIndVar( back, op );
    if( new_ind == NULL ) {
        new_ind = static_cast<induction *>(CGAlloc( back, sizeof( induction ) ));
        new_ind->name = op;
        new_ind->state = static_cast<ind_var_state>(EMPTY);
        if( prev == NULL ) {
            new_ind->basic = new_ind;
            SetVarState( new_ind, IV_BASIC | IV_TOP );
        } else {
            new_ind->basic = prev->basic;
            ClrVarState( prev, IV_TOP );
            SetVarState( new_ind, IV_TOP );
        }
        new_ind->use_count = 0;
        new_ind->index_use_count = 0;
        new_ind->prev = prev;
        new_ind->times = times;
        new_ind->ivtimes = ivtimes;
        new_ind->lasttimes = lasttimes;
        new_ind->plus = plus;
        new_ind->plus2 = plus2;
        new_ind->header = back.loopopts.Head;
        new_ind->invar = CopyInvariant( back, invar );
        MulInvariant( new_ind->invar, iv_mult );
        new_ind->ins = ins;
        new_ind->type_class = type_class;
        new_ind->alias = new_ind;
        new_ind->next = back.loopopts.IndVarList;
        back.loopopts.IndVarList = new_ind;
        back.loopopts_file->NumIndVars++;
        for( alias = back.loopopts.IndVarList; alias != NULL; alias = alias->next ) {
            if( alias == new_ind )
                continue;
            if( HasVarState( alias, IV_ALIAS ) )
                continue;
            if( DifferentIV( alias, new_ind ) != MB_FALSE )
                continue;
            new_ind->alias = alias->alias;
            alias->alias = new_ind;
            SetVarState( new_ind, IV_ALIAS );
            break;
        }
    }
    return( new_ind );
}


static induction    *FindOrAddIndVar( BackRun& back, name *op, type_class_def type_class )
/******************************************************************************
 * Find an existing induction variable entry for "op", or add one if op
 * is an alias for a temporary with an existing entry.
 */
{
    induction   *var1;
    induction   *var2;

    var1 = FindIndVar( back, op );
    if( var1 != NULL )
        return( var1 );
    if( op->n.class_ != N_TEMP )
        return( NULL );
    if( op->v.offset != 0 )
        return( NULL );
    var1 = FindIndVar( back, DeAlias( op ) );
    if( var1 == NULL )
        return( NULL );
    if( HasVarState( var1, IV_DEAD )
      || LacksVarState( var1, IV_BASIC ) )
        return( NULL );
    var2 = AddIndVar( back, var1->ins, op, var1->prev, var1->invar, var1->ivtimes,
                      var1->lasttimes, var1->times, var1->plus,
                      var1->plus2, 1, type_class );
    if( HasVarState( var1, IV_SURVIVED ) )
        SetVarState( var2, IV_SURVIVED );
    if( HasVarState( var1, IV_INTRODUCED ) )
        SetVarState( var2, IV_INTRODUCED );
    return( var2 );
}


static void     FreeVar( BackRun& back, induction *var )
/******************************************
 * Free one induction variable
 */
{
    FreeInvariant( back, var->invar );
    CGFree( back, var );
}


void    FiniIndVars( BackRun& back )
/**************************
 * clean up the induction variable lists.
 */
{
    induction   *next;

    for( ; back.loopopts.IndVarList != NULL; back.loopopts.IndVarList = next ) {
        next = back.loopopts.IndVarList->next;
        FreeVar( back, back.loopopts.IndVarList );
    }
    back.loopopts_file->NumIndVars = 0;
}


static  bool    ListContainsVar( name *list, name *ivname )
/*************************************************************
 * Does the invariant list "list" contain the invariant name "ivname"
 */
{
    for( ; list != NULL; list = list->n.next_name ) {
        if( list == ivname ) {
            return( true );
        }
    }
    return( false );
}


static  void    FreeBadVars( BackRun& back )
/**********************************
 * LoopInsDead may have freed some N_TEMP names.  Free the associated
 * induction variables.
 */
{
    induction   **owner;
    induction   *var;

    owner = &back.loopopts.IndVarList;
    for( ;; ) {
        var = *owner;
        if( var == NULL )
            break;
        if( ListContainsVar( back.data.Names[N_TEMP], var->name )
          || ListContainsVar( back.data.Names[N_MEMORY], var->name ) ) {
            owner = &var->next;
        } else {
            *owner = var->next;
            FreeVar( back, var );
        }
    }
}


bool    Inducable( block *blk, instruction *ins )
/***********************************************/
{
    name        *cons;
    name        *op;

    if( blk->depth == 0 )
        return( false );
    if( ins->head.opcode != OP_ADD
      && ins->head.opcode != OP_SUB ) {
        return( false );
    }
    if( IsFloating( ins->type_class )
      || IsI64( ins->type_class ) ) {
        return( false );
    }
    if( ins->type_class == PT ) {
        return( false );
    }
    op = ins->operands[0];
    cons = ins->operands[1];
    if( cons->n.class_ != N_CONSTANT ) {
        op = ins->operands[1];
        cons = ins->operands[0];
    }
    if( cons->n.class_ != N_CONSTANT )
        return( false );
    if( cons->c.const_type != CONS_ABSOLUTE )
        return( false );
    if( op != ins->result )
        return( false );
    if( op->n.class_ != N_TEMP
      && op->n.class_ != N_MEMORY )
        return( false );
    return( true );
}


static void     CheckBasic( BackRun& back, instruction *ins, name *op, name *cons )
/*********************************************************************
 * Check if "ins" qualifies as an instruction creating a basic
 * induction variable.  Add an induction variable entry if it is.
 */
{
    if( op->n.class_ == N_TEMP
      || op->n.class_ == N_MEMORY ) {
        if( FindIndVar( back, op ) == NULL ) {
            if( ins->head.opcode == OP_ADD ) {
                AddIndVar( back, ins, op, NULL, NULL, NULL, 0, 1,
                           cons->c.lo.u.int_value, 0, 1, ins->type_class );
            } else { /* OP_SUB*/
                AddIndVar( back, ins, op, NULL, NULL, NULL, 0, 1,
                          -(cons->c.lo.u.int_value), 0, 1, ins->type_class );
            }
        }
    }
}

static  bool    BasicNotRedefined( BackRun& back, induction *var, instruction *ins )
/********************************************************************
 * Check that there is no assignment to the basic induction variable of var
 * between the assignment to var and ins (in the same block).
 */
{
    instruction *other;

    for( other = var->ins->head.next; other->head.opcode != OP_BLOCK; other = other->head.next ) {
        if( other == ins )
            return( true );
        if( IsReDefinedBy( back, other, var->basic->name ) ) {
            break;
        }
    }
    return( false );
}


static  bool    KillIndVars( BackRun& back, instruction *ins )
/**********************************************
 * Kill all induction variables that haven't already survived one pass
 * of the loop and are modified by the current instruction (defined >1
 * times in loop)
 */
{
    induction   *var;
    bool        killed;

    killed = false;
    for( var = back.loopopts.IndVarList; var != NULL; var = var->next ) {
        if( var->ins != ins ) {
            if( LacksVarState( var, IV_SURVIVED ) ) {
                if( IsReDefinedBy( back, ins, var->name ) ) {
                    killed = true;
                    SetVarState( var, IV_DEAD );
                }
            }
        }
    }
    return( killed );
}


static  void    CheckNonBasic( BackRun& back, instruction *ins, induction *var,
                               name *cons, bool reverse )
/**********************************************************
 * See if "ins" creates a non-basic induction variable.  (A linear
 * function of another induction variable).  If it does, add an
 * approptriate induction variable entry for it the result of the
 * instruction.  We try to keep things simple by turning a*( b + c )
 * into a*b + a*c when we add induction variables, since they will be
 * constants and fold.
 */
{
    int_32      plus;
    int_32      plus2;
    int_32      times;
    int_32      c;
    int_32      mul;
    invariant   *invar;
    name        *ivtimes;
    invar_id    lasttimes;

    if( var == NULL )
        return;
    if( HasVarState( var, IV_DEAD ) )
        return;
    if( ins->result->n.class_ != N_TEMP
      && ins->result->n.class_ != N_MEMORY )
        return;
    if( reverse
      && ( ins->head.opcode == OP_DIV
      || ins->head.opcode == OP_LSHIFT ) )
        return;
    if( HasVarState( var, IV_BASIC ) ) {
        plus = 0;
        plus2 = 0;
        times = 1;
    } else {
        plus2 = var->plus2;
        plus = var->plus;
        times = var->times;
        if( !BasicNotRedefined( back, var, ins ) ) {
            return;
        }
    }
    invar = var->invar;
    ivtimes = var->ivtimes;
    lasttimes = var->lasttimes;
    c = 0;
    if( cons != NULL ) {
        c = cons->c.lo.u.int_value;
    }
    switch( ins->head.opcode ) {
    case OP_MOV:
    case OP_CONVERT:
        AddIndVar( back, ins, ins->result, var, invar, ivtimes, lasttimes,
                   times, plus, plus2, 1, ins->type_class );
        break;
    case OP_ADD:
        AddIndVar( back, ins, ins->result, var, invar, ivtimes, lasttimes,
                   times, plus + c, plus2, 1, ins->type_class );
        break;
    case OP_MUL:
        AddIndVar( back, ins, ins->result, var, invar, ivtimes, lasttimes,
                   c * times, c * plus, c * plus2, c, ins->type_class );
        break;
    case OP_SUB:
        if( reverse ) {
            AddIndVar( back, ins, ins->result, var, invar, ivtimes, lasttimes,
                       -times, c-plus, -plus2, -1, ins->type_class );
        } else {
            AddIndVar( back, ins, ins->result, var, invar, ivtimes, lasttimes,
                       times, plus-c, plus2, 1, ins->type_class );
        }
        break;
    case OP_LSHIFT:
        mul = 1;
        while( c != 0 ) {
            c--;
            mul <<= 1;
        }
        AddIndVar( back, ins, ins->result, var, invar, ivtimes, lasttimes,
                   mul * times, mul * plus, mul * plus2, mul, ins->type_class );
        break;
    }
}


static  void    CheckInvariant( BackRun& back, instruction *ins, induction *var,
                                name *invariant_op, bool reverse )
/*******************************************************************
 * Check if "ins" is adding, subtracting or multiplying an existing
 * induction variable by a loop invariant (non-constant) variable.  If
 * it is, add an appropriate induction variable entry for the result of
 * the instruction.
 */
{
    int_32      plus;
    int_32      plus2;
    int_32      times;
    int         iv_mult;
    invariant   *invar;
    name        *ivtimes;
    invar_id    lasttimes;

    if( var == NULL )
        return;
    if( HasVarState( var, IV_DEAD ) )
        return;
    if( ins->result->n.class_ != N_TEMP
      && ins->result->n.class_ != N_MEMORY ) {
        return;
    }
    iv_mult = 1;
    ivtimes = var->ivtimes;
    lasttimes = var->lasttimes;
    plus = var->plus;
    plus2 = var->plus2;
    times = var->times;
    if( HasVarState( var, IV_BASIC ) ) {
        times = 1;
        plus = 0;
        plus2 = 0;
    } else {
        if( !BasicNotRedefined( back, var, ins ) ) {
            return;
        }
    }
    if( ins->head.opcode != OP_SUB ) {
        reverse = false;
    }
    switch( ins->head.opcode ) {
    case OP_SUB:
        /*
         * be careful to only allow ONE pointer into an induction expression
         */
        if( ins->type_class == CP )
            return;
        if( ins->type_class == PT )
            return;
        if( reverse ) { /* reverse subtract. Negate! */
            times = -times;
            plus = -plus;
            plus2 = -plus2;
        } else {
            iv_mult = -1;
        }
        /* fall through */
    case OP_ADD:
        invar = NewInvariant( back, invariant_op, iv_mult );
        invar->next = CopyInvariant( back, var->invar );
        if( invar->next == NULL ) {
            invar->id = 1;
        } else {
            invar->id = invar->next->id + 1;
        }
        if( reverse ) {
            MulInvariant( invar->next, -1 );
        }
        break;
    case OP_MUL:
        if( var->ivtimes != NULL )
            return;
        ivtimes = invariant_op;
        plus2 = plus;
        plus = 0;
        invar = CopyInvariant( back, var->invar );
        if( invar != NULL ) {
            lasttimes = invar->id;
        } else {
            lasttimes = 0;
        }
        break;
    default:
        return;
    }
    AddIndVar( back, ins, ins->result, var, invar, ivtimes, lasttimes,
               times, plus, plus2, 1, ins->type_class );
    FreeInvariant( back, invar );
}


static  iv_usage        Uses( BackRun& back, name *op, name *indvar )
/*****************************************************
 * Find out how "indvar" is used within "op".
 */
{
    iv_usage    uses;

    uses = IVU_UNUSED;
    if( op->n.class_ == N_INDEXED ) {
        if( SameThing( op->i.index, indvar ) ) {
            uses = IVU_USED_AS_INDEX;
        }
    } else {
        if( SameThing( op, indvar ) ) {
            uses = IVU_USED_AS_OPERAND;
        } else if( InvariantOp( back, op ) ) {
            uses = IVU_USED_AS_INVARIANT;
        }
    }
    return( uses );
}


static  void    ChkIVUses( BackRun& back, induction *var, name *op )
/***************************************************/
{
    iv_usage    usage;

    usage = Uses( back, op, var->name );
    if( usage == IVU_USED_AS_INDEX
      || usage == IVU_USED_AS_OPERAND ) {
        var->use_count++;
        if( usage == IVU_USED_AS_OPERAND ) {
            SetVarState( var, IV_USED );
        }
    }
    if( usage == IVU_USED_AS_INDEX ) {
        var->index_use_count++;
        SetVarState( var, IV_INDEXED );
    }
    if( usage == IVU_USED_AS_INDEX ) {
        op = op->i.index;
        usage = IVU_USED_AS_OPERAND;
    }
    if( usage == IVU_USED_AS_OPERAND ) {
        if( op != var->name ) {
            SetVarState( var, IV_NOREPLACE );
        }
    }
}


static  void    MarkUses( BackRun& back, instruction *ins )
/******************************************/
{
    induction   *var;
    opcnt       i;

    for( var = back.loopopts.IndVarList; var != NULL; var = var->next ) {
        if( HasVarState( var, IV_SURVIVED ) )
            continue;
        if( ins == var->ins )
            continue;
        for( i = ins->num_operands; i-- > 0; ) {
            ChkIVUses( back, var, ins->operands[i] );
        }
        if( ins->result != NULL ) {
            ChkIVUses( back, var, ins->result );
        }
    }
}


static  void    MarkSurvivors( BackRun& back )
/************************************
 * Mark induction variables that have survived one pass of the loop
 */
{
    block       *blk;
    instruction *ins;
    induction   *var;

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            KillIndVars( back, ins );
            MarkUses( back, ins );
        }
    }
    for( var = back.loopopts.IndVarList; var != NULL; var = var->next ) {
        if( LacksVarState( var, IV_DEAD ) ) {
            SetVarState( var, IV_SURVIVED );
        }
    }
}


static  void    AdjOneIndex( BackRun& back, name **pop, induction *var, induction *new_ind )
/***************************************************************************/
{
    name        *op;

    op = *pop;
    if( op->n.class_ != N_INDEXED )
        return;
    if( Uses( back, op, var->name ) != IVU_USED_AS_INDEX )
        return;
    *pop = ScaleIndex( back, new_ind->name, op->i.base,
                      op->i.constant - ( new_ind->plus - var->plus ),
                      op->n.type_class, op->n.size,
                      op->i.scale, op->i.index_flags );
}


static  void    AdjustIndex( BackRun& back, induction *var, induction *new_ind )
/***************************************************************/
{
    block       *blk;
    instruction *ins;
    opcnt       i;

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            for( i = 0; i < ins->num_operands; ++i ) {
                AdjOneIndex( back, &ins->operands[i], var, new_ind );
            }
            if( ins->result != NULL ) {
                AdjOneIndex( back, &ins->result, var, new_ind );
            }
        }
    }
}


static  void    MergeVars( BackRun& back )
/*******************************/
{
    induction   *var;
    block       *varblock;
    block       *otherblock;
    induction   *other;

    for( var = back.loopopts.IndVarList; var != NULL; var = var->next ) {
        if( LacksVarState( var, IV_TOP ) )
            continue;
        if( HasVarState( var, IV_DEAD ) )
            continue;
        if( HasVarState( var , IV_BASIC | IV_ALIAS ) )
            continue;
        varblock = InsBlock( var->ins );
        for( other = var->next; other != NULL; other = other->next ) {
            if( LacksVarState( other, IV_TOP ) )
                continue;
            if( HasVarState( other, IV_DEAD ) )
                continue;
            if( HasVarState( other , IV_BASIC | IV_ALIAS ) )
                continue;
            if( DifferentIV( var, other ) != MB_MAYBE )
                continue;
            if( HasVarState( other, IV_USED ) )
                continue;
            if( LacksVarState( other, IV_INDEXED ) )
                continue;
            otherblock = InsBlock( other->ins );
            if( otherblock != varblock )
                continue;
            InsNotBlock( back, var->ins );
            InsNotBlock( back, other->ins );
            if( var->ins->id < other->ins->id ) {
                if( !BasicNotRedefined( back, var, other->ins ) )
                    continue;
                AdjustIndex( back, other, var );
            } else {
                if( !BasicNotRedefined( back, other, var->ins ) )
                    continue;
                AdjustIndex( back, var, other );
                var = var->next;
                break;
            }
        }
    }
}


static  bool    IsAddressMode( induction *var )
/*********************************************/
{
    /* unused parameters */ (void)var;

    return( false );
}


static  void    ScanNonBasic( BackRun& back, instruction *ins )
/***********************************************
 * see FindNonBasics ^
 */
{
    induction   *var;
    name        *op;

    if( KillIndVars( back, ins ) )
        return;
    if( IsFloating( ins->type_class )
      || IsI64( ins->type_class ) )
        return;
    if( ins->head.opcode == OP_MOV ) {
        var = FindIndVar( back, ins->operands[0] );
        CheckNonBasic( back, ins, var, NULL, false );
        return;
    }
    if( ins->head.opcode != OP_ADD
      && ins->head.opcode != OP_SUB
      && ins->head.opcode != OP_MUL
      && ins->head.opcode != OP_LSHIFT )
        return;
    if( ins->operands[0]->n.class_ == N_CONSTANT ) {
        op = ins->operands[0];
        var = FindOrAddIndVar( back, ins->operands[1], ins->type_class );
        if( op->c.const_type == CONS_ABSOLUTE ) {
            CheckNonBasic( back, ins, var, op, true );
        } else {
            CheckInvariant( back, ins, var, op, true );
        }
    } else if( ins->operands[1]->n.class_ == N_CONSTANT ) {
        op = ins->operands[1];
        var = FindOrAddIndVar( back, ins->operands[0], ins->type_class );
        if( op->c.const_type == CONS_ABSOLUTE ) {
            CheckNonBasic( back, ins, var, op, false );
        } else {
            CheckInvariant( back, ins, var, op, false );
        }
    } else if( InvariantOp( back, ins->operands[0] ) ) {
        var = FindOrAddIndVar( back, ins->operands[1], ins->type_class );
        CheckInvariant( back, ins, var, ins->operands[0], true );
    } else if( InvariantOp( back, ins->operands[1] ) ) {
        var = FindOrAddIndVar( back, ins->operands[0], ins->type_class );
        CheckInvariant( back, ins, var, ins->operands[1], false );
    }
}


static  void    ScanForNonBasics( BackRun& back, block *blk )
/*********************************************
 * see FindNonBasics ^
 */
{
    instruction *ins;

    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        ScanNonBasic( back, ins );
    }
}


static  void    FindNonBasics( BackRun& back )
/************************************
 * Find all non-basic induction variables 'i' in the loop this is a
 * variable whose only definition in the loop is i = j + - * / << const
 * or  i = j +- loop invariant variable where j is an existing
 * induction variable
 */
{
    block       *blk;

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        ScanForNonBasics( back, blk );
    }
    MarkSurvivors(back);
}


static  void    ScanBasic( BackRun& back, instruction *ins )
/********************************************
 * see FindBasics ^
 */
{
    name    *op;

    if( KillIndVars( back, ins ) )
        return;
    if( ins->head.opcode != OP_ADD
      && ins->head.opcode != OP_SUB )
        return;
    if( IsFloating( ins->type_class )
      || IsI64( ins->type_class ) )
        return;
    if( ins->type_class == PT )
        return;
    op = ins->operands[0];
    if( op->n.class_ == N_CONSTANT
      && op->c.const_type == CONS_ABSOLUTE ) {
        if( ins->operands[1] == ins->result ) {
            if( ins->head.opcode == OP_ADD ) {
                CheckBasic( back, ins, ins->result, op );
            }
        }
    } else {
        op = ins->operands[1];
        if( op->n.class_ == N_CONSTANT
          && op->c.const_type == CONS_ABSOLUTE ) {
            if( ins->operands[0] == ins->result ) {
                CheckBasic( back, ins, ins->result, op );
            }
        }
    }
}


static  void    ScanForBasics( BackRun& back, block *blk )
/******************************************
 * See FindBasics ^
 */
{
    instruction *ins;

    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        ScanBasic( back, ins );
    }
}


static  void    FindBasics( BackRun& back )
/*********************************
 * Find all the basic induction variables in the current loop a basic
 * induction variable is one whose only defn in the loop is i = i +-
 * constant;
 */
{
    block       *blk;

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        ScanForBasics( back, blk );
    }
    MarkSurvivors(back);
}


static  void    ReplaceOccurences( BackRun& back, name *of, name *with )
/**********************************************************
 * Replace all occurences of "of" with "with" in "Loop".
 */
{
    block       *blk;
    instruction *ins;
    opcnt       i;

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
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


static  name    *FindPointerPart( induction *var )
/***************************************************
 * Given induction variable "var", find out where the pointer part of
 * the induction variable is.  There is only one since expressions
 * involving pointers only allow one pointer with the addition and
 * subtraction of integers.
 */
{
    invariant   *invar;
    name        *first;

    first = var->basic->name;
    if( first->n.type_class == CP
      || first->n.type_class == PT )
        return( first );
    for( invar = var->invar; invar != NULL; invar = invar->next ) {
        first = invar->name;
        if( first->n.type_class == CP
          || first->n.type_class == PT ) {
            return( first );
        }
    }
    return( NULL );
}


static  instruction     *Multiply( BackRun& back, name *op, int_32 by, name *temp,
                            type_class_def type_class, instruction *prev )
/*************************************************************************
 * Generate "optimal" code for op*by => temp and suffix instruction
 * "prev" with it.  Return a pointer to the last instruction generated.
 */
{
    bool        negative;
    int         log2;
    instruction *ins;

    if( by < 0 ) {
        negative = true;
        by = -by;
    } else {
        negative = false;
    }
    log2 = GetLog2( by );
    if( log2 == 0 ) {
        ins = MakeMove( back, op, temp, type_class );
        SuffixIns( back, prev, ins );
    } else if( log2 == -1 ) {
        ins = MakeBinary( back, OP_MUL, op, AllocS32Const( back, by ), temp, type_class );
        SuffixIns( back, prev, ins );
    } else {
        ins = MakeBinary( back, OP_LSHIFT, op, AllocS32Const( back, log2 ), temp, type_class );
        SuffixIns( back, prev, ins );
    }
    if( negative ) {
        prev = MakeUnary( back, OP_NEGATE, temp, temp, op->n.type_class );
        SuffixIns( back, ins, prev );
        ins = prev;
    }
    return( ins );
}


static  instruction     *MakeMul( BackRun& back, instruction *prev,
                                  name *op, int_32 by, name *ivtimes )
/*********************************************************************
 * Generate "optimal" instruction calcualte "op" * "by" * "ivtimes" and
 * place them after "prev".  Return a pointer to the last instruction
 * generated.
 */
{
    instruction         *ins;
    name                *temp;
    type_class_def      type_class;

    type_class = op->n.type_class;
    temp = AllocTemp( back, type_class );
    ins = Multiply( back, op, by, temp, type_class, prev );
    if( ivtimes != NULL ) {
        prev = MakeBinary( back, OP_MUL, temp, ivtimes, temp, type_class );
        SuffixIns( back, ins, prev );
        ins = prev;
    }
    return( ins );
}


void    SuffixPreHeader( BackRun& back, instruction *ins )
/********************************************
 * Suffix the pre-header of "Loop" with ins.
 */
{
    instruction *last;

    for( last = back.loopopts.PreHead->ins.head.prev; last->head.opcode == OP_NOP; last = last->head.prev ) {
        if( last->flags.u.nop_flags & NOP_ZAP_INFO ) {
            break;
        }
    }
    SuffixIns( back, last, ins );
}


static  void    IncAndInit( BackRun& back, induction *var, name *iv, type_class_def type_class )
/****************************************************************************
 * Generate code in the pre-header to initialize newly created
 * induction variable "iv", based on the information found in "var".
 * There is a fair bit of jiggery pokery going on here, since an
 * induction variable entry may contain one entry of type pointer
 * (either the basic induction variable or one of the invariant
 * expressions) and the rest of type integer.  This means we have to be
 * REAL careful with typing when generating code.
 */
{
    instruction *ins;
    invariant   *invar;
    name        *first;
    induction   *basic;
    name        *temp;

    /*
     * generate the addition of the appropriate constant in the loop
     */
    basic = var->basic;
    if( var->ivtimes != NULL ) {
        temp = AllocTemp( back, var->ivtimes->n.type_class );
        ins = Multiply( back, var->ivtimes, var->times * basic->plus,
                        temp, temp->n.type_class, back.loopopts.PreHead->ins.head.prev );
        temp = ins->result;
        temp->t.temp_flags |= CROSSES_BLOCKS;
    } else {
        temp = AllocS32Const( back, var->times * basic->plus );
    }
    ins = MakeBinary( back, OP_ADD, iv, temp, iv, type_class );
    PrefixIns( back, basic->ins, ins );
    /*
     * initialize the new induction variable in the loop preheader.
     */
    temp = AllocTemp( back, type_class );
    if( type_class != CP
      && type_class != PT ) {
        first = NULL;
    } else {
        first = FindPointerPart( var );
    }
    if( first != NULL ) { /* can do this because ptr multiplies aren't allowed*/
        ins = MakeBinary( back, OP_ADD, first, AllocS32Const( back, var->plus ), temp, type_class );
        SuffixPreHeader( back, ins );
    }
    if( first != basic->name ) {
        ins = MakeMul( back, back.loopopts.PreHead->ins.head.prev, basic->name, var->times, var->ivtimes );
        if( first == NULL ) {
            ins->result = temp;
        } else {
            ins = MakeBinary( back, OP_ADD, temp, ins->result, temp, type_class );
            SuffixPreHeader( back, ins );
        }
    }
    if( first == NULL ) {
        ins = MakeBinary( back, OP_ADD, temp, AllocS32Const( back, var->plus ), temp, type_class );
        SuffixPreHeader( back, ins );
    }
    for( invar = var->invar; invar != NULL; invar = invar->next ) {
        if( invar->name != first ) {
            if( invar->times == 1
              && ( invar->id > var->lasttimes ) ) {
                ins = MakeBinary( back, OP_ADD, temp, invar->name, temp, type_class );
                SuffixPreHeader( back, ins );
            } else {
                ins = MakeMul(back, back.loopopts.PreHead->ins.head.prev,invar->name, invar->times,
                      (invar->id > var->lasttimes) ? NULL : var->ivtimes );
                ins = MakeBinary( back, OP_ADD, temp, ins->result, temp, type_class );
                SuffixPreHeader( back, ins );
            }
        }
    }
    if( var->plus2 != 0 ) {
        ins = Multiply( back, var->ivtimes, var->plus2,
                        AllocTemp( back, var->ivtimes->n.type_class ),
                        var->ivtimes->n.type_class, back.loopopts.PreHead->ins.head.prev );
        ins = MakeBinary( back, OP_ADD, temp, ins->result, iv, type_class );
    } else {
        ins = MakeMove( back, temp, iv, type_class );
    }
    SuffixPreHeader( back, ins );
}


static  void *MarkDown( BackRun& back, block *blk )
/**********************************/
{
    block_num   i;

    if( !IsBlkAttr( blk, BLK_IN_LOOP ) )
        return( NULL );
    if( blk == back.loopopts.Head )
        return( NULL );
    if( !IsBlkMarked( blk ) )
        return( NULL );
    MarkBlkUnMarked( blk );
    for( i = blk->targets; i-- > 0; ) {
        SafeRecurseCG( back, (func_sr)MarkDown, blk->edge[i].destination.u.blk );
    }
    return( NULL );
}


static  void    LabelDown( BackRun& back, instruction *frum,
                           instruction *avoiding, bool go_around )
/********************************************************************
 * start labeling instructions INS_VISITED starting at "frum", stopping
 * if we hit instruction "avoiding".  If we missed "avoiding", label
 * all successor blocks as BLK_BLOCK_VISITED. See paint analogy in PathFrom.
 */
{
    block       *blk;
    block_num   i;
    block_edge  *edge;
    instruction *ins;

    for( ; frum->head.opcode != OP_BLOCK; frum = frum->head.next ) {
        if( frum == avoiding )
            return;
        frum->ins_flags |= INS_VISITED;
    }
    if( frum == avoiding )
        return;
    blk = BlockOf( frum );
    edge = &blk->edge[0];
    for( i = blk->targets; i > 0; --i ) {
        blk = edge->destination.u.blk;
        if( ( go_around
          || blk != back.loopopts.Head )
          && IsBlkAttr( blk, BLK_IN_LOOP ) ) {
            ins = blk->ins.head.next;
            if( ins->head.opcode == OP_BLOCK
              || (ins->ins_flags & INS_VISITED) == 0 ) {
                MarkBlkVisited( blk );
            }
            ++edge;
        }
    }
}


static  bool    PathFrom( BackRun& back, instruction *frum, instruction *to,
                          instruction *avoiding, bool go_around )
/******************************************************************
 * Return true if there is a path from 'from' to 'to' avoiding
 * 'avoiding'.  Conceptually, this is done by dropping some paint on
 * instruction "from" and letting it flow until it hits either "to",
 * "avoiding", or the end of the program.  If "to" gets painted, then
 * we return true.
 */
{
    bool        change;
    block       *blk;
    bool        foundpath;
    instruction *ins;

    LabelDown( back, frum, avoiding, go_around );
    do {
        change = false;
        for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
            if( IsBlkVisited( blk ) ) {
                MarkBlkUnVisited( blk );
                change = true;
                LabelDown( back, blk->ins.head.next, avoiding, go_around );
            }
        }
    } while( change );
    if( to->ins_flags & INS_VISITED ) {
        foundpath = true;
    } else {
        foundpath = false;
    }
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        MarkBlkUnVisited( blk );
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            ins->ins_flags &= ~INS_VISITED;
        }
    }
    return( foundpath );
}


static  bool    NoPathThru( BackRun& back, instruction *ins1,
                            instruction *ins2, instruction *ins3 )
/*******************************************************************
 * return true if there is no path from ins1 to ins2 going through ins3
 */
{
    if( PathFrom( back, ins3, ins2, ins1, false )
      && PathFrom( back, ins1, ins3, ins2, false ) ) {
        return( false ); /* we found a path from ins1 to ins2 going thru ins3 */
    }
    if( PathFrom( back, ins2, ins3, ins1, false )
      && PathFrom( back, ins3, ins1, ins2, false ) ) {
        return( false ); /* we found a path from ins2 to ins1 going thru ins3 */
    }
    return( true );
}


static  void    MarkWillExecBlocks( BackRun& back )
/****************************************/
{
    block       *blk;
    block_num   i;
    instruction *nop;

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        MarkBlkMarked( blk );
    }
    /*
     * First, prune some blocks we know won't necessarily execute
     */
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        if( !IsBlkAttr( blk, BLK_LOOP_EXIT ) )
            continue;
        for( i = blk->targets; i-- > 0; ) {
            MarkDown( back, blk->edge[i].destination.u.blk );
        }
    }

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        if( blk->targets <= 1 )
            continue;
        for( i = blk->targets; i-- > 0; ) {
            if( blk->edge[i].destination.u.blk->inputs == 1 ) {
                MarkBlkUnMarked( blk->edge[i].destination.u.blk );
            }
        }
    }

    if( back.loopopts.Head->ins.head.prev == back.loopopts.Head->ins.head.next ) {
        nop = MakeNop(back);
        /*
         * Must (can hit block) and Ok to (ins killed) not renumber
         */
        PrefixInsRenum( back, back.loopopts.Head->ins.head.next, nop, false ); // so PathFrom works
    } else {
        nop = NULL;
    }

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        if( !IsBlkMarked( blk ) )
            continue;
        if( blk->ins.head.next == (instruction *)&blk->ins ) {
            MarkBlkUnMarked( blk );
            continue;
        }
        if( PathFrom( back, back.loopopts.Head->ins.head.prev, back.loopopts.Head->ins.head.next, blk->ins.head.next, true ) ) {
            MarkBlkUnMarked( blk );
        }
    }
    if( nop != NULL ) {
        FreeIns( back, nop );
    }
}


static  void    UnMarkWillExecBlocks( BackRun& back )
/******************************************/
{
    block       *blk;

    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        MarkBlkUnMarked( blk );
    }
}

static  bool    InstructionWillExec( BackRun& back, instruction *ins )
/********************************************************
 * true if the given instruction will be executed every time through the loop.
 */
{
    instruction *top;

    top = back.loopopts.Head->ins.head.next;
    if( top == ins )
        return( true );
    if( PathFrom( back, top->head.next, top, ins, true ) ) {
        return( false );
    }
    return( true );
}


void    MoveDownLoop( BackRun& back, block *cond )
/**********************************
 * Muck about so that "cond" will come out after the blocks which jump
 * to it when we sort the blocks into original order (gen_blk_id) prior to
 * actual code dumping.
 */
{
    block_id    cond_blk_id;
    block_id    after_blk_id;
    block       *blk;
    block_edge  *edge;
    block       *after;
    block_num   i;

    after = cond->input_edges->source;
    for( edge = cond->input_edges; edge != NULL; edge = edge->next_source ) {
        if( edge->source->gen_blk_id > after->gen_blk_id ) {
            after = edge->source;
        }
    }
    if( cond->gen_blk_id > after->gen_blk_id )
        return;
    cond_blk_id = cond->gen_blk_id;
    after_blk_id = after->gen_blk_id;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( blk->gen_blk_id >= cond_blk_id
          && blk->gen_blk_id <= after_blk_id ) {
            blk->gen_blk_id--;
        }
    }
    cond->gen_blk_id = after_blk_id;
    cond->edge[0].flags &= ~BEF_BLOCK_LABEL_DIES;
    for( edge = cond->input_edges; edge != NULL; edge = edge->next_source ) {
        edge->flags &= ~BEF_DEST_LABEL_DIES;
    }
    for( i = cond->targets; i-- > 0; ) {
        cond->edge[i].flags &= ~BEF_DEST_LABEL_DIES;
        cond->edge[i].destination.u.blk->edge[0].flags &= ~BEF_BLOCK_LABEL_DIES;
    }
}


static  void            AdjustOp( BackRun& back, instruction *blk_end, name **pop,
                                  name *var, int_32 adjust )
/***************************************************************/
{
    name        *op;

    op = *pop;
    if( op->n.class_ == N_TEMP
      && blk_end == NULL ) {
        op->t.temp_flags |= CROSSES_BLOCKS;
    }
    if( op->n.class_ != N_INDEXED )
        return;
    if( op->i.index != var )
        return;
    *pop = ScaleIndex( back, op->i.index, op->i.base,
                       op->i.constant + adjust,
                       op->n.type_class, op->n.size,
                       op->i.scale, op->i.index_flags );
}


instruction     *DupIns( BackRun& back, instruction *blk_end, instruction *ins,
                                 name *var, int_32 adjust )
/**************************************************************/
{
    instruction *new_ins;
    opcnt       num_operands;
    opcnt       i;

    new_ins = NewIns( back, ins->num_operands );
    num_operands = ins->num_operands;
    memcpy( new_ins, ins, offsetof( instruction, operands ) + num_operands * sizeof( name * ) );
    for( i = 0; i < num_operands; ++i ) {
        AdjustOp( back, blk_end, &new_ins->operands[i], var, adjust );
    }
    if( new_ins->result != NULL ) {
        AdjustOp( back, blk_end, &new_ins->result, var, adjust );
    }
    if( blk_end == NULL ) {
        SuffixPreHeader( back, new_ins );
    } else {
        SuffixIns( back, blk_end, new_ins );
        blk_end = new_ins;
    }
    new_ins->head.line_num = ins->head.line_num;
    return( blk_end );
}


instruction     *DupInstrs( BackRun& back, instruction *blk_end,
                            instruction *first, instruction *last,
                            induction *var, int_32 adjust )
/****************************************************************/
{
    instruction *ins;
    name        *ind;

    ind = NULL;
    if( var != NULL ) {
        ind = var->name;
    }
    for( ins = first; ins != last->head.next; ins = ins->head.next ) {
        blk_end = DupIns( back, blk_end, ins, ind, adjust );
    }
    return( blk_end );
}




static  induction       *FindReplacement( BackRun& back, induction *var )
/***********************************************************
 * Find a replacement induction variable for var - the one with the
 * 'simplest' times and plus fields is the the prefered replacement
 */
{
    induction           *replacement;
    induction           *other;
    uint                log2rep;
    uint                log2oth;
    type_class_def      var_type_class;
    type_class_def      oth_type_class;

    if( var->ivtimes != NULL )
        return( NULL );
    replacement = NULL;
    var_type_class = Unsigned[var->name->n.type_class];
    if( Unsigned[var->ins->type_class] != var_type_class )
        return( NULL );
    log2rep = 0;
    for( other = back.loopopts.IndVarList; other != NULL; other = other->next ) {
        if( HasVarState( other, IV_DEAD ) )
            continue;
        if( LacksVarState( other, ( IV_INTRODUCED | EMPTY ) ) )
            continue;
        if( other == var )
            continue;
        if( other->ivtimes != NULL )
            continue;
        if( other->basic != var )
            continue;
        oth_type_class = Unsigned[other->name->n.type_class];
        if( oth_type_class == var_type_class
          || ( oth_type_class == WD
          && var_type_class == CP )
          ) {
            log2oth = GetLog2( other->times );
            if( ( replacement == NULL )
              || ( replacement->invar != NULL && other->invar == NULL )
              || ( log2oth < log2rep )
              || ( log2oth == log2rep && other->plus < replacement->plus ) ) {
                replacement = other;
                log2rep = GetLog2( replacement->times );
            }
        }
    }
    return( replacement );
}


bool    AnalyseLoop( BackRun& back, induction *var, bool *ponecond,
                             instruction **pcond, block **pcond_blk )
/********************************************************************/
{
    bool        can_replace;
    block       *blk;
    block       *first_blk;
    instruction *ins;
    iv_usage    usage;
    opcnt       i;

    *ponecond = false;
    *pcond = NULL;
    *pcond_blk = NULL;
    can_replace = true;
    first_blk = back.loopopts.PreHead->edge[0].destination.u.blk;
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( var == NULL
              || ins != var->ins ) {
                if( !OpIsCondition( ins->head.opcode )
                  || ins->result != NULL ) {
                    if( var != NULL ) {
                        for( i = ins->num_operands; i-- > 0; ) {
                            usage = Uses( back, ins->operands[i], var->name );
                            if( usage == IVU_USED_AS_INDEX
                              || usage == IVU_USED_AS_OPERAND ) {
                                can_replace = false;
                            }
                        }
                        if( ins->result != NULL ) {
                            usage = Uses( back, ins->result, var->name );
                            if( usage == IVU_USED_AS_INDEX
                              || usage == IVU_USED_AS_OPERAND ) {
                                can_replace = false;
                            }
                        }
                    }
                } else {
                    if( IsBlkAttr( blk, BLK_LOOP_EXIT ) ) {
                        if( *pcond == NULL ) {
                            *pcond = ins;
                            *ponecond = true;
                            *pcond_blk = blk;
                        } else {
                            *ponecond = false;
                            if( first_blk == blk ) { // preferred by TwistLoop
                                *pcond = ins;
                                *pcond_blk = blk;
                            }
                        }
                        if( var != NULL ) {
                            for( i = ins->num_operands; i-- > 0; ) {
                                usage = Uses( back, ins->operands[i], var->name );
                                if( usage == IVU_USED_AS_INDEX ) {
                                    can_replace = false;
                                }
                            }
                        }
                    } else if( var != NULL ) {
                        for( i = ins->num_operands; i-- > 0; ) {
                            usage = Uses( back, ins->operands[i], var->name );
                            if( usage == IVU_USED_AS_INDEX
                              || usage == IVU_USED_AS_OPERAND ) {
                                can_replace = false;
                            }
                        }
                    }
                }
            }
        }
    }
    return( can_replace );
}


static int_32       Sgn( int_32 x )
/*********************************/
{
    if( x < 0 )
        return( -1 );
    return( 1 );
}


static int_32       Abs( int_32 x )
/*********************************/
{
    if( x < 0 )
        return( -x );
    return( x );
}


static  name    *InitialValue( BackRun& back, name *op )
/****************************************
 * Scan the loop pre-header to find an assignment of a constant to
 * "op", to find out what the initial value of "op" is going into the
 * loop.
 */
{
    instruction *other;
    block       *blk;

    other = back.loopopts.PreHead->ins.head.prev;
    for( ;; ) {
        if( other->head.opcode == OP_BLOCK ) {
            blk = BlockOf( other );
            if( IsBlkAttr( blk, BLK_IN_LOOP ) )
                return( NULL );
            if( blk->inputs != 1 )
                return( NULL );
            other = blk->input_edges->source->ins.head.prev;
            continue;  /* re-check in case of empty blocks */
        }
        if( IsReDefinedBy( back, other, op ) )
            break;
        other = other->head.prev;
    }
    if( other->head.opcode != OP_MOV )
        return( NULL );
    if( other->result != op )
        return( NULL );
    op = other->operands[0];
    if( op->n.class_ != N_CONSTANT )
        return( NULL );
    return( op );
}


bool    CalcFinalValue( BackRun& back, induction *var, block *blk, instruction *ins,
                                int_32 *final, int_32 *initial )
/*********************************************************************
 * See if we can figure out what the final value of induction variable
 * "var" will be, based on the facts that instruction "ins" (in block
 * "blk") is the only exit from "Loop".
 */
{
    name            *temp;
    name            *op;
    int_32          init;
    int_32          test;
    int_32          incr;
    int_32          remd;
    int_32          dist;

    if( HasVarState( var, IV_DEAD ) )
        return( false );
    if( ins == NULL )
        return( false );
    if( ins->head.opcode == OP_BIT_TEST_TRUE )
        return( false );
    if( ins->head.opcode == OP_BIT_TEST_FALSE )
        return( false );
    if( ins->operands[0]->n.class_ == N_CONSTANT ) {
        temp = ins->operands[0];
        ins->operands[0] = ins->operands[1];
        ins->operands[1] = temp;
        RevCond( ins );
    }
    if( IsBlkAttr( blk->edge[TrueIndex( ins )].destination.u.blk, BLK_IN_LOOP ) ) {
        SetBlockIndex( ins, FalseIndex( ins ), TrueIndex( ins ) );
        FlipCond( ins );
    }
    if( ins->head.opcode == OP_CMP_NOT_EQUAL )
        return( false );
    op = ins->operands[1];
    if( op->n.class_ != N_CONSTANT )
        return( false );
    if( op->c.const_type != CONS_ABSOLUTE )
        return( false );
    if( ins->operands[0] != var->name )
        return( false );
    op = InitialValue( back, var->name );
    if( op == NULL )
        return( false );
    if( op->c.const_type != CONS_ABSOLUTE )
        return( false );
    init = op->c.lo.u.int_value;
    *initial = init;
    test = ins->operands[1]->c.lo.u.int_value;
    incr = var->plus;
    dist = test - init;
    if( Sgn( dist ) != Sgn( incr ) )
        return( false );
    remd = ( Abs( dist ) % Abs( incr ) ) * Sgn( incr );
    switch( ins->head.opcode ) {
    case OP_CMP_EQUAL:
        if( remd != 0 )
            return( false );
        *final = test;
        break;
    case OP_CMP_GREATER:
        if( incr <= 0 )
            return( false );
        if( test <= init )
            return( false );     // wraps or exits immediately
        *final = test + ( incr - remd );
        break;
    case OP_CMP_GREATER_EQUAL:
        if( incr <= 0 )
            return( false );
        if( test <= init )
            return( false );     // wraps or exits immediately
        if( remd == 0 ) {
            *final = test;
        } else {
            *final = test + ( incr - remd );
        }
        break;
    case OP_CMP_LESS:
        if( incr >= 0 )
            return( false );
        if( test >= init )
            return( false );     // wraps or exits immediately
        *final = test + ( incr - remd );
        break;
    case OP_CMP_LESS_EQUAL:
        if( incr >= 0 )
            return( false );
        if( test >= init )
            return( false );     // wraps or exits immediately
        if( remd == 0 ) {
            *final = test;
        } else {
            *final = test + ( incr - remd );
        }
        break;
    }
    if( InstructionWillExec( back, var->ins ) ) {
        back.loopopts.Head->iterations = ( *final - *initial ) / incr;
        MarkBlkAttrSet( back.loopopts.Head, BLK_ITERATIONS_KNOWN );
    }
    return( true );
}


static  bool    FinalValue( BackRun& back, instruction *ins, block *blk, induction *var )
/***************************************************************************
 * Figure out the final value of the induction variable if possible.  If
 * we can, put assignments of the final value at each loop exit so that
 * the value of the induction instruction is not used outside the loop
 * Also, change the exit condition to be exit when equal since we can
 * always replace induction variables with other ones in equality
 * comparisons
 */
{
    instruction         *other;
    name                *op;
    int_32              final;
    int_32              initial;
    type_class_def      type_class;
    block               *dest;

    if( !CalcFinalValue( back, var, blk, ins, &final, &initial ) ) {
        return( false );
    }
    dest = blk->edge[TrueIndex( ins )].destination.u.blk;
    if( IsBlkAttr( dest, BLK_UNKNOWN_DESTINATION ) )
        return( false );
    if( IsBlkAttr( dest, BLK_LOOP_HEADER ) )
        return( false );
//    type_class = var->name->n.type_class;
    type_class = ins->type_class;
    assert( TypeClassSize[type_class] == TypeClassSize[var->name->n.type_class] );
    op = AllocS32Const( back, final );
    other = MakeCondition( back, OP_CMP_EQUAL, var->name, op,
                           TrueIndex( ins ), FalseIndex( ins ), type_class );
    ReplIns( back, ins, other );
    if( dest->inputs == 1 ) {
        /*
         * if dest has 1 input then we can do this assignment since we
         * know we must have come from the loop when arriving there
         * (otherwise we might be mucking with vars used in a later block)
         */
        ins = MakeMove( back, op, var->name, type_class );
        SuffixIns( back, (instruction *)&dest->ins, ins );
    }
    return( true );
}


static  bool    PointerOk( name *op )
/***********************************/
{
    /* unused parameters */ (void)op;

    return( false );
}


static  bool    DangerousTypeChange( induction *var, induction *other )
/**********************************************************************
 * Is it dangerous to replace "var" with "other"?  This is true if
 * "other" might wrap around its limits.
 */
{
    invariant   *invar;

    if( Unsigned[var->type_class] == Unsigned[other->type_class] )
        return( false );
    if( PointerOk( other->name ) )
        return( false );
    for( invar = other->invar; invar != NULL; invar = invar->next ) {
        if( PointerOk( invar->name ) ) {
            return( false );
        }
    }
    return( true );
}

static  bool    ConstOverflowsType( BackRun& back, const signed_64 *val, type_class_def type_class )
/************************************************************************************
 * Return true if the given (signed) constant is too big or too small for
 * the given type_class.
 */
{
    type_length         len;
    signed_64           min;
    signed_64           max;
    signed_64           one;

    if( type_class == XX ) {
        Zoiks( back, ZOIKS_139 );
        return( false );
    }
    if( type_class == I8
      || type_class == U8 )
        return( false );
    len = back.data.TypeClassSize[type_class] * 8;
    Set64Val1p( one );
    if( IsSigned( type_class ) ) {
        /*
         * signed type of length 'len' bits
         */
        U64ShiftL( &max, &one, len - 1 );
        U64Neg( &min, &max );
        U64SubEq( &max, &one );
    } else {
        /*
         * unsigned type of length 'len' bits
         */
        Set64ValZero( min );
        U64ShiftL( &max, &one, len );
        U64SubEq( &max, &one );
    }
    if( I64Cmp( val, &min ) < 0 )
        return( true );
    if( I64Cmp( val, &max ) > 0 )
        return( true );
    return( false );
}

static  bool    DoReplacement( BackRun& back, instruction *ins, induction *rep,
                               opcnt ind, opcnt non_ind, type_class_def type_class )
/***********************************************************************************
 * Replace operands[ind] with "rep" in instruction "ins".
 * operands[non_ind] is guaranteed not to be another induction variable.
 * This may generate a bunch of code to calculate the correct linear
 * function of the non-induction operand that we need to compare
 * against "rep", but this code will all be loop invariant, so it'll
 * get hauled out into the pre-header.
 */
{
    name        *non_ind_op;
    instruction *prev_ins;
    instruction *new_ins;
    invariant   *invar;

    non_ind_op = ins->operands[non_ind];
    prev_ins = back.loopopts.PreHead->ins.head.prev;
    if( non_ind_op->n.class_ == N_CONSTANT
      && non_ind_op->c.const_type == CONS_ABSOLUTE ) {
        signed_64       big_cons;
        signed_64       temp;

        /*
         * if we are going to overflow our type, bail!
         */
        Set64ValI32( big_cons, non_ind_op->c.lo.u.int_value );
        Set64ValI32( temp, rep->times );
        U64MulEq( &big_cons, &temp );
        Set64ValI32( temp, rep->plus );
        U64AddEq( &big_cons, &temp );
        /*
         * make sure we always allow negative values - hack for BMark
         */
        if( big_cons.u.sign.v == 0 ) {
            if( ConstOverflowsType( back, &big_cons, type_class ) ) {
                return( false );
            }
        }
        ins->type_class = type_class;
        ins->operands[ind] = rep->name;
        ins->operands[non_ind] = AllocS64Const( back, U64Low( big_cons ), U64High( big_cons ) );
    } else {
        ins->type_class = type_class;
        ins->operands[ind] = rep->name;
        new_ins = MakeMul( back, prev_ins, non_ind_op, rep->times, NULL );
        prev_ins = new_ins;
        new_ins = MakeBinary( back, OP_ADD, new_ins->result, AllocS32Const( back, rep->plus ), new_ins->result, type_class );
        ins->operands[non_ind] = new_ins->result;
        SuffixIns( back, prev_ins, new_ins );
        prev_ins = new_ins;
    }
    if( rep->times < 0 ) {
        RevCond( ins );
    }
    for( invar = rep->invar; invar != NULL; invar = invar->next ) {
        new_ins = MakeMul( back, prev_ins, invar->name, invar->times, NULL );
        prev_ins = new_ins;
        if( DifferentClasses( new_ins->result->n.type_class, type_class ) ) {
            new_ins = MakeConvert( back, new_ins->result, AllocTemp( back, type_class ),
                                   type_class, ins->operands[non_ind]->n.type_class );
            SuffixIns( back, prev_ins, new_ins );
            prev_ins = new_ins;
        }
        new_ins = MakeBinary( back, OP_ADD, new_ins->result, ins->operands[non_ind],
                                AllocTemp( back, type_class ), type_class );
        ins->operands[non_ind] = new_ins->result;
        SuffixIns( back, prev_ins, new_ins );
        prev_ins = new_ins;
    }
    return( true );
}

static  bool    RepIndVar( BackRun& back, instruction *ins, induction *rep,
                           opcnt i_ind, opcnt i_non_ind, type_class_def type_class )
/******************************************************************************
 * One operand of "ins" needs to be replaced with "rep"
 */
{
    if( InvariantOp( back, ins->operands[i_non_ind] ) ) {
        if( !DoReplacement( back, ins, rep, i_ind, i_non_ind, type_class ) ) {
            return( false );
        }
    }
    return( true );
}


static  void    RepBoth( instruction *ins, induction *rep, type_class_def type_class )
/****************************************************************
 * Both operands of "ins" need to be replaced with "rep".
 */
{
    ins->operands[0] = rep->name;
    ins->operands[1] = rep->name;
    if( rep->times < 0 ) {
        RevCond( ins );
    }
    ins->type_class = type_class;
}


static  bool    ReplUses( BackRun& back, induction *var, induction *rep,
                          instruction *ins, type_class_def type_class )
/*******************************************************************
 * Replace all uses of induction variable "var" with "rep" in
 * instruction "ins".
 */
{
    iv_usage    op1use;
    iv_usage    op2use;

    if( ins->head.opcode == OP_CMP_EQUAL
      && !DangerousTypeChange( var, rep ) ) {
        op1use = Uses( back, ins->operands[0], var->name ); /* UNUSED | USED_AS_OP*/
        op2use = Uses( back, ins->operands[1], var->name ); /* UNUSED | USED_AS_OP*/
        if( op1use == IVU_USED_AS_OPERAND ) {
            if( op2use == IVU_USED_AS_OPERAND ) {
                RepBoth( ins, rep, type_class );
            } else if( op2use == IVU_USED_AS_INVARIANT ) {
                if( !RepIndVar( back, ins, rep, 0, 1, type_class ) ) {
                    return( false );
                }
            }
        } else if( op2use == IVU_USED_AS_OPERAND ) {
            if( op1use == IVU_USED_AS_INVARIANT ) {
                if( !RepIndVar( back, ins, rep, 1, 0, type_class ) ) {
                    return( false );
                }
            }
        }
        return( true );
    }
    return( false );
}


static  void    Replace( BackRun& back, induction *var, induction *replacement )
/******************************************************************
 * Replace all occurences of "var" with "replacement" in conditional
 * loop exit instructions, provided that these are the only places that
 * "var" is used.  Also, if we can calculate what the final value of
 * "var" will be based on the loop exit condition, etc, put an
 * assignment of the final value following the loop.  This might cause
 * all updates of the variable within the loop to become obsolete.
 */
{
    block               *blk;
    instruction         *ins;
    instruction         *cond;
    block               *cond_blk;
    bool                onecond;
    type_class_def      type_class;
    bool                free_ins;

    if( !AnalyseLoop( back, var, &onecond, &cond, &cond_blk ) )
        return;
    type_class = replacement->type_class;
    free_ins = false;
    if( onecond ) {
        if( FinalValue( back, cond, cond_blk, var ) ) {
            free_ins = true;
        }
    }
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( OpIsCompare( ins->head.opcode ) ) {
                if( !ReplUses( back, var, replacement, ins, type_class ) ) {
                    return;
                }
            }
        }
    }
    if( free_ins ) {
        FreeIns( back, var->ins );
    }
    SetVarState( var, IV_DEAD );
}


static  bool    DoLoopInvariant( BackRun& back, bool(*rtn)(BackRun&) )
/*************************************************
 * Do loop invariant code motion from the outside in.  First haul any
 * invariant expressions out of the innermost loops, then the next
 * level, etc, so that an invariant expression gets pulled out of all
 * loops with a minimum number of passes.
 */
{
    level_depth         max_depth;
    level_depth         depth;
    block               *blk;
    bool                change;

    change = false;
    max_depth = MaxDepth(back);
    /*
     * do loop invariant code motion from the outside in
     */
    for( depth = 1; depth <= max_depth; ++depth ) {
        for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
            if( IsBlkAttr( blk, BLK_LOOP_HEADER )
              && blk->depth == depth ) {
                LPBlip();
                back.loopopts.Head = blk;
                MarkLoop(back);
                back.loopopts_file->LoopProtected = false;
                MarkInvariants(back);
                while( rtn(back) ) {
                    change = true;
                }
                UnMarkInvariants(back);
                UnMarkLoop(back);
            }
        }
    }
    return( change );
}


static  bool    FindRegInvariants( BackRun& back )
/****************************************
 * Find any invariant, hoistable expressions in "Loop", and
 * pre-calculate the expression in the loop pre-header, putting the
 * result into a tempory and replaceing the calculation within the loop
 * with a reference to that temporary.  Also, if there are any
 * invariant operands whose pre-loop values can be calculated,
 * propagate the constant values into the loop.  The copy propagator in
 * CSE does not handle this so we do it here.
 */
{
    block       *blk;
    instruction *ins;
    instruction *next;
    opcnt       i;
    bool        change;

    change = false;
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
            next = ins->head.next;
            if( ins->result == NULL )
                continue;
            if( ins->result->n.class_ != N_REGISTER )
                continue;
            if( HwOvlap( ins->result->r.reg, back.loopopts.Head->ins.head.next->head.live.regs ) )
                continue;
            if( ins->result->r.reg_index != 1 )
                continue;
            if( SideEffect( back, ins ) )
                continue;
            if( OpIsCall( ins->head.opcode ) )
                continue;
            if( OpIsCondition( ins->head.opcode ) )
                continue;
            for( i = 0; i < ins->num_operands; ++i ) {
                if( !HW_CEqual( ins->zap->reg, HW_EMPTY ) )
                    break;
                if( InvariantReg( ins->operands[i] ) )
                    continue;
                if( !InvariantOp( back, ins->operands[i] ) ) {
                    break;
                }
            }
            if( i != ins->num_operands )
                continue;
            next->head.prev = ins->head.prev;
            ins->head.prev->head.next = next;
            SuffixPreHeader( back, ins );
            UpdateLoopLiveInfo( back, ins );
            change = true;
        }
    }
    return( change );
}


static  bool    FindInvariants( BackRun& back )
/*************************************
 * Find any invariant, hoistable expressions in "Loop", and
 * pre-calculate the expression in the loop pre-header, putting the
 * result into a tempory and replacing the calculation within the loop
 * with a reference to that temporary.  Also, if there are any
 * invariant operands whose pre-loop values can be calculated,
 * propagate the constant values into the loop.  The copy propagator in
 * CSE does not handle this so we do it here.
 */
{
    block       *blk;
    instruction *ins;
    instruction *next;
    name        *op;
    name        *temp;
    opcnt       i;
    bool        change;

    change = false;
    for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
            next = ins->head.next;
            if( InvariantExpr( back, ins, blk ) ) {
                next = ins->head.next;
                next->head.prev = ins->head.prev;
                ins->head.prev->head.next = next;
                op = ins->result;
                if( ( op->n.class_ == N_TEMP
                  || op->n.class_ == N_MEMORY )
                  && ChkLoopUsage( op, VU_VARIED_ONCE )
                  && (op->v.usage & USE_IN_ANOTHER_BLOCK) == 0
                  && !back.data.BlockByBlock ) {
                    SuffixPreHeader( back, ins );
                    if( op->n.class_ == N_TEMP ) {
                        op->t.temp_flags |= ( CROSSES_BLOCKS | ONE_DEFINITION );
                    }
                    SetLoopUsage( op, VU_INVARIANT );
                    FPNotStack( op );
                } else {
                    temp = AllocTemp( back, op->n.type_class );
                    temp->n.size = op->n.size;
                    temp->t.temp_flags |= ( CROSSES_BLOCKS | ONE_DEFINITION );
                    ins->result = temp;
                    SuffixPreHeader( back, ins );
                    PrefixIns( back, next, MakeMove( back, temp, op, op->n.type_class ) );
                }
                change = true;
            } else if( ins->head.opcode != OP_CALL_INDIRECT ) {
                /*
                 * propagate some constants into the loop
                 */
                for( i = 0; i < ins->num_operands; ++i ) {
                    op = ins->operands[i];
                    if( !InvariantOp( back, op ) )
                        continue;
                    op = InitialValue( back, op );
                    if( op == NULL )
                        continue;
                    /*
                     * don't propagate a LEA into the loop!
                     */
                    if( op->c.const_type == CONS_TEMP_ADDR )
                        continue;
                    if( op->c.const_type == CONS_HIGH_ADDR )
                        continue;
                    ins->operands[i] = op;
                    change = true;
                }
            }
        }
    }
    return( change );
}


bool    LoopInvariant( BackRun& back )
/***************************/
{
    return( DoLoopInvariant( back, &FindInvariants ) );
}


bool    LoopRegInvariant( BackRun& back )
/******************************/
{
    return( DoLoopInvariant( back, &FindRegInvariants ) );
}


void    LoopEnregister( BackRun& back )
/****************************/
{
    level_depth         depth;
    block               *blk;

    /*
     * do loop enregistering from the inside out
     */
    for( depth = MaxDepth(back); depth >= 1; --depth ) {
        for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
            if( IsBlkAttr( blk, BLK_LOOP_HEADER )
              && blk->depth == depth ) {
                back.loopopts.Head = blk;
                MarkLoop(back);
                ConstToTemp( back, back.loopopts.PreHead, back.loopopts.Loop, NextInLoop );
                UnMarkLoop(back);
            }
        }
    }
}


static  void    ElimIndVars( BackRun& back )
/**********************************
 * Eliminate induction variables whose only use in the loop is in
 * comparisons and the single induction instruction by replacing it
 * with another induction variable of it's family in the compare
 * instructions.
 */
{
    induction   *next;
    induction   *var;
    induction   *replacement;

    for( var = back.loopopts.IndVarList; var != NULL; var = next ) {
        next = var->next;
        if( LacksVarState( var, IV_BASIC | IV_INTRODUCED ) ) {
            SetVarState( var, IV_DEAD );
        }
    }
    for( var = back.loopopts.IndVarList; var != NULL; var = next ) {
        next = var->next;
        if( LacksVarState( var, IV_NOREPLACE )
          && LacksVarState( var, IV_DEAD )
          && HasVarState( var, IV_BASIC ) ) {
            replacement = FindReplacement( back, var );
            if( replacement != NULL ) {
                Replace( back, var, replacement );
            }
        }
    }
}


static  bool    ReduceVar( BackRun& back, induction *var )
/******************************************
 * Do strength reduction on induction variable "var", if it is current
 * to this loop, and the top, or end result, of an induction
 * expression.  Once strength reduction is done on the "top" of an
 * induction expression, the intermediate induction variables drop away
 * since they will no longer be used.  Strength reduction involves
 * replacing all occurences of the induction variable in a loop with a
 * new induction variable (IV_INTRODUCED) which is initialized at the
 * top of the loop and has some appropriate value added to it where the
 * basic induction variable gets updated.  For example
 *     for( i = 0; i < 10; ++i ) x[i] = 0;
 * becomes
 *     for( i = 0, p = &i[0]; p < &i[10]; i++, p++ ) *p = 0;
 * If "i" is not used after the loop, all references to it in the loop
 * will be removed.
 */
{
    induction           *alias;
    name                *new_temp;
    type_class_def      type_class;
    induction           *new_ind;
    instruction         *ins;

    if( LacksVarState( var, IV_TOP ) )
        return( false );
    if( HasVarState( var, IV_DEAD ) )
        return( false );
    while( var->ins->num_operands == 1 ) {
        var = FindIndVar( back, var->ins->operands[0] );
    }
    if( HasVarState( var, IV_BASIC | IV_ALIAS ) )
        return( false );
    if( IsAddressMode( var ) )
        return( false );
    type_class = var->type_class;
    new_temp = AllocTemp( back, type_class );
    new_temp->t.temp_flags |= CROSSES_BLOCKS;
    ins = MakeMove( back, new_temp, var->name, type_class );
    ReplIns( back, var->ins, ins );
    var->ins = ins;
    IncAndInit( back, var, new_temp, type_class );
    for( alias = var->alias; alias != var; alias = alias->alias ) {
        if( NoPathThru( back, var->ins, alias->ins, var->basic->ins )
          && (alias->name->v.usage & USE_IN_ANOTHER_BLOCK) == 0
          && (var->name->v.usage & USE_IN_ANOTHER_BLOCK) == 0 ) {
            ReplaceOccurences( back, alias->name, var->name );
            ins = MakeMove( back, new_temp, alias->name, type_class );
            PrefixIns( back, alias->ins, ins );
            ins = MakeMove( back, new_temp, var->name, type_class );
        } else {
            ins = MakeMove( back, new_temp, alias->name, type_class );
        }
        ReplIns( back, alias->ins, ins );
        alias->ins = ins;
        /*
         * why?
         */
//        ins = MakeMove( new_temp, alias->name, type_class );
//        SuffixPreHeader( ins );
    }
    new_ind = AddIndVar( back, var->basic->ins->head.prev, new_temp,
                     var->basic, var->invar, var->ivtimes,
                     var->lasttimes, var->times,
                     var->plus, var->plus2, 1, ins->type_class );
    SetVarState( new_ind, IV_INTRODUCED );
    return( true );
}


static  bool    ReduceInStrength( BackRun& back )
/***************************************
 * For each induction variable, see if we can reduce it in strength.
 */
{
    induction   *var;
    bool        change;

    change = false;
    for( var = back.loopopts.IndVarList; var != NULL; var = var->next ) {
        change |= ReduceVar( back, var );
    }
    return( change );
}



static  bool    TwistLoop( BackRun& back, block_list *header_list, bool unroll )
/****************************************************************
 * Try to fall into loops without testing the condition the first
 * time if we know how many times the loop will go around.  We
 * need a loop which goes around more than once, whose preheader
 * jumps straight to the condition which in turn jumps into or
 * out of the loop.  We may be recursed several levels deep in
 * DoInduction here.  header_list is the list of all loops we've
 * recursed through.  If we replace a block which is on this
 * list, we have to update the pointer in "header_list".
 */
{
    instruction         *cond;
    instruction         *dupcond;
    block               *cond_blk;
    block_edge          *exit_edge;
    block_edge          *loop_edge;
    bool                onecond;
    int_32              final;
    int_32              initial;
    induction           *var;
    block_edge          *edge;
    bool                know_bounds;
    bool                do_the_twist;
    block_list          *list;
    block               *old_prehead;
    block               *old_head;
    block               *new_head;
    block               *blk;

    AnalyseLoop( back, back.loopopts.IndVarList, &onecond, &cond, &cond_blk );
    if( cond_blk == NULL )
        return( false );
    know_bounds = false;
    if( onecond ) {
        var = FindIndVar( back, cond->operands[0] );
        if( var == NULL )
            var = FindIndVar( back, cond->operands[1] );
        if( var != NULL ) {
            if( CalcFinalValue( back, var, cond_blk, cond, &final, &initial ) ) {
                if( final != initial ) {
                    know_bounds = true;
                }
            }
        }
    }
    loop_edge = &cond_blk->edge[0];
    exit_edge = &cond_blk->edge[1];
    if( !IsBlkAttr( loop_edge->destination.u.blk, BLK_IN_LOOP ) ) {
        loop_edge = &cond_blk->edge[1];
        exit_edge = &cond_blk->edge[0];
    }
    if( unroll
      && IsBlkAttr( back.loopopts.Head, BLK_ITERATIONS_KNOWN )
      && back.loopopts.Head->iterations == 1 ) {
        if( cond_blk != back.loopopts.Head
          || back.loopopts.Loop->u.loop == NULL ) {
            exit_edge->flags |= BEF_ONE_ITER_EXIT;
        }
    }
    if( unroll ) {
        old_head = back.loopopts.Head;
        if( UnRoll(back) ) {
            if( back.loopopts.Head != old_head ) {
                /*
                 * we were beheaded!
                 */
                for( list = header_list; list != NULL; list = list->next ) {
                    if( list->blk == old_head ) {
                        list->blk = back.loopopts.Head;
                    }
                }

            }
            for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
                if( IsBlkAttr( blk, BLK_IN_LOOP ) ) {
                    PropIndexes( back, blk );
                }
            }
            return( true );
        }
    }
    do_the_twist = true;
    edge = &back.loopopts.PreHead->edge[0];
    if( edge->destination.u.blk != cond_blk ) {
        do_the_twist = false;
    }
    if( cond != cond_blk->ins.head.next
      || cond != cond_blk->ins.head.prev ) {
        do_the_twist = false;
    }
    if( do_the_twist ) {
        new_head = NULL;
        if( know_bounds ) {
            RemoveInputEdge( edge );
            PointEdge( edge, loop_edge->destination.u.blk );
            MoveDownLoop( back, cond_blk );
            new_head = loop_edge->destination.u.blk;
//            DupNoncondInstrs( cond_blk, cond, PreHead );
        } else if( back.data.OptForSize < 50
          && back.loopopts.PreHead->gen_blk_id < cond_blk->gen_blk_id ) {
            RemoveInputEdge( edge );
            old_prehead = back.loopopts.PreHead;
            back.loopopts.PreHead = ReGenBlock( back, back.loopopts.PreHead, NULL );
            for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
                if( blk->loop_head == old_prehead ) {
                    blk->loop_head = back.loopopts.PreHead;
                }
            }
            for( list = header_list; list != NULL; list = list->next ) {
                if( list->blk == old_prehead ) {
                    list->blk = back.loopopts.PreHead;
                }
            }
//            DupNoncondInstrs( cond_blk, cond, PreHead );
            MarkBlkAttrClr( back.loopopts.PreHead, BLK_JUMP );
            MarkBlkAttrSet( back.loopopts.PreHead, BLK_CONDITIONAL );
            dupcond = MakeCondition( back, cond->head.opcode, cond->operands[0],
                                     cond->operands[1], TrueIndex( cond ),
                                     FalseIndex( cond ), cond->type_class );
            SuffixPreHeader( back, dupcond );
            edge = &back.loopopts.PreHead->edge[0];
            edge->flags = BEF_DEST_IS_BLOCK;
            PointEdge( edge, cond_blk->edge[0].destination.u.blk );
            edge = &back.loopopts.PreHead->edge[1];
            edge->flags = BEF_DEST_IS_BLOCK;
            PointEdge( edge, cond_blk->edge[1].destination.u.blk );
            MoveDownLoop( back, cond_blk );
            new_head = loop_edge->destination.u.blk;
        }
        if( new_head != NULL
          && new_head != back.loopopts.Head ) {
            for( blk = back.loopopts.Loop; blk != NULL; blk = blk->u.loop ) {
                if( blk->loop_head == back.loopopts.Head ) {
                    blk->loop_head = new_head;
                }
            }
            new_head->loop_head = back.loopopts.Head->loop_head;
            back.loopopts.Head->loop_head = new_head;
            MarkBlkAttrClr( back.loopopts.Head, BLK_LOOP_HEADER );
            MarkBlkAttrSet( new_head, BLK_LOOP_HEADER );
            back.loopopts.Head = new_head;
        }
    }
    if( do_the_twist
      || know_bounds ) {
        UnMarkLoop(back);
        MarkLoop(back);
        MarkWillExecBlocks(back);
        back.loopopts_file->LoopProtected = true;
        MarkInvariants(back);
        while( FindInvariants(back) )
            ;
        UnMarkInvariants(back);
        UnMarkWillExecBlocks(back);
        return( true );
    }
    return( false );
}


static  bool    DoInduction( BackRun& back, block_list *header, bool reduce, bool unroll )
/**************************************************************************
 * Process induction variables in the loop whose loop header is
 * "header".  We do any loops contained inside this loop before
 * we do this loop, since by the strict definition of an
 * induction variable, an induction variable in a loop is also an
 * induction variable any loop containing that loop.  For
 * practical purposes though, once an induction variable is
 * processed for its "natural" loop (the deepest one) we don't
 * consider it for any outer loops.  "header" points to a linked
 * list of pointers to blocks.  These are, the current loop
 * header, its loop header, etc ...
 */
{
    block_list  curr_block;
    int         old;
    bool        change;
    induction   **owner;
    induction   *list;

    change = false;
    list = NULL;
    owner = &list;
    /*
     * do all loops inside this loop separately
     */
    for( curr_block.blk = back.data.HeadBlock; curr_block.blk != NULL; curr_block.blk = curr_block.blk->next_block ) {
        if( IsBlkAttr( curr_block.blk, BLK_LOOP_HEADER )
          && curr_block.blk->loop_head == header->blk ) {
            curr_block.next = header;
            change |= DoInduction( back, &curr_block, reduce, unroll );
            while( *owner != NULL ) {   // hook inner indvars onto "list"
                owner = &(*owner)->next;
            }
            *owner = back.loopopts.IndVarList;
            back.loopopts.IndVarList = NULL;
        }
    }
    /*
     * now do this loop, BUT, don't consider any induction
     * variables from the inner loops
     */
    back.loopopts.IndVarList = list;
    if( header->blk != NULL ) {
        LPBlip();
        while( list != NULL ) {
            SetVarState( list, IV_DEAD );
            list = list->next;
        }
        back.loopopts.Head = header->blk;
        back.loopopts_file->NumIndVars = 0;
        MarkLoop(back);
        FindBasics(back);
        if( !reduce
          || back.loopopts_file->NumIndVars == 0 ) {
            if( !back.data.BlockByBlock
              && TwistLoop( back, header, unroll ) ) {
                change = true;
            }
        } else {
            back.loopopts_file->LoopProtected = false;
            MarkInvariants(back);
            for( ;; ) {
                old = back.loopopts_file->NumIndVars;
                FindNonBasics(back);
                if( old == back.loopopts_file->NumIndVars ) {
                    break;
                }
            }
            MergeVars(back);
            if( ReduceInStrength(back) ) {
                change = true;
                MarkBlkAttrSet( back.loopopts.PreHead, BLK_IN_LOOP );
                LoopInsDead(back);
                MarkBlkAttrClr( back.loopopts.PreHead, BLK_IN_LOOP );
            }
            FreeBadVars(back);
            ElimIndVars(back);
            if( !back.data.BlockByBlock
              && TwistLoop( back, header, unroll ) ) {
                change = true;
            }
            UnMarkInvariants(back);
        }
        UnMarkLoop(back);
    }
    return( change );
}


static  bool    Induction( BackRun& back, bool reduce, bool unroll )
/****************************************************
 * Do induction variable processing
 */
{
    block_list  list;
    bool        change;

    list.blk = NULL;
    list.next = NULL;
    InitIndVars(back);
    change = DoInduction( back, &list, reduce, unroll );
    FiniIndVars(back);
    if( unroll ) {
        change |= KillOneTrippers(back);
    }
    return( change );
}


bool    IndVars( BackRun& back )
/*********************/
{
    return( Induction( back, true, false ) );
}


bool    TransLoops( BackRun& back, bool unroll )
/*******************************/
{
    return( Induction( back, false, unroll ) );
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
LoopoptsFileState* loopopts_file_make() { return new LoopoptsFileState(); }
void loopopts_file_free(LoopoptsFileState* part) { delete part; }
