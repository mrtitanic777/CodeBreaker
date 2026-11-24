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
* Description:  Intel instruction encoding, part II. Processes labels,
*               jumps and the like.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "optmain.h"
#include "cgmem.h"
#include "system.h"
#include "x86objd.h"
#include "pcencode.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "seldef.h"
#include "pccode.h"
#include "encode.h"
#include "data.h"
#include "types.h"
#include "utils.h"
#include "objout.h"
#include "object.h"
#include "intrface.h"
#include "targetin.h"
#include "targetdb.h"
#include "opttell.h"
#include "x87.h"
#include "x86esc.h"
#include "rgtbl.h"
#include "split.h"
#include "namelist.h"
#include "i64.h"
#include "x86enc.h"
#include "x86enc2.h"
#include "feprotos.h"
#include "backrun.hpp"




static const byte UCondTable[] = {
/***************************
 * The 8086 code for an unsigned jmp
 */
    5,              /* OP_BIT_TEST_TRUE */
    4,              /* OP_BIT_TEST_FALSE */
    4,              /* OP_CMP_EQUAL */
    5,              /* OP_CMP_NOT_EQUAL */
    7,              /* OP_CMP_GREATER */
    6,              /* OP_CMP_LESS_EQUAL */
    2,              /* OP_CMP_LESS */
    3               /* OP_CMP_GREATER_EQUAL */
};

static const byte SCondTable[] = {
/***************************
 * The 8086 code for a signed jmp
 */
    5,              /* OP_BIT_TEST_TRUE */
    4,              /* OP_BIT_TEST_FALSE */
    4,              /* OP_CMP_EQUAL */
    5,              /* OP_CMP_NOT_EQUAL */
    15,             /* OP_CMP_GREATER */
    14,             /* OP_CMP_LESS_EQUAL */
    12,             /* OP_CMP_LESS */
    13              /* OP_CMP_GREATER_EQUAL */
};

static const byte rev_condition[] = {
/******************************
 * Reverse the sense of an 8086 jmp (ie: ja -> jbe)
 * i.e. XOR 1
 */
    1,0,3,2,5,4,7,6,9,8,11,10,13,12,15,14
};

static const issigned signed_type[] = {
/********************************
 * What kind of a jump does the instruction need following it
 */
    UNSIGNED,       /* U1*/
    SIGNED_BOTH,    /* I1*/
    UNSIGNED,       /* U2*/
    SIGNED_BOTH,    /* I2*/
    UNSIGNED,       /* U4*/
    SIGNED_BOTH,    /* I4*/
    UNSIGNED,       /* U8*/
    SIGNED_BOTH,    /* I8*/
    UNSIGNED,       /* CP*/
    UNSIGNED,       /* PT*/
    SIGNED_86,      /* FS*/
    SIGNED_86,      /* FD*/
    SIGNED_86,      /* FL*/
    UNSIGNED        /* XX*/
};
namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct X86enc2State: x86enc2_state.h) */

}  // namespace



unsigned DepthAlign( BackRun& back, unsigned depth )
/***********************************/
{

    if( back.x86enc2.DepthAlign_AlignArray[0] == 0 || depth == PROC_ALIGN ) {
        unsigned char *align_info_bytes = static_cast<unsigned char *>(FEAuxInfo( *back.front, NULL, FEINF_CODE_LABEL_ALIGNMENT ));
        memcpy( back.x86enc2.DepthAlign_AlignArray, align_info_bytes, align_info_bytes[0] + 1 );
    }
    if( back.data.OptForSize > 0 )
        return( 1 );
    if( CPULevel( back, CPU_486 ) ) {
        if( depth == PROC_ALIGN || depth == DEEP_LOOP_ALIGN )
            return( 16 );
        return( 1 );
    }
    if( CPULevel( back, CPU_386 ) ) {
        if( depth == PROC_ALIGN || depth == DEEP_LOOP_ALIGN )
            return( 4 );
        return( 1 );
    }
    if( depth == PROC_ALIGN || depth == DEEP_LOOP_ALIGN ) {
        return( back.x86enc2.DepthAlign_AlignArray[1] );
    }
    if( depth == 0 )
        depth = 1;
    if( depth >= back.x86enc2.DepthAlign_AlignArray[0] ) {
        depth = back.x86enc2.DepthAlign_AlignArray[0] - 1;
    }
    return( back.x86enc2.DepthAlign_AlignArray[depth + 1] );
}

