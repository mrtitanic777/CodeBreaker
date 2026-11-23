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
* Description:  Optimize x87 FPU instruction sequences.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "makeins.h"
#include "data.h"
#include "namelist.h"
#include "redefby.h"
#include "makeblk.h"
#include "fpu.h"
#include "x87.h"
#include "expand.h"
#include "split.h"
#include "insutil.h"
#include "rgtbl.h"
#include "typemap.h"
#include "optab.h"
#include "inssegs.h"
#include "bldcall.h"
#include "fixindex.h"
#include "inssched.h"
#include "bldins.h"
#include "liveinfo.h"
#include "x86regn.h"
#include "backrun.hpp"


void    FPParms( BackRun& back )
/**************************
 * Find sequences like
 *          PARM_DEF         => ST(0),
 *          MOV        ST(0) => foobar
 * in HeadBlock and record where the parameters in the 8087 are going
 * to end up in Parm8087, so that the code to deal with the parameters
 * can be folded into the prolog sequence.
 */
{
    instruction *ins;
    instruction *next;
    int         i;

    if( back.data.CurrProc->label == back.data.HeadBlock->label ) {
        for( i = MAX_8087_REG + 1; i-- > 0; ) {
            back.i87data.Parm8087[i] = NULL;
        }
        i = 0;
        for( ins = back.data.HeadBlock->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( ins->head.opcode == OP_PARM_DEF ) {
                if( FPRegNum( ins->result ) == 0 ) {
                    next = ins->head.next;
                    if( next->head.opcode == OP_BLOCK
                     || reinterpret_cast<instruction *>(ins->operands[2]) != next /* not associated*/
                     || reinterpret_cast<instruction *>(next->operands[2]) != ins ) {
                        back.i87data.Parm8087[i] = reinterpret_cast<name *>(back.data.CurrProc); /* klude - means parm ignored*/
                    } else {
                        back.i87data.Parm8087[i] = next->result;
                        DoNothing( back, next );
                        ins = next;
                    }
                    ++i;
                }
            }
        }
    }
}

static  bool    CanPushImmed( BackRun& back, pn parm, int *num_parms )
/******************************************************
 * find out if we can "push" floating point parameters onto the 8087
 * stack immediately as they are calculated, or if we must delay
 * pushing until just before the call instruction.
 */
{
    instruction *next;
    instruction *ins;
    an          addr;

    for( ; parm != NULL; parm = parm->next ) {
        if( parm->ins == NULL ) {
            addr = parm->name;
            if( addr->tipe->attr & TYPE_FLOAT ) {
                ++*num_parms;
                ins = addr->u.i.ins;
                for( next = ins->head.next; next->head.opcode != OP_BLOCK; next = next->head.next ) {
                    if( OpIsCall( next->head.opcode ) )
                        return( false );
                    if( OpIsIFunc( next->head.opcode ) ) {
                        if( FPStkReq( back, next ) != 0 ) {
                            return( false );
                        }
                    }
                }
                if( BlockOf( next ) != back.data.CurrBlock ) {
                    return( false );
                }
            }
        }
    }
    return( true );
}


static  void    Pushes( instruction *ins )
/****************************************/
{
    ins->stk_entry = 0;
    ins->stk_exit = 1;
}


static  int     FPPushImmed( BackRun& back, pn parm )
/*************************************
 * Push the list of floating point parameters onto the 8087 stack,
 * immediately following the definition of each parameter.  (Just set
 * the result of each instruction to ST(0)).
 */
{
    an          addr;
    int         parms;

    parms = 0;
    for( ; parm != NULL; parm = parm->next ) {
        if( parm->ins == NULL ) {
            addr = parm->name;
            if( (addr->tipe->attr & TYPE_FLOAT)
              && HW_COvlap( parm->regs, HW_FLTS ) ) {
                if( addr->format != NF_INS ) {
                    Zoiks( back, ZOIKS_043 );
                }
                addr->u.i.ins->result = AllocRegName( back, HW_ST0 );
                Pushes( addr->u.i.ins );
                parm->ins = addr->u.i.ins;
                addr->format = NF_ADDR; /* so instruction doesn't get freed! */
                BGDone( back, addr );
                ++parms;
            }
        }
    }
    return( parms );
}

