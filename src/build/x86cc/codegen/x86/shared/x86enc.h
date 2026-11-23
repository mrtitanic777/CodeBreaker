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
* Description:  Take the instruction stream from the code generator and
*               writes the instructions into the object file.
*
****************************************************************************/


#pragma once
#include "x86regn.h"


extern void             ReFormat( BackRun& back, oc_class class_ );
extern void             AddByte( BackRun& back, byte b );
extern void             AddToTemp( BackRun& back, byte b );
extern void             EmitByte( BackRun& back, byte b );
extern void             EmitPtr( BackRun& back, pointer p );
extern void             EmitSegId( BackRun& back, segment_id segid );
extern void             EmitOffset( BackRun& back, offset i );
extern void             LayOpbyte( BackRun& back, gen_opcode op );
extern void             LayOpword( BackRun& back, gen_opcode op );
extern void             LayW( BackRun& back, type_class_def type_class );
extern void             LayReg( BackRun& back, hw_reg_set r );
extern void             LayRMRegOp( BackRun& back, name *r );
extern void             LayRegRM( BackRun& back, hw_reg_set r );
extern void             LayRegAC( BackRun& back, hw_reg_set r );
extern void             GenSeg( BackRun& back, hw_reg_set regs );
extern type_class_def   OpndSize( BackRun& back, hw_reg_set reg );
extern void             QuickSave( BackRun& back, hw_reg_set reg, opcode_defs opcode );
extern void             GenRegXor( BackRun& back, hw_reg_set src, hw_reg_set dst );
extern void             GenRegNeg( BackRun& back, hw_reg_set src );
extern void             GenRegMove( BackRun& back, hw_reg_set src, hw_reg_set dst );
extern void             AddWData( BackRun& back, int_32 value, type_class_def type_class );
extern void             AddWCons( BackRun& back, name *op, type_class_def type_class );
extern void             AddSData( BackRun& back, int_32 value, type_class_def type_class );
extern void             GenRegAdd( BackRun& back, hw_reg_set dst, type_length value );
extern void             GenRegSub( BackRun& back, hw_reg_set dst, type_length value );
extern void             GenRegAnd( BackRun& back, hw_reg_set dst, type_length value );
extern void             GFldMorC( BackRun& back, name *what );
extern void             GFldM( BackRun& back, pointer what );
extern void             GFstpM( BackRun& back, pointer what );
extern void             GFstp( BackRun& back, int i );
extern void             GFxch( BackRun& back, int i );
extern void             GFldz( BackRun& back );
extern void             GFld( BackRun& back, int i );
extern void             GCondFwait( BackRun& back );
extern void             GFwait( BackRun& back );
extern void             Gpusha( BackRun& back );
extern void             Gpopa( BackRun& back );
extern void             Gcld( BackRun& back );
extern void             GenLeave( BackRun& back );
extern void             GenTouchStack( BackRun& back, bool sp_might_point_at_something );
extern void             GenEnter( BackRun& back, int size, int level );
extern void             GenPushOffset( BackRun& back, byte offset );
extern void             GenUnkSub( BackRun& back, hw_reg_set dst, pointer value );
extern void             GenUnkMov( BackRun& back, hw_reg_set dst, pointer value );
extern void             GenUnkEnter( BackRun& back, pointer value, int level );
extern void             GenWindowsProlog( BackRun& back );
extern void             GenCheapWindowsProlog( BackRun& back );
extern void             GenWindowsEpilog( BackRun& back );
extern void             GenCheapWindowsEpilog( BackRun& back );
extern void             GenRdosdevProlog( BackRun& back );
extern void             GenRdosdevEpilog( BackRun& back );
extern void             GenLoadDS( BackRun& back );

extern void             DoRepOp( BackRun& back, instruction *ins );
extern void             Do4CXShift( BackRun& back, instruction *ins, void (*rtn)(BackRun&, instruction *) );
extern void             LayLeaRegOp( BackRun& back, instruction *ins );
extern void             DoMAddr( BackRun& back, name *op );
extern byte             DoMDisp( BackRun& back, name *op, bool alt_encoding );
extern void             LayModRM( BackRun& back, name *op );
extern byte             Displacement( BackRun& back, int_32 val, hw_reg_set regs );
extern byte             DoIndex( BackRun& back, hw_reg_set regs );
extern void             DoRelocConst( BackRun& back, name *op, type_class_def type_class );
extern void             Do4Shift( BackRun& back, instruction *ins );
extern void             Do4RShift( BackRun& back, instruction *ins );
extern void             Gen4RNeg( BackRun& back, instruction *ins );
extern void             Gen4Neg( BackRun& back, instruction *ins );
extern void             By2Div( BackRun& back, instruction *ins );
extern void             Pow2Div( BackRun& back, instruction *ins );
extern void             GFstp10( BackRun& back, type_length where );
extern void             GFld10( BackRun& back, type_length where );

#if _TARGET & _TARG_8086
extern void             Pow2Div286(BackRun& back, instruction*);
#else
extern type_length      TmpLoc( BackRun& back, name *base, name *op );
extern void             GenUnkPush( BackRun& back, pointer value );
extern void             GenPushC( BackRun& back, int_32 value );
extern pointer          GenFar16Thunk( BackRun& back, pointer label, uint_16 parms_size, bool remove_parms );
extern void             GenP5ProfilingProlog( BackRun& back, label_handle label );
extern void             GenP5ProfilingEpilog( BackRun& back, label_handle label );
#endif
