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


#define WORDS_COUNT(size)   (((size) + WORD_SIZE - 1) / WORD_SIZE)

#define WINDOWS_CHEAP  ( \
    ( _IsModel( CGSW_GEN_DLL_RESIDENT_CODE ) \
      && (data_state().CurrProc->state.attr & ROUTINE_LOADS_DS) ) \
    || ( _IsTargetModel( CGSW_X86_CHEAP_WINDOWS ) \
      && (data_state().CurrProc->prolog_state & PST_EXPORT) == 0 ) \
    || ( _IsTargetModel( CGSW_X86_WINDOWS ) \
      && (data_state().CurrProc->prolog_state & (PST_EXPORT | PST_PROLOG_FAT)) == 0 ) )

#define DO_WINDOWS_CRAP ( _IsTargetModel( CGSW_X86_WINDOWS ) \
               && ( !WINDOWS_CHEAP || data_state().CurrProc->contains_call ) )

#define DO_BP_CHAIN ( ( ( _IsTargetModel( CGSW_X86_NEED_STACK_FRAME ) \
                          || (data_state().CurrProc->state.attr & ROUTINE_NEEDS_BP_CHAIN) \
                          || _IsModel( CGSW_GEN_DBG_CV ) ) \
                        && data_state().CurrProc->contains_call ) \
                      || (data_state().CurrProc->prolog_state & PST_PROLOG_FAT) )

#define CHAIN_FRAME ( DO_WINDOWS_CRAP || DO_BP_CHAIN )

#define CHEAP_FRAME ( _IsTargetModel( CGSW_X86_NEED_STACK_FRAME ) \
                      || _IsntTargetModel( CGSW_X86_WINDOWS ) \
                      || WINDOWS_CHEAP )

#define FAR_RET_ON_STACK ( _RoutineIsLong( data_state().CurrProc->state.attr ) \
             && (data_state().CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS) == 0 )


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
struct X86procFileState {
    bool_ ParmsReferenced = true;
};

}  // namespace

static inline X86procFileState& x86proc_file_state() { return cbrun_part<X86procFileState>(); }


static  bool_    OpUsesStackParm( name *op )
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


static  bool_    ScanInstructions( void )
/**************************************/
{
    block       *blk;
    instruction *ins;
    name        *addr;
    bool_        sp_constant;

    data_state().CurrProc->contains_call = true;
    x86proc_file_state().ParmsReferenced = true;
    if( data_state().BlockByBlock )
        return( false );
    data_state().CurrProc->contains_call = false;
    x86proc_file_state().ParmsReferenced = false;
    sp_constant = true;
    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( ins->head.opcode != OP_PARM_DEF ) {
                opcnt   k;

                for( k = ins->num_operands; k-- > 0; ) {
                    if( OpUsesStackParm( ins->operands[k] ) ) {
                        x86proc_file_state().ParmsReferenced = true;
                    }
                }
                if( OpUsesStackParm( ins->result ) ) {
                    x86proc_file_state().ParmsReferenced = true;
                }
            }
            if( _OpIsCall( ins->head.opcode ) ) {
                if( ins->head.opcode == OP_CALL_INDIRECT ) {
                    data_state().CurrProc->contains_call = true;
                } else {
                    addr = ins->operands[CALL_OP_ADDR];
                    if( addr->n.class_ != N_MEMORY
                      || addr->m.memory_type != CG_LBL
                      || !AskIfRTLabel( static_cast<label_handle>(addr->v.symbol) ) ) {
                        data_state().CurrProc->contains_call = true;
                    }
                }
                if( HW_COvlap( ins->zap->reg, HW_xSP ) ) {
                    data_state().CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
                    sp_constant = false;
                }
                if( ins->flags.u.call_flags & CALL_NEEDS_BP_CHAIN ) {
                    data_state().CurrProc->state.attr |= ROUTINE_NEEDS_BP_CHAIN;
                }
            }
        }
    }
    return( sp_constant );
}


#if _TARGET & _TARG_80386
static  void    ChkFDOp( name *op, level_depth depth )
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
    data_state().CurrProc->targ.has_fd_temps = true;
}
#endif


#if _TARGET & _TARG_80386
static  void    ScanForFDOps( void )
/**********************************/
{
    block       *blk;
    instruction *ins;
    opcnt       i;
    level_depth depth;

    data_state().CurrProc->contains_call = false;
    if( data_state().BlockByBlock )
        return;
    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        depth = blk->depth;
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( ins->type_class == FD
              || ins->type_class == FL ) {
                for( i = ins->num_operands; i-- > 0; ) {
                    ChkFDOp( ins->operands[i], depth );
                }
                if( ins->result != NULL ) {
                    ChkFDOp( ins->result, depth );
                }
            }
        }
    }
}
#endif


#if _TARGET & _TARG_80386
static  block *ScanForLabelReturn( block *blk )
/*********************************************/
{
    block       *son;
    block_num   i;

    if( _IsBlkAttr( blk, BLK_RETURN | BLK_CALL_LABEL ) )
        return( NULL );
    blk->edge[0].flags |= BEF_DOWN_ONE_CALL;
    if( _IsBlkAttr( blk, BLK_LABEL_RETURN ) )
        return( blk );
    for( i = 0; i < blk->targets; ++i ) {
        son = blk->edge[i].destination.u.blk;
        if( son->edge[0].flags & BEF_DOWN_ONE_CALL )
            continue;
        if( SafeRecurseCG( (func_sr)ScanForLabelReturn, son ) == NULL ) {
            return( NULL );
        }
    }
    return( blk );
}
#endif