byte    CondCode( BackRun& back, instruction *cond )
/************************************
 * Return the condition code number for the encoding, associated with "cond"
 */
{
    issigned        is_signed;

    if( FPULevel( back, FPU_87 ) ) {
        is_signed = SIGNED_87;
    } else {
        is_signed = SIGNED_86;
    }
    if( is_signed & signed_type[cond->type_class] ) {
        return( SCondTable[cond->head.opcode - FIRST_CONDITION] );
    } else {
        return( UCondTable[cond->head.opcode - FIRST_CONDITION] );
    }
}

void    GenSetCC( BackRun& back, instruction *cond )
/************************************
 * Given a conditional "cond", generate the correct setxx instruction
 */
{
    Format( back, OC_CODE );
    LayOpword( back, M_SETCC | CondCode( back, cond ) );
    if( cond->result->n.class_ == N_REGISTER ) {
        LayRMRegOp( back, cond->result );
    } else {
        LayModRM( back, cond->result );
    }
    AddToTemp( back, M_SECONDARY );
    Finalize( back );
}

byte    ReverseCondition( byte cond )
/************************************
 * Reverse the sense of a conditional jump (already encoded)
 */
{
    return( rev_condition[cond] );
}

void    DoCall( BackRun& back, label_handle lbl, bool imported, bool big, bool pop )
/********************************************************************
 * Call routine "lbl".
 */
{
    oc_class        occlass;
    oc_dest_attr    destattr;

    /* unused parameters */ (void)imported;

    occlass = OC_CALL;
    if( pop )
        occlass |= OC_ATTR_POP;
    if( big )
        occlass |= OC_ATTR_FAR;
    if( !big ) {
        destattr = OC_DEST_NEAR;
    } else if( AskIfRTLabel( back, lbl )
            || imported //NYI:multi-code-segment, this can go when FORTRAN is fixed up
            || AskCodeSeg(back) != FESegID( *back.front, AskForLblSym( back, lbl ) ) ) {
        destattr = OC_DEST_FAR;
    } else {
        destattr = OC_DEST_CHEAP;
    }
    CodeHandle( back, occlass, OptInsSize( back, occlass, destattr ), lbl );
}

