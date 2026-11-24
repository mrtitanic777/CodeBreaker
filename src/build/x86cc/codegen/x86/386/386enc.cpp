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
* Description:  32-bit instruction encodings.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "pccode.h"
#include "system.h"
#include "pcencode.h"
#include "zoiks.h"
#include "zeropage.h"
#include "cgaux.h"
#include "p5prof.h"
#include "data.h"
#include "objout.h"
#include "x86objd.h"
#include "objprof.h"
#include "object.h"
#include "encode.h"
#include "x86proc.h"
#include "targetdb.h"
#include "opttell.h"
#include "x86obj.h"
#include "x86esc.h"
#include "rgtbl.h"
#include "namelist.h"
#include "optab.h"
#include "blktrim.h"
#include "tree.h"
#include "treefold.h"
#include "x86data.h"
#include "x86segs.h"
#include "x86enc.h"
#include "feprotos.h"
#include "backrun.hpp"


#define RMR_MOD_IND     0x80
#define RMR_MOD_DIR     5
#define RMR_MOD_SIB     4

static const hw_reg_set IndexTab[] = {
    HW_D( HW_EAX ),
    HW_D( HW_ECX ),
    HW_D( HW_EDX ),
    HW_D( HW_EBX ),
    HW_D( HW_ESP ),
    HW_D( HW_EBP ),
    HW_D( HW_ESI ),
    HW_D( HW_EDI )
};


static void OpndSizeIf( BackRun& back, bool if_32 )
/**********************************/
{
    if( ( if_32 && IsTargetModel( back, CGSW_X86_USE_32 ) )
      || IsntTargetModel( back, CGSW_X86_USE_32 ) ) {
        AddToTemp( back, M_OPND_SIZE );
    }
}


static void TakeUpSlack( BackRun& back, type_length size )
/*****************************************/
{
    for( ; size >= 2; size -= 2 ) {
        if( IsTargetModel( back, CGSW_X86_USE_32 ) )
            AddByte( back, M_OPND_SIZE );
        AddByte( back, M_MOVSW );
    }
    for( ; size >= 1; --size ) {
        AddByte( back, M_MOVSB );
    }
}


void    DoRepOp( BackRun& back, instruction *ins )
/*********************************/
{
    type_length size;
    bool        first;

    size = ins->result->n.size;
    first = true;
    if( ins->head.opcode == OP_MOV
      && !UseRepForm( back, size ) ) {
        for( ; size >= 4; size -= 4 ) {
            if( first ) {
                LayOpbyte( back, M_MOVSW );
                OpndSizeIf( back, false );
                first = false;
            } else {
                if( IsntTargetModel( back, CGSW_X86_USE_32 ) )
                    AddByte( back, M_OPND_SIZE );
                AddByte( back, M_MOVSW );
            }
        }
        TakeUpSlack( back, size );
    } else {
        LayOpbyte( back, M_REPE );
        if( ins->head.opcode == OP_MOV ) {
            if( (size & ( 4 - 1 )) == 0
              || back.data.OptForSize <= 50 ) {
                if( IsntTargetModel( back, CGSW_X86_USE_32 ) )
                    AddByte( back, M_OPND_SIZE );
                AddByte( back, M_MOVSW );
                TakeUpSlack( back, size & ( 4 - 1 ) );
            } else {
                AddByte( back, M_MOVSB );
            }
        } else {
            if( ins->operands[1]->n.size & 1 ) {
                AddByte( back, M_CMPSB );
            } else {
                AddByte( back, M_CMPSW );
                if( IsntTargetModel( back, CGSW_X86_USE_32 ) ) {
                    AddByte( back, M_OPND_SIZE );
                }
            }
        }
    }
}

static  void    Add32Displacement( BackRun& back, int_32 val )
/*********************************************/
{
    AddByte( back, val );
    AddByte( back, val >> 8 );
    AddByte( back, val >> 16 );
    AddByte( back, val >> 24 );
}