static  instruction     *PushDelayed( BackRun& back, instruction *ins, an addr, call_state *state )
/***********************************************************************************
 * Put the parm "addr" into a temporary, and then add a "push" just
 * before the call instruction.
 */
{
    ins->result = BGNewTemp( back, addr->tipe );
    if( addr->flags & FL_ADDR_CROSSED_BLOCKS ) {
        ins->result->v.usage |= USE_IN_ANOTHER_BLOCK;
    }
    ins = MakeMove( back, ins->result, AllocRegName( back, HW_ST0 ), TypeClass( back, addr->tipe ) );
    Pushes( ins );
    AddIns( back, ins );
    addr->format = NF_ADDR; /* so instruction doesn't get freed! */
    BGDone( back, addr );
#if _TARGET & _TARG_8086
    /* unused parameters */ (void)state;
#else
    if( state->attr & ROUTINE_STACK_RESERVE ) {
        ReserveStack( back, state, ins, addr->tipe->length );
    }
#endif
    return( ins );
}


static bool PushDelayedIfStackOperand( BackRun& back, instruction *ins, pn parm, call_state *state )
/************************************************************************************
 * Delay the "push" of a floating point parameter if any of its
 * operands are 8087 stack operands.
 */
{
    opcnt       i;
    an          addr;

    addr = parm->name;
    ins = addr->u.i.ins;
    for( i = ins->num_operands; i-- > 0; ) {
        if( FPIsStack( ins->operands[i] ) ) {
            parm->ins = PushDelayed( back, ins, addr, state );
//            CGFree( parm );
            return( true );
        }
    }
    return( false );
}


static bool PushDelayedIfRedefinition( BackRun& back, instruction *ins, pn parm, call_state *state )
/************************************************************************************
 * Delay the "push" of a floating point parameter if any of its
 * operands are  redefined between the calculation of the parm and the
 * call.
 */
{
    instruction *next;
    opcnt       i;

    next = ins->head.next;
    for( ;; ) {
        for( ; next->head.opcode != OP_BLOCK; next = next->head.next ) {
            for( i = ins->num_operands; i-- > 0; ) {
                if( IsReDefinedBy( back, next, ins->operands[i] ) ) {
                    parm->ins = PushDelayed( back, ins, parm->name, state );
//                    CGFree( parm );
                    return( true );
                }
            }
        }
        if( BlockOf( next ) == back.data.CurrBlock )
            break;
        if( BlockOf( next )->next_block == NULL ) {
            next = back.data.CurrBlock->ins.head.next;
        } else {
            next = BlockOf( next )->next_block->ins.head.next;
        }
    }
    return( false );
}

static  void    UseInOther( name *op )
{
    if( op == NULL )
        return;
    switch( op->n.class_ ) {
    case N_TEMP:
    case N_MEMORY:
        op->v.usage |= USE_IN_ANOTHER_BLOCK;
        break;
    case N_INDEXED:
        UseInOther( op->i.index );
        UseInOther( op->i.base );
        break;
    default:
        break;
    }
}

static  int     FPPushDelay( BackRun& back, pn parm, call_state *state )
/********************************************************
 * For each parm, move it into a temp, and "push" the temp onto the
 * 8087 stack just before the call instruction.  If the calculation of
 * the parm can be moved down just in front of the call, we do that,
 * instead of using a temporary.
 */
{
    instruction *ins;
    instruction *new_ins;
    an          addr;
    int         parms;


    parms = 0;
    for( ; parm != NULL; parm = parm->next ) {
        if( parm->ins == NULL ) {
            addr = parm->name;
            if( (addr->tipe->attr & TYPE_FLOAT)
              && HW_COvlap( parm->regs, HW_FLTS ) ) {
                ++parms;
                if( addr->format != NF_INS ) {
                    Zoiks( back, ZOIKS_043 );
                }
                ins = addr->u.i.ins;
                if( PushDelayedIfStackOperand( back, ins, parm, state ) )
                    continue;
                if( PushDelayedIfRedefinition( back, ins, parm, state ) )
                    continue;
                /*
                 * we can push it just before the CALL
                 */
                if( addr->flags & FL_ADDR_CROSSED_BLOCKS ) {
                    UseInOther( ins->operands[0] );
                    if( ins->num_operands > 1 ) {
                        UseInOther( ins->operands[1] );
                    }
                }
                new_ins = MakeNary( back, ins->head.opcode, ins->operands[0],
                                    ins->operands[1], AllocRegName( back, HW_ST0 ),
                                    ins->type_class, ins->base_type_class,
                                    ins->num_operands );
                Pushes( new_ins );
                new_ins->num_operands = ins->num_operands;
                AddIns( back, new_ins );
#if _TARGET & _TARG_80386
                if( state->attr & ROUTINE_STACK_RESERVE ) {
                    ReserveStack( back, state, new_ins, addr->tipe->length );
                }
#endif
                BGDone( back, addr ); /* so ins DOES get freed*/
                parm->ins = new_ins;
            }
        }
    }
    return( parms );
}

