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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "score.h"
#include "backrun.hpp"


void    RegInsert( BackRun& back, score_hdl scoreboard, int dst_idx, int src_idx )
/*****************************************************************/
{
    score_hdl   dst;
    score_hdl   src;
    score_hdl   next;
    byte        gen_num;
    list_head   **free_heads;

    src = &scoreboard[src_idx];
    dst = &scoreboard[dst_idx];
    ScoreFreeList( back, dst );
    free_heads = (list_head **)&scoreboard[back.score.ScoreCount];
    *dst->list = (list_head)*free_heads;
    *free_heads = dst->list;
    next = dst;
    do {
        next->list = src->list;
        next = next->next_reg;
    } while( next != dst );
    gen_num = 0;
    next = src;
    do {
        ++gen_num;
        next = next->next_reg;
    } while( next != src );
    dst->generation = gen_num;
    src->prev_reg->next_reg = dst;
    dst->prev_reg->next_reg = src;
    next = src->prev_reg;
    src->prev_reg = dst->prev_reg;
    dst->prev_reg = next;
}


bool    RegsEqual( score_hdl scoreboard, int i1, int i2 )
/*******************************************************/
{
    return( scoreboard[i1].list == scoreboard[i2].list );
}


static void     MergeDown( BackRun& back, score_hdl scoreboard, int dst_idx, int src_idx )
/*************************************************************************/
{
    int dst_hi;
    int src_hi;
    int dst_lo;
    int src_lo;

    dst_hi = back.score.ScoreList[dst_idx]->high;
    src_hi = back.score.ScoreList[src_idx]->high;
    if( dst_hi != NO_INDEX && src_hi != NO_INDEX ) {
        if( !RegsEqual( scoreboard, src_hi, dst_hi ) ) {
            RegInsert( back, scoreboard, dst_hi, src_hi );
            MergeDown( back, scoreboard, dst_hi, src_hi );
        }
    }
    src_lo = back.score.ScoreList[src_idx]->low;
    dst_lo = back.score.ScoreList[dst_idx]->low;
    if( dst_lo != NO_INDEX && src_lo != NO_INDEX ) {
        if( !RegsEqual( scoreboard, src_lo, dst_lo ) ) {
            RegInsert( back, scoreboard, dst_lo, src_lo );
            MergeDown( back, scoreboard, dst_lo, src_lo );
        }
    }
}


static  void    MergeUp( BackRun& back, score_hdl scoreboard, int dst_idx, int src_idx )
/***********************************************************************/
{
    int dst_hi;
    int src_hi;
    int dst_lo;
    int src_lo;

    dst_hi = back.score.ScoreList[dst_idx]->high_of;
    src_hi = back.score.ScoreList[src_idx]->high_of;
    dst_lo = back.score.ScoreList[dst_idx]->low_of;
    src_lo = back.score.ScoreList[src_idx]->low_of;
    if( dst_hi != NO_INDEX && src_hi != NO_INDEX ) {
        dst_lo = back.score.ScoreList[dst_hi]->low;
        src_lo = back.score.ScoreList[src_hi]->low;
        if( dst_lo != NO_INDEX && src_lo != NO_INDEX
         && RegsEqual( scoreboard, dst_lo, src_lo )
         && !RegsEqual( scoreboard, dst_hi, src_hi ) ) {
            RegInsert( back, scoreboard, dst_hi, src_hi );
            MergeUp( back, scoreboard, dst_hi, src_hi );
        }
    } else if( dst_lo != NO_INDEX && src_lo != NO_INDEX ) {
        dst_hi = back.score.ScoreList[dst_lo]->high;
        src_hi = back.score.ScoreList[src_lo]->high;
        if( dst_hi != NO_INDEX && src_hi != NO_INDEX
         && RegsEqual( scoreboard, dst_hi, src_hi )
         && !RegsEqual( scoreboard, dst_lo, src_lo ) ) {
            RegInsert( back, scoreboard, dst_lo, src_lo );
            MergeUp( back, scoreboard, dst_lo, src_lo );
        }
    }
}


void    RegKill( BackRun& back, score_hdl scoreboard, hw_reg_set regs )
/******************************************************/
{
    score_reg   *entry;
    score_list  *curr_list;
    score_list  **owner;
    int         i;
    list_head   **free_heads;

    if( !HW_CEqual( regs, HW_EMPTY ) ) {
        entry = *back.score.ScoreList;
        free_heads = (list_head **)&scoreboard[back.score.ScoreCount];
        for( i = back.score.ScoreCount; i > 0; --i ) {
            if( HW_Ovlap( entry->reg, regs ) ) {
                if( scoreboard->list != NULL ) {
                    if( scoreboard->next_reg == scoreboard ) {
                        ScoreFreeList( back, scoreboard );
                    } else {
                        scoreboard->list = *free_heads;
                        *free_heads = (list_head *)**free_heads;
                        *scoreboard->list = NULL;
                    }
                }
                scoreboard->prev_reg->next_reg = scoreboard->next_reg;
                scoreboard->next_reg->prev_reg = scoreboard->prev_reg;
                scoreboard->next_reg = scoreboard;
                scoreboard->prev_reg = scoreboard;
                scoreboard->generation = 0;
            } else {
                for( owner = scoreboard->list; (curr_list = *owner) != NULL; ) {
                    if( curr_list->info.reg_index != NO_INDEX
                      && HW_Ovlap( back.score.ScoreList[curr_list->info.reg_index]->reg, regs ) ) {
                        *owner = curr_list->next;
                        FreeScListEntry( back, curr_list );
                    } else {
                        owner = &curr_list->next;
                    }
                }
            }
            ++entry;
            ++scoreboard;
        }
    }
}


void    RegAdd( BackRun& back, score_hdl scoreboard, int dst_idx, int src_idx )
/**************************************************************/
/* NB: it is important that dst_idx has just become equal to src_idx*/
/*     NOT vice-versa. Ie: we just did a  MOV R(src_idx) ==> R(dst_idx)*/
/*     or equivalent*/
{
    if( back.score.ScoreList[dst_idx]->size != back.score.ScoreList[src_idx]->size )
        return;
    if( !ScAddOk( back, back.score.ScoreList[dst_idx]->reg, back.score.ScoreList[src_idx]->reg ) )
        return;
    if( RegsEqual( scoreboard, dst_idx, src_idx ) )
        return;
    MergeDown( back, scoreboard, dst_idx, src_idx );
    MergeUp( back, scoreboard, dst_idx, src_idx );
    RegInsert( back, scoreboard, dst_idx, src_idx );
}