byte    Displacement( BackRun& back, int_32 val, hw_reg_set regs )
/*************************************************/
{
    if( val == 0
      && !HW_COvlap( regs, HW_EBP ) )
        return( DISP0 );
    if( val <= 127
      && val >= -128 ) {
        AddByte( back, val & 0xff );
        return( DISP8 );
    } else {
        Add32Displacement( back, val );
        return( DISPW );
    }
}


byte    DoIndex( BackRun& back, hw_reg_set regs )
/********************************/
{
    byte i;

    for( i = 0; i < sizeof( IndexTab ) / sizeof( IndexTab[0] ); i++ ) {
        if( HwEqual( regs, IndexTab[i] ) ) {
            return( i << S_RMR_RM );
        }
    }
    Zoiks( back, ZOIKS_033 );
    return( 0 );
}


static  byte    DoScaleIndex( BackRun& back, hw_reg_set base_reg, hw_reg_set idx_reg, scale_typ scale )
/**************************************************************************************/
{
    byte        sib;

    sib = scale << 6;
    sib |= DoIndex( back, base_reg ) >> S_RMR_RM;
    sib |= ( DoIndex( back, idx_reg ) >> S_RMR_RM ) << S_RMR_REG;
    AddByte( back, sib );
    return( RMR_MOD_SIB );
}


type_length TmpLoc( BackRun& back, name *base, name *op )
/****************************************/
{
    return( NewBase( back, base ) + op->v.offset - base->v.offset );
}


static  byte    DoDisp( BackRun& back, type_length val, hw_reg_set base_reg, name *op )
/**********************************************************************/
{
    name        *base;

    if( op == NULL ) {
        return( Displacement( back, val, base_reg ) );
    } else if( op->n.class_ == N_TEMP ) {
        base = DeAlias( op );
        if( base->t.location == NO_LOCATION ) {
            Zoiks( back, ZOIKS_034 );
        }
        val += TmpLoc( back, base, op );
        return( Displacement( back, val, base_reg ) );
    } else if( op->n.class_ == N_MEMORY ) {
        back.pcencode.ILen += WORD_SIZE;
        DoSymRef( back, op, val, false );
        return( DISPW );
    } else if( op->n.class_ == N_CONSTANT ) {
        DoRelocConst( back, op, U4 );
        return( DISPW );
    } else {
        Zoiks( back, ZOIKS_126 );
        return( DISP0 );
    }
}

byte    DoMDisp( BackRun& back, name *op, bool alt_encoding )
/********************************************/
{
    hw_reg_set          regs;

    /* unused parameters */ (void)alt_encoding;

    regs = CalcSegment( back, static_cast<cg_sym_handle>(op->v.symbol), op->m.memory_type );
    if( HW_COvlap( regs, HW_SEGS ) ) {
        GenSeg( back, regs );
    }
    DoMAddr( back, op );
    return( RMR_MOD_DIR );
}


static  void    EA( BackRun& back, hw_reg_set base, hw_reg_set index, scale_typ scale,
                         int_32 val, name *mem_loc, bool lea )
