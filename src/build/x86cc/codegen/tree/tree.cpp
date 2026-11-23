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
* Description:  Functions to manipulate the expression tree.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "tree.h"
#include "zoiks.h"
#include "freelist.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "data.h"

#include "addrfold.h"
#include "bgcall.h"
#include "bldcall.h"
#include "bldins.h"
#include "blips.h"
#include "display.h"
#include "foldins.h"
#include "inline.h"
#include "makeaddr.h"
#include "namelist.h"
#include "patch.h"
#include "rgtbl.h"
#include "stackcg.h"
#include "treeprot.h"
#include "treeconv.h"
#include "treefold.h"
#include "typemap.h"
#include "types.h"
#include "i64.h"
#include "utils.h"
#include "blips.h"
#include "typemap.h"
#include "bgcall.h"

#include "feprotos.h"
#include "cgprotos.h"

    #include "x86segs.h"
#include "backrun.hpp"


static  void    FreeTreeNode( BackRun& back, tn node );
static  void    Control( BackRun& back, cg_op opcode, tn node, label_handle lbl, bool gen );


static const type_class_def BinMat[] = {
/*********************************
 * What is the result type of a binary operation
 *
 *  U1 I1 U2 I2 U4 I4 U8 I8 CP PT FS FD FL      */
    U1,U1,U2,I2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* U1*/
    U1,I1,U2,I2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* I1*/
    U2,U2,U2,U2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* U2*/
    I2,I2,U2,I2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* I2*/
    U4,U4,U4,U4,U4,U4,U8,I8,CP,PT,FS,FD,FL,/* U4*/
    I4,I4,I4,I4,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* I4*/
    U8,U8,U8,U8,U8,U8,U8,U8,CP,PT,FD,FD,FL,/* U8*/
    I8,I8,I8,I8,I8,I8,U8,I8,CP,PT,FD,FD,FL,/* I8*/
    CP,CP,CP,CP,CP,CP,CP,CP,CP,PT,ER,ER,ER,/* CP*/
    PT,PT,PT,PT,PT,PT,PT,PT,PT,PT,ER,ER,ER,/* PT*/
    FS,FS,FS,FS,FS,FS,FD,FD,ER,ER,FS,FD,FL,/* FS*/
    FD,FD,FD,FD,FD,FD,FD,FD,ER,ER,FD,FD,FL,/* FD*/
    FL,FL,FL,FL,FL,FL,FL,FL,ER,ER,FL,FL,FL /* FL*/
};


/*
 * Routines TGxxxx build up the expression trees. TNxxx tear them down.
 */

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct TreeState: tree_state.h) */

}  // namespace


static  tn  NewTreeNode( BackRun& back )
/******************************
 * gimme a new tree node
 */
{
    tn  new_tn;

    new_tn = static_cast<tn>(AllocFrl( back, &back.tree.TreeFrl, sizeof( tree_node ) ));
    new_tn->flags = TF_USED;
    return( new_tn );
}


tn  TGLeaf( BackRun& back, an addr )
/****************************
 * create a leaf node for "addr"
 */
{
    tn  node;

    node = NewTreeNode(back);
    node->u1.t.base = NULL;
    node->u1.t.alignment = 0;
    node->u1.t.op = O_NOP;
    node->u.addr = addr;
    node->u1.t.rite = NULL;
    node->kids = 0;
    node->tipe = addr->tipe;
    node->class_ = TN_LEAF;
    if( addr->format == NF_BOOL ) {
        node->tipe = back.data.TypeBoolean;
    } else if( addr->format == NF_CONS ) {
        node->class_ = TN_CONS;
        node->u.name = addr->u.n.name;
        BGDone( back, addr );
    }
    return( node );
}


tn  TGBitMask( BackRun& back, tn left, byte start, byte len, const type_def *tipe )
/*********************************************************************
 * return a tree node for bits "start" for "len" selected from "left"
 * whose type is "tipe".  Takes and yields an lvalue.
 */
{
    tn new_tn;

    new_tn = NewTreeNode(back);
    new_tn->u.left = left;
    new_tn->tipe = tipe;
    new_tn->u1.b.start = start;
    new_tn->u1.b.len = len;
    new_tn->u1.b.is_signed = false;
    new_tn->kids = left->kids + 1;
    new_tn->class_ = TN_BIT_LVALUE;
    return( new_tn );
}


tn  TGNode( BackRun& back, tn_class class_, cg_op opcode, tn left, tn rite, const type_def *tipe )
/*****************************************************************************
 * create a general node
 */
{
    tn  node;

    node = NewTreeNode(back);
    node->u1.t.base = NULL;
    node->u1.t.alignment = 0;
    node->class_ = class_;
    node->u1.t.op = opcode;
    node->u.left = left;
    node->u1.t.rite = rite;
    node->tipe = tipe;
    node->kids = 0;
    if( rite != NULL && class_ != TN_PARM ) {
        node->kids += rite->kids + 1;
    }
    if( left != NULL ) {
        node->kids += left->kids + 1;
        node->flags |= TF_HAS_LEFT;
    }
    return( node );
}


tn  TGWarp( BackRun& back, tn before, label_handle label, tn after )
/************************************************************
 * evaluate "before", call label "label" and yield value "after".
 */
{
    tn  result;

    result = TGNode( back, TN_COLON, O_NOP, NULL, before, after->tipe );
    result->u.handle = label;
    result = TGNode( back, TN_WARP, O_NOP, result, after, after->tipe );
    return( result );
}

tn  TGHandle( BackRun& back )
/***************************
 * make a handle node - this is a leaf which holds a ptr
 */
{
    tn  node;

    node = NewTreeNode(back);
    node->u1.t.base = NULL;
    node->u1.t.alignment = 0;
    node->tipe = TypeAddress( back, TY_DEFAULT );
    node->class_ = TN_HANDLE;
    node->u1.t.op = O_NOP;
    return( node );
}

tn  TGCallback( BackRun& back, cg_callback rtn, callback_handle ptr )
/*************************************************************
 * make a special callback node which will be used to communicate with
 * the front end
 */
{
    tn      node;

    node = TGHandle(back);
    node->u.handle = ptr;
    node = TGNode( back, TN_CALLBACK, O_NOP, NULL, node, TypeAddress( back, TY_DEFAULT ) );
    node->u.left = TGHandle(back);
    node->u.left->u.callback = rtn;
    return( node );
}

static const type_def   *ResultType( BackRun& back, tn left, tn rite, const type_def *tipe,
                    const type_class_def *mat_type_class, bool demote_const )
/***********************************************************************
 * What is the resulting type of "left" opcode "rite" given that the front
 * end says it should be "tipe" (T_DEFAULT if its not sure).
 * "demote_const" is true if we are allowed to do demotion of type even
 * if the front end said it wanted a different type.  For example if
 * the front end says do ( (char)i & 3 ) as an integer, we know better.
 */
{
    tn              temp;
    const type_def  *ltipe;
    const type_def  *rtipe;

    if( !demote_const && tipe->refno != TY_DEFAULT )
        return( tipe );
    if( left->class_ == TN_CONS ) {
        temp = left;
        left = rite;
        rite = temp;
    }
    ltipe = left->tipe;
    if( ltipe == back.data.TypeBoolean ) {
        ltipe = back.data.TypeInteger;
    }
    rtipe = rite->tipe;
    if( rtipe == back.data.TypeBoolean ) {
        rtipe = back.data.TypeInteger;
    }
    if( tipe->refno == TY_DEFAULT ) {
        return( TypeOfTypeClass( back, mat_type_class[TypeClass( back, ltipe ) * XX + TypeClass( back, rtipe )] ) );
    }
    if( left->tipe->length >= tipe->length )
        return( tipe );
    if( rite->class_ != TN_CONS )
        return( tipe );
    if( left->tipe->attr & TYPE_FLOAT )
        return( tipe );
    if( rite->tipe->attr & TYPE_FLOAT )
        return( tipe );
    if( tipe->length > back.data.TypeClassSize[U4] )
        return( tipe );
    if( left->tipe->attr & TYPE_SIGNED ) {
        if( !CFSignedSize( rite->u.name->c.u.cfval, left->tipe->length ) ) {
            return( tipe );
        }
        if( rite->u.name->c.lo.u.int_value < 0 )
            return( tipe );
        if( (tipe->attr & TYPE_SIGNED) == 0 ) {
            return( tipe );
        }
    } else {
        if( !CFUnSignedSize( rite->u.name->c.u.cfval, left->tipe->length ) )  {
            return( tipe );
        }
    }
    return( left->tipe );
}

static bool RHSLongPointer( tn rite )
{
    if( rite->class_ == TN_LEAF && rite->u.addr->format == NF_ADDR ) {
        switch( rite->u.addr->class_ ) {
        case CL_ADDR_GLOBAL:
        case CL_ADDR_TEMP:
            return( true );
        default:
            break;
        }
    } else {
        switch( rite->tipe->refno ) {
        case TY_LONG_POINTER:
        case TY_HUGE_POINTER:
        case TY_LONG_CODE_PTR:
            return( true );
            break;
        }
    }
    return( false );
}


tn  TGCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/******************************************************************
 * build a relational operator node
 */
{
    tn      new_tn;
    bool    can_demote;

    can_demote = true;
    if( ( left->tipe == rite->tipe )
      && ( left->tipe != back.data.TypeBoolean )
      && ( (left->tipe->attr & ~TYPE_SIGNED) == (tipe->attr & ~TYPE_SIGNED) ) ) {
        tipe = left->tipe;
    } else {
        tipe = ResultType( back, left, rite, tipe, BinMat, can_demote );
    }
    left = TGConvert( back, left, tipe );
    rite = TGConvert( back, rite, tipe );
    new_tn = FoldCompare( back, opcode, left, rite, tipe );
    if( new_tn != NULL )
        return( new_tn );
    new_tn = FoldBitCompare( back, opcode, left, rite );
    if( new_tn != NULL )
        return( new_tn );
    new_tn = FoldPostGetsCompare( back, opcode, left, rite, tipe );
    if( new_tn != NULL )
        return( new_tn );
    new_tn = TGNode( back, TN_COMPARE, opcode, left, rite, back.data.TypeBoolean );
    return( new_tn );
}


