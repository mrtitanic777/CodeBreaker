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


namespace {

/* the statics of this file, one set for each run of the compiler */
struct OptrelState {
    label_handle Handle;
};

}  // namespace

static inline OptrelState& optrel_state() { return cbrun_part<OptrelState>(); }


static  bool_    Jmp_to_lbl( ins_entry *instr )
/********************************************/
{
  optbegin
    if( _Class( instr ) != OC_JMP )
        optreturn( false );
    if( _Label( instr ) != optrel_state().Handle )
        optreturn( false );
    optreturn( true );
}


static  bool_    CanReach( label_handle lbl, ins_entry **add_ptr,
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
    if( _TstStatus( lbl, UNREACHABLE ) ) {
        /* can't do anything with it */
    } else if( lbl->lbl.address == ADDR_UNKNOWN ) {
        if( _TstStatus( lbl, SHORTREACH ) )
            return( true );
        obj_len = OptInsSize( OC_JMP, OC_DEST_SHORT );
        lbl_ins = optrel_state().Handle->ins;
        for( instr = NextIns( optdata_state().FirstIns ); instr != NULL; instr = NextIns( instr ) ) {
            obj_len += _ObjLen( instr );
            if( obj_len > MAX_SHORT_FWD )
                break;
            if( Jmp_to_lbl( instr ) ) {
                jmp = instr;
            } else {
                cl = _Class( instr );
                if( _TransferClass( cl ) ) {
                    add = instr;
                }
            }
            if( instr == lbl_ins ) {
                return( true );
            }
        }
    } else if( (AskLocation() - lbl->lbl.address) <= MAX_SHORT_BWD ) {
        return( true );
    }
    if( add_ptr != NULL )
        *add_ptr = add;
    if( jmp_ptr != NULL )
        *jmp_ptr = jmp;
    return( false );
}


static  void    HndlRedirect( label_handle new_lbl )
/**************************************************/
{
    label_handle    redir;

  optbegin
    redir = optrel_state().Handle->redirect;
    if( redir != NULL && redir != new_lbl ) {
        _ClrStatus( redir, REDIRECTION );
        TryScrapLabel( redir );
    }
    optrel_state().Handle->redirect = new_lbl;
    if( new_lbl != NULL ) {
        _SetStatus( new_lbl, REDIRECTION );
    }
  optend
}


static  bool_    InRange( void )
/*****************************/
/* Check if redirection is still within range*/
{
    label_handle    jmp_lbl;

  optbegin
    if( _IsModel( CGSW_GEN_NO_OPTIMIZATION ) )
        optreturn( false );
    jmp_lbl = optrel_state().Handle->redirect;
    /* we don't have a redirection for this label*/
    if( jmp_lbl == NULL )
        optreturn( false );
    /* forward jump: must still be in range*/
    if( jmp_lbl->lbl.address == ADDR_UNKNOWN )
        optreturn( true );
    /* can't redirect the redirection jump to itself*/
    if( jmp_lbl->lbl.address == AskLocation() )
        optreturn( false );
    /* can still reach old redirection with short backward jump*/
    if( (AskLocation() - jmp_lbl->lbl.address) <= MAX_SHORT_BWD )
        optreturn( true );
    /* can't get at old redirection any more*/
    HndlRedirect( NULL );
    optreturn( false );
}


