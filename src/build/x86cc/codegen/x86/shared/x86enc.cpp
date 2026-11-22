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
* Description:  Take the instruction stream from the code generator and
*               writes the instructions into the object file.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "optmain.h"
#include "pccode.h"
#include "system.h"
#include "pcencode.h"
#include "zoiks.h"
#include "zeropage.h"
#include "cgaux.h"
#include "p5prof.h"
#include "data.h"
#include "display.h"
#include "rtrtn.h"
#include "utils.h"
#include "x86objd.h"
#include "objout.h"
#include "dbsyms.h"
#include "objprof.h"
#include "x86enc2.h"
#include "encode.h"
#include "object.h"
#include "x86proc.h"
#include "targetin.h"
#include "x87.h"
#include "x86esc.h"
#include "rgtbl.h"
#include "split.h"
#include "namelist.h"
#include "fixindex.h"
#include "x86segs.h"
#include "x86enc.h"
#include "x86split.h"
#include "x86opcod.h"
#include "feprotos.h"
#include "backrun.hpp"


/*
 * register, register class and range definitions
 */
#define pick_item(id)    ARCH_IDX_ ## id
#define pick_start(id)   ARCH_ ## id ## _START
#define pick_end(id)     ARCH_ ## id ## _END

enum {
    #define pick(id,idx,cls) pick_item(id),
    #include "regindex.h"
    #undef pick
    pick_item( END ),
    pick_item( START ) = 0
};

enum {
    #define MAPREGCLASS
    #include "regindex.h"
    #undef MAPREGCLASS
};

#undef pick_end
#undef pick_start
#undef pick_item

const arch_reg_info RegsTab[] = {
    #define pick(id,idx,cls) { HW_D_1( HW_ ## id ), idx, cls ## _IDX, WV_REG_ ## id, DW_REG_ ## id },
    #include "regindex.h"
    #undef pick
};


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct X86encState: x86enc_state.h) */

}  // namespace


                                    /* checked for escapes when copied into Temp */
#if 0
static int findArchRegIndex( hw_reg_set regs, reg_cls cls )
{
    hw_reg_set  save_regs;
    int         i;

    if( cls & SEG_IDX ) {
        save_regs = regs;
    }
    HW_CTurnOff( regs, HW_SEGS );
    if( cls & GPR_IDX ) {
        for( i = ARCH_GPR_START; i < ARCH_GPR_END; i++ ) {
            if( HW_Equal( regs, RegsTab[i].hw_reg ) ) {
                return( i );
            }
        }
    }
    if( cls & FPR_IDX ) {
        for( i = ARCH_FPR_START; i < ARCH_FPR_END; i++ ) {
            if( HW_Equal( regs, RegsTab[i].hw_reg ) ) {
                return( i );
            }
        }
    }
    if( cls & SEG_IDX ) {
        HW_COnlyOn( save_regs, HW_SEGS );
        for( i = ARCH_SEG_START; i < ARCH_SEG_END; i++ ) {
            if( HW_Equal( save_regs, RegsTab[i].hw_reg ) ) {
                return( i );
            }
        }
    }
    return( -1 );
}

hw_reg_set GetArchReg( int idx, reg_cls cls )
{
    if( cls & GPRB_IDX ) {
        return( RegsTab[ARCH_BYTE_START + idx].hw_reg );
    }
    if( cls & GPRW_IDX ) {
        return( RegsTab[ARCH_WORD_START + idx].hw_reg );
    }
    if( cls & GPRD_IDX ) {
        return( RegsTab[ARCH_DWORD_START + idx].hw_reg );
    }
    if( cls & FPR_IDX ) {
        return( RegsTab[ARCH_FPR_START + idx].hw_reg );
    }
    if( cls & SEG_IDX ) {
        return( RegsTab[ARCH_SEG_START + idx].hw_reg );
    }
    if( cls & GPR_IDX ) {
        return( RegsTab[ARCH_GPR_START + idx].hw_reg );
    }
    return( HW_EMPTY );
}
#endif

/* routines that maintain instruction buffers*/

void    Format( BackRun& back, oc_class class_ )
/*******************************
 * Get Temp and Inst ready to accept instructions.  Each instruction is
 * formatted into Inst, transferred into Temp (as an opcode_entry) and
 * then dumped into the peephole optimizer.
 */
{
    SetFPPatchType( back, FPP_NONE );
    back.pcencode.Temp.hdr.class_ = class_;
    back.pcencode.Temp.hdr.objlen = 0;
    back.pcencode.Temp.hdr.reclen = offsetof( template_, data );
    back.x86enc.ICur = 0;
    back.pcencode.ILen = 0;
    back.x86enc.IEsc = 0;
}

void    ReFormat( BackRun& back, oc_class class_ )
/*********************************
 * Change the class of Temp
 */
{
    back.pcencode.Temp.hdr.class_ = class_;
}

void    AddByte( BackRun& back, byte b )
/************************
 * Add a byte to Inst[]
 */
{
    if( b == ESC ) {
        back.pcencode.Inst[back.x86enc.ICur++] = b;
    }
    back.pcencode.Inst[back.x86enc.ICur++] = b;
    back.pcencode.ILen++;
}

void    AddToTemp( BackRun& back, byte b )
/********************************
 * Add a byte to the end of Temp
 */
{
    if( b == ESC ) {
        back.pcencode.Temp.data[back.pcencode.Temp.hdr.reclen++ - offsetof( template_, data )] = b;
    }
    back.pcencode.Temp.data[back.pcencode.Temp.hdr.reclen++ - offsetof( template_, data )] = b;
    back.pcencode.Temp.hdr.objlen++;
}

void    EmitByte( BackRun& back, byte b )
/**************************
 * Plop a byte into Inst[]
 */
{
    back.pcencode.Inst[back.x86enc.ICur++] = b;
}

void    EmitPtr( BackRun& back, pointer p )
/*****************************
 * Plop a pointer into Inst[]
 */
{
    memcpy( back.pcencode.Inst + back.x86enc.ICur, &p, sizeof( pointer ) );
    back.x86enc.ICur += sizeof( pointer );
}

void    EmitSegId( BackRun& back, segment_id segid )
/************************************
 * Plop a segment_id into Inst[]
 */
{
    memcpy( back.pcencode.Inst + back.x86enc.ICur, &segid, sizeof( segment_id ) );
    back.x86enc.ICur += sizeof( segment_id );
}

void    EmitOffset( BackRun& back, offset i )
/*****************************
 * Plop an "offset" int Inst[] (a machine word)
 */
{
    memcpy( back.pcencode.Inst + back.x86enc.ICur, &i, sizeof( offset ) );
    back.x86enc.ICur += sizeof( offset );
}

static void     TransferIns( BackRun& back )
/**********************************
 * Transfer an instruction from Inst[] to Temp
 */
{
    unsigned i;
    unsigned j;

    j = back.pcencode.Temp.hdr.reclen - offsetof( template_, data );
    for( i = 0; i < back.x86enc.IEsc; ++i ) {
        if( back.pcencode.Inst[i] == ESC ) {
            back.pcencode.Temp.data[j++] = ESC;
            back.pcencode.Temp.hdr.reclen++;
        }
        back.pcencode.Temp.data[j++] = back.pcencode.Inst[i];
    }
    Copy( &back.pcencode.Inst[i], &back.pcencode.Temp.data[j], back.x86enc.ICur - i );
    back.pcencode.Temp.hdr.reclen += back.x86enc.ICur;
    back.pcencode.Temp.hdr.objlen += back.pcencode.ILen;
}

void    EjectInst( BackRun& back )
/************************
 * Dump the current instruction into the peephole optimizer
 */
{
    TransferIns(back);
    back.x86enc.ICur = 0;
    back.pcencode.ILen = 0;
    back.x86enc.IEsc = 0;
}

void    Finalize( BackRun& back )
/***********************
 * Invoked by macro _Emit. Spits Temp into the peephole optimizer
 */
{
    EjectInst(back);
    FPPatchTypeRef(back);
    if( back.pcencode.Temp.hdr.objlen != 0 ) {
        InputOC( back, (any_oc *)&back.pcencode.Temp );
    }
}


static void     LayInitial( BackRun& back, instruction *ins, gentype gen )
/**********************************************************
 * Do some really magical jiggery pokery based on "gen" to get the
 * right opcode int Inst[].  See PCCodeTable if you want, but this is
 * not for the faint of heart!
 */
{
    int                 index;
    const pccode_def    *table;

    table = PCCodeTable;
    for( ;; ) {
        if( gen < table->low_gen )
            break;
        if( table->width == 0 )
            return;
        ++ table;
    }
    -- table;
    index = 0;
    for( ;; ) {
        if( table->opcode_list[index] == ins->head.opcode )
            break;
        if( table->opcode_list[index] == OP_NOP )
            break;
        ++ index;
    }
    index = index * table->width + gen - table->low_gen;
    if( table->flags & NEED_WAIT ) {
        back.i87data.Used87 = true;
#if _TARGET & _TARG_8086
        if( gen == G_FINIT || !_CPULevel( CPU_286 ) || _IsEmulation() ) {
            if( _IsEmulation() ) {
                SetFPPatchType( back, FPP_NORMAL );
            }
            LayOpbyte( back, 0x9b );
            _Next;
        }
#endif
    }
    if( table->flags & BYTE_OPCODE ) {
        LayOpbyte( back, table->gen_opcode_table[index] );
    } else {
        LayOpword( back, table->gen_opcode_table[index] );
    }
    if( table->flags & BYTE_WORD ) {
        LayW( back, ins->type_class );
    }
    if( table->flags & SIGN_UNSIGN ) {
        if( ins->type_class == I1
          || ins->type_class == I2
          || ins->type_class == I4 ) {
            if( ins->head.opcode >= OP_MUL && ins->head.opcode <= OP_MOD ) {
                back.pcencode.Inst[RMR] |= B_RMR_MUL_SGN;
            } else if( ins->head.opcode == OP_RSHIFT ) {
                back.pcencode.Inst[RMR] |= B_RMR_SHR_SAR;
            }
        }
    }
}

