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
* Description:  Intel 80x87 floating point instruction expansion.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "cgaux.h"
#include "data.h"
#include "makeins.h"
#include "namelist.h"
#include "nullprop.h"
#include "fpu.h"
#include "x87.h"
#include "rgtbl.h"
#include "insutil.h"
#include "optab.h"
#include "inssegs.h"
#include "fixindex.h"
#include "revcond.h"
#include "temps.h"
#include "opctable.h"
#include "x86regn.h"
#include "feprotos.h"
#include "backrun.hpp"


//NYI: probably need more opcode entries for more resolution with func. units
static const opcode_entry    FNOP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                          SETS_CC, V_NO,           RG_,          G_NO,           FU_FOP )
};


static const opcode_entry    RRFBIN[]  = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBIN,       FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBIN,       FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBIN,       FU_FDIV ),
};
static const opcode_entry    RNFBIN[]  = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBIN,       FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBIN,       FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBIN,       FU_FDIV ),
};
static const opcode_entry    RRFBINP[] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBINP,      FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBINP,      FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBINP,      FU_FDIV ),
};
static const opcode_entry    RNFBINP[] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBINP,      FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBINP,      FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBINP,      FU_FDIV ),
};
static const opcode_entry    RRFBIND[] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBIND,      FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBIND,      FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBIND,      FU_FDIV ),
};
static const opcode_entry    RNFBIND[] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBIND,      FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBIND,      FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBIND,      FU_FDIV ),
};
static const opcode_entry    MRFBIN[]  = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_MRFBIN,       FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_MRFBIN,       FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_MRFBIN,       FU_FDIV ),
};
static const opcode_entry    MNFBIN[]  = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_MNFBIN,       FU_FADD ),
_OE(                         PRESERVE, V_NO,           RG_,          G_MNFBIN,       FU_FMUL ),
_OE(                         PRESERVE, V_NO,           RG_,          G_MNFBIN,       FU_FDIV ),
};

static const opcode_entry    MFLD[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_MFLD,         FU_FOP )
};
static const opcode_entry    RFLD[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RFLD,         FU_FOP )
};
static const opcode_entry    MFST[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_MFST,         FU_FOP )
};
static const opcode_entry    MFSTRND[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_MFSTRND,      FU_FOP )
};
static const opcode_entry    MFST2[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_MFST,         FU_FOP )
};
static const opcode_entry    RFST[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RFST,         FU_FOP )
};
static const opcode_entry    FCHS[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FCHS,         FU_FOP )
};
static const opcode_entry    FMATH[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FMATH,        FU_TRIG )
};
static const opcode_entry    IFUNC[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                            NO_CC, V_NO,           RG_,          G_IFUNC,        FU_TRIG )
};
static const opcode_entry    FCHOP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FCHOP,        FU_FOP )
};
static const opcode_entry    FLDZ[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FLDZ,         FU_FOP )
};
static const opcode_entry    FLD1[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FLD1,         FU_FOP )
};
static const opcode_entry    FCOMPP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                          SETS_CC, V_NO,           RG_,          G_FCOMPP,       FU_FOP )
};
static const opcode_entry    MCOMP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                          SETS_CC, V_NO,           RG_,          G_MCOMP,        FU_FOP )
};
static const opcode_entry    RCOMP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                          SETS_CC, V_NO,           RG_,          G_RCOMP,        FU_FOP )
};
static const opcode_entry    MFSTNP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_MFSTNP,       FU_FOP )
};
static const opcode_entry    RFSTNP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RFSTNP,       FU_FOP )
};
static const opcode_entry    FWAIT[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FWAIT,        FU_FOP )
};//NYI:??
static const opcode_entry    FXCH[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FXCH,         FU_FOP )
};
static const opcode_entry    RC[1]   = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RC,           FU_FOP )
};
#if _TARGET & _TARG_8086
static const opcode_entry    RR1[1]  = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RR1,          FU_FOP )
};
static const opcode_entry    WORDR1[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_WORDR1,       FU_FOP )
};
#endif

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct I87expState {
    name *ST0;
    name *ST1;
};
namespace {

}  // namespace



static const opcode_entry *GenTab( instruction *ins, const opcode_entry *array )
/******************************************************************************/
{
    switch( ins->head.opcode ) {
    case OP_DIV:
        return( array + 2 );
    case OP_MUL:
        return( array + 1 );
    default:
        return( array );
    }
}


bool    FPStackReg( BackRun& back, name *reg_name )
/**********************************/
{
    int         reg_num;

    reg_num = FPRegNum( reg_name );
    if( reg_num == -1 )
        return( false );
    if( reg_num < back.i87data.Max87Stk )
        return( true );
    return( false );
}