/***************************************************************/
{
    if( HW_CEqual( index, HW_ESP ) )
        Zoiks( back, ZOIKS_079 );
    if( HW_CEqual( base, HW_EMPTY ) ) {
        if( HW_CEqual( index, HW_EMPTY ) ) {
            /*
             * [disp32]
             */
            if( scale != SCALE_NONE || val != 0 )
                Zoiks( back, ZOIKS_079 );
            back.pcencode.Inst[RMR] |= DoMDisp( back, mem_loc, true );
        } else {
            if( scale != SCALE_NONE ) {
                /*
                 * [disp32+scale_index]
                 */
                back.pcencode.Inst[RMR] |= DoScaleIndex( back, HW_EBP, index, scale );
                back.pcencode.Inst[RMR] |= DISP0;
                if( mem_loc != NULL ) {
                    back.pcencode.ILen += 4;
                    DoSymRef( back, mem_loc, val, false );
                } else {
                    Add32Displacement( back, val );
                }
            } else {
                /*
                 * [(disp0|disp8|disp32)+index]
                 */
                back.pcencode.Inst[RMR] |= DoIndex( back, index );
                back.pcencode.Inst[RMR] |= DoDisp( back, val, index, mem_loc );
            }
        }
    } else {
        /*
         * [(disp0|disp8|disp32)+base+scale_index]
         */
        if( HW_CEqual( index, HW_EMPTY ) ) {
            if( scale == SCALE_NONE
              && !HW_CEqual( base, HW_ESP ) ) {
                back.pcencode.Inst[RMR] |= DoIndex( back, base );
            } else {
                back.pcencode.Inst[RMR] |= DoScaleIndex( back, base, HW_ESP, scale );
            }
        } else {
            if( scale == SCALE_NONE
              && HW_CEqual( base, HW_EBP )
              && ( lea || IsntTargetModel( back, CGSW_X86_FLOATING_DS ) && IsntTargetModel( back, CGSW_X86_FLOATING_SS ) ) ) {
                /*
                 * flip base & index registers so that we might avoid having
                 * to output a byte displacement (can't have a zero sized
                 * displacement with EBP as the base register)
                 */
                base = index;
                HW_CAsgn( index, HW_EBP );
            }
            back.pcencode.Inst[RMR] |= DoScaleIndex( back, base, index, scale );
        }
        back.pcencode.Inst[RMR] |= DoDisp( back, val, base, mem_loc );
    }
}

/*
 * 2004-11-05  RomanT
 *
 * Check that following instruction is addition or subtraction with constant,
 * and this instruction is safe to delete (ALU flags not generated).
 * If yes, return constant's value and delete instruction.
 * If no, return 0.
 * Caller must embed returned constant into current opcode.
 */

static int_32   GetNextAddConstant( BackRun& back, instruction *ins )
/****************************************************/
{
    instruction *next;
    int         neg;
    int_32      disp;

    next = ins->head.next;
    neg = 1;
    disp = 0;
    switch( next->head.opcode ) {
    case OP_SUB:
        neg = -1;
        /* fall through */
    case OP_ADD:
        if( next->operands[0] != ins->result )
            break;
        if( next->result != ins->result )
            break;
        if( next->operands[1]->n.class_ != N_CONSTANT )
            break;
        if( next->operands[1]->c.const_type != CONS_ABSOLUTE )
            break;
        /*
         * we should not remove the ADD if its flags are used!
         */
        if( next->ins_flags & INS_CC_USED )
            break;
        /*
         * we've got something like:
         *      LEA     EAX, [ECX+EDX]
         *      ADD     EAX, 3
         * turn it into:
         *      LEA     EAX, 3[ECX+EDX]
         */
        disp = neg * next->operands[1]->c.lo.u.int_value;
        DoNothing( back, next );
        break;
    }
    return( disp );
}

void    LayLeaRegOp( BackRun& back, instruction *ins )
/*************************************/
{
    name        *left;
    name        *right;
    scale_typ   scale;
    int         neg;
    int_32      disp;

    left = ins->operands[0];
    right = ins->operands[1];
    neg = 1;
    switch( ins->head.opcode ) { /* add/sub/mul/shift transformed into lea */
    case OP_SUB:
        neg = -1;
        /* fall through */
    case OP_ADD:
        if( right->n.class_ == N_CONSTANT ) {
            if( right->c.const_type == CONS_ABSOLUTE ) {
                EA( back, HW_EMPTY, left->r.reg, SCALE_NONE, neg * right->c.lo.u.int_value, NULL, true );
            } else {
                EA( back, HW_EMPTY, left->r.reg, SCALE_NONE, 0, right, true );
            }
        } else if( right->n.class_ == N_REGISTER ) {
            disp = GetNextAddConstant( back, ins );
            EA( back, left->r.reg, right->r.reg, SCALE_NONE, disp, NULL, true );
        }
        break;
    case OP_MUL:
        switch( right->c.lo.u.int_value ) {
        case 3:     scale = SCALE_2;    break;
        case 5:     scale = SCALE_4;    break;
        case 9:     scale = SCALE_8;    break;
        default:    scale = SCALE_NONE; break;
        }
        disp = GetNextAddConstant( back, ins );   /* 2004-11-05  RomanT */
        EA( back, left->r.reg, left->r.reg, scale, disp, NULL, true );
        break;
    case OP_LSHIFT:
        disp = GetNextAddConstant( back, ins );   /* 2004-11-05  RomanT */
        scale = static_cast<scale_typ>(right->c.lo.u.int_value);
        switch( scale ) {
        case SCALE_2:
            if( CPULevel( back, CPU_586 ) ) {
                /*
                 * want to avoid the extra big-fat 0 on 586's
                 * but two base registers is slower on a 486
                 */
                EA( back, left->r.reg, left->r.reg, SCALE_NONE, disp, NULL, true );
                break;
            }
            /* fall through */
        default:
            EA( back, HW_EMPTY, left->r.reg, scale, disp, NULL, true );
        }
        break;
    }
}