static reg_idx  SegTrans( BackRun& back, hw_reg_set regs )
/******************************************
 * Return the encoding of a segment register name
 */
{
    int         i;

    HW_COnlyOn( regs, HW_SEGS );
    for( i = ARCH_SEG_START; i < ARCH_SEG_END; i++ ) {
        if( HW_Equal( regs, RegsTab[i].hw_reg ) ) {
            return( RegsTab[i].idx );
        }
    }
    _Zoiks( ZOIKS_032 );
    return( 0 );
}

reg_idx  RegTrans( BackRun& back, hw_reg_set regs )
/***********************************
 * Translate reg to register class (8/16/32-bit) relative index
 */
{
    int         i;

    HW_CTurnOff( regs, HW_SEGS );
    for( i = ARCH_GPR_START; i < ARCH_GPR_END; i++ ) {
        if( HW_Equal( regs, RegsTab[i].hw_reg ) ) {
            return( RegsTab[i].idx );
        }
    }
    _Zoiks( ZOIKS_031 );
    return( 0 );
}

static int FPRegTrans( hw_reg_set reg )
/*************************************/
{
    int         i;

    for( i = ARCH_FPR_START; i < ARCH_FPR_END; i++ ) {
        if( HW_Equal( reg, RegsTab[i].hw_reg ) ) {
            return( RegsTab[i].idx );
        }
    }
    return( 0 );
}

dw_regs RegTransDW( BackRun& back, hw_reg_set reg )
/***********************************
 * Translate reg to Dwarf enum name
 */
{
    int         i;

    for( i = ARCH_IDX_START; i < ARCH_IDX_END; i++ ) {
        if( HW_Equal( reg, RegsTab[i].hw_reg ) ) {
            return( RegsTab[i].dw_idx );
        }
    }
    _Zoiks( ZOIKS_031 );
    return( DW_REG_END );
}


int RegTransWV( BackRun& back, hw_reg_set reg )
/*******************************
 * Translate reg to WATCOM enum name
 */
{
    int         i;

    for( i = ARCH_IDX_START; i < ARCH_IDX_END; i++ ) {
        if( HW_Equal( reg, RegsTab[i].hw_reg ) ) {
            return( RegsTab[i].wv_idx );
        }
    }
    _Zoiks( ZOIKS_031 );
//    return( WV_REG_END );
    return( -1 );
}


int     FPRegNum( name *reg_name )
/*********************************
 * given a name, return the 8087 register number (0-7) or -1 if
 * it isn't an 8087 register
 */
{
    if( reg_name != NULL
      && reg_name->n.class_ == N_REGISTER
      && HW_COvlap( reg_name->r.reg, HW_FLTS ) ) {
        return( FPRegTrans( reg_name->r.reg ) );
    }
    return( -1 );
}

hw_reg_set   GetFPReg( int idx )
/******************************/
{
    return( RegsTab[ARCH_FPR_START + idx].hw_reg );
}


int     CountFPRegs( hw_reg_set regs )
/*************************************
 * Count the number of 8087 registers named in hw_reg_set "regs".
 */
{
    int         count;
    int         i;

    count = 0;
    for( i = ARCH_FPR_START; i < ARCH_FPR_END; i++ ) {
        if( HW_Ovlap( regs, RegsTab[i].hw_reg ) ) {
            ++count;
        }
    }
    return( count );
}

#if _TARGET & _TARG_80386
static bool     NeedOpndSize( instruction *ins )
/***********************************************
 * Do we REALLY, REALLY need the operand size override.
 */
{
    opcnt       i;
    hw_reg_set  result_reg;
    hw_reg_set  full_reg;
    instruction *next;

    switch( ins->head.opcode ) {
    case OP_MOD:
    case OP_DIV:
    case OP_RSHIFT:
        return( true );
    default:
        break;
    }
    if( _OpIsCondition( ins->head.opcode ) )
        return( true );
    if( ins->ins_flags & INS_CC_USED )
        return( true );
    if( ins->result == NULL )
        return( true );
    switch( ins->result->n.class_ ) {
    case N_TEMP:
        /*
         * it's OK to store a DWORD into a WORD temp because there's always
         * two bytes of slack on the stack
         */
        if( ins->result->t.alias != ins->result )
            return( true );
        break;
    case N_REGISTER:
        /*
         * check that it's OK to trash high word of register
         */
        next = ins->head.next;
        if( next == NULL )
            return( true );
        result_reg = ins->result->r.reg;
        full_reg = FullReg( result_reg );
        HW_TurnOff( full_reg, result_reg );
        if( HW_Ovlap( full_reg, next->head.live.regs ) )
            return( true );
        break;
    default:
        return( true );
    }
    for( i = ins->num_operands; i-- > 0; ) {
        /*
         * Could be smarter here and allow N_MEMORY,N_INDEXED if
         * they're in DGROUP
         */
        switch( ins->operands[i]->n.class_ ) {
        case N_TEMP:
        case N_REGISTER:
        case N_CONSTANT:
            break;
        default:
            return( true );
        }
    }
    ins->type_class = U4; /* get correct size of constants */
    return( false );
}
#endif


static  bool    LayOpndSize( BackRun& back, instruction *ins, gentype gen )
/***********************************************************
 * Generate an operand size prefix if the instruction needs it
 * (Eg: MOV    AX,DX need it in a USE32 segment)
 */
{
#if _TARGET & _TARG_8086
    switch( ins->head.opcode ) {
    case OP_PUSH:
        if( ( gen == G_C1 || gen == G_M1 ) && ins->type_class == I4 ) {
            AddToTemp( back, M_OPND_SIZE );
            return( true );
        }
        break;
    case OP_MOV:
        if( ins->type_class == U4 || ins->type_class == I4 ) {
            if( ( gen == G_MOVMC ) ) {
                AddToTemp( back, M_OPND_SIZE );
                return( true );
            }
        }
        break;
    default:
        break;
    }
    return( false );
#else
    switch( ins->head.opcode ) {
    case OP_CONVERT: /* these ones handle themselves */
    case OP_CALL:
    case OP_CALL_INDIRECT:
    case OP_SELECT:
        return( false );
        break;
    default:
        switch( gen ) {
        case G_SR:
        case G_RS:
        case G_MS1:
        case G_SM1:
        case G_SEG_SEG:
        case G_SEGR1:
        case G_POW2DIV:
        case G_DIV2:
            return( false );
        default:
            break;
        }
        if( _IsTargetModel( CGSW_X86_USE_32 ) ) {
            if( ins->type_class == U2 || ins->type_class == I2 ) {
                if( NeedOpndSize( ins ) ) {
                    AddToTemp( back, M_OPND_SIZE );
                    return( true );
                }
            }
        } else {
            if( ins->type_class == U4
             || ins->type_class == I4
             || ins->type_class == FS ) {
                AddToTemp( back, M_OPND_SIZE );
                return( true );
            }
        }
        return( false );
    }
#endif
}

static  void    LayST( BackRun& back, name *op )
/********************************
 * add the ST(i) operand to a floating point instruction
 */
{
    back.pcencode.Inst[RMR] |= FPRegTrans( op->r.reg );
}


static  void    LayMF( BackRun& back, name *op )
/********************************
 * For a floating point instruction, add the qword ptr stuff
 */
{
    if( op->n.class_ != N_CONSTANT ) {
        switch( op->n.type_class ) {
        case FS:
            back.pcencode.Inst[KEY] |= MF_FS;
            break;
        case FD:
            back.pcencode.Inst[KEY] |= MF_FD;
            break;
        case FL:
            back.pcencode.Inst[KEY] |= MF_FL;
            back.pcencode.Inst[RMR] |= B_RMR_FMT_FL;
            break;
        case U2:
        case I2:
            back.pcencode.Inst[KEY] |= MF_I2;
            break;
        case U4:
        case I4:
            back.pcencode.Inst[KEY] |= MF_I4;
            break;
        case U8:
        case I8:
        case XX:
            back.pcencode.Inst[KEY] |= MF_I8;
            back.pcencode.Inst[RMR] |= B_RMR_FMT_I8;
            break;
        default:
            _Zoiks( ZOIKS_029 );
            break;
        }
    }
}

#if 1
static  void    DoP5RegisterDivide( BackRun& back, instruction *ins )
/****************************************************/
{
    int                 st_reg;
    byte                reverse;
    byte                pop;
    byte                dest;
    int                 ins_key;
    label_handle        lbl;
    label_handle        lbl_2;
    any_oc              oc;

    lbl = AskForNewLabel(back);
    lbl_2 = AskForNewLabel(back);

    oc.oc_jcond.hdr.class_ = OC_JCOND | OC_ATTR_SHORT;
    oc.oc_jcond.hdr.reclen = sizeof( oc_jcond );
    oc.oc_jcond.hdr.objlen = OptInsSize( back, OC_JCOND, OC_DEST_NEAR );
    oc.oc_jcond.ref = NULL;
    oc.oc_jcond.cond = 4;
    oc.oc_jcond.handle = lbl;
    InputOC( back, &oc );
    st_reg = FPRegTrans( ins->operands[0]->r.reg );
    reverse = false;
    pop = false;
    dest = false;
    switch( G( ins ) ) {
    case G_RRFBIN:
        reverse = true;
        break;
    case G_RNFBIN:
        break;
    case G_RRFBINP:
        reverse = true;
        pop = true;
        dest = true;
        break;
    case G_RNFBINP:
        pop = true;
        dest = true;
        break;
    case G_RRFBIND:
        reverse = true;
        dest = true;
        break;
    case G_RNFBIND:
        dest = true;
        break;
    }
    ins_key = ( pop & 1 ) | ( ( reverse & 1 ) << 1 ) | ( ( dest & 1 ) << 2 );
    ins_key |= st_reg << 3;
    // je ok
    _Code;
    _Next;
    LayOpbyte( back, 0x50 );                  /* push [e]ax           */
    _Next;
    LayOpbyte( back, 0xb8 );                  /* mov #cons -> [e]ax   */
    AddWData( back, ins_key, WD );
    _Emit;
    DoRTCall( back, RT_FDIV_FPREG, false );   /* call __fdiv_fpr      */
    _Code;
    LayOpbyte( back, 0x58 );                  /* pop [e]ax            */
    _Emit;
    GenJumpLabel( back, lbl_2 );
    CodeLabel( back, lbl, 0 );
    _Code;
    LayInitial( back, ins, G( ins ) );
    LayOpndSize( back, ins, G( ins ) );
    LayST( back, ins->operands[0] );
    _Emit;
    CodeLabel( back, lbl_2, 0 );
    GenKillLabel( back, lbl );
    GenKillLabel( back, lbl_2 );
}

