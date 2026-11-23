/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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


#define GB_DEFINE_GBITNEXT
#include "_cgstd.h"
#include "coderep.h"
#include "data.h"
#include "cgsrtlst.h"
#include "namelist.h"
#include "conflict.h"
#include "dataflo.h"
#include "savings.h"
#include "regsave.h"
#include "varusage.h"
#include "feprotos.h"
#include "backrun.hpp"


static  void            PropagateConflicts( BackRun& back );
static  void            LiveAnalysis( block *tail, global_bit_set memory_bits );

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct DatafloState: dataflo_state.h) */

}  // namespace



static  void    AddTempSave( BackRun& back, name *op, block *blk )
/*************************************************/
{
    conflict_node *conf;

    if( op->n.class_ == N_INDEXED ) {
        op = op->i.index;
    }
    if( op->n.class_ != N_TEMP )
        return;
    op = DeAlias( op );
    conf = op->v.conflict;
    if( conf == NULL )
        return;
    conf->savings += Weight( back, 1, blk );
}


static  bool    AllocBefore( BackRun& back, void *n1, void *n2 )
/***********************************************/
{
    name    *t1 = static_cast<name *>(n1);
    name    *t2 = static_cast<name *>(n2);

    /* const temps after all others */
    if( (t1->t.temp_flags & CONST_TEMP) && (t2->t.temp_flags & CONST_TEMP) == 0 ) {
        return( false );
    }
    if( (t1->t.temp_flags & CONST_TEMP) == 0 && (t2->t.temp_flags & CONST_TEMP) ) {
        return( true );
    }
    if( t1->v.conflict == NULL && t2->v.conflict != NULL ) {
        return( false );
    }
    if( t1->v.conflict != NULL ) {
        if( t2->v.conflict == NULL )
            return( true );
        return( t1->v.conflict->savings > t2->v.conflict->savings );
    } else {
        return( t1->t.v.id > t2->t.v.id );
    }
}


static  void    RoughSortTemps( BackRun& back )
/*************************************

    Do a real rough sort on the templist by savings in case
    we run out of global bits. This will help the register
    allocator do the right thing
*/
{
    name                *actual_name;
    name                *opnd;
    block               *blk;
    instruction         *ins;
    opcnt               i;

    for( opnd = back.data.Names[N_TEMP]; opnd != NULL; opnd = opnd->n.next_name ) {
        if( ( opnd->v.usage & (USE_MEMORY | USE_ADDRESS) ) ) {
            opnd->v.usage |= NEEDS_MEMORY | USE_MEMORY;
        } else if( opnd->v.usage & USE_IN_ANOTHER_BLOCK ) {
            actual_name = DeAlias( opnd );
            if( actual_name->v.conflict == NULL ) {
                AddConflictNode( back, actual_name );
            }
        }
    }
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            for( i = 0; i < ins->num_operands; ++i ) {
                AddTempSave( back, ins->operands[i], blk );
            }
            if( ins->result != NULL ) {
                AddTempSave( back, ins->result, blk );
            }
        }
    }
    back.data.Names[N_TEMP] = static_cast<name *>(SortList( back, back.data.Names[N_TEMP], offsetof( name, n.next_name ), AllocBefore ));
}


static  global_bit_set AssignGlobalBits( BackRun& back, name_class_def class_,
                             global_bit_set *bit, bool first_time )
/*****************************************************************/
{
    conflict_node       *conf;
    global_bit_set      all_used;
    name                *actual_name;
    name                *opnd;

    if( class_ == N_TEMP ) {
        _GBitInit( all_used, EMPTY );
        if( !back.dataflo.MoreUseInOtherTemps )
            return( all_used );
        back.dataflo.MoreUseInOtherTemps = false;
    }
    _GBitInit( all_used, EMPTY );
    for( opnd = back.data.Names[class_]; opnd != NULL; opnd = opnd->n.next_name ) {
        if( ( opnd->v.usage & (USE_MEMORY | USE_ADDRESS) ) ) {
            opnd->v.usage |= NEEDS_MEMORY | USE_MEMORY;
        } else if( opnd->v.usage & USE_IN_ANOTHER_BLOCK ) {
            actual_name = opnd;
            if( class_ == N_TEMP ) {
                actual_name = DeAlias( actual_name );
            }
            if( _GBitEmpty( *bit ) && ( class_ == N_MEMORY ) ) {
                actual_name->v.usage |= NEEDS_MEMORY | USE_MEMORY;
            } else {
                conf = actual_name->v.conflict;
                if( conf == NULL && first_time ) {
                    conf = AddConflictNode( back, actual_name );
                }
                if( conf != NULL && _Isnt( conf, CST_CONFLICT_ON_HOLD ) && _GBitEmpty( conf->id.out_of_block ) ) {
                    _GBitAssign( conf->id.out_of_block, *bit );
                    _GBitTurnOn( all_used, *bit );
                    _GBitNext( bit );
                    if( _GBitEmpty( conf->id.out_of_block ) ) {
                        _SetTrue( conf, CST_CONFLICT_ON_HOLD );
                        back.dataflo.MoreUseInOtherTemps = true;
                    }
                }
            }
        }
    }
    return( all_used );
}