static  an  Int( BackRun& back, uint_32 num )
/*****************************
 * return an address name for an integer
 */
{
    return( BGInteger( back, num, back.data.TypeInteger ) );
}


static  an  Int64( BackRun& back, unsigned_64 num )
/***********************************
 * return an address name for a 64-bit integer
 */
{
    return( BGInt64( back, num, back.data.TypeLongLongInteger ) );
}

uint_32    TGMask32( tn node )
/*****************************
 * return a mask of 1's in the positions a bit field occupies.
 */
{
    uint_32     mask;
    uint        len;
    uint_32     bit;

    len = node->u1.b.len;
    mask = 0;
    bit = (uint_32)1 << node->u1.b.start;
    for( ;; ) {
        mask |= bit;
        bit <<= 1;
        if( --len == 0 ) {
            break;
        }
    }
    return( mask );
}


static  unsigned_64    TGMask64( tn node )
/*****************************************
 * like TGMask32(), only 64-bit
 */
{
    unsigned_64     mask;
    uint            len;
    unsigned_64     bit;
    unsigned_64     tmp;

    len = node->u1.b.len;
    Set64ValZero( mask );
    Set64Val1p( tmp );
    U64ShiftL( &bit, &tmp, node->u1.b.start );
    for( ;; ) {
        U64Or( tmp, bit, mask );
        mask = tmp;
        U64ShiftL( &tmp, &bit, 1 );
        bit = tmp;
        if( --len == 0 ) {
            break;
        }
    }
    return( mask );
}


tn  TGConvert( BackRun& back, tn name, const type_def *tipe )
/***********************************************
 * convert "name" to "tipe".  This may require turning a short circuit
 * boolean expression into an integer 0 or 1.
 */
{
    const type_def  *node_type;
    tn              new_tn;

    node_type = name->tipe;
    new_tn = name;
    if( tipe->refno != TY_DEFAULT && tipe != node_type ) {
        if( tipe == back.data.TypeBoolean ) {
            if( node_type != back.data.TypeBoolean ) {
                new_tn = TGCompare( back, O_NE, new_tn, TGLeaf( back, Int( back, 0 ) ), node_type );
            }
        } else {
            if( node_type == back.data.TypeBoolean ) {
                new_tn = TGNode( back, TN_FLOW_OUT, O_NOP, new_tn, NULL, tipe );
            } else {
                new_tn = FoldCnvRnd( back, O_CONVERT, new_tn, tipe );
                if( new_tn == NULL ) {
                    new_tn = TGNode( back, TN_UNARY, O_CONVERT, name, NULL, tipe );
                }
            }
        }
    }
    return( new_tn );
}


static const type_def   *BinResult( BackRun& back, cg_op opcode, tn *l, tn *r, const type_def *tipe, int commie )
/*********************************************************************************
 * Calculate the resulting type of a binary operation "l" opcode "r".  Tipe
 * is what the front end thinks is should be.  Sometimes we can do
 * better.  Also, convert the operands to the type of the result if
 * necessary before the operation is performed. There are some wacky cases
 * like pointer + int where we don't want the integer operand promoted
 * prior to the operation, hence all the jiggery pokery.
 * Finally if PREGETS don't commute operands for 370
 * cause PreGets gets confused.
 */
{
    tn              rite;
    tn              left;
    tn              temp;
    const type_def  *otipe;

    /* unused parameters */ (void)commie;

    rite = *r;
    left = *l;
    switch( opcode ) {
    case O_LSHIFT:
    case O_RSHIFT:
        left = TGConvert( back, left, tipe );
        tipe = left->tipe;
        rite = TGConvert( back, rite, back.data.TypeUnsigned );
        break;
    case O_DIV:
    case O_MOD:
        otipe = tipe;
        tipe = ResultType( back, left, rite, tipe, BinMat, false );
        if( otipe == back.data.TypeNone ) { /* do integer divide to make C happy.*/
            if( tipe->refno == TY_UINT_1 || tipe->refno == TY_INT_1 ) {
                tipe = back.data.TypeInteger;
            }
        }
        left = TGConvert( back, left, tipe );
        rite = TGConvert( back, rite, tipe );
        break;
    case O_CONVERT: /* based pointer junk */
        left = TGConvert( back, left, TypeAddress( back, TY_NEAR_POINTER ) );
        if( !RHSLongPointer( rite ) ) {
            rite = TGConvert( back, rite, TypeAddress( back, TY_UINT_2 ) );
        }
        switch( tipe->refno ) {
        case TY_DEFAULT:
        case TY_NEAR_POINTER:
            tipe = TypeAddress( back, TY_LONG_POINTER );
            break;
        case TY_LONG_POINTER:
        case TY_HUGE_POINTER:
            /* OK */
            break;
        default:
            Zoiks( back, ZOIKS_082 );
            break;
        }
        break;
    case O_PLUS: /* pointer arithmetic is a pain*/
        if( left->class_ == TN_CALL ) { /* address of return value*/
            tipe = back.data.TypePtr;
            if( tipe->refno == TY_HUGE_POINTER ) {
                rite = TGConvert( back, rite, back.data.TypeHugeInteger );
            } else if( tipe->refno == TY_LONG_POINTER ) {
                rite = TGConvert( back, rite, back.data.TypeLongInteger );
            } else {
                rite = TGConvert( back, rite, back.data.TypeNearInteger );
            }
        } else {
            tipe = ResultType( back, left, rite, tipe, BinMat, false );
            if( tipe->attr & TYPE_POINTER ) {
                if( (left->tipe->attr & TYPE_POINTER) == 0 ) {
                    temp = rite;
                    rite = left;
                    left = temp;
                }
                if( tipe->refno == TY_HUGE_POINTER ) {
                    rite = TGConvert( back, rite, back.data.TypeHugeInteger );
                } else if( tipe->refno == TY_LONG_POINTER ) {
                    rite = TGConvert( back, rite, back.data.TypeLongInteger );
                } else {
                    rite = TGConvert( back, rite, back.data.TypeNearInteger );
                }
                if( left->class_ == TN_LEAF ) {
                    if( NeedPtrConvert( left->u.addr, tipe ) ) {
                        left = TGConvert( back, left, tipe );
                    }
                } else {
                    left = TGConvert( back, left, tipe );
                }
            } else { /* non pointer add */
                rite = TGConvert( back, rite, tipe );
                if( left->class_ == TN_LEAF ) {
                    if( NeedPtrConvert( left->u.addr, tipe ) ) {
                        left = TGConvert( back, left, tipe );
                    }
                } else {
                    left = TGConvert( back, left, tipe );
                }
            }
        }
        break;
    case O_MINUS:
        tipe = ResultType( back, left, rite, tipe, back.tree.SubMat, false );
        /* pointer subtraction yields a different result type than ops!*/
        if( tipe->refno == back.data.TypeHugeInteger->refno
          && left->tipe->refno == TY_HUGE_POINTER
          && rite->tipe->refno == TY_HUGE_POINTER ) {
            /* nothing*/
        } else if( tipe->refno == back.data.TypeLongInteger->refno
          && left->tipe->refno == TY_LONG_POINTER
          && rite->tipe->refno == TY_LONG_POINTER ) {
             /* nothing*/
        } else if( tipe->refno == back.data.TypeNearInteger->refno
          && left->tipe->refno == TY_NEAR_POINTER
          && rite->tipe->refno == TY_NEAR_POINTER ) {
             /* nothing*/
        } else if( tipe->refno == TY_HUGE_POINTER ) {
            rite = TGConvert( back, rite, back.data.TypeHugeInteger );
        } else if( tipe->refno == TY_LONG_POINTER ) {
            rite = TGConvert( back, rite, back.data.TypeLongInteger );
        } else if( tipe->refno == TY_NEAR_POINTER ) {
            rite = TGConvert( back, rite, back.data.TypeNearInteger );
        } else {
            rite = TGConvert( back, rite, tipe );
        }
        left = TGConvert( back, left, tipe );
        break;
    case O_TIMES:
        tipe = ResultType( back, left, rite, tipe, BinMat, false );
#if _TARGET & _TARG_8086
        if( tipe->refno == TY_INT_4
          && left->tipe->length <= 2
          && rite->tipe->length <= 2 ) {
            left = TGConvert( back, left, back.data.TypeInteger );
        } else {
            left = TGConvert( back, left, tipe );
            rite = TGConvert( back, rite, tipe );
        }
#else
        left = TGConvert( back, left, tipe );
        rite = TGConvert( back, rite, tipe );
#endif
        break;
    case O_AND:
    case O_OR:
    case O_XOR:
        tipe = ResultType( back, left, rite, tipe, BinMat, true );
        left = TGConvert( back, left, tipe );
        rite = TGConvert( back, rite, tipe );
        break;
    case O_POW:
    case O_ATAN2:
    case O_FMOD:
        tipe = ResultType( back, left, rite, tipe, BinMat, false );
        left = TGConvert( back, left, tipe );
        rite = TGConvert( back, rite, tipe );
        break;
    case O_COMMA:
        tipe = ResultType( back, left, rite, tipe, BinMat, true );
        break;
    case O_SIDE_EFFECT:
        tipe = ResultType( back, rite, left, tipe, BinMat, true );
        break;
    default:
        _Zoiks( ZOIKS_054 );
        break;
    }
    *l = left;
    *r = rite;
    return( tipe );
}


