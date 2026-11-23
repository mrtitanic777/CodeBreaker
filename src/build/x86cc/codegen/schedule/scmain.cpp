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
* Description:  Scoreboard entire routine.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "score.h"
#include "freelist.h"
#include "spawn.h"
#include "memout.h"
#include "data.h"
#include "utils.h"
#include "stackcg.h"
#include "nullprop.h"
#include "generate.h"
#include "scmain.h"
#include "liveinfo.h"
#include "memmgt.h"
#include "backrun.hpp"


void    ScoreInit( BackRun& back )
/***********************/
{
    InitFrl( &back.score.ScListFrl );
}

void    ScoreFini( void )
/***********************/
{
}


static  void    ScoreSeed( BackRun& back, block *blk, block *son, byte index )
/**************************************************************
    If block ends with a single comparison for == or !=, then the
    one of the sons (true index for ==, false index for !=) knows
    that the two operands are in fact equal.
*/
{
    instruction     *cmp;
    byte            which;

    if( !_IsBlkAttr( blk, BLK_CONDITIONAL ) )
        return;
    if( _IsBlkAttr( blk, BLK_MULTIPLE_EXITS ) )
        return;
    for( cmp = blk->ins.head.prev; cmp->head.opcode == OP_NOP; ) {
        cmp = cmp->head.prev;
    }
    switch( cmp->head.opcode ) {
    case OP_CMP_EQUAL:
        which = _TrueIndex( cmp );
        break;
    case OP_CMP_NOT_EQUAL:
        which = _FalseIndex( cmp );
        break;
    default:
        return;
    }
    if( which != index )
        return;
    ScoreMakeEqual( back, son->u1.scoreboard, cmp->operands[0], cmp->operands[1] );
}


static  void    CopyList( BackRun& back, score_hdl frm, score_hdl to,
                          list_head **sc_heads, int i )
/*****************************************************/
{
    score_list  *first;
    score_list  *new_sc;
    score_hdl   next;

    if( to[i].list == NULL ) {
        to[i].list = *sc_heads;
        *sc_heads = (list_head *)**sc_heads;
        *to[i].list = NULL;
        for( next = to[i].next_reg; next->list == NULL; next = next->next_reg ) {
            next->list = next->prev_reg->list;
         }
    }
    if( *to[i].list == NULL ) {
        next = to[i].next_reg;
        for( first = *frm[i].list; first != NULL; first = first->next ) {
            new_sc = NewScListEntry(back);
            Copy( &first->info, &new_sc->info, sizeof( score_info ) );
            new_sc->next = *next->list;
            *next->list = new_sc;
        }
    }
}


static  void    ScoreCopy( BackRun& back, score_hdl other_scoreboard, score_hdl scoreboard )
/***************************************************************************/
{
    list_head   **sc_heads;
    int         i;

    FreeScoreBoard( back, scoreboard );
    sc_heads = (list_head **)&scoreboard[back.score.ScoreCount];
    for( i = back.score.ScoreCount; i-- > 0; ) {
        scoreboard[i].next_reg = &scoreboard[other_scoreboard[i].next_reg->index];
        scoreboard[i].prev_reg = &scoreboard[other_scoreboard[i].prev_reg->index];
        scoreboard[i].generation = other_scoreboard[i].generation;
        scoreboard[i].list = NULL;
        *sc_heads = (list_head *)sc_heads + 1;
        ++sc_heads;
    }
    *sc_heads = NULL;
    sc_heads = (list_head **)&scoreboard[back.score.ScoreCount];
    for( i = back.score.ScoreCount; i-- > 0; ) {
        CopyList( back, other_scoreboard, scoreboard, sc_heads, i );
    }
}


