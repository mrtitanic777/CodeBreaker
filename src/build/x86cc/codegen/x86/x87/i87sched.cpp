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
* Description:  Instruction scheduling for x87 FPU.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "cgmem.h"
#include "i87sched.h"
#include "data.h"
#include "edge.h"
#include "redefby.h"
#include "cgsrtlst.h"
#include "indvars.h"
#include "loopopts.h"
#include "fpu.h"
#include "x87.h"
#include "namelist.h"
#include "optab.h"
#include "fixindex.h"
#include "revcond.h"
#include "x86regn.h"
#include "backrun.hpp"


static const opcode_entry    RFST[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RFST,         FU_NO )
};
static const opcode_entry    RFSTNP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RFSTNP,       FU_NO )
};
static const opcode_entry    RFLD[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RFLD,         FU_NO )
};
static const opcode_entry    RCOMP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RCOMP,        FU_NO )
};
static const opcode_entry    FCOMPP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_FCOMPP,       FU_NO )
};
static const opcode_entry    RRFBIN[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBIN,       FU_NO )
};
static const opcode_entry    RNFBIN[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBIN,       FU_NO )
};
static const opcode_entry    RRFBINP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RRFBINP,      FU_NO )
};
static const opcode_entry    RNFBINP[1] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE(                         PRESERVE, V_NO,           RG_,          G_RNFBINP,      FU_NO )
};

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct I87schedFileState: i87sched_state.h) */

}  // namespace


/*
 * global variables
 */

/*
 * this must stick!
 */
#define FP_INS_INTRODUCED       INS_VISITED

static  int     InsMaxDepth( instruction *ins )
/*********************************************/
{
    if( ins->stk_entry > ins->stk_exit )
        return( ins->stk_entry );
    return( ins->stk_exit );
}

static  fp_attr FPAttr( BackRun& back, instruction *ins )
/****************************************/
{
    if( _OpIsCall( ins->head.opcode ) && ins->result != NULL ) {
        if( ins->result->n.class_ != N_REGISTER )
            return( POPS_ALL );
        if( !HW_COvlap( ins->result->r.reg, HW_FLTS ) )
            return( POPS_ALL );
        return static_cast<fp_attr>(( PUSHES + POPS_ALL ));
    }
    if( G( ins ) == G_FCHOP )
        return( NEEDS_ST0 );
    if( !_GenIs8087( G( ins ) ) ) {
        return( NEEDS_NOTHING );
    }
    switch( G( ins ) ) {
    case G_RRFBINP:
    case G_RNFBINP:
        return static_cast<fp_attr>(( NEEDS_ST0+POPS+SETS_ST1 ));
    case G_MFLD:
    case G_FLDZ:
    case G_FLD1:
    case G_RFLD:
        return( PUSHES );
    case G_FRNDINT:
    case G_FCHS:
    case G_MFSTNP:
    case G_RFSTNP:
    case G_FTST:
    case G_FMATH:
    case G_RRFBIN:
    case G_RNFBIN:
    case G_MRFBIN:
    case G_MNFBIN:
        return( NEEDS_ST0 );
    case G_FCOMPP:
        return static_cast<fp_attr>(( NEEDS_ST0_ST1+POPS2 ));
    case G_MFST:
    case G_MFSTRND:
    case G_RFST:
    case G_MCOMP:
    case G_RCOMP:
        return static_cast<fp_attr>(( NEEDS_ST0+POPS ));
    case G_IFUNC:
        if( OpcodeNumOperands( ins ) == 2 ) {
            return static_cast<fp_attr>(( NEEDS_ST0_ST1+POPS ));
        } else {
            return( NEEDS_ST0 );
        }
    case G_FXCH:
        return static_cast<fp_attr>(( NEEDS_ST0+EXCHANGES ));
    case G_FSINCOS:
    default:
        Zoiks( back, ZOIKS_075 );
        return( NEEDS_NOTHING );
    }
}


static const opcode_entry    *RegAction( instruction *ins )
/*********************************************************/
{
    switch( G( ins ) ) {
    case G_MFLD:
        return( RFLD );
    case G_MCOMP:
        return( RCOMP );
    case G_MRFBIN:
        return( RRFBIN );
    case G_MNFBIN:
        return( RNFBIN );
    default:
        return( NULL );
    }
}


static  void    PopVirtualStack( BackRun& back, instruction *ins )
/*************************************************/
{
    virtual_st_locn     virtual_locn;

    for( virtual_locn = VIRTUAL_0; virtual_locn < VIRTUAL_NONE - 1; virtual_locn++ ) {
        InsSTLoc( back, ins, virtual_locn ) = InsSTLoc( back, ins, virtual_locn + 1 );
    }
    InsSTLoc( back, ins, VIRTUAL_7 ) = ACTUAL_NONE;
}


