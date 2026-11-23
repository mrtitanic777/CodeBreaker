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
* Description:  Intel x86 procedure call generation.
*
****************************************************************************/


#include <stdio.h>
#include "_cgstd.h"
#include "coderep.h"
#include "cgaux.h"
#include "stackcg.h"
#include "zoiks.h"
#include "data.h"
#include "display.h"
#include "rtrtn.h"
#include "utils.h"
#include "objout.h"
#include "dbsyms.h"
#include "flowsave.h"
#include "object.h"
#include "x86enc2.h"
#include "encode.h"
#include "targetin.h"
#include "targetdb.h"
#include "x86proc.h"
#include "opttell.h"
#include "i87data.h"
#include "x86objd.h"
#include "x86obj.h"
#include "optab.h"
#include "rgtbl.h"
#include "x86base.h"
#include "pccode.h"
#include "pcencode.h"
#include "x86enc.h"
#include "x86temps.h"
#include "feprotos.h"
#include "backrun.hpp"


inline int WordsCount( int size ) { return (((size) + WORD_SIZE - 1) / WORD_SIZE); }

static CB_ALWAYS_INLINE bool WindowsCheap( BackRun& back )
{
    return ( IsModel( back, CGSW_GEN_DLL_RESIDENT_CODE )
             && (back.data.CurrProc->state.attr & ROUTINE_LOADS_DS) )
        || ( IsTargetModel( back, CGSW_X86_CHEAP_WINDOWS )
             && (back.data.CurrProc->prolog_state & PST_EXPORT) == 0 )
        || ( IsTargetModel( back, CGSW_X86_WINDOWS )
             && (back.data.CurrProc->prolog_state & (PST_EXPORT | PST_PROLOG_FAT)) == 0 );
}

static CB_ALWAYS_INLINE bool DoWindowsCrap( BackRun& back )
{
    return IsTargetModel( back, CGSW_X86_WINDOWS )
        && ( !WindowsCheap( back ) || back.data.CurrProc->contains_call );
}

static CB_ALWAYS_INLINE bool DoBpChain( BackRun& back )
{
    return ( ( IsTargetModel( back, CGSW_X86_NEED_STACK_FRAME )
               || (back.data.CurrProc->state.attr & ROUTINE_NEEDS_BP_CHAIN)
               || IsModel( back, CGSW_GEN_DBG_CV ) )
             && back.data.CurrProc->contains_call )
        || (back.data.CurrProc->prolog_state & PST_PROLOG_FAT);
}

static CB_ALWAYS_INLINE bool ChainFrame( BackRun& back ) { return DoWindowsCrap( back ) || DoBpChain( back ); }

static CB_ALWAYS_INLINE bool CheapFrame( BackRun& back )
{
    return IsTargetModel( back, CGSW_X86_NEED_STACK_FRAME )
        || IsntTargetModel( back, CGSW_X86_WINDOWS )
        || WindowsCheap( back );
}

static CB_ALWAYS_INLINE bool FarRetOnStack( BackRun& back )
{
    return RoutineIsLong( back.data.CurrProc->state.attr )
        && (back.data.CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS) == 0;
}


#define HW_STACK_CHECK HW_xAX
#define HW_LOAD_DS     HW_xAX


extern const hw_reg_set PushRegs[] = {
    HW_D( HW_xAX ),
    HW_D( HW_xBX ),
    HW_D( HW_xCX ),
    HW_D( HW_xDX ),
    HW_D( HW_xSI ),
    HW_D( HW_xDI ),
    HW_D( HW_DS ),
    HW_D( HW_ES ),
    HW_D( HW_FS ),
    HW_D( HW_GS ),
    HW_D( HW_SS ),
    HW_D( HW_xBP ),
    HW_D( HW_EMPTY )
};


/* Is any stack parameter of the routine referenced by an instruction? (set by ScanInstructions: a routine whose parameters are
   never read needs no BP to address them, only the callee's `ret n` to remove them) */
namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct X86procFileState: x86proc_state.h) */

}  // namespace



static  bool    OpUsesStackParm( name *op )
/*****************************************/
{
    if( op == NULL )
        return( false );
    if( op->n.class_ == N_TEMP )
        return( (op->t.temp_flags & STACK_PARM) != 0 );
    if( op->n.class_ == N_INDEXED ) {
        if( OpUsesStackParm( op->i.index ) )
            return( true );
        if( op->i.base != NULL && OpUsesStackParm( op->i.base ) )
            return( true );
    }
    return( false );
}


static  bool    ScanInstructions( BackRun& back )
/**************************************/
{
    block       *blk;
    instruction *ins;
    name        *addr;
    bool        sp_constant;

    back.data.CurrProc->contains_call = true;
    back.x86proc_file.ParmsReferenced = true;
    if( back.data.BlockByBlock )
        return( false );
    back.data.CurrProc->contains_call = false;
    back.x86proc_file.ParmsReferenced = false;
    sp_constant = true;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( ins->head.opcode != OP_PARM_DEF ) {
                opcnt   k;

                for( k = ins->num_operands; k-- > 0; ) {
                    if( OpUsesStackParm( ins->operands[k] ) ) {
                        back.x86proc_file.ParmsReferenced = true;
                    }
                }
                if( OpUsesStackParm( ins->result ) ) {
                    back.x86proc_file.ParmsReferenced = true;
                }
            }
            if( OpIsCall( ins->head.opcode ) ) {
                if( ins->head.opcode == OP_CALL_INDIRECT ) {
                    back.data.CurrProc->contains_call = true;
                } else {
                    addr = ins->operands[CALL_OP_ADDR];
                    if( addr->n.class_ != N_MEMORY
                      || addr->m.memory_type != CG_LBL
                      || !AskIfRTLabel( back, static_cast<label_handle>(addr->v.symbol) ) ) {
                        back.data.CurrProc->contains_call = true;
                    }
                }
                if( HW_COvlap( ins->zap->reg, HW_xSP ) ) {
                    back.data.CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
                    sp_constant = false;
                }
                if( ins->flags.u.call_flags & CALL_NEEDS_BP_CHAIN ) {
                    back.data.CurrProc->state.attr |= ROUTINE_NEEDS_BP_CHAIN;
                }
            }
        }
    }
    return( sp_constant );
}


