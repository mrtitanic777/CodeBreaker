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
#include "regset.h"
#include "model.h"
#include "system.h"
#include "zoiks.h"
#include "makeins.h"
#include "namelist.h"
#include "rgtbl.h"
#include "split.h"
#include "insutil.h"
#include "optab.h"
#include "inssegs.h"
#include "optimize.h"
#include "x86split.h"
#include "overlap.h"
#include "x86segs.h"
#include "confldef.h"
#include "liveinfo.h"
#include "_x86half.h"
#include "backrun.hpp"


bool    IndexOverlaps( instruction *ins, opcnt i )
/************************************************/
{
    if( ins->operands[i]->n.class_ != N_INDEXED )
        return( false );
    if( SameThing( ins->operands[i]->i.index, ins->result ) )
        return( true );
    return( false );
}

#if _TARGET & _TARG_8086
    #define LONG_WORD       U4
inline unsigned int HighWord( name *x ) { return ((((x)->c.lo.u.uint_value) >> 16) & 0xffff); }
#else
    #define LONG_WORD       U8
inline unsigned int &HighWord( name *x ) { return ((x)->c.hi.u.uint_value); }
#endif


instruction     *rSPLITOP( BackRun& back, instruction *ins )
/*******************************************/
{
    instruction *new_ins;
    instruction *ins2;
    name        *temp;

    if( IndexOverlaps( ins, 0 ) || IndexOverlaps( ins, 1 ) ) {
        temp = AllocTemp( back, LONG_WORD );
        HalfType( ins );
        new_ins = MakeBinary( back, ins->head.opcode,
                        LowPart( back, ins->operands[0], WD ),
                        LowPart( back, ins->operands[1], WD ),
                        LowPart( back, temp,             WD ),
                        WD );
        ins2 = MakeBinary( back, ins->head.opcode,
                        HighPart( back, ins->operands[0], WD ),
                        HighPart( back, ins->operands[1], WD ),
                        HighPart( back, temp,             WD ),
                        WD );
        if( ins->head.opcode == OP_ADD ) {
            ins2->head.opcode = OP_EXT_ADD;
        } else if( ins->head.opcode == OP_SUB ) {
            ins2->head.opcode = OP_EXT_SUB;
        }
        ins2->table = CodeTable( back, ins2 );
        new_ins->table = ins2->table;
        DupSegOp( back, ins, new_ins, 0 );
        DupSegOp( back, ins, ins2, 0 );
        DupSegOp( back, ins, new_ins, 1 );
        DupSegOp( back, ins, ins2, 1 );
        ins->operands[0] = temp;
        ins->operands[1] = temp;
        PrefixIns( back, ins, new_ins );
        PrefixIns( back, ins, ins2 );
        ins2 = MakeMove( back, LowPart( back, temp, WD ), LowPart( back, ins->result, WD ), WD );
        DupSegRes( back, ins, ins2 );
        PrefixIns( back, ins, ins2 );
        ins2 = MakeMove( back, HighPart( back, temp, WD ),
                          HighPart( back, ins->result, WD ), WD );
        DupSegRes( back, ins, ins2 );
        ReplIns( back, ins, ins2 );
    } else {
        HalfType( ins );
        new_ins = MakeBinary( back, ins->head.opcode,
                        LowPart( back, ins->operands[0], ins->type_class ),
                        LowPart( back, ins->operands[1], ins->type_class ),
                        LowPart( back, ins->result,      ins->type_class ),
                        ins->type_class );
        DupSeg( ins, new_ins );
        ins->operands[0] = HighPart( back, ins->operands[0], ins->type_class );
        ins->operands[1] = HighPart( back, ins->operands[1], ins->type_class );
        ins->result = HighPart( back, ins->result, ins->type_class );
        if( ins->head.opcode == OP_ADD ) {
            ins->head.opcode = OP_EXT_ADD;
        } else if( ins->head.opcode == OP_SUB ) {
            ins->head.opcode = OP_EXT_SUB;
        }
/* Assign fake reduce table (from OP_EXT) to new_ins; default reduce table
   can generate INC and DEC which'll not set condition codes
 */
        ins->table = CodeTable( back, ins );
        new_ins->table = ins->table;

        PrefixIns( back, ins, new_ins );
    }
    new_ins->ins_flags |= INS_CC_USED;
    return( new_ins );
}


