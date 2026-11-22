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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "freelist.h"
#include "makeins.h"
#include "data.h"
#include "types.h"
#include "addrfold.h"
#include "display.h"
#include "utils.h"
#include "makeaddr.h"
#include "namelist.h"
#include "makeblk.h"
#include "typemap.h"
#include "temps.h"
#include "bldins.h"
#include "feprotos.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct MakeaddrState: makeaddr_state.h) */

}  // namespace



static  void    CopyAddr( an src, an dst )
/****************************************/
{
    an  link;

    link = dst->link;
    Copy( src, dst, sizeof( address_name ) );
    dst->link = link;
}


an      NewAddrName( BackRun& back )
/*************************/
{
    an  addr;

    addr = static_cast<an>(AllocFrl( back, &back.makeaddr.AddrNameFrl, sizeof( address_name ) ));
    addr->link = back.data.AddrList;
    back.data.AddrList = addr;
    addr->tipe = NULL;
    addr->u.n.index = NULL;
    addr->u.n.offset = 0;
    addr->u.n.name = NULL;
    addr->format = NF_ADDR;
    addr->flags = static_cast<addr_flags>(0);
    addr->u.n.base = NULL;
    addr->u.n.alignment = 0;
    return( addr );
}


an      NewBoolNode( BackRun& back )
/*************************/
{
    an  addr;

    addr = static_cast<an>(AllocFrl( back, &back.makeaddr.AddrNameFrl, sizeof( address_name ) ));
    addr->tipe = NULL;
    addr->format = NF_BOOL;
    addr->flags = static_cast<addr_flags>(0);
    return( addr );
}


an      MakeTypeTempAddr( BackRun& back, name *op, const type_def *tipe )
/********************************************************/
{
    an          addr;

    addr = NewAddrName(back);
    addr->tipe = tipe;
    addr->u.n.name = op;
    addr->class_ = CL_ADDR_TEMP;
    return( addr );
}

an      MakeTempAddr( BackRun& back, name *op )
/******************************/
{
    return( MakeTypeTempAddr( back, op, TypeAddress( back, TY_NEAR_POINTER ) ) );
}

void    InitMakeAddr( BackRun& back )
/**************************/
{
    InitFrl( &back.makeaddr.AddrNameFrl );
    back.data.AddrList = NULL;
}


name    *GenIns( BackRun& back, an addr )
/************************/
{
    return( GetValue( back, addr, NULL ) );
}


void    AddrFree( BackRun& back, an node )
/*************************/
{
    an  *owner;

    if( node->format != NF_BOOL ) {
        owner = &back.data.AddrList;
        for( ;; ) {
            if( *owner == node ) {
                *owner = node->link;
                break;
            }
            owner = &(*owner)->link;
        }
        if( node->format == NF_INS ) {
            FreeIns( back, node->u.i.ins );
        }
    }
    FrlFreeSize( back, &back.makeaddr.AddrNameFrl, (pointer *)node, sizeof( address_name ) );
}


void    InsToAddr( BackRun& back, an addr )
/**************************/
{
    an          new_an;
    instruction *ins;

    if( addr->format == NF_INS ) {
        ins = addr->u.i.ins;
        ins->result = BGNewTemp( back, addr->tipe );
        new_an = AddrName( back, ins->result, addr->tipe );
        new_an->flags = addr->flags;
        new_an->u.n.base = addr->u.i.base;
        new_an->u.n.alignment = addr->u.i.alignment;
        CopyAddr( new_an, addr );
        AddrFree( back, new_an );
    }
}


void    NamesCrossBlocks( BackRun& back )
/******************************/
{
    an          addr;
    an          new_an;
    an          next;
    name        *temp;

    /*
     * Careful. The list shifts under our feet.
     */
    for( addr = back.data.AddrList; addr != NULL; addr = next ) {
        next = addr->link;
        if( addr->flags & FL_ADDR_OK_ACROSS_BLOCKS ) {
            addr->flags |= FL_ADDR_CROSSED_BLOCKS;
        } else if( addr->format == NF_INS ) {
            InsToAddr( back, addr );
            addr->u.n.name->v.usage |= USE_IN_ANOTHER_BLOCK;
        } else if( addr->format != NF_CONS
             && addr->format != NF_BOOL
             && ( addr->format != NF_ADDR
                || ( addr->class_ != CL_ADDR_GLOBAL
                  && addr->class_ != CL_ADDR_TEMP ) ) ) {
            temp = GenIns( back, addr );
            if( temp->n.class_ == N_TEMP ) {
                temp->v.usage |= USE_IN_ANOTHER_BLOCK;
            } else if( temp->n.class_ == N_INDEXED ) {
                if( temp->i.index->n.class_ == N_TEMP ) {
                    temp->i.index->v.usage
                        |= USE_IN_ANOTHER_BLOCK;
                }
            }
            new_an = AddrName( back, temp, addr->tipe );
            CopyAddr( new_an, addr );
            AddrFree( back, new_an );
        }
    }
}


