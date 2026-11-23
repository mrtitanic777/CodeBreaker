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
* Description:  Constant folding on instruction level.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "tree.h"
#include "makeins.h"
#include "foldins.h"
#include "data.h"
#include "treefold.h"
#include "treeconv.h"
#include "namelist.h"
#include "objout.h"
#include "insutil.h"
#include "typemap.h"
#include "optab.h"
#include "inssegs.h"
#include "fixindex.h"
#include "cse.h"
#include "bldins.h"
#include "treeprot.h"
#include "feprotos.h"
#include "backrun.hpp"


bool     IsTrickyPointerConv( BackRun& back, instruction *ins )
/******************************************************
    Is "ins" a near (based) to far pointer conversion that has
    to be carefully converted with taking segments into account?
*/
{
    if( (ins->head.opcode == OP_CONVERT)
      && IsPointerClass( ins->type_class ) ) {
        if( ins->base_type_class == U2
          && back.data.TypeClassSize[ins->type_class] > WORD_SIZE ) {
            return( true );
        }
    }
    return( false );
}

static  bool    RelocConst( name *op ) {
/***************************************
    Is "op" a relocatable constant?
*/

    bool        reloc_const;

    reloc_const = false;
    if( op->n.class_ == N_CONSTANT ) {
        if( op->c.const_type != CONS_ABSOLUTE ) {
            reloc_const = true;
        }
    }
    return( reloc_const );
}


static bool ActiveCompare( instruction *ins ) {
/*********************************************/

    if( FalseIndex( ins ) == TrueIndex( ins ) )
        return( false );
    /* if unexpanded OR expanded and not just placeholder conditional */
    if( ins->table == NULL )
        return( true );
    if( ins->u.gen_table == NULL )
        return( false );
    if( DoesSomething( ins ) )
        return( true );
    return( false );
}


static  instruction *KillCompare( BackRun& back, instruction *ins, name *result ) {
/******************************************************************/

    instruction *new_ins;

    if( ins->result != NULL ) {
        new_ins = MakeMove( back, result, ins->result, ins->result->n.type_class );
        DupSeg( ins, new_ins );
        SetCSEBits( ins, new_ins );
        ReplIns( back, ins, new_ins );
    } else {
        new_ins = ins;
        DoNothing( back, ins );
    }
    return( new_ins );
}


static  instruction *CmpRelocZero( BackRun& back, instruction *ins, opcnt c, opcnt r )
/*********************************************************************/
{
    name        *cons;
    name        *rel;
    bool        truth;

    if( OpcodeNumOperands( ins ) != 2 )
        return( NULL );
    cons = ins->operands[c];
    if( cons->n.class_ != N_CONSTANT )
        return( NULL );
    if( cons->c.const_type != CONS_ABSOLUTE )
        return( NULL );
    if( CFTest( cons->c.u.cfval ) != 0 )
        return( NULL );
    rel = ins->operands[r];
    if( rel->c.const_type == CONS_OFFSET
      && !AskSegIsNear( back, (segment_id)rel->c.lo.u.int_value ) )
        return( NULL );
    switch( ins->head.opcode ) {
    case OP_BIT_TEST_FALSE:
    case OP_CMP_EQUAL:
    case OP_CMP_LESS:
    case OP_CMP_LESS_EQUAL:
        truth = false;
        break;
    case OP_BIT_TEST_TRUE:
    case OP_CMP_GREATER:
    case OP_CMP_GREATER_EQUAL:
    case OP_CMP_NOT_EQUAL:
        truth = true;
        break;
    default:
        return( NULL );
    }
    if( !ActiveCompare( ins ) )
        return( NULL );
    if( c != 1 )
        truth = !truth;
    if( truth ) {
        SetBlockIndex( ins, TrueIndex(ins), TrueIndex(ins) );
    } else {
        SetBlockIndex( ins, FalseIndex(ins), FalseIndex(ins) );
    }
    return( KillCompare( back, ins, AllocIntConst( back, truth ? FETrue() : 0 ) ) );
}