void    FPPushParms( BackRun& back, pn parm, call_state *state )
/**********************************************************
 * "push" parameters onto the 8087 stack.  If parameters can be pushed
 * as soon as they are calculated, do that (FPPushImmed), otherwise we
 * have to "push" it onto the 8087 just before the call instruction
 * (FPPushDelay). For each parameter there is an address_node (an) of
 * type NF_INS, which not had the result field filled in yet.
 */
{
    int parms;

    if( FPULevel( back, FPU_87 ) ) {
        parms = 0;
        if( CanPushImmed( back, parm, &parms )
          && parms < back.i87data.Max87Stk
          && (state->attr & ROUTINE_STACK_RESERVE) == 0 ) {
            parms = FPPushImmed( back, parm );
        } else {
            if( parms != 0 ) {
                parms = FPPushDelay( back, parm, state );
            }
        }
        SetFPParmsUsed( state, parms );
    }
}

static const opcode_entry    FSINCOS[] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FSINCOS,      FU_TRIG )
};

inline opcode_defs OtherTrigOp( opcode_defs x ) { return ( (x) == OP_SIN ? OP_COS : OP_SIN ); }

static  bool    FSinCos( BackRun& back, instruction *ins1 )
/********************************************
 * find the FSINCOS sequence
 */
{
    instruction *ins2;
    instruction *ins3;
    instruction *ins4;

    if( IsntModel( back, CGSW_GEN_I_MATH_INLINE ) ) {
        return( false );
    }
    ins2 = ins1->head.next;
    ins3 = ins2->head.next;
    ins4 = ins3->head.next;
    if( InsGenerate( ins1 ) == G_MFLD ) {
        if( ins2->head.opcode != OP_SIN && ins2->head.opcode != OP_COS ) {
            return( false );
        }
        if( InsGenerate( ins3 ) != G_MFLD ) {
            return( false );
        }
        if( ins1->operands[0] != ins3->operands[0] ) {
            return( false );
        }
        if( ins4->head.opcode != OtherTrigOp( ins2->head.opcode ) ) {
            return( false );
        }
    } else if( InsGenerate( ins1 ) == G_RFLD ) {
        if( ins2->head.opcode != OP_SIN && ins2->head.opcode != OP_COS ) {
            return( false );
        }
        if( InsGenerate( ins3 ) != G_RFLD ) {
            return( false );
        }
        if( FPRegNum( ins1->operands[0] ) != FPRegNum( ins3->operands[0] ) - 1 ) {
            return( false );
        }
        if( ins4->head.opcode != OtherTrigOp( ins2->head.opcode ) ) {
            return( false );
        }
    } else if( ins1->head.opcode == OP_SIN ) {
        if( InsGenerate( ins2 ) != G_RFLD ) {
            return( false );
        }
        if( FPRegNum( ins2->operands[0] ) != 1 ) {
            return( false );
        }
        if( ins3->head.opcode != OtherTrigOp( ins1->head.opcode ) ) {
            return( false );
        }
        ins4 = ins3;
        ins3 = ins2;
        ins2 = ins1;
    } else {
        return( false );
    }
    ins2->u.gen_table = FSINCOS;
    if( ins2->head.opcode != OP_SIN ) {
        PrefFXCH( back, ins3, 1 );
    }
    FreeIns( back, ins3 );
    FreeIns( back, ins4 );
    return( true );
}

static  instruction     *Next87Ins( instruction *ins )
/****************************************************/
{
    instruction *next;

    for( next = ins->head.next; next->head.opcode != OP_BLOCK; next = next->head.next ) {
        if( DoesSomething( next ) ) {
            return( next );
        }
    }
    return( ins );
}

static instruction *BackUpAndFree( BackRun& back, instruction *ins, instruction *junk1,
                                        instruction *junk2 )
/**********************************************************/
{
    instruction *ret;

    ret = ins->head.prev;
    if( junk1 != NULL )
        FreeIns( back, junk1 );
    if( junk2 != NULL )
        FreeIns( back, junk2 );
    if( ret->head.opcode == OP_BLOCK )
        ret = ret->head.next;
    return( ret );
}

