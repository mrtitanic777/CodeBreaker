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
* Description:  Block creation and destruction functions.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "cgauxinf.h"
#include "zoiks.h"
#include "makeins.h"
#include "data.h"
#include "utils.h"
#include "stackcg.h"
#include "makeaddr.h"
#include "namelist.h"
#include "targetin.h"
#include "opttell.h"
#include "makeblk.h"
#include "typemap.h"
#include "blktrim.h"
#include "bgcall.h"
#include "edge.h"
#include "feprotos.h"


#define BLOCK_SIZE(n)   (sizeof( block ) + (n - 1) * sizeof( block_edge ))

block   *MakeBlockInit( block_num targets )
/*****************************************/
{
    block       *blk;
    block_num   i;

    blk = static_cast<block *>(CGAlloc( BLOCK_SIZE( targets ) ));
    blk->next_block = NULL;
    blk->prev_block = NULL;
    blk->label = NULL;
    blk->class_ = static_cast<block_class>(0);
    blk->ins.head.line_num = 0;
    blk->ins.head.next = (instruction *)&blk->ins;
    blk->ins.head.prev = (instruction *)&blk->ins;
    blk->ins.head.opcode = OP_BLOCK;
    HW_CAsgn( blk->ins.head.live.regs, HW_EMPTY );
    _LBitInit( blk->ins.head.live.within_block, EMPTY );
    _GBitInit( blk->ins.head.live.out_of_block, EMPTY );
    blk->ins.blk = blk;
    blk->u.interval = NULL;
    blk->inputs = 0;
    blk->input_edges = NULL;
    blk->targets = 0;
    blk->dataflow = NULL;
    blk->u1.cc = NULL;
    blk->loop_head = NULL;
    blk->unroll_count = 0;
    blk->stack_depth = 0;
    blk->depth = 0;
    _DBitInit( blk->dom.id, 0U );
    for( i = 0; i < targets; i++ ) {
        blk->edge[i].source = blk;
        blk->edge[i].flags = BEF_NONE;
    }
    blk->blk_id = BLK_ID_NONE;
    blk->gen_blk_id = BLK_ID_NONE;
    return( blk );
}


block   *MakeBlockCopy( block_num targets, block *src, block_num src_targets )
{
    block       *blk;

    blk = static_cast<block *>(CGAlloc( BLOCK_SIZE( targets ) ));
    Copy( src, blk, BLOCK_SIZE( src_targets ) );
    return( blk );
}


void    FreeABlock( block * blk )
/*******************************/
{
    if( blk->targets <= 1 ) {
        CGFree( blk );
    } else {
        CGFree( blk );
    }
}


void    FreeBlock( void )
/***********************/
{
    while( data_state().CurrBlock->ins.head.next != (instruction *)&data_state().CurrBlock->ins ) {
        FreeIns( data_state().CurrBlock->ins.head.next );
    }
    if( data_state().CurrBlock->dataflow != NULL ) {
        CGFree( data_state().CurrBlock->dataflow );
    }
    FreeABlock( data_state().CurrBlock );
}


void    EnLink( label_handle label )
/**********************************/
{
    block       *blk;

    blk = MakeBlockInit( 1 );
    blk->label = label;
    blk->ins.head.line_num = data_state().SrcLine;
    data_state().CurrBlock = blk;
    data_state().SrcLine = 0;
}

void    AddIns( instruction *ins )
/********************************/
{
    if( !data_state().HaveCurrBlock ) {
        EnLink( AskForNewLabel() );
        data_state().CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
        data_state().HaveCurrBlock = true;
    }
    ins->head.next = (instruction *)&data_state().CurrBlock->ins;
    ins->head.prev = data_state().CurrBlock->ins.head.prev;
    data_state().CurrBlock->ins.head.prev->head.next = ins;
    data_state().CurrBlock->ins.head.prev = ins;
    ins->head.line_num = data_state().SrcLine;
    _INS_NOT_BLOCK( ins );
    ins->id = ++data_state().InsId;
    data_state().SrcLine = 0;
}