static  void    BigBranch( ins_entry *add, ins_entry *jmp )
/**********************************************************
    We can't get to where we want to go, have to redirect
*/
{
  optbegin
    if( jmp != NULL && _IsntModel( CGSW_GEN_NO_OPTIMIZATION ) ) {
        /* jump to a jump that's going where we want*/
        HndlRedirect( AddNewLabel( PrevIns( jmp ), 0 ) );
        ChgLblRef( optdata_state().FirstIns, optrel_state().Handle->redirect );
    } else if( InRange() ) {
        /* use old redirection label*/
        ChgLblRef( optdata_state().FirstIns, optrel_state().Handle->redirect );
    } else if( add != NULL && _IsntModel( CGSW_GEN_NO_OPTIMIZATION ) ) {
        /* add a jump that's in range*/
        AddNewJump( add, optrel_state().Handle );
        _Savings( OPT_JUMPS, -_ObjLen( NextIns( add ) ) );
        HndlRedirect( AddNewLabel( add, 0 ) );
        ChgLblRef( optdata_state().FirstIns, optrel_state().Handle->redirect );
    } else {
        /* reverse the branch*/
        _JmpCond( optdata_state().FirstIns ) = ReverseCondition( _JmpCond( optdata_state().FirstIns ) );
        AddNewJump( optdata_state().FirstIns, optrel_state().Handle );
        ChgLblRef( optdata_state().FirstIns, AddNewLabel( NextIns( optdata_state().FirstIns ), 0 ) );
        HndlRedirect( AddNewLabel( optdata_state().FirstIns, 0 ) );
    }
  optend
}


static  void    SetShort( void )
/******************************/
{
    ins_entry   *l_ins;
    obj_length  size;
    bool_        floating;

  optbegin
    floating = false;
    if( _ChkAttr( optdata_state().FirstIns, OC_ATTR_FLOAT ) ) {
        floating = true;
    }
    size = OptInsSize( static_cast<oc_class>(_Class( optdata_state().FirstIns )), OC_DEST_SHORT );
    if( _Class( optdata_state().FirstIns ) == OC_JMP ) {
        _Savings( OPT_JUMPS, _ObjLen( FirstIns ) - size );
        _ObjLen( optdata_state().FirstIns ) = size;
        _ResetClass( optdata_state().FirstIns, OC_JMP );
        if( floating ) {
            _SetAttr( optdata_state().FirstIns, OC_ATTR_FLOAT );
        }
    } else {
        _Savings( OPT_JCONDS, _ObjLen( FirstIns ) - size );
        _ObjLen( optdata_state().FirstIns ) = size;
        _ResetClass( optdata_state().FirstIns, OC_JCOND );
        if( floating ) {
            _SetAttr( optdata_state().FirstIns, OC_ATTR_FLOAT );
        }
    }
    l_ins = _Label( optdata_state().FirstIns )->ins;
    if( l_ins != NULL ) {
        _SetAttr( l_ins, OC_ATTR_SHORT );
    }
  optend
}


void    SetBranches( void )
/*************************/
/* Check whether the actual label itself can be targeted*/
{
  ins_entry     *add;
  ins_entry     *jmp;
  ins_entry     *next;
  bool_          was_keep;

  optbegin
    optrel_state().Handle = _Label( optdata_state().FirstIns );
    if( CanReach( optrel_state().Handle, &add, &jmp ) ) {
        _SetStatus( optrel_state().Handle, SHORTREACH );
        HndlRedirect( NULL );
        SetShort();
    } else {
        if( _ChkAttr( optdata_state().FirstIns, OC_ATTR_SHORT ) ) {
            /* HAS to be a short branch */
            next = NextIns( optdata_state().FirstIns );
            if( _Class(next) == OC_JMP && CanReach(_Label(next), NULL, NULL) ) {
                /* we have:
                        Jcond   L1
                        JMP     L2
                   and L2 is reachable, but L1 is not.
                   Turn it into:
                        J~cond  L2
                        JMP     L1
                */
                _JmpCond( optdata_state().FirstIns ) =
                                ReverseCondition( _JmpCond( optdata_state().FirstIns ) );
                was_keep = _TstStatus( optrel_state().Handle, KEEPLABEL );
                _SetStatus( optrel_state().Handle, KEEPLABEL );
                ChgLblRef( optdata_state().FirstIns, _Label( next ) );
                ChgLblRef( next, optrel_state().Handle );
                if( !was_keep ) {
                    _ClrStatus( optrel_state().Handle, KEEPLABEL );
                }
            } else {
                BigBranch( add, jmp );
            }
            SetShort();
        } else if( data_state().OptForSize > 50 && InRange() ) {
            /* use an old redirection label*/
            ChgLblRef( optdata_state().FirstIns, optrel_state().Handle->redirect );
            SetShort();
        }
    }
  optend
}
