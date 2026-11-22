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
* Description:  fixup related variables and routines
*
****************************************************************************/

#ifndef ASMFIXUP_H
#define ASMFIXUP_H

#include "fppatch.h"


/* the variables of asmfixup.h, one set for each run of the compiler (run/cbrun.hpp) */
struct AsmfixupState {
    asmfixup *InsFixups[OPND_MAX];
};
#include "asminlin.h"
extern asmfixup     *AddFixup( AsmRun& assembler, asm_sym_handle sym, fixup_types fixup_type, fixup_options fixup_option );
extern void         add_frame( void );
extern bool         BackPatch( AsmRun& assembler, asm_sym_handle sym );
extern void         mark_fixupp( AsmRun& assembler, OPNDTYPE determinant, operand_idx index );
extern bool         store_fixup( operand_idx index );

extern bool         AddFPPatchAndFixups( AsmRun& assembler, fp_patches patch );

#endif
