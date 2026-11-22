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
* Description:  prototypes for bldins.c
*
****************************************************************************/

#pragma once
extern  an      BGVarargsBasePtr( BackRun& back, const type_def *tipe );
extern  an      BGStackValue( BackRun& back, const type_def *tipe );
extern  an      BGInteger( BackRun& back, int_32 value, const type_def *tipe );
extern  an      BGInt64( BackRun& back, signed_64 value, const type_def *tipe );
extern  an      BGFloat( BackRun& back, const char *value, const type_def *tipe );
extern  an      BGName( BackRun& back, cg_class cl, pointer sym, const type_def *tipe );
extern  an      BGTempName( BackRun& back, name *temp, const type_def *tipe );
extern  bool    FiniLabel( label_handle lbl, block *blk );
extern  void    BGFiniLabel( BackRun& back, label_handle lbl );
extern  bool    NeedConvert( const type_def *from, const type_def *to );
extern  name    *BGNewTemp( BackRun& back, const type_def *tipe );
extern  name    *BGGlobalTemp( BackRun& back, const type_def *tipe );
extern  an      Arithmetic( BackRun& back, an name, const type_def *tipe );
extern  an      BGCompare( BackRun& back, cg_op opcode, an left, an rite, label_handle entry, const type_def *tipe );
extern  an      Boolean( BackRun& back, an node, label_handle entry );
extern  label_handle BGGetEntry( BackRun& back );
extern  void    BG3WayControl( BackRun& back, an node, label_handle lt, label_handle eq, label_handle gt );
extern  void    BGControl( BackRun& back, cg_op opcode, an expr, label_handle lbl );
extern  void    BGGenCtrl( BackRun& back, cg_op opcode, an expr, label_handle lbl, bool gen );
extern  void    BGBigLabel( BackRun& back, back_handle bck );
extern  void    BGBigGoto( BackRun& back, label_handle lbl, level_depth level );
extern  uint_32 BGUnrollCount( BackRun& back, uint_32 unroll_count );
extern  an      BGUnary( BackRun& back, cg_op opcode, an left, const type_def *tipe );
extern  an      BGBinary( BackRun& back, cg_op opcode, an left, an rite, const type_def *tipe, bool fold_addr );
extern  an      BGOpGets( BackRun& back, cg_op opcode, an left, an rite, const type_def *tipe, const type_def *optipe );
extern  an      BGConvert( BackRun& back, an left, const type_def *tipe );
extern  an      BGFlow( BackRun& back, cg_op opcode, an left, an rite );
extern  an      BGAssign( BackRun& back, an dst, an src, const type_def *tipe );
extern  an      BGCopy( BackRun& back, an node );
extern  an      BGDuplicate( BackRun& back, an node );
extern  void    BGDone( BackRun& back, an node );
extern  void    BGTrash( BackRun& back, an node );
extern  void    FlowOff( BackRun& back, an name );
extern  void    BGStartBlock( BackRun& back );

