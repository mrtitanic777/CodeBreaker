/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024      The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Prototypes for makeaddr.c
*
****************************************************************************/


#pragma once
extern  an      NewAddrName( BackRun& back );
extern  an      NewBoolNode( BackRun& back );
extern  an      MakeTypeTempAddr( BackRun& back, name *op, const type_def *tipe );
extern  an      MakeTempAddr( BackRun& back, name *op );
extern  void    InitMakeAddr( BackRun& back );
extern  name    *GenIns( BackRun& back, an addr );
extern  void    NamesCrossBlocks( BackRun& back );
extern  bool    AddrFrlFree( BackRun& back );
extern  void    AddrFree( BackRun& back, an node );
extern  an      MakeGets( BackRun& back, an dst, an src, const type_def *tipe );
extern  an      MakeConst( BackRun& back, float_handle cf, const type_def *tipe );
extern  an      MakePoints( BackRun& back, an name, const type_def *tipe );
extern  an      RegName( BackRun& back, hw_reg_set reg, const type_def *tipe );
extern  an      InsName( BackRun& back, instruction *ins, const type_def *tipe );
extern  name    *LoadTemp( BackRun& back, name *temp, type_class_def type_class );
extern  an      AddrEval( BackRun& back, an addr );
extern  void    MoveAddress( an src, an dest );
extern  void    Convert( BackRun& back, an addr, type_class_def type_class );
extern  bool    PointLess( an l_addr, an r_addr );
extern  an      AddrToIns( BackRun& back, an addr );
extern  void    InsToAddr( BackRun& back, an addr );
extern  an      AddrDuplicate( BackRun& back, an node );
extern  an      AddrCopy( BackRun& back, an node );
extern  an      AddrSave( BackRun& back, an node );
extern  void    AddrDemote( an node );
extern  name    *MaybeTemp( BackRun& back, name *op, type_class_def type_class );
extern  void    CheckPointer( BackRun& back, an addr );
extern  void    FixCodePtr( an addr );
extern  bool    NeedPtrConvert( an addr, const type_def *tipe );
extern  name    *LoadAddress( BackRun& back, name *op, name *suggest, const type_def *type_ptr );
extern  an      MakeAddrName( BackRun& back, cg_class class_, cg_sym_handle sym, const type_def *tipe );
extern  cg_type NamePtrType(BackRun& back, name *op);
