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
#include "cgmem.h"
#include "symdbg.h"
#include "model.h"
#include "types.h"
#include "zoiks.h"
#ifdef DEVBUILD
#include "echoapi.h"
#endif
#include "i64.h"
#include "utils.h"
#if _TARGET_INTEL
#include "wvtypes.h"
#endif
#include "dw.h"
#include "dftypes.h"
#include "cvdbg.h"
#include "cvtypes.h"
#include "dbsupp.h"
#include "cgprotos.h"
#include "backrun.hpp"


#define MAX_TYPE_SIZE  (1024 * 16)

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct DbtypesState: dbtypes_state.h) */

}  // namespace



/*
  need to fix dbg_type so it's a handle that can be used
  by any format WV, DW and CV.
  figure how fundamental types are done
*/
dbg_type _CGAPI DBFtnType( BackRun& back, cchar_ptr name, dbg_ftn_type tipe )
/********************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBFtnType( %c, %i )", name, tipe );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFFtnType( back, name, tipe );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVFtnType( name, tipe );
    } else {
#if _TARGET_INTEL
        ret = WVFtnType( back, name, tipe );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


dbg_type _CGAPI DBScalar( BackRun& back, cchar_ptr name, cg_type tipe )
/**************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBScalar( %c,%t )", name, tipe );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFScalar( back, name, tipe );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVScalar( back, name, tipe );
    } else {
#if _TARGET_INTEL
        ret = WVScalar( back, name, tipe );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


dbg_type _CGAPI DBScope( BackRun& back, cchar_ptr name )
/***********************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBScope( %c )", name );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFScope( name );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVScope( name );
    } else {
#if _TARGET_INTEL
        ret = WVScope( back, name );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


dbg_name _CGAPI DBBegName( BackRun& back, cchar_ptr nm, dbg_type scope )
/***************************************************************/
{
    dbg_name    name;
    uint        len;

#ifdef DEVBUILD
    EchoAPI( "DBBegName( %c, %i )", nm, scope );
#endif
    len = strlen( nm );
    name = static_cast<dbg_name>(CGAlloc( back, sizeof( name_entry ) + len ));
    strcpy( name->name, nm );
    name->len = len;
    name->scope = scope;
    name->refno = DBG_NIL_TYPE;
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", name );
#endif
    return( name );
}


dbg_type _CGAPI DBForward( BackRun& back, dbg_name name )
/************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DBForward( %i )", name );
#endif
    if( name->refno == DBG_NIL_TYPE ) {
        if( _IsModel( CGSW_GEN_DBG_DF ) ) {
            /* do nothing */
        } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
            CVDumpName( back, name, DBG_FWD_TYPE );
#if _TARGET_INTEL
        } else {
            WVDumpName( back, name, DBG_FWD_TYPE );
#endif
        }
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", name->refno );
#endif
    return( name->refno );
}


dbg_type _CGAPI DBEndName( BackRun& back, dbg_name name, dbg_type tipe )
/***************************************************************/
{
    dbg_type    retv;

#ifdef DEVBUILD
    EchoAPI( "DBEndName( %i, %i )", name, tipe );
#endif
    if( name->refno == DBG_NIL_TYPE ) {
        if( _IsModel( CGSW_GEN_DBG_DF ) ) {
           DFDumpName( back, name, tipe );
        } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
           CVDumpName( back, name, tipe );
#if _TARGET_INTEL
        } else {
           WVDumpName( back, name, tipe );
#endif
        }
    } else if( _IsModel( CGSW_GEN_DBG_TYPES ) ) {
        if( _IsModel( CGSW_GEN_DBG_DF ) ) {
            DFBackRefType( back, name, tipe );
        } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
            CVBackRefType( back, name, tipe );
#if _TARGET_INTEL
        } else {
            WVBackRefType( back, name, tipe );
#endif
        }
    }
    retv = name->refno;
    CGFree( back, name );
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", retv );
#endif
    return( retv );
}