void    GenBlock( block_class class_, block_num targets )
/******************************************************/
{
    block       *new_blk;
    block_edge  *edge;
    instruction *ins;

    NamesCrossBlocks();
    if( data_state().HeadBlock == NULL ) {
        data_state().HeadBlock = data_state().CurrBlock;
        data_state().CurrBlock->blk_id = 1;
        data_state().CurrBlock->gen_blk_id = 1;
    } else {
        data_state().BlockList->next_block = data_state().CurrBlock;
        data_state().CurrBlock->blk_id = data_state().BlockList->blk_id + 1;
        data_state().CurrBlock->gen_blk_id = data_state().BlockList->gen_blk_id + 1;
    }
    if( data_state().SrcLine != 0 ) {
        /*
         * Add an instruction to carry the line number for the block ending
         * opcode (the AddIns code puts in line number automatically).
         */
        ins = MakeNop();
        ins->flags.u.nop_flags |= NOP_SOURCE_QUEUE;
        AddIns( ins );
    }
    data_state().CurrBlock->prev_block = data_state().BlockList;
    data_state().BlockList = data_state().CurrBlock;
    data_state().CurrBlock->next_block = NULL;
    if( targets > 1 ) {
        new_blk = MakeBlockCopy( targets, data_state().CurrBlock, 1 );
        if( data_state().CurrBlock->ins.head.next == (instruction *)&data_state().CurrBlock->ins ) {
            new_blk->ins.head.next = (instruction *)&new_blk->ins;
            new_blk->ins.head.prev = (instruction *)&new_blk->ins;
        } else {
            new_blk->ins.head.next->head.prev = (instruction *)&new_blk->ins;
            new_blk->ins.head.prev->head.next = (instruction *)&new_blk->ins;
        }
        new_blk->ins.blk = new_blk;
        /*
         * Move all references to CurrBlock
         */
        if( data_state().HeadBlock == data_state().CurrBlock ) {
            data_state().HeadBlock = new_blk;
        }
        if( data_state().BlockList == data_state().CurrBlock ) {
            data_state().BlockList = new_blk;
        }
        if( new_blk->prev_block != NULL ) {
            new_blk->prev_block->next_block = new_blk;
        }
        if( new_blk->next_block != NULL ) {
            new_blk->next_block->prev_block = new_blk;
        }
        for( edge = new_blk->input_edges; edge != NULL; edge = edge->next_source ) {
            edge->destination.u.blk = new_blk;
        }
        CGFree( data_state().CurrBlock );
        data_state().CurrBlock = new_blk;
    }
    if( _IsBlkAttr( data_state().CurrBlock, BLK_BIG_LABEL ) )        /* the only one that sticks*/
        class_ |= BLK_BIG_LABEL;
    data_state().CurrBlock->class_ = class_;
    while( targets-- > 1 ) {
        data_state().CurrBlock->edge[targets].flags = BEF_NONE;
    }
}


block   *ReGenBlock( block *blk, label_handle lbl )
/*************************************************/
{
    block       *new_blk;
    block_edge  *edge;
    block_num   targets;

    targets = blk->targets + 1;
    new_blk = MakeBlockCopy( targets, blk, targets - 1 );
    new_blk->edge[targets - 1].destination.u.lbl = lbl;
    new_blk->edge[targets - 1].flags = BEF_NONE;
    new_blk->targets = targets;
    /*
     * Move all references to blk
     */
    if( blk->ins.head.next == (instruction *)&blk->ins ) {
        new_blk->ins.head.next = (instruction *)&new_blk->ins;
        new_blk->ins.head.prev = (instruction *)&new_blk->ins;
    } else {
        blk->ins.head.next->head.prev = (instruction *)&new_blk->ins;
        blk->ins.head.prev->head.next = (instruction *)&new_blk->ins;
    }
    while( targets-- > 0 ) {
        new_blk->edge[targets].source = new_blk;
    }
    new_blk->ins.blk = new_blk;
    if( data_state().HeadBlock == blk ) {
        data_state().HeadBlock = new_blk;
    }
    if( data_state().BlockList == blk ) {
        data_state().BlockList = new_blk;
    }
    if( new_blk->prev_block != NULL ) {
        new_blk->prev_block->next_block = new_blk;
    }
    if( new_blk->next_block != NULL ) {
        new_blk->next_block->prev_block = new_blk;
    }
    for( edge = new_blk->input_edges; edge != NULL; edge = edge->next_source ) {
        edge->destination.u.blk = new_blk;
    }
    FreeABlock( blk );
    return( new_blk );
}


type_class_def  InitCallState( const type_def *tipe )
/***************************************************/
{
    name            *name;
    cg_sym_handle   sym;
    aux_handle      aux;

    sym = AskForLblSym( static_cast<label_handle>(data_state().CurrProc->label) );
    name = AllocMemory( sym, 0, CG_FE, TypeClass( tipe ) );
    name->v.usage |= USE_MEMORY;        /* so not put in conflict graph*/
    aux = static_cast<aux_handle>(FEAuxInfo( sym, FEINF_AUX_LOOKUP ));
    return( CallState( aux, tipe, &data_state().CurrProc->state ) );
}


