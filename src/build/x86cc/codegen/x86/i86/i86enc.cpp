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
* Description:  16-bit instruction encodings.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "pccode.h"
#include "system.h"
#include "pcencode.h"
#include "zoiks.h"
#include "zeropage.h"
#include "data.h"
#include "x86objd.h"
#include "objprof.h"
#include "targetdb.h"
#include "x86obj.h"
#include "x86esc.h"
#include "rgtbl.h"
#include "namelist.h"
#include "tree.h"
#include "treefold.h"
#include "x86segs.h"
#include "x86enc.h"
#include "x86opseg.h"
#include "backrun.hpp"


#define RMR_MOD_DIR     6
#define RMR_MOD_IND     0x80


static const hw_reg_set IndexTab[] = {
    HW_D_2( HW_BX, HW_SI ),
    HW_D_2( HW_BX, HW_DI ),
    HW_D_2( HW_BP, HW_SI ),
    HW_D_2( HW_BP, HW_DI ),
    HW_D_1( HW_SI ),
    HW_D_1( HW_DI ),
    HW_D_1( HW_BP ),
    HW_D_1( HW_BX )
};

static void OpndSizeIf( BackRun& back )
/****************************/
{
    if( IsTargetModel( back, CGSW_X86_USE_32 ) ) {
        AddToTemp( back, M_OPND_SIZE );
    }
}

byte    DoIndex( BackRun& back, hw_reg_set regs )
/********************************/
{
    int i;

    for( i = 0; i < sizeof( IndexTab ) / sizeof( IndexTab[0] ); i++ ) {
        if( HW_Equal( regs, IndexTab[i] ) ) {
            return( i << S_RMR_RM );
        }
    }
    Zoiks( back, ZOIKS_033 );
    return( 0 );
}


byte    Displacement( BackRun& back, int_32 val, hw_reg_set regs )
/*************************************************/
{
    HW_CTurnOff( regs, HW_SEGS );
    if( val == 0 && !HW_CEqual( regs, HW_BP ) )
        return( DISP0 );
    if( val <= 127 && val >= -128 ) {
        AddByte( back, val & 0xff );
        return( DISP8 );
    } else {
        val &= 0xffff;
        AddByte( back, val );
        AddByte( back, val >> 8 );
        return( DISPW );
    }
}


static  byte    DoDisp( BackRun& back, name *op, hw_reg_set regs )
/*************************************************/
{
    name        *base;
    int         val;
    byte        dmod;
    name        *temp_base;

    val = op->i.constant;              /* use integer value*/
    base = op->i.base;
    if( !HasTrueBase( op ) ) {
        dmod = Displacement( back, val, regs );
    } else if( base->n.class_ == N_TEMP ) {
        temp_base = DeAlias( base );
        if( temp_base->t.location == NO_LOCATION ) {
            Zoiks( back, ZOIKS_034 );
        }
        val += NewBase( back, temp_base ) + base->v.offset - temp_base->v.offset;
        dmod = Displacement( back, val, regs );
    } else {
        dmod = DISPW;
        back.pcencode.ILen += 2;
        DoSymRef( back, op->i.base, val, false );
    }
    return( dmod );
}


void    DoRepOp( BackRun& back, instruction *ins )
/*********************************/
{
    int size;

    size = ins->result->n.size;
    if( ins->head.opcode == OP_MOV && !UseRepForm( back, size ) ) {
        LayOpbyte( back, M_MOVSW );
        size -= 2;
        while( size > 1 ) {
            AddByte( back, M_MOVSW );
            size -= 2;
        }
        if( size == 1 ) {
            AddByte( back, M_MOVSB );
        }
    } else {
        LayOpbyte( back, M_REPE );
        if( ins->head.opcode == OP_MOV ) {
            if( ( size & 1 ) == 0 || back.data.OptForSize <= 50 ) {
                AddByte( back, M_MOVSW );
                if( size & 1 ) {
                    AddByte( back, M_MOVSB );
                }
            } else {
                AddByte( back, M_MOVSB );
            }
        } else {
            if( ins->operands[1]->n.size & 1 ) {
                AddByte( back, M_CMPSB );
            } else {
                AddByte( back, M_CMPSW );
            }
        }
    }
}


