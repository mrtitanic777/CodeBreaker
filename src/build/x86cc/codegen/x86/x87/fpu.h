/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2017-2018 The Open Watcom Contributors. All Rights Reserved.
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
**  ========================================================================
*
* Description:  Prototypes specific to x87 FPU regsiter allocation. These
*               functions are called from generic code and must be stubbed
*               for non-x86 platforms.
*
****************************************************************************/


#pragma once
extern void             FPSetStack( name * );
extern void             FPNotStack( name * );
extern void             FPPushParms( BackRun& back, pn, call_state * );
extern bool             FPDivIsADog( BackRun& back, type_class_def type_class );
extern bool             FPStackOp( BackRun& back, name * );
extern bool             FPIsConvert( BackRun& back, instruction * );
extern type_class_def   FPInsClass( BackRun& back, instruction * );
extern bool             FPIsStack( name * );
extern void             FPInit( BackRun& back );
extern void             FPExpand( BackRun& back );
extern void             FPOptimize( BackRun& back );
extern void             FPRegAlloc( BackRun& back );
extern void             FPParms( BackRun& back );
extern bool             FPSideEffect( BackRun& back, instruction * );
extern bool             FPStackIns( BackRun& back, instruction * );
extern bool             FPStackReg( BackRun& back, name * );
extern bool             FPInsIntroduced( instruction * );
extern bool             FPFreeIns( BackRun& back, instruction * );
extern int              FPStackExit( BackRun& back, block * );
extern int              FPStkOver( BackRun& back, instruction *, int );
extern void             FPCalcStk( BackRun& back, instruction *, int * );
extern void             FPPostSched( BackRun& back, block * );
extern void             FPPreSched( BackRun& back, block * );