static  void    CodeSequence( BackRun& back, const byte *p, byte_seq_len len )
/**************************************************************
 * Dump an inline sequence, taking into account the floating fixups and
 * the "seg foo", "offset foo" sequences.
 */
{
    bool            first;
    const byte      *endp;
    const byte      *startp;
    byte            type;
    cg_sym_handle   sym = 0;
    offset          off = 0;
    fe_attr         attr = static_cast<fe_attr>(0);
    name            *temp;

    first = false;
    endp = p + len;
    while( p < endp ) {
        Format( back, OC_CODE );
        startp = p;
        for( ; p < endp && ( p - startp ) < ( INSSIZE - 5 ); ) {
            if( p[0] == FLOATING_FIXUP_BYTE ) {
                type = p[1];
                switch( type ) {
                case FLOATING_FIXUP_BYTE:
                    ++p;
                    break;
                case FIX_SYM_OFFSET:
                case FIX_SYM_SEGMENT:
                case FIX_SYM_RELOFF:
                    p += 2;
                    {
                        BYTE_SEQ_SYM    tmp;

                        memcpy( &tmp, p, sizeof( BYTE_SEQ_SYM ) );
                        p += sizeof( BYTE_SEQ_SYM );
                        sym = static_cast<cg_sym_handle>(tmp);
                    }
                    {
                        BYTE_SEQ_OFF    tmp;

                        memcpy( &tmp, p, sizeof( BYTE_SEQ_OFF ) );
                        p += sizeof( BYTE_SEQ_OFF );
                        off = tmp;
                    }
                    attr = FEAttr( *back.front, sym );
                    switch( type ) {
                    case FIX_SYM_SEGMENT:
                        back.pcencode.ILen += 2;
                        if( attr & (FE_STATIC | FE_GLOBAL) ) {
                            DoFESymRef( back, sym, CG_FE, off, FE_FIX_BASE );
                        } else {
                            FEMessage( *back.front, FEMSG_ERROR, const_cast<pointer>(static_cast<const void *>("aux seg used with local symbol")) );
                        }
                        break;
                    case FIX_SYM_OFFSET:
                        back.pcencode.ILen += WORD_SIZE;
                        if( attr & (FE_STATIC | FE_GLOBAL) ) {
                            DoFESymRef( back, sym, CG_FE, off, FE_FIX_OFF );
                        } else {
                            temp = DeAlias( AllocUserTemp( back, sym, U1 ) );
                            if( temp->t.location != NO_LOCATION ) {
                                EmitOffset( back, NewBase( back, temp ) - temp->v.offset + off );
                            } else {
                                FEMessage( *back.front, FEMSG_ERROR, const_cast<pointer>(static_cast<const void *>("aux offset used with register symbol")) );
                            }
                        }
                        break;
                    case FIX_SYM_RELOFF:
                        back.pcencode.ILen += WORD_SIZE;
                        if( attr & FE_PROC ) {
                            DoFESymRef( back, sym, CG_FE, off, FE_FIX_SELF );
                        } else {
                            FEMessage( *back.front, FEMSG_ERROR, const_cast<pointer>(static_cast<const void *>("aux reloff used with data symbol")) );
                        }
                        break;
                    }
                    continue;
                default:
                    // floating point fixups
                    if( !first ) {
                        // ensure previous instructions be emited and
                        // start new FPU instruction
                        startp = p - INSSIZE;
                        first = true;
                        continue;
                    }
                    p += 2;
                    if( IsEmulation( back ) ) {
                        SetFPPatchType( back, static_cast<fp_patches>(type) );
                        back.i87data.Used87 = true;
                    }
                    break;
                }
            }
            if( first ) {
                LayOpbyte( back, *p );
                first = false;
            } else {
                AddByte( back, *p );
            }
            ++p;
        }
        Finalize( back );
    }
}


static  void    GenNoReturn( BackRun& back )
/**********************************
 * Generate a noreturn instruction (pseudo instruction)
 */
{
    any_oc          oc;

    oc.oc_entry.hdr.class_ = OC_NORET;
    oc.oc_entry.hdr.reclen = offsetof( oc_entry, data );
    oc.oc_entry.hdr.objlen = 0;
    InputOC( back, &oc );
}

static  void    Pushf( BackRun& back )
/***************************/
{
    LayOpbyte( back, 0x9c ); /* PUSHF*/
    Finalize( back );
    Format( back, OC_CODE );
}