instruction *PrefFLDOp( BackRun& back, instruction *ins, operand_type op, name *opnd )
/*********************************************************************/
{
    instruction *new_ins = NULL;

    switch( op ) {
    case OP_STK0:
    case OP_STKI:
        new_ins = MakeUnary( back, OP_MOV, opnd, back.i87exp->ST0, FD );
        new_ins->u.gen_table = RFLD;
        break;
    case OP_MEM:
        new_ins = MakeUnary( back, OP_MOV, opnd, back.i87exp->ST0, FD );
        new_ins->u.gen_table = MFLD;
        MoveSegOp( back, ins, new_ins, 0 );
        break;
    case OP_CONS:
        new_ins = MakeUnary( back, OP_MOV, opnd, back.i87exp->ST0, FD );
        if( CFTest( opnd->c.u.cfval ) ) {
            new_ins->u.gen_table = FLD1;
        } else {
            new_ins->u.gen_table = FLDZ;
        }
        break;
    }
    PrefixIns( back, ins, new_ins );
    return( new_ins );
}


static  void    PrefixFLDOp( BackRun& back, instruction *ins, operand_type op, opcnt i )
/************************************************************************
    Prefix the floating point instruction "ins" with an FLD instruction
    for one of its operands (ins->operands[i]).  That operand has
    classification "op".
*/
{
    name        *opnd;
    instruction *new_ins;

    opnd = ins->operands[i];
    ins->operands[i] = back.i87exp->ST0;
    new_ins = PrefFLDOp( back, ins, op, opnd );
    new_ins->stk_exit = new_ins->stk_entry + 1;
    ins->stk_entry++;
    ins->operands[i] = back.i87exp->ST0;
}


bool            FPResultNotNeeded( instruction *ins )
/***************************************************/
{
    return( ins->u.gen_table == MFST2 );
}


instruction *SuffFSTPRes( BackRun& back, instruction *ins, name *opnd, result_type res )
/***********************************************************************/
{
    instruction *new_ins;

    new_ins = MakeUnary( back, OP_MOV, back.i87exp->ST0, opnd, FD );
    if( res == RES_STKI ) {
        new_ins->u.gen_table = RFST;
    } else if( res == RES_MEM_THROWAWAY ) {
        new_ins->u.gen_table = MFST2; // so we can turn it into fstp st(0) later
    } else {
        new_ins->u.gen_table = MFST;
    }
    if( ins->head.opcode != OP_BLOCK ) {
        MoveSegRes( back, ins, new_ins );
    }
    SuffixIns( back, ins, new_ins );
    return( new_ins );
}


static  instruction     *SuffixFSTPRes( BackRun& back, instruction *ins )
/*********************************************************
    Suffix the floating point instruction "ins" with an FSTP instruction
    for its result (ins->result).
*/
{
    instruction *new_ins;
    name        *opnd;

    opnd = ins->result;
    ins->result = back.i87exp->ST0;
    new_ins = SuffFSTPRes( back, ins, opnd, RES_MEM );
    new_ins->stk_entry = ins->stk_exit + 1;
    ins->stk_exit++;
    return( new_ins );
}

instruction     *SuffFXCH( BackRun& back, instruction *ins, int i )
/**************************************************/
{
    instruction *new_ins;

    new_ins = MakeUnary( back, OP_MOV, back.i87exp->ST0, ST(back, i), FD );
    new_ins->u.gen_table = FXCH;
    SuffixIns( back, ins, new_ins );
    new_ins->stk_exit = new_ins->stk_entry;
    return( new_ins );
}

instruction     *PrefFXCH( BackRun& back, instruction *ins, int i )
/***************************************************
    Prefix an instruction "ins" with an FXCH ST(i)
*/
{
    instruction *new_ins;

    new_ins = MakeUnary( back, OP_MOV, back.i87exp->ST0, ST(back, i), FD );
    new_ins->u.gen_table = FXCH;
    PrefixIns( back, ins, new_ins );
    new_ins->stk_exit = new_ins->stk_entry;
    return( new_ins );
}


static  void    PrefixChop( BackRun& back, instruction *ins )
/*********************************************
    Prefix an instruction "ins" with an instruction that will truncate
    ST(0) to the nearest integer.
*/
{
    instruction *new_ins;

    if( ins->head.opcode == OP_ROUND )
        return;
    if( IsFloating( ins->result->n.type_class ) )
        return;
    new_ins = MakeUnary( back, OP_MOV, back.i87exp->ST0, back.i87exp->ST0, FD );
    new_ins->u.gen_table = FCHOP;
    PrefixIns( back, ins, new_ins );
    new_ins->stk_exit = ins->stk_entry;
}

static    int     WantsChop( instruction *ins )
/**********************************************
    Check whether instruction "ins" needs an instruction that will truncate
    ST(0) to the nearest integer.
*/
{
    if( ins->head.opcode == OP_ROUND )
        return( false );
    if( IsFloating( ins->result->n.type_class ) )
        return( false );
    return( true );
}

static instruction  *ExpUnary( BackRun& back, instruction *ins, operand_type src,
                            result_type res, const opcode_entry *table )
