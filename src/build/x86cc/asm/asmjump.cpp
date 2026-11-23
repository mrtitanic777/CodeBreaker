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
* Description:  CALL/JMP processing
*
****************************************************************************/


#include "asmglob.h"
#include "asmfixup.h"
#include "asmeval.h"
#include "asmjump.h"
#include "asmrun.hpp"

/* prototypes */
bool ptr_operator( AsmRun& assembler, memtype mem_type, bool fix_mem_type );


bool jmp( AsmRun& assembler, token_buffer *tokbuf, expr_list *opndx, int *flags )
/*
  determine the displacement of jmp;
*/
{
    int_32          addr;
    fixup_types     fixup_type;
    fixup_options   fixup_option;
    enum sym_state  state;
    asm_sym_handle  sym;


    /* unused parameters */ (void)tokbuf;

    *flags = 0;
    assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count] = opndx->value;
    sym = opndx->sym;
    if( sym == NULL ) {
        if( assembler.asmdefs.Code->info.token == T_CALL ) {
            assembler.asmdefs.Code->info.token = T_CALLF;
        } else if( assembler.asmdefs.Code->info.token == T_JMP ) {
            assembler.asmdefs.Code->info.token = T_JMPF;
        }
        if( assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count] > USHRT_MAX ) {
            assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
        } else {
            assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
        }
        return( RC_OK );
    }

    state = sym->state;

    if( !assembler.asmdefs.Code->mem_type_fixed ) {
        assembler.asmdefs.Code->mem_type = MT_EMPTY;
    }
    fixup_option = OPTJ_NONE;
    fixup_type = FIX_RELOFF8;
    switch( state ) {
    case SYM_INTERNAL:
        if(  ( assembler.asmdefs.Code->mem_type == MT_EMPTY || assembler.asmdefs.Code->mem_type == MT_SHORT
                || assembler.asmdefs.Code->mem_type == MT_NEAR )
            && sym->mem_type != MT_WORD
            && sym->mem_type != MT_DWORD
            && sym->mem_type != MT_FWORD
            && !IsJmpCallF( assembler.asmdefs.Code->info.token ) ) {
            addr = sym->addr;
            addr -= ( assembler.asminlin->AsmCodeAddress + 2 );  // calculate the displacement
            addr += assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count];
            switch( assembler.asmdefs.Code->info.token ) {
            case T_JCXZ:
            case T_LOOPW:
            case T_LOOPEW:
            case T_LOOPZW:
            case T_LOOPNEW:
            case T_LOOPNZW:
                if( assembler.asmdefs.Code->use32 ) {
                    // 1 extra byte for OPNSIZ
                    addr--;
                }
                break;
            case T_JECXZ:
            case T_LOOPD:
            case T_LOOPED:
            case T_LOOPZD:
            case T_LOOPNED:
            case T_LOOPNZD:
                if( !assembler.asmdefs.Code->use32 ) {
                    // 1 extra byte for OPNSIZ
                    addr--;
                }
                break;
            }
            if( assembler.asmdefs.Code->info.token == T_CALL && assembler.asmdefs.Code->mem_type == MT_EMPTY ) {
                assembler.asmdefs.Code->mem_type = MT_NEAR;
            }
            if( assembler.asmdefs.Code->mem_type != MT_NEAR && assembler.asmdefs.Code->info.token != T_CALL
                && ( addr >= SCHAR_MIN && addr <= SCHAR_MAX ) ) {
                assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
            } else {
                /* near jmp */
                if( assembler.asmdefs.Code->use32 ) {
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                    addr -= 3; // 32 bit displacement
                } else {
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
                    addr -= 1; // 16 bit displacement
                }
                if( IsJmp( assembler.asmdefs.Code->info.token ) ) {
                    switch( assembler.asmdefs.Code->info.token ) {
                    case T_JMP:
                    case T_JMPF:
                    case T_JCXZ:
                    case T_JECXZ:
                        break;
                    default:
                        // 1 extra byte for opcode ( 0F )
                        addr--;
                        break;
                    }
                }
            }

            /* store the displacement */
            assembler.asmdefs.Code->data[assembler.asmdefs.Opnd_Count] = addr;

            switch( assembler.asmdefs.Code->info.token ) {
            case T_JCXZ:
            case T_JECXZ:
            case T_LOOP:
            case T_LOOPE:
            case T_LOOPNE:
            case T_LOOPNZ:
            case T_LOOPZ:
            case T_LOOPD:
            case T_LOOPED:
            case T_LOOPNED:
            case T_LOOPNZD:
            case T_LOOPZD:
            case T_LOOPW:
            case T_LOOPEW:
            case T_LOOPNEW:
            case T_LOOPNZW:
            case T_LOOPZW:
                if( (assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] != OP_I8) ) {
                    AsmError( *assembler.front, JUMP_OUT_OF_RANGE );
                    return( RC_ERROR );
                }
                assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
                break;
            }

            if( (assembler.asmdefs.Code->info.cpu & P_CPU_MASK) < P_386 && IsJmp( assembler.asmdefs.Code->info.token ) ) {
                /* look into jump extension */
                switch( assembler.asmdefs.Code->info.token ) {
                case T_JMP:
                case T_JMPF:
                    break;
                default:
                    if( assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] != OP_I8 ) {
                        AsmError( *assembler.front, JUMP_OUT_OF_RANGE );
                        return( RC_ERROR );
                    }
                }
            }
            break;
        }
        /* otherwise fall through & get handled like external symbols */
    case SYM_UNDEFINED:
    case SYM_EXTERNAL:

        /* forward ref, or external symbol */
        if( assembler.asmdefs.Code->mem_type == MT_EMPTY && sym->mem_type != MT_EMPTY ) {
            switch( sym->mem_type ) {
            case MT_FAR:
                if( assembler.asmdefs.Code->info.token == T_CALL ) {
                    assembler.asmdefs.Code->info.token = T_CALLF;
                } else if( assembler.asmdefs.Code->info.token == T_JMP ) {
                    assembler.asmdefs.Code->info.token = T_JMPF;
                }
                // fall through
            case MT_SHORT:
            case MT_NEAR:
                assembler.asmdefs.Code->mem_type = sym->mem_type;
                break;
            case MT_FWORD:
                if( ptr_operator( assembler, MT_FWORD, true ) )
                    return( RC_ERROR );
                break;
            default:
                assembler.asmdefs.Code->mem_type = sym->mem_type;
            }
        }
        if( assembler.asmdefs.Code->mem_type == MT_FAR ) {
            if( assembler.asmdefs.Code->info.token == T_CALL ) {
                assembler.asmdefs.Code->info.token = T_CALLF;
            } else if( assembler.asmdefs.Code->info.token == T_JMP ) {
                assembler.asmdefs.Code->info.token = T_JMPF;
            }
        }
        switch( assembler.asmdefs.Code->info.token ) {
        case T_CALLF:
        case T_JMPF:
            switch( assembler.asmdefs.Code->mem_type ) {
            case MT_SHORT:
            case MT_NEAR:
                if( assembler.asmdefs.Opnd_Count == OPND1 && assembler.asmdefs.Code->mem_type_fixed ) {
                    AsmError( *assembler.front, CANNOT_USE_SHORT_OR_NEAR );
                    return( RC_ERROR );
                }
                /* fall through */
            case MT_FAR:
            case MT_EMPTY:
                if( assembler.asmdefs.Opnd_Count == OPND2 ) {
                    if( IsOpsiz32( assembler.asmdefs.Code ) ) {
                        fixup_type = FIX_OFF32;
                        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                    } else {
                        fixup_type = FIX_OFF16;
                        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
                    }
                } else {
                    if( IsOpsiz32( assembler.asmdefs.Code ) ) {
                        fixup_type = FIX_PTR32;
                        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_J48;
                    } else {
                        fixup_type = FIX_PTR16;
                        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_J32;
                    }
                }
                break;
            case MT_BYTE:
            case MT_WORD:
                AsmError( *assembler.front, INVALID_SIZE );
                return( RC_ERROR );
            case MT_DWORD:
            case MT_FWORD:
                *flags = INDIRECT_JUMP;
                return( RC_OK );
            case MT_QWORD:
            case MT_TBYTE:
            case MT_OWORD:
                AsmError( *assembler.front, INVALID_SIZE );
                return( RC_ERROR );
            }
            break;
        case T_CALL:
            if( assembler.asmdefs.Code->mem_type == MT_SHORT ) {
                AsmError( *assembler.front, CANNOT_USE_SHORT_WITH_CALL );
                return( RC_ERROR );
            } else if( assembler.asmdefs.Code->mem_type == MT_EMPTY ) {
                fixup_option = OPTJ_NONE;
                if( assembler.asmdefs.Code->use32 ) {
                    fixup_type = FIX_RELOFF32;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                } else {
                    fixup_type = FIX_RELOFF16;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
                }
                break;
            }
            /* fall through */
        case T_JMP:
            switch( assembler.asmdefs.Code->mem_type ) {
            case MT_SHORT:
                fixup_option = OPTJ_EXPLICIT;
                fixup_type = FIX_RELOFF8;
                assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
                break;
            case MT_FAR:
                AsmError( *assembler.front, SYNTAX_ERROR );
                break;
            case MT_EMPTY:
                // forward reference
                // inline assembler jmp default distance is near
                // stand-alone assembler jmp default distance is short
                fixup_option = OPTJ_NONE;
                if( assembler.asmdefs.Code->use32 ) {
                    fixup_type = FIX_RELOFF32;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                } else {
                    fixup_type = FIX_RELOFF16;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
                }
                break;
            case MT_NEAR:
                fixup_option = OPTJ_EXPLICIT;
                if( assembler.asmdefs.Code->use32 ) {
                    fixup_type = FIX_RELOFF32;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                } else {
                    fixup_type = FIX_RELOFF16;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
                }
                break;
            case MT_DWORD:
            case MT_WORD:
                *flags = INDIRECT_JUMP;
                return( RC_OK );
            case MT_BYTE:
            case MT_FWORD:
            case MT_QWORD:
            case MT_TBYTE:
            case MT_OWORD:
                AsmError( *assembler.front, INVALID_SIZE );
                return( RC_ERROR );
            }
//            check_assume( sym, PREFIX_EMPTY );
            break;
        case T_JCXZ:
        case T_JECXZ:
            // JCXZ and JECXZ always require SHORT label
        case T_LOOP:
        case T_LOOPE:
        case T_LOOPNE:
        case T_LOOPNZ:
        case T_LOOPZ:
        case T_LOOPD:
        case T_LOOPED:
        case T_LOOPNED:
        case T_LOOPNZD:
        case T_LOOPZD:
        case T_LOOPW:
        case T_LOOPEW:
        case T_LOOPNEW:
        case T_LOOPNZW:
        case T_LOOPZW:
            if( ( assembler.asmdefs.Code->mem_type != MT_EMPTY ) &&
                ( assembler.asmdefs.Code->mem_type != MT_SHORT ) ) {
                AsmError( *assembler.front, ONLY_SHORT_DISPLACEMENT_IS_ALLOWED );
                return( RC_ERROR );
            }
            assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
            fixup_option = OPTJ_EXPLICIT;
            fixup_type = FIX_RELOFF8;
            break;
        default:
            if( (assembler.asmdefs.Code->info.cpu & P_CPU_MASK) >= P_386 ) {
                switch( assembler.asmdefs.Code->mem_type ) {
                case MT_SHORT:
                    fixup_option = OPTJ_EXPLICIT;
                    fixup_type = FIX_RELOFF8;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
                    break;
                case MT_EMPTY:
                    // forward reference
                    // inline assembler default distance is near
                    // stand-alone assembler default distance is short
                case MT_NEAR:
                    fixup_option = OPTJ_EXPLICIT;
                    if( assembler.asmdefs.Code->use32 ) {
                        fixup_type = FIX_RELOFF32;
                        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I32;
                    } else {
                        fixup_type = FIX_RELOFF16;
                        assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I16;
                    }
                    break;
                case MT_FAR:
                default:
                    AsmError( *assembler.front, ONLY_SHORT_AND_NEAR_DISPLACEMENT_IS_ALLOWED );
                    return( RC_ERROR );
                }
            } else {
                // the only mode in 8086, 80186, 80286 is
                // Jxx SHORT
                switch( assembler.asmdefs.Code->mem_type ) {
                case MT_EMPTY:
                case MT_SHORT:
                    fixup_option = OPTJ_EXPLICIT;
                    fixup_type = FIX_RELOFF8;
                    assembler.asmdefs.Code->info.opnd_type[assembler.asmdefs.Opnd_Count] = OP_I8;
                    break;
                default:
                    AsmError( *assembler.front, ONLY_SHORT_DISPLACEMENT_IS_ALLOWED );
                    return( RC_ERROR );
                }
            }
        }
        AddFixup( assembler, sym, fixup_type, fixup_option );
        break;
    default: /* SYM_STACK */
        AsmError( *assembler.front, NO_JUMP_TO_AUTO );
        return( RC_ERROR );
    }
    return( RC_OK );
}

