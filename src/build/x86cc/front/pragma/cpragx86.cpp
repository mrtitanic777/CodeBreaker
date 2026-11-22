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
* Description:  Pragmas specific to x86 targets.
*
****************************************************************************/


#include <ctype.h>
#include "cvars.h"
#include "i64.h"
#include "cgswitch.h"
#include "pdefn2.h"
#include "asmstmt.h"
#include "frontrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct Cpragx86State: cpragx86_state.h) */

}  // namespace



static void pragmaAuxInfoInit( FrontRun& front )
/***********************************/
{
    HW_CTurnOff( front.pdefn2.AsmRegsSaved, HW_xAX );
    HW_CTurnOff( front.pdefn2.AsmRegsSaved, HW_xBX );
    HW_CTurnOff( front.pdefn2.AsmRegsSaved, HW_xCX );
    HW_CTurnOff( front.pdefn2.AsmRegsSaved, HW_xDX );
    HW_CTurnOff( front.pdefn2.AsmRegsSaved, HW_xSI );
    HW_CTurnOff( front.pdefn2.AsmRegsSaved, HW_xDI );
#if _CPU == 8086
    HW_CTurnOff( front.pdefn2.AsmRegsSaved, HW_ES );
#endif

#if _CPU == 386
    /*
     * these are internal, and will never be pointed to by
     * an aux_entry, so we don't have to worry about them
     * or their fields being freed
     */
    front.pragdefn.STOSBInfo = WatcallInfo;
    front.pragdefn.STOSBInfo.cclass        = FECALL_GEN_NONE;
    front.pragdefn.STOSBInfo.cclass_target = FECALL_X86_NO_FLOAT_REG_RETURNS |
                              FECALL_X86_NO_STRUCT_REG_RETURNS |
                              FECALL_X86_SPECIAL_STRUCT_RETURN |
                              FECALL_X86_NONE;
    front.pragdefn.STOSBInfo.parms = front.cpragx86.STOSBParms;
    front.pragdefn.STOSBInfo.objname = const_cast<char *>("*");
#endif
}

void PragmaInit( FrontRun& front )
/*********************/
{
    int         cpu;
    int         fpu;

    pragmaAuxInfoInit(front);

    switch( GET_CPU( front.cvars.ProcRevision ) ) {
    case CPU_86:    cpu = 0; break;
    case CPU_186:   cpu = 1; break;
    case CPU_286:   cpu = 2; break;
    case CPU_386:   cpu = 3; break;
    case CPU_486:   cpu = 4; break;
    case CPU_586:   cpu = 5; break;
    case CPU_686:   cpu = 6; break;
#if _CPU == 8086
    default:        cpu = 0; break;
#else
    default:        cpu = 3; break;
#endif
    }
    switch( GET_FPU_LEVEL( front.cvars.ProcRevision ) ) {
    case FPU_NONE:  fpu = 0; break;
    case FPU_87:    fpu = 1; break;
//    case FPU_287:   fpu = 2; break;
    case FPU_387:   fpu = 3; break;
    case FPU_586:   fpu = 3; break;
    case FPU_686:   fpu = 3; break;
#if _CPU == 8086
    default:        fpu = 1; break;
#else
    default:        fpu = 3; break;
#endif
    }

#if _CPU == 8086
    AsmEnvInit( *front.assembler, 0, cpu, fpu, GET_FPU_EMU( front.cvars.ProcRevision ) );
#else
    AsmEnvInit( *front.assembler, 1, cpu, fpu, false );
#endif
}


void PragmaFini( void )
/*********************/
{
}

static void InitAuxInfo( FrontRun& front )
/*****************************/
{
    front.pragdefn.CurrAlias = NULL;
    front.pragdefn.CurrInfo = NULL;
    front.pragdefn.CurrEntry = NULL;

    memset( &front.cpragx86.AuxInfo, 0, sizeof( front.cpragx86.AuxInfo ) );

    front.cpragx86.AuxInfoFlg.f_near = false;
    front.cpragx86.AuxInfoFlg.f_routine_pops = false;
    front.cpragx86.AuxInfoFlg.f_caller_return = false;
    front.cpragx86.AuxInfoFlg.f_8087_returns = false;
}

