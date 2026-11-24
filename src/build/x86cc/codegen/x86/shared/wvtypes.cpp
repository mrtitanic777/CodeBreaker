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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "wvdbg.h"
#include "data.h"
#include "types.h"
#include "objout.h"
#include "wvtypes.h"
#include "dbsyms.h"
#include "x86dbsup.h"
#include "dbsupp.h"
#include "wvsupp.h"
#include "i64.h"
#include "backrun.hpp"


#define MAX_TYPE_SIZE   (1024 * 16)

static  void            NewType( BackRun& back, temp_buff *temp, uint ty_def );
static  void            EndType( BackRun& back, bool check_too_big );

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct WvtypesState: wvtypes_state.h) */

}  // namespace



static  byte    GetScalar( BackRun& back, cg_type tipe ) {
/*****************************************/

    byte            scalar;
    const type_def  *tipe_addr;

    tipe_addr = TypeAddress( back, tipe );
    if( tipe_addr->refno == TY_DEFAULT ) {
        return( SCALAR_VOID );
    }
    scalar = tipe_addr->length - 1;
    if( tipe_addr->attr & TYPE_FLOAT ) {
        scalar |= SCALAR_FLOAT;
    } else if( tipe_addr->attr & TYPE_SIGNED ) {
        scalar |= SCALAR_INT;
    } else {
        scalar |= SCALAR_UNSIGNED;
    }
    return( scalar );
}


static  uint    SignedSizeClass( int_32 num ) {
/*******************************************/

    uint        class_;

    if( num >= -128 && num <= 127 ) {
        class_ = 0;
    } else if( num >= -32768 && num <= 32767 ) {
        class_ = 1;
    } else {
        class_ = 2;
    }
    return( class_ );
}

static  uint    SignedSizeClass64( signed_64 val )
/************************************************/
{
    uint        class_;

    if( I64High( val ) == 0 || I64High( val ) == -1 ) {
        class_ = SignedSizeClass( I64Low( val ) );
    } else {
        class_ = 3;
    }
    return( class_ );
}

void    WVSrcCueLoc( BackRun& back  )
/**************************/
// Leave a dword to be back patched with the offset of line info
{
    temp_buff   temp;

    BuffStart( back, &temp, WT_NAME + NAME_CUEINFO );
    BuffForward( back, &back.wvtypes.CueInfoOffset ); /* does a  0 word */
    BuffWord( back, 0 ); /* another 0 word */
    EndType( back, false );
}

void    WVTypesEof( BackRun& back )
/************************/
// Leave Eof indicator for types
{
    temp_buff   temp;

    BuffStart( back, &temp, WT_NAME + NAME_EOF );
    EndType( back, false );
}

dbg_type        WVFtnType( BackRun& back, const char *name, dbg_ftn_type tipe )
/**************************************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_NAME + NAME_SCALAR );
    BuffByte( back, tipe );
    BuffWSLString( back, name );
    EndType( back, true );
    return( back.data.TypeIdx );
}


dbg_type        WVScalar( BackRun& back, const char *name, cg_type tipe )
/********************************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_NAME + NAME_SCALAR );
    BuffByte( back, GetScalar( back, tipe ) );
    BuffWSLString( back, name );
    EndType( back, true );
    return( back.data.TypeIdx );
}


dbg_type        WVScope( BackRun& back, const char *name )
/*****************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_NAME + NAME_SCOPE );
    BuffWSLString( back, name );
    EndType( back, true );
    return( back.data.TypeIdx );
}


void    WVDumpName( BackRun& back, dbg_name name, dbg_type tipe )
/************************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_NAME + NAME_NAME );
    name->refno = back.data.TypeIdx;
    if( IsModel( back, CGSW_GEN_DBG_TYPES ) ) {
        BuffIndex( back, name->scope );
        if( tipe == DBG_FWD_TYPE ) {
            BuffForward( back, &name->dpatch );
        } else {
            BuffIndex( back, tipe );
        }
        BuffString( back, name->len, name->name );
    }
    EndType( back, true );
}

void WVBackRefType( BackRun& back, dbg_name name, dbg_type tipe )
/************************************************/
{
    offset      here;

    { segment_id old_segid = ChangeOP( back, name->dpatch.segid );
        here = AskLocation(back);
        SetLocation( back, name->dpatch.offset );
        DataShort( back, 0x80 | (tipe >> 8) | (tipe << 8) );
        SetLocation( back, here );
    SetOP( back, old_segid ); }
}