void    Do4CXShift( BackRun& back, instruction *ins, void (*rtn)(BackRun&, instruction *) )
/****************************************************************/
{
    hw_reg_set  hreg;
    hw_reg_set  lreg;
    unsigned    shift;

    if( ins->operands[1]->n.class_ == N_CONSTANT ) {
        shift = ins->operands[1]->c.lo.u.int_value;
        if( shift < 16 && back.data.OptForSize < 50 && ins->result->n.class_ == N_REGISTER ) {
            hreg = ins->result->r.reg;
            lreg = Low32Reg( hreg );
            hreg = High32Reg( hreg );
            if( ins->head.opcode == OP_LSHIFT ) {
                LayOpbyte( back, 0xb0 );   /* Move register immediate (byte)*/
                LayRegAC( back, HW_CL );
                AddByte( back, shift );
                EjectInst( back );
                LayOpword( back, M_SHL1 | B_KEY_W | 0x02 ); /* SHL reg,CL (word) */
                LayRegRM( back, hreg );
                EjectInst( back );
                LayOpword( back, 0x00d3 );    /* ROL reg,CL (word) */
                LayRegRM( back, lreg );
                EjectInst( back );
                LayOpword( back, M_XORRR | B_KEY_W );
                LayReg( back, lreg );
                LayRegRM( back, hreg );
                EjectInst( back );
                LayOpword( back, 0x2081 ); /* AND reg,immed (word) */
                LayRegRM( back, lreg );
                AddWData( back, ((uint_16)~0) << shift, U2 );
                EjectInst( back );
                LayOpword( back, M_XORRR | B_KEY_W );
                LayReg( back, lreg );
                LayRegRM( back, hreg );
                return;
            } else if( ins->type_class == U4 ) {
                LayOpbyte( back, 0xb0 );   /* Move register immediate (byte)*/
                LayRegAC( back, HW_CL );
                AddByte( back, shift );
                EjectInst( back );
                LayOpword( back, M_SHR1 | B_KEY_W | 0x02 ); /* SHR reg,CL (word) */
                LayRegRM( back, lreg );
                EjectInst( back );
                LayOpword( back, 0x08d3 );    /* ROR reg,CL (word) */
                LayRegRM( back, hreg );
                EjectInst( back );
                LayOpword( back, M_XORRR | B_KEY_W );
                LayReg( back, hreg );
                LayRegRM( back, lreg );
                EjectInst( back );
                LayOpword( back, 0x2081 ); /* AND reg,immed (word) */
                LayRegRM( back, hreg );
                AddWData( back, ((uint_16)~0) >> shift, U2 );
                EjectInst( back );
                LayOpword( back, M_XORRR | B_KEY_W );
                LayReg( back, hreg );
                LayRegRM( back, lreg );
                return;
            }
        }
        LayOpbyte( back, 0xb8 );   /* Move register immediate (word)*/
        LayRegAC( back, HW_CX );
        AddWCons( back, ins->operands[1], U2 );
        EjectInst( back );
        rtn( back, ins );
        EjectInst( back );
        LayOpword( back, M_LOOP );
        back.pcencode.Inst[DATALO] = - ( -3 + 2 + back.pcencode.Temp.hdr.objlen );
    } else {
        LayOpbyte( back, 0xe3 );    /* JCXZ*/
        AddByte( back, 0 );        /* to be patched*/
        EjectInst( back );
        rtn( back, ins );
        EjectInst( back );
        LayOpword( back, M_LOOP );
        back.pcencode.Inst[DATALO] = - ( -2 + 2 + back.pcencode.Temp.hdr.objlen );
        back.pcencode.Temp.data[1] = -back.pcencode.Inst[DATALO];
    }
}


