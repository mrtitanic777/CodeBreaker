/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "cgaux.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "zoiks.h"
#include "makeins.h"
#include "data.h"
#include "display.h"
#include "rtrtn.h"
#include "objout.h"
#include "types.h"
#include "makeaddr.h"
#include "namelist.h"
#include "makeblk.h"
#include "rgtbl.h"
#include "split.h"
#include "insutil.h"
#include "typemap.h"
#include "bldcall.h"
#include "bldins.h"
#include "x86segs.h"
#include "parm.h"
#include "bgcall.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "backrun.hpp"


static  void    AddCall( BackRun& back, instruction *ins, cn call );

#if _TARGET & _TARG_80386
static  void    Far16Parms( BackRun& back, cn call )
/***********************************/
{
    instruction         *ins;
    type_length         parm_size;
    pn                  parm, next;
    instruction         *call_ins;
    name                *eax;
    name                *ecx;
    name                *esi;
    label_handle        lbl;
    type_length         offset;
    name                *parmlist;
    call_state          *state;
    rt_class            rtindex;

    call_ins = call->ins;
    parm_size = 0;
    state = call->state;
    for( parm = call->parms; parm != NULL; parm = parm->next ) {
        parm_size += RoundUp( parm->name->tipe->length, 2 );
    }
    parmlist = SAllocTemp( back, XX, parm_size );
    parmlist->v.usage |= NEEDS_MEMORY | USE_IN_ANOTHER_BLOCK | USE_ADDRESS;
    offset = 0;
    for( parm = call->parms; parm != NULL; parm = parm->next ) {
        parm->name->u.i.ins->result = STempOffset( back, parmlist, offset,
                                                 TypeClass( back, parm->name->tipe ),
                                                 parm->name->tipe->length );
        offset += RoundUp( parm->name->tipe->length, 2 );
    }
    for( parm = call->parms; parm != NULL; parm = next ) {
        next = parm->next;
        parm->name->format = NF_ADDR;   /* so instruction doesn't get freed! */
        BGDone( back, parm->name );
        CGFree( back, parm );
    }
    eax = AllocRegName( back, HW_EAX );
    ecx = AllocRegName( back, HW_ECX );
    esi = AllocRegName( back, HW_ESI );
    HwTurnOn( state->parm.used, eax->r.reg );
    HwTurnOn( state->parm.used, ecx->r.reg );
    HwTurnOn( state->parm.used, esi->r.reg );
    ins = MakeMove( back, AllocS32Const( back, parm_size ), ecx, WD );
    AddIns( back, ins );
    ins = MakeUnary( back, OP_LA, parmlist, esi, WD );
    AddIns( back, ins );
    if( ins->head.opcode == OP_CALL ) {
        ins = MakeUnary( back, OP_LA, call->name->u.n.name, eax, WD );
    } else {
        ins = MakeMove( back, GenIns( back, call->name ), eax, WD );
        call_ins->head.opcode = OP_CALL;
    }
    call_ins->num_operands = 2;
    AddIns( back, ins );
    if( call_ins->type_class == XX ) {
        if( state->attr & ROUTINE_ALLOCS_RETURN ) {
            rtindex = RT_Far16Cdecl;
        } else {
            rtindex = RT_Far16Pascal;
        }
    } else {
        rtindex = RT_Far16Func;
    }
    lbl = RTLabel( back, rtindex );
    call->name->u.n.name = AllocMemory( back, lbl, 0, CG_LBL, WD );
    call_ins->flags.u.call_flags |= CALL_FAR16 | CALL_POPS_PARMS;
    call_ins->operands[CALL_OP_USED] = AllocRegName( back, state->parm.used );
    call_ins->operands[CALL_OP_POPS] = AllocS32Const( back, 0 );
    call_ins->zap = &call_ins->operands[CALL_OP_USED]->r;
}
#endif


