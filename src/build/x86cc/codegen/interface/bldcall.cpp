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
* Description:  Build a subroutine or a subroutine call.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "zoiks.h"
#include "namelist.h"
#include "makeins.h"
#include "convins.h"
#include "data.h"
#include "bldins.h"
#include "makeaddr.h"
#include "bgcall.h"
#include "fpu.h"
#include "dbsyms.h"
#include "blips.h"
#include "redefby.h"
#include "intrface.h"
#include "makeblk.h"
#include "x86objd.h"
#include "x86obj.h"
#include "rgtbl.h"
#include "insutil.h"
#include "namelist.h"
#include "typemap.h"
#include "bldcall.h"
#include "parmreg.h"
#include "generate.h"
#include "parm.h"
#include "trecurse.h"
#include "tree.h"
#include "treeprot.h"
#include "feprotos.h"
#include "backrun.hpp"


type_class_def  AddCallBlock( BackRun& back, cg_sym_handle sym, const type_def *tipe )
/*********************************************************************
 * create the initial basic block for routine "sym", and call some
 * other initialization routines.
 */
{
    type_class_def      type_class;

    if( back.blips.BlipsOn ) {
        PGBlip( FEName( *back.front, sym ) );
    }
    NewProc( back, FELexLevel( sym ) );
    EnLink( back, AskForSymLabel( back, sym, CG_FE ) );
    MarkBlkAttrSet( back.data.CurrBlock, BLK_BIG_LABEL );
    back.data.CurrProc->label = back.data.CurrBlock->label;
    type_class = InitCallState( back, tipe );
    AddIns( back, MakeNop(back) );
    return( type_class );
}


void    BGFiniCall( BackRun& back, cn call )
/************************************
 * free up a call node
 */
{
    BGDone( back, call->name );
    CGFree( back, call->state->parm.table );
    CGFree( back, call->state );
    CGFree( back, call );
}


cn      BGInitCall( BackRun& back, an node, const type_def *tipe, aux_handle aux )
/******************************************************************
 * initialize a call node for a call to routine "node", with type
 * "tipe" and aux handle "aux". This also allocates the instruction
 * to hold the call and parially initializes it.
 */
{
    cn                  call;
    type_class_def      type_class;
    name                *mem;
    void                *cookie;

    if( tipe->refno == TY_DEFAULT ) {
        tipe = back.data.TypeInteger;
    }
    call = static_cast<cn>(CGAlloc( back, sizeof( call_node ) ));
    call->state = static_cast<struct call_state *>(CGAlloc( back, sizeof( call_state ) ));
    call->name = node;
    call->tipe = tipe;
    call->parms = NULL;
    call->ins = NewIns( back, 3 );
    if( node->format == NF_ADDR && node->class_ == CL_ADDR_GLOBAL ) {
        call->ins->head.opcode = OP_CALL;
        type_class = CallState( back, aux, tipe, call->state );
        mem = node->u.n.name;
        mem = SAllocMemory( back, mem->v.symbol, mem->v.offset, mem->m.memory_type,
                            mem->n.type_class, mem->n.size );
        node->u.n.name = mem;
    } else {
        call->ins->head.opcode = OP_CALL_INDIRECT;
        type_class = CallState( back, aux, tipe, call->state );
        cookie = FEAuxInfo( *back.front, aux, FEINF_VIRT_FUNC_REFERENCE );
        if( cookie != NULL )
            TellObjVirtFuncRef( back, cookie );
    }
    call->ins->type_class = type_class;
    return( call );
}


void    BGAddParm( BackRun& back, cn call, an parm )
/**********************************************
 * link a parm into the list of parms for a call node
 */
{
    pn          new_parm;

    new_parm = static_cast<pn>(CGAlloc( back, sizeof( parm_node ) ));
    new_parm->name = AddrToIns( back, parm );
    new_parm->name->flags |= FL_ADDR_OK_ACROSS_BLOCKS; /* always taken care of by BGCall*/
    new_parm->next = call->parms;
    call->parms = new_parm;
}


