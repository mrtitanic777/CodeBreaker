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


static  void    PullQueue( void )
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
        OptPull();
        if( optdata_state().FirstIns == NULL )
            break;
        if( _Class( optdata_state().FirstIns ) == OC_LABEL ) {
            if( UniqueLabel( _Label( optdata_state().FirstIns ) ) ) {
                /*
                 * take off extra byte added when the ins went into queue
                 */
                optdata_state().FirstIns->oc.oc_header.objlen--;
            }
            /*
             * We want to order the alias list so that the labels with
             * symbols dump out first. They might be FE_COMMON labels.
             */
            be_lbls = NULL;
            owner = &_Label( optdata_state().FirstIns );
            for( lbl = _Label( optdata_state().FirstIns ); lbl != NULL; lbl = next ) {
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
            next_ins = NextIns( optdata_state().FirstIns );
            if( next_ins != NULL && _Class( next_ins ) == OC_LABEL ) {
                OutputOC( &optdata_state().FirstIns->oc, &next_ins->oc );
            } else {
                OutputOC( &optdata_state().FirstIns->oc, NULL );
            }
            cl = _Class( optdata_state().FirstIns );
            if( cl != OC_LABEL )
                break;
            lbl = _Label( optdata_state().FirstIns );
            lbl->ins = NULL;
            next = lbl->alias;
            if( next == NULL )
                break;
            lbl->alias = NULL;
            TryScrapLabel( lbl );
            _Label( optdata_state().FirstIns ) = next;
        }
        DelInstr( optdata_state().FirstIns );
        FreePendingDeletes();
        /*
         * Head of queue must be immediately after an unconditional control
         * flow instruction since things may be bypassing the control
         * flow optimizer.
         */
        if( _TransferClass( cl ) )
            break;
        if( optdata_state().FirstIns == NULL ) {
            break;
        }
    }
  optend
}


static  bool_    LDone( any_oc *oc )
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
    TryScrapLabel( lbl );
    optreturn( true );
}

void    InputOC( any_oc *oc )
/***************************/
{
    oc_class        cl;

  optbegin
    PSBlip();
    if( !LDone( oc ) ) {
        if( OC_BASE_CLASS( oc->oc_header.class_ ) != OC_INFO
         && OC_BASE_CLASS( oc->oc_header.class_ ) != OC_LABEL ) {
            cl = PrevClass( NULL );
            if( _TransferClass( cl ) ) {
                optreturnvoid; /*dead code*/
            }
        }
        while( optdata_state().QCount >= Q_MAX ) {
            PullQueue();
        }
        AddInstr( NewInstr( oc ), optdata_state().LastIns );
        switch( _Class( optdata_state().LastIns ) ) {
        case OC_LABEL:
            if( _TstStatus( _Label( optdata_state().LastIns ), UNIQUE ) ) {
                /*
                 * Unique labels might need an addition byte of spacing
                 */
                optdata_state().LastIns->oc.oc_header.objlen++;
            }
            /* fall through */
        case OC_LREF:
        case OC_JCOND:
        case OC_JMP:
        case OC_CALL:
            _ClrStatus( _Label( optdata_state().LastIns ), DYINGLABEL );
            break;
        }
        OptPush();
    }
  optend
}


bool_    ShrinkQueue( pointer_uint size )
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
        if( optdata_state().QCount <= Q_MIN )
            break;
        freed += MemInUse();
        PullQueue();
        freed -= MemInUse();
    }
    optreturn( freed >= need );
}


void    InitQueue( void )
/***********************/
{
  optbegin
    optdata_state().QCount = 0;
    optdata_state().FirstIns = NULL;
    optdata_state().LastIns = NULL;
    optdata_state().RetList = NULL;
    optdata_state().NoRetList = NULL;
    optdata_state().PendingDeletes = NULL;
    optdata_state().Handles = NULL;
    InitFrl( &optdata_state().InstrFrl );
#if( OPTIONS & SAVINGS )
    Savings = 0;
#endif
  optend
}


void    EmptyQueue( void )
/************************/
{
  optbegin
    while( optdata_state().QCount != 0 ) {
        PullQueue();
    }
  optend
}

void    FlushQueue( void )
/************************/
{
  optbegin
    PUSH_OP( AskCodeSeg() );
        EmptyQueue();
    POP_OP();
  optend
}

void    FiniQueue( void )
/***********************/
{
  optbegin
    PUSH_OP( AskCodeSeg() );
        EmptyQueue();
        _DumpSavings();
    POP_OP();
  optend
}


void    AbortQueue( void )
/************************/
{
    ins_entry   *next;

  optbegin
    for( ; optdata_state().FirstIns != NULL; optdata_state().FirstIns = next ) {
        next = optdata_state().FirstIns->ins.next;
        FreeInstr( optdata_state().FirstIns );
    }
  optend
}
