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
* Description:  Initialization of data ( e.g. int foo[] = { 0, 1, 2 }; )
*
****************************************************************************/


#include "cvars.h"
#include <limits.h>
#include "i64.h"
#include "roundmac.h"
#include "frontrun.hpp"


#define DATA_QUAD_SEG_SIZE_DEF  _32K
#define DATA_QUAD_MAX_INDEX_DEF 0xFFFFF

#define DATA_QUAD_PER_SEG       (DATA_QUAD_SEG_SIZE_DEF / sizeof( DATA_QUAD_LIST ))
#define DATA_QUAD_SEG_SIZE      (DATA_QUAD_PER_SEG * sizeof( DATA_QUAD_LIST ))
#define DATA_QUAD_SEG_MAX       (DATA_QUAD_MAX_INDEX_DEF / DATA_QUAD_PER_SEG + 1)

typedef enum {
    IS_VALUE,
    IS_ADDR
} fold_state;

typedef struct {
    target_ssize        offset;
    union {
        STR_HANDLE      str_h;
        SYM_HANDLE      sym_h;
    } u;
    bool                is_str;
    bool                addr_set;
    bool                is_error;
    fold_state          state;
} addrfold_info;

/*
 * use a double-linked list of dataquads to facilitate insertions
 */
typedef struct data_quad_list {
    DATA_QUAD               dq;
    target_size             size;
    struct data_quad_list   *prev;
    struct data_quad_list   *next;
} DATA_QUAD_LIST;


static DATA_QUAD_LIST   *NewDataQuad( FrontRun& front );
static bool             CharArray( FrontRun& front, TYPEPTR typ );
static bool             WCharArray( FrontRun& front, TYPEPTR typ );
static void             InitCharArray( FrontRun& front, TYPEPTR typ );
static void             InitWCharArray( FrontRun& front, TYPEPTR typ );
static void             StoreFloat( FrontRun& front, DATA_TYPE dtype, target_size size );
static void             StoreInt64( FrontRun& front, TYPEPTR typ );

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct CdinitState {
    DATA_QUAD_LIST *DataQuadSegs[DATA_QUAD_SEG_MAX];   /* segments for data quads */
    DATA_QUAD_LIST *CurDataQuad;
    int DataQuadSegIndex;
    int DataQuadIndex;
    DATA_QUAD_LIST *NewDataQuad_DataQuadPtr;
    DATA_QUAD_LIST *StoreIValue_LastCurDataQuad;
    bool DesignatedInit_new_field = true;
};
namespace {

}  // namespace


bool DataQuadsAvailable( FrontRun& front )
{
    DATA_QUAD_LIST  *dql = front.cdinit->DataQuadSegs[0];

    return( dql != NULL && dql->next != NULL );
}

void InitDataQuads( FrontRun& front )
{
    front.cdinit->DataQuadIndex = DATA_QUAD_PER_SEG;
    front.cdinit->DataQuadSegIndex = -1;
    memset( front.cdinit->DataQuadSegs, 0, sizeof( front.cdinit->DataQuadSegs ) );
    /*
     * put a guard at the start
     */
    front.cdinit->CurDataQuad = NewDataQuad(front);
    front.cdinit->CurDataQuad->prev = NULL;
    front.cdinit->CurDataQuad->next = NULL;
    front.cdinit->CurDataQuad->size = 0;
}

void FreeDataQuads( FrontRun& front )
{
    unsigned    i;

    for( i = 0; i < DATA_QUAD_SEG_MAX; i++ ) {
        if( front.cdinit->DataQuadSegs[i] == NULL )
            break;
        FEfree( front, front.cdinit->DataQuadSegs[i] );
        front.cdinit->DataQuadSegs[i] = NULL;
    }
}

void *StartDataQuadAccess( FrontRun& front )
{
    void    *cur_dqp;

    if( DataQuadsAvailable(front) ) {
        cur_dqp = front.cdinit->CurDataQuad;
        front.cdinit->CurDataQuad = front.cdinit->DataQuadSegs[0]->next;
        return( cur_dqp );  /* indicate data quads exist */
    }
    return( NULL );         /* indicate no data quads */
}

void EndDataQuadAccess( FrontRun& front, void *p )
{
    front.cdinit->CurDataQuad = static_cast<DATA_QUAD_LIST *>(p);
}

DATA_QUAD *NextDataQuad( FrontRun& front )
{
    DATA_QUAD   *dq_ptr;

    if( front.cdinit->CurDataQuad == NULL )
        return( NULL );
    dq_ptr = &front.cdinit->CurDataQuad->dq;
    front.cdinit->CurDataQuad = front.cdinit->CurDataQuad->next;
    return( dq_ptr );
}

static DATA_QUAD_LIST *NewDataQuad( FrontRun& front )
{
    DATA_QUAD_LIST          *dql;

    if( front.cdinit->DataQuadIndex >= (DATA_QUAD_PER_SEG - 1) ) {
        if( front.cdinit->DataQuadSegIndex == DATA_QUAD_SEG_MAX ) {
            CErr1( front, ERR_INTERNAL_LIMIT_EXCEEDED );
            CSuicide(front);
        }
        ++front.cdinit->DataQuadSegIndex;
        front.cdinit->DataQuadIndex = 0;
        front.cdinit->NewDataQuad_DataQuadPtr = static_cast<DATA_QUAD_LIST *>(FEmalloc( front, DATA_QUAD_SEG_SIZE ));
        front.cdinit->DataQuadSegs[front.cdinit->DataQuadSegIndex] = front.cdinit->NewDataQuad_DataQuadPtr;
    }
    dql = front.cdinit->NewDataQuad_DataQuadPtr;
    ++front.cdinit->DataQuadIndex;
    ++front.cdinit->NewDataQuad_DataQuadPtr;
    return( dql );
}

static void SplitDataQuad( FrontRun& front, DATA_QUAD_LIST *dql, target_size size )
/*****************************************************************
 * splits the dataquad pointed to by dql so that the current one
 * will have size "size" and the new one "oldsize - size"
 */
{
    DATA_QUAD_LIST  *ndql;
    DATA_QUAD       *ndq;
    DATA_QUAD       *dq;
    target_size     oldsize;

    ndql = NewDataQuad(front);
    ndql->next = dql->next;
    ndql->prev = dql;
    dql->next = ndql;
    if( ndql->next != NULL )
        ndql->next->prev = ndql;
    oldsize = dql->size;
    ndql->size = oldsize - size;
    dql->size = size;

    ndq = &ndql->dq;
    dq = &dql->dq;
    memcpy( ndq, dq, sizeof( *dq ) );

    if( dq->flags & Q_DATA ) {
        if( dq->flags & Q_2_INTS_IN_ONE ) {
            dq->flags = ndq->flags = Q_DATA;
            ndq->u_long_value1 = dq->u_long_value2;
            ndq->u_long_value2 = 0;
            dq->u_long_value2 = 0;
            size = 0;
        } else if( dq->flags & Q_REPEATED_DATA ) {
            dq->u_rpt_count = size / ( oldsize / dq->u_rpt_count );
            ndq->u_rpt_count -= dq->u_rpt_count;
            size = 0;
        } else if( dq->type == QDT_CONSTANT ) {
            dq->u_size = size;
            ndq->u_size -= size;
            size = 0;
        } else if( dq->type == QDT_CONST ) {
            dq->u.string_leaf->length = size;
            ndq->u.string_leaf->literal += size;
            ndq->u.string_leaf->length = oldsize - size;
            size = 0;
        }
    }
    if( size != 0 ) {
        /*
         * can't happen !
         */
        CErr2p( front, ERR_FATAL_ERROR, "Bad initializer quad" );
        CSuicide(front);
    }
}