static  tn  BinFold( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/**********************************************************************
 * Try to fold "left" "opcode" "rite".  Return NULL if it is not possible,
 * a new tree node if it is possible (freeing "left" and "rite").
 */
{
    switch( opcode ) {
    case O_PLUS:
        return( FoldPlus( back, left, rite, tipe ) );
    case O_MINUS:
        return( FoldMinus( back, left, rite, tipe ) );
    case O_TIMES:
        return( FoldTimes( back, left, rite, tipe ) );
    case O_DIV:
        return( FoldDiv( back, left, rite, tipe ) );
    case O_MOD:
        return( FoldMod( back, left, rite, tipe ) );
    case O_LSHIFT:
        return( FoldLShift( back, left, rite, tipe ) );
    case O_RSHIFT:
        return( FoldRShift( back, left, rite, tipe ) );
    case O_AND:
        return( FoldAnd( back, left, rite, tipe ) );
    case O_OR:
        return( FoldOr( back, left, rite, tipe ) );
    case O_XOR:
        return( FoldXor( back, left, rite, tipe ) );
    case O_COMMA:
        return( TGNode( back, TN_COMMA, O_NOP, left, rite, rite->tipe ) );
    case O_SIDE_EFFECT:
        return( TGNode( back, TN_SIDE_EFFECT, O_NOP, left, rite, left->tipe ) );
    case O_POW:
        return( FoldPow( back, left, rite, tipe ) );
    default:
        break;
    }
    return( NULL );
}


tn  TGBinary( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/***************************************************************
 * build a binary operator tree node
 */
{
    tn      result;
    tn      l;
    tn      r;

    l = left;
    r = rite;
    tipe = BinResult( back, opcode, &l, &r, tipe, true );
    result = BinFold( back, opcode, l, r, tipe );
    if( result == NULL ) {
        result = TGNode( back, TN_BINARY, opcode, l, r, tipe );
    }
    return( result );
}


tn  TGUnary( BackRun& back, cg_op opcode, tn left, const type_def *tipe )
/*****************************************************
 * build a unary operator tree node
 */
{
    tn  new_tn;

    new_tn = NULL;

    if( opcode != O_POINTS ) { /* for O_POINTS, the tipe given is always correct*/
        if( tipe == back.data.TypeNone ) {
            tipe = left->tipe;
        }
    }

    switch( opcode ) {
    case O_UMINUS:
        left = TGConvert( back, left, tipe );
        new_tn = FoldUMinus( back, left, tipe );
        break;
    case O_COMPLEMENT:
        left = TGConvert( back, left, tipe );
        new_tn = Fold1sComp( back, left, tipe );
        break;
    case O_POINTS:
        if( left->class_ == TN_BIT_LVALUE ) {
            new_tn = left;
            new_tn->class_ = TN_BIT_RVALUE;
            if( tipe->attr & TYPE_SIGNED ) {
                new_tn->u1.b.is_signed = true;
            }
        }
        break;
    case O_ROUND:
        new_tn = FoldCnvRnd( back, opcode, left, tipe );
        break;
    case O_CONVERT:
        new_tn = FoldCnvRnd( back, opcode, left, tipe );
        if( new_tn == NULL ) {
            new_tn = TGConvert( back, left, tipe );
        }
        break;
    case O_SQRT:
        left = TGConvert( back, left, tipe );
        new_tn = FoldSqrt( back, left, tipe );
        break;
    case O_LOG:
    case O_LOG10:
        left = TGConvert( back, left, tipe );
        new_tn = FoldLog( back, opcode, left, tipe );
        break;
    case O_COS:
    case O_SIN:
    case O_TAN:
    case O_FABS:
    case O_ACOS:
    case O_ASIN:
    case O_ATAN:
    case O_COSH:
    case O_SINH:
    case O_TANH:
    case O_EXP:
        left = TGConvert( back, left, tipe );
        break;
    case O_PARENTHESIS:
    /*
     * This is to stop optimizations from happening across the tree
     * such as constant folding and the like. Since the code generator
     * does not currently do anything like that, we can just NOP the
     * sucker. If we start getting more agressive, this will have to
     * actually do something.
     */
        new_tn = left;
        break;
    case O_PTR_TO_NATIVE:
    case O_PTR_TO_FOREIGN:
        break;
    case O_STACK_ALLOC:
        break;
    default:
        _Zoiks( ZOIKS_055 );
        break;
    }
    if( new_tn == NULL ) {
        new_tn = TGNode( back, TN_UNARY, opcode, left, NULL, tipe );
    }
    return( new_tn );
}

call_handle TGInitCall( BackRun& back, tn left, const type_def *tipe, cg_sym_handle sym )
/*************************************************************************
 * Return a tree node for a call to "left".  TGAddParm may add parms to
 * the call node.  TGCall finalizes the call node.
 */
{
    tn      node;

    node = TGNode( back, TN_PARM, O_NOP, left, (tn)sym, NULL );
    node = TGNode( back, TN_CALL, O_NOP, node, NULL, tipe );
    if( (call_class)(pointer_uint)FindAuxInfoSym( sym, FEINF_CALL_CLASS ) & FECALL_GEN_REVERSE_PARMS ) {
        node->flags |= TF_REVERSE;
    }
    return( node );
}


tn  TGAddParm( BackRun& back, call_handle call, tn parm, const type_def *tipe )
/***************************************************************
 * see TGInitCall ^
 */
{
    tn  new_tn;
    tn  scan;

    if( tipe->refno == TY_DEFAULT ) {
        tipe = parm->tipe;
        if( tipe == back.data.TypeBoolean ) {
            tipe = back.data.TypeInteger;
        }
    }
    parm = TGConvert( back, parm, tipe );
    new_tn = TGNode( back, TN_PARM, O_NOP, parm, NULL, tipe );
    if( call->flags & TF_REVERSE ) {
        new_tn->u1.t.rite = call->u1.t.rite;
        call->u1.t.rite = new_tn;
    } else {
        for( scan = call; scan->u1.t.rite != NULL; ) {
            scan = scan->u1.t.rite;
        }
        scan->u1.t.rite = new_tn;
    }
    return( call );
}


tn  TGCall( call_handle call )
/*****************************
 * see TGInitCall ^
 */
{
    return( call );
}


tn  TGIndex( BackRun& back, tn left, tn rite, const type_def *tipe, const type_def *ptipe )
/***************************************************************************
 * return a tree for &left[rite].  "ptipe" is the pointer type of
 * "left".  "tipe" is the type of the object pointed to by "left".
 * Resulting node is a pointer node.
 */
{
    /*
     * For machines with an indexed addressing mode (like the VAX)
     * the following would probably not be done
     */
    if( tipe->length != 1 ) {
        if( ptipe->refno == TY_HUGE_POINTER ) {
            rite = CGBinary( back, O_TIMES, rite,
                  CGInteger( back, tipe->length, TY_INTEGER ), TY_INT_4 );
        } else {
            rite = CGBinary( back, O_TIMES, rite,
                  CGInteger( back, tipe->length, TY_INTEGER ), TY_INTEGER );
        }
    }
    rite = CGBinary( back, O_PLUS, left, rite, ptipe->refno );
    return( rite );
}


tn  DoTGAssign( BackRun& back, tn dst, tn src, const type_def *tipe, tn_class class_ )
/*********************************************************************
 * build dst = src
 */
{
    tn              node;
    const type_def  *node_tipe;

    if( tipe->refno == TY_DEFAULT ) {
        tipe = src->tipe;
    }
    src = TGConvert( back, src, tipe );
    node_tipe = tipe;
    if( class_ == TN_LV_ASSIGN ) {
        node_tipe = dst->tipe;
    }
    node = TGNode( back, class_, O_NOP, dst, src, node_tipe );
    node->optipe = tipe;
    return( node );
}


tn  TGAssign( BackRun& back, tn dst, tn src, const type_def *tipe )
/***************************************************
 * build dst = src
 */
{
    return( DoTGAssign( back, dst, src, tipe, TN_ASSIGN ) );
}


tn  TGLVAssign( BackRun& back, tn dst, tn src, const type_def *tipe )
/*****************************************************
 * build dst = src
 */
{
    return( DoTGAssign( back, dst, src, tipe, TN_LV_ASSIGN ) );
}


bool    TGCanDuplicate( tn node )
/****************************************
 * return true if node safe to duplicate? (Has no side effects)
 */
{
    switch( node->class_ ) {
    case TN_LEAF:
    case TN_CONS:
        return( true );
    case TN_UNARY:
    case TN_BIT_LVALUE:
    case TN_BIT_RVALUE:
        return( TGCanDuplicate( node->u.left ) );
    case TN_BINARY:
        if( !TGCanDuplicate( node->u.left ) )
            return( false );
        if( !TGCanDuplicate( node->u1.t.rite ) )
            return( false );
        return( true );
    default:
        return( false );
    }
}


name    *TGetName( BackRun& back, tn node )
/***********************************
 * given a leaf/cons node, return the associated "name"
 */
{
    name    *opnd;

    opnd = NULL;
    if( node->class_ == TN_CONS ) {
        opnd = node->u.name;
    } else if( node->class_ == TN_LEAF ) {
        opnd = AddrToName( node->u.addr );
    }
    BurnTree( back, node );
    return( opnd );
}


tn  TGReLeaf( BackRun& back, an addr )
/******************************
 * create another leaf node for "addr".  This means it has been used
 * twice in the same expression tree, and cannot be a "push/pop" style
 * temporary.
 */
{
    tn      node;

    addr->flags |= FL_NEVER_STACK;
    node = TGLeaf( back, addr );
    node->u1.t.base = addr->u.n.base;
    return( node );
}


tn  TGTmpLeaf( BackRun& back, an addr )
/**********************/
{
    name    *base;

    base = addr->u.n.base;
    addr = AddrEval( back, addr );
    addr->u.n.base = base;
    return( TGReLeaf( back, addr ) );
}

tn  TGConst( BackRun& back, float_handle cons, const type_def *tipe )
/*******************************************************
 * return a leaf node for "cons" (a cfloat pointer)
 */
{
    return( TGLeaf( back, MakeConst( back, cons, tipe ) ) );
}


tn  TName( BackRun& back, name *name, const type_def *tipe )
/********************************************
 * return a leaf node for "name"
 */
{
    return( TGLeaf( back, AddrName( back, name, tipe ) ) );
}


tn  TGDuplicate( BackRun& back, tn node )
/*********************************
 * Return a duplicate for tree "node"
 */
{
    tn  new_tn;

    if( node != NULL ) {
        switch( node->class_ ) {
        case TN_CONS:
            new_tn = TGConst( back, CFCopy( back, &back.cfloat.cgh, node->u.name->c.u.cfval ), node->tipe );
            break;
        case TN_LEAF:
            new_tn = TGLeaf( back, AddrCopy( back, node->u.addr ) );
            break;
        case TN_BIT_LVALUE:
        case TN_BIT_RVALUE:
            new_tn = TGBitMask( back, TGDuplicate( back, node->u.left ), node->u1.b.start, node->u1.b.len, node->tipe );
            break;
        default:
            new_tn = TGNode( back, node->class_, node->u1.t.op, TGDuplicate( back, node->u.left ),
                    TGDuplicate( back, node->u1.t.rite ), node->tipe );
            break;
        }
        new_tn->class_ = node->class_;
    } else {
        new_tn = NULL;
    }
    return( new_tn );
}


tn  DoTGPreGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe,
                 tn_class class_, tn_class assn_class )
/*******************************************************************
 * Build a node for left op= right.  We try to turn it into "left =
 * left op rite" by duplicating the node for left, (but we can't do
 * this if left contains a call), and then constant fold "left op
 * rite".  If this works out, it'll turn into a straight assignment.
 * Notice that "Pre" refers to the position of the operator (++x), not
 * whether we are yielding the value before the operation.
 */
{
    tn              leftp;
    tn              result;
    tn              dupleft;
    const type_def  *optipe;
    tn              l;
    tn              r;

    switch( opcode ) {
    case O_DIV:
    case O_MOD:
    case O_RSHIFT:
        break;
    default:
        TGDemote( back, rite, tipe );
        break;
    }
    if( TGCanDuplicate( left ) ) {
        dupleft = TGDuplicate( back, left );
    } else {
        dupleft = NULL;
    }
    leftp = TGUnary( back, O_POINTS, left, tipe );
    l = leftp;
    r = rite;
    /* ok to use defaults here since we're assining the result to left*/
    optipe = BinResult( back, opcode, &l, &r, back.data.TypeNone, false );
    rite = r;
    leftp = l;
    if( dupleft == NULL ) {
        result = NULL;
    } else {
        result = BinFold( back, opcode, leftp, rite, optipe );
    }
    if( result == NULL ) {
        // rite = TGUnary( O_CONVERT, rite, tipe );
        result = TGNode( back, class_, opcode, left, rite, tipe );
        result->optipe = optipe;
        if( tipe != optipe ) { /* someone might have put a convert onto leftp*/
            if( leftp != left ) {
                if( leftp->u.left != left ) {
                    FreeTreeNode( back, leftp->u.left );
                }
                FreeTreeNode( back, leftp );
            }
        } else if( leftp != left ) {
            FreeTreeNode( back, leftp );
        }
        if( left->class_ == TN_BIT_RVALUE ) {
            left->class_ = TN_BIT_LVALUE;
        }
        if( dupleft != NULL ) {
            BurnTree( back, dupleft );
        }
    } else if( result->class_ == TN_BINARY
      && result->u.left == leftp
      && leftp->class_ == TN_UNARY
      && leftp->u1.t.op == O_POINTS
      && leftp->u.left == left ) {
        result->class_ = class_;
        result->u.left = left;
        result->optipe = optipe;
        if( left->class_ == TN_BIT_RVALUE ) {
            left->class_ = TN_BIT_LVALUE;
        } else {
            FreeTreeNode( back, leftp );
        }
        BurnTree( back, dupleft );
    } else {
        result = DoTGAssign( back, dupleft, result, tipe, assn_class );
        if( leftp == left ) {
            result->u.left->class_ = TN_BIT_LVALUE;
        }
    }
    return( result );
}


tn  TGPreGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/*******************************************************************/
{
    return( DoTGPreGets( back, opcode, left, rite, tipe, TN_PRE_GETS, TN_ASSIGN ) );
}


tn  TGLVPreGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/*********************************************************************/
{
    return( DoTGPreGets( back, opcode, left, rite, tipe, TN_LV_PRE_GETS, TN_LV_ASSIGN ) );
}


tn  TGPostGets( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/*******************************************************************
 * node for left op= right, but yields the rvalue of left before the
 * assignment took place.  (for x++) Notice that "Post" refers to the
 * position of the operator (x++), not whether we are yielding the
 * value after the operation.
 */
{
    rite = TGConvert( back, rite, tipe );
    return( TGNode( back, TN_POST_GETS, opcode, left, rite, tipe ) );
}


cg_type TGType( tn node )
/***********************/
{
    return( node->tipe->refno );
}


tn  TGPatch( BackRun& back, patch_handle patch, const type_def *tipe )
/******************************************************
 * create a patch node for "hdl"
 */
{
    tn  node;

    node = NewTreeNode(back);
    node->u1.t.base = NULL;
    node->u1.t.alignment = 0;
    node->u1.t.op = O_NOP;
    node->u.handle = patch;
    node->u1.t.rite = NULL;
    node->kids = 0;
    node->tipe = tipe;
    node->class_ = TN_PATCH;
    return( node );
}

tn  TGFlow( BackRun& back, cg_op opcode, tn left, tn rite )
/***********************************************
 * create a short circuit boolean expression node
 */
{
    tn  result;

    switch( opcode ) {
    case O_FLOW_AND:
        result = FoldFlAnd( back, left, rite );
        break;
    case O_FLOW_OR:
        result = FoldFlOr( back, left, rite );
        break;
    case O_FLOW_NOT:
        result = FoldFlNot( back, left );
        break;
    default:
        result = NULL;
        _Zoiks( ZOIKS_056 );
        break;
    }
    if( result != NULL )
        return( result );
    left = TGConvert( back, left, back.data.TypeBoolean );
    if( rite != NULL ) {  /* O_FLOW_NOT*/
        rite = TGConvert( back, rite, back.data.TypeBoolean );
    }
    return( TGNode( back, TN_FLOW, opcode, left, rite, back.data.TypeBoolean ) );
}


tn  TGTrash( tn node )
/*****************************
 * evaluate "node", then throw away the resulting value
 */
{
    node->flags &= ~TF_USED;
    if( node->class_ == TN_UNARY ) {
        if( node->u1.t.op == O_POINTS ) {
            node->u.left->flags &= ~TF_USED;
        }
    }
    return( node );
}


tn   TGAttr( tn node, cg_sym_attr attr )
/**********************************************
 * mark the tree node as having a particular attribute.
 * It must be a pointer to the location with the desired attr.
 */
{
    switch( attr ) {
    case CG_SYM_VOLATILE:
        node->flags |= TF_VOLATILE;
        break;
    case CG_SYM_CONSTANT:
        node->flags |= TF_CONSTANT;
        break;
    case CG_SYM_UNALIGNED:
        node->u1.t.alignment = 1;
        break;
    }
    return( node );
}

tn  TGAlign( tn node, uint align )
/*****************************************
 * mark the tree node as aligned on an 'align' byte boundary.
 * This must be done just prior to a fetch or LVAssign of some
 * form.
 */
{
    node->u1.t.alignment = align;
    return( node );
}


tn  TGVolatile( tn node )
/********************************
 * mark the tree node as volatile.  It must be a pointer to the
 * volatile location. Here for history's sake.
 */
{
    return( TGAttr( node, CG_SYM_VOLATILE ) );
}


static  bool    SetAddress( BackRun& back, bool value )
/***************************************
 * Indicate we're processing an address expression.  Return the old
 * value of IsAddress.
 */
{
    bool    old_value;

    old_value = back.tree.IsAddress;
    back.tree.IsAddress = value;
    return( old_value );
}


static  an  NotAddrGen( BackRun& back, tn node )
/********************************
 * generate a node which is NOT an address expression.
 */
{
    bool    was_address;
    an      retv;

    was_address = SetAddress( back, false );
    retv = TreeGen( back, node );
    SetAddress( back, was_address );
    return( retv );
}


void    TG3WayControl( BackRun& back, tn node, label_handle lt, label_handle eq, label_handle gt )
/*******************************************************
 * for FORTRAN if( x ) 10,20,30
 */
{
    BG3WayControl( back, NotAddrGen( back, node ), lt, eq, gt );
}


void    TGControl( BackRun& back, cg_op opcode, tn node, label_handle lbl )
/***************************************************************
 * generate a simple flow of control. The tree must be complete when this is called.
 */
{
    switch( opcode ) {
    case O_IF_TRUE:
        if( !FoldIfTrue( back, node, lbl ) ) {
            Control( back, opcode, node, lbl, true );
        }
        break;
    case O_IF_FALSE:
        if( !FoldIfFalse( back, node, lbl ) ) {
            Control( back, opcode, node, lbl, true );
        }
        break;
    default:
        BGGenCtrl( back, opcode, NULL, lbl, true );
        break;
    }
}


static  void    Control( BackRun& back, cg_op opcode, tn node, label_handle lbl, bool gen )
/***********************************************************************
 * see TGControl ^
 */
{
    an  addr;

    addr = NotAddrGen( back, TGConvert( back, node, back.data.TypeBoolean ) );
    if( addr->format == NF_CONS ) { /* will either be NF_CONS or NF_BOOL*/
        if( CFTest( addr->u.n.name->c.u.cfval ) != 0 ) {
            if( opcode == O_IF_TRUE ) {
                BGGenCtrl( back, O_GOTO, NULL, lbl, gen );
            }
        } else {
            if( opcode == O_IF_FALSE ) {
                BGGenCtrl( back, O_GOTO, NULL, lbl, gen );
            }
        }
        BGDone( back, addr );
    } else {
        BGGenCtrl( back, opcode, addr, lbl, gen );
    }
}


static  name    *TNGetLeafName( BackRun& back, tn node )
/****************************************
 * given leaf "node", return the "name" associated with it.
 */
{
    an      addr;

    addr = node->u.addr;
    if( addr->format != NF_ADDR )
        return( NULL );
    switch( addr->class_ ) {
    case CL_ADDR_TEMP:
    case CL_ADDR_GLOBAL:
        return( addr->u.n.name );
    case CL_POINTER:
        if( _IsModel( CGSW_GEN_FORTRAN_ALIASING ) )
            return( addr->u.n.name );
        return( NULL );
    default:
        return( NULL );
    }
}


static  name *TNFindBase( BackRun& back, tn node )
/*****************************************
 * Given "node", return the "name" which is the base of the expression.
 * For example, the base of x[i+j] is x.  We use this base for aliasing
 * information.  In fortran, a variable that has had its address taken
 * may only be modified by a routine call if the address (base) is
 * passed directly to the routine.  We generate an instruction that
 * simulates this modification for live information and don't assume
 * that it is destroyed by all calls. Go left first for comma operators,
 * since fortran generates stuff like  *(c,x) = 7. This means *x = 7,
 * but I'm really modifying c.
 */
{
    name    *op;

    if( node == NULL )
        return( NULL );
    if( node->u1.t.base != NULL )
        return( node->u1.t.base );
    switch( node->class_ ) {
    case TN_LEAF:
        return( TNGetLeafName( back, node ) );
    case TN_COMMA:
    case TN_SIDE_EFFECT:
        if( (node->tipe->attr & TYPE_POINTER) == 0 )
            return( NULL );
        op = static_cast<name *>(SafeRecurseCG( back, (func_sr)TNFindBase, ( node->class_ == TN_COMMA ) ? node->u.left : node->u1.t.rite ));
        if( op != NULL )
            return( op );
        /* fall through */
    case TN_BINARY:
        // creating a based pointer via a binary convert - don't want a fake base
        if( node->class_ == TN_BINARY && node->u1.t.op == O_CONVERT )
            return( NULL );
        /* fall through */
    case TN_LV_ASSIGN:
    case TN_LV_PRE_GETS:
        if( (node->tipe->attr & TYPE_POINTER) == 0 )
            return( NULL );
        op = static_cast<name *>(SafeRecurseCG( back, (func_sr)TNFindBase, node->u1.t.rite ));
        if( op != NULL )
            return( op );
        if( node->u1.t.op == O_CONVERT )
            return( NULL );
        if( node->class_ != TN_BINARY )
            return( NULL );
        return static_cast<name *>(( SafeRecurseCG( back, (func_sr)TNFindBase, node->u.left ) ));
    case TN_UNARY:
        if( (node->tipe->attr & TYPE_POINTER) == 0 )
            return( NULL );
        switch( node->u1.t.op ) {
        case O_PTR_TO_NATIVE:
        case O_PTR_TO_FOREIGN:
        case O_CONVERT:
            return static_cast<name *>(( SafeRecurseCG( back, (func_sr)TNFindBase, node->u.left ) ));
        default:
            break;
        }
        if( _IsntModel( CGSW_GEN_FORTRAN_ALIASING ) )
            return( NULL );
        if( node->u1.t.op != O_POINTS )
            return( NULL );
        switch( node->u.left->class_ ) {
        case TN_SIDE_EFFECT:
            return static_cast<name *>(( SafeRecurseCG( back, (func_sr)TNFindBase, node->u.left->u1.t.rite ) ));
        case TN_COMMA:
            return static_cast<name *>(( SafeRecurseCG( back, (func_sr)TNFindBase, node->u.left->u.left ) ));
        case TN_LV_ASSIGN:
        case TN_LV_PRE_GETS:
            return static_cast<name *>(( SafeRecurseCG( back, (func_sr)TNFindBase, node->u.left->u1.t.rite ) ));
        default:
            break;
        }
        node = node->u.left;
        if( node->class_ != TN_LEAF )
            return( NULL );
        op = TNGetLeafName( back, node );
        if( op == NULL )
            return( NULL );
        if( op->n.class_ == N_TEMP ) {
            if( op->v.symbol == NULL )
                return( NULL );
            op = SAllocUserTemp( back, FEAuxInfo( *back.front, op->v.symbol, FEINF_SHADOW_SYMBOL ), op->n.type_class, op->n.size );
        }
        return( op );
    default:
        break;
    }
    return( NULL );
}

an  TGen( BackRun& back, tn node, const type_def *tipe )
/******************************************
 * generate basic blocks (call BG routines) for "node"
 */
{
    an      retv;
    name    *base;

    TGBlip();
    base = TNFindBase( back, node );
    node = TGConvert( back, node, tipe );
    retv = TreeGen( back, node );
    if( retv->format != NF_BOOL ) {
        retv->flags &= ~FL_STACKABLE;
        retv->u.n.base = base;
    }
    return( retv );
}


an  TGReturn( BackRun& back, tn node, const type_def *tipe )
/**********************************************
 * make the current procedure return the value of "node"
 */
{
    an  retv;

    retv = TGen( back, node, tipe );
    retv->flags |= FL_STACKABLE;
    return( retv );
}


static  an  AddrGen( BackRun& back, tn node )
/*****************************
 * generate a tree node which we know is an address expression.  (may
 * be folded by AddrPlus and friends)
 */
{
    bool        was_address;
    an          retv;
    name        *base;
    tn_flags    flags;
    type_length alignment;

    was_address = SetAddress( back, true );
    base = TNFindBase( back, node );
    flags = node->flags;
    alignment = node->u1.t.alignment;
    retv = TreeGen( back, node );
    if( flags & TF_VOLATILE ) {
        if( retv->format != NF_BOOL ) {
            retv->flags |= FL_VOLATILE;
        }
    }
    if( alignment != 0 ) {
        if( retv->format != NF_BOOL ) {
            retv->u.n.alignment = alignment;
        }
    }
    if( retv->format != NF_BOOL )
        retv->u.n.base = base;
    SetAddress( back, was_address );
    return( retv );
}


static  an  TNCallback( BackRun& back, tn node )
/********************************
 * call the front end supplied function
 */
{
    cg_callback     rtn;
    callback_handle parm;

    rtn = node->u.left->u.callback;
    parm = (callback_handle)node->u1.t.rite->u.handle;
    if( rtn != NULL ) {
        rtn( back, parm );
    }
    FreeTreeNode( back, node->u.left );
    FreeTreeNode( back, node->u1.t.rite );
    return( BGInteger( back, 0, TypeAddress( back, TY_UINT_2 ) ) );
}


an  TNFlow( BackRun& back, tn node )
/********************
 * generate basic blocks for a flow of control node
 */
{
    an      retv;
    an      left;
    an      rite;

    rite = NULL;
    left = NotAddrGen( back, node->u.left );
    if( node->u1.t.rite != NULL ) {
        rite = NotAddrGen( back, node->u1.t.rite );
    }
    retv = BGFlow( back, node->u1.t.op, left, rite );
    return( retv );
}


#if _TARGET != _TARG_8086
    #define JUST_USE_SHIFTS
#endif

static  an  TNBitShift( BackRun& back, an retv, tn node, bool already_masked )
/**************************************************************
 * Yield the integer value of the bit field of rvalue "retv" based on
 * the bit field specification in "node". For an unsigned bit field
 * we can leave the high order bits turned on.
 */
{
    const type_def  *tipeu;
    const type_def  *tipes;
    unsigned_64     mask;

    tipeu = node->tipe;
    switch( tipeu->length ) {
    case 1:
        tipeu = TypeAddress( back, TY_UINT_1 );
        tipes = TypeAddress( back, TY_INT_1 );
        break;
    case 2:
        tipeu = TypeAddress( back, TY_UINT_2 );
        tipes = TypeAddress( back, TY_INT_2 );
        break;
    case 4:
        tipeu = TypeAddress( back, TY_UINT_4 );
        tipes = TypeAddress( back, TY_INT_4 );
        break;
    case 8:
        tipeu = TypeAddress( back, TY_UINT_8 );
        tipes = TypeAddress( back, TY_INT_8 );
        break;
    default:
        tipes = NULL;
        _Zoiks( ZOIKS_092 );
        break;
    }
    mask = TGMask64( node );
#ifdef JUST_USE_SHIFTS
    if( !node->u1.b.is_signed && node->u1.b.start == 0 ) {
        if( !already_masked ) {
            retv = BGBinary( back, O_AND, retv, Int64( back, mask ), tipeu, true );
        }
    } else {
        retv = BGBinary( back, O_LSHIFT, retv, Int( back, 8*tipeu->length - node->u1.b.len - node->u1.b.start ), tipeu, true );
        retv = BGBinary( back, O_RSHIFT, retv, Int( back, 8*tipeu->length - node->u1.b.len ), node->u1.b.is_signed ? tipes : tipeu, true );
    }
#else
    if( node->u1.b.is_signed ) {
        retv = BGBinary( back, O_RSHIFT, retv, Int( back, node->u1.b.start ), tipeu, true );
        U64ShiftREq( &mask, node->u1.b.start );
        retv = BGBinary( back, O_AND, retv, Int64( back, mask ), tipeu, true );
        U64ShiftREq( &mask, 1 );
        U64NotEq( mask );
        if( U64Low( mask ) == 0xffffffff ) { /* a one-bit signed bit field */
            signed_64 one;

            Set64Val1p( one );
            retv = BGUnary( back, O_COMPLEMENT, retv, tipeu );
            retv = BGBinary( back, O_PLUS, retv, Int64( back, one ), tipes, true );
        } else if( U64Low( mask ) == 0xfffff80 ) { /* an eight-bit signed bit field */
            switch( tipeu->length ) {
            case 1:
                break;
            case 2:
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_1 ) );
                break;
            case 4:
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_1 ) );
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_2 ) );
                break;
            case 8:
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_1 ) );
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_2 ) );
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_4 ) );
                break;
            }
            retv = BGConvert( back, retv, tipes );
        } else if( U64Low( mask ) == 0xffff8000 ) { /* a sixteen-bit signed bit field */
            switch( tipeu->length ) {
            case 2:
                break;
            case 4:
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_2 ) );
                break;
            case 8:
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_2 ) );
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_4 ) );
                break;
            }
            retv = BGConvert( back, retv, tipes );
        } else if( U64Low( mask ) == 0x80000000 ) { /* a 32-bit signed bit field */
            switch( tipeu->length ) {
            case 4:
                break;
            case 8:
                retv = BGConvert( back, retv, TypeAddress( back, TY_INT_4 ) );
                break;
            }
            retv = BGConvert( back, retv, tipes );
        } else {
            retv = BGBinary( back, O_XOR, retv, Int64( back, mask ), tipeu, true );
            retv = BGBinary( back, O_MINUS, retv, Int64( back, mask ), tipes, true );
        }
    } else {
        retv = BGBinary( back, O_RSHIFT, retv, Int( back, node->u1.b.start ), node->tipe, true );
        if( !already_masked && ( node->tipe->length * 8 - node->u1.b.start - node->u1.b.len != 0 )) {
            U64ShiftREq( &mask, node->u1.b.start );
            retv = BGBinary( back, O_AND, retv, Int64( back, mask ), node->tipe, true );
        }
    }