static  void    DoP5MemoryDivide( BackRun& back, instruction *ins )
/**************************************************/
{
    any_oc              oc;
    rt_class            rtindex;
    label_handle        lbl;
    label_handle        lbl_2;
    name                *high;
    name                *low;
#if _TARGET & _TARG_8086
    name                *h;
    name                *l;
#endif
    name                *seg;

    lbl = AskForNewLabel(back);
    lbl_2 = AskForNewLabel(back);

    oc.oc_jcond.hdr.class_ = OC_JCOND | OC_ATTR_SHORT;
    oc.oc_jcond.hdr.reclen = sizeof( oc_jcond );
    oc.oc_jcond.hdr.objlen = OptInsSize( back, OC_JCOND, OC_DEST_NEAR );
    oc.oc_jcond.ref = NULL;
    oc.oc_jcond.cond = 4;
    oc.oc_jcond.handle = lbl;
    InputOC( back, &oc );

    seg = NULL;
    if( ins->num_operands > OpcodeNumOperands( ins ) ) {
        seg = ins->operands[ins->num_operands - 1];
    }

    _Code;
    switch( ins->operands[0]->n.type_class ) {
    case FS:
#if _TARGET & _TARG_8086
        high = HighPart( back, ins->operands[0], U2 );
        low = LowPart( back, ins->operands[0], U2 );
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, high );
        _Next;
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, low );
#else
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, ins->operands[0] );
#endif
        if( G( ins ) == G_MRFBIN ) {
            rtindex = RT_FDIV_MEM32R;
        } else {
            rtindex = RT_FDIV_MEM32;
        }
        break;
    case FD:
        high = HighPart( back, ins->operands[0], U4 );
        low = LowPart( back, ins->operands[0], U4 );
#if _TARGET & _TARG_8086
        h = HighPart( back, high, U2 );
        l = LowPart( back, high, U2 );
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, h );
        _Next;
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, l );
        _Next;
        h = HighPart( back, low, U2 );
        l = LowPart( back, low, U2 );
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, h );
        _Next;
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, l );
        _Next;
#else
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, high );
        _Next;
        back.x86proc.StackDepth += WORD_SIZE;
        if( seg != NULL )
            GenSeg( back, seg->r.reg );
        LayOpword( back, 0x30ff );
        LayModRM( back, low );
        back.x86proc.StackDepth -= WORD_SIZE;
#endif
        if( G( ins ) == G_MRFBIN ) {
            rtindex = RT_FDIV_MEM64R;
        } else {
            rtindex = RT_FDIV_MEM64;
        }
        break;
    default:
        rtindex = RT_FDIV_MEM64;
        _Zoiks( ZOIKS_114 );
    }
    _Emit;
    DoRTCall( back, rtindex, false );
    GenJumpLabel( back, lbl_2 );
    CodeLabel( back, lbl, 0 );
    _Code;
    if( seg != NULL ) {
        GenSeg( back, seg->r.reg );
    }
    LayInitial( back, ins, G( ins ) );
    LayOpndSize( back, ins, G( ins ) );
    LayMF( back, ins->operands[0] );
    LayModRM( back, ins->operands[0] );
    _Emit;
    CodeLabel( back, lbl_2, 0 );
    GenKillLabel( back, lbl );
    GenKillLabel( back, lbl_2 );
}

static  void    DoP5Divide( BackRun& back, instruction *ins )
/*********************************************
 * Emit a most disgusting sequence of code to
 * test for the Pentium FDIV bug and call a
 * runtime routine to correct it.
 */
{
    bool        used_ds = false;

    _Code;
    LayOpbyte( back, 0x9c );          /* pushf */
    _Emit;
#if _TARGET & _TARG_80386
    back.x86proc.StackDepth += WORD_SIZE;
#endif
    _Code;
    if( _IsTargetModel( CGSW_X86_FLOATING_DS ) ) {
        if( _IsTargetModel( CGSW_X86_FLOATING_SS ) ) {
            LayOpbyte( back, 0x1e );  /* push ds    */
            AddByte( back, 0x50 );    /* push [e]ax */
            _Emit;
            GenLoadDS(back);
            _Code;
            used_ds = true;
        } else {
            AddToTemp( back, 0x36 );
        }
    }
#if ( _TARGET & _TARG_8086 )
    LayOpword( back, 0x06f6 );        /* test byte ptr L1,1 */
#else
    LayOpword( back, 0x05f6 );        /* test byte ptr L1,1 */
#endif
    back.pcencode.ILen += WORD_SIZE;
    DoLblRef( back, RTLabel( back, RT_BUGLIST ), AskBackSeg(back), 0, ESCA_OFST );
    AddWData( back, 1, U1 );
    _Emit;
    if( used_ds ) {
        _Code;
        LayOpbyte( back, 0x58 );      /* pop [e]ax */
        AddByte( back, 0x1f );        /* pop ds    */
        _Emit;
    }
    switch( G( ins ) ) {
    case G_RRFBIN:
    case G_RNFBIN:
    case G_RRFBIND:
    case G_RNFBIND:
    case G_RRFBINP:
    case G_RNFBINP:
        // assuming G_RXFBINZ means
        //      X -> R (reverse) and N (normal)
        //      Z -> D (destination is argument)
        //        -> P (argument is popped)
        DoP5RegisterDivide( back, ins );
        break;
    case G_MRFBIN:
    case G_MNFBIN:
        DoP5MemoryDivide( back, ins );
    }
    _Code;
    LayOpbyte( back, 0x9d );          /* popf */
    _Emit;
#if _TARGET & _TARG_80386
    back.x86proc.StackDepth -= WORD_SIZE;
#endif
}
#endif


static  void    SetCC( BackRun& back )
/****************************
 * Generate code to set the condition codes based on an 8087 FCMP instruction
 */
{
    if( _IsEmulation() ) {
        _Emit;
        _Code;
        SetFPPatchType( back, FPP_NORMAL );
    }
    if( _CPULevel( CPU_386 ) ) {
        if( _IsEmulation() ) {
            AddByte( back, 0x9b );        /*  FWAIT - needed for FIDRQQ to work */
        }
        AddByte( back, 0xdf );            /*  FSTSW  AX       */
        AddByte( back, 0xe0 );            /*  ..              */
    } else {
        if( !_CPULevel( CPU_386 ) ) {
            AddByte( back, 0x9b );        /*  FWAIT           */
        }
        if( _CPULevel( CPU_286 ) && !_IsEmulation() ) {
            AddByte( back, 0xdf );        /*  FSTSW  AX       */
            AddByte( back, 0xe0 );        /*  ..              */
        } else {
            _Next;                  /*                  */
            LayOpword( back, 0x38dd );    /*  FSTSW  temp     */
            LayModRM( back, back.i87data.FPStatWord ); /*  ..              */
            if( _IsEmulation() ) {
                _Emit;              /*                  */
                GFwait(back);           /*  FWAIT           */
                _Code;              /*                  */
            } else {
                AddByte( back, 0x9b );    /*  FWAIT           */
                _Next;              /*                  */
            }
            LayOpword( back, 0x008a );    /*  MOV    AX,temp  */
            LayReg( back, HW_AX );        /*  ..              */
            LayW( back, U2 );             /*  ..              */
            LayModRM( back, back.i87data.FPStatWord ); /*  ..              */
        }
    }
    AddByte( back, 0x9e );                /*  SAHF            */
}


/*
 * Lay Routines
 */

void    LayOpbyte( BackRun& back, gen_opcode op )
/**********************************
 * Add a one byte opcode to Inst[]
 */
{
    back.pcencode.Inst[KEY] = op & 0xff;
    back.x86enc.ICur = 1;
    back.pcencode.ILen = 1;
    back.x86enc.IEsc = 1;
}

void    LayOpword( BackRun& back, gen_opcode op )
/*********************************
 * Add a 2 byte opcode to Inst[]
 */
{
    back.pcencode.Inst[KEY] = op & 0xff;
    back.pcencode.Inst[RMR] = (op >> 8) & 0xff;
    back.x86enc.ICur = 2;
    back.pcencode.ILen = 2;
    back.x86enc.IEsc = 2;
}

void    LayW( BackRun& back, type_class_def type_class )
/***************************************/
{
    switch( type_class ) {
    case U2:
    case I2:
    case U4:
    case I4:
    case FS:
        back.pcencode.Inst[KEY] |= B_KEY_W;       /* turn on the W bit*/
        break;
    default:
        break;
    }
}

static  void    LayRegOp( BackRun& back, name *r )
/**********************************
 * Add the register op to the instruction
 */
{
    back.pcencode.Inst[RMR] |= RegTrans( back, r->r.reg ) << S_RMR_REG;
}

void    LayReg( BackRun& back, hw_reg_set r )
/***************************************
 * Add the register op to the instruction
 */
{
    back.pcencode.Inst[RMR] |= RegTrans( back, r ) << S_RMR_REG;
}

void    LayRMRegOp( BackRun& back, name *r )
/****************************
 * Add the register op to a MOD/RM instruction
 */
{
    back.pcencode.Inst[RMR] |= ( RegTrans( back, r->r.reg ) << S_RMR_RM ) + RMR_MOD_REG;
}