static void DeleteDataQuad( DATA_QUAD_LIST *dql )
{
    dql->prev->next = dql->next;
    if( dql->next != NULL ) {
        dql->next->prev = dql->prev;
    }
}

static void GenDataQuad( FrontRun& front, DATA_QUAD *dq, target_size size )
{
    DATA_QUAD_LIST  *dql;
    target_size     cursize;

    dql = front.cdinit->CurDataQuad->next;
    if( dql != NULL ) {
        /*
         * overwrite the current dataquad
         */
        cursize = dql->size;
        while( size > cursize && dql->next != NULL ) {
            DeleteDataQuad( dql );
            dql = dql->next;
            cursize += dql->size;
        }
        /*
         * dql now takes up cursize bytes but was defined to
         * take up dql->size bytes: split into a dataquad with
         * dql->size - (cursize - size) bytes and one with
         * cursize - size bytes.
         */
        if( size < cursize ) {
            SplitDataQuad( front, dql, dql->size - ( cursize - size ) );
        }
    } else {
        dql = NewDataQuad(front);
        front.cdinit->CurDataQuad->next = dql;
        dql->prev = front.cdinit->CurDataQuad;
        dql->next = NULL;
    }
    dql->size = size;
    memcpy( &dql->dq, dq, sizeof( DATA_QUAD ) );
    front.cdinit->CurDataQuad = dql;
}

static void ZeroBytes( FrontRun& front, target_size n )
{
    DATA_QUAD   dq;

    if( n == 0 )
        return;
    dq.type = QDT_CONSTANT;
    dq.flags = Q_DATA;
    dq.u_size = n;
    GenDataQuad( front, &dq, n );
}

static void RelSeekBytes( FrontRun& front, target_ssize n )
{
    DATA_QUAD_LIST  *dql;

    dql = front.cdinit->CurDataQuad;
    while( n < 0 && n <= -(target_ssize)dql->size ) {
        n += dql->size;
        dql = dql->prev;
    }
    while( n > 0 && dql->next != NULL ) {
        dql = dql->next;
        n -= dql->size;
    }
    /*
     * now -dql->size < n <= 0 or dql->next == NULL
     */
    if( n < 0 ) {
        SplitDataQuad( front, dql, dql->size + n );
    }
    front.cdinit->CurDataQuad = dql;
    if( n > 0 ) {
        /*
         * dql->next == NULL
         */
        ZeroBytes( front, n );
    }
}

static void StoreIValue( FrontRun& front, DATA_TYPE dtype, int value, target_size size )
{
    DATA_QUAD_LIST          *dql;
    DATA_QUAD               *dq_ptr;
    DATA_QUAD               dq;

    dq.flags = static_cast<enum quad_flags>(0);
    dq.type = static_cast<enum quad_type>(0);
    dq_ptr = &dq;
    if( front.cdinit->StoreIValue_LastCurDataQuad == front.cdinit->CurDataQuad ) {
        dq_ptr = &front.cdinit->CurDataQuad->dq;
    }
    if( (DATA_TYPE)dq_ptr->type == dtype
      && dq_ptr->flags == (Q_DATA | Q_REPEATED_DATA)
      && dq_ptr->u_long_value1 == value ) {
        dq_ptr->u_rpt_count++;                  /* increment repeat count */
        front.cdinit->CurDataQuad->size += size;
    } else if( (DATA_TYPE)dq_ptr->type == dtype
      && dq_ptr->flags == Q_DATA ) {
        if( dq_ptr->u_long_value1 == value ) {
            dq_ptr->flags |= Q_REPEATED_DATA;
            dq_ptr->u_rpt_count = 2;            /* repeat count */
        } else {
            dq_ptr->flags |= Q_2_INTS_IN_ONE;
            dq_ptr->u_long_value2 = value;
        }
        front.cdinit->CurDataQuad->size += size;
    } else {
        dq.type = (enum quad_type)dtype;
        dq.flags = Q_DATA;
        dq.u_long_value1 = value;
        if( value != 0 )
            front.cvars.CompFlags.non_zero_data = true;
        GenDataQuad( front, &dq, size );
        front.cdinit->StoreIValue_LastCurDataQuad = front.cdinit->CurDataQuad;
        return;
    }
    /*
     * if the next dataquad is non-empty we'll have to delete it
     */
    dql = front.cdinit->CurDataQuad->next;
    if( dql != NULL ) {
        if( dql->size > size ) {
            SplitDataQuad( front, dql, size );
        }
        /*
         * no need to "free" the next one, just remove it from the list
         */
        DeleteDataQuad( dql );
    }
}

static void StoreIValue64( FrontRun& front, DATA_TYPE dtype, int64 value )
{
    DATA_QUAD           dq;

    dq.type = (enum quad_type)dtype;
    dq.flags = Q_DATA;
    dq.u.long64 = value;
    front.cvars.CompFlags.non_zero_data = true;
    GenDataQuad( front, &dq, TARGET_LONG64 );
}

