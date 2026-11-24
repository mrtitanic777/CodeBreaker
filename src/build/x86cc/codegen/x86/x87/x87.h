/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2017-2023 The Open Watcom Contributors. All Rights Reserved.
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
#include "i87data.h"
#include "fppatch.h"

inline bool Is87Ins( instruction *ins ) { return ( IsFloating( (ins)->type_class ) || IsFloating( (ins)->base_type_class ) ); }


/* i87exp.c*/
extern instruction      *PrefFLDOp( BackRun& back, instruction *ins, operand_type op, name *opnd );
extern bool             FPResultNotNeeded( instruction *ins );
extern instruction      *SuffFSTPRes( BackRun& back, instruction *ins, name *opnd, result_type res );
extern instruction      *SuffFXCH( BackRun& back, instruction *ins, int i );
extern instruction      *PrefFXCH( BackRun& back, instruction *ins, int i );
extern void             NoPopRBin( instruction *ins );
extern void             NoPopBin( instruction *ins );
extern void             ToPopBin( instruction *ins );
extern void             ReverseFPGen( instruction *ins );
extern void             ToRFld( instruction *ins );
extern void             ToRFstp( instruction *ins );
extern void             NoPop( instruction *ins );
extern void             NoMemBin( instruction *ins );
extern instruction      *MakeWait( BackRun& back );

/* i87opt.c */

/* i87reg.c */
extern void             SetFPParmsUsed( call_state *state, int parms );
extern name             *ST( BackRun& back, int i );
extern void             FPInitStkReq( BackRun& back );
extern int              FPStkReq( BackRun& back, instruction *ins );
extern void             SetFPPatchSegm( BackRun& back, int i );
extern void             SetFPPatchType( BackRun& back, fp_patches type );
extern void             FPPatchTypeRef( BackRun& back );

/* i87sched.c */
extern bool             FPInsIntroduced( instruction * );

/* i87wait.c */
extern void             Wait8087( BackRun& back );
