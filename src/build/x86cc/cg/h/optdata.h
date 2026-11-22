#pragma once
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
* Description:  Optimizer global data declaration
*
****************************************************************************/


#ifndef CB_STATE_OPTDATA_H
#define CB_STATE_OPTDATA_H

/* the variables of optdata.h, one set for each run of the compiler (run/cbrun.hpp) */
struct OptdataState {
    ins_entry *FirstIns;
    ins_entry *LastIns;
    ins_entry *PendingDeletes;
    bool_ InsDelete;
    uint QCount;
    label_handle Handles;
    ins_entry *RetList;
    ins_entry *NoRetList;
    pointer *InstrFrl;
};
inline OptdataState& optdata_state() { return cbrun_part<OptdataState>(); }

#endif
//      pointer         *CodeLblFrl;
#if( OPTIONS & SAVINGS )
extern  uint            Savings;
#endif