static  actual_st_locn  *ActualStackOwner( BackRun& back, actual_st_locn actual_locn )
/*********************************************************************/
{
    int                 i;
    virtual_st_locn     virtual_locn;
    temp_entry          *temp;

    for( i = 0; i < back.i87sched.MaxSeq; ++i ) {
        for( virtual_locn = VIRTUAL_0; virtual_locn < VIRTUAL_NONE; virtual_locn++ ) {
            if( RegSTLoc( back, i, virtual_locn ) == actual_locn ) {
                return( &RegSTLoc( back, i, virtual_locn ) );
            }
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->actual_locn == actual_locn ) {
            return( &temp->actual_locn );
        }
    }
    return( NULL );
}


static  void    PrefixExchange( BackRun& back, instruction *ins, actual_st_locn actual_locn )
/****************************************************************************/
{
    if( actual_locn != ACTUAL_0 ) {
        PrefFXCH( back, ins, actual_locn );
    }
}


static  void    GetToTopOfStack( BackRun& back, instruction *ins, virtual_st_locn virtual_locn )
/*******************************************************************************/
{
    actual_st_locn  actual_locn;
    actual_st_locn  *actual_top_owner;

    actual_locn = InsSTLoc( back, ins, virtual_locn );
    if( actual_locn == ACTUAL_0 )
        return;
    actual_top_owner = ActualStackOwner( back, ACTUAL_0 );
    if( actual_top_owner != NULL ) {
        PrefixExchange( back, ins, actual_locn );
        InsSTLoc( back, ins, virtual_locn ) = ACTUAL_0;
        *actual_top_owner = actual_locn;
        return;
    }
    Zoiks( back, ZOIKS_076 );
}


static  fp_attr ResultToReg( BackRun& back, instruction *ins, temp_entry *temp, fp_attr attr )
/*****************************************************************************/
{
    if( ins == temp->first && !temp->whole_block ) {
        DoNothing( back, ins );
        temp->actual_locn = InsSTLoc( back, ins, VIRTUAL_0 );
        PopVirtualStack( back, ins );
        attr &= ~POPS;
    } else {
        GetToTopOfStack( back, ins, VIRTUAL_0 );
        if( G( ins ) == G_MFST ) {
            ins->u.gen_table = RFST;
        } else {
            ins->u.gen_table = RFSTNP;
        }
        ins->result = ST( back, temp->actual_locn );
    }
    return( attr );
}


static  void    PushVirtualStack( BackRun& back, instruction *ins )
/**************************************************/
{
    virtual_st_locn     virtual_locn;

    for( virtual_locn = static_cast<virtual_st_locn>(VIRTUAL_NONE - 1); virtual_locn > VIRTUAL_0; virtual_locn-- ) {
        InsSTLoc( back, ins, virtual_locn ) = InsSTLoc( back, ins, virtual_locn - 1 );
    }
    InsSTLoc( back, ins, VIRTUAL_0 ) = ACTUAL_0;
}


static  void    DecrementAll( BackRun& back )
/**********************************/
{
    int                 i;
    virtual_st_locn     virtual_locn;
    temp_entry          *temp;

    for( i = 0; i < back.i87sched.MaxSeq; ++i ) {
        for( virtual_locn = VIRTUAL_0; virtual_locn < VIRTUAL_NONE - 1; virtual_locn++ ) {
            if( RegSTLoc( back, i, virtual_locn ) != ACTUAL_NONE ) {
                --RegSTLoc( back, i, virtual_locn );
            }
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->actual_locn != ACTUAL_NONE ) {
            temp->actual_locn--;
        }
    }
}


static  void    PopStack( BackRun& back, instruction *ins )
/******************************************/
{
    PopVirtualStack( back, ins );
    DecrementAll(back);
}


static  void    IncrementAll( BackRun& back )
/**********************************/
{
    int                 i;
    virtual_st_locn     virtual_locn;
    temp_entry          *temp;

    for( i = 0; i < back.i87sched.MaxSeq; ++i ) {
        for( virtual_locn = VIRTUAL_0; virtual_locn < VIRTUAL_NONE - 1; virtual_locn++ ) {
            if( RegSTLoc( back, i, virtual_locn ) != ACTUAL_NONE ) {
                ++RegSTLoc( back, i, virtual_locn );
            }
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->actual_locn != ACTUAL_NONE ) {
            temp->actual_locn++;
        }
    }
}