an      BGCall( BackRun& back, cn call, bool use_return, bool aux_inline )
/*********************************************************/
{
    instruction         *call_ins;
    call_state          *state;
    name                *ret_ptr = NULL;
    name                *result;
    name                *temp;
    name                *reg_name;
    instruction         *ret_ins = NULL;
    hw_reg_set          return_reg;
    hw_reg_set          zap_reg;

    if( call->name->tipe == back.data.TypeProcParm ) {
        SaveDisplay( back, OP_PUSH );
    }

    state = call->state;
    result = BGNewTemp( back, call->tipe );
    call_ins = call->ins;
    /*
     * If we have a return value that won't fit in a register
     * pass a pointer to result as the first parm
     */
    if( call_ins->type_class == XX ) {
        if( RoutineIsFar16( state->attr ) ) {
            if( state->attr & ROUTINE_ALLOCS_RETURN ) {
                HW_CAsgn( state->return_reg, HW_EAX );
            } else {
                HW_CAsgn( state->return_reg, HW_EBX );
            }
        }
        if( (state->attr & ROUTINE_ALLOCS_RETURN) == 0 ) {
            if( HW_CEqual( state->return_reg, HW_EMPTY ) ) {
                ret_ptr = AllocTemp( back, WD );
            } else {
                ret_ptr = AllocRegName( back, state->return_reg );
            }
            ret_ins = MakeUnary( back, OP_LA, result, ret_ptr, WD );
            HwTurnOn( state->parm.used, state->return_reg );
            call_ins->flags.u.call_flags |= CALL_RETURNS_STRUCT;
        }
    }
    if( IsTargetModel( back, CGSW_X86_FLOATING_DS )
      && (state->attr & ROUTINE_NEEDS_DS_LOADED) ) {
        HW_CTurnOn( state->parm.used, HW_DS );
    }
    if( RoutineIsFar16( state->attr ) ) {
#if _TARGET & _TARG_80386
        Far16Parms( back, call );
#endif
    } else {
        if( AssgnParms( back, call, aux_inline ) ) {
            if( state->attr & ROUTINE_REMOVES_PARMS ) {
                call_ins->flags.u.call_flags |= CALL_POPS_PARMS;
            }
        }
    }

    if( state->attr & (ROUTINE_MODIFIES_NO_MEMORY | ROUTINE_NEVER_RETURNS_ABORTS | ROUTINE_NEVER_RETURNS_NORETURN) ) {
        /*
         * a routine that never returns can not write any memory
         * as far as this routine is concerned
         */
        call_ins->flags.u.call_flags |= CALL_WRITES_NO_MEMORY;
    }
    if( state->attr & ROUTINE_READS_NO_MEMORY ) {
        call_ins->flags.u.call_flags |= CALL_READS_NO_MEMORY;
    }
    if( state->attr & ROUTINE_NEVER_RETURNS_ABORTS ) {
        call_ins->flags.u.call_flags |= CALL_ABORTS;
    }
    if( state->attr & ROUTINE_NEVER_RETURNS_NORETURN ) {
        call_ins->flags.u.call_flags |= CALL_NORETURN;
    }
    if( RoutineIsInterrupt( state->attr ) ) {
        call_ins->flags.u.call_flags |= CALL_INTERRUPT | CALL_POPS_PARMS;
    }
    if( state->attr & ROUTINE_NEEDS_BP_CHAIN ) {
        call_ins->flags.u.call_flags |= CALL_NEEDS_BP_CHAIN;
    }
    if( !use_return ) {
        call_ins->flags.u.call_flags |= CALL_IGNORES_RETURN;
    }
    if( call_ins->type_class == XX ) {
        reg_name = AllocRegName( back, state->return_reg );
        if( state->attr & ROUTINE_ALLOCS_RETURN ) {
            call_ins->result = reg_name;
            AddCall( back, call_ins, call );
            if( use_return ) {
                temp = AllocTemp( back, WD ); /* assume near pointer*/
                AddIns( back, MakeMove( back, reg_name, temp, WD ) );
                temp = SAllocIndex( back, temp, NULL, 0,
                                    result->n.type_class, call->tipe->length );
                AddIns( back, MakeMove( back, temp, result, result->n.type_class ) );
            }
        } else {
            call_ins->result = result;
            AddIns( back, ret_ins );
            if( HW_CEqual( state->return_reg, HW_EMPTY ) ) {
                AddIns( back, MakeUnary( back, OP_PUSH, ret_ptr, NULL, WD ) );
                state->parm.offset += back.data.TypeClassSize[WD];
                call_ins->operands[CALL_OP_POPS] =
                        AllocS32Const( back, call_ins->operands[CALL_OP_POPS]->c.lo.u.int_value + back.data.TypeClassSize[WD] );
                if( state->attr & ROUTINE_REMOVES_PARMS ) {
                    call_ins->flags.u.call_flags |= CALL_POPS_PARMS;
                }
            }
            AddCall( back, call_ins, call );
        }
    } else {
        return_reg = state->return_reg;
        zap_reg = call_ins->zap->reg;
        HW_CTurnOn( zap_reg, HW_FLTS );
        HwOnlyOn( return_reg, zap_reg );
        call_ins->result = AllocRegName( back, return_reg );
        reg_name = AllocRegName( back, state->return_reg );
        AddCall( back, call_ins, call );
        if( use_return ) {
            ret_ins = MakeMove( back, reg_name, result, result->n.type_class );
            if( HW_COvlap( reg_name->r.reg, HW_FLTS ) ) {
                ret_ins->stk_entry = 1;
                ret_ins->stk_exit = 0;
            }
            AddIns( back, ret_ins );
        }
    }
    if( state->parm.offset != 0
      && (state->attr & ROUTINE_REMOVES_PARMS) == 0 ) {
        reg_name = AllocRegName( back, HW_xSP );
        AddIns( back, MakeBinary( back, OP_ADD, reg_name,
                AllocS32Const( back, state->parm.offset ), reg_name, WD ) );
    }
    return( MakeTempAddr( back, result ) );
}