static void PragmaAuxEnd( FrontRun& front )
/******************************/
{
    hw_reg_set      default_flt_n_seg;
    hw_reg_set      flt_n_seg;

    if( front.pragdefn.CurrEntry == NULL ) {
        /*
         * Redefining a built-in calling convention
         */
    } else {
        front.pragdefn.CurrInfo = (aux_info *)CMemAlloc( front, sizeof( aux_info ) );
        *front.pragdefn.CurrInfo = *front.pragdefn.CurrAlias;
    }
    if( front.cpragx86.AuxInfo.code != NULL ) {
        front.pragdefn.CurrInfo->code = front.cpragx86.AuxInfo.code;
    }
    if( front.cpragx86.AuxInfoFlg.f_near ) {
        front.pragdefn.CurrInfo->cclass_target &= ~FECALL_X86_FAR_CALL;
    }
    if( front.cpragx86.AuxInfoFlg.f_routine_pops ) {
        front.pragdefn.CurrInfo->cclass &= ~FECALL_GEN_CALLER_POPS;
    }
    if( front.cpragx86.AuxInfoFlg.f_caller_return ) {
        front.pragdefn.CurrInfo->cclass_target &= ~FECALL_X86_ROUTINE_RETURN;
    }
    if( front.cpragx86.AuxInfoFlg.f_8087_returns ) {
        front.pragdefn.CurrInfo->cclass_target &= ~FECALL_X86_NO_8087_RETURNS;
    }
    front.pragdefn.CurrInfo->cclass        |= front.cpragx86.AuxInfo.cclass;
    front.pragdefn.CurrInfo->cclass_target |= front.cpragx86.AuxInfo.cclass_target;

    front.pragdefn.CurrInfo->flags |= front.cpragx86.AuxInfo.flags;
    if( front.cpragx86.AuxInfo.objname != NULL )
        front.pragdefn.CurrInfo->objname = front.cpragx86.AuxInfo.objname;
    if( front.cpragx86.AuxInfo.cclass_target & FECALL_X86_SPECIAL_RETURN )
        front.pragdefn.CurrInfo->returns = front.cpragx86.AuxInfo.returns;
    if( front.cpragx86.AuxInfo.cclass_target & FECALL_X86_SPECIAL_STRUCT_RETURN )
        front.pragdefn.CurrInfo->streturn = front.cpragx86.AuxInfo.streturn;
    if( front.cpragx86.AuxInfo.parms != NULL )
        front.pragdefn.CurrInfo->parms = front.cpragx86.AuxInfo.parms;

    if( !HW_CEqual( front.cpragx86.AuxInfo.save, HW_EMPTY ) ) {
        HW_CTurnOn( front.pragdefn.CurrInfo->save, HW_FULL );
        if( (front.cpragx86.AuxInfo.cclass_target & FECALL_X86_MODIFY_EXACT) == 0 && !front.cvars.CompFlags.save_restore_segregs ) {
            HW_Asgn( default_flt_n_seg, WatcallInfo.save );
            HW_CAsgn( flt_n_seg, HW_FLTS );
            HW_CTurnOn( flt_n_seg, HW_SEGS );
            HW_TurnOff( front.pragdefn.CurrInfo->save, flt_n_seg );
            HW_OnlyOn( default_flt_n_seg, flt_n_seg );
            HW_TurnOn( front.pragdefn.CurrInfo->save, default_flt_n_seg );
        }
        HW_TurnOff( front.pragdefn.CurrInfo->save, front.cpragx86.AuxInfo.save );
    }
    PragmaAuxEnding(front);
}

bool GetPragmaAuxAlias( FrontRun& front )
{
    bool    isfar16;

    isfar16 = PragRecogId( front, "far16" );
    if( IS_ID_OR_KEYWORD( front.cvars.CurToken ) ) {
        front.pragdefn.CurrAlias = PragmaAuxAlias( front, front.cvars.Buffer );
        PPNextToken(front);
    }
    if( front.cvars.CurToken == T_RIGHT_PAREN )
        PPNextToken(front);
    if( isfar16 )
        front.cpragx86.AuxInfo.flags |= AUX_FLAG_FAR16;
    PragmaAuxEnd(front);
    return( true );
}

typedef enum {
    FIXWORD_NONE,
    FIXWORD_FLOAT,
    FIXWORD_SEGMENT,
    FIXWORD_OFFSET,
    FIXWORD_RELOFF
} fix_words;

static fix_words FixupKeyword( FrontRun& front )
/***********************************/
{
    fix_words retn;             // - return

    if( PragRecogId( front, "float" ) ) {
        retn = FIXWORD_FLOAT;
    } else if( PragRecogId( front, "seg" ) ) {
        retn = FIXWORD_SEGMENT;
    } else if( PragRecogId( front, "offset" ) ) {
        retn = FIXWORD_OFFSET;
    } else if( PragRecogId( front, "reloff" ) ) {
        retn = FIXWORD_RELOFF;
    } else {
        retn = FIXWORD_NONE;
    }
    return( retn );
}