static  void    PushStack( BackRun& back, instruction *ins )
/*******************************************/
{
    IncrementAll(back);
    PushVirtualStack( back, ins );
}


static instruction *OpToReg( BackRun& back, instruction *ins, temp_entry *temp, fp_attr attr )
/*****************************************************************************/
{
    if( ins == temp->last && !temp->whole_block ) {
        switch( G( ins ) ) {
        case G_MFLD:
            PushVirtualStack( back, ins );
            InsSTLoc( back, ins, VIRTUAL_0 ) = temp->actual_locn;
            DoNothing( back, ins );
            break;
        case G_MCOMP:
//            if( temp->actual_locn != ACTUAL_1 ) _Zoiks( ZOIKS_076 );
            ins->operands[0] = ST( back, ACTUAL_1 );
            ins->operands[1] = ST( back, ACTUAL_0 );
            if( temp->actual_locn != ACTUAL_1 ) {
                actual_st_locn  *actual_locn_owner;

                actual_locn_owner = ActualStackOwner( back, ACTUAL_1 );
                *actual_locn_owner = temp->actual_locn;
                PrefFXCH( back, ins, ACTUAL_1 );
                PrefFXCH( back, ins, temp->actual_locn );
                RevCond( ins );
            }
            ins->u.gen_table = FCOMPP;
            PopStack( back, ins );
            PopStack( back, ins );
            break;
        case G_MNFBIN:
        case G_MRFBIN:
            if( G( ins ) == G_MRFBIN ) {
                ins->u.gen_table = RNFBINP;
            } else {
                ins->u.gen_table = RRFBINP;
            }
            ins->operands[0] = ST( back, temp->actual_locn );
            ins->result = ins->operands[0];
            ins->operands[1] = ST( back, ACTUAL_0 );
            InsSTLoc( back, ins, VIRTUAL_0 ) = temp->actual_locn;
            DecrementAll(back);
            break;
        }
        temp->actual_locn = ACTUAL_NONE;
    } else {
        if( ( ins == temp->first ) && !temp->defined && !temp->whole_block ) {
            PrefFLDOp( back, ins, OP_MEM, temp->actual_op );
            IncrementAll(back);
            temp->actual_locn = ACTUAL_0;
            if( G( ins ) == G_MFLD ) {
//                PushStack( ins );
                ins->operands[0] = ST( back, ACTUAL_0 );
            } else {
                GetToTopOfStack( back, ins, VIRTUAL_0 );
                ins->operands[0] = ST( back, temp->actual_locn );
            }
        } else {
            ins->operands[0] = ST( back, temp->actual_locn );
//            if( attr & PUSHES ) {
//                PushStack( ins );
//            }
        }
        if( attr & PUSHES ) {
            PushStack( back, ins );
        } else if( attr & POPS2 ) {
            PopStack( back, ins );
            PopStack( back, ins );
        } else if( attr & POPS ) {
            PopStack( back, ins );
        }
        ins->u.gen_table = RegAction( ins );
    }
    return( ins );
}


static  void    SetResultSTReg( BackRun& back, instruction *ins, virtual_st_locn virtual_locn )
/******************************************************************************/
{
    ins->result = ST( back, InsSTLoc( back, ins, virtual_locn ) );
    ins->operands[0] = ins->result;
}

static  instruction     *FComppKluge( BackRun& back, instruction *ins )
/******************************************************/
{
    actual_st_locn  actual_0;
    actual_st_locn  actual_1;
    actual_st_locn  *owner_0;
    actual_st_locn  *owner_1;

    actual_0 = InsSTLoc( back, ins, VIRTUAL_0 );
    actual_1 = InsSTLoc( back, ins, VIRTUAL_1 );
    owner_0 = ActualStackOwner( back, ACTUAL_0 );
    owner_1 = ActualStackOwner( back, ACTUAL_1 );
    if( actual_0 == ACTUAL_0 ) {
        if( actual_1 == ACTUAL_1 ) {    // actual_0=0 actual_1=1
            return( ins );
        } else {                        // actual_0=0 actual_1=n
            PrefFXCH( back, ins, ACTUAL_1 );
            PrefFXCH( back, ins, actual_1 );
            PrefFXCH( back, ins, ACTUAL_1 );
            *owner_1 = actual_1;
        }
    } else if( actual_1 == ACTUAL_0 ) {
        if( actual_0 == ACTUAL_1 ) {    // actual_0=1 actual_1=0
            PrefFXCH( back, ins, ACTUAL_1 );
        } else {                        // actual_0=n actual_1=0
            PrefFXCH( back, ins, ACTUAL_1 );
            PrefFXCH( back, ins, actual_0 );
            *owner_1 = actual_0;
        }
    } else if( actual_1 == ACTUAL_1 ) { // actual_1=1 actual_0=n
        PrefFXCH( back, ins, actual_0 );
        *owner_0 = actual_0;
    } else {                            // actual_0=1,n actual_1=n
        PrefFXCH( back, ins, actual_1 );
        PrefFXCH( back, ins, ACTUAL_1 );
        PrefFXCH( back, ins, actual_0 );
        *owner_0 = actual_1;
        *owner_1 = actual_0;
    }
    InsSTLoc( back, ins, VIRTUAL_0 ) = ACTUAL_0;
    InsSTLoc( back, ins, VIRTUAL_1 ) = ACTUAL_1;
    return( ins );
}