#if _TARGET & _TARG_80386
static  bool_    ScanLabelCalls( void )
/*************************************
 * Make sure that all blocks that are called are only called
 * one level deep. Mark output edges of all such blocks as
 * DOWN_ONE_CALL. This is so we can adjust our x[esp] offsets
 * accordingly.
 */
{
    block   *blk;

    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !_IsBlkAttr( blk, BLK_CALL_LABEL ) )
            continue;
        if( ScanForLabelReturn( blk->edge[0].destination.u.blk ) == NULL ) {
            return( false );
        }
    }
    return( true );
}
#endif


static  void    AdjustPushLocals( void )
/**************************************/
{
    instruction *ins;

    for( ins = data_state().HeadBlock->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( DoesSomething( ins ) )
            break;
        if( ins->head.opcode == OP_MOV
          && ins->head.state == OPERANDS_NEED_WORK ) {
            QuickSave( ins->operands[0]->r.reg, OP_PUSH );
            AdjustPushLocal( ins->result );
        }
    }
}

static  bool_    NeedBPProlog( void )
/**********************************/
{
    if( data_state().CurrProc->parms.size != 0 && x86proc_file_state().ParmsReferenced )
        return( true );
    if( data_state().CurrProc->locals.size != 0 )
        return( true );
    if( data_state().CurrProc->targ.push_local_size != 0 )
        return( true );
    if( data_state().CurrProc->lex_level > 0 )
        return( true );
    if( data_state().CurrProc->targ.sp_align )
        return( true );
    if( data_state().BlockByBlock != 0 )
        return( true );
    if( data_state().CurrProc->state.attr & ROUTINE_NEEDS_PROLOG )
        return( true );
    if( FAR_RET_ON_STACK ) {
        if( CHAIN_FRAME ) {
            return( true );
        }
    }
    return( false );
}


static void FindIfExported( void )
/********************************/
{
    cg_sym_handle sym;

    sym = AskForLblSym( static_cast<label_handle>(data_state().CurrProc->label) );
    if( sym == NULL )
        return;
    if( SymIsExported( sym ) ) {
        data_state().CurrProc->prolog_state |= PST_EXPORT;
    }
}


static  bool_    NeedStackCheck( void )
/************************************/
{
    return( FEStackChk( AskForLblSym( static_cast<label_handle>(data_state().CurrProc->label) ) ) != 0 );
}


static void DoStackCheck( void )
/******************************/
{
    if( data_state().CurrProc->prolog_state & (PST_PROLOG_THUNK | PST_PROLOG_RDOSDEV) )
        return;
#if _TARGET & _TARG_80386
    if( data_state().CurrProc->prolog_state & PST_GROW_STACK ) {
        if( data_state().BlockByBlock
          || data_state().CurrProc->locals.size >= 4096 ) {
            GenUnkPush( &data_state().CurrProc->targ.stack_check );
            DoRTCall( RT_GROW, true );
        }
        return;
    }
#endif
    if( NeedStackCheck() ) {
#if _TARGET & _TARG_8086
        if( HW_COvlap( data_state().CurrProc->state.parm.used, HW_STACK_CHECK ) ) {
            QuickSave( HW_STACK_CHECK, OP_PUSH );
        }
        GenUnkMov( HW_STACK_CHECK, &data_state().CurrProc->targ.stack_check );
        DoRTCall( RT_CHK, false );
        if( HW_COvlap( data_state().CurrProc->state.parm.used, HW_STACK_CHECK ) ) {
            QuickSave( HW_STACK_CHECK, OP_POP );
        }
#else
        GenUnkPush( &data_state().CurrProc->targ.stack_check );
        DoRTCall( RT_CHK, true );
#endif
    }
}


static  void    EmitNameInCode( void )
/************************************/
{
    cg_sym_handle   sym;
    const char      *name;
    label_handle    lbl;
    uint            len;

    sym = AskForLblSym( static_cast<label_handle>(data_state().CurrProc->label) );
    if( sym == NULL )
        return;
    name = FEName( sym );
    len = Length( name );
    lbl = AskForNewLabel();
    TellKeepLabel( lbl );
    CodeLabel( lbl, 0 );
    GenKillLabel( lbl );
    EyeCatchBytes( name, len );
}


static  int ProEpiDataSize( void )
/********************************/
{
    return( _RoundUp( (int)(pointer_uint)FEAuxInfo( NULL, FEINF_PROEPI_DATA_SIZE ), WORD_SIZE ) );
}


static  void    PrologHook( void )
/********************************/
{
    int      size;

    if( (data_state().CurrProc->prolog_state & PST_PROLOG_HOOKS) == 0 )
        return;
    size = ProEpiDataSize();
    if( size != 0 ) {
        GenRegSub( HW_xSP, size );
        data_state().CurrProc->targ.base_adjust += size;
    }
#if _TARGET & _TARG_8086
    DoRTCall( RT_PROHOOK, false );
#else
//    GenPushC( CurrProc->parms.size );
    DoRTCall( RT_PROHOOK, false );
#endif
}