/***********************************************************************
    Expand a unary instruction "ins" using classifications "src" and "res"
*/
{
    instruction *unary;

    ins->u.gen_table = table;
    unary = ins;
    if( src != OP_STK0 ) {
        PrefixFLDOp( back, ins, src, 0 );
    }
    if( res != RES_STK0 ) {
        ins = SuffixFSTPRes( back, ins );
    }
    unary->operands[0] = back.i87exp->ST0;
    unary->result = back.i87exp->ST0;
    return( ins );
}


static  instruction     *ExpCall( BackRun& back, instruction *ins )
/***************************************************
    Expand a call instruction.  Its not really an 8087 instruction but
    we must take into account how many parameters it pops off the 8087
    stack and whether it returns a value on the 8087.  If that return
    value is ignored, we have to FSTP ST(0) to get rid of it.
*/
{
    instruction *new_ins;

    new_ins = ins;
    if( ins->result != NULL
     && ins->result->n.class_ == N_REGISTER
     && HW_CEqual( ins->result->r.reg, HW_ST0 ) ) {
        if( ins->flags.u.call_flags & CALL_IGNORES_RETURN ) {
            new_ins = SuffixFSTPRes( back, ins );
            new_ins->u.gen_table = RFST;
        }
    }
    return( new_ins );
}

inline constexpr int MoveClass( operand_type src, result_type dst ) { return ( dst + src*RES_NONE ); }

static instruction  *ExpMove( BackRun& back, instruction *ins, operand_type src,
                                result_type dst )
/**********************************************************************
    Expand a floating point move "ins" using classifications "src" and "dst".
*/
{
    switch( MoveClass( src, dst ) ) {
    case MoveClass( OP_STK1, RES_STK0 ):
    case MoveClass( OP_STK1, RES_MEM ):
        Zoiks( back, ZOIKS_008 );
        break;
    case MoveClass( OP_MEM , RES_MEM ):
        PrefixFLDOp( back, ins, src, 0 );
        /*
        PrefixChop( ins );
        DoNothing( ins );
        ins = SuffixFSTPRes( ins );
        */
        if( IsModel( back, CGSW_GEN_FPU_ROUNDING_INLINE ) ) {
            DoNothing( back, ins );
            ins = SuffixFSTPRes( back, ins );
            if( WantsChop( ins ) ) {
                ins->u.gen_table = MFSTRND;
            }
        } else if( IsModel( back, CGSW_GEN_FPU_ROUNDING_OMIT ) ) {
            DoNothing( back, ins );
            ins = SuffixFSTPRes( back, ins );
        } else {
            PrefixChop( back, ins );
            DoNothing( back, ins );
            ins = SuffixFSTPRes( back, ins );
        }
        break;
    case MoveClass( OP_STK0, RES_STK0 ):
        DoNothing( back, ins );
        break;
    case MoveClass( OP_STK0, RES_MEM  ):
        if( IsModel( back, CGSW_GEN_FPU_ROUNDING_INLINE ) ) {
            if( WantsChop( ins ) ) {
                ins->u.gen_table = MFSTRND;
            } else {
                ins->u.gen_table = MFST;
            }
        } else if( IsModel( back, CGSW_GEN_FPU_ROUNDING_OMIT ) ) {
            ins->u.gen_table = MFST;
        } else {
            PrefixChop( back, ins );
            ins->u.gen_table = MFST;
        }
        break;
    case MoveClass( OP_MEM , RES_STK0 ):
        ins->u.gen_table = MFLD;
        break;
    case MoveClass( OP_CONS, RES_STK0 ):
        if( CFTest( ins->operands[0]->c.u.cfval ) != 0 ) {
            ins->u.gen_table = FLD1;
        } else {
            ins->u.gen_table = FLDZ;
        }
        ins->result = back.i87exp->ST0;
        break;
    case MoveClass( OP_CONS, RES_MEM ):
        PrefixFLDOp( back, ins, src, 0 );
        DoNothing( back, ins );
        ins = SuffixFSTPRes( back, ins );
        break;
    default:
        Zoiks( back, ZOIKS_009 );
        break;
    }
    return( ins );
}


