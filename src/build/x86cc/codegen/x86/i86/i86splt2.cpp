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
* Description:  i86 instruction splitting (simplifications/optimizations).
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
#include "overlap.h"
#include "x86split.h"
#include "x86segs.h"
#include "confldef.h"
#include "liveinfo.h"
#include "i64.h"
#include "x86splt2.h"
#include "backrun.hpp"


typedef struct eight_byte_name {
    name        *low;
    name        *mid_low;
    name        *mid_high;
    name        *high;
} eight_byte_name;

name    *LowPart( BackRun& back, name *tosplit, type_class_def type_class )
/***********************************************************/
{
    name                *new_part = NULL;
    name                *new_cons;
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
            } else if( type_class == FL ) {
                Zoiks( back, ZOIKS_125 );
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
        } else {
            new_part = AllocRegName( back, Low32Reg( tosplit->r.reg ) );
        }
        break;
    case N_TEMP:
        new_part = TempOffset( back, tosplit, 0, type_class );
        if( new_part->t.temp_flags & CONST_TEMP ) {
            if( tosplit->n.type_class == FS ) {
                new_cons = IntEquivalent( back, static_cast<name *>(tosplit->v.symbol) );
            } else {
                new_cons = static_cast<name *>(tosplit->v.symbol);
            }
            new_part->v.symbol = LowPart( back, new_cons, type_class );
        }
        break;
    case N_MEMORY:
        new_part = AllocMemory( back, tosplit->v.symbol, tosplit->v.offset,
                                tosplit->m.memory_type, type_class );
        new_part->v.usage = tosplit->v.usage;
        break;
    case N_INDEXED:
        new_part = ScaleIndex( back, tosplit->i.index, tosplit->i.base,
                                tosplit->i.constant, type_class, 0, tosplit->i.scale,
                                tosplit->i.index_flags );
        break;
    default:
        break;
    }
    return( new_part );
}


name    *HighPart( BackRun& back, name *tosplit, type_class_def type_class )
/***********************************************************/
{
    name                *new_part = NULL;
    name                *new_cons;
    name                *op;
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
            } else if( type_class == FL ) {
                Zoiks( back, ZOIKS_125 );
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
        } else {
            new_part = AllocRegName( back, High32Reg( tosplit->r.reg ) );
        }
        break;
    case N_TEMP:
        new_part = TempOffset( back, tosplit, tosplit->n.size / 2, type_class );
        if( new_part->t.temp_flags & CONST_TEMP ) {
            if( tosplit->n.type_class == FS ) {
                new_cons = IntEquivalent( back, static_cast<name *>(tosplit->v.symbol) );
            } else {
                new_cons = static_cast<name *>(tosplit->v.symbol);
            }
            op = HighPart( back, new_cons, type_class );
            if( op->n.class_ == N_REGISTER )
                return( op );
            new_part->v.symbol = op;
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
                tosplit->i.constant+ tosplit->n.size / 2, type_class, 0,
                tosplit->i.scale, tosplit->i.index_flags );
        break;
    default:
        break;
    }
    return( new_part );
}


name    *OffsetPart( BackRun& back, name *tosplit )
/**********************************/
{
    return( LowPart( back, tosplit, U2 ) );
}


name    *SegmentPart( BackRun& back, name *tosplit )
/***********************************/
{
    return( HighPart( back, tosplit, U2 ) );
}