dbg_type        WVCharBlock( BackRun& back, uint_32 len )
/****************************************/
{
    temp_buff   temp;
    int         class_;

    class_ = SignedSizeClass( len );
    NewType( back, &temp, WT_CHAR_BLOCK + NAME_CHAR_BYTE + class_ );
    BuffValue( back, len, class_ );
    EndType( back, true );
    return( back.data.TypeIdx );
}

dbg_type    WVIndCharBlock( BackRun& back, back_handle len, cg_type len_type, int off )
/**********************************************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_CHAR_BLOCK + NAME_CHAR_IND );
    BuffByte( back, GetScalar( back, len_type ) );
    BuffBack( back, len, off );
    EndType( back, true );
    return( back.data.TypeIdx );
}

dbg_type        WVLocCharBlock( BackRun& back, dbg_loc loc, cg_type len_type )
/*************************************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_CHAR_BLOCK + NAME_CHAR_LOC );
    BuffByte( back, GetScalar( back, len_type ) );
    LocDump( back, LocDupl( loc ) );
    EndType( back, true );
    return( back.data.TypeIdx );
}


dbg_type    WVFtnArray( BackRun& back, back_handle dims, cg_type lo_bound_tipe, cg_type num_elems_tipe, int off, dbg_type base )
/***************************************************************************************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_ARRAY + FORTRAN_TYPE );
    BuffByte( back, GetScalar( back, lo_bound_tipe ) );
    BuffByte( back, GetScalar( back, num_elems_tipe ) );
    BuffBack( back, dims, off );
    BuffIndex( back, base );
    EndType( back, true );
    return( back.data.TypeIdx );
}


dbg_type        WVArray( BackRun& back, dbg_type idx, dbg_type base )
/****************************************************/
{
    temp_buff   temp;

    NewType( back, &temp, WT_ARRAY + ARRAY_TYPE );
    BuffIndex( back, idx );
    BuffIndex( back, base );
    EndType( back, true );
    return( back.data.TypeIdx );
}


dbg_type        WVIntArray( BackRun& back, uint_32 hi, dbg_type base )
/*****************************************************/
{
    uint        class_;
    temp_buff   temp;

    class_ = SignedSizeClass( hi );
    NewType( back, &temp, WT_ARRAY + ARRAY_BYTE + class_ );
    BuffValue( back, hi, class_ );
    BuffIndex( back, base );
    EndType( back, true );
    return( back.data.TypeIdx );
}

dbg_type    WVSubRange( BackRun& back, int_32 lo, int_32 hi, dbg_type base )
/***********************************************************/
{
    uint        class_lo;
    uint        class_hi;
    temp_buff   temp;

    class_lo = SignedSizeClass( lo );
    class_hi = SignedSizeClass( hi );
    if( class_lo > class_hi ) {
       class_hi = class_lo;
    }
    NewType( back, &temp, WT_SUBRANGE + RANGE_BYTE + class_hi );
    BuffValue( back, lo, class_hi );
    BuffValue( back, hi, class_hi );
    BuffIndex( back, base );
    EndType( back, true );
    return( back.data.TypeIdx );
}


dbg_type    WVEndArray( BackRun& back, dbg_array ar )
/************************************/
{
    dim_any   *dim;
    dbg_type  ret = 0;
    dbg_type  sub;

//    ReverseDims( ar );
    while( (dim = ar->list) != NULL ) {
        ar->list = dim->next;
        switch( dim->kind ) {
        case DIM_CON:
            sub = WVSubRange( back, dim->u.con.lo, dim->u.con.hi, dim->u.con.idx );
            ret = WVArray( back, sub, ar->base );
            ar->base = ret;
            break;
        case DIM_VAR:
            ret = WVFtnArray( back, dim->u.var.dims, dim->u.var.lo_bound_tipe,
                     dim->u.var.num_elems_tipe, dim->u.var.off, ar->base );
            ar->base = ret;
            break;

        }
        CGFree( back, dim  );
    }
    return( ret );
}

static  dbg_type        DbgPtr( BackRun& back, cg_type ptr_type, dbg_type base, int adjust,
                                dbg_loc loc_segment ) {
/***************************************************************************/

    temp_buff   temp;

    switch( TypeAddress( back, ptr_type )->refno ) {
    case TY_NEAR_POINTER:
    case TY_NEAR_CODE_PTR:
        NewType( back, &temp, WT_POINTER + POINTER_NEAR + adjust );
        break;
    default:
        NewType( back, &temp, WT_POINTER + POINTER_FAR + adjust );
    }
    BuffIndex( back, base );
    if( loc_segment != NULL ) {
        LocDump( back, LocDupl( loc_segment ) );
    }
    EndType( back, true );
    return( back.data.TypeIdx );
}