static void AddrFold( FrontRun& front, TREEPTR tree, addrfold_info *info )
/********************************************************
 * Assume tree has been const folded
 */
{
    SYM_ENTRY           sym;
    target_ssize        offset;

    offset = 0;
    switch( tree->op.opr ) {
    case OPR_PUSHINT:
        info->state = IS_ADDR;
        info->offset = tree->op.u2.long_value;
        if( info->offset != 0 )
            front.cvars.CompFlags.non_zero_data = true;
        break;
    case OPR_PUSHFLOAT:
        info->state = IS_ADDR;
        if( tree->op.u2.float_value->len != 0 ) {
            info->offset = (target_ssize)atof( tree->op.u2.float_value->string );
        } else {
            info->offset = (target_ssize)tree->op.u2.float_value->ld.u.value;
        }
        if( info->offset != 0 )
            front.cvars.CompFlags.non_zero_data = true;
        break;
    case OPR_PUSHSTRING:
        info->state = IS_ADDR;
        if( info->addr_set ) {
            info->is_error = true;
        }
        info->addr_set = true;
        info->is_str = true;
        info->u.str_h = tree->op.u2.string_handle;
        tree->op.u2.string_handle->ref_count++;
        front.cvars.CompFlags.non_zero_data = true;
        break;
    case OPR_PUSHADDR:
        info->state = IS_ADDR;
        /* fall through */
    case OPR_PUSHSYM:
        if( info->addr_set ) {
            info->is_error = true;
        }
        info->addr_set = true;
        info->is_str = false;
        SymGet( front, &sym, tree->op.u2.sym_handle );
        info->u.sym_h = tree->op.u2.sym_handle;
        front.cvars.CompFlags.non_zero_data = true;
        if( sym.attribs.stg_class == SC_AUTO ) {
            info->is_error = true;
        }
        break;
    case OPR_INDEX:
        if( tree->right->op.opr == OPR_PUSHINT ) {
            AddrFold( front, tree->left, info );
            offset = tree->right->op.u2.long_value;
        } else if( tree->left->op.opr == OPR_PUSHINT ) {
            AddrFold( front, tree->right, info );
            offset = tree->left->op.u2.long_value;
        } else {
            info->is_error = true;
        }
        if( info->state == IS_VALUE ) { // must be foldable
            info->is_error = true;
        }
        info->offset += offset * SizeOfArg( front, tree->u.expr_type );
        if( tree->op.flags & OPFLAG_RVALUE ) {
            info->state = IS_VALUE;
        }
        break;
    case OPR_ADD:
    case OPR_SUB:
        if( tree->right->op.opr == OPR_PUSHINT ) {
            AddrFold( front, tree->left, info );
            if( tree->op.opr == OPR_ADD ) {
                info->offset = info->offset + tree->right->op.u2.long_value;
            } else {
                info->offset = info->offset - tree->right->op.u2.long_value;
            }
        } else if( tree->left->op.opr == OPR_PUSHINT ) {
            AddrFold( front, tree->right, info );
            if( tree->op.opr == OPR_ADD ) {
                info->offset = tree->left->op.u2.long_value + info->offset;
            } else {
                info->offset = tree->left->op.u2.long_value - info->offset;
            }
        } else {
            info->is_error = true;
        }
        if( info->state == IS_VALUE ) { // must be foldable
            info->is_error = true;
        }
        break;
    case OPR_ADDROF:
        AddrFold( front, tree->right, info );
        info->state = IS_ADDR;
        front.cvars.CompFlags.non_zero_data = true;
        break;
    case OPR_ARROW:
        AddrFold( front, tree->left, info );
        if( info->state == IS_VALUE ) { // must be foldable
            info->is_error = true;
        }
        info->offset += tree->right->op.u2.long_value;
        if( tree->op.flags & OPFLAG_RVALUE ) {
            info->state = IS_VALUE;
        }
        break;
    case OPR_POINTS:
        AddrFold( front, tree->left, info );
        if( info->state == IS_VALUE ) { // must be foldable
            info->is_error = true;
        }
        if( tree->op.flags & OPFLAG_RVALUE ) {
            info->state = IS_VALUE;
        }
        break;
    case OPR_DOT:
        AddrFold( front, tree->left, info );
        info->offset += tree->right->op.u2.long_value;
        front.cvars.CompFlags.non_zero_data = true;
        if( tree->op.flags & OPFLAG_RVALUE ) {
            info->state = IS_VALUE;
        }
        break;
    case OPR_CONVERT:   // should check for pointer to smaller
    case OPR_CONVERT_PTR:
        AddrFold( front, tree->right, info );
        break;
    case OPR_ERROR:     // error has already been issued
        break;
    default:
        info->is_error = true;
        break;
    }
}

static void StorePointer( FrontRun& front, TYPEPTR typ, target_size size )
{
    TREEPTR             tree;
    TYPEPTR             typ2;
    DATA_QUAD           dq;
    addrfold_info       info;

    dq.flags = Q_DATA;
    if( typ->decl_type == TYP_POINTER ) {
        if( typ->u.p.decl_flags & (FLAG_FAR | FLAG_HUGE) ) {
            dq.flags |= Q_FAR_POINTER;
        } else if( typ->u.p.decl_flags & FLAG_NEAR ) {
            dq.flags |= Q_NEAR_POINTER;
        } else {
            typ2 = typ->object;
            SkipTypedefs( typ2 );
            if( typ2->decl_type == TYP_FUNCTION ) {
                dq.flags |= Q_CODE_POINTER;
            } else if( TypeSize( front, typ ) == TARGET_FAR_POINTER ) {
                dq.flags |= Q_FAR_POINTER;
            }
        }
    }

    dq.type = QDT_ID;
    dq.u.var.sym_handle = SYM_NULL;
    dq.u.var.offset = 0;
#if _CPU == 8086
    /*
     * CodeBreaker: a __segment initialized with the segment of an object or a function,
     * `(__segment)&sym` -- the selector the loader fills in (a segment fixup), as the
     * original's data holds it
     */
    if( (typ->type_flags & TF2_TYP_SEGMENT) && front.cvars.CurToken != T_RIGHT_BRACE ) {
        TREEPTR     opnd;

        tree = AddrExpr(front);
        tree = InitAsgn( front, typ, tree );
        for( opnd = tree; opnd->op.opr == OPR_CONVERT || opnd->op.opr == OPR_CONVERT_SEG; ) {
            if( opnd->op.opr == OPR_CONVERT_SEG ) {
                opnd = opnd->right;
                break;
            }
            opnd = opnd->right;
        }
        if( tree->op.opr == OPR_CONVERT_SEG || opnd != tree ) {
            info.offset = 0;
            info.u.sym_h = SYM_NULL;
            info.addr_set = false;
            info.state = IS_VALUE;
            info.is_str = false;
            info.is_error = false;
            AddrFold( front, opnd, &info );
            if( !info.is_error && !info.is_str && info.state != IS_VALUE && info.u.sym_h != SYM_NULL ) {
                dq.flags |= Q_SEGMENT;
                dq.u.var.sym_handle = info.u.sym_h;
                GenDataQuad( front, &dq, size );
                FreeExprTree( front, tree );
                return;
            }
        }
        /*
         * otherwise a constant, as any integer
         */
        info.offset = 0;
        info.u.sym_h = SYM_NULL;
        info.addr_set = false;
        info.state = IS_VALUE;
        info.is_str = false;
        info.is_error = false;
        AddrFold( front, tree, &info );
        if( info.u.sym_h != SYM_NULL || info.is_error || info.is_str ) {
            if( tree->op.opr != OPR_ERROR ) {
                CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
            }
        } else {
            StoreIValue( front, typ->decl_type, info.offset, size );
        }
        FreeExprTree( front, tree );
        return;
    }
#endif
    if( front.cvars.CurToken != T_RIGHT_BRACE ) {
        tree = AddrExpr(front);
        tree = InitAsgn( front, typ, tree ); // as if we are assigning
        info.offset = 0;
        info.u.sym_h = SYM_NULL;
        info.addr_set = false;
        info.state = IS_VALUE;
        info.is_str = false;
        info.is_error = false;
        AddrFold( front, tree, &info );
        if( info.state == IS_VALUE ) { // must be foldable  into addr+offset
            info.is_error = true;
        }
        if( info.is_str ) { // need to fix cgen an DATAQUADS to handle str+off
            if( info.offset != 0 ) {
                info.is_error = true;
            }
        }
        if( info.is_error ) {
            if( tree->op.opr != OPR_ERROR ) {
                CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
            }
        } else {
            if( info.is_str ) {
                dq.type = QDT_STRING;
                dq.u.string_leaf = info.u.str_h;
            } else {
                dq.u.var.sym_handle = info.u.sym_h;
                dq.u.var.offset = info.offset;
            }
        }
        FreeExprTree( front, tree );
    }
    if( typ->decl_type == TYP_POINTER ) {
        GenDataQuad( front, &dq, size );
    } else if( dq.type == QDT_STRING ) {
        if( TypeSize( front, typ ) != front.cvars.DataPtrSize
          || front.cvars.CompFlags.strict_ANSI ) {
            CErr1( front, ERR_INVALID_INITIALIZER );
        }
        GenDataQuad( front, &dq, size );
    } else { /* dq.type == QDT_ID */
        if( dq.u.var.sym_handle != SYM_NULL ) {
            if( TypeSize( front, typ ) != front.cvars.DataPtrSize ) {
                CErr1( front, ERR_INVALID_INITIALIZER );
            }
            GenDataQuad( front, &dq, size );
        } else {
            StoreIValue( front, typ->decl_type, dq.u.var.offset, size );
        }
    }
}

