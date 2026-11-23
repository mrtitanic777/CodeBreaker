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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


/* set this macro to 0 if you do NOT want to allow structures to be defined
 * ie:
 *    struct foo
 *      ...
 *    foo ends
 *    ...
 *    bar foo < 4, 2, 3 >
 */

#include "asmglob.h"
#include "hash.h"
#include "asmfixup.h"
#include "asmeval.h"
#include "asmlabel.h"
#include "asmjump.h"
#include "asminsd.h"
#include "asmopsd.gh"

#include "asmdefs.h"
#include "asmrun.hpp"


extern bool             ptr_operator( AsmRun& assembler, memtype, bool );


static void             SizeString( AsmRun& assembler, unsigned op_size );
static bool             check_size( AsmRun& assembler );
static bool             segm_override_jumps( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx );





extern void             make_inst_hash_table( void );


static bool             mem2code( AsmRun& assembler, unsigned char, asm_token, asm_token, asm_sym_handle sym );


const asm_ins ASMI86FAR *get_instruction( const char *string )
/************************************************************/
{
    asm_token   token;

    for( token = static_cast<asm_token>(inst_table[hashpjw( string )]); token--; token = static_cast<asm_token>(AsmOpcode[token].next) ) {
        if( strnicmp( string, AsmChars + AsmOpcode[token].index, AsmOpcode[token].len ) == 0
            && string[AsmOpcode[token].len] == '\0' ) {
            return( AsmOpTable + AsmOpcode[token].position );
        }
    }
    return( NULL );
}

static bool comp_mem( AsmRun& assembler, asm_token reg1, asm_token reg2, unsigned char *rm_field )
/*****************************************************************************/
/*
- compare and return the r/m field encoding of 16-bit address mode;
- call by mem2code() only;
*/
{
    switch( reg1 ) {
    case T_BX:
        switch( reg2 ) {
        case T_SI:
            *rm_field = MEM_BX_SI;
            return( RC_OK );
        case T_DI:
            *rm_field = MEM_BX_DI;
            return( RC_OK );
        }
        break;
    case T_BP:
        switch( reg2 ) {
        case T_SI:
            *rm_field = MEM_BP_SI;
            return( RC_OK );
        case T_DI:
            *rm_field = MEM_BP_DI;
            return( RC_OK );
        }
        break;
    }
    AsmError( *assembler.front, INVALID_MEMORY_POINTER );
    return( RC_ERROR );
}

static void seg_override( AsmRun& assembler, asm_token seg_reg, asm_sym_handle sym )
/***************************************************************/
/*
- determine if segment override is necessary with the current address mode;
*/
{
    prefix_reg      default_seg;
    /* unused parameters */ (void)sym;

    if( assembler.asmdefs.Code->prefix.seg != PREFIX_EMPTY ) {
        switch( seg_reg ) {
        case T_SS:
        case T_BP:
        case T_EBP:
        case T_ESP:
            default_seg = PREFIX_SS;
            break;
        default:
            default_seg = PREFIX_DS;
        }
        if( assembler.asmdefs.Code->prefix.seg == default_seg ) {
            assembler.asmdefs.Code->prefix.seg = PREFIX_EMPTY;
        }
    }
}


static unsigned char Reg386( asm_token reg_token )
/************************************************/
{
    switch( reg_token ) {
    case T_EAX:         return( 0 );
    case T_ECX:         return( 1 );
    case T_EDX:         return( 2 );
    case T_EBX:         return( 3 );
    case T_ESP:         return( 4 );
    case T_EBP:         return( 5 );
    case T_ESI:         return( 6 );
    case T_EDI:         return( 7 );
    }
    /* I don't think this should happen */
    return( 0 );
}

int OperandSize( AsmRun& assembler, OPNDTYPE opnd )
/******************************/
{
    if( ( opnd == OP_NONE ) || ( opnd & OP_SPECIAL ) ) {
        return( 0 );
    } else if( opnd == OP_M ) {
        /* fixme */
        switch( assembler.asmdefs.Code->mem_type ) {
        case MT_EMPTY:  return( 0 );
        case MT_BYTE:    return( 1 );
        case MT_WORD:    return( 2 );
        case MT_DWORD:   return( 4 );
        case MT_FWORD:   return( 6 );
        case MT_QWORD:   return( 8 );
        case MT_TBYTE:   return( 10 );
        case MT_OWORD:   return( 16 );
        default:         break;
        }
    } else if( opnd & ( OP_M8_R8 | OP_M_B | OP_I8 | OP_I_1 | OP_I_3 | OP_U8 ) ) {
        return( 1 );
    } else if( opnd & ( OP_M16_R16 | OP_M_W | OP_I16 | OP_SR ) ) {
        return( 2 );
    } else if( opnd & ( OP_R32 | OP_M_DW | OP_I32 | OP_J32 | OP_SPEC_REG ) ) {
        return( 4 );
    } else if( opnd & ( OP_I | OP_J48 ) ) {
        return( 6 );
    } else if( opnd & ( OP_M_QW | OP_MMX ) ) {
        return( 8 );
    } else if( opnd & ( OP_STI | OP_M_TB ) ) {
        return( 10 );
    } else if( opnd & ( OP_M_OW | OP_XMM ) ) {
        return( 16 );
    }
    return( 0 );
}

bool InRange( unsigned long val, unsigned bytes )
/**********************************************/
/*
     Can 'val' be represented in 'bytes' bytes?
*/
{
    unsigned long   max;
    unsigned long   mask;

    max = ( 1UL << ( bytes * 8 ) ) - 1;
    if( val <= max ) /* absolute value fits */
        return( true );
    mask = ~(max >> 1);
    if( ( val & mask ) == mask ) /* just a sign extension */
        return( true );
    return( false );

}

static bool mem2code( AsmRun& assembler, unsigned char ss, asm_token index, asm_token base, asm_sym_handle sym )
/*******************************************************************************************/
/*
  encode the memory operand to machine code
*/
{
    asm_code        *rCode = assembler.asmdefs.Code;
    unsigned char   mod_field;
    unsigned char   rm_field;

    // clear mod
    rm_field = 0;
    if( assembler.asmfixup.InsFixups[assembler.asmdefs.Opnd_Count] != NULL ) {
        mod_field = MOD_10;
    } else if( rCode->data[assembler.asmdefs.Opnd_Count] == 0 ) {
        mod_field = MOD_00;
    } else if( ( rCode->data[assembler.asmdefs.Opnd_Count] > SCHAR_MAX )
       || ( rCode->data[assembler.asmdefs.Opnd_Count] < SCHAR_MIN ) ) {
        mod_field = MOD_10;
    } else {
        mod_field = MOD_01;
    }
    if( ( index == T_NULL ) && ( base == T_NULL ) ) {
        // direct memory
        // clear the rightmost 3 bits
        mod_field = MOD_00;
        if( IS_ADRSIZ_32( rCode ) ) {
            rm_field = D32;
        } else {
            if( !InRange( rCode->data[assembler.asmdefs.Opnd_Count], 2 ) ) {
                // expect 16-bit but got 32-bit address
                AsmError( *assembler.front, DISPLACEMENT_OUT_OF_RANGE );
                return( RC_ERROR );
            }
            rm_field = D16;
        }
        // default is DS:[], DS: segment override is not needed
        seg_override( assembler, T_DS, sym );
    } else if( ( index == T_NULL ) && ( base != T_NULL ) ) {
        switch( base ) {
        case T_SI:
            rm_field = 0x04; // SI
            // default is DS:[], DS: segment override is not needed
            break;
        case T_DI:
            rm_field = 0x05; // DI
            // default is DS:[], DS: segment override is not needed
            break;
        case T_BP:
            rm_field = BP;
            if( mod_field == MOD_00 ) {
                mod_field = MOD_01;
            }
            // default is SS:[], SS: segment override is not needed
            break;
        case T_BX:
            rm_field = 0x07; // BX
            // default is DS:[], DS: segment override is not needed
            break;
        case T_EBP:
            rm_field = EBP;
            if( mod_field == MOD_00 ) {
                mod_field = MOD_01;
            }
            // default is SS:[], SS: segment override is not needed
            break;
        case T_ESP:
            rm_field = ESP;
            // ss = 00, index = 100 ( no index ), base = 100 ( ESP )
            rCode->sib = 0x24;
            // default is SS:[], SS: segment override is not needed
            break;
        default: // for 386 and up
            rm_field = Reg386( base );
            // default is DS:[], DS: segment override is not needed
        }
        seg_override( assembler, base, sym );
    } else if( ( index != T_NULL ) && ( base == T_NULL ) ) {
        // mod field is 00
        mod_field = MOD_00;
        // s-i-b is present ( r/m = 100 )
        rm_field = S_I_B;
        // scale factor, index, base ( 0x05 => no base reg )
        rCode->sib = ( ss | ( Reg386( index ) << 3 ) | 0x05 );
        // default is DS:[], DS: segment override is not needed
        seg_override( assembler, T_DS, sym );
    } else {
        // base != T_NULL && index != T_NULL
        switch( index ) {
        case T_BX:
        case T_BP:
            if( comp_mem( assembler, index, base, &rm_field ) )
                return( RC_ERROR );
            seg_override( assembler, index, sym );
            break;
        case T_SI:
        case T_DI:
            if( comp_mem( assembler, base, index, &rm_field ) )
                return( RC_ERROR );
            seg_override( assembler, base, sym );
            break;
        case T_ESP:
            AsmError( *assembler.front, ESP_CANNOT_BE_USED_AS_INDEX );
            return( RC_ERROR );
        default:
            if( base < T_EAX ) {
                AsmError( *assembler.front, CANNOT_MIX_16_AND_32_BIT_REGISTERS );
                return( RC_ERROR );
            } else if( base == T_EBP ) {
                if( mod_field == MOD_00 ) {
                    mod_field = MOD_01;
                }
            }
            // s-i-b is present ( r/m = 100 )
            rm_field |= S_I_B;
            rCode->sib = ( ss | (Reg386( index ) << 3) | Reg386( base ) );
            seg_override( assembler, base, sym );
        }
    }
    if( assembler.asmdefs.Opnd_Count == OPND2 ) {
        // shift the register field to left by 3 bit
        rCode->info.rm_byte = mod_field | ( rm_field << 3 ) | ( rCode->info.rm_byte & BIT_012 );
    } else if( assembler.asmdefs.Opnd_Count == OPND1 ) {
        rCode->info.rm_byte = mod_field | rm_field;
    }
    return( RC_OK );
}

