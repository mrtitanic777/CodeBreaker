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
* Description:  Intel i86/386 instruction spliting reductions.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "system.h"
#include "zoiks.h"
#include "makeins.h"
#include "convins.h"
#include "data.h"
#include "utils.h"
#include "namelist.h"
#include "x86objd.h"
#include "regalloc.h"
#include "x86obj.h"
#include "split.h"
#include "insutil.h"
#include "rtcall.h"
#include "inssegs.h"
#include "fixindex.h"
#include "revcond.h"
#include "conflict.h"
#include "x86split.h"
#include "x86segs.h"
#include "liveinfo.h"
#include "x86table.h"
#include "x86rtrn.h"
#include "_split.h"
#include "x86splt1.h"
#include "x86splt2.h"
#include "_x86rtrn.h"
#include "_x86half.h"
#include "i64.h"
#include "backrun.hpp"


instruction *(* const ReduceTab[])( BackRun&, instruction * ) = {
    #define _R_( x, f )     f
    #include "r.h"
    #undef _R_
};


bool    UnChangeable( instruction *ins )
/**************************************/
{
    return( IsString( ins->table ) );
}

instruction      *rSAVEFACE( BackRun& back, instruction *ins )
/*********************************************/
{
    instruction         *new_ins;

    /*
     * we have a EDX:EAX op 1 or DX:AX op 1 here which the constant
     * folder was not able to catch, because of the weird regs
     */
    new_ins = MakeMove( back, AllocRegName( back, HW_xAX ), ins->result, ins->type_class );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}

instruction      *rMULSAVEFACE( BackRun& back, instruction *ins )
/************************************************/
{
    instruction         *new_ins;

    /*
     * we have a r1 mul 1 -> DX:AX here which the constant
     * folder was not able to catch, because of the weird regs
     */
    new_ins = MakeMove( back, ins->operands[0], AllocRegName( back, HW_xAX ), ins->type_class );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}

instruction      *rMAKECYPMUL( BackRun& back, instruction *ins )
/***********************************************/
{
    HalfType( ins );
    return( ins );
}