static void StoreInt64( FrontRun& front, TYPEPTR typ )
{
    TREEPTR     tree;
    DATA_QUAD   dq;

    dq.type = (enum quad_type)typ->decl_type;
    dq.flags = Q_DATA;
    Set64ValZero( dq.u.long64 );
    if( front.cvars.CurToken != T_RIGHT_BRACE ) {
        tree = SingleExpr(front);
        tree = InitAsgn( front, typ, tree ); // as if we are assigning
        if( IsConstLeaf( tree ) ) {
            CastConstValue( front, tree, typ->decl_type );
            dq.u.long64 = tree->op.u2.ulong64_value;
        } else {
            CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
        }
        FreeExprTree( front, tree );
        front.cvars.CompFlags.non_zero_data = true;
    }
    GenDataQuad( front, &dq, TARGET_LONG64 );
}

static void LoadBitField( FrontRun& front, uint64 *val64 )
{
    DATA_QUAD           *dqp;

    Set64ValZero( *val64 );
    if( front.cdinit->CurDataQuad->next == NULL )
        return;
    dqp = &front.cdinit->CurDataQuad->next->dq;
    if( dqp->type == QDT_CONSTANT )
        return;
    if( front.cdinit->CurDataQuad->next->size == TARGET_LONG64 ) {
        *val64 = dqp->u.long64;
    } else {
        U64Low( *val64 ) = dqp->u.ulong_values[0];
    }
}

static void ResetBitField( uint64 *val64, unsigned start, unsigned width )
{
    uint64      mask;

    /*
     * mask = ( 1 << width ) - 1;
     */
    Set64Val1p( mask );
    U64ShiftLEq( &mask, width );
    U64Dec( mask );
    U64ShiftLEq( &mask, start );
    U64NotAndEq( *val64, mask );
}

static void InitBitField( FrontRun& front, FIELDPTR field )
{
    TYPEPTR             typ;
    target_size         size;
    uint64              value64;
    const_val           bit_value;
    TOKEN               token;
    bool                is64bit;
    DATA_TYPE           dtype;
    unsigned            width;

    token = front.cvars.CurToken;
    if( front.cvars.CurToken == T_LEFT_BRACE )
        NextToken(front);
    typ = field->field_type;
    size = SizeOfArg( front, typ );
    dtype = typ->u.f.field_type;
    is64bit = ( dtype == TYP_LONG64 || dtype == TYP_ULONG64 );
    LoadBitField( front, &value64 );
    if( typ->u.f.field_type == TYP_BOOL ) {
        width = 1;
    } else {
        width = typ->u.f.field_width;
    }
    ResetBitField( &value64, typ->u.f.field_start, width );
    if( front.cvars.CurToken != T_RIGHT_BRACE ) {
        if( ConstExprAndType( front, &bit_value ) ) {
            if( CheckAssignBits( &bit_value.value, width, true ) ) {
                CWarn1( front, ERR_CONSTANT_TOO_BIG );
            }
            U64ShiftLEq( &bit_value.value, typ->u.f.field_start );
            U64OrEq( value64, bit_value.value );
        }
    }
    if( is64bit ) {
        StoreIValue64( front, dtype, value64 );
    } else {
        StoreIValue( front, dtype, U64Low( value64 ), size );
    }
    if( token == T_LEFT_BRACE ) {
        if( front.cvars.CurToken == T_COMMA )
            NextToken(front);
        MustRecog( front, T_RIGHT_BRACE );
    }
}


static void *DesignatedInit( FrontRun& front, TYPEPTR typ, TYPEPTR ctyp, void *field )
/********************************************************************
 * Detects a C99 designated initializer
 *
 * typ is the (perhaps subaggregate) object that is initialized.
 * ctyp refers to the current object which is the object
 * associated with the closest brace pair. It may or may not be
 * equal to typ
 */
{
    TREEPTR         tree;
    target_size     offs;

    if( !front.cvars.CompFlags.extensions_enabled
      && ( front.cvars.CompVars.cstd < STD_C99 ) ) {
        return( field );
    }

    if( front.cvars.CurToken != T_LEFT_BRACKET
      && front.cvars.CurToken != T_DOT ) {
        front.cdinit->DesignatedInit_new_field = true;
        return( field );
    }
    /*
     * if designator refers to outer type: back out
     */
    if( typ != ctyp
      && front.cdinit->DesignatedInit_new_field )
        return( NULL );

    front.cdinit->DesignatedInit_new_field = false;
    if( typ->decl_type == TYP_ARRAY ) {
        if( front.cvars.CurToken != T_LEFT_BRACKET )
            return( NULL );
        NextToken(front);
        tree = SingleExpr(front);
        if( IsConstLeaf( tree ) ) {
            CastConstValue( front, tree, typ->decl_type );
            *(target_size *)field = tree->op.u2.ulong_value;
        } else {
            CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
        }
        FreeExprTree( front, tree );
        MustRecog( front, T_RIGHT_BRACKET );
    } else {
        if( front.cvars.CurToken != T_DOT )
            return( NULL );
        NextToken(front);
        if( front.cvars.CurToken != T_ID ) {
            CErr1( front, ERR_EXPECTING_ID );
        }
        offs = 0;
        field = SearchFields( &typ, &offs, front.cvars.Buffer );
        if( field == NULL ) {
            CErr3p( front, ERR_NAME_NOT_FOUND_IN_STRUCT, front.cvars.Buffer, typ->u.tag->name );
        }
        NextToken(front);
    }

    if( front.cvars.CurToken != T_LEFT_BRACKET
      && front.cvars.CurToken != T_DOT ) {
        front.cdinit->DesignatedInit_new_field = true;
        MustRecog( front, T_EQUAL );
    }
    return( field );
}

static bool DesignatedInSubAggregate( FrontRun& front, DATA_TYPE decl_type )
{
    switch( decl_type ) {
    case TYP_ARRAY:
    case TYP_STRUCT:
    case TYP_UNION:
    case TYP_FCOMPLEX:
    case TYP_DCOMPLEX:
    case TYP_LDCOMPLEX:
        /*
         * A subaggregate can be stopped by a designated initializer.
         * in that case the comma was already eaten...
         */
        return( front.cvars.CurToken == T_DOT || front.cvars.CurToken == T_LEFT_BRACKET );
    default:
        return( false );
    }
}

