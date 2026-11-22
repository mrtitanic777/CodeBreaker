/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024      The Open Watcom Contributors. All Rights Reserved.
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
#include "optwif.h"


void    InsertQueue( ins_entry *old_ins, ins_entry *new_ins )
/***********************************************************/
{
  optbegin
    if( optdata_state().FirstIns == NULL ) {
        optdata_state().FirstIns = old_ins;
        optdata_state().LastIns = old_ins;
        optdata_state().FirstIns->ins.prev = NULL;
        optdata_state().FirstIns->ins.next = NULL;
    } else if( new_ins == NULL ) {
        optdata_state().FirstIns->ins.prev = old_ins;
        old_ins->ins.next = optdata_state().FirstIns;
        old_ins->ins.prev = NULL;
        optdata_state().FirstIns = old_ins;
    } else if( new_ins == optdata_state().LastIns ) {
        optdata_state().LastIns->ins.next = old_ins;
        old_ins->ins.next = NULL;
        old_ins->ins.prev = optdata_state().LastIns;
        optdata_state().LastIns = old_ins;
    } else {
        old_ins->ins.next = new_ins->ins.next;
        old_ins->ins.prev = new_ins;
        new_ins->ins.next->ins.prev = old_ins;
        new_ins->ins.next = old_ins;
    }
    ++optdata_state().QCount;
  optend
}


void    DeleteQueue( ins_entry *old_ins )
/***************************************/
{
  optbegin
    if( old_ins == optdata_state().FirstIns ) {
        optdata_state().FirstIns = optdata_state().FirstIns->ins.next;
        if( optdata_state().FirstIns != NULL ) {
            optdata_state().FirstIns->ins.prev = NULL;
        } else {
            optdata_state().LastIns = NULL;
        }
    } else if( old_ins == optdata_state().LastIns ) {
        optdata_state().LastIns = optdata_state().LastIns->ins.prev;
        optdata_state().LastIns->ins.next = NULL;
    } else {
        old_ins->ins.next->ins.prev = old_ins->ins.prev;
        old_ins->ins.prev->ins.next = old_ins->ins.next;
    }
    --optdata_state().QCount;
  optend
}
