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
* Description:  386 instruction splitting (simplifications/optimizations).
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "regset.h"
#include "model.h"
#include "system.h"
#include "zoiks.h"
#include "confldef.h"
#include "makeins.h"
#include "namelist.h"
#include "rgtbl.h"
#include "split.h"
#include "insutil.h"
#include "optab.h"
#include "inssegs.h"
#include "optimize.h"
#include "fixindex.h"
#include "x86split.h"
#include "overlap.h"
#include "x86segs.h"
#include "liveinfo.h"
#include "i64.h"
#include "x86splt2.h"
#include "backrun.hpp"


name    *LowPart( BackRun& back, name *tosplit, type_class_def type_class )
/**********************************************************/
{
    name                *new_part = NULL;
    signed_8            s8;
    unsigned_8          u8;
    int_16              s16;
    uint_16             u16;
    constant_defn       *floatval;

    switch( tosplit->n.class_ ) {
    case N_CONSTANT:
        if( tosplit->c.const_type == CONS_ABSOLUTE ) {
            if( type_class == U1 ) {
                u8 = tosplit->c.lo.u.int_value & 0xff;
                new_part = AllocUIntConst( back, u8 );
            } else if( type_class == I1 ) {
                s8 = tosplit->c.lo.u.int_value & 0xff;
                new_part = AllocIntConst( back, s8 );
            } else if( type_class == U2 ) {
                u16 = tosplit->c.lo.u.int_value & 0xffff;
                new_part = AllocUIntConst( back, u16 );
            } else if( type_class == I2 ) {
                s16 = tosplit->c.lo.u.int_value & 0xffff;
                new_part = AllocIntConst( back, s16 );
            } else if( type_class == I4 ) {
                new_part = AllocS32Const( back, tosplit->c.lo.u.int_value );
            } else if( type_class == U4 ) {
                new_part = AllocU32Const( back, tosplit->c.lo.u.uint_value );
            } else if( type_class == FL ) {
                Zoiks( back, ZOIKS_129 );
            } else { /* FD */
                floatval = GetFloat( back, tosplit, FD );
                new_part = AllocConst( back, CFCnvU32F( back, &back.cfloat.cgh, ( U64LowLE( floatval->buffer.u64 ) ) ) );
            }
        } else if( tosplit->c.const_type == CONS_ADDRESS ) {
            new_part = AddrConst( back, tosplit->c.u.op, (segment_id)tosplit->c.lo.u.int_value, CONS_OFFSET );
        } else {
            Zoiks( back, ZOIKS_044 );
        }
        break;
    case N_REGISTER:
        if( type_class == U1 || type_class == I1 ) {
            new_part = AllocRegName( back, Low16Reg( tosplit->r.reg ) );
        } else if( type_class == U2 || type_class == I2 ) {
            new_part = AllocRegName( back, Low32Reg( tosplit->r.reg ) );
        } else {
            new_part = AllocRegName( back, Low64Reg( tosplit->r.reg ) );
        }
        break;
    case N_TEMP:
        new_part = TempOffset( back, tosplit, 0, type_class );
        if( new_part->t.temp_flags & CONST_TEMP ) {
            name *cons = static_cast<name *>(tosplit->v.symbol);
            if( tosplit->n.type_class == FD ) {
                cons = Int64Equivalent( back, cons );
            }
            new_part->v.symbol = LowPart( back, cons, type_class );
        }
        break;
    case N_MEMORY:
        new_part = AllocMemory( back, tosplit->v.symbol, tosplit->v.offset,
                                tosplit->m.memory_type, type_class );
        new_part->v.usage = tosplit->v.usage;
        break;
    case N_INDEXED:
        new_part = ScaleIndex( back, tosplit->i.index, tosplit->i.base,
                                tosplit->i.constant, type_class,
                                0, tosplit->i.scale, tosplit->i.index_flags );
        break;
    }
    return( new_part );
}