static  instruction     *ExpPush( BackRun& back, instruction *ins, operand_type op )
/********************************************************************
    expand a PUSH instruction.  On the 386 we generate FSTP 0[esp].  On
    the 8086..286 try MOV BP,SP  FSTP 0[bp].  If thats not possible,
    pick a register from SI,DI,BX and generate for PUSH reg, MOV reg,SP,
    FSTP ss:0[reg], POP reg.
*/
{
    instruction         *new_ins;
    name                *sp;
    int                 size;
    name                *index;

    sp = AllocRegName( back, HW_xSP );
    size = back.data.TypeClassSize[ins->type_class];
    new_ins = MakeBinary( back, OP_SUB, sp, AllocIntConst( back, size ), sp, WD );
    new_ins->u.gen_table = RC;
    PrefixIns( back, ins, new_ins );
#if _TARGET & _TARG_8086
    {
        instruction         *pop_ins;
        hw_reg_set          avail_index;
        const hw_reg_set    *idx;

        HW_CAsgn( avail_index, HW_IDX16 );
        HwTurnOff( avail_index, ins->head.live.regs );
        idx = IdxRegs();
        for( ;; ) {
            if( HW_CEqual( *idx, HW_EMPTY ) ) {
                HW_CAsgn( avail_index, HW_xBP );
                break;
            }
            if( HwSubset( avail_index, *idx ) ) {
                HW_CAsgn( avail_index, HW_SS );
                HwTurnOn( avail_index, *idx );
                break;
            }
            ++idx;
        }
        if( IsntTargetModel( back, CGSW_X86_FLOATING_DS ) && IsntTargetModel( back, CGSW_X86_FLOATING_SS ) ) {
            HW_CTurnOff( avail_index, HW_SS );
        }
        index = AllocRegName( back, avail_index );
        if( !HW_CEqual( avail_index, HW_xBP ) ) {
            new_ins = MakeMove( back, sp, index, WD );
            new_ins->u.gen_table = RR1;
            PrefixIns( back, ins, new_ins );
            index = AllocIndex( back, index, NULL, 0, ins->type_class );
            new_ins = MakeMove( back, ins->operands[0], index,ins->type_class );
            ReplIns( back, ins, new_ins );
            ins = ExpMove( back, new_ins, op, RES_MEM );

       } else {
            new_ins = MakeUnary( back, OP_PUSH, index, NULL, WD );
            new_ins->u.gen_table = WORDR1;
            PrefixIns( back, ins, new_ins );
            pop_ins = MakeUnary( back, OP_POP, NULL, index, WD );
            pop_ins->num_operands = 0;
            pop_ins->u.gen_table = WORDR1;
            SuffixIns( back, ins, pop_ins );
            new_ins = MakeMove( back, sp, index, WD );
            new_ins->u.gen_table = RR1;
            PrefixIns( back, ins, new_ins );
            index = AllocIndex( back, index, NULL, 2, ins->type_class );
            new_ins = MakeMove( back, ins->operands[0], index,ins->type_class );
            ReplIns( back, ins, new_ins );
            ins = ExpMove( back, new_ins, op, RES_MEM );
            ins = pop_ins;
        }
    }
#elif _TARGET & _TARG_80386
    {
        index = AllocIndex( back, sp, NULL, 0, ins->type_class );
        new_ins = MakeMove( back, ins->operands[0], index,ins->type_class );
        ReplIns( back, ins, new_ins );
        ins = ExpMove( back, new_ins, op, RES_MEM );
    }
#endif
    return( ins );
}


inline constexpr int OpsClass( operand_type op1, operand_type op2 ) { return ( op1 + op2*OP_NONE ); }

static void ExpBinFunc( BackRun& back, instruction *ins, operand_type op1, operand_type op2 )
/*****************************************************************************
    Expand a floating point binary math instructon "ins", like pow,
    atan2, etc using classifications "op1" and "op2".
*/
{
    switch( OpsClass( op1, op2 ) ) {
    case OpsClass( OP_STK1, OP_STK1 ):
    case OpsClass( OP_MEM , OP_STK1 ):
    case OpsClass( OP_CONS, OP_STK1 ):
    case OpsClass( OP_STK1, OP_MEM  ):
    case OpsClass( OP_STK1, OP_CONS ):
    case OpsClass( OP_CONS, OP_CONS ):
        Zoiks( back, ZOIKS_010 );
        break;
    case OpsClass( OP_STK0, OP_MEM  ):
    case OpsClass( OP_STK0, OP_CONS ):
        PrefixFLDOp( back, ins, op2, 1 );
        PrefFXCH( back, ins, 1 );
        break;
    case OpsClass( OP_STK1, OP_STK0 ):
        PrefFXCH( back, ins, 1 );
        break;
    case OpsClass( OP_STK0, OP_STK0 ):
    case OpsClass( OP_MEM , OP_STK0 ):
    case OpsClass( OP_CONS, OP_STK0 ):
        PrefixFLDOp( back, ins, op1, 0 );
        break;
    case OpsClass( OP_STK0, OP_STK1 ):
        break;
    case OpsClass( OP_MEM , OP_MEM  ):
    case OpsClass( OP_CONS, OP_MEM  ):
    case OpsClass( OP_MEM , OP_CONS ):
        PrefixFLDOp( back, ins, op2, 1 );
        PrefixFLDOp( back, ins, op1, 0 );
        break;
    default:
        Zoiks( back, ZOIKS_011 );
        break;
    }
    ins->u.gen_table = IFUNC;
}