static  instruction     *GetST0andST1( BackRun& back, instruction *ins )
/*******************************************************/
{
    actual_st_locn  actual_0;
    actual_st_locn  actual_1;

    actual_0 = InsSTLoc( back, ins, VIRTUAL_0 );
    actual_1 = InsSTLoc( back, ins, VIRTUAL_1 );
    if( actual_0 == ACTUAL_0 ) {
        if( actual_1 == ACTUAL_1 ) {    // actual_0=0 actual_1=1
            return( ins );
        } else {                        // actual_0=0 actual_1=n
            PrefFLDOp( back, ins, OP_STKI, ST( back, actual_1 ) );
            PrefFXCH( back, ins, ACTUAL_1 );
            return( SuffFSTPRes( back, ins, ST( back, actual_1 ), RES_STKI ) );
        }
    } else if( actual_1 == ACTUAL_0 ) {
        if( actual_0 == ACTUAL_1 ) {    // actual_0=1 actual_1=0
            GetToTopOfStack( back, ins, VIRTUAL_0 );
            return( ins );
        } else {                        // actual_0=n actual_1=0
            InsSTLoc( back, ins, VIRTUAL_1 ) = actual_0;
            PrefFLDOp( back, ins, OP_STKI, ST( back, actual_0 ) );
            return( SuffFSTPRes( back, ins, ST( back, actual_0 ), RES_STKI ) );
        }
    } else if( actual_1 == ACTUAL_1 ) { // actual_1=1 actual_0=n
        GetToTopOfStack( back, ins, VIRTUAL_0 );
        return( ins );
    } else {                            // actual_0=1,n actual_1=n
        GetToTopOfStack( back, ins, VIRTUAL_1 );
        InsSTLoc( back, ins, VIRTUAL_1 ) = actual_0;
        PrefFLDOp( back, ins, OP_STKI, ST( back, actual_0 ) );
        return( SuffFSTPRes( back, ins, ST( back, actual_0 ), RES_STKI ) );
    }
}


static  void    InitStackLocations( BackRun& back )
/****************************************/
{
    temp_entry          *temp;
    int                 i;
    virtual_st_locn     virtual_locn;

    back.i87sched.STLocations = static_cast<st_seq *>(CGAlloc( back, back.i87sched.MaxSeq * sizeof( *back.i87sched.STLocations ) ));
    for( i = 0; i < back.i87sched.MaxSeq; ++i ) {
        for( virtual_locn = VIRTUAL_0; virtual_locn < VIRTUAL_NONE; virtual_locn++ ) {
            RegSTLoc( back, i, virtual_locn ) = ACTUAL_NONE;
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        temp->actual_locn = ACTUAL_NONE;
    }
}


static  void    FiniStackLocations( BackRun& back )
/****************************************/
{
    CGFree( back, back.i87sched.STLocations );
    back.i87sched.STLocations = NULL;
}


static  temp_entry  *LookupTempEntry( BackRun& back, name *op )
/**********************************************/
{
    temp_entry  *temp;

    if( op == NULL )
        return( NULL );
    if( op->n.class_ == N_TEMP )
        op = DeAlias( op );
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->op == op ) {
            return( temp );
        }
    }
    return( NULL );
}


static  temp_entry      *AddTempEntry( BackRun& back, name *op )
/***********************************************/
{
    temp_entry  *temp;

    temp = LookupTempEntry( back, op );
    if( temp == NULL ) {
        temp = static_cast<temp_entry *>(CGAlloc( back, sizeof( *temp ) ));
        temp->next = back.i87sched.TempList;
        back.i87sched.TempList = temp;
        temp->actual_op = op;
        if( op->n.class_ == N_TEMP )
            op = DeAlias( op );
        temp->op = op;
        temp->first = NULL;
        temp->last = NULL;
        temp->savings = 0;
        temp->actual_locn = ACTUAL_NONE;
        temp->savings = 0;
        temp->cached = false;
        temp->defined = false;
        temp->killed = false;
        temp->global = false;
        temp->whole_block = false;
    }
    return( temp );
}


