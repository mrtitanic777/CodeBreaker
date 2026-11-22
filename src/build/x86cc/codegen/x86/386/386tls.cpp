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
* Description:  386 TLS support gear (for Win32).
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "data.h"
#include "feprotos.h"
#include "makeins.h"
#include "rtrtn.h"
#include "namelist.h"
#include "insutil.h"
#include "x86tls.h"
#include "backrun.hpp"


static  name    *RTMemRef( BackRun& back, rt_class rtindex )
/********************************************
    create a memory_name to reference the given runtime label.
*/
{
    label_handle    lbl;

    lbl = RTLabel( back, rtindex );
    return( AllocMemory( back, lbl, 0, CG_LBL, WD ) );
}

static  void    AddSegOverride( BackRun& back, instruction *ins, hw_reg_set reg )
/****************************************************************/
{
    name                *reg_name;

    reg_name = AllocRegName( back, reg );
    ins->operands[ins->num_operands++] = reg_name;
}

static  name    *GetNTTLSDataRef( BackRun& back, instruction *ins, name *op, type_class_def type_class )
/****************************************************************************************
    Emit instructions to load allow a reference to op (a piece of
    TLS data) and return the resulting index name.
*/
{
    name                *tls_index;
    name                *tls_array;
    name                *t1;
    name                *t2;
    name                *t3;
    name                *temp_index;
    name                *result_index;
    instruction         *new_ins;

    tls_index = RTMemRef( back, RT_TLS_INDEX );
    tls_array = RTMemRef( back, RT_TLS_ARRAY );
    t1 = AllocTemp( back, WD );
    t2 = AllocTemp( back, WD );
    t3 = AllocTemp( back, WD );
    new_ins = MakeMove( back, tls_array, t1, WD );
    AddSegOverride( back, new_ins, HW_FS );
    PrefixIns( back, ins, new_ins );
    new_ins = MakeMove( back, tls_index, t2, WD );
    PrefixIns( back, ins, new_ins );
    new_ins = MakeBinary( back, OP_MUL, t2, AllocS32Const( back, 4 ), t2, WD );
    PrefixIns( back, ins, new_ins );
    new_ins = MakeBinary( back, OP_ADD, t1, t2, t1, WD );
    PrefixIns( back, ins, new_ins );
    temp_index = AllocIndex( back, t1, NULL, 0, WD );
    new_ins = MakeMove( back, temp_index, t3, WD );
    PrefixIns( back, ins, new_ins );
    result_index = ScaleIndex( back, t3, op, op->v.offset, type_class, back.data.TypeClassSize[type_class], SCALE_NONE, static_cast<i_flags>(0) );
    return( result_index );
}

static  void    DropCall( BackRun& back, instruction *ins, name *temp )
/******************************************************/
{
    name                *eax_name;
    name                *null_name;
    instruction         *new_ins;

    null_name = AllocRegName( back, HW_EMPTY );
    eax_name = AllocRegName( back, HW_EAX );
    new_ins = NewIns( back, 3 );
    new_ins->head.opcode = OP_CALL;
    new_ins->type_class = WD;
    new_ins->operands[CALL_OP_USED] = null_name;
    new_ins->operands[CALL_OP_USED2] = null_name;
    new_ins->operands[CALL_OP_ADDR] = RTMemRef( back, RT_TLS_REGION );
    new_ins->result = eax_name;
    new_ins->zap = &eax_name->r;
    new_ins->num_operands = 2;         /* special case for OP_CALL*/
    PrefixIns( back, ins, new_ins );
    new_ins = MakeMove( back, eax_name, temp, WD );
    PrefixIns( back, ins, new_ins );
}

static  name    *GetGenericTLSDataRef( BackRun& back, instruction *ins, name *op, type_class_def type_class )
/********************************************************************************************/
{
    name                *tls;
    name                *result_index;
    name                *temp;
    instruction         *new_ins;


    tls = back.data.CurrProc->targ.tls_index;
    if( tls == NULL ) {
        tls = AllocTemp( back, WD );
        back.data.CurrProc->targ.tls_index = tls;
        if( !back.data.BlockByBlock ) {
            /* 2007-06-28 RomanT
             * Grrr... DropCall() inserts new ins _before_ old one, screwing
             * startup sequence. So keep call in first block, but make it
             * before next instruction of last one  (== after last one).
             */
            DropCall( back, back.data.HeadBlock->ins.head.prev->head.next, tls );
        }
    }
    if( back.data.BlockByBlock ) {
        DropCall( back, ins, tls );
    }
    temp = AllocTemp( back, WD );
    new_ins = MakeMove( back, tls, temp, WD );
    PrefixIns( back, ins, new_ins );
    result_index = ScaleIndex( back, temp, op, op->v.offset, type_class, back.data.TypeClassSize[type_class], SCALE_NONE, static_cast<i_flags>(0) );
    return( result_index );
}