static  void    ExchangeOps( instruction *ins )
/*********************************************/
{
    name        *op1;
    name        *op2;

    op1 = ins->operands[0];
    op2 = ins->operands[1];
    ins->operands[0] = op2;
    ins->operands[1] = op1;
}


static void ExpBinary( BackRun& back, instruction *ins, operand_type op1, operand_type op2 )
/****************************************************************************
    Expand a floating point binary instructon "ins" using
    classifications "op1" and "op2".
*/
{
    /* expand a binary floating point instruction. op1 is the implied locn.*/

    switch( OpsClass( op1, op2 ) ) {
    case OpsClass( OP_STK1, OP_STK1 ):
    case OpsClass( OP_MEM , OP_STK1 ):
    case OpsClass( OP_CONS, OP_STK1 ):
    case OpsClass( OP_STK1, OP_MEM  ):
    case OpsClass( OP_STK1, OP_CONS ):
    case OpsClass( OP_CONS, OP_CONS ):
/*   case 0,6,7,8,9,11,16,21,24:*/
        Zoiks( back, ZOIKS_010 );
        break;
    case OpsClass( OP_STK0, OP_STK0 ):
        ins->u.gen_table = GenTab( ins, RNFBIN );
        break;
    case OpsClass( OP_STK1, OP_STK0 ):
        ins->u.gen_table = GenTab( ins, RNFBINP );
        break;
    case OpsClass( OP_MEM , OP_STK0 ):
        ins->u.gen_table = GenTab( ins, MRFBIN );
        break;
    case OpsClass( OP_CONS, OP_STK0 ):
        PrefixFLDOp( back, ins, op1, 0 );
        ins->u.gen_table = GenTab( ins, RRFBINP );
        ins->operands[0] = back.i87exp->ST1;
        break;
    case OpsClass( OP_STK0, OP_STK1 ):
        ins->u.gen_table = GenTab( ins, RRFBINP );
        ExchangeOps( ins );
        break;
    case OpsClass( OP_STK0, OP_MEM  ):
        ins->u.gen_table = GenTab( ins, MNFBIN );
        ExchangeOps( ins );
        break;
    case OpsClass( OP_MEM , OP_MEM  ):
    case OpsClass( OP_CONS, OP_MEM  ):
        PrefixFLDOp( back, ins, op1, 0 );
        ins->u.gen_table = GenTab( ins, MNFBIN );
        ExchangeOps( ins );
        break;
    case OpsClass( OP_STK0, OP_CONS ):
        PrefixFLDOp( back, ins, op2, 1 );
        ins->u.gen_table = GenTab( ins, RNFBINP );
        ins->operands[0] = back.i87exp->ST1;
        ins->operands[1] = back.i87exp->ST0;
        break;
    case OpsClass( OP_MEM , OP_CONS ):
        PrefixFLDOp( back, ins, op2, 1 );
        ins->u.gen_table = GenTab( ins, MRFBIN );
        break;
    default:
        Zoiks( back, ZOIKS_011 );
        break;
    }
}


static  void    RevOtherCond( block *blk, instruction *ins )
/*************************************************************
    Run through block "blk" and its successors, continuing until we hit
    an instruction that changes the condition codes, or uses a previous
    set of condition codes.  When we hit and instruction that uses
    previous codes, reverse it since we changed the compare instruction
    up in RevFPCond.
*/
{
    block_num   i;
    block       *target;

    MarkBlkVisited( blk );
    for( ;; ) {
        ins = ins->head.next;
        if( ins->head.opcode == OP_BLOCK ) {
            for( i = blk->targets; i-- > 0; ) {
                target = blk->edge[i].destination.u.blk;
                if( !IsBlkVisited( target ) ) {
                    RevOtherCond( target, (instruction *)&target->ins );
                }
            }
            break;
        }
        if( ( ins->u.gen_table->op_type & MASK_CC ) != PRESERVE )
            break;
        if( OpIsCondition( ins->head.opcode ) && IsNop( ins->table ) ) { /* used cond codes of original ins */
            RevCond( ins );
            ins->table = FNOP;
            ins->u.gen_table = FNOP;
        }
    }
}


static  void    RevFPCond( BackRun& back, instruction *ins )
/********************************************
    Reverse the sense of a floating point comarison (condition).  For
    example, if we're trying to generate OP_COMPARE_GREATER  X, ST(0),
    we would use FCOM X, and then flip the the comparison to
    OP_COMPARE_LESS.  Since condition code scoreboarding has already
    been done, we have to also go to any subsequent basic blocks and
    flip any comparisons that rely on this comparison to set condition
    codes as well.
*/
{
    MarkBlkAllUnVisited( back );
    RevOtherCond( InsBlock( ins ), ins );
    RevCond( ins );
}


