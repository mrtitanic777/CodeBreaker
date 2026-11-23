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
* Description:  Turn expression tree into pseudo-assembly instructions.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "memcheck.h"
#include "makeins.h"
#include "data.h"
#include "bldins.h"
#include "addrfold.h"
#include "display.h"
#include "utils.h"
#include "tree.h"
#include "treeprot.h"
#include "makeaddr.h"
#include "namelist.h"
#include "makeblk.h"
#include "encode.h"
#include "rgtbl.h"
#include "typemap.h"
#include "generate.h"
#include "types.h"
#include "i64.h"
#include "feprotos.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct BldinsState: bldins_state.h) */

}  // namespace



an      BGVarargsBasePtr( BackRun& back, const type_def *tipe )
/**********************************************/
{
    an                  addr;

    addr = RegName( back, VarargsHomePtr(back), tipe );
    return( addr );
}

an      BGStackValue( BackRun& back, const type_def *tipe )
/******************************************/
{
    an                  addr;

    addr = RegName( back, StackReg(), tipe );
    return( addr );
}

an      BGInteger( BackRun& back, int_32 value, const type_def *tipe )
/*****************************************************/
{
    float_handle    cf;

    if( tipe->attr & TYPE_SIGNED ) {
        cf = CFCnvI32F( back, &back.cfloat.cgh, value );
    } else {
        cf = CFCnvU32F( back, &back.cfloat.cgh, value );
    }
    return( MakeConst( back, cf, tipe ) );
}

an      BGInt64( BackRun& back, signed_64 value, const type_def *tipe )
/******************************************************/
{
    name   *cname;

    if( tipe->attr & TYPE_SIGNED ) {
        cname = AllocS64Const( back, U64Low( value ), U64High( value ) );
    } else {
        cname = AllocU64Const( back, U64Low( value ), U64High( value ) );
    }
    return( AddrName( back, cname, tipe ) );
}

an  BGFloat( BackRun& back, const char *value, const type_def *tipe )
/****************************************************/
{
    return( MakeConst( back, CFCnvSF( back, &back.cfloat.cgh, value ), tipe ) );
}


an      BGName( BackRun& back, cg_class cl, pointer sym, const type_def *tipe )
/**************************************************************/
{
    return( MakeAddrName( back, cl, static_cast<cg_sym_handle>(sym), tipe ) );
}


an      BGTempName( BackRun& back, name *temp, const type_def *tipe )
/****************************************************/
{
    temp->v.usage |= USE_IN_ANOTHER_BLOCK;
    return( MakeTypeTempAddr( back, temp, tipe ) );
}


bool    FiniLabel( label_handle lbl, block *blk )
/***********************************************/
{
    block_num   i;

    for( i = blk->targets; i-- > 0; ) {
        if( blk->edge[i].destination.u.lbl == lbl ) {
            blk->edge[i].flags |= BEF_DEST_LABEL_DIES;
            return( true );
        }
    }
    if( blk->label == lbl ) {
        blk->edge[0].flags |= BEF_BLOCK_LABEL_DIES;
        return( true );
    }
    return( false );
}

void    BGFiniLabel( BackRun& back, label_handle lbl )
/*************************************/
{
    block       *blk;

    if( back.data.HaveCurrBlock && FiniLabel( lbl, back.data.CurrBlock ) )
        return;
    for( blk = back.data.BlockList; blk != NULL; blk = blk->prev_block ) {
        if( FiniLabel( lbl, blk ) ) {
            return;
        }
    }
    GenKillLabel( back, lbl );
}


bool    NeedConvert( const type_def *from, const type_def *to )
/*************************************************************/
{
    if( from == to )
        return( false );
    if( from->attr & TYPE_FLOAT )
        return( true );
    if( to->attr & TYPE_FLOAT )
        return( true );
    if( from->length != to->length )
        return( true );
#if _TARGET & _TARG_8086
    if( (to->attr & TYPE_POINTER) != (from->attr & TYPE_POINTER) && to->length != WORD_SIZE )
        return( true ); /* long <=> pointer */
#endif
    return( false );
}


