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
* Description:  Architecture independent instruction splitting (reductions).
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "regset.h"
#include "confldef.h"
#include "makeins.h"
#include "namelist.h"
#include "regalloc.h"
#include "rgtbl.h"
#include "split.h"
#include "insutil.h"
#include "optab.h"
#include "inssegs.h"
#include "revcond.h"
#include "conflict.h"
#include "_split.h"
#include "backrun.hpp"


const type_class_def    HalfClass[] = {
    static_cast<const type_class_def>(0),                  /* U1 */
    static_cast<const type_class_def>(0),                  /* I1 */
    U1,                 /* U2 */
    I1,                 /* I2 */
    U2,                 /* U4 */
    I2,                 /* I4 */
    U4,                 /* U8 */
    I4,                 /* I8 */
    U2,                 /* CP */
    U2,                 /* PT */
    I2,                 /* FS */
    I4,                 /* FD */
    U4,                 /* FL */
    U4                  /* XX */
};

const type_class_def    DoubleClass[] = {
/*******************************/

    U2,                 /* U1 */
    I2,                 /* I1 */
    U4,                 /* U2 */
    I4,                 /* I2 */
    U8,                 /* U4 */
    I8,                 /* I4 */
    XX,                 /* U8 */
    XX,                 /* I8 */
    static_cast<const type_class_def>(0),                  /* CP */
    static_cast<const type_class_def>(0),                  /* PT */
    static_cast<const type_class_def>(0),                  /* FS */
    static_cast<const type_class_def>(0),                  /* FD */
    static_cast<const type_class_def>(0),                  /* FL */
    XX                  /* XX */
};

const type_class_def    Unsigned[] = {
/****************************/

    U1,                 /* U1 */
    U1,                 /* I1 */
    U2,                 /* U2 */
    U2,                 /* I2 */
    U4,                 /* U4 */
    U4,                 /* I4 */
    U8,                 /* U8 */
    U8,                 /* U8 */
    CP,                 /* CP */
    PT,                 /* PT */
    FS,                 /* FS */
    FD,                 /* FD */
    FL,                 /* FL */
    XX                  /* XX */
};

const type_class_def    Signed[] = {
/**************************/

    I1,                 /* U1 */
    I1,                 /* I1 */
    I2,                 /* U2 */
    I2,                 /* I2 */
    I4,                 /* U4 */
    I4,                 /* I4 */
    I8,                 /* U8 */
    I8,                 /* I8 */
    CP,                 /* CP */
    PT,                 /* PT */
    FS,                 /* FS */
    FD,                 /* FD */
    FL,                 /* FL */
    XX                  /* XX */
};

static  reg_set_index   ResultPossible( instruction *ins )
/********************************************************/
{
    return( RegList[ins->u.gen_table->reg_set].result );
}


static  reg_set_index   Op2Possible( instruction *ins )
/*****************************************************/
{
    return( RegList[ins->u.gen_table->reg_set].right );
}

static  reg_set_index   Op1Possible( instruction *ins )
/*****************************************************/
{
    return( RegList[ins->u.gen_table->reg_set].left );
}

static  reg_set_index   ResPossible( instruction *ins )
/*****************************************************/
{
    return( RegList[ins->u.gen_table->reg_set].result );
}

hw_reg_set      Op1Reg( BackRun& back, instruction *ins )
/****************************************/
{
    const hw_reg_set    *list;

    list = RegSetList(back, Op1Possible( ins ));
    return( *list );
}

hw_reg_set      ResultReg( BackRun& back, instruction *ins )
/*******************************************/
{
    const hw_reg_set    *list;

    list = RegSetList(back, ResultPossible( ins ));
    return( *list );
}

hw_reg_set      ZapReg( BackRun& back, instruction *ins )
/****************************************/
{
    const hw_reg_set    *list;

    list = RegSetList(back, RegList[ins->u.gen_table->reg_set].zap);
    return( *list );
}

instruction *MoveConst( BackRun& back, uint_32 value, name *result, type_class_def type_class )
/******************************************************************************/
{
    return( MakeMove( back, AllocConst( back, CFCnvU32F( back, &back.cfloat.cgh, value ) ), result, type_class ) );
}


instruction *Reduce( BackRun& back, instruction *ins )
/*************************************/
{
    ins->head.state = INS_NEEDS_WORK;
    return( ReduceTab[InsGenerate( ins ) - FIRST_REDUCT]( back, ins ) );
}


void    HalfType( instruction *ins )
/**********************************/
{
    ins->type_class = HalfClass[ins->type_class];
    ins->table = NULL;
    ins->head.state = INS_NEEDS_WORK;
}