static  void    CheckSize( BackRun& back )
/*******************************/
{
    if( IsntTargetModel( back, CGSW_X86_USE_32 ) ) {
        AddToTemp( back, M_ADDR_SIZE );
    }
}


static  void    LayIdxModRM( BackRun& back, name *op )
/*************************************/
{
    hw_reg_set  base_reg;
    hw_reg_set  idx_reg;
    name        *mem_loc;

    CheckSize(back);
    if( HW_COvlap( op->i.index->r.reg, HW_SEGS ) ) {
        /*
         * check for seg override
         */
        GenSeg( back, op->i.index->r.reg );
    }
    idx_reg = op->i.index->r.reg;
    HW_CTurnOff( idx_reg, HW_SEGS );
    HW_CAsgn( base_reg, HW_EMPTY );
    if( HasTrueBase( op ) ) {
        if( op->i.base->n.class_ == N_TEMP ) {
            if( BaseIsSP( back, op->i.base ) ) {
                HW_CAsgn( base_reg, HW_ESP );
            } else {
                HW_CAsgn( base_reg, HW_EBP );
            }
        } else if( op->i.base->n.class_ == N_MEMORY ) {
            GenSeg( back, CalcSegment( back, static_cast<cg_sym_handle>(op->i.base->v.symbol),
                                 op->i.base->m.memory_type ) );
        }
        mem_loc = op->i.base;
    } else {
        mem_loc = NULL;
    }
    if( op->i.index_flags & X_BASE ) {
        if( op->i.index_flags & X_HIGH_BASE ) {
            base_reg = High64Reg( idx_reg );
        } else {
            base_reg = Low64Reg( idx_reg );
        }
    }
    HwTurnOff( idx_reg, base_reg );
    if( HW_CEqual( idx_reg, HW_ESP )
      && HW_CEqual( base_reg, HW_EMPTY ) ) {
        HW_CAsgn( base_reg, HW_ESP );
        HW_CAsgn( idx_reg, HW_EMPTY );
    }
    EA( back, base_reg, idx_reg, op->i.scale, op->i.constant, mem_loc, false );
}


void    LayModRM( BackRun& back, name *op )
/**************************/
{
    name        *base;

    switch( op->n.class_ ) {
    case N_MEMORY:
        CheckSize(back);
        EA( back, HW_EMPTY, HW_EMPTY, SCALE_NONE, 0, op, false );
        break;
    case N_TEMP:
        CheckSize(back);
        base = DeAlias( op );
        if( base->t.location == NO_LOCATION ) {
            Zoiks( back, ZOIKS_030 );
        }
        if( BaseIsSP( back, base ) ) {
            EA( back, HW_ESP, HW_EMPTY, SCALE_NONE, TmpLoc( back, base, op ), NULL, false );
        } else {
            EA( back, HW_EBP, HW_EMPTY, SCALE_NONE, TmpLoc( back, base, op ), NULL, false );
        }
        break;
    case N_INDEXED:
        LayIdxModRM( back, op );
        break;
    case N_REGISTER:
        LayRMRegOp( back, op );
        break;
    }
}


