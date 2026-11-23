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
* Description:  Interface to Optimizing code generator for DATA
*
****************************************************************************/


#include "cvars.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "cgen.h"
#include "frontrun.hpp"

#define ZEROS_BLOCK_SIZE    (8 * 1024)

void EmitZeros( FrontRun& front, target_size amount )
{
    for( ; amount > ZEROS_BLOCK_SIZE; amount -= ZEROS_BLOCK_SIZE ) {
        DGIBytes( *front.back, ZEROS_BLOCK_SIZE, 0 );
    }
    DGIBytes( *front.back, amount, 0 );
}

void AlignIt( FrontRun& front, TYPEPTR typ )
{
    if( front.cvars.OptSize == 0 ) {        /* optimize for time */
        DGAlign( *front.back, GetTypeAlignment( front, typ ) );
    }
}

target_size GetDQuadPointerSize( FrontRun& front, enum quad_flags dq_flags )
{
    target_size         size;

    size = TARGET_POINTER;
    if( dq_flags & Q_SEGMENT ) {                /* CodeBreaker: a selector */
        size = TARGET_SHORT;
    } else if( dq_flags & Q_NEAR_POINTER ) {
        size = TARGET_NEAR_POINTER;
    } else if( dq_flags & Q_FAR_POINTER ) {
        size = TARGET_FAR_POINTER;
    } else if( dq_flags & Q_CODE_POINTER ) {
        if( front.cvars.TargetSwitches & CGSW_X86_BIG_CODE ) {
            size = TARGET_FAR_POINTER;
        }
    } else {
        if( front.cvars.TargetSwitches & CGSW_X86_BIG_DATA ) {
            size = TARGET_FAR_POINTER;
        }
    }
    return( size );
}

static cg_type GetDQuadPointerCGType( enum quad_flags dq_flags )
{
    cg_type             cgtype;

    if( dq_flags & Q_NEAR_POINTER ) {
        cgtype = TY_NEAR_POINTER;
    } else if( dq_flags & Q_FAR_POINTER ) {
        cgtype = TY_LONG_POINTER;
    } else if( dq_flags & Q_CODE_POINTER ) {
        cgtype = TY_CODE_PTR;
    } else {
        cgtype = TY_POINTER;
    }
    return( cgtype );
}
namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CgendataState: cgendata_state.h) */

}  // namespace