instruction     *SplitUnary( BackRun& back, instruction *ins )
/*********************************************/
{
    instruction *new_ins;
    name        *high_res;
    name        *low_res;

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

static  instruction     *SplitOverlapped( BackRun& back, instruction *ins, opcnt op )
/********************************************************************/
/* Used to split 8 byte operation because of overlap problems */
{
    instruction         *move;
    name                *tmp;

    tmp = AllocTemp( back, ins->type_class );
    move = MakeMove( back, ins->operands[op], tmp, ins->type_class );
    MoveSegOp( back, ins, move, op );
    ins->operands[op] = tmp;
    PrefixIns( back, ins, move );
    UpdateLive( back, move, ins );
    return( move );
}


static  void    Split8Name( BackRun& back, instruction *ins, name *tosplit, eight_byte_name *out )
/*********************************************************************************/
{
    type_length         offset;
    cg_sym_handle       symbol;
    constant_defn       *floatval;
    cg_class            cg;
    eight_byte_name     konst;

    out->low      = NULL;
    out->mid_low  = NULL;
    out->mid_high = NULL;
    out->high     = NULL;

    if( tosplit == NULL )
        return;
    switch( tosplit->n.class_ ) {
    case N_REGISTER:
        out->low      = AllocRegName( back, HW_DX );
        out->mid_low  = AllocRegName( back, HW_CX );
        out->mid_high = AllocRegName( back, HW_BX );
        out->high     = AllocRegName( back, HW_AX );
        break;
    case N_INDEXED:
        offset = tosplit->i.constant;
        out->low      = ScaleIndex( back, tosplit->i.index,
                        tosplit->i.base, offset, I2, 0, tosplit->i.scale,
                        tosplit->i.index_flags );
        out->mid_low  = ScaleIndex( back, tosplit->i.index,
                        tosplit->i.base, offset + 2, I2, 0, tosplit->i.scale,
                        tosplit->i.index_flags );
        out->mid_high = ScaleIndex( back, tosplit->i.index,
                        tosplit->i.base, offset + 4, I2, 0, tosplit->i.scale,
                        tosplit->i.index_flags );
        out->high     = ScaleIndex( back, tosplit->i.index,
                        tosplit->i.base, offset + 6, I2, 0, tosplit->i.scale,
                        tosplit->i.index_flags );
        break;
    case N_TEMP:
        out->low      = TempOffset( back, tosplit, 0, I2 );
        out->mid_low  = TempOffset( back, tosplit, 2, I2 );
        out->mid_high = TempOffset( back, tosplit, 4, I2 );
        out->high     = TempOffset( back, tosplit, 6, I2 );
        if( tosplit->t.temp_flags & CONST_TEMP ) {
            Split8Name( back, ins, static_cast<name *>(tosplit->v.symbol), &konst );
            out->low->v.symbol          = konst.low;
            out->mid_low->v.symbol      = konst.mid_low;
            out->mid_high->v.symbol     = konst.mid_high;
            out->high->v.symbol         = konst.high;
        }
        break;
    case N_MEMORY:
        symbol = static_cast<cg_sym_handle>(tosplit->v.symbol);
        cg = tosplit->m.memory_type;
        offset = tosplit->v.offset;
        out->low      = AllocMemory( back, symbol, offset, cg, I2 );
        out->mid_low  = AllocMemory( back, symbol, offset + 2, cg, I2 );
        out->mid_high = AllocMemory( back, symbol, offset + 4, cg, I2 );
        out->high     = AllocMemory( back, symbol, offset + 6, cg, I2 );
        out->low->v.usage      = tosplit->v.usage;
        out->mid_low->v.usage  = tosplit->v.usage;
        out->mid_high->v.usage = tosplit->v.usage;
        out->high->v.usage     = tosplit->v.usage;
        break;
    case N_CONSTANT:
        switch( ins->type_class ) {
        case I8:
        case U8:
            out->low      = AllocIntConst( back, ( tosplit->c.lo.u.uint_value & 0xffff ) );
            out->mid_low  = AllocIntConst( back, ( tosplit->c.lo.u.uint_value >> 16 ) );
            out->mid_high = AllocIntConst( back, ( tosplit->c.hi.u.uint_value & 0xffff ) );
            out->high     = AllocIntConst( back, ( tosplit->c.hi.u.uint_value >> 16 ) );
            break;
        case FD:
            floatval = GetFloat( back, tosplit, FD );
            out->low      = AllocIntConst( back, ( U64Word( floatval->buffer.u64, 0 ) ) );
            out->mid_low  = AllocIntConst( back, ( U64Word( floatval->buffer.u64, 1 ) ) );
            out->mid_high = AllocIntConst( back, ( U64Word( floatval->buffer.u64, 2 ) ) );
            out->high     = AllocIntConst( back, ( U64Word( floatval->buffer.u64, 3 ) ) );
            break;
        default:
            Zoiks( back, ZOIKS_136 );
            break;
        }
        break;
    default:
        Zoiks( back, ZOIKS_135 );
        break;
    }
}

instruction     *rMOVE8LOW( BackRun& back, instruction *ins )
/********************************************/
/*   Move low 4 bytes of 8 byte thingy */
{
    instruction         *lo_ins;
    instruction         *mid_lo_ins;
    eight_byte_name     left;

    if( IndexOverlaps( ins, 0 ) ) {
        return( SplitOverlapped( back, ins, 0 ) );
    }
    Split8Name( back, ins, ins->operands[0], &left );
    lo_ins = MakeMove( back, left.low, LowPart( back, ins->result, U2 ), U2 );
    mid_lo_ins = MakeMove( back, left.mid_low, HighPart( back, ins->result, U2 ), U2 );
    DupSeg( ins, lo_ins );
    DupSeg( ins, mid_lo_ins );
    PrefixIns( back, ins, lo_ins );
    ReplIns( back, ins, mid_lo_ins );
    UpdateLive( back, lo_ins, mid_lo_ins );
    return( lo_ins );
}

instruction     *rSPLIT8( BackRun& back, instruction *ins )
/*******************************************/
/* Used to split 8 byte push, pop and move */
{
    opcode_defs         opcode;
    instruction         *lo_ins;
    instruction         *mid_lo_ins;
    instruction         *mid_hi_ins;
    instruction         *hi_ins;
    instruction         *first_ins;
    instruction         *last_ins;
    eight_byte_name     result;
    eight_byte_name     left;

    if( IndexOverlaps( ins, 0 ) ) {
        return( SplitOverlapped( back, ins, 0 ) );
    }
    Split8Name( back, ins, ins->operands[0], &left );
    Split8Name( back, ins, ins->result, &result );
    opcode = ins->head.opcode;
    lo_ins = MakeUnary( back, opcode, left.low, result.low, U2 );
    mid_lo_ins = MakeUnary( back, opcode, left.mid_low, result.mid_low, U2 );
    mid_hi_ins = MakeUnary( back, opcode, left.mid_high, result.mid_high, U2 );
    hi_ins = MakeUnary( back, opcode, left.high, result.high, U2 );
    DupSeg( ins, lo_ins );
    DupSeg( ins, mid_lo_ins );
    DupSeg( ins, mid_hi_ins );
    DupSeg( ins, hi_ins );
    if( opcode == OP_POP ) {       /* do low first*/
        first_ins = lo_ins;
        PrefixIns( back, ins, lo_ins );
        PrefixIns( back, ins, mid_lo_ins );
        PrefixIns( back, ins, mid_hi_ins );
        ReplIns( back, ins, hi_ins );
        last_ins = hi_ins;
    } else {
        first_ins = hi_ins;
        PrefixIns( back, ins, hi_ins );
        PrefixIns( back, ins, mid_hi_ins );
        PrefixIns( back, ins, mid_lo_ins );
        ReplIns( back, ins, lo_ins );
        last_ins = lo_ins;
    }
    UpdateLive( back, first_ins, last_ins );
    return( first_ins );
}

instruction     *rSPLIT8BIN( BackRun& back, instruction *ins )
/*********************************************/
/*   Used to split 8 byte binary operations (ADD,SUB,Logicals) */
{
    opcode_defs         opcode;
    instruction         *lo_ins;
    instruction         *mid_lo_ins;
    instruction         *mid_hi_ins;
    instruction         *hi_ins;
    eight_byte_name     result;
    eight_byte_name     left;
    eight_byte_name     rite;

    if( IndexOverlaps( ins, 0 ) ) {
        return( SplitOverlapped( back, ins, 0 ) );
    }
    if( IndexOverlaps( ins, 1 ) ) {
        return( SplitOverlapped( back, ins, 1 ) );
    }
    Split8Name( back, ins, ins->operands[0], &left );
    Split8Name( back, ins, ins->operands[1], &rite );
    Split8Name( back, ins, ins->result, &result );
    opcode = ins->head.opcode;
    lo_ins = MakeBinary( back, opcode, left.low, rite.low, result.low, U2 );
    switch( opcode ) {
    case OP_ADD:
        opcode = OP_EXT_ADD;
        break;
    case OP_SUB:
        opcode = OP_EXT_SUB;
        break;
    default:
        break;
    }
    mid_lo_ins = MakeBinary( back, opcode, left.mid_low, rite.mid_low, result.mid_low, U2 );
    mid_hi_ins = MakeBinary( back, opcode, left.mid_high, rite.mid_high, result.mid_high, U2 );
    hi_ins = MakeBinary( back, opcode, left.high, rite.high, result.high, U2 );
    switch( opcode ) {
    case OP_EXT_ADD:
    case OP_EXT_SUB:
        hi_ins->table = CodeTable( back, hi_ins );
        mid_hi_ins->table = hi_ins->table;
        mid_hi_ins->ins_flags |= INS_CC_USED;
        mid_lo_ins->table = hi_ins->table;
        mid_lo_ins->ins_flags |= INS_CC_USED;
        lo_ins->table = hi_ins->table;
        lo_ins->ins_flags |= INS_CC_USED;
        break;
    default:
        break;
    }
    DupSeg( ins, lo_ins );
    DupSeg( ins, mid_lo_ins );
    DupSeg( ins, mid_hi_ins );
    DupSeg( ins, hi_ins );
    PrefixIns( back, ins, lo_ins );
    PrefixIns( back, ins, mid_lo_ins );
    PrefixIns( back, ins, mid_hi_ins );
    ReplIns( back, ins, hi_ins );
    UpdateLive( back, lo_ins, hi_ins );
    return( lo_ins );
}

instruction     *rSPLIT8NEG( BackRun& back, instruction *ins )
/*********************************************/
/*   Used to split 8 byte negate */
{
    instruction         *new_ins;

    /*  OK, so we're not really splitting the thing. But it works just
     *  as well.
     */
    new_ins = MakeBinary( back, OP_SUB, AllocIntConst( back, 0 ), ins->operands[0],
                        ins->result, ins->type_class );
    DupSeg( ins, new_ins );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}

instruction     *rSPLIT8TST( BackRun& back, instruction *ins )
/*********************************************/
/*   Used to split 8 byte test */
{
    eight_byte_name     left;
    eight_byte_name     rite;
    cond_dst_idx        true_idx;
    cond_dst_idx        false_idx;
    opcode_defs         opcode;
    instruction         *lo_ins;
    instruction         *mid_lo_ins;
    instruction         *mid_hi_ins;
    instruction         *hi_ins;

    Split8Name( back, ins, ins->operands[0], &left );
    Split8Name( back, ins, ins->operands[1], &rite );
    true_idx = TrueIndex( ins );
    false_idx = FalseIndex( ins );
    opcode = ins->head.opcode;
    lo_ins = MakeCondition( back, opcode, left.low, rite.low, true_idx, false_idx, U2 );
    switch( opcode ) {
    case OP_BIT_TEST_FALSE:
        opcode = OP_BIT_TEST_TRUE;
        true_idx = false_idx;
        break;
    default:
        break;
    }
    false_idx = NO_JUMP;
    mid_lo_ins = MakeCondition( back, opcode, left.mid_low, rite.mid_low, true_idx, false_idx, U2 );
    mid_hi_ins = MakeCondition( back, opcode, left.mid_high, rite.mid_high, true_idx, false_idx, U2 );
    hi_ins = MakeCondition( back, opcode, left.high, rite.high, true_idx, false_idx, U2 );
    DupSeg( ins, lo_ins );
    DupSeg( ins, mid_lo_ins );
    DupSeg( ins, mid_hi_ins );
    DupSeg( ins, hi_ins );
    PrefixIns( back, ins, hi_ins );
    PrefixIns( back, ins, mid_hi_ins );
    PrefixIns( back, ins, mid_lo_ins );
    ReplIns( back, ins, lo_ins );
    UpdateLive( back, hi_ins, lo_ins );
    return( hi_ins );
}

instruction     *rSPLIT8CMP( BackRun& back, instruction *ins )
/*********************************************/
/*   Used to split 8 byte compare */
{
    eight_byte_name     left;
    eight_byte_name     rite;
    cond_dst_idx        true_idx;
    cond_dst_idx        false_idx;
    instruction         *new_ins[8];
    type_class_def      high_type_class;
    unsigned            i;
    unsigned            j;

/*
 * 2005-04-06 RomanT (bug #407)
 * I removed calls do DoNothing(), it seems ok, extra jumps are perfectly
 * optimized out in other places of compiler. Calling DoNothing() on chain
 * of conditions to reuse existing CC flags is ugly and causes unpredictable
 * logical faults in other places.
 */
    Split8Name( back, ins, ins->operands[0], &left );
    Split8Name( back, ins, ins->operands[1], &rite );
    true_idx = TrueIndex( ins );
    false_idx = FalseIndex( ins );
    high_type_class = HalfClass[HalfClass[ins->type_class]];
    i = 0;
    switch( ins->head.opcode ) {
    case OP_CMP_EQUAL:
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.high, rite.high,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_high, rite.mid_high,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_low, rite.mid_low,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_EQUAL, left.low, rite.low,
                                        true_idx, false_idx, U2 );
        break;
    case OP_CMP_NOT_EQUAL:
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.high, rite.high,
                                        true_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_high, rite.mid_high,
                                        true_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_low, rite.mid_low,
                                        true_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.low, rite.low,
                                        true_idx, false_idx, U2 );
        break;
    case OP_CMP_GREATER:
    case OP_CMP_GREATER_EQUAL:
        new_ins[i++] = MakeCondition( back, OP_CMP_GREATER, left.high, rite.high,
                                        true_idx, NO_JUMP, high_type_class );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.high, rite.high,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_GREATER, left.mid_high, rite.mid_high,
                                        true_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_high, rite.mid_high,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_GREATER, left.mid_low, rite.mid_low,
                                        true_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_low, rite.mid_low,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, ins->head.opcode, left.low, rite.low,
                                        true_idx, false_idx, U2 );
        break;
    case OP_CMP_LESS:
    case OP_CMP_LESS_EQUAL:
        new_ins[i++] = MakeCondition( back, OP_CMP_LESS, left.high, rite.high,
                                        true_idx, NO_JUMP, high_type_class );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.high, rite.high,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_LESS, left.mid_high, rite.mid_high,
                                        true_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_high, rite.mid_high,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_LESS, left.mid_low, rite.mid_low,
                                        true_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, OP_CMP_NOT_EQUAL, left.mid_low, rite.mid_low,
                                        false_idx, NO_JUMP, U2 );
        new_ins[i++] = MakeCondition( back, ins->head.opcode, left.low, rite.low,
                                        true_idx, false_idx, U2 );
        break;
    default:
        Zoiks( back, ZOIKS_125 );
    }
    --i;
    for( j = 0; j < i; ++j ) {
        DupSeg( ins, new_ins[j] );
        PrefixIns( back, ins, new_ins[j] );
    }
    DupSeg( ins, new_ins[i] );
    ReplIns( back, ins, new_ins[i] );
    UpdateLive( back, new_ins[0], new_ins[i] );
    return( new_ins[0] );
}