static  void    EpilogHook( void )
/********************************/
{
    int      size;

    if( data_state().CurrProc->prolog_state & PST_EPILOG_HOOKS ) {
        DoRTCall( RT_EPIHOOK, false );
    }
    size = ProEpiDataSize();
    if( size != 0 ) {
        GenRegAdd( HW_xSP, size );
    }
}


static  void    DoLoadDS( void )
/******************************/
{
#if _TARGET & _TARG_80386
    if( _IsntTargetModel( CGSW_X86_LOAD_DS_DIRECTLY ) ) {
        DoRTCall( RT_GETDS, false );
    } else {
#endif
        if( HW_COvlap( data_state().CurrProc->state.parm.used, HW_LOAD_DS ) ) {
            QuickSave( HW_LOAD_DS, OP_PUSH );
        }
        GenLoadDS();
        if( HW_COvlap( data_state().CurrProc->state.parm.used, HW_LOAD_DS ) ) {
            QuickSave( HW_LOAD_DS, OP_POP );
        }
#if _TARGET & _TARG_80386
    }
#endif
}


static  int LoadDS( void )
/************************/
{
    int     size;

    size = 0;
    if( data_state().CurrProc->state.attr & ROUTINE_LOADS_DS ) {
        if( HW_COvlap( data_state().CurrProc->state.unalterable, HW_DS ) ) {
            QuickSave( HW_DS, OP_PUSH );
            size = WORD_SIZE;
            DoLoadDS();
        }
    }
    return( size );
}


static  void    UnloadDS( void )
/******************************/
{
    if( data_state().CurrProc->state.attr & ROUTINE_LOADS_DS ) {
        if( HW_COvlap( data_state().CurrProc->state.unalterable, HW_DS ) ) {
            QuickSave( HW_DS, OP_POP );
        }
    }
}


static  void    MoveParms( void )
/*******************************/
{
    int     i;

    for( i = 0; i87data_state().Parm8087[i] != NULL; ++i ) {
        GFstpM( i87data_state().Parm8087[i] );
    }
}


static  void    AllocStack( void )
/********************************/
{
    type_length     size;

    /*
     * keep stack aligned
     */
    size = _RoundUp( data_state().CurrProc->locals.size, WORD_SIZE );
    data_state().CurrProc->locals.size = size;
    if( data_state().BlockByBlock ) {
        GenUnkSub( HW_xSP, &data_state().CurrProc->targ.prolog_loc );
        if( data_state().CurrProc->prolog_state & PST_TOUCH_STACK ) {
            GenTouchStack( true );
        }
    } else if( size <= 2 * WORD_SIZE
      && data_state().OptForSize > 50 ) {
        while( size > 0 ) {
            QuickSave( HW_xAX, OP_PUSH );
            size -= WORD_SIZE;
        }
    } else if( size != 0 ) {
        GenRegSub( HW_xSP, size );
        if( data_state().CurrProc->prolog_state & PST_TOUCH_STACK ) {
            GenTouchStack( false );
        }
    }
    if( data_state().CurrProc->targ.sp_align ) {
        GenRegAnd( HW_xSP, -( 2 * WORD_SIZE ) );
        data_state().CurrProc->prolog_state |= PST_RESET_SP;
    }
}


static  int PushAll( void )
/**************************
 * Save all registers and establish somewhat sane environment.
 * Used for __interrupt routines only.
 */
{
    if( _CPULevel( CPU_186 ) ) {
        Gpusha();
    } else {
        QuickSave( HW_xAX, OP_PUSH );
        QuickSave( HW_xCX, OP_PUSH );
        QuickSave( HW_xDX, OP_PUSH );
        QuickSave( HW_xBX, OP_PUSH );
        QuickSave( HW_xSP, OP_PUSH );
        QuickSave( HW_xBP, OP_PUSH );
        QuickSave( HW_xSI, OP_PUSH );
        QuickSave( HW_xDI, OP_PUSH );
    }
    QuickSave( HW_DS, OP_PUSH );
    QuickSave( HW_ES, OP_PUSH );
    if( _CPULevel( CPU_386 ) ) {
        QuickSave( HW_FS, OP_PUSH );
        QuickSave( HW_GS, OP_PUSH );
    } else {
        QuickSave( HW_xAX, OP_PUSH );
        QuickSave( HW_xAX, OP_PUSH );
    }
    GenRegMove( HW_xSP, HW_xBP );
    AllocStack();
    Gcld();
    if( HW_COvlap( data_state().CurrProc->state.unalterable, HW_DS ) ) {
        DoLoadDS();
        /*
         * If ES is also unalterable, copy DS to ES; else things
         * like memcpy() are likely to blow up
         */
        if( HW_COvlap( data_state().CurrProc->state.unalterable, HW_ES ) ) {
            QuickSave( HW_DS, OP_PUSH );
            QuickSave( HW_ES, OP_POP );
        }
    }
    /*
     * 8 general purpose registers + 4 segment registers
     */
    return( 12 * WORD_SIZE );
}


