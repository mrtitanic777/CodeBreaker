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
* Description:  Manage a list of names.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "hwreg.h"
#include "freelist.h"
#include "zoiks.h"
#include "namelist.h"
#include "data.h"
#include "seldef.h"
#include "bldsel.h"
#include "intrface.h"
#include "rgtbl.h"
#include "typemap.h"
#include "i64.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct NamelistState: namelist_state.h) */

}  // namespace



static const int    Size[] = {
    #define pick(e,s)   s,
    #include "nclass.h"
    #undef pick
};

static const type_class_def OneClass[] = {
    U1,             /* U1 */
    U1,             /* I1 */
    U2,             /* U2 */
    U2,             /* I2 */
    U4,             /* U4 */
    U4,             /* I4 */
    U8,             /* U8 */
    U8,             /* U8 */
    CP,             /* CP */
    PT,             /* PT */
    FS,             /* FS */
    FD,             /* FD */
    FL,             /* FL */
    XX              /* XX */
};


static name *AllocName( BackRun& back, name_class_def class_, type_class_def type_class, type_length size )
/*****************************************************************************************/
{
    name        *new_name;

    new_name = static_cast<name *>(AllocFrl( back, &back.namelist.FrlHead[class_], Size[class_] ));
    new_name->n.class_ = class_;
    new_name->n.next_name = back.data.Names[class_];
    back.data.Names[class_] = new_name;
    new_name->n.type_class = type_class;
    new_name->n.size = back.data.TypeClassSize[type_class];
    if( new_name->n.size == 0 ) {
        new_name->n.size = size;
    }
    return( new_name );
}

static name *findConst64( BackRun& back, uint_32 low, uint_32 high, float_handle cf_value )
/**************************************************************************/
{
    name        *new_c;
    name        **last;

    last = &back.data.Names[N_CONSTANT];
    for( new_c = back.data.Names[N_CONSTANT]; new_c != NULL; new_c = new_c->n.next_name ) {
        if( new_c->c.const_type == CONS_ABSOLUTE ) {
            if( new_c->c.lo.u.uint_value == low
              && new_c->c.hi.u.uint_value == high ) {
                if( CFCompare( new_c->c.u.cfval, cf_value ) == 0 ) {
                    // move constant found to front of list
                    *last = new_c->n.next_name;
                    new_c->n.next_name = back.data.Names[N_CONSTANT];
                    back.data.Names[N_CONSTANT] = new_c;
                    break;
                }
            }
        }
        last = &new_c->n.next_name;
    }
    return( new_c );
}


static void ZapXX( BackRun& back, name *xx, type_class_def type_class, type_length size )
/************************************************************************/
{
    if( type_class != XX ) {                     /* if he's making a type with same size as xx */
        xx->n.type_class = type_class;           /* zap the XX one to be a real type */
        xx->n.size = back.data.TypeClassSize[type_class];
    } else if( size != 0 ) {
        xx->n.size = size;
    }
}


