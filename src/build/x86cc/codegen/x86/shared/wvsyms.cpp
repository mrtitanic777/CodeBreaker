/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "wvdbg.h"
#include "data.h"
#include "types.h"
#include "objout.h"
#include "wvsyms.h"
#include "objio.h"
#include "x86dbsup.h"
#include "dbsupp.h"
#include "wvtypes.h"
#include "x86objd.h"
#include "x86obj.h"
#include "wvsupp.h"
#include "feprotos.h"
#include "cgprotos.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct WvsymsState: wvsyms_state.h) */

}  // namespace



static  void    DumpDbgBlkStart( BackRun& back, dbg_block *blk, offset lc ) {
/************************************************************/

    block_patch *bpatch;
    offset      off;
    segment_id  old_segid;

    old_segid = AskOP(back);
    while( blk->bpatches != NULL ) {
        bpatch = blk->bpatches;
        blk->bpatches = bpatch->link;
        SetOP( back, bpatch->dpatch.segid );
        off = AskLocation(back);
        SetLocation( back, bpatch->dpatch.offset );
        DataShort( back, off );
        SetLocation( back, off );
        CGFree( back, bpatch );
    }
    SetOP( back, old_segid );
    BuffOffset( back, blk->start - back.wvsyms.CodeOffset );
    BuffOffset( back, lc - blk->start );
}


static  void    DumpParentPtr( BackRun& back, dbg_block *blk ) {
/***********************************************/

    block_patch *bpatch;

    blk = blk->parent;
    if( blk == NULL ) {
        BuffWord( back, 0 );
   } else {
        bpatch = static_cast<block_patch *>(CGAlloc( back, sizeof( block_patch ) ));
        bpatch->link = blk->bpatches;
        blk->bpatches = bpatch;
        BuffForward( back, &bpatch->dpatch );
    }
}

static  void    DumpDbgBlk( BackRun& back, dbg_block *blk, offset lc ) {
/*******************************************************/

    DumpDbgBlkStart( back, blk, lc );
    DumpParentPtr( back, blk );
}

static  void    DumpLocals( BackRun& back, dbg_local *local ) {
/*************************************************/


    dbg_local   *junk;
    temp_buff   temp;
    dbg_type    tipe;

    while( local != NULL ) {
        tipe = FEDbgType( *back.front, static_cast<cg_sym_handle>(local->sym) );
        BuffStart( back, &temp, SYM_VARIABLE + VAR_LOCAL );
        LocDump( back, local->loc );
        BuffIndex( back, tipe );
        BuffWSLString( back, FEName( *back.front, static_cast<cg_sym_handle>(local->sym) ) );
        BuffEnd( back, back.data.DbgLocals );
        junk = local;
        local = local->link;
        CGFree( back, junk );
    }
}

void    WVInitDbgInfo( BackRun& back )
/***************************/
{
    back.data.TypeIdx   = 0;
    back.data.DbgLocals = 0;
    back.data.DbgTypes  = 0;
}

void    WVObjInitDbgInfo( BackRun& back )
/******************************/
// Called right after define seg's in Obj
{
    WVSrcCueLoc(back);
}

void    WVFiniDbgInfo( void )
/***************************/
{
}

void    WVObjFiniDbgInfo( void )
/******************************/
{
}

void    WVGenStatic( BackRun& back, cg_sym_handle sym, dbg_loc loc )
/***************************************************/
{
    temp_buff   temp;
    dbg_type    tipe;

    tipe = FEDbgType( *back.front, sym );
    if( LocSimpStatic( loc ) == sym ) {
        BuffStart( back, &temp, SYM_VARIABLE + VAR_MODULE );
        BuffAddr( back, sym );
    } else {
        BuffStart( back, &temp, SYM_VARIABLE + VAR_MODULE_LOC );
        LocDump( back, LocDupl( loc ) );
    }
    BuffIndex( back, tipe );
    BuffWSLString( back, FEName( *back.front, sym ) );
    BuffEnd( back, back.data.DbgLocals );
}

