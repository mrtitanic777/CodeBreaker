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
* Description:  Instruction scoreboarding.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "score.h"
#include "makeins.h"
#include "data.h"
#include "fpu.h"
#include "rgtbl.h"
#include "expand.h"
#include "namelist.h"
#include "split.h"
#include "index.h"
#include "fixindex.h"
#include "_split.h"
#include "backrun.hpp"


const opcode_entry  *ResetGenEntry( BackRun& back, instruction *ins )
/****************************************************/
{
    const opcode_entry  *try_;
    bool                dummy;

    try_ = FindGenEntry( back, ins, &dummy );
#if _TARGET_INTEL
    /*
     * Real architectures are orthogonal and don't need to swap
     * op's. BBB
     */
    switch( try_->generate ) {
    case R_SWAPOPS:
        rSWAPOPS( back, ins );
        try_ = FindGenEntry( back, ins, &dummy );
        break;
    case R_SWAPCMP:
        rSWAPCMP( back, ins );
        try_ = FindGenEntry( back, ins, &dummy );
        break;
    }
#endif
    return( try_ );
}


bool    ChangeIns( BackRun& back, instruction *ins, name *to, name **op, change_type flags )
/****************************************************************************
 * Is it alright to change operand "*op" to a reference to "to" in
 * instruction "ins".  We check here that the instruction would still
 * be generatable by looking up its generate entry.  We must also check
 * that it sets the condition codes the same way as the original
 * instruction would.  For example, SUB R1,1 => R1 becoming LA 1[R1] =>
 * R1 would be no good if the first form set condition codes but the
 * second form did not.
 */
{
    const opcode_entry  *try_;
    const opcode_entry  *table;
    const opcode_entry  *gen_table;
    name                *save_result;
    name                *old_op;
    opcnt               i;
    name                *save_ops[MAX_OPS_PER_INS];
    type_class_def      type_class;
    bool                ok;

    ok = true;
    table = ins->table;
    gen_table = ins->u.gen_table;
    old_op = *op;
    if( ins->head.opcode == OP_CONVERT ) {
        type_class = ins->base_type_class;
    } else {
        type_class = ins->type_class;
    }
    if( to->n.class_ != N_CONSTANT ) {
        if( back.data.TypeClassSize[type_class] != to->n.size ) {
            return( false );
        }
    }
    for( i = ins->num_operands; i-- > 0; ) {
        save_ops[i] = ins->operands[i];
    }
    save_result = ins->result;
    if( ( flags & CHANGE_ALL ) || to->n.class_ == N_INDEXED ) {
        for( i = ins->num_operands; i-- > 0; ) {
            if( ins->operands[i] == old_op ) {
                ins->operands[i] = to;
            }
        }
        if( ins->result == old_op ) {
            ins->result = to;
        }
    } else {
        *op = to;
    }
    if( ( flags & CHANGE_GEN ) ) {
        try_ = ResetGenEntry( back, ins );
        if( try_->generate < G_UNKNOWN &&
            (( (ins->ins_flags & INS_CC_USED) == 0)
                || ((ins->u.gen_table->op_type & MASK_CC) ==
                    (gen_table->op_type & MASK_CC))) ) {
            if( to->n.class_ != N_INDEXED && old_op->n.class_ == N_INDEXED ) {
                ins->num_operands = OpcodeNumOperands( ins );
            }
        } else {
            ok = false;
        }
    }
    if( !ok || (flags & CHANGE_CHECK) ) {
        /*
         * if we failed or we were just checking then
         * restore the original instruction
         */
        for( i = ins->num_operands; i-- > 0; ) {
            ins->operands[i] = save_ops[i];
        }
        ins->result = save_result;
        ins->table = table;
        ins->u.gen_table = gen_table;
    }
    return( ok );
}