void    DoMAddr( BackRun& back, name *op )
/*************************/
{
    back.pcencode.ILen += 4;
    if( op->n.class_ == N_CONSTANT ) {
        Zoiks( back, ZOIKS_035 );
    } else {
        /*
         * assume global name
         */
        DoSymRef( back, op, op->v.offset, false );
    }
}


void    DoRelocConst( BackRun& back, name *op, type_class_def type_class )
/*********************************************************/
{
    if( op->c.const_type == CONS_OFFSET ) {
        back.pcencode.ILen += 4;
        DoSymRef( back, op->c.u.op, op->c.u.op->v.offset, false );
    } else if( op->c.const_type == CONS_SEGMENT ) {
        back.pcencode.ILen += 2;
        if( op->c.u.op == NULL ) {
            DoSegRef( back, static_cast<segment_id>(op->c.lo.u.int_value) );
        } else {
            DoSymRef( back, op->c.u.op, 0, true );
        }
        if( type_class == U4
          || type_class == I4 ) {        /* as in PUSH seg _x */
            AddByte( back, 0 );
            AddByte( back, 0 );
        }
    } else if( op->c.const_type == CONS_ADDRESS ) {
        Zoiks( back, ZOIKS_045 );
    }
}


void    GenUnkPush( BackRun& back, pointer value )
/*********************************/
{
    Format( back, OC_CODE );
    LayOpbyte( back, M_PUSHI );
    OpndSizeIf( back, false );
    back.pcencode.ILen += 4;
    DoAbsPatch( back, static_cast<abspatch_handle *>(value), 4 );
    Finalize( back );
}


void    GenPushC( BackRun& back, int_32 value )
/******************************/
{
    Format( back, OC_CODE );
    LayOpbyte( back, M_PUSHI );
    OpndSizeIf( back, false );
    AddSData( back, value, I4 );
    Finalize( back );
}


pointer GenFar16Thunk( BackRun& back, pointer label, uint_16 parms_size, bool remove_parms )
/***************************************************************************/
{
    pointer     code_32;

    { segment_id old_segid = ChangeOP( back, AskCode16Seg(back) );
//        CodeLabel( label, DepthAlign( PROC_ALIGN ) );
        code_32 = AskForNewLabel(back);
        TellOptimizerByPassed(back);
        SetUpObj( back, false );
        OutLabel( back, static_cast<label_handle>(label) );
        OutDataByte( back, 0xb9 );                /* mov cx,# */
        OutDataShort( back, parms_size );
        OutDataByte( back, 0x9a );
        OutRTImport( back, RT_Far32Func, F_FAR16 );
        OutDataShort( back, 0 );
        OutDataShort( back, 0 );
        if( remove_parms ) {
            OutDataByte( back, 0xca );
            OutDataShort( back, parms_size );
        } else {
            OutDataByte( back, 0xcb );
            OutDataShort( back, 0 );              // padding
        }
        /*
         * emit "reloc for offset of code_32 label"
         */
        SetUpObj( back, true );
        TellKeepLabel( back, static_cast<label_handle>(code_32) );
        OutReloc( back, AskCodeSeg(back), F_OFFSET, false );
        OutLblPatch( back, static_cast<label_handle>(code_32), F_OFFSET, 0 );
        TellByPassOver(back);
    SetOP( back, old_segid ); }
    return( code_32 );
}


static void    doProfilingCode( BackRun& back, char *fe_name, label_handle *data, bool prolog )
/******************************************************************************/
{
    if( prolog )
        GenProfileData( back, fe_name, data, NULL );
    Format( back, OC_CODE );
    LayOpbyte( back, 0x68 );
    back.pcencode.ILen += 4;
    DoLblRef( back, *data, static_cast<segment_id>(reinterpret_cast<pointer_uint>(FEAuxInfo( *back.front, NULL, FEINF_P5_PROF_SEG ))), 0, ESCA_OFST );
    Finalize( back );
    DoRTCall( back, prolog ? RT_PROFILE_ON : RT_PROFILE_OFF, true );
}


