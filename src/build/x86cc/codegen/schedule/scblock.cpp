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
* Description:  Register scoreboarding on a basic block.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "score.h"
#include "zerobits.h"
#include "makeins.h"
#include "blips.h"
#include "insdead.h"
#include "optab.h"
#include "confldef.h"
#include "liveinfo.h"
#include "split.h"
#include "backrun.hpp"


void    FreeJunk( BackRun& back, block *blk )
/*************************************
 * Free instructions which aren't going to be generated.
 */
{
    instruction *ins;
    instruction *next;

    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
        next = ins->head.next;
        if( !DoesSomething( ins ) && !SideEffect( back, ins )
         && ins->head.opcode < FIRST_OP_WITH_LABEL
         && ins->head.opcode != OP_NOP ) { /*% there for zap info*/
            FreeIns( back, ins );
        }
    }
}

static bool StupidMove( BackRun& back, score_hdl scoreboard, instruction *ins )
/**************************************************************/
{
    score_info  info;
    int         dst_index;

    if( ins->head.opcode != OP_MOV )
        return( false );
    if( ins->result->n.class_ != N_REGISTER )
        return( false );
    dst_index = ins->result->r.reg_index;
    if( ins->operands[0]->n.class_ == N_REGISTER ) {
        if( !RegsEqual( scoreboard, ins->operands[0]->r.reg_index, dst_index )  ) {
            return( false );
        }
    } else {
        ScoreInfo( back, &info, ins->operands[0] );
        if( !ScoreEqual( back, scoreboard, dst_index, &info ) ) {
            return( false );
        }
    }
    /*
     * We've seen a condition already in the
     * block, but we've got a move instruction
     * that assigns a value to the result register that the
     * register already has. Kill the sucker.
     */
    DoNothing( back, ins );
    return( true );
}


static  bool    RemDeadCode( BackRun& back, block *blk )
/****************************************
 * This removes any instructions in "blk" which assign to a register which
 * dies immediately following that instruction and has no side effects.
 * Returns true if any instructions were killed, in which case the
 * live information must be updated.
 */
{
    bool        change;
    name        *result;
    instruction *ins;
    instruction *next;

    change = false;
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
        next = ins->head.next;
        result = ins->result;
        /*
         * if result is a register and it dies after this instruction
         */
        if( !_OpIsCall( ins->head.opcode )
          && !UnChangeable( ins )
          && !SideEffect( back, ins )
          && result != NULL
          && !ScConvert( back, ins )
          && result->n.class_ == N_REGISTER
          && !HW_Ovlap( ins->head.next->head.live.regs, result->r.reg ) ) {
            FreeIns( back, ins );
            change = true;
        }
    }
    return( change );
}