static  void    MoveThrough( BackRun& back, name *from, name *to, instruction *from_ins,
                             instruction *to_ins, name *reg,
                             type_class_def type_class )
/****************************************************
 * Move from "from" to "to" using register name "reg". Segments if
 * any should be taken from "from_ins" and "to_ins".
 */
{
    bool        dummy;
    instruction *new_ins;

    new_ins = MakeMove( back, from, reg, type_class );
    new_ins->u.gen_table = FindGenEntry( back, new_ins, &dummy );
    DupSeg( from_ins, new_ins );
    PrefixIns( back, to_ins, new_ins );
    new_ins = MakeMove( back, reg, to, type_class );
    DupSeg( to_ins, new_ins );
    new_ins->u.gen_table = FindGenEntry( back, new_ins, &dummy );
    PrefixIns( back, to_ins, new_ins );
}

static  instruction    *To86Move( BackRun& back, instruction *ins, instruction *next )
/**********************************************************************
 * Turn a move which uses the 8087 (FLD X, FSTP Y) into a move using
 * the 8086 using an available 8086 register.
 */
{
    const hw_reg_set    *regs;
    name                *reg;
    instruction         *ret;

    ret = ins->head.next;
    for( regs = RegSetList(back, RL_MOVE); ; ++regs ) {
        if( HW_CEqual( *regs, HW_EMPTY ) )
            return( ret );
        if( HwOvlap( *regs, next->head.live.regs ) )
            continue;
        if( HwOvlap( *regs, ins->head.live.regs ) )
            continue;
        reg = AllocRegName( back, *regs );
        break;
    }
    if( next->result->n.type_class == FS ) {
#if _TARGET & _TARG_8086
        if( back.data.OptForSize > 50 )
            return( ret );
        MoveThrough( back, LowPart( back, ins->operands[0], U2 ),
                     LowPart( back, next->result, U2 ), ins, next, reg, U2 );
        MoveThrough( back, HighPart( back, ins->operands[0], U2 ),
                     HighPart( back, next->result, U2 ), ins, next, reg, U2 );
#else
        MoveThrough( back, ins->operands[0], next->result, ins, next, reg, U4 );
#endif
    } else {
        if( back.data.OptForSize > 50 )
            return( ret );
#if _TARGET & _TARG_8086
        MoveThrough( back, LowPart( back, LowPart( back, ins->operands[0], U4 ), U2 ),
                     LowPart( back, LowPart( back, next->result      , U4 ), U2 ),
                     ins, next, reg, U2 );
        MoveThrough( back, HighPart( back, LowPart( back, ins->operands[0], U4 ), U2 ),
                     HighPart( back, LowPart( back, next->result      , U4 ), U2 ),
                     ins, next, reg, U2 );
        MoveThrough( back, LowPart( back, HighPart( back, ins->operands[0], U4 ), U2 ),
                     LowPart( back, HighPart( back, next->result      , U4 ), U2 ),
                     ins, next, reg, U2 );
        MoveThrough( back, HighPart( back, HighPart( back, ins->operands[0], U4 ), U2 ),
                     HighPart( back, HighPart( back, next->result      , U4 ), U2 ),
                     ins, next, reg, U2 );
#else
        MoveThrough( back, LowPart( back, ins->operands[0], U4 ),
                     LowPart( back, next->result, U4 ), ins, next, reg, U4 );
        MoveThrough( back, HighPart( back, ins->operands[0], U4 ),
                     HighPart( back, next->result, U4 ), ins, next, reg, U4 );
#endif
    }
    return( BackUpAndFree( back, ins, ins, next ) );
}


static  bool    RedundantStore( BackRun& back, instruction *ins )
/************************************************/
{
    instruction         *next;

    for( next = ins->head.next; next->head.opcode != OP_BLOCK; next = next->head.next ) {
        if( next->result == ins->result ) {
            if( InsGenerate( next ) == G_MFSTNP || InsGenerate( next ) == G_MFST ) {
                return( true );
            }
        }
        if( InsOrderDependant( back, ins, next ) ) {
            return( false );
        }
    }
    return( false );
}


