/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2018 The Open Watcom Contributors. All Rights Reserved.
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


#define _DBit_DEFINE_BITNEXT
#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "edge.h"
#include "data.h"
#include "dominate.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct DominateState: dominate_state.h) */

}  // namespace



static bool             AssignDominatorBits( BackRun& back )
/*************************************************/
{
    block       *blk;
    dom_bit_set id;

    back.dominate.ReturnBlock = NULL;
    _DBitFirst( id );
    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        if( _IsBlkAttr( blk, BLK_RETURN ) ) {
            if( back.dominate.ReturnBlock != NULL )
                return( false );
            back.dominate.ReturnBlock = blk;
        }
        _DBitAssign( blk->dom.id, id );
        if( _DBitEmpty( id ) )
            return( false );
        _DBitNext( &id );
    }
    return( back.dominate.ReturnBlock != NULL );
}

bool CalcDominatorInfo( BackRun& back )
/****************************/
{
    block       *blk;
    block_edge  *edge;
    block_num   i;

    dom_bit_set predecessors;
    dom_bit_set successors;
    dom_bit_set full_set;
    dom_bit_set old_dominator;
    dom_bit_set temp_bits;
    bool        change;
    bool        have_info;

    have_info = false;
    if( AssignDominatorBits(back) ) {
        have_info = true;
        _DBitInit( full_set, ~0U );
        for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
            _DBitAssign( blk->dom.dominator,      full_set );
            _DBitAssign( blk->dom.post_dominator, full_set );
        }
        _DBitAssign( back.data.HeadBlock->dom.dominator, back.data.HeadBlock->dom.id );
        _DBitAssign( back.dominate.ReturnBlock->dom.post_dominator, back.dominate.ReturnBlock->dom.id );
        do {
            change = false;
            for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
                if( blk != back.data.HeadBlock ) {
                    _DBitAssign( old_dominator, blk->dom.dominator );
                    _DBitAssign( predecessors, full_set );
                    for( edge = blk->input_edges; edge != NULL; edge = edge->next_source ) {
                        _DBitAssign( temp_bits, full_set );
                        _DBitTurnOff( temp_bits, edge->source->dom.dominator );
                        _DBitTurnOff( predecessors, temp_bits );
                    }
                    _DBitTurnOn( predecessors, blk->dom.id );
                    _DBitAssign( blk->dom.dominator, predecessors );
                    if( !_DBitSame( blk->dom.dominator, old_dominator ) ) {
                        change = true;
                    }
                }
                if( blk != back.dominate.ReturnBlock ) {
                    _DBitAssign( old_dominator, blk->dom.post_dominator );
                    _DBitAssign( successors, full_set );
                    edge = &blk->edge[0];
                    for( i = 0; i < blk->targets; i++ ) {
                        _DBitAssign( temp_bits, full_set );
                        _DBitTurnOff( temp_bits, edge->destination.u.blk->dom.post_dominator );
                        _DBitTurnOff( successors, temp_bits );
                        edge++;
                    }
                    _DBitTurnOn( successors, blk->dom.id );
                    _DBitAssign( blk->dom.post_dominator, successors );
                    if( !_DBitSame( blk->dom.post_dominator, old_dominator ) ) {
                        change = true;
                    }
                }
            }
        } while( change );
    }
    return( have_info );
}
