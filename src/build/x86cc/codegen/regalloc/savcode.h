/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023-2025 The Open Watcom Contributors. All Rights Reserved.
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


#pragma once
#ifdef _InRegAssgn

#define _UpdateLive( ins, conf, reg_name ) \
             if( ( GBitOverlap( ins->head.live.out_of_block, conf->id.out_of_block ) ) \
              || ( LBitOverlap( ins->head.live.within_block, conf->id.within_block ) ) ) { \
                 HwTurnOn( ins->head.live.regs, reg_name->r.reg ); \
             }

#define _Equal( op1, op2 ) ContainedIn( op1, op2 )

#define _SuffixLoad( back, ins, x, y, c ) \
             SuffixIns( back, ins, MakeMove( back, x, y, c ) ); \
             AddSegment( back, (ins)->head.next )

#define _PrefixLoad( back, ins, x, y, c ) \
             PrefixIns( back, ins, MakeMove( back, x, y, c ) ); \
             AddSegment( back, (ins)->head.prev )

#define _SuffixStore( back, ins, x, y, c ) \
             SuffixIns( back, ins, MakeMove( back, x, y, c ) ); \
             AddSegment( back, (ins)->head.next )

#define _PrefixStore( back, ins, x, y, c ) \
             PrefixIns( back, ins, MakeMove( back, x, y, c ) ); \
             AddSegment( back, (ins)->head.prev )

#define _ReplaceOpnd( back, tree, ins, i, reg_name, _1 ) \
             DelSegOp( back, ins, i ); \
             ins->operands[i] = FindReg( back, tree, ins->operands[i], reg_name ); \
             FixGenEntry( back, ins );

#define _ReplaceIdxOpnd( back, tree, ins, i, reg_name ) \
             ins->operands[i] = ReplIndex(back, ins,tree,ins->operands[i],reg_name); \
             ins->head.state = INS_NEEDS_WORK

#define _ReplaceResult( back, tree, ins, reg_name, _1 ) \
             DelSegRes( back, ins ); \
             ins->result = FindReg( back, tree, ins->result, reg_name ); \
             FixGenEntry( back, ins );

#define _ReplaceIdxResult( back, tree, ins, reg_name ) \
             ins->result = ReplIndex( back, ins, tree, ins->result, reg_name ); \
             ins->head.state = INS_NEEDS_WORK

#define _UpdateCost( back, _1, _2 )

#else

#define _UpdateLive( _1, _2, _3 )

#define _Equal( op1, op2 ) ( ( (op1)->n.class_==N_TEMP ? DeAlias(op1) : (op1) ) == (op2) )

#define _SuffixLoad( back, _1, _2, _3, class_ )   \
             block_cost += back.regsave.Save.load_cost[class_];

#define _PrefixLoad( back, _1, _2, _3, class_ )   \
             block_cost += back.regsave.Save.load_cost[class_];

#define _SuffixStore( back, _1, _2, _3, class_ )  \
             block_cost += back.regsave.Save.store_cost[class_];

#define _PrefixStore( back, _1, _2, _3, class_ )  \
             block_cost += back.regsave.Save.store_cost[class_];

#define _ReplaceOpnd( back, _1, ins, i, _2, class_ ) \
             block_save += back.regsave.Save.use_save[class_]; \
             if( i == 0 \
              && ins->head.opcode < FIRST_CONDITION \
              && ins->result != NULL \
              && ins->result->n.class_ == N_REGISTER ) { \
                 block_save += back.regsave.Save.use_save[class_]; \
             }

#define _ReplaceResult( back, _1, ins, _2, class_ ) \
            if( ( ins->head.opcode==OP_MOV || ins->head.opcode==OP_CONVERT ) \
             && ins->result->n.class_ == N_TEMP \
             && ins->operands[0]->n.class_ == N_TEMP \
             && ins->result->t.location != NO_LOCATION \
             && ins->result->t.location == ins->operands[0]->t.location ) { \
                block_cost+=back.regsave.Save.load_cost[class_]; \
            } else { \
                block_save+=back.regsave.Save.def_save[class_]; \
                if( ins->num_operands != 0 \
                 && ins->operands[0]->n.class_ == N_REGISTER ) { \
                    block_save+=back.regsave.Save.use_save[class_]; \
                } \
            }

#define _ReplaceIdxResult( back, _1, _2, _3 ) \
             block_save += back.regsave.Save.index_save;

#define _ReplaceIdxOpnd( back, _1, _2, _3, _4 ) \
             block_save += back.regsave.Save.index_save;

#define _UpdateCost( back, blk, reg_name ) \
             save += Weight( back, block_save, blk ); \
             cost += Weight( back, block_cost, blk ); \
             block_save = 0; \
             block_cost = 0