static  an Unary( BackRun& back, cg_op opcode, an left, const type_def *tipe )
/*************************************************************/
{
    instruction *ins;
    an          res;

    ins = MakeNary( back, (opcode_defs)opcode, GenIns( back, left ), NULL, NULL, TypeClass( back, tipe ), TypeClass( back, left->tipe ), 1 );
    res = InsName( back, ins, tipe );
    AddIns( back, ins );
    BGDone( back, left );
    return( res );
}


static  an      CnvRnd( BackRun& back, an name, const type_def *tipe, cg_op opcode )
/*******************************************************************/
{
    if( NeedConvert( name->tipe, tipe ) ) {
        name = Unary( back, opcode, name, tipe );
    } else if( name->tipe != tipe ) {
        AddrDemote( name ); /* it's not quite the right type */
    }
    name->tipe = tipe;
    return( name );
}


name        *BGNewTemp( BackRun& back, const type_def *tipe )
/********************************************/
{
    name        *temp;

    temp = AllocTemp( back, TypeClass( back, tipe ) );
    if( temp->n.size == 0 ) {
        temp->n.size = tipe->length;
    }
    return( temp );
}


name        *BGGlobalTemp( BackRun& back, const type_def *tipe )
/***********************************************/
{
    name        *temp;

    temp = BGNewTemp( back, tipe );
    temp->v.usage |= USE_IN_ANOTHER_BLOCK;
    return( temp );
}


static  an      FlowOut( BackRun& back, an node, const type_def *tipe ) {
/********************************************************/

    name                *temp;
    label_handle        lbl;

    lbl = AskForNewLabel(back);
    temp = BGGlobalTemp( back, tipe );
    AddIns( back, MakeMove( back, AllocIntConst( back, FETrue() ), temp, temp->n.type_class ) );
    *(node->u.b.t) = back.data.CurrBlock->label;
    GenBlock( back, BLK_JUMP, 1 );
    AddTarget( back, lbl, false );
    EnLink( back, AskForNewLabel(back) );
    back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    AddIns( back, MakeMove( back, AllocIntConst( back, 0 ), temp, temp->n.type_class ) );
    *(node->u.b.f) = back.data.CurrBlock->label;
    GenBlock( back, BLK_JUMP, 1 );
    AddTarget( back, lbl, false );
    EnLink( back, lbl );
    back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    NamesCrossBlocks(back);
    AddrFree( back, node );
    return( AddrName( back, temp, tipe ) );
}


an      Arithmetic( BackRun& back, an name, const type_def *tipe )
/*************************************************/
{
    if( name->format == NF_BOOL ) {
        if( (tipe->attr & TYPE_FLOAT) != 0 || ( tipe->length > back.data.TypeInteger->length ) ) {
            name = FlowOut( back, name, back.data.TypeInteger );
            name = Unary( back, O_CONVERT, name, tipe );
        } else {
            name = FlowOut( back, name, tipe );
        }
    }
    return( name );
}

an      BGCompare( BackRun& back, cg_op opcode, an left, an rite, label_handle entry, const type_def *tipe )
/***************************************************************************************/
{
    an                  new_an;
    instruction         *ins;
    name                *newleft;
    name                *newrite;

    back.bldins.LastCmpType = tipe;
    newleft = GenIns( back, left );
    newrite = GenIns( back, rite );
    BGDone( back, left );
    BGDone( back, rite );
    NamesCrossBlocks(back);
    ins = MakeCondition( back, (opcode_defs)opcode, newleft, newrite, 0, 1, TypeClass( back, tipe ) );
    AddIns( back, ins );
    GenBlock( back, BLK_CONDITIONAL, 2 );
    AddTarget( back, NULL, false );
    AddTarget( back, NULL, false );
    new_an = NewBoolNode(back);
    new_an->u.b.e = entry;
    new_an->u.b.t = &back.data.CurrBlock->edge[0].destination.u.lbl;
    new_an->u.b.f = &back.data.CurrBlock->edge[1].destination.u.lbl;
    EnLink( back, AskForNewLabel(back) );
    back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    return( new_an );
}


an      Boolean( BackRun& back, an node, label_handle entry )
/********************************************/
{
    if( node->format != NF_BOOL ) {
        node = BGCompare( back, O_NE, node, BGInteger( back, 0, node->tipe ), entry, node->tipe );
    }
    return( node );
}