void    BGProcDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe )
/***********************************************************/
{
    hw_reg_set          reg;
    name                *temp;
    type_class_def      type_class;
    label_handle        lbl;

    back.model.SaveTargetModel = back.model.TargetModel;
    type_class = AddCallBlock( back, sym, tipe );
    if( tipe != back.data.TypeNone ) {
        if( type_class == XX ) {
            if( back.data.CurrProc->state.attr & ROUTINE_ALLOCS_RETURN ) {
                { segment_id old_segid = ChangeOP( back, AskBackSeg(back) );
                    lbl = AskForNewLabel(back);
                    DataLabel( back, lbl );
                    DGUBytes( back, tipe->length );
                    back.data.CurrProc->targ.return_points = SAllocMemory( back, lbl, 0, CG_LBL, TypeClass( back, tipe ), tipe->length );
                SetOP( back, old_segid ); }
            } else {
                reg = back.data.CurrProc->state.return_reg;
                if( HW_CEqual( reg, HW_EMPTY ) ) {
                    temp = DoParmDecl( back, NULL, back.data.TypeInteger, HW_EMPTY );
                } else {
                    temp = AllocTemp( back, WD );
                    temp->v.usage |= USE_IN_ANOTHER_BLOCK;
                    AddIns( back, MakeMove( back, AllocRegName( back, reg ), temp, WD ) );
                    HwTurnOn( back.data.CurrProc->state.parm.used, reg );
                }
                back.data.CurrProc->targ.return_points = temp;
            }
        }
    }
    if( back.data.CurrProc->state.attr & ROUTINE_FARSS ) {
        back.model.TargetModel |= CGSW_X86_FLOATING_SS;
    }
}


name    *StReturn( BackRun& back, an retval, const type_def *tipe, instruction **pins )
/**********************************************************************/
{
    name        *retp;
    name        *ptr;
    name        *off;
    name        *seg;
    hw_reg_set  reg;

    if( back.data.CurrProc->state.attr & ROUTINE_ALLOCS_RETURN ) {
        retp = back.data.CurrProc->targ.return_points;
        AddIns( back, MakeUnary( back, OP_LA, retp, AllocRegName( back, back.data.CurrProc->state.return_reg ), WD ) );
        *pins = NULL;
    } else {
        if( IsTargetModel( back, CGSW_X86_FLOATING_SS )
          || IsTargetModel( back, CGSW_X86_FLOATING_DS ) ) {
            ptr = AllocTemp( back, CP );
            off = OffsetPart( back, ptr );
            seg = SegmentPart( back, ptr );
            AddIns( back, MakeMove( back, AllocRegName( back, HW_SS ), seg, U2 ) );
        } else {
            ptr = AllocTemp( back, WD );
            off = ptr;
        }
        AddIns( back, MakeMove( back, back.data.CurrProc->targ.return_points, off, WD ) );
        retp = SAllocIndex( back, ptr, NULL, 0, TypeClass( back, retval->tipe ), tipe->length );
        reg = ReturnReg( back, WD, false );
        *pins = MakeMove( back, back.data.CurrProc->targ.return_points, AllocRegName( back, reg ), WD );
        back.data.CurrProc->state.return_reg = reg;
    }
    return( retp );
}