static  void *ScoreDescendants( BackRun& back, block *blk )
/******************************************/
{
    block_num   i;
    block       *son;
    hw_reg_set  regs;

    for( i = blk->targets; i-- > 0; ) {
        son = blk->edge[i].destination.u.blk;
        if( ( son->inputs == 1 ) && !_IsBlkVisited( son ) ) {
            son->u1.scoreboard = static_cast<score_hdl>(ScAlloc( back, back.score.ScoreCount * sizeof( score ) + ( back.score.ScoreCount + 1 ) * sizeof( list_head ) ));
            ScoreClear( back, son->u1.scoreboard );
            for( ;; ) {
                ScoreCopy( back, blk->u1.scoreboard, son->u1.scoreboard );
                ScoreSeed( back, blk, son, i );
                if( !DoScore( back, son ) )
                    break;
                UpdateLive( back, son->ins.head.next, son->ins.head.prev );
            }
            _MarkBlkVisited( son );
            _MarkBlkMarked( son );
            SafeRecurseCG( back, (func_sr)ScoreDescendants, son );
            FreeScoreBoard( back, son->u1.scoreboard );
            ScFree( back, son->u1.scoreboard );
            son->u1.scoreboard = NULL;
        }
    }
    HW_CAsgn( regs, HW_EMPTY );
    for( i = blk->targets; i-- > 0; ) {
        son = blk->edge[i].destination.u.blk;
        if( _IsBlkMarked( son ) ) {
            HW_TurnOn( regs, son->ins.head.next->head.live.regs );
            _MarkBlkUnMarked( son );
        }
    }
    HW_TurnOn( blk->ins.head.live.regs, regs );
    UpdateLive( back, blk->ins.head.next, blk->ins.head.prev );
    return( NULL );
}


static  void    InitZero( BackRun& back )
/******************************/
/* Must be allocd. Could be modified by ScoreAssign but that's ok */
/* since it will just set offset to 0 */
{
    back.score.ScZero = static_cast<score_info *>(ScAlloc( back, sizeof( score_info ) ));
    back.score.ScZero->class_       = SC_N_CONSTANT;
    back.score.ScZero->offset      = 0;
    back.score.ScZero->symbol.u.p  = NULL;
    back.score.ScZero->reg_index   = NO_INDEX;
    back.score.ScZero->base        = NULL;
}


static  void    ScoreRoutine( BackRun& back )
/**********************************/
{
    block       *blk;
//    bool        change;

    ScoreCalcList(back);
    if( back.score.ScoreCount != 0 ) {
        InitZero(back);
        MarkBlkAllUnVisited( back );
        MakeLiveInfo(back);
//        change = false;
        for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
            if( !_IsBlkVisited( blk ) ) {
                blk->u1.scoreboard = static_cast<score_hdl>(ScAlloc( back, back.score.ScoreCount * sizeof( score ) + ( back.score.ScoreCount + 1 ) * sizeof( list_head ) ));
                ScoreClear( back, blk->u1.scoreboard );
                for( ;; ) {
                    FreeScoreBoard( back, blk->u1.scoreboard );
                    ScInitRegs( back, blk->u1.scoreboard );
                    if( !DoScore( back, blk ) )
                        break;
                    UpdateLive( back, blk->ins.head.next, blk->ins.head.prev );
                }
                _MarkBlkVisited( blk );
                ScoreDescendants( back, blk );
                FreeScoreBoard( back, blk->u1.scoreboard );
                ScFree( back, blk->u1.scoreboard );
                blk->u1.scoreboard = NULL;
            }
        }
    }
}


static  void    CleanUp( BackRun& back )
/*****************************/
{
    block       *blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        FreeScoreBoard( back, blk->u1.scoreboard );
        ScFree( back, blk->u1.scoreboard );
        blk->u1.scoreboard = NULL;
    }
    ScFree( back, back.score.ScoreList );
    ScFree( back, back.score.ScZero );
}


static  void    ConstSizes( BackRun& back )
/********************************/
{
    name        *cons;

    for( cons = back.data.Names[N_CONSTANT]; cons != NULL; cons = cons->n.next_name ) {
        if( cons->c.const_type == CONS_ABSOLUTE ) {
            if( CFIsU16( cons->c.u.cfval ) ) {
                cons->n.size = 2;
            } else {
                cons->n.size = 4;
            }
        }
    }
}


void    Score( BackRun& back )
/*******************/
{
    mem_out_action      old_memout;
    block               *blk;

    ConstSizes(back);
    old_memout = SetMemOut( back, MO_SUICIDE );
    back.score.ScZero = NULL;               /* in case of Suicide*/
    back.score.ScoreList = NULL;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        blk->u1.scoreboard = NULL;
    }
    if( Spawn( back, &ScoreRoutine ) != 0 ) {
        FEMessageCurrProc( back, FEMSG_SCOREBOARD_DIED );
    }
    CleanUp(back);
    SetMemOut( back, old_memout );
}
