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
#include "backrun.hpp"


inline unsigned long long BlockSize( unsigned short n ) { return (sizeof( block ) + (n - 1) * sizeof( block_edge )); }

block   *MakeBlockInit( BackRun& back, block_num targets )
/*****************************************/
{
    block       *blk;
    block_num   i;

    blk = static_cast<block *>(CGAlloc( back, BlockSize( targets ) ));
    blk->next_block = NULL;
    blk->prev_block = NULL;
    blk->label = NULL;
    blk->class_ = static_cast<block_class>(0);
    blk->ins.head.line_num = 0;
    blk->ins.head.next = reinterpret_cast<instruction *>(&blk->ins);
    blk->ins.head.prev = reinterpret_cast<instruction *>(&blk->ins);
    blk->ins.head.opcode = OP_BLOCK;
    HW_CAsgn( blk->ins.head.live.regs, HW_EMPTY );
    LBitInit( blk->ins.head.live.within_block, EMPTY );
    GBitInit( blk->ins.head.live.out_of_block, EMPTY );
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
    DBitInit( blk->dom.id, 0U );
    for( i = 0; i < targets; i++ ) {
        blk->edge[i].source = blk;
        blk->edge[i].flags = BEF_NONE;
    }
    blk->blk_id = BLK_ID_NONE;
    blk->gen_blk_id = BLK_ID_NONE;
    return( blk );
}


block   *MakeBlockCopy( BackRun& back, block_num targets, block *src, block_num src_targets )
{
    block       *blk;

    blk = static_cast<block *>(CGAlloc( back, BlockSize( targets ) ));
    memcpy( blk, src, BlockSize( src_targets ) );
    return( blk );
}


void    FreeABlock( BackRun& back, block * blk )
/*******************************/
{
    if( blk->targets <= 1 ) {
        CGFree( back, blk );
    } else {
        CGFree( back, blk );
    }
}


void    FreeBlock( BackRun& back )
/***********************/
{
    while( back.data.CurrBlock->ins.head.next != reinterpret_cast<instruction *>(&back.data.CurrBlock->ins) ) {
        FreeIns( back, back.data.CurrBlock->ins.head.next );
    }
    if( back.data.CurrBlock->dataflow != NULL ) {
        CGFree( back, back.data.CurrBlock->dataflow );
    }
    FreeABlock( back, back.data.CurrBlock );
}


void    EnLink( BackRun& back, label_handle label )
/**********************************/
{
    block       *blk;

    blk = MakeBlockInit( back, 1 );
    blk->label = label;
    blk->ins.head.line_num = back.data.SrcLine;
    back.data.CurrBlock = blk;
    back.data.SrcLine = 0;
}

void    AddIns( BackRun& back, instruction *ins )
/********************************/
{
    if( !back.data.HaveCurrBlock ) {
        EnLink( back, AskForNewLabel(back) );
        back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
        back.data.HaveCurrBlock = true;
    }
    ins->head.next = reinterpret_cast<instruction *>(&back.data.CurrBlock->ins);
    ins->head.prev = back.data.CurrBlock->ins.head.prev;
    back.data.CurrBlock->ins.head.prev->head.next = ins;
    back.data.CurrBlock->ins.head.prev = ins;
    ins->head.line_num = back.data.SrcLine;
    InsNotBlock( back, ins );
    ins->id = ++back.data.InsId;
    back.data.SrcLine = 0;
}


