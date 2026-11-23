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
#include "optwif.h"
#include "optutil.h"
#include "backrun.hpp"


void    AddLblDef( BackRun& back, ins_entry *instr )
/***********************************/
{
    label_handle    lbl;

  OptBegin( back );
    LblRef( instr ) = NULL;
    lbl = LabelOf( instr );
    for( ;; ) {
        SetStatus( lbl, CODELABEL );
        lbl->ins = instr;
        lbl = lbl->alias;
        if( lbl == NULL ) {
            break;
        }
    }
  OptEnd( back );
}


void    DelLblDef( BackRun& back, ins_entry *instr )
/***********************************/
{
    label_handle    lbl;

  OptBegin( back );
    lbl = LabelOf( instr );
    if( lbl == NULL )
        { OptEnd( back ); return; }
    if( lbl->refs != NULL )
        { OptEnd( back ); return; }
    if( !TstStatus( lbl, DYINGLABEL ) )
        { OptEnd( back ); return; }
    if( TstStatus( lbl, REDIRECTION ) )
        { OptEnd( back ); return; }
    ScrapCodeLabel( back, lbl );
  OptEnd( back );
}


void    AddLblRef( BackRun& back, ins_entry *instr )
/***********************************/
{
    label_handle    lbl;

  OptBegin( back );
    lbl = LabelOf( instr );
    if( lbl->ins != NULL ) {
        lbl = LabelOf( lbl->ins );
        LabelOf( instr ) = lbl;
    }
    LblRef( instr ) = lbl->refs;
    lbl->refs = instr;
  OptEnd( back );
}


void    DelLblRef( BackRun& back, ins_entry *instr )
/***********************************/
{
    label_handle    old_lbl;

  OptBegin( back );
    old_lbl = LabelOf( instr );
    DelRef( back, &old_lbl->refs, instr );
    TryScrapLabel( back, old_lbl );
  OptEnd( back );
}


void    ChgLblRef( BackRun& back, ins_entry *instr, label_handle new_lbl )
/*********************************************************/
{
    ins_entry       **owner;
    ins_entry       *curr;
    label_handle    old_lbl;

  OptBegin( back );
    old_lbl = LabelOf( instr );
    if( old_lbl != new_lbl ) {
        owner = &old_lbl->refs;
        for( ;; ) {
            curr = *owner;
            if( curr == instr )
                break;
            owner = &LblRef( curr );
        }
        *owner = LblRef( curr );
        LblRef( curr ) = new_lbl->refs;
        new_lbl->refs = curr;
        LabelOf( curr ) = new_lbl;
        TryScrapLabel( back, old_lbl );
    }
  OptEnd( back );
}


bool    UniqueLabel( BackRun& back, label_handle lbl )
/**************************************
        Does the label have any alias which is a UNIQUE label?
*/
{
  OptBegin( back );
    for( ; lbl != NULL; lbl = lbl->alias ) {
        if( TstStatus( lbl, UNIQUE ) ) {
            { OptEnd( back ); return( true ); }
        }
    }
    { OptEnd( back ); return( false ); }
}


ins_entry       *AliasLabels( BackRun& back, ins_entry *old_ins, ins_entry *new_ins )
/********************************************************************/
{
    label_handle    old_lbl;
    label_handle    new_lbl;
    ins_entry       *old_jmp;
    ins_entry       **owner;

  OptBegin( back );
    if( old_ins != new_ins ) {
        /* use maximum of the two aligment requests */
        if( ObjLen( old_ins ) > ObjLen( new_ins ) ) {
            ObjLen( new_ins ) = ObjLen( old_ins );
        }
        SetAttr( new_ins, GetAttr( old_ins ) & OC_ATTR_SHORT );
        new_lbl = LabelOf( new_ins );
        old_lbl = LabelOf( old_ins );
        if( new_lbl->redirect == old_lbl ) {
            new_lbl->redirect = NULL;
            ClrStatus( old_lbl, REDIRECTION );
        } else if( old_lbl->redirect == new_lbl ) {
            old_lbl->redirect = NULL;
            ClrStatus( new_lbl, REDIRECTION );
        }
        for( ;; ) {
            old_lbl->ins = new_lbl->ins;
            old_lbl = old_lbl->alias;
            if( old_lbl == NULL ) {
                break;
            }
        }
        old_lbl = LabelOf( old_ins );
        owner = &old_lbl->refs;
        for( ;; ) {
            old_jmp = *owner;
            if( old_jmp == NULL )
                break;
            LabelOf( old_jmp ) = new_lbl;
            owner = &LblRef( old_jmp );
        }
        *owner = new_lbl->refs;
        new_lbl->refs = old_lbl->refs;
        old_lbl->refs = NULL;
        while( new_lbl->alias != NULL ) {
            new_lbl = new_lbl->alias;
        }
        new_lbl->alias = old_lbl;
        LabelOf( old_ins ) = NULL;
        old_ins = DelInstr( back, old_ins );
    }
    { OptEnd( back ); return( old_ins ); }
}


static  void    KillDeadLabels( BackRun& back, ins_entry *instr )
/************************************************/
{
    label_handle    curr;
    label_handle    *owner;

  OptBegin( back );
    owner = &LabelOf( instr );
    for( ;; ) {
        curr = *owner;
        if( curr == NULL )
            break;
        ValidLbl( curr );
        if( curr->refs == NULL
          && !TstStatus( curr, REDIRECTION | KEEPLABEL )
          && TstStatus( curr, DYINGLABEL ) ) {
            *owner = curr->alias;
            if( curr->redirect != NULL ) {
                ClrStatus( curr->redirect, REDIRECTION );
                TryScrapLabel( back, curr->redirect );
                curr->redirect = NULL;
            }
            ScrapCodeLabel( back, curr );
        } else {
            owner = &curr->alias;
        }
    }
    if( LabelOf( instr ) == NULL ) {
        DelInstr( back, instr );
    }
  OptEnd( back );
}


void    ScrapCodeLabel( BackRun& back, label_handle lbl )
/****************************************/
{
    label_handle    *owner;
    label_handle    redir;
    bool            code;

  OptBegin( back );
    owner = &back.optdata.Handles;
    for( ;; ) {
        if( *owner == NULL )
            { OptEnd( back ); return; }
        if( *owner == lbl )
            break;
        owner = &(*owner)->lbl.link;
    }
    *owner = lbl->lbl.link;
    redir = lbl->redirect;
    code = TstStatus( lbl, CODELABEL );
    CGFree( back, lbl );
    if( !code )
        { OptEnd( back ); return; }
    if( redir == NULL )
        { OptEnd( back ); return; }
    ClrStatus( redir, REDIRECTION );
    TryScrapLabel( back, redir );
  OptEnd( back );
}


void    TryScrapLabel( BackRun& back, label_handle old_lbl )
/*******************************************/
{
  OptBegin( back );
    if( old_lbl->refs != NULL )
        { OptEnd( back ); return; }
    if( old_lbl->ins != NULL ) {
        KillDeadLabels( back, old_lbl->ins );
    } else if( TstStatus( old_lbl, DYINGLABEL )
         && !TstStatus( old_lbl, REDIRECTION | KEEPLABEL ) ) {
        ScrapCodeLabel( back, old_lbl );
    }
  OptEnd( back );
}