instruction      *rSEG_SEG( BackRun& back, instruction *ins )
/********************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocTemp( back, U2 );
    new_ins = MakeMove( back, ins->operands[0], name1, U2 );
    ins->operands[0] = name1;
    PrefixIns( back, ins, new_ins );
    MarkPossible( back, ins, name1, RL_WORD );
    ins->u.gen_table = NULL;
    GiveRegister( back, NameConflict( back, ins, name1 ), true );
    return( new_ins );
}


instruction      *rOP2CL( BackRun& back, instruction *ins )
/******************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocRegName( back, HW_CL );
    new_ins = MakeConvert( back, ins->operands[1], name1, U1, ins->operands[1]->n.type_class );
    ins->operands[1] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rOP2CX( BackRun& back, instruction *ins )
/******************************************/
{
    instruction         *new_ins;
    name                *name1;

    name1 = AllocRegName( back, HW_CX );
    new_ins = MakeConvert( back, ins->operands[1], name1, U2, ins->operands[1]->n.type_class );
    ins->operands[1] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rMULREGISTER( BackRun& back, instruction *ins )
/************************************************/
{
    instruction         *new_ins;
    instruction         *ins2;
    name                *name1;
    name                *name2;

    name1 = AllocRegName( back, Op1Reg( back, ins ) );
    new_ins = MakeMove( back, ins->operands[0], name1, ins->type_class );
    ins->operands[0] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    name2 = AllocRegName( back, ResultReg( back, ins ) );
    ins2 = MakeUnary( back, OP_MOV, name1, ins->result, ins->type_class );
    ins2->base_type_class = DoubleClass[ins->type_class];
    ins->result = name2;
    ins->zap = &AllocRegName( back, ZapReg( back, ins ) )->r;
    MoveSegRes( back, ins, ins2 );
    SuffixIns( back, ins, ins2 );
    return( new_ins );
}


instruction      *rDIVREGISTER( BackRun& back, instruction *ins )
/************************************************/
{
    instruction         *new_ins;
    instruction         *ins2;
    name                *name1;
    name                *name2;

    name1 = AllocRegName( back, Op1Reg( back, ins ) );
    new_ins = MakeConvert( back, ins->operands[0], name1, DoubleClass[ins->type_class], ins->type_class );
    ins->operands[0] = name1;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    name2 = AllocRegName( back, ResultReg( back, ins ) );
    ins2 = MakeMove( back, name2, ins->result, ins->type_class );
    ins->result = name2;
    ins->zap = &AllocRegName( back, ZapReg( back, ins ) )->r;
    MoveSegRes( back, ins, ins2 );
    SuffixIns( back, ins, ins2 );
    return( new_ins );
}

name    *IntEquivalent( BackRun& back, name *name )
/**********************************/
{
    constant_defn       *floatval;

    floatval = GetFloat( back, name, FS );
    return( AllocConst( back, CFCnvU32F( back, &back.cfloat.cgh, ( U64LowLE( floatval->buffer.u64 ) ) ) ) );
}

name    *Int64Equivalent( BackRun& back, name *name )
/************************************/
{
    constant_defn       *floatval;

    floatval = GetFloat( back, name, FD );
    return( AllocU64Const( back, U64LowLE( floatval->buffer.u64 ), U64HighLE( floatval->buffer.u64 ) ) );
}

instruction      *rFSCONSCMP( BackRun& back, instruction *ins )
/**********************************************/
{
    name                *name1;

    name1 = ins->operands[1];
    if( CFTest( name1->c.u.cfval ) > 0 ) {
        ChangeType( ins, I4 );
    } else {
        ChangeType( ins, U4 );
        RevCond( ins );
    }
    ins->operands[1] = IntEquivalent( back, name1 );
    ins->table = NULL;
    return( ins );
}


instruction      *rHIGHLOWMOVE( BackRun& back, instruction *ins )
/************************************************/
{
    instruction         *new_ins;

    /*
     * for moving constants such as 0xabcdabcd
     */
    new_ins = MakeMove( back, LowPart( back, ins->result, U2 ), HighPart( back, ins->result, U2 ), U2 );
    ins->result = LowPart( back, ins->result, U2 );
    ChangeType( ins, U2 );
    DupSegRes( back, ins, new_ins );
    SuffixIns( back, ins, new_ins );
    return( ins );
}

#if _TARGET & _TARG_8086
    #define     LP              PT
    #define     MOV_SIZE        3
#else
    #define     LP              CP
    #define     MOV_SIZE        5
#endif

bool UseRepForm( BackRun& back, unsigned size )
/*************************************
 * Do we want to use "rep movs{w|d}", or string out multiple "movs{w|d}"
 */
{
    unsigned    count;
    unsigned    extra = 0;
    unsigned    rep_startup;
    unsigned    rep_iter;
    unsigned    movs_cost;

    count = size / WORD_SIZE;
    /*
     * if move than 10 movs, then always use rep form
     */
    if( count > 10 )
        return( true );
    if( back.data.OptForSize > 50 ) {
        switch( size % WORD_SIZE ) {
        case 0: extra = 0;      break;
        case 1: extra = 1;      break;
#if WORD_SIZE > 2
        case 2: extra = 1;      break;
        case 3: extra = 2;      break;
#endif
        }
        return( count + extra > MOV_SIZE + 2 );
    }
    if( CPULevel( back, CPU_586 ) ) {
        rep_startup = 11;
        rep_iter    = 1;
        movs_cost   = 4;
    } else if( CPULevel( back, CPU_486 ) ) {
        rep_startup = 12;
        rep_iter    = 3;
        movs_cost   = 7;
    } else if( CPULevel( back, CPU_386 ) ) {
        rep_startup = 8;
        rep_iter    = 4;
        movs_cost   = 8;
    } else if( CPULevel( back, CPU_286 ) ) {
        rep_startup = 5;
        rep_iter    = 4;
        movs_cost   = 5;
    } else {
        rep_startup = 9;
        rep_iter    = 17;
        movs_cost   = 18;
    }
    /*
     * The "+ 4" is to account for the "MOV [E]CX, const" clocks and some
     * slop for larger instructions (cache effects)
     */
    return( rep_startup + (count * rep_iter) + 4 < movs_cost * count );
}

static  bool    CanLoadStringOps( BackRun& back, instruction *ins )
/**************************************************/
{
    hw_reg_set  needs;

    if( UseRepForm( back, ins->operands[0]->n.size ) ) {
        needs = HW_xCX;
    } else {
        HW_CAsgn( needs, HW_EMPTY );
    }
    HW_CAsgn( needs, HW_xSI );
    HW_CTurnOn( needs, HW_xDI );
    if( IsTargetModel( back, CGSW_X86_FLOATING_DS ) ) {
        HW_CTurnOn( needs, HW_DS );
    }
    if( IsTargetModel( back, CGSW_X86_FLOATING_ES ) ) {
       HW_CTurnOn( needs, HW_ES );
    }
    return( !HwOvlap( needs, ins->head.live.regs ) );
}

static name *FakeIndex( BackRun& back, name *op, hw_reg_set index )
/**************************************************/
{
    name                *base;
    i_flags             flags;

    if( op->n.class_ == N_TEMP
      || op->n.class_ == N_MEMORY ) {
        base = op;
        flags = X_FAKE_BASE;
    } else {
        base = NULL;
        flags = static_cast<i_flags>(EMPTY);
    }
    return( ScaleIndex( back, AllocRegName( back, index ), base, 0, op->n.type_class, op->n.size, SCALE_NONE, flags ) );
}

static  bool    SegmentFloats( BackRun& back, name *op )
/***************************************/
{
    name        *segname;

    segname = SegName( back, op );
    if( segname->n.class_ != N_REGISTER )
        return( true );
    if( HW_COvlap( segname->r.reg, HW_DS ) )
        return( IsTargetModel( back, CGSW_X86_FLOATING_DS ) );
    if( HW_COvlap( segname->r.reg, HW_SS ) )
        return( IsTargetModel( back, CGSW_X86_FLOATING_SS ) );
    return( true );
}

static  instruction     *LoadStringOps( BackRun& back, instruction *ins,
                                        name **op1, name **op2 )
/**************************************************************/
{
    instruction         *load_op1;
    instruction         *load_op2;
    instruction         *load_len;
    instruction         *first_ins;
    instruction         *pop;
    name                *ds_reg;
    name                *es_reg;
    hw_reg_set          new_op1;
    bool                ds_needs_save;
    bool                es_needs_save;

    if( ins->head.opcode == OP_MOV
      && !UseRepForm( back, (*op1)->n.size ) ) {
        load_len = NULL;
        HW_CAsgn( new_op1, HW_EMPTY );
    } else {
        if( ( (*op1)->n.size & (WORD_SIZE - 1) ) == 0
          || ( back.data.OptForSize <= 50
          && ins->head.opcode == OP_MOV ) ) {
            load_len = MoveConst( back, (*op1)->n.size / WORD_SIZE, AllocRegName( back, HW_xCX ), WD );
            PrefixIns( back, ins, load_len );
        } else {
            load_len = MoveConst( back, (*op1)->n.size, AllocRegName( back, HW_xCX ), WD );
            PrefixIns( back, ins, load_len );
        }
        new_op1 = HW_xCX;
    }
    /*
     * careful here. Make sure we load DS last
     */
    if( ins->num_operands > OpcodeNumOperands( ins ) ) {
        if( (*op1)->n.class_ == N_INDEXED
          || (*op1)->n.class_ == N_MEMORY ) {
            load_op1 = MakeUnary( back, OP_LA, *op2, AllocRegName( back, HW_ES_xDI ), LP );
            PrefixIns( back, ins, load_op1 );
            load_op2 = MakeUnary( back, OP_LA, *op1, AllocRegName( back, HW_xSI ), WD );
            PrefixIns( back, ins, load_op2 );
            load_op2 = MakeMove( back, ins->operands[ins->num_operands - 1], AllocRegName( back, HW_DS ), U2 );
            PrefixIns( back, ins, load_op2 );
            es_needs_save = SegmentFloats( back, *op2 );
            ds_needs_save = true;
        } else {
            load_op1 = MakeMove( back, ins->operands[ins->num_operands - 1], AllocRegName( back, HW_ES ), U2 );
            PrefixIns( back, ins, load_op1 );
            load_op2 = MakeUnary( back, OP_LA, *op2, AllocRegName( back, HW_xDI ), WD );
            PrefixIns( back, ins, load_op2 );
            load_op2 = MakeUnary( back, OP_LA, *op1, AllocRegName( back, HW_DS_xSI ), LP );
            PrefixIns( back, ins, load_op2 );
            ds_needs_save = SegmentFloats( back, *op1 );
            es_needs_save = true;
        }
        DelSeg( ins );
    } else if( IsTargetModel( back, CGSW_X86_FLAT_MODEL )
      && !SegmentFloats( back, *op1 )
      && !SegmentFloats( back, *op2 ) ) {
        load_op1 = MakeUnary( back, OP_LA, *op2, AllocRegName( back, HW_xDI ), WD );
        PrefixIns( back, ins, load_op1 );
        load_op2 = MakeUnary( back, OP_LA, *op1, AllocRegName( back, HW_xSI ), WD );
        PrefixIns( back, ins, load_op2 );
        ds_needs_save = es_needs_save = false;
    } else {
        load_op1 = MakeUnary( back, OP_LA, *op2, AllocRegName( back, HW_ES_xDI ), LP );
        PrefixIns( back, ins, load_op1 );
        load_op2 = MakeUnary( back, OP_LA, *op1, AllocRegName( back, HW_DS_xSI ), LP );
        PrefixIns( back, ins, load_op2 );
        ds_needs_save = SegmentFloats( back, *op1 );
        es_needs_save = SegmentFloats( back, *op2 );
    }
    if( IsntTargetModel( back, CGSW_X86_FLOATING_DS )
      && ds_needs_save ) {
        /*
         * restore DS
         */
        ds_reg = AllocRegName( back, HW_DS );
        pop = MakeUnary( back, OP_POP, NULL, ds_reg, U2 );
        pop->num_operands = 0;
        SuffixIns( back, ins, pop );
        PrefixIns( back, load_op2, MakeUnary( back, OP_PUSH, ds_reg, NULL, U2 ) );
    }
    if( IsntTargetModel( back, CGSW_X86_FLOATING_ES )
      && es_needs_save ) {
        /*
         * restore ES
         */
        es_reg = AllocRegName( back, HW_ES );
        pop = MakeUnary( back, OP_POP, NULL, es_reg, U2 );
        pop->num_operands = 0;
        SuffixIns( back, ins, pop );
        PrefixIns( back, load_op1, MakeUnary( back, OP_PUSH, es_reg, NULL, U2 ) );
        load_op1 = load_op1->head.prev;
    }
    HW_CTurnOn( new_op1, HW_DS_xSI );
    HW_CTurnOn( new_op1, HW_ES_xDI );
    ins->table = GetString();
    ins->head.state = INS_NEEDS_WORK;
    *op1 = FakeIndex( back, ins->operands[0], new_op1 );
    ins->operands[1] = ins->operands[0];
    HW_CTurnOff( new_op1, HW_SS );
    HW_CTurnOff( new_op1, HW_xSP );
    HW_CTurnOff( new_op1, HW_ES );
    HW_CTurnOff( new_op1, HW_DS );
    ins->zap = &AllocRegName( back, new_op1 )->r;
    if( ins->head.opcode == OP_MOV ) {
        ins->result = FakeIndex( back, ins->result, HW_ES_xDI );
    } else {
        ins->result = NULL;
    }
    if( load_len != NULL ) {
        first_ins = load_len;
    } else {
        first_ins = load_op1;
    }
    UpdateLive( back, first_ins, ins );
    return( first_ins );
}


instruction      *rMAKESTRCMP( BackRun& back, instruction *ins )
/***********************************************/
{
    return( LoadStringOps( back, ins, &ins->operands[0], &ins->operands[1] ) );
}


instruction      *rMAKESTRMOVE( BackRun& back, instruction *ins )
/************************************************/
{
    return( LoadStringOps( back, ins, &ins->operands[0], &ins->result ) );
}


instruction      *rMAYBSTRMOVE( BackRun& back, instruction *ins )
/************************************************/
{
    if( CanLoadStringOps( back, ins ) ) {
        return( LoadStringOps( back, ins, &ins->operands[0], &ins->result ) );
    } else {
        return( rSPLIT8( back, ins ) );
    }
}


instruction      *rEXT_PUSHC( BackRun& back, instruction *ins )
/**********************************************/
{
    CnvOpToInt( back, ins, 0 );
    ChangeType( ins, WD );
    return( ins );
}


instruction      *rMOVELOW( BackRun& back, instruction *ins )
/*********************************************
 * e.g. convert U2==>U1
 */
{
    ins->head.opcode = OP_MOV;
    ins->operands[0] = LowPart( back, ins->operands[0], ins->type_class );
    ins->table = NULL;
    return( ins );
}


instruction      *rSPLITUNARY( BackRun& back, instruction *ins )
/***********************************************/
{
    instruction         *new_ins;

    CnvOpToInt( back, ins, 0 );
    new_ins = SplitUnary( back, ins );
    return( new_ins );
}


instruction      *rMOVRESMEM( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;
    name                *name_flt;
    name                *name_int;
    type_class_def      type_class;

    type_class = ins->type_class;
    name_flt = AllocTemp( back, type_class );
    if( type_class == FD
      || type_class == FL ) {
        name_int = name_flt;
    } else {
        name_int = TempOffset( back, name_flt, 0, U4 );
    }
    name_flt->v.usage |= USE_MEMORY | NEEDS_MEMORY;
    new_ins = MakeMove( back, name_int, ins->result, name_int->n.type_class);
    ins->result = name_flt;
    MoveSegRes( back, ins, new_ins );
    SuffixIns( back, ins, new_ins );
    return( ins );
}


instruction      *rMOVOP1MEM( BackRun& back, instruction *ins )
/**********************************************/
{
    instruction         *new_ins;
    name                *name_flt;
    name                *name_int;
    type_class_def      type_class;

    type_class = ins->type_class;
    name_flt = AllocTemp( back, type_class );
    if( type_class == FD
      || type_class == FL ) {
        name_int = name_flt;
    } else {
        name_int = TempOffset( back, name_flt, 0, U4 );
    }
    name_flt->v.usage |= USE_MEMORY | NEEDS_MEMORY;
    new_ins = MakeMove( back, ins->operands[0], name_int, name_int->n.type_class);
    ins->operands[0] = name_flt;
    MoveSegOp( back, ins, new_ins, 0 );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rOP1CMEM( BackRun& back, instruction *ins )
/********************************************/
{
    ins->operands[0] = Addressable( back, ins->operands[0], ins->type_class );
    return( ins );
}


instruction      *rOP2CMEM( BackRun& back, instruction *ins )
/********************************************/
{
    ins->operands[1] = Addressable( back, ins->operands[1], ins->type_class );
    return( ins );
}


instruction      *rU_TEST( BackRun& back, instruction *ins )
/*******************************************/
{
    instruction         *new_ins;
    instruction         *ins2;
    name                *name1;
    name                *name2;

    ChangeType( ins, U2 );
    name1 = ins->operands[0];
    name2 = AllocTemp( back, U2 );
    new_ins = MakeMove( back, HighPart( back, name1, U2 ), name2, U2 );
    ins2 = MakeBinary( back, OP_OR, name2, LowPart( back, name1, U2 ), name2, U2 );
    ins->operands[0] = name2;
    DupSeg( ins, new_ins );
    DupSeg( ins, ins2 );
    DelSeg( ins );
    PrefixIns( back, ins, new_ins );
    PrefixIns( back, ins, ins2 );
    return( new_ins );
}


instruction      *rCPSUB( BackRun& back, instruction *ins )
/******************************************/
{
    instruction         *new_ins;

    new_ins = MakeBinary( back, OP_SUB, OffsetPart( back, ins->operands[0] ),
                           OffsetPart( back, ins->operands[1] ),
                           ins->result, SW );
    DupSeg( ins, new_ins );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}


instruction      *rPTSUB( BackRun& back, instruction *ins )
/******************************************/
{
    instruction         *new_ins;

    ChangeType( ins, PT );
    new_ins = rMAKECALL( back, ins );
    return( new_ins );
}


instruction      *rEXTPT( BackRun& back, instruction *ins )
/******************************************/
{
    instruction         *new_ins;
    name                *name1;

    if( ins->ins_flags & INS_CODE_POINTER ) {
        name1 = AllocRegName( back, HW_CS );
    } else {
        name1 = NearSegment(back);
    }
    new_ins = MakeMove( back, name1, SegmentPart( back, ins->result ), U2 );
    ins->head.opcode = OP_MOV;
    ins->type_class = WD;
    ins->result = OffsetPart( back, ins->result );
    ins->table = NULL;
    DupSegRes( back, ins, new_ins );
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


instruction     *rMAKEU4CONS( BackRun& back, instruction *ins )
/**********************************************/
{
    ChangeType( ins, U4 );
    ins->operands[0] = IntEquivalent( back, ins->operands[0] );
    return( ins );
}


instruction      *rCHPPT( BackRun& back, instruction *ins )
/******************************************/
{
    ins->head.opcode = OP_MOV;
    ChangeType( ins, WD );
    ins->operands[0] = OffsetPart( back, ins->operands[0] );
    return( ins );
}


/*
 * NB: The following two routines are intended for 386 only
 */

instruction      *rMOVPTI8( BackRun& back, instruction *ins )
/********************************************/
{
    instruction         *new_ins;
    instruction         *ins2;

    new_ins = MakeMove( back, OffsetPart( back, ins->operands[0] ), LowPart( back, ins->result, U4 ), U4 );
    ins2    = MakeConvert( back, SegmentPart( back, ins->operands[0] ), HighPart( back, ins->result, U4 ), U4, U2 );
    DupSegRes( back, ins, ins2 );
    SuffixIns( back, ins, ins2 );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}

instruction      *rMOVI8PT( BackRun& back, instruction *ins )
/********************************************/
{
    instruction         *new_ins;
    instruction         *ins2;

    new_ins = MakeMove( back, LowPart( back, ins->operands[0], U4 ), OffsetPart( back, ins->result ), U4 );
    ins2    = MakeMove( back, HighPart( back, ins->operands[0], U2 ), SegmentPart( back, ins->result ), U2 );
    DupSegRes( back, ins, ins2 );
    SuffixIns( back, ins, ins2 );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}


void    CheckCC( instruction *ins, instruction *new_ins )
/*******************************************************/
{
    if( ins->head.opcode == OP_EXT_ADD
      || ins->head.opcode == OP_EXT_SUB ) {
        /*
         * ensure it doesn't set the condition codes
         */
        new_ins->table = GetMoveNoCCEntry();
        new_ins->ins_flags |= INS_CC_USED;
    }
}


static  instruction     *SplitPush( BackRun& back, instruction *ins, type_length size )
/**********************************************************************/
{
    instruction         *new_ins;
    instruction         *first_ins;
    name                *op;
    name                *new_op = NULL;

    size = RoundUp( size, WORD_SIZE );
    op = ins->operands[0];
    first_ins = NULL;
    for( ;; ) {
        size -= WORD_SIZE;
        switch( op->n.class_ ) {
        case N_MEMORY:
            new_op = AllocMemory( back, op->v.symbol, op->v.offset + size,
                                  op->m.memory_type, WD );
            break;
        case N_TEMP:
            new_op = TempOffset( back, op, size, WD );
            new_op->t.temp_flags |= CG_INTRODUCED;
            break;
        case N_INDEXED:
            new_op = ScaleIndex( back, op->i.index, op->i.base,
                              op->i.constant + size, WD, 0,
                              op->i.scale, op->i.index_flags );
            break;
        default:
            Zoiks( back, ZOIKS_051 );
            break;
        }
        new_ins = MakeUnary( back, OP_PUSH, new_op, NULL, WD );
        if( size == 0 )
            break;
        if( first_ins == NULL ) {
            first_ins = new_ins;
        }
        DupSeg( ins, new_ins );
        PrefixIns( back, ins, new_ins );
    }
    DupSeg( ins, new_ins );
    ReplIns( back, ins, new_ins );
    if( first_ins == NULL )
        return( new_ins );
    return( first_ins );
}


instruction     *rDOLONGPUSH( BackRun& back, instruction *ins )
/**********************************************/
{
    name        *sp;
    name        *at_sp;
    name        *temp;
    type_length size;
    instruction *move;
    instruction *sub_sp;
    instruction *temp_ins;
    hw_reg_set  hw_ss_sp;

    size = ins->operands[0]->n.size;
    if( size <= 4 * WORD_SIZE ) {
        return( SplitPush( back, ins, size ) );
    } else {
        HW_CAsgn( hw_ss_sp, HW_SS );
        HW_CTurnOn( hw_ss_sp, HW_xSP );
        sp = AllocRegName( back, HW_xSP );
        if( IsTargetModel( back, CGSW_X86_FLOATING_SS ) ) {
            temp = AllocTemp( back, CP );
            temp_ins = MakeMove( back, AllocRegName( back, hw_ss_sp ), temp, CP );
        } else {
            temp = AllocTemp( back, WD );
            temp_ins = MakeMove( back, sp, temp, WD );
        }
        at_sp = AllocIndex( back, temp, NULL, 0, XX );
        at_sp->n.size = size;
        move = MakeMove( back, ins->operands[0], at_sp, XX );
        ReplIns( back, ins, move );
        ins = LoadStringOps( back, move, &move->operands[0], &move->result );
        /*
         * % mov CX,const will be the first if it's there so try for SUB SP,CX
         */
        sub_sp = MakeBinary( back, OP_SUB, sp, AllocIntConst( back, RoundUp( size, WORD_SIZE ) ), sp, WD );
        if( ins->result != NULL
          && HW_CEqual( ins->result->r.reg, HW_xCX ) ) {
            SuffixIns( back, ins, sub_sp );
        } else {
            PrefixIns( back, ins, sub_sp );
            ins = sub_sp;
        }
        SuffixIns( back, sub_sp, temp_ins );
    }
    return( ins );
}


name    *OpAdjusted( BackRun& back, name *op, int bias, type_class_def type_class )
/*******************************************************************
 * Return a new op of type 'type' which is offset from the old op by the
 * amount specified by 'bias'.
 */
{
    name        *new_op = NULL;

    switch( op->n.class_ ) {
    case N_MEMORY:
        new_op = AllocMemory( back, op->v.symbol, op->v.offset + bias, op->m.memory_type, type_class );
        break;
    case N_TEMP:
        new_op = TempOffset( back, op, bias, type_class );
        new_op->t.temp_flags |= CG_INTRODUCED;
        break;
    case N_INDEXED:
        new_op = ScaleIndex( back, op->i.index, op->i.base,
                          op->i.constant + bias, SW, 0,
                          op->i.scale, op->i.index_flags );
        break;
    default:
        Zoiks( back, ZOIKS_051 );
        break;
    }
    return( new_op );
}


instruction     *rFLIPSIGN( BackRun& back, instruction *ins )
/********************************************/
{
    instruction         *new_ins;
    name                *new_op;

    new_op = OpAdjusted( back, ins->operands[0], ins->operands[0]->n.size - 1, U1 );
    new_ins = MakeBinary( back, OP_XOR, new_op, AllocIntConst( back, 0x80 ), new_op, U1 );
    DupSegRes( back, ins, new_ins );
    ReplIns( back, ins, new_ins );
    return( new_ins );
}


instruction     *rTEMP2CONST( BackRun& back, instruction *ins )
/**********************************************/
{
    opcnt       i;
    name        *op;
    instruction *new_ins;

    /*
     * Never modify const temps operands "in place" - ReplIns() will be
     * unable to move conflict edges and they will point to nowhere.
     * Instead, new_ins instruction must be created and ReplIns()'ed.
     */
    new_ins = NewIns( back, ins->num_operands );
    /*
     * copy instruction without operands
     */
    memcpy( new_ins, ins, sizeof( instruction ) );
    for( i = ins->num_operands; i-- > 0; ) {
        op = ins->operands[i];
        if( ConstTemp( op ) ) {
            new_ins->operands[i] = static_cast<name *>(op->v.symbol);
        } else {
            new_ins->operands[i] = op;
        }
    }
    ReplIns( back, ins, new_ins );
    return ( new_ins );
}


void    CnvOpToInt( BackRun& back, instruction * ins, opcnt op )
/***********************************************/
{
    name                *name1;

    switch( ins->type_class ) {
    case FS:
        name1 = ins->operands[op];
        if( name1->n.class_ == N_CONSTANT ) {
            ins->operands[op] = IntEquivalent( back, name1 );
        }
        break;
#if _TARGET & _TARG_80386
    /*
     * this is for the I8 stuff - can't tell what to do in
     * HighPart and LowPart if we don't get rid on constant
     * here
     */
    case FD:
        name1 = ins->operands[op];
        if( name1->n.class_ == N_CONSTANT ) {
            ins->operands[op] = Int64Equivalent( back, name1 );
        }
        break;
#endif
    default:
        break;
    }
}


instruction     *rCMPCP( BackRun& back, instruction *ins )
/*****************************************/
{
    assert( ins->type_class == CP );
    assert( ins->operands[1]->n.class_ == N_CONSTANT );
    assert( ins->operands[1]->c.lo.u.int_value == 0 );
    if( ins->head.opcode == OP_CMP_EQUAL
      || ins->head.opcode == OP_CMP_NOT_EQUAL ) {
        if( IsTargetModel( back, CGSW_X86_NULL_SELECTOR_BAD ) ) {
            ins->operands[0] = HighPart( back, ins->operands[0], U2 );
            ChangeType( ins, U2 );
            return( ins );
        }
    }
    return( rMAKEU4( back, ins ) );
}