dbg_type        WVDereference( BackRun& back, cg_type ptr_type, dbg_type base )
/**************************************************************/
{
    return( DbgPtr( back, ptr_type, base, DEREF_NEAR - POINTER_NEAR, NULL ) );
}

dbg_type        WVPtr( BackRun& back, cg_type ptr_type, dbg_type base )
/******************************************************/
{
    return( DbgPtr( back, ptr_type, base, 0, NULL ) );
}

dbg_type    WVBasedPtr( BackRun& back, cg_type ptr_type, dbg_type base, dbg_loc loc_segment )
/****************************************************************************/
{
    return( DbgPtr( back, ptr_type, base, 0, loc_segment ) );
}

static  void    AddField( field_any **owner, field_any *field )
/*** Sort according to WV(brian)******************************/
{
    field_any     *curr;
    unsigned      strt;
    offset        off;

    strt = field->member.b_strt;
    off  = field->member.u.off;
    for( ;; ) {
        curr = *owner;
        if( curr == NULL )
            break;
        if( curr->member.entry.field_type == FIELD_OFFSET ) {
            if( (off == curr->member.u.off) && (strt >= curr->member.b_strt) )
                break;
            if( off >= curr->member.u.off ) {
                break;
            }
        }
        owner = &curr->member.entry.next;
    }
    field->entry.next = curr;
    *owner = field;
}

static  void    SortFields( dbg_struct st )
/*****************************************/
{
    field_any   *curr;
    field_any   *next;
    field_any   *head;

    curr = st->list;
    head = NULL;
    while( curr != NULL ) {
        next = curr->entry.next;
        curr->entry.next = NULL;
        if( curr->entry.field_type == FIELD_OFFSET ) {
            AddField(  &head, curr );
        } else {
            curr->entry.next = head;
            head = curr;
        }
        curr = next;
    }
    st->list = head;
}


dbg_type        WVEndStruct( BackRun& back, dbg_struct st )
/******************************************/
{
    field_any   *field;
    uint        class_;
    temp_buff   temp;

    NewType( back, &temp, WT_STRUCTURE + STRUCT_LIST );
    BuffWord( back, st->num );
    BuffDWord( back, st->size );
    EndType( back, true );
    SortFields( st );
    for( ;; ) {
        field = st->list;
        if( field == NULL )
            break;
        switch( field->entry.field_type ) {
        case FIELD_INHERIT:
            BuffStart( back, &temp, WT_STRUCTURE + STRUCT_INHERIT );
            LocDump( back, field->bclass.u.adjustor );
            BuffIndex( back, field->bclass.base );
            break;
        case FIELD_LOC:
            if( field->member.b_len == 0 ) {
                BuffStart( back, &temp, WT_STRUCTURE + STRUCT_F_LOC );
                BuffByte( back, field->member.attr );
                LocDump( back, field->member.u.loc );
            } else {
                BuffStart( back, &temp, WT_STRUCTURE + STRUCT_BF_LOC );
                BuffByte( back, field->member.attr );
                LocDump( back, field->member.u.loc );
                BuffByte( back, field->member.b_strt );
                BuffByte( back, field->member.b_len );
            }
            BuffIndex( back, field->member.base );
            BuffString( back, field->member.len, field->member.name );
            break;
        case FIELD_OFFSET:
            if( field->member.u.off <= 0x00ff ) {
                class_ = 0;
            } else if( field->member.u.off <= 0xffff ) {
                class_ = 1;
            } else {
                class_ = 2;
            }
            if( field->member.b_len == 0 ) {
                BuffStart( back, &temp, WT_STRUCTURE + STRUCT_F_BYTE + class_ );
                BuffValue( back, field->member.u.off, class_ );
            } else {
                BuffStart( back, &temp, WT_STRUCTURE + STRUCT_BF_BYTE + class_ );
                BuffValue( back, field->member.u.off, class_ );
                BuffByte( back, field->member.b_strt );
                BuffByte( back, field->member.b_len );
            }
            BuffIndex( back, field->member.base );
            BuffString( back, field->member.len, field->member.name );
            break;
        case FIELD_METHOD:
            break;
        case FIELD_NESTED:
            break;
        case FIELD_VFUNC:
            break;
        default:
            break;
        }
        EndType( back, false );
        st->list = field->entry.next;
        CGFree( back, field  );
    }
    return( back.data.TypeIdx );
}