static  void    PopAll( void )
/****************************/
{
    if( data_state().CurrProc->locals.size != 0 ) {
        GenRegMove( HW_xBP, HW_xSP );
    }
    if( _CPULevel( CPU_386 ) ) {
        QuickSave( HW_GS, OP_POP );
        QuickSave( HW_FS, OP_POP );
    } else {
        QuickSave( HW_xAX, OP_POP );
        QuickSave( HW_xAX, OP_POP );
    }
    QuickSave( HW_ES, OP_POP );
    QuickSave( HW_DS, OP_POP );
    if( _CPULevel( CPU_186 ) ) {
        Gpopa();
    } else {
        QuickSave( HW_xDI, OP_POP );
        QuickSave( HW_xSI, OP_POP );
        QuickSave( HW_xBP, OP_POP );
        QuickSave( HW_xBX, OP_POP );
        QuickSave( HW_xBX, OP_POP );
        QuickSave( HW_xDX, OP_POP );
        QuickSave( HW_xCX, OP_POP );
        QuickSave( HW_xAX, OP_POP );
    }
}


static  void    DoEnter( level_depth level )
/******************************************/
{
    type_length size;

    /*
     * keep stack aligned
     */
    size = _RoundUp( data_state().CurrProc->locals.size, WORD_SIZE );
    data_state().CurrProc->locals.size = size;

    data_state().CurrProc->parms.base += WORD_SIZE;
    if( data_state().BlockByBlock ) {
        GenUnkEnter( &data_state().CurrProc->targ.prolog_loc, level );
        if( data_state().CurrProc->prolog_state & PST_TOUCH_STACK ) {
            GenTouchStack( true );
        }
    } else {
        GenEnter( size, level );
        if( size != 0
          && (data_state().CurrProc->prolog_state & PST_TOUCH_STACK) ) {
            GenTouchStack( false );
        }
    }
}


static  void    Enter( void )
/***************************/
{
    level_depth lex_level;
    int         i;

    lex_level = data_state().CurrProc->lex_level;
#if _TARGET & _TARG_80386
    if( !data_state().CurrProc->targ.sp_frame
      && _CPULevel( CPU_186 )
      && data_state().CurrProc->locals.size < 65536
      && ( lex_level > 0 || ( data_state().CurrProc->locals.size != 0 && data_state().OptForSize > 50 ) ) ) {
#else
    if( !data_state().CurrProc->targ.sp_frame
      && _CPULevel( CPU_186 )
      && ( lex_level > 0 || ( data_state().CurrProc->locals.size != 0 && data_state().OptForSize > 50 ) ) ) {
#endif
        DoEnter( lex_level );
        HW_CTurnOn( data_state().CurrProc->state.used, HW_xBP );
        data_state().CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
    } else {
        if( NeedBPProlog() ) {
            if( !data_state().CurrProc->targ.sp_frame
              || data_state().CurrProc->targ.sp_align ) {
                HW_CTurnOn( data_state().CurrProc->state.used, HW_xBP );
                data_state().CurrProc->parms.base += WORD_SIZE;
                QuickSave( HW_xBP, OP_PUSH );
                i = 0;
                while( --lex_level > 0 ) {
                    i -= 2;
                    GenPushOffset( i );
                }
                GenRegMove( HW_xSP, HW_xBP );
                if( data_state().CurrProc->lex_level > 1 ) {
                    GenRegAdd( HW_xBP, ( data_state().CurrProc->lex_level - 1 ) * WORD_SIZE );
                }
                if( data_state().CurrProc->lex_level > 0 ) {
                    QuickSave( HW_xBP, OP_PUSH );
                }
            }
            data_state().CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
        }
        AllocStack();
    }
}


static  void    CalcUsedRegs( void )
/**********************************/
{
    block   *blk;
    instruction *ins;
    name    *result;
    hw_reg_set  used;

    HW_CAsgn( used, HW_EMPTY );
    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            result = ins->result;
            if( result != NULL
              && result->n.class_ == N_REGISTER ) {
                HW_TurnOn( used, result->r.reg );
            }
            /*
             * place holder for big label doesn't really zap anything
             */
            if( ins->head.opcode != OP_NOP ) {
                HW_TurnOn( used, ins->zap->reg );
                if( HW_COvlap( ins->zap->reg, HW_xSP ) ) {
                    data_state().CurrProc->prolog_state |= PST_RESET_SP;
                }
            }
        }
    }
    if( !data_state().CurrProc->targ.sp_frame
      || data_state().CurrProc->targ.sp_align ) {
        HW_CTurnOff( used, HW_xBP );
    }
    HW_TurnOn( data_state().CurrProc->state.used, used );
}


static  int Push( hw_reg_set to_push )
/************************************/
{
    int         size;
    int         i;

    size = 0;
    if( _IsntModel( CGSW_GEN_NO_OPTIMIZATION )
      && data_state().CurrProc->targ.sp_frame
      && !data_state().CurrProc->targ.sp_align ) {
        FlowSave( &to_push );
    }
    for( i = 0; i < sizeof( PushRegs ) / sizeof( PushRegs[0] ) - 1; i++ ) {
        if( HW_CEqual( to_push, HW_EMPTY ) )
            break;
        if( HW_Ovlap( PushRegs[i], to_push ) ) {
            QuickSave( PushRegs[i], OP_PUSH );
            size += WORD_SIZE;
            HW_TurnOff( to_push, PushRegs[i] );
        }
    }
    return( size );
}