name    *AllocConst( BackRun& back, float_handle cf_value )
/******************************************/
{
    name        *new_c;
    name        **last;
    int_32      int_value;
    int         test;

    int_value = CFCnvF32( cf_value );
    test = CFTest( cf_value );
    if( test == 0
      && back.namelist.ConstZero != NULL ) {
        CFFree( back, &back.cfloat.cgh, cf_value );
        return( back.namelist.ConstZero );
    }
    if( int_value == 1
      && back.namelist.ConstOne != NULL ) {
        CFFree( back, &back.cfloat.cgh, cf_value );
        return( back.namelist.ConstOne );
    }
    last = &back.data.Names[N_CONSTANT];
    for( new_c = back.data.Names[N_CONSTANT]; new_c != NULL; new_c = new_c->n.next_name ) {
        if( new_c->c.const_type == CONS_ABSOLUTE ) {
            if( new_c->c.lo.u.int_value == int_value ) {
                if( CFCompare( new_c->c.u.cfval, cf_value ) == 0 ) {
                    CFFree( back, &back.cfloat.cgh, cf_value );
                    // move constant found to front of list
                    *last = new_c->n.next_name;
                    new_c->n.next_name = back.data.Names[N_CONSTANT];
                    back.data.Names[N_CONSTANT] = new_c;
                    return( new_c );
                }
            }
        }
        last = &new_c->n.next_name;
    }
    new_c = AllocName( back, N_CONSTANT, XX, 0 );
    new_c->c.u.cfval = cf_value;
    new_c->c.lo.u.int_value = int_value;
    if( test < 0 ) {
        new_c->c.hi.u.int_value = -1; //sign extend
    } else {
        new_c->c.hi.u.int_value = 0;
    }
    new_c->c.static_defn = NULL;
    new_c->c.const_type = CONS_ABSOLUTE;
    if( back.namelist.ConstOne == NULL
      && new_c->c.lo.u.int_value == 1 ) {
        back.namelist.ConstOne = new_c;
    }
    if( back.namelist.ConstZero == NULL
      && new_c->c.lo.u.int_value == 0
      && test == 0 ) {
        back.namelist.ConstZero = new_c;
    }
    if( int_value == 0 ) {
        if( CFIsI64( cf_value )
          || CFIsU64( cf_value ) ) {
            unsigned_64         i64val;

            i64val = CFCnvF64( cf_value );
            new_c->c.lo.u.int_value = I64Low( i64val );
            new_c->c.hi.u.int_value = I64High( i64val );
        }
    }
    return( new_c );
}

name    *AllocAddrConst( BackRun& back, name *op, int seg, constant_type_class const_type, type_class_def type_class )
/*****************************************************************************************************/
{
    name        *new_c;

    for( new_c = back.data.Names[N_CONSTANT]; new_c != NULL; new_c = new_c->n.next_name ) {
        if( new_c->c.const_type == const_type
          && new_c->c.u.op == op
          && new_c->n.type_class == type_class
          && new_c->c.lo.u.int_value == seg ) {
            return( new_c );
        }
    }
    new_c = AllocName( back, N_CONSTANT, type_class, 0 );
    new_c->c.u.op = op;
    new_c->c.lo.u.int_value = seg;
    new_c->c.static_defn = NULL;
    new_c->c.const_type = const_type;
    return( new_c );
}


name    *FindIntValue( BackRun& back, int_32 value )
/***********************************/
{
    if( value == 0
      && back.namelist.ConstZero != NULL )
        return( back.namelist.ConstZero );
    if( value == 1
      && back.namelist.ConstOne != NULL )
        return( back.namelist.ConstOne );
    return( NULL );
}


name    *AllocIntConst( BackRun& back, int value )
/*********************************/
{
    name        *konst;

    konst = FindIntValue( back, value );
    if( konst != NULL )
        return( konst );
    return( AllocConst( back, CFCnvIF( back, &back.cfloat.cgh, value ) ) );
}


name    *AllocUIntConst( BackRun& back, uint value )
/***********************************/
{
    name        *konst;

    konst = FindIntValue( back, value );
    if( konst != NULL )
        return( konst );
    return( AllocConst( back, CFCnvUF( back, &back.cfloat.cgh, value ) ) );
}


name    *AllocS32Const( BackRun& back, int_32 value )
/************************************/
{
    name        *konst;

    konst = FindIntValue( back, value );
    if( konst != NULL )
        return( konst );
    return( AllocConst( back, CFCnvI32F( back, &back.cfloat.cgh, value ) ) );
}

name    *AllocU32Const( BackRun& back, uint_32 value )
/*************************************/
{
    name        *konst;

    konst = FindIntValue( back, value );
    if( konst != NULL )
        return( konst );
    return( AllocConst( back, CFCnvU32F( back, &back.cfloat.cgh, value ) ) );
}