#if _TARGET & _TARG_80386
static  void    ChkFDOp( BackRun& back, name *op, level_depth depth )
/****************************************************/
{
    if( op->n.class_ != N_TEMP )
        return;
    if( (op->v.usage & (USE_IN_ANOTHER_BLOCK | USE_ADDRESS)) == 0 )
        return;
    if( op->t.temp_flags & STACK_PARM )
        return;
    if( op->t.location != NO_LOCATION ) {
        if( op->v.usage & (USE_ADDRESS | HAS_MEMORY) )
            return;
        if( op->t.alias != op )
            return;
        if( depth == 0 )
            return;
        op->t.location = NO_LOCATION;
    }
    op->t.temp_flags |= USED_AS_FD;
    back.data.CurrProc->targ.has_fd_temps = true;
}
#endif


#if _TARGET & _TARG_80386
static  void    ScanForFDOps( BackRun& back )
/**********************************/
{
    block       *blk;
    instruction *ins;
    opcnt       i;
    level_depth depth;

    back.data.CurrProc->contains_call = false;
    if( back.data.BlockByBlock )
        return;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        depth = blk->depth;
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( ins->type_class == FD
              || ins->type_class == FL ) {
                for( i = ins->num_operands; i-- > 0; ) {
                    ChkFDOp( back, ins->operands[i], depth );
                }
                if( ins->result != NULL ) {
                    ChkFDOp( back, ins->result, depth );
                }
            }
        }
    }
}
#endif


#if _TARGET & _TARG_80386
static  block *ScanForLabelReturn( BackRun& back, block *blk )
/*********************************************/
{
    block       *son;
    block_num   i;

    if( IsBlkAttr( blk, BLK_RETURN | BLK_CALL_LABEL ) )
        return( NULL );
    blk->edge[0].flags |= BEF_DOWN_ONE_CALL;
    if( IsBlkAttr( blk, BLK_LABEL_RETURN ) )
        return( blk );
    for( i = 0; i < blk->targets; ++i ) {
        son = blk->edge[i].destination.u.blk;
        if( son->edge[0].flags & BEF_DOWN_ONE_CALL )
            continue;
        if( SafeRecurseCG( back, (func_sr)ScanForLabelReturn, son ) == NULL ) {
            return( NULL );
        }
    }
    return( blk );
}
#endif


#if _TARGET & _TARG_80386
static  bool    ScanLabelCalls( BackRun& back )
/*************************************
 * Make sure that all blocks that are called are only called
 * one level deep. Mark output edges of all such blocks as
 * DOWN_ONE_CALL. This is so we can adjust our x[esp] offsets
 * accordingly.
 */
{
    block   *blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !IsBlkAttr( blk, BLK_CALL_LABEL ) )
            continue;
        if( ScanForLabelReturn( back, blk->edge[0].destination.u.blk ) == NULL ) {
            return( false );
        }
    }
    return( true );
}
#endif


static  void    AdjustPushLocals( BackRun& back )
/**************************************/
{
    instruction *ins;

    for( ins = back.data.HeadBlock->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( DoesSomething( ins ) )
            break;
        if( ins->head.opcode == OP_MOV
          && ins->head.state == OPERANDS_NEED_WORK ) {
            QuickSave( back, ins->operands[0]->r.reg, OP_PUSH );
            AdjustPushLocal( back, ins->result );
        }
    }
}

static  bool    NeedBPProlog( BackRun& back )
/**********************************/
{
    if( back.data.CurrProc->parms.size != 0 && back.x86proc_file.ParmsReferenced )
        return( true );
    if( back.data.CurrProc->locals.size != 0 )
        return( true );
    if( back.data.CurrProc->targ.push_local_size != 0 )
        return( true );
    if( back.data.CurrProc->lex_level > 0 )
        return( true );
    if( back.data.CurrProc->targ.sp_align )
        return( true );
    if( back.data.BlockByBlock != 0 )
        return( true );
    if( back.data.CurrProc->state.attr & ROUTINE_NEEDS_PROLOG )
        return( true );
    if( FarRetOnStack( back ) ) {
        if( ChainFrame( back ) ) {
            return( true );
        }
    }
    return( false );
}


static void FindIfExported( BackRun& back )
/********************************/
{
    cg_sym_handle sym;

    sym = AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) );
    if( sym == NULL )
        return;
    if( SymIsExported( back, sym ) ) {
        back.data.CurrProc->prolog_state |= PST_EXPORT;
    }
}


static  bool    NeedStackCheck( BackRun& back )
/************************************/
{
    return( FEStackChk( *back.front, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ) ) != 0 );
}


