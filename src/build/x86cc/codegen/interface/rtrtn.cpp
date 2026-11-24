/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024-2025 The Open Watcom Contributors. All Rights Reserved.
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
#include "zoiks.h"
#include "data.h"
#include "rtrtn.h"
#include "backrun.hpp"

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct RtrtnState: rtrtn_state.h) */

}  // namespace



rt_class    LookupRoutine( BackRun& back, instruction *ins )
/*******************************************/
{
    const rtn_info  *rtn;
    rt_class        rtindex;
    opcode_defs     opcode;

    opcode = ins->head.opcode;
    if( opcode >= FIRST_CONDITION ) {
        opcode = OP_CMP;
    }
    rtn = RTInfo;
    rtindex = static_cast<rt_class>(0);
    while( rtn->opcode != opcode || rtn->operand_class != ins->type_class ) {
        ++rtn;
        ++rtindex;
        if( rtn->opcode == OP_NOP ) {
            Zoiks( back, ZOIKS_021 );
            break;
        }
    }
    return( rtindex );
}


void    InitRT( BackRun& back )
/********************/
{
    rt_class    rtindex;

    for( rtindex = static_cast<rt_class>(0); rtindex < RTSIZE; ++rtindex ) {
        back.rtrtn.RTHdls[rtindex] = NOT_IMPORTED;
        back.rtrtn.RTLbls[rtindex] = NULL;
    }
}


import_handle   AskRTHandle( BackRun& back, rt_class rtindex )
/*********************************************/
{
    return( back.rtrtn.RTHdls[rtindex] );
}


void    TellRTHandle( BackRun& back, rt_class rtindex, import_handle imphdl )
/************************************************************/
{
    back.rtrtn.RTHdls[rtindex] = imphdl;
}


label_handle RTLabel( BackRun& back, rt_class rtindex )
/**************************************/
{
    label_handle    lbl;

    lbl = back.rtrtn.RTLbls[rtindex];
    if( lbl == NULL ) {
        lbl = AskForRTLabel( back, rtindex );
        back.rtrtn.RTLbls[rtindex] = lbl;
    }
    return( lbl );
}


rt_class    FindRTLabel( BackRun& back, label_handle lbl )
/*****************************************/
{
    rt_class    rtindex;

    for( rtindex = static_cast<rt_class>(0); rtindex < RTSIZE; ++rtindex ) {
        if( back.rtrtn.RTLbls[rtindex] == lbl ) {
            break;
        }
    }
    return( rtindex );
}