static  void    doProfilingPrologEpilog( BackRun& back, label_handle label, bool prolog )
/************************************************************************/
{
    if( IsTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) ) {
        doProfilingCode( back, const_cast<char *>(""), &back.data.CurrProc->targ.routine_profile_data, prolog );
    } else {
        back_handle     bck;
        label_handle    data_lbl;
        segment_id      data_segid;

        bck = static_cast<back_handle>(FEAuxInfo( *back.front, AskForLblSym( back, label ), FEINF_P5_PROF_DATA ));
        if( bck == NULL )
            return;
        data_lbl = bck->lbl;
        data_segid = static_cast<segment_id>(reinterpret_cast<pointer_uint>(FEAuxInfo( *back.front, NULL, FEINF_P5_PROF_SEG )));
        TellKeepLabel( back, data_lbl );
        Format( back, OC_CODE );
        if( prolog ) {
            LayOpword( back, 0x05ff );                        /* inc L1+count */
            back.pcencode.ILen += 4;
            DoLblRef( back, data_lbl, data_segid, offsetof( P5_timing_info, count ), ESCA_OFST );
            EjectInst( back );
        }
        LayOpword( back, prolog ? 0x05ff : 0x0dff );          /* inc/dec L1+semaphore */
        back.pcencode.ILen += 4;
        DoLblRef( back, data_lbl, data_segid, offsetof( P5_timing_info, semaphore ), ESCA_OFST );
        EjectInst( back );
        if( IsTargetModel( back, CGSW_X86_P5_PROFILING_CTR0 ) ) {
            LayOpword( back, prolog ? 0x1675 : 0x167d );      /* jne/jge skip */
            EjectInst( back );
            LayOpbyte( back, 0x51 );                          /* push ecx */
            EjectInst( back );
        } else {
            LayOpword( back, prolog ? 0x1275 : 0x127d );      /* jne/jge skip */
            EjectInst( back );
        }
        LayOpbyte( back, 0x52 );                              /* push edx */
        EjectInst( back );
        if( IsTargetModel( back, CGSW_X86_P5_PROFILING_CTR0 ) ) {
            LayOpword( back, 0xc931 );                        /* xor ecx,ecx */
            EjectInst( back );
        }
        LayOpbyte( back, 0x50 );                              /* push eax */
        EjectInst( back );
        if( IsTargetModel( back, CGSW_X86_P5_PROFILING_CTR0 ) ) {
            LayOpword( back, 0x330f );                        /* rdpmc */
            EjectInst( back );
        } else {
            LayOpword( back, 0x310f );                        /* rdtsc */
            EjectInst( back );
        }
        LayOpword( back, prolog ? 0x0529 : 0x0501 );          /* sub/add L1+lo_cycle,eax */
        back.pcencode.ILen += 4;
        DoLblRef( back, data_lbl, data_segid, offsetof( P5_timing_info, lo_cycle ), ESCA_OFST );
        EjectInst( back );
        LayOpbyte( back, 0x58 );                              /* pop eax */
        EjectInst( back );
        LayOpword( back, prolog ? 0x1519 : 0x1511 );          /* sbb/adc L1+hi_cycle,edx */
        back.pcencode.ILen += 4;
        DoLblRef( back, data_lbl, data_segid, offsetof( P5_timing_info, hi_cycle ), ESCA_OFST );
        EjectInst( back );
        LayOpbyte( back, 0x5a );                              /* pop edx */
        EjectInst( back );
        if( IsTargetModel( back, CGSW_X86_P5_PROFILING_CTR0 ) ) {
            LayOpbyte( back, 0x59 );                          /* pop ecx */
            EjectInst( back );
        }
        Finalize( back );
    }
}