void    ChangeType( instruction *ins, type_class_def type_class )
/***************************************************************/
{
    ins->type_class = type_class;
    ins->table = NULL;
    ins->head.state = INS_NEEDS_WORK;
}


static  void    ForceToMemory( BackRun& back, name *name )
/*****************************************/
{
    while( name->v.conflict != NULL ) {
        InMemory( back, name->v.conflict );
    }
}


instruction     *rMOVOP1TEMP( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;
    type_class_def      type_class;
    name                *name;

    type_class = OpClass( ins );
    name = AllocTemp( back, type_class );
    new_ins = MakeMove( back, ins->operands[0], name, type_class );
    ins->operands[0] = name;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rMOVOP2TEMP( BackRun& back, instruction *ins )
/***********************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocTemp( back, ins->type_class );
    new_ins = MakeMove( back, ins->operands[1], name1, ins->type_class );
    ins->operands[1] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rOP1REG( BackRun& back, instruction *ins )
/*******************************************/
{
    instruction         *new_ins;
    name                *name1;
    type_class_def      type_class;

    type_class = OpClass( ins );
    name1 = AllocTemp( back, type_class );
    new_ins = MakeMove( back, ins->operands[0], name1, type_class );
    ins->operands[0] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    MarkPossible( back, ins, name1, Op1Possible( ins ) );
    ins->u.gen_table = NULL;
    GiveRegister( back, NameConflict( back, ins, name1 ), true );
    return( new_ins );
}

instruction      *rOP2REG( BackRun& back, instruction *ins )
/*******************************************/
{
    instruction         *new_ins;
    name                *name1;
    type_class_def      type_class;

    type_class = OpClass( ins );
    name1 = AllocTemp( back, type_class );
    new_ins = MakeMove( back, ins->operands[1], name1, type_class );
    ins->operands[1] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    MarkPossible( back, ins, name1, Op1Possible( ins ) );
    ins->u.gen_table = NULL;
    GiveRegister( back, NameConflict( back, ins, name1 ), true );
    return( new_ins );
}


/* 370 */
instruction      *rMOVRESREG( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocTemp( back, ins->type_class );
    new_ins = MakeMove( back, name1, ins->result, ins->type_class );
    ins->result = name1;
    MoveSegRes( back, ins, new_ins );
    SuffixIns( back, ins, new_ins );
    MarkPossible( back, ins, name1, ResPossible( ins ) );
    ins->u.gen_table = NULL;
    GiveRegister( back, NameConflict( back, ins, name1 ), true );
    return( ins );
}

instruction      *rMOVRESTEMP( BackRun& back, instruction *ins )
/***********************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocTemp( back, ins->type_class );
    new_ins = MakeMove( back, name1, ins->result, ins->type_class );
    ins->result = name1;
    MoveSegRes( back, ins, new_ins );
    SuffixIns( back, ins, new_ins );
    MarkPossible( back, ins, name1, ResPossible( ins ) );
    ins->u.gen_table = NULL;
    return( ins );
}


instruction      *rRESREG( BackRun& back, instruction *ins )
/*******************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocTemp( back, ins->type_class );
    new_ins = MakeMove( back, name1, ins->result, ins->type_class );
    ins->result = name1;
    MoveSegRes( back, ins, new_ins );
    SuffixIns( back, ins, new_ins );
    MarkPossible( back, ins, name1, ResultPossible( ins ) );
    ins->u.gen_table = NULL;
    GiveRegister( back, NameConflict( back, ins, name1 ), true );
    new_ins = ins;
    return( new_ins );
}


instruction      *rOP1RESREG( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;
    instruction         *ins2;
    name                *name1;
    name                *name2;

    name1 = AllocRegName( back, Op1Reg( back, ins ) );
    new_ins = MakeMove( back, ins->operands[0], name1, OpClass( ins ) );
    ins->operands[0] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    name2 = AllocRegName( back, ResultReg( back, ins ) );
    ins2 = MakeMove( back, name2, ins->result, ins->type_class );
    ins->result = name2;
    MoveSegRes( back, ins, ins2 );
    SuffixIns( back, ins, ins2 );
    return( new_ins );
}

/* 370 */
static  bool    CanUseOp1( instruction *ins, name *op1 )
/******************************************************/
{
    name        *name2;

    if( op1->n.class_ != N_REGISTER )
        return( false );
    if( HwOvlap( op1->r.reg, ins->head.next->head.live.regs ) )
        return( false );
    if( ins->result->n.class_ == N_INDEXED ) {
        name2 = ins->result->i.index;
        if( name2->n.class_ == N_REGISTER ) {
            if( HwOvlap( name2->r.reg, op1->r.reg ) ) {
                return( false );
            }
        }
    }
    return( true );
}

/* 370 */
instruction      *rUSEREGISTER( BackRun& back, instruction *ins )
/************************************************/
{
    instruction         *new_ins;
    instruction         *ins2;
    name                *name1;

    name1 = ins->operands[0];
    if( CanUseOp1( ins, name1 ) ) {
        new_ins = MakeMove( back, name1, ins->result, ins->type_class );
        ins->result = name1;
        MoveSegRes( back, ins, new_ins );
        SuffixIns( back, ins, new_ins );
        new_ins = ins;
    } else {
        name1 = AllocTemp( back, ins->type_class );
        new_ins = MakeMove( back, ins->operands[0], name1, ins->type_class );
        CheckCC( ins, new_ins );
        ins->operands[0] = name1;
        MoveSegOp( back, ins, new_ins, 0 );
        PrefixIns( back, ins, new_ins );
        ins2 = MakeMove( back, name1, ins->result, ins->type_class );
        ins->result = name1;
        MoveSegRes( back, ins, ins2 );
        SuffixIns( back, ins, ins2 );
        MarkPossible( back, ins, name1, ResultPossible( ins ) );
        ins->u.gen_table = NULL;
        GiveRegister( back, NameConflict( back, ins, name1 ), true );
    }
    return( new_ins );
}


instruction      *rCHANGESHIFT( BackRun& back, instruction *ins )
/************************************************/
{
    int_32      shift_count;

    shift_count = ins->operands[1]->c.lo.u.int_value;
    assert( shift_count >= 0 );
    ins->operands[1] = AllocS32Const( back, shift_count & ( ( WORD_SIZE * 8 ) - 1 ) );
    return( ins );
}


instruction      *rFIXSHIFT( BackRun& back, instruction *ins )
/*********************************************/
{
    instruction     *new_ins;

    /* Fix up shift instructions that try to shift by too large amounts. This
     * can happen when optimizer merges adjacent shift instructions. Arithmetic
     * rights shifts must never exceed (REGISTER_BITS - 1), logical shifts can
     * be replaced with loads of zero constant.
     */
    if( ins->head.opcode == OP_RSHIFT
      && Signed[ins->type_class] == ins->type_class ) {
        ins->operands[1] = AllocS32Const( back, REG_SIZE * 8 - 1 );
        return( ins );
    } else {
        new_ins = MoveConst( back, 0, ins->result, ins->type_class );
        DupSeg( ins, new_ins );
        ReplIns( back, ins, new_ins );
        return( new_ins );
    }
}


instruction      *rCLRHI_BW( BackRun& back, instruction *ins )
/*********************************************/
{
    instruction         *new_ins;
    instruction         *ins2;
    name                *name1;
    type_class_def      half_type_class;

    half_type_class = HalfClass[ins->type_class];
    name1 = AllocTemp( back, ins->type_class );
    new_ins = MakeMove( back, ins->operands[0], LowPart( back, name1, half_type_class ), half_type_class );
    ins->operands[0] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    ins2 = MoveConst( back, 0, HighPart( back, name1, half_type_class ), half_type_class );
    PrefixIns( back, ins, ins2 );
    ins->head.opcode = OP_MOV;
    ins->table = NULL;
    ins->u.gen_table = NULL;
    return( new_ins );
}


instruction      *rCLRHI_R( BackRun& back, instruction *ins )
/********************************************/
{
    instruction         *new_ins;
    instruction         *and_ins;
    type_class_def      type_class;
    hw_reg_set          high;
    name                *res;
    int_32              value;
    type_class_def      half_type_class;
    name                *op;

    half_type_class = HalfClass[ins->type_class];
    op = ins->operands[0];
    type_class = ins->base_type_class;
    res = ins->result;
    high = HighReg( back, res->r.reg );
    if( op->n.class_ == N_INDEXED
      && ( ( op->i.index->n.class_ == N_REGISTER
      && HwOvlap( op->i.index->r.reg, res->r.reg ) ) /* (1) */
      || ( CheckIndecies( back, ins, res->r.reg, HW_EMPTY, NULL ) == MB_MAYBE ) ) ) { /* 2 */
        /* (1) would have gen'd movzd  eax,[eax]
         * (2) avoid incorrect reduction of "cnv [foo] => BX" to
         *      xor bx,bx / mov bx, [foo] / mov bl, [bx]
         *     (zoiks register allocator)
         */
        new_ins = NULL;
    } else if( !HW_CEqual( high, HW_EMPTY )
      && half_type_class == type_class ) {
        if( op->n.class_ == N_REGISTER
          && HwEqual( op->r.reg, high ) ) {
            /* look out for movzd ax,ah */
            new_ins = NULL;
        } else {
            new_ins = MoveConst( back, 0, HighPart( back, res, type_class ), type_class );
        }
    } else if( op->n.class_ == N_REGISTER
      && HwOvlap( op->r.reg, res->r.reg ) ) {
        /* would have gen'd movzd eax,ah */
        new_ins = NULL;
    } else {
        new_ins = MoveConst( back, 0, res, ins->type_class );
    }
    ins->result = LowPart( back, res, type_class );
    if( new_ins != NULL ) {
        PrefixIns( back, ins, new_ins );
    } else {
        value = 0;
        switch( type_class ) {
        case U1:
        case I1:
            value = 0xff;
            break;
        case U2:
        case I2:
            value = 0xffff;
            break;
        }
        high = res->r.reg;
        HwTurnOff( high, ins->result->r.reg );
        new_ins = MakeNop(back);
        new_ins->zap = reinterpret_cast<register_name *>(AllocRegName( back, high ));
        SuffixIns( back, ins, new_ins ); // don't cause high part to be live on entry

        and_ins = MakeBinary( back, OP_AND, res, AllocS32Const( back, value ), res, ins->type_class );
        SuffixIns( back, new_ins, and_ins );
        new_ins = ins;
    }
    ins->type_class = type_class;
    ins->head.opcode = OP_MOV;
    ins->table = NULL;
    ins->u.gen_table = NULL;
    return( new_ins );
}


instruction      *rCONVERT_LOW( BackRun& back, instruction *ins )
/************************************************/
/* change convert 4 byte==>1 byte to 2 byte ==> 1 byte */
{
    ins->operands[0] = LowPart( back, ins->operands[0], HalfClass[ins->operands[0]->n.type_class] );
    ins->base_type_class = HalfClass[ins->base_type_class];
    ins->table = NULL;
    return( ins );
}


instruction      *rCYPHIGH( BackRun& back, instruction *ins )
/********************************************/
{
    HalfType( ins );
    ins->operands[0] = HighPart( back, ins->operands[0], ins->type_class );
    ins->operands[1] = HighPart( back, ins->operands[1], ins->type_class );
    if( ins->result != NULL
      && ins->head.opcode < FIRST_CONDITION ) {
        ins->result = HighPart( back, ins->result, ins->type_class );
    }
    return( ins );
}


instruction      *rCYPLOW( BackRun& back, instruction *ins )
/*******************************************/
{
    HalfType( ins );
    ins->operands[0] = LowPart( back, ins->operands[0], ins->type_class );
    ins->operands[1] = LowPart( back, ins->operands[1], ins->type_class );
    if( ins->result != NULL
      && ins->head.opcode < FIRST_CONDITION ) {
        ins->result = LowPart( back, ins->result, ins->type_class );
    }
    return( ins );
}


instruction      *rDOUBLEHALF( BackRun& back, instruction *ins )
/***********************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = ins->operands[1];
    name1 = AllocIntConst( back, name1->c.lo.u.int_value >> 1 );
    ins->operands[1] = name1;
    new_ins = MakeBinary( back, ins->head.opcode, ins->operands[0], name1, ins->result, ins->type_class );
    new_ins->table = ins->table;
    DupSeg( ins, new_ins );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rOP1MEM( BackRun& back, instruction *ins )
/*******************************************/
{
    ForceToMemory( back, ins->operands[0] );
    return( ins );
}


instruction      *rOP2MEM( BackRun& back, instruction *ins )
/*******************************************/
{
    ForceToMemory( back, ins->operands[1] );
    return( ins );
}


instruction      *rFORCERESMEM( BackRun& back, instruction *ins )
/************************************************/
{
    ForceToMemory( back, ins->result );
    return( ins );
}


instruction      *rMAKEMOVE( BackRun& back, instruction *ins )
/*********************************************/
{
    instruction         *new_ins;

    new_ins = MakeMove( back, ins->operands[0], ins->result, ins->type_class );
    DupSeg( ins, new_ins );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rMAKEXORRR( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;

    new_ins = MakeBinary( back, OP_XOR, ins->result, ins->result, ins->result, ins->type_class );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rADDRR( BackRun& back, instruction *ins )
/******************************************/
{
    ins->head.opcode = OP_ADD;
    ins->operands[1] = ins->operands[0];
    ins->table = NULL;
    return( ins );
}


instruction      *rMOVOP2( BackRun& back, instruction *ins )
/*******************************************/
{
    instruction         *new_ins;

    new_ins = MakeMove( back, ins->operands[1], ins->result, ins->type_class );
    MoveSegOp( back, ins, new_ins, 0 );
    DupSegRes( back, ins, new_ins );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rMOVOP1RES( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;

    new_ins = MakeMove( back, ins->operands[0], ins->result, ins->type_class );
    ins->operands[0] = ins->result;
    MoveSegOp( back, ins, new_ins, 0 );
    DupSegRes( back, ins, new_ins );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


/* 386 */
instruction      *rMOVOP2RES( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;

    new_ins = MakeMove( back, ins->operands[1], ins->result, ins->type_class );
    ins->operands[1] = ins->result;
    MoveSegOp( back, ins, new_ins, 1 );
    DupSegRes( back, ins, new_ins );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}

/* 370 */
instruction      *rSWAPOPS( BackRun& back, instruction *ins )
/********************************************/
{
    name                *name1;

    name1 = ins->operands[0];
    ins->operands[0] = ins->operands[1];
    ins->operands[1] = name1;
    return( ins );
}


/* 370 */
instruction      *rSWAPCMP( BackRun& back, instruction *ins )
/********************************************/
{
    RevCond( ins );
    return( rSWAPOPS( back, ins ) );
}


instruction      *rMOVEINDEX( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;

    new_ins = MakeMove( back, ins->operands[0]->i.index, ins->result, ins->type_class );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}

/* 370 */
instruction      *rLOADOP2( BackRun& back, instruction *ins )
/********************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocTemp( back, ins->type_class );
    new_ins = MakeMove( back, ins->operands[1], name1, ins->type_class );
    CheckCC( ins, new_ins );
    ins->operands[1] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    MarkPossible( back, ins, name1, Op2Possible( ins ) );
    ins->u.gen_table = NULL;
    GiveRegister( back, NameConflict( back, ins, name1 ), true );
    return( new_ins );
}


instruction      *rNEGADD( BackRun& back, instruction *ins )
/**************************************************/
{
    instruction         *new_ins;

    new_ins = MakeUnary( back, OP_NEGATE, ins->operands[1], AllocTemp( back, U4 ), U4 );
    MoveSegOp( back, ins, new_ins, 0 );
    ins->operands[1] = new_ins->result;
    ins->head.opcode = OP_ADD;
    ins->table = NULL;
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


static  void    NegOp2( BackRun& back, instruction *ins )
/****************************************/
{
    ins->operands[1] = AllocConst( back, CFCnvI32F( back, &back.cfloat.cgh, -ins->operands[1]->c.lo.u.int_value ) );
}


instruction      *rMAKEADD( BackRun& back, instruction *ins )
/********************************************/
{
    ins->head.opcode = OP_ADD;
    NegOp2( back, ins );
    ins->table = NULL;
    return( ins );
}


instruction      *rMAKENEG( BackRun& back, instruction *ins )
/********************************************/
{
    instruction         *new_ins;

    new_ins = MakeUnary( back, OP_NEGATE, ins->operands[1], ins->result, ins->type_class );
    DupSeg( ins, new_ins );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rMAKESUB( BackRun& back, instruction *ins )
/********************************************/
{
    ins->head.opcode = OP_SUB;
    NegOp2( back, ins );
    ins->table = NULL;
    return( ins );
}


instruction      *rCMPTRUE( BackRun& back, instruction *ins )
/********************************************/
{
    DoNothing( back, ins );
    SetBlockIndex( ins, TrueIndex( ins ), TrueIndex( ins ) );
    return( ins );
}


instruction      *rCMPFALSE( BackRun& back, instruction *ins )
/*********************************************/
{
    DoNothing( back, ins );
    SetBlockIndex( ins, FalseIndex( ins ), FalseIndex( ins ) );
    return( ins );
}


instruction      *rOP1RESTEMP( BackRun& back, instruction *ins )
/***********************************************/
{
    instruction         *new_ins;
    instruction         *ins2;
    name                *name1;

    name1 = AllocTemp( back, ins->type_class );
    new_ins = MakeMove( back, ins->operands[0], name1, ins->type_class );
    ins->operands[0] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    ins2 = MakeMove( back, name1, ins->result, ins->type_class );
    ins->result = name1;
    MoveSegRes( back, ins, ins2 );
    SuffixIns( back, ins, ins2 );
    return( new_ins );
}
