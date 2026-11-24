/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2017-2017 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Conflict manager functions (selection of best register set
*               for variable).
*
****************************************************************************/


#pragma once
extern conflict_node    *AddConflictNode( BackRun& back, name *opnd );
extern conflict_node    *FindConflictNode( BackRun& back, name *opnd, block *blk, instruction *ins );
extern void             MarkSegment( BackRun& back, instruction *ins, name *opnd );
extern conflict_node    *NameConflict( BackRun& back, instruction *ins, name *opnd );
extern reg_set_index    GetPossibleForTemp(conflict_node *conf, name *temp);
extern void             MarkPossible( BackRun& back, instruction *ins, name *opnd, reg_set_index idx );
extern reg_set_index    MarkIndex( BackRun& back, instruction *ins, name *opnd, bool is_temp_index );
extern void             FreePossibleForAlias( BackRun& back, conflict_node *conf );
extern void             FreeConflicts( BackRun& back );
extern void             FreeAConflict( BackRun& back, conflict_node *conf );
extern void             InitConflict( BackRun& back );
extern bool             ConfFrlFree( BackRun& back );