#if _CPU == 8086
    #define SYM_INT     SYM_INT2
    #define SYM_FFAR    SYM_FAR2
    #define SYM_FNEAR   SYM_NEAR2
    #define SYM_DFAR    SYM_INT4
    #define SYM_DNEAR   SYM_INT2
#else
    #define SYM_INT     SYM_INT4
    #define SYM_FFAR    SYM_FAR4
    #define SYM_FNEAR   SYM_NEAR2
    #define SYM_DFAR    SYM_INT6
    #define SYM_DNEAR   SYM_INT4
#endif

static int AsmCodePtrType( FrontRun& front, type_modifiers flags )
/***********************************************/
{
    if( flags & FLAG_FAR ) {
        return( SYM_FFAR );
    } else if( flags & FLAG_NEAR ) {
        return( SYM_FNEAR );
    } else if( front.cvars.TargetSwitches & CGSW_X86_BIG_CODE ) {
        return( SYM_FFAR );
    } else {
        return( SYM_FNEAR );
    }
}


static int AsmPtrType( FrontRun& front, TYPEPTR typ, type_modifiers flags )
/********************************************************/
{

    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_FUNCTION ) {
        return( AsmCodePtrType( front, flags ) );
    } else if( flags & (FLAG_FAR | FLAG_HUGE) ) {
        return( SYM_DFAR );
    } else if( flags & FLAG_NEAR ) {
        return( SYM_DNEAR );
    } else if( front.cvars.TargetSwitches & CGSW_X86_BIG_DATA ) {
        return( SYM_DFAR );
    } else {
        return( SYM_DNEAR );
    }
}

/*
 * matches enum DataType in ctypes.h
 */
static enum sym_type const AsmDataType[] = {
    #define pick1(type,dtype,cgtype,x86asmtype,name,size) x86asmtype,
    #include "cdatatyp.h"
    #undef  pick1
};

static int AsmType( FrontRun& front, TYPEPTR typ, type_modifiers flags )
/****************************************************/
{
    SKIP_TYPEDEFS( typ );
    switch( typ->decl_type ) {
    case TYP_STRUCT:
    case TYP_UNION:
        return( SYM_INT1 );
    case TYP_ARRAY:
        return( AsmType( front, typ->object, flags ) );
    case TYP_FIELD:
    case TYP_UFIELD:
        return( AsmDataType[typ->u.f.field_type] );
    case TYP_FUNCTION:
        return( AsmCodePtrType( front, flags ) );
    case TYP_POINTER:
        return( AsmPtrType( front, typ->object, typ->u.p.decl_flags ) );
    case TYP_ENUM:
        typ = typ->object;
        /* fall through */
    default:
        return( AsmDataType[typ->decl_type] );
    }
}

enum sym_type AsmQueryType( FrontRun& front, void *handle )
/****************************************/
{
    SYM_HANDLE  sym_handle = (SYM_HANDLE)handle;
    SYM_ENTRY   sym;

    if( sym_handle == SYM_NULL )
        return( SYM_INT1 );
    SymGet( front, &sym, sym_handle );
    return static_cast<enum sym_type>(( AsmType( front, sym.sym_type, sym.mods ) ));
}

void AsmUsesAuto( aux_info *info )
/********************************/
{
    info->cclass_target |= FECALL_X86_NEEDS_BP_CHAIN;
}

