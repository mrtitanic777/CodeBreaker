/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024-2025 The Open Watcom Contributors. All Rights Reserved.
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
#include "zoiks.h"
#include "inslist.h"
#include "object.h"
#include "opttell.h"
#include "encode.h"
#include "backrun.hpp"


void    TellOptimizerByPassed( BackRun& back )
/***********************************/
/* tell the optimizer we're going around it so it won't flush while we're*/
/* in the object file generator and zap our static variables*/
{
  optbegin
}


void    TellByPassOver( BackRun& back )
/****************************/
{
  optend
}


void    TellAddress( BackRun& back, label_handle lbl, offset addr )
/**************************************************/
{
  optbegin
    _ValidLbl( lbl );
    lbl->lbl.address = addr;
  optend
}


void    TellDonePatches( BackRun& back, label_handle lbl )
/*****************************************/
{
  optbegin
    _ValidLbl( lbl );
    lbl->lbl.patches = NULL;
  optend
}


void    TellReachedLabel( BackRun& back, label_handle lbl )
/******************************************/
{
  optbegin
    _ValidLbl( lbl );
    _SetStatus( lbl, REACHED );
  optend
}

void    TellProcLabel( label_handle lbl )
/***************************************/
{
    _ValidLbl( lbl );
    _SetStatus( lbl, PROCEDURE );
}

void    TellCommonLabel( BackRun& back, label_handle lbl, import_handle imphdl )
/***************************************************************/
{
  optbegin
    _ValidLbl( lbl );
    lbl->lbl.sym = IMPHDL2SYM( imphdl );
    _SetStatus( lbl, COMMON_LBL );
  optend
}


void    TellKeepLabel( BackRun& back, label_handle lbl )
/***************************************/
{
  optbegin
    _ValidLbl( lbl );
    _SetStatus( lbl, KEEPLABEL );
  optend
}


void    TellNoSymbol( BackRun& back, label_handle lbl )
/**************************************/
{
  optbegin
    if( lbl != NULL ) {
        _ValidLbl( lbl );
        lbl->lbl.sym = NULL;
        if( _TstStatus( lbl, CODELABEL ) ) {
            GenKillLabel( back, lbl );
        }
    }
  optend
}

static  void    ReallyScrapLabel( BackRun& back, label_handle lbl )
/**************************************************/
{
    label_handle *owner;

  optbegin
    owner = &back.optdata.Handles;
    for( ;; ) {
        if( *owner == NULL )
            optreturnvoid;
        if( *owner == lbl )
            break;
        owner = &(*owner)->lbl.link;
    }
    *owner = lbl->lbl.link;
    CGFree( back, lbl );
  optend
}


void    TellScrapLabel( BackRun& back, label_handle lbl )
/****************************************/
{
  optbegin
    /* data labels in the code segment can't get freed until the end */
    if( _TstStatus( lbl, CODELABEL ) ) {
        ReallyScrapLabel( back, lbl );
    }
  optend
}


void    TellCondemnedLabel( BackRun& back, label_handle lbl )
/********************************************/
{
  optbegin
    _ValidLbl( lbl );
    _SetStatus( lbl, CONDEMNED );
  optend
}


static  label_handle    NextCondemned( BackRun& back, label_handle lbl )
/*******************************************************/
{
  optbegin
    for( ;; ) {
        if( lbl == NULL )
            break;
        if( _TstStatus( lbl, CONDEMNED ) ) {
            _ClrStatus( lbl, CONDEMNED );
            break;
        }
        lbl = lbl->lbl.link;
    }
    optreturn( lbl );
}


void    TellBeginExecutions( BackRun& back )
/*********************************/
{
    label_handle    dead;

  optbegin
    for( ;; ) {
        dead = NextCondemned( back, back.optdata.Handles );
        if( dead == NULL )
            break;
        GenKillLabel( back, dead );
    }
  optend
}


void    TellFreeAllLabels( BackRun& back )
/*******************************/
{
    bool        unfreed;

  optbegin
    unfreed = false;
    while( back.optdata.Handles != NULL ) {
#ifndef PRODUCTION
        if( _TstStatus( back.optdata.Handles, CODELABEL )
         && back.optdata.Handles->lbl.sym == NULL
         && !_TstStatus( back.optdata.Handles, REDIRECTION )
         && !unfreed ) {
            _Zoiks( ZOIKS_001 );
            unfreed = true;
        }
#endif
        ReallyScrapLabel( back, back.optdata.Handles );
    }
  optend
}


void    TellUnreachLabels( BackRun& back )
/********************************
    Mark all the labels that have come out of the control flow queue as
    unreachable by a short jump.
*/
{
    label_handle    lbl;

  optbegin
    for( lbl = back.optdata.Handles; lbl != NULL; lbl = lbl->lbl.link ) {
        if( lbl->lbl.address != ADDR_UNKNOWN ) {
            _SetStatus( lbl, UNREACHABLE );
        }
    }
  optend
}

void    KillLblRedirects( BackRun& back )
/******************************/
{
    label_handle    lbl;

  optbegin
#if  OPTIONS & SHORT_JUMPS
    for( lbl = back.optdata.Handles; lbl != NULL; lbl = lbl->lbl.link ) {
        lbl->redirect = NULL;
    }
#endif
    TellUnreachLabels(back);        // a little too conservative - stop gap
  optend
}