void    BGAutoDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe )
/*************************************************************
 * declare an automatic variable with name "sym" and type "tipe". This
 * just creates the appropriate back end symbol table entry. (eg: N_TEMP)
 */
{
    BGDone( back, MakeAddrName( back, CG_FE, sym, tipe ) );
}

static  void    LinkParmIns( instruction *parm_def, instruction *ins )
/********************************************************************/
{
    ins->operands[2] = reinterpret_cast<name *>(parm_def); /* link them together*/
    parm_def->operands[2] = reinterpret_cast<name *>(ins);
    ins->ins_flags |= INS_PARAMETER;
    parm_def->ins_flags |= INS_PARAMETER;
}

static  instruction *DoParmDef( BackRun& back, name *result, type_class_def type_class )
/***********************************************************************/
{
    instruction *parm_def;

    parm_def = NewIns( back, 0 );
    parm_def->head.opcode = OP_PARM_DEF;
    parm_def->type_class = type_class;
    parm_def->result = result;
    AddIns( back, parm_def );
    return( parm_def );
}


name    *DoParmDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe, hw_reg_set reg )
/**************************************************************************
 * Declare a parameter of type "tipe" for the current routine. This
 * routine determines whether the parameter is coming in in a register
 * or on the stack and generates a temporary to hold the parm (temp) and
 *     PARMDEF   => x
 *     MOV     x => temp
 * and links these two instructions together. (x may be a register or
 * a temporary that is pre-allocated to a location on the stack)
 * in the current basic block. If the parm is coming in on the stack,
 * it sets the location field of the temp to be the same as the location it
 * arrives in on the stack, so if we do assign the
 * parm to memory, it gets its incoming location on the stack.
 */
{
    instruction         *ins;
    instruction         *parm_def;
    name                *temp;
    name                *parm_name;
    type_class_def      type_class;
    const type_def      *ptipe;
    bool                no_temp;

    ptipe = QParmType( back, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ), sym, tipe );
    type_class = TypeClass( back, ptipe );
    if( sym == NULL ) {
        temp = AllocTemp( back, TypeClass( back, tipe ) );
    } else {
        temp = SAllocUserTemp( back, sym, TypeClass( back, tipe ), tipe->length );
        BGDone( back, MakeAddrName( back, CG_FE, sym, tipe ) );
    }
    temp->v.usage |= USE_IN_ANOTHER_BLOCK;

    no_temp = ( type_class == XX );
    /*
     * The arguments of an interrupt routine coming in on the stack are used
     * for input and output; routine epilog pops them into registers. Handle
     * them specially to stop the optimizer from eliminating the writes (don't
     * create temps normally used with simple types).
     */
    if( RoutineIsInterrupt( back.data.CurrProc->state.attr ) && HW_CEqual( reg, HW_EMPTY ) ) {
        no_temp = true;
        temp->v.usage |= USE_ADDRESS;
    }
    if( HW_CEqual( reg, HW_EMPTY ) ) {
        if( no_temp ) {
            parm_name = temp;
            parm_name->t.location = ParmMem( back, tipe->length, ParmAlignment( ptipe ), &back.data.CurrProc->state );
        } else {
            parm_name = AllocTemp( back, type_class );
            parm_name->t.location = ParmMem( back, ptipe->length, ParmAlignment( ptipe ), &back.data.CurrProc->state );
        }
        parm_name->t.temp_flags |= STACK_PARM;
//        parm_name->n.size = tipe->length;
        parm_name->n.size = ptipe->length;
        parm_name->v.usage |= NEEDS_MEMORY | HAS_MEMORY | USE_MEMORY;
    } else {
        parm_name = AllocRegName( back, ActualParmReg( reg ) );
#if _TARGET & _TARG_80386
        if( back.data.CurrProc->state.attr & ROUTINE_STACK_RESERVE ) {
            /*
             * Just make sure we are taking up some space
             */
            ParmMem( back, ptipe->length, ParmAlignment( ptipe ), &back.data.CurrProc->state );
        }
#endif
    }
    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
        if( sym != NULL ) {
            DbgParmLoc( back, parm_name, sym );
        }
    }
    parm_def = DoParmDef( back, parm_name, type_class );
    if( type_class != XX ) {
        ins = MakeConvert( back, parm_name, temp, TypeClass( back, tipe ), type_class );
        LinkParmIns( parm_def, ins );
        AddIns( back, ins );
        TRDeclareParm( back, ins );
        if( temp->n.class_ == N_TEMP && parm_name->n.class_ == N_TEMP ) {
            temp->t.location = parm_name->t.location;
        }
    }
    back.data.MaxStack = 0;
    return( temp );
}