bool AsmInsertFixups( FrontRun& front, aux_info *info )
/************************************/
{
    byte_seq            *seq;
    byte_seq_len        len;
                        /* additional slop in buffer to simplify the code */
    unsigned char       temp[MAXIMUM_BYTESEQ + 1 + 2 + sizeof( BYTE_SEQ_SYM ) + sizeof( BYTE_SEQ_OFF )];
    struct asmfixup     *fix;
    struct asmfixup     *head;
    struct asmfixup     *chk;
    struct asmfixup     *next;
    struct asmfixup     **owner;
    unsigned char       *dst;
    unsigned char       *src;
    unsigned char       *end;
    bool                perform_fixups;
    unsigned char       cg_fix;
    SYM_HANDLE          sym_handle;
    SYM_ENTRY           sym;
    char                *name;
    unsigned            skip;
    bool                mutate_to_segment;
    bool                uses_auto;
#if _CPU == 8086
    bool                fixup_padding;
#endif

    src = front.asminlin->AsmCodeBuffer;
    len = front.asminlin->AsmCodeAddress;
    sym_handle = SYM_NULL;
    uses_auto = false;
    perform_fixups = false;
    head = front.asminlin->FixupHead;
    if( head != NULL ) {
        front.asminlin->FixupHead = NULL;
        /*
         * sort the fixup list in increasing fixup_loc's
         */
        for( fix = head; fix != NULL; fix = next ) {
            for( owner = &front.asminlin->FixupHead; (chk = *owner) != NULL; owner = &chk->next ) {
                if( chk->fixup_loc > fix->fixup_loc ) {
                    break;
                }
            }
            next = fix->next;
            fix->next = *owner;
            *owner = fix;
        }
        dst = temp;
        end = src + len;
        fix = front.asminlin->FixupHead;
        owner = &front.asminlin->FixupHead;
        /*
         * insert fixup escape sequences
         */
        while( src < end ) {
            if( fix != NULL && fix->fixup_loc == (src - front.asminlin->AsmCodeBuffer) ) {
                name = fix->name;
                if( name != NULL ) {
                    sym_handle = SymLook( front, CalcHashID( name ), name );
                    if( sym_handle == SYM_NULL ) {
                        CErr2p( front, ERR_UNDECLARED_SYM, name );
                        return( false );
                    }
                    SymGet( front, &sym, sym_handle );
                    sym.flags |= SYM_REFERENCED | SYM_ADDR_TAKEN;
                    switch( sym.attribs.stg_class ) {
                    case SC_REGISTER:
                    case SC_AUTO:
                        sym.flags |= SYM_USED_IN_PRAGMA;
                        front.cvars.CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
                        uses_auto = true;
                        break;
                    }
                    SymReplace( front, &sym, sym_handle );
                }
                /*
                 * insert fixup information
                 */
                skip = 0;
                *dst++ = FLOATING_FIXUP_BYTE;
                mutate_to_segment = false;
                cg_fix = 0;
#if _CPU == 8086
                fixup_padding = false;
#endif
                switch( fix->fixup_type ) {
                case FIX_FPPATCH:
                    *dst++ = fix->u_fppatch;
                    break;
                case FIX_SEG:
                    if( name == NULL ) {
                        /*
                         * special case for floating point fixup
                         */
                        if( ( src[0] == 0x90 ) && ( src[1] == 0x9B ) ) {
                            /*
                             * inline assembler FWAIT instruction 0x90, 0x9b
                             */
                            *dst++ = FIX_FPP_WAIT;
                        } else if( src[0] == 0x9b && (src[1] & 0xd8) == 0xd8 ) {
                            /*
                             * FWAIT as first byte and FPU instruction opcode as second byte
                             */
                            *dst++ = FIX_FPP_NORMAL;
                        } else if( src[0] == 0x9b && (src[2] & 0xd8) == 0xd8 ) {
                            /*
                             * FWAIT as first byte and FPU instruction opcode as third byte
                             * second byte should be segment override prefix
                             */
                            switch( src[1] ) {
                            case PREFIX_ES: *dst++ = FIX_FPP_ES;    break;
                            case PREFIX_CS: *dst++ = FIX_FPP_CS;    break;
                            case PREFIX_SS: *dst++ = FIX_FPP_SS;    break;
                            case PREFIX_DS: *dst++ = FIX_FPP_DS;    break;
                            case PREFIX_GS: *dst++ = FIX_FPP_GS;    break;
                            case PREFIX_FS: *dst++ = FIX_FPP_FS;    break;
                            default: --dst; break;  // skip FP patch
                            }
                        } else {
                            /*
                             * skip FP patch
                             */
                            --dst;
                        }
                    } else {
                        skip = 2;
                        cg_fix = FIX_SYM_SEGMENT;
                    }
                    break;
                case FIX_RELOFF16:
                    skip = 2;
                    cg_fix = FIX_SYM_RELOFF;
                    break;
                case FIX_RELOFF32:
                    skip = 4;
                    cg_fix = FIX_SYM_RELOFF;
#if _CPU == 8086
                    fixup_padding = true;
#endif
                    break;
                case FIX_PTR16:
                    mutate_to_segment = true;
                    /* fall through */
                case FIX_OFF16:
                    skip = 2;
                    cg_fix = FIX_SYM_OFFSET;
                    break;
                case FIX_PTR32:
                    mutate_to_segment = true;
                    /* fall through */
                case FIX_OFF32:
                    skip = 4;
                    cg_fix = FIX_SYM_OFFSET;
#if _CPU == 8086
                    fixup_padding = true;
#endif
                    break;
                default:
                    CErr2p( front, ERR_UNREACHABLE_SYM, name );
                    break;
                }
                if( skip != 0 ) {
                    *dst++ = cg_fix;
                    memcpy( dst, &sym_handle, sizeof( BYTE_SEQ_SYM ) );
                    dst += sizeof( BYTE_SEQ_SYM );
                    memcpy( dst, &fix->u_offset, sizeof( BYTE_SEQ_OFF ) );
                    dst += sizeof( BYTE_SEQ_OFF );
                    src += skip;
                }
#if _CPU == 8086
                if( fixup_padding ) {
                    /*
                     * add offset fixup padding to 32-bit
                     * cg create only 16-bit offset fixup
                     */
                    *dst++ = 0;
                    *dst++ = 0;
                    //
                }
#endif
                if( mutate_to_segment ) {
                    /*
                     * Since the CG escape sequences don't allow for
                     * FAR pointer fixups, we have to split them into two.
                     * This is done by doing the offset fixup first, then
                     * mutating the fixup structure to look like a segment
                     * fixup one near pointer size later.
                     */
                    fix->fixup_type = FIX_SEG;
                    fix->fixup_loc += skip;
                    fix->u_offset = 0;
                } else {
                    head = fix;
                    fix = fix->next;
                    if( head->external ) {
                        *owner = fix;
                        if( head->name != NULL ) {
                            CMemFree( front, head->name );
                        }
                        CMemFree( front, head );
                    } else {
                        owner = &head->next;
                    }
                }
            } else {
                if( *src == FLOATING_FIXUP_BYTE ) {
                    *dst++ = FLOATING_FIXUP_BYTE;
                }
                *dst++ = *src++;
            }
            if( dst > &temp[MAXIMUM_BYTESEQ] ) {
                CErr1( front, ERR_TOO_MANY_BYTES_IN_PRAGMA );
                return( false );
            }
        }
        src = temp;
        len = (byte_seq_len)( dst - temp );
        perform_fixups = true;
    }
    seq = (byte_seq *)CMemAlloc( front, offsetof( byte_seq, data ) + len );
    seq->relocs = perform_fixups;
    seq->length = len;
    memcpy( &seq->data[0], src, len );
    info->code = seq;
    return( uses_auto );
}