void    AddTarget( label_handle dest, bool dest_label_dies )
/***********************************************************
 * Don't handle expression jumps yet
 */
{
    block_edge  *edge;

    edge = &data_state().CurrBlock->edge[data_state().CurrBlock->targets++];
    edge->source = data_state().CurrBlock;
    edge->destination.u.lbl = dest;
    edge->next_source = NULL;
    if( dest_label_dies ) {
        edge->flags |= BEF_DEST_LABEL_DIES;
    }
}


block   *FindBlockWithLbl( label_handle label )
/*********************************************/
{
    block       *blk;

    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( blk->label == label ) {
            break;
        }
    }
    return( blk );
}


void    FixEdges( void )
/**********************/
{
    block       *blk;
    block       *dest;
    block_num   i;
    block_edge  *edge;

    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !_IsBlkAttr( blk, BLK_BIG_JUMP ) ) {
            for( i = blk->targets; i-- > 0; ) {
                edge = &blk->edge[i];
                dest = FindBlockWithLbl( edge->destination.u.lbl );
                if( dest != NULL ) {
                    edge->flags |= BEF_DEST_IS_BLOCK;
                    PointEdge( edge, dest );
                }
            }
        }
    }
}


namespace {

/* the statics of this file, one set for each run of the compiler */
struct MakeblkState {
    label_handle LinkReturnsParms[2];
};

}  // namespace

static inline MakeblkState& makeblk_state() { return cbrun_part<MakeblkState>(); }


static void *LinkReturns( void *arg )
/***********************************/
{
    block               *blk;
    block_num           i;
//    bool                found;
    label_handle        link_to;
    label_handle        to_search;

    /* unused parameters */ (void)arg;

    link_to = makeblk_state().LinkReturnsParms[0];
    to_search = makeblk_state().LinkReturnsParms[1];
    blk = FindBlockWithLbl( to_search );
//    found = false;
    if( blk == NULL )
        return( TO_SR_VALUE( false ) );
    if( _IsBlkVisited( blk ) )
        return( TO_SR_VALUE( true ) );
    if( _IsBlkAttr( blk, BLK_LABEL_RETURN ) ) {
        for( i = blk->targets; i-- > 0; ) {
            if( blk->edge[i].destination.u.lbl == link_to ) {
                /*
                 * kick out ... already linked
                 */
                return( TO_SR_VALUE( true ) );
            }
        }
        blk = ReGenBlock( blk, link_to );
//        found = true;
    } else {
        _MarkBlkVisited( blk );
        if( _IsBlkAttr( blk, BLK_CALL_LABEL ) ) {
            if( blk->next_block == NULL )
                return( TO_SR_VALUE( false ) );
            makeblk_state().LinkReturnsParms[0] = link_to;
            makeblk_state().LinkReturnsParms[1] = blk->next_block->label;
            if( !FROM_SR_VALUE( SafeRecurseCG( LinkReturns, NULL ), bool ) ) {
                return( TO_SR_VALUE( false ) );
            }
        } else {
            for( i = blk->targets; i-- > 0; ) {
                makeblk_state().LinkReturnsParms[0] = link_to;
                makeblk_state().LinkReturnsParms[1] = blk->edge[i].destination.u.lbl;
                if( !FROM_SR_VALUE( SafeRecurseCG( LinkReturns, NULL ), bool ) ) {
                    return( TO_SR_VALUE( false ) );
                }
            }
        }
    }
    return( TO_SR_VALUE( true ) );
}

bool        FixReturns( void )
/*****************************
 * link all LABEL_RETURN blocks to any CALL_LABEL block they could
 * have been invoked from
 */
{
    block       *blk;
    block       *other_blk;

    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( _IsBlkAttr( blk, BLK_CALL_LABEL ) ) {
            _MarkBlkVisited( blk );
            if( blk->next_block == NULL )
                return( false );
            _MarkBlkAttrSet( blk, BLK_RETURNED_TO );
            makeblk_state().LinkReturnsParms[0] = blk->next_block->label;
            makeblk_state().LinkReturnsParms[1] = blk->edge[0].destination.u.lbl;
            if( !FROM_SR_VALUE( LinkReturns( NULL ), bool ) ) {
                return( false );
            }
            for( other_blk = data_state().HeadBlock; other_blk != NULL; other_blk = other_blk->next_block ) {
                _MarkBlkUnVisited( other_blk );
            }
        }
    }
    return( true );
}


void    UnFixEdges( void )
/************************/
{
    block       *blk;
    block_num   i;
    block_edge  *edge;

    for( blk = data_state().HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !_IsBlkAttr( blk, BLK_BIG_JUMP ) ) {
            for( i = blk->targets; i-- > 0; ) {
                edge = &blk->edge[i];
                if( edge->flags & BEF_DEST_IS_BLOCK ) {
                    RemoveInputEdge( edge );
                    edge->destination.u.lbl = edge->destination.u.blk->label;
                    edge->flags &= ~BEF_DEST_IS_BLOCK;
                }
            }
        }
    }
}


