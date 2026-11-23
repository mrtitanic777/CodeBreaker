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
* Description:  Common code detection and optimization.
*
****************************************************************************/


#include "_cgstd.h"
#include "optwif.h"
#include "utils.h"
#include "inslist.h"
#include "block.h"
#include "data.h"
#include "optmain.h"
#include "encode.h"
#include "optutil.h"
#include "optmkins.h"
#include "optcom.h"
#include "optins.h"
#include "optpull.h"
#include "backrun.hpp"


typedef struct common_info {
        ins_entry   *start_com;
        ins_entry   *start_del;
        uint        save;
} common_info;

static  bool    JustMoveLabel( BackRun& back, common_info *max, ins_entry *ins )
/***************************************************************/
{
    oc_class            cl;
    ins_entry           *lbl;
    ins_entry           *add;
    ins_entry           *next;

  OptBegin( back );
    if( PrevClass( back, max->start_del ) != OC_LABEL )
        { OptEnd( back ); return( false ); }
    lbl = PrevIns( back, max->start_del );
    if( _ChkAttr( lbl, OC_ATTR_SHORT ) )
        { OptEnd( back ); return( false ); }
    cl = PrevClass( back, lbl );
    if( !_TransferClass( cl ) )
        { OptEnd( back ); return( false ); }
    DeleteQueue( back, lbl );
    InsertQueue( back, lbl, PrevIns( back, max->start_com ) );
    add = PrevIns( back, max->start_del );
    for( ;; ) {
        next = NextIns( back, add );
        DelInstr( back, next );
        if( next == ins ) {
            break;
        }
    }
    Untangle( back, lbl );
    { OptEnd( back ); return( true ); }
}


static  void    TransformJumps( BackRun& back, ins_entry *ins, ins_entry *first )
/****************************************************************/
{
    ins_entry           *add;
    ins_entry           *next;
    ins_entry           *lbl;
    oc_class            cl;

  OptBegin( back );
    cl = _Class( ins );
    if( cl == OC_RET || cl == OC_NORET )
        { OptEnd( back ); return; }
    lbl = _Label( ins )->ins;
    for( add = lbl; add != NULL; add = NextIns( back, add ) ) {
        cl = _Class( add );
        if( _TransferClass( cl ) )
            break;
        if( cl == OC_LABEL ) {
            if( _ChkAttr( add, OC_ATTR_SHORT ) )
                { OptEnd( back ); return; }
            _ClrStatus( _Label( add ), SHORTREACH );
        }
    }
    if( add == NULL || add == first || add == ins )
        { OptEnd( back ); return; }
    if( FindShort( back, first, lbl ) )
        { OptEnd( back ); return; }
    for( ;; ) {
        next = PrevIns( back, add );
        DeleteQueue( back, add );
        InsertQueue( back, add, first );
        if( add == lbl )
            break;
        add = next;
    }
    DeleteQueue( back, first );
    InsertQueue( back, first, next );
    Untangle( back, NextIns( back, first ) );
  OptEnd( back );
}


static  bool    CommonInstr( BackRun& back, ins_entry *old, ins_entry *add )
/***********************************************************/
{
  OptBegin( back );
    if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION ) )
        { OptEnd( back ); return( false ); }
    if( _ClassInfo( add ) != _ClassInfo( old ) )
        { OptEnd( back ); return( false ); }
    if( _InsLen( add ) != _InsLen( old ) )
        { OptEnd( back ); return( false ); }
    if( _ObjLen( add ) != _ObjLen( old ) )
        { OptEnd( back ); return( false ); }
    switch( _Class( old ) ) {
    case OC_BDATA:
        /* User may be doing stupid stuff like stringing together a bunch
         * of '__asm _emit x' or '__asm db x' statements which aren't complete
         * instructions. If we try to optimize and split them, things are
         * going to go boom. So let's just not touch those - it's extremely
         * unlikely that we'd be missing any real optimization opportunities.
         */
        /* fall through */
    case OC_DEAD:
        { OptEnd( back ); return( false ); }
    case OC_JMP:
    case OC_CALL:
    case OC_LABEL:
    case OC_LREF:
        if( _Label( old ) != _Label( add ) )
            { OptEnd( back ); return( false ); }
        break;
    case OC_JCOND:
        if( _JmpCond( old ) != _JmpCond( add ) )
            { OptEnd( back ); return( false ); }
        if( _Label( old ) != _Label( add ) )
            { OptEnd( back ); return( false ); }
        break;
    case OC_RET:
        if( _RetPop( old ) != _RetPop( add ) )
            { OptEnd( back ); return( false ); }
        break;
    case OC_NORET:
        break;
    default:
        if( !Equal( &add->oc.oc_entry.data, &old->oc.oc_entry.data, _InsLen( add ) - offsetof( oc_entry, data ) ) )
            { OptEnd( back ); return( false ); }
        break;
    }
    { OptEnd( back ); return( true ); }
}