instruction     *rCLRHI_D( BackRun& back, instruction *ins )
/*******************************************/
/*   Clear the high dword of an 8 byte thingy (zero extend) */
{
    eight_byte_name     result;
    instruction         *move1;
    instruction         *move2;
    instruction         *move3;
    instruction         *move4;

    if( IndexOverlaps( ins,  0 ) ) {
        return( SplitOverlapped( back, ins, 0 ) );
    }
    Split8Name( back, ins, ins->result, &result );
    move1 = MakeMove( back, LowPart( back, ins->operands[0], U2 ), result.low, U2 );
    move2 = MakeMove( back, HighPart( back, ins->operands[0], U2 ), result.mid_low, U2 );
    move3 = MakeMove( back, AllocIntConst( back, 0 ), result.mid_high, U2 );
    move4 = MakeMove( back, AllocIntConst( back, 0 ), result.high, U2 );
    DupSeg( ins, move1 );
    DupSeg( ins, move2 );
    DupSegRes( back, ins, move3 );
    DupSegRes( back, ins, move4 );
    PrefixIns( back, ins, move1 );
    PrefixIns( back, ins, move2 );
    PrefixIns( back, ins, move3 );
    ReplIns( back, ins, move4 );
    UpdateLive( back, move1, move4 );
    return( move1 );
}