label_handle BGGetEntry( BackRun& back )
/*****************************/
{
    return( back.data.CurrBlock->label );
}


void    BG3WayControl( BackRun& back, an node, label_handle lt, label_handle eq, label_handle gt )
/*********************************************************************************/
{
    instruction         *ins;
    name                *op;
    label_handle        lbl;
    type_class_def      type_class;

    node = Arithmetic( back, node, node->tipe );
    type_class = TypeClass( back, node->tipe );
    NamesCrossBlocks(back);
    op = GenIns( back, node );
    BGDone( back, node );
    ins = NULL;
#if _TARGET & _TARG_80386
    if( type_class == FS ) {
        ins = MakeCondition( back, OP_BIT_TEST_FALSE, op, AllocS32Const( back, 0x7FFFFFFFL ), 0, 1, SW );
    }
#endif
    if( ins == NULL ) {
        ins = MakeCondition( back, OP_CMP_EQUAL, op, AllocIntConst( back, 0 ), 0, 1, type_class );
    }
    AddIns( back, ins );
    GenBlock( back, BLK_CONDITIONAL, 2 );
    AddTarget( back, eq, false );
    lbl = AskForNewLabel(back);
    AddTarget( back, lbl, false );

    EnLink( back, lbl );
    back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
#if _TARGET & _TARG_80386
    if( type_class == FS ) {
        type_class = SW;
    }
#endif
    ins = MakeCondition( back, OP_CMP_LESS, op, AllocIntConst( back, 0 ), 0, 1, type_class );
    AddIns( back, ins );
    GenBlock( back, BLK_CONDITIONAL, 2 );
    AddTarget( back, lt, false );
    AddTarget( back, gt, false );
    Generate( back, false );
    back.data.HaveCurrBlock = false;
}


void    BGControl( BackRun& back, cg_op opcode, an expr, label_handle lbl )
/**********************************************************/
{
    BGGenCtrl( back, opcode, expr, lbl, false );
}


void    BGGenCtrl( BackRun& back, cg_op opcode, an expr, label_handle lbl, bool gen )
/********************************************************************/
{
    switch( opcode ) {
    case O_LABEL:
        if( back.data.HaveCurrBlock ) {
            GenBlock( back, BLK_JUMP, 1 );  /* block with 1 target*/
            AddTarget( back, lbl, false );
            if( gen ) {
                Generate( back, false );
            }
        }
        EnLink( back, lbl );
        back.data.CurrBlock->unroll_count = back.bldins.UnrollValue;
        back.data.HaveCurrBlock = true;
        break;
    case O_GOTO:
        if( back.data.HaveCurrBlock ) {
            GenBlock( back, BLK_JUMP, 1 );
            AddTarget( back, lbl, false );
            if( gen ) {
                Generate( back, false );
            }
        }
        back.data.HaveCurrBlock = false;
        break;
    case O_INVOKE_LABEL:
        if( back.data.HaveCurrBlock ) {
            GenBlock( back, BLK_CALL_LABEL, 1 );
            AddTarget( back, lbl, false );
            EnLink( back, AskForNewLabel(back) );
            back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
        }
        break;
    case O_LABEL_RETURN:
        if( back.data.HaveCurrBlock ) {
            AddIns( back, MakeNop(back) );
            GenBlock( back, BLK_LABEL_RETURN, 0 );
            if( gen ) {
                Generate( back, false );
            }
        }
        back.data.HaveCurrBlock = false;
        break;
    case O_IF_TRUE:
        *(expr->u.b.f) = back.data.CurrBlock->label;
        *(expr->u.b.t) = lbl;
        AddrFree( back, expr );
        if( gen ) {
            Generate( back, false );
        }
        break;
    case O_IF_FALSE:
        *(expr->u.b.t) = back.data.CurrBlock->label;
        *(expr->u.b.f) = lbl;
        AddrFree( back, expr );
        if( gen ) {
            Generate( back, false );
        }
        break;
    default:
        break;
    }
}


void    BGBigLabel( BackRun& back, back_handle bck )
/***********************************/
{
    if( back.data.HaveCurrBlock ) {
        GenBlock( back, BLK_JUMP, 1 );  /* block with 1 target*/
        AddTarget( back, bck->lbl, false );
        Generate( back, false );
    }
    EnLink( back, bck->lbl );
    _MarkBlkAttrSet( back.data.CurrBlock, BLK_BIG_LABEL );
    back.data.HaveCurrBlock = true;
    BigLabel(back);
}