static void InitArray( FrontRun& front, TYPEPTR typ, TYPEPTR ctyp )
{
    target_size n;
    target_size m;
    target_size *pm;
    target_size array_size;
    target_size elem_size;

    array_size = TypeSize( front, typ );
    n = 0;
    pm = &m;
    for( ;; ) {
        m = n;
        pm = static_cast<target_size *>(DesignatedInit( front, typ, ctyp, pm ));
        if( pm == NULL )
            break;
        if( m != n ) {
            elem_size = SizeOfArg( front, typ->object );
            if( typ->u.array->unspecified_dim
              && m > array_size ) {
                RelSeekBytes( front, ( array_size - n ) * elem_size );
                ZeroBytes( front, ( m - array_size ) * elem_size );
            } else {
                RelSeekBytes( front, ( m - n ) * elem_size );
            }
            n = m;
        }
        n++;
        if( n > array_size ) {
            if( !typ->u.array->unspecified_dim )
                break;
            array_size = n;
            /*
             * clear out the new element just in case
             */
            elem_size = SizeOfArg( front, typ->object );
            ZeroBytes( front, elem_size );
            RelSeekBytes( front, -(target_ssize)elem_size );
        }
        InitSymData( front, typ->object, ctyp, 1 );
        if( front.cvars.CurToken == T_EOF )
            break;
        if( front.cvars.CurToken == T_RIGHT_BRACE )
            break;
        if( DesignatedInSubAggregate( front, typ->object->decl_type ) )
            continue;
        if( n < array_size
          || typ == ctyp
          || typ->u.array->unspecified_dim ) {
            MustRecog( front, T_COMMA );
        }
        if( front.cvars.CurToken == T_RIGHT_BRACE ) {
            break;
        }
    }
    if( typ->u.array->unspecified_dim ) {
        typ->u.array->dimension = array_size;
    }
    if( array_size > n ) {
        RelSeekBytes( front, ( array_size - n ) * SizeOfArg( front, typ->object ) );
    }
}

static void InitStructUnion( FrontRun& front, TYPEPTR typ, TYPEPTR ctyp, FIELDPTR field )
/***********************************************************************
 * Initialize struct or union fields
 * Detects a C99 designated initializer for fields
 */
{
    TYPEPTR         ftyp;
    target_size     n;
    target_size     offset;

    /*
     * get full size of the struct or union
     */
    n = typ->u.tag->size;
    offset = 0;
    for( ;; ) {
        field = static_cast<FIELDPTR>(DesignatedInit( front, typ, ctyp, field ));
        if( field == NULL )
            break;
        /*
         * The first field might not start at offset 0
         */
        if( field->offset != offset ) {
            RelSeekBytes( front, field->offset - offset );
        }
        ftyp = field->field_type;
        offset = field->offset + SizeOfArg( front, ftyp );
        if( ftyp->decl_type == TYP_FIELD
          || ftyp->decl_type == TYP_UFIELD ) {
            InitBitField( front, field );
        } else {
            InitSymData( front, ftyp, ctyp, 1 );
        }
        field = field->next_field;
        if( typ->decl_type == TYP_UNION ) {
            if( offset < n ) {
                ZeroBytes( front, n - offset );    /* pad the rest */
            }
            offset = n;
            /*
             * designated initializers may still override this field
             */
            field = NULL;
        }
        if( front.cvars.CurToken == T_EOF )
            break;
        if( front.cvars.CurToken == T_RIGHT_BRACE )
            break;
        if( DesignatedInSubAggregate( front, ftyp->decl_type ) )
            continue;
        if( field != NULL
          || typ == ctyp ) {
            MustRecog( front, T_COMMA );
        }
        if( front.cvars.CurToken == T_RIGHT_BRACE ) {
            break;
        }
    }
    RelSeekBytes( front, n - offset );
}

static void InitStruct( FrontRun& front, TYPEPTR typ, TYPEPTR ctyp )
{
    InitStructUnion( front, typ, ctyp, typ->u.tag->u.field_list );
}

static void InitUnion( FrontRun& front, TYPEPTR typ, TYPEPTR ctyp )
{
    FIELDPTR            field;
    TYPEPTR             ftyp;

    /*
     * skip unnamed bit fields
     */
    for( field = typ->u.tag->u.field_list; field != NULL; field = field->next_field ) {
        ftyp = field->field_type;
        SkipTypedefs( ftyp );
        if( field->name[0] != '\0' )
            break;
        if( ftyp->decl_type == TYP_STRUCT )
            break;
        if( ftyp->decl_type == TYP_UNION ) {
            break;
        }
    }
    InitStructUnion( front, typ, ctyp, field );
}

void InitSymData( FrontRun& front, TYPEPTR typ, TYPEPTR ctyp, int level )
{
    TOKEN           token;
    target_size     size;

    token = front.cvars.CurToken;
    if( front.cvars.CurToken == T_LEFT_BRACE ) {
        NextToken(front);
        if( front.cvars.CurToken == T_RIGHT_BRACE
          || front.cvars.CurToken == T_COMMA ) {
            CErr1( front, ERR_EMPTY_INITIALIZER_LIST );
        }
    }
    /*
     * skip typedefs, go into enum base
     */
    typ = SkipTypeFluff( typ );
    size = SizeOfArg( front, typ );
    switch( typ->decl_type ) {
    case TYP_ARRAY:
        if( CharArray( front, typ->object ) ) {
            InitCharArray( front, typ );
        } else if( WCharArray( front, typ->object ) ) {
            InitWCharArray( front, typ );
        } else {
            if( token == T_LEFT_BRACE ) {
                ctyp = typ;
            } else if( level == 0 ) {
                CErr1( front, ERR_NEED_BRACES );
            }
            if( typ == ctyp ) {
                /*
                 * initialize new current type
                 *
                 * first zero out the whole array; otherwise
                 * overlapping fields caused by designated
                 * initializers will make life very difficult
                 */
                ZeroBytes( front, size );
                RelSeekBytes( front, -(target_ssize)size );
            }
            InitArray( front, typ, ctyp );
        }
        break;
    case TYP_FCOMPLEX:
    case TYP_DCOMPLEX:
    case TYP_LDCOMPLEX:
    case TYP_STRUCT:
        if( token == T_LEFT_BRACE ) {
            ctyp = typ;
        } else if( level == 0 ) {
            CErr1( front, ERR_NEED_BRACES );
        }
        if( typ == ctyp ) {
            /*
             * initialize new current type
             *
             * zero out all fields; otherwise overlapping fields caused
             * by designated initializers will make life very difficult
             */
            ZeroBytes( front, size );
            RelSeekBytes( front, -(target_ssize)size );
        }
        InitStruct( front, typ, ctyp );
        break;
    case TYP_UNION:
        if( token == T_LEFT_BRACE ) {
            ctyp = typ;
        } else if( level == 0 ) {
            CErr1( front, ERR_NEED_BRACES );
        }
        InitUnion( front, typ, ctyp );
        break;
    case TYP_CHAR:
    case TYP_UCHAR:
    case TYP_BOOL:
    case TYP_SHORT:
    case TYP_USHORT:
    case TYP_INT:
    case TYP_UINT:
    case TYP_LONG:
    case TYP_ULONG:
    case TYP_POINTER:
        StorePointer( front, typ, size );
        break;
    case TYP_LONG64:
    case TYP_ULONG64:
        StoreInt64( front, typ );
        break;
    case TYP_FLOAT:
    case TYP_DOUBLE:
    case TYP_FIMAGINARY:
    case TYP_DIMAGINARY:
        StoreFloat( front, typ->decl_type, size );
        break;
    case TYP_LONG_DOUBLE:
    case TYP_LDIMAGINARY:
//        StoreFloat( typ->decl_type, size );
        StoreFloat( front, TYP_DOUBLE, size );
        break;
    default:
        break;
    }
    if( token == T_LEFT_BRACE ) {
        if( front.cvars.CurToken == T_COMMA ) {
            NextToken(front);
        }
        if( front.cvars.CurToken != T_RIGHT_BRACE ) {
            CErr1( front, ERR_TOO_MANY_INITS );
        }
        while( front.cvars.CurToken != T_RIGHT_BRACE ) {
            if( front.cvars.CurToken == T_EOF )
                break;
            if( front.cvars.CurToken == T_SEMI_COLON )
                break;
            if( front.cvars.CurToken == T_LEFT_BRACE )
                break;
            NextToken(front);
        }
        MustRecog( front, T_RIGHT_BRACE );
    }
}