static  void    DefUseTemp( BackRun& back, name *op, instruction *ins, bool defined )
/********************************************************************/
{
    temp_entry *temp;

    temp = AddTempEntry( back, op );
    if( op->v.offset != temp->actual_op->v.offset ||
        op->n.size != temp->actual_op->n.size ) {
        temp->killed = true;
        return;
    }
    if( temp->first == NULL )
        temp->first = ins;
    temp->last = ins;
    temp->savings++;
    if( defined ) {
        temp->defined = true;
    }
}


static  void            KillTempEntry( BackRun& back, name *op )
/***********************************************/
{
    temp_entry  *temp;

    temp = AddTempEntry( back, op );
    temp->killed = true;
}


static  void    CheckTemp( BackRun& back, instruction *ins, name *op, bool defined )
/*******************************************************************/
{
    if( op->n.class_ == N_MEMORY ) {
        if( IsntModel( back, CGSW_GEN_RELAX_ALIAS ) ) {
            return;
        }
    } else if( op->n.class_ != N_TEMP ) {
        return;
    }
    if( op->v.usage & USE_ADDRESS )
        return;
    if( !_GenIs8087( G( ins ) ) ) {
        KillTempEntry( back, op );
    } else {
        DefUseTemp( back, op, ins, defined );
    }
}


static  bool Better( BackRun& back, void *t1, void *t2 )
/***************************************/
{
    return( ((temp_entry *)t1)->savings > ((temp_entry *)t2)->savings );
}


static void    InitTempEntries( BackRun& back, block *blk )
/******************************************/
{
    instruction *ins;
    opcnt       i;

    back.i87sched.TempList = NULL;
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( ins->ins_flags & FP_INS_INTRODUCED )
            continue;
        for( i = 0; i < ins->num_operands; ++i ) {
            CheckTemp( back, ins, ins->operands[i], false );
        }
        if( ins->result != NULL ) {
            CheckTemp( back, ins, ins->result, true );
        }
    }
    back.i87sched.TempList = static_cast<temp_entry *>(SortList( back, back.i87sched.TempList, offsetof( temp_entry, next ), Better ));
}


static void    FiniTempEntries( BackRun& back )
/************************************/
{
    temp_entry  *temp;
    temp_entry  *next;

    for( temp = back.i87sched.TempList; temp != NULL; temp = next ) {
        next = temp->next;
        CGFree( back, temp );
    }
    back.i87sched.TempList = NULL;
}


static  bool    StackBetween( BackRun& back, instruction *first, instruction *last, int inc )
/****************************************************************************/
{
    instruction *ins;
    bool        enough;

    enough = true;
    for( ins = first; ins != last; ins = ins->head.next ) {
        if( ins->fp.u.stk_depth >= ( back.i87data.Max87Stk - 1 ) || _OpIsCall( ins->head.opcode ) ) {
            enough = false;
        }
        ins->fp.u.stk_depth += inc;
    }
    return( enough );
}


static  void    KillRelatedTemps( BackRun& back, name *op )
/******************************************/
{
    temp_entry  *temp;

    if( op->n.class_ != N_MEMORY )
        return;
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->op->n.class_ == N_MEMORY &&
            temp->op->v.symbol == op->v.symbol ) {
            temp->killed = true;
        }
    }
}

static  bool    OKToCache( BackRun& back, temp_entry *temp ) {
/*********************************************/

    instruction         *ins;
    name                *seg;

    ins = temp->first;
    if( ins->num_operands <= OpcodeNumOperands( ins ) )
        return( true );
    seg = ins->operands[ins->num_operands - 1];
    for( ins = ins->head.prev; ins->head.opcode != OP_BLOCK; ins = ins->head.prev ) {
        /*
         * Might be a segment load or some other sort of nonsense here.
         */
        if( IsReDefinedBy( back, ins, seg ) ) {
            return( false );
        }
    }
    return( _BLOCK( ins ) == back.i87sched_file.Entry );
}

