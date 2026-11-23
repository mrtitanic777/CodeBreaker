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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "data.h"
#include "savings.h"
#include "rgtbl.h"
#include "conflict.h"
#include "regsave.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct X86regsvState: x86regsv_state.h) */

}  // namespace



inline uint Cost( uint size, uint time, int s, int t ) { return ( s * size + t * time ) / TOTAL_WEIGHT; }
inline conflict_node *&NextCon( conflict_node *c ) { return (*reinterpret_cast<conflict_node**>(&((c)->tree))); }

void    InitWeights( BackRun& back, uint size )
/*****************************************
    Set up a structure describing the savings/costs involved
    with operations like loads, stores, saving a memory reference, etc.
    "size" is the importance of code size (vs. speed) expressed
    as a percentage between 0 and 100.
*/
{
    uint        time;

    AdjTimeSize( &time, &size );
    SetLoopCost( back, time );

/*   Indexing    - save load of index register*/
/*   Loads       - cost move memory to register instruction*/
/*   Stores      - cost move register to memory instruction*/
/*   Uses        - replace memory reference with register reference*/
/*   Definitions - save move register to memory*/
/*   Prologs    - push a register*/
/*   Epilogs    - pop a register*/


    if( CPULevel( back, CPU_486 ) /* or 586 */ ) {
        SetCost( back.regsave.Save.load_cost, Cost( size, time, 3,1 ) );
        SetCost( back.regsave.Save.store_cost, Cost( size, time, 3,1 ) );
        SetCost( back.regsave.Save.use_save, Cost( size, time, 1,1 ) );
        SetCost( back.regsave.Save.def_save, Cost( size, time, 1,1 ) );
        SetCost( back.regsave.Save.push_cost, Cost( size, time, 1,1 ) );
        SetCost( back.regsave.Save.pop_cost, Cost( size, time, 1,1 ) );
    } else if( CPULevel( back, CPU_386 ) ) {
        SetCost( back.regsave.Save.load_cost, Cost( size, time, 3,4 ) );
        SetCost( back.regsave.Save.store_cost, Cost( size, time, 3,2 ) );
        SetCost( back.regsave.Save.use_save, Cost( size, time, 1,(4-2) ) );
        SetCost( back.regsave.Save.def_save, Cost( size, time, 1,2 ) );
        SetCost( back.regsave.Save.push_cost, Cost( size, time, 1,2 ) );
        SetCost( back.regsave.Save.pop_cost, Cost( size, time, 1,4 ) );
    } else { // how about some 286 and 186 timings!
        SetCost( back.regsave.Save.load_cost, Cost( size, time, 3,17 ) );
        SetCost( back.regsave.Save.store_cost, Cost( size, time, 3,18 ) );
        SetCost( back.regsave.Save.use_save, Cost( size, time, 1,(18-3) ) );
        SetCost( back.regsave.Save.def_save, Cost( size, time, 1,18 ) );
        SetCost( back.regsave.Save.push_cost, Cost( size, time, 1,11 ) );
        SetCost( back.regsave.Save.pop_cost, Cost( size, time, 1,8 ) );
    }
    back.regsave.Save.index_save     = back.regsave.Save.load_cost[WD];
}


bool    WorthProlog( BackRun& back, conflict_node *conf, hw_reg_set reg )
/*******************************************************************
    decide if the savings associated with giving conflict "conf"
    is worth the cost incurred by generating a prolog to
    save and restore register "reg"
*/
{
    save_def            cost;
    save_def            savings;
    hw_reg_set          must_save;
    type_class_def      type_class;
    name                *op;

    type_class = conf->name->n.type_class;
    must_save = MustSaveRegs(back);
    if( back.data.BlockByBlock || HwOvlap( reg, back.data.GivenRegisters ) ||
       !HwOvlap( reg, must_save ) ) {
        cost = 0;
    } else {
        cost = back.regsave.Save.pop_cost[type_class] + back.regsave.Save.push_cost[type_class];
    }
    op = conf->name;
    savings = conf->savings;
    if( ConstTemp( op ) ) {
        /* adjust for the initial load */
        cost += Weight( back, back.regsave.Save.load_cost[type_class] + back.regsave.Save.def_save[type_class], conf->start_block );
        /* Adjust by a fudge factor */
        savings /= LOOP_FACTOR;
    } else {
        savings -= back.x86regsv.MaxConstSave;
    }
    return( savings >= cost );
}


void        ConstSavings( BackRun& back )
/**************************************

    Ensure constants are cached last by making sure that all
    "real" conflict nodes have higher savings than all conflict
    nodes for temps holding a constant value.  Make sure as well,
    that all "outer" loop constant temporary conflicts inherit the
    savings of any inner conflict that they define.
*/
{
    conflict_node       *conf;
    conflict_node       *other;
    block               *blk;
    instruction         *ins;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = ins->head.next ) {
            if( ins->head.opcode != OP_MOV )
                continue;
            if( !ConstTemp(ins->operands[0]) )
                continue;
            if( !ConstTemp(ins->result) )
                continue;
            conf = NameConflict( back, ins, ins->operands[0] );
            if( conf == NULL )
                continue;
            other = NameConflict( back, ins, ins->result );
            if( other == NULL )
                continue;
            if( LacksConflictState( conf, CST_SAVINGS_JUST_CALCULATED ) )
                continue;
            if( LacksConflictState( other, CST_SAVINGS_CALCULATED ) )
                continue;
            NextCon( conf ) = other;
        }
    }
    for( conf = back.data.ConfList; conf != NULL; conf = conf->next_conflict ) {
        if( HasConflictState( conf, CST_SAVINGS_JUST_CALCULATED ) && ConstTemp( conf->name ) ) {
            for( other=NextCon(conf); other != NULL; other=NextCon(other) ) {
                conf->savings += other->savings;
                if( HasConflictState( other, CST_CONF_VISITED ) ) {
                    break;
                }
            }
            SetTrue( conf, CST_CONF_VISITED ); /* already summed down the list */
        }
    }
    for( conf = back.data.ConfList; conf != NULL; conf = conf->next_conflict ) {
        NextCon( conf ) = NULL;
        SetFalse( conf, CST_CONF_VISITED );
    }
    back.x86regsv.MaxConstSave = 0;
    for( conf = back.data.ConfList; conf != NULL; conf = conf->next_conflict ) {
        if( !ConstTemp( conf->name ) )
            continue;
        SetFalse( conf, CST_SAVINGS_JUST_CALCULATED );
        if( LacksConflictState( conf, CST_SAVINGS_CALCULATED ) )
            continue;
        if( conf->savings < back.x86regsv.MaxConstSave )
            continue;
        back.x86regsv.MaxConstSave = conf->savings;
    }
    for( conf = back.data.ConfList; conf != NULL; conf = conf->next_conflict ) {
        if( ConstTemp( conf->name ) )
            continue;
        if( LacksConflictState( conf, CST_SAVINGS_JUST_CALCULATED ) )
            continue;
        SetFalse( conf, CST_SAVINGS_JUST_CALCULATED );
        if( conf->savings != 0 ) {
            conf->savings += back.x86regsv.MaxConstSave;
        }
    }
}
