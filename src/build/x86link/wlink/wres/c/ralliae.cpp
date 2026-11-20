/****************************************************************************
*
*                            Open Watcom Project
*
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

#include <stdlib.h>
#include "mem2.h"


namespace {

/* every list item starts with these two links */
struct LLItem {
    LLItem  *next;
    LLItem  *prev;
};

} // namespace

/*
 * ResAddLLItemAtEnd - create a new item at the tail of a linked list
 */
void ResAddLLItemAtEnd( void **head, void **tail, void *item )
{
    auto **headptr = reinterpret_cast<LLItem **>( head );
    auto **tailptr = reinterpret_cast<LLItem **>( tail );
    auto *itemptr = static_cast<LLItem *>( item );

    if( *headptr == nullptr ) {
        *headptr = *tailptr = itemptr;
        itemptr->next = nullptr;
        itemptr->prev = nullptr;
    } else {
        itemptr->prev = *tailptr;
        itemptr->next = nullptr;
        (*tailptr)->next = itemptr;
        *tailptr = itemptr;
    }
}