name    *AllocS64Const( BackRun& back, uint_32 low, uint_32 high )
/*************************************************/
{
    name            *new_c;
    float_handle    cf_value = CFCnvI64F( back, &back.cfloat.cgh, low, high );

    new_c = findConst64( back, low, high, cf_value );
    if( new_c == NULL ) {
        new_c = AllocName( back, N_CONSTANT, XX, 0 );
        new_c->c.u.cfval = cf_value;
        new_c->c.lo.u.uint_value = low;
        new_c->c.hi.u.uint_value = high;
        new_c->c.static_defn = NULL;
        new_c->c.const_type = CONS_ABSOLUTE;
    } else {
        CFFree( back, &back.cfloat.cgh, cf_value );
    }
    return( new_c );
}

name    *AllocU64Const( BackRun& back, uint_32 low, uint_32 high )
/*************************************************/
{
    name            *new_c;
    float_handle    cf_value = CFCnvU64F( back, &back.cfloat.cgh, low, high );

    new_c = findConst64( back, low, high, cf_value );
    if( new_c == NULL ) {
        new_c = AllocName( back, N_CONSTANT, XX, 0 );
        new_c->c.u.cfval = cf_value;
        new_c->c.lo.u.uint_value = low;
        new_c->c.hi.u.uint_value = high;
        new_c->c.static_defn = NULL;
        new_c->c.const_type = CONS_ABSOLUTE;
    } else {
        CFFree( back, &back.cfloat.cgh, cf_value );
    }
    return( new_c );
}

constant_defn   *GetFloat( BackRun& back, name *cons, type_class_def type_class )
/****************************************************************/
{
    constant_defn       *defn;

    for( defn = cons->c.static_defn; defn != NULL; defn = defn->next_defn ) {
        if( defn->const_class == type_class ) {
            return( defn );
        }
    }
    defn = static_cast<constant_defn *>(AllocFrl( back, &back.namelist.ConstDefnFrl, sizeof( constant_defn ) ));
    defn->const_class = type_class;
    defn->label = NULL;
    CFCnvTarget( cons->c.u.cfval, &defn->buffer, back.data.TypeClassSize[type_class] );
    defn->next_defn = cons->c.static_defn;
    cons->c.static_defn = defn;
    return( defn );
}


name    *SAllocMemory( BackRun& back, pointer symbol, type_length offset, cg_class class_,
                            type_class_def type_class, type_length size )
/************************************************************************/
{
    name        *new_m;
    name        *other;
    name        *xx;

    other = NULL;
    xx = NULL;
    for( new_m = back.data.Names[N_MEMORY]; new_m != NULL; new_m = new_m->m._v._n.next_name ) {
        if( new_m->m._v.symbol == symbol
          && new_m->m.memory_type == class_ ) {
            if( new_m->m._v.offset != offset ) {
                other = new_m;
                new_m->m._v.usage |= USE_MEMORY | NEEDS_MEMORY;
            } else {
                if( type_class == XX
                  && size == 0 )
                    return( new_m );
                if( new_m->m._v._n.type_class == type_class   /*exact!*/
                  && type_class != XX ) {
                    return( new_m );
                }
                if( new_m->m._v._n.type_class == XX
                  && new_m->m._v._n.size == size ) {
                    xx = new_m;
                }
                other = new_m;
                new_m->m._v.usage |= USE_MEMORY | NEEDS_MEMORY;
            }
        }
    }
    if( xx != NULL ) {
        ZapXX( back, xx, type_class, size );
        return( xx );
    }
    new_m = AllocName( back, N_MEMORY, type_class, size );
    new_m->m._v.symbol = symbol;
    new_m->m._v.offset = offset;
    new_m->m.memory_type = class_;
    new_m->m.alignment = 0;
    if( other != NULL ) {
        if( other->m.same_sym != NULL ) {
            new_m->m.same_sym = other->m.same_sym;
        } else {
            new_m->m.same_sym = other;
        }
        other->m.same_sym = new_m;
        new_m->m._v.usage = USE_MEMORY | NEEDS_MEMORY;
    } else {
        new_m->m._v.usage = NEEDS_MEMORY;
        new_m->m.same_sym = NULL;
    }
    if( class_ == CG_FE
      && _IsModel( CGSW_GEN_NO_OPTIMIZATION ) ) {
        new_m->m._v.usage |= USE_MEMORY;
    }
    new_m->m._v.block_usage = static_cast<var_usage>(0);
    new_m->m._v.conflict = NULL;
    if( class_ == CG_LBL
      || class_ == CG_CLB ) {
        new_m->m._v.usage |= USE_MEMORY; /* so not put in conflict graph*/
    }
    return( new_m );
}


