/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Prototypes for treefold.c
*
****************************************************************************/

#pragma once
extern  int             GetLog2( uint_32 value );
extern  tn              FoldTimes( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  float_handle    OkToNegate( BackRun& back, float_handle value, const type_def *tipe );
extern  tn      FoldMinus( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldPlus( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldPow( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldAnd( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldOr( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldXor( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldRShift( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldLShift( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldDiv( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      FoldMod( BackRun& back, tn left, tn rite, const type_def *tipe );
extern  tn      Fold1sComp( BackRun& back, tn left, const type_def *tipe );
extern  tn      FoldUMinus( BackRun& back, tn left, const type_def *tipe );
extern  tn      FoldSqrt( BackRun& back, tn left, const type_def *tipe );
extern  tn      FoldLog( BackRun& back, cg_op opcode, tn left, const type_def *tipe );
extern  tn      FoldFlAnd( BackRun& back, tn left, tn rite );
extern  tn      FoldFlOr( BackRun& back, tn left, tn rite );
extern  tn      FoldFlNot( BackRun& back, tn left );
extern  tn      FoldBitCompare( BackRun& back, cg_op opcode, tn left, tn rite );
extern  float_handle CnvCFToType( BackRun& back, float_handle cf, const type_def *tipe );
extern  tn      FoldCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern  tn      FoldPostGetsCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern  an      FoldConsCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern  bool    FoldIfTrue( BackRun& back, tn left, label_handle lbl );
extern  bool    FoldIfFalse( BackRun& back, tn left, label_handle lbl );
