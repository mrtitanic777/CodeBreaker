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
#include "i64.h"
#include "utils.h"
#include "wvtypes.h"
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

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFFtnType( back, name, tipe );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVFtnType( name, tipe );
    } else {
        ret = WVFtnType( back, name, tipe );
    }
    return( ret );
}


dbg_type _CGAPI DBScalar( BackRun& back, cchar_ptr name, cg_type tipe )
/**************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFScalar( back, name, tipe );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVScalar( back, name, tipe );
    } else {
        ret = WVScalar( back, name, tipe );
    }
    return( ret );
}


dbg_type _CGAPI DBScope( BackRun& back, cchar_ptr name )
/***********************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFScope( name );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVScope( name );
    } else {
        ret = WVScope( back, name );
    }
    return( ret );
}


dbg_name _CGAPI DBBegName( BackRun& back, cchar_ptr nm, dbg_type scope )
/***************************************************************/
{
    dbg_name    name;
    uint        len;

    len = strlen( nm );
    name = static_cast<dbg_name>(CGAlloc( back, sizeof( name_entry ) + len ));
    strcpy( name->name, nm );
    name->len = len;
    name->scope = scope;
    name->refno = DBG_NIL_TYPE;
    return( name );
}


dbg_type _CGAPI DBForward( BackRun& back, dbg_name name )
/************************************************/
{
    if( name->refno == DBG_NIL_TYPE ) {
        if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
            /* do nothing */
        } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
            CVDumpName( back, name, DBG_FWD_TYPE );
        } else {
            WVDumpName( back, name, DBG_FWD_TYPE );
        }
    }
    return( name->refno );
}


dbg_type _CGAPI DBEndName( BackRun& back, dbg_name name, dbg_type tipe )
/***************************************************************/
{
    dbg_type    retv;

    if( name->refno == DBG_NIL_TYPE ) {
        if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
           DFDumpName( back, name, tipe );
        } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
           CVDumpName( back, name, tipe );
        } else {
           WVDumpName( back, name, tipe );
        }
    } else if( IsModel( back, CGSW_GEN_DBG_TYPES ) ) {
        if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
            DFBackRefType( back, name, tipe );
        } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
            CVBackRefType( back, name, tipe );
        } else {
            WVBackRefType( back, name, tipe );
        }
    }
    retv = name->refno;
    CGFree( back, name );
    return( retv );
}


dbg_type _CGAPI DBCharBlock( BackRun& back, uint_32 len )
/****************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFCharBlock( back, len );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVCharBlock( back, len );
    } else {
        ret = WVCharBlock( back, len );
    }
    return( ret );
}

dbg_type _CGAPI DBCharBlockNamed( BackRun& back, cchar_ptr name, uint_32 len )
/*************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFCharBlockNamed( back, name, len );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVCharBlock( back, len );
    } else {
        ret = WVCharBlock( back, len );
    }
    return( ret );
}

dbg_type _CGAPI DBIndCharBlock( BackRun& back, back_handle len, cg_type len_type, int off )
/**********************************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFIndCharBlock( back, len, len_type, off );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVIndCharBlock( back, len, len_type, off );
    } else {
        ret = WVIndCharBlock( back, len, len_type, off );
    }
    return( ret );
}

dbg_type _CGAPI DBLocCharBlock( BackRun& back, dbg_loc loc, cg_type len_type )
/*********************************************************************/
{
    dbg_type ret;


    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFLocCharBlock( back, loc, len_type );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVLocCharBlock( back, loc, len_type );
    } else {
        ret = WVLocCharBlock( back, loc, len_type );
    }
    return( ret );
}


dbg_type _CGAPI DBFtnArray( BackRun& back, back_handle dims, cg_type lo_bound_tipe,
                                    cg_type num_elems_tipe, int off,
                                    dbg_type base )
/***************************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFFtnArray( back, dims, lo_bound_tipe, num_elems_tipe, off, base );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVFtnArray( back, dims, lo_bound_tipe, num_elems_tipe, off, base );
    } else {
        ret = WVFtnArray( back, dims, lo_bound_tipe, num_elems_tipe, off, base );
    }
    return( ret );
}


dbg_type _CGAPI DBArray( BackRun& back, dbg_type idx, dbg_type base )
/************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFArray( back, idx, base );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVArray( back, idx, base );
    } else {
        ret = WVArray( back, idx, base );
    }
    return( ret );
}

dbg_array _CGAPI DBBegArray( BackRun& back, dbg_type base, cg_type tipe, bool is_col_major )
/***************************************************************************/
{
    dbg_array       ar;
//    const type_def  *tipe_addr;

    /* unused parameters */ (void)tipe;

    ar = static_cast<dbg_array>(CGAlloc( back, sizeof( *ar ) ));
    ar->num = 0;
//  tipe_addr = TypeAddress( tipe );
//  ar->size = tipe_addr->length;
    ar->list = NULL;
    ar->base = base;
    ar->is_col_major = is_col_major;
    ar->is_variable = false;
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

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret =  DFEndArray( back, ar );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret =  CVEndArray( back, ar );
    } else {
        ret = WVEndArray( back, ar );
    }
    CGFree( back, ar );
    return( ret );
}

dbg_type _CGAPI DBIntArray( BackRun& back, uint_32 hi, dbg_type base )
/*****************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFIntArray( back, hi, base );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVIntArray( back, hi, base );
    } else {
        ret = WVIntArray( back, hi, base );
    }
    return( ret );
}

dbg_type _CGAPI DBIntArrayCG( BackRun& back, cg_type tipe, uint_32 hi, dbg_type base )
/*********************************************************************/
{
    dbg_type        ret;
    const type_def  *tipe_addr;

    tipe_addr = TypeAddress( back, tipe );
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFIntArray( back, hi, base );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVArraySize( back, tipe_addr->length, hi, base );
    } else {
        ret = WVIntArray( back, hi, base );
    }
    return( ret );
}