static  void        Pop( hw_reg_set to_pop )
/******************************************/
{
    int         i;

    if( _IsntModel( CGSW_GEN_NO_OPTIMIZATION )
      && data_state().CurrProc->targ.sp_frame
      && !data_state().CurrProc->targ.sp_align ) {
        FlowRestore( &to_pop );
    }
    for( i = sizeof( PushRegs ) / sizeof( PushRegs[0] ) - 1; i-- > 0 ; ) {
        if( HW_CEqual( to_pop, HW_EMPTY ) )
            break;
        if( HW_Ovlap( PushRegs[i], to_pop ) ) {
            QuickSave( PushRegs[i], OP_POP );
            HW_TurnOff( to_pop, PushRegs[i] );
        }
    }
}

static void    GenLeaSP( int offset )
/************************************
 * LEA         [e]sp,[[e]bp+offset]
 */
{
    _Code;
    LayOpword( M_LEA );
    OpndSize( HW_xSP );
    LayReg( HW_xSP );
    pcencode_state().Inst[RMR] |= Displacement( offset, HW_xBP );
    pcencode_state().Inst[RMR] |= DoIndex( HW_xBP );
    _Emit;
}

bool_    EpilogIsLeaveOnly( void )
/*************************************
 * The epilog is LEAVE and the return, nothing between: SP is set back from BP whatever it was
 * -- under -zmsc on a 186 or later, a frame with no registers of the caller's to pop
 */
{
    hw_reg_set  to_pop;

    if( _IsntTargetModel( CGSW_X86_MSC8 ) || !_CPULevel( CPU_186 ) )
        return( false );
    if( _RoutineIsInterrupt( data_state().CurrProc->state.attr ) || CHAIN_FRAME )
        return( false );
    if( (data_state().CurrProc->state.attr & ROUTINE_NEEDS_PROLOG) == 0 )
        return( false );
    if( data_state().CurrProc->targ.sp_frame || data_state().CurrProc->targ.base_adjust != 0 )
        return( false );
    if( (data_state().CurrProc->state.attr & ROUTINE_LOADS_DS) && HW_COvlap( data_state().CurrProc->state.unalterable, HW_DS ) )
        return( false );
    to_pop = SaveRegs();
    HW_CTurnOff( to_pop, HW_FLTS );
    HW_CTurnOff( to_pop, HW_xBP );
    return( HW_CEqual( to_pop, HW_EMPTY ) );
}

static  void    DoEpilog( void )
/******************************/
{
    hw_reg_set  to_pop;
    bool_    is_long;
    type_length size;

    if( _RoutineIsInterrupt( data_state().CurrProc->state.attr ) ) {
        PopAll();
    } else {
        to_pop = SaveRegs();
        HW_CTurnOff( to_pop, HW_FLTS );
        if( CHAIN_FRAME ) {
            if( (data_state().CurrProc->state.attr & ROUTINE_NEEDS_PROLOG)
              || ( data_state().CurrProc->locals.size + data_state().CurrProc->targ.push_local_size ) != 0 ) {
                if( data_state().CurrProc->targ.base_adjust == 0 ) {
                    GenRegMove( HW_xBP, HW_xSP );
                } else {
                    GenLeaSP( -data_state().CurrProc->targ.base_adjust );
                }
            }
            HW_CTurnOff( to_pop, HW_xBP );
        } else {
            if( data_state().CurrProc->state.attr & ROUTINE_NEEDS_PROLOG ) {
                size = data_state().CurrProc->locals.size + data_state().CurrProc->targ.push_local_size;
                if( (data_state().CurrProc->prolog_state & PST_RESET_SP)
                  || size != 0
                  /*
                   * Microsoft C 8 ends every frame with LEAVE on a 186 or later, whether or not
                   * it has locals to drop (-zmsc)
                   */
                  || ( _IsTargetModel( CGSW_X86_MSC8 ) && _CPULevel( CPU_186 ) ) ) {
                    /*
                     * sp is not pointing at saved registers already
                     */
                    if( data_state().CurrProc->targ.sp_frame ) {
                        if( data_state().CurrProc->targ.sp_align ) {
                            GenRegMove( HW_xBP, HW_xSP );
                            QuickSave( HW_xBP, OP_POP );
                        } else if( size != 0 ) {
                            GenRegAdd( HW_xSP, size );
                        }
                    } else if( data_state().CurrProc->targ.base_adjust != 0 ) {
                        GenLeaSP( -data_state().CurrProc->targ.base_adjust );
                    } else if( _CPULevel( CPU_186 )
                      && (!_CPULevel( CPU_486 ) || data_state().OptForSize > 50) ) {
                        GenLeave();
                        HW_CTurnOff( to_pop, HW_xBP );
                    } else {
                        GenRegMove( HW_xBP, HW_xSP );
                    }
                }
            }
        }
        Pop( to_pop );
        UnloadDS();
        if( CHAIN_FRAME ) {
            if( NeedBPProlog() ) {
                EpilogHook();
                if( FAR_RET_ON_STACK ) {
                    if( CHEAP_FRAME ) {
                        GenCheapWindowsEpilog();
                    } else {
                        GenWindowsEpilog();
                    }
                } else {
                    QuickSave( HW_xBP, OP_POP );
                }
            }
        }
        if( data_state().CurrProc->prolog_state & PST_PROLOG_RDOSDEV ) {
            GenRdosdevEpilog();
        }
    }

    is_long = ( _RoutineIsLong( data_state().CurrProc->state.attr )
                || _RoutineIsFar16( data_state().CurrProc->state.attr ) );
#if _TARGET & _TARG_80386
    if( data_state().CurrProc->prolog_state & PST_PROLOG_THUNK ) {
        QuickSave( HW_xSP, OP_POP );
    }
    if( _IsTargetModel( CGSW_X86_NEW_P5_PROFILING )
      || _IsTargetModel( CGSW_X86_P5_PROFILING ) ) {
        GenP5ProfilingEpilog( static_cast<label_handle>(data_state().CurrProc->label) );
    }
#endif

    if( _RoutineIsInterrupt( data_state().CurrProc->state.attr ) ) {
        GenIRET();
    } else if( data_state().CurrProc->state.attr & ROUTINE_REMOVES_PARMS ) {
        GenReturn( data_state().CurrProc->parms.size, is_long );
    } else {
        GenReturn( 0, is_long );
    }
}

