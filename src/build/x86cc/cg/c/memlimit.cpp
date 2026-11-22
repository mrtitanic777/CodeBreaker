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


namespace {

/* the statics of this file, one set for each run of the compiler */
struct MemlimitState {
    pointer_uint MemLimit;
    bool_ IckyWicky;
};

}  // namespace

static inline MemlimitState& memlimit_state() { return cbrun_part<MemlimitState>(); }


static  bool_    FlushSomeOpt( pointer_uint size )
/***********************************************/
{
    bool_        freed;

    if( optmac_state().InOptimizer == 0 && HaveCodeSeg() ) {
        PUSH_OP( AskCodeSeg() );
            freed = ShrinkQueue( size );
        POP_OP();
        if( _IsntModel( CGSW_GEN_NO_OPTIMIZATION ) && !memlimit_state().IckyWicky ) {
            memlimit_state().IckyWicky = true;
            FEMessage( FEMSG_PEEPHOLE_FLUSHED, NULL );
        }
    } else {
        freed = false;
    }
    return( freed );
}


static  bool_    ChkMemLimit( pointer_uint limit )
/***********************************************/
{
    if( _IsModel( CGSW_GEN_MEMORY_LOW_FAILS ) )
        return( false );
    if( MemInUse() - data_state().FrlSize <= limit )
        return( false );
    FlushSomeOpt( MemInUse() - limit - data_state().FrlSize );
    if( MemInUse() - data_state().FrlSize <= limit )
        return( false );
    return( true );
}


void    CalcMemLimit( void )
/**************************/
{
    pointer_uint    size;

    size = MemSize();
    memlimit_state().MemLimit = size - size / 4;
    memlimit_state().IckyWicky = false;
}


void    FlushOpt( void )
/**********************/
{
    FlushSomeOpt( (pointer_uint)-1 );
}


bool_    MemCritical( void )
/*************************/
{
    return( ChkMemLimit( memlimit_state().MemLimit - memlimit_state().MemLimit/4 ) );
}


bool_    MemLow( void )
/********************/
{
    return( ChkMemLimit( memlimit_state().MemLimit ) );
}


void    BlowAwayFreeLists( void )
/*******************************/
{
    AddrFrlFree();
    ScoreFrlFree();
    TreeFrlFree();
    RegTreeFrlFree();
    InsFrlFree();
    InstrFrlFree();
    CFFrlFree( &cfloat_state().cgh );
    ConfFrlFree();
    NameFrlFree();
    SchedFrlFree();
}


bool_    MemCheck( size_t size )
/*****************************/
{
    if( FEMoreMem( size ) )
        return( true );
    if( FreeObjCache() )
        return( true );
    if( ScoreFrlFree() )
        return( true );
    if( AddrFrlFree() )
        return( true );
    if( RegTreeFrlFree() )
        return( true );
    if( CFFrlFree( &cfloat_state().cgh ) )
        return( true );
    if( InsFrlFree() )
        return( true );
    if( TreeFrlFree() )
        return( true );
    if( ConfFrlFree() )
        return( true );
    if( NameFrlFree() )
        return( true );
    if( InstrFrlFree() )
        return( true );
    if( SchedFrlFree() )
        return( true );
    if( FlushSomeOpt( size ) )
        return( true );
    return( false );
}