dbg_type _CGAPI DBCharBlock( BackRun& back, uint_32 len )
/****************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBCharBlock( %i )", len );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFCharBlock( back, len );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVCharBlock( back, len );
    } else {
#if _TARGET_INTEL
        ret = WVCharBlock( back, len );
#else
        ret = 0;
#endif
    }
    return( ret );
}

dbg_type _CGAPI DBCharBlockNamed( BackRun& back, cchar_ptr name, uint_32 len )
/*************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBCharBlock( %i )", len );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFCharBlockNamed( back, name, len );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVCharBlock( back, len );
    } else {
#if _TARGET_INTEL
        ret = WVCharBlock( back, len );
#else
        ret = 0;
#endif
    }
    return( ret );
}

dbg_type _CGAPI DBIndCharBlock( BackRun& back, back_handle len, cg_type len_type, int off )
/**********************************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBIndCharBlock( %i, %t, %i )", len,len_type, off );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFIndCharBlock( back, len, len_type, off );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVIndCharBlock( back, len, len_type, off );
    } else {
#if _TARGET_INTEL
        ret = WVIndCharBlock( back, len, len_type, off );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_type _CGAPI DBLocCharBlock( BackRun& back, dbg_loc loc, cg_type len_type )
/*********************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBLocCharBlock( %i, %t )", loc, len_type );
#endif

    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFLocCharBlock( back, loc, len_type );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVLocCharBlock( back, loc, len_type );
    } else {
#if _TARGET_INTEL
        ret = WVLocCharBlock( back, loc, len_type );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


dbg_type _CGAPI DBFtnArray( BackRun& back, back_handle dims, cg_type lo_bound_tipe,
                                    cg_type num_elems_tipe, int off,
                                    dbg_type base )
/***************************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBFtnArray( %B,%t,%t,%i,%i)", dims, lo_bound_tipe,
             num_elems_tipe, off, base );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFFtnArray( back, dims, lo_bound_tipe, num_elems_tipe, off, base );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVFtnArray( back, dims, lo_bound_tipe, num_elems_tipe, off, base );
    } else {
#if _TARGET_INTEL
        ret = WVFtnArray( back, dims, lo_bound_tipe, num_elems_tipe, off, base );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


dbg_type _CGAPI DBArray( BackRun& back, dbg_type idx, dbg_type base )
/************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBArray( %i, %i)", idx, base );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFArray( back, idx, base );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVArray( back, idx, base );
    } else {
#if _TARGET_INTEL
        ret = WVArray( back, idx, base );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_array _CGAPI DBBegArray( BackRun& back, dbg_type base, cg_type tipe, bool is_col_major )
/***************************************************************************/
{
    dbg_array       ar;
//    const type_def  *tipe_addr;

    /* unused parameters */ (void)tipe;

#ifdef DEVBUILD
    EchoAPI( "DBBegArray( %i,%t,%i)", base, tipe, is_col_major );
#endif
    ar = static_cast<dbg_array>(CGAlloc( back, sizeof( *ar ) ));
    ar->num = 0;
//  tipe_addr = TypeAddress( tipe );
//  ar->size = tipe_addr->length;
    ar->list = NULL;
    ar->base = base;
    ar->is_col_major = is_col_major;
    ar->is_variable = false;
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ar );
#endif
    return( ar );
}

static  void    AddDim( dbg_array ar, dim_any *dim )
/**************************************************/
{
    dim_any *curr;
    dim_any **owner;

    owner = &ar->list;
    for( ;; ) {
        curr = *owner;
        if( curr == NULL )
            break;
        owner = &curr->next;
    }
    dim->next = NULL;
    *owner = dim;
    ar->num++;
}

void _CGAPI DBDimCon( BackRun& back, dbg_array ar, dbg_type idx, int_32 lo, int_32 hi )
/**********************************************************************/
{
    dim_any *dim;

#ifdef DEVBUILD
    EchoAPI( "DBDimCon( %i, %i, %i, %i )\n", ar, idx, lo, hi );
#endif
    dim = static_cast<dim_any *>(CGAlloc( back, sizeof( *dim ) ));
    dim->kind = DIM_CON;
    dim->u.con.lo = lo;
    dim->u.con.hi = hi;
    dim->u.con.idx = idx;
    AddDim( ar, dim );
}

void _CGAPI DBDimVar( BackRun& back, dbg_array ar, back_handle dims, int off,
                        cg_type lo_bound_tipe,
                        cg_type num_elems_tipe )