#endif
    return( retv );
}


static  an  TNBitRVal( BackRun& back, an retv, tn node )
/****************************************
 * Yield the integer value of the bit field of lvalue "retv" based on
 * the bit field specification in "node".
 */
{
    retv = BGUnary( back, O_POINTS, retv, node->tipe );
    retv = TNBitShift( back, retv, node, false );
    return( retv );
}


static  void    DoAnd( BackRun& back, an left, unsigned_64 mask, tn node )
/**********************************************************
 * Turn off bits "mask" in address name "left"
 */
{
    BGDone( back, BGOpGets( back, O_AND, AddrCopy( back, left ), Int( back, ~(U64Low( mask )) ),
              node->tipe, node->tipe ) );
}

static  void    DoAnd64( BackRun& back, an left, unsigned_64 mask, tn node )
/************************************************************
 * Turn off bits "mask" in address name "left"
 */
{
    unsigned_64     tmp;

    U64Not( tmp, mask );
    BGDone( back, BGOpGets( back, O_AND, AddrCopy( back, left ), Int64( back, tmp ),
              node->tipe, node->tipe ) );
}

static  an  TNBitOpGets( BackRun& back, tn node, const type_def *tipe, bool yield_before_op )
/***********************************************************************
 * Do a bit field assgnment, like a += b, or a = b or b++.  If
 * "yield_before_op" is true, we want to yield the value of the bit
 * field before the assignment.  Assignment is accomplished by
 * extracting the RHS into an integer, shifting it about and then ORing
 * it into the destination. "tipe" is the actual tipe which should be used
 * for the operation.
 */
{
    an          left;
    an          rite;
    an          before_value;
    an          after_value;
    an          retv;
    an          free_retv;
    tn          lhs;
    uint        shift;
    unsigned_64 mask;
    unsigned_64 shiftmask;

    lhs = node->u.left;
    left = AddrGen( back, lhs->u.left );
    before_value = TNBitRVal( back, AddrCopy( back, left ), lhs );
    rite = NotAddrGen( back, node->u1.t.rite );
    if( node->u1.t.op != O_NOP ) {
        // convert everything - better safe than sorry
        after_value = BGBinary( back, node->u1.t.op,
                BGConvert( back, BGDuplicate( back, before_value ), tipe ),
                BGConvert( back, rite, tipe ),
                tipe, true );
    } else {
        after_value = rite;
    }
    after_value = BGConvert( back, after_value, node->tipe );
    mask = TGMask64( lhs );
    shift = lhs->u1.b.start;
    FreeTreeNode( back, lhs );
    U64ShiftR( &shiftmask, &mask, shift );  // shiftmask = mask >> shift;
    if( after_value->format == NF_CONS && after_value->class_ == CL_CONS2 ) {
        retv = Int( back, U64Low( shiftmask ) & (uint_32)after_value->u.n.name->c.lo.u.int_value );
        if( (uint_32)retv->u.n.name->c.lo.u.int_value != U64Low( shiftmask ) ) {
            DoAnd( back, left, mask, node );
        }
        if( retv->u.n.name->c.lo.u.int_value != 0 ) {
            free_retv = retv;
            retv = Int( back, retv->u.n.name->c.lo.u.int_value << shift );
            BGDone( back, free_retv );
            retv = BGOpGets( back, O_OR, left, retv, node->tipe, node->tipe );
        } else {
            BGDone( back, left );
        }
        BGDone( back, retv );
    } else {
        retv = BGBinary( back, O_AND, AddrCopy( back, after_value ), Int64( back, shiftmask ), node->tipe, true );
        DoAnd64( back, left, mask, node );
        retv = BGBinary( back, O_LSHIFT, retv, Int( back, shift ), node->tipe, true );
        retv = BGOpGets( back, O_OR, left, retv, node->tipe, node->tipe );
        BGDone( back, retv );
    }
    if( yield_before_op ) {
        BGDone( back, after_value );
        retv = before_value;
    } else {
        BGDone( back, before_value );
        retv = BGBinary( back, O_AND, after_value, Int64( back, shiftmask ), node->tipe, true );
    }
    return( retv );
}