segment_id GenProfileData( BackRun& back, char *fe_name, label_handle *data, label_handle *stack )
/**********************************************************************************
 * generate P5 profiler code
 */
{
    segment_id      data_segid = static_cast<segment_id>(reinterpret_cast<pointer_uint>(FEAuxInfo( *back.front, NULL, FEINF_P5_PROF_SEG )));

    { segment_id old_segid = ChangeOP( back, data_segid );
        TellOptimizerByPassed(back);
        SetUpObj( back, true );
        *data = AskForNewLabel(back);
        OutLabel( back, *data );
        OutDataByte( back, *fe_name );                    //flag
        OutDataByte( back, 0 );
        OutDataByte( back, 0 );
        OutDataByte( back, 0 );
        OutDataLong( back, 0 );                           //semaphore
        if( stack == NULL ) {
            OutDataLong( back, 0 );                       //stack
        } else {
            OutReloc( back, data_segid, F_OFFSET, false );  //caller
            OutLblPatch( back, *stack, F_OFFSET, 0 );
        }
        OutDataLong( back, 0 );                           //esp
        OutDataLong( back, 0 );                           //dynamic
        OutDataLong( back, 0 );                           //lo_count
        OutDataLong( back, 0 );                           //hi_count
        OutDataLong( back, 0 );                           //lo_cycle
        OutDataLong( back, 0 );                           //hi_cycle
        OutDataLong( back, 0 );                           //lo_start_time
        OutDataLong( back, 0 );                           //hi_start_time
        OutReloc( back, AskCodeSeg(back), F_OFFSET, false );  //caller
        OutLblPatch( back, static_cast<label_handle>(back.data.CurrProc->label), F_OFFSET, 0 );
        OutDataLong( back, 0 );                           //call_ins
        OutDataLong( back, 0 );                           //callee
        TellByPassOver(back);
    SetOP( back, old_segid ); }
    return( data_segid );
}


void    GenP5ProfilingProlog( BackRun& back, label_handle label )
/************************************************/
{
    doProfilingPrologEpilog( back, label, true );
}


void    GenP5ProfilingEpilog( BackRun& back, label_handle label )
/************************************************/
{
    doProfilingPrologEpilog( back, label, false );
}


void    GFstp10( BackRun& back, type_length where )
/**********************************/
{
    GCondFwait(back);
    CheckSize(back);
    LayOpword( back, 0x3ddb );
    EA( back, HW_EMPTY, HW_EBP, SCALE_NONE, -where, NULL, false );
    Finalize( back );
}


void    GFld10( BackRun& back, type_length where )
/*********************************/
{
    GCondFwait(back);
    CheckSize(back);
    LayOpword( back, 0x2ddb );
    EA( back, HW_EMPTY, HW_EBP, SCALE_NONE, -where, NULL, false );
    Finalize( back );
}


void    Do4Shift( BackRun& back, instruction *ins )
/**********************************/
{
    /* unused parameters */ (void)ins;
}


void    Do4RShift( BackRun& back, instruction *ins )
/************************************
 * NOT NEEDED ON 386
 */
{
    /* unused parameters */ (void)ins;
}


void    Gen4RNeg( BackRun& back, instruction *ins )
/***********************************
 * NOT NEEDED ON 386
 */
{
    /* unused parameters */ (void)ins;
}


void    Pow2Div( BackRun& back, instruction *ins )
/*********************************/
{
    int         log2;
    bool        if_32;

    if_32 = false;
    log2 = GetLog2( ins->operands[1]->c.lo.u.int_value );
    switch( ins->type_class ) {
    case I1:
    case U1:
        LayOpword( back, 0xe4c0 );    /* shl  ah,n */
        AddByte( back, log2 );
        EjectInst( back );
        LayOpword( back, 0xc41a );    /* sbb  al,ah */
        EjectInst( back );
        LayOpword( back, 0xf8c0 );    /* sar  al,n */
        AddByte( back, log2 );
        break;
    case I2:
    case U2:
        if_32 = true;
        /* fall through */
    case I4:
    case U4:
        LayOpword( back, 0xe2c1 );    /* shl  edx,n */
        OpndSizeIf( back, if_32 );
        AddByte( back, log2 );
        EjectInst( back );
        LayOpword( back, 0xc21b );    /* sbb  eax,edx */
        OpndSizeIf( back, if_32 );
        EjectInst( back );
        LayOpword( back, 0xf8c1 );    /* sar  eax,n */
        OpndSizeIf( back, if_32 );
        AddByte( back, log2 );
        break;
    }
}