void    GenBlock( BackRun& back, block_class class_, block_num targets )
/******************************************************/
{
    block       *new_blk;
    block_edge  *edge;
    instruction *ins;

    NamesCrossBlocks(back);
    if( back.data.HeadBlock == NULL ) {
        back.data.HeadBlock = back.data.CurrBlock;
        back.data.CurrBlock->blk_id = 1;
        back.data.CurrBlock->gen_blk_id = 1;
    } else {
        back.data.BlockList->next_block = back.data.CurrBlock;
        back.data.CurrBlock->blk_id = back.data.BlockList->blk_id + 1;
        back.data.CurrBlock->gen_blk_id = back.data.BlockList->gen_blk_id + 1;
    }
    if( back.data.SrcLine != 0 ) {
        /*
         * Add an instruction to carry the line number for the block ending
         * opcode (the AddIns code puts in line number automatically).
         */
        ins = MakeNop(back);
        ins->flags.u.nop_flags |= NOP_SOURCE_QUEUE;
        AddIns( back, ins );
    }
    back.data.CurrBlock->prev_block = back.data.BlockList;
    back.data.BlockList = back.data.CurrBlock;
    back.data.CurrBlock->next_block = NULL;
    if( targets > 1 ) {
        new_blk = MakeBlockCopy( back, targets, back.data.CurrBlock, 1 );
        if( back.data.CurrBlock->ins.head.next == reinterpret_cast<instruction *>(&back.data.CurrBlock->ins) ) {
            new_blk->ins.head.next = reinterpret_cast<instruction *>(&new_blk->ins);
            new_blk->ins.head.prev = reinterpret_cast<instruction *>(&new_blk->ins);
        } else {
            new_blk->ins.head.next->head.prev = reinterpret_cast<instruction *>(&new_blk->ins);
            new_blk->ins.head.prev->head.next = reinterpret_cast<instruction *>(&new_blk->ins);
        }
        new_blk->ins.blk = new_blk;
        /*
         * Move all references to CurrBlock
         */
        if( back.data.HeadBlock == back.data.CurrBlock ) {
            back.data.HeadBlock = new_blk;
        }
        if( back.data.BlockList == back.data.CurrBlock ) {
            back.data.BlockList = new_blk;
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
        CGFree( back, back.data.CurrBlock );
        back.data.CurrBlock = new_blk;
    }
    if( IsBlkAttr( back.data.CurrBlock, BLK_BIG_LABEL ) )        /* the only one that sticks*/
        class_ |= BLK_BIG_LABEL;
    back.data.CurrBlock->class_ = class_;
    while( targets-- > 1 ) {
        back.data.CurrBlock->edge[targets].flags = BEF_NONE;
    }
}


block   *ReGenBlock( BackRun& back, block *blk, label_handle lbl )
/*************************************************/
{
    block       *new_blk;
    block_edge  *edge;
    block_num   targets;

    targets = blk->targets + 1;
    new_blk = MakeBlockCopy( back, targets, blk, targets - 1 );
    new_blk->edge[targets - 1].destination.u.lbl = lbl;
    new_blk->edge[targets - 1].flags = BEF_NONE;
    new_blk->targets = targets;
    /*
     * Move all references to blk
     */
    if( blk->ins.head.next == reinterpret_cast<instruction *>(&blk->ins) ) {
        new_blk->ins.head.next = reinterpret_cast<instruction *>(&new_blk->ins);
        new_blk->ins.head.prev = reinterpret_cast<instruction *>(&new_blk->ins);
    } else {
        blk->ins.head.next->head.prev = reinterpret_cast<instruction *>(&new_blk->ins);
        blk->ins.head.prev->head.next = reinterpret_cast<instruction *>(&new_blk->ins);
    }
    while( targets-- > 0 ) {
        new_blk->edge[targets].source = new_blk;
    }
    new_blk->ins.blk = new_blk;
    if( back.data.HeadBlock == blk ) {
        back.data.HeadBlock = new_blk;
    }
    if( back.data.BlockList == blk ) {
        back.data.BlockList = new_blk;
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
    FreeABlock( back, blk );
    return( new_blk );
}


type_class_def  InitCallState( BackRun& back, const type_def *tipe )
/***************************************************/
{
    name            *name;
    cg_sym_handle   sym;
    aux_handle      aux;

    sym = AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) );
    name = AllocMemory( back, sym, 0, CG_FE, TypeClass( back, tipe ) );
    name->v.usage |= USE_MEMORY;        /* so not put in conflict graph*/
    aux = static_cast<aux_handle>(FEAuxInfo( *back.front, sym, FEINF_AUX_LOOKUP ));
    return( CallState( back, aux, tipe, &back.data.CurrProc->state ) );
}


void    AddTarget( BackRun& back, label_handle dest, bool dest_label_dies )
/***********************************************************
 * Don't handle expression jumps yet
 */
{
    block_edge  *edge;

    edge = &back.data.CurrBlock->edge[back.data.CurrBlock->targets++];
    edge->source = back.data.CurrBlock;
    edge->destination.u.lbl = dest;
    edge->next_source = NULL;
    if( dest_label_dies ) {
        edge->flags |= BEF_DEST_LABEL_DIES;
    }
}


