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
* Description:  Prototypes for *rgtbl.c
*
****************************************************************************/


#pragma once
extern const hw_reg_set     *RegSetList( BackRun& back, unsigned index );   /* the registers of a register list */
extern const op_regs    RegList[];

extern void             InitRegTbl( BackRun& back );
extern reg_set_index    RegIntersect( reg_set_index s1, reg_set_index s2 );
extern const hw_reg_set *ParmChoices( BackRun& back, type_class_def type_class );
extern hw_reg_set       InLineParm( hw_reg_set regs, hw_reg_set used );
extern hw_reg_set       StructReg( void );
extern reg_set_index    SegIndex( void );
extern reg_set_index    NoSegments( reg_set_index idx );
extern reg_set_index    IndexIntersect( reg_set_index curr, type_class_def type_class, bool is_temp_index );
extern bool             IsIndexReg( BackRun& back, hw_reg_set reg, type_class_def type_class, bool is_temp_index );
extern type_class_def   RegClass( BackRun& back, hw_reg_set regs );
extern bool             IndexRegOk( BackRun& back, hw_reg_set reg, bool is_temp_index );
extern bool             IsSegReg( hw_reg_set regs );
extern hw_reg_set       Low16Reg( hw_reg_set regs );
extern hw_reg_set       High16Reg( hw_reg_set regs );
extern hw_reg_set       Low32Reg( hw_reg_set regs );
extern hw_reg_set       High32Reg( hw_reg_set regs );
extern hw_reg_set       HighReg( BackRun& back, hw_reg_set regs );
extern hw_reg_set       HighOffsetReg( BackRun& back, hw_reg_set regs );
extern hw_reg_set       HighTreePart( BackRun& back, hw_reg_set regs );
extern hw_reg_set       LowReg( BackRun& back, hw_reg_set regs );
extern hw_reg_set       LowOffsetReg( BackRun& back, hw_reg_set regs );
extern hw_reg_set       LowTreePart( BackRun& back, hw_reg_set regs );
extern hw_reg_set       FullReg( hw_reg_set regs );
extern bool             IsRegClass( BackRun& back, hw_reg_set regs, type_class_def type_class );
extern hw_reg_set       ActualParmReg( hw_reg_set reg );
extern hw_reg_set       FixedRegs( BackRun& back );
extern bool             IsStackReg( name *sp );
extern hw_reg_set       StackReg( void );
extern hw_reg_set       DisplayReg( BackRun& back );
extern int              SizeDisplayReg( void );
extern hw_reg_set       AllCacheRegs( BackRun& back );
extern const hw_reg_set *IdxRegs( void );
extern hw_reg_set       ParmRegConflicts(hw_reg_set);
extern hw_reg_set       High48Reg( hw_reg_set regs );
extern hw_reg_set       Low48Reg( hw_reg_set regs );
extern hw_reg_set       High64Reg( hw_reg_set regs );
extern hw_reg_set       Low64Reg( hw_reg_set regs );
extern hw_reg_set       VarargsHomePtr( BackRun& back );
extern hw_reg_set       ReturnAddrReg( void );
extern hw_reg_set       FirstReg( BackRun& back, reg_set_index index );
extern hw_reg_set       SaveRegs( BackRun& back );
extern hw_reg_set       MustSaveRegs( BackRun& back );
extern hw_reg_set       ScratchReg( BackRun& back );

extern hw_reg_set       ReturnReg( BackRun& back, type_class_def type_class, bool use_87 );
