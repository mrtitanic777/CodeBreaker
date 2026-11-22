/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2018 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Routine call specific to Intel x86 architecture.
*
****************************************************************************/


#ifndef CB_STATE_X86PROC_H
#define CB_STATE_X86PROC_H

/* the variables of x86proc.h, one set for each run of the compiler (run/cbrun.hpp) */
struct X86procState {
    type_length StackDepth;
};

#endif

extern bool         CanZapBP( BackRun& back );
extern void         DoRTCall( BackRun& back, rt_class rtindex, bool pop );
extern void         AdjustStackDepth( BackRun& back, instruction *ins );
extern void         AdjustStackDepthDirect( BackRun& back, int adjust );
extern bool         BaseIsSP( BackRun& back, name *op );
extern bool         EpilogIsLeaveOnly( BackRun& back );