static void ExpCompare( BackRun& back, instruction *ins, operand_type op1, operand_type op2 )
/*****************************************************************************
    Expand a floating point comparison using classifications "op1" and
    "op2".
*/
{
    if( !CPULevel( back, CPU_386 ) ) {
        if( back.i87data.FPStatWord == NULL && ( !CPULevel( back, CPU_286 ) || IsEmulation( back ) ) ) {
            back.i87data.FPStatWord = AllocTemp( back, U2 );
            back.i87data.FPStatWord->v.usage |= VAR_VOLATILE | USE_ADDRESS; /* so that it really gets allocd */
            AllocALocal( back, back.i87data.FPStatWord );
        }
    }
    switch( OpsClass( op1, op2 ) ) {
    case OpsClass( OP_STK1, OP_STK1 ):
    case OpsClass( OP_MEM , OP_STK1 ):
    case OpsClass( OP_CONS, OP_STK1 ):
    case OpsClass( OP_STK1, OP_MEM  ):
    case OpsClass( OP_STK1, OP_CONS ):
    case OpsClass( OP_CONS, OP_CONS ):
/*   case 0,6,7,8,9,11,16,21,24:*/
        Zoiks( back, ZOIKS_012 );
        break;
    case OpsClass( OP_STK0, OP_STK0 ):
        ins->u.gen_table = RCOMP;
        break;
    case OpsClass( OP_STK1, OP_STK0 ):
        ins->u.gen_table = FCOMPP;
        RevFPCond( back, ins );
        break;
    case OpsClass( OP_MEM , OP_STK0 ):
        ins->u.gen_table = MCOMP;
        RevFPCond( back, ins );
        break;
    case OpsClass( OP_CONS, OP_STK0 ):
        PrefixFLDOp( back, ins, op1, 0 );
        ins->u.gen_table = FCOMPP;
        break;
    case OpsClass( OP_STK0, OP_STK1 ):
        ins->u.gen_table = FCOMPP;
        break;
    case OpsClass( OP_STK0, OP_MEM  ):
        ins->u.gen_table = MCOMP;
        ExchangeOps( ins );
        break;
    case OpsClass( OP_MEM , OP_MEM  ):
    case OpsClass( OP_CONS, OP_MEM  ):
        PrefixFLDOp( back, ins, op1, 0 );
        ins->u.gen_table = MCOMP;
        ExchangeOps( ins );
        break;
    case OpsClass( OP_STK0, OP_CONS ):
        PrefixFLDOp( back, ins, op2, 1 );
        ins->u.gen_table = FCOMPP;
        RevFPCond( back, ins );
        break;
    case OpsClass( OP_MEM , OP_CONS ):
        PrefixFLDOp( back, ins, op2, 1 );
        ins->u.gen_table = MCOMP;
        RevFPCond( back, ins );
        break;
    default:
        Zoiks( back, ZOIKS_013 );
        break;
    }
}


static instruction *ExpandFPIns( BackRun& back, instruction *ins, operand_type op1,
                                      operand_type op2, result_type res )
/**************************************************************************
    Using the operand/result classifications "op1", "op2", "res", turn
    "ins" into a real live 8087 instruction by deciding what to generate
    for it (setting gen_table).  Note that it may turn into multiple
    8087 instructions.
*/
{
    if( !Is87Ins( ins ) )
        return( ins );
    if( op2 != OP_NONE ) {
        switch( ins->head.opcode ) {
        case OP_P5DIV:
        case OP_POW:
        case OP_ATAN2:
        case OP_FMOD:
            ExpBinFunc( back, ins, op1, op2 );
            if( res != RES_STK0 ) {
                ins = SuffixFSTPRes( back, ins );
            }
            break;
        default:
            switch( res ) {
            case RES_MEM:
                ExpBinary( back, ins, op1, op2 );
                ins = SuffixFSTPRes( back, ins );
                break;
            case RES_STK0:
                ExpBinary( back, ins, op1, op2 );
                break;
            case RES_NONE:
                ExpCompare( back, ins, op1, op2 );
                break;
            default:
                break;
            }
            break;
        }
   } else {
        switch( ins->head.opcode ) {
        case OP_PUSH:
            ins = ExpPush( back, ins, op1 );
            break;
        case OP_NEGATE:
            ins = ExpUnary( back, ins, op1, res, FCHS );
            break;
        case OP_FABS:
            ins = ExpUnary( back, ins, op1, res, FMATH );
            break;
        case OP_COS:
        case OP_SIN:
        case OP_TAN:
        case OP_SQRT:
        case OP_ATAN:
            if( FPULevel( back, FPU_387 )
              && IsModel( back, CGSW_GEN_I_MATH_INLINE )
              && IsntTargetModel( back, CGSW_X86_P5_DIVIDE_CHECK ) ) {
                ins = ExpUnary( back, ins, op1, res, FMATH );
            } else {
                ins = ExpUnary( back, ins, op1, res, IFUNC );
            }
            break;
        case OP_LOG:
        case OP_LOG10:
        case OP_EXP:
            if( FPULevel( back, FPU_387 )
              && IsModel( back, CGSW_GEN_I_MATH_INLINE )
              && IsntTargetModel( back, CGSW_X86_P5_DIVIDE_CHECK )
              && back.data.OptForSize <= 50 ) {
                ins = ExpUnary( back, ins, op1, res, FMATH );
            } else {
                ins = ExpUnary( back, ins, op1, res, IFUNC );
            }
            break;
        case OP_ACOS:
        case OP_ASIN:
        case OP_COSH:
        case OP_SINH:
        case OP_TANH:
            ins = ExpUnary( back, ins, op1, res, IFUNC );
            break;
        case OP_MOV:
        case OP_CONVERT:
        case OP_ROUND:
            if( ins->result->n.class_ == N_REGISTER
              && HW_CEqual( ins->result->r.reg, HW_EMPTY ) ) {
                ins->result = back.i87exp->ST0;
                ins->u.gen_table = RFST;
            } else {
                ins = ExpMove( back, ins, op1, res );
            }
            break;
        default:
            break;
        }
    }
    return( ins );
}