dbg_type _CGAPI DBSubRange( BackRun& back, int_32 lo, int_32 hi, dbg_type base )
/***************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFSubRange( lo, hi, base );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVSubRange( back, lo, hi, base );
    } else {
        ret = WVSubRange( back, lo, hi, base );
    }
    return( ret );
}


dbg_type _CGAPI DBDereference( BackRun& back, cg_type ptr_type, dbg_type base )
/**********************************************************************/
{
    dbg_type ret;


    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFDereference( back, ptr_type, base );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVDereference( back, ptr_type, base );
    } else {
        ret = WVDereference( back, ptr_type, base );
    }

    return( ret );
}

dbg_type _CGAPI DBPtr( BackRun& back, cg_type ptr_type, dbg_type base )
/**************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFPtr( back, ptr_type, base );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVPtr( back, ptr_type, base );
    } else {
        ret = WVPtr( back, ptr_type, base );
    }
    return( ret );
}

dbg_type _CGAPI DBBasedPtr( BackRun& back, cg_type ptr_type, dbg_type base,
                                        dbg_loc loc_segment )
/****************************************************************/
{
    dbg_type ret;

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFBasedPtr( back, ptr_type, base, loc_segment );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVBasedPtr( back, ptr_type, base, loc_segment );
    } else {
        ret = WVBasedPtr( back, ptr_type, base, loc_segment );
    }
    return( ret );
}


bool _CGAPI DBNested( BackRun& back, bool nested )
/*********************************/
{
    bool ret;

    ret = back.dbtypes.Nested;
    back.dbtypes.Nested = nested;
    return( ret );
}

dbg_struct _CGAPI DBBegNameStruct( BackRun& back, cchar_ptr nm, cg_type tipe, bool is_struct )
/*****************************************************************************/
{
    uint        n_len;
    dbg_struct  st;

    n_len = strlen( nm );
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
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFBegStruct( back, st );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
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
    return( st );
}

dbg_struct _CGAPI DBBegStruct( BackRun& back, cg_type tipe, bool is_struct )
/***********************************************************/
{
    dbg_struct  st;

    st = DBBegNameStruct( back, "", tipe, is_struct );
    return( st );
}

static  field_member     *CreateMember( BackRun& back, const char *nm, byte strt,
                              byte len, dbg_type base, uint attr ) {
/******************************************************************/
    uint          n_len;
    field_member *field;

    n_len = strlen( nm );
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

    field = CreateMember( back, nm, strt, len, base, 0 );
    field->entry.field_type = FIELD_OFFSET;
    field->u.off= off;
    AddField( st, (field_any *)field );
}


void _CGAPI DBAddField( BackRun& back, dbg_struct st, uint_32 off,
                        cchar_ptr nm, dbg_type  base )
/****************************************************/
{
    DBAddBitField( back, st, off, 0, 0, nm, base );
}


void _CGAPI DBAddLocField( BackRun& back, dbg_struct st, dbg_loc loc, uint attr,
                         byte strt, byte len, cchar_ptr nm, dbg_type base )
/*************************************************************************/
{
    field_member *field;
    offset      off;

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

    n_len = strlen( nm );
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

    n_len = strlen( nm );
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

    n_len = strlen( nm );
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

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret =  DFEndStruct( back, st );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret =  CVEndStruct( back, st );
    } else {
        ret = WVEndStruct( back, st );
    }
    CGFree( back, st );
    return( ret );
}

dbg_type _CGAPI DBStructForward( dbg_struct st )
/******************************************************/
{
    return( st->me );
}

dbg_enum _CGAPI DBBegEnum( BackRun& back, cg_type tipe )
/***********************************************/
{
    dbg_enum    en;

    en = static_cast<dbg_enum>(CGAlloc( back, sizeof( enum_list ) ));
    en->num = 0;
    en->list = NULL;
    en->tipe = tipe;
    en->is_nested = back.dbtypes.Nested;
    en->is_c  = true;
    return( en );
}

void _CGAPI DBAddConst( BackRun& back, dbg_enum en, cchar_ptr nm, int_32 val )
/*************************************************************/
{
    const_entry *cons;
    uint        len;

    len = strlen( nm );
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

    len = strlen( nm );
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

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret = DFEndEnum( back, en );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret = CVEndEnum( back, en );
    } else {
        ret = WVEndEnum( back, en );
    }
    CGFree( back, en );
    return( ret );
}


dbg_proc _CGAPI DBBegProc( BackRun& back, cg_type call_type, dbg_type  ret )
/*******************************************************************/
{
    dbg_proc    pr;

    pr = static_cast<dbg_proc>(CGAlloc( back, sizeof( proc_list ) ));
    pr->num = 0;
    pr->list = NULL;
    pr->call = TypeAddress( back, call_type )->refno;
    pr->ret = ret;
    pr->cls = DBG_NIL_TYPE;
    pr->this_ = DBG_NIL_TYPE;
    return(pr);
}

void  _CGAPI DBAddMethParms( dbg_proc pr, dbg_type cls, dbg_type this_ )
/*****************************************************************************/
{
    pr->cls = cls;
    pr->this_ = this_;
}

void _CGAPI DBAddParm( BackRun& back, dbg_proc pr, dbg_type tipe )
/*********************************************************/
{
    parm_entry  *parm;
    parm_entry  **owner;

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

    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        ret =  DFEndProc( back, pr );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        ret =  CVEndProc( back, pr );
    } else {
        ret = WVEndProc( back, pr );
    }
    CGFree( back, pr );
    return( ret );
}