static void AddAFix( FrontRun& front, unsigned loc, char *name, unsigned type, unsigned off )
/**************************************************************************/
{
    struct asmfixup     *fix;

    fix = (struct asmfixup *)CMemAlloc( front, sizeof( *fix ) );
    fix->external = 1;
    fix->fixup_loc = loc;
    fix->name = name;
    fix->u_offset = off;
    fix->fixup_type = static_cast<fixup_types>(type);
    fix->next = front.asminlin->FixupHead;
    front.asminlin->FixupHead = fix;
}

void AsmSysLine( FrontRun& front, const char *buff )
/*********************************/
{
#if _CPU == 8086
    AsmLine( *front.assembler, buff, GET_FPU_EMU( front.cvars.ProcRevision ) );
#else
    AsmLine( *front.assembler, buff, false );
#endif
}

static bool checkEnum( FrontRun& front, unsigned char *byte_value )
/************************************************/
{
    id_hash_idx hash;
    SYM_HANDLE  sym_handle;
    ENUMPTR     ep;

    if( front.cvars.CurToken == T_ID ) {
        hash = CalcHashID( front.cvars.Buffer );
        sym_handle = SymLook( front, hash, front.cvars.Buffer );
        if( sym_handle == NULL ) {
            ep = EnumLookup( front, hash, front.cvars.Buffer );
            if( ep != NULL ) {
                *byte_value = (unsigned char)ep->value.u._64[0];
                return( true );
            }
        }
    }
    return( false );
}