instruction     *rSPLITMOVE( BackRun& back, instruction *ins )
/*********************************************/
{
    instruction *new_ins;
    instruction *ins2;
    name        *temp;

    CnvOpToInt( back, ins, 0 );
    if( IndexOverlaps( ins, 0 ) ) {
        temp = AllocTemp( back, LONG_WORD );
        new_ins = MakeMove( back, LowPart( back, ins->operands[0], WD ), LowPart( back, temp, WD ), WD );
        ins2 = MakeMove( back, HighPart( back, ins->operands[0], WD ), HighPart( back, temp, WD ), WD );
        DupSegOp( back, ins, new_ins, 0 );
        DupSegOp( back, ins, ins2, 0 );
        ins->operands[0] = temp;
        PrefixIns( back, ins, new_ins );
        PrefixIns( back, ins, ins2 );
        ins2 = MakeMove( back, LowPart( back, temp, WD ), LowPart( back, ins->result, WD ), WD );
        DupSegRes( back, ins, ins2 );
        PrefixIns( back, ins, ins2 );
        ins2 = MakeMove( back, HighPart( back, temp, WD ), HighPart( back, ins->result, WD ), WD );
        ReplIns( back, ins, ins2 );
    } else {
        HalfType( ins );
        new_ins = MakeMove( back, LowPart( back, ins->operands[0], ins->type_class ),
                             LowPart( back, ins->result, ins->type_class ),
                             ins->type_class );
        DupSeg( ins, new_ins );
        ins->operands[0] = HighPart( back, ins->operands[0], ins->type_class );
        ins->result = HighPart( back, ins->result, ins->type_class );
        if( new_ins->result->n.class_ == N_REGISTER
         && ins->operands[0]->n.class_ == N_REGISTER
         && HwOvlap( new_ins->result->r.reg, ins->operands[0]->r.reg ) ) {
            SuffixIns( back, ins, new_ins );
            new_ins = ins;
        } else {
            PrefixIns( back, ins, new_ins );
        }
    }
    return( new_ins );
}


instruction     *rSPLITNEG( BackRun& back, instruction *ins )
/********************************************/
{
    name        *hi_res;
    name        *lo_res;
    name        *hi_src;
    name        *lo_src;
    instruction *hi_ins;
    instruction *lo_ins;
    instruction *subtract;

    HalfType( ins );
    hi_res = HighPart( back, ins->result, ins->type_class );
    hi_src = HighPart( back, ins->operands[0], ins->type_class );
    lo_res = LowPart( back, ins->result, ins->type_class );
    lo_src = LowPart( back, ins->operands[0], ins->type_class );
    hi_ins = MakeUnary( back, OP_NEGATE, hi_src, hi_res, ins->type_class );
    lo_ins = MakeUnary( back, OP_NEGATE, lo_src, lo_res, ins->type_class );
    lo_ins->ins_flags |= INS_CC_USED;
    subtract = MakeBinary( back, OP_EXT_SUB, hi_res, AllocIntConst( back, 0 ), hi_res, ins->type_class );
    DupSegRes( back, ins, subtract );
    DupSeg( ins, hi_ins );
    DupSeg( ins, lo_ins );
    PrefixIns( back, ins, hi_ins );
    ins->operands[0] = ins->result;
    ins->operands[1] = AllocIntConst( back, 0 );
    PrefixIns( back, ins, lo_ins );
    ReplIns( back, ins, subtract );
    UpdateLive( back, hi_ins, subtract );
    return( hi_ins );
}