void    GenCall( BackRun& back, instruction *ins )
/**********************************
 * Generate a call for "ins". (eg: call foo, or call far ptr foo)
 */
{
    name            *op;
    cg_sym_handle   sym;
    bool            imp;
    call_class      cclass;
    const byte_seq  *code;
    label_handle    lbl;
    bool            far_call;

    if( ins->flags.u.call_flags & CALL_INTERRUPT ) {
        Pushf(back);
    }
    op = ins->operands[CALL_OP_ADDR];
    cclass = static_cast<call_class>(reinterpret_cast<pointer_uint>(FindAuxInfo( back, op, FEINF_CALL_CLASS )));
    far_call = ( (static_cast<call_class_target>(reinterpret_cast<pointer_uint>(FindAuxInfo( back, op, FEINF_CALL_CLASS_TARGET ))) & FECALL_X86_FAR_CALL) != 0 );
    code = static_cast<const byte_seq *>(FindAuxInfo( back, op, FEINF_CALL_BYTES ));
    if( code != NULL ) {
        Finalize( back );
        if( code->relocs ) {
            CodeSequence( back, code->data, code->length );
        } else {
            CodeBytes( back, code->data, code->length );
        }
    } else if( (cclass & FECALL_GEN_ABORTS)
      && IsntTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) ) {
        sym = static_cast<cg_sym_handle>(op->v.symbol);
        lbl = FEBack( *back.front, sym )->lbl;
        if( far_call && (FEAttr( *back.front, sym ) & FE_IMPORT) ) {
            CodeHandle( back, OC_JMP | OC_ATTR_FAR, OptInsSize( back, OC_JMP, OC_DEST_FAR ), lbl );
        } else {
            CodeHandle( back, OC_JMP, OptInsSize( back, OC_JMP, OC_DEST_NEAR ), lbl );
        }
        return;
    } else {
        sym = static_cast<cg_sym_handle>(op->v.symbol);
        if( op->m.memory_type == CG_FE ) {
            lbl = FEBack( *back.front, sym )->lbl;
            imp = ( (FEAttr( *back.front, sym ) & (FE_COMMON | FE_IMPORT)) != 0 );
        } else {
            /*
             * handles mismatch Fix it!
             */
            lbl = reinterpret_cast<label_handle>(sym);
            imp = true;
        }
        DoCall( back, lbl, imp, far_call, (ins->flags.u.call_flags & CALL_POPS_PARMS) != 0 );
    }
    if( (cclass & (FECALL_GEN_ABORTS | FECALL_GEN_NORETURN))
      && IsntTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) ) {
        GenNoReturn(back);
    }
}


void    GenCallIndirect( BackRun& back, instruction *ins )
/******************************************
 * Generate an indirect call for "ins" (eg: call dword ptr [eax])
 */
{
    oc_class        occlass;
    gen_opcode      opcode;
    name            *op;

    if( ins->flags.u.call_flags & CALL_INTERRUPT ) {
        Pushf(back);
    }
    if( (ins->flags.u.call_flags & CALL_ABORTS)
      && IsntTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) ) {
        occlass = OC_JMPI;
    } else {
        occlass = OC_CALLI;
    }
    if( ins->flags.u.call_flags & CALL_POPS_PARMS ) {
        occlass |= OC_ATTR_POP;
    }
    op = ins->operands[CALL_OP_ADDR];
    opcode = M_CJINEAR;
    if( op->n.type_class == PT
      || op->n.type_class == CP ) {
        opcode = M_CJILONG;
        occlass |= OC_ATTR_FAR;
    }
    ReFormat( back, occlass );
    LayOpword( back, opcode );
    LayModRM( back, op );
    Finalize( back );
    if( (ins->flags.u.call_flags & CALL_NORETURN)
      && IsntTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) ) {
        GenNoReturn(back);
    }
}


void    GenCallRegister( BackRun& back, instruction *ins )
/******************************************
 * Generate a call to a register (eg: call eax)
 */
{
    name            *op;
    oc_class        occlass;

    if( ins->flags.u.call_flags & CALL_INTERRUPT ) {
        Pushf(back);
    }
    op = ins->operands[CALL_OP_ADDR];
    occlass = OC_CALLI;
    if( ins->flags.u.call_flags & CALL_POPS_PARMS ) {
        occlass |= OC_ATTR_POP;
    }
    ReFormat( back, occlass );
    LayOpword( back, M_CJINEAR );
    LayRegRM( back, op->r.reg );
    Finalize( back );
    if( (ins->flags.u.call_flags & CALL_NORETURN)
      && IsntTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) ) {
        GenNoReturn(back);
    }
}


