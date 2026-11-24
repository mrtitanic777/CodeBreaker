/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Abnormal termination processing.
*
****************************************************************************/


#include "_cgstd.h"
#include <stdlib.h>
#include <signal.h>
#include "cgmisc.h"
#include "coderep.h"
#include "objio.h"
#include "onexit.h"
#include "qtimer.h"
#include "feprotos.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct OnexitState: onexit_state.h) */

}  // namespace



/* A signal handler is called with nothing but the signal and so cannot name a run: the interrupt it notes belongs to the process (the
 * one variable of the compiler that does), and the run that asks for a break (TBreak) takes it. */
static volatile sig_atomic_t Interrupted;

static void SigIntFunc( int sig_num )
/***********************************/
{
    /* unused parameters */ (void)sig_num;

    Interrupted = 1;
}


static void BrkInit( void )
/******************/
{
    signal( SIGINT, SigIntFunc );
}


static void BrkFini( BackRun& back )
/******************/
{
}


static void InitOnExit( BackRun& back )
/*********************/
{
    if( back.onexit.OnExitFlag ) {
        BrkInit();
        GrabTimer();
        back.onexit.OnExitFlag = false;
    }
}


bool TBreak( BackRun& back )
/*****************/
{
    int         brk;

    InitOnExit(back);
    if( Interrupted ) {
        Interrupted = 0;
        back.onexit.BrkFlag = true;
    }
    brk = back.onexit.BrkFlag;
    back.onexit.BrkFlag = 0;
    return( brk != 0 );
}


void CauseTBreak( BackRun& back )
/**********************/
{
    back.onexit.BrkFlag = true;
}


void FatalError( BackRun& back, const char *str )
/********************************/
{
     ReleTimer(back);
     BrkFini(back);
     ScratchObj(back);
     FEMessage( *back.front, FEMSG_FATAL, const_cast<pointer>(static_cast<const void *>(str)) );
}
