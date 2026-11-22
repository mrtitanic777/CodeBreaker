/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Block creation and destruction functions.
*
****************************************************************************/


#pragma once
extern block            *MakeBlockInit( BackRun& back, block_num targets );
extern block            *MakeBlockCopy( BackRun& back, block_num targets, block *src, block_num src_targets );
extern void             FreeABlock( BackRun& back, block *blk );
extern void             FreeBlock( BackRun& back );
extern void             EnLink( BackRun& back, label_handle label );
extern void             AddIns( BackRun& back, instruction *ins );
extern void             GenBlock( BackRun& back, block_class class_, block_num targets );
extern block            *ReGenBlock( BackRun& back, block *blk, label_handle lbl );
extern type_class_def   InitCallState( BackRun& back, const type_def *tipe );
extern type_class_def   CallState( BackRun& back, aux_handle aux, const type_def *tipe, call_state *state );
extern void             AddTarget( BackRun& back, label_handle dest, bool dest_label_dies );
extern block            *FindBlockWithLbl( BackRun& back, label_handle label );
extern void             FixEdges( BackRun& back );
extern bool             FixReturns( BackRun& back );
extern void             UnFixEdges( BackRun& back );
extern void             AddAnIns( BackRun& back, block *blk, instruction *ins );
extern bool             BlkTooBig( BackRun& back );
extern void             NewProc( BackRun& back, level_depth level );
extern void             FreeProc( BackRun& back );