static bool CharArray( FrontRun& front, TYPEPTR typ )
{
    if( front.cvars.CurToken == T_STRING ) {
        SkipTypedefs( typ );
        if( typ->decl_type == TYP_CHAR
          || typ->decl_type == TYP_UCHAR ) {
            return( true );
        }
    }
    return( false );
}


static bool WCharArray( FrontRun& front, TYPEPTR typ )
{
    if( front.cvars.CurToken == T_STRING ) {
        SkipTypedefs( typ );
        if( typ->decl_type == TYP_SHORT
          || typ->decl_type == TYP_USHORT ) {
            return( true );
        }
    }
    return( false );
}

static void InitCharArray( FrontRun& front, TYPEPTR typ )
/***************************************
 * This function handles the initialization of statements like:
 * char  name[4] = "abcd";
 */
{
    target_size         len;
    STR_HANDLE          str_lit;
    target_size         size;
    DATA_QUAD           dq;

    str_lit = GetLiteral(front);
    if( front.cvars.CompFlags.wide_char_string )
        CErr1( front, ERR_TYPE_MISMATCH );
    len = str_lit->length;
    if( typ->u.array->unspecified_dim )
        typ->u.array->dimension = len;
    size = typ->u.array->dimension;
    if( len > size ) {
        if( ( len - size ) > 1 ) {
            CWarn1( front, ERR_LIT_TOO_LONG );
        }
        /*
         * chop the string
         */
        len = size;
        str_lit->length = size;
    }
    dq.type = QDT_CONST;
    dq.flags = Q_DATA;
    dq.u.string_leaf = str_lit;
    GenDataQuad( front, &dq, len );
    if( size > len ) {                  /* if array > len of literal */
        ZeroBytes( front, size - len );
    }
    front.cvars.CompFlags.non_zero_data = true;
}


static void InitWCharArray( FrontRun& front, TYPEPTR typ )
/****************************************
 * This function handles the initialization of statements like:
 * wchar_t  name[5] = L"abcd";
 */
{
    target_size         len;
    target_size         i;
    STR_HANDLE          str_lit;
    unsigned short      value;
    unsigned short      *pwc;
    target_size         size;
    DATA_QUAD           dq;

    dq.type = QDT_SHORT;
    dq.flags = Q_DATA;
    str_lit = GetLiteral(front);
    if( !front.cvars.CompFlags.wide_char_string )
        CErr1( front, ERR_TYPE_MISMATCH );
    len = str_lit->length / sizeof( unsigned short );
    if( typ->u.array->unspecified_dim ) {
        typ->u.array->dimension = len;
    }
    size = typ->u.array->dimension;
    if( len > size ) {
        if( ( len - size ) > 1 ) {
            CWarn1( front, ERR_LIT_TOO_LONG );
        }
        len = size;
    }
    pwc = (unsigned short *)str_lit->literal;
    for( i = 0; i < len; ++i ) {
        value = *pwc++;
        if( value != 0 )
            front.cvars.CompFlags.non_zero_data = true;
        dq.u_long_value1 = value;
        GenDataQuad( front, &dq, TARGET_WCHAR );
    }
    if( i < size ) {
        ZeroBytes( front, ( size - i ) * sizeof( unsigned short ) );
    }
    FreeLiteral( front, str_lit );
}


static void StoreFloat( FrontRun& front, DATA_TYPE dtype, target_size size )
{
    TREEPTR     tree;
    DATA_QUAD   dq;

    dq.type = (enum quad_type)dtype;
    dq.flags = Q_DATA;
    dq.u.double_value = 0.0;
    if( front.cvars.CurToken != T_RIGHT_BRACE ) {
        tree = SingleExpr(front);
        if( IsConstLeaf( tree ) ) {
            CastConstValue( front, tree, dtype );
            dq.u.double_value = tree->op.u2.float_value->ld.u.value;
        } else {
            CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
        }
        FreeExprTree( front, tree );
        if( dq.u.double_value != 0.0 ) {
            front.cvars.CompFlags.non_zero_data = true;
        }
    }
    GenDataQuad( front, &dq, size );
}

static void GenStaticDataQuad( FrontRun& front, SYM_HANDLE sym_handle )
{
    DATA_QUAD       dq;

    dq.type = QDT_STATIC;
    dq.flags = Q_DATA;
    dq.u.var.sym_handle = sym_handle;
    dq.u.var.offset = 0;
    GenDataQuad( front, &dq, 0 );
}

void StaticInit( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    TYPEPTR             typ;
    TYPEPTR             struct_typ;
    TYPEPTR             last_array;

    GenStaticDataQuad( front, sym_handle );
    front.cvars.CompFlags.non_zero_data = false;
    struct_typ = NULL;
    last_array = NULL;
    typ = sym->sym_type;
    /*
     * Follow chain of typedefs/structs/arrays
     */
    for( ;; ) {
        SkipTypedefs( typ );
        if( typ->decl_type == TYP_ARRAY ) {
            /*
             * Remember innermost array type
             */
            last_array = typ;
            typ = typ->object;
        } else if( typ->decl_type == TYP_STRUCT ) {
            FIELDPTR    field;

            /*
             * Remember outermost structure type
             */
            if( struct_typ == NULL ) {
                /*
                 * last_array cannot to be outside this struct!
                 */
                last_array = NULL;
                struct_typ = typ;
            }
            /*
             * Determine the type of the last field in the struct
             */
            field = typ->u.tag->u.field_list;
            if( field == NULL )
                break;
            while( field->next_field != NULL )
                field = field->next_field;
            typ = field->field_type;
        } else {
            break;
        }
    }
    typ = last_array;
    /*
     * If innermost array had unspecified dimension, create new types whose
     * dimensions will be determined by number of initializers
     */
    if( (typ != NULL)
      && typ->u.array->unspecified_dim ) {
        if( struct_typ == NULL ) {
            /*
             * Array was not inside struct
             */
            sym->sym_type = ArrayNode( front, typ->object );
            sym->sym_type->u.array->unspecified_dim = true;
        } else {
            typ = sym->sym_type;
            /*
             * Create new structure type
             */
            sym->sym_type = TypeNode( front, TYP_STRUCT, ArrayNode( front, last_array->object ) );
            sym->sym_type->u.tag = struct_typ->u.tag;
            struct_typ = sym->sym_type;
            /*
             * Create new array types as necessary
             */
            for( ;; ) {
                SkipTypedefs( typ );
                if( typ->decl_type != TYP_ARRAY )
                    break;
                sym->sym_type = ArrayNode( front, sym->sym_type );
                sym->sym_type->u.array->unspecified_dim = true;
                typ = typ->object;
            }
            typ = last_array;
        }
    } else {
        struct_typ = NULL;
    }
    SymReplace( front, sym, sym_handle );
    InitSymData( front, sym->sym_type, sym->sym_type, 0 );
    SymGet( front, sym, sym_handle );
    if( struct_typ != NULL ) {
        /*
         * Structure contains an unspecified length array as last field
         */
        struct_typ->object->u.array->dimension = typ->u.array->dimension;
        typ->u.array->unspecified_dim = true;
        typ->u.array->dimension = 0;    /* Reset back to 0 */
    }
    if( sym->u.var.segid == SEG_NULL ) {
        SetFarHuge( front, sym, false );
        SetSegment( front, sym );
        SetSegAlign( front, sym );
    }
}