void    WVSetBase( BackRun& back )
/***********************/
{
    temp_buff   temp;
    back_handle bck;

    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) && NeedBaseSet(back) ) {
        bck = BENewBack( back, NULL );
        bck->segid = AskOP(back);
        OutLabel( back, bck->lbl );
        back.wvsyms.CodeOffset = AskAddress( back, bck->lbl );
        BuffStart( back, &temp, NEW_BASE+SET_BASE );
        BuffBack( back, bck, 0 );
        BuffEnd( back, back.data.DbgLocals );
        BEFreeBack( back, bck );
        SetUpObj( back, false );
    }
}

void    WVObjectPtr(  BackRun& back, cg_type ptr_type )
/**************************************/
{
    switch( TypeAddress( back, ptr_type )->refno ) {
    case TY_NEAR_POINTER:
    case TY_NEAR_CODE_PTR:
        back.data.CurrProc->targ.debug->obj_ptr_type = POINTER_NEAR;
        break;
    default:
        back.data.CurrProc->targ.debug->obj_ptr_type = POINTER_FAR;
        break;
    }
}

/**/
/* Going into optimizer queue*/
/**/


/**/
/* Coming out of optimizer queue*/
/**/


void    WVBlkEnd( BackRun& back, dbg_block *blk, offset lc )
/*******************************************/
{
    temp_buff   temp;

    BuffStart( back, &temp, SYM_CODE + CODE_BLOCK );
    DumpDbgBlk( back, blk, lc );
    BuffEnd( back, back.data.DbgLocals );
    DumpLocals( back, blk->locals );
}


void    WVRtnEnd( BackRun& back, dbg_rtn *rtn, offset lc )
/*****************************************/
{
    uint                count;
    dbg_local           *parm;
    dbg_local           *junk;
    temp_buff           temp;
    cg_sym_handle       sym;
    dbg_type            tipe;
    offset              off;

    off = 0;
    if( rtn->obj_type != DBG_NIL_TYPE ) {
        /* is a member function */
        { segment_id old_segid = ChangeOP( back, back.data.DbgLocals );
            off = AskLocation(back);
            BuffStart( back, &temp, SYM_CODE + CODE_MEMBER_SCOPE );
            DumpParentPtr( back, rtn->blk );
            BuffIndex( back, rtn->obj_type );
            if( rtn->obj_loc != NULL ) {
                BuffByte( back, rtn->obj_ptr_type ); /* 'this' pointer type */
                LocDump( back, rtn->obj_loc );
            }
            BuffEnd( back, back.data.DbgLocals );
        SetOP( back, old_segid ); }
    }
    sym = AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) );
    tipe = FEDbgType( *back.front, sym );
    if( static_cast<call_class_target>(reinterpret_cast<pointer_uint>(FindAuxInfoSym( back, sym, FEINF_CALL_CLASS_TARGET ))) & FECALL_X86_FAR_CALL ) {
        BuffStart( back, &temp, SYM_CODE + CODE_FAR_RTN );
    } else {
        BuffStart( back, &temp, SYM_CODE + CODE_NEAR_RTN );
    }
    DumpDbgBlkStart( back, rtn->blk, lc );
    if( off != 0 ) {
        BuffWord( back, off );
    } else {
        DumpParentPtr( back, rtn->blk );
    }
    BuffByte( back, rtn->pro_size );
    BuffByte( back, lc - rtn->epi_start );
    BuffOffset( back, rtn->ret_offset );
    BuffIndex( back, tipe );
    if( rtn->return_loc == NULL ) {
        BuffByte( back, 0 ); /* no return location */
    } else {
        LocDump( back, rtn->return_loc );
    }
    count = 0;
    parm = rtn->parms;
    while( parm != NULL ) {
        ++count;
        parm = parm->link;
    }
    BuffByte( back, count );
    parm = rtn->parms;
    while( parm != NULL ) {
        LocDump( back, parm->loc );
        junk = parm;
        parm = parm->link;
        CGFree( back, junk );
    }
    BuffWSLString( back, FEName( *back.front, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ) ) );
    BuffEnd( back, back.data.DbgLocals );
    DumpLocals( back, rtn->blk->locals );
}