bool_    CanZapBP( void )
/**********************/
{
    return( !CHAIN_FRAME );
}


void    AddCacheRegs( void )
/**************************/
{
#if _TARGET & _TARG_80386
    if( data_state().CurrProc->targ.never_sp_frame )
        return;
    if( _IsntModel( CGSW_GEN_MEMORY_LOW_FAILS ) )
        return;
    if( data_state().OptForSize > 50 )
        return;
    if( _IsTargetModel( CGSW_X86_FLOATING_DS )
      || _IsTargetModel( CGSW_X86_FLOATING_SS ) )
        return;
    if( !ScanInstructions() )
        return;
    if( !CanZapBP() )
        return;
    if( !ScanLabelCalls() )
        return;
    if( data_state().CurrProc->state.attr & ROUTINE_WANTS_DEBUGGING )
        return;
    if( data_state().CurrProc->lex_level > 0 )
        return;
    if( _FPULevel( FPU_586 ) ) {
        ScanForFDOps();
    }
    if( data_state().CurrProc->targ.has_fd_temps ) {
        data_state().CurrProc->targ.sp_frame = true;
        data_state().CurrProc->targ.sp_align = true;
    } else if( !DO_BP_CHAIN
      && _IsntTargetModel( CGSW_X86_WINDOWS )
      && !_RoutineIsInterrupt( data_state().CurrProc->state.attr ) ) {
        /*
         * We cannot make EBP available under Windows because the SS
         * selector might not cover the data segment and so we cannot use
         * it as an index. Puke - BBB Feb 18, 1994
         */
        data_state().CurrProc->targ.sp_frame = true;
        HW_CTurnOff( data_state().CurrProc->state.unalterable, HW_xBP );
    }
#endif
}


void DoRTCall( rt_class rtindex, bool_ pop )
/*****************************************/
{
    DoCall( RTLabel( rtindex ), true, _IsTargetModel( CGSW_X86_BIG_CODE ), pop );
}

static unsigned returnAddressStackSize( void )
/********************************************/
{
    unsigned    size;

    if( _RoutineIsInterrupt( data_state().CurrProc->state.attr ) ) {
        size = 0;
    } else if( data_state().CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS ) {
        size = 0;
    } else if( _RoutineIsLong( data_state().CurrProc->state.attr ) ) {
        size = 2 * WORD_SIZE;
    } else if( _RoutineIsFar16( data_state().CurrProc->state.attr ) ) {
        size = 2 * WORD_SIZE;
    } else {
        size = WORD_SIZE;
    }
    return( size );
}