bool    AddrFrlFree( BackRun& back )
/*************************/
{
    return( FrlFreeAll( back, &back.makeaddr.AddrNameFrl, sizeof( address_name ) ) );
}


static  name    *Display( BackRun& back, cg_sym_handle symbol, level_depth level )
/*****************************************************************/
{
    proc_def    *old_currproc;
    name        *old_names;
    name        *op;

    old_currproc = back.data.CurrProc;
    old_names = back.data.Names[N_TEMP];
    while( level != back.data.CurrProc->lex_level ) {
        back.data.CurrProc = back.data.CurrProc->next_proc;
    }
    back.data.Names[N_TEMP] = back.data.CurrProc->names[N_TEMP];
    for( op = back.data.Names[N_TEMP]; op->v.symbol != symbol; ) {
        op = op->n.next_name;
    }
    op->v.usage |= ( NEEDS_MEMORY | USE_MEMORY | USE_IN_ANOTHER_BLOCK);
    AllocALocal( back, op );
    back.data.CurrProc = old_currproc;
    back.data.Names[N_TEMP] = old_names;
    return( MakeDisplay( back, op, level ) );
}


an      MakeGets( BackRun& back, an dst, an src, const type_def *tipe )
/******************************************************/
{
    name                *dst_name;
    name                *src_name;
    instruction         *ins;
    type_class_def      type_class;
    name                *temp;

    InsToAddr( back, dst );
    dst_name = Points( back, dst, tipe );
    ins = src->u.i.ins;
    if( src->format == NF_INS && back.data.CurrBlock->ins.head.prev == ins ) {
        ins->result = dst_name;
        src->format = NF_ADDR;  /* so instruction doesn't get freed! */
    } else {
        src_name = GetValue( back, src, dst_name );
        if( src_name != dst_name ||
         (( src_name->n.class_ == N_MEMORY ) && ( src_name->v.usage & VAR_VOLATILE )) ) {
            type_class = TypeClass( back, tipe );
            src_name = GenIns( back, src );
            if( dst_name->n.class_ == N_INDEXED &&
             (dst_name->i.index_flags & X_VOLATILE) == 0 ) {
                /*
                 * don't give him back an indexed name - it extends the life of
                 * a pointer
                 */
                temp = SAllocTemp( back, dst_name->n.type_class, dst_name->n.size );
                AddIns( back, MakeMove( back, src_name, dst_name, type_class ) );
                AddIns( back, MakeMove( back, dst_name, temp, type_class ) );
                dst_name = temp;
            } else {
                AddIns( back, MakeMove( back, src_name, dst_name, type_class ) );
            }
        }
    }
    BGDone( back, src );
    BGDone( back, dst );
    return( AddrName( back, dst_name, tipe ) );
}


an      MakeConst( BackRun& back, float_handle cf, const type_def *tipe )
/********************************************************/
{
    return( AddrName( back, AllocConst( back, cf ), tipe ) );
}


an      MakePoints( BackRun& back, an name, const type_def *tipe )
/*************************************************/
{
    an  new_an;

    new_an = AddrName( back, Points( back, name, tipe ), tipe );
    BGDone( back, name );
    return( new_an );
}

an      RegName( BackRun& back, hw_reg_set reg, const type_def *tipe )
/*****************************************************/
{
    an  addr;

    addr = NewAddrName(back);
    addr->format = NF_NAME;
    addr->tipe = tipe;
    addr->u.n.name = AllocRegName( back, reg );
    return( addr );
}