block   *FindBlockWithLbl( BackRun& back, label_handle label )
/*********************************************/
{
    block       *blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( blk->label == label ) {
            break;
        }
    }
    return( blk );
}


void    FixEdges( BackRun& back )
/**********************/
{
    block       *blk;
    block       *dest;
    block_num   i;
    block_edge  *edge;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !IsBlkAttr( blk, BLK_BIG_JUMP ) ) {
            for( i = blk->targets; i-- > 0; ) {
                edge = &blk->edge[i];
                dest = FindBlockWithLbl( back, edge->destination.u.lbl );
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
/* (struct MakeblkState: makeblk_state.h) */

}  // namespace



static void *LinkReturns( BackRun& back, void *arg )
/***********************************/
{
    block               *blk;
    block_num           i;
//    bool                found;
    label_handle        link_to;
    label_handle        to_search;

    /* unused parameters */ (void)arg;

    link_to = back.makeblk.LinkReturnsParms[0];
    to_search = back.makeblk.LinkReturnsParms[1];
    blk = FindBlockWithLbl( back, to_search );
//    found = false;
    if( blk == NULL )
        return( ToSrValue( false ) );
    if( IsBlkVisited( blk ) )
        return( ToSrValue( true ) );
    if( IsBlkAttr( blk, BLK_LABEL_RETURN ) ) {
        for( i = blk->targets; i-- > 0; ) {
            if( blk->edge[i].destination.u.lbl == link_to ) {
                /*
                 * kick out ... already linked
                 */
                return( ToSrValue( true ) );
            }
        }
        blk = ReGenBlock( back, blk, link_to );
//        found = true;
    } else {
        MarkBlkVisited( blk );
        if( IsBlkAttr( blk, BLK_CALL_LABEL ) ) {
            if( blk->next_block == NULL )
                return( ToSrValue( false ) );
            back.makeblk.LinkReturnsParms[0] = link_to;
            back.makeblk.LinkReturnsParms[1] = blk->next_block->label;
            if( !FromSrValue<bool>( SafeRecurseCG( back, LinkReturns, NULL )) ) {
                return( ToSrValue( false ) );
            }
        } else {
            for( i = blk->targets; i-- > 0; ) {
                back.makeblk.LinkReturnsParms[0] = link_to;
                back.makeblk.LinkReturnsParms[1] = blk->edge[i].destination.u.lbl;
                if( !FromSrValue<bool>( SafeRecurseCG( back, LinkReturns, NULL )) ) {
                    return( ToSrValue( false ) );
                }
            }
        }
    }
    return( ToSrValue( true ) );
}

bool        FixReturns( BackRun& back )
/*****************************
 * link all LABEL_RETURN blocks to any CALL_LABEL block they could
 * have been invoked from
 */
{
    block       *blk;
    block       *other_blk;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( IsBlkAttr( blk, BLK_CALL_LABEL ) ) {
            MarkBlkVisited( blk );
            if( blk->next_block == NULL )
                return( false );
            MarkBlkAttrSet( blk, BLK_RETURNED_TO );
            back.makeblk.LinkReturnsParms[0] = blk->next_block->label;
            back.makeblk.LinkReturnsParms[1] = blk->edge[0].destination.u.lbl;
            if( !FromSrValue<bool>( LinkReturns( back, NULL )) ) {
                return( false );
            }
            for( other_blk = back.data.HeadBlock; other_blk != NULL; other_blk = other_blk->next_block ) {
                MarkBlkUnVisited( other_blk );
            }
        }
    }
    return( true );
}