static  void    CacheTemps( BackRun& back, block *blk )
/**************************************/
{
    temp_entry *temp, **owner;
    block_edge  *exit_edge = NULL;

    back.i87sched_file.Entry = NULL;
    back.i87sched_file.Exit = NULL;
    if( _IsBlkAttr( blk, BLK_LOOP_HEADER ) && blk->inputs == 2 && blk->targets == 2 ) {
        if( blk->edge[0].destination.u.blk == blk ) {
            back.i87sched_file.Exit = blk->edge[1].destination.u.blk;
            exit_edge = &blk->edge[1];
        } else if( blk->edge[1].destination.u.blk == blk ) {
            back.i87sched_file.Exit = blk->edge[0].destination.u.blk;
            exit_edge = &blk->edge[0];
        } else {
            back.i87sched_file.Exit = NULL;
        }
        if( back.i87sched_file.Exit != NULL ) {
            if( blk->input_edges->source == blk ) {
                back.i87sched_file.Entry = blk->input_edges->next_source->source;
            } else if( blk->input_edges->next_source->source == blk ) {
                back.i87sched_file.Entry = blk->input_edges->source;
            } else {
                back.i87sched_file.Entry = NULL;
                back.i87sched_file.Exit = NULL;
            }
        }
        if( back.i87sched_file.Entry != NULL ) {
            if( _IsBlkAttr( back.i87sched_file.Entry, BLK_JUMP ) ) {
                if( back.i87sched_file.Exit->inputs != 1 ) {
                    back.i87sched_file.Exit = AddPreBlock( back, back.i87sched_file.Exit );
                    MoveEdge( exit_edge, back.i87sched_file.Exit );
                }
            }
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->killed )
            continue;
        if( !OKToCache( back, temp ) ) {
            temp->killed = true;
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->killed ) {
            KillRelatedTemps( back, temp->op );
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->killed )
            continue;
        if( temp->op->v.usage & USE_IN_ANOTHER_BLOCK ) {
            if( back.i87sched_file.Entry != NULL ) {
                temp->first = blk->ins.head.next;
                temp->last = blk->ins.head.prev;
                temp->whole_block = true;
            } else {
                if( temp->defined )
                    continue;
                if( temp->first == temp->last ) {
                    continue;
                }
            }
            temp->global = true;
        } else {
            if( !temp->defined )
                continue; // I'm not sure if these save anything
            if( temp->defined && G( temp->first ) != G_MFST )
                continue;
            if( RegAction( temp->last ) == NULL )
                continue;
            if( temp->first == temp->last ) {
                continue;
            }
        }
        if( StackBetween( back, temp->first, temp->last, 0 ) ) {
            StackBetween( back, temp->first, temp->last, 1 );
            temp->cached = true;
        }
    }
    owner = &back.i87sched.TempList;
    for( ;; ) {
        temp = *owner;
        if( temp == NULL )
            break;
        if( temp->cached ) {
            owner = &temp->next;
        } else {
            *owner = temp->next;
            CGFree( back, temp );
        }
    }
}


static  void    FiniGlobalTemps( BackRun& back )
/*************************************/
{
    temp_entry  *temp;
    instruction *ins;

    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->whole_block ) {
            ins = (instruction *)&back.i87sched_file.Exit->ins;
            if( temp->defined ) {
                SuffFSTPRes( back, ins, temp->actual_op, RES_MEM );
            } else {
                SuffFSTPRes( back, ins, temp->actual_op, RES_MEM_THROWAWAY );
            }
            ins->head.next->ins_flags |= FP_INS_INTRODUCED;
        }
    }
}


static  void    InitGlobalTemps( BackRun& back )
/*************************************/
{
    temp_entry      *temp;
    instruction     *ins;
    actual_st_locn  actual_locn;

    actual_locn = ACTUAL_0;
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->whole_block ) {
            actual_locn++;
        }
    }
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->whole_block ) {
            for( ins = back.i87sched_file.Entry->ins.head.prev; ins->head.opcode == OP_NOP; ins = ins->head.prev ) {
                if( ins->flags.u.nop_flags & NOP_ZAP_INFO ) {
                    break;
                }
            }
            ins = ins->head.next;
            PrefFLDOp( back, ins, OP_MEM, temp->actual_op );
            ins->head.prev->ins_flags |= FP_INS_INTRODUCED;
            temp->actual_locn = --actual_locn;
        }
    }
}


static  void    XchForCall( BackRun& back, instruction *ins, int i )
/***************************************************/
{
    actual_st_locn  *actual_i_owner;
    actual_st_locn  *actual_0_owner;

    if( i <= 0 )
        return;
    if( InsSTLoc( back, ins, VIRTUAL_0 + i ) != ACTUAL_0 + i ) {
        GetToTopOfStack( back, ins, static_cast<virtual_st_locn>(VIRTUAL_0 + i) );
        if( InsSTLoc( back, ins, VIRTUAL_0 + i ) != ACTUAL_0 + i ) {
            PrefixExchange( back, ins, static_cast<actual_st_locn>(ACTUAL_0 + i) );
            actual_i_owner = ActualStackOwner( back, static_cast<actual_st_locn>(ACTUAL_0 + i) );
            actual_0_owner = ActualStackOwner( back, ACTUAL_0 );
            *actual_i_owner = ACTUAL_0;
            *actual_0_owner = static_cast<actual_st_locn>(ACTUAL_0 + i);
        }
    }
    XchForCall( back, ins, i - 1 );
}