static asm_cpu get_cpu_flags( asm_token token )
/*********************************************/
/*
  Convert token to CPU/FPU flags
*/
{
    switch( token ) {
    case T_DOT_NO87:
        return( P_NO87 );
    case T_DOT_8086:
        return( P_86 );
    case T_DOT_8087:
        return( P_87 );
    case T_DOT_186:
        return( P_186 );
    case T_DOT_286:
        return( P_286 );
    case T_DOT_287:
        return( P_287 );
    case T_DOT_286P:
        return( P_286p );
    case T_DOT_386:
        return( P_386 );
    case T_DOT_387:
        return( P_387 );
    case T_DOT_386P:
        return( P_386p );
    case T_DOT_486:
        return( P_486 );
    case T_DOT_486P:
        return( P_486p );
    case T_DOT_586:
        return( P_586 );
    case T_DOT_586P:
        return( P_586p );
    case T_DOT_686:
        return( P_686 );
    case T_DOT_686P:
        return( P_686p );
    case T_DOT_MMX:
        return( P_MMX );
    case T_DOT_K3D:
        return( P_K3D | P_MMX );
    case T_DOT_XMM:
        return( P_SSE | P_MMX );
    case T_DOT_XMM2:
        return( P_SSE2 | P_SSE | P_MMX );
    case T_DOT_XMM3:
        return( P_SSE3 | P_SSE2 | P_SSE | P_MMX );
    }
    // not found
    return( P_EMPTY );
}

static asm_cpu get_fpu_flags( asm_token token )
/*********************************************/
/*
  derive FPU flags from CPU token
*/
{
    switch( token ) {
    case T_DOT_8086:
    case T_DOT_186:
        return( P_87 );
    case T_DOT_286:
    case T_DOT_286P:
        return( P_287 );
    case T_DOT_386:
    case T_DOT_386P:
    case T_DOT_486:
    case T_DOT_486P:
    case T_DOT_586:
    case T_DOT_586P:
    case T_DOT_686:
    case T_DOT_686P:
        return( P_387 );
    default:
        return static_cast<asm_cpu>(( 0 ));
    }
}


static asm_cpu add_cpu_flags( asm_token token, asm_cpu cpu, asm_cpu new_cpu )
/***************************************************************************/
{
    if( token == T_DOT_NO87 ) {
        cpu &= ~P_FPU_MASK;                         // turn off FPU bits
    } else if( new_cpu & P_EXT_MASK ) {
        cpu |= new_cpu & P_EXT_MASK;                // turn on desired bit(s)
    } else if( new_cpu & P_FPU_MASK ) {
        cpu &= ~P_FPU_MASK;
        cpu |= new_cpu & P_FPU_MASK;                // setup FPU bits
    } else {
        cpu &= ~( P_CPU_MASK | P_PM );
        cpu |= new_cpu & ( P_CPU_MASK | P_PM );     // setup CPU bits
        cpu &= ~P_FPU_MASK;
        cpu |= get_fpu_flags( token ) & P_FPU_MASK; // setup FPU bits
    }
    return( cpu );
}

bool cpu_directive( AsmRun& assembler, asm_token token )
/***********************************/
{
    asm_cpu     new_cpu;

    if( (new_cpu = get_cpu_flags( token )) == P_EMPTY ) {
        AsmError( *assembler.front, UNKNOWN_DIRECTIVE );
        return( RC_ERROR );
    }
    assembler.asmdefs.Code->info.cpu = add_cpu_flags( token, assembler.asmdefs.Code->info.cpu, new_cpu );

    return( RC_OK );
}

static bool idata_float( AsmRun& assembler, long value )
/***********************************/
/*
  check the correct operand/data size for float immediate operand;
*/
{
    switch( assembler.asmdefs.Code->mem_type ) {
    case MT_EMPTY:
        if( assembler.asmdefs.Code->info.token == T_PUSHW ) { // sigh. another special case
            // expect 32-bit code but get 16-bit
            AsmError( *assembler.front, IMMEDIATE_DATA_OUT_OF_RANGE );
            return( RC_ERROR );
        }
        break;
    case MT_FAR:
    case MT_NEAR:
    case MT_SHORT:
        AsmError( *assembler.front, SYNTAX_ERROR );
        return( RC_ERROR );
    case MT_BYTE:
    case MT_WORD:
        AsmError( *assembler.front, OPERANDS_MUST_BE_THE_SAME_SIZE );
        return( RC_ERROR );
    case MT_DWORD:
        // set w-bit
        assembler.asmdefs.Code->info.opcode |= W_BIT;
        break;
    default:
        break;
    }
    SET_OPSIZ_32( assembler.asmdefs.Code );
    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
    assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count] = value;
    return( RC_OK );
}

static unsigned char get_sr_rm_byte( prefix_reg seg_prefix )
/***************************************************************/
{
    switch( seg_prefix ) {
    case PREFIX_ES:
        return( 0 );
    case PREFIX_CS:
        return( 1 );
    case PREFIX_SS:
        return( 2 );
    case PREFIX_DS:
        return( 3 );
    case PREFIX_FS:
        return( 4 );
    case PREFIX_GS:
        return( 5 );
    default:
        break;
    }
    return( 0 );
}


static bool process_jumps( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx, int *jmp_flags )
/**********************************************************************************
 * parse the jumps instructions operands
 */
{
    segm_override_jumps( assembler, tokbuf, opndx );

    if( ptr_operator( assembler, opndx->mem_type, opndx->explicit_ ) )
        return( RC_ERROR );
    if( ptr_operator( assembler, MT_PTR, opndx->explicit_ ) ) {
        return( RC_ERROR );
    }
    if( opndx->mbr != NULL ) {
        if( ptr_operator( assembler, opndx->mbr->mem_type, false ) )
            return( RC_ERROR );
        if( ptr_operator( assembler, MT_PTR, false ) ) {
            return( RC_ERROR );
        }
    }
    return( jmp( assembler, tokbuf, opndx, jmp_flags ) );
}

static bool segm_override_jumps( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx )
/***********************************************************************/
{
    if( ISVALID_IDX( opndx->override ) ) {
        if( tokbuf->tokens[opndx->override].class_ == TC_REG ) {
            assembler.asmdefs.Code->prefix.seg = static_cast<prefix_reg>(AsmOpTable[AsmOpcode[tokbuf->tokens[opndx->override].u.token].position].opcode);
        } else {
        }
    }
    return( RC_OK );
}

static bool segm_override_idata( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx )
/***********************************************************************/
{
    if( ISVALID_IDX( opndx->override ) ) {
        if( tokbuf->tokens[opndx->override].class_ == TC_REG ) {
            assembler.asmdefs.Code->prefix.seg = static_cast<prefix_reg>(AsmOpTable[AsmOpcode[tokbuf->tokens[opndx->override].u.token].position].opcode);
        } else {
        }
    }
    return( RC_OK );
}