static  void    CheckGlobals( BackRun& back )
/**********************************/
{
    name        *op;

#define FORCE_MEM (USE_MEMORY|USE_ADDRESS)
    if( IsntModel( back, CGSW_GEN_RELAX_ALIAS ) ) {
        for( op = back.data.Names[N_MEMORY]; op != NULL; op = op->n.next_name ) {
            if( op->m.memory_type == CG_FE ) {
                if( ( op->v.usage & FORCE_MEM ) != FORCE_MEM ) {
                    if( FEAttr( *back.front, static_cast<cg_sym_handle>(op->v.symbol) ) & ( FE_VISIBLE | FE_GLOBAL ) ) {
                        op->v.usage |= FORCE_MEM;
                    }
                }
            }
        }
    }
}


void    MakeConflicts( BackRun& back )
/***************************/
{
    global_bit_set     bit;

    CheckGlobals(back);
    back.dataflo.MoreUseInOtherTemps = true;
    _GBitFirst( bit );
    if( !back.data.BlockByBlock ) {
        RoughSortTemps(back);
    }
    AssignGlobalBits( back, N_TEMP, &bit, true );
    back.data.MemoryBits = AssignGlobalBits( back, N_MEMORY, &bit, true );
    PropagateConflicts(back);
}


bool    MoreConflicts( BackRun& back )
/***************************/
{
    global_bit_set     bit;

    _GBitFirst( bit );
    _GBitInit( back.data.MemoryBits, EMPTY );
    bit = AssignGlobalBits( back, N_TEMP, &bit, false );
    if( !_GBitEmpty( bit ) ) {
        PropagateConflicts(back);
        return( true );
    } else {
        return( false );
    }
}


static  void    PropagateConflicts( BackRun& back )
/****************************************/
{
    block       *blk;

    /*   Assign global bits to temporaries first in case we run out of*/
    /*   bits.*/
    /*   (When we run out of bits, everything } else { is forced to memory)*/

    FindReferences(back);
    if( !back.data.BlockByBlock ) {
        LiveAnalysis( back.data.HeadBlock, back.data.MemoryBits );
    }
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        SetInOut( back, blk );
    }
}


void    SetInOut( BackRun& back, block *blk )
/****************************/
{
    if( back.data.BlockByBlock ) {
        blk->dataflow->in  = blk->dataflow->use;
        _GBitTurnOn( blk->dataflow->in, blk->dataflow->def );
        _GBitAssign( blk->dataflow->out, blk->dataflow->in );
    }
    if( _IsBlkAttr( blk, BLK_RETURN ) ) {
        HW_TurnOn( blk->ins.head.live.regs, back.data.CurrProc->state.return_reg );
    }
    _LBitInit( blk->ins.head.live.within_block, EMPTY );
    _GBitAssign( blk->ins.head.live.out_of_block, blk->dataflow->out );
    _LBitInit( blk->available_bit, ~EMPTY );
}


static  void    LiveAnalysis( block *tail, global_bit_set memory_bits )
/*********************************************************************/
{
    block               *blk;
    block               *target;
    data_flow_def       *data;
    global_bit_set      new_gbs;
    block_num           i;
    bool                change;


    /*   Want tail of block list, to do inside-out traversal of*/
    /*   depth first spanning tree*/

    while( tail->next_block != NULL ) {
        tail = tail->next_block;
    }
    for( ;; ) {
        change = false;
        for( blk = tail; blk != NULL; blk = blk->prev_block ) {
            data = blk->dataflow;

            /*   The OUT set of a return block includes any globals*/
            /*   defined within the procedure*/

            if( _IsBlkAttr( blk, BLK_RETURN | BLK_LABEL_RETURN ) ) {
                new_gbs = memory_bits;
            } else {
                _GBitInit( new_gbs, EMPTY );
            }
            if( !_IsBlkAttr( blk, BLK_BIG_JUMP ) ) {
                for( i = blk->targets; i-- > 0; ) {
                    target = blk->edge[i].destination.u.blk;

                    /*   new OUT = union of successors' IN*/

                    _GBitTurnOn( new_gbs, target->dataflow->in );
                }
            }
            if( !_GBitSame( data->out, new_gbs ) ) {
                data->out = new_gbs;
                change = true;
            }

            /*   new IN == ( new OUT - DEF ) union USE*/

            _GBitTurnOff( new_gbs, data->def );
            _GBitTurnOn( new_gbs, data->use );
            if( !_GBitSame( data->in, new_gbs ) ) {
                _GBitAssign( data->in, new_gbs );
                change = true;
            }
        }
        if( !change ) {
            break;
        }
    }
}