static void AssignAggregate( FrontRun& front, TREEPTR lvalue, TREEPTR rvalue, TYPEPTR typ )
{
    TREEPTR tree;

    tree = ExprNode( front, lvalue, OPR_EQUALS, rvalue );
    tree->right->op.opr = OPR_PUSHSYM;
    tree->u.expr_type = typ;
    tree->op.u2.result_type = typ;
    AddStmt( front, tree );
}

static void AggregateVarDeclEquals( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    SYM_HANDLE  sym2_handle;
    SYM_ENTRY   sym2;

    sym2_handle = MakeNewDotSym( front, &sym2, 'X', sym->sym_type, SC_STATIC );
    sym2.flags |= SYM_INITIALIZED;
    front.cvars.CompFlags.initializing_data = true;
    StaticInit( front, &sym2, sym2_handle );
    front.cvars.CompFlags.initializing_data = false;
    SymReplace( front, &sym2, sym2_handle );
    /*
     * StaticInit will change sym2.sym_type if it is an incomplete array type
     */
    sym->sym_type = sym2.sym_type;
    AssignAggregate( front, VarLeaf( front, sym, sym_handle ),
                     VarLeaf( front, &sym2, sym2_handle ), sym->sym_type );
}

static void InitStructField( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle, target_size base, FIELDPTR field, TREEPTR value )
{
    TREEPTR     opnd;
    TYPEPTR     typ;
    FIELDPTR    ufield;

    typ = field->field_type;
    SkipTypedefs( typ );
    if( typ->decl_type == TYP_UNION ) {
        ufield = typ->u.tag->u.field_list;
        typ = ufield->field_type;
        SkipTypedefs( typ );
    }
    opnd = VarLeaf( front, sym, sym_handle );
    opnd = ExprNode( front, opnd, OPR_DOT, UIntLeaf( front, base + field->offset ) );
    opnd->u.expr_type = typ;
    opnd->op.u2.result_type = typ;
    AddStmt( front, AsgnOp( front, opnd, T_ASSIGN_LAST, value ) );
}

static void InitStructUnionVarZero( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle, int index, TYPEPTR typ )
{
    FIELDPTR    field;
    target_size base;

    base = index * SizeOfArg( front, typ );
    for( field = typ->u.tag->u.field_list; field != NULL; field = field->next_field ) {
        InitStructField( front, sym, sym_handle, base, field, IntLeaf( front, 0 ) );
    }
}

static void InitStructUnionVar( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle, int index, TYPEPTR typ )
{
    TREEPTR     value;
    FIELDPTR    field;
    FIELDPTR    new_field;
    FIELDPTR    last_field;
    TOKEN       token;
    target_size base;

    base = index * SizeOfArg( front, typ );
    last_field = typ->u.tag->u.field_list;
    for( field = typ->u.tag->u.field_list; field != NULL; field = field->next_field ) {
        new_field = static_cast<FIELDPTR>(DesignatedInit( front, typ, typ, field ));
        if( new_field != NULL ) {
            for( field = last_field->next_field; field != NULL; field = field->next_field ) {
                 if( field->offset >= new_field->offset  )
                     break;
                 InitStructField( front, sym, sym_handle, base, field, IntLeaf( front, 0 ) );
            }
            field = new_field;
        }
        token = front.cvars.CurToken;
        if( token == T_LEFT_BRACE ) // allow {}, and extra {expr}..}
            NextToken(front);
        if( front.cvars.CurToken == T_RIGHT_BRACE ) {
            value = IntLeaf( front, 0 );
        } else {
            value = CommaExpr(front);
        }
        InitStructField( front, sym, sym_handle, base, field, value );
        if( last_field->offset < field->offset )
            last_field = field;
        if( token == T_LEFT_BRACE )
            MustRecog( front, T_RIGHT_BRACE );
        if( front.cvars.CurToken == T_EOF )
            break;
        if( front.cvars.CurToken != T_RIGHT_BRACE ) {
            MustRecog( front, T_COMMA );
        }
        if( field->next_field == NULL ) {
            break;
        }
    }
}

static bool SimpleUnion( TYPEPTR typ )
{
    FIELDPTR    field;

    field = typ->u.tag->u.field_list;
    typ = field->field_type;
    SkipTypedefs( typ );
    switch( typ->decl_type ) {
    case TYP_ARRAY:
    case TYP_STRUCT:
    case TYP_UNION:
    case TYP_FIELD:
    case TYP_UFIELD:
        return( false );        // give up on these
    default:
        break;
    }
    return( true );
}

static bool SimpleStruct( TYPEPTR typ )
{
    FIELDPTR    field;

    if( typ->decl_type == TYP_UNION ) {
        return( false );
    }
    for( field = typ->u.tag->u.field_list; field != NULL; field = field->next_field ) {
        typ = field->field_type;
        SkipTypedefs( typ );
        switch( typ->decl_type ) {
        case TYP_UNION:
            if( SimpleUnion( typ ) ) {
                break;              // go 1 deep to get by MFC examples
            }
            /* fall through */
        case TYP_ARRAY:
        case TYP_STRUCT:
        case TYP_FIELD:
        case TYP_UFIELD:
            return( false );        // give up on these
        default:
            break;
        }
    }
    return( true );
}

static void InitArraySimpleVar( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle, TYPEPTR typ, int index, TREEPTR value )
{
    TREEPTR     opnd;

    opnd = VarLeaf( front, sym, sym_handle );
    opnd = ExprNode( front, opnd, OPR_INDEX, IntLeaf( front, index ) );
    opnd->u.expr_type = typ;
    opnd->op.u2.result_type = typ;
    AddStmt( front, AsgnOp( front, opnd, T_ASSIGN_LAST, value ) );
}