void    LayLeaRegOp( BackRun& back, instruction *ins )
/*************************************/
{
    name        *left;
    name        *right;

    left = ins->operands[0];
    right = ins->operands[1];
    back.pcencode.Inst[RMR] |= DoIndex( back, left->r.reg );
    if( ins->head.opcode == OP_ADD ) {
        if( right->c.const_type == CONS_ABSOLUTE ) {
            back.pcencode.Inst[RMR] |= Displacement( back, right->c.lo.u.int_value, left->r.reg );
        } else {
            back.pcencode.Inst[RMR] |= DISPW;
            DoRelocConst( back, right, U2 );
        }
    } else {
        back.pcencode.Inst[RMR] |= Displacement( back, -right->c.lo.u.int_value, left->r.reg );
    }
}


static  void    CheckSize( BackRun& back )
/*******************************/
{
    if( IsTargetModel( back, CGSW_X86_USE_32 ) ) {
        AddToTemp( back, M_ADDR_SIZE );
    }
}


void    DoMAddr( BackRun& back, name *op )
/*************************/
{
    back.pcencode.ILen += 2;
    if( op->n.class_ == N_CONSTANT ) {
        Zoiks( back, ZOIKS_035 );
    } else {        /* assume global name*/
        DoSymRef( back, op, op->v.offset, false );
    }
}


byte    DoMDisp( BackRun& back, name *op, bool alt_encoding )
/********************************************/
{
    hw_reg_set          regs;
    zero_page_scheme    zptype;


    regs = CalcSegment( back, static_cast<cg_sym_handle>(op->v.symbol), op->m.memory_type );
    if( ( !alt_encoding && back.x86opseg.ZPageType != ZP_USES_DS )
      ||  !HW_COvlap( regs, HW_SS ) ) {
        zptype = ZP_USES_SS;
    } else {
        zptype = back.x86opseg.ZPageType;
    }
    switch( zptype ) {
    case ZP_USES_SS:
        if( HW_COvlap( regs, HW_SEGS ) ) {
            GenSeg( back, regs );
        }
        DoMAddr( back, op );
        return( RMR_MOD_DIR );
    case ZP_USES_DS:
        DoMAddr( back, op );
        return( RMR_MOD_DIR );
    case ZP_USES_BP:
        DoMAddr( back, op );
        return( DoIndex( back, HW_BP ) + RMR_MOD_IND );
    case ZP_USES_DI:
        DoMAddr( back, op );
        return( DoIndex( back, HW_BP_DI ) + RMR_MOD_IND );
    case ZP_USES_SI:
        DoMAddr( back, op );
        return( DoIndex( back, HW_BP_SI ) + RMR_MOD_IND );
    }
    return( 0 );
}


void    LayModRM( BackRun& back, name *op )
/**************************/
{
    hw_reg_set  regs;
    name        *base;
    hw_reg_set  tmp;

    switch( op->n.class_ ) {
    case N_MEMORY:
        CheckSize(back);
        back.pcencode.Inst[RMR] |= DoMDisp( back, op, true );
        break;
    case N_TEMP:
        CheckSize(back);
        base = DeAlias( op );
        if( base->t.location == NO_LOCATION ) {
            Zoiks( back, ZOIKS_030 );
        }
        back.pcencode.Inst[RMR] |= Displacement( back, NewBase( back, base ) + op->v.offset - base->v.offset, HW_BP );
        back.pcencode.Inst[RMR] |= DoIndex( back, HW_BP );
        break;
    case N_INDEXED:
        CheckSize(back);
        regs = op->i.index->r.reg;
        if( HW_COvlap( regs, HW_SEGS ) ) {
            /* check for seg override*/
            GenSeg( back, regs );
            HW_CTurnOff( regs, HW_SEGS );
        }
        if( HasTrueBase( op ) ) {
            if( op->i.base->n.class_ == N_TEMP ) {
                HW_CTurnOn( regs, HW_BP );
            } else if( op->i.base->n.class_ == N_MEMORY ) {
                tmp = CalcSegment( back, static_cast<cg_sym_handle>(op->i.base->v.symbol),
                                  op->i.base->m.memory_type );
                HW_TurnOn( regs, tmp );
                if( HW_COvlap( regs, HW_SS ) ) {
                    switch( back.x86opseg.ZPageType ) {
                    case ZP_USES_SS:
                    case ZP_USES_DI:
                    case ZP_USES_SI:
                        GenSeg( back, regs );
                        break;
                    case ZP_USES_DS:
                        /* nothing*/
                        break;
                    case ZP_USES_BP:
                        if( HW_COvlap( regs, HW_BX ) ) {
                            GenSeg( back, regs );
                        } else {
                            HW_CTurnOn( regs, HW_BP );
                        }
                        break;
                    }
                } else {
                    GenSeg( back, regs );
                }
                HW_CTurnOff( regs, HW_SEGS );
            }
        }
        back.pcencode.Inst[RMR] |= DoDisp( back, op, regs );
        back.pcencode.Inst[RMR] |= DoIndex( back, regs );
        break;
    case N_REGISTER:
        LayRMRegOp( back, op );
        break;
    default:
        break;
    }
}


