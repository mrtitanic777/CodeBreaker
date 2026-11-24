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
* Description:  Internal memory management with optional tracking.
*
****************************************************************************/


#include "_cgstd.h"
#include <stdio.h>
#include "memsydep.h"
#include "memcheck.h"
#include "zoiks.h"
#include "spawn.h"
#include "memout.h"
#include "cgmem.h"
#include "optmac.h"
#include "memlimit.h"
#include "onexit.h"
#include "memmgt.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct MemmgtState: memmgt_state.h) */

}  // namespace




void    CGMemInit( BackRun& back, pointer trmemhdl )
/***********************************/
{
    _SysReInit();
    back.memmgt.MemOut = MO_FATAL;
    /* unused parameters */ (void)trmemhdl;
    CalcMemLimit(back);
}

void    CGMemFini( BackRun& back )
/***********************/
{
    MemFini(back);
}


mem_out_action    SetMemOut( BackRun& back, mem_out_action what )
/************************************************/
{
    mem_out_action      old;

    old = back.memmgt.MemOut;
    back.memmgt.MemOut = what;
    return( old );
}

pointer CGAlloc( BackRun& back, size_t size )
/******************************************/
{
    pointer     chunk;

    MemLow( back );
    for( ;; ) {
        chunk = _SysAlloc( back, size );
        if( chunk != NULL ) {
            _AlignmentCheck( chunk, 8 );
            return( chunk );
        }
        if( !MemCheck( back, size ) ) {
            break;
        }
    }
    if( ( back.memmgt.MemOut == MO_FATAL ) || ( back.optmac.InOptimizer != 0 ) ) {
        FatalError( back, "Out of memory" );
    } else if( back.memmgt.MemOut == MO_SUICIDE ) {
        Suicide(back);
    }
    return( NULL );
}


void    CGFree( BackRun& back, pointer chunk )
/*******************************************/
{
    _SysFree( back, chunk );
}