name    *AllocMemory( BackRun& back, pointer symbol, type_length offset, cg_class class_, type_class_def type_class )
/***************************************************************************************************/
{
    return( SAllocMemory( back, symbol, offset, class_, type_class, 0 ) );
}


name    *STempOffset( BackRun& back, name *temp, type_length offset, type_class_def type_class, type_length size )
/*************************************************************************************************/
{
    name        *new_t;
    name        *xx;

    type_class = OneClass[type_class];
    offset += temp->v.offset;
    new_t = temp->t.alias;
    xx = NULL;
    for( ;; ) {
        if( new_t->t.v.id == temp->t.v.id
          && new_t->v.offset == offset ) {
            if( type_class == XX
              && size == 0 )
                return( new_t ); /* 89-07-07 */
            if( new_t->n.type_class == type_class
              && type_class != XX ) {
                return( new_t ); /* exact match */
            }
            if( new_t->n.type_class == XX
              && new_t->n.size == size ) {
                xx = new_t; /* an XX with the right size */
            }
        }
        if( new_t == temp )
            break;
        new_t = new_t->t.alias;
    }
    if( xx != NULL ) {
        ZapXX( back, xx, type_class, size );
        return( xx );
    }
    new_t = AllocName( back, N_TEMP, type_class, size );
    new_t->t.v.id = temp->t.v.id;
    new_t->v.symbol = temp->v.symbol;
    new_t->v.offset = offset;
    new_t->v.usage = temp->v.usage;
    new_t->v.block_usage = static_cast<var_usage>(0);
    new_t->v.conflict = temp->v.conflict;
    new_t->t.u.blk_id = temp->t.u.blk_id;
    new_t->t.alias = temp->t.alias;
    temp->t.alias = new_t;
    new_t->t.temp_flags = ALIAS;
    new_t->t.temp_flags |= static_cast<t_flags>(temp->t.temp_flags & PERM_TEMP_FLAGS);
    if( temp->t.location == NO_LOCATION ) {
        new_t->t.location = NO_LOCATION;
    } else {
        new_t->t.location = temp->t.location + offset;
    }
    return( new_t );
}


name    *SAllocTemp( BackRun& back, type_class_def type_class, type_length size )
/****************************************************************/
{
    name        *new_t;

    type_class = OneClass[type_class];
    new_t = AllocName( back, N_TEMP, type_class, size );
    new_t->t.v.id = ++back.data.TempId;
    new_t->v.symbol = NULL;
    new_t->v.offset = 0;
    new_t->v.usage = static_cast<var_usage>(0);
    new_t->v.block_usage = static_cast<var_usage>(0);
    new_t->v.conflict = NULL;
    new_t->t.u.blk_id = BLK_ID_NONE;
    new_t->t.location = NO_LOCATION;
    new_t->t.alias = new_t;
    new_t->t.temp_flags = static_cast<t_flags>(0);
    return( new_t );
}


name    *AllocTemp( BackRun& back, type_class_def type_class )
/****************************************/
{
    return( SAllocTemp( back, type_class, 0 ) );
}


name    *TempOffset( BackRun& back, name *temp, type_length offset, type_class_def type_class )
/******************************************************************************/
{
    return( STempOffset( back, temp, offset, type_class, 0 ) );
}


name    *SAllocUserTemp( BackRun& back, pointer symbol, type_class_def type_class, type_length size )
/************************************************************************************/
{
    name        *new_t;

    type_class = OneClass[type_class];
    new_t = LkAddBackUserTemp( back, static_cast<cg_sym_handle>(symbol), NULL );
    if( new_t == NULL ) {
        new_t = AllocTemp( back, type_class );
        new_t->v.symbol = symbol;
        if( size != 0 ) {
            new_t->n.size = size;
        }
        LkAddBackUserTemp( back, static_cast<cg_sym_handle>(symbol), new_t );
        return( new_t );
    } else {
        if( new_t->n.type_class == type_class
          && type_class != XX )
            return( new_t );
        return( STempOffset( back, new_t, 0, type_class, size ) );
    }
}


