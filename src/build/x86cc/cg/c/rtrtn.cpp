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

namespace {

/* the statics of this file, one set for each run of the compiler */
struct RtrtnState {
    import_handle RTHdls[RTSIZE];
    label_handle RTLbls[RTSIZE];
};

}  // namespace

static inline RtrtnState& rtrtn_state() { return cbrun_part<RtrtnState>(); }


rt_class    LookupRoutine( instruction *ins )
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
            _Zoiks( ZOIKS_021 );
            break;
        }
    }
    return( rtindex );
}


void    InitRT( void )
/********************/
{
    rt_class    rtindex;

    for( rtindex = static_cast<rt_class>(0); rtindex < RTSIZE; ++rtindex ) {
        rtrtn_state().RTHdls[rtindex] = NOT_IMPORTED;
        rtrtn_state().RTLbls[rtindex] = NULL;
    }
}


import_handle   AskRTHandle( rt_class rtindex )
/*********************************************/
{
    return( rtrtn_state().RTHdls[rtindex] );
}


void    TellRTHandle( rt_class rtindex, import_handle imphdl )
/************************************************************/
{
    rtrtn_state().RTHdls[rtindex] = imphdl;
}


label_handle RTLabel( rt_class rtindex )
/**************************************/
{
    label_handle    lbl;

    lbl = rtrtn_state().RTLbls[rtindex];
    if( lbl == NULL ) {
        lbl = AskForRTLabel( rtindex );
        rtrtn_state().RTLbls[rtindex] = lbl;
    }
    return( lbl );
}


rt_class    FindRTLabel( label_handle lbl )
/*****************************************/
{
    rt_class    rtindex;

    for( rtindex = static_cast<rt_class>(0); rtindex < RTSIZE; ++rtindex ) {
        if( rtrtn_state().RTLbls[rtindex] == lbl ) {
            break;
        }
    }
    return( rtindex );
}
