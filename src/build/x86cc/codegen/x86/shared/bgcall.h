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
* Description:  Prototypes for i86call.c
*
****************************************************************************/


#pragma once
extern an               BGCall( BackRun& back, cn call, bool use_return, bool aux_inline );
extern void             BGProcDecl( BackRun& back, cg_sym_handle sym, const type_def *tipe );
extern name             *StReturn( BackRun& back, an retval, const type_def *tipe, instruction **pins );
extern reg_set_index    CallIPossible( instruction *ins );
extern void             InitTargProc( BackRun& back );
extern void             SaveToTargProc( BackRun& back );
extern void             RestoreFromTargProc( BackRun& back );
extern void             PushInSameBlock( BackRun& back, instruction *ins );
extern instruction      *PushOneParm( BackRun& back, instruction *ins, name *curr, type_class_def type_class, type_length offset, call_state *state );
extern void             PreCall( cn call );
extern void             PostCall( cn call );
extern const type_def   *PassParmType( BackRun& back, cg_sym_handle func, const type_def *tipe );