void    GenSelEntry( BackRun& back, bool starts )
/*********************************
 * Dump a queue that a select table is starting/ending ("starts") into
 * the code segment queue.
 */
{
    any_oc          oc;

    oc.oc_select.hdr.class_ = static_cast<oc_class>(OC_INFO + OC_INFO_SELECT);
    oc.oc_select.hdr.reclen = sizeof( oc_select );
    oc.oc_select.hdr.objlen = 0;
    oc.oc_select.starts = starts;
    InputOC( back, &oc );
}


void    Gen1ByteValue( BackRun& back, byte value )
/**********************************
 * Drop an 8 bit integer into the queue.
 */
{
    Format( back, OC_CODE );
    AddByte( back, value );
    Finalize( back );
}


void    Gen2ByteValue( BackRun& back, uint_16 value )
/*************************************
 * Drop a 16 bit integer into the queue.
 */
{
    Format( back, OC_CODE );
    AddByte( back, value & 0xFF );
    AddByte( back, ( value >> 8 ) & 0xFF );
    Finalize( back );
}


void    Gen4ByteValue( BackRun& back, uint_32 value )
/*************************************
 * Drop a 32 bit integer into the queue.
 */
{
    Format( back, OC_CODE );
    AddByte( back, value & 0xFF );
    AddByte( back, ( value >> 8 ) & 0xFF );
    AddByte( back, ( value >> 16 ) & 0xFF );
    AddByte( back, ( value >> 24 ) & 0xFF );
    Finalize( back );
}


void    Gen8ByteValue( BackRun& back, unsigned_64 *value )
/*****************************************
 * Drop a 64 bit integer into the queue.
 */
{
    Format( back, OC_CODE );
    AddByte( back, U64Byte( *value, I64B0 ) );
    AddByte( back, U64Byte( *value, I64B1 ) );
    AddByte( back, U64Byte( *value, I64B2 ) );
    AddByte( back, U64Byte( *value, I64B3 ) );
    AddByte( back, U64Byte( *value, I64B4 ) );
    AddByte( back, U64Byte( *value, I64B5 ) );
    AddByte( back, U64Byte( *value, I64B6 ) );
    AddByte( back, U64Byte( *value, I64B7 ) );
    Finalize( back );
}


void    GenCodePtr( BackRun& back, pointer label )
/**********************************
 * Dump a near reference to a label into the code segment.
 */
{
    CodeHandle( back, OC_LREF, TypeAddress( back, TY_NEAR_CODE_PTR )->length, static_cast<label_handle>(label) );
}


void    GenCallLabel( BackRun& back, pointer label )
/************************************
 * Generate a call to a label within the procedure (near call)
 */
{
    DoCall( back, static_cast<label_handle>(label), false, false, false );
}


void    GenLabelReturn( BackRun& back )
/*****************************
 * Generate a return from CALL_LABEL instruction (near return)
 */
{
    GenReturn( back, 0, false );
}

void    GenReturn( BackRun& back, int pop, bool is_long )
/*****************************************
 * Generate a return instruction
 */
{
    any_oc          oc;

    oc.oc_ret.hdr.class_ = OC_RET;
    oc.oc_ret.hdr.reclen = sizeof( oc_ret );
    oc.oc_ret.hdr.objlen = 1;
    if( pop != 0 ) {
        oc.oc_ret.hdr.class_ |= OC_ATTR_POP;
        oc.oc_ret.hdr.objlen += 2;
    }
    if( is_long ) {
        oc.oc_ret.hdr.class_ |= OC_ATTR_FAR;
    }
    oc.oc_ret.ref = NULL;
    oc.oc_ret.pops = pop;
    InputOC( back, &oc );
}

void    GenIRET( BackRun& back )
/**********************
 * Generate a IRET instruction
 */
{
    any_oc          oc;

    oc.oc_ret.hdr.class_ = OC_RET | OC_ATTR_IRET;
    oc.oc_ret.hdr.reclen = sizeof( oc_ret );
    oc.oc_ret.hdr.objlen = 1;
    oc.oc_ret.ref = NULL;
    oc.oc_ret.pops = 0;
    InputOC( back, &oc );
}