static bool GetByteSeq( FrontRun& front, aux_info *info )
/**************************************/
{
    unsigned char       buff[MAXIMUM_BYTESEQ + ASM_BLOCK];
    char                *name;
    unsigned            offset;
    fix_words           fixword;
    bool                uses_auto;
    bool                too_many_bytes;
    unsigned char       byte_value;
#if _CPU == 8086
    bool                use_fpu_emu = false;
#endif

    AsmSysInit( front, buff );
    PPNextToken(front);
    too_many_bytes = false;
    offset = 0;
    name = NULL;
    for( ;; ) {
        if( front.cvars.CurToken == T_STRING ) {
#if _CPU == 8086
            AsmLine( *front.assembler, front.cvars.Buffer, use_fpu_emu );
            use_fpu_emu = false;
#else
            AsmLine( *front.assembler, front.cvars.Buffer, false );
#endif
            PPNextToken(front);
            if( front.cvars.CurToken == T_COMMA ) {
                PPNextToken(front);
            }
        } else if( front.cvars.CurToken == T_CONSTANT ) {
#if _CPU == 8086
            if( use_fpu_emu ) {
                AddAFix( front, front.asminlin->AsmCodeAddress, NULL, FIX_SEG, 0 );
                use_fpu_emu = false;
            }
#endif
            front.asminlin->AsmCodeBuffer[front.asminlin->AsmCodeAddress++] = U8FetchTrunc( front.cvars.Constant64 );
            PPNextToken(front);
        } else if( checkEnum( front, &byte_value ) ) {
            front.asminlin->AsmCodeBuffer[front.asminlin->AsmCodeAddress++] = byte_value;
            PPNextToken(front);
        } else {
#if _CPU == 8086
            use_fpu_emu = false;
#endif
            fixword = FixupKeyword(front);
            if( fixword == FIXWORD_NONE )
                break;
            if( fixword == FIXWORD_FLOAT ) {
#if _CPU == 8086
                if( GET_FPU_EMU( front.cvars.ProcRevision ) ) {
                    use_fpu_emu = true;
                }
#endif
            } else { /* seg or offset */
                if( !IS_ID_OR_KEYWORD( front.cvars.CurToken ) ) {
                    CErr1( front, ERR_EXPECTING_ID );
                } else {
                    name = CMemStrdup( front, front.cvars.Buffer );
                    PPNextToken(front);
                    if( front.cvars.CurToken == T_PLUS ) {
                        PPNextToken(front);
                        if( front.cvars.CurToken == T_CONSTANT ) {
                            offset = U32FetchTrunc( front.cvars.Constant64 );
                            PPNextToken(front);
                        }
                    } else if( front.cvars.CurToken == T_MINUS ) {
                        PPNextToken(front);
                        if( front.cvars.CurToken == T_CONSTANT ) {
                            offset = -(int)U32FetchTrunc( front.cvars.Constant64 );
                            PPNextToken(front);
                        }
                    }
                }
                switch( fixword ) {
                case FIXWORD_RELOFF:
#if _CPU == 8086
                    AddAFix( front, front.asminlin->AsmCodeAddress, name, FIX_RELOFF16, offset );
                    front.asminlin->AsmCodeAddress += 2;
#else
                    AddAFix( front, front.asminlin->AsmCodeAddress, name, FIX_RELOFF32, offset );
                    front.asminlin->AsmCodeAddress += 4;
#endif
                    break;
                case FIXWORD_OFFSET:
#if _CPU == 8086
                    AddAFix( front, front.asminlin->AsmCodeAddress, name, FIX_OFF16, offset );
                    front.asminlin->AsmCodeAddress += 2;
#else
                    AddAFix( front, front.asminlin->AsmCodeAddress, name, FIX_OFF32, offset );
                    front.asminlin->AsmCodeAddress += 4;
#endif
                    break;
                case FIXWORD_SEGMENT:
                    AddAFix( front, front.asminlin->AsmCodeAddress, name, FIX_SEG, 0 );
                    front.asminlin->AsmCodeAddress += 2;
                    break;
                }
            }
        }
        if( front.asminlin->AsmCodeAddress > MAXIMUM_BYTESEQ ) {
            if( !too_many_bytes ) {
                CErr1( front, ERR_TOO_MANY_BYTES_IN_PRAGMA );
                too_many_bytes = true;
            }
            front.asminlin->AsmCodeAddress = 0;          // reset index to we don't overrun buffer
        }
    }
    if( too_many_bytes ) {
        uses_auto = false;
    } else {
        uses_auto = AsmInsertFixups( front, info );
    }
    AsmSysFini(front);
    return( uses_auto );
}


hw_reg_set PragRegName( FrontRun& front, const char *regname )
/*******************************************/
{
    int             index;
    hw_reg_set      name;

    if( *regname != '\0' ) {
        /*
         * search register or alias name
         */
        index = PragRegIndex( Registers, regname, true );
        if( index != -1 ) {
            return( RegBits[RegMap[index]] );
        }
        if( strcmp( regname, "8087" ) == 0 ) {
            HW_CAsgn( name, HW_FLTS );
            return( name );
        }
        PragRegNameErr( front, regname );
    }
    HW_CAsgn( name, HW_EMPTY );
    return( name );
}

hw_reg_set PragReg( FrontRun& front )
/************************/
{
    char            buffer[REG_BUFF_SIZE];
    size_t          len;
    const char      *p;

    p = SkipUnderscorePrefix( front, front.cvars.Buffer );
    if( p == NULL ) {
        /*
         * error, missing undercore prefix
         */
        PragRegNameErr( front, front.cvars.Buffer );
        p = front.cvars.Buffer;
    }
    len = 0;
    while( *p != '\0' && len < ( sizeof( buffer ) - 1 ) ) {
        buffer[len++] = *p++;
    }
    buffer[len] = '\0';
    return( PragRegName( front, buffer ) );
}

