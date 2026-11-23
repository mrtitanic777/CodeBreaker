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
* Description:  Instruction queue manipulation.
*
****************************************************************************/


#include "_cgstd.h"
#include "optwif.h"
#include "freelist.h"
#include "objout.h"
#include "inslist.h"
#include "blips.h"
#include "optmain.h"
#include "optutil.h"
#include "optmkins.h"
#include "optins.h"
#include "memsydep.h"
#include "backrun.hpp"


static  void    PullQueue( BackRun& back )
/*******************************/
{
    label_handle    lbl;
    label_handle    next;
    oc_class        cl;
    ins_entry       *next_ins;
    label_handle    be_lbls;
    label_handle    *owner;

  optbegin
    for( ;; ) {
        PLBlip();
        OptPull(back);
        if( back.optdata.FirstIns == NULL )
            break;
        if( _Class( back.optdata.FirstIns ) == OC_LABEL ) {
            if( UniqueLabel( back, _Label( back.optdata.FirstIns ) ) ) {
                /*
                 * take off extra byte added when the ins went into queue
                 */
                back.optdata.FirstIns->oc.oc_header.objlen--;
            }
            /*
             * We want to order the alias list so that the labels with
             * symbols dump out first. They might be FE_COMMON labels.
             */
            be_lbls = NULL;
            owner = &_Label( back.optdata.FirstIns );
            for( lbl = _Label( back.optdata.FirstIns ); lbl != NULL; lbl = next ) {
                next = lbl->alias;
                if( lbl->lbl.sym != NULL ) {
                    *owner = lbl;
                    owner = &lbl->alias;
                } else {
                    lbl->alias = be_lbls;
                    be_lbls = lbl;
                }
            }
            *owner = be_lbls;
        }
        for( ;; ) {
            next_ins = NextIns( back, back.optdata.FirstIns );
            if( next_ins != NULL && _Class( next_ins ) == OC_LABEL ) {
                OutputOC( back, &back.optdata.FirstIns->oc, &next_ins->oc );
            } else {
                OutputOC( back, &back.optdata.FirstIns->oc, NULL );
            }
            cl = _Class( back.optdata.FirstIns );
            if( cl != OC_LABEL )
                break;
            lbl = _Label( back.optdata.FirstIns );
            lbl->ins = NULL;
            next = lbl->alias;
            if( next == NULL )
                break;
            lbl->alias = NULL;
            TryScrapLabel( back, lbl );
            _Label( back.optdata.FirstIns ) = next;
        }
        DelInstr( back, back.optdata.FirstIns );
        FreePendingDeletes(back);
        /*
         * Head of queue must be immediately after an unconditional control
         * flow instruction since things may be bypassing the control
         * flow optimizer.
         */
        if( _TransferClass( cl ) )
            break;
        if( back.optdata.FirstIns == NULL ) {
            break;
        }
    }
  optend
}


static  bool    LDone( BackRun& back, any_oc *oc )
/*********************************/
{
    label_handle    lbl;

  optbegin
    if( oc->oc_header.class_ != OC_LDONE )
        optreturn( false );
    lbl = oc->oc_handle.handle;
    _ValidLbl( lbl );
    if( !_TstStatus( lbl, CODELABEL ) )
        optreturn( false );
    _SetStatus( lbl, DYINGLABEL );
    TryScrapLabel( back, lbl );
    optreturn( true );
}

void    InputOC( BackRun& back, any_oc *oc )
/***************************/
{
    oc_class        cl;

  optbegin
    PSBlip();
    if( !LDone( back, oc ) ) {
        if( OC_BASE_CLASS( oc->oc_header.class_ ) != OC_INFO
         && OC_BASE_CLASS( oc->oc_header.class_ ) != OC_LABEL ) {
            cl = PrevClass( back, NULL );
            if( _TransferClass( cl ) ) {
                optreturnvoid; /*dead code*/
            }
        }
        while( back.optdata.QCount >= Q_MAX ) {
            PullQueue(back);
        }
        AddInstr( back, NewInstr( back, oc ), back.optdata.LastIns );
        switch( _Class( back.optdata.LastIns ) ) {
        case OC_LABEL:
            if( _TstStatus( _Label( back.optdata.LastIns ), UNIQUE ) ) {
                /*
                 * Unique labels might need an addition byte of spacing
                 */
                back.optdata.LastIns->oc.oc_header.objlen++;
            }
            /* fall through */
        case OC_LREF:
        case OC_JCOND:
        case OC_JMP:
        case OC_CALL:
            _ClrStatus( _Label( back.optdata.LastIns ), DYINGLABEL );
            break;
        }
        OptPush(back);
    }
  optend
}


bool    ShrinkQueue( BackRun& back, pointer_uint size )
/**************************************/
{
    pointer_uint    freed;
    pointer_uint    need;

  optbegin
    need = size;
    freed = 0;
    for( ;; ) {
        if( freed >= need )
            break;
        if( back.optdata.QCount <= Q_MIN )
            break;
        freed += MemInUse(back);
        PullQueue(back);
        freed -= MemInUse(back);
    }
    optreturn( freed >= need );
}


void    InitQueue( BackRun& back )
/***********************/
{
  optbegin
    back.optdata.QCount = 0;
    back.optdata.FirstIns = NULL;
    back.optdata.LastIns = NULL;
    back.optdata.RetList = NULL;
    back.optdata.NoRetList = NULL;
    back.optdata.PendingDeletes = NULL;
    back.optdata.Handles = NULL;
    InitFrl( &back.optdata.InstrFrl );
  optend
}


void    EmptyQueue( BackRun& back )
/************************/
{
  optbegin
    while( back.optdata.QCount != 0 ) {
        PullQueue(back);
    }
  optend
}

void    FlushQueue( BackRun& back )
/************************/
{
  optbegin
    PUSH_OP( AskCodeSeg(back) );
        EmptyQueue(back);
    POP_OP();
  optend
}

void    FiniQueue( BackRun& back )
/***********************/
{
  optbegin
    PUSH_OP( AskCodeSeg(back) );
        EmptyQueue(back);
        _DumpSavings();
    POP_OP();
  optend
}


void    AbortQueue( BackRun& back )
/************************/
{
    ins_entry   *next;

  optbegin
    for( ; back.optdata.FirstIns != NULL; back.optdata.FirstIns = next ) {
        next = back.optdata.FirstIns->ins.next;
        FreeInstr( back, back.optdata.FirstIns );
    }
  optend
}