static bool segm_override_memory( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx )
/************************************************************************/
{
    if( ISVALID_IDX( opndx->override ) ) {
        if( tokbuf->tokens[opndx->override].class_ == TC_REG ) {
            assembler.asmdefs.Code->prefix.seg = static_cast<prefix_reg>(AsmOpTable[AsmOpcode[tokbuf->tokens[opndx->override].u.token].position].opcode);
        } else {
        }
    }
    return( RC_OK );
}

static bool idata_nofixup( AsmRun& assembler, expr_list *opndx )
/*******************************************/
{
    OPNDTYPE    op_type = OP_NONE;
    long        value;

    value = opndx->value;
    assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count] = value;

    /*
     * A shift/rotate count is an UNSIGNED imm8 (C1 /n ib), never sign-extended
     * and independent of the destination size, so classify 80h..FFh as I8 up
     * front -- the size-specific arms below would otherwise call it I16/I32 (the
     * generic sign-extend rule that is right for ADD/etc.), breaking e.g.
     * `sar dword ptr [ebx], 0c3h'.  value 0..127 and the `,1' short form fall
     * through to the normal path unchanged.
     */
    switch( assembler.asmdefs.Code->info.token ) {
    case T_SAR: case T_SHL: case T_SHR: case T_SAL:
    case T_ROL: case T_ROR: case T_RCL: case T_RCR:
        if( ( value > SCHAR_MAX ) && ( value <= UCHAR_MAX ) ) {
            assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
            return( RC_OK );
        }
        break;
    default:
        break;
    }

    switch( assembler.asmdefs.Code->mem_type ) {
    case MT_EMPTY:
        if( assembler.asmdefs.Code->info.token == T_PUSH ) {
            if( opndx->explicit_ ) {
                if( opndx->mem_type == MT_BYTE ) {
                    op_type = OP_I8;
                } else if( opndx->mem_type == MT_WORD ) {
                    op_type = OP_I16;
                    SET_OPSIZ_16( assembler.asmdefs.Code );
                } else if( opndx->mem_type == MT_DWORD ) {
                    op_type = OP_I32;
                    SET_OPSIZ_32( assembler.asmdefs.Code );
                } else {
                    // FIXME !!
                    op_type = OP_I32;
                    SET_OPSIZ_32( assembler.asmdefs.Code );
                }
                break;
            }
            if( assembler.asmdefs.Code->use32 ) {
                if( (int_8)value == (int_32)value ) {
                    op_type = OP_I8;
                } else {
                    op_type = OP_I32;
                }
            } else if( (unsigned long)value > USHRT_MAX ) {
                SET_OPSIZ_32( assembler.asmdefs.Code );
                if( (int_8)value == (int_32)value ) {
                    op_type = OP_I8;
                } else {
                    op_type = OP_I32;
                }
            } else {
                if( (int_8)value == (int_16)value ) {
                    op_type = OP_I8;
                } else {
                    op_type = OP_I16;
                }
            }
            break;
        } else if( assembler.asmdefs.Code->info.token == T_PUSHW ) {
            op_type = OP_I16;
            if( (int_8)value == (int_16)value ) {
                op_type = OP_I8;
            }
            break;
        } else if( assembler.asmdefs.Code->info.token == T_PUSHD ) {
            op_type = OP_I32;
            if( (int_8)value == (int_32)value ) {
                op_type = OP_I8;
            }
            break;
        } else if( ( assembler.asmdefs.Code->info.token == T_RET
                  || assembler.asmdefs.Code->info.token == T_RETN
                  || assembler.asmdefs.Code->info.token == T_RETD
                  || assembler.asmdefs.Code->info.token == T_RETF )
                && ( value > SHRT_MAX ) && ( value <= USHRT_MAX ) ) {
            /*
             * RET/RETF stack-adjust is an UNSIGNED imm16 (C2/CA iw).  MASM does
             * not treat it as signed, so 0C4C3h is I16, not I32 -- without this
             * `ret 0c4c3h' (seen in the reconstructed VxDs) was E040.  Only the
             * 8000h..FFFFh range is redirected; smaller values use the generic
             * path (which the imm16 form already accepts).
             */
            op_type = OP_I16;
            break;
        }
        /* fall through */
        if( ( value > SHRT_MAX ) || ( value < SHRT_MIN ) ) {
            op_type = OP_I32;
        } else if( ( value > SCHAR_MAX ) || ( value < SCHAR_MIN ) ) {
            op_type = OP_I16;
        } else {
            op_type = OP_I8;
        }
        break;
    case MT_FAR:
        if( ( value > SHRT_MAX ) || ( value < SHRT_MIN ) ) {
            op_type = OP_I32;
        } else if( ( value > SCHAR_MAX ) || ( value < SCHAR_MIN ) ) {
            op_type = OP_I16;
        } else {
            op_type = OP_I8;
        }
        break;
    case MT_NEAR:
        if( !assembler.asmdefs.Code->use32 ) {
            op_type = OP_I16;
        } else {
            op_type = OP_I32;
        }
        break;
    case MT_SHORT:
        if( ( value > SCHAR_MAX ) || ( value < SCHAR_MIN ) ) {
            // expect 8-bit but got 16 bit
            AsmError( *assembler.front, JUMP_OUT_OF_RANGE );
            return( RC_ERROR );
        }
        op_type = OP_I8;
        break;
    case MT_BYTE:
        if( !InRange( value, 1 ) ) {
            // expect 8-bit but got 16 bit
            AsmError( *assembler.front, IMMEDIATE_DATA_OUT_OF_RANGE );
            return( RC_ERROR );
        }
        op_type = OP_I8;
        break;
    case MT_WORD:
            if( !InRange( value, 2 ) ) {
                AsmError( *assembler.front, IMMEDIATE_DATA_OUT_OF_RANGE );
                return( RC_ERROR );
            }
            if( ( value > SCHAR_MAX ) || ( value < SCHAR_MIN ) ) {
                op_type = OP_I16;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            assembler.asmdefs.Code->info.opcode |= W_BIT;
        break;
    case MT_DWORD:
            if( ( value > SCHAR_MAX ) || ( value < SCHAR_MIN ) ) {
                op_type = OP_I32;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            assembler.asmdefs.Code->info.opcode |= W_BIT;
        break;
    case MT_QWORD:
            if( ( value > SCHAR_MAX ) || ( value < SCHAR_MIN ) ) {
                op_type = OP_I32;
            } else {
                op_type = OP_I8;
            }
            // set w-bit
            assembler.asmdefs.Code->info.opcode |= W_BIT;
        break;
    default:
        break;
    }
    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = op_type;
    return( RC_OK );
}

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct AsminsState {
    bool ConstantOnly;
    asm_cpu CPUinfo;
};
namespace {

}  // namespace


static bool idata_fixup( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx )
/***************************************************************/
{
    asmfixup        *fixup;
    fixup_types     fixup_type;
    int             type;
    bool            sym32;

    assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count] = opndx->value;
    segm_override_idata( assembler, tokbuf, opndx );

    sym32 = assembler.asmdefs.Code->use32;
    if( opndx->instr != T_NULL ) {
        if( ISVALID_IDX( opndx->base_reg )
            || ISVALID_IDX( opndx->idx_reg ) ) {
            AsmError( *assembler.front, INVALID_MEMORY_POINTER );
            return( RC_ERROR );
        }
    }
    if( opndx->instr == T_SEG ) {
        if( opndx->sym->state == SYM_STACK ) {
            AsmError( *assembler.front, CANNOT_SEG_AUTO );
            return( RC_ERROR );
        }
    } else {
        if( MEM_TYPE( assembler.asmdefs.Code->mem_type, BYTE ) && !opndx->abs ) {
            AsmError( *assembler.front, OFFSET_TOO_SMALL );
            return( RC_ERROR );
        }
        if( opndx->sym->state == SYM_STACK ) {
            AsmError( *assembler.front, CANNOT_OFFSET_AUTO );
            return( RC_ERROR );
        }
    }
    switch( assembler.asmdefs.Code->mem_type ) {
    case MT_BYTE:
        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
        break;
    case MT_EMPTY:
        if( assembler.asmdefs.Opnd_Count > OPND1 ) {
            type = OperandSize( assembler, assembler.asmdefs.Code->info.opnd_type[OPND1] );
            if( type == 4 ) {
                assembler.asmdefs.Code->mem_type = MT_DWORD;
                assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                SET_OPSIZ_32( assembler.asmdefs.Code );
                break;
            } else if( type == 2 ) {
                assembler.asmdefs.Code->mem_type = MT_WORD;
                assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
                SET_OPSIZ_16( assembler.asmdefs.Code );
                break;
            }
        }
        if( opndx->abs ) {
            if( opndx->mem_type == MT_BYTE ) {
                assembler.asmdefs.Code->mem_type = MT_BYTE;
                assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
                break;
            } else if( opndx->mem_type == MT_EMPTY ) {
                SET_OPSIZ_OFF( assembler.asmdefs.Code );
                if( IS_OPSIZ_32( assembler.asmdefs.Code ) ) {
                    assembler.asmdefs.Code->mem_type = MT_DWORD;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                    sym32 = true;
                    break;
                }
            } else if( opndx->mem_type == MT_DWORD ) {
                sym32 = true;
            }
        }
        if( ( assembler.asmdefs.Code->info.token == T_PUSHD ) || sym32 ) {
            assembler.asmdefs.Code->mem_type = MT_DWORD;
            assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
            SET_OPSIZ_32( assembler.asmdefs.Code );
            break;
        }
        assembler.asmdefs.Code->mem_type = MT_WORD;
        /* fall through */
    case MT_WORD:
        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
        SET_OPSIZ_16( assembler.asmdefs.Code );
        break;
    case MT_DWORD:
        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
        SET_OPSIZ_32( assembler.asmdefs.Code );
        break;
    default:
        break;
    }
    if( opndx->instr == T_SEG ) {
        fixup_type = FIX_SEG;
    } else {
        if( assembler.asmdefs.Code->mem_type == MT_BYTE ) {
            fixup_type = FIX_LOBYTE;
        } else if( IS_OPSIZ_32( assembler.asmdefs.Code ) ) {
            fixup_type = ( sym32 ) ? FIX_OFF32 : FIX_OFF16;
        } else {
            if( sym32 ) {
                // fixme !!!! warning
                // operand size is 16bit
                // but fixup is 32-bit
            }
            fixup_type = FIX_OFF16;
        }
    }
    assembler.asmins->ConstantOnly = true;
    if( !MEM_TYPE( assembler.asmdefs.Code->mem_type, BYTE ) ) {
        assembler.asmdefs.Code->info.opcode |= W_BIT;
    }


    fixup = AddFixup( assembler, opndx->sym, fixup_type, OPTJ_NONE );
//    add_frame();   // ???
    if( fixup == NULL ) {
        return( RC_ERROR );
    }
    return( RC_OK );
}