static  bool    TryOldIndex( BackRun& back, score_hdl scoreboard, instruction *ins, name **opp )
/********************************************************************************
 * Try to find an 'older' index register to replace the index of
 * "*opp", in instruction "ins", given register scoreboard "sc".
 */
{
    name        *op;
    score_hdl   this_reg;
    score_hdl   curr_reg;
    name        *index;
    name        *reg_name;

    op = *opp;
    if( op->n.class_ != N_INDEXED )
        return( false );
    if( op->i.index->n.class_ != N_REGISTER )
        return( false );
    if( op->i.index->r.reg_index == NO_INDEX )
        return( false );
    this_reg = &scoreboard[op->i.index->r.reg_index];
    for( curr_reg = this_reg->next_reg; curr_reg != this_reg; curr_reg = curr_reg->next_reg ) {
        if( curr_reg->generation < this_reg->generation ) {
            reg_name = back.score.ScoreList[curr_reg->index]->reg_name;
            /*
             * OOPS! what about "op [edx] -> [edx]" which zaps edx...
             */
//            if( opp != &ins->result ||
            if( *opp != ins->result ||
                !HW_Ovlap( reg_name->r.reg, ins->zap->reg ) ) {
                index = ScaleIndex( back, reg_name,
                                    op->i.base,
                                    op->i.constant,
                                    op->n.type_class,
                                    op->n.size, op->i.scale,
                                    op->i.index_flags );
                if( IndexOkay( back, ins, index ) && ChangeIns( back, ins, index, opp, CHANGE_NORMAL ) ) {
                    return( true );
                }
            }
        }
    }
    return( false );
}

static  bool    TryRegOp( BackRun& back, score_hdl scoreboard, instruction *ins, name **opp )
/*****************************************************************************
 * See if we can find an equivalent register operand for the operand
 * "*opp" in instruction "ins", given that the current state of
 * registers is reflected by scoreboard "sc".
 */
{
    name        *op;
    int         i;
    hw_reg_set  live;
    score_hdl   this_reg;
    score_hdl   curr_reg;
    score_info  info;

    if( !CanReplace( ins ) )
        return( false );
    op = *opp;
    if( op->n.class_ == N_REGISTER ) {
        live = ins->head.next->head.live.regs;
        this_reg = &scoreboard[op->r.reg_index];
        if( !HW_Ovlap( live, op->r.reg ) ) {
            for( curr_reg = this_reg->next_reg; curr_reg != this_reg; curr_reg = curr_reg->next_reg ) {
                if( HW_Ovlap( live, back.score.ScoreList[curr_reg->index]->reg )
                && curr_reg->generation < this_reg->generation
                && ChangeIns( back, ins, back.score.ScoreList[curr_reg->index]->reg_name, opp, CHANGE_GEN ) ) {
                     return( true );
                }
            }
        }
        for( curr_reg = this_reg->next_reg; curr_reg != this_reg; curr_reg = curr_reg->next_reg ) {
            if( curr_reg->generation < this_reg->generation
             && ChangeIns( back, ins, back.score.ScoreList[curr_reg->index]->reg_name, opp, CHANGE_GEN ) ) {
                return( true );
            }
        }
        return( false );
    } else {
        ScoreInfo( back, &info, op );
        if( info.class_ == SC_N_CONSTANT ) {
            if( _OpIsCondition( ins->head.opcode ) &&
                info.symbol.u.p == NULL && info.offset == 0 ) {
                /*
                 * don't change cmp x,0
                 */
                return( false );
            }
            if( _IsFloating( ins->type_class ) ) {
                /*
                 * careful -- info->offset is NOT right for FP consts!
                 */
                return( false );
            }
        }
        for( i = back.score.ScoreCount; i-- > 0; ) {
            if( ScoreEqual( back, scoreboard, i, &info ) && ChangeIns( back, ins, back.score.ScoreList[i]->reg_name, opp, CHANGE_GEN ) ) {
                return( true );
            }
        }

        /*
         * % couldn't find a register operand, try for an older index
         */
        return( TryOldIndex( back, scoreboard, ins, opp ) );
    }
}


bool    FindRegOpnd( BackRun& back, score_hdl scoreboard, instruction *ins )
/************************************************************
 * See if we can find an operand of "ins" that could be replaces by a
 * register or an 'older' register (one that was defined first).
 */
{
    opcnt       i;
    bool        change;

    if( IsStackReg( ins->result ) )
        return( false );
    change = false;
    for( i = OpcodeNumOperands( ins ); i-- > 0; ) {
        if( TryRegOp( back, scoreboard, ins, &ins->operands[i] ) ) {
            change = true;
        }
    }
    if( ins->result != NULL ) {
        if( TryOldIndex( back, scoreboard, ins, &ins->result ) ) {
            change = true;
        }
    }
    return( change );
}