void    DoRelocConst( BackRun& back, name *op, type_class_def type_class )
/*********************************************************/
{
    /* unused parameters */ (void)type_class;

    back.pcencode.ILen += 2;
    if( op->c.const_type == CONS_OFFSET ) {
        DoSymRef( back, op->c.u.op, op->c.u.op->v.offset, false );
    } else if( op->c.const_type == CONS_SEGMENT ) {
        if( op->c.u.op == NULL ) {
            DoSegRef( back, op->c.lo.u.int_value );
        } else {
            DoSymRef( back, op->c.u.op, 0, true );
        }
    } else if( op->c.const_type == CONS_ADDRESS ) {
        Zoiks( back, ZOIKS_045 );
    }
}

static  void    SetOff( name *op, int val )
/*****************************************/
{
    if( op->n.class_ == N_INDEXED ) {
        op->i.constant += val;
    } else if( op->n.class_ == N_TEMP ) {
        op = DeAlias( op );
        op->t.location += val;
    } else { /* N_MEMORY*/
        op->v.offset += val;
    }
}


void    Do4Shift( BackRun& back, instruction *ins )
/**********************************/
{
    name        *op;

    op = ins->result;
    if( ins->head.opcode == OP_LSHIFT ) {
        LayOpword( back, M_SHL1 | B_KEY_W );
        LayModRM( back, op );
        EjectInst( back );
        SetOff( op,  2 );
        LayOpword( back, M_RCL1 | B_KEY_W );
        LayModRM( back, op );
        SetOff( op, -2 );
    } else {
        SetOff( op,  2 );
        LayOpword( back, M_SHR1 | B_KEY_W );
        if( ins->type_class == I4 ) {
            back.pcencode.Inst[RMR] |= B_RMR_SHR_SAR;
        }
        LayModRM( back, op );
        EjectInst( back );
        SetOff( op, -2 );
        LayOpword( back, M_RCR1 | B_KEY_W );
        LayModRM( back, op );
    }
}


void    Do4RShift( BackRun& back, instruction *ins )
/***********************************/
{
    hw_reg_set  regs;

    regs = ins->result->r.reg;
    if( ins->head.opcode == OP_LSHIFT ) {
        LayOpword( back, M_SHL1 | B_KEY_W );
        LayRegRM( back, Low32Reg( regs ) );
        EjectInst( back );
        LayOpword( back, M_RCL1 | B_KEY_W );
        LayRegRM( back, High32Reg( regs ) );
    } else {
        LayOpword( back, M_SHR1 | B_KEY_W );
        if( ins->type_class == I4 ) {
            back.pcencode.Inst[RMR] |= B_RMR_SHR_SAR;
        }
        LayRegRM( back, High32Reg( regs ) );
        EjectInst( back );
        LayOpword( back, M_RCR1 | B_KEY_W );
        LayRegRM( back, Low32Reg( regs ) );
    }
}


void    Gen4RNeg( BackRun& back, instruction *ins )
/**********************************/
{
    hw_reg_set  regs;

    regs = ins->result->r.reg;
    LayOpword( back, M_NEG );
    LayW( back, I2 );
    LayRegRM( back, High32Reg( regs ) );
    EjectInst( back );
    LayOpword( back, M_NEG );
    LayW( back, I2 );
    LayRegRM( back, Low32Reg( regs ) );
    EjectInst( back );
    LayOpword( back, M_SBB );
    LayW( back, I2 );
    LayRegRM( back, High32Reg( regs ) );
    AddSData( back, 0, I2 );
}