name    *OffsetPart( BackRun& back, name *tosplit )
/**********************************/
{
    name        *new_part;

    switch( tosplit->n.class_ ) {
    case N_REGISTER:
        return( AllocRegName( back, Low48Reg( tosplit->r.reg) ) );
    case N_CONSTANT:
        if( tosplit->c.const_type == CONS_ABSOLUTE ) {
            return( tosplit );
        } else if( tosplit->c.const_type == CONS_ADDRESS ) {
            return( AddrConst( back, tosplit->c.u.op, (segment_id)tosplit->c.lo.u.int_value, CONS_OFFSET ) );
        } else {
            Zoiks( back, ZOIKS_044 );
            return( tosplit );
        }
    case N_TEMP:
        new_part = LowPart( back, tosplit, U4 );
        if( new_part->t.temp_flags & CONST_TEMP ) {
            new_part->v.symbol = OffsetPart( back, static_cast<name *>(tosplit->v.symbol) );
        }
        return( new_part );
    case N_MEMORY:
    case N_INDEXED:
        return( LowPart( back, tosplit, U4 ) );
    default:
        Zoiks( back, ZOIKS_129 );
        return( NULL );
    }
}


name    *SegmentPart( BackRun& back, name *tosplit )
/***********************************/
{
    name        *new_part;
    name        *seg;

    switch( tosplit->n.class_ ) {
    case N_CONSTANT:
        if( tosplit->c.const_type == CONS_ABSOLUTE ) {
            return( AllocIntConst( back, 0 ) );
        } else if( tosplit->c.const_type == CONS_ADDRESS ) {
            return( AddrConst( back, tosplit->c.u.op, (segment_id)tosplit->c.lo.u.int_value, CONS_SEGMENT ) );
        } else {
            Zoiks( back, ZOIKS_044 );
            return( NULL );
        }
        break;
    case N_REGISTER:
        return( AllocRegName( back, High48Reg( tosplit->r.reg ) ) );
    case N_TEMP:
        new_part = TempOffset( back, tosplit, 4, U2 );
        if( new_part->t.temp_flags & CONST_TEMP ) {
            seg = SegmentPart( back, static_cast<name *>(tosplit->v.symbol) );
            if( seg->n.class_ == N_REGISTER )
                return( seg );
            new_part->v.symbol = seg;
        }
        return( new_part );
    case N_MEMORY:
        new_part = AllocMemory( back, tosplit->v.symbol,
                                tosplit->v.offset + 4,
                                tosplit->m.memory_type, U2 );
        new_part->v.usage = tosplit->v.usage;
        return( new_part );
    case N_INDEXED:
        new_part = ScaleIndex( back, tosplit->i.index, tosplit->i.base,
                tosplit->i.constant+ 4, U2, 0,
                tosplit->i.scale, tosplit->i.index_flags );
        return( new_part );
    default:
        Zoiks( back, ZOIKS_129 );
        return( NULL );
    }
}


