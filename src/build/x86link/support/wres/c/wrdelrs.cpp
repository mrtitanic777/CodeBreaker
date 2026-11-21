/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2026      The Open Watcom Contributors. All Rights Reserved.
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

#include "layer0.h"
#include "wres.h"
#include "wresrtns.h"


namespace {

/*
 * delAResource - delete a single resource node and update pointers/counters
 *                This routine assumes that all language nodes for this
 *                resource have already been deleted.
 */
void delAResource( WResDir dir, WResTypeNode *typenode, WResResNode *resnode )
{
    if( resnode->Next != nullptr ) {
        resnode->Next->Prev = resnode->Prev;
    } else {
        typenode->Tail = resnode->Prev;
    }
    if( resnode->Prev != nullptr ) {
        resnode->Prev->Next = resnode->Next;
    } else {
        typenode->Head = resnode->Next;
    }
    typenode->Info.NumResources--;
    dir->NumResources--;
    WRESFREE( resnode );
}

/*
 * delAType - delete a type node.  This routine assumes that all
 *            resource nodes for the type have already been deleted
 */
void delAType( WResDir dir, WResTypeNode *typenode )
{
    if( typenode->Next != nullptr ) {
        typenode->Next->Prev = typenode->Prev;
    } else {
        dir->Tail = typenode->Prev;
    }
    if( typenode->Prev != nullptr ) {
        typenode->Prev->Next = typenode->Next;
    } else {
        dir->Head = typenode->Next;
    }
    dir->NumTypes--;
    WRESFREE( typenode );
}

/*
 * doDelResource - delete a resource and if this leave a type with no
 *                 resources delete the type too. This routine assumes that
 *                 all language nodes for the resource have already been
 *                 deleted
 */
void doDelResource( WResDir dir, WResResNode *resnode, WResTypeNode *typenode )
{
    delAResource( dir, typenode, resnode );
    if( typenode->Info.NumResources == 0 ) {
        delAType( dir, typenode );
    }
}

} // namespace

void WResDelResource( WResDir dir, const WResID *type_id, const WResID *res_id )
{
    WResTypeNode    *typenode = __FindType( type_id, dir );

    if( typenode == nullptr )
        return;

    WResResNode     *resnode = __FindRes( res_id, typenode );

    if( resnode == nullptr )
        return;
    __FreeLangList( resnode );
    doDelResource( dir, resnode, typenode );
}