instruction     *rSPLITCMP( BackRun& back, instruction *ins )
/********************************************/
{
    name                *left;
    name                *right;
    instruction         *low = NULL;
    instruction         *high = NULL;
    instruction         *not_equal = NULL;
    type_class_def      high_type_class;
    type_class_def      low_type_class;
    cond_dst_idx        true_idx;
    cond_dst_idx        false_idx;

    high_type_class = HalfClass[ins->type_class];
    low_type_class  = Unsigned[high_type_class];
    left = ins->operands[0];
    right = ins->operands[1];
    true_idx = TrueIndex( ins );
    false_idx = FalseIndex( ins );
    switch( ins->head.opcode ) {
    case OP_BIT_TEST_TRUE:
        high = MakeCondition( back, ins->head.opcode,
                        HighPart( back, left, high_type_class ),
                        HighPart( back, right, high_type_class ),
                        true_idx, NO_JUMP,
                        WD );
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, left, low_type_class ),
                        LowPart( back, right, low_type_class ),
                        true_idx, false_idx,
                        WD );
        not_equal = NULL;
        break;
    case OP_BIT_TEST_FALSE:
        high = MakeCondition( back, OP_BIT_TEST_TRUE,
                        HighPart( back, left, high_type_class ),
                        HighPart( back, right, high_type_class ),
                        false_idx, NO_JUMP,
                        WD );
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, left, low_type_class ),
                        LowPart( back, right, low_type_class ),
                        true_idx, false_idx,
                        WD );
        not_equal = NULL;
        break;
    case OP_CMP_EQUAL:
        high = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, left, high_type_class ),
                        HighPart( back, right, high_type_class ),
                        false_idx, NO_JUMP,
                        WD );
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, left, low_type_class ),
                        LowPart( back, right, low_type_class ),
                        true_idx, false_idx,
                        WD );
        not_equal = NULL;
        break;
    case OP_CMP_NOT_EQUAL:
        high = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, left, high_type_class ),
                        HighPart( back, right, high_type_class ),
                        true_idx, NO_JUMP,
                        WD );
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, left, low_type_class ),
                        LowPart( back, right, low_type_class ),
                        true_idx, false_idx,
                        WD );
        not_equal = NULL;
        break;
    case OP_CMP_LESS:
    case OP_CMP_LESS_EQUAL:
        not_equal = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, left, high_type_class ),
                        HighPart( back, right, high_type_class ),
                        false_idx, NO_JUMP,
                        high_type_class );
        if( high_type_class == WD
          && right->n.class_ == N_CONSTANT
          && right->c.const_type == CONS_ABSOLUTE
          && HighWord( right ) == 0 ) {
            high = NULL;
        } else {
            high = MakeCondition( back, OP_CMP_LESS,
                        not_equal->operands[0], not_equal->operands[1],
                        true_idx, NO_JUMP,
                        high_type_class );
        }
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, left, low_type_class ),
                        LowPart( back, right, low_type_class ),
                        true_idx, false_idx,
                        low_type_class );
        break;
    case OP_CMP_GREATER_EQUAL:
    case OP_CMP_GREATER:
        not_equal = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, left, high_type_class ),
                        HighPart( back, right, high_type_class ),
                        false_idx, NO_JUMP,
                        high_type_class );
        if( high_type_class == WD
          && right->n.class_ == N_CONSTANT
          && right->c.const_type == CONS_ABSOLUTE
          && HighWord( right ) == 0 ) {
            SetBlockIndex( not_equal, true_idx, NO_JUMP );
            high = NULL;
        } else {
            high = MakeCondition( back, OP_CMP_GREATER,
                        not_equal->operands[0], not_equal->operands[1],
                        true_idx, NO_JUMP,
                        high_type_class );
        }
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, left, low_type_class ),
                        LowPart( back, right, low_type_class ),
                        true_idx, false_idx,
                        low_type_class );
        break;
    default:
        break;
    }
    if( high != NULL ) {
/*
 * 2005-04-06 RomanT (bug #407)
 * I removed calls do DoNothing(), it seems ok, extra jumps are perfectly
 * optimized out in other places of compiler. Calling DoNothing() on chain
 * of conditions to reuse existing CC flags is ugly and causes unpredictable
 * logical faults in other places.
 */
        DupSeg( ins, high );
        PrefixIns( back, ins, high );
    } else {
        high = not_equal;              /* for return value*/
    }
    if( not_equal != NULL ) {
        DupSeg( ins, not_equal );
        PrefixIns( back, ins, not_equal );
    }
    DupSeg( ins, low );
    ReplIns( back, ins, low );
    return( high );
}

