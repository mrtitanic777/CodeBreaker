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
#include "backrun.hpp"


void    InsertQueue( BackRun& back, ins_entry *old_ins, ins_entry *new_ins )
/***********************************************************/
{
  OptBegin( back );
    if( back.optdata.FirstIns == NULL ) {
        back.optdata.FirstIns = old_ins;
        back.optdata.LastIns = old_ins;
        back.optdata.FirstIns->ins.prev = NULL;
        back.optdata.FirstIns->ins.next = NULL;
    } else if( new_ins == NULL ) {
        back.optdata.FirstIns->ins.prev = old_ins;
        old_ins->ins.next = back.optdata.FirstIns;
        old_ins->ins.prev = NULL;
        back.optdata.FirstIns = old_ins;
    } else if( new_ins == back.optdata.LastIns ) {
        back.optdata.LastIns->ins.next = old_ins;
        old_ins->ins.next = NULL;
        old_ins->ins.prev = back.optdata.LastIns;
        back.optdata.LastIns = old_ins;
    } else {
        old_ins->ins.next = new_ins->ins.next;
        old_ins->ins.prev = new_ins;
        new_ins->ins.next->ins.prev = old_ins;
        new_ins->ins.next = old_ins;
    }
    ++back.optdata.QCount;
  OptEnd( back );
}


void    DeleteQueue( BackRun& back, ins_entry *old_ins )
/***************************************/
{
  OptBegin( back );
    if( old_ins == back.optdata.FirstIns ) {
        back.optdata.FirstIns = back.optdata.FirstIns->ins.next;
        if( back.optdata.FirstIns != NULL ) {
            back.optdata.FirstIns->ins.prev = NULL;
        } else {
            back.optdata.LastIns = NULL;
        }
    } else if( old_ins == back.optdata.LastIns ) {
        back.optdata.LastIns = back.optdata.LastIns->ins.prev;
        back.optdata.LastIns->ins.next = NULL;
    } else {
        old_ins->ins.next->ins.prev = old_ins->ins.prev;
        old_ins->ins.prev->ins.next = old_ins->ins.next;
    }
    --back.optdata.QCount;
  OptEnd( back );
}