void    AddAnIns( block *blk, instruction *ins )
/**********************************************/
{
    block       *curr_block;

    curr_block = data_state().CurrBlock;
    data_state().CurrBlock = blk;
    AddIns( ins );
    data_state().CurrBlock = curr_block;
}


bool    BlkTooBig( void )
/***********************/
{
    label_handle    blk;

    if( !data_state().HaveCurrBlock )
        return( false );
    if( data_state().CurrBlock == NULL )
        return( false );
    if( data_state().CurrBlock->ins.head.next == (instruction *)&data_state().CurrBlock->ins )
        return( false );
    _INS_NOT_BLOCK( data_state().CurrBlock->ins.head.next );
    if( (data_state().InsId - data_state().CurrBlock->ins.head.next->id) < INS_PER_BLOCK )
        return( false );
    if( data_state().CurrBlock->targets != 0 )
        return( false );
    blk = AskForNewLabel();
    GenBlock( BLK_JUMP, 1 );
    AddTarget( blk, false );
    EnLink( blk );
    data_state().CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    return( true );
}


void    NewProc( level_depth level )
/**********************************/
{
    proc_def        *new_proc;
    name_class_def  class_;

    if( data_state().CurrProc != NULL ) {
        SaveToTargProc();
        data_state().CurrProc->head_block = data_state().HeadBlock;
        data_state().CurrProc->tail_block = data_state().BlockList;
        data_state().CurrProc->curr_block = data_state().CurrBlock;
        data_state().CurrProc->lasttemp = data_state().LastTemp;
        data_state().CurrProc->dummy_index = data_state().DummyIndex;
        for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; class_++ ) {
            data_state().CurrProc->names[class_] = data_state().Names[class_];
        }
        data_state().CurrProc->block_by_block = data_state().BlockByBlock;
        data_state().CurrProc->ins_id = data_state().InsId;
        data_state().CurrProc->untrimmed = data_state().BlocksUnTrimmed;
    }
    data_state().HaveCurrBlock = true;
    data_state().BlocksUnTrimmed = true;
    data_state().MaxStack = 0;
    data_state().InsId = 0;
    data_state().HeadBlock = NULL;
    data_state().BlockList = NULL;
    ReInitNames();
    new_proc = static_cast<proc_def *>(CGAlloc( sizeof( proc_def ) ));
    memset( new_proc, 0, sizeof( proc_def ) );
    new_proc->next_proc = data_state().CurrProc;
    data_state().CurrProc = new_proc;
    new_proc->frame_index = NULL;
    new_proc->lex_level = level;
    new_proc->parms.size = 0;
    new_proc->parms.base = 0;
    new_proc->locals.size = 0;
    new_proc->locals.base = AskDisplaySize( level );
    new_proc->prolog_state = static_cast<prolog_state_def>(0);
    new_proc->parms_list = NULL;
    InitTargProc();
}


void    FreeProc( void )
/**********************/
{
    proc_def        *oldproc;
    name_class_def  class_;

    for( data_state().CurrBlock = data_state().HeadBlock; data_state().CurrBlock != NULL; data_state().CurrBlock = data_state().HeadBlock ) {
        data_state().HeadBlock = data_state().CurrBlock->next_block;
        FreeBlock();
    }
    data_state().BlockList = NULL;
    FreeNames();
    TellBeginExecutions();
    oldproc = data_state().CurrProc;
    if( oldproc != NULL ) {
        data_state().CurrProc = data_state().CurrProc->next_proc;
        if( oldproc->state.parm.table ) {
            CGFree( oldproc->state.parm.table );
        }
        CGFree( oldproc );
        if( data_state().CurrProc != NULL ) {
            RestoreFromTargProc();
            data_state().InsId = data_state().CurrProc->ins_id;
            data_state().HeadBlock = data_state().CurrProc->head_block;
            data_state().BlockList = data_state().CurrProc->tail_block;
            data_state().CurrBlock = data_state().CurrProc->curr_block;
            for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; class_++ ) {
                data_state().Names[class_] = data_state().CurrProc->names[class_];
            }
            data_state().LastTemp = data_state().CurrProc->lasttemp;
            data_state().DummyIndex = data_state().CurrProc->dummy_index;
            data_state().BlockByBlock = data_state().CurrProc->block_by_block;
            data_state().BlocksUnTrimmed = data_state().CurrProc->untrimmed;
        } else {
            data_state().InsId = 0;
            data_state().BlockByBlock = false;
        }
    }
}