static void InitArrayVar( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle, TYPEPTR typ )
{
    target_size i;
    target_size n;
    TYPEPTR     typ2;
    SYM_HANDLE  sym2_handle;
    SYM_ENTRY   sym2;
    TOKEN       token;
    target_size j;
    target_size dim;

    typ2 = typ->object;
    SkipTypedefs( typ2 );
    switch( typ2->decl_type ) {
    case TYP_CHAR:
    case TYP_UCHAR:
    case TYP_SHORT:
    case TYP_USHORT:
    case TYP_INT:
    case TYP_UINT:
    case TYP_LONG:
    case TYP_ULONG:
    case TYP_LONG64:
    case TYP_ULONG64:
    case TYP_FLOAT:
    case TYP_DOUBLE:
    case TYP_POINTER:
    case TYP_LONG_DOUBLE:
    case TYP_FIMAGINARY:
    case TYP_DIMAGINARY:
    case TYP_LDIMAGINARY:
    case TYP_BOOL:
        /*
         * skip over T_LEFT_BRACE
         */
        NextToken(front);
        if( CharArray( front, typ->object ) ) {
            sym2_handle = MakeNewDotSym( front, &sym2, 'X', typ, SC_STATIC );
            sym2.flags |= SYM_INITIALIZED;
            if( sym2.u.var.segid == SEG_NULL ) {
                SetFarHuge( front, &sym2, false );
                SetSegment( front, &sym2 );
                SetSegAlign( front, &sym2 );
            }
            SymReplace( front, &sym2, sym2_handle );
            GenStaticDataQuad( front, sym2_handle );
            InitCharArray( front, typ );
            AssignAggregate( front, VarLeaf( front, sym, sym_handle ),
                             VarLeaf( front, &sym2, sym2_handle ), typ );
        } else if( WCharArray( front, typ->object ) ) {
            sym2_handle = MakeNewDotSym( front, &sym2, 'X', typ, SC_STATIC );
            sym2.flags |= SYM_INITIALIZED;
            if( sym2.u.var.segid == SEG_NULL ) {
                SetFarHuge( front, &sym2, false );
                SetSegment( front, &sym2 );
                SetSegAlign( front, &sym2 );
            }
            SymReplace( front, &sym2, sym2_handle );
            GenStaticDataQuad( front, sym2_handle );
            InitWCharArray( front, typ );
            AssignAggregate( front, VarLeaf( front, sym, sym_handle ),
                             VarLeaf( front, &sym2, sym2_handle ), typ );
        } else {
            n = typ->u.array->dimension;
            dim = 0;
            i = 0;
            for( ;; ) {     // accept some C++ { {1},.. }
                if( DesignatedInit( front, typ, sym->sym_type, &i ) != NULL ) {
                    for( j = dim; j < i; j++ ) {
                        InitArraySimpleVar( front, sym, sym_handle, typ2, j, IntLeaf( front, 0 ) );
                    }
                }
                token = front.cvars.CurToken;
                if( token == T_LEFT_BRACE )
                    NextToken(front);
                InitArraySimpleVar( front, sym, sym_handle, typ2, i, CommaExpr(front) );
                if( token == T_LEFT_BRACE )
                    MustRecog( front, T_RIGHT_BRACE );
                ++i;
                if( dim < i )
                    dim = i;
                if( front.cvars.CurToken == T_EOF )
                    break;
                if( front.cvars.CurToken == T_RIGHT_BRACE )
                    break;
                MustRecog( front, T_COMMA );
                if( front.cvars.CurToken == T_RIGHT_BRACE )
                    break;
                if( n
                  && i >= n ) {
                    CErr1( front, ERR_TOO_MANY_INITS );
                }
            }
            if( typ->u.array->unspecified_dim ) {
                typ->u.array->dimension = dim;
            } else {
                for( i = dim; i < n; i++ ) {
                    InitArraySimpleVar( front, sym, sym_handle, typ2, i, IntLeaf( front, 0 ) );
                }
            }
        }
        MustRecog( front, T_RIGHT_BRACE );
        break;
    case TYP_FCOMPLEX:
    case TYP_DCOMPLEX:
    case TYP_LDCOMPLEX:
    case TYP_STRUCT:
    case TYP_UNION:
        if( SimpleStruct( typ2 ) ) {
            /*
             * skip over T_LEFT_BRACE
             */
            NextToken(front);
            n = typ->u.array->dimension;
            dim = 0;
            i = 0;
            for( ;; ) {
                if( DesignatedInit( front, typ, typ, &i ) != NULL ) {
                    for( j = dim; j < i; j++ ) {
                        InitStructUnionVarZero( front, sym, sym_handle, j, typ2 );
                    }
                }
                token = front.cvars.CurToken;
                if( token == T_LEFT_BRACE ) {
                    NextToken(front);
                }
                InitStructUnionVar( front, sym, sym_handle, i, typ2 );
                if( token == T_LEFT_BRACE ) {
                    MustRecog( front, T_RIGHT_BRACE );
                }
                ++i;
                if( dim < i )
                    dim = i;
                if( front.cvars.CurToken == T_EOF )
                    break;
                if( front.cvars.CurToken == T_RIGHT_BRACE )
                    break;
                MustRecog( front, T_COMMA );
                if( front.cvars.CurToken == T_RIGHT_BRACE )
                    break;
                if( n
                  && i >= n ) {
                    CErr1( front, ERR_TOO_MANY_INITS );
                }
            }
            if( typ->u.array->unspecified_dim ) {
                typ->u.array->dimension = dim;
            } else {
                for( i = dim; i < n; i++ ) { // mop up
                    InitStructUnionVarZero( front, sym, sym_handle, i, typ2 );
                }
            }
            /*
             * skip over T_RIGHT_BRACE
             */
            NextToken(front);
            break;
        }
        /* fall through */
    default:
        AggregateVarDeclEquals( front, sym, sym_handle );
        break;
    }
}

void VarDeclEquals( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    TYPEPTR     typ;

    if( front.cvars.SymLevel == 0
      || sym->attribs.stg_class == SC_STATIC ) {
        if( sym->flags & SYM_INITIALIZED ) {
            CErrSymName( front, ERR_VAR_ALREADY_INITIALIZED, sym, sym_handle );
        }
        sym->flags |= SYM_INITIALIZED;
        front.cvars.CompFlags.initializing_data = true;
        StaticInit( front, sym, sym_handle );
        front.cvars.CompFlags.initializing_data = false;
    } else {
        SymReplace( front, sym, sym_handle );
        front.cvars.SrcLoc = front.cvars.TokenLoc;
        typ = sym->sym_type;
        SkipTypedefs( typ );
        /*
         * check for { before checking for array, struct or union
         */
        if( front.cvars.CurToken != T_LEFT_BRACE
          && typ->decl_type != TYP_ARRAY ) {
            AddStmt( front, AsgnOp( front, VarLeaf( front, sym, sym_handle ), T_ASSIGN_LAST, CommaExpr(front) ) );
            sym->flags |= SYM_ASSIGNED;
        } else if( typ->decl_type == TYP_ARRAY ) {
            if( front.cvars.CurToken == T_LEFT_BRACE
              && front.cvars.CompFlags.auto_agg_inits ) {
                InitArrayVar( front, sym, sym_handle, typ );
            } else {
                AggregateVarDeclEquals( front, sym, sym_handle );
            }
        } else if( typ->decl_type == TYP_STRUCT
          || typ->decl_type == TYP_UNION ) {
            if( front.cvars.CurToken == T_LEFT_BRACE
              && front.cvars.CompFlags.auto_agg_inits
              && SimpleStruct( typ ) ) {
                NextToken(front);    /* skip T_LEFT_BRACE */
                InitStructUnionVar( front, sym, sym_handle, 0, typ );
                NextToken(front);    /* skip T_RIGHT_BRACE */
            } else {
                AggregateVarDeclEquals( front, sym, sym_handle );
            }
        } else {
            NextToken(front);
            AddStmt( front, AsgnOp( front, VarLeaf( front, sym, sym_handle ), T_ASSIGN_LAST, CommaExpr(front) ) );
            sym->flags |= SYM_ASSIGNED;
            MustRecog( front, T_RIGHT_BRACE );
        }
    }
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
CdinitState* cdinit_make() { return new CdinitState(); }
void cdinit_free(CdinitState* part) { delete part; }
