/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2022 The Open Watcom Contributors. All Rights Reserved.
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
#include "optwif.h"
#include "escape.h"
#include "inslist.h"
#include "block.h"
#include "data.h"
#include "optmain.h"
#include "objout.h"
#include "optutil.h"
#include "optmkins.h"
#include "optins.h"
#include "optpull.h"
#include "optpush.h"
#include "backrun.hpp"


bool    FindShort( BackRun& back, ins_entry *ins, ins_entry *end )
/*************************************************/
{
    for( ; ins != NULL && ins != end; ins = NextIns( back, ins ) ) {
        if( _Class( ins ) == OC_LABEL ) {
            if( _ChkAttr( ins, OC_ATTR_SHORT ) )
                return( true );
            _ClrStatus( _Label( ins ), SHORTREACH );
        }
    }
    return( false );
}

static  void    DoCloneCode( BackRun& back, ins_entry *jmp, ins_entry *hoist )
/*************************************************************/
{
    ins_entry   *clone_point;
    ins_entry   *clone;
    oc_class    cl;

    clone_point = PrevIns( back, jmp );
    for( ;; ) {
        cl = _Class( hoist );
        switch( cl ) {
        case OC_LABEL:
        case OC_INFO:
        case OC_LREF:
            break;
        default:
            clone = NewInstr( back, &hoist->oc );
            AddInstr( back, clone, clone_point );
            if( _TransferClass( cl ) )
                return;
            clone_point = clone;
            break;
        }
        hoist = NextIns( back, hoist );
    }
}

#define MAX_CLONE_SIZE  40

void    CloneCode( BackRun& back, label_handle lbl )
/************************************
    consider:

        JMP L1
            ....
  L1:   POP   AX
        RET

    we want to clone the code at L1 at the point of the jump,
    since it will be faster (and smaller since POP AX/RET is only 2 bytes
    while the JMP might be 3/5).
*/
{
    ins_entry   *next;
    ins_entry   *lbl_ins;
    ins_entry   *hoist;
    ins_entry   *jmp;
    unsigned    size;
    unsigned    max_size;
    oc_class    cl;

    lbl_ins = lbl->ins;
    if( lbl_ins == NULL )
        return;
    size = 0;
    hoist = NextIns( back, lbl_ins );
    for( next = hoist; next != NULL; next = NextIns( back, next ) ) {
        cl = _Class( next );
        if( cl == OC_CODE && CodeHasAbsPatch( &next->oc.oc_entry ) )
            return;
        if( cl != OC_LABEL ) {
            size += _ObjLen( next );
            if( size > MAX_CLONE_SIZE )
                return;
            if( _TransferClass( cl ) ) {
                break;
            }
        }
    }
    if( next == NULL
      || _Class( next ) == OC_JMP && _Label( next ) == lbl )
        return;
    for( jmp = lbl->refs; jmp != NULL; jmp = _LblRef( jmp ) ) {
        if( next == jmp )
            continue;
        cl = _Class( jmp );
        if( !_TransferClass( cl ) )
            continue;
        max_size = _ObjLen( jmp );
        if( size > max_size && FindShort( back, jmp, NULL ) )
            continue;
        if( back.data.OptForSize < 50 ) {
            max_size *= ( 100 - back.data.OptForSize ) / 25;
        }
        if( size > max_size )
            continue;
        DoCloneCode( back, jmp, hoist );
        DelInstr( back, jmp );
    }
}


ins_entry       *IsolatedCode( BackRun& back, ins_entry *instr )
/***********************************************/
{
    ins_entry   *next;

  OptBegin( back );
    next = NextIns( back, instr );
    for( ;; ) {
        if( next == NULL )
            { OptEnd( back ); return( next ); }
        if( _Class( next ) == OC_LABEL )
            break;
        _Savings( OPT_ISOLATED, _ObjLen( next ) );
        if( _Class( next ) == OC_INFO ) {
            next = next->ins.next;
        } else {
            next = DelInstr( back, next );
        }
        if( _Class( instr ) == OC_DEAD ) {
            break;
        }
    }
    next = Untangle( back, next );
    { OptEnd( back ); return( next ); }
}