static  name    *GetTLSDataRef( BackRun& back, instruction *ins, name *op, type_class_def type_class )
/*************************************************************************************/
{
    if( _IsTargetModel( CGSW_X86_GENERIC_TLS ) ) {
        return( GetNTTLSDataRef( back, ins, op, type_class ) );
    } else {
        return( GetGenericTLSDataRef( back, ins, op, type_class ) );
    }
}

static  void    ExpandTlsOp( BackRun& back, instruction *ins, name **pop )
/**********************************************************
    If *pop is a ref to a piece of thread-local data, replace
    it by a ref to an index [t1] and prepend the magic sequence
    to get the address of a piece of tls data to the instruction.
    Here is the sequence to access variable foo:
        mov fs:__tls_array -> t1
        mov __tls_index -> t2
        mov t2 * 4 -> t2
        add t1, t2 -> t1
        mov [t1] -> t3
        mov foo[t3] -> result
*/
{
    fe_attr             attr;
    name                *op;
    name                *temp;
    name                *tls_data;
    name                *index;
    name                *base;
    instruction         *new_ins;

    op = *pop;
    switch( op->n.class_ ) {
    case N_MEMORY:
        if( op->m.memory_type == CG_FE ) {
            attr = FEAttr( *back.front, static_cast<cg_sym_handle>(op->v.symbol) );
            if( ( attr & FE_THREAD_DATA ) != 0 ) {
                *pop = GetTLSDataRef( back, ins, op, _OpClass(ins) );
            }
        }
        break;
    case N_INDEXED:
        // gotta check for the base being one of these stupid TLS things
        if( op->i.base != NULL && ( op->i.index_flags & X_FAKE_BASE ) == 0 ) {
            base = op->i.base;
            if( base->n.class_ != N_MEMORY || base->m.memory_type != CG_FE )
                break;
            attr = FEAttr( *back.front, static_cast<cg_sym_handle>(base->v.symbol) );
            if( ( attr & FE_THREAD_DATA ) == 0 )
                break;
            tls_data = GetTLSDataRef( back, ins, base, _OpClass(ins) );
            temp = AllocTemp( back, WD );
            new_ins = MakeUnary( back, OP_LA, tls_data, temp, WD );
            PrefixIns( back, ins, new_ins );
            index = op->i.index;
            if( op->i.scale != SCALE_NONE ) {
                const int_32 values[] = { 1, 2, 4, 8, 16 };
                if( op->i.scale > SCALE_16 )
                    _Zoiks( ZOIKS_134 );
                index = AllocTemp( back, WD );
                new_ins = MakeBinary( back, OP_MUL, op->i.index,
                                AllocS32Const( back, values[op->i.scale] ),
                                index, WD );
                PrefixIns( back, ins, new_ins );
            }
            new_ins = MakeBinary( back, OP_ADD, temp, index, temp, WD );
            PrefixIns( back, ins, new_ins );
            *pop = ScaleIndex( back, temp, NULL, 0,
                            _OpClass(ins),
                            back.data.TypeClassSize[_OpClass(ins)],
                            SCALE_NONE, static_cast<i_flags>(0) );
        }
        break;
    }
}

void    ExpandThreadDataRef( BackRun& back, instruction *ins )
/******************************************************
    Expand any references to thread-local data into the
    appropriate magical sequence of instructions.  Note
    that since this is done independantly for each
    reference to tls data, we end up with horrible,
    redundant code.  However, the optimizer should be
    able to clean this up providing -oa has been
    used.
*/
{
    opcnt       i;

    for( i = 0; i < ins->num_operands; i++ ) {
        ExpandTlsOp( back, ins, &ins->operands[i] );
    }
    if( ins->result != NULL ) {
        ExpandTlsOp( back, ins, &ins->result );
    }
}