/*************************************************/
{
    dim_any     *dim;

#ifdef DEVBUILD
    EchoAPI( "DBDimVar(%i, %B, %i, %t, %t)\n", ar, dims, off, lo_bound_tipe, num_elems_tipe );
#endif
    dim = static_cast<dim_any *>(CGAlloc( back, sizeof( *dim ) ));
    dim->kind = DIM_VAR;
    dim->u.var.dims = dims;
    dim->u.var.off = off;
    dim->u.var.lo_bound_tipe = lo_bound_tipe;
    dim->u.var.num_elems_tipe = num_elems_tipe;
    AddDim( ar, dim );
    ar->is_variable = true;
}

dbg_type _CGAPI DBEndArray( BackRun& back, dbg_array ar )
/************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBEndArray( %i )", ar );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret =  DFEndArray( back, ar );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret =  CVEndArray( back, ar );
    } else {
#if _TARGET_INTEL
        ret = WVEndArray( back, ar );
#else
        ret = 0;
#endif
    }
    CGFree( back, ar );
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_type _CGAPI DBIntArray( BackRun& back, uint_32 hi, dbg_type base )
/*****************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI(  "DBIntArray( %i, %i )", hi, base );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFIntArray( back, hi, base );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVIntArray( back, hi, base );
    } else {
#if _TARGET_INTEL
        ret = WVIntArray( back, hi, base );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_type _CGAPI DBIntArrayCG( BackRun& back, cg_type tipe, uint_32 hi, dbg_type base )
/*********************************************************************/
{
    dbg_type        ret;
    const type_def  *tipe_addr;

#ifdef DEVBUILD
    EchoAPI( "DBIntArrayCG( %t, %i, %i )", tipe, hi, base );
#endif
    tipe_addr = TypeAddress( back, tipe );
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFIntArray( back, hi, base );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVArraySize( back, tipe_addr->length, hi, base );
    } else {
#if _TARGET_INTEL
        ret = WVIntArray( back, hi, base );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_type _CGAPI DBSubRange( BackRun& back, int_32 lo, int_32 hi, dbg_type base )
/***************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBSubRange( %i, %i, %i )", lo, hi, base );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFSubRange( lo, hi, base );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVSubRange( back, lo, hi, base );
    } else {
#if _TARGET_INTEL
        ret = WVSubRange( back, lo, hi, base );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


dbg_type _CGAPI DBDereference( BackRun& back, cg_type ptr_type, dbg_type base )
/**********************************************************************/
{
    dbg_type ret;


#ifdef DEVBUILD
    EchoAPI( "DBDereference( %t, %i)", ptr_type, base );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFDereference( back, ptr_type, base );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVDereference( back, ptr_type, base );
    } else {
#if _TARGET_INTEL
        ret = WVDereference( back, ptr_type, base );
#else
        ret = 0;
#endif
    }

#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_type _CGAPI DBPtr( BackRun& back, cg_type ptr_type, dbg_type base )
/**************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBPtr( %t, %i )", ptr_type, base );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFPtr( back, ptr_type, base );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVPtr( back, ptr_type, base );
    } else {
#if _TARGET_INTEL
        ret = WVPtr( back, ptr_type, base );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_type _CGAPI DBBasedPtr( BackRun& back, cg_type ptr_type, dbg_type base,
                                        dbg_loc loc_segment )
/****************************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI("DBBasedPtr( %t, %i, %i )", ptr_type, base, loc_segment );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFBasedPtr( back, ptr_type, base, loc_segment );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVBasedPtr( back, ptr_type, base, loc_segment );
    } else {
#if _TARGET_INTEL
        ret = WVBasedPtr( back, ptr_type, base, loc_segment );
#else
        ret = 0;
#endif
    }
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


bool _CGAPI DBNested( BackRun& back, bool nested )
/*********************************/
{
    bool ret;

#ifdef DEVBUILD
    EchoAPI( "DBNested(%i)", nested );
#endif
    ret = back.dbtypes.Nested;
    back.dbtypes.Nested = nested;
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_struct _CGAPI DBBegNameStruct( BackRun& back, cchar_ptr nm, cg_type tipe, bool is_struct )
/*****************************************************************************/
{
    uint        n_len;
    dbg_struct  st;

#ifdef DEVBUILD
    EchoAPI( "DBBegNameStruct( %c, %t, %i )", nm, tipe, is_struct );
#endif
    n_len = Length( nm );
    st = static_cast<dbg_struct>(CGAlloc( back, sizeof( struct_list ) + n_len ));
    strcpy( st->name, nm );
    st->num = 0;
    st->list = NULL;
    st->list_tail = &st->list;
    st->size = TypeAddress( back, tipe )->length;
    st->is_struct = is_struct;   /* v.s. union */
    st->is_class = false;
    st->is_nested = back.dbtypes.Nested;
    st->is_cnested = false;
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        DFBegStruct( back, st );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
//       CVBegStruct( st );
        st->me = DBG_NIL_TYPE;
    } else {
        st->me = DBG_NIL_TYPE;
    }
    st->vtbl_off = 0;
    st->vtbl_type = DBG_NIL_TYPE;
    st->ptr_type = static_cast<cg_type>(0);
    st->vtbl_esize = 0;
    st->vf = NULL;
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", st );
#endif
    return( st );
}

dbg_struct _CGAPI DBBegStruct( BackRun& back, cg_type tipe, bool is_struct )
/***********************************************************/
{
    dbg_struct  st;

#ifdef DEVBUILD
    EchoAPI( "DBBegStruct( %t, %i )", tipe, is_struct );
#endif
    st = DBBegNameStruct( back, "", tipe, is_struct );
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", st );
#endif
    return( st );
}

static  field_member     *CreateMember( BackRun& back, const char *nm, byte strt,
                              byte len, dbg_type base, uint attr ) {
/******************************************************************/
    uint          n_len;
    field_member *field;

    n_len = Length( nm );
    field = static_cast<field_member *>(CGAlloc( back, sizeof( field_member ) + n_len ));
    strcpy( field->name, nm );
    field->attr = attr;
    field->len = n_len;
    field->base = base;
    field->b_strt = strt;
    field->b_len  = len;
    return( field );
}

static  void    AddField( dbg_struct st, field_any *field )
/*********************************************************/
{
    field->entry.next = NULL;
    *st->list_tail = field;
    st->list_tail = &field->entry.next;
    st->num++;
}


void _CGAPI DBAddBitField( BackRun& back, dbg_struct st, uint_32 off, byte strt,
                            byte len, cchar_ptr nm, dbg_type base )
/*****************************************************************/
{
    field_member *field;

#ifdef DEVBUILD
    EchoAPI( "DBAddBitField(%i,%i,%i,%i,%c,%i)\n", st, off, strt, len, nm, base );
#endif
    field = CreateMember( back, nm, strt, len, base, 0 );
    field->entry.field_type = FIELD_OFFSET;
    field->u.off= off;
    AddField( st, (field_any *)field );
}


void _CGAPI DBAddField( BackRun& back, dbg_struct st, uint_32 off,
                        cchar_ptr nm, dbg_type  base )
/****************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DBAddField( %i, %i,%c,%i)\n", st, off, nm, base );
#endif
    DBAddBitField( back, st, off, 0, 0, nm, base );
}


void _CGAPI DBAddLocField( BackRun& back, dbg_struct st, dbg_loc loc, uint attr,
                         byte strt, byte len, cchar_ptr nm, dbg_type base )
/*************************************************************************/
{
    field_member *field;
    offset      off;

#ifdef DEVBUILD
    EchoAPI( "DBAddLocField( %i,%i,%i,%i,%i,%c,%i)\n", st, loc, attr, strt, len, nm, base );
#endif
    field = CreateMember( back, nm, strt, len, base, attr );
    off = LocSimpField( loc );
    if( off != NO_OFFSET && (attr==FIELD_ATTR_NONE || attr==FIELD_ATTR_PUBLIC) ) {
        field->entry.field_type = FIELD_OFFSET;
        field->u.off= off;
    } else {
        field->entry.field_type = FIELD_LOC;
        field->u.loc = LocDupl( loc );
    }
    AddField( st, (field_any *)field );
}

void _CGAPI DBAddStField( BackRun& back, dbg_struct st, dbg_loc loc, cchar_ptr nm, uint_32 attr, dbg_type base )
/***********************************************************************************************/
{
    uint          n_len;
    field_stfield *field;

#ifdef DEVBUILD
    EchoAPI( "DBAddStField(%i,%i,%c,%i,%i)\n", st, loc, nm, attr, base );
#endif
    n_len = Length( nm );
    field = static_cast<field_stfield *>(CGAlloc( back, sizeof( field_stfield ) + n_len ));
    strcpy( field->name, nm );
    field->entry.field_type = FIELD_STFIELD;
    field->loc = LocDupl( loc );
    field->attr = attr;
    field->base = base;
    AddField( st, (field_any *)field );
}

void _CGAPI DBAddMethod( BackRun& back, dbg_struct st, dbg_loc loc, uint attr,
                                 uint kind, cchar_ptr nm, dbg_type base )
/***********************************************************************/
{
    uint          n_len;
    field_method *field;

#ifdef DEVBUILD
    EchoAPI( "DBAddMethod( %i,%i,%i,%i,%c,%i)\n", st, loc, attr, kind, nm, base );
#endif
    n_len = Length( nm );
    field = static_cast<field_method *>(CGAlloc( back, sizeof( field_method ) + n_len ));
    strcpy( field->name, nm );
    field->entry.field_type = FIELD_METHOD;
    field->u.loc = LocDupl( loc );
    field->attr = attr;
    field->kind = kind;
    field->len = n_len;
    field->base = base;
    AddField( st, (field_any *)field );
}

void _CGAPI DBAddNestedType( BackRun& back, dbg_struct st, cchar_ptr nm, dbg_type base )
/*******************************************************************************/
{
    uint          n_len;
    field_nested *field;

#ifdef DEVBUILD
    EchoAPI( "DBAddNestedType( %i,%c,%i)\n", st, nm, base );
#endif
    n_len = Length( nm );
    field = static_cast<field_nested *>(CGAlloc( back, sizeof( field_nested ) + n_len ));
    strcpy( field->name, nm );
    field->entry.field_type = FIELD_NESTED;
    field->base = base;
    AddField( st, (field_any *)field );
    st->is_cnested = true;
}


void _CGAPI DBAddInheritance( BackRun& back, dbg_struct st, dbg_type inherit,
                                  uint attr, uint kind,  dbg_loc loc )
/**********************************************************************/
{
    field_bclass *field;

#ifdef DEVBUILD
    EchoAPI( "DBAddInheritance(%i,%i,%i,%i,%i)\n", st, inherit, attr, kind, loc );
#endif
    field = static_cast<field_bclass *>(CGAlloc( back, sizeof( field_bclass ) ));
    field->entry.field_type = FIELD_INHERIT;
    field->attr = attr;
    field->base = inherit;
    field->attr = attr;
    field->kind = kind;
    field->u.adjustor = LocDupl( loc );
    AddField( st, (field_any *)field );
}

void _CGAPI DBAddBaseInfo( dbg_struct st, uint_32 vb_off, int esize,
                                    dbg_type vtbl, cg_type ptr_type )
/*******************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DBAddBaseInfo( %i,%i,%i,%i,%t)\n", st, vb_off, esize, vtbl, ptr_type );
#endif
    st->vtbl_off    = vb_off;
    st->vtbl_type  = vtbl;
    st->ptr_type  =  ptr_type;
    st->vtbl_esize = esize;
}


void _CGAPI DBAddVFuncInfo( BackRun& back, dbg_struct st, uint_32 vfptr_off,
                                int size, cg_type vft_cgtype )
/************************************************************/
{
    field_vfunc  *field;

#ifdef DEVBUILD
    EchoAPI( "DBAddVFuncInfo( %i,%i,%i,%t )\n", st, vfptr_off, size, vft_cgtype );
#endif
    field = static_cast<field_vfunc *>(CGAlloc( back, sizeof( field_vfunc ) ));
    field->entry.field_type = FIELD_VFUNC;
    field->vfptr_off = vfptr_off;
    field->vft_cgtype = vft_cgtype;
    field->vft_size = size;
    st->vf = field;
    AddField( st, (field_any *)field );
}

dbg_type _CGAPI DBEndStruct( BackRun& back, dbg_struct st )
/**************************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBEndStruct(%i)", st );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret =  DFEndStruct( back, st );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret =  CVEndStruct( back, st );
    } else {
#if _TARGET_INTEL
        ret = WVEndStruct( back, st );
#else
        ret = 0;
#endif
    }
    CGFree( back, st );
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}

dbg_type _CGAPI DBStructForward( dbg_struct st )
/******************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DBStructForward( %i )", st );
    EchoAPI( " -> %i\n", st->me );
#endif
    return( st->me );
}

dbg_enum _CGAPI DBBegEnum( BackRun& back, cg_type tipe )
/***********************************************/
{
    dbg_enum    en;

#ifdef DEVBUILD
    EchoAPI( "DBBegEnum( %t )", tipe );
#endif
    en = static_cast<dbg_enum>(CGAlloc( back, sizeof( enum_list ) ));
    en->num = 0;
    en->list = NULL;
    en->tipe = tipe;
    en->is_nested = back.dbtypes.Nested;
    en->is_c  = true;
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", en );
#endif
    return( en );
}

void _CGAPI DBAddConst( BackRun& back, dbg_enum en, cchar_ptr nm, int_32 val )
/*************************************************************/
{
    const_entry *cons;
    uint        len;

#ifdef DEVBUILD
    EchoAPI( "DBAddConst( %i,%c,%i)\n", en, nm, val );
#endif
    len = Length( nm );
    cons = static_cast<const_entry *>(CGAlloc( back, sizeof( const_entry ) + len ));
    strcpy( cons->name, nm );
    cons->len = len;
    Set64ValI32( cons->val, val );
    cons->next = en->list;
    en->list = cons;
    en->num++;
}

void _CGAPI DBAddConst64( BackRun& back, dbg_enum en, cchar_ptr nm, signed_64  val )
/*******************************************************************/
{
    const_entry *cons;
    uint        len;

#ifdef DEVBUILD
    EchoAPI( "DBAddConst( %i,%c,%x %x )\n", en, nm, val.u._32[0],val.u._32[1] );
#endif
    len = Length( nm );
    cons = static_cast<const_entry *>(CGAlloc( back, sizeof( const_entry ) + len ));
    strcpy( cons->name, nm );
    cons->len = len;
    cons->val = val;
    cons->next = en->list;
    en->list = cons;
    en->num++;
}

dbg_type _CGAPI DBEndEnum( BackRun& back, dbg_enum en )
/**********************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBEndEnum(%i)", en );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret = DFEndEnum( back, en );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret = CVEndEnum( back, en );
    } else {
#if _TARGET_INTEL
        ret = WVEndEnum( back, en );
#else
        ret = 0;
#endif
    }
    CGFree( back, en );
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}


dbg_proc _CGAPI DBBegProc( BackRun& back, cg_type call_type, dbg_type  ret )
/*******************************************************************/
{
    dbg_proc    pr;

#ifdef DEVBUILD
    EchoAPI( "DBBegProc( %t,%i)", call_type, ret );
#endif
    pr = static_cast<dbg_proc>(CGAlloc( back, sizeof( proc_list ) ));
    pr->num = 0;
    pr->list = NULL;
    pr->call = TypeAddress( back, call_type )->refno;
    pr->ret = ret;
    pr->cls = DBG_NIL_TYPE;
    pr->this_ = DBG_NIL_TYPE;
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", pr );
#endif
    return(pr);
}

void  _CGAPI DBAddMethParms( dbg_proc pr, dbg_type cls, dbg_type this_ )
/*****************************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DBAddMethParms( %i, %i, %i)\n", pr, cls, this_ );
#endif
    pr->cls = cls;
    pr->this_ = this_;
}

void _CGAPI DBAddParm( BackRun& back, dbg_proc pr, dbg_type tipe )
/*********************************************************/
{
    parm_entry  *parm;
    parm_entry  **owner;

#ifdef DEVBUILD
    EchoAPI( "DBAddParm( %i,%i )\n", pr, tipe );
#endif
    parm = static_cast<parm_entry *>(CGAlloc( back, sizeof( parm_entry ) ));
    pr->num++;
    for( owner = &pr->list; *owner != NULL; ) {
        owner = &(*owner)->next;
    }
    *owner = parm;
    parm->tipe = tipe;
    parm->next = NULL;
}


dbg_type _CGAPI DBEndProc( BackRun& back, dbg_proc pr )
/**********************************************/
{
    dbg_type ret;

#ifdef DEVBUILD
    EchoAPI( "DBEndProc( %i )", pr );
#endif
    if( _IsModel( CGSW_GEN_DBG_DF ) ) {
        ret =  DFEndProc( back, pr );
    } else if( _IsModel( CGSW_GEN_DBG_CV ) ) {
        ret =  CVEndProc( back, pr );
    } else {
#if _TARGET_INTEL
        ret = WVEndProc( back, pr );
#else
        ret = 0;
#endif
    }
    CGFree( back, pr );
#ifdef DEVBUILD
    EchoAPI( " -> %i\n", ret );
#endif
    return( ret );
}