static  void    ReOrderForCall( BackRun& back, instruction *ins )
/************************************************/
{
    int         i;
    int         count;

    count = CountFPRegs( ins->operands[CALL_OP_USED]->r.reg );
    XchForCall( back, ins, count - 1 );
    for( i = 0; i < count; ++i ) {
        PopStack( back, ins );
    }
}

bool    FPInsIntroduced( instruction *ins )
/******************************************
 * Used by the scheduler. We want to make sure that introduced instructions
 * stay put.
 */
{
    return( ( ins->ins_flags & FP_INS_INTRODUCED ) != 0 );
}


bool    FPFreeIns( BackRun& back, instruction *ins )
/**********************************************
 * Return true if "ins" is going to be vaporized by the cacheing
 * algorithm in FPPostSched.
 */
{
    temp_entry  *temp;

    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        if( temp->whole_block ) {
            if( G( ins ) == G_MFST &&
                temp->actual_op == ins->result ) {
                return( true ); // will likely merge into the previous op
            }
        } else if( ins == temp->first && temp->defined ) {
            return( true );
        } else if( ins == temp->last && G( ins ) == G_MFLD ) {
            return( true );
        }
    }
    return( false );
}

int     FPStkOver( BackRun& back, instruction *ins, int stk_depth )
/***********************************************************
 * Return >= 0 if scheduling "ins" could get us into a spot that
 * we cannot get out of with the amount of stack left. What this
 * says is that we can only schedule an instruction in sequence i
 * if there's enough stack left to complete sequence i, plus
 * the maximum stack requirement of all sequences that might
 * need to be scheduled before sequence i can complete. i+1..MaxSeq
 * is a conservative (and cheaply calculated) estimate of the
 * those dependencies. We know that sequences 0..i-1 are ok since they
 * were in front of i in the original ordering.
 */
{
    int         i;
    int         depth;
    int         max_depth;

    max_depth = 0;
    for( i = ins->sequence + 1; i < back.i87sched.MaxSeq; ++i ) {
        depth = back.i87sched.SeqMaxDepth[i] - back.i87sched.SeqCurDepth[i];
        if( depth > max_depth ) {
            max_depth = depth;
        }
    }
    return( back.i87sched.SeqMaxDepth[ins->sequence] - back.i87sched.SeqCurDepth[ins->sequence] + max_depth + stk_depth - back.i87data.Max87Stk );
}


void    FPCalcStk( BackRun& back, instruction *ins, int *pdepth )
/*********************************************************
 * Set pdepth to the stack level before "ins" executes.  Also,
 * recalculate the stk_entry, stk_exit. s.stk_depth
 * now means the maximum depth the stack attains during this
 * instruction.
 */
{
    int         affect;

    if( FPStackIns( back, ins ) )
        back.i87sched.SeqMaxDepth[ins->sequence] = ins->u2.stk_max;
    affect = ins->stk_entry - ins->stk_exit;
    back.i87sched.SeqCurDepth[ins->sequence] += affect;
    ins->stk_exit = *pdepth;
    ins->stk_entry = *pdepth + affect;
    ins->fp.u.stk_depth = *pdepth + ins->fp.u.stk_extra;
    if( affect > 0 )
        ins->fp.u.stk_depth += affect;
    *pdepth += affect;
}

int     FPStackExit( BackRun& back, block *blk )
/******************************************
 * Return the depth of the FPU stack upon exit from this
 * block. This is the stk_exit from the last FPU
 * instruction in the block.
 */
{
    instruction *curr;

    for( curr = blk->ins.head.prev; curr->head.opcode != OP_BLOCK; curr = curr->head.prev ) {
        if( FPStackIns( back, curr ) ) {
            return( curr->stk_exit );
        }
    }
    return( 0 );
}

/*
 * just have to make sure this is not a valid FP-stack depth
 */
#define SEQ_INIT_VALUE  0xff

