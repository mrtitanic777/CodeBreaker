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
* Description:  Generate calls to runtime support routines.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "seldef.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "makeins.h"
#include "convins.h"
#include "data.h"
#include "rtrtn.h"
#include "x86objd.h"
#include "objout.h"
#include "namelist.h"
#include "regalloc.h"
#include "makeblk.h"
#include "rgtbl.h"
#include "insutil.h"
#include "rtcall.h"
#include "optab.h"
#include "inssegs.h"
#include "fixindex.h"
#include "conflict.h"
#include "x86data.h"
#include "x86segs.h"
#include "liveinfo.h"
#include "x86rtrn.h"
#include "bldselco.h"
#include "i64.h"
#include "_x86rtrn.h"
#include "backrun.hpp"


/*
 * OK, so I'm going to go to hell for this, it works don't it?
 */
#define RL_SI   RL_TEMP_INDEX

/*
 * If you add a new routine, add it to the debugger's symbol list
 * so the debugger can recognize it.
 */
const rtn_info RTInfo[] = {
    #define PICK(e,name,op,class_,left,right,result) {name, op, class_, left, right, result},
    #define PICK1(e,name,op,class_,left,right,result) __FP80BIT(PICK(e,name,op,class_,left,right,result),)
    #include "_rtinfo.h"
    #undef PICK1
    #undef PICK
};


namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct I86rtrtnState {
    const struct STRUCT_BYTE_SEQ( 6 ) Scn1 = {
         6, false,
        {0xF2,              /*      repne           */
         0xAE,              /*      scasb           */
         0xD1, 0xE1,        /*      shl     cx,1    */
         0x89, 0xCF}        /*      mov     di,cx   */
    };
    const struct STRUCT_BYTE_SEQ( 2 ) Scn2 = {
         2, false,
        {0xF2,              /*      repne           */
         0xAF}              /*      scasw           */
    };
    const struct STRUCT_BYTE_SEQ( 18 ) Scn4 = {
         18, false,
        {0x83, 0xC7, 0x02,  /* L1:  add     d1,2    */
         0x49,              /* L2:  dec     cx      */
         0x74, 0x08,        /*      je      L3      */
         0xAF,              /*      scasw           */
         0x75, 0xF7,        /*      jne     L1      */
         0x92,              /*      xchg    dx,ax   */
         0xAF,              /*      scasw           */
         0x92,              /*      xchg    dx,ax   */
         0x75, 0xF5,        /*      jne     L2      */
         0xD1, 0xE1,        /* L3:  shl     cx,1    */
         0x89, 0xCF}        /*      mov     di,cx   */
    };
};
namespace {

}  // namespace



const char  *AskRTName( BackRun& back, rt_class rtindex )
/****************************************/
{
    return( RTInfo[rtindex].name );
}


name    *Addressable( BackRun& back, name *cons, type_class_def type_class )
/************************************************************
 * make sure a floating point constant is addressable (dropped
 * it into memory if it isnt)
 */
{
    unsigned_64         buffer;

    if( cons->n.class_ == N_CONSTANT ) {
        switch( type_class ) {
        case FL:
        case FD:
        case FS:
            return( GenFloat( back, cons, type_class ) );
        case U8:
        case I8:
            U64Low( buffer ) = cons->c.lo.u.int_value;
            U64High( buffer ) = cons->c.hi.u.int_value;
            return( GenConstData( back, &buffer, type_class ) );
        default:
            Zoiks( back, ZOIKS_138 );
            break;
        }
        return( GenFloat( back, cons, type_class ) );
    }
    return( cons );
}


static rt_class CheckForPCS( instruction *ins, rt_class rtindex )
/****************************************************************
 * check to see if pointer subtract is really pointer - pointer
 * or pointer - integer (PCS = Pointer Constant Subtract)
 */
{
    if( rtindex == RT_PTS ) {
        if( ins->operands[1]->n.type_class != PT
          && ins->operands[1]->n.type_class != CP ) {
            rtindex = RT_PCS;
        }
    }
    return( rtindex );
}