void    ScoreMakeEqual( BackRun& back, score_hdl scoreboard, name *op1, name *op2 )
/*****************************************************************
 * Make 'op1' and 'op2' equivalent in scoreboarder information
 *     - one of them must be a register
 */
{
    int         op2_index;
    name        *tmp;
    score_info  info;

    if( op1->n.class_ == N_REGISTER ) {
        tmp = op1;
        op1 = op2;
        op2 = tmp;
    }
    if( op2->n.class_ == N_REGISTER ) {
        op2_index = op2->r.reg_index;
        if( op1->n.class_ == N_REGISTER ) {
            RegAdd( back, scoreboard, op2_index, op1->r.reg_index );
        } else {
            ScoreInfo( back, &info, op1 );
            /*
             * NB: reg can never have the value x[reg]
             */
            if( info.reg_index == NO_INDEX
                || !HW_Ovlap( op2->r.reg, back.score.ScoreList[info.reg_index]->reg ) ) {
                ScoreAssign( back, scoreboard, op2_index, &info );
            }
        }
    }
}

bool    ScoreMove( BackRun& back, score_hdl scoreboard, instruction *ins )
/**********************************************************
 * Update "sc" to reflect the affect of an OP_MOV instruction "ins" on
 * the registers and memory locations.
 */
{
    name        *src;
    name        *dst;
    int         src_index;
    int         dst_index;
    score_info  info;

    src = ins->operands[0];
    dst = ins->result;
    src_index = 0;
    if( src->n.class_ == N_REGISTER ) {
        src_index = src->r.reg_index;
    }
    if( dst->n.class_ == N_REGISTER ) {
        dst_index = dst->r.reg_index;
        if( src->n.class_ == N_REGISTER ) {
            if( RegsEqual( scoreboard,  dst_index, src_index ) ) {
                FreeIns( back, ins );
                return( true );
            } else {
                RegKill( back, scoreboard, dst->r.reg );
                RegAdd( back, scoreboard, dst_index, src_index );
            }
        } else {
            ScoreInfo( back, &info, src );
            if( ScoreEqual( back, scoreboard, dst->r.reg_index, &info ) ) {
                FreeIns( back, ins );
                return( true );
            } else {
                RegKill( back, scoreboard, dst->r.reg );
                /*
                 * NB: reg can never have the value x[reg]
                 */
                if( info.reg_index == NO_INDEX
                  || !HW_Ovlap( dst->r.reg, back.score.ScoreList[info.reg_index]->reg ) ) {
                    if( !FPIsConvert( back, ins ) ) {
                        ScoreAssign( back, scoreboard, dst_index, &info );
                    }
                }
            }
        }
    } else {
        ScoreInfo( back, &info, dst );
        if( src->n.class_ == N_REGISTER ) {       /* and dst is not a register*/
            if( ScoreEqual( back, scoreboard, src->r.reg_index, &info ) ) {
                FreeIns( back, ins );
                return( true );
            } else {
                ScoreKillInfo( back, scoreboard, dst, &info, src->r.reg );
                if( !FPIsConvert( back, ins ) ) {
                    ScoreAssign( back, scoreboard, src_index, &info );
                }
            }
        } else {
            ScoreKillInfo( back, scoreboard, dst, &info, HW_EMPTY );
        }
    }
    return( false );
}


bool    ScoreLA( BackRun& back, score_hdl scoreboard, instruction *ins )
/********************************************************
 * Update "sc" to reflect the affect of an OP_MOV instruction "ins" on
 * the registers and memory locations.
 */
{
    name        *src;
    name        *dst;
    int         dst_index;
    score_info  info;

    src = ins->operands[0];
    dst = ins->result;
    if( dst->n.class_ == N_REGISTER ) {
        dst_index = dst->r.reg_index;
        if( !ScoreLAInfo( &info, src ) ) {
            RegKill( back, scoreboard, dst->r.reg );
        } else if( ScoreEqual( back, scoreboard, dst->r.reg_index, &info ) ) {
            FreeIns( back, ins );
            return( true );
        } else {
            RegKill( back, scoreboard, dst->r.reg );
            ScoreAssign( back, scoreboard, dst_index, &info );
        }
    } else {
        ScoreInfo( back, &info, dst );
        ScoreKillInfo( back, scoreboard, dst, &info, HW_EMPTY );
    }
    return( false );
}


void    ScZeroCheck( BackRun& back, score_hdl scoreboard, instruction *ins )
/************************************************************
 * Check if instruction "ins" ends up with the result being Zero.  For
 * example SUB R1,R1 => R1, results in R1 becoming 0.
 */
{
    int i;

    if( ins->head.opcode != OP_XOR
     && ins->head.opcode != OP_MOD
     && ins->head.opcode != OP_SUB )
        return;
    if( ins->operands[0] != ins->operands[1] )
        return;
    if( ins->operands[0] != ins->result )
        return;
    if( ins->operands[0]->n.class_ != N_REGISTER )
        return;
    i = ins->result->r.reg_index;
    ScoreAssign( back, scoreboard, i, back.score.ScZero );
}