bool    StraightenCode( BackRun& back, ins_entry *jump )
/***************************************/
{
    ins_entry   *next;
    ins_entry   *insert;
    ins_entry   *hoist;
    ins_entry   *end_hoist;
    oc_class    cl;
    obj_length  align;

  OptBegin( back );
    hoist = _Label( jump )->ins;
    if( hoist == NULL )
        { OptEnd( back ); return( false ); }
    if( hoist == back.optdata.LastIns )
        { OptEnd( back ); return( false ); }
    cl = PrevClass( back, hoist );
    if( !_TransferClass( cl ) )
        { OptEnd( back ); return( false ); }

    end_hoist = NULL;
    for( next = hoist; ; next = NextIns( back, next ) ) {
        if( next == jump ) { // pushing code down to jump
            if( end_hoist == NULL )
                { OptEnd( back ); return( false ); }
            if( FindShort( back, hoist, end_hoist ) )
                { OptEnd( back ); return( false ); }
            break;
        }
        if( next == NULL ) { // hauling code up to jump
            if( FindShort( back, jump, hoist ) )
                { OptEnd( back ); return( false ); }
            break;
        }
        cl = _Class( next );
        if( end_hoist == NULL && _TransferClass( cl ) ) {
            end_hoist = next;
        }
    }

    align = _ObjLen( hoist );
    insert = jump;
    for( ;; ) {
        if( hoist == NULL ) {
            ChgLblRef( back, jump, AddNewLabel( back, back.optdata.LastIns, align ) );
            next = back.optdata.LastIns;
            break;
        }
        next = NextIns( back, hoist );
        DeleteQueue( back, hoist );
        InsertQueue( back, hoist, insert );
        if( hoist == jump )
            { OptEnd( back ); return( false ); }
        insert = hoist;
        cl = _Class( hoist );
        if( _TransferClass( cl ) ) {
            IsolatedCode( back, insert );
            break;
        }
        hoist = next;
    }
    back.optdata.InsDelete = true;
    Untangle( back, next );
    if( _Class( jump ) != OC_DEAD ) {
        Untangle( back, _Label( jump )->ins );
    }
    { OptEnd( back ); return( true ); }
}


void    CheckStraightenCode( BackRun& back, ins_entry  *lbl_ins )
/************************************************/
{
    label_handle    lbl;
    ins_entry       *jmp;
    oc_class        cl;

  OptBegin( back );
    if( lbl_ins != NULL ) {
        lbl = _Label( lbl_ins );
        for( jmp = lbl->refs; jmp != NULL; jmp = _LblRef( jmp ) ) {
            cl = _Class( jmp );
            if( !_TransferClass( cl ) )
                continue;
            if( StraightenCode( back, jmp ) ) {
                break;
            }
        }
    }
  OptEnd( back );
}


void    CallRet( BackRun& back, ins_entry *instr )
/*********************************/
{
    ins_entry   *lbl;

  OptBegin( back );
    if( _ChkAttr( instr, OC_ATTR_POP ) )
        { OptEnd( back ); return; }
    lbl = _Label( instr )->ins;
    if( lbl == NULL )
        { OptEnd( back ); return; }
    if( NextClass( back, lbl ) != OC_RET )
        { OptEnd( back ); return; }
    _Savings( OPT_CALLTORET, _ObjLen( instr ) );
    DelInstr( back, instr );
  OptEnd( back );
}


void    JmpRet( BackRun& back, ins_entry *instr )
/********************************/
{
    ins_entry   *ret;

  OptBegin( back );
    if( back.optdata.InsDelete )
        { OptEnd( back ); return; }
    ret = _Label( instr )->ins;
    if( ret == NULL )
        { OptEnd( back ); return; }
    ret = NextIns( back, ret );
    if( ret == NULL )
        { OptEnd( back ); return; }
    if( _Class( ret ) != OC_RET )
        { OptEnd( back ); return; }
    JmpToRet( back, instr, ret );
  OptEnd( back );
}
