/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Segment register optimizations.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zeropage.h"
#include "data.h"
#include "object.h"
#include "fixindex.h"
#include "x86segs.h"
#include "pccode.h"
#include "x86enc.h"
#include "x86proc.h"
#include "x86opseg.h"
#include "backrun.hpp"


static  int     Overs( BackRun& back, name *op )
/********************************
    return the number of SS: overrides associated with "op" (0 or 1)
*/
{
    name        *base;
    name        *index;

    if( op->n.class_ == N_MEMORY ) {
        if( SegIsSS( back, op ) )
            return( 1 );
        return( 0 );
    } else if( op->n.class_ == N_INDEXED ) {
        base = op->i.base;
        if( !HasTrueBase( op ) )
            return( 0 );
        if( base->n.class_ != N_MEMORY )
            return( 0 );
        if( !SegIsSS( back, base ) )
            return( 0 );
        index = op->i.index;
        if( HW_CEqual( index->r.reg, HW_BX ) )
            return( 0 );
        if( back.x86opseg.ZPageType == ZP_USES_SI )
            return( 0 );
        if( back.x86opseg.ZPageType == ZP_USES_DI )
            return( 0 );
        return( 1 );
    }
    return( 0 );
}

static  int     CountSegOvers( BackRun& back )
/************************************
    Count the number of SS: overrides in the routine.
*/
{
    block       *blk;
    instruction *ins;
    opcnt       i;
    int         overs;

    overs = 0;
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( OpcodeNumOperands( ins ) == ins->num_operands && ( back.x86opseg.ZPageType == ZP_USES_DS
               || ( G( ins ) != G_MOVAM && G( ins ) != G_MOVMA ) ) ) {
                for( i = OpcodeNumOperands( ins ); i-- > 0; ) {
                    overs += Overs( back, ins->operands[i] );
                }
                if( ins->result != NULL ) {
                    overs += Overs( back, ins->result );
                }
            }
        }
    }
    return( overs );
}

void    InitZeroPage( BackRun& back )
/***************************
    Decide what type of "zeropage" scheme we should use (if it's worthwhile).
    This is sort of a misleading name, carried over from the days of the 6809.
    What it is is cheap addressing to the stack segment. We'll set BP to zero
    and use _X[BP], or, if that's not possible, set DI or SI to -BP and
    use _x[bp+di] or _x[bp+si]. This generates the code to initialize
    the "zeropage" register, as well as pushing it to save it.
*/
{
    back.x86opseg.ZPageType = ZP_USES_SS;
    if( _IsTargetModel( CGSW_X86_FLOATING_DS )
      && back.data.OptForSize > 50 ) {
        if( !HW_COvlap( back.data.CurrProc->state.used, HW_DS ) ) {
            back.x86opseg.ZPageType = ZP_USES_DS;
            if( CountSegOvers(back) > 4 ) {
                QuickSave( back, HW_DS, OP_PUSH );
                QuickSave( back, HW_SS, OP_PUSH );
                QuickSave( back, HW_DS, OP_POP );
            } else {
                back.x86opseg.ZPageType = ZP_USES_SS;
            }
        } else if( CanZapBP(back)
          && !HW_COvlap( back.data.CurrProc->state.used, HW_BP ) ) {
            back.x86opseg.ZPageType = ZP_USES_BP;
            if( CountSegOvers(back) > 4 ) {
                QuickSave( back, HW_BP, OP_PUSH );
                GenRegXor( back, HW_BP, HW_BP );
            } else {
                back.x86opseg.ZPageType = ZP_USES_SS;
            }
        } else if( !HW_COvlap( back.data.CurrProc->state.used, HW_DI ) ) {
            back.x86opseg.ZPageType = ZP_USES_DI;
            if( CountSegOvers(back) > 6 ) {
                QuickSave( back, HW_DI, OP_PUSH );
                GenRegMove( back, HW_BP, HW_DI );
                GenRegNeg( back, HW_DI );
            } else {
                back.x86opseg.ZPageType = ZP_USES_SS;
            }
        } else if( !HW_COvlap( back.data.CurrProc->state.used, HW_SI ) ) {
            back.x86opseg.ZPageType = ZP_USES_SI;
            if( CountSegOvers(back) > 6 ) {
                QuickSave( back, HW_SI, OP_PUSH );
                GenRegMove( back, HW_BP, HW_SI );
                GenRegNeg( back, HW_SI );
            } else {
                back.x86opseg.ZPageType = ZP_USES_SS;
            }
        }
    }
}


void    FiniZeroPage( BackRun& back )
/***************************
    Pop the "zeropage" register.
*/
{
    switch( back.x86opseg.ZPageType ) {
    case ZP_USES_SS:
        /* nothing*/
        break;
    case ZP_USES_DS:
        QuickSave( back, HW_DS, OP_POP );
        break;
    case ZP_USES_BP:
        QuickSave( back, HW_BP, OP_POP );
        break;
    case ZP_USES_SI:
        QuickSave( back, HW_SI, OP_POP );
        break;
    case ZP_USES_DI:
        QuickSave( back, HW_DI, OP_POP );
        break;
    }
    back.x86opseg.ZPageType = ZP_USES_SS;
}