static void DoStackCheck( BackRun& back )
/******************************/
{
    if( back.data.CurrProc->prolog_state & (PST_PROLOG_THUNK | PST_PROLOG_RDOSDEV) )
        return;
#if _TARGET & _TARG_80386
    if( back.data.CurrProc->prolog_state & PST_GROW_STACK ) {
        if( back.data.BlockByBlock
          || back.data.CurrProc->locals.size >= 4096 ) {
            GenUnkPush( back, &back.data.CurrProc->targ.stack_check );
            DoRTCall( back, RT_GROW, true );
        }
        return;
    }
#endif
    if( NeedStackCheck(back) ) {
#if _TARGET & _TARG_8086
        if( HW_COvlap( back.data.CurrProc->state.parm.used, HW_STACK_CHECK ) ) {
            QuickSave( back, HW_STACK_CHECK, OP_PUSH );
        }
        GenUnkMov( back, HW_STACK_CHECK, &back.data.CurrProc->targ.stack_check );
        DoRTCall( back, RT_CHK, false );
        if( HW_COvlap( back.data.CurrProc->state.parm.used, HW_STACK_CHECK ) ) {
            QuickSave( back, HW_STACK_CHECK, OP_POP );
        }
#else
        GenUnkPush( back, &back.data.CurrProc->targ.stack_check );
        DoRTCall( back, RT_CHK, true );
#endif
    }
}


static  void    EmitNameInCode( BackRun& back )
/************************************/
{
    cg_sym_handle   sym;
    const char      *name;
    label_handle    lbl;
    uint            len;

    sym = AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) );
    if( sym == NULL )
        return;
    name = FEName( *back.front, sym );
    len = strlen( name );
    lbl = AskForNewLabel(back);
    TellKeepLabel( back, lbl );
    CodeLabel( back, lbl, 0 );
    GenKillLabel( back, lbl );
    EyeCatchBytes( back, name, len );
}


static  int ProEpiDataSize( BackRun& back )
/********************************/
{
    return( RoundUp( (int)(pointer_uint)FEAuxInfo( *back.front, NULL, FEINF_PROEPI_DATA_SIZE ), WORD_SIZE ) );
}


static  void    PrologHook( BackRun& back )
/********************************/
{
    int      size;

    if( (back.data.CurrProc->prolog_state & PST_PROLOG_HOOKS) == 0 )
        return;
    size = ProEpiDataSize(back);
    if( size != 0 ) {
        GenRegSub( back, HW_xSP, size );
        back.data.CurrProc->targ.base_adjust += size;
    }
#if _TARGET & _TARG_8086
    DoRTCall( back, RT_PROHOOK, false );
#else
//    GenPushC( CurrProc->parms.size );
    DoRTCall( back, RT_PROHOOK, false );
#endif
}


static  void    EpilogHook( BackRun& back )
/********************************/
{
    int      size;

    if( back.data.CurrProc->prolog_state & PST_EPILOG_HOOKS ) {
        DoRTCall( back, RT_EPIHOOK, false );
    }
    size = ProEpiDataSize(back);
    if( size != 0 ) {
        GenRegAdd( back, HW_xSP, size );
    }
}


static  void    DoLoadDS( BackRun& back )
/******************************/
{
#if _TARGET & _TARG_80386
    if( IsntTargetModel( back, CGSW_X86_LOAD_DS_DIRECTLY ) ) {
        DoRTCall( back, RT_GETDS, false );
    } else {
#endif
        if( HW_COvlap( back.data.CurrProc->state.parm.used, HW_LOAD_DS ) ) {
            QuickSave( back, HW_LOAD_DS, OP_PUSH );
        }
        GenLoadDS(back);
        if( HW_COvlap( back.data.CurrProc->state.parm.used, HW_LOAD_DS ) ) {
            QuickSave( back, HW_LOAD_DS, OP_POP );
        }
#if _TARGET & _TARG_80386
    }
#endif
}


static  int LoadDS( BackRun& back )
/************************/
{
    int     size;

    size = 0;
    if( back.data.CurrProc->state.attr & ROUTINE_LOADS_DS ) {
        if( HW_COvlap( back.data.CurrProc->state.unalterable, HW_DS ) ) {
            QuickSave( back, HW_DS, OP_PUSH );
            size = WORD_SIZE;
            DoLoadDS(back);
        }
    }
    return( size );
}


static  void    UnloadDS( BackRun& back )
/******************************/
{
    if( back.data.CurrProc->state.attr & ROUTINE_LOADS_DS ) {
        if( HW_COvlap( back.data.CurrProc->state.unalterable, HW_DS ) ) {
            QuickSave( back, HW_DS, OP_POP );
        }
    }
}


static  void    MoveParms( BackRun& back )
/*******************************/
{
    int     i;

    for( i = 0; back.i87data.Parm8087[i] != NULL; ++i ) {
        GFstpM( back, back.i87data.Parm8087[i] );
    }
}


static  void    AllocStack( BackRun& back )
/********************************/
{
    type_length     size;

    /*
     * keep stack aligned
     */
    size = RoundUp( back.data.CurrProc->locals.size, WORD_SIZE );
    back.data.CurrProc->locals.size = size;
    if( back.data.BlockByBlock ) {
        GenUnkSub( back, HW_xSP, &back.data.CurrProc->targ.prolog_loc );
        if( back.data.CurrProc->prolog_state & PST_TOUCH_STACK ) {
            GenTouchStack( back, true );
        }
    } else if( size <= 2 * WORD_SIZE
      && back.data.OptForSize > 50 ) {
        while( size > 0 ) {
            QuickSave( back, HW_xAX, OP_PUSH );
            size -= WORD_SIZE;
        }
    } else if( size != 0 ) {
        GenRegSub( back, HW_xSP, size );
        if( back.data.CurrProc->prolog_state & PST_TOUCH_STACK ) {
            GenTouchStack( back, false );
        }
    }
    if( back.data.CurrProc->targ.sp_align ) {
        GenRegAnd( back, HW_xSP, -( 2 * WORD_SIZE ) );
        back.data.CurrProc->prolog_state |= PST_RESET_SP;
    }
}


