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

  OptBegin( back );
    for( ;; ) {
        PLBlip();
        OptPull(back);
        if( back.optdata.FirstIns == NULL )
            break;
        if( OcClass( back.optdata.FirstIns ) == OC_LABEL ) {
            if( UniqueLabel( back, LabelOf( back.optdata.FirstIns ) ) ) {
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
            owner = &LabelOf( back.optdata.FirstIns );
            for( lbl = LabelOf( back.optdata.FirstIns ); lbl != NULL; lbl = next ) {
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
            if( next_ins != NULL && OcClass( next_ins ) == OC_LABEL ) {
                OutputOC( back, &back.optdata.FirstIns->oc, &next_ins->oc );
            } else {
                OutputOC( back, &back.optdata.FirstIns->oc, NULL );
            }
            cl = OcClass( back.optdata.FirstIns );
            if( cl != OC_LABEL )
                break;
            lbl = LabelOf( back.optdata.FirstIns );
            lbl->ins = NULL;
            next = lbl->alias;
            if( next == NULL )
                break;
            lbl->alias = NULL;
            TryScrapLabel( back, lbl );
            LabelOf( back.optdata.FirstIns ) = next;
        }
        DelInstr( back, back.optdata.FirstIns );
        FreePendingDeletes(back);
        /*
         * Head of queue must be immediately after an unconditional control
         * flow instruction since things may be bypassing the control
         * flow optimizer.
         */
        if( TransferClass( cl ) )
            break;
        if( back.optdata.FirstIns == NULL ) {
            break;
        }
    }
  OptEnd( back );
}


static  bool    LDone( BackRun& back, any_oc *oc )
/*********************************/
{
    label_handle    lbl;

  OptBegin( back );
    if( oc->oc_header.class_ != OC_LDONE )
        { OptEnd( back ); return( false ); }
    lbl = oc->oc_handle.handle;
    ValidLbl( lbl );
    if( !TstStatus( lbl, CODELABEL ) )
        { OptEnd( back ); return( false ); }
    SetStatus( lbl, DYINGLABEL );
    TryScrapLabel( back, lbl );
    { OptEnd( back ); return( true ); }
}

void    InputOC( BackRun& back, any_oc *oc )
/***************************/
{
    oc_class        cl;

  OptBegin( back );
    PSBlip();
    if( !LDone( back, oc ) ) {
        if( OcBaseClass( oc->oc_header.class_ ) != OC_INFO
         && OcBaseClass( oc->oc_header.class_ ) != OC_LABEL ) {
            cl = PrevClass( back, NULL );
            if( TransferClass( cl ) ) {
                { OptEnd( back ); return; } /*dead code*/
            }
        }
        while( back.optdata.QCount >= Q_MAX ) {
            PullQueue(back);
        }
        AddInstr( back, NewInstr( back, oc ), back.optdata.LastIns );
        switch( OcClass( back.optdata.LastIns ) ) {
        case OC_LABEL:
            if( TstStatus( LabelOf( back.optdata.LastIns ), UNIQUE ) ) {
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
            ClrStatus( LabelOf( back.optdata.LastIns ), DYINGLABEL );
            break;
        }
        OptPush(back);
    }
  OptEnd( back );
}


bool    ShrinkQueue( BackRun& back, pointer_uint size )
/**************************************/
{
    pointer_uint    freed;
    pointer_uint    need;

  OptBegin( back );
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
    { OptEnd( back ); return( freed >= need ); }
}


void    InitQueue( BackRun& back )
/***********************/
{
  OptBegin( back );
    back.optdata.QCount = 0;
    back.optdata.FirstIns = NULL;
    back.optdata.LastIns = NULL;
    back.optdata.RetList = NULL;
    back.optdata.NoRetList = NULL;
    back.optdata.PendingDeletes = NULL;
    back.optdata.Handles = NULL;
    InitFrl( &back.optdata.InstrFrl );
  OptEnd( back );
}


void    EmptyQueue( BackRun& back )
/************************/
{
  OptBegin( back );
    while( back.optdata.QCount != 0 ) {
        PullQueue(back);
    }
  OptEnd( back );
}

void    FlushQueue( BackRun& back )
/************************/
{
  OptBegin( back );
    { segment_id old_segid = ChangeOP( back, AskCodeSeg(back) );
        EmptyQueue(back);
    SetOP( back, old_segid ); }
  OptEnd( back );
}

void    FiniQueue( BackRun& back )
/***********************/
{
  OptBegin( back );
    { segment_id old_segid = ChangeOP( back, AskCodeSeg(back) );
        EmptyQueue(back);
    SetOP( back, old_segid ); }
  OptEnd( back );
}


void    AbortQueue( BackRun& back )
/************************/
{
    ins_entry   *next;

  OptBegin( back );
    for( ; back.optdata.FirstIns != NULL; back.optdata.FirstIns = next ) {
        next = back.optdata.FirstIns->ins.next;
        FreeInstr( back, back.optdata.FirstIns );
    }
  OptEnd( back );
}