static void GetParmInfo( FrontRun& front )
/****************************/
{
    struct {
        bool     f_pop       : 1;
        bool     f_reverse   : 1;
        bool     f_loadds    : 1;
        bool     f_nomemory  : 1;
        bool     f_list      : 1;
    } have;

    have.f_pop = false;
    have.f_reverse = false;
    have.f_loadds = false;
    have.f_nomemory = false;
    have.f_list = false;
    for( ;; ) {
        if( !have.f_pop && PragRecogId( front, "caller" ) ) {
            front.cpragx86.AuxInfo.cclass |= FECALL_GEN_CALLER_POPS;
            have.f_pop = true;
        } else if( !have.f_pop && PragRecogId( front, "routine" ) ) {
            front.cpragx86.AuxInfo.cclass &= ~ FECALL_GEN_CALLER_POPS;
            front.cpragx86.AuxInfoFlg.f_routine_pops = true;
            have.f_pop = true;
        } else if( !have.f_reverse && PragRecogId( front, "reverse" ) ) {
            front.cpragx86.AuxInfo.cclass |= FECALL_GEN_REVERSE_PARMS;
            have.f_reverse = true;
        } else if( !have.f_nomemory && PragRecogId( front, "nomemory" ) ) {
            front.cpragx86.AuxInfo.cclass |= FECALL_GEN_NO_MEMORY_READ;
            have.f_nomemory = true;
        } else if( !have.f_loadds && PragRecogId( front, "loadds" ) ) {
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_LOAD_DS_ON_CALL;
            have.f_loadds = true;
        } else if( !have.f_list && PragRegSet(front) != T_NULL ) {
            front.cpragx86.AuxInfo.parms = PragManyRegSets(front);
            have.f_list = true;
        } else {
            break;
        }
    }
}


static void GetSTRetInfo( FrontRun& front )
/*****************************/
{
    struct {
        bool     f_float     : 1;
        bool     f_struct    : 1;
        bool     f_allocs    : 1;
        bool     f_list      : 1;
    } have;

    have.f_float = false;
    have.f_struct = false;
    have.f_allocs = false;
    have.f_list = false;
    for( ;; ) {
        if( !have.f_float && PragRecogId( front, "float" ) ) {
            have.f_float = true;
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_NO_FLOAT_REG_RETURNS;
        } else if( !have.f_struct && PragRecogId( front, "struct" ) ) {
            have.f_struct = true;
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_NO_STRUCT_REG_RETURNS;
        } else if( !have.f_allocs && PragRecogId( front, "routine" ) ) {
            have.f_allocs = true;
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_ROUTINE_RETURN;
        } else if( !have.f_allocs && PragRecogId( front, "caller" ) ) {
            have.f_allocs = true;
            front.cpragx86.AuxInfo.cclass_target &= ~FECALL_X86_ROUTINE_RETURN;
            front.cpragx86.AuxInfoFlg.f_caller_return = true;
        } else if( !have.f_list && PragRegSet(front) != T_NULL ) {
            have.f_list = true;
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_SPECIAL_STRUCT_RETURN;
            front.cpragx86.AuxInfo.streturn = PragRegList(front);
        } else {
            break;
        }
    }
}


static void GetRetInfo( FrontRun& front )
/***************************/
{
    struct {
        bool     f_no8087    : 1;
        bool     f_list      : 1;
        bool     f_struct    : 1;
    } have;

    have.f_no8087 = false;
    have.f_list = false;
    have.f_struct = false;
    front.cpragx86.AuxInfo.cclass_target &= ~ FECALL_X86_NO_8087_RETURNS;
    front.cpragx86.AuxInfoFlg.f_8087_returns = true;
    for( ;; ) {
        if( !have.f_no8087 && PragRecogId( front, "no8087" ) ) {
            have.f_no8087 = true;
            HW_CTurnOff( front.cpragx86.AuxInfo.returns, HW_FLTS );
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_NO_8087_RETURNS;
        } else if( !have.f_list && PragRegSet(front) != T_NULL ) {
            have.f_list = true;
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_SPECIAL_RETURN;
            front.cpragx86.AuxInfo.returns = PragRegList(front);
        } else if( !have.f_struct && PragRecogId( front, "struct" ) ) {
            have.f_struct = true;
            GetSTRetInfo(front);
        } else {
            break;
        }
    }
}