static  int PushAll( BackRun& back )
/**************************
 * Save all registers and establish somewhat sane environment.
 * Used for __interrupt routines only.
 */
{
    if( CPULevel( back, CPU_186 ) ) {
        Gpusha(back);
    } else {
        QuickSave( back, HW_xAX, OP_PUSH );
        QuickSave( back, HW_xCX, OP_PUSH );
        QuickSave( back, HW_xDX, OP_PUSH );
        QuickSave( back, HW_xBX, OP_PUSH );
        QuickSave( back, HW_xSP, OP_PUSH );
        QuickSave( back, HW_xBP, OP_PUSH );
        QuickSave( back, HW_xSI, OP_PUSH );
        QuickSave( back, HW_xDI, OP_PUSH );
    }
    QuickSave( back, HW_DS, OP_PUSH );
    QuickSave( back, HW_ES, OP_PUSH );
    if( CPULevel( back, CPU_386 ) ) {
        QuickSave( back, HW_FS, OP_PUSH );
        QuickSave( back, HW_GS, OP_PUSH );
    } else {
        QuickSave( back, HW_xAX, OP_PUSH );
        QuickSave( back, HW_xAX, OP_PUSH );
    }
    GenRegMove( back, HW_xSP, HW_xBP );
    AllocStack(back);
    Gcld(back);
    if( HW_COvlap( back.data.CurrProc->state.unalterable, HW_DS ) ) {
        DoLoadDS(back);
        /*
         * If ES is also unalterable, copy DS to ES; else things
         * like memcpy() are likely to blow up
         */
        if( HW_COvlap( back.data.CurrProc->state.unalterable, HW_ES ) ) {
            QuickSave( back, HW_DS, OP_PUSH );
            QuickSave( back, HW_ES, OP_POP );
        }
    }
    /*
     * 8 general purpose registers + 4 segment registers
     */
    return( 12 * WORD_SIZE );
}


static  void    PopAll( BackRun& back )
/****************************/
{
    if( back.data.CurrProc->locals.size != 0 ) {
        GenRegMove( back, HW_xBP, HW_xSP );
    }
    if( CPULevel( back, CPU_386 ) ) {
        QuickSave( back, HW_GS, OP_POP );
        QuickSave( back, HW_FS, OP_POP );
    } else {
        QuickSave( back, HW_xAX, OP_POP );
        QuickSave( back, HW_xAX, OP_POP );
    }
    QuickSave( back, HW_ES, OP_POP );
    QuickSave( back, HW_DS, OP_POP );
    if( CPULevel( back, CPU_186 ) ) {
        Gpopa(back);
    } else {
        QuickSave( back, HW_xDI, OP_POP );
        QuickSave( back, HW_xSI, OP_POP );
        QuickSave( back, HW_xBP, OP_POP );
        QuickSave( back, HW_xBX, OP_POP );
        QuickSave( back, HW_xBX, OP_POP );
        QuickSave( back, HW_xDX, OP_POP );
        QuickSave( back, HW_xCX, OP_POP );
        QuickSave( back, HW_xAX, OP_POP );
    }
}


static  void    DoEnter( BackRun& back, level_depth level )
/******************************************/
{
    type_length size;

    /*
     * keep stack aligned
     */
    size = RoundUp( back.data.CurrProc->locals.size, WORD_SIZE );
    back.data.CurrProc->locals.size = size;

    back.data.CurrProc->parms.base += WORD_SIZE;
    if( back.data.BlockByBlock ) {
        GenUnkEnter( back, &back.data.CurrProc->targ.prolog_loc, level );
        if( back.data.CurrProc->prolog_state & PST_TOUCH_STACK ) {
            GenTouchStack( back, true );
        }
    } else {
        GenEnter( back, size, level );
        if( size != 0
          && (back.data.CurrProc->prolog_state & PST_TOUCH_STACK) ) {
            GenTouchStack( back, false );
        }
    }
}


static  void    Enter( BackRun& back )
/***************************/
{
    level_depth lex_level;
    int         i;

    lex_level = back.data.CurrProc->lex_level;
#if _TARGET & _TARG_80386
    if( !back.data.CurrProc->targ.sp_frame
      && CPULevel( back, CPU_186 )
      && back.data.CurrProc->locals.size < 65536
      && ( lex_level > 0 || ( back.data.CurrProc->locals.size != 0 && back.data.OptForSize > 50 ) ) ) {
#else
    if( !back.data.CurrProc->targ.sp_frame
      && CPULevel( back, CPU_186 )
      && ( lex_level > 0 || ( back.data.CurrProc->locals.size != 0 && back.data.OptForSize > 50 ) ) ) {
#endif
        DoEnter( back, lex_level );
        HW_CTurnOn( back.data.CurrProc->state.used, HW_xBP );
        back.data.CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
    } else {
        if( NeedBPProlog(back) ) {
            if( !back.data.CurrProc->targ.sp_frame
              || back.data.CurrProc->targ.sp_align ) {
                HW_CTurnOn( back.data.CurrProc->state.used, HW_xBP );
                back.data.CurrProc->parms.base += WORD_SIZE;
                QuickSave( back, HW_xBP, OP_PUSH );
                i = 0;
                while( --lex_level > 0 ) {
                    i -= 2;
                    GenPushOffset( back, i );
                }
                GenRegMove( back, HW_xSP, HW_xBP );
                if( back.data.CurrProc->lex_level > 1 ) {
                    GenRegAdd( back, HW_xBP, ( back.data.CurrProc->lex_level - 1 ) * WORD_SIZE );
                }
                if( back.data.CurrProc->lex_level > 0 ) {
                    QuickSave( back, HW_xBP, OP_PUSH );
                }
            }
            back.data.CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
        }
        AllocStack(back);
    }
}


static  void    CalcUsedRegs( BackRun& back )
/**********************************/
{
    block   *blk;
    instruction *ins;
    name    *result;
    hw_reg_set  used;

    HW_CAsgn( used, HW_EMPTY );
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            result = ins->result;
            if( result != NULL
              && result->n.class_ == N_REGISTER ) {
                HwTurnOn( used, result->r.reg );
            }
            /*
             * place holder for big label doesn't really zap anything
             */
            if( ins->head.opcode != OP_NOP ) {
                HwTurnOn( used, ins->zap->reg );
                if( HW_COvlap( ins->zap->reg, HW_xSP ) ) {
                    back.data.CurrProc->prolog_state |= PST_RESET_SP;
                }
            }
        }
    }
    if( !back.data.CurrProc->targ.sp_frame
      || back.data.CurrProc->targ.sp_align ) {
        HW_CTurnOff( used, HW_xBP );
    }
    HwTurnOn( back.data.CurrProc->state.used, used );
}


