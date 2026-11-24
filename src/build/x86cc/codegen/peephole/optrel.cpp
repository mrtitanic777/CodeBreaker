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
* Description:  Relative branch optimizations.
*
****************************************************************************/


#include "_cgstd.h"
#include "optwif.h"
#include "inslist.h"
#include "block.h"
#include "data.h"
#include "objout.h"
#include "optmain.h"
#include "encode.h"
#include "optutil.h"
#include "optmkins.h"
#include "optrel.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct OptrelState: optrel_state.h) */

}  // namespace



static  bool    Jmp_to_lbl( BackRun& back, ins_entry *instr )
/********************************************/
{
  OptBegin( back );
    if( OcClass( instr ) != OC_JMP )
        { OptEnd( back ); return( false ); }
    if( LabelOf( instr ) != back.optrel.Handle )
        { OptEnd( back ); return( false ); }
    { OptEnd( back ); return( true ); }
}


static  bool    CanReach( BackRun& back, label_handle lbl, ins_entry **add_ptr,
                                ins_entry **jmp_ptr )
/************************************************************
    Can a short branch at instruction 'FirstIns' reach label 'lbl'?
*/
{
    ins_entry   *add;
    ins_entry   *jmp;
    ins_entry   *instr;
    ins_entry   *lbl_ins;
    offset      obj_len;
    oc_class    cl;

    add = NULL;
    jmp = NULL;
    if( TstStatus( lbl, UNREACHABLE ) ) {
        /* can't do anything with it */
    } else if( lbl->lbl.address == ADDR_UNKNOWN ) {
        if( TstStatus( lbl, SHORTREACH ) )
            return( true );
        obj_len = OptInsSize( back, OC_JMP, OC_DEST_SHORT );
        lbl_ins = back.optrel.Handle->ins;
        for( instr = NextIns( back, back.optdata.FirstIns ); instr != NULL; instr = NextIns( back, instr ) ) {
            obj_len += ObjLen( instr );
            if( obj_len > MAX_SHORT_FWD )
                break;
            if( Jmp_to_lbl( back, instr ) ) {
                jmp = instr;
            } else {
                cl = OcClass( instr );
                if( TransferClass( cl ) ) {
                    add = instr;
                }
            }
            if( instr == lbl_ins ) {
                return( true );
            }
        }
    } else if( (AskLocation(back) - lbl->lbl.address) <= MAX_SHORT_BWD ) {
        return( true );
    }
    if( add_ptr != NULL )
        *add_ptr = add;
    if( jmp_ptr != NULL )
        *jmp_ptr = jmp;
    return( false );
}


static  void    HndlRedirect( BackRun& back, label_handle new_lbl )
/**************************************************/
{
    label_handle    redir;

  OptBegin( back );
    redir = back.optrel.Handle->redirect;
    if( redir != NULL && redir != new_lbl ) {
        ClrStatus( redir, REDIRECTION );
        TryScrapLabel( back, redir );
    }
    back.optrel.Handle->redirect = new_lbl;
    if( new_lbl != NULL ) {
        SetStatus( new_lbl, REDIRECTION );
    }
  OptEnd( back );
}


static  bool    InRange( BackRun& back )
/*****************************/
/* Check if redirection is still within range*/
{
    label_handle    jmp_lbl;

  OptBegin( back );
    if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION ) )
        { OptEnd( back ); return( false ); }
    jmp_lbl = back.optrel.Handle->redirect;
    /* we don't have a redirection for this label*/
    if( jmp_lbl == NULL )
        { OptEnd( back ); return( false ); }
    /* forward jump: must still be in range*/
    if( jmp_lbl->lbl.address == ADDR_UNKNOWN )
        { OptEnd( back ); return( true ); }
    /* can't redirect the redirection jump to itself*/
    if( jmp_lbl->lbl.address == AskLocation(back) )
        { OptEnd( back ); return( false ); }
    /* can still reach old redirection with short backward jump*/
    if( (AskLocation(back) - jmp_lbl->lbl.address) <= MAX_SHORT_BWD )
        { OptEnd( back ); return( true ); }
    /* can't get at old redirection any more*/
    HndlRedirect( back, NULL );
    { OptEnd( back ); return( false ); }
}