bool    RTLeaveOp2( BackRun& back, instruction *ins )
/*************************************
 * return true if it's a bad idea to put op2 into a temporary since we're
 * gonna take the bugger's address in rMAKECALL anyway for FDD, FDC, EDA, etc
 */
{
    switch( ins->type_class ) {
    case FD:
        if( FPULevel( back, FPU_87 ) )
            return( false );
        break;
/* -- This is not true now - I8 math and parameters are kept in registers -- [RomanT]
    case I8:
    case U8:
        break;
*/
    default:
        return( false );
    }
    if( OpcodeNumOperands( ins ) != 2 )
        return( false );
    return( true );
}


static  void    FlipIns( instruction *ins )
/******************************************
 * maybe flip the const/mem into the second operand so it goes in local data
 */
{
    name        *temp;

    switch( ins->head.opcode ) {
    case OP_ADD:
    case OP_MUL:
        break;
    default:
        return;
    }
    switch( ins->type_class ) {
    case FD:
    case I8:
    case U8:
        break;
    default:
        return;
    }
    temp = ins->operands[0];
    if( temp->n.class_ == N_CONSTANT
      || temp->n.class_ == N_MEMORY
      || temp->n.class_ == N_INDEXED ) {
        ins->operands[0] = ins->operands[1];
        ins->operands[1] = temp;
    }
}