instruction     *rCDQ( BackRun& back, instruction *ins )
/***************************************/
/*   Sign extend a 4 byte thingy to an 8 byte thingy */
{
    eight_byte_name     result;
    instruction         *shift;
    instruction         *move1, *move2;
    instruction         *lo_ins;
    instruction         *mid_lo_ins;
    name                *temp;
    name                *high;

    if( IndexOverlaps( ins, 0 ) ) {
        return( SplitOverlapped( back, ins, 0 ) );
    }
    Split8Name( back, ins, ins->result, &result );
    /*
     * 2005-09-26 RomanT
     * Use temp name to avoid creation of instruction where both
     * operand and result have segment prefix (cg cannot handle this)
     */
    temp = AllocTemp( back, I2 );
    high = HighPart( back, ins->operands[0], U2 );
    lo_ins = MakeMove( back, LowPart( back, ins->operands[0], U2 ), result.low, U2 );
    mid_lo_ins = MakeMove( back, high, result.mid_low, U2 );
    shift = MakeBinary( back, OP_RSHIFT, high, AllocIntConst( back, 15 ), temp, I2 );
    move1 = MakeMove( back, temp, result.mid_high, U2 );
    move2 = MakeMove( back, temp, result.high, U2 );
    DupSeg( ins, lo_ins );
    DupSeg( ins, mid_lo_ins );
    DupSeg( ins, shift );
    DupSegRes( back, ins, move1 );
    DupSegRes( back, ins, move2 );
    PrefixIns( back, ins, lo_ins );
    PrefixIns( back, ins, mid_lo_ins );
    PrefixIns( back, ins, shift );
    PrefixIns( back, ins, move1 );
    ReplIns( back, ins, move2 );
    UpdateLive( back, lo_ins, move2 );
    return( lo_ins );
}