static operand_type CheckFPOp( BackRun& back, instruction *ins, int i )
/*******************************************************
    The bulk of this routine is spent classifying the operands of
    the instruction, and if they are an 8087 register, adjusting
    them to the real register number to be used in the instruction.
*/
{
    int             reg_num;
    operand_type    op_type;

    reg_num = FPRegNum( ins->operands[i] );
    if( reg_num != -1 ) {
        if( reg_num == 0 ) {
            op_type = OP_STK0;
        } else {
            /* ??? op_type should be setup to OP_STK1/OP_STKI ??? */
            op_type = static_cast<operand_type>(ins->stk_entry - reg_num);
            ins->operands[i] = ST( back, ins->stk_entry - reg_num );
        }
    } else if( ins->operands[i]->n.class_ == N_CONSTANT ) {
        op_type = OP_CONS;
    } else {
        op_type = OP_MEM;
    }
    return( op_type );
}

static result_type CheckFPRes( BackRun& back, instruction *ins )
/*******************************************************
    The bulk of this routine is spent classifying the result of
    the instruction, and if it is an 8087 register, adjusting
    them to the real register number to be used in the instruction.
*/
{
    int             reg_num;
    result_type     res_type;

    if( OpIsCondition( ins->head.opcode ) ) {
        res_type = RES_NONE;
    } else {
        reg_num = FPRegNum( ins->result );
        res_type = RES_NONE;
        if( reg_num != -1 ) {
            /* ??? res_type should be setup to RES_STK0/RES_STKI ??? */
            res_type = RES_STK0;
            ins->result = back.i87exp->ST0;
        } else {
            res_type = RES_MEM;
        }
    }
    return( res_type );
}

static  instruction     *DoExpand( BackRun& back, instruction *ins )
/******************************************************
    Expand one instruction "ins" The bulk of this routine is
    spent classifying the operands and result of the instruction, and if
    they are an 8087 register, adjusting them to the real register
    number to be used in the instruction.
*/
{
    opcnt               i;
    operand_type        op1_type;
    operand_type        op2_type;
    result_type         res_type;

    op1_type = OP_NONE;
    op2_type = OP_NONE;
    i = OpcodeNumOperands( ins );
    if( i > 0 ) {
        op1_type = CheckFPOp( back, ins, 0 );
        if( i > 1 ) {
            op2_type = CheckFPOp( back, ins, 1 );
        }
    }
    res_type = CheckFPRes( back, ins );
    return( ExpandFPIns( back, ins, op1_type, op2_type, res_type ) );
}

static  void    Expand( BackRun& back )
/*******************************
    Run through the instruction stream expanding each
    instruction.  All 8087 instructions will have
        gen_table->generate == G_UNKNOWN.

*/
{
    block       *blk;
    instruction *ins;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( DoesSomething( ins ) ) {
                if( OpIsCall( ins->head.opcode ) ) {
                    ins = ExpCall( back, ins );
                } else if( InsGenerate( ins ) == G_UNKNOWN ) {
                    ins = DoExpand( back, ins );
                }
            }
        }
    }
}


void    NoPopRBin( instruction *ins )
/*********************************************
    Turn a binary floating point instruction into its equivalent version
    that does not pop the stack. For example FMULP ST(1),ST  -> FMUL ST(1),ST
*/
{
    if( InsGenerate( ins ) == G_RRFBINP ) {
        ins->u.gen_table = GenTab( ins, RRFBIND );
        ins->stk_exit++;
    } else if( InsGenerate( ins ) == G_RNFBINP ) {
        ins->u.gen_table = GenTab( ins, RNFBIND );
        ins->stk_exit++;
    }
}