name    *AllocUserTemp( BackRun& back, pointer symbol, type_class_def type_class )
/*****************************************************************/
{
    return( SAllocUserTemp( back, symbol, type_class, 0 ) );
}


name    *DeAlias( name *temp )
/****************************/
{
    while( temp->t.temp_flags & ALIAS ) {
        temp = temp->t.alias;
    }
    return( temp );
}


name    *AllocRegName( BackRun& back, hw_reg_set regs )
/**************************************/
{
    name        *new_r;

    if( HW_CEqual( regs, HW_EMPTY ) ) { /* here for speed. we grab this a lot! */
        if( back.namelist.NullReg == NULL ) {
            new_r = AllocName( back, N_REGISTER, RegClass( back, regs ), 0 );
            HW_CAsgn( new_r->r.reg, HW_EMPTY );
            new_r->r.reg_index = -1;
            back.namelist.NullReg = new_r;
        }
        return( back.namelist.NullReg );
    }
    for( new_r = back.data.Names[N_REGISTER]; new_r != NULL; new_r = new_r->n.next_name ) {
        if( HW_Equal( new_r->r.reg, regs ) ) {
            return( new_r );
        }
    }
    new_r = AllocName( back, N_REGISTER, RegClass( back, regs ), 0 );
    new_r->r.reg = regs;
    new_r->r.reg_index = -1;
    return( new_r );
}


name    *ScaleIndex( BackRun& back, name *index, name *base, type_length offset, type_class_def type_class,
                                        type_length size, scale_typ scale, i_flags flags )
/*************************************************************************************/
{
    name        *new_x;

    type_class = OneClass[type_class];
    for( new_x = back.data.Names[N_INDEXED]; new_x != NULL; new_x = new_x->n.next_name ) {
        if( new_x->i.base == base
          && new_x->i.index == index
          && new_x->i.constant == offset
          && new_x->i.scale == scale
          && new_x->i.index_flags == flags
          && ( ( new_x->n.type_class == type_class
          && new_x->n.type_class != XX )
          || ( new_x->n.type_class == XX
          && new_x->n.size == size ) ) ) {
            if( type_class != XX ) {
                new_x->n.type_class = type_class;
                new_x->n.size = back.data.TypeClassSize[type_class];
            }
            if( index->n.class_ == N_TEMP ) {
                index->t.temp_flags |= INDEXED;
            }
            return( new_x );
        }
    }
    new_x = AllocName( back, N_INDEXED, type_class, size );
    new_x->i.index = index;
    new_x->i.base = base;
    new_x->i.constant = offset;
    new_x->i.index_flags = flags;
    new_x->i.scale = scale;
    if( index->n.class_ == N_TEMP ) {
        index->t.temp_flags |= INDEXED;
    }
    return( new_x );
}


name    *SAllocIndex( BackRun& back, name *index, name *base, type_length offset,
                            type_class_def type_class, type_length size )
/***********************************************************************/
{
    return( ScaleIndex( back, index, base, offset, type_class, size, SCALE_NONE, static_cast<i_flags>(EMPTY) ) );
}


name    *AllocIndex( BackRun& back, name *index, name *base, type_length offset,
                                        type_class_def type_class )
/*****************************************************************/
{
    return( SAllocIndex( back, index, base, offset, type_class, 0 ) );
}


void    InitNames( BackRun& back )
/***********************/
{
    name_class_def      class_;
    type_class_def      type_class;

    for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; ++class_ ) {
        InitFrl( &back.namelist.FrlHead[class_] );
    }
    InitFrl( &back.namelist.ConstDefnFrl );
    for( type_class = U1; type_class <= XX; ++type_class ) {
        back.data.TypeClassSize[type_class] = TypeOfTypeClass( back, type_class )->length;
    }
    ReInitNames(back);
}


