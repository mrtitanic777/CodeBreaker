/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Routines to deal with low memory conditions.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "data.h"
#include "objout.h"
#include "memcheck.h"
#include "memlimit.h"
#include "optmain.h"
#include "namelist.h"
#include "optmkins.h"
#include "makeins.h"
#include "conflict.h"
#include "inssched.h"
#include "memsydep.h"
#include "regtree.h"
#include "score.h"
#include "makeaddr.h"
#include "tree.h"
#include "treeprot.h"
#include "feprotos.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct MemlimitState: memlimit_state.h) */

}  // namespace



static  bool    FlushSomeOpt( BackRun& back, pointer_uint size )
/***********************************************/
{
    bool        freed;

    if( back.optmac.InOptimizer == 0 && HaveCodeSeg(back) ) {
        PUSH_OP( AskCodeSeg(back) );
            freed = ShrinkQueue( back, size );
        POP_OP();
        if( _IsntModel( CGSW_GEN_NO_OPTIMIZATION ) && !back.memlimit.IckyWicky ) {
            back.memlimit.IckyWicky = true;
            FEMessage( *back.front, FEMSG_PEEPHOLE_FLUSHED, NULL );
        }
    } else {
        freed = false;
    }
    return( freed );
}


static  bool    ChkMemLimit( BackRun& back, pointer_uint limit )
/***********************************************/
{
    if( _IsModel( CGSW_GEN_MEMORY_LOW_FAILS ) )
        return( false );
    if( MemInUse(back) - back.data.FrlSize <= limit )
        return( false );
    FlushSomeOpt( back, MemInUse(back) - limit - back.data.FrlSize );
    if( MemInUse(back) - back.data.FrlSize <= limit )
        return( false );
    return( true );
}


void    CalcMemLimit( BackRun& back )
/**************************/
{
    pointer_uint    size;

    size = MemSize(back);
    back.memlimit.MemLimit = size - size / 4;
    back.memlimit.IckyWicky = false;
}


void    FlushOpt( BackRun& back )
/**********************/
{
    FlushSomeOpt( back, (pointer_uint)-1 );
}


bool    MemCritical( BackRun& back )
/*************************/
{
    return( ChkMemLimit( back, back.memlimit.MemLimit - back.memlimit.MemLimit/4 ) );
}


bool    MemLow( BackRun& back )
/********************/
{
    return( ChkMemLimit( back, back.memlimit.MemLimit ) );
}


void    BlowAwayFreeLists( BackRun& back )
/*******************************/
{
    AddrFrlFree(back);
    ScoreFrlFree(back);
    TreeFrlFree(back);
    RegTreeFrlFree(back);
    InsFrlFree(back);
    InstrFrlFree(back);
    CFFrlFree( back, &back.cfloat.cgh );
    ConfFrlFree(back);
    NameFrlFree(back);
    SchedFrlFree(back);
}


bool    MemCheck( BackRun& back, size_t size )
/*****************************/
{
    if( FEMoreMem( size ) )
        return( true );
    if( FreeObjCache(back) )
        return( true );
    if( ScoreFrlFree(back) )
        return( true );
    if( AddrFrlFree(back) )
        return( true );
    if( RegTreeFrlFree(back) )
        return( true );
    if( CFFrlFree( back, &back.cfloat.cgh ) )
        return( true );
    if( InsFrlFree(back) )
        return( true );
    if( TreeFrlFree(back) )
        return( true );
    if( ConfFrlFree(back) )
        return( true );
    if( NameFrlFree(back) )
        return( true );
    if( InstrFrlFree(back) )
        return( true );
    if( SchedFrlFree(back) )
        return( true );
    if( FlushSomeOpt( back, size ) )
        return( true );
    return( false );
}