void    LayRegRM( BackRun& back, hw_reg_set r )
/*******************************
 * Add the register op to a MOD/RM instruction
 */
{
    back.pcencode.Inst[RMR] |= ( RegTrans( back, r ) << S_RMR_RM ) + RMR_MOD_REG;
}

static  void    LayACRegOp( BackRun& back, name *r )
/************************************
 * Add the other register op to a REG/AX (Accumulator) operation
 */
{
    back.pcencode.Inst[KEY] |= RegTrans( back, r->r.reg ) << S_KEY_REG;
}

void    LayRegAC( BackRun& back, hw_reg_set r )
/*****************************************
 * Add the other register op to a REG/AX (Accumulator) operation
 */
{
    back.pcencode.Inst[KEY] |= RegTrans( back, r ) << S_KEY_REG;
}

static  void    LaySROp( BackRun& back, name *r )
/*********************************
 * Add a segment register op
 */
{
    back.pcencode.Inst[RMR] |= SegTrans( back, r->r.reg ) << S_RMR_SR;
}

void    GenSeg( BackRun& back, hw_reg_set regs )
/******************************************
 * Generate a segment override if we need one. regs if the full address.
 * For example, if regs is DS:BP, we need an override. SS:BP wouldn't
 */
{
    hw_reg_set  segreg;
    int         i;

    segreg = regs;
    HW_COnlyOn( segreg, HW_SEGS );
    if( HW_CEqual( segreg, HW_EMPTY ) )
        return;
    if( HW_COvlap( regs, HW_xBP ) ) {
        if( HW_CEqual( segreg, HW_SS ) )
            return;
        if( HW_CEqual( segreg, HW_DS )
          && _IsntTargetModel( CGSW_X86_FLOATING_DS )
          && _IsntTargetModel( CGSW_X86_FLOATING_SS ) ) {
            return;
        }
    } else {
        if( HW_CEqual( segreg, HW_DS ) ) {
            return;
        }
    }
    /*
     * produce segment override prefix
     */
    if( HW_COvlap( regs, HW_FS ) ) {
        AddToTemp( back, M_SEGFS );
    } else if( HW_COvlap( regs, HW_GS ) ) {
        AddToTemp( back, M_SEGGS );
    } else {
        AddToTemp( back, M_SEGOVER | ( SegTrans( back, regs ) << S_KEY_SR ) );
    }
    if( _IsEmulation() ) {
        for( i = ARCH_SEG_START; i < ARCH_SEG_END; i++ ) {
            if( HW_Equal( segreg, RegsTab[i].hw_reg ) ) {
                SetFPPatchSegm( back, RegsTab[i].idx );
                break;
            }
        }
    }
}

type_class_def  OpndSize( BackRun& back, hw_reg_set regs )
/******************************************
 * Generate an operand size prefix if we need it and return the
 * type_class of the register "reg"
 */
{
    if( HW_COvlap( regs, HW_SEGS ) )
        return( U2 );
#if _TARGET & _TARG_8086
    if( _IsTargetModel( CGSW_X86_USE_32 ) )
        AddToTemp( back, M_OPND_SIZE );
    return( U2 );
#else
    if( HW_COvlap( regs, HW_32 ) ) {
        if( _IsntTargetModel( CGSW_X86_USE_32 ) )
            AddToTemp( back, M_OPND_SIZE );
        return( U4 );
    } else {
        if( _IsTargetModel( CGSW_X86_USE_32 ) )
            AddToTemp( back, M_OPND_SIZE );
        return( U2 );
    }
#endif
}

static  void    PushSeg( BackRun& back, hw_reg_set reg )
/****************************************
 * PUSH segreg
 */
{
    if( HW_COvlap( reg, HW_FS ) ) {
        LayOpword( back, M_PUSHFS );
    } else if( HW_COvlap( reg, HW_GS ) ) {
        LayOpword( back, M_PUSHGS );
    } else {
        LayOpbyte( back, M_PUSHSEG | ( SegTrans( back, reg ) << S_KEY_SR ) );
    }
}

static  void    PopSeg( BackRun& back, hw_reg_set reg )
/***************************************
 * POP segreg
 */
{
    if( HW_COvlap( reg, HW_FS ) ) {
        LayOpword( back, M_POPFS );
    } else if( HW_COvlap( reg, HW_GS ) ) {
        LayOpword( back, M_POPGS );
    } else {
        LayOpbyte( back, M_POPSEG | ( SegTrans( back, reg ) << S_KEY_SR ) );
    }
}


void    QuickSave( BackRun& back, hw_reg_set reg, opcode_defs opcode )
/******************************************************
 * PUSH/POP    reg     - based on "opcode"
 * assume register is valid for PUSH/POP
 */
{
    _Code;
    if( HW_COvlap( reg, HW_SEGS ) ) {
        if( opcode == OP_PUSH ) {
            PushSeg( back, reg );
        } else {
            PopSeg( back, reg );
        }
    } else {
        if( opcode == OP_PUSH ) {
            LayOpbyte( back, M_PUSH | ( RegTrans( back, reg ) << S_KEY_REG ) );
        } else {
            LayOpbyte( back, M_POP | ( RegTrans( back, reg ) << S_KEY_REG ) );
        }
        OpndSize( back, reg );
    }
    _Emit;
}


void    GenRegXor( BackRun& back, hw_reg_set src, hw_reg_set dst )
/**************************************************
 * XOR         dst,src
 */
{
    _Code;
    LayOpword( back, M_XORRR | B_KEY_W );
    OpndSize( back, src );
    LayReg( back, src );
    LayRegRM( back, dst );
    _Emit;
}


void    GenRegNeg( BackRun& back, hw_reg_set src )
/**********************************
 * NEG         src
 */
{
    _Code;
    LayOpword( back, M_NEGR | B_KEY_W );
    OpndSize( back, src );
    LayRegRM( back, src );
    _Emit;
}


void    GenRegMove( BackRun& back, hw_reg_set src, hw_reg_set dst )
/***************************************************
 * MOV         dst,src
 */
{
    _Code;
#if _TARGET & _TARG_8086
    if( _IsTargetModel( CGSW_X86_MSC8 ) ) {
        /* Microsoft C 8 moves a register into a register as MOV reg,r/m (8B: MOV BP,SP is 8B EC) (-zmsc) */
        LayOpword( back, M_MOVRR | 2 | B_KEY_W );
        OpndSize( back, src );
        LayReg( back, dst );
        LayRegRM( back, src );
        _Emit;
        return;
    }
#endif
    LayOpword( back, M_MOVRR | B_KEY_W );
    OpndSize( back, src );
    LayReg( back, src );
    LayRegRM( back, dst );
    _Emit;
}


static  void    AddSWData( BackRun& back, opcode_defs opcode, int_32 val, type_class_def type_class )
/*************************************************************************************
 * Add a const (int_32) to Inst[] with sign extension if the opcode
 * allows it
 */
{
    switch( opcode ) {
    case OP_ADD:
    case OP_EXT_ADD:
    case OP_SUB:
    case OP_EXT_SUB:
    case OP_AND:
    case OP_OR:
    case OP_XOR:
    case OP_MUL:
    case OP_CMP_EQUAL:
    case OP_CMP_NOT_EQUAL:
    case OP_CMP_GREATER:
    case OP_CMP_LESS_EQUAL:
    case OP_CMP_LESS:
    case OP_CMP_GREATER_EQUAL:
        AddSData( back, val, type_class );
        break;
    default:
        AddWData( back, val, type_class );
        break;
    }
}


static  void    AddSWCons( BackRun& back, opcode_defs opcode, name *opnd, type_class_def type_class )
/*************************************************************************************
 * Add a const (name *) to Inst[] with sign extension if the opcode allows it
 */
{
    if( opnd->c.const_type == CONS_ABSOLUTE ) {
        AddSWData( back, opcode, opnd->c.lo.u.int_value, type_class );
    } else {
        switch( type_class ) {
        case U2:
        case I2:
        case U4:
        case I4:
            DoRelocConst( back, opnd, type_class );
            break;
        default:
            Zoiks( back, ZOIKS_045 );
            break;
        }
    }
}


void    AddWData( BackRun& back, int_32 value, type_class_def type_class )
/*********************************************************/
{
    AddByte( back, (byte)value );
    if( type_class == U1 || type_class == I1 )
        return;
    value >>= 8;
    AddByte( back, (byte)value );
    if( type_class == U2 || type_class == I2 )
        return;
    value >>= 8;
    AddByte( back, (byte)value );
    value >>= 8;
    AddByte( back, (byte)value );
}

void    AddWCons( BackRun& back, name *op, type_class_def type_class )
/************************************************
 * Add a WORD constant to Inst[]
 */
{
    if( op->c.const_type == CONS_ABSOLUTE ) {
        AddWData( back, op->c.lo.u.int_value, type_class );
    } else  {
        switch( type_class ) {
        case U2:
        case I2:
        case U4:
        case I4:
            DoRelocConst( back, op, type_class );
            break;
        default:
            Zoiks( back, ZOIKS_045 );
            break;
        }
    }
}

void    AddSData( BackRun& back, int_32 value, type_class_def type_class )
/**********************************************************
 * Add a constant (int_32) to Inst[], with possible sign extension
 */
{
    if( ( type_class == U2 || type_class == I2 )
        && ( ( value & 0xff80 ) == 0xff80
          || ( value & 0xff80 ) == 0 ) ) {
        back.pcencode.Inst[KEY] |= B_KEY_S;
        AddByte( back, _IntToByte( value ) );
    } else if( ( type_class == U4 || type_class == I4 )
        && ( ( value & 0xffffff80 ) == 0xffffff80
          || ( value & 0xffffff80 ) == 0 ) ) {
        back.pcencode.Inst[KEY] |= B_KEY_S;
        AddByte( back, _IntToByte( value ) );
    } else {
        AddWData( back, value, type_class );
    }
}