void    ReInitNames( BackRun& back )
/*************************/
{
    name_class_def  class_;

    back.data.TempId = 0;
    back.namelist.NullReg = NULL;
    back.namelist.ConstOne = NULL;
    back.namelist.ConstZero = NULL;
    back.data.LastTemp = NULL;
    back.data.DummyIndex = NULL;
    for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; ++class_ ) {
        back.data.Names[class_] = NULL;
    }
}


void    FreeNames( BackRun& back )
/***********************/
{
    name_class_def  class_;
    name            *temp;
    name            *next;

    for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; ++class_ ) {
        for( temp = back.data.Names[class_]; temp != NULL; temp = next ) {
            next = temp->n.next_name;
            FreeAName( back, temp );
        }
        back.data.Names[class_] = NULL;
    }
}


void    FreeAName( BackRun& back, name *op )
/***************************/
{
    constant_defn       *defn;
    constant_defn       *next;

    if( op->n.class_ == N_CONSTANT ) {
        if( op == back.namelist.ConstZero ) {
            back.namelist.ConstZero = NULL;
        }
        if( op == back.namelist.ConstOne ) {
            back.namelist.ConstOne = NULL;
        }
        if( op->c.const_type == CONS_ABSOLUTE ) {
            CFFree( back, &back.cfloat.cgh, op->c.u.cfval );
            for( defn = op->c.static_defn; defn != NULL; defn = next ) {
                next = defn->next_defn;
                FrlFreeSize( back, &back.namelist.ConstDefnFrl, (pointer *)defn, sizeof( constant_defn ) );
            }
        }
    } else if( op->n.class_ == N_MEMORY ) {
        if( op->m.memory_type == CG_TBL ) {
            FreeTable( back, static_cast<tbl_control *>(op->v.symbol) );
        }
    } else if( op->n.class_ == N_REGISTER ) {
        if( op == back.namelist.NullReg ) {
            back.namelist.NullReg = NULL;
        }
    } else if( op->n.class_ == N_TEMP ) {
        if( op == back.data.LastTemp )
            back.data.LastTemp = op->n.next_name;
        if( op == back.data.DummyIndex ) {
            back.data.DummyIndex = NULL;
        }
    }
    FrlFreeSize( back, &back.namelist.FrlHead[op->n.class_], (pointer *)op, Size[op->n.class_] );
}


bool    NameFrlFree( BackRun& back )
/*************************/
{
    bool            freed;
    name_class_def  class_;

    freed = false;
    for( class_ = static_cast<name_class_def>(0); class_ < N_CLASS_MAX; ++class_ ) {
        freed |= FrlFreeAll( back, &back.namelist.FrlHead[class_], Size[class_] );
    }
    freed |= FrlFreeAll( back, &back.namelist.ConstDefnFrl, sizeof( constant_defn ) );
    return( freed );
}

i_flags AlignmentToFlags( BackRun& back, type_length alignment )
/***********************************************/
{
    i_flags             flags;

    flags = static_cast<i_flags>(0);
    switch( alignment ) {
    case 0:
        break;
    case 1:
        flags |= X_ALIGNED_1;
        break;
    case 2:
        flags |= X_ALIGNED_2;
        break;
    case 4:
        flags |= X_ALIGNED_4;
        break;
    case 8:
        flags |= X_ALIGNED_8;
        break;
    default:
        _Zoiks( ZOIKS_098 );
    }
    return( flags );
}

type_length     FlagsToAlignment( i_flags flags )
/***********************************************/
{
    type_length         alignment;

    alignment = 0;
    if( flags & X_ALIGNED_1 ) {
        alignment = 1;
    }
    if( flags & X_ALIGNED_2 ) {
        alignment = 2;
    }
    if( flags & X_ALIGNED_4 ) {
        alignment = 4;
    }
    if( flags & X_ALIGNED_8 ) {
        alignment = 8;
    }
    return( alignment );
}
