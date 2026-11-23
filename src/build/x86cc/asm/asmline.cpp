/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  processing input line data and line queueing for macros
*
****************************************************************************/


#include "asmglob.h"
#include <ctype.h>
#include "asminlin.h"
#include "asmrun.hpp"






void AsmByte( AsmRun& assembler, unsigned char byte )
/*********************************
 * Write a byte to the object file
 */
{
    if( assembler.asminlin->AsmCodeAddress <= assembler.asminlin->AsmCodeLimit ) {
        assembler.asminlin->AsmCodeBuffer[assembler.asminlin->AsmCodeAddress++] = byte;
    }
}


void AsmLine( AsmRun& assembler, const char *string, bool use_emu )
/**********************************************/
{
    enum fpe        old_floating_point;
    token_buffer    tokbuf;

    old_floating_point = static_cast<enum fpe>(assembler.asmglob.SWData.fpt);
    if( old_floating_point != NO_FP_ALLOWED ) {
        assembler.asmglob.SWData.fpt = ( use_emu ) ? DO_FP_EMULATION : NO_FP_EMULATION;
    }
    if( !AsmScan( assembler, &tokbuf, string ) ) {
        if( tokbuf.count > 0 ) {
            AsmParse( assembler, &tokbuf, string );
        }
    }
    assembler.asmglob.SWData.fpt = old_floating_point;
}

