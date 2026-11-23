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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "model.h"
#include "objout.h"
#include "redefby.h"
#include "regset.h"
#include "rgtbl.h"
#include "namelist.h"
#include "feprotos.h"
#include "backrun.hpp"


static bool_maybe   ZapsIndexed( BackRun& back, name *, name * );

bool    TempsOverlap( name *name1, name *name2 )
/***********************************************
 * Do N_TEMP names "name1" and "name2" occupy the same memory?
 */
{
    type_length start_1;
    type_length end_1;
    type_length start_2;
    type_length end_2;

    start_1 = name1->v.offset;
    end_1 = start_1 + name1->n.size - 1;
    start_2 = name2->v.offset;
    end_2 = start_2 + name2->n.size - 1;
    if( start_2 > end_1
      || end_2 < start_1 )
        return( false );
    return( true );
}


static  bool  ZapsMemory( BackRun& back, name *result, name *op, bool for_index )
/*****************************************************************
 * Could redefining "result" redefine N_MEMORY name "op"?
 */
{
    switch( result->n.class_ ) {
    case N_TEMP:
        return( false );
    case N_MEMORY:
        if( result->v.symbol != op->v.symbol )
            return( false );
        /* can be used for memory ops as well */
        if( for_index )
            return( true );
        return( TempsOverlap( result, op ) );
    case N_INDEXED:
        if( result->i.base == NULL ) {
            if( IsModel( back, CGSW_GEN_FORTRAN_ALIASING ) )
                return( false );
            if( op->v.usage & USE_ADDRESS )
                return( true );
            return( IsntModel( back, CGSW_GEN_RELAX_ALIAS ) );
        }
        if( result->i.base->n.class_ == N_TEMP ) {
            return( IsntModel( back, CGSW_GEN_RELAX_ALIAS ) );
        } else { /* it must be N_MEMORY*/
            return( ZapsMemory( back, result->i.base, op, true ) );
        }
    case N_REGISTER:
        return( false );
    default:
        Zoiks( back, ZOIKS_023 );
        return( false );
    }
}


static  bool  ZapsTemp( BackRun& back, name *result, name *op, bool for_index )
/***************************************************************
 * Could redefining "result" redefine N_TEMP name "op"?
 */
{
    switch( result->n.class_ ) {
    case N_TEMP:
        if( DeAlias( result ) != DeAlias( op ) )
            return( false );
        if( for_index )
            return( true );
        return( TempsOverlap( op, result ) );
    case N_MEMORY:
        return( false );
    case N_INDEXED:
        /* might be a structured move with a fake base */
        if( result->i.base != NULL
          && result->i.base->n.class_ == N_TEMP ) {
            if( DeAlias( result->i.base ) == DeAlias( op ) ) {
                return( true );
            }
        }
        if( ( op->v.usage & USE_ADDRESS ) == 0 )
            return( false );
        if( result->i.base == NULL )
            return( true );
        /* bound index*/
        return( false );
    case N_REGISTER:
        {
            hw_reg_set      tmp;

            tmp = DisplayReg(back);
            return( HwOvlap( result->r.reg, tmp ) );
        }
    default:
        Zoiks( back, ZOIKS_023 );
        return( false );
    }
}


static bool_maybe   ZapsTheOp( BackRun& back, name *result, name *op )
/*****************************************************/
{
    if( result == NULL )
        return( MB_FALSE );
    switch( op->n.class_ ) {
    case N_MEMORY:
        if( op->v.usage & VAR_VOLATILE )
            return( MB_TRUE );
        return( ZapsMemory( back, result, op, false ) ? MB_TRUE : MB_FALSE );
    case N_INDEXED:
        if( op->i.index_flags & X_VOLATILE )
            return( MB_TRUE );
        return( ZapsIndexed( back, result, op ) );
    case N_TEMP:
        if( op->v.usage & VAR_VOLATILE )
            return( MB_TRUE );
        return( ZapsTemp( back, result, op, false ) ? MB_TRUE : MB_FALSE );
    case N_REGISTER:
        if( result->n.class_ == N_REGISTER
          && HwOvlap( result->r.reg, op->r.reg ) )
            return( MB_TRUE );
        return( MB_FALSE );
    case N_CONSTANT:
        return( MB_FALSE );
    default:
        Zoiks( back, ZOIKS_023 );
        return( MB_FALSE );
    }
}


