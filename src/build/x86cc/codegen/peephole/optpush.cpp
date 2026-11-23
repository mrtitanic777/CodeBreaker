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
#include "model.h"
#include "inslist.h"
#include "encode.h"
#include "optutil.h"
#include "optmkins.h"
#include "optins.h"
#include "optpull.h"
#include "optpush.h"
#include "backrun.hpp"


void    RetAftrCall( BackRun& back, ins_entry *ret_instr )
/*****************************************/
{
    ins_entry   *call_instr;

  optbegin
    if( _IsModel( CGSW_GEN_NO_CALL_RET_TRANSFORM ) )
        optreturnvoid;
    for( call_instr = ret_instr; PrevClass( back, call_instr ) == OC_LABEL; ) {
        call_instr = PrevIns( back, call_instr );
    }
    if( PrevClass( back, call_instr ) != OC_CALL )
        optreturnvoid;
    call_instr = PrevIns( back, call_instr );
    if( _ChkAttr( call_instr, OC_ATTR_POP ) )
        optreturnvoid;
    if( _ChkAttr( ret_instr, OC_ATTR_POP ) )
        optreturnvoid;
    if( ( _ChkAttr( call_instr, OC_ATTR_FAR ) ^ _ChkAttr( ret_instr, OC_ATTR_FAR ) ) != 0 )
        optreturnvoid;
    back.optdata.InsDelete = true;
    _ChgClass( call_instr, OC_JMP );
    /* NB! this code assumes that we aren't making the instruction longer */
    if( _ObjLen( call_instr ) == OptInsSize( back, OC_CALL, OC_DEST_CHEAP ) ) {
        _ObjLen( call_instr ) = OptInsSize( back, OC_JMP, OC_DEST_NEAR );
        _ClassInfo( call_instr ) &= ~OC_ATTR_FAR;
    } else if( _ObjLen( call_instr ) == OptInsSize( back, OC_CALL, OC_DEST_FAR ) ) {
        _ObjLen( call_instr ) = OptInsSize( back, OC_JMP, OC_DEST_FAR );
    } else {
        _ObjLen( call_instr ) = OptInsSize( back, OC_JMP, OC_DEST_NEAR );
    }
    IsolatedCode( back, call_instr );
    Untangle( back, PrevIns( back, call_instr ) );
  optend
}


void    JmpToRet( BackRun& back, ins_entry *instr, ins_entry *ret )
/**************************************************/
{
  optbegin
    ret = NewInstr( back, &ret->oc );
    AddInstr( back, ret, instr );
    _Savings( OPT_JMPTORET, _ObjLen( instr ) - _ObjLen( ret ) );
    DelInstr( back, instr );
    RetAftrCall( back, ret );
  optend
}


bool    RetAftrLbl( BackRun& back, ins_entry *ret )
/**********************************/
{
    ins_entry       *ref;
    label_handle    lbl;
    ins_entry       *next;
    bool            change;

  optbegin
    change = false;
    if( PrevClass( back, ret ) == OC_LABEL ) {
        lbl = _Label( PrevIns( back, ret ) );
        for( ref = lbl->refs; ref != NULL; ref = next ) {
            next = _LblRef( ref );
            if( _Class( ref ) == OC_JMP ) {
                JmpToRet( back, ref, ret );
                change = true;
            } else if( _Class( ref ) == OC_CALL ) {
                if( !_ChkAttr( ret, OC_ATTR_POP ) ) {
                    _Savings( OPT_CALLTORET, _ObjLen( ref ) );
                    DelInstr( back, ref );
                    change = true;
                }
            }
        }
    }
    optreturn( change );
}


void    MultiLineNums( BackRun& back, ins_entry *ins )
/*************************************/
{
    ins_entry   *prev;

  optbegin
    if( _ClassInfo( ins ) == OC_LINENUM ) {
        prev = ins->ins.prev;
        for( ;; ) {
            if( prev == NULL )
                break;
            if( _Class( prev ) != OC_INFO )
                break;
            if( _ClassInfo( prev ) == OC_LINENUM ) {
                ins = prev->ins.next;
                UnLinkInstr( back, prev );
                _ResetClass( prev, OC_DEAD );
                prev = ins->ins.prev;
            } else {
                prev = prev->ins.prev;
            }
        }
    }
  optend
}