void    BGBigGoto( BackRun& back, label_handle lbl, level_depth level )
/******************************************************/
{
    GenBlock( back, BLK_BIG_JUMP, 1 ); // No longer supported!
    AddTarget( back, lbl, false );
    BigGoto( back, level );
    Generate( back, false );
    EnLink( back, AskForNewLabel(back) );
    back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
}


uint_32 BGUnrollCount( BackRun& back, uint_32 unroll_count )
/*******************************************/
{
    uint_32     old_value;

    old_value = back.bldins.UnrollValue;
    back.bldins.UnrollValue = unroll_count;
    return( old_value );
}


an      BGUnary( BackRun& back, cg_op opcode, an left, const type_def *tipe )
/************************************************************/
{
    an          new_an;

    new_an = NULL;
    switch( opcode ) {
    case O_POINTS:
        new_an = MakePoints( back, left, tipe );
        break;
    case O_CONVERT:
        if( tipe == left->tipe || !NeedPtrConvert( left, tipe ) ) {
            new_an = left;
        } else {
            new_an = CnvRnd( back, left, tipe, O_CONVERT );
        }
        break;
    case O_ROUND:
        if( tipe == left->tipe ) {
            new_an = left;
        } else {
            new_an = CnvRnd( back, left, tipe, O_ROUND );
        }
        break;
    default:
        break;
    }
    if( new_an == NULL ) {
        new_an = Unary( back, opcode, left, tipe );
    }
    new_an->tipe = tipe;
    return( new_an );
}


static  an      CheckType( BackRun& back, an op, const type_def *tipe ) {
/********************************************************/

    if( op->format != NF_ADDR )
        return( op );
    return( BGUnary( back, O_CONVERT, op, tipe ) );
}


an      BGBinary( BackRun& back, cg_op opcode, an left, an rite, const type_def *tipe, bool fold_addr )
/**********************************************************************************/
{
    an          result;
    instruction *ins;

    result = NULL;
    if( opcode == O_PLUS || opcode == O_MINUS
       && ( tipe->attr & TYPE_POINTER )
       && ( left->tipe->attr & TYPE_POINTER )
       && (rite->tipe->attr & (TYPE_POINTER | TYPE_FLOAT)) == 0 ) {
        /* Special case for pointer +- int. Don't convert! */
    } else {
        left = CheckType( back, left, tipe );
        rite = CheckType( back, rite, tipe );
    }
    switch( opcode ) {
    case O_PLUS:
        if( fold_addr ) {
            if( TGIsAddress(back) || CypAddrPlus( back, left, rite, tipe ) ) {
                result = AddrPlus( back, left, rite, tipe );
            }
        }
        break;
    case O_LSHIFT:
        if( fold_addr ) {
            if( TGIsAddress(back) || CypAddrShift( back, left, rite, tipe ) ) {
                result = AddrShift( back, left, rite, tipe );
            }
        }
        break;
    default:
        break;
    }
    if( result == NULL ) {
        left = CheckType( back, left, tipe );
        ins = MakeBinary( back, (opcode_defs)opcode, GenIns( back, left ), GenIns( back, rite ), NULL, TypeClass( back, tipe ) );
        result = InsName( back, ins, tipe );
        AddIns( back, ins );
        BGDone( back, left );
        BGDone( back, rite );
    }
    result->tipe = tipe;
    return( result );
}


an      BGOpGets( BackRun& back, cg_op opcode, an left, an rite, const type_def *tipe, const type_def *optipe )
/**********************************************************************************************/
{
    an                  result;
    an                  leftp;
    name                *temp;
    type_class_def      op_type_class;
    type_class_def      type_class;
    name                *left_name;
    instruction         *ins;

    leftp = MakePoints( back, left, tipe );
    left_name = GenIns( back, leftp );
    type_class = TypeClass( back, tipe );
    op_type_class = TypeClass( back, optipe );
    if( NeedConvert( tipe, optipe ) ) {
        temp = AllocTemp( back, op_type_class );
        ins = MakeConvert( back, left_name, temp, op_type_class, type_class );
        AddIns( back, ins );
        AddIns( back, MakeBinary( back, (opcode_defs)opcode, temp, GenIns( back, rite ), temp, op_type_class ) );
        ins = MakeConvert( back, temp, left_name, type_class, op_type_class );
        AddIns( back, ins );
    } else {
        ins = MakeBinary( back, (opcode_defs)opcode, left_name, GenIns( back, rite ), left_name, op_type_class );
        if( tipe != optipe ) {
            ins->ins_flags |= INS_DEMOTED; /* its not quite the right type */
        }
        AddIns( back, ins );
    }
    result = AddrName( back, left_name, tipe );
    BGDone( back, rite );
    BGDone( back, leftp );
    return( result );
}