void    UnFixEdges( BackRun& back )
/************************/
{
    block       *blk;
    block_num   i;
    block_edge  *edge;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( !IsBlkAttr( blk, BLK_BIG_JUMP ) ) {
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


void    AddAnIns( BackRun& back, block *blk, instruction *ins )
/**********************************************/
{
    block       *curr_block;

    curr_block = back.data.CurrBlock;
    back.data.CurrBlock = blk;
    AddIns( back, ins );
    back.data.CurrBlock = curr_block;
}


bool    BlkTooBig( BackRun& back )
/***********************/
{
    label_handle    blk;

    if( !back.data.HaveCurrBlock )
        return( false );
    if( back.data.CurrBlock == NULL )
        return( false );
    if( back.data.CurrBlock->ins.head.next == reinterpret_cast<instruction *>(&back.data.CurrBlock->ins) )
        return( false );
    InsNotBlock( back, back.data.CurrBlock->ins.head.next );
    if( (back.data.InsId - back.data.CurrBlock->ins.head.next->id) < INS_PER_BLOCK )
        return( false );
    if( back.data.CurrBlock->targets != 0 )
        return( false );
    blk = AskForNewLabel(back);
    GenBlock( back, BLK_JUMP, 1 );
    AddTarget( back, blk, false );
    EnLink( back, blk );
    back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    return( true );
}


void    NewProc( BackRun& back, level_depth level )
/**********************************/
{
    proc_def        *new_proc;
    name_class_def  class_;

    if( back.data.CurrProc != NULL ) {
        SaveToTargProc(back);
        back.data.CurrProc->head_block = back.data.HeadBlock;
        back.data.CurrProc->tail_block = back.data.BlockList;
        back.data.CurrProc->curr_block = back.data.CurrBlock;
        back.data.CurrProc->lasttemp = back.data.LastTemp;
        back.data.CurrProc->dummy_index = back.data.DummyIndex;
        for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; class_++ ) {
            back.data.CurrProc->names[class_] = back.data.Names[class_];
        }
        back.data.CurrProc->block_by_block = back.data.BlockByBlock;
        back.data.CurrProc->ins_id = back.data.InsId;
        back.data.CurrProc->untrimmed = back.data.BlocksUnTrimmed;
    }
    back.data.HaveCurrBlock = true;
    back.data.BlocksUnTrimmed = true;
    back.data.MaxStack = 0;
    back.data.InsId = 0;
    back.data.HeadBlock = NULL;
    back.data.BlockList = NULL;
    ReInitNames(back);
    new_proc = static_cast<proc_def *>(CGAlloc( back, sizeof( proc_def ) ));
    memset( new_proc, 0, sizeof( proc_def ) );
    new_proc->next_proc = back.data.CurrProc;
    back.data.CurrProc = new_proc;
    new_proc->frame_index = NULL;
    new_proc->lex_level = level;
    new_proc->parms.size = 0;
    new_proc->parms.base = 0;
    new_proc->locals.size = 0;
    new_proc->locals.base = AskDisplaySize( level );
    new_proc->prolog_state = static_cast<prolog_state_def>(0);
    new_proc->parms_list = NULL;
    InitTargProc(back);
}


void    FreeProc( BackRun& back )
/**********************/
{
    proc_def        *oldproc;
    name_class_def  class_;

    for( back.data.CurrBlock = back.data.HeadBlock; back.data.CurrBlock != NULL; back.data.CurrBlock = back.data.HeadBlock ) {
        back.data.HeadBlock = back.data.CurrBlock->next_block;
        FreeBlock(back);
    }
    back.data.BlockList = NULL;
    FreeNames(back);
    TellBeginExecutions(back);
    oldproc = back.data.CurrProc;
    if( oldproc != NULL ) {
        back.data.CurrProc = back.data.CurrProc->next_proc;
        if( oldproc->state.parm.table ) {
            CGFree( back, oldproc->state.parm.table );
        }
        CGFree( back, oldproc );
        if( back.data.CurrProc != NULL ) {
            RestoreFromTargProc(back);
            back.data.InsId = back.data.CurrProc->ins_id;
            back.data.HeadBlock = back.data.CurrProc->head_block;
            back.data.BlockList = back.data.CurrProc->tail_block;
            back.data.CurrBlock = back.data.CurrProc->curr_block;
            for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; class_++ ) {
                back.data.Names[class_] = back.data.CurrProc->names[class_];
            }
            back.data.LastTemp = back.data.CurrProc->lasttemp;
            back.data.DummyIndex = back.data.CurrProc->dummy_index;
            back.data.BlockByBlock = back.data.CurrProc->block_by_block;
            back.data.BlocksUnTrimmed = back.data.CurrProc->untrimmed;
        } else {
            back.data.InsId = 0;
            back.data.BlockByBlock = false;
        }
    }
}
