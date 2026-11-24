/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023-2024 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Prototypes for patch.c; the DeadBeef stuff seems to be
*               truely dead beef.
*
****************************************************************************/


#pragma once

typedef struct patch_info {
    union {
        tn              node;
        instruction     *ins;
    } u;
    bool        in_tree;        // are we hanging in a tree somewhere?
    bool        patched;        // has someone made a node out of us yet?
} patch_info;

extern  patch_handle    BGNewPatch( BackRun& back );
extern  cg_name         BGPatchNode( BackRun& back, patch_handle patch, const type_def *tipe );
extern  void            BGPatchInteger( BackRun& back, patch_handle patch, int_32 value );
extern  void            BGFiniPatch( BackRun& back, patch_handle patch );
extern  an              TNPatch( BackRun& back, tn node );
extern  tn              TGPatch( BackRun& back, patch_handle patch, const type_def *tipe );
