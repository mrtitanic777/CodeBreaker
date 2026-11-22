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
* Description:  Prototypes for bldcall.c
*
****************************************************************************/


#pragma once
extern  type_class_def  AddCallBlock( BackRun& back, cg_sym_handle sym, const type_def *tipe );
extern  void            BGFiniCall( BackRun& back, cn call );
extern  cn              BGInitCall( BackRun& back, an node, const type_def *tipe, aux_handle aux );
extern  void            BGAddParm( BackRun& back, cn call, an parm );
extern  void            BGAutoDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe );
extern  name            *DoParmDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe, hw_reg_set reg );
extern  void            BGParmDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe );
extern  void            AddCallIns( BackRun& back, instruction *ins, cn call );
extern  void            ReverseParmNodeList( pn *owner );
extern  void            PushParms( BackRun& back, pn parm, call_state *state );
extern  void            ReserveStack( BackRun& back, call_state *state, instruction *prev, type_length len );
extern  void            ParmIns( BackRun& back, pn parm, call_state *state );
extern  void            BGZapBase( BackRun& back, name *base, const type_def *tipe );
extern  void            BGReturn( BackRun& back, an retval, const type_def *tipe );
extern  bool            AssgnParms( BackRun& back, cn call, bool aux_inline );
extern  hw_reg_set      CallZap( BackRun& back, call_state *state );
extern  void            UpdateReturn( BackRun& back, call_state *, const type_def *, type_class_def, aux_handle );