an      InsName( BackRun& back, instruction *ins, const type_def *tipe )
/*******************************************************/
{
    an  addr;

    addr = NewAddrName(back);
    addr->u.i.ins = ins;
    addr->tipe = tipe;
    addr->format = NF_INS;
    return( addr );
}

name    *LoadTemp( BackRun& back, name *temp, type_class_def type_class )
/********************************************************/
{
    instruction *ins;

    ins = MakeMove( back, temp, AllocTemp( back, type_class ), type_class );
    temp = ins->result;
    AddIns( back, ins );
    return( temp );
}


static  name    *Temporary( BackRun& back, name *temp, const type_def *tipe )
/************************************************************/
{
    if( temp->n.class_ != N_TEMP ) {
        temp = LoadTemp( back, temp, TypeClass( back, tipe ) /*temp->n.type_class*/ );
    }
    return( temp );
}


an      AddrEval( BackRun& back, an addr )
/*************************/
{
    an  new_an;

    new_an = AddrName( back, Temporary( back, GenIns( back, addr ), addr->tipe ), addr->tipe );
    AddrFree( back, addr );
    return( new_an );
}


void    MoveAddress(  an src,  an  dest )
/***************************************/
{
    dest->format = src->format;
    dest->class_ = src->class_;
    dest->u.n.name  = src->u.n.name;
    dest->u.n.index = src->u.n.index;
    dest->u.n.offset= src->u.n.offset;
}


void    Convert( BackRun& back, an addr, type_class_def type_class )
/***************************************************/
{
    instruction *ins;

    if( addr->u.n.offset != 0 ) {
        ins = MakeBinary( back, OP_ADD, addr->u.n.name,
                                AllocIntConst( back, addr->u.n.offset ),
                                AllocTemp( back, addr->u.n.name->n.type_class ),
                                TypeClass( back, addr->tipe ) );
        addr->u.n.name = ins->result;
        AddIns( back, ins );
    }
    ins = MakeUnary( back, OP_CONVERT, addr->u.n.name, AllocTemp( back, type_class ), type_class );
    addr->u.n.name = ins->result;
    addr->u.n.offset = 0;
    AddIns( back, ins );
}


bool    PointLess( an l_addr, an r_addr )
/***************************************/
{
    if( l_addr->class_ != CL_POINTER && r_addr->class_ != CL_POINTER )
        return( false );
    if( l_addr->u.n.offset != 0 || r_addr->u.n.offset != 0 )
        return( false );
    return( true );
}


an      AddrToIns( BackRun& back, an addr )
/**************************/
{
    instruction *ins;
    an          new_an;

    if( addr->format != NF_INS ) {
        ins = MakeMove( back, GenIns( back, addr ), NULL, TypeClass( back, addr->tipe ) );
        new_an = InsName( back, ins, addr->tipe );
        new_an->flags = addr->flags;
        new_an->u.n.alignment = addr->u.n.alignment;
        BGDone( back, addr );
        AddIns( back, ins );
    } else {
        new_an = addr;
    }
    return( new_an );
}


an      AddrDuplicate( BackRun& back, an node )
/******************************/
{
    an          new_an;
    name        *op;

    InsToAddr( back, node );
    op = GenIns( back, node );
    new_an = AddrName( back, op, node->tipe );
    CopyAddr( new_an, node );
    return( new_an );
}

an      AddrCopy( BackRun& back, an node )
/*************************/
{
    an  new_an;

    InsToAddr( back, node );
    new_an = NewAddrName(back);
    CopyAddr( node, new_an );
    return( new_an );
}


an      AddrSave( BackRun& back, an node )
/*************************/
{
    InsToAddr( back, node );
    return( node );
}


void    AddrDemote( an node )
/***************************/
{
    node->flags |= FL_ADDR_DEMOTED;
    if( node->format == NF_INS ) {
        node->u.i.ins->ins_flags |= INS_DEMOTED;
    }
}


name    *MaybeTemp( BackRun& back, name *op, type_class_def type_class )
/*******************************************************/
{
    if( op == NULL ) {
        op = AllocTemp( back, type_class );
    }
    return( op );
}


void    CheckPointer( BackRun& back, an addr )
/*****************************/
{
    InsToAddr( back, addr );
    if( addr->format == NF_NAME && ( addr->tipe->attr & TYPE_POINTER ) ) {
        addr->u.n.index = Temporary( back, addr->u.n.name, addr->tipe );
        addr->u.n.name = NULL;
        addr->u.n.offset = 0;
        addr->class_ = CL_POINTER;
        addr->format = NF_ADDR;
    }
}