#endif

    class_ = opnd->n.type_class;
    blk = conf->start_block;
    ins = conf->ins_range.first;
    last = false;
    if( LBitOverlap( conf->ins_range.first->head.live.within_block, conf->id.within_block ) ) {
        flows_in = true;
    } else {
        flows_in = false;
    }
    for( ;; ) {
        flow = blk->dataflow;
        /*  for each block in conflict range */
        final_defn = NULL;
        /*  the very last definition */
        last_defn = NULL;
        /*  the previous reaching definition */
        first_use = NULL;
        /*  the very first use */
        for( ;; ) {
            /*  for each instruction in conflict range */
            if( ins->head.opcode == OP_BLOCK )
                break;
            next = ins->head.next;
            /*  reload volatile names after calls */
            if( OpIsCall( ins->head.opcode ) ) {
                if( ( ( opnd->v.usage & USE_ADDRESS )
                   || ( ( opnd->v.usage & NEEDS_MEMORY )
                        && LacksConflictState( conf, CST_OK_ACROSS_CALLS ) ) )
                        && !GBitOverlap( conf->id.out_of_block, flow->call_exempt ) ) {
                    if( (ins->flags.u.call_flags & CALL_WRITES_NO_MEMORY) == 0
                     || ( opnd->n.class_ == N_TEMP
                       && ( opnd->v.usage & USE_ADDRESS ) ) ) {
                        _SuffixLoad( back, ins, opnd, reg_name, class_ );
                    }
                    if( ( last_defn != NULL
                       || ( ( opnd->v.usage & USE_IN_ANOTHER_BLOCK )
                          && GBitOverlap( conf->id.out_of_block, flow->in ) ) )
                     && ( ( (ins->flags.u.call_flags & CALL_WRITES_NO_MEMORY) == 0 )
                       || ( (ins->flags.u.call_flags & CALL_READS_NO_MEMORY) == 0 )
                       || ( opnd->n.class_ == N_TEMP
                            && ( opnd->v.usage & USE_ADDRESS ) ) ) ) {
                        last_defn = NULL;
                        temp_ins = ins->head.prev;
                        _PrefixStore( back, ins, reg_name, opnd, class_ );
                        if( first_use == NULL ) {
                            first_use = temp_ins->head.next;
                        }
                    }
                }
                if( first_use == NULL ) {
                    first_use = ins;
                }
            }
            for( i = ins->num_operands; i-- > 0; ) {
                if( _Equal( ins->operands[i], opnd ) ) {
                    if( first_use == NULL ) {
                        first_use = ins;
                    }
                    _ReplaceOpnd( back, tree, ins, i, reg_name, class_ );
                } else if( ins->operands[i]->n.class_ == N_INDEXED
                     && _Equal( ins->operands[i]->i.index, opnd ) ) {
                    if( first_use == NULL ) {
                        first_use = ins;
                    }
                    _ReplaceIdxOpnd( back, tree, ins, i, reg_name );
                }
            }
            if( ins->result != NULL ) {
                if( _Equal( ins->result, opnd ) ) {
                    last_defn = ins;
                    final_defn = ins;
                    _ReplaceResult( back, tree, ins, reg_name, class_ );
                } else if( ins->result->n.class_ == N_INDEXED
                        && _Equal( ins->result->i.index, opnd ) ) {
                    _ReplaceIdxResult( back, tree, ins, reg_name );
                }
            }
            _UpdateLive( ins, conf, reg_name );
            ins = next;
            last = ( ins->head.prev == conf->ins_range.last );
            if( last ) {
                break;
            }
        }
        _UpdateLive( ins, conf, reg_name );
        if( opnd->v.usage & USE_IN_ANOTHER_BLOCK ) {
#ifdef _InRegAssgn
            if( first_use == NULL ) {
                first_use = conf->ins_range.first;
                InsNotBlock( back, first_use );
                InsNotBlock( back, blk->ins.head.next );
                if( first_use->id < blk->ins.head.next->id ) {
                    first_use = blk->ins.head.next;
                }
            }
            if( final_defn == NULL ) {
                final_defn = conf->ins_range.last;
                InsNotBlock( back, final_defn );
                InsNotBlock( back, blk->ins.head.prev );
                if( final_defn->id > blk->ins.head.prev->id ) {
                    final_defn = blk->ins.head.prev;
                    if( IsBlkAttr( blk, BLK_CONDITIONAL | BLK_SELECT ) ) {
                        final_defn = blk->ins.head.next;
                        while( !OpIsJump( final_defn->head.opcode ) ) {
                            final_defn = final_defn->head.next;
                        }
                        final_defn = final_defn->head.prev;
                    } else {
                        while( final_defn->head.opcode == OP_NOP ) {
                            final_defn = final_defn->head.prev;
                        }
                    }
                }
            }
#endif
            if( reinterpret_cast<instruction *>(&blk->ins) == blk->ins.head.next ) {
                if( GBitOverlap( conf->id.out_of_block, flow->need_store ) ) {
                    _SuffixStore( back, reinterpret_cast<instruction *>(&blk->ins), reg_name, opnd, class_ );
                }
                if( GBitOverlap( conf->id.out_of_block, flow->need_load ) ) {
                    _SuffixLoad( back, reinterpret_cast<instruction *>(&blk->ins), opnd, reg_name, class_ );
                }
            } else {
                if( GBitOverlap( conf->id.out_of_block, flow->need_store ) ) {
                    _SuffixStore( back, final_defn, reg_name, opnd, class_ );
#ifndef _InRegAssgn
                    /* unused parameters */ (void)final_defn;
#endif
                }
                if( GBitOverlap( conf->id.out_of_block, flow->need_load ) ) {
#ifdef _InRegAssgn
                    if( first_use->head.prev == final_defn->head.next ) {
                        if( first_use->head.prev->head.opcode != OP_BLOCK ) {
                            first_use = first_use->head.prev;
                        }
                    }
#endif
                    _PrefixLoad( back, first_use, opnd, reg_name, class_ );
                }
            }
        } else if( opnd->v.usage & ( NEEDS_MEMORY | USE_ADDRESS ) ) {
            if( last_defn != NULL ) {
                _SuffixStore( back, last_defn, reg_name, opnd, class_ );
/*  } else { it was already stored after its last definition */
            }
            if( flows_in ) {
                _PrefixLoad( back, conf->ins_range.first, opnd, reg_name, class_ );
            }
        }
        _UpdateCost( back, blk, opnd );
        if( last )
            break;
        blk = blk->next_block;
        ins = blk->ins.head.next;
    }