an  TNPostGets( BackRun& back, tn node )
/************************
 * generate blocks for an x++ type node
 */
{
    an      retv;
    an      left;
    an      rite;
    an      leftp;

    if( node->u.left->class_ == TN_BIT_LVALUE ) {
        retv = TNBitOpGets( back, node, node->tipe, true );
    } else {
        left = AddrGen( back, node->u.left );
        rite = NotAddrGen( back, node->u1.t.rite );
        retv = MakeTempAddr( back, BGNewTemp( back, node->tipe ) );
        leftp = BGUnary( back, O_POINTS, AddrCopy( back, left ), node->tipe );
        retv = BGAssign( back, retv, leftp, node->tipe );
        BGDone( back, BGOpGets( back, node->u1.t.op, left, rite, node->tipe, node->tipe ) );
    }
    return( retv );
}


an  TNPreGets( BackRun& back, tn node )
/***********************
 * generate a += *= type node
 */
{
    an      retv;
    an      left;
    an      rite;

    if( node->u.left->class_ == TN_BIT_LVALUE ) {
        retv = TNBitOpGets( back, node, node->optipe, false );
    } else {
        retv = NULL;
        left = AddrGen( back, node->u.left );
        rite = NotAddrGen( back, node->u1.t.rite );
        if( node->class_ == TN_LV_PRE_GETS ) {
            retv = BGCopy( back, left );
        }
        left = BGOpGets( back, node->u1.t.op, left, rite, node->tipe, node->optipe );
        if( node->class_ == TN_LV_PRE_GETS ) {
            BGDone( back, left );
        } else {
            retv = left;
        }
    }
    retv->flags |= FL_STACKABLE;
    return( retv );
}