void    NoPopBin( instruction *ins )
/*********************************************
    Turn a binary floating point instruction into its equivalent version
    that does not pop the stack. For example FMULP ST(1),ST  -> FMUL X
*/
{
    if( InsGenerate( ins ) == G_RRFBINP ) {
        if( ins->operands[0]->n.class_ == N_REGISTER ) {
            ins->u.gen_table = GenTab( ins, RRFBIN );
        } else {
            ins->u.gen_table = GenTab( ins, MRFBIN );
        }
        ins->stk_exit++;
    } else if( InsGenerate( ins ) == G_RNFBINP ) {
        if( ins->operands[0]->n.class_ == N_REGISTER ) {
            ins->u.gen_table = GenTab( ins, RNFBIN );
        } else {
            ins->u.gen_table = GenTab( ins, MNFBIN );
        }
        ins->stk_exit++;
    }
}


void    ToPopBin( instruction *ins )
/***********************************
    Turn a binary floating point instruction into its equivalent version
    that pops the stack. For example FSUB ST,ST(1) -> FSUBRP ST(1),ST
*/
{
    if( InsGenerate( ins ) == G_RRFBIN ) {
        ins->u.gen_table = GenTab( ins, RNFBINP );
        ins->stk_exit--;
    } else if( InsGenerate( ins ) == G_RNFBIN ) {
        ins->u.gen_table = GenTab( ins, RRFBINP );
        ins->stk_exit--;
    }
}


void    ReverseFPGen( instruction *ins )
/*********************************************
    Turn a binary floating point instruction into its reverse instruction
    For example FSUB ST(1)  -> FSUBR ST(1)
*/
{
    switch( InsGenerate( ins ) ) {
    case G_RRFBIN:
        ins->u.gen_table = GenTab( ins, RNFBIN );
        break;
    case G_RNFBIN:
        ins->u.gen_table = GenTab( ins, RRFBIN );
        break;
    case G_RRFBINP:
        ins->u.gen_table = GenTab( ins, RNFBINP );
        break;
    case G_RNFBINP:
        ins->u.gen_table = GenTab( ins, RRFBINP );
        break;
    case G_MRFBIN:
        ins->u.gen_table = GenTab( ins, MNFBIN );
        break;
    case G_MNFBIN:
        ins->u.gen_table = GenTab( ins, MRFBIN );
        break;
    }
}


void    ToRFld( instruction *ins )
/**********************************
    Turn an FLD x into an FLD ST(i)
*/
{
    ins->u.gen_table = RFLD;
}


void    ToRFstp( instruction *ins )
/**********************************
    Turn an FLD x into an FLD ST(i)
*/
{
    ins->u.gen_table = RFST;
}


void    NoPop( instruction *ins )
/*************************************
    Turn an FSTP instruction into FST.
*/
{
    if( InsGenerate( ins ) == G_MFST ) {
        ins->u.gen_table = MFSTNP;
        ins->stk_exit++;
    } else if( InsGenerate( ins ) == G_RFST ) {
        ins->u.gen_table = RFSTNP;
        ins->stk_exit++;
    }
}


void    NoMemBin( instruction *ins )
/**************************************
    FADD x  ==>  FADD ST(0) for example
*/
{
    if( InsGenerate( ins ) == G_MRFBIN ) {
        ins->u.gen_table = GenTab( ins, RRFBIN );
    } else if( InsGenerate( ins ) == G_MNFBIN ) {
        ins->u.gen_table = GenTab( ins, RNFBIN );
    }
}


instruction     *MakeWait( BackRun& back )
/************************************
    Create an "instruction" that will generate an FWAIT.
*/
{
    instruction *new_ins;

    new_ins = MakeUnary( back, OP_MOV, back.i87exp->ST0, back.i87exp->ST0, FD );
    new_ins->u.gen_table = FWAIT;
    return( new_ins );
}

void    FPInit( BackRun& back )
/*********************
    Initialize.
*/
{
    back.i87data.Max87Stk = (int)(pointer_uint)FEAuxInfo( *back.front, NULL, FEINF_STACK_SIZE_8087 );
    if( back.i87data.Max87Stk > 8 )
        back.i87data.Max87Stk = 8;
    if( back.i87data.Max87Stk < 4 )
        back.i87data.Max87Stk = 4;
    back.i87data.Used87 = false;
    FPInitStkReq(back);
}

void    FPExpand( BackRun& back )
/**************************
    Expand the 8087 instructions.  The instructions so far
    have been assigned registers, with ST(1) ..  ST(Max87Stk) being the
    "stack" portion of the 8087 with the register number indicating the
    depth of the stack at the time the instruction is executed.
    ST(0) is special and always means the current top of stack

*/
{
    if( FPULevel( back, FPU_87 ) ) {
        back.i87exp->ST0 = ST( back, 0 );
        back.i87exp->ST1 = ST( back, 1 );
        Expand(back);
    }
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
I87expState* i87exp_make() { return new I87expState(); }
void i87exp_free(I87expState* part) { delete part; }