name    *HighPart( BackRun& back, name *tosplit, type_class_def type_class )
/***********************************************************/
{
    name                *new_part = NULL;
    signed_8            s8;
    unsigned_8          u8;
    int_16              s16;
    uint_16             u16;
    constant_defn       *floatval;

    switch( tosplit->n.class_ ) {
    case N_CONSTANT:
        if( tosplit->c.const_type == CONS_ABSOLUTE ) {
            if( type_class == U1 ) {
                u8 = ( tosplit->c.lo.u.int_value >> 8 ) & 0xff;
                new_part = AllocUIntConst( back, u8 );
            } else if( type_class == I1 ) {
                s8 = ( tosplit->c.lo.u.int_value >> 8 ) & 0xff;
                new_part = AllocIntConst( back, s8 );
            } else if( type_class == U2 ) {
                u16 = ( tosplit->c.lo.u.int_value >> 16 ) & 0xffff;
                new_part = AllocUIntConst( back, u16 );
            } else if( type_class == I2 ) {
                s16 = ( tosplit->c.lo.u.int_value >> 16 ) & 0xffff;
                new_part = AllocIntConst( back, s16 );
            } else if( type_class == I4 ) {
                new_part = AllocS32Const( back, tosplit->c.hi.u.int_value );
            } else if( type_class == U4 ) {
                new_part = AllocU32Const( back, tosplit->c.hi.u.uint_value );
            } else if( type_class == FL ) {
                Zoiks( back, ZOIKS_129 );
            } else { /* FD */
                floatval = GetFloat( back, tosplit, FD );
                new_part = AllocConst( back, CFCnvU32F( back, &back.cfloat.cgh, ( U64HighLE( floatval->buffer.u64 ) ) ) );
            }
        } else if( tosplit->c.const_type == CONS_ADDRESS ) {
            new_part = AddrConst( back, tosplit->c.u.op, (segment_id)tosplit->c.lo.u.int_value, CONS_SEGMENT );
        } else {
            Zoiks( back, ZOIKS_044 );
        }
        break;
    case N_REGISTER:
        if( type_class == U1 || type_class == I1 ) {
            new_part = AllocRegName( back, High16Reg( tosplit->r.reg ) );
        } else if( type_class == U2 || type_class == I2 ) {
            new_part = AllocRegName( back, High32Reg( tosplit->r.reg ) );
        } else {
            new_part = AllocRegName( back, High64Reg( tosplit->r.reg ) );
        }
        break;
    case N_TEMP:
        new_part = TempOffset( back, tosplit, tosplit->n.size / 2, type_class );
        if( new_part->t.temp_flags & CONST_TEMP ) {
            name *cons = static_cast<name *>(tosplit->v.symbol);
            if( tosplit->n.type_class == FD ) {
                cons = Int64Equivalent( back, cons );
            }
            new_part->v.symbol = HighPart( back, cons, type_class );
        }
        break;
    case N_MEMORY:
        new_part = AllocMemory( back, tosplit->v.symbol,
                                tosplit->v.offset + tosplit->n.size / 2,
                                tosplit->m.memory_type, type_class );
        new_part->v.usage = tosplit->v.usage;
        break;
    case N_INDEXED:
        new_part = ScaleIndex( back, tosplit->i.index, tosplit->i.base,
                tosplit->i.constant+ tosplit->n.size / 2, type_class,
                0, tosplit->i.scale, tosplit->i.index_flags );
        break;
    }
    return( new_part );
}


instruction     *SplitUnary( BackRun& back, instruction *ins )
/**********************************************
 * Pushing floating point */
{
    instruction     *new_ins;
    name            *high_res;
    name            *low_res;

    HalfType( ins );
    if( ins->result == NULL ) {
        high_res = NULL;
        low_res = NULL;
    } else {
        high_res = HighPart( back, ins->result, ins->type_class );
        low_res  = LowPart( back, ins->result, ins->type_class );
    }
    new_ins = MakeUnary( back, ins->head.opcode,
                         LowPart( back, ins->operands[0], ins->type_class ),
                         low_res, ins->type_class );
    ins->operands[0] = HighPart( back, ins->operands[0],ins->type_class );
    ins->result = high_res;
    if( ins->head.opcode == OP_PUSH ) {
        DupSeg( ins, new_ins );
        SuffixIns( back, ins, new_ins );
        new_ins = ins;
    } else {
        DupSeg( ins, new_ins );
        PrefixIns( back, ins, new_ins );
    }
    return( new_ins );
}


instruction     *rSPLITPUSH( BackRun& back, instruction *ins )
/**********************************************
 * Pushing a 6 byte pointer */
{
    instruction     *new_ins;
    instruction     *ins2;
    name            *op;
    name            *temp;

    new_ins = MakeUnary( back, ins->head.opcode, OffsetPart( back, ins->operands[0] ), NULL, U4 );
    ins->operands[0] = SegmentPart( back, ins->operands[0] );
    op = ins->operands[0];
    DupSeg( ins, new_ins );
    if( ins->head.opcode == OP_PUSH ) {
        SuffixIns( back, ins, new_ins );
        ChangeType( ins, U2 );
        return( ins );
    } else {
        PrefixIns( back, ins, new_ins );
        if( op->n.class_ != N_REGISTER ) {
            ChangeType( ins, U4 );
            temp = AllocTemp( back, U4 );
            ins2 = MakeMove( back, LowPart( back, temp, U2 ), op, U2 );
            ins->operands[0] = temp;
            MoveSegOp( back, ins, ins2, 1 );
            SuffixIns( back, ins, ins2 );
        } else {
            ChangeType( ins, U2 );
        }
        return( new_ins );
    }
}