static  void    AddSCons( BackRun& back, name *op, type_class_def type_class )
/**********************************************************
 * Add a constant (name *) to Inst[], with possible sign extension
 */
{
    if( op->c.const_type == CONS_ABSOLUTE ) {
        AddSData( back, op->c.lo.u.int_value, type_class );
    } else {
        switch( type_class ) {
        case U2:
        case I2:
        case U4:
        case I4:
            DoRelocConst( back, op, type_class );
            break;
        default:
            Zoiks( back, ZOIKS_045 );
            break;
        }
    }
}


static  void    GenRegOp( BackRun& back, hw_reg_set dst, type_length value, gen_opcode op )
/**************************************************************************/
{
    type_class_def      type_class;

    _Code;
    LayOpword( back, op | B_KEY_W );
    type_class = OpndSize( back, dst );
    LayRegRM( back, dst );
    AddSData( back, value, type_class );
    _Emit;
}


void    GenRegAdd( BackRun& back, hw_reg_set dst, type_length value )
/*****************************************************
 * ADD dst,value
 */
{
    GenRegOp( back, dst, value, M_ADDRC );
}


void    GenRegSub( BackRun& back, hw_reg_set dst, type_length value )
/*****************************************************
 * SUB         dst,value
 */
{
    GenRegOp( back, dst, value, M_SUBRC );
}


void    GenRegAnd( BackRun& back, hw_reg_set dst, type_length value )
/*****************************************************
 * SUB         dst,value
 */
{
    GenRegOp( back, dst, value, M_ANDRC );
}


static  void    GFld1( BackRun& back )
/****************************
 * FLD1
 */
{
    GCondFwait(back);
    LayOpword( back, 0xe8d9 );
    _Emit;
}


void    GFldMorC( BackRun& back, name *what )
/***************************************
 * FLD         what, where what is either 0,1,or a memory location
 */
{
    if( what->n.class_ == N_MEMORY || what->n.class_ == N_TEMP ) {
        GFldM( back, what );
    } else if( what->c.lo.u.int_value == 0 ) {
        GFldz(back);
    } else {
        GFld1(back);
    }
}

void    GFldM( BackRun& back, pointer what )
/**************************************
 * FLD         tbyte ptr what
 */
{
    GCondFwait(back);
    LayOpword( back, 0x00d9 );
    LayMF( back, static_cast<name *>(what) );
    LayModRM( back, static_cast<name *>(what) );
    _Emit;
}


void    GFstpM( BackRun& back, pointer what )
/*****************************
 * FSTP        tbyte ptr what
 */
{
    GCondFwait(back);
    LayOpword( back, 0x18d9 );
    LayMF( back, static_cast<name *>(what) );
    LayModRM( back, static_cast<name *>(what) );
    _Emit;
}


void    GFstp( BackRun& back, int i )
/*********************
 * FSTP        ST(i)
 */
{
    GCondFwait(back);
    LayOpword( back, 0xd8dd );
    back.pcencode.Inst[RMR] |= i;
    _Emit;
}


void    GFxch( BackRun& back, int i )
/*********************
 * FXCH        ST(i)
 */
{
    GCondFwait(back);
    LayOpword( back, 0xc8d9 );
    back.pcencode.Inst[RMR] |= i;
    _Emit;
}


void    GFldz( BackRun& back )
/********************
 * FLDZ
 */
{
    GCondFwait(back);
    LayOpword( back, 0xeed9 );
    _Emit;
}


void    GFld( BackRun& back, int i )
/********************
 * FLD         ST(i)
 */
{
    GCondFwait(back);
    LayOpword( back, 0xc0d9 );
    back.pcencode.Inst[RMR] |= i;
    _Emit;
}


void    GCondFwait( BackRun& back )
/**********************************
 * FWAIT, but only if we need one
 */
{
    _Code;
    back.i87data.Used87 = true;
#if _TARGET & _TARG_8086
    if( !_CPULevel( CPU_286 ) || _IsEmulation() ) {
        if( _IsEmulation() ) {
            SetFPPatchType( back, FPP_NORMAL );
        }
        LayOpbyte( back, 0x9b );
        _Next;
    }
#endif
}


void    GFwait( BackRun& back )
/*********************
 * FWAIT
 */
{
    if( _CPULevel( CPU_386 ) )
        return;
    _Code;
    back.i87data.Used87 = true;
    if( _IsEmulation() ) {
        SetFPPatchType( back, FPP_WAIT );
        LayOpword( back, 0x9b90 );
    } else {
        LayOpbyte( back, 0x9b );
    }
   _Emit;
}

void    Gpusha( BackRun& back )
/*********************
 * PUSHA{d}
 */
{
    _Code;
    LayOpbyte( back, 0x60 );
   _Emit;
}

void    Gpopa( BackRun& back )
/********************
 * POPA{d}
 */
{
    _Code;
    LayOpbyte( back, 0x61 );
   _Emit;
}

void    Gcld( BackRun& back )
/*******************
 * CLD
 */
{
    _Code;
    LayOpbyte( back, 0xfc );
   _Emit;
}

void    GenLeave( BackRun& back )
/***********************
 * LEAVE
 */
{
    _Code;
    LayOpbyte( back, 0xc9 );
    OpndSize( back, HW_xBP );
    _Emit;
}


void    GenTouchStack( BackRun& back, bool sp_might_point_at_something )
/********************************************************
 * MOV         [esp],eax
 */
{
#if _TARGET & _TARG_8086
    /* unused parameters */ (void)sp_might_point_at_something;
#else
    if( sp_might_point_at_something || back.data.OptForSize == 100 ) {
        QuickSave( back, HW_xAX, OP_PUSH );
        QuickSave( back, HW_xAX, OP_POP );
    } else {
        _Code;
        LayOpword( back, 0x0489 );    /*  mov dword ptr [esp],eax */
        OpndSize( back, HW_xSP );     /*  ..                      */
        AddByte( back, 0x24 );        /*  ..                      */
        _Emit;
    }
#endif
}


void    GenEnter( BackRun& back, int size, int level )
/**************************************
 * ENTER       size,level
 */
{
    _Code;
    LayOpbyte( back, 0xc8 );
    OpndSize( back, HW_xBP );
    AddWData( back, size, U2 );
    AddByte( back, (byte)level );
    _Emit;
}


void    GenPushOffset( BackRun& back, byte offset )
/***********************************
 * PUSH        word ptr offset[BP]
 */
{
    _Code;
    LayOpword( back, M_PUSHATBP );
    AddWData( back, offset, I1 );
    _Emit;
}


void    GenUnkSub( BackRun& back, hw_reg_set dst, pointer value )
/*************************************************
 * SUB         dst,??? - to be patched later
 */
{
    _Code;
    LayOpword( back, M_SUBRC | B_KEY_W );
    OpndSize( back, dst );
    LayRegRM( back, dst );
    back.pcencode.ILen += WORD_SIZE;
    DoAbsPatch( back, static_cast<abspatch_handle *>(value), WORD_SIZE );
    _Emit;
}


void    GenUnkMov( BackRun& back, hw_reg_set dst, pointer value )
/*************************************************
 * MOV         dst,??? - to be patched
 */
{
    /* unused parameters */ (void)dst;

    _Code;
    LayOpbyte( back, 0xb8 );
    OpndSize( back, dst );
    back.pcencode.ILen += WORD_SIZE;
    DoAbsPatch( back, static_cast<abspatch_handle *>(value), WORD_SIZE );
    _Emit;
}


void    GenUnkEnter( BackRun& back, pointer value, int level )
/**********************************************
 * ENTER       ??,level - to be patched later
 */
{
    _Code;
    LayOpbyte( back, 0xc8 );
    DoAbsPatch( back, static_cast<abspatch_handle *>(value), 2 );
    back.pcencode.ILen += 2;
    AddByte( back, (byte)level );
    _Emit;
}

void    GenWindowsProlog( BackRun& back )
/******************************/
{
    _Code;
    if( _IsTargetModel( CGSW_X86_SMART_WINDOWS ) ) {
        LayOpbyte( back, 0x8c );          /*  mov     [e]ax, ss   */
        AddByte( back, 0xd0 );            /*  ..                  */
    } else {
        LayOpbyte( back, 0x1e );          /*  push    ds          */
        AddByte( back, 0x58 );            /*  pop     [e]ax       */
        AddByte( back, 0x90 );            /*  nop                 */
    }
    if( !_CPULevel( CPU_386 ) ) {
        AddByte( back, 0x45 );            /*  inc     [e]bp       */
    }
    AddByte( back, 0x55 );                /*  push    [e]bp       */
    AddByte( back, 0x89 );                /*  mov     [e]bp,[e]sp */
    AddByte( back, 0xe5 );                /*  ..                  */
    AddByte( back, 0x1e );                /*  push    ds          */
    AddByte( back, 0x8e );                /*  mov     ds,[e]ax    */
    AddByte( back, 0xd8 );                /*  ..                  */
    _Emit;
}

void    GenCheapWindowsProlog( BackRun& back )
/************************************
 * Generate a "cheap" windows prolog
 */
{
    _Code;
    if( !_CPULevel( CPU_386 ) ) {
        LayOpbyte( back, 0x45 );          /*  inc     [e]bp       */
    }
    AddByte( back, 0x55 );                /*  push    [e]bp       */
    AddByte( back, 0x89 );                /*  mov     [e]bp,[e]sp */
    AddByte( back, 0xe5 );                /*  ..                  */
    _Emit;
}

void    GenWindowsEpilog( BackRun& back )
/******************************/
{
    _Code;
    LayOpbyte( back, 0x1f );              /*  pop     ds          */
    AddByte( back, 0x5d );                /*  pop     [e]bp       */
    if( !_CPULevel( CPU_386 ) ) {
        AddByte( back, 0x4d );            /*  dec     [e]bp       */
    }
    _Emit;
}