void    FixCodePtr( an addr )
/***************************/
{
    if( addr->format == NF_INS ) {
        addr->u.i.ins->ins_flags |= INS_CODE_POINTER;
    }
}


bool    NeedPtrConvert( an addr, const type_def *tipe )
/*****************************************************/
{
    if( addr->format != NF_ADDR )
        return( true );
    if( addr->class_ == CL_ADDR_GLOBAL || addr->class_ == CL_ADDR_TEMP ) {
        if( tipe->refno == TY_NEAR_POINTER )
            return( false );
        if( tipe->refno == TY_LONG_POINTER )
            return( false );
        if( tipe->refno == TY_HUGE_POINTER ) {
            return( false );
        }
    }
    return( true );
}


name    *LoadAddress( BackRun& back, name *op, name *suggest, const type_def *type_ptr )
/***********************************************************************/
{
    name                *new_addr;
    type_class_def      type_class;

    if( op->n.class_ == N_INDEXED && !HasTrueBase( op ) ) {
        if( op->i.constant != 0 ) {
            type_class = op->i.index->n.type_class;
            new_addr = MaybeTemp( back, suggest, type_class );
            AddIns( back, MakeBinary( back, OP_ADD, op->i.index,
                         AllocS32Const( back, op->i.constant ),
                         new_addr, type_class ) );
        } else {
            new_addr = op->i.index;
        }
    } else {
        if( suggest != NULL ) {
            type_class = suggest->n.type_class;
        } else {
            if( type_ptr->length == WORD_SIZE ) {
                type_class = WD;
            } else {
                type_class = CP;
            }
        }
        new_addr = MaybeTemp( back, suggest, type_class );
        AddIns( back, MakeUnary( back, OP_LA, op, new_addr, type_class ) );
    }
    return( new_addr );
}


an      MakeAddrName( BackRun& back, cg_class class_, cg_sym_handle sym, const type_def *tipe )
/*****************************************************************************/
{
    an          addr;
    fe_attr     attr;
    level_depth level;
    name        *op;

    addr = NewAddrName(back);
    if( class_ != CG_FE ) {
        op = SAllocMemory( back, sym, 0, class_, TypeClass( back, tipe ), tipe->length );
        addr->u.n.name = op;
        addr->class_ = CL_ADDR_GLOBAL;
    } else {
        attr = FEAttr( *back.front, sym );
        level = FELexLevel( sym );
        if( attr & FE_STATIC ) {
            op = SAllocMemory( back, sym, 0, class_, TypeClass(back, tipe), tipe->length );
            if( ( attr & FE_MEMORY ) != EMPTY ) {
                op->v.usage |= NEEDS_MEMORY | USE_MEMORY;
            }
            addr->u.n.name = op;
            addr->class_ = CL_ADDR_GLOBAL;
            if( attr & FE_VOLATILE ) {
                op->v.usage |= VAR_VOLATILE | NEEDS_MEMORY | USE_MEMORY;
            }
            if( attr & FE_UNALIGNED ) {
                op->v.usage |= VAR_UNALIGNED;
            }
            if( attr & FE_CONSTANT ) {
                op->v.usage |= VAR_CONSTANT;
            }
        } else if( level != back.data.CurrProc->lex_level ) {
            op = Display( back, sym, level );
            addr->u.n.name = op;
            addr->format = NF_NAME;
        } else {
            op = SAllocUserTemp( back, sym, TypeClass( back, tipe ), tipe->length );
            if( attr & FE_MEMORY ) {
                op->v.usage |= NEEDS_MEMORY | USE_MEMORY;
            }
            if( attr & FE_ADDR_TAKEN ) {
                op->v.usage |= USE_ADDRESS;
            }
            addr->u.n.name = op;
            addr->class_ = CL_ADDR_TEMP;
            op->v.usage |= USE_IN_ANOTHER_BLOCK;
            if( attr & FE_VOLATILE ) {
                op->v.usage |= VAR_VOLATILE | NEEDS_MEMORY | USE_MEMORY;
            }
        }
    }
    addr->tipe = TypeAddress( back, NamePtrType( back, op ) );
    return( addr );
}