static  int Push( BackRun& back, hw_reg_set to_push )
/************************************/
{
    int         size;
    int         i;

    size = 0;
    if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION )
      && back.data.CurrProc->targ.sp_frame
      && !back.data.CurrProc->targ.sp_align ) {
        FlowSave( back, &to_push );
    }
    for( i = 0; i < sizeof( PushRegs ) / sizeof( PushRegs[0] ) - 1; i++ ) {
        if( HW_CEqual( to_push, HW_EMPTY ) )
            break;
        if( HwOvlap( PushRegs[i], to_push ) ) {
            QuickSave( back, PushRegs[i], OP_PUSH );
            size += WORD_SIZE;
            HwTurnOff( to_push, PushRegs[i] );
        }
    }
    return( size );
}

static  void        Pop( BackRun& back, hw_reg_set to_pop )
/******************************************/
{
    int         i;

    if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION )
      && back.data.CurrProc->targ.sp_frame
      && !back.data.CurrProc->targ.sp_align ) {
        FlowRestore( back, &to_pop );
    }
    for( i = sizeof( PushRegs ) / sizeof( PushRegs[0] ) - 1; i-- > 0 ; ) {
        if( HW_CEqual( to_pop, HW_EMPTY ) )
            break;
        if( HwOvlap( PushRegs[i], to_pop ) ) {
            QuickSave( back, PushRegs[i], OP_POP );
            HwTurnOff( to_pop, PushRegs[i] );
        }
    }
}

static void    GenLeaSP( BackRun& back, int offset )
/************************************
 * LEA         [e]sp,[[e]bp+offset]
 */
{
    Format( back, OC_CODE );
    LayOpword( back, M_LEA );
    OpndSize( back, HW_xSP );
    LayReg( back, HW_xSP );
    back.pcencode.Inst[RMR] |= Displacement( back, offset, HW_xBP );
    back.pcencode.Inst[RMR] |= DoIndex( back, HW_xBP );
    Finalize( back );
}

bool    EpilogIsLeaveOnly( BackRun& back )
/*************************************
 * The epilog is LEAVE and the return, nothing between: SP is set back from BP whatever it was
 * -- under -zmsc on a 186 or later, a frame with no registers of the caller's to pop
 */
{
    hw_reg_set  to_pop;

    if( IsntTargetModel( back, CGSW_X86_MSC8 ) || !CPULevel( back, CPU_186 ) )
        return( false );
    if( RoutineIsInterrupt( back.data.CurrProc->state.attr ) || ChainFrame( back ) )
        return( false );
    if( (back.data.CurrProc->state.attr & ROUTINE_NEEDS_PROLOG) == 0 )
        return( false );
    if( back.data.CurrProc->targ.sp_frame || back.data.CurrProc->targ.base_adjust != 0 )
        return( false );
    if( (back.data.CurrProc->state.attr & ROUTINE_LOADS_DS) && HW_COvlap( back.data.CurrProc->state.unalterable, HW_DS ) )
        return( false );
    to_pop = SaveRegs(back);
    HW_CTurnOff( to_pop, HW_FLTS );
    HW_CTurnOff( to_pop, HW_xBP );
    return( HW_CEqual( to_pop, HW_EMPTY ) );
}