dbg_type        WVEndEnum( BackRun& back, dbg_enum en )
/**************************************/
{
    const_entry *cons;
    uint        class_;
    temp_buff   temp;
    signed_64   val;

    NewType( back, &temp, WT_ENUMERATED + ENUM_LIST );
    BuffWord( back, en->num );
    BuffByte( back, GetScalar( back, en->tipe ) );
    EndType( back, true );
    for( ;; ) {
        cons = en->list;
        if( cons == NULL )
            break;
        val = cons->val;
        class_ = SignedSizeClass64( val );
        BuffStart( back, &temp, WT_ENUMERATED + ENUM_BYTE + class_ );
        if( class_ == 3 ) {
            BuffValue( back, U64Low( val ), 2 );
            BuffValue( back, U64High( val ), 2 );
        } else {
            BuffValue( back, U64Low( val ), class_ );
        }
        BuffString( back, cons->len, cons->name );
        EndType( back, false );
        en->list = cons->next;
        CGFree( back, cons );
    }
    return( back.data.TypeIdx );
}


dbg_type        WVEndProc( BackRun& back, dbg_proc pr )
/**************************************/
{
    parm_entry  *parm;
    temp_buff   temp;
    dbg_type    proc_type;

    if( pr->call == TY_NEAR_CODE_PTR ) {
        NewType( back, &temp, WT_PROCEDURE + PROC_NEAR );
    } else {
        NewType( back, &temp, WT_PROCEDURE + PROC_FAR );
    }
    proc_type = back.data.TypeIdx;
    BuffIndex( back, pr->ret );
    BuffByte( back, pr->num );
    for( ;; ) {
        parm = pr->list;
        if( parm == NULL )
            break;
        if( BuffLoc(back) > DB_BUFF_SIZE - 4 ) {
            /* record is getting too big - split it up */
            EndType( back, true );
            NewType( back, &temp, WT_PROCEDURE + PROC_EXT_PARM_LIST );
        }
        BuffIndex( back, parm->tipe );
        pr->list = parm->next;
        CGFree( back, parm );
    }
    EndType( back, false );
    return( proc_type );
}


static  void    NewType( BackRun& back, temp_buff *temp, uint ty_def ) {
/*******************************************************/

    ++back.data.TypeIdx;
    BuffStart( back, temp, ty_def );
}


static  void    EndType( BackRun& back, bool check_too_big ) {
/*********************************************/


    if( IsModel( back, CGSW_GEN_DBG_TYPES ) ) {
        if( check_too_big )
            ChkDbgSegSize( back, MAX_TYPE_SIZE, true );
        BuffEnd( back, back.data.DbgTypes );
    }
}

static void DmpFileInfo( BackRun& back )
/*****************************/
{
    fname_lst   *lst;
    uint_16     index;

    DataShort( back, back.dbsyms.DBFiles.count );
    index = 0;
    for( lst = back.dbsyms.DBFiles.lst; lst != NULL; lst = lst->next ) {
        DataShort( back, index );
        index += lst->len;
    }
    for( lst = back.dbsyms.DBFiles.lst; lst != NULL; lst = lst->next ) {
        DataBytes( back, lst->len, lst->fname );
    }
}

void WVDmpCueInfo( BackRun& back, long_offset where )
/************************************/
// Assume here is offset from first dbgtype segment to here
// and we are in our segement for writing
{
    cue_ctl     *ctl;
    cue_blk     *blk;
    cue_state   *curr;
    cue_state   *end;
    offset      here;


    { segment_id old_segid = ChangeOP( back, back.wvtypes.CueInfoOffset.segid );
        here = AskLocation(back);
        SetLocation( back, back.wvtypes.CueInfoOffset.offset );
        DataLong( back, where );  // current location in DbgTypes segment
        SetLocation( back, here );
    SetOP( back, old_segid ); }
    ctl = &back.dbsyms.LineInfo;
    blk = ctl->head;
    DataShort( back, ctl->count );  // number of entries
    if( ctl->count > 0 ) {
        curr = &ctl->start[0];
        DataShort( back, curr->cue );
        DataShort( back, curr->fno );
        DataShort( back, curr->line );
        DataShort( back, curr->col );
    }
    while( blk != NULL ) {
        curr = &blk->info[0];
        if( blk->next == NULL ) {
            end = ctl->next;
        } else {
            end = &blk->info[CUES_PER_BLK];
        }
        while( curr != end ) {
            DataShort( back, curr->cue );
            DataShort( back, curr->fno );
            DataShort( back, curr->line );
            DataShort( back, curr->col );
            ++curr;
        }
        blk = blk->next;
    }
    DmpFileInfo(back);
}