static  instruction *StraightLine( BackRun& back, instruction *ins, tn fold, bool is_true )
/***************************************************************************
    See if we can turn a comparison into a straight line piece of code.
*/
{
    name        *result;

    result = TGetName( back, fold );
    if( result == NULL )
        return( NULL );
    if( result->n.class_ != N_CONSTANT )
        return( NULL );
    if( result->c.const_type != CONS_ABSOLUTE )
        return( NULL );
    if( result->c.lo.u.int_value == 0 )
        is_true = !is_true;
    if( is_true ) {
        SetBlockIndex( ins, TrueIndex(ins), TrueIndex(ins) );
    } else {
        SetBlockIndex( ins, FalseIndex(ins), FalseIndex(ins) );
    }
    return( KillCompare( back, ins, result ) );
}

static  instruction    *FoldAbsolute( BackRun& back, instruction *ins ) {
/*********************************************************
    See below.
*/
    instruction     *new_ins;
    opcnt           num_operands;
    name            *result;
    tn              fold;
    const type_def  *tipe;
    const type_def  *left_tipe;
    const type_def  *rite_tipe;
    const type_def  *fold_tipe;
    pointer         left;
    pointer         rite;
    name            *tmp;
    name            *new_const;
    float_handle    cf_value;

    tipe = TypeOfTypeClass( back, ins->type_class );
    left_tipe = TypeOfTypeClass( back, OpClass( ins ) );
    num_operands = OpcodeNumOperands( ins );
    left = NULL;
    rite = NULL;
    if( num_operands != 0 ) {
        left = TName( back, ins->operands[0], left_tipe );
        if( num_operands > 1 ) {
            if( ins->operands[1]->n.type_class == XX ) {
                rite_tipe = tipe;
            } else {
                rite_tipe = TypeOfTypeClass( back, ins->operands[1]->n.type_class );
            }
            rite = TName( back, ins->operands[1], rite_tipe );
        }
    }
    fold = NULL;
    switch( ins->head.opcode ) {
    case OP_ADD:
        fold = FoldPlus( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_SUB:
        fold = FoldMinus( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_MUL:
        fold = FoldTimes( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_DIV:
        fold = FoldDiv( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_MOD:
        fold = FoldMod( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_AND:
        fold = FoldAnd( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_OR:
        fold = FoldOr( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_XOR:
        fold = FoldXor( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_RSHIFT:
        fold = FoldRShift( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_LSHIFT:
        fold = FoldLShift( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
        break;
    case OP_NEGATE:
        fold = FoldUMinus( back, static_cast<tn>(left), tipe );
        break;
    case OP_COMPLEMENT:
        fold = Fold1sComp( back, static_cast<tn>(left), tipe );
        break;
    case OP_CONVERT:
        /* look out for CNV PT U2 t1 type instructions; if sizeof( PT ) is greater */
        /* than sizeof( U2 ), we don't want to fold or we'll screw up based pointers */
        if( IsTrickyPointerConv( back, ins ) )
            return( NULL );
        /* fall through */
    case OP_ROUND:
        fold = FoldCnvRnd( back, (cg_op)ins->head.opcode, static_cast<tn>(left), tipe );
        break;
    case OP_CMP_EQUAL:
    case OP_CMP_NOT_EQUAL:
    case OP_CMP_GREATER:
    case OP_CMP_LESS_EQUAL:
    case OP_CMP_LESS:
    case OP_CMP_GREATER_EQUAL:
        if( ActiveCompare( ins ) ) {
            fold = FoldCompare( back, (cg_op)ins->head.opcode, static_cast<tn>(left), static_cast<tn>(rite), tipe );
            if( fold != NULL ) {
                return( StraightLine( back, ins, fold, true ) );
            }
        }
        break;
    case OP_BIT_TEST_TRUE:
        if( ActiveCompare( ins ) ) {
            fold = FoldAnd( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
            if( fold != NULL ) {
                return( StraightLine( back, ins, fold, true ) );
            }
        }
        break;
    case OP_BIT_TEST_FALSE:
        if( ActiveCompare( ins ) ) {
            fold = FoldAnd( back, static_cast<tn>(left), static_cast<tn>(rite), tipe );
            if( fold != NULL ) {
                return( StraightLine( back, ins, fold, false ) );
            }
        }
        break;
    default:
        fold = NULL;
        break;
    }
    if( fold != NULL ) {
        fold_tipe = fold->tipe;
        result = TGetName( back, fold );
        if( result != NULL
          && !NeedConvert( fold_tipe, tipe ) ) {
            ins->table = NULL;
            /* look out for scary DIV U4 EDX:EAX, c1 -> t1 type instructions */
            if( result->n.class_ != N_CONSTANT
              && result->n.size != back.data.TypeClassSize[ins->type_class] )
                return( NULL );
            /* look out for scary MUL U4 EDX:EAX, c1 -> t1 type instructions */
            if( result->n.class_ != N_CONSTANT
              && ins->result->n.size != back.data.TypeClassSize[ins->type_class] )
                return( NULL );
            new_ins = MakeMove( back, result, ins->result, ins->type_class );
            SetCSEBits( ins, new_ins );
            DupSeg( ins, new_ins );
            ReplIns( back, ins, new_ins );
            return( new_ins );
        }
    } else {
        if( left != NULL ) {
            TGetName( back, static_cast<tn>(left) );
        }
        if( rite != NULL ) {
            TGetName( back, static_cast<tn>(rite) );
        }
    }
    switch( ins->head.opcode ) {
    case OP_SUB:
        /* change sub t1, k -> add t1, -k */
        if( ins->operands[1]->n.class_ == N_CONSTANT
          && ins->operands[1]->c.const_type == CONS_ABSOLUTE ) {
            cf_value = OkToNegate( back, ins->operands[1]->c.u.cfval, tipe );
            if( cf_value != NULL ) {
                new_const = AllocConst( back, cf_value );
                new_ins = MakeBinary( back, OP_ADD, ins->operands[0], new_const, ins->result, ins->type_class );
                SetCSEBits( ins, new_ins );
                DupSeg( ins, new_ins );
                ReplIns( back, ins, new_ins );
                return( new_ins );
            }
        }
        break;
    case OP_ADD:
    case OP_EXT_ADD:
    case OP_MUL:
    case OP_EXT_MUL:
    case OP_OR:
    case OP_AND:
    case OP_XOR:
        /* for commutative op's prefer constant on right */
        if( IsPointerClass( ins->type_class ) )
            break;
        tmp = ins->operands[0];
        if( tmp->n.class_ == N_CONSTANT
          && tmp->c.const_type == CONS_ABSOLUTE ) {
            ins->operands[0] = ins->operands[1];
            ins->operands[1] = tmp;
        }
        break;
    }
    return( NULL );
}

instruction    *FoldIns( BackRun& back, instruction *ins ) {
/****************************************************
    See if we can do any constant folding on instruction "ins". This is
    done by building an expression tree to represent "ins", and then
    calling the constant folder used at tree building time. If a folded
    result comes back (non-NULL), turn that result into an instruction
    to replace the original "ins".
*/

    opcnt       i;
    bool        have_const;

    if( ins->ins_flags & INS_CC_USED )
        return( NULL );
    have_const = false;
    for( i = ins->num_operands; i-- > 0; ) {
        if( ins->operands[i]->n.class_ == N_CONSTANT ) {
            if( ins->operands[i]->c.const_type == CONS_ABSOLUTE ) {
                return( FoldAbsolute( back, ins ) );
            }
            have_const = true;
        }
    }
    if( have_const ) {
        i = OpcodeNumOperands( ins );
        if( i > 1 ) {
            if( RelocConst( ins->operands[1] ) ) {
                return( CmpRelocZero( back, ins, 0, 1 ) );
            }
        }
        if( i > 0 ) {
            if( RelocConst( ins->operands[0] ) ) {
                return( CmpRelocZero( back, ins, 1, 0 ) );
            }
        }
    }
    return( NULL );
}


bool    ConstFold( BackRun& back, block *root ) {
/****************************************
    For each instruction in the program, see if we can do some constant
    folding that wasn't caught in the tree phase. This will come from
    copy/constant propagation causing an operand to be replaced with
    a constant.
*/

    instruction *ins;
    instruction *next;
    bool        change;
    block       *blk;

    change = false;
    blk = root;
    for( ;; ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
            next = ins->head.next;
            if( FoldIns( back, ins ) != NULL ) {
                change = true;
            }
        }
        blk = blk->u.partition;
        if( blk == root ) {
            break;
        }
    }
    return( change );
}
