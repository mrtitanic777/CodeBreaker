/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Expand instructions so that machine code can be generated.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "memcheck.h"
#include "zoiks.h"
#include "data.h"
#include "namelist.h"
#include "blips.h"
#include "expand.h"
#include "split.h"
#include "insdead.h"
#include "rgtbl.h"
#include "optab.h"
#include "index.h"
#include "fixindex.h"
#include "generate.h"
#include "conflict.h"
#include "cgaux.h"
#include "bgcall.h"
#include "targetin.h"
#include "backrun.hpp"


static instruction  *DoReduce( BackRun& back, instruction *ins, const opcode_entry *try_, bool has_index )
/****************************************************************************************/
{
    const hw_reg_set    *zap;
    hw_reg_set          zap_all;

    if( try_->generate == G_NO )
        return( ins );
    if( try_->generate >= FIRST_REDUCT )
        return( Reduce( back, ins ) );
    zap_all = ins->zap->reg;
    for( zap = RegSetList(back, RegList[try_->reg_set].zap); !HW_CEqual( *zap, HW_EMPTY ); ++zap ) {
        HwTurnOn( zap_all, *zap );
    }
    ins->zap = reinterpret_cast<register_name *>(AllocRegName( back, zap_all ));
    if( has_index || ins->num_operands > OpcodeNumOperands( ins ) ) {
        ins = NeedIndex( back, ins );
    } else {
        ins->u2.index_needs = RL_;
    }
    return( ins );
}


inline bool AnyMul( operand_types ops, unsigned int mul ) { return ( ( ops & (ANY*mul) ) == (ANY*mul) ); }

static  bool    VerifyRegs( BackRun& back, instruction *ins, operand_types ops )
/***************************************************************/
{

    const opcode_entry  *try_;
    const hw_reg_set    *possible;
    name                *name;
    const op_regs       *need;
    hw_reg_set          regs;
    reg_set_index       left_index;
    reg_set_index       right_index;
    reg_set_index       result_index;
    reg_set_index       special_index;

    try_ = ins->u.gen_table;
    need = &RegList[try_->reg_set];
    result_index = RL_;
    name = ins->result;
    if( ( ops & R_R ) == 0 ) {
        if( ops & U_R ) {
            result_index = need->result;
        }
    } else if( need->result != RL_ ) {
        if( name->n.class_ != N_REGISTER ) {
            result_index = need->result;
        } else if( !AnyMul( try_->op_type, RESULT_MUL ) ) {
            regs = name->r.reg;
            for( possible = RegSetList(back, need->result); ; ++possible ) {
                if( HW_CEqual( *possible, HW_EMPTY ) )
                    return( false );
                if( HwEqual( *possible, regs ) ) {
                    break;
                }
            }
        }
    }
    left_index = RL_;
    if( ( ops & R_1 ) == 0 ) {
        if( ops & U_1 ) {
            left_index = need->left;
        }
    } else if( need->left != RL_ ) {
        name = ins->operands[0];
        if( name->n.class_ != N_REGISTER ) {
            left_index = need->left;
        } else if( !AnyMul( try_->op_type, OP1_MUL ) ) {
            regs = name->r.reg;
            for( possible = RegSetList(back, need->left); ; ++possible ) {
                if( HW_CEqual( *possible, HW_EMPTY ) )
                    return( false );
                if( HwEqual( *possible, regs ) ) {
                    break;
                }
            }
        }
    }
    right_index = RL_;
    if( ( ops & R_2 ) == 0 ) {
        if( ops & U_2 ) {
            right_index = need->right;
        }
    } else if( need->right != RL_ ) {
        name = ins->operands[1];
        if( name->n.class_ != N_REGISTER ) {
            right_index = need->right;
        } else if( !AnyMul( try_->op_type, OP2_MUL ) ) {
            regs = name->r.reg;
            for( possible = RegSetList(back, need->right); ; ++possible ) {
                if( HW_CEqual( *possible, HW_EMPTY ) )
                    return( false );
                if( HwEqual( *possible, regs ) ) {
                    break;
                }
            }
        }
    }
    if( try_->generate < FIRST_REDUCT ) {
        special_index = SpecialPossible( ins );
        if( special_index != RL_ ) {
            MarkPossible( back, ins, ins->result, special_index );
        } else if( result_index != RL_ ) {
            MarkPossible( back, ins, ins->result, result_index );
        }
        if( left_index != RL_ ) {
            MarkPossible( back, ins, ins->operands[0], left_index );
        }
        if( right_index != RL_ ) {
            MarkPossible( back, ins, ins->operands[1], right_index );
        }
    }
    return( true );
}


