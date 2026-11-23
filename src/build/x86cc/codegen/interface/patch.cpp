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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "addrname.h"
#include "tree.h"
#include "cgmem.h"
#include "types.h"
#include "addrfold.h"
#include "makeins.h"
#include "namelist.h"
#include "patch.h"
#include "procdef.h"
#include "makeblk.h"
#include "typemap.h"
#include "backrun.hpp"


patch_handle    BGNewPatch( BackRun& back )
{
    patch_handle    patch;

    patch = static_cast<patch_handle>(CGAlloc( back, sizeof( patch_info ) ));
    patch->in_tree = false;
    patch->patched = false;
    return( patch );
}

an      TNPatch( BackRun& back, tn node )
{
    patch_handle        patch;
    an                  addr;
    type_class_def      type_class;

    patch = (patch_handle)node->u.handle;
    patch->in_tree = false;
    type_class = TypeClass( back, node->tipe );
    addr = AddrName( back, AllocTemp( back, type_class ), node->tipe );
    patch->u.ins = MakeMove( back, NULL, addr->u.n.name, type_class );
    patch->u.ins->num_operands = 0;
    AddIns( back, patch->u.ins );
    return( addr );
}

cg_name BGPatchNode( BackRun& back, patch_handle patch, const type_def *tipe )
{
    patch->patched = true;
    patch->in_tree = true;
    patch->u.node = TGPatch( back, patch, tipe );
    return( patch->u.node );
}

void    BGPatchInteger( BackRun& back, patch_handle patch, int_32 value )
{
    tn                  node;
    name                *c;

    if( patch->patched ) {
        c = AllocS32Const( back, value );
        if( patch->in_tree ) {
            node = patch->u.node;
            node->class_ = TN_CONS;
            node->u.name = c;
        } else {
            patch->u.ins->operands[0] = c;
            patch->u.ins->num_operands = 1;
        }
    }
}

void    BGFiniPatch( BackRun& back, patch_handle patch )
{
    CGFree( back, patch );
}