static  name    *High8Part( BackRun& back, instruction *ins, name *temp )
/********************************************************/
{
    eight_byte_name     expand;

    Split8Name( back, ins, temp, &expand );
    return( expand.high );
}


instruction     *rCYPSHIFT( BackRun& back, instruction *ins )
/********************************************/
{
    instruction *ins1;
    instruction *ins2;
    instruction *ins3;
    name        *temp;
    unsigned    half_count;

    HalfType( ins );
    temp = AllocTemp( back, ins->type_class );
    half_count = ins->operands[1]->c.lo.u.int_value - temp->n.size * 8;
    if( ins->head.opcode == OP_LSHIFT ) {
        ins1 = MakeBinary( back, OP_LSHIFT,
                        LowPart( back, ins->operands[0], ins->type_class ),
                        AllocIntConst( back, half_count ),
                        temp, ins->type_class );
        ins2 = MakeMove( back, temp, HighPart( back, ins->result, ins->type_class ),
                          ins->type_class );
        ins3 = MoveConst( back, 0, LowPart( back, ins->result, ins->type_class ),
                           ins->type_class );
        DupSegRes( back, ins, ins2 );
    } else {
        ins1 = MakeBinary( back, OP_RSHIFT,
                        HighPart( back, ins->operands[0], ins->type_class ),
                        AllocIntConst( back, half_count ),
                        temp, ins->type_class );
        ins2 = NULL;
        ins3 = MakeConvert( back, temp, ins->result, ins->result->n.type_class,
                            ins->type_class );
    }
    DupSegOp( back, ins, ins1, 0 );
    DupSegRes( back, ins, ins3 );
    ins->operands[0] = AllocIntConst( back, 0 ); // so false live info not gen'd
    PrefixIns( back, ins, ins1 );
    if( ins2 != NULL )
        PrefixIns( back, ins, ins2 );
    ReplIns( back, ins, ins3 );
    UpdateLive( back, ins1, ins3 );
    return( ins1 );
}