an      BGConvert( BackRun& back, an left, const type_def *tipe )
/************************************************/
{
    an          new_an;
    type_attr   left_attr;

    left_attr = left->tipe->attr;
    new_an = BGUnary( back, O_CONVERT, left, tipe );
    if( left_attr & TYPE_CODE ) {
        FixCodePtr( new_an ); /*% kludge for code pointer conversions*/
    }
    return( new_an );
}


an      BGFlow( BackRun& back, cg_op opcode, an left, an rite )
/**********************************************/
{
    an                  new_an = NULL;
    label_handle        *temp;

    if( opcode == O_FLOW_NOT ) {
        temp = left->u.b.t;
        left->u.b.t = left->u.b.f;
        left->u.b.f = temp;
        new_an = left;
    } else {
        switch( opcode ) {
        case O_FLOW_AND:
            *(left->u.b.t) = rite->u.b.e;
            *(left->u.b.f) = back.data.CurrBlock->label;
            *(rite->u.b.f) = back.data.CurrBlock->label;
            GenBlock( back, BLK_JUMP, 1 );
            AddTarget( back, NULL, false );
            left->u.b.f = &back.data.CurrBlock->edge[0].destination.u.lbl;
            left->u.b.t = rite->u.b.t;
            new_an = left;
            AddrFree( back, rite );
            EnLink( back, AskForNewLabel(back) );
            back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
            break;
        case O_FLOW_OR:
            *(left->u.b.f) = rite->u.b.e;
            *(left->u.b.t) = back.data.CurrBlock->label;
            *(rite->u.b.t) = back.data.CurrBlock->label;
            GenBlock( back, BLK_JUMP, 1 );
            AddTarget( back, NULL, false );
            left->u.b.t = &back.data.CurrBlock->edge[0].destination.u.lbl;
            left->u.b.f = rite->u.b.f;
            new_an = left;
            AddrFree( back, rite );
            EnLink( back, AskForNewLabel(back) );
            back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
            break;
        default:
            break;
        }
    }
    return( new_an );
}


an      BGAssign( BackRun& back, an dst, an src, const type_def *tipe )
/******************************************************/
{
    an          res;

    res = MakeGets( back, dst, src, tipe );
    res->tipe = tipe;
    return( res );
}


an      BGCopy( BackRun& back, an node )
/***********************/
{
    return( AddrCopy( back, node ) );
}


an      BGDuplicate( BackRun& back, an node )
/****************************/
{
    return( AddrDuplicate( back, node ) );
}


void    BGDone( BackRun& back, an node )
/***********************/
{
    if( node->format == NF_BOOL ) {
        FlowOff( back, node );
    }
    AddrFree( back, node );
}


void    BGTrash( BackRun& back, an node )
/************************/
{
    BGDone( back, node );
}


void    FlowOff( BackRun& back, an name )
/************************/
{
    *(name->u.b.t) = back.data.CurrBlock->label;
    *(name->u.b.f) = back.data.CurrBlock->label;
    NamesCrossBlocks(back);
}


void    BGStartBlock( BackRun& back )
/**************************/
{
    label_handle    lbl;

    if( _MemLow ) { /* break the block here and generate code*/
        lbl = AskForNewLabel(back);
        GenBlock( back, BLK_JUMP, 1 );
        AddTarget( back, lbl, false );
        Generate( back, false );
        EnLink( back, lbl );
        back.data.CurrBlock->edge[0].flags = BEF_BLOCK_LABEL_DIES;
    } else { /* check if the block is getting too big*/
        BlkTooBig(back);
    }
}