static  an  TNBitAssign( BackRun& back, tn node )
/*********************************
 * do an assignment to a bit field.  If the RHS of the assignment is a
 * similar bit field, zero the destination bit field, and OR in the
 * appropriate bits.  If not, Convert the RHS to an integer, then back
 * to the new bit field location of the left hand side.
 */
{
    an          left;
    an          rite;
    an          retv;
    tn          lhs;
    tn          rhs;
    uint_32     mask;

    lhs = node->u.left;
    rhs = node->u1.t.rite;
    if( rhs->class_ == TN_BIT_RVALUE
      && rhs->u1.b.start == lhs->u1.b.start
      && rhs->u1.b.len == lhs->u1.b.len
      && rhs->tipe->length == lhs->tipe->length ) {
        mask = TGMask32( lhs );
        left = AddrGen( back, lhs->u.left );
        rite = AddrGen( back, rhs->u.left );
        rite = BGUnary( back, O_POINTS, rite, node->tipe );
        retv = BGBinary( back, O_AND, rite, Int( back, mask ), node->tipe, true );
        BGDone( back, BGOpGets( back, O_AND, AddrCopy(back, left), Int( back, ~mask ), node->tipe, node->tipe ) );
        BGDone( back, BGOpGets( back, O_OR, left, AddrCopy( back, retv ), node->tipe, node->tipe ) );
        retv = TNBitShift( back, retv, lhs, true );
        FreeTreeNode( back, rhs );
        FreeTreeNode( back, lhs );
    } else {
        retv = TNBitOpGets( back, node, node->tipe, false );
    }
    return( retv );
}


