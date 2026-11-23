/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024      The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Instruction creation and deletion.
*
****************************************************************************/


#include "_cgstd.h"
#include "optwif.h"
#include "freelist.h"
#include "utils.h"
#include "inslist.h"
#include "encode.h"
#include "optutil.h"
#include "optmkins.h"
#include "backrun.hpp"


ins_entry       *NewInstr( BackRun& back, any_oc *oc )
/*************************************/
{
    ins_entry   *instr;
    oc_length   len;

  OptBegin( back );
    len = oc->oc_header.reclen + sizeof( ins_link );
    if( len <= OCENTRY_SIZE ) {
        instr = static_cast<ins_entry *>(AllocFrl( back, &back.optdata.InstrFrl, OCENTRY_SIZE ));
    } else {
        instr = static_cast<ins_entry *>(CGAlloc( back, len ));
    }
    instr->ins.prev = NULL;
    instr->ins.next = NULL;
    memcpy( &instr->oc, oc, oc->oc_header.reclen );
    { OptEnd( back ); return( instr ); }
}


void    FreeInstr( BackRun& back, ins_entry *instr )
/***********************************/
{
    oc_length   len;

    len = instr->oc.oc_header.reclen + sizeof( ins_link );
    if( len <= OCENTRY_SIZE ) {
        FrlFreeSize( back, &back.optdata.InstrFrl, (pointer *)instr, OCENTRY_SIZE );
    } else {
        CGFree( back, instr );
    }
}


bool    InstrFrlFree( BackRun& back )
/**************************/
{
    return( FrlFreeAll( back, &back.optdata.InstrFrl, OCENTRY_SIZE ) );
}


label_handle AddNewLabel( BackRun& back, ins_entry *new_ins, obj_length align )
/**************************************************************/
{
    label_handle    lbl;
    any_oc          oc;

  OptBegin( back );
    if( NextClass( back, new_ins ) == OC_LABEL )
        { OptEnd( back ); return( LabelOf( NextIns( back, new_ins ) ) ); }
    if( new_ins != NULL && OcClass( new_ins ) == OC_LABEL )
        { OptEnd( back ); return( LabelOf( new_ins ) ); }
    oc.oc_handle.hdr.class_ = OC_LABEL;
    oc.oc_handle.hdr.reclen = sizeof( oc_handle );
    oc.oc_handle.hdr.objlen = align;
    lbl = AskForNewLabel(back);
    oc.oc_handle.ref = NULL;
    oc.oc_handle.handle = lbl;
    AddInstr( back, NewInstr( back, &oc ), new_ins );
    SetStatus( lbl, DYINGLABEL );
  { OptEnd( back ); return( lbl ); }
}


void    AddNewJump( BackRun& back, ins_entry *new_ins, label_handle lbl )
/********************************************************/
{
    any_oc     oc;

  OptBegin( back );
    oc.oc_handle.hdr.class_ = OC_JMP;
    oc.oc_handle.hdr.reclen = sizeof( oc_handle );
    oc.oc_handle.hdr.objlen = OptInsSize( back, OC_JMP, OC_DEST_NEAR );
    oc.oc_handle.ref = NULL;
    oc.oc_handle.handle = lbl;
    AddInstr( back, NewInstr( back, &oc ), new_ins );
  OptEnd( back );
}