void    Gen4Neg( BackRun& back, instruction *ins )
/*********************************/
{
    name        *res;

    res = ins->result;
    SetOff( res,  2 );
    LayOpword( back, M_NEG );
    LayW( back, I2 );
    LayModRM( back, res );
    EjectInst( back );
    SetOff( res, -2 );
    LayOpword( back, M_NEG );
    LayW( back, I2 );
    LayModRM( back, res );
    EjectInst( back );
    SetOff( res,  2 );
    LayOpword( back, M_SBB );
    LayW( back, I2 );
    LayModRM( back, res );
    AddSData( back, 0, I2 );
    SetOff( res, -2 );
}


void    By2Div( BackRun& back, instruction *ins )
/********************************/
{
    switch( ins->type_class ) {
    case I1:
    case U1:
        LayOpword( back, 0xc42a );    /* sub  al,ah */
        EjectInst( back );
        LayOpword( back, 0xf8d0 );    /* sar  al,1 */
        break;
    case I2:
    case U2:
        LayOpword( back, 0xc22b );    /* sub  ax,dx */
        OpndSizeIf(back);
        EjectInst( back );
        LayOpword( back, 0xf8d1 );    /* sar  ax,1 */
        OpndSizeIf(back);
    default:
        break;
    }
}


void Pow2Div286( BackRun& back, instruction *ins )
/*********************************/
{
    int         log2;

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
        LayOpword( back, 0xe2c1 );    /* shl  dx,n */
        OpndSizeIf(back);
        AddByte( back, log2 );
        EjectInst( back );
        LayOpword( back, 0xc21b );    /* sbb  ax,dx */
        OpndSizeIf(back);
        EjectInst( back );
        LayOpword( back, 0xf8c1 );    /* sar  ax,n */
        OpndSizeIf(back);
        AddByte( back, log2 );
        break;
    default:
        break;
    }
}


void    Pow2Div( BackRun& back, instruction *ins )
/*********************************/
{
    int         log2;

    log2 = GetLog2( ins->operands[1]->c.lo.u.int_value );
    switch( ins->type_class ) {
    case I1:
    case U1:
        LayOpbyte( back, 0xb1 );              /* mov  cl,n */
        AddByte( back, log2 );
        EjectInst( back );
        LayOpbyte( back, 0xe4d2 );    /* shl  ah,cl */
        EjectInst( back );
        LayOpword( back, 0xc41a );    /* sbb  al,ah */
        EjectInst( back );
        break;
    case I2:
    case U2:
        LayOpbyte( back, 0xb1 );              /* mov  cl,n */
        AddByte( back, log2 );
        EjectInst( back );
        LayOpword( back, 0xe2d3 );    /* shl  dx,cl */
        OpndSizeIf(back);
        EjectInst( back );
        LayOpword( back, 0xc21b );    /* sbb  ax,dx */
        OpndSizeIf(back);
        EjectInst( back );
        LayOpword( back, 0xf8d3 );    /* sar  ax,cl */
        OpndSizeIf(back);
        break;
    default:
        break;
    }
}

void    GFstp10( BackRun& back, type_length where )
/**********************************/
{
    GCondFwait(back);
    LayOpword( back, 0x3edb );
    back.pcencode.Inst[RMR] |= Displacement( back, -where, HW_BP );
    Finalize( back );
}


void    GFld10( BackRun& back, type_length where )
/*********************************/
{
    GCondFwait(back);
    LayOpword( back, 0x2edb );
    back.pcencode.Inst[RMR] |= Displacement( back, -where, HW_BP );
    Finalize( back );
}

void StartBlockProfiling( BackRun& back, block *blk )
/************************************/
{
    /* unused parameters */ (void)blk;
}

void EndBlockProfiling( void )
/****************************/
{
}

segment_id GenProfileData( BackRun& back, char *fe_name, label_handle *data, label_handle *stack )
/***********************************************************************************/
{
    /* unused parameters */ (void)fe_name; (void)data; (void)stack;

    return( 0 );
}