void    GenJmpMemory( BackRun& back, instruction *ins )
/***************************************
 * Generate a jump indirect through memory instruction.
 */
{
    label_handle    lbl;
    name            *base;

    if( ins->head.opcode != OP_SELECT
      && IsTargetModel( back, CGSW_X86_BIG_CODE ) ) {
        ReFormat( back, OC_JMPI | OC_ATTR_FAR );
        LayOpword( back, M_CJILONG );
    } else {
        ReFormat( back, OC_JMPI );
        LayOpword( back, M_CJINEAR );
    }
    LayModRM( back, ins->operands[0] );
    if( ins->head.opcode == OP_SELECT &&
        ins->operands[0]->n.class_ == N_INDEXED ) {
        base = ins->operands[0]->i.base;
        if( base != NULL ) {
            lbl = AskForSymLabel( back, base->v.symbol, CG_TBL );
            if( AskAddress( back, lbl ) != ADDR_UNKNOWN ) {
                TellScrapLabel( back, lbl );
            }
        }
    }
}

static  void    JumpReg( BackRun& back, instruction *ins, name *reg_name )
/**********************************************************
 * Generate a jump to register instruction (eg: jmp eax)
 */
{
    hw_reg_set      regs;

    /* unused parameters */ (void)ins;

    regs = reg_name->r.reg;
    if( reg_name->n.size > WORD_SIZE ) {
        /* fake up intersegment jump*/
        Format( back, OC_CODE );
        LayOpbyte( back, M_PUSH );
        LayRegAC( back, High32Reg( regs ) );
        EjectInst( back );
        LayOpbyte( back, M_PUSH );
        LayRegAC( back, Low32Reg( regs ) );
        Finalize( back );
        GenReturn( back, 0, true );
    } else {
        ReFormat( back, OC_JMPI );
        LayOpword( back, M_CJINEAR );
        LayRegRM( back, regs );
        Finalize( back );
    }
}

void    GenJmpRegister( BackRun& back, instruction *ins )
/*****************************************
 * Generate a jump to register instruction (eg: jmp eax)
 */
{
    JumpReg( back, ins, ins->operands[0] );
}


static  void    DoCodeBytes( BackRun& back, const void *src, byte_seq_len len, oc_class occlass )
/*********************************************************************************
 * Dump bytes "src" directly into the queue, for length "len".
 */
{
    any_oc          *oc;
    uint            addlen;

    oc = static_cast<any_oc *>(CGAlloc( back, offsetof( oc_entry, data ) + MAX_OBJ_LEN ));
    oc->oc_entry.hdr.class_ = occlass;
    oc->oc_entry.hdr.reclen = offsetof( oc_entry, data ) + MAX_OBJ_LEN;
    oc->oc_entry.hdr.objlen = MAX_OBJ_LEN;
    addlen = 0;
    if( occlass == OC_IDATA ) {
        if( len > 255 )
            len = 255;
        addlen = 1;
    }
    while( len + addlen > MAX_OBJ_LEN ) {
        memcpy( oc->oc_entry.data, src, MAX_OBJ_LEN );
        InputOC( back, oc );
        src = static_cast<const char *>(src) + MAX_OBJ_LEN;
        len -= MAX_OBJ_LEN;
    }
    oc->oc_entry.hdr.reclen = offsetof( oc_entry, data ) + len + addlen;
    oc->oc_entry.hdr.objlen = len + addlen;
    memcpy( oc->oc_entry.data, src, len );
    if( addlen )
        oc->oc_entry.data[len] = static_cast<byte>(len);
    InputOC( back, oc );
    CGFree( back, oc );
}

void    CodeBytes( BackRun& back, const void *src, byte_seq_len len )
/****************************************************/
{
    DoCodeBytes( back, src, len, OC_BDATA );
}

void    EyeCatchBytes( BackRun& back, const void *src, byte_seq_len len )
/********************************************************/
{
    DoCodeBytes( back, src, len, OC_IDATA );
}
