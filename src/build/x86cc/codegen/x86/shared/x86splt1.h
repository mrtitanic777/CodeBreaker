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
extern instruction      *rFLIPSIGN( BackRun& back, instruction * );
extern instruction      *rTEMP2CONST( BackRun& back, instruction * );
extern instruction      *rSAVEFACE( BackRun& back, instruction * );
extern instruction      *rMULSAVEFACE( BackRun& back, instruction * );
extern instruction      *rDOLONGPUSH( BackRun& back, instruction * );
extern instruction      *rOP1CMEM( BackRun& back, instruction * );
extern instruction      *rOP2CMEM( BackRun& back, instruction * );
extern instruction      *rFSCONSCMP( BackRun& back, instruction * );
extern instruction      *rHIGHLOWMOVE( BackRun& back, instruction * );
extern instruction      *rMAKECYPMUL( BackRun& back, instruction * );
extern instruction      *rMAKESTRCMP( BackRun& back, instruction * );
extern instruction      *rMAKESTRMOVE( BackRun& back, instruction * );
extern instruction      *rMOVELOW( BackRun& back, instruction * );
extern instruction      *rMOVOP1MEM( BackRun& back, instruction * );
extern instruction      *rOP2CL( BackRun& back, instruction * );
extern instruction      *rOP2CX( BackRun& back, instruction * );
extern instruction      *rSPLITUNARY( BackRun& back, instruction * );
extern instruction      *rMULREGISTER( BackRun& back, instruction * );
extern instruction      *rDIVREGISTER( BackRun& back, instruction * );
extern instruction      *rCPSUB( BackRun& back, instruction * );
extern instruction      *rPTSUB( BackRun& back, instruction * );
extern instruction      *rU_TEST( BackRun& back, instruction * );
extern instruction      *rEXTPT( BackRun& back, instruction * );
extern instruction      *rMAYBSTRMOVE( BackRun& back, instruction * );
extern instruction      *rSEG_SEG( BackRun& back, instruction * );
extern instruction      *rCHPPT( BackRun& back, instruction * );
extern instruction      *rMOVRESMEM( BackRun& back, instruction * );
extern instruction      *rMAKEU4CONS( BackRun& back, instruction * );
extern instruction      *rEXT_PUSHC( BackRun& back, instruction * );
extern instruction      *rCMPCP( BackRun& back, instruction * );
extern instruction      *rMOVPTI8( BackRun& back, instruction * );
extern instruction      *rMOVI8PT( BackRun& back, instruction * );