static void GetSaveInfo( FrontRun& front )
/****************************/
{
    struct {
        bool     f_exact     : 1;
        bool     f_nomemory  : 1;
        bool     f_list      : 1;
    } have;

    have.f_exact = false;
    have.f_nomemory = false;
    have.f_list = false;
    for( ;; ) {
        if( !have.f_exact && PragRecogId( front, "exact" ) ) {
            front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_MODIFY_EXACT;
            have.f_exact = true;
        } else if( !have.f_nomemory && PragRecogId( front, "nomemory" ) ) {
            front.cpragx86.AuxInfo.cclass |= FECALL_GEN_NO_MEMORY_CHANGED;
            have.f_nomemory = true;
        } else if( !have.f_list && PragRegSet(front) != T_NULL ) {
            HW_TurnOn( front.cpragx86.AuxInfo.save, PragRegList(front) );
            have.f_list = true;
        } else {
            break;
        }
    }
}

void PragAux( FrontRun& front )
/******************/
{
    struct {
        bool     f_call      : 1;
        bool     f_loadds    : 1;
        bool     f_rdosdev   : 1;
        bool     f_export    : 1;
        bool     f_parm      : 1;
        bool     f_value     : 1;
        bool     f_modify    : 1;
        bool     f_frame     : 1;
        bool     uses_auto   : 1;
    } have;

    InitAuxInfo(front);
    PPCTL_ENABLE_MACROS();
    PPNextToken(front);
    if( GetPragmaAuxAliasInfo(front) ) {
        SetCurrInfo( front, front.cvars.Buffer );
        PPNextToken(front);
        PragObjNameInfo( front, &front.cpragx86.AuxInfo.objname );
        have.f_call = false;
        have.f_loadds = false;
        have.f_rdosdev = false;
        have.f_export = false;
        have.f_parm = false;
        have.f_value = false;
        have.f_modify = false;
        have.f_frame = false;
        have.uses_auto = false;
        for( ;; ) {
            if( !have.f_call && front.cvars.CurToken == T_EQUAL ) {
                have.uses_auto = GetByteSeq( front, &front.cpragx86.AuxInfo );
                have.f_call = true;
            } else if( !have.f_call && PragRecogId( front, "far" ) ) {
                front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_FAR_CALL;
                have.f_call = true;
            } else if( !have.f_call && PragRecogId( front, "near" ) ) {
                front.cpragx86.AuxInfo.cclass_target &= ~FECALL_X86_FAR_CALL;
                front.cpragx86.AuxInfoFlg.f_near = true;
                have.f_call = true;
            } else if( !have.f_loadds && PragRecogId( front, "loadds" ) ) {
                front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_LOAD_DS_ON_ENTRY;
                have.f_loadds = true;
            } else if( !have.f_rdosdev && PragRecogId( front, "rdosdev" ) ) {
                front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_LOAD_RDOSDEV_ON_ENTRY;
                have.f_rdosdev = true;
            } else if( !have.f_export && PragRecogId( front, "export" ) ) {
                front.cpragx86.AuxInfo.cclass |= FECALL_GEN_DLL_EXPORT;
                have.f_export = true;
            } else if( !have.f_parm && PragRecogId( front, "parm" ) ) {
                GetParmInfo(front);
                have.f_parm = true;
            } else if( !have.f_value && PragRecogId( front, "value" ) ) {
                GetRetInfo(front);
                have.f_value = true;
            } else if( !have.f_value && PragRecogId( front, "aborts" ) ) {
                front.cpragx86.AuxInfo.cclass |= FECALL_GEN_ABORTS;
                have.f_value = true;
            } else if( !have.f_modify && PragRecogId( front, "modify" ) ) {
                GetSaveInfo(front);
                have.f_modify = true;
            } else if( !have.f_frame && PragRecogId( front, "frame" ) ) {
                front.cpragx86.AuxInfo.cclass_target |= FECALL_X86_GENERATE_STACK_FRAME;
                have.f_frame = true;
            } else {
                break;
            }
        }
        if( have.uses_auto ) {
            AsmUsesAuto( &front.cpragx86.AuxInfo );
        }
        PragmaAuxEnd(front);
    }
    PPCTL_DISABLE_MACROS();
}

void AsmSysInit( FrontRun& front, unsigned char *buf )
/***********************************/
{
    AsmInit(*front.assembler);
    front.asminlin->AsmCodeBuffer = buf;
    front.asminlin->AsmCodeLimit = MAXIMUM_BYTESEQ;
    front.asminlin->AsmCodeAddress = 0;
}

void AsmSysFini( FrontRun& front )
/*********************/
{
    AsmFiniRelocs(*front.assembler);
    AsmFini(*front.assembler);
}

char const *AsmSysDefineByte( void )
/**********************************/
{
    return( "db " );
}