instruction     *rBYTESHIFT( BackRun& back, instruction *ins )
/*********************************************/
{
    instruction *ins1;
    instruction *ins2;
    instruction *ins3;
    instruction *ins4;
    name        *op1;
    name        *res;

    op1 = ins->operands[0];
    res = ins->result;
    if( ins->head.opcode == OP_LSHIFT ) {
        ins1 = MakeMove(  back, LowPart( back, HighPart( back, op1, U2 ), U1 ),
                          HighPart( back, HighPart( back, res, U2 ), U1 ), U1 );
        ins2 = MakeMove( back, HighPart(  back, LowPart( back, op1, U2 ), U1 ),
                           LowPart( back, HighPart( back, res, U2 ), U1 ), U1 );
        ins3 = MakeMove(  back, LowPart(  back, LowPart( back, op1, U2 ), U1 ),
                          HighPart(  back, LowPart( back, res, U2 ), U1 ), U1 );
        ins4 = MoveConst( back, 0, LowPart( back, LowPart( back, res, U2 ), U1 ), U1 );
    } else {
        ins1 = MakeMove( back, HighPart(  back, LowPart( back, op1, U2 ), U1 ),
                           LowPart(  back, LowPart( back, res, U2 ), U1 ), U1 );
        ins2 = MakeMove(  back, LowPart( back, HighPart( back, op1, U2 ), U1 ),
                          HighPart(  back, LowPart( back, res, U2 ), U1 ), U1 );
        ins3 = MakeMove( back, HighPart( back, HighPart( back, op1, U2 ), U1 ),
                           LowPart( back, HighPart( back, res, U2 ), U1 ), U1 );
        ins4 = MoveConst( back, 0, HighPart( back, HighPart( back, res, U2 ), U1 ), U1 );
    }
    DupSeg( ins, ins1 );
    DupSeg( ins, ins2 );
    DupSeg( ins, ins3 );
    DupSegRes( back, ins, ins4 );
    PrefixIns( back, ins, ins1 );
    PrefixIns( back, ins, ins2 );
    ins->operands[0] = AllocIntConst( back, 0 );
    PrefixIns( back, ins, ins3 );
    ReplIns( back, ins, ins4 );
    UpdateLive( back, ins1, ins4 );
    return( ins1 );
}