static void EmitDQuad( FrontRun& front, DATA_QUAD *dq )
{
    cg_type             data_type;
    target_size         amount;
    SYM_ENTRY           sym;


#if _CPU == 8086
    if( front.cgendata.EmitDQuad_size >= 0x10000 ) {
        if( front.cgendata.EmitDQuad_segid != SEG_CONST && front.cgendata.EmitDQuad_segid != SEG_DATA ) {
            ++front.cgendata.EmitDQuad_segid;
            BESetSeg( *front.back, front.cgendata.EmitDQuad_segid );
            front.cgendata.EmitDQuad_size -= 0x10000;
        }
    }
#endif
    switch( dq->type ) {
    case QDT_STATIC:
        SymGet( front, &sym, dq->u.var.sym_handle );
        front.cgendata.EmitDQuad_segid = sym.u.var.segid;
        BESetSeg( *front.back, front.cgendata.EmitDQuad_segid );
        AlignIt( front, sym.sym_type );
        DGLabel( *front.back, FEBack( front, static_cast<CGSYM_HANDLE>(dq->u.var.sym_handle) ) );
        front.cgendata.EmitDQuad_size = 0;
        break;
    case QDT_CHAR:
    case QDT_UCHAR:
    case QDT_BOOL:
        DGInteger( *front.back, dq->u_long_value1, TY_UINT_1 );
        front.cgendata.EmitDQuad_size += sizeof( char );
        if( dq->flags & Q_2_INTS_IN_ONE ) {
            DGInteger( *front.back, dq->u_long_value2, TY_UINT_1 );
            front.cgendata.EmitDQuad_size += sizeof( char );
        }
        break;
    case QDT_SHORT:
    case QDT_USHORT:
        DGInteger( *front.back, dq->u_long_value1, TY_UINT_2 );
        front.cgendata.EmitDQuad_size += TARGET_SHORT;
        if( dq->flags & Q_2_INTS_IN_ONE ) {
            DGInteger( *front.back, dq->u_long_value2, TY_UINT_2 );
            front.cgendata.EmitDQuad_size += TARGET_SHORT;
        }
        break;
    case QDT_INT:
    case QDT_UINT:
        DGInteger( *front.back, dq->u_long_value1, TY_INTEGER );
        front.cgendata.EmitDQuad_size += TARGET_INT;
        if( dq->flags & Q_2_INTS_IN_ONE ) {
            DGInteger( *front.back, dq->u_long_value2, TY_INTEGER );
            front.cgendata.EmitDQuad_size += TARGET_INT;
        }
        break;
    case QDT_LONG:
    case QDT_ULONG:
        DGInteger( *front.back, dq->u_long_value1, TY_UINT_4 );
        front.cgendata.EmitDQuad_size += TARGET_LONG;
        if( dq->flags & Q_2_INTS_IN_ONE ) {
            DGInteger( *front.back, dq->u_long_value2, TY_UINT_4 );
            front.cgendata.EmitDQuad_size += TARGET_LONG;
        }
        break;
    case QDT_LONG64:
    case QDT_ULONG64:
        DGInteger64( *front.back, dq->u.long64, TY_UINT_8 );
        front.cgendata.EmitDQuad_size += TARGET_LONG64;
        break;

    case QDT_FLOAT:
    case QDT_FIMAGINARY:
//        ftoa( dq->u.double_value, Buffer );
//        DGFloat( Buffer, TY_SINGLE );
        {
            DATA_QUAD   local_dq;
            float       float_value;

            /*
             * dq->u.double_value may not have proper alignment on Alpha
             * so copy pieces to local copy on stack which will have
             * proper alignment
             */
            local_dq.u_long_value1 = dq->u_long_value1;
            local_dq.u_long_value2 = dq->u_long_value2;
            float_value = static_cast<float>(local_dq.u.double_value);
            DGBytes( *front.back, TARGET_FLOAT, reinterpret_cast<char *>(&float_value) );
        }
        front.cgendata.EmitDQuad_size += TARGET_FLOAT;
        break;
    case QDT_DOUBLE:
    case QDT_DIMAGINARY:
//        ftoa( dq->u.double_value, Buffer );
//        DGFloat( Buffer, TY_DOUBLE );
        DGBytes( *front.back, TARGET_DOUBLE, reinterpret_cast<char *>(&dq->u.double_value) );
        front.cgendata.EmitDQuad_size += TARGET_DOUBLE;
        break;
    case QDT_LONG_DOUBLE:
    case QDT_LDIMAGINARY:
        DGBytes( *front.back, TARGET_LDOUBLE, reinterpret_cast<char *>(&dq->u.long_double_value) );
        front.cgendata.EmitDQuad_size += TARGET_LDOUBLE;
        break;
    case QDT_STRING:
        data_type = GetDQuadPointerCGType( dq->flags );
        EmitStrPtr( front, dq->u.string_leaf, data_type );
        front.cgendata.EmitDQuad_size += GetDQuadPointerSize( front, dq->flags );
        break;
    case QDT_POINTER:
    case QDT_ID:
        data_type = GetDQuadPointerCGType( dq->flags );
        if( dq->u.var.sym_handle == SYM_NULL ) {
            DGInteger( *front.back, dq->u.var.offset, data_type );
        } else if( dq->flags & Q_SEGMENT ) {    /* CodeBreaker: the selector of the symbol's segment */
            DGFEPtrBase( *front.back, static_cast<CGSYM_HANDLE>(dq->u.var.sym_handle) );
        } else {
            DGFEPtr( *front.back, static_cast<CGSYM_HANDLE>(dq->u.var.sym_handle), data_type, dq->u.var.offset );
        }
        front.cgendata.EmitDQuad_size += GetDQuadPointerSize( front, dq->flags );
        break;
    case QDT_CONST:                       /* array of characters */
        front.cgendata.EmitDQuad_size += EmitBytes( front, dq->u.string_leaf );
        break;
    case QDT_CONSTANT:
#if _CPU == 8086
        for( amount = dq->u_size; amount != 0; ) {
            if( amount + front.cgendata.EmitDQuad_size >= 0x00010000 ) {
                EmitZeros( front, 0x10000 - front.cgendata.EmitDQuad_size );
                amount -= ( 0x10000 - front.cgendata.EmitDQuad_size );
                front.cgendata.EmitDQuad_size = 0;
                if( front.cgendata.EmitDQuad_segid != SEG_CONST && front.cgendata.EmitDQuad_segid != SEG_DATA ) {
                    ++front.cgendata.EmitDQuad_segid;
                    BESetSeg( *front.back, front.cgendata.EmitDQuad_segid );
                }
            } else {
                EmitZeros( front, amount );
                front.cgendata.EmitDQuad_size += amount;
                amount = 0;
            }
        }
#else
        amount = dq->u_size;
        EmitZeros( front, amount );
        front.cgendata.EmitDQuad_size += amount;
#endif
        break;
    }
}


void EmitDataQuads( FrontRun& front )
{
    DATA_QUAD   *dq;

    if( StartDataQuadAccess(front) != NULL ) {
        for( ; (dq = NextDataQuad(front)) != NULL; ) {
            do {
                EmitDQuad( front, dq );
            } while( (dq->flags & Q_REPEATED_DATA) && (dq->u_rpt_count)-- > 1 );
        }
    }
}
