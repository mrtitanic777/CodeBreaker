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


void __FreeLangList( WResResNode *resnode )
/*****************************************/
{
    WResLangNode    *nextlangnode;

    for( WResLangNode *langnode = resnode->Head; langnode != nullptr; langnode = nextlangnode ) {
        nextlangnode = langnode->Next;
        WRESFREE( langnode );
    }
    resnode->Head = nullptr;
    resnode->Tail = nullptr;
}

void __FreeResList( WResTypeNode *typenode )
/******************************************/
{
    WResResNode     *nextresnode;

    for( WResResNode *resnode = typenode->Head; resnode != nullptr; resnode = nextresnode ) {
        nextresnode = resnode->Next;
        __FreeLangList( resnode );
        WRESFREE( resnode );
    }
    typenode->Head = nullptr;
    typenode->Tail = nullptr;
}

void __FreeTypeList( WResDir dir )
/********************************/
{
    WResTypeNode    *nexttypenode;

    for( WResTypeNode *typenode = dir->Head; typenode != nullptr; typenode = nexttypenode ) {
        nexttypenode = typenode->Next;
        __FreeResList( typenode );
        WRESFREE( typenode );
    }
    dir->Head = nullptr;
    dir->Tail = nullptr;
}

void WResFreeDir( WResDir dir )
/*****************************/
{
    if( dir != nullptr ) {
        __FreeTypeList( dir );
        WRESFREE( dir );
    }
}