bool ptr_operator( AsmRun& assembler, memtype mem_type, bool fix_mem_type )
/******************************************************/
/*
  determine what should be done with SHORT, NEAR, FAR, BYTE, WORD, DWORD, PTR
  operator;
*/
{
    /* new idea:
     * when we get a near/far/dword/etc, just set distance / mem_type
     * operator will be called again with PTR, then we set the opsiz, etc.
     */

    if( assembler.asmdefs.Code->info.token == T_LEA )
        return( RC_OK );
    if( assembler.asmdefs.Code->info.token == T_SMSW )
        return( RC_OK );
    if( mem_type == MT_PTR ) {
        /* finish deciding what type to make the inst NOW
         * ie: decide size overrides etc.
         */
        if( assembler.asmdefs.Code->use32 && MEM_TYPE( assembler.asmdefs.Code->mem_type, WORD ) ) {
            // if we are in use32 mode, we have to add OPSIZ prefix for
            // most of the 386 instructions ( except MOVSX and MOVZX )
            // when we find WORD PTR

            if( !IsBranch( assembler.asmdefs.Code->info.token ) ) {
                if( assembler.asmdefs.Code->info.opnd_type[OPND1] == OP_MMX ) {
                /* JBS 2001/02/19
                no WORD operands for MMX instructions, only 64-bit or 128-bit
                so no WORD override needed
                    */
                } else {
                    switch( assembler.asmdefs.Code->info.token ) {
                    case T_MOVSX:
                    case T_MOVZX:
                        break;
                    default:
                        SetOpsizOn( assembler.asmdefs.Code );
                        break;
                    }
                }
            }

        } else if( !assembler.asmdefs.Code->use32 && MEM_TYPE( assembler.asmdefs.Code->mem_type, DWORD ) ) {

            /* if we are not in use32 mode, we have to add OPSIZ
             * when we find DWORD PTR
             * unless we have a LXS ins.
             * which moves a DWORD ptr into SR:word reg
             * fixme  - can this be done by breaking up the LXS instructions in
             *          asmins.h, and then putting F_32 or F_16 to append
             *      opsize bytes when necessary ?
             */
            if( !IsBranch( assembler.asmdefs.Code->info.token ) ) {

                if( assembler.asmdefs.Code->info.opnd_type[OPND1] == OP_MMX ) {
                    /* JBS 2001/02/19
                       no WORD operands for MMX instructions, only 64-bit or 128-bit
                       so no DWORD override needed
                     */
                } else {
                    switch( assembler.asmdefs.Code->info.token ) {
                    case T_LDS:
                    case T_LES:
                    case T_LFS:
                    case T_LGS:
                    case T_LSS:
                        /* in these cases, opsize does NOT need to be changed  */
                        break;
                    default:
                        // OPSIZ prefix
                        SetOpsizOn( assembler.asmdefs.Code );
                    }
                }
            }
        }
    } else {
        if( ( mem_type != MT_EMPTY ) && !assembler.asmdefs.Code->mem_type_fixed ) {
                assembler.asmdefs.Code->mem_type = mem_type;
                if( fix_mem_type ) {
                    assembler.asmdefs.Code->mem_type_fixed = true;
                    if( mem_type == MT_FAR ) {
                        if( assembler.asmdefs.Code->info.token == T_CALL ) {
                            assembler.asmdefs.Code->info.token = T_CALLF;
                        } else if( assembler.asmdefs.Code->info.token == T_JMP ) {
                            assembler.asmdefs.Code->info.token = T_JMPF;
                        }
                    }
                }

        }
    }
    return( RC_OK );
}