void    BGParmDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe )
/***********************************************************/
{
    hw_reg_set          reg;
    const type_def      *t;

    t = QParmType( back, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ), sym, tipe );
    reg = ParmReg( back, TypeClass( back, t ), t->length, ParmAlignment( tipe ), &back.data.CurrProc->state );
    DoParmDecl( back, sym, tipe, reg );
}


static  void    LinkParms( BackRun& back, instruction *call_ins, pn *owner )
/**************************************************************
 * call TRAddParm for each parm in order and delete the parm nodes
 */
{
    pn          next;
    pn          parm;

    for( parm = *owner; parm != NULL; parm = next ) {
        next = parm->next;
        /*
         * because of FPPushParms and delayed stuff
         */
//        if( parm->ins == NULL ) _Zoiks( ZOIKS_XXX );
        TRAddParm( call_ins, parm->ins );
        CGFree( back, parm );
    }
    *owner = NULL;
}

void    AddCallIns( BackRun& back, instruction *ins, cn call )
/*********************************************************
 * stick the call instruction into the current basic block
 */
{
    name                *call_name;
    fe_attr             attr;
    type_class_def      addr_type_class;
    name                *temp;
    instruction         *new_ins;

    PreCall( call );
    if( ins->head.opcode == OP_CALL ) {
        call_name = call->name->u.n.name;
        attr = static_cast<fe_attr>(0);
        if( call_name->m.memory_type == CG_FE ) {
            attr = FEAttr( *back.front, static_cast<cg_sym_handle>(call_name->v.symbol) );
        }
        /*
         * don't do this for far16 functions since they are handled
         * in a weird manner by Far16Parms and will not call data labels
         */
        if( (attr & FE_PROC) == 0 && (ins->flags.u.call_flags & CALL_FAR16) == 0 ) {
            /*
             * indirect since calling data labels directly
             * screws up the back end
             */
            addr_type_class = WD;
            if( static_cast<call_class_target>(reinterpret_cast<pointer_uint>(FindAuxInfo( back, call_name, FEINF_CALL_CLASS_TARGET ))) & FECALL_X86_FAR_CALL ) {
                addr_type_class = CP;
            }
            temp = AllocTemp( back, addr_type_class );
            new_ins = MakeUnary( back, OP_LA, call_name, temp, addr_type_class );
            AddIns( back, new_ins );
            call_name = temp;
            ins->head.opcode = OP_CALL_INDIRECT;
            ins->num_operands = 3;
        }
        ins->operands[CALL_OP_ADDR] = call_name;
    } else {
        ins->operands[CALL_OP_ADDR] = GenIns( back, call->name );
    }
    AddIns( back, ins );
    TNZapParms(back);
    PostCall( call );
}


void    ReverseParmNodeList( pn *owner )
/***************************************
 * reverse a linked list of parm_nodes.
 */
{
    pn  parm;
    pn  next;

    parm = *owner;
    *owner = NULL;
    for( ; parm != NULL; parm = next ) {
        next = parm->next;
        parm->next = *owner;
        *owner = parm;
    }
}