static  void    DoEpilog( BackRun& back )
/******************************/
{
    hw_reg_set  to_pop;
    bool    is_long;
    type_length size;

    if( RoutineIsInterrupt( back.data.CurrProc->state.attr ) ) {
        PopAll(back);
    } else {
        to_pop = SaveRegs(back);
        HW_CTurnOff( to_pop, HW_FLTS );
        if( ChainFrame( back ) ) {
            if( (back.data.CurrProc->state.attr & ROUTINE_NEEDS_PROLOG)
              || ( back.data.CurrProc->locals.size + back.data.CurrProc->targ.push_local_size ) != 0 ) {
                if( back.data.CurrProc->targ.base_adjust == 0 ) {
                    GenRegMove( back, HW_xBP, HW_xSP );
                } else {
                    GenLeaSP( back, -back.data.CurrProc->targ.base_adjust );
                }
            }
            HW_CTurnOff( to_pop, HW_xBP );
        } else {
            if( back.data.CurrProc->state.attr & ROUTINE_NEEDS_PROLOG ) {
                size = back.data.CurrProc->locals.size + back.data.CurrProc->targ.push_local_size;
                if( (back.data.CurrProc->prolog_state & PST_RESET_SP)
                  || size != 0
                  /*
                   * Microsoft C 8 ends every frame with LEAVE on a 186 or later, whether or not
                   * it has locals to drop (-zmsc)
                   */
                  || ( IsTargetModel( back, CGSW_X86_MSC8 ) && CPULevel( back, CPU_186 ) ) ) {
                    /*
                     * sp is not pointing at saved registers already
                     */
                    if( back.data.CurrProc->targ.sp_frame ) {
                        if( back.data.CurrProc->targ.sp_align ) {
                            GenRegMove( back, HW_xBP, HW_xSP );
                            QuickSave( back, HW_xBP, OP_POP );
                        } else if( size != 0 ) {
                            GenRegAdd( back, HW_xSP, size );
                        }
                    } else if( back.data.CurrProc->targ.base_adjust != 0 ) {
                        GenLeaSP( back, -back.data.CurrProc->targ.base_adjust );
                    } else if( CPULevel( back, CPU_186 )
                      && (!CPULevel( back, CPU_486 ) || back.data.OptForSize > 50) ) {
                        GenLeave(back);
                        HW_CTurnOff( to_pop, HW_xBP );
                    } else {
                        GenRegMove( back, HW_xBP, HW_xSP );
                    }
                }
            }
        }
        Pop( back, to_pop );
        UnloadDS(back);
        if( ChainFrame( back ) ) {
            if( NeedBPProlog(back) ) {
                EpilogHook(back);
                if( FarRetOnStack( back ) ) {
                    if( CheapFrame( back ) ) {
                        GenCheapWindowsEpilog(back);
                    } else {
                        GenWindowsEpilog(back);
                    }
                } else {
                    QuickSave( back, HW_xBP, OP_POP );
                }
            }
        }
        if( back.data.CurrProc->prolog_state & PST_PROLOG_RDOSDEV ) {
            GenRdosdevEpilog(back);
        }
    }

    is_long = ( RoutineIsLong( back.data.CurrProc->state.attr )
                || RoutineIsFar16( back.data.CurrProc->state.attr ) );
#if _TARGET & _TARG_80386
    if( back.data.CurrProc->prolog_state & PST_PROLOG_THUNK ) {
        QuickSave( back, HW_xSP, OP_POP );
    }
    if( IsTargetModel( back, CGSW_X86_NEW_P5_PROFILING )
      || IsTargetModel( back, CGSW_X86_P5_PROFILING ) ) {
        GenP5ProfilingEpilog( back, static_cast<label_handle>(back.data.CurrProc->label) );
    }
#endif

    if( RoutineIsInterrupt( back.data.CurrProc->state.attr ) ) {
        GenIRET(back);
    } else if( back.data.CurrProc->state.attr & ROUTINE_REMOVES_PARMS ) {
        GenReturn( back, back.data.CurrProc->parms.size, is_long );
    } else {
        GenReturn( back, 0, is_long );
    }
}

bool    CanZapBP( BackRun& back )
/**********************/
{
    return( !ChainFrame( back ) );
}


void    AddCacheRegs( BackRun& back )
/**************************/
{
#if _TARGET & _TARG_80386
    if( back.data.CurrProc->targ.never_sp_frame )
        return;
    if( IsntModel( back, CGSW_GEN_MEMORY_LOW_FAILS ) )
        return;
    if( back.data.OptForSize > 50 )
        return;
    if( IsTargetModel( back, CGSW_X86_FLOATING_DS )
      || IsTargetModel( back, CGSW_X86_FLOATING_SS ) )
        return;
    if( !ScanInstructions(back) )
        return;
    if( !CanZapBP(back) )
        return;
    if( !ScanLabelCalls(back) )
        return;
    if( back.data.CurrProc->state.attr & ROUTINE_WANTS_DEBUGGING )
        return;
    if( back.data.CurrProc->lex_level > 0 )
        return;
    if( FPULevel( back, FPU_586 ) ) {
        ScanForFDOps(back);
    }
    if( back.data.CurrProc->targ.has_fd_temps ) {
        back.data.CurrProc->targ.sp_frame = true;
        back.data.CurrProc->targ.sp_align = true;
    } else if( !DoBpChain( back )
      && IsntTargetModel( back, CGSW_X86_WINDOWS )
      && !RoutineIsInterrupt( back.data.CurrProc->state.attr ) ) {
        /*
         * We cannot make EBP available under Windows because the SS
         * selector might not cover the data segment and so we cannot use
         * it as an index. Puke - BBB Feb 18, 1994
         */
        back.data.CurrProc->targ.sp_frame = true;
        HW_CTurnOff( back.data.CurrProc->state.unalterable, HW_xBP );
    }
#endif
}


void DoRTCall( BackRun& back, rt_class rtindex, bool pop )
/*****************************************/
{
    DoCall( back, RTLabel( back, rtindex ), true, IsTargetModel( back, CGSW_X86_BIG_CODE ), pop );
}

static unsigned returnAddressStackSize( BackRun& back )
/********************************************/
{
    unsigned    size;

    if( RoutineIsInterrupt( back.data.CurrProc->state.attr ) ) {
        size = 0;
    } else if( back.data.CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS ) {
        size = 0;
    } else if( RoutineIsLong( back.data.CurrProc->state.attr ) ) {
        size = 2 * WORD_SIZE;
    } else if( RoutineIsFar16( back.data.CurrProc->state.attr ) ) {
        size = 2 * WORD_SIZE;
    } else {
        size = WORD_SIZE;
    }
    return( size );
}