void    By2Div( BackRun& back, instruction *ins )
/********************************/
{
    bool        if_32;

    if_32 = false;
    switch( ins->type_class ) {
    case I1:
    case U1:
        LayOpword( back, 0xc42a );    /* sub  al,ah */
        EjectInst( back );
        LayOpword( back, 0xf8d0 );    /* sar  al,1 */
        break;
    case I2:
    case U2:
        if_32 = true;
        /* fall through */
    case I4:
    case U4:
        LayOpword( back, 0xc22b );    /* sub  eax,edx */
        OpndSizeIf( back, if_32 );
        EjectInst( back );
        LayOpword( back, 0xf8d1 );    /* sar  eax,1 */
        OpndSizeIf( back, if_32 );
        break;
    }
}


void    Gen4Neg( BackRun& back, instruction *ins )
/**********************************
 * NOT NEEDED ON 386
 */
{
    /* unused parameters */ (void)ins;
}


void    Do4CXShift( BackRun& back, instruction *ins, void (*rtn)(BackRun&, instruction *) )
/*****************************************************************
 * NOT NEEDED ON 386
 */
{
    /* unused parameters */ (void)ins; (void)rtn;
}


void StartBlockProfiling( BackRun& back, block *blk )
/************************************/
{
    segment_id          data_segid;
    label_handle        data;

    if( IsntTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) )
        return;
    if( IsntTargetModel( back, CGSW_X86_STATEMENT_COUNTING ) )
        return;
    data_segid = static_cast<segment_id>(reinterpret_cast<pointer_uint>(FEAuxInfo( *back.front, NULL, FEINF_P5_PROF_SEG )));
    if( blk->label == NULL )
        return;
    TellKeepLabel( back, blk->label );
    { segment_id old_segid = ChangeOP( back, data_segid );
        TellOptimizerByPassed(back);
        SetUpObj( back, true );
        data = AskForNewLabel(back);
        TellKeepLabel( back, data );
        OutLabel( back, data );
        OutDataByte( back, PROFILE_FLAG_BLOCK );          //flag
        OutDataByte( back, 0 );                           //...
        OutDataByte( back, 0 );                           //...
        OutDataByte( back, 0 );                           //...
        OutDataLong( back, 0 );                           //lo_count
        OutDataLong( back, 0 );                           //hi_count
        OutReloc( back, AskCodeSeg(back), F_OFFSET, false );  //block
        OutLblPatch( back, blk->label, F_OFFSET, 0 );
        OutReloc( back, AskCodeSeg(back), F_OFFSET, false );  //function
        OutLblPatch( back, static_cast<label_handle>(back.data.CurrProc->label), F_OFFSET, 0 );
        TellByPassOver(back);
    SetOP( back, old_segid ); }
    Format( back, OC_CODE );
    LayOpword( back, 0x0583 );                /* add L1+lo_count,1 */
    back.pcencode.ILen += 4;
    DoLblRef( back, data, data_segid, offsetof( block_count_info, lo_count ), ESCA_OFST );
    AddByte( back, 1 );
    EjectInst( back );
    LayOpword( back, 0x1583 );                /* adc L1+hi_count,0 */
    back.pcencode.ILen += 4;
    DoLblRef( back, data, data_segid, offsetof( block_count_info, hi_count ), ESCA_OFST );
    AddByte( back, 0 );
    Finalize( back );
}


void EndBlockProfiling( void )
/****************************/
{
}