an  TNAssign( BackRun& back, tn node )
/**********************
 * generate an assignment
 */
{
    an      retv;
    an      left;
    an      rite;

    if( node->u.left->class_ == TN_BIT_LVALUE ) {
        retv = TNBitAssign( back, node );
    } else {
        retv = NULL;
        rite = NotAddrGen( back, node->u1.t.rite );
        left = AddrGen( back, node->u.left );
        if( node->class_ == TN_LV_ASSIGN ) {
            retv = BGCopy( back, left );
        }
        left = BGAssign( back, left, rite, node->optipe );
        if( node->class_ == TN_ASSIGN ) {
            retv = BGDuplicate( back, left );
        }
        BGDone( back, left );
    }
    retv->flags |= FL_STACKABLE;
    return( retv );
}


an  TNCompare( BackRun& back, tn node )
/***********************
 * generate a comparison.
 */
{
    an              retv;
    an              left;
    an              rite;
    label_handle    entry;

    entry = BGGetEntry(back);
    retv = FoldConsCompare( back, node->u1.t.op, node->u.left, node->u1.t.rite, node->u.left->tipe );
    if( retv == NULL ) {
        if( node->u.left->kids >= node->u1.t.rite->kids ) {
            left = NotAddrGen( back, node->u.left );
            rite = NotAddrGen( back, node->u1.t.rite );
        } else {
            rite = NotAddrGen( back, node->u1.t.rite );
            left = NotAddrGen( back, node->u.left );
        }
        retv = BGCompare( back, node->u1.t.op, left, rite, entry, left->tipe );
    } else {
        retv = Boolean( back, Arithmetic( back, retv, back.data.TypeInteger ), entry );
    }
    return( retv );
}


an  TNUnary( BackRun& back, tn node )
/*********************
 * generate a unary node
 */
{
    an      left;
    an      retv;

    retv = NULL;
    if( node->u1.t.op == O_POINTS ) {
       left = AddrGen( back, node->u.left );
    } else if( node->u1.t.op == O_CONVERT ) {
        left = TreeGen( back, node->u.left );
        retv = BGConvert( back, left, node->tipe );
    } else {
        left = TreeGen( back, node->u.left );
    }
    if( retv == NULL ) {
        retv = BGUnary( back, node->u1.t.op, left, node->tipe );
    }
    retv->flags |= FL_STACKABLE;
    if( node->flags & TF_DEMOTED ) {
        AddrDemote( retv );
    }
    return( retv );
}

static an   MakeBased( BackRun& back, an left, an rite, const type_def *tipe )
/**************************************************************
 * Create a far pointer from the ashes of a near pointer on the left
 * and a segment value on the right
 */
{
    an              temp;
    an              seg_dest;
    name            *temp_var;
    const type_def  *near_type;
    const type_def  *short_type;
    an              seg;

    temp_var = BGNewTemp( back, tipe );
    temp = MakeTempAddr( back, temp_var );
    near_type = TypeAddress( back, TY_NEAR_POINTER );
    short_type = TypeAddress( back, TY_UINT_2 );
    if( rite->format == NF_ADDR
      && ( rite->class_ == CL_ADDR_GLOBAL || rite->class_ == CL_ADDR_TEMP ) ) {
        BGDone( back, BGAssign( back, AddrCopy( back, temp ), left, near_type ) );
        seg = AddrName( back, SegName( back, rite->u.n.name ), short_type );
        BGDone( back, rite );
        seg_dest = BGBinary( back, O_PLUS, AddrCopy(back, temp), Int(back, near_type->length), back.data.TypePtr, true );
        BGDone( back, BGAssign( back, seg_dest, seg, short_type ) );
    } else {
        switch( rite->tipe->refno ) {
        case TY_LONG_POINTER:
        case TY_HUGE_POINTER:
        case TY_LONG_CODE_PTR:
            BGDone( back, BGAssign( back, AddrCopy( back, temp ), rite, rite->tipe ) );
            BGDone( back, BGAssign( back, AddrCopy( back, temp ), left, near_type ) );
            break;
        default:
            BGDone( back, BGAssign( back, AddrCopy( back, temp ), left, near_type ) );
            seg_dest = BGBinary( back, O_PLUS, AddrCopy(back, temp), Int(back, near_type->length), back.data.TypePtr, true );
            BGDone( back, BGAssign( back, seg_dest, rite, short_type ) );
            break;
        }
    }
    temp = BGUnary( back, O_POINTS, temp, tipe );
    temp->flags |= FL_STACKABLE;
    return( temp );
}


an  TNBinary( BackRun& back, tn node )
/**********************
 * generate a binary node
 */
{
    an      left;
    an      rite;
    an      retv;
    tn      l;
    tn      r;
    tn      folded;
    bool    was_address;

    if( node->u.left->kids >= node->u1.t.rite->kids ) {
        left = TreeGen( back, node->u.left );
        rite = TreeGen( back, node->u1.t.rite );
    } else {
        rite = TreeGen( back, node->u1.t.rite );
        left = TreeGen( back, node->u.left );
    }

    // Ugly hack to make sure constants get folded
    l = TGLeaf( back, AddrCopy( back, left ) );
    r = TGLeaf( back, AddrCopy( back, rite ) );
    folded = BinFold( back, node->u1.t.op, l, r, node->tipe );
    if( folded != NULL ) {
        BGDone( back, left );
        BGDone( back, rite );
        return( TreeGen( back, folded ) );
    }
    BurnTree( back, l );
    BurnTree( back, r );
    // end of ugly hack

    /* based pointer junk */
    if( node->u1.t.op == O_CONVERT ) {
        retv = MakeBased( back, left, rite, node->tipe );
    } else {
        if( node->flags & TF_DEMOTED ) {
            was_address = SetAddress( back, false ); /* force it to be generated! */
            retv = BGBinary( back, node->u1.t.op, left, rite, node->tipe, false );
            AddrDemote( retv );
            SetAddress( back, was_address );
        } else {
            retv = BGBinary( back, node->u1.t.op, left, rite, node->tipe, true );
        }
    }
    retv->flags |= FL_STACKABLE;
    return( retv );
}

static  an  TNWarp( BackRun& back, tn node )
/****************************
 * generate a TN_WARP node
 */
{
    an      dst;
    an      src;

    if( node->u.left->u1.t.rite != NULL ) {
        BGDone( back, NotAddrGen( back, node->u.left->u1.t.rite ) );
    }
    BGControl( back, O_INVOKE_LABEL, NULL, static_cast<label_handle>(node->u.left->u.handle) );
    FreeTreeNode( back, node->u.left );
    dst = MakeTempAddr( back, BGNewTemp( back, node->tipe ) );
    src = NotAddrGen( back, node->u1.t.rite );
    return( BGAssign( back, dst, src, node->tipe ) );
}


static  an  TNQuestion( BackRun& back, tn node )
/********************************
 * Generate blocks for a ?  b : c.  Note that we call BGControl, rather
 * than BGGenControl.  BGGenControl can start to flush out blocks if it
 * is low on memory, and we can't allow this until the ?  operation is
 * done since there may be an instruction hanging about that hasn't had
 * its result field filled in yet.
 */
{
    label_handle    false_lbl;
    label_handle    around_lbl;
    an              retv;
    an              temp;
    name            *temp_var;

    false_lbl = AskForNewLabel(back);
    around_lbl = AskForNewLabel(back);
    Control( back, O_IF_FALSE, node->u.left, false_lbl, false );
    temp_var = BGNewTemp( back, node->tipe );
    temp_var->v.usage |= USE_IN_ANOTHER_BLOCK;
    temp = MakeTempAddr( back, temp_var );
    retv = TreeGen( back, node->u1.t.rite->u.left );
    BGDone( back, BGAssign( back, AddrCopy( back, temp ), retv, node->tipe ) );
    BGControl( back, O_GOTO, NULL, around_lbl );
    BGControl( back, O_LABEL, NULL, false_lbl );
    BGFiniLabel( back, false_lbl );
    retv = TreeGen( back, node->u1.t.rite->u1.t.rite );
    BGDone( back, BGAssign( back, AddrCopy( back, temp ), retv, node->tipe ) );
    BGControl( back, O_LABEL, NULL, around_lbl );
    BGFiniLabel( back, around_lbl );
    temp = BGUnary( back, O_POINTS, temp, node->tipe );
    FreeTreeNode( back, node->u1.t.rite );
    temp->flags |= FL_STACKABLE;
    return( temp );
}


static  bool    FunctionModifiesSP( BackRun& back, tn call )
/********************************************
 * does the given call modify sp?
 */
{
    hw_reg_set      *pregs;
    cg_sym_handle   sym;

    sym = (cg_sym_handle)call->u.left->u1.t.rite;
    pregs = static_cast<hw_reg_set *>(FindAuxInfoSym( sym, FEINF_SAVE_REGS ));
    if( !HW_Ovlap( *pregs, StackReg() ) ) {
        return( true );
    }
    return( false );
}

static  bool    ModifiesSP( BackRun& back, tn node )
/************************************
 * see if the given tree can modify SP in any way - for now,
 * just see if we make a call to a routine which modifies SP
 * or use an O_STACK_ALLOC unary expression (might want to check
 * results of assignments for SP in the future).
 */
{
    switch( node->class_ ) {
    case TN_BIT_LVALUE:
    case TN_BIT_RVALUE:
    case TN_LEAF:
    case TN_CALLBACK:
    case TN_HANDLE:
    case TN_PATCH:
        break;
    case TN_CALL:
        // need to check if routine modifies SP
        if( FunctionModifiesSP( back, node ) )
            return( true );
        // or if any of it's parm nodes modifies SP
        if( node->u1.t.rite != NULL && ModifiesSP( back, node->u1.t.rite ) )
            return( true );
        break;
    case TN_UNARY:
        if( node->u1.t.op == O_STACK_ALLOC )
            return( true );
        /* fall through */
    default:
        if( node->u1.t.rite != NULL ) {
            if( ModifiesSP( back, node->u1.t.rite ) ) {
                return( true );
            }
        }
        if( node->flags & TF_HAS_LEFT ) {
            assert( node->u.left != NULL );
            if( ModifiesSP( back, node->u.left ) ) {
                return( true );
            }
        }
    }
    return( false );
}