instruction      *rLOADLONGADDR( BackRun& back, instruction *ins )
/*************************************************/
{
    instruction         *new_ins;
    name                *name1;

    new_ins = MakeMove( back, SegName( back, ins->operands[0] ),
                            HighPart( back, ins->result, U2 ), U2 );
    ChangeType( ins, U2 );
    ins->result = LowPart( back, ins->result, U2 );
    name1 = ins->operands[0];
    if( name1->n.class_ == N_INDEXED ) {
        if( name1->i.index->n.size == 4 ) {
            ins->operands[0] = ScaleIndex(
                back, LowPart(back, name1->i.index,U2),
                name1->i.base,
                name1->i.constant,
                name1->n.type_class,
                name1->n.size,
                name1->i.scale,
                name1->i.index_flags );
        }
    }
    DupSegRes( back, ins, new_ins );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction     *rHIGHCMP( BackRun& back, instruction *ins )
/*******************************************/
{
    name                *name1;

    name1 = ins->operands[0];
    switch( ins->type_class ) {
    /* floating point comparison with 0*/
    case FD:
    case I8:
    case U8:
        name1 = High8Part( back, ins, name1 );
        break;
    default:
        name1 = HighPart( back, name1, I2 );
        break;
    }
    ins->operands[0] = name1;
    ChangeType( ins, I2 );
    return( ins );
}


instruction      *rMAKEU4( BackRun& back, instruction *ins )
/*******************************************/
{
    instruction         *new_ins;
    instruction         *ins2;

/* change pointer '==' or '!=' to U4 compare*/
    new_ins = MakeMove( back, ins->operands[0], AllocTemp( back, U4 ), U4 );
    DupSegOp( back, ins, new_ins, 0 );
    ins2 = MakeMove( back, ins->operands[1], AllocTemp( back, U4 ), U4 );
    DupSegOp( back, ins, ins2, 0 );
    ins->operands[0] = new_ins->result;
    ins->operands[1] = ins2->result;
    DelSeg( ins );
    PrefixIns( back, ins, new_ins );
    PrefixIns( back, ins, ins2 );
    ChangeType( ins, U4 );
    return( new_ins );
}

instruction     *rSPLITPUSH( BackRun& back, instruction *ins )
/*********************************************/
/* NOT NEEDED */
{
    return( ins );
}


instruction     *rEXT_PUSH1( BackRun& back, instruction *ins )
/*********************************************/
{
    name        *temp;
    instruction *new_ins;

    temp = AllocTemp( back, U2 );
    new_ins = MakeMove( back, ins->operands[0], LowPart( back, temp, U1 ), U1 );
    ins->operands[0] = temp;
    ChangeType( ins, U2 );
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction     *rEXT_PUSH2( BackRun& back, instruction *ins )
/*********************************************/
/* NOT NEEDED */
{
    return( ins );
}


instruction     *rINTCOMP( BackRun& back, instruction *ins )
/*******************************************/
{
    name                *left;
    name                *rite;
    instruction         *low;
    instruction         *high;
    type_class_def      half_type_class;
    type_class_def      quarter_type_class;
    cond_dst_idx        true_idx;
    cond_dst_idx        false_idx;
    cond_dst_idx        first_idx;
    bool                rite_is_zero;

    rite_is_zero = CFTest( ins->operands[1]->c.u.cfval ) == 0;
    half_type_class = HalfClass[ins->type_class];
    left = ins->operands[0];
    rite = ins->operands[1];
    true_idx = TrueIndex( ins );
    false_idx = FalseIndex( ins );
    if( ins->head.opcode == OP_CMP_EQUAL ) {
        first_idx = false_idx;
    } else {
        first_idx = true_idx;
    }
    if( ins->type_class == FD ) {
        quarter_type_class = HalfClass[half_type_class];
        if( rite_is_zero ) {
            high = MakeCondition( back, OP_BIT_TEST_TRUE,
                        HighPart( back, HighPart( back, left, half_type_class ), quarter_type_class ),
                        AllocIntConst( back, 0x7fff ),
                        first_idx, NO_JUMP, quarter_type_class );
        } else {
            high = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, HighPart( back, left, half_type_class ), quarter_type_class ),
                        HighPart( back, HighPart( back, rite, half_type_class ), quarter_type_class ),
                        first_idx, NO_JUMP, quarter_type_class );
        }
        DupSeg( ins, high );
        PrefixIns( back, ins, high );
        low = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        LowPart( back, HighPart( back, left, half_type_class ), quarter_type_class ),
                        LowPart( back, HighPart( back, rite, half_type_class ), quarter_type_class ),
                        first_idx, NO_JUMP, quarter_type_class );
        DupSeg( ins, low );
        PrefixIns( back, ins, low );
        low = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, LowPart( back, left, half_type_class ), quarter_type_class ),
                        HighPart( back, LowPart( back, rite, half_type_class ), quarter_type_class ),
                        first_idx, NO_JUMP, quarter_type_class );
        DupSeg( ins, low );
        PrefixIns( back, ins, low );
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, LowPart( back, left, half_type_class ), quarter_type_class ),
                        LowPart( back, LowPart( back, rite, half_type_class ), quarter_type_class ),
                        true_idx, false_idx, quarter_type_class );
    } else {
        rite = IntEquivalent( back, rite );
        if( rite_is_zero ) {
            high = MakeCondition( back, OP_BIT_TEST_TRUE,
                        HighPart( back, left, half_type_class ),
                        AllocIntConst( back, 0x7fff ),
                        first_idx, NO_JUMP, half_type_class );
        } else {
            high = MakeCondition( back, OP_CMP_NOT_EQUAL,
                        HighPart( back, left, half_type_class ),
                        HighPart( back, rite, half_type_class ),
                        first_idx, NO_JUMP, half_type_class );
        }
        DupSeg( ins, high );
        PrefixIns( back, ins, high );
        low = MakeCondition( back, ins->head.opcode,
                        LowPart( back, left, half_type_class ),
                        LowPart( back, rite, half_type_class ),
                        true_idx, false_idx,
                        half_type_class );
    }
    DupSeg( ins, low );
    ReplIns( back, ins, low );
    return( high );
}