instruction     *rMAKECALL( BackRun& back, instruction *ins )
/*********************************************
 * turn an instruction into the approprate runtime call sequence, using
 * the tables above to decide where parms go.
 */
{
    const rtn_info      *info;
    label_handle        lbl;
    instruction         *left_ins;
    instruction         *new_ins;
    instruction         *la_ins;
    instruction         *last_ins;
    name                *reg_name;
    hw_reg_set          regs;
    hw_reg_set          all_regs;
    name                *temp;
    name                *also_used;
    conflict_node       *conf;
    hw_reg_set          tmp;
    type_class_def      parm2_class;
    rt_class            rtindex;

    if( !_IsConvert( ins ) ) {
        rtindex = CheckForPCS( ins, LookupRoutine( back, ins ) );
    } else {
        /*
         * rtindex might be wrong if we ran out of memory in ExpandIns
         */
        rtindex = LookupConvertRoutine( back, ins );
    }
    FlipIns( ins );
    info = &RTInfo[rtindex];
    regs = FirstReg( back, info->left );
    all_regs = regs;
    tmp = ReturnReg( back, WD, false );
    HW_TurnOn( all_regs, tmp );
    left_ins = MakeMove( back, ins->operands[0], AllocRegName( back, regs ), info->operand_class );
    ins->operands[0] = left_ins->result;
    MoveSegOp( back, ins, left_ins, 0 );
    PrefixIns( back, ins, left_ins );
    regs = FirstReg( back, info->right );
    also_used = NULL;
    if( !HW_CEqual( regs, HW_EMPTY ) ) {
        if( info->right == RL_8 ) {
            temp = ins->operands[1];
            if( temp->n.class_ == N_TEMP
              && (temp->t.temp_flags & CONST_TEMP) ) {
                temp = static_cast<name *>(temp->v.symbol);
            }
            if( temp->n.class_ == N_CONSTANT ) {
                temp = Addressable( back, temp, info->operand_class );
                la_ins = MakeUnary( back, OP_CAREFUL_LA, temp, AllocRegName( back, HW_SI ), U2 );
                also_used = la_ins->operands[0];
                ins->operands[1] = la_ins->result;
                PrefixIns( back, ins, la_ins );
                if( !SegIsSS( back, temp ) ) {
                    new_ins = MakeMove( back, GetSegment( back, temp ), AllocRegName( back, HW_ES ), U2 );
                    HW_CTurnOn( all_regs, HW_ES );
                    PrefixIns( back, ins, new_ins );
                    ++rtindex;
                }
            } else if( temp->n.class_ == N_TEMP ) {
                la_ins = MakeUnary( back, OP_CAREFUL_LA,temp, AllocRegName( back, HW_SI ),U2 );
                also_used = temp;
                ins->operands[1] = la_ins->result;
                MoveSegOp( back, ins, la_ins, 0 );
                DelSeg( la_ins );
                PrefixIns( back, ins, la_ins );
            } else if( ins->num_operands == 3 ) {
                new_ins = MakeMove( back, ins->operands[2], AllocRegName( back, HW_ES ), U2 );
                ins->operands[2] = new_ins->result;
                PrefixIns( back, ins, new_ins );
                la_ins = MakeUnary( back, OP_CAREFUL_LA, temp, AllocRegName( back, HW_SI ), U2 );
                also_used = temp;
                ins->operands[1] = la_ins->result;
                MoveSegOp( back, ins, la_ins, 0 );
                DelSeg( la_ins );
                PrefixIns( back, ins, la_ins );
                HW_CTurnOn( all_regs, HW_ES );
                ++rtindex;
            } else if( ( temp->n.class_ == N_MEMORY
                    && !SegIsSS( back, temp ) )
                  || ( temp->n.class_ == N_INDEXED
                    && temp->i.base != NULL
                    && !SegIsSS( back, temp->i.base ) ) ) {
                la_ins = MakeUnary( back, OP_CAREFUL_LA, ins->operands[1], AllocRegName( back, HW_ES_SI ), PT );
                also_used = ins->operands[1];
                ins->operands[1] = la_ins->result;
                PrefixIns( back, ins, la_ins );
                HW_CTurnOn( all_regs, HW_ES );
                ++rtindex;
            } else {
                la_ins = MakeUnary( back, OP_CAREFUL_LA, temp, AllocRegName( back, HW_SI ), U2 );
                also_used = temp;
                ins->operands[1] = la_ins->result;
                MoveSegOp( back, ins, la_ins, 0 );
                DelSeg( la_ins );
                PrefixIns( back, ins, la_ins );
            }
            HW_CTurnOn( all_regs, HW_SI );
        } else {
            /*
             * If I knew how to turn a register list index into a type class,
             * I'd do that, and avoid this if
             */
            if( info->right == RL_SI ) {
                new_ins = MakeMove( back, ins->operands[1], AllocRegName( back, regs ), U2 );
            } else {
                new_ins = MakeMove( back, ins->operands[1], AllocRegName( back, regs ), info->operand_class );
            }
            ins->operands[1] = new_ins->result;
            MoveSegOp( back, ins, new_ins, 0 );
            HW_TurnOn( all_regs, regs );
            PrefixIns( back, ins, new_ins );
        }
    } else if( OpcodeNumOperands( ins ) == 2 ) {
        if( rtindex == RT_DPOWI ) {
            parm2_class = I4;
        } else {
            parm2_class = ins->type_class;
        }
        new_ins = MakeUnary( back, OP_PUSH, ins->operands[1], NULL, parm2_class );
        MoveSegOp( back, ins, new_ins, 0 );
        PrefixIns( back, ins, new_ins );
    }
    reg_name = AllocRegName( back, all_regs );
    lbl = RTLabel( back, rtindex );
    new_ins = NewIns( back, 3 );
    new_ins->head.opcode = OP_CALL;
    new_ins->type_class = ins->type_class;
    new_ins->operands[CALL_OP_USED] = reg_name;
    if( also_used != NULL ) {
        switch( also_used->n.class_ ) {
        case N_REGISTER:
        case N_CONSTANT:
            Zoiks( back, ZOIKS_047 );
            break;
        case N_MEMORY:
        case N_TEMP:
            also_used->v.usage |= (USE_MEMORY | NEEDS_MEMORY);
            conf = also_used->v.conflict;
            if( conf != NULL ) {
                InMemory( back, conf );
            }
            break;
        case N_INDEXED:
            /*
             * ! the index doesn't need an index register !
             */
            also_used = reg_name;
            /* fall through */
        default:
            break;
        }
        new_ins->operands[CALL_OP_USED2] = also_used;
    } else {
        new_ins->operands[CALL_OP_USED2] = reg_name;
    }
    new_ins->operands[CALL_OP_ADDR] = AllocMemory( back, lbl, 0, CG_LBL, ins->type_class );
    new_ins->result = NULL;
    new_ins->num_operands = 2;                      /* special case for OP_CALL*/
    new_ins->zap = &AllocRegName( back, all_regs )->r;    /* all parm regs could be zapped*/
    last_ins = new_ins;
    if( ins->result != NULL ) {
        regs = FirstReg( back, info->result );
        tmp = regs;
        HW_TurnOn( tmp, new_ins->zap->reg );
        new_ins->zap = &AllocRegName( back, tmp )->r;
        reg_name = AllocRegName( back, regs );
        new_ins->result = reg_name;
        last_ins = MakeMove( back, reg_name, ins->result, ins->type_class );
        ins->result = last_ins->operands[0];
        MoveSegRes( back, ins, last_ins );
        SuffixIns( back, ins, last_ins );
        ReplIns( back, ins, new_ins );
    } else {
        /*
         * comparison, still need conditional jumps
         */
        ins->operands[0] = AllocIntConst( back, 0 );
        ins->operands[1] = AllocIntConst( back, 1 );
        DelSeg( ins );
        DoNothing( back, ins );       /* just conditional jumps for ins */
        PrefixIns( back, ins, new_ins );
        new_ins->ins_flags |= INS_CC_USED;
        last_ins = ins;
    }
    UpdateLive( back, left_ins, last_ins );
    return( left_ins );
}