static  void    MakeSPSafe( BackRun& back, tn scan )
/************************************
 * run through the parm list and yank any trees which could
 * modify the stack pointer out - this allows us to handle an
 * alloca in a parm list and other such nonsense.
 */
{
    tn              parmtn;
    an              temp;
    an              parman;
    const type_def  *tipe;

    for( ; scan != NULL; scan = scan->u1.t.rite ) {
        parmtn = scan->u.left;
        if( ModifiesSP( back, parmtn ) ) {
            tipe = parmtn->tipe;
            temp = MakeTempAddr( back, BGNewTemp( back, tipe ) );
            parman = NotAddrGen( back, parmtn );
            BGDone( back, BGAssign( back, BGDuplicate( back, temp ), parman, parman->tipe ) );
            scan->u.left = TGUnary( back, O_POINTS, TGLeaf( back, temp ), tipe );
        }
    }
}

static  an  TNCall( BackRun& back, tn callhandle, bool ignore_return )
/******************************************************
 * generate block for a TN_CALL node
 */
{
    tn              scan;
    tn              addr;
    an              temp;
    tn              parmtn;
    cn              callnode;
    an              retv;
    an              parman;
    const type_def  *tipe;
    bool            aux_inline;     /* ASM inline code */
    bool            call_inline;    /* C inline code */
    name            *base;
    call_class      cclass;
    cg_sym_handle   sym;
    aux_handle      aux;

    /* unused parameters */ (void)ignore_return;

    callnode = NULL;
    addr = callhandle->u.left; /* address to call*/
    sym = (cg_sym_handle)addr->u1.t.rite;
    aux = static_cast<aux_handle>(FEAuxInfo( *back.front, sym, FEINF_AUX_LOOKUP ));
    aux_inline = ( FEAuxInfo( *back.front, aux, FEINF_CALL_BYTES ) != NULL );
    cclass = (call_class)(pointer_uint)FEAuxInfo( *back.front, aux, FEINF_CALL_CLASS );
    call_inline = ( (cclass & FECALL_GEN_MAKE_CALL_INLINE) != 0 );
    retv = TreeGen( back, addr->u.left );
    if( call_inline ) {
        BGDone( back, retv );
        BGStartInline( back, sym );
    } else {
        callnode = BGInitCall( back, retv, callhandle->tipe, aux );
    }
    MakeSPSafe( back, callhandle->u1.t.rite );
    for( scan = callhandle->u1.t.rite; scan != NULL; scan = scan->u1.t.rite ) {
        base = TNFindBase( back, scan->u.left );
        parmtn = scan->u.left;
        scan->u.name = base;
        if( cclass & FECALL_GEN_PARMS_BY_ADDRESS ) {
            if( parmtn->class_ == TN_UNARY && parmtn->u1.t.op == O_POINTS ) {
                parman = AddrGen( back, parmtn->u.left );
                parmtn->u.left = NULL;
                parmtn->u1.t.rite = NULL;
                BurnTree( back, parmtn );
            } else {
                temp = MakeTempAddr( back, BGNewTemp( back, parmtn->tipe ) );
                parman = AddrGen( back, parmtn );
                BGDone( back, BGAssign( back, BGDuplicate( back, temp ), parman, parman->tipe ) );
                parman = temp;
            }
            tipe = back.data.TypePtr;
        } else {
            parman = NotAddrGen( back, parmtn );
            tipe = scan->tipe;
        }
        if( tipe->refno == TY_DEFAULT ) {
            tipe = parman->tipe;
        }
        if( tipe == back.data.TypeProcParm ) {
            if( parman->tipe != back.data.TypeProcParm ) {
                retv = PassProcParm( back, parman );
                BGDone( back, parman );
                parman = retv;
            }
        } else {
            if( aux_inline || call_inline ) {
                parman = BGConvert( back, parman, tipe );
            } else {
                parman = BGConvert( back, parman, PassParmType( back, sym, tipe ) );
            }
        }
        parman->flags |= FL_STACKABLE;
        if( call_inline ) {
            BGAddInlineParm( back, parman );
        } else {
            BGAddParm( back, callnode, parman );
        }
    }
    FreeTreeNode( back, addr );
    if( call_inline ) {
        retv = BGStopInline( back, callhandle, callhandle->tipe );
        back.tree.NodesToZap = callhandle->u1.t.rite;
        TNZapParms(back);
    } else {
        back.tree.NodesToZap = callhandle->u1.t.rite;
        retv = BGCall( back, callnode, (callhandle->flags & TF_USED) != 0, aux_inline );
        retv->flags |= FL_STACKABLE;
        BGFiniCall( back, callnode );
    }
    return( retv );
}


void    TNZapParms( BackRun& back )
/************************/
{
    tn      next;
    tn      scan;

    for( scan = back.tree.NodesToZap; scan != NULL; scan = next ) {
        next = scan->u1.t.rite;
        BGZapBase( back, scan->u.name, scan->tipe );
        FreeTreeNode( back, scan );
    }
}


bool    TGIsAddress( BackRun& back )
/**********************************
 * Are we processing an address expression?
 */
{
    return( back.tree.IsAddress );
}

tn  TGQuestion( BackRun& back, tn sel, tn left, tn rite, const type_def *tipe )
/***************************************************************
 * sel ? left : rite
 */
{
    tn  result;

    sel = TGConvert( back, sel, back.data.TypeBoolean );
    left = TGConvert( back, left, tipe );
    rite = TGConvert( back, rite, tipe );
    result = TGNode( back, TN_COLON, O_NOP, left, rite, tipe );
    result = TGNode( back, TN_QUESTION, O_NOP, sel, result, tipe );
    return( result );
}


void    BurnTree( BackRun& back, tn node )
/**********************************
 * figure it out
 */
{
    switch( node->class_ ) {
    case TN_LEAF:
        BGDone( back, node->u.addr );
        break;
    case TN_CONS:
        break;
    case TN_BIT_RVALUE:
    case TN_BIT_LVALUE:
        if( node->u.left != NULL ) {
            BurnTree( back, node->u.left );
        }
        break;
    case TN_CALL:
        BurnTree( back, node->u.left->u.left );
        FreeTreeNode( back, node->u.left );
        if( node->u1.t.rite != NULL ) {
            BurnTree( back, node->u1.t.rite );
        }
        break;
    default:
        if( node->u1.t.rite != NULL ) {
            BurnTree( back, node->u1.t.rite );
        }
        if( node->u.left != NULL ) {
            BurnTree( back, node->u.left );
        }
        break;
    }
    FreeTreeNode( back, node );
}


static  void    FreeTreeNode( BackRun& back, tn node )
/**************************************
 * free one tree node.
 */
{
    FrlFreeSize( back, &back.tree.TreeFrl, (pointer *)node, sizeof( tree_node ) );
}


void    TInit( BackRun& back )
/****************************
 * Initialize for tree processing
 */
{
    SetAddress( back, false );
    back.tree.SubMat[CP + XX * CP] = TypeClass( back, back.data.TypeLongInteger );
    back.tree.SubMat[PT + XX * PT] = TypeClass( back, back.data.TypeHugeInteger );
    InitFrl( &back.tree.TreeFrl );
}


bool    TreeFrlFree( BackRun& back )
/*************************/
{
    return( FrlFreeAll( back, &back.tree.TreeFrl, sizeof( tree_node ) ) );
}


void    TFini( BackRun& back )
/*******************/
{
    TreeFrlFree(back);
}


static  an DoTreeGen( BackRun& back, tn node )
/*****************************/
{
    an          retv;

    switch( node->class_ ) {
    case TN_LEAF:
        retv = node->u.addr;
        break;
    case TN_CONS:
        retv = AddrName( back, node->u.name, node->tipe );
        break;
    case TN_COMMA:
        BGDone( back, NotAddrGen( back, node->u.left ) );
        retv = TreeGen( back, node->u1.t.rite );
        break;
    case TN_SIDE_EFFECT:
        retv = TreeGen( back, node->u.left );
        BGDone( back, NotAddrGen( back, node->u1.t.rite ) );
        break;
    case TN_BINARY:
        retv = TNBinary( back, node );
        break;
    case TN_UNARY:
        retv = TNUnary( back, node );
        break;
    case TN_COMPARE:
        retv = TNCompare( back, node );
        break;
    case TN_LV_ASSIGN:
    case TN_ASSIGN:
        retv = TNAssign( back, node );
        break;
    case TN_LV_PRE_GETS:
    case TN_PRE_GETS:
        retv = TNPreGets( back, node );
        break;
    case TN_POST_GETS:
        retv = TNPostGets( back, node );
        break;
    case TN_FLOW:
        retv = TNFlow( back, node );
        break;
    case TN_CALL:
        retv = TNCall( back, node, false );
        break;
    case TN_FLOW_OUT:
        retv = Arithmetic( back, NotAddrGen( back, node->u.left ), node->tipe );
        break;
    case TN_QUESTION:
        retv = TNQuestion( back, node );
        break;
    case TN_BIT_RVALUE:
        retv = TNBitRVal( back, AddrGen( back, node->u.left ), node );
        break;
    case TN_WARP:
        retv = TNWarp( back, node );
        break;
    case TN_CALLBACK:
        retv = TNCallback( back, node );
        break;
    case TN_PATCH:
        retv = TNPatch( back, node );
        break;
    default:
        retv = NULL;
        _Zoiks( ZOIKS_057 );
    }
    FreeTreeNode( back, node );
    return( retv );
}


an  TreeGen( BackRun& back, tn node )
/*****************************
 * generate tree node "node"
 */
{
    return static_cast<an>(( SafeRecurseCG( back, (func_sr)DoTreeGen, node ) ));
}
