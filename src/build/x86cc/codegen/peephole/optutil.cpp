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
* Description:  Optimizer utility routines.
*
****************************************************************************/


#include "_cgstd.h"
#include "optwif.h"
#include "stackcg.h"
#include "optutil.h"
#include "optmkins.h"
#include "optins.h"
#include "optpull.h"
#include "optpush.h"
#include "backrun.hpp"


ins_entry       *ValidIns( BackRun& back, ins_entry *instr )
/*******************************************/
{
  OptBegin( back );
    for( ;; ) {
        if( instr == NULL )
            break;
        if( OcClass( instr ) != OC_DEAD )
            break;
        instr = instr->ins.next;
    }
    { OptEnd( back ); return( instr ); }
}


ins_entry       *PrevIns( BackRun& back, ins_entry *instr )
/******************************************/
{
  OptBegin( back );
    if( instr == NULL ) {
        instr = back.optdata.LastIns;
    } else if( OcClass( instr ) == OC_DEAD ) {
        instr = NULL;
    } else {
        instr = instr->ins.prev;
    }
    for( ;; ) {
        if( instr == NULL )
            break;
        if( OcClass( instr ) != OC_INFO )
            break;
        instr = instr->ins.prev;
    }
    { OptEnd( back ); return( instr ); }
}


oc_class        PrevClass( BackRun& back, ins_entry *instr )
/*******************************************/
{
    oc_class    class_;

  OptBegin( back );
    instr = PrevIns( back, instr );
    if( instr != NULL ) {
        class_ = OcClass( instr );
    } else {
        class_ = OC_DEAD;
    }
    { OptEnd( back ); return( class_ ); }
}


ins_entry       *NextIns( BackRun& back, ins_entry *instr )
/******************************************/
{
  OptBegin( back );
    if( instr == NULL ) {
        instr = back.optdata.FirstIns;
    } else if( OcClass( instr ) == OC_DEAD ) {
        instr = NULL;
    } else {
        instr = instr->ins.next;
    }
    for( ;; ) {
        if( instr == NULL )
            break;
        if( OcClass( instr ) != OC_INFO )
            break;
        instr = instr->ins.next;
    }
    { OptEnd( back ); return( instr ); }
}


oc_class        NextClass( BackRun& back, ins_entry *instr )
/*******************************************/
{
    oc_class    class_;

  OptBegin( back );
    instr = NextIns( back, instr );
    if( instr != NULL ) {
        class_ = OcClass( instr );
    } else {
        class_ = OC_DEAD;
    }
    { OptEnd( back ); return( class_ ); }
}


void    AddInstr( BackRun& back, ins_entry *instr, ins_entry *insert )
/*****************************************************/
{
  OptBegin( back );
    InsertQueue( back, instr, insert );
    switch( OcClass( instr ) ) {
    case OC_LABEL:
        ValidLbl( LabelOf( instr ) );
        AddLblDef( back, instr );
        break;
    case OC_LREF:
    case OC_CALL:
    case OC_JCOND:
    case OC_JMP:
        ValidLbl( LabelOf( instr ) );
        AddLblRef( back, instr );
        break;
    case OC_RET:
        LblRef( instr ) = back.optdata.RetList;
        back.optdata.RetList = instr;
        break;
    case OC_NORET:
        LblRef( instr ) = back.optdata.NoRetList;
        back.optdata.NoRetList = instr;
        break;
    }
  OptEnd( back );
}


void    DelRef( BackRun& back, ins_entry **owner, ins_entry *instr )
/***************************************************/
{
    ins_entry   *curr;

  OptBegin( back );
    for( ;; ) {
        curr = *owner;
        if( curr == instr )
            break;
        owner = &LblRef( curr );
    }
    *owner = LblRef( curr );
  OptEnd( back );
}


void    UnLinkInstr( BackRun& back, ins_entry *old )
/***********************************/
{
  OptBegin( back );
    back.optdata.InsDelete = true;
    DeleteQueue( back, old );
    old->ins.prev = back.optdata.PendingDeletes;
    back.optdata.PendingDeletes = old;
  OptEnd( back );
}


static  ins_entry *DelInstr_Helper( BackRun& back, ins_entry *old )
/**************************************************/
{
    ins_entry   *next;

    UnLinkInstr( back, old );
    switch( OcClass( old ) ) {
    case OC_LABEL:
        if( LabelOf( old ) != NULL ) {
            ValidLbl( LabelOf( old ) );
        }
        DelLblDef( back, old );
        next = ValidIns( back, old->ins.next );
        switch( PrevClass( back, next ) ) {
        case OC_JMP:
        case OC_JMPI:
            IsolatedCode( back, PrevIns( back, next ) );
            break;
        case OC_JCOND:
            Untangle( back, LabelOf( PrevIns( back, next ) )->ins );
            break;
        }
        break;
    case OC_LREF:
    case OC_CALL:
    case OC_JCOND:
    case OC_JMP:
        ValidLbl( LabelOf( old ) );
        DelLblRef( back, old );
        break;
    case OC_RET:
        DelRef( back, &back.optdata.RetList, old );
        break;
    case OC_NORET:
        DelRef( back, &back.optdata.NoRetList, old );
        break;
    }
    ResetClass( old, OC_DEAD );
    next = ValidIns( back, old->ins.next );
    for( ;; ) {
        if( next == NULL )
            return( NULL );
        if( OcClass( next ) != OC_INFO )
            break;
        next = next->ins.next;
    }
    if( next->ins.prev == NULL )
        return( next );
    MultiLineNums( back, next->ins.prev );
    return( next );
}


ins_entry       *DelInstr( BackRun& back, ins_entry *old )
/*****************************************/
{
    if( OcClass( old ) != OC_DEAD ) {
        return static_cast<ins_entry *>(( SafeRecurseCG( back, reinterpret_cast<func_sr>(DelInstr_Helper), old ) ));
    }
    return( ValidIns( back, old->ins.next ) );
}


void    FreePendingDeletes( BackRun& back )
/********************************/
{
    ins_entry   *next;

  OptBegin( back );
    for( ; back.optdata.PendingDeletes != NULL; back.optdata.PendingDeletes = next ) {
        next = back.optdata.PendingDeletes->ins.prev;
        FreeInstr( back, back.optdata.PendingDeletes );
    }
  OptEnd( back );
}