static  bool_maybe  ZapsIndexed( BackRun& back, name *result, name *op )
/********************************************************
 * Could redefining "result" redefine N_INDEXED name "op"?
 */
{
    switch( result->n.class_ ) {
    case N_TEMP:
        result = DeAlias( result );
        if( op->i.base == NULL ) { /* free index */
            if( result->v.usage & USE_ADDRESS ) {
                return( MB_TRUE );
            }
        } else if( op->i.base->n.class_ == N_TEMP ) {
            if( result == DeAlias( op->i.base ) ) {
                return( MB_TRUE );
            }
        }
        if( op->i.index->n.class_ != N_TEMP )
            return( MB_FALSE );
        if( result == DeAlias( op->i.index ) )
            return( MB_TRUE );
        return( MB_FALSE );
    case N_MEMORY:
        if( op->i.base == NULL ) {
            if( IsModel( back, CGSW_GEN_FORTRAN_ALIASING ) )
                return( MB_FALSE );
            if( op->v.usage & USE_ADDRESS )
                return( MB_TRUE );
            return( IsModel( back, CGSW_GEN_RELAX_ALIAS ) ? MB_FALSE : MB_TRUE );
        } else if( op->i.base->n.class_ == N_TEMP ) {
            return( IsModel( back, CGSW_GEN_RELAX_ALIAS ) ? MB_FALSE : MB_TRUE );
        } else {
            return( ZapsMemory( back, result, op->i.base, true ) ? MB_TRUE : MB_FALSE );
        }
    case N_INDEXED:
        if( result->i.base == op->i.base
          && result->i.index == op->i.index
          && result->i.index_flags == op->i.index_flags
          && result->i.scale == op->i.scale
          && ( result->i.constant >= op->i.constant + op->n.size
          || op->i.constant >= result->i.constant + result->n.size ) ) {
            return( MB_MAYBE ); // no overlap if index is the same
        }
        if( result->i.base == NULL )
            return( MB_TRUE );
        if( op->i.base == NULL )
            return( MB_TRUE );
        if( result->i.base->n.class_ != op->i.base->n.class_ )
            return( MB_FALSE );
        if( result->i.base->n.class_ == N_TEMP ) {
            return( ZapsTemp( back, result->i.base, op->i.base, true ) ? MB_TRUE : MB_FALSE );
        }
        return( ZapsMemory( back, result->i.base, op->i.base, true ) ? MB_TRUE : MB_FALSE );
    case N_REGISTER:
        if( op->i.base != NULL
          && ZapsTheOp( back, result, op->i.base ) ) {
            return( MB_TRUE );
        }
        return( ZapsTheOp( back, result, op->i.index ) );
    default:
        Zoiks( back, ZOIKS_023 );
        return( MB_FALSE );
    }
}


bool    NameIsConstant( BackRun& back, name *op )
/********************************/
{
    if( op->n.class_ == N_TEMP
      && ( op->v.usage & VAR_CONSTANT ) )
        return( true );
    if( op->n.class_ != N_MEMORY )
        return( false );
    if( op->v.usage & VAR_VOLATILE )
        return( false );
    if( op->v.usage & VAR_CONSTANT )
        return( true );
    if( op->m.memory_type == CG_FE
      && (FEAttr( *back.front, static_cast<cg_sym_handle>(op->v.symbol) ) & FE_CONSTANT) )
        return( true );
    return( AskNameIsROM( back, op->v.symbol, op->m.memory_type ) );
}

bool_maybe  VisibleToCall( BackRun& back, instruction *ins, name *op, bool modifies )
/*********************************************************************
 * Is the operand 'op' visible to the code in invoked by the call 'ins'?
 * The 'modifies' flag means we only care if the routine can modify 'op'.
 */
{
    switch( op->n.class_ ) {
    case N_MEMORY:
        if( modifies
          && (ins->flags.u.call_flags & CALL_WRITES_NO_MEMORY) )
            return( MB_FALSE );
        if( IsModel( back, CGSW_GEN_FORTRAN_ALIASING ) ) {
            switch( op->m.memory_type ) {
            case CG_FE:
                if( (FEAttr( *back.front, static_cast<cg_sym_handle>(op->v.symbol) ) & FE_VISIBLE ) == 0 ) {
                    return( MB_FALSE );
                }
                return( MB_TRUE );
            case CG_BACK:
            case CG_TBL:
            case CG_VTB:
                return( MB_FALSE );
            default:
                return( MB_TRUE );
            }
        }
        return( MB_TRUE );
    case N_INDEXED:
        if( op->i.base == NULL )
            return( MB_TRUE );
        return( ReDefinedBy( back, ins, op->i.base ) );
    case N_TEMP:
        if( op->v.usage & USE_ADDRESS )
            return( MB_TRUE );
        return( MB_FALSE );
    }
    return( MB_FALSE );
}

static  bool    ZappedBySTQ_U( instruction *ins, name *op )
/*********************************************************/
{
    /* unused parameters */ (void)op; (void)ins;
    return( false );
}

bool_maybe  ReDefinedBy( BackRun& back, instruction *ins, name *op )
/****************************************************
 * Is it possible that operand "op" could be redefined by instruction "ins"?
 */
{
    bool_maybe  zaps;

    if( op->n.class_ == N_REGISTER ) {
        if( HwOvlap( ins->zap->reg, op->r.reg ) ) {
            return( MB_TRUE );
        }
    }
    if( NameIsConstant( back, op ) )
        return( MB_FALSE );
    if( ZappedBySTQ_U( ins, op ) )
        return( MB_TRUE );
    zaps = ZapsTheOp( back, ins->result, op );
    if( zaps != MB_FALSE )
        return( zaps );
    if( !OpIsCall( ins->head.opcode ) )
        return( MB_FALSE );
    return( VisibleToCall( back, ins, op, true ) );
}


bool    IsVolatile( name *op )
/*****************************
 * Is "op" volatile in the C sense of the word?
 */
{
    switch( op->n.class_ ) {
    case N_MEMORY:
        if( op->v.usage & VAR_VOLATILE )
            return( true );
        return( false );
    case N_INDEXED:
        if( op->i.index_flags & X_VOLATILE )
            return( true );
        return( false );
    case N_TEMP:
        if( op->v.usage & VAR_VOLATILE )
            return( true );
        return( false );
    default:
        return( false );
    }
}