void            PushParms( BackRun& back, pn parm, call_state *state )
/**************************************************************
 * Run through the list of parameters, generating pushes
 * for the ones that are being passed on the stack. Unhook them
 * from the list, but leave the others (register parms) alone.
 * Add pointers to push instructions to the call instruction.
 */
{
    instruction         *ins;
    instruction         *push_ins;
    an                  addr;

    for( ; parm != NULL; parm = parm->next ) {
        if( parm->ins == NULL ) {
            if( HW_CEqual( parm->regs, HW_EMPTY ) ) {
                addr = parm->name;
                if( addr->format != NF_INS ) {
                    Zoiks( back, ZOIKS_043 );
                }
                ins = addr->u.i.ins;
                PushInSameBlock( back, ins );
                if( ins->head.opcode == OP_MOV && !IsVolatile( ins->operands[0] ) && (addr->flags & FL_VOLATILE) == 0 ) {
                    push_ins = PushOneParm( back, ins, ins->operands[0], ins->type_class, parm->offset, state );
//                    ins->result = ins->operands[0]; /* was this useful? */
                    FreeIns( back, ins );
                } else {
                    ins->result = BGNewTemp( back, addr->tipe );
                    FPNotStack( ins->result );
                    if( addr->flags & FL_ADDR_CROSSED_BLOCKS ) {
                        ins->result->v.usage |= USE_IN_ANOTHER_BLOCK;
                    }
                    push_ins = PushOneParm( back, ins, ins->result, ins->result->n.type_class, parm->offset, state );
                }
                addr->format = NF_ADDR; /* so instruction doesn't get freed! */
                BGDone( back, addr );
                parm->ins = push_ins;
            }
        }
    }
}

#if _TARGET & _TARG_80386
void    ReserveStack( BackRun& back, call_state *state, instruction *prev, type_length len )
/****************************************************************************
 * grab len bytes off the stack - doesn't matter what goes in there
 * as long as the space is allocated.  Guaranteed that len is a multiple
 * of 4 bytes in size.
 */
{
    name        *reg;
    instruction *ins;

    back.data.CurrProc->targ.never_sp_frame = true;
    reg = AllocRegName( back, HW_xAX );
    switch( len ) {
    case 8:
        ins = MakeUnary( back, OP_PUSH, reg, NULL, WD );
        SuffixIns( back, prev, ins );
        /* fall through */
    case 4:
        ins = MakeUnary( back, OP_PUSH, reg, NULL, WD );
        SuffixIns( back, prev, ins );
        break;
    default:
        Zoiks( back, ZOIKS_133 );
    }
    state->parm.offset += len;
    if( state->parm.offset > back.data.MaxStack ) {
        back.data.MaxStack = state->parm.offset;
    }
}
#endif

void    ParmIns( BackRun& back, pn parm, call_state *state )
/******************************************************
 * generate the move instructions for parameters that are passed in registers.
 * This should be all that's left on the list by now. The rest have all
 * been pushed on the stack.
 */
{
    name                *reg;
    name                *curr;
    instruction         *ins;
    an                  addr;

    for( ; parm != NULL; parm = parm->next ) {
        if( parm->ins == NULL ) {
            addr = parm->name;
            if( addr->format != NF_INS ) {
                Zoiks( back, ZOIKS_043 );
            }
            reg = AllocRegName( back, ActualParmReg( parm->regs ) );
            ins = addr->u.i.ins;
            ins->result = BGNewTemp( back, addr->tipe );
            if( addr->flags & FL_ADDR_CROSSED_BLOCKS ) {
                ins->result->v.usage |= USE_IN_ANOTHER_BLOCK;
            }
            curr = ins->result;
            if( addr->tipe->length == reg->n.size ) {
                ins = MakeMove( back, curr, reg, TypeClass( back, addr->tipe ) );
                AddIns( back, ins );
            } else if( !CvtOk( back, TypeClass( back, addr->tipe ), reg->n.type_class ) ) {
                ins = NULL;
                FEMessage( *back.front, FEMSG_BAD_PARM_REGISTER, reinterpret_cast<pointer>(static_cast<pointer_uint>(parm->num)) );
            } else if( HW_CEqual( reg->r.reg, HW_ABCD ) ) {
                ins = NULL;
                FEMessage( *back.front, FEMSG_BAD_PARM_REGISTER, reinterpret_cast<pointer>(static_cast<pointer_uint>(parm->num)) );
            } else {
                ins = MakeConvert( back, curr, reg, reg->n.type_class, TypeClass( back, addr->tipe ) );
                AddIns( back, ins );
            }
#if _TARGET & _TARG_80386
            if( state->attr & ROUTINE_STACK_RESERVE ) {
                /*
                 * this is for the stupid OS/2 _Optlink calling convention
                 */
                ReserveStack( back, state, ins, addr->tipe->length );
            }
#else
            /* unused parameters */ (void)state;
#endif
            addr->format = NF_ADDR; /* so instruction doesn't get freed! */
            BGDone( back, addr );
            parm->ins = ins;
        }
    }
}