name    *ScanCall( BackRun& back, tbl_control *table, name *value, type_class_def type_class )
/******************************************************************************
 * generates a fake call to a runtime routine that looks up "value" in a table
 * and jumps to the appropriate case, using either a pointer or index
 * returned by the "routine". The "routine" will be generated inline later.
 * See BEAuxInfo for the code sequences generated. That will explain
 * how the jump destination is determined as well.
 */
{
    instruction *new_ins;
    name        *reg_name;
    name        *result;
    name        *label;
    hw_reg_set  tmp;
    name        *temp_result;
    rt_class    rtindex;

    switch( type_class ) {
    case U1:
        rtindex = RT_SCAN1;
        break;
    case U2:
        rtindex = RT_SCAN2;
        break;
    case U4:
        rtindex = RT_SCAN4;
        break;
    default:
        rtindex = RT_NOP;
        break;
    }

    reg_name = AllocRegName( back, FirstReg( back, RTInfo[rtindex].left ) );
    new_ins = MakeConvert( back, value, reg_name, type_class, value->n.type_class );
    AddIns( back, new_ins );

    reg_name = AllocRegName( back, HW_CX );
    new_ins = MakeMove( back, AllocRegName( back, HW_CS ), reg_name, U2 );
    AddIns( back, new_ins );
    new_ins = MakeMove( back, reg_name, AllocRegName( back, HW_ES ), U2 );
    AddIns( back, new_ins );
    new_ins = MakeMove( back, AllocIntConst( back, table->size + 1 ), reg_name, U2 );
    AddIns( back, new_ins );

    reg_name = AllocRegName( back, HW_DI );
    if( type_class == U4 ) {
        label = AllocMemory( back, table, -2, CG_VTB, U2 );
    } else {
        label = AllocMemory( back, table, 0, CG_VTB, U2 );
    }
    label = AddrConst( back, label, AskCodeSeg(back), CONS_OFFSET );
    new_ins = MakeMove( back, label, reg_name, U2 );
    AddIns( back, new_ins );

    new_ins = NewIns( back, 3 );
    new_ins->head.opcode = OP_CALL;
    new_ins->type_class = U2;
    tmp = FirstReg( back, RTInfo[rtindex].left );
    HW_CTurnOn( tmp, HW_ES_DI );
    HW_CTurnOn( tmp, HW_CX );
    new_ins->operands[CALL_OP_USED] = AllocRegName( back, tmp );
    new_ins->operands[CALL_OP_USED2] = new_ins->operands[CALL_OP_USED];
    new_ins->operands[CALL_OP_ADDR] = AllocMemory( back, RTLabel( back, rtindex ), 0, CG_LBL, U2 );
    new_ins->result = NULL;
    new_ins->num_operands = 2;
    new_ins->zap = &AllocRegName( back, HW_CX_DI )->r;
    new_ins->result = reg_name;
    AddIns( back, new_ins );

    result = AllocMemory( back, table, 0, CG_TBL, U2 ); /* so table gets freed!*/
    if( type_class == U2 ) {
        result = AllocRegName( back, HW_ES_DI );
        result = AllocIndex( back, result, NULL, ( table->size - 1 ) * 2, U2 );
    } else {
        result = AllocIndex( back, reg_name, result, 0, U2 );
    }
    /*
     * this is here because we can get ourselves into trouble
     * by hoisting expressions into the spot between the call and
     * the OP_SELECT instruction at the end of the block if those
     * expressions expand to instructions which require ECX.
     * Same goes for EDI in the U4 case.
     */
    temp_result = AllocTemp( back, WD );
    new_ins = MakeMove( back, result, temp_result, WD );
    AddIns( back, new_ins );
    return( temp_result );
}