void    GenProlog( void )
/***********************/
{
    hw_reg_set  to_push;
    unsigned    ret_size;
    pointer     label;
    pointer     origlabel;  /* Original label for generated __far16 thunks */
    fe_attr     attr;

    ScanInstructions();     /* Do These 2 calls before using DO_WINDOWS_CRAP! */
    FindIfExported();
    PUSH_OP( AskCodeSeg() );
        if( data_state().CurrProc->prolog_state & PST_FUNCTION_NAME ) {
            EmitNameInCode();
        }

        if( _IsModel( CGSW_GEN_DBG_NUMBERS ) ) {
            CodeLineNumber( data_state().HeadBlock->ins.head.line_num, false );
        }

        if( _IsModel( CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
            EmitRtnBeg();
        }
        if( data_state().CurrProc->state.attr & ROUTINE_WANTS_DEBUGGING ) {
            data_state().CurrProc->state.attr |= ROUTINE_NEEDS_PROLOG;
        }

        data_state().CurrProc->parms.base = 0;
        data_state().CurrProc->parms.size = data_state().CurrProc->state.parm.offset;

        origlabel = label = data_state().CurrProc->label;

#if _TARGET & _TARG_80386
        if( _RoutineIsFar16( data_state().CurrProc->state.attr ) ) {
            label = GenFar16Thunk( data_state().CurrProc->label, data_state().CurrProc->parms.size,
                        data_state().CurrProc->state.attr & ROUTINE_REMOVES_PARMS );
//            CurrProc->label = label; /* ugly mess if following are combined */
        }
#endif

        if( _IsTargetModel( CGSW_X86_MSC8 ) ) {
            /*
             * Microsoft C 8 starts every function on an even byte, at /Os as well, padding
             * with NOP (-zmsc)
             */
            CodeHandle( OC_LABEL, 2 - 1, static_cast<label_handle>(label) );
        } else {
            CodeLabel( static_cast<label_handle>(label), DepthAlign( PROC_ALIGN ) );
        }

        attr = FEAttr( AskForLblSym( static_cast<label_handle>(origlabel) ) );

        if( data_state().CurrProc->prolog_state & PST_PROLOG_RDOSDEV ) {
            GenRdosdevProlog();
        }

    #if _TARGET & _TARG_80386
        if( (attr & FE_NAKED) == 0 ) {
            if( _IsTargetModel( CGSW_X86_NEW_P5_PROFILING )
              || _IsTargetModel( CGSW_X86_P5_PROFILING ) ) {
                GenP5ProfilingProlog( static_cast<label_handle>(label) );
            }
            if( data_state().CurrProc->prolog_state & PST_PROLOG_THUNK ) {
                QuickSave( HW_xSP, OP_PUSH );
                GenPushC( data_state().CurrProc->parms.size );
                GenUnkPush( &data_state().CurrProc->targ.stack_check );
                if( NeedStackCheck() ) {
                    DoRTCall( RT_THUNK_STK, true );
                } else {
                    DoRTCall( RT_THUNK, true );
                }
                data_state().CurrProc->parms.base += WORD_SIZE;
            }
        }
    #endif

        ret_size = returnAddressStackSize();
        data_state().CurrProc->parms.base += ret_size;
        CalcUsedRegs();

        to_push = SaveRegs();
        HW_CTurnOff( to_push, HW_FLTS );
        if( !data_state().CurrProc->targ.sp_frame
          || data_state().CurrProc->targ.sp_align ) {
            HW_CTurnOff( to_push, HW_xBP );
        }

        if( attr & FE_NAKED ) {
            /*
             * don't do anything - empty prologue
             */
        } else if( _RoutineIsInterrupt( data_state().CurrProc->state.attr ) ) {
            ret_size = -PushAll();
            data_state().CurrProc->targ.base_adjust = 0;
            MoveParms();
        } else {
            if( CHAIN_FRAME ) {
                data_state().CurrProc->targ.base_adjust = 0;
                if( NeedBPProlog() ) {
                    HW_CTurnOn( data_state().CurrProc->state.used, HW_xBP );
                    data_state().CurrProc->parms.base += WORD_SIZE;
                    if( FAR_RET_ON_STACK ) {
                        if( CHEAP_FRAME ) {
                            GenCheapWindowsProlog();
                        } else {
    #if _TARGET & _TARG_8086
                            /*
                             * Windows prologs zap AX, so warn idiot user if we
                             * generate one for a routine in which AX is live
                             * upon entry to routine, or unalterable.
                             */
                            if( HW_COvlap( data_state().CurrProc->state.unalterable, HW_xAX )
                              || HW_COvlap( data_state().CurrProc->state.parm.used, HW_xAX ) ) {
                                #define MSGFMT  "exported routine '%-.256s' with AX live on entry"

                                cg_sym_handle sym;
                                const char    *src;
                                char          buffer[sizeof( MSGFMT ) - 7 + 256];

                                src = NULL;
                                sym = AskForLblSym( static_cast<label_handle>(data_state().CurrProc->label) );
                                if( sym != NULL ) {
                                    src = FEExtName( sym, EXTN_BASENAME );
                                }
                                if( src == NULL ) {
                                    src = "";
                                }
                                sprintf( buffer, MSGFMT, src );
                                FEMessage( FEMSG_ERROR, buffer );
                                #undef MSGFMT
                            }
    #endif
                            GenWindowsProlog();
                            data_state().CurrProc->targ.base_adjust += 2; /* the extra push DS */
                        }
                    } else {
                        QuickSave( HW_xBP, OP_PUSH );
                        GenRegMove( HW_xSP, HW_xBP );
                    }
                    PrologHook();
                }
                DoStackCheck();
                data_state().CurrProc->targ.base_adjust += LoadDS();
                data_state().CurrProc->targ.base_adjust += Push( to_push );
                data_state().CurrProc->parms.base += data_state().CurrProc->targ.base_adjust;
                AllocStack();
                AdjustPushLocals();
            } else {
                DoStackCheck();
                data_state().CurrProc->parms.base += LoadDS();
                if( (data_state().CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS) == 0 ) {
                    data_state().CurrProc->parms.base += Push( to_push );
                }
                Enter();
                AdjustPushLocals();
                if( _IsModel( CGSW_GEN_NO_OPTIMIZATION )
                  || data_state().CurrProc->targ.sp_frame ) {
                    data_state().CurrProc->targ.base_adjust = 0;
                } else {
                    data_state().CurrProc->targ.base_adjust = AdjustBase();
                    if( data_state().CurrProc->targ.base_adjust != 0 ) {
                        GenRegSub( HW_xBP, -data_state().CurrProc->targ.base_adjust );
                    }
                }
            }
            RelocParms();
            MoveParms();
        }
        data_state().CurrProc->prolog_state |= PST_PROLOG_GENERATED;

        if( _IsModel( CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
            DbgRetOffset( data_state().CurrProc->parms.base - data_state().CurrProc->targ.base_adjust - ret_size );
            EmitProEnd();
        }
    POP_OP();

    if( data_state().CurrProc->prolog_state & PST_EXPORT ) {
        OutDLLExport( WORDS_COUNT( data_state().CurrProc->parms.size ), AskForLblSym( static_cast<label_handle>(data_state().CurrProc->label) ) );
    }
}


void    InitStackDepth( block *blk )
/**********************************/
{
    if( blk->edge[0].flags & BEF_DOWN_ONE_CALL ) {
        x86proc_state().StackDepth = WORD_SIZE;
    } else {
        x86proc_state().StackDepth = 0;
    }
    x86proc_state().StackDepth += blk->stack_depth;
}


void        AdjustStackDepth( instruction *ins )
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
            _Zoiks( ZOIKS_077 );
            return;
        }
        adjust = op->c.lo.u.int_value;
        if( ins->head.opcode == OP_SUB ) {
            x86proc_state().StackDepth += adjust;
        } else {
            x86proc_state().StackDepth -= adjust;
        }
        break;
    case OP_PUSH:
        x86proc_state().StackDepth += WORD_SIZE;
        break;
    case OP_POP:
        x86proc_state().StackDepth -= WORD_SIZE;
        break;
    case OP_CALL:
    case OP_CALL_INDIRECT:
        if( ins->flags.u.call_flags & CALL_POPS_PARMS ) {
            op = ins->operands[CALL_OP_POPS];
            if( op->n.class_ == N_CONSTANT ) {
                x86proc_state().StackDepth -= op->c.lo.u.int_value;
            }
        }
    default:
        break;
    }
}