static  operand_types   ClassifyOps( instruction *ins, bool *has_index )
/**********************************************************************/
{
    name                *name;
    operand_types       ops;
    opcnt               num_operands;

    num_operands = OpcodeNumOperands( ins );
    *has_index = false;
    ops = NONE;
    name = ins->result;
    if( name != NULL && num_operands != 0 ) {
        if( ins->operands[0] == name ) {
            ops &= EQ_R1;
        }
        if( num_operands != 1 ) {
            if( ins->operands[1] == name ) {
                ops &= EQ_R2;
            }
        }
    }
    if( name != NULL ) {
        switch( name->n.class_ ) {
        case N_REGISTER:
            ops |= R_R;
            break;
        case N_INDEXED:
            *has_index = true;
            ops |= M_R;
            break;
        case N_MEMORY:
        case N_TEMP:
            if( name->v.conflict == NULL ) {
                ops |= M_R;
            } else if( name->v.usage & USE_MEMORY ) {
                ops |= M_R;
            } else {
                ops |= U_R;
                ins->head.state = OPERANDS_NEED_WORK;
            }
            break;
        }
    }
    if( num_operands != 0 ) {
        name = ins->operands[0];
        switch( name->n.class_ ) {
        case N_CONSTANT:
            ops |= C_1;
            break;
        case N_REGISTER:
            ops |= R_1;
            break;
        case N_INDEXED:
            *has_index = true;
            ops |= M_1;
            break;
        case N_MEMORY:
        case N_TEMP:
            if( name->v.conflict == NULL ) {
                ops |= M_1;
            } else if( name->v.usage & USE_MEMORY ) {
                ops |= M_1;
            } else {
                ops |= U_1;
                ins->head.state = OPERANDS_NEED_WORK;
            }
            break;
        }
    }
    if( num_operands > 1 ) {
        name = ins->operands[1];
        switch( name->n.class_ ) {
        case N_CONSTANT:
            ops |= C_2;
            break;
        case N_REGISTER:
            ops |= R_2;
            break;
        case N_INDEXED:
            *has_index = true;
            ops |= M_2;
            break;
        case N_MEMORY:
        case N_TEMP:
            if( name->v.conflict == NULL ) {
                ops |= M_2;
            } else if( name->v.usage & USE_MEMORY ) {
                ops |= M_2;
            } else {
                ops |= U_2;
                ins->head.state = OPERANDS_NEED_WORK;
            }
            break;
        }
        if( num_operands > 2 ) {
            name = ins->operands[2];
            if( name->n.class_ == N_INDEXED ) {
                *has_index = true;
            }
            if( ins->head.opcode == OP_CALL_INDIRECT ) {
                switch( name->n.class_ ) {
                case N_MEMORY:
                case N_TEMP:
                    if( name->v.conflict != NULL && (name->v.usage & USE_MEMORY) == 0 ) {
                        ins->head.state = OPERANDS_NEED_WORK;
                    }
                }
            }
        }
    }
    return( ops );
}


const opcode_entry  *FindGenEntry( BackRun& back, instruction *ins, bool *has_index )
/********************************************************************/
{
    const opcode_entry  *try_;
    operand_types       ops;
    vertype             verify;

    ins->head.state = INS_READY;
    try_ = ins->table;
    if( try_ == NULL ) {
        try_ = CodeTable( back, ins );
        ins->table = try_;
    }
    if( try_ == NULL )
        return( try_ );
    ops = ClassifyOps( ins, has_index );
    for( ;; ++try_ ) {
        if( ( try_->op_type & ops ) != ops )
            continue;
        verify = try_->verify;
        if( verify != V_NO ) {
            if( !DoVerify( back, verify & ~NOT_VOLATILE, ins ) )
                continue;
            if( (verify & NOT_VOLATILE) && VolatileIns( ins ) ) {
                continue;
            }
        }
        ins->u.gen_table = try_;
        if( try_->reg_set != RG_ ) {
            if( !VerifyRegs( back, ins, ops ) ) {
                continue;
            }
        }
        if( ins->head.opcode == OP_CALL_INDIRECT ) {
            MarkPossible(back, ins, ins->operands[CALL_OP_ADDR], CallIPossible(ins));
        }
        break;
    }
    return( try_ );
}


void    FixGenEntry( BackRun& back, instruction *ins )
/*************************************/
{
/* update the instruction to reflect new instruction needs*/

    const opcode_entry  *old;
    bool                dummy;

    old = ins->u.gen_table;
    FindGenEntry( back, ins, &dummy );
    if( InsGenerate( ins ) >= FIRST_REDUCT ) {
        ins->u.gen_table = old;
    }
    ins->head.state = INS_NEEDS_WORK;
}


instruction     *PostExpandIns( BackRun& back, instruction *ins )
/************************************************/
{
    const opcode_entry  *try_;
    bool                dummy;

    try_ = FindGenEntry( back, ins, &dummy );
    if( try_ == NULL )
        return( ins );
    if( try_->generate == G_NO )
        return( ins );
    if( try_->generate >= FIRST_REDUCT )
        return( Reduce( back, ins ) );
    return( ins );
}


instruction     *ExpandIns( BackRun& back, instruction *ins )
/********************************************/
{
    const opcode_entry  *try_;
    bool                has_index;

    try_ = FindGenEntry( back, ins, &has_index );
    if( try_ == NULL )
        return( ins );
    return( DoReduce( back, ins, try_, has_index ) );
}


int     ExpandOps( BackRun& back, bool keep_on_truckin )
/***************************************/
{
    block       *blk;
    instruction *ins;
    int         unknowns;

    unknowns = 0;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        EXBlip();
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ) {
            if( !keep_on_truckin && MemLow( back ) ) {
                return( -1 );
            }
            ins = ExpandIns( back, ins );
            if( ins->head.state == INS_NEEDS_WORK ) {
                ins = ExpandIns( back, ins );
            } else {
                if( ins->head.state == OPERANDS_NEED_WORK ) {
                    ++unknowns;
                }
                ins = ins->head.next;
            }
        }
    }
    return( unknowns );
}