bool    DoScore( BackRun& back, block *blk )
/************************************
 * Do register scoreboarding on a basic block. Remember which
 * registers contain which values as we run through the block and
 * then use that information to try to replace memory references
 * with register references.
 */
{
    instruction *next;
    score_hdl   scoreboard;
    name        *dst;
    bool        change;
    bool        had_condition;
    score_info  info;
    instruction *ins;
    hw_reg_set  tmp;

    change = false;
    for( ;; ) {
        SCBlip();
        while( RemDeadCode( back, blk ) ) {
            UpdateLive( back, blk->ins.head.next, blk->ins.head.prev );
            change = true;
        }
        if( !RegThrash( back, blk ) )
            break;
        change = true;
    }
    scoreboard = blk->u1.scoreboard;
    had_condition = false;
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
        ScoreSegments( back, scoreboard );
        /*
         * May all intel designers rot in hell forever and ever, amen
         */
        if( _OpIsCondition( ins->head.opcode ) && ins->result == NULL ) {
            if( had_condition ) {
                _MarkBlkAttrSet( blk, BLK_MULTIPLE_EXITS );
            }
            had_condition = true;
        }
        if( ScoreZero( back, scoreboard, &ins ) ) {
            change = true;
            UpdateLive( back, blk->ins.head.next, blk->ins.head.prev );
        }
        if( ins->head.opcode == OP_BLOCK )
            break;
        /*
         * ScoreZero freed the last instr!
         */
        next = ins->head.next;
        dst = ins->result;
        if( UnChangeable( ins ) ) {
            if( dst != NULL ) {
                ScoreInfo( back, &info, dst );
                ScoreKillInfo( back, scoreboard, dst, &info, HW_EMPTY );
                RegKill( back, scoreboard, ins->zap->reg );
            }
        } else if( DoesSomething( ins ) ) {
            if( ins->head.opcode == OP_CALL || ins->head.opcode == OP_CALL_INDIRECT ) {
                MemChanged( back, scoreboard,
                    ( ins->flags.u.call_flags & CALL_WRITES_NO_MEMORY ) == 0 );
                RegKill( back, scoreboard, ins->zap->reg );
                if( dst != NULL && dst->n.class_ != N_REGISTER ) {
                    ScoreInfo( back, &info, dst );
                    ScoreKillInfo( back, scoreboard, dst, &info, HW_EMPTY );
                }
            } else {
                if( FindRegOpnd( back, scoreboard, ins ) ) {
                    change = true;
                    UpdateLive( back, blk->ins.head.next, blk->ins.head.prev );
                }
                if( ins->head.opcode == OP_MOV && !had_condition ) {
                    if( ScoreMove( back, scoreboard, ins ) ) {
                        change = true;
                        UpdateLive( back, blk->ins.head.next, blk->ins.head.prev );
                    }
                    if( next->head.prev == ins ) {
                        RegKill( back, scoreboard, ins->zap->reg );
                    }
                } else if( ins->head.opcode == OP_LA && !had_condition ) {
                    if( ScoreLA( back, scoreboard, ins ) ) {
                        change = true;
                        UpdateLive( back, blk->ins.head.next, blk->ins.head.prev );
                    }
                    if( next->head.prev == ins ) {
                        RegKill( back, scoreboard, ins->zap->reg );
                    }
                } else if( dst == NULL ) {
                    RegKill( back, scoreboard, ins->zap->reg );
                } else if( StupidMove( back, scoreboard, ins ) ) {
                    change = true;
                } else {
                    if( dst->n.class_ == N_REGISTER ) {
                        tmp = ins->zap->reg;
                        HW_TurnOn( tmp, dst->r.reg );
                        RegKill( back, scoreboard, tmp );
                    } else {
                        ScoreInfo( back, &info, dst );
                        ScoreKillInfo( back, scoreboard, dst, &info, HW_EMPTY );
                        RegKill( back, scoreboard, ins->zap->reg );
                    }
                    if( !had_condition ) {
                        ScZeroCheck( back, scoreboard, ins );
                    }
                }
            }
        } else {
            if( ins->head.opcode == OP_NOP && dst != NULL ) {
                ScoreInfo( back, &info, dst );
                ScoreKillInfo( back, scoreboard, dst, &info, HW_EMPTY );
            }
            RegKill( back, scoreboard, ins->zap->reg );
        }
    }
    return( change );
}


byte    HasZero( BackRun& back, score_hdl scoreboard, name *n )
/***********************************************
 * given a scoreboard "sc", determine if name "n" is equal to
 * zero or has any portions which are equal to zero. This is recursive
 * since a regisiter like EAX on the 386 has pieces AX, AH, and AL.
 */
{
    byte        bits;
    int         i;
    int         hi;
    int         lo;

    bits = 0;
    if( n->n.class_ == N_CONSTANT ) {
        if( n->c.const_type == CONS_ABSOLUTE ) {
            if( n->c.lo.u.int_value == 0 ) {
                bits |= LO_HALF | HI_HALF;
            } else if( n->n.size == 2 ) {
                if( ( n->c.lo.u.int_value & 0x00ff ) == 0 ) {
                    bits |= LO_HALF;
                }
                if( ( n->c.lo.u.int_value & 0xff00 ) == 0 ) {
                    bits |= HI_HALF;
                }
            } else if( n->n.size == 4 ) {
                if( ( n->c.lo.u.int_value & 0x0000ffff ) == 0 ) {
                    bits |= LO_HALF;
                }
                if( ( n->c.lo.u.int_value & 0xffff0000 ) == 0 ) {
                    bits |= HI_HALF;
                }
            }
        }
    } else if( n->n.class_ == N_REGISTER ) {
        i  = n->r.reg_index;
        if( ScoreLookup( &scoreboard[i], back.score.ScZero ) ) {
            bits |= LO_HALF | HI_HALF;
        } else {
            hi = back.score.ScoreList[i]->high;
            lo = back.score.ScoreList[i]->low;
            if( hi != NO_INDEX && lo != NO_INDEX ) {
                if( _IsZero( HasZero( back, scoreboard, back.score.ScoreList[lo]->reg_name ) ) ) {
                    bits |= LO_HALF;
                }
                if( _IsZero( HasZero( back, scoreboard, back.score.ScoreList[hi]->reg_name ) ) ) {
                    bits |= HI_HALF;
                }
            }
        }
    }
    if( n->n.size == 1 ) {
        bits |= IS_BYTE;
    }
    return( bits );
}