void     AdjustStackDepthDirect( int adjust )
/*******************************************/
{
    x86proc_state().StackDepth += adjust;
}


bool_    BaseIsSP( name *op )
/**************************/
{
    if( !data_state().CurrProc->targ.sp_frame )
        return( false );
    if( data_state().CurrProc->targ.sp_align
      && (op->t.temp_flags & STACK_PARM) ) {
        return( false );
    }
    return( true );
}


type_length NewBase( name *op )
/*****************************/
{
    if( !BaseIsSP( op ) ) {
        return( op->t.location - data_state().CurrProc->targ.base_adjust );
    }
    return( op->t.location + data_state().CurrProc->locals.size
        + data_state().CurrProc->targ.push_local_size + x86proc_state().StackDepth );
}

type_length PushSize( type_length len )
/*************************************/
{
    if( len < WORD_SIZE )
        return( WORD_SIZE );
    return( _RoundUp( len, WORD_SIZE ) );
}


void    GenEpilog( void )
/***********************/
{
    type_length stack;
    fe_attr attr;

    attr = FEAttr( AskForLblSym( static_cast<label_handle>(data_state().CurrProc->label) ) );

    stack = - data_state().CurrProc->locals.size - data_state().CurrProc->locals.base
         - data_state().CurrProc->targ.push_local_size;
    PatchBigLabels( stack );

    if( _IsModel( CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
        EmitEpiBeg();
    }

    if( (attr & FE_NAKED) == 0 ) {
        if( (data_state().CurrProc->state.attr & ROUTINE_NEVER_RETURNS_ABORTS) == 0 ) {
            DoEpilog();
        }

        if( data_state().BlockByBlock ) {
            AbsPatch( data_state().CurrProc->targ.prolog_loc, data_state().CurrProc->locals.size );
        }

        if( data_state().CurrProc->targ.stack_check != NULL ) {
            AbsPatch( data_state().CurrProc->targ.stack_check,
                  data_state().CurrProc->locals.size +
                  data_state().CurrProc->parms.base  +
                  WORD_SIZE * data_state().CurrProc->lex_level +
                  data_state().CurrProc->targ.push_local_size +
                  data_state().MaxStack );
        }
    }


    data_state().CurrProc->prolog_state |= PST_EPILOG_GENERATED;
    if( _IsModel( CGSW_GEN_DBG_LOCALS ) ) {  // d1+ or d2
        EmitRtnEnd();
    }
}


int AskDisplaySize( level_depth level )
/*************************************/
{
    return( level * WORD_SIZE );
}

int ParmsAtPrologue( void )
/*************************/
{
    unsigned    ret_size;
    unsigned    parms_off_sp;

    parms_off_sp= 0;

#if _TARGET & _TARG_80386
    if( data_state().CurrProc->prolog_state & PST_PROLOG_THUNK ) {
        parms_off_sp += WORD_SIZE;
    }
#endif

    ret_size = returnAddressStackSize();
    parms_off_sp += ret_size;
    return( parms_off_sp );
}
