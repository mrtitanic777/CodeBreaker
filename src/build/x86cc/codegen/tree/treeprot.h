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
* Description:  Prototypes for tree.c
*
****************************************************************************/


#pragma once
extern tn           TGLeaf( BackRun& back, an addr );
extern tn           TGBitMask( BackRun& back, tn left, byte start, byte len, const type_def *tipe );
extern tn           TGNode( BackRun& back, tn_class class_, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern tn           TGWarp( BackRun& back, tn before, label_handle label, tn after );
extern tn           TGHandle( BackRun& back );
extern tn           TGCallback( BackRun& back, cg_callback rtn, callback_handle ptr );
extern tn           TGCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern uint_32      TGMask32( tn node );
extern tn           TGConvert( BackRun& back, tn name, const type_def *tipe );
extern tn           TGBinary( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern tn           TGUnary( BackRun& back, cg_op opcode, tn left, const type_def *tipe );
extern call_handle  TGInitCall( BackRun& back, tn left, const type_def *tipe, cg_sym_handle sym );
extern tn           TGAddParm( BackRun& back, call_handle call, tn parm, const type_def *tipe );
extern tn           TGCall( call_handle call );
extern tn           TGIndex( BackRun& back, tn left, tn rite, const type_def *tipe, const type_def *ptipe );
extern tn           DoTGAssign( BackRun& back, tn dst, tn src, const type_def *tipe, tn_class class_ );
extern tn           TGAssign( BackRun& back, tn dst, tn src, const type_def *tipe );
extern tn           TGLVAssign( BackRun& back, tn dst, tn src, const type_def *tipe );
extern name         *TGetName( BackRun& back, tn node );
extern tn           TGReLeaf( BackRun& back, an addr );
extern tn           TGTmpLeaf( BackRun& back, an addr );
extern tn           TGConst( BackRun& back, float_handle cons, const type_def *tipe );
extern tn           DoTGPreGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe, tn_class class_, tn_class assn_class );
extern tn           TGPreGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern tn           TGLVPreGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern tn           TGPostGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe );
extern cg_type      TGType( tn node );
extern tn           TGFlow( BackRun& back, cg_op opcode, tn left, tn rite );
extern tn           TGTrash( tn node );
extern tn           TGAttr( tn node, cg_sym_attr attr );
extern tn           TGAlign( tn node, uint align );
extern tn           TGVolatile( tn node );
extern void         TG3WayControl( BackRun& back, tn node, label_handle lt, label_handle eq, label_handle gt );
extern void         TGControl( BackRun& back, cg_op opcode, tn node, label_handle lbl );
extern an           TGen( BackRun& back, tn node, const type_def *tipe );
extern an           TGReturn( BackRun& back, tn node, const type_def *tipe );
extern an           TNFlow( BackRun& back, tn node );
extern an           TNPostGets( BackRun& back, tn node );
extern an           TNPreGets( BackRun& back, tn node );
extern an           TNAssign( BackRun& back, tn node );
extern an           TNCompare( BackRun& back, tn node );
extern an           TNUnary( BackRun& back, tn node );
extern an           TNBinary( BackRun& back, tn node );
extern void         TNZapParms( BackRun& back );
extern bool         TGIsAddress( BackRun& back );
extern tn           TGQuestion( BackRun& back, tn sel, tn left, tn rite, const type_def *tipe );
extern void         BurnTree( BackRun& back, tn node );
extern void         TInit( BackRun& back );
extern bool         TreeFrlFree( BackRun& back );
extern void         TFini( BackRun& back );
extern an           TreeGen( BackRun& back, tn node );
extern tn           TGDuplicate( BackRun& back, tn node );
extern bool         TGCanDuplicate( tn node );
extern tn           TName( BackRun& back, name *name, const type_def *tipe );