instruction     *rMAKEU2( BackRun& back, instruction *ins )
/*******************************************
 * for 48 bit pointers operations */
{
    instruction     *new_ins;
    instruction     *ins2;
    name            *temp;

    if( Overlaps( ins->result, ins->operands[0] )
     || ( ins->num_operands >= 2 &&
            Overlaps( ins->result, ins->operands[1] ) ) ) {
        ChangeType( ins, U4 );
        if( ins->result != NULL ) {
            new_ins = MakeMove( back, SegmentPart( back, ins->operands[0] ), AllocTemp( back, U2 ), U2 );
            temp = SegmentPart( back, ins->result );
            ins->result = OffsetPart( back, ins->result );
            DupSegOp( back, ins, new_ins, 0 );
            PrefixIns( back, ins, new_ins );
        } else {
            temp = NULL;
            new_ins = ins;
        }
        ins->operands[0] = OffsetPart( back, ins->operands[0] );
        if( ins->result != NULL ) {
            ins2 = MakeMove( back, new_ins->result, temp, U2 );
            DupSegRes( back, ins, ins2 );
            SuffixIns( back, ins, ins2 );
        }
    } else {
        ChangeType( ins, U4 );
        if( ins->result != NULL ) {
            new_ins = MakeMove( back, SegmentPart( back, ins->operands[0] ), SegmentPart( back, ins->result ), U2 );
            ins->result = OffsetPart( back, ins->result );
            DupSegOp( back, ins, new_ins, 0 );
            DupSegRes( back, ins, new_ins );
            PrefixIns( back, ins, new_ins );
        } else {
            new_ins = ins;
        }
        ins->operands[0] = OffsetPart( back, ins->operands[0] );
        if( OpcodeNumOperands( ins ) >= 2 ) {
            ins->operands[1] = OffsetPart( back, ins->operands[1] );
        }
    }
    return( new_ins );
}