void    GenProlog( BackRun& back )
/***********************/
{
    hw_reg_set  to_push;
    unsigned    ret_size;
    pointer     label;
    pointer     origlabel;  /* Original label for generated __far16 thunks */
    fe_attr     attr;

    ScanInstructions(back);     /* Do These 2 calls before using DoWindowsCrap! */
    FindIfExported(back);
    { segment_id old_segid = ChangeOP( back, AskCodeSeg(back) );
        if( back.data.CurrProc->prolog_state & PST_FUNCTION_NAME ) {
            EmitNameInCode(back);
        }

        if( IsModel( back, CGSW_GEN_DBG_NUMBERS ) ) {
            CodeLineNumber( back, back.data.HeadBlock->ins.head.line_num, false );
        }

        if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
            EmitRtnBeg(back);
        }
        if( back.data.CurrProc->state.attr & ROUTINE_WANTS_DEBUGGING ) {
            back.data.CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
        }

        back.data.CurrProc->parms.base = 0;
        back.data.CurrProc->parms.size = back.data.CurrProc->state.parm.offset;

        origlabel = label = back.data.CurrProc->label;

#if _TARGET & _TARG_80386
        if( RoutineIsFar16( back.data.CurrProc->state.attr ) ) {
            label = GenFar16Thunk( back, back.data.CurrProc->label, back.data.CurrProc->parms.size,
                        ( back.data.CurrProc->state.attr & ROUTINE_REMOVES_PARMS ) != 0 );
//            CurrProc->label = label; /* ugly mess if following are combined */
        }
#endif

        if( IsTargetModel( back, CGSW_X86_MSC8 ) ) {
            /*
             * Microsoft C 8 starts every function on an even byte, at /Os as well, padding
             * with NOP (-zmsc)
             */
            CodeHandle( back, OC_LABEL, 2 - 1, static_cast<label_handle>(label) );
        } else {
            CodeLabel( back, static_cast<label_handle>(label), DepthAlign( back, PROC_ALIGN ) );
        }

        attr = FEAttr( *back.front, AskForLblSym( back, static_cast<label_handle>(origlabel) ) );

        if( back.data.CurrProc->prolog_state & PST_PROLOG_RDOSDEV ) {
            GenRdosdevProlog(back);
        }

    #if _TARGET & _TARG_80386
        if( (attr & FE_NAKED) == 0 ) {
            if( IsTargetModel( back, CGSW_X86_NEW_P5_PROFILING )
              || IsTargetModel( back, CGSW_X86_P5_PROFILING ) ) {
                GenP5ProfilingProlog( back, static_cast<label_handle>(label) );
            }
            if( back.data.CurrProc->prolog_state & PST_PROLOG_THUNK ) {
                QuickSave( back, HW_xSP, OP_PUSH );
                GenPushC( back, back.data.CurrProc->parms.size );
                GenUnkPush( back, &back.data.CurrProc->targ.stack_check );
                if( NeedStackCheck(back) ) {
                    DoRTCall( back, RT_THUNK_STK, true );
                } else {
                    DoRTCall( back, RT_THUNK, true );
                }
                back.data.CurrProc->parms.base += WORD_SIZE;
            }
        }
    #endif

        ret_size = returnAddressStackSize(back);
        back.data.CurrProc->parms.base += ret_size;
        CalcUsedRegs(back);

        to_push = SaveRegs(back);
        HW_CTurnOff( to_push, HW_FLTS );
        if( !back.data.CurrProc->targ.sp_frame
          || back.data.CurrProc->targ.sp_align ) {
            HW_CTurnOff( to_push, HW_xBP );
        }

        if( attr & FE_NAKED ) {
            /*
             * don't do anything - empty prologue
             */
        } else if( RoutineIsInterrupt( back.data.CurrProc->state.attr ) ) {
            ret_size = -PushAll(back);
            back.data.CurrProc->targ.base_adjust = 0;
            MoveParms(back);
        } else {
            if( ChainFrame( back ) ) {
                back.data.CurrProc->targ.base_adjust = 0;
                if( NeedBPProlog(back) ) {
                    HW_CTurnOn( back.data.CurrProc->state.used, HW_xBP );
                    back.data.CurrProc->parms.base += WORD_SIZE;
                    if( FarRetOnStack( back ) ) {
                        if( CheapFrame( back ) ) {
                            GenCheapWindowsProlog(back);
                        } else {
    #if _TARGET & _TARG_8086
                            /*
                             * Windows prologs zap AX, so warn idiot user if we
                             * generate one for a routine in which AX is live
                             * upon entry to routine, or unalterable.
                             */
                            if( HW_COvlap( back.data.CurrProc->state.unalterable, HW_xAX )
                              || HW_COvlap( back.data.CurrProc->state.parm.used, HW_xAX ) ) {
                                #define MSGFMT  "exported routine '%-.256s' with AX live on entry"

                                cg_sym_handle sym;
                                const char    *src;
                                char          buffer[sizeof( MSGFMT ) - 7 + 256];

                                src = NULL;
                                sym = AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) );
                                if( sym != NULL ) {
                                    src = FEExtName( *back.front, sym, EXTN_BASENAME );
                                }
                                if( src == NULL ) {
                                    src = "";
                                }
                                sprintf( buffer, MSGFMT, src );
                                FEMessage( *back.front, FEMSG_ERROR, buffer );
                                #undef MSGFMT
                            }
    #endif
                            GenWindowsProlog(back);
                            back.data.CurrProc->targ.base_adjust += 2; /* the extra push DS */
                        }
                    } else {
                        QuickSave( back, HW_xBP, OP_PUSH );
                        GenRegMove( back, HW_xSP, HW_xBP );
                    }
                    PrologHook(back);
                }
                DoStackCheck(back);
                back.data.CurrProc->targ.base_adjust += LoadDS(back);
                back.data.CurrProc->targ.base_adjust += Push( back, to_push );
                back.data.CurrProc->parms.base += back.data.CurrProc->targ.base_adjust;
                AllocStack(back);
                AdjustPushLocals(back);
            } else {
                DoStackCheck(back);
                back.data.CurrProc->parms.base += LoadDS(back);
                if( (back.data.CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS) == 0 ) {
                    back.data.CurrProc->parms.base += Push( back, to_push );
                }
                Enter(back);
                AdjustPushLocals(back);
                if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION )
                  || back.data.CurrProc->targ.sp_frame ) {
                    back.data.CurrProc->targ.base_adjust = 0;
                } else {
                    back.data.CurrProc->targ.base_adjust = AdjustBase(back);
                    if( back.data.CurrProc->targ.base_adjust != 0 ) {
                        GenRegSub( back, HW_xBP, -back.data.CurrProc->targ.base_adjust );
                    }
                }
            }
            RelocParms(back);
            MoveParms(back);
        }
        back.data.CurrProc->prolog_state |= PST_PROLOG_GENERATED;

        if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
            DbgRetOffset( back, back.data.CurrProc->parms.base - back.data.CurrProc->targ.base_adjust - ret_size );
            EmitProEnd(back);
        }
    SetOP( back, old_segid ); }

    if( back.data.CurrProc->prolog_state & PST_EXPORT ) {
        OutDLLExport( back, WordsCount( back.data.CurrProc->parms.size ), AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ) );
    }
}