void    GenCheapWindowsEpilog( BackRun& back )
/************************************
 * Generate a "cheap" windows epilog
 */
{
    _Code;
    LayOpbyte( back, 0x5d );              /*  pop     [e]bp       */
    if( !_CPULevel( CPU_386 ) ) {
        AddByte( back, 0x4d );            /*  dec     [e]bp       */
    }
    _Emit;
}


void    GenRdosdevProlog( BackRun& back )
/******************************/
{
    _Code;
    LayOpbyte( back, 0x1e );              /*  push    ds        */
    _Emit;

    _Code;
    LayOpbyte( back, 0x6 );               /*  push    es        */
    _Emit;

    _Code;
    LayOpbyte( back, 0xf );               /*  push    fs        */
    AddByte( back, 0xa0 );
    _Emit;

    _Code;
    LayOpbyte( back, 0xf );               /*  push    gs        */
    AddByte( back, 0xa8 );
    _Emit;

    _Code;
    LayOpbyte( back, 0x68 );              /*  push    DGROUP    */
    back.pcencode.ILen += WORD_SIZE;
    DoSegRef( back, AskBackSeg(back) );
    AddByte( back, 0x0 );
    AddByte( back, 0x0 );
    AddByte( back, 0x1F );                /*  pop     ds        */
    _Emit;
}

void    GenRdosdevEpilog( BackRun& back )
/******************************/
{
    _Code;
    LayOpbyte( back, 0xf );               /*  pop     gs      */
    AddByte( back, 0xa9 );
    _Emit;

    _Code;
    LayOpbyte( back, 0xf );               /*  pop     fs      */
    AddByte( back, 0xa1 );
    _Emit;

    _Code;
    LayOpbyte( back, 0x7 );               /*  pop     es      */
    _Emit;

    _Code;
    LayOpbyte( back, 0x1f );              /*  pop     ds      */
    _Emit;
}

void    GenLoadDS( BackRun& back )
/***********************/
{
    _Code;
    LayOpbyte( back, 0xb8 );              /*  mov     [e]ax,DGROUP */
    OpndSize( back, HW_xAX );
    back.pcencode.ILen += WORD_SIZE;
    DoSegRef( back, AskBackSeg(back) );
    AddByte( back, 0x8e );                /*  mov     ds,[e]ax   */
    AddByte( back, 0xd8 );                /*  ..                 */
    _Emit;
}


static  void    OutputFP( BackRun& back, gen_opcode op )
/***************************************/
{
    GCondFwait(back);
    LayOpword( back, op );
}

static  void    MathFunc( BackRun& back, instruction *ins )
/*******************************************
 * Generate inline code for a math function instruction
 */
{
    switch( ins->head.opcode ) {
    case OP_EXP:
        OutputFP( back, 0xE8D9 ); _Emit;  /*   FLD1           */
        OutputFP( back, 0xEAD9 ); _Emit;  /*   FLDL2E         */
        OutputFP( back, 0xCAD8 ); _Emit;  /*   FMUL ST,ST(2)  */
        OutputFP( back, 0xD2DD ); _Emit;  /*   FST  ST(2)     */
        OutputFP( back, 0xF8D9 ); _Emit;  /*   FPREM          */
        OutputFP( back, 0xF0D9 ); _Emit;  /*   F2XM1          */
        OutputFP( back, 0xC1DE ); _Emit;  /*   FADDP ST(1),ST */
        OutputFP( back, 0xFDD9 ); _Emit;  /*   FSCALE         */
        OutputFP( back, 0xD9DD );         /*   FSTP ST(1)     */
        break;
    case OP_LOG:
        OutputFP( back, 0xEDD9 ); _Emit;  /*   FLDLN2         */
        OutputFP( back, 0xC9D9 ); _Emit;  /*   FXCH    ST(1)  */
        OutputFP( back, 0xF1D9 );         /*   FYL2X          */
        break;
    case OP_LOG10:
        OutputFP( back, 0xECD9 ); _Emit;  /*   FLDLG2         */
        OutputFP( back, 0xC9D9 ); _Emit;  /*   FXCH    ST(1)  */
        OutputFP( back, 0xF1D9 );         /*   FYL2X          */
        break;
    case OP_COS:
        OutputFP( back, 0xffD9 );         /*   FCOS           */
        break;
    case OP_SIN:
        OutputFP( back, 0xfeD9 );         /*   FSIN           */
        break;
    case OP_TAN:
        OutputFP( back, 0xF2D9 ); _Emit;  /*   FPTAN          */
        OutputFP( back, 0xD8DD );         /*   FSTP    ST(0)  */
        break;
    case OP_ATAN:
        OutputFP( back, 0xE8D9 ); _Emit;  /*   FLD1           */
        OutputFP( back, 0xF3D9 );         /*   FPATAN         */
        break;
    case OP_SQRT:
        OutputFP( back, 0xFAD9 );         /*   FSQRT          */
        break;
    case OP_FABS:
        OutputFP( back, 0xE1D9 );         /*   FABS           */
        break;
    default:
        break;
    }
}

static  void    CallMathFunc( BackRun& back, instruction *ins )
/***********************************************
 * Call a runtime routine for a math function instructions
 */
{
    rt_class    rtindex;

    rtindex = LookupRoutine( back, ins );
    DoCall( back, RTLabel( back, rtindex ), true, _IsTargetModel( CGSW_X86_BIG_CODE ), false );
}

static void     GenUnkLea( BackRun& back, pointer value )
/****************************************/
{
    LayOpword( back, M_LEA );
    OpndSize( back, HW_xSP );
    LayReg( back, HW_xSP );
    back.pcencode.Inst[RMR] |= DISPW;
    back.pcencode.ILen += WORD_SIZE;
    DoAbsPatch( back, static_cast<abspatch_handle *>(value), WORD_SIZE );
    back.pcencode.Inst[RMR] |= DoIndex( back, HW_xBP );
}

#if ( _TARGET & _TARG_8086 )
static bool CleanupDiesAtLeave( BackRun& back, instruction *ins )
/*************************************************
 * Nothing after `ins` in its block reaches the stack, the block returns, and the epilog is
 * LEAVE alone: SP added to here is set back from BP all the same
 */
{
    instruction *next;
    opcnt       i;

    for( next = ins->head.next; next->head.opcode != OP_BLOCK; next = next->head.next ) {
        switch( next->head.opcode ) {
        case OP_MOV: case OP_ADD: case OP_SUB: case OP_AND: case OP_OR: case OP_XOR:
        case OP_NEGATE: case OP_COMPLEMENT: case OP_LSHIFT: case OP_RSHIFT: case OP_CONVERT:
        case OP_LA: case OP_CAREFUL_LA: case OP_NOP: case OP_EXT_ADD: case OP_EXT_SUB:
            break;
        default:
            return( false );
        }
        if( next->result != NULL && next->result->n.class_ == N_REGISTER
          && HW_COvlap( next->result->r.reg, HW_SP ) )
            return( false );
        for( i = 0; i < next->num_operands; ++i ) {
            if( next->operands[i]->n.class_ == N_REGISTER && HW_COvlap( next->operands[i]->r.reg, HW_SP ) ) {
                return( false );
            }
        }
    }
    if( !_IsBlkAttr( _BLOCK( next ), BLK_RETURN ) )
        return( false );
    return( EpilogIsLeaveOnly(back) );
}
#endif

