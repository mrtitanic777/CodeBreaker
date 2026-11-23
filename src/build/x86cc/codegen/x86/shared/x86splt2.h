/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2020 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Intel architecture reduce function prototype declarations
*
****************************************************************************/


#pragma once
extern instruction      *rSPLITPUSH( BackRun& back, instruction *ins );
extern instruction      *rMAKEU2( BackRun& back, instruction *ins );
extern instruction      *rLOADLONGADDR( BackRun& back, instruction *ins );
extern instruction      *rHIGHCMP( BackRun& back, instruction *ins );
extern instruction      *rMAKEU4( BackRun& back, instruction *ins );
extern instruction      *rCLRHI_D( BackRun& back, instruction *ins );
extern instruction      *rEXT_PUSH1( BackRun& back, instruction *ins );
extern instruction      *rEXT_PUSH2( BackRun& back, instruction *ins );
extern instruction      *rINTCOMP( BackRun& back, instruction *ins );
extern instruction      *rCDQ( BackRun& back, instruction *ins );
extern instruction      *rCONVERT_UP( BackRun& back, instruction *ins );
extern instruction      *rCYP_SEX( BackRun& back, instruction *ins );
extern instruction      *rSPLIT8( BackRun& back, instruction *ins );
extern instruction      *rSPLIT8BIN( BackRun& back, instruction *ins );
extern instruction      *rSPLIT8NEG( BackRun& back, instruction *ins );
extern instruction      *rSPLIT8TST( BackRun& back, instruction *ins );
extern instruction      *rSPLIT8CMP( BackRun& back, instruction *ins );
extern instruction      *rCYPSHIFT( BackRun& back, instruction *ins );
extern instruction      *rBYTESHIFT( BackRun& back, instruction *ins );
extern instruction      *rMOVE8LOW( BackRun& back, instruction *ins );