instruction      *rLOADLONGADDR( BackRun& back, instruction *ins )
/*************************************************/
{
    instruction         *new_ins;
    name                *name1;

    new_ins = MakeMove( back, SegName( back, ins->operands[0] ),
                            SegmentPart( back, ins->result ), U2 );
    ChangeType( ins, U4 );
    ins->result = OffsetPart( back, ins->result );
    name1 = ins->operands[0];
    if( name1->n.class_ == N_INDEXED ) {
        if( name1->i.index->n.size == 6 ) {
            ins->operands[0] = ScaleIndex(
                back, OffsetPart( back, name1->i.index ),
                name1->i.base, name1->i.constant,
                name1->n.type_class, name1->n.size,
                name1->i.scale, name1->i.index_flags );
        }
    }
    DupSegRes( back, ins, new_ins );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction     *rHIGHCMP( BackRun& back, instruction *ins )
/********************************************
 * floating point comparison with 0 */
{
    if( ins->type_class == FD ) {
        ins->operands[0] = HighPart( back, ins->operands[0], SW );
    }
    ChangeType( ins, SW );
    return( ins );
}


instruction     *rMAKEU4( BackRun& back, instruction *ins )
/*******************************************
 * change pointer '==' or '!=' to 6 byte compare */
{
    name                *left;
    name                *rite;
    instruction         *new_ins;
    instruction         *low;
    instruction         *high;
    instruction         *first;
    cond_dst_idx        true_idx;
    cond_dst_idx        false_idx;
    name                *lseg;
    name                *rseg;
    name                *temp;

    left = ins->operands[0];
    rite = ins->operands[1];
    true_idx = TrueIndex( ins );
    false_idx = FalseIndex( ins );
    if( ins->head.opcode == OP_CMP_NOT_EQUAL ) {
        low = MakeCondition( back, ins->head.opcode, OffsetPart( back, left ),
                             OffsetPart( back, rite ), true_idx, NO_JUMP, U4 );
    } else {
        low = MakeCondition( back, ins->head.opcode, OffsetPart( back, left ),
                             OffsetPart( back, rite ), false_idx, NO_JUMP, U4 );
    }
    lseg = SegmentPart( back, left );
    first = NULL;
    if( lseg->n.class_ == N_REGISTER && ( HW_COvlap( lseg->r.reg, HW_SEGS ) ) ) {
        temp = AllocTemp( back, U2 );
        new_ins = MakeMove( back, lseg, temp, U2 );
        PrefixIns( back, ins, new_ins );
        if( first == NULL )
            first = new_ins;
        lseg = temp;
    }
    rseg = SegmentPart( back, rite );
    if( rseg->n.class_ == N_REGISTER && ( HW_COvlap( rseg->r.reg, HW_SEGS ) ) ) {
        temp = AllocTemp( back, U2 );
        new_ins = MakeMove( back, rseg, temp, U2 );
        PrefixIns( back, ins, new_ins );
        if( first == NULL )
            first = new_ins;
        rseg = temp;
    }
    high = MakeCondition( back, ins->head.opcode, lseg,
                          rseg, true_idx, false_idx, U2 );
    if( low->head.opcode == OP_CMP_EQUAL ) {
        low->head.opcode = OP_CMP_NOT_EQUAL;
    }
    if( first == NULL )
        first = low;
    DupSeg( ins, low );
    PrefixIns( back, ins, low );
    DupSeg( ins, high );
    ReplIns( back, ins, high );
    UpdateLive( back, first, high );
    return( first );
}


instruction     *rCLRHI_D( BackRun& back, instruction *ins )
/*******************************************/
{
    name                *high;
    name                *low;
    instruction         *new_ins;
    type_class_def      half_type_class;

    half_type_class = HalfClass[ins->type_class];
    low = LowPart( back, ins->result, half_type_class );
    high = HighPart( back, ins->result, half_type_class );
    ChangeType( ins, half_type_class );
    ins->head.opcode = OP_MOV;
    ins->result = low;
    new_ins = MakeMove( back, AllocS32Const( back, 0 ), high, half_type_class );
    DupSegRes( back, ins, new_ins );         /* 2004-11-01  RomanT (same as bug #341) */
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction     *rEXT_PUSH1( BackRun& back, instruction *ins )
/*********************************************/
{
    name        *temp;
    instruction *new_ins;

    temp = AllocTemp( back, U4 );
    new_ins = MakeConvert( back, ins->operands[0], temp, U4, U1 );
    ins->operands[0] = temp;
    ChangeType( ins, U4 );
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction     *rEXT_PUSH2( BackRun& back, instruction *ins )
/*********************************************/
{
    name        *temp;
    instruction *new_ins;

    temp = AllocTemp( back, U4 );
    new_ins = MakeConvert( back, ins->operands[0], temp, U4, U2 );
    ins->operands[0] = temp;
    ChangeType( ins, U4 );
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}

instruction     *rINTCOMP( BackRun& back, instruction *ins )
/*******************************************/
{
    name                *left;
    name                *right;
    instruction         *low;
    instruction         *high;
    type_class_def      half_type_class;
    cond_dst_idx        true_idx;
    cond_dst_idx        false_idx;
    cond_dst_idx        first_idx;
    bool                rite_is_zero;

    rite_is_zero = CFTest( ins->operands[1]->c.u.cfval ) == 0;
    if( ins->type_class == FS ) {
        ChangeType( ins, I4 );
        // V_INTCOMP ensures that this is only called
        // for OP_CMP_EQUAL and OP_CMP_NOT_EQUAL if
        // rite_is_zero
        if( rite_is_zero ) {
            ins->operands[1] = AllocS32Const( back, 0x7fffffff );
            if( ins->head.opcode == OP_CMP_EQUAL ) {
                ins->head.opcode = OP_BIT_TEST_FALSE;
            } else if( ins->head.opcode == OP_CMP_NOT_EQUAL ) {
                ins->head.opcode = OP_BIT_TEST_TRUE;
            }
        } else {
            ins->operands[1] = IntEquivalent( back, ins->operands[1] );
        }
        return( ins );
    }
    half_type_class = HalfClass[ins->type_class];
    left = ins->operands[0];
    if( left->n.class_ == N_CONSTANT && left->c.const_type == CONS_ABSOLUTE ) {
        CnvOpToInt( back, ins, 0 );
        left = ins->operands[0];
    }
    right = ins->operands[1];
    if( right->n.class_ == N_CONSTANT && right->c.const_type == CONS_ABSOLUTE ) {
        CnvOpToInt( back, ins, 1 );
        right = ins->operands[1];
    }
    true_idx = TrueIndex( ins );
    false_idx = FalseIndex( ins );
    first_idx = ( ins->head.opcode == OP_CMP_EQUAL ) ? false_idx : true_idx;
    if( rite_is_zero ) {
        high = MakeCondition( back, OP_BIT_TEST_TRUE,
                        HighPart( back, left, half_type_class ),
                        AllocS32Const( back, 0x7fffffff ),
                        first_idx, NO_JUMP, half_type_class );
    } else {
        high = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, left, half_type_class ),
                        HighPart( back, right, half_type_class ),
                        first_idx, NO_JUMP, half_type_class );
    }
    DupSeg( ins, high );
    PrefixIns( back, ins, high );
    low = MakeCondition( back, ins->head.opcode,
                    LowPart( back, left, half_type_class ),
                    LowPart( back, right, half_type_class ),
                    true_idx, false_idx,
                    half_type_class );
    DupSeg( ins, low );
    ReplIns( back, ins, low );
    return( high );
}

instruction     *rCDQ( BackRun& back, instruction *ins )
/***************************************/
{
    instruction *ins2;
    name        *high;

    high = HighPart( back, ins->result, WD );
    ins->result = LowPart( back, ins->result, WD );
    ins->head.opcode = OP_MOV;
    ins->type_class = WD;
    ins->base_type_class = WD;
    ins->table = NULL;
    ins2 = MakeBinary( back, OP_RSHIFT, ins->operands[0], AllocIntConst( back, 31 ), high, SW );
    DupSeg( ins, ins2 );
    SuffixIns( back, ins, ins2 );
    return( ins );
}

instruction     *rCONVERT_UP( BackRun& back, instruction *ins )
/**********************************************/
{
    name                *temp;
    instruction         *ins1;
    instruction         *ins2;
    type_class_def      type_class;

    // change a CNV I8 I1 op -> res into a pair of instructions
    //          CNV I4 I1 op -> temp and CNV I8 I4 temp -> res
    //
    // 2005-09-25 RomanT
    // Fixed wrong type of temp.variable (must have sign of source operand).
    // Now it goes: U1/2->U4->U8/I8, I1/2->I4->U8/I8.
    //
    type_class = ( IsSigned( ins->base_type_class ) ) ? I4 : U4;
    temp = AllocTemp( back, type_class );
    ins1 = MakeConvert( back, ins->operands[0], temp, type_class, ins->base_type_class );
    DupSegOp( back, ins, ins1, 0 );    // 2005-09-25 RomanT (bug #341)
    PrefixIns( back, ins, ins1 );
    ins2 = MakeConvert( back, temp, ins->result, ins->type_class, type_class );
    DupSegRes( back, ins, ins2 );      // 2004-11-01 RomanT (same as bug #341, *pInt64=char)
    ReplIns( back, ins, ins2 );
    return( ins1 );
}

instruction     *rCYP_SEX( BackRun& back, instruction *ins )
/*******************************************/
{
    name            *op;
    name            *new_op;
    instruction     *shft;
    int             size;

    op = ins->operands[0];
    size = WORD_SIZE - op->n.size;
    new_op = OpAdjusted( back, op, -size, SW );
    ChangeType( ins, SW );
    ins->operands[0] = new_op;
    ins->head.opcode = OP_MOV;
    op = ins->result;
    shft = MakeBinary( back, OP_RSHIFT, op, AllocIntConst( back, size * 8 ), op, SW );
    DupSeg( ins, shft );
    SuffixIns( back, ins, shft );
    return( ins );
}

instruction     *rSPLIT8( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rSPLIT8BIN( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rSPLIT8NEG( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rSPLIT8TST( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rSPLIT8CMP( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rCYPSHIFT( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rBYTESHIFT( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rMOVE8LOW( BackRun& back, instruction *ins ) { return( ins ); }
