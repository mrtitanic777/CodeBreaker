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
* Description:  All architectures common reduce function prototypes
*
****************************************************************************/


#pragma once
extern instruction      *rDOCVT( BackRun& back, instruction * );
extern instruction      *rMOVOP1TEMP( BackRun& back, instruction *ins );
extern instruction      *rMOVOP2TEMP( BackRun& back, instruction *ins );
extern instruction      *rOP1REG( BackRun& back, instruction *ins );
extern instruction      *rOP2REG( BackRun& back, instruction *ins );
extern instruction      *rMOVRESREG( BackRun& back, instruction *ins );
extern instruction      *rMOVRESTEMP( BackRun& back, instruction *ins );
extern instruction      *rRESREG( BackRun& back, instruction *ins );
extern instruction      *rOP1RESREG( BackRun& back, instruction *ins );
extern instruction      *rUSEREGISTER( BackRun& back, instruction *ins );
extern instruction      *rCHANGESHIFT( BackRun& back, instruction *ins );
extern instruction      *rCLRHI_BW( BackRun& back, instruction *ins );
extern instruction      *rCLRHI_R( BackRun& back, instruction *ins );
extern instruction      *rCONVERT_LOW( BackRun& back, instruction *ins );
extern instruction      *rCYPHIGH( BackRun& back, instruction *ins );
extern instruction      *rCYPLOW( BackRun& back, instruction *ins );
extern instruction      *rDOUBLEHALF( BackRun& back, instruction *ins );
extern instruction      *rOP1MEM( BackRun& back, instruction *ins );
extern instruction      *rOP2MEM( BackRun& back, instruction *ins );
extern instruction      *rFORCERESMEM( BackRun& back, instruction *ins );
extern instruction      *rMAKEMOVE( BackRun& back, instruction *ins );
extern instruction      *rMAKEXORRR( BackRun& back, instruction *ins );
extern instruction      *rADDRR( BackRun& back, instruction *ins );
extern instruction      *rMOVOP2( BackRun& back, instruction *ins );
extern instruction      *rMOVOP1RES( BackRun& back, instruction *ins );
extern instruction      *rMOVOP2RES( BackRun& back, instruction *ins );
extern instruction      *rSWAPOPS( BackRun& back, instruction *ins );
extern instruction      *rSWAPCMP( BackRun& back, instruction *ins );
extern instruction      *rMOVEINDEX( BackRun& back, instruction *ins );
extern instruction      *rLOADOP2( BackRun& back, instruction *ins );
extern instruction      *rNEGADD( BackRun& back, instruction *ins );
extern instruction      *rMAKEADD( BackRun& back, instruction *ins );
extern instruction      *rMAKENEG( BackRun& back, instruction *ins );
extern instruction      *rMAKESUB( BackRun& back, instruction *ins );
extern instruction      *rCMPTRUE( BackRun& back, instruction *ins );
extern instruction      *rCMPFALSE( BackRun& back, instruction *ins );
extern instruction      *rOP1RESTEMP( BackRun& back, instruction *ins );
extern instruction      *rFIXSHIFT( BackRun& back, instruction * );