void    GenObjCode( BackRun& back, instruction *ins )
/*************************************
 * Generate object code for the instruction "ins" based on gen_table->generate
 */
{
    gentype     gen;
    name        *result;
    name        *left = NULL;
    name        *right = NULL;
    opcnt       i;
    bool        opnd_size;

    gen = G( ins );
    if( gen != G_NO ) {
#if 1
        // fixme - should be _IsTargetModel
        if( _IsTargetModel( CGSW_X86_P5_DIVIDE_CHECK ) ) {
            if( ins->head.opcode == OP_DIV && _IsFloating( ins->type_class ) ) {
                DoP5Divide( back, ins );
                return;
            }
        }
#endif
        result = ins->result;
        if( ins->num_operands != 0 ) {
            left = ins->operands[0];
            if( ins->num_operands != 1 ) {
                right = ins->operands[1];
            }
        }

#if ( _TARGET & _TARG_8086 )
        if( _IsTargetModel( CGSW_X86_MSC8 ) && ins->head.opcode == OP_ADD && result != NULL
          && result->n.class_ == N_REGISTER && HW_CEqual( result->r.reg, HW_SP )
          && right != NULL && right->n.class_ == N_CONSTANT && right->c.const_type == CONS_ABSOLUTE
          && right->c.lo.u.int_value > 0 && CleanupDiesAtLeave( back, ins ) ) {
            /*
             * Microsoft C 8 does not take a call's arguments off when all that is left of the
             * function is its LEAVE: SP is set back from BP there (-zmsc)
             */
            AdjustStackDepth( back, ins );
            return;
        }
        if( _IsTargetModel( CGSW_X86_MSC8 ) && back.data.OptForSize == 100
          && ins->head.opcode == OP_ADD && result != NULL && result->n.class_ == N_REGISTER
          && HW_CEqual( result->r.reg, HW_SP ) && right != NULL && right->n.class_ == N_CONSTANT
          && right->c.const_type == CONS_ABSOLUTE && right->c.lo.u.int_value == 2 ) {
            /*
             * Microsoft C 8 at /Os takes two bytes of a call's arguments off with POP BX -- the
             * callee may have changed BX, so nothing lives in it (-zmsc)
             */
            _Code;
            LayOpbyte( back, 0x5b );
            _Emit;
            return;
        }
#endif
        if( gen == G_MFSTRND ) {
            _Code;
#if ( _TARGET & _TARG_8086 )
            AddByte(back, 0x6a);      /*  push    0x00            */
            AddByte(back, 0x00);      /*  ..                      */
            AddByte(back, 0x68);      /*  push    0x0c3f          */
            AddByte(back, 0x3f);      /*  ..                      */
            AddByte(back, 0x0c);      /*  ..                      */
            AddByte(back, 0x55);      /*  push    bp              */
            AddByte(back, 0x89);      /*  mov     bp,sp           */
            AddByte(back, 0xe5);      /*  ..                      */
            AddByte(back, 0xd9);      /*  fnstcw  word ptr [bp+4] */
            AddByte(back, 0x7e);      /*  ..                      */
            AddByte(back, 0x04);      /*  ..                      */
            AddByte(back, 0xd9);      /*  fldcw   word ptr [bp+2] */
            AddByte(back, 0x6e);      /*  ..                      */
            AddByte(back, 0x02);      /*  ..                      */
            AddByte(back, 0x5d);      /*  pop     bp              */
#else
            AddByte(back, 0x68);      /*  push    0x00000c3f      */
            AddByte(back, 0x3f);      /*  ..                      */
            AddByte(back, 0x0c);      /*  ..                      */
            AddByte(back, 0x00);      /*  ..                      */
            AddByte(back, 0x00);      /*  ..                      */
            AddByte(back, 0xd9);      /*  fnstcw  word ptr [esp+2]*/
            AddByte(back, 0x7c);      /*  ..                      */
            AddByte(back, 0x24);      /*  ..                      */
            AddByte(back, 0x02);      /*  ..                      */
            AddByte(back, 0xd9);      /*  fldcw   word ptr [esp]  */
            AddByte(back, 0x2c);      /*  ..                      */
            AddByte(back, 0x24);      /*  ..                      */
#endif
            _Emit;

#if ( _TARGET & _TARG_8086 )
            AdjustStackDepthDirect( back, 2 * WORD_SIZE );
#else
            AdjustStackDepthDirect( back, WORD_SIZE );
#endif
        }

        _Code;
        LayInitial( back, ins, gen );
        if( ins->num_operands > OpcodeNumOperands( ins ) ) {
            i = ins->num_operands - 1;
            if( ins->operands[i]->n.class_ == N_REGISTER ) {
                GenSeg( back, ins->operands[i]->r.reg );
            } else {
                _Zoiks( ZOIKS_027 );
            }
        }
        opnd_size = LayOpndSize( back, ins, gen );
        switch( gen ) {
        case G_RC:
            LayRMRegOp( back, left );
            AddSWCons( back, ins->head.opcode, right, ins->type_class );
            break;
        case G_AC:
            back.x86enc.ICur--;
            back.pcencode.ILen--;
            back.x86enc.IEsc--;
            AddWCons( back, right, ins->type_class );
            break;
        case G_MC:
            LayModRM( back, left );
            AddSWCons( back, ins->head.opcode, right, ins->type_class );
            break;
        case G_RR2:
#if _TARGET & _TARG_8086
            /*
             * Microsoft C 8 writes a register-to-register MOV, ADD, OR, ADC, SBB, AND, SUB, XOR or
             * CMP in its reg,r/m form, the direction bit set: 8B, 03, 2B ... (-zmsc)
             */
            if( _IsTargetModel( CGSW_X86_MSC8 ) && ( ( back.pcencode.Inst[KEY] & 0xc6 ) == 0x00 || ( back.pcencode.Inst[KEY] & 0xfe ) == 0x88 ) ) {
                back.pcencode.Inst[KEY] |= 0x02;
                LayRegOp( back, left );
                LayRMRegOp( back, right );
                break;
            }
#endif
            LayRegOp( back, right );
            LayRMRegOp( back, left );
            break;
        case G_0FRR2:
            LayOpword( back, M_386MUL );
            LayRegOp( back, left );
            LayRMRegOp( back, right );
            AddToTemp( back, M_SECONDARY );
            break;
        case G_0FRM2:
            LayOpword( back, M_386MUL );
            LayModRM( back, right );
            LayRegOp( back, left );
            AddToTemp( back, M_SECONDARY );
            break;
        case G_PUSHFS:
            LayOpbyte( back, M_PUSHI );
            AddSCons( back, IntEquivalent( back, left ), U4 );
            break;
        case G_RM2:
            LayModRM( back, right );
            LayRegOp( back, left );
            break;
        case G_MR2:
            LayModRM( back, left );
            LayRegOp( back, right );
            break;
        case G_WORDR1:
            if( ins->head.opcode == OP_POP ) {
                LayACRegOp( back, result );
                break;
            }
            LayACRegOp( back, left );
            if( OpcodeNumOperands( ins ) != 1 && right->c.lo.u.int_value == 2 ) {
                /*
                 * never address
                 */
                TransferIns(back);
                if( opnd_size ) {
                    AddToTemp( back, M_OPND_SIZE );
                }
            }
            break;
        case G_R1:
            LayRMRegOp( back, left );
            if( ins->num_operands != 1 && right->c.lo.u.int_value == 2 ) {
                TransferIns(back);
                if( opnd_size ) {
                    AddToTemp( back, M_OPND_SIZE );
                }
            }
            break;
        case G_R1SHIFT:
        case G_RCLSHIFT:
            LayRMRegOp( back, left );
            break;
        case G_M1:
        case G_1SHIFT:
        case G_CLSHIFT:
            LayModRM( back, left );
            break;
        case G_RNSHIFT:
            LayRMRegOp( back, left );
            AddByte( back, (byte)right->c.lo.u.int_value ); /* never address */
            break;
        case G_NSHIFT:
            LayModRM( back, left );
            AddByte( back, (byte)right->c.lo.u.int_value ); /* never address */
            break;
        case G_R2:
            LayRMRegOp( back, right );
            break;
        case G_M2:
            LayModRM( back, right );
            break;
        case G_RR1:
            LayRegOp( back, left );
            LayRMRegOp( back, result );
            break;
        case G_RM1:
            LayRegOp( back, result );
            LayModRM( back, left );
            break;
        case G_MR1:
            LayRegOp( back, left );
            LayModRM( back, result );
            break;
        case G_LEA:
            LayRegOp( back, result );
            if( left->n.class_ == N_REGISTER ) {  /* add/sub transformed to LEA */
                LayLeaRegOp( back, ins );
            } else {
                LayModRM( back, left );
            }
            break;
        case G_LDSES:
            LayRegOp( back, result );
            LayModRM( back, left );
            if( HW_COvlap( result->r.reg, HW_SS ) ) {
                AddToTemp( back, M_SECONDARY );   /* load SS */
                break;
            }
            if( HW_COvlap( result->r.reg, HW_FS_GS ) ) {
                back.pcencode.Inst[KEY] += B_KEY_FSGS;    /* load FS or GS */
                AddToTemp( back, M_SECONDARY );
            } else {
                back.pcencode.Inst[KEY] += B_KEY_DSES;    /* load DS or ES */
            }
            if( HW_COvlap( result->r.reg, HW_DS_GS ) ) {
                back.pcencode.Inst[KEY] += 1;             /* indicate load to DS and GS */
            }
            break;
        case G_MS1:
            LaySROp( back, left );
            LayModRM( back, result );
            break;
        case G_SM1:
            LaySROp( back, result );
            LayModRM( back, left );
            break;
        case G_RS:
            LaySROp( back, left );
            LayRMRegOp( back, result );
            break;
        case G_SR:
            LaySROp( back, result );
            LayRMRegOp( back, left );
            break;
        case G_MOVMC:
            LayW( back, ins->type_class );
            LayModRM( back, result );
            AddWCons( back, left, ins->type_class );
            break;
        case G_MADDR:
            LayModRM( back, result );
            DoMAddr( back, left );
            break;
        case G_TEST:
            LayW( back, ins->type_class );
            LayRegOp( back, left );
            LayRMRegOp( back, left );
            break;
        case G_SEGR1:
            {
                uint    ins_loc;
                byte    extra_bits;
                reg_idx reg_seg;

                ins_loc = KEY;
                if( ins->head.opcode == OP_POP ) {
                    extra_bits = B_KEY_POPSEG;
                    reg_seg = SegTrans( back, result->r.reg );
                } else {
                    extra_bits = 0;
                    reg_seg = SegTrans( back, left->r.reg );
                }
                if( reg_seg > 3 ) {
                    ins_loc += 1;
                    back.pcencode.Inst[KEY] = 0x0f;
                    AddByte( back, 0x80 );
                }
                back.pcencode.Inst[ins_loc] |= extra_bits;
                back.pcencode.Inst[ins_loc] |= reg_seg << S_KEY_SR;
            }
            break;
        case G_MOVAM:
            LayW( back, ins->type_class );
            DoMDisp( back, left, false );
            break;
        case G_MOVMA:
            LayW( back, ins->type_class );
            DoMDisp( back, result, false );
            break;
        case G_MOVRC:
            if( ins->type_class == U2 || ins->type_class == I2
             || ins->type_class == U4 || ins->type_class == I4
             || ins->type_class == FS ) {
                back.pcencode.Inst[KEY] |= B_KEY_AW;
            }
            LayACRegOp( back, result );
            AddWCons( back, left, ins->type_class );
            break;
        case G_RADDR:
            LayACRegOp( back, result );
            DoMAddr( back, left );
            break;
        case G_REPOP:
            DoRepOp( back, ins );
            break;
        case G_LOADADDR:
            if( AskIsFrameIndex( back, left ) ) {
                GenUnkLea( back, NextFramePatch(back) );
                break;
            }
#if _TARGET & _TARG_80386
            if( ( left->n.class_ == N_TEMP ) && TmpLoc( back, DeAlias( left ), left ) == 0 ) {
                /*
                 * turn "LEA <reg>,[ESP+0]" into "MOV <reg>,ESP"
                 * or "LEA <reg>,[EBP+0]" into "MOV <reg>,EBP"
                 */
                LayOpword( back, M_MOVRR | B_KEY_W );
                if( BaseIsSP( back, left ) ) {
                    LayReg( back, HW_xSP );
                } else {
                    LayReg( back, HW_xBP );
                }
                LayRMRegOp( back, result );
                break;
            }
#endif
            LayOpword( back, M_LEA );
            LayRegOp( back, result );
            LayModRM( back, left );
            break;
        case G_4SHIFT:
            Do4Shift( back, ins );
            break;
        case G_4RSHIFT:
            Do4RShift( back, ins );
            break;
        case G_CXSHIFT:
            Do4CXShift( back, ins, &Do4Shift );
            break;
        case G_RCXSHIFT:
            Do4CXShift( back, ins, &Do4RShift );
            break;
        case G_4NEG:
            Gen4Neg( back, ins );
            break;
        case G_4RNEG:
            Gen4RNeg( back, ins );
            break;
        case G_ENTER:
            /*
             * do nothing, prolog should already be generated
             */
            break;
        case G_CALL:
            GenCall( back, ins );
            AdjustStackDepth( back, ins );
            if( _IsTargetModel( CGSW_X86_NEW_P5_PROFILING ) ) {
                label_handle    lbl;
                segment_id      segid;

                segid = GenProfileData( back, const_cast<char *>(""), &lbl, &back.data.CurrProc->targ.routine_profile_data );
                _Code;
                LayOpword( back, 0xc4f7 );                    /* test [e]sp, offset L1  */
                back.pcencode.ILen += WORD_SIZE;                      /* ..                     */
                DoLblRef( back, lbl, segid, 0, ESCA_OFST );   /* ..                     */
                _Emit;
            }
            return;
        case G_CALLI:
            if( ins->operands[CALL_OP_ADDR]->n.class_ == N_REGISTER ) {
                GenCallRegister( back, ins );
            } else {
                GenCallIndirect( back, ins );
            }
            AdjustStackDepth( back, ins );
            if( _IsTargetModel( CGSW_X86_NEW_P5_PROFILING ) ) {
                label_handle    lbl;
                label_handle    junk;
                segment_id      segid;
                char            c[2];

                segid = GenProfileData( back, const_cast<char *>(""), &lbl, &back.data.CurrProc->targ.routine_profile_data );
                GenProfileData( back, const_cast<char *>(""), &junk, &back.data.CurrProc->targ.routine_profile_data );
                c[0] = PROFILE_FLAG_END_GROUP;
                c[1] = 0;
                GenProfileData( back, c, &junk, &back.data.CurrProc->targ.routine_profile_data );
                _Code;
                LayOpword( back, 0xc5f7 );                    /* test [e]bp, offset L1  */
                back.pcencode.ILen += WORD_SIZE;                      /* ..                     */
                DoLblRef( back, lbl, segid, 0, ESCA_OFST );   /* ..                     */
                _Emit;
            }
            return;
        case G_JMPR:
            GenJmpRegister( back, ins );
            AdjustStackDepth( back, ins );
            return;
        case G_JMPM:
            GenJmpMemory( back, ins );
            break;
        case G_SIGNEX:
            switch( ins->type_class ) {
#if _TARGET & _TARG_80386
            case U8:
            case I8:
                LayOpbyte( back, M_CWD );
                if( _IsntTargetModel( CGSW_X86_USE_32 ) )
                    AddToTemp( back, M_OPND_SIZE );
                break;
#endif
            case U4:
            case I4:
#if _TARGET & _TARG_8086
                LayOpbyte( back, M_CWD );
#elif _TARGET & _TARG_80386
                switch( ins->base_type_class ) {
                case U1:
                case I1:
                    if( _IsTargetModel( CGSW_X86_USE_32 ) )
                        AddToTemp( back, M_OPND_SIZE );
                    AddToTemp( back, M_CBW );
                    break;
                default:
                    break;
                }
                if( HW_CEqual( ins->result->r.reg, HW_DX_AX ) ) {
                    LayOpbyte( back, M_CWD );
                    if( _IsTargetModel( CGSW_X86_USE_32 ) ) {
                        AddToTemp( back, M_OPND_SIZE );
                    }
                } else {
                    LayOpbyte( back, M_CBW );
                    if( _IsntTargetModel( CGSW_X86_USE_32 ) ) {
                        AddToTemp( back, M_OPND_SIZE ); /* CWDE */
                    }
                }
#endif
                break;
            case U2:
            case I2:
                LayOpbyte( back, M_CBW );
                if( _IsTargetModel( CGSW_X86_USE_32 ) )
                    AddToTemp( back, M_OPND_SIZE );
                break;
            default:
                break;
            }
            break;
        case G_C1:
            AddSCons( back, left, ins->type_class );
            break;
        case G_186RMUL:
            LayOpword( back, 0xc069 );
            LayRegOp( back, result );
            LayRMRegOp( back, left );
            AddSCons( back, right, ins->type_class );
            break;
        case G_186MUL:
            LayOpword( back, 0x0069 );
            LayRegOp( back, result );
            LayModRM( back, left );
            AddSCons( back, right, ins->type_class );
            break;
        case G_MRFBIN:
        case G_MNFBIN:
        case G_MFLD:
            LayMF( back, left );
            LayModRM( back, left );
            break;
        case G_RFLD:
        case G_RRFBIN:
        case G_RRFBINP:
        case G_RRFBIND:
        case G_RNFBIN:
        case G_RNFBINP:
        case G_RNFBIND:
            LayST( back, left );
            break;
        case G_RFST:
        case G_RFSTNP:
            LayST( back, result );
            break;
        case G_MFST:
        case G_MFSTNP:
        case G_MFSTRND:
            LayMF( back, result );
            LayModRM( back, result );
            break;
        case G_FCHS:
        case G_FLD1:
        case G_FLDZ:
        case G_FINIT:
            break;
        case G_IFUNC:
            CallMathFunc( back, ins );
            break;
        case G_FMATH:
            MathFunc( back, ins );
            break;
        case G_FSINCOS:
            GCondFwait(back);
            LayOpword( back, 0xFBD9 );
            break;
        case G_FCHOP:
            DoCall( back, RTLabel( back, RT_CHOP ), true, _IsTargetModel( CGSW_X86_BIG_CODE ), false );
            break;
        case G_FTST:
        case G_FCOMPP:
            back.i87data.Used87 = true;
            SetCC(back);
            break;
        case G_FWAIT:
            back.i87data.Used87 = true;
            if( _IsEmulation() ) {
                SetFPPatchType( back, FPP_WAIT );
                LayOpword( back, 0x9b90 );
            } else {
                LayOpbyte( back, 0x9b );
            }
            break;
        case G_FXCH:
            back.i87data.Used87 = true;
            GCondFwait(back);
            LayOpword( back, 0xC8D9 );    /* fxch st(1) */
            LayST( back, result );
            break;
        case G_MCOMP:
            LayMF( back, left );
            LayModRM( back, left );
            SetCC(back);
            break;
        case G_RCOMP:
            LayST( back, left );
            SetCC(back);
            break;
        case G_DEBUG:
            EmitDbgInfo( back, ins );
            break;
        case G_SEG_SEG:
            PushSeg( back, left->r.reg );
            _Next;
            PopSeg( back, result->r.reg );
            break;
        case G_POW2DIV:
            Pow2Div( back, ins );
            break;
        case G_DIV2:
            By2Div( back, ins );
            break;
#if _TARGET & _TARG_8086
        case G_POW2DIV_286:
            Pow2Div286( back, ins );
            break;
#endif
        case G_MOVSX:
        case G_MOVZX:
            if( gen == G_MOVSX ) {
                LayOpword( back, M_MOVSX );
            } else {
                LayOpword( back, M_MOVZX );
            }
#if _TARGET & _TARG_80386
            if( ins->operands[0]->n.size == 2 ) {
                back.pcencode.Inst[KEY] |= B_KEY_W;
            } else { /* base is byte */
                switch( ins->type_class ) {
                case U2:
                case I2:
                    if( _IsTargetModel( CGSW_X86_USE_32 ) )
                        AddToTemp( back, M_OPND_SIZE );
                    break;
                case U4:
                case I4:
                    if( _IsntTargetModel( CGSW_X86_USE_32 ) )
                        AddToTemp( back, M_OPND_SIZE );
                    break;
                default:
                    break;
                }
            }
#endif
            LayModRM( back, left );
            LayRegOp( back, result );
            AddToTemp( back, M_SECONDARY );
            break;
        case G_UNKNOWN:
            _Zoiks( ZOIKS_097 );
            break;
        default:
            _Zoiks( ZOIKS_028 );
            break;
        }
        _Emit;
        if( gen == G_MFSTRND ) {
#if ( _TARGET & _TARG_8086 )
            AdjustStackDepthDirect( back, -2 * WORD_SIZE );
#else
            AdjustStackDepthDirect( back, -WORD_SIZE );
#endif

            _Code;
#if ( _TARGET & _TARG_8086 )
            AddByte(back, 0x55);      /*  push    bp              */
            AddByte(back, 0x89);      /*  mov     bp,sp           */
            AddByte(back, 0xe5);      /*  ..                      */
            AddByte(back, 0xd9);      /*  fldcw   word ptr [bp+4] */
            AddByte(back, 0x6e);      /*  ..                      */
            AddByte(back, 0x04);      /*  ..                      */
            AddByte(back, 0x5d);      /*  pop     bp              */
            AddByte(back, 0x83);      /*  add     sp,4            */
            AddByte(back, 0xc4);      /*  ..                      */
            AddByte(back, 0x04);      /*  ..                      */
#else
            AddByte(back, 0xd9);      /*  fldcw   word ptr [esp+2]*/
            AddByte(back, 0x6c);      /*  ..                      */
            AddByte(back, 0x24);      /*  ..                      */
            AddByte(back, 0x02);      /*  ..                      */
            AddByte(back, 0x8d);      /*  lea     esp,[esp+4]     */
            AddByte(back, 0x64);      /*  ..                      */
            AddByte(back, 0x24);      /*  ..                      */
            AddByte(back, 0x04);      /*  ..                      */
#endif
            _Emit;
        }
    }
    AdjustStackDepth( back, ins );
    if( _OpIsCondition( ins->head.opcode ) ) {
        EndBlockProfiling();
        GenCondJump( back, ins );
    }
}