static bool idata_operand( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx )
/*****************************************************************/
{
    if( opndx->sym == NULL ) {
        return( idata_nofixup( assembler, opndx ) );
    } else {
        return( idata_fixup( assembler, tokbuf, opndx ) );
    }
}

static bool memory_operand( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx )
/******************************************************************/
{
    unsigned char   ss = SCALE_FACTOR_1;
    asm_token       index = T_NULL;
    asm_token       base = T_NULL;
    asm_sym_handle  sym;
    bool            base_lock = false;
    fixup_types     fixup_type;

    assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count] = opndx->value;
    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_M;

    segm_override_memory( assembler, tokbuf, opndx );

    if( ptr_operator( assembler, opndx->mem_type, opndx->explicit_ ) )
        return( RC_ERROR );
    if( ptr_operator( assembler, MT_PTR, opndx->explicit_ ) ) {
        return( RC_ERROR );
    }
    if( opndx->mbr != NULL ) {
        if( ptr_operator( assembler, opndx->mbr->mem_type, false ) )
            return( RC_ERROR );
        if( ptr_operator( assembler, MT_PTR, false ) ) {
            return( RC_ERROR );
        }
    }
    if( ISVALID_IDX( opndx->base_reg ) ) {
        base = tokbuf->tokens[opndx->base_reg].u.token;
        switch( base ) {     // check for base registers
        case T_EAX:
        case T_EBX:
        case T_ECX:
        case T_EDX:
        case T_ESP:
        case T_EBP:
        case T_ESI:
        case T_EDI:
            if( ( assembler.asmdefs.Code->info.cpu & ( P_CPU_MASK | P_PM ) ) <= P_286p ) {
                // 286 and down cannot use 386 registers
                AsmError( *assembler.front, CANNOT_USE_386_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                return( RC_ERROR );
            }
            SET_ADRSIZ_32( assembler.asmdefs.Code );
            break;
        case T_BX:
        case T_BP:
        case T_SI:
        case T_DI:
            SET_ADRSIZ_16( assembler.asmdefs.Code );
            break;
        default:
            AsmError( *assembler.front, INVALID_MEMORY_POINTER );
            return( RC_ERROR );
        }
    }
    if( ISVALID_IDX( opndx->idx_reg ) ) {
        index = tokbuf->tokens[opndx->idx_reg].u.token;
        switch( index ) {     // check for index registers
        case T_EAX:
        case T_EBX:
        case T_ECX:
        case T_EDX:
        case T_ESP:
        case T_EBP:
        case T_ESI:
        case T_EDI:
            if( ( assembler.asmdefs.Code->info.cpu & ( P_CPU_MASK | P_PM ) ) <= P_286p ) {
                // 286 and down cannot use 386 registers
                AsmError( *assembler.front, CANNOT_USE_386_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                return( RC_ERROR );
            }
            SET_ADRSIZ_32( assembler.asmdefs.Code );
            break;
        case T_BX:
        case T_BP:
        case T_SI:
        case T_DI:
            SET_ADRSIZ_16( assembler.asmdefs.Code );
            break;
        default:
            AsmError( *assembler.front, INVALID_MEMORY_POINTER );
            return( RC_ERROR );
        }
        if( tokbuf->tokens[opndx->idx_reg].u.token == T_ESP ) {
            if( opndx->scale == 1 ) {
                index = base;
                base = tokbuf->tokens[opndx->idx_reg].u.token;
            } else {
                AsmError( *assembler.front, ESP_CANNOT_BE_USED_AS_INDEX );
                return( RC_ERROR );
            }
        }
        if( IS_ADRSIZ_32( assembler.asmdefs.Code ) ) {
            if( ( assembler.asmdefs.Code->info.cpu & P_CPU_MASK ) >= P_386 ) {
                if( !assembler.asmdefs.Code->use32 )
                    SET_ADRSIZ_ON( assembler.asmdefs.Code );
                switch( index ) {
                case T_ESP:
                case T_BX:
                case T_BP:
                case T_SI:
                case T_DI:
                    // cannot use ESP or 16-bit reg as index
                    AsmError( *assembler.front, INVALID_INDEX_REGISTER );
                    return( RC_ERROR );
                default:
                    if( !assembler.asmdefs.Code->use32 )
                        SET_ADRSIZ_ON( assembler.asmdefs.Code );
                    switch( opndx->scale ) {
                    case 1:
                        // ss = 00
                        break;
                    case 2:
                        // ss = 01
                        ss = SCALE_FACTOR_2;
                        break;
                    case 4:
                        // ss = 10
                        ss = SCALE_FACTOR_4;
                        break;
                    case 8:
                        // ss = 11
                        ss = SCALE_FACTOR_8;
                        break;
                    default: // must be * 1, 2, 4 or 8
                        AsmError( *assembler.front, SCALE_FACTOR_MUST_BE_1_2_4_OR_8 );
                        return( RC_ERROR );
                    }
                }
            } else {
                // 286 and down cannot use this memory mode
                AsmError( *assembler.front, INVALID_ADDRESSING_MODE_WITH_CURRENT_CPU_SETTING );
                return( RC_ERROR );
            }
        }
    }
    sym = opndx->sym;
    if( sym != NULL ) {
        switch( sym->state ) {
        case SYM_UNDEFINED:
            // forward reference
            break;
        case SYM_STACK:
            if( base != T_NULL ) {
                if( base_lock ) {
                    // [reg + data][reg + data] is not allowed
                    AsmError( *assembler.front, TOO_MANY_BASE_REGISTERS );
                    return( RC_ERROR );
                }
                index = base;
            }
            if( assembler.asmdefs.Code->use32 ) {
                base = T_EBP;
            } else {
                base = T_BP;
            }
            base_lock = true;   // add lock
            /* fall through */
        default:
            if( assembler.asmdefs.Code->mem_type == MT_EMPTY ) {
                if( ptr_operator( assembler, sym->mem_type, false ) )
                    return( RC_ERROR );
                if( ptr_operator( assembler, MT_PTR, false ) ) {
                    return( RC_ERROR );
                }
            }
            break;
        }

        fixup_type = ( assembler.asmdefs.Code->use32 ) ? FIX_OFF32 : FIX_OFF16;

        AddFixup( assembler, sym, fixup_type, OPTJ_NONE );

        if( mem2code( assembler, ss, index, base, sym ) ) {
            return( RC_ERROR );
        }
    } else {
        if( mem2code( assembler, ss, index, base, sym ) ) {
            return( RC_ERROR );
        }
    }
    if( assembler.asmdefs.Code->mem_type == MT_DWORD && !assembler.asmdefs.Code->use32 ) {
        if( assembler.asmdefs.Code->info.token == T_CALL ) {
            assembler.asmdefs.Code->info.token = T_CALLF;
        } else if( assembler.asmdefs.Code->info.token == T_JMP ) {
            assembler.asmdefs.Code->info.token = T_JMPF;
        }
    }
    return( RC_OK );
}

static bool process_address( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx, int *jmp_flags )
/************************************************************************************
 * parse the memory reference operand
 */
{
    memtype     mem_type;

    *jmp_flags = 0;
    if( opndx->indirect ) {           // indirect operand
        return( memory_operand( assembler, tokbuf, opndx ) );
    } else {                          // direct operand
        if( opndx->instr != T_NULL ) { // OFFSET ..., SEG ...
            if( IS_ANY_BRANCH( assembler.asmdefs.Code->info.token ) ) {  // jumps/call processing
                return( process_jumps( assembler, tokbuf, opndx, jmp_flags ) );
            } else {
                return( idata_operand( assembler, tokbuf, opndx ) );
            }
        } else {                      // direct operand only
            if( opndx->sym == NULL ) {       // without symbol
                if( ISVALID_IDX( opndx->override ) ) {
                    // direct absolute memory without fixup ... DS:[0]
                    return( memory_operand( assembler, tokbuf, opndx ) );
                } else if( IS_ANY_BRANCH( assembler.asmdefs.Code->info.token ) ) {  // jumps/call processing
                    return( process_jumps( assembler, tokbuf, opndx, jmp_flags ) );
                } else {
                    return( idata_operand( assembler, tokbuf, opndx ) );  // error ????
                }
            } else {                  // with symbol
                if( ( opndx->sym->state == SYM_UNDEFINED ) && !opndx->explicit_ ) {
                    // undefined symbol, it is not possible to determine
                    // operand type and size
                    if( IS_ANY_BRANCH( assembler.asmdefs.Code->info.token ) ) {  // jumps/call processing
                        return( process_jumps( assembler, tokbuf, opndx, jmp_flags ) );
                    } else {
                        switch( assembler.asmdefs.Code->info.token ) {
                        case T_PUSH:
                        case T_PUSHW:
                        case T_PUSHD:
                            return( idata_nofixup( assembler, opndx ) );
                            break;
                        default:
                            return( memory_operand( assembler, tokbuf, opndx ) );
                            break;
                        }
                    }
                } else {
                    // CODE location is converted to OFFSET symbol
                    mem_type = ( opndx->explicit_ ) ? opndx->mem_type : opndx->sym->mem_type;
                    switch( mem_type ) {
                    case MT_FAR:
                    case MT_NEAR:
                    case MT_SHORT:
                        if( assembler.asmdefs.Code->info.token == T_LEA ) {
                            return( memory_operand( assembler, tokbuf, opndx ) );
                        } else if( opndx->mbr != NULL ) { // structure or structure member
                            return( memory_operand( assembler, tokbuf, opndx ) );
                        } else {
                            if( IS_ANY_BRANCH( assembler.asmdefs.Code->info.token ) ) {  // jumps/call processing
                                return( process_jumps( assembler, tokbuf, opndx, jmp_flags ) );
                            }
                            return( idata_operand( assembler, tokbuf, opndx ) );
                        }
                        break;
                    default:
                        // direct memory with fixup
                        return( memory_operand( assembler, tokbuf, opndx ) );
                        break;
                    }
                }
            }
        }
    }
//    return( RC_OK );
}

static bool process_const( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx, int *jmp_flags )
/*********************************************************************************/
{
    if( IS_ANY_BRANCH( assembler.asmdefs.Code->info.token ) )    // jumps/call processing
        return( process_jumps( assembler, tokbuf, opndx, jmp_flags ) );
    if( ( assembler.asmdefs.Code->info.token == T_IMUL )
        && ( assembler.asmdefs.Code->info.opnd_type[OPND1] & OP_R ) ) {
        if( assembler.asmdefs.Opnd_Count == OPND2 ) {
            assembler.asmdefs.Code->info.rm_byte = ( assembler.asmdefs.Code->info.rm_byte & ~BIT_345 )
                          | ( ( assembler.asmdefs.Code->info.rm_byte & BIT_012 ) << 3 );
        } else if( assembler.asmdefs.Opnd_Count == OPND3 ) {
            assembler.asmdefs.Code->info.opnd_type[OPND1] = assembler.asmdefs.Code->info.opnd_type[OPND2];
            assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_NONE;
            assembler.asmdefs.Code->data[OPND1] = assembler.asmdefs.Code->data[OPND2];
            assembler.asmdefs.Code->data[OPND2] = 0;
            assembler.asmfixup.InsFixups[OPND1] = assembler.asmfixup.InsFixups[OPND2];
            assembler.asmfixup.InsFixups[OPND2] = NULL;
            assembler.asmdefs.Opnd_Count = OPND2;
        }
    }
    return( idata_operand( assembler, tokbuf, opndx ) );
}

static bool process_reg( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx, int *jmp_flags )
/*******************************************************************************/
/*
- parse and encode the register operand;
*/
{
    unsigned char       st_reg;
    unsigned char       reg;
    const asm_ins       ASMI86FAR *ins;

    if( opndx->indirect )  // simple register indirect operand ... [EBX]
        return( process_address( assembler, tokbuf, opndx, jmp_flags ) );
    ins = AsmOpTable + AsmOpcode[tokbuf->tokens[opndx->base_reg].u.token].position;
    reg = ins->opcode;
    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = ins->opnd_type2;
    switch( ins->opnd_type2 ) {
    case OP_AL:
    case OP_R8:
        assembler.asmdefs.Code->info.opcode &= NOT_W_BIT;         // clear w-bit
        break;
    case OP_CL: /* only appears in "shift opnd,CL" instructions */
        break;
    case OP_AX:
    case OP_DX: /* only appears in "in" and "out" instructions  */
    case OP_R16:
        assembler.asmdefs.Code->info.opcode |= W_BIT;             // set w-bit
        if( assembler.asmdefs.Code->use32 )
            SET_OPSIZ_ON( assembler.asmdefs.Code );
        break;
    case OP_MMX:
        break;
    case OP_XMM:
        break;
    case OP_ST:
        st_reg = (unsigned char)opndx->idx_reg & BIT_012;
        assembler.asmdefs.Code->info.rm_byte |= st_reg;
        if( st_reg != 0 )
            assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_ST_REG;
        break;
    case OP_SR3:                        // 386 segment register
        if( ( assembler.asmdefs.Code->info.cpu & ( P_CPU_MASK | P_PM ) ) <= P_286p ) {
            // 8086 ins cannot use 80386 segment register
            AsmError( *assembler.front, CANNOT_USE_386_SEGMENT_REGISTER_WITH_CURRENT_CPU_SETTING );
            return( RC_ERROR );
        }
        /* fall through */
    case OP_SR:                                 // any seg reg
    case OP_SR2:                                // 8086 segment register
        if( tokbuf->tokens[opndx->base_reg].u.token == T_CS ) {
            // POP CS is not allowed
            if( assembler.asmdefs.Code->info.token == T_POP ) {
                AsmError( *assembler.front, POP_CS_IS_NOT_ALLOWED );
                return( RC_ERROR );
            }
        }
        reg = get_sr_rm_byte( static_cast<prefix_reg>(ins->opcode) );
        break;
    case OP_EAX:
    case OP_R32:
        if( ( assembler.asmdefs.Code->info.cpu & ( P_CPU_MASK | P_PM ) ) <= P_286p ) {
            // 8086 ins cannot use 386 register
            AsmError( *assembler.front, CANNOT_USE_386_REGISTER_WITH_CURRENT_CPU_SETTING );
            return( RC_ERROR );
        }
        assembler.asmdefs.Code->info.opcode |= W_BIT;             // set w-bit
        if( !assembler.asmdefs.Code->use32 )
            SET_OPSIZ_ON( assembler.asmdefs.Code );
        break;
    case OP_TR:                 // Test registers
        switch( tokbuf->tokens[opndx->base_reg].u.token ) {
        case T_TR3:
        case T_TR4:
        case T_TR5:
            if( ( ( ( assembler.asmdefs.Code->info.cpu & P_CPU_MASK ) < P_486 )
               || ( ( assembler.asmdefs.Code->info.cpu & P_CPU_MASK ) >= P_686 ) )
                && ( ( ins->cpu & P_CPU_MASK ) >= P_486 ) ) {
                // TR3, TR4, TR5 are available on 486 only
                AsmError( *assembler.front, CANNOT_USE_TR3_TR4_TR5_IN_CURRENT_CPU_SETTING );
                return( RC_ERROR );
            }
            break;
        case T_TR6:
        case T_TR7:
            if( ( ( ( assembler.asmdefs.Code->info.cpu & P_CPU_MASK ) < P_386 )
               || ( ( assembler.asmdefs.Code->info.cpu & P_CPU_MASK ) >= P_686 ) )
                && ( ( ins->cpu & P_CPU_MASK ) >= P_386 ) ) {
                // TR6, TR7 are available on 386...586 only
                AsmError( *assembler.front, CANNOT_USE_TR3_TR4_TR5_IN_CURRENT_CPU_SETTING );
                return( RC_ERROR );
            }
            break;
        }
        break;
    case OP_CR:                 // Control registers
    case OP_DR:                 // Debug registers
        if( assembler.asmdefs.Code->info.token != T_MOV ) {
            AsmError( *assembler.front, ONLY_MOV_CAN_USE_SPECIAL_REGISTER );
            return( RC_ERROR );
        }
        break;
    }
    if( assembler.asmdefs.Opnd_Count == OPND1 ) {
        // the first operand
        // r/m is treated as a 'reg' field
        assembler.asmdefs.Code->info.rm_byte |= MOD_11;
        // fill the r/m field
        assembler.asmdefs.Code->info.rm_byte |= reg;
    } else {
        // the second operand
        if( ( assembler.asmdefs.Code->info.token == T_XCHG )
            && ( ( assembler.asmdefs.Code->info.opnd_type[OPND1] == OP_AX )
            || ( assembler.asmdefs.Code->info.opnd_type[OPND1] == OP_EAX ) ) ) {
            // XCHG can use short form if op1 is AX or EAX
            assembler.asmdefs.Code->info.rm_byte = ( assembler.asmdefs.Code->info.rm_byte & BIT_67 ) | reg;
        } else {
            // fill reg field with reg
            assembler.asmdefs.Code->info.rm_byte = ( assembler.asmdefs.Code->info.rm_byte & ~BIT_345 ) | ( reg << 3 );
        }
    }
    return( RC_OK );
}

bool AsmParse( AsmRun& assembler, token_buffer *tokbuf, const char *curline )
/*********************************************************
 * - co-ordinate the parsing process;
 * - it is a basically a big loop to loop through all the tokens and identify them
 *   with the switch statement;
 */
{
    token_idx       i;
    bool            cur_opnd_label = false;
    bool            last_opnd_label = false;
    asm_code        *rCode = assembler.asmdefs.Code;
    expr_list       opndx;
    token_idx       n;
    operand_idx     j;
    int             jmp_flags;
    bool            flag;

    /* unused parameters */ (void)curline;

    //init
    rCode->info.token     = T_NULL;
    rCode->info.opcode    = 0;
    rCode->info.rm_byte   = 0;
    rCode->prefix.ins     = T_NULL;
    rCode->prefix.seg     = PREFIX_EMPTY;
    SET_ADRSIZ_OFF( rCode );
    SET_OPSIZ_OFF( rCode );
    rCode->mem_type       = MT_EMPTY;
    rCode->mem_type_fixed = false;
    rCode->extended_ins   = EMPTY;
    rCode->sib            = 0;            // assume ss is *1
    rCode->indirect       = false;
    for( j = static_cast<operand_idx>(0); j < OPND_MAX; j++ ) {
        rCode->info.opnd_type[j] = OP_NONE;
        rCode->data[j] = 0;
        assembler.asmfixup.InsFixups[j] = NULL;
    }
    assembler.asmdefs.Opnd_Count = static_cast<operand_idx>(0);

    // check if continue initializing array
    if( NextArrayElement( assembler, tokbuf, &flag ) )
        return( RC_ERROR );
    if( flag )
        return( RC_OK );


    for( i = 0; i < tokbuf->count; i++ ) {
        switch( tokbuf->tokens[i].class_ ) {
        case TC_INSTR:
//            ExpandTheWorld( i, false, true );
            if( last_opnd_label ) {
                // illegal operand is put before instruction
                AsmError( *assembler.front, SYNTAX_ERROR );
                return( RC_ERROR );
            }
            cur_opnd_label = false;
            switch( tokbuf->tokens[i].u.token ) {
            // prefix
            case T_LOCK:
            case T_REP:
            case T_REPE:
            case T_REPNE:
            case T_REPNZ:
            case T_REPZ:
                rCode->prefix.ins = tokbuf->tokens[i].u.token;
                // prefix has to be followed by an instruction
                if( tokbuf->tokens[i+1].class_ != TC_INSTR ) {
                    AsmError( *assembler.front, PREFIX_MUST_BE_FOLLOWED_BY_AN_INSTRUCTION );
                    return( RC_ERROR );
                }
                continue;
            default:
                rCode->info.token = tokbuf->tokens[i].u.token;
                break;
            }
            i++;
            if( EvalOperand( assembler, tokbuf, &i, tokbuf->count, &opndx, true ) ) {
                return( RC_ERROR );
            }
            if( opndx.empty )
                break;
            switch( opndx.type ) {
            case EXPR_ADDR:
                if( process_address( assembler, tokbuf, &opndx, &jmp_flags ) )
                    return( RC_ERROR );
//                if( jmp_flags == INDIRECT_JUMP )
//                    return( RC_ERROR );
                if( jmp_flags == SCRAP_INSTRUCTION )
                    return( RC_OK );
                break;
            case EXPR_CONST:
                process_const( assembler, tokbuf, &opndx, &jmp_flags );
                break;
            case EXPR_REG:
                process_reg( assembler, tokbuf, &opndx, &jmp_flags );
                break;
            case EXPR_UNDEF:
                return( RC_ERROR );
            default:
                break;
            }
            i--;
            break;
        case TC_RES_ID:
            if( rCode->info.token == T_NULL ) {
                n = ( i == 0 ) ? INVALID_IDX : 0;
                return( data_init( assembler, tokbuf, n, i ) );
            }
            AsmError( *assembler.front, SYNTAX_ERROR );
            return( RC_ERROR );
        case TC_DIRECTIVE:
            return( cpu_directive( assembler, tokbuf->tokens[i].u.token ) );
        case TC_ID:
            if( i == 0 ) {   // a new label

                switch( tokbuf->tokens[i+1].class_ ) {
                case TC_COLON:
                    cur_opnd_label = true;
                    break;
                case TC_RES_ID:
                    return( data_init( assembler, tokbuf, i, i+1 ) );
                    break;
                default:
                    AsmError( *assembler.front, SYNTAX_ERROR );
                    return( RC_ERROR );
                }
            }
            break;
        case TC_COMMA:
            if( assembler.asmdefs.Opnd_Count > OPND2 ) {
                AsmError( *assembler.front, TOO_MANY_COMMAS );
                return( RC_ERROR );
            }
            i++;
            cur_opnd_label = false;
            if( EvalOperand( assembler, tokbuf, &i, tokbuf->count, &opndx, true ) ) {
                return( RC_ERROR );
            }
            assembler.asmdefs.Opnd_Count++;
            if( opndx.empty ) {
                if( tokbuf->tokens[i].class_ == TC_FLOAT
                    || tokbuf->tokens[i].class_ == TC_MINUS
                    || tokbuf->tokens[i].class_ == TC_PLUS ) {
                    i--;
                    continue;
                }
                AsmError( *assembler.front, SYNTAX_ERROR );
                return( RC_ERROR );
            }
            if( assembler.asmdefs.Opnd_Count == OPND3 ) {
                if( rCode->info.token == T_SHLD || rCode->info.token == T_SHRD ) {
                    if( opndx.type == EXPR_CONST ) {
                        assembler.asmdefs.Opnd_Count--;
                    } else if( opndx.type == EXPR_REG ) {
                        if( tokbuf->tokens[opndx.base_reg].u.token == T_CL ) {
                            assembler.asmdefs.Opnd_Count--;
                            i--;
                            break;
                        }
                    }
                }
            }
            switch( opndx.type ) {
            case EXPR_ADDR:
                if( process_address( assembler, tokbuf, &opndx, &jmp_flags ) )
                    return( RC_ERROR );
//                if( jmp_flags == INDIRECT_JUMP )
//                    return( RC_ERROR );
                if( jmp_flags == SCRAP_INSTRUCTION )
                    return( RC_OK );
                break;
            case EXPR_CONST:
                process_const( assembler, tokbuf, &opndx, &jmp_flags );
                break;
            case EXPR_REG:
                process_reg( assembler, tokbuf, &opndx, &jmp_flags );
                break;
            case EXPR_UNDEF:
                return( RC_ERROR );
            default:
                break;
            }
            i--;
            break;
        case TC_COLON:
            if( last_opnd_label ) {
                if( tokbuf->tokens[i+1].class_ != TC_RES_ID ) {
                    if( MakeLabel( assembler, tokbuf, i - 1, MT_NEAR ) ) {
                         return( RC_ERROR );
                    }
                }
                cur_opnd_label = false;
            } else {
                AsmError( *assembler.front, SYNTAX_ERROR_UNEXPECTED_COLON );
                return( RC_ERROR );
            }
            break;
        case TC_PLUS:
        case TC_MINUS:
            break;
        case TC_FLOAT:
            if( idata_float( assembler, tokbuf->tokens[i].u.value ) ) {
                return( RC_ERROR );
            }
            if( tokbuf->tokens[i-1].class_ == TC_MINUS ) {
                rCode->data[assembler.asmdefs.Opnd_Count] ^= 0x80000000;
            }
            break;
        default:
            break;
        }
        last_opnd_label = cur_opnd_label;
    }
    switch( rCode->info.token ) {
    case T_LODS:
    case T_SCAS:
    case T_STOS:
        SizeString( assembler, OperandSize( assembler, assembler.asmdefs.Code->info.opnd_type[OPND1] ) );
        break;
    }
    if( assembler.asmdefs.Opnd_Count > OPND1 ) {
        if( check_size(assembler) ) {
            return( RC_ERROR );
        }
    }
    return( match_phase_1(assembler) );
}

static void SizeString( AsmRun& assembler, unsigned op_size )
/****************************************/
{
    /* size an string instruction based on it's operand's size */
    switch( op_size ) {
    case 1:
        assembler.asmdefs.Code->mem_type = MT_BYTE;
        assembler.asmdefs.Code->info.opcode &= NOT_W_BIT;
        if( assembler.asmdefs.Code->use32 )
            SET_OPSIZ_OFF( assembler.asmdefs.Code );
        break;
    case 2:
        assembler.asmdefs.Code->mem_type = MT_WORD;
        assembler.asmdefs.Code->info.opcode |= W_BIT;
        SET_OPSIZ_16( assembler.asmdefs.Code );
        break;
    case 4:
        assembler.asmdefs.Code->mem_type = MT_DWORD;
        assembler.asmdefs.Code->info.opcode |= W_BIT;
        SET_OPSIZ_32( assembler.asmdefs.Code );
        break;
    }
}

static bool check_size( AsmRun& assembler )
/****************************/
/*
- use to make sure the size of first operand match the size of second operand;
- optimize MOV instruction;
*/
{
    OPNDTYPE        op1 = assembler.asmdefs.Code->info.opnd_type[OPND1];
    OPNDTYPE        op2 = assembler.asmdefs.Code->info.opnd_type[OPND2];
    bool            state;
    long            temp;
    int             op1_size;
    int             op2_size;
    int             op_size = 0;
    unsigned char   rm_byte;

    state = RC_OK;
    switch( assembler.asmdefs.Code->info.token ) {
    case T_IN:
        if( op2 == OP_DX ) {
            switch( op1 ) {
            case OP_AX:
                break;
            case OP_AL:
                assembler.asmdefs.Code->info.opcode &= NOT_W_BIT;         // clear w-bit
            case OP_EAX:
                if( assembler.asmdefs.Code->use32 ) {
                    SET_OPSIZ_OFF( assembler.asmdefs.Code );
                }
                break;
            }
        }
        break;
    case T_OUT:
        if( op1 == OP_DX ) {
            switch( op2 ) {
            case OP_AX:
                break;
            case OP_AL:
                assembler.asmdefs.Code->info.opcode &= NOT_W_BIT;         // clear w-bit
            case OP_EAX:
                if( assembler.asmdefs.Code->use32 ) {
                    SET_OPSIZ_OFF( assembler.asmdefs.Code );
                }
            }
        }
        break;
    case T_INS:
    case T_CMPS:
        op_size = OperandSize( assembler, op1 );
        /* fall through */
    case T_MOVS:
    case T_OUTS:
        if( op_size == 0 )
            op_size = OperandSize( assembler, op2 );

        SizeString( assembler, op_size );
        break;
    case T_LEA:
        switch( OperandSize( assembler, op1 ) ) {
        case 2:
        case 4:
            break;
        default:
            AsmError( *assembler.front, OPERANDS_MUST_BE_THE_SAME_SIZE );
            state = RC_ERROR;
        }
        break;
    case T_RCL:
    case T_RCR:
    case T_ROL:
    case T_ROR:
    case T_SAL:
    case T_SAR:
    case T_SHL:
    case T_SHR:
    case T_LDS:
    case T_LES:
    case T_LFS:
    case T_LGS:
    case T_LSS:
        // checking will be done later
        break;
    case T_ENTER:
        // ENTER has to be OP_I16, OP_I8
        if( op1 == OP_I32 ) {
            //parse_phase_1 will treat 16-bit data as OP_I32 if CPU is 386
            if( assembler.asmdefs.Code->data[OPND1] > (signed long)USHRT_MAX ) {
                // if op1 is really 32-bit data, then error
                AsmError( *assembler.front, IMMEDIATE_DATA_TOO_BIG );
                state = RC_ERROR;
            }
        }
        // type cast op1 to OP_I16
        assembler.asmdefs.Code->info.opnd_type[OPND1] = OP_I16;
        // op2 have to be 8-bit data
        if( op2 >= OP_I16 ) {
            if( assembler.asmdefs.Code->data[OPND2] > UCHAR_MAX ) {
                AsmError( *assembler.front, IMMEDIATE_DATA_TOO_BIG );
                state = RC_ERROR;
            }
            assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I8;
        }
        break;
    case T_CALLF:
    case T_JMPF:
        // segment can only be 16-bit
        if( op1 > OP_I16 ) {
            AsmError( *assembler.front, SEGMENT_TOO_BIG );
            state = RC_ERROR;
        }
        if( ( assembler.asmdefs.Code->info.cpu & ( P_CPU_MASK | P_PM ) ) <= P_286p ) {
            // offset can only be 16-bit if CPU is 286 and down
            if( op2 > OP_I16 ) {
                AsmError( *assembler.front, OFFSET_TOO_BIG );
                state = RC_ERROR;
            }
        }
        // swap the 2 opnds to make output easier
        if( assembler.asmfixup.InsFixups[OPND2] != NULL ) {
            // absolute segment + offset nnnn,offset
            assembler.asmdefs.Code->info.opnd_type[OPND1] = assembler.asmdefs.Code->info.opnd_type[OPND2];
            assembler.asmfixup.InsFixups[OPND1] = assembler.asmfixup.InsFixups[OPND2];
            assembler.asmfixup.InsFixups[OPND2] = NULL;
        } else {
            // absolute ptr nnnn,nnnn
            assembler.asmdefs.Code->info.opnd_type[OPND1] = ( assembler.asmdefs.Code->use32 ) ? OP_I32 : OP_I16;
        }
        temp = assembler.asmdefs.Code->data[OPND1];
        assembler.asmdefs.Code->data[OPND1] = assembler.asmdefs.Code->data[OPND2];
        assembler.asmdefs.Code->data[OPND2] = temp;
        assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I16;
        assembler.asmdefs.Code->info.opcode = 0;
        break;
    case T_MOVSX:
    case T_MOVZX:
        assembler.asmdefs.Code->info.opcode &= NOT_W_BIT;
        op2_size = OperandSize( assembler, op2 );
        switch( OperandSize( assembler, op1 ) ) {
        case 4:
            switch( op2_size ) {
            case 2:
                assembler.asmdefs.Code->info.opcode |= W_BIT;
                break;
            case 4:
                AsmError( *assembler.front, OP_2_TOO_BIG );
                state = RC_ERROR;
            }
            if( assembler.asmdefs.Code->use32 ) {
                SET_OPSIZ_OFF( assembler.asmdefs.Code );     // - don't need opnd size prefix
            }
            break;
        case 2:
            if( op2_size >= 2 ) {
                AsmError( *assembler.front, OP_2_TOO_BIG );
                state = RC_ERROR;
            }
            break;
        default:
            // op1 have to be r16/r32
            AsmError( *assembler.front, OP_1_TOO_SMALL );
            state = RC_ERROR;
        }
        break;
    case T_LSL:                                 /* 19-sep-93 */
        op1_size = OperandSize( assembler, op1 );
        switch( op1_size ) {
        case 2:
            if( assembler.asmdefs.Code->use32 )
                SET_OPSIZ_ON( assembler.asmdefs.Code );
            break;
        case 4:
            if( assembler.asmdefs.Code->use32 )
                SET_OPSIZ_OFF( assembler.asmdefs.Code );
            break;
        default:
            AsmError( *assembler.front, INVALID_SIZE );
            return( RC_ERROR );
        }
        op2_size = OperandSize( assembler, op2 );
        switch( op2_size ) {
        case 2:
        case 4:
            break;
        default:
            AsmError( *assembler.front, INVALID_SIZE );
            state = RC_ERROR;
            break;
        }
        break;
    case T_CVTSD2SI:
    case T_CVTTSD2SI:
    case T_CVTSS2SI:
    case T_CVTTSS2SI:
    case T_MOVNTI:
        break;
    case T_BOUND:
        /*
         * BOUND r16/r32, m16&16/m32&32: the memory operand is deliberately
         * TWICE the register width (a lower/upper bound pair), so MASM writes it
         * as `bound eax, qword ptr [..]' (32-bit reg, 64-bit mem).  The generic
         * same-size check below would wrongly reject that -- skip it; the
         * encoding is driven by the register/operand-size, not the mem size.
         */
        break;
    case T_MOVD:
        break;
    case T_MOV:
        if( op1 & OP_SR ) {
            op2_size = OperandSize( assembler, op2 );
            if( ( op2_size == 2 ) || ( op2_size == 4 ) ) {
//                SET_OPSIZ_OFF( Code );
                return( state );
            }
        } else if( op2 & OP_SR ) {
            op1_size = OperandSize( assembler, op1 );
            if( ( op1_size == 2 ) || ( op1_size == 4 ) ) {
//                if( op1 == OP_M )
//                    SET_OPSIZ_OFF( Code );
                return( state );
            }
        } else if( ( op1 == OP_M ) || ( op2 == OP_M ) ) {
            // to optimize MOV
            rm_byte = assembler.asmdefs.Code->info.rm_byte;
            if( assembler.asmdefs.Code->info.opnd_type[OPND1] & OP_A ) {
                rm_byte = ( rm_byte & BIT_67 ) | ( ( rm_byte & BIT_012 ) << 3 ) | ( ( rm_byte & BIT_345 ) >> 3 );
                if( IS_ADRSIZ_32( assembler.asmdefs.Code ) && ( rm_byte == D32 )
                    || IS_ADRSIZ_16( assembler.asmdefs.Code ) && ( rm_byte == D16 ) ) {
                    // DS:[d32] or DS:[d16] can use MOV Mem with Acc (short form)
                } else {
                    // we have to change OP_A to OP_R
                    assembler.asmdefs.Code->info.opnd_type[OPND1] &= ~OP_A;
                }
            } else if( assembler.asmdefs.Code->info.opnd_type[OPND2] & OP_A ) {
                if( IS_ADRSIZ_32( assembler.asmdefs.Code ) && ( rm_byte == D32 )
                    || IS_ADRSIZ_16( assembler.asmdefs.Code ) && ( rm_byte == D16 ) ) {
                    // DS:[d32] or DS:[d16] can use MOV Mem with Acc (short form)
                } else {
                    // we have to change OP_A to OP_R
                    assembler.asmdefs.Code->info.opnd_type[OPND2] &= ~OP_A;
                }
            }
        } else if( ( op1 & OP_SPEC_REG ) || ( op2 & OP_SPEC_REG ) ) {
            SET_OPSIZ_OFF( assembler.asmdefs.Code );
            return( state );
        }
        /* fall through */
    default:
        // make sure the 2 opnds are of the same type
        op1_size = OperandSize( assembler, op1 );
        op2_size = OperandSize( assembler, op2 );
        if( op1_size > op2_size ) {
            if( ( op2 >= OP_I8 ) && ( op2 <= OP_I32 ) ) {     /* immediate */
                op2_size = op1_size;    /* promote to larger size */
            }
        }
        if( ( op1_size == 1 ) && ( op2 == OP_I16 )
            && ( assembler.asmdefs.Code->data[OPND2] <= UCHAR_MAX ) ) {
            return( state ); // OK cause no sign extension
        }
        if( ( op1_size == 2 ) && ( op2 == OP_I32 )
            && ( assembler.asmdefs.Code->data[OPND2] <= USHRT_MAX ) ) {
            return( state ); // OK cause no sign extension
        }
        if( op1_size != op2_size ) {
            /* if one or more are !defined, set them appropriately */
            if( ( op1 | op2 ) & ( OP_MMX | OP_XMM ) ) {
            } else if( ( op1_size != 0 ) && ( op2_size != 0 ) ) {
                AsmError( *assembler.front, OPERANDS_MUST_BE_THE_SAME_SIZE );
//                state = RC_ERROR;
            }
            if( op1_size == 0 ) {
                if( ( op1 & OP_M_ANY ) && ( op2 & OP_I ) ) {
                    if( (unsigned long)assembler.asmdefs.Code->data[OPND2] > USHRT_MAX ) {
                         assembler.asmdefs.Code->mem_type = MT_DWORD;
                         assembler.asmdefs.Code->info.opcode |= W_BIT;
                         assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I32;
                    } else if( (unsigned long)assembler.asmdefs.Code->data[OPND2] > UCHAR_MAX ) {
                         assembler.asmdefs.Code->mem_type = MT_WORD;
                         assembler.asmdefs.Code->info.opcode |= W_BIT;
                         assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I16;
                    } else {
                         assembler.asmdefs.Code->mem_type = MT_BYTE;
                         assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I8;
                    }
                } else if( ( op1 & OP_M_ANY ) && ( op2 & ( OP_R | OP_SR ) ) ) {
                } else if( ( op1 & ( OP_MMX | OP_XMM ) ) && ( op2 & OP_I ) ) {
                    if( (unsigned long)assembler.asmdefs.Code->data[OPND2] > USHRT_MAX ) {
                         assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I32;
                    } else if( (unsigned long)assembler.asmdefs.Code->data[OPND2] > UCHAR_MAX ) {
                         assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I16;
                    } else {
                         assembler.asmdefs.Code->info.opnd_type[OPND2] = OP_I8;
                    }
                } else if( ( op1 | op2 ) & ( OP_MMX | OP_XMM ) ) {
                } else {
                    switch( op2_size ) {
                    case 1:
                        assembler.asmdefs.Code->mem_type = MT_BYTE;
                        break;
                    case 2:
                        assembler.asmdefs.Code->mem_type = MT_WORD;
                        assembler.asmdefs.Code->info.opcode |= W_BIT;
                        if( assembler.asmdefs.Code->use32 )
                            SET_OPSIZ_ON( assembler.asmdefs.Code );
                        break;
                    case 4:
                        assembler.asmdefs.Code->mem_type = MT_DWORD;
                        assembler.asmdefs.Code->info.opcode |= W_BIT;
                        break;
                    }
                }
            }
        }
    }
    return( state );
}


void AsmEnvInit( AsmRun& assembler, int use32, int cpu, int fpu, bool fpu_emu )
/**********************************************************/
{
    assembler.asmdefs.Code->use32 = ( use32 != 0 );
    switch( cpu ) {
    case 0: assembler.asmdefs.Code->info.cpu = P_86;   break;
    case 1: assembler.asmdefs.Code->info.cpu = P_186;  break;
    case 2: assembler.asmdefs.Code->info.cpu = P_286p; break;
    case 3: assembler.asmdefs.Code->info.cpu = P_386p; break;
    case 4: assembler.asmdefs.Code->info.cpu = P_486p; break;
    case 5: assembler.asmdefs.Code->info.cpu = P_586p | P_K3D | P_MMX; break;
    case 6: assembler.asmdefs.Code->info.cpu = P_686p | P_K3D | P_MMX | P_SSE | P_SSE2 | P_SSE3; break;
    }
    assembler.asmglob.SWData.fpt = ( fpu_emu ) ? DO_FP_EMULATION : NO_FP_EMULATION;
    switch( fpu ) {
    case 0: assembler.asmglob.SWData.fpt = NO_FP_ALLOWED; break;
    case 1: assembler.asmdefs.Code->info.cpu |= P_87;  break;
    case 2: assembler.asmdefs.Code->info.cpu |= P_287; break;
    case 3: assembler.asmdefs.Code->info.cpu |= P_387; break;
    }
}


void AsmInit( AsmRun& assembler )
/******************/
{
    assembler.asmins->CPUinfo = assembler.asmdefs.Code->info.cpu;
    AsmSymInit();
}

void AsmFini( AsmRun& assembler )
/******************/
{
    AsmSymFini(assembler);
    assembler.asmdefs.Code->info.cpu = assembler.asmins->CPUinfo;
}


/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
AsminsState* asmins_make() { return new AsminsState(); }
void asmins_free(AsminsState* part) { delete part; }