void    InitStackDepth( BackRun& back, block *blk )
/**********************************/
{
    if( blk->edge[0].flags & BEF_DOWN_ONE_CALL ) {
        back.x86proc.StackDepth = WORD_SIZE;
    } else {
        back.x86proc.StackDepth = 0;
    }
    back.x86proc.StackDepth += blk->stack_depth;
}


void        AdjustStackDepth( BackRun& back, instruction *ins )
/**********************************************/
{
    name        *op;
    type_length adjust;

    if( !DoesSomething( ins ) )
        return;
    switch( ins->head.opcode ) {
    case OP_ADD:
    case OP_SUB:
        if( ins->operands[0] != ins->result )
            return;
        if( ins->result->n.class_ != N_REGISTER )
            return;
        if( !HW_CEqual( ins->result->r.reg, HW_xSP ) )
            return;
        op = ins->operands[1];
        if( op->n.class_ != N_CONSTANT ) {
            Zoiks( back, ZOIKS_077 );
            return;
        }
        adjust = op->c.lo.u.int_value;
        if( ins->head.opcode == OP_SUB ) {
            back.x86proc.StackDepth += adjust;
        } else {
            back.x86proc.StackDepth -= adjust;
        }
        break;
    case OP_PUSH:
        back.x86proc.StackDepth += WORD_SIZE;
        break;
    case OP_POP:
        back.x86proc.StackDepth -= WORD_SIZE;
        break;
    case OP_CALL:
    case OP_CALL_INDIRECT:
        if( ins->flags.u.call_flags & CALL_POPS_PARMS ) {
            op = ins->operands[CALL_OP_POPS];
            if( op->n.class_ == N_CONSTANT ) {
                back.x86proc.StackDepth -= op->c.lo.u.int_value;
            }
        }
    default:
        break;
    }
}

void     AdjustStackDepthDirect( BackRun& back, int adjust )
/*******************************************/
{
    back.x86proc.StackDepth += adjust;
}


bool    BaseIsSP( BackRun& back, name *op )
/**************************/
{
    if( !back.data.CurrProc->targ.sp_frame )
        return( false );
    if( back.data.CurrProc->targ.sp_align
      && (op->t.temp_flags & STACK_PARM) ) {
        return( false );
    }
    return( true );
}


type_length NewBase( BackRun& back, name *op )
/*****************************/
{
    if( !BaseIsSP( back, op ) ) {
        return( op->t.location - back.data.CurrProc->targ.base_adjust );
    }
    return( op->t.location + back.data.CurrProc->locals.size
        + back.data.CurrProc->targ.push_local_size + back.x86proc.StackDepth );
}

type_length PushSize( type_length len )
/*************************************/
{
    if( len < WORD_SIZE )
        return( WORD_SIZE );
    return( RoundUp( len, WORD_SIZE ) );
}


void    GenEpilog( BackRun& back )
/***********************/
{
    type_length stack;
    fe_attr attr;

    attr = FEAttr( *back.front, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ) );

    stack = - back.data.CurrProc->locals.size - back.data.CurrProc->locals.base
         - back.data.CurrProc->targ.push_local_size;
    PatchBigLabels( back, stack );

    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
        EmitEpiBeg(back);
    }

    if( (attr & FE_NAKED) == 0 ) {
        if( (back.data.CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS) == 0 ) {
            DoEpilog(back);
        }

        if( back.data.BlockByBlock ) {
            AbsPatch( back, back.data.CurrProc->targ.prolog_loc, back.data.CurrProc->locals.size );
        }

        if( back.data.CurrProc->targ.stack_check != NULL ) {
            AbsPatch( back, back.data.CurrProc->targ.stack_check,
                  back.data.CurrProc->locals.size +
                  back.data.CurrProc->parms.base  +
                  WORD_SIZE * back.data.CurrProc->lex_level +
                  back.data.CurrProc->targ.push_local_size +
                  back.data.MaxStack );
        }
    }


    back.data.CurrProc->prolog_state |= PST_EPILOG_GENERATED;
    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) {  // d1+ or d2
        EmitRtnEnd(back);
    }
}


int AskDisplaySize( level_depth level )
/*************************************/
{
    return( level * WORD_SIZE );
}

int ParmsAtPrologue( BackRun& back )
/*************************/
{
    unsigned    ret_size;
    unsigned    parms_off_sp;

    parms_off_sp= 0;

#if _TARGET & _TARG_80386
    if( back.data.CurrProc->prolog_state & PST_PROLOG_THUNK ) {
        parms_off_sp += WORD_SIZE;
    }
#endif

    ret_size = returnAddressStackSize(back);
    parms_off_sp += ret_size;
    return( parms_off_sp );
}