instruction     *rCONVERT_UP( BackRun& back, instruction *ins ) { return( ins ); }
instruction     *rCYP_SEX( BackRun& back, instruction *ins ) { return( ins ); }

instruction     *rMAKEU2( BackRun& back, instruction *ins )
/******************************************/
{
    instruction *new_ins;
    instruction *ins2;
    name        *temp = NULL;

    if( IndexOverlaps( ins, 0 ) || IndexOverlaps( ins, 1 ) ) {
        ChangeType( ins, U2 );
        if( ins->result != NULL ) {
            new_ins = MakeMove( back, HighPart( back, ins->operands[0], U2 ), AllocTemp( back, U2 ), U2 );
            temp = HighPart( back, ins->result, U2 );
            ins->result = LowPart( back, ins->result, U2 );
            DupSegOp( back, ins, new_ins, 0 );
            PrefixIns( back, ins, new_ins );
        } else {
            new_ins = ins;
        }
        ins->operands[0] = LowPart( back, ins->operands[0], U2 );
        if( ins->operands[1]->n.type_class == U4 || ins->operands[1]->n.type_class == I4 ) {
            ins->operands[1] = LowPart( back, ins->operands[1], U2 );
        }
        if( ins->result != NULL ) {
            ins2 = MakeMove( back, new_ins->result, temp, U2 );
            DupSegRes( back, ins, ins2 );
            SuffixIns( back, ins, ins2 );
        }
    } else {
        ChangeType( ins, U2 );
        if( ins->result != NULL ) {
            new_ins = MakeMove( back, HighPart( back, ins->operands[0], U2 ), HighPart( back, ins->result, U2 ), U2 );
            ins->result = LowPart( back, ins->result, U2 );
            DupSegOp( back, ins, new_ins, 0 );
            DupSegRes( back, ins, new_ins );
            PrefixIns( back, ins, new_ins );
        } else {
            new_ins = ins;
        }
        ins->operands[0] = LowPart( back, ins->operands[0], U2 );
        ins->operands[1] = LowPart( back, ins->operands[1], U2 );
    }
    return( new_ins );
}