static  void    AddCall( BackRun& back, instruction *ins, cn call ) {
/****************************************************/

    name        *proc_name;

    if( IsTargetModel( back, CGSW_X86_FLOATING_DS )
      && (call->state->attr & ROUTINE_NEEDS_DS_LOADED) ) {
        AddIns( back, MakeMove( back, NearSegment(back), AllocRegName( back, HW_DS ), U2 ) );
    }
    if( call->name->tipe == back.data.TypeProcParm ) {
        proc_name = AllocTemp( back, back.data.ClassPointer );
/* what to do?        proc_name->usage |= USE_REGISTER;*/
        AddIns( back, MakeMove( back, ins->operands[CALL_OP_ADDR], proc_name, back.data.ClassPointer ));
        ins->operands[CALL_OP_ADDR] = proc_name;
        SetDisplay( back, GenIns( back, call->name ) );
        AddIns( back, ins );
        SaveDisplay( back, OP_POP );
    } else {
        AddCallIns( back, ins, call );
    }
}


reg_set_index   CallIPossible( instruction *ins )
/***********************************************/
{
     if( ins->operands[CALL_OP_ADDR]->n.type_class == CP )
        return( RL_ );
     if( ins->operands[CALL_OP_ADDR]->n.type_class == PT )
        return( RL_ );
#if _TARGET & _TARG_8086
     return( RL_WORD );
#else
     return( RL_DOUBLE );
#endif
}


void    InitTargProc( BackRun& back )
/**************************/
{
    back.data.CurrProc->targ.stack_check = NULL;
    back.data.CurrProc->targ.push_local_size = 0;
    back.data.CurrProc->targ.debug = NULL;
    back.data.CurrProc->targ.return_points = NULL;
    back.data.CurrProc->targ.sp_frame = false;
    back.data.CurrProc->targ.sp_align = false;
    back.data.CurrProc->targ.has_fd_temps = false;
    back.data.CurrProc->targ.never_sp_frame = false;
    back.data.CurrProc->targ.tls_index = NULL;
}


void    SaveToTargProc( BackRun& back )
/****************************/
{
    back.data.CurrProc->targ.max_stack = back.data.MaxStack;
}


void    RestoreFromTargProc( BackRun& back )
/*********************************/
{
    back.data.MaxStack = back.data.CurrProc->targ.max_stack;
}


void    PushInSameBlock( BackRun& back, instruction *ins )
/*****************************************/
{
#if ( _TARGET & _TARG_8086 )
    /* unused parameters */ (void)ins;
#else
    if( InsBlock( ins ) != back.data.CurrBlock ) {
        back.data.CurrProc->targ.never_sp_frame = true;
    }
#endif
}


instruction *   PushOneParm( BackRun& back, instruction *ins, name *curr,
                                type_class_def type_class,
                                type_length offset,
                                call_state *state )
/********************************************************/
{
    instruction *new_ins;
//    int         size;

    /* unused parameters */ (void)state; (void)offset;

    new_ins = MakeUnary( back, OP_PUSH, curr, NULL, type_class );
    SuffixIns( back, ins, new_ins );
    return( new_ins );
}


void    PreCall( cn call )
/************************/
{
    /* unused parameters */ (void)call;
}


void    PostCall( cn call )
/*************************/
{
    /* unused parameters */ (void)call;
}

const type_def  *PassParmType( BackRun& back, cg_sym_handle func, const type_def *tipe )
/***********************************************************************/
{
    if( (call_class_target)(pointer_uint)FindAuxInfoSym( back, func, FEINF_CALL_CLASS_TARGET ) & FECALL_X86_FAR16_CALL )
        return( tipe );
    return( QParmType( back, func, NULL, tipe ) );
}
