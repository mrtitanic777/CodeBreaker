/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023      The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Loop optimizations.
*
****************************************************************************/


#pragma once
block    *AddPreBlock( BackRun& back, block *postblk );
void     MarkLoop( BackRun& back );
void     UnMarkLoop( BackRun& back );
block    *NextInLoop( BackRun& back, block *blk );
block    *NextInProg( block *blk );
void     MakeJumpBlock( block *cond_blk, block_edge *exit_edge );
void     UnMarkInvariants( BackRun& back );
void     MarkInvariants( BackRun& back );
bool     InvariantOp( BackRun& back, name *op );
bool     Hoistable( BackRun& back, instruction *ins, block *blk );
induction    *FindIndVar( BackRun& back, name *op );
void     CommonInvariant( BackRun& back );
void    FiniIndVars( BackRun& back );
bool    Inducable( block *blk, instruction *ins );
void    SuffixPreHeader( BackRun& back, instruction *ins );
void    MoveDownLoop( BackRun& back, block *cond );
instruction     *DupIns( BackRun& back, instruction *blk_end, instruction *ins, name *var, int_32 adjust );
instruction     *DupInstrs( BackRun& back, instruction *blk_end, instruction *first, instruction *last, induction *var, int_32 adjust );
bool    AnalyseLoop( BackRun& back, induction *var, bool *ponecond, instruction **pcond, block **pcond_blk );
bool    CalcFinalValue( BackRun& back, induction *var, block *blk, instruction *ins, int_32 *final, int_32 *initial );
bool    LoopInvariant( BackRun& back );
bool    LoopRegInvariant( BackRun& back );
void    LoopEnregister( BackRun& back );
bool    IndVars( BackRun& back );
bool    TransLoops( BackRun& back, bool unroll );

#ifndef CB_STATE_LOOPOPTS_H
#define CB_STATE_LOOPOPTS_H

/* the variables of loopopts.h, one set for each run of the compiler (run/cbrun.hpp) */
struct LoopoptsState {
    block *Head;
    block *PreHead;
    induction *IndVarList;
    block *Loop;
};

#endif