static  void    BigBranch( BackRun& back, ins_entry *add, ins_entry *jmp )
/**********************************************************
    We can't get to where we want to go, have to redirect
*/
{
  OptBegin( back );
    if( jmp != NULL && IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
        /* jump to a jump that's going where we want*/
        HndlRedirect( back, AddNewLabel( back, PrevIns( back, jmp ), 0 ) );
        ChgLblRef( back, back.optdata.FirstIns, back.optrel.Handle->redirect );
    } else if( InRange(back) ) {
        /* use old redirection label*/
        ChgLblRef( back, back.optdata.FirstIns, back.optrel.Handle->redirect );
    } else if( add != NULL && IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
        /* add a jump that's in range*/
        AddNewJump( back, add, back.optrel.Handle );
        _Savings( OPT_JUMPS, -ObjLen( NextIns( add ) ) );
        HndlRedirect( back, AddNewLabel( back, add, 0 ) );
        ChgLblRef( back, back.optdata.FirstIns, back.optrel.Handle->redirect );
    } else {
        /* reverse the branch*/
        JmpCond( back.optdata.FirstIns ) = ReverseCondition( JmpCond( back.optdata.FirstIns ) );
        AddNewJump( back, back.optdata.FirstIns, back.optrel.Handle );
        ChgLblRef( back, back.optdata.FirstIns, AddNewLabel( back, NextIns( back, back.optdata.FirstIns ), 0 ) );
        HndlRedirect( back, AddNewLabel( back, back.optdata.FirstIns, 0 ) );
    }
  OptEnd( back );
}


static  void    SetShort( BackRun& back )
/******************************/
{
    ins_entry   *l_ins;
    obj_length  size;
    bool        floating;

  OptBegin( back );
    floating = false;
    if( ChkAttr( back.optdata.FirstIns, OC_ATTR_FLOAT ) ) {
        floating = true;
    }
    size = OptInsSize( back, static_cast<oc_class>(OcClass( back.optdata.FirstIns )), OC_DEST_SHORT );
    if( OcClass( back.optdata.FirstIns ) == OC_JMP ) {
        _Savings( OPT_JUMPS, ObjLen( FirstIns ) - size );
        ObjLen( back.optdata.FirstIns ) = size;
        ResetClass( back.optdata.FirstIns, OC_JMP );
        if( floating ) {
            SetAttr( back.optdata.FirstIns, OC_ATTR_FLOAT );
        }
    } else {
        _Savings( OPT_JCONDS, ObjLen( FirstIns ) - size );
        ObjLen( back.optdata.FirstIns ) = size;
        ResetClass( back.optdata.FirstIns, OC_JCOND );
        if( floating ) {
            SetAttr( back.optdata.FirstIns, OC_ATTR_FLOAT );
        }
    }
    l_ins = LabelOf( back.optdata.FirstIns )->ins;
    if( l_ins != NULL ) {
        SetAttr( l_ins, OC_ATTR_SHORT );
    }
  OptEnd( back );
}


void    SetBranches( BackRun& back )
/*************************/
/* Check whether the actual label itself can be targeted*/
{
  ins_entry     *add;
  ins_entry     *jmp;
  ins_entry     *next;
  bool          was_keep;

  OptBegin( back );
    back.optrel.Handle = LabelOf( back.optdata.FirstIns );
    if( CanReach( back, back.optrel.Handle, &add, &jmp ) ) {
        SetStatus( back.optrel.Handle, SHORTREACH );
        HndlRedirect( back, NULL );
        SetShort(back);
    } else {
        if( ChkAttr( back.optdata.FirstIns, OC_ATTR_SHORT ) ) {
            /* HAS to be a short branch */
            next = NextIns( back, back.optdata.FirstIns );
            if( OcClass(next) == OC_JMP && CanReach(back, LabelOf(next), NULL, NULL) ) {
                /* we have:
                        Jcond   L1
                        JMP     L2
                   and L2 is reachable, but L1 is not.
                   Turn it into:
                        J~cond  L2
                        JMP     L1
                */
                JmpCond( back.optdata.FirstIns ) =
                                ReverseCondition( JmpCond( back.optdata.FirstIns ) );
                was_keep = TstStatus( back.optrel.Handle, KEEPLABEL );
                SetStatus( back.optrel.Handle, KEEPLABEL );
                ChgLblRef( back, back.optdata.FirstIns, LabelOf( next ) );
                ChgLblRef( back, next, back.optrel.Handle );
                if( !was_keep ) {
                    ClrStatus( back.optrel.Handle, KEEPLABEL );
                }
            } else {
                BigBranch( back, add, jmp );
            }
            SetShort(back);
        } else if( back.data.OptForSize > 50 && InRange(back) ) {
            /* use an old redirection label*/
            ChgLblRef( back, back.optdata.FirstIns, back.optrel.Handle->redirect );
            SetShort(back);
        }
    }
  OptEnd( back );
}