static  void    FindCommon( BackRun& back, common_info *c, ins_entry *p1, ins_entry *p2 )
/************************************************************************/
{
  OptBegin( back );
    c->save = 0;
    for( ;; ) {
        if( !CommonInstr( back, p1, p2 ) )
            break;
        c->start_com = p1;
        c->start_del = p2;
        c->save += _ObjLen( p1 );
        for( ;; ) {
            p1 = PrevIns( back, p1 );
            if( p1 == NULL )
                { OptEnd( back ); return; }
            if( _Class( p1 ) != OC_LABEL ) {
                break;
            }
        }
        p2 = PrevIns( back, p2 );
        if( p2 == NULL ) {
            break;
        }
    }
  OptEnd( back );
}

bool    ComTail( BackRun& back, ins_entry *list, ins_entry *ins )
/************************************************/
{
    ins_entry           *try_;
    ins_entry           *add;
    ins_entry           *next;
    ins_entry           *lbl;
    ins_entry           *first;
    common_info         common;
    common_info         max;
    obj_length          align;

  OptBegin( back );
    align = (_Class( list ) == OC_LABEL) ? _ObjLen( list ) : 0;
    try_ = list;
    first = NULL;
    max.save = 0;
    for( ;; ) {
        if( try_ != ins ) {
            FindCommon( back, &common, try_, ins );
            if( common.save > max.save ) {
                max = common;
                first = try_;
            }
        }
        try_ = _LblRef( try_ );
        if( try_ == NULL ) {
            break;
        }
    }
    if( max.save == 0 )
        { OptEnd( back ); return( false ); }
    if( JustMoveLabel( back, &max, ins ) )
        { OptEnd( back ); return( true ); }
    if( back.data.OptForSize < 25 )
        { OptEnd( back ); return( false ); }
    if( max.save <= OptInsSize( back, OC_JMP, OC_DEST_NEAR ) )
        { OptEnd( back ); return( false ); }
    TransformJumps( back, ins, first );
    add = PrevIns( back, max.start_del );
    try_ = PrevIns( back, max.start_com );
    if( FindShort( back, try_, NULL ) )
        align = 0;
    AddNewJump( back, add, AddNewLabel( back, PrevIns( back, max.start_com ), align ) );
    lbl = PrevIns( back, max.start_com );
    add = NextIns( back, add );
    for( ;; ) {
        next = NextIns( back, add );
        DelInstr( back, next );
        if( next == ins ) {
            break;
        }
    }
    Untangle( back, lbl );
    IsolatedCode( back, add );
    { OptEnd( back ); return( true ); }
}


bool    ComCode( BackRun& back, ins_entry *jmp )
/*******************************/
{
    ins_entry   *new_ins;
    ins_entry   *com;
    obj_length  align;
    bool        common;

  OptBegin( back );
    common = false;
    com = NULL;
    new_ins = _Label( jmp )->ins;
    if( new_ins != NULL ) {
        align = _ObjLen( new_ins );
        for( ;; ) {
            for( ;; ) {
                new_ins = PrevIns( back, new_ins );
                if( new_ins == NULL )
                    break;
                if( new_ins == jmp )
                    break;
                if( _Class( new_ins ) != OC_LABEL ) {
                    break;
                }
            }
            if( new_ins == NULL )
                break;
            if( new_ins == jmp )
                break;
            com = PrevIns( back, jmp );
            if( com == NULL )
                break;
            if( !CommonInstr( back, new_ins, com ) )
                break;
            com = PrevIns( back, DelInstr( back, com ) );
            common = true;
            if( _Class( jmp ) == OC_DEAD ) {
                { OptEnd( back ); return( common ); }
            }
        }
        if( common ) {
            if( FindShort( back, new_ins, NULL ) )
                align = 0;
            ChgLblRef( back, jmp, AddNewLabel( back, new_ins, align ) );
            Untangle( back, NextIns( back, new_ins ) );
            Untangle( back, NextIns( back, jmp ) );
            Untangle( back, com );
        }
    }
    if( _Class( jmp ) == OC_DEAD )
        { OptEnd( back ); return( common ); }
    common |= ComTail( back, _Label( jmp )->refs, jmp );
    { OptEnd( back ); return( common ); }
}


void    TraceCommon( BackRun& back, ins_entry *lbl_ins )
/***************************************/
{
    ins_entry   *ref;
    ins_entry   *ins;

  OptBegin( back );
    for( ;; ) {
        ref = _Label( lbl_ins )->refs;
        for( ;; ) {
            for( ;; ) {
                if( ref == NULL )
                    { OptEnd( back ); return; }
                ins = ref;
                ref = _LblRef( ref );
                if( _Class( ins ) == OC_JMP ) {
                    break;
                }
            }
            if( ComCode( back, ins ) ) {
                break;
            }
        }
        if( ref == NULL ) {
            break;
        }
    }
    { OptEnd( back ); return; }
}