void    BGZapBase( BackRun& back, name *base, const type_def *tipe )
/********************************************************
 * for FORTRAN. Generate a NOP with result of base[temp], so that we
 * know that a pass by reference argument can be modified by the
 * call. The NOP instruction right after the call.
 */
{
    instruction *ins;

    if( base == NULL )
        return;
    if( IsntModel( back, CGSW_GEN_FORTRAN_ALIASING ) )
        return;
    if( (tipe->attr & TYPE_POINTER) == 0 )
        return;
    ins = MakeNop(back);
    if( back.data.DummyIndex == NULL )
        back.data.DummyIndex = AllocTemp( back, WD );
    ins->result = ScaleIndex( back, back.data.DummyIndex, base, 0, XX, tipe->length, SCALE_NONE, X_FAKE_BASE );
    ins->flags.u.nop_flags |= NOP_ZAP_INFO;
    AddIns( back, ins );
}


void    BGReturn( BackRun& back, an retval, const type_def *tipe )
/******************************************************
 * generate the instructions to return the value 'retval', then
 * go off and call generate to optimize the whole routine and
 * spew it out into the object file.
 */
{
    instruction         *ins;
    instruction         *last_ins;
    instruction         *ret_ins;
    name                *name;
    type_class_def      tipe_type_class;
    type_class_def      type_class;

    ins = MakeNop(back);
    last_ins = NULL;
    if( retval != NULL ) {
        tipe_type_class = TypeClass( back, tipe );
        type_class = ReturnTypeClass( back, tipe, back.data.CurrProc->state.attr );
        UpdateReturn( back, &back.data.CurrProc->state, tipe, type_class, static_cast<aux_handle>(FEAuxInfo( *back.front, AskForLblSym(back, static_cast<label_handle>(back.data.CurrProc->label)), FEINF_AUX_LOOKUP )) );
        if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) { // d1+ or d2
            DbgRetLoc(back);
        }
        if( type_class == XX ) {
            name = StReturn( back, retval, tipe, &last_ins );
            AddIns( back, MakeMove( back, GenIns( back, retval ), name, tipe_type_class ) );
        } else {
            if( HW_CEqual( back.data.CurrProc->state.return_reg, HW_EMPTY ) ) {
                name = StReturn( back, retval, tipe, &last_ins );
            } else {
                name = AllocRegName( back, back.data.CurrProc->state.return_reg );
            }
                if( tipe->length == name->n.size ) {
                    ret_ins = MakeMove( back, GenIns( back, retval ), name, name->n.type_class );
                } else {
                    ret_ins = MakeConvert( back, GenIns( back, retval ), name, name->n.type_class, tipe_type_class );
                }
                /*
                 * BBB - we can get a situation where we are returning
                 * a float in eax (when compiling -3s) and we don't want
                 * to do a convert - ack.
                 */
//                ret_ins = MakeConvert( GenIns( retval ), name, name->n.type_class, class );
            AddIns( back, ret_ins );
        }
        BGDone( back, retval );
    } else {
        ins->zap = &AllocRegName( back, back.data.CurrProc->state.return_reg )->r;
        ins->flags.u.nop_flags |= NOP_ZAP_INFO;
    }
    AddIns( back, ins );
    if( last_ins != NULL )
        AddIns( back, last_ins );
    GenBlock( back, BLK_RETURN, 0 );
    if( back.data.AddrList != NULL ) {
        Zoiks( back, ZOIKS_003 );
    }
    Generate( back, true );
    back.model.TargetModel = back.model.SaveTargetModel;
}


