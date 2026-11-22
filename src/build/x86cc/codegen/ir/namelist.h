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
* Description:  Prototypes for namelist.c
*
****************************************************************************/


#pragma once
extern name             *AllocConst( BackRun& back, float_handle value );
extern name             *AllocAddrConst( BackRun& back, name *value, int seg, constant_type_class const_type, type_class_def type_class );
extern name             *FindIntValue( BackRun& back, int_32 value );
extern name             *AllocIntConst( BackRun& back, int value );
extern name             *AllocUIntConst( BackRun& back, uint value );
extern name             *AllocS32Const( BackRun& back, int_32 value );
extern name             *AllocU32Const( BackRun& back, uint_32 value );
extern name             *AllocS64Const( BackRun& back, uint_32 low, uint_32 high );
extern name             *AllocU64Const( BackRun& back, uint_32 low, uint_32 high );
extern constant_defn    *GetFloat( BackRun& back, name *cons, type_class_def type_class );
extern name             *SAllocMemory( BackRun& back, pointer symbol, type_length offset, cg_class class_, type_class_def type_class, type_length size );
extern name             *AllocMemory( BackRun& back, pointer symbol, type_length offset, cg_class class_, type_class_def type_class );
extern name             *STempOffset( BackRun& back, name *temp, type_length offset, type_class_def type_class, type_length size );
extern name             *SAllocTemp( BackRun& back, type_class_def type_class, type_length size );
extern name             *AllocTemp( BackRun& back, type_class_def type_class );
extern name             *TempOffset( BackRun& back, name *temp, type_length offset, type_class_def type_class );
extern name             *SAllocUserTemp( BackRun& back, pointer symbol, type_class_def type_class, type_length size );
extern name             *AllocUserTemp( BackRun& back, pointer symbol, type_class_def type_class );
extern name             *DeAlias( name *temp );
extern name             *AllocRegName( BackRun& back, hw_reg_set regs );
extern name             *ScaleIndex( BackRun& back, name *index, name *base, type_length offset, type_class_def type_class, type_length size, scale_typ scale, i_flags flags );
extern name             *SAllocIndex( BackRun& back, name *index, name *base, type_length offset, type_class_def type_class, type_length size );
extern name             *AllocIndex( BackRun& back, name *index, name *base, type_length offset, type_class_def type_class );
extern void             InitNames( BackRun& back );
extern void             ReInitNames( BackRun& back );
extern void             FreeNames( BackRun& back );
extern void             FreeAName( BackRun& back, name *op );
extern bool             NameFrlFree( BackRun& back );
extern i_flags          AlignmentToFlags( BackRun& back, type_length alignment );
extern type_length      FlagsToAlignment( i_flags flags );