void    FPPreSched( BackRun& back, block *blk )
/******************************/
{
    temp_entry  *temp;
    instruction *ins;
    int         i;
    int         depth;

    back.i87sched.MaxSeq = 0;
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        if( ins->sequence > back.i87sched.MaxSeq ) {
            back.i87sched.MaxSeq = ins->sequence;
        }
    }
    ++back.i87sched.MaxSeq;
    InitTempEntries( back, blk );
    CacheTemps( back, blk );
    for( temp = back.i87sched.TempList; temp != NULL; temp = temp->next ) {
        StackBetween( back, temp->first, temp->last, -1 );
    }
    back.i87sched.SeqCurDepth = static_cast<byte *>(CGAlloc( back, back.i87sched.MaxSeq * sizeof( *back.i87sched.SeqCurDepth ) ));
    back.i87sched.SeqMaxDepth = static_cast<byte *>(CGAlloc( back, back.i87sched.MaxSeq * sizeof( *back.i87sched.SeqMaxDepth ) ));
    for( i = 0; i < back.i87sched.MaxSeq; ++i ) {
        back.i87sched.SeqCurDepth[i] = SEQ_INIT_VALUE;
        back.i87sched.SeqMaxDepth[i] = 0;
    }
    for( ins = blk->ins.head.prev; ins->head.opcode != OP_BLOCK; ins = ins->head.prev ) {
        if( back.i87sched.SeqCurDepth[ins->sequence] == SEQ_INIT_VALUE ) {
            if( FPStackIns( back, ins ) ) {
                back.i87sched.SeqCurDepth[ins->sequence] = ins->stk_exit;
            }
        }
    }
    for( i = 0; i < back.i87sched.MaxSeq; ++i ) {
        if( back.i87sched.SeqCurDepth[i] == SEQ_INIT_VALUE ) {
            back.i87sched.SeqCurDepth[i] = 0;
        }
    }
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
        /*
         * We do this here in order to not be faked out by inheriting bogus values
         * when we prefix an instruction to another FP instruction. This would screw
         * up our instruction scheduling by digging a hole which FPStkOver would
         * be convinced we could not get out of. For an illustration of the problem,
         * consider the two sequences "pow a, b -> c" and "pow d, e -> f" before
         * RegAlloc et all and how we would schedule these with -fpr.
         */
        ins->fp.u.stk_extra = FPStkReq( back, ins );
        depth = InsMaxDepth( ins ) + ins->fp.u.stk_extra;
        ins->u2.stk_max = back.i87sched.SeqMaxDepth[ins->sequence];
        if( depth > back.i87sched.SeqMaxDepth[ins->sequence] ) {
            back.i87sched.SeqMaxDepth[ins->sequence] = depth;
        }
    }
}


void    FPPostSched( BackRun& back, block *blk )
/*******************************/
{
    fp_attr         attr;
    instruction     *ins;
    instruction     *next;
    temp_entry      *temp;

    CGFree( back, back.i87sched.SeqCurDepth );
    CGFree( back, back.i87sched.SeqMaxDepth );
    FiniTempEntries(back);
    InitStackLocations(back);
    InitTempEntries( back, blk );
    CacheTemps( back, blk );
    InitGlobalTemps(back);
    for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next->head.next ) {
        next = ins;
        if( (ins->ins_flags & FP_INS_INTRODUCED) == 0 ) {
            attr = FPAttr( back, ins );
            temp = LookupTempEntry( back, ins->result );
            if( attr & NEEDS_ST0 && temp == NULL ) {
                GetToTopOfStack( back, ins, VIRTUAL_0 );
            } else if( attr & NEEDS_ST0_ST1 ) {
                if( attr & POPS2 ) {
                    /*
                     * the dreaded FCOMPP
                     */
                    next = FComppKluge( back, ins );
                } else {
                    next = GetST0andST1( back, ins );
                }
            } else if( attr & POPS_ALL ) {
                ReOrderForCall( back, ins );
            }
            if( temp != NULL ) {
                attr = ResultToReg( back, ins, temp, attr );
            } else if( attr & SETS_ST1 ) {
                SetResultSTReg( back, ins, VIRTUAL_1 );
            } else if( attr & EXCHANGES ) {
                /*
                 * ??? what is in ins->result ???
                 * ??? virtual or actual      ???
                 */
                SetResultSTReg( back, ins, (virtual_st_locn)FPRegNum( ins->result ) );
            }
            temp = LookupTempEntry( back, ins->operands[0] );
            if( temp != NULL ) {
                next = OpToReg( back, ins, temp, attr );
            } else if( attr & PUSHES ) {
                PushStack( back, ins );
            } else if( attr & POPS2 ) {
                PopStack( back, ins );
                PopStack( back, ins );
            } else if( attr & POPS ) {
                PopStack( back, ins );
            }
        }
    }
    FiniGlobalTemps(back);
    FiniStackLocations(back);
    FiniTempEntries(back);
}