bool        AssgnParms( BackRun& back, cn call, bool aux_inline )
/********************************************************
 * Decide what registers the parms should go in.
 * Assign registers to first parm first, etc. Also, assign a congolmeration
 * of all the parameter registers to one of the operands of the call
 * so that the dataflow knows that the registers are live between the
 * move into them and the call.
 */
{
    pn                  parm;
    call_state          *state;
    bool                push_no_pop;
    int                 parms;
    instruction         *call_ins;
    type_class_def      parm_type_class;
    type_class_def      reg_type_class;


    push_no_pop = false;
    parm = call->parms;
    state = call->state;
    call_ins = call->ins;
    parms = 0;
    for( ; parm != NULL; parm = parm->next ) {
        if( aux_inline ) {
            parm->regs = ParmInLineReg( &state->parm );
            if( HW_CEqual( parm->regs, HW_EMPTY ) ) {
                if( !HW_CEqual( *(state->parm.curr_entry), HW_EMPTY ) ) {
                    FEMessage( *back.front, FEMSG_ERROR, const_cast<pointer>(static_cast<const void *>("More parameters than registers in pragma")) );
                } else {
                    parm->offset = ParmMem( back, parm->name->tipe->length, ParmAlignment( parm->name->tipe ), state );
                    push_no_pop = true;
                }
            } else {
                parm_type_class = TypeClass( back, parm->name->tipe );
                reg_type_class  = AllocRegName( back, parm->regs )->n.type_class;
                if( parm_type_class != FD || reg_type_class != U8 ) {
                    if( !CvtOk( back, parm_type_class, reg_type_class ) ) {
                        FEMessage( *back.front, FEMSG_BAD_PARM_REGISTER, reinterpret_cast<pointer>(static_cast<pointer_uint>( parms + 1 )) );
                    }
                }
            }
        } else {
            parm->alignment = 1;
            parm->regs = ParmReg( back, TypeClass( back, parm->name->tipe ),
                                   parm->name->tipe->length,
                                   parm->alignment,
                                   state );
            if( HW_CEqual( parm->regs, HW_EMPTY ) ) {
                parm->offset = ParmMem( back, parm->name->tipe->length, parm->alignment, state );
                push_no_pop = true;
            }
        }
        ++parms;
        parm->num = parms;
        parm->ins = NULL;
    }
    ReverseParmNodeList( &call->parms );
    PushParms( back, call->parms, state );
    FPPushParms( back, call->parms, state );
    ParmIns( back, call->parms, state );
    ReverseParmNodeList( &call->parms );
    LinkParms( back, call_ins, &call->parms );
    if( call_ins->head.opcode == OP_CALL ) {
        /*
         * special case for call so we ignore address in dataflow
         */
        call_ins->num_operands = 2;
        call_ins->operands[CALL_OP_USED] = AllocRegName( back, state->parm.used );
    } else {
        call_ins->operands[CALL_OP_USED] = AllocRegName( back, state->parm.used );
    }
    call_ins->operands[CALL_OP_POPS] = AllocS32Const( back, state->parm.offset );
    call_ins->zap = &AllocRegName( back, CallZap( back, state ) )->r;
    return( push_no_pop );
}