static  instruction    *Opt87Sequence( BackRun& back, instruction *ins, bool *again )
/*********************************************************************
 * Look for silly 8087 sequences and fix them up.
 */
{
    instruction         *next;
    instruction         *third;
    instruction         *ret;
    type_class_def      type_class;
    int                 st_reg;

    next = Next87Ins( ins );
    ret = ins->head.next;
    if( next == ins )
        return( ret );
    if( InsGenerate( ins ) == G_RFLD && FPRegNum( ins->operands[0] ) == 0 ) {
        if( InsGenerate( next ) == G_MFST ) {
            /*
             * FLD ST, FSTP X ===> FST X
             */
            FreeIns( back, ins );
            if( FPResultNotNeeded( next ) ) {
                ret = BackUpAndFree( back, ins, next, NULL );
            } else {
                ret = next;
                NoPop( next );
            }
            *again = true;
        } else if( InsGenerate( next ) == G_RFST ) {
            /*
             * FLD ST, FSTP ST(i) ===> FST ST(i-1)
             */
            st_reg = FPRegNum( next->result );
            if( st_reg == 0 ) {
                ret = BackUpAndFree( back, ins, ins, next );
            } else {
                next->result = ST( back, st_reg - 1 );
                FreeIns( back, ins );
                ret = next;
                NoPop( next );
            }
            *again = true;
        } else if( InsGenerate( next ) == G_RNFBINP || InsGenerate( next ) == G_RRFBINP ) {
            /*
             * FLD ST, FopP ST(i),ST ==> Fop ST(i-1),ST
             */
            st_reg = FPRegNum( next->operands[0] );
            if( st_reg == 0 ) {
                ret = BackUpAndFree( back, ins, ins, next );
            } else {
                next->operands[0] = ST( back, st_reg - 1 );
                NoPopRBin( next );
                FreeIns( back, ins );
                ret = next;
            }
            *again = true;
        } else if( InsGenerate( next ) == G_FXCH && FPRegNum( next->result ) == 1 ) {
            FreeIns( back, next );
            *again = true;
            ret = ins;
        }
    } else if( InsGenerate( ins ) == G_RFLD || InsGenerate( ins ) == G_MFLD ) {
        if( ( InsGenerate( next ) == G_RRFBINP || InsGenerate( next ) == G_RNFBINP ) ) {
            if( InsGenerate( ins ) == G_MFLD ) {
                type_class = ins->operands[0]->n.type_class;
                if( !IsFloating( type_class ) ) {
                    return( ret ); /* need convert! */
                }
            }
            if( FPRegNum( next->operands[0] ) == 1 ) {
                /*
                 * FLD X, FxxP ST(1) ==> Fxx X
                 */
                next->result = ST( back, 0 );
                next->operands[0] = ins->operands[0];
                if( ins->num_operands > OpcodeNumOperands( ins )
                  && next->num_operands <= OpcodeNumOperands( next ) ) {
                    next->operands[next->num_operands] = ins->operands[OpcodeNumOperands( ins )];
                    next->num_operands++;
                }
                FreeIns( back, ins );
                NoPopBin( next );
                *again = true;
                ret = next;
            }
        } else if( InsGenerate( next ) == G_RFLD || InsGenerate( next ) == G_MFLD ) {
            if( InsGenerate( ins ) == G_MFLD
              && InsGenerate( next ) == G_MFLD
              && ins->operands[0] == next->operands[0] ) {
                /*
                 * FLD X, FLD X ==> FLD X, FLD ST(0)
                 */
                next->operands[0] = ST( back, 0 );
                ToRFld( next );
                *again = true;
                ret = next;
            } else {
                /*
                 * FLD X, FLD Y, FXCH ST(1) ==> FLD Y, FLD X
                 */
                third = Next87Ins( next );
                if( third == next )
                    return( ret );
                if( InsGenerate( third ) == G_FXCH && FPRegNum( third->result ) == 1 ) {
                    FreeIns( back, third );
                    if( FPRegNum( next->operands[0] ) != 0 ) {
                        ins->head.next->head.prev = ins->head.prev;
                        ins->head.prev->head.next = ins->head.next;
                        SuffixIns( back, next, ins );
                        if( InsGenerate( ins ) == G_RFLD ) {
                            st_reg = FPRegNum( ins->operands[0] );
                            ins->operands[0] = ST( back, st_reg + 1 );
                        }
                        if( InsGenerate( next ) == G_RFLD ) {
                            st_reg = FPRegNum( next->operands[0] );
                            next->operands[0] = ST( back, st_reg - 1 );
                        }
                        ret = next;
                    } else {
                        ret = ins;
                    }
                    *again = true;
                }
            }
        } else if( InsGenerate( ins ) == G_MFLD ) {
            if( InsGenerate( next ) == G_MFST ) {
                if( ins->operands[0]->n.type_class == next->result->n.type_class ) {
                    ret = To86Move( back, ins, next );
                }
            } else if( InsGenerate( next ) == G_MRFBIN || InsGenerate( next ) == G_MNFBIN ) {
                if( ins->operands[0] == next->operands[0] ) {
                    /*
                     * FLD X, FOP X ==> FLD X, FOP ST
                     */
                    next->operands[0] = ST( back, 0 );
                    DelSeg( next );
                    NoMemBin( next );
                    *again = true;
                    ret = ins;
                }
            }
        }
    } else if( InsGenerate( ins ) == G_MFST || InsGenerate( ins ) == G_RFST ) {
        if( InsGenerate( next ) == G_MFLD
          && ( next->operands[0] == ins->result )
          && IsFloating( ins->result->n.type_class )
          && !IsVolatile( ins->result ) ) {
            /*
             * FSTP X, FLD X ==> FST X
             */
            FreeIns( back, next );
            if( FPResultNotNeeded( ins ) ) {
                ret = BackUpAndFree( back, ins, ins, NULL );
            } else {
                ret = ins;
                NoPop( ins );
            }
            *again = true;
        } else if( InsGenerate( next ) == G_RFLD
          && FPRegNum( next->operands[0] ) + 1 == FPRegNum( ins->result ) ) {
            /*
             * FSTP ST(i), FLD ST(i-1) ==> FST ST(i)
             */
            FreeIns( back, next );
            NoPop( ins );
            *again = true;
            ret = ins;
        }
    } else if( InsGenerate( ins ) == G_FXCH ) {
        if( InsGenerate( next ) == G_FXCH ) {
            /*
             * FXCH ST(i) FXCH ST(i) => nothing
             */
            if( FPRegNum( ins->result ) == FPRegNum( next->result ) ) {
                FreeIns( back, next );
                next = BackUpAndFree( back, ins, ins, NULL );
                *again = true;
                ret = next;
            }
        } else {
            if( InsGenerate( next ) == G_RRFBINP || InsGenerate( next ) == G_RNFBINP ) {
                if( FPRegNum( ins->result ) == FPRegNum( next->operands[0] ) ) {
                    /*
                     * FXCH ST(i), FopP ST(i),ST -> FopRP ST(i),ST
                     */
                    FreeIns( back, ins );
                    ReverseFPGen( next );
                    *again = true;
                    ret = next;
                }
            }
        }
    } else if( InsGenerate( ins ) == G_MFSTNP ) {
        if( InsGenerate( next ) == G_MFLD && ins->result == next->operands[0] ) {
            /*
             * FST X, FLD X => FST X, FLD ST
             */
            next->operands[0] = ST( back, 0 );
            ToRFld( next );
            *again = true;
            ret = ins;
        } else if( RedundantStore( back, ins ) ) {
            ret = BackUpAndFree( back, ins, ins, NULL );
            *again = true;
        }
    } else if( InsGenerate( ins ) == G_RNFBIN || InsGenerate( ins ) == G_RRFBIN ) {
        if( InsGenerate( next ) == G_RFST && next->result == ins->operands[0] ) {
            ToPopBin( ins );
            FreeIns( back, next );
            *again = true;
            ret = ins;
        }
    }
    return( ret );
}

static void    Opt8087( BackRun& back )
/*****************************
 * Look for silly 8087 sequences and change them into better ones.
 */
{
    block       *blk;
    instruction *ins;
    int         i;
    bool        again;
    instruction *next;

    for( blk = back.data.HeadBlock; blk != NULL; ) {
        i = 0;
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            ins->sequence = ++i;
        }
        again = false;
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
            next = ins->head.next;
            if( GenIs8087( InsGenerate( ins ) ) ) {
                if( !FSinCos( back, ins ) ) {
                    next = Opt87Sequence( back, ins, &again );
                }
            }
        }
        if( !again ) {
            for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
                if( FPResultNotNeeded( ins ) ) {
                    ins->result = ST( back, 0 );
                    ToRFstp( ins );
                }
            }
            blk = blk->next_block;
        }
    }
}


void    FPOptimize( BackRun& back )
/*****************************
 * Fix up the 8087 instructions.  The instructions so far
 */
{
    if( FPULevel( back, FPU_87 ) ) {
        if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
            Opt8087(back);
        }
        Wait8087(back);
    }
}

bool    FPDivIsADog( BackRun& back, type_class_def type_class )
/**********************************************/
{

    return( FPULevel( back, FPU_87 ) && IsFloating( type_class ) );
}