instruction     *rMAKEFNEG( BackRun& back, instruction *ins )
/*********************************************
 * negating a floating point value which is in the 386 registers only
 * needs to change the register containing the exponent, so this is
 * handled as a special case rather than using rMAKERTCALL that would
 * assume all of the registers containing the number were used
 * and modified by the call.
 */
{
    const rtn_info      *info;
    label_handle        lbl;
    instruction         *left_ins;
    instruction         *new_ins;
    instruction         *last_ins;
    name                *reg_name;
    name                *exp_reg;
    rt_class            rtindex;

    rtindex = CheckForPCS( ins, LookupRoutine( back, ins ) );
    lbl = RTLabel( back, rtindex );
    info = &RTInfo[rtindex];
    reg_name = AllocRegName( back, FirstReg( back, info->left ) );
    left_ins = MakeMove( back, ins->operands[0], reg_name, info->operand_class );
    ins->operands[0] = left_ins->result;
    MoveSegOp( back, ins, left_ins, 0 );
    PrefixIns( back, ins, left_ins );
    if( ins->type_class == FD ) {
        /*
         * exponent in AX
         */
        exp_reg = AllocRegName( back, HW_AX );
    } else {
        /*
         * exponent in DX
         */
        exp_reg = AllocRegName( back, HW_DX );
    }
    new_ins = NewIns( back, 3 );
    new_ins->head.opcode = OP_CALL;
    new_ins->type_class = U2;
    new_ins->operands[CALL_OP_USED] = exp_reg;
    new_ins->operands[CALL_OP_USED2] = exp_reg;
    new_ins->operands[CALL_OP_ADDR] = AllocMemory( back, lbl, 0, CG_LBL, U2 );
    new_ins->result = NULL;
    new_ins->num_operands = 2;
    new_ins->zap = &exp_reg->r;
    last_ins = new_ins;
    new_ins->result = exp_reg;
    last_ins = MakeMove( back, reg_name, ins->result, info->operand_class );
    ins->result = last_ins->operands[0];
    MoveSegRes( back, ins, last_ins );
    SuffixIns( back, ins, last_ins );
    ReplIns( back, ins, new_ins );
    UpdateLive( back, left_ins, last_ins );
    return( left_ins );
}


pointer BEAuxInfo( BackRun& back, pointer hdl, aux_class request )
/**************************************************
 * see ScanCall for explanation
 */
{
    switch( request ) {
    case FEINF_AUX_LOOKUP:
        switch( FindRTLabel( back, static_cast<label_handle>(hdl) ) ) {
        case RT_SCAN1:
            return( (pointer)&back.i86rtrtn->Scn1 );
        case RT_SCAN2:
            return( (pointer)&back.i86rtrtn->Scn2 );
        case RT_SCAN4:
            return( (pointer)&back.i86rtrtn->Scn4 );
        default:
            break;
        }
        break;
    case FEINF_CALL_CLASS:
        return( (pointer)FECALL_GEN_NONE );
    case FEINF_CALL_CLASS_TARGET:
        return( (pointer)FECALL_X86_NONE );
    case FEINF_CALL_BYTES:
        return( hdl );
    default:
        Zoiks( back, ZOIKS_128 );
        break;
    }
    return( NULL );
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
I86rtrtnState* i86rtrtn_make() { return new I86rtrtnState(); }
void i86rtrtn_free(I86rtrtnState* part) { delete part; }
