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
#include "optmain.h"
#include "optutil.h"
#include "optcom.h"
#include "optins.h"
#include "optpull.h"
#include "optpush.h"
#include "optrel.h"
#include "backrun.hpp"


static  ins_entry       *Redirect( BackRun& back, ins_entry *, ins_entry * );


static  bool    LineLabel( ins_entry *label )
/*******************************************/
{
    /* unused parameters */ (void)label;
    return( false );
}


static  bool    CompressLabels( BackRun& back, ins_entry *label )
/************************************************/
{
    ins_entry   *other_label;
    bool        lbl_unique;

  OptBegin( back );
    lbl_unique = UniqueLabel( back, _Label( label ) );
    for( ;; ) {
        if( PrevClass( back, label ) != OC_LABEL )
            break;
        other_label = PrevIns( back, label );
        if( lbl_unique && UniqueLabel( back, _Label( other_label ) ) )
            break;
        if( LineLabel( label ) && LineLabel( other_label ) )
            break;
        AliasLabels( back, other_label, label );
        if( _Class( label ) == OC_DEAD ) {
            { OptEnd( back ); return( false ); }
        }
    }
    for( ;; ) {
        if( NextClass( back, label ) != OC_LABEL )
            break;
        other_label = NextIns( back, label );
        if( lbl_unique && UniqueLabel( back, _Label( other_label ) ) )
            break;
        if( LineLabel( label ) && LineLabel( other_label ) )
            break;
        AliasLabels( back, other_label, label );
        if( _Class( label ) == OC_DEAD ) {
            { OptEnd( back ); return( false ); }
        }
    }
    { OptEnd( back ); return( true ); }
}


static  bool    UnTangle1( BackRun& back, ins_entry *jmp, ins_entry **instr )
/************************************************************/
{
    ins_entry   *c_jmp;
    oc_class    cl;

    if( jmp == NULL )
        return( false );
    cl = _Class( jmp );
    if( (cl != OC_JCOND) && (cl != OC_JMP) )
        return( false );
    if( _Label( jmp ) == _Label( *instr ) ) {
        /* jump next*/
        *instr = DelInstr( back, jmp );
        return( true );
    }
    if( cl != OC_JMP )
        return( false );
    c_jmp = PrevIns( back, jmp );
    if( c_jmp == NULL )
        return( false );
    if( _Class( c_jmp ) != OC_JCOND )
        return( false );
    if( _Label( c_jmp ) == _Label( *instr ) && !_ChkAttr( jmp, OC_ATTR_FAR ) ) {
        /* conditional jump around a jump*/
        _JmpCond( c_jmp ) = ReverseCondition( _JmpCond( c_jmp ) );
        cl = _ClassInfo( jmp );
        _ClassInfo( jmp ) = static_cast<oc_class>(OC_DEAD_JMP);    /* to stop dead code removal*/
        ChgLblRef( back, c_jmp, _Label( jmp ) );
        _ClassInfo( jmp ) = cl;
        *instr = DelInstr( back, jmp );
        return( true );
    } else if( _Label( c_jmp ) == _Label( jmp ) ) {
        /* conditional jump followed by a jump to the same label*/
        DelInstr( back, c_jmp );
        return( true );
    }
    return( false );
}


static  bool    UnTangle2( BackRun& back, ins_entry *jmp, ins_entry **instr )
/************************************************************/
{
    oc_class    cl;
    ins_entry   *ins;

    if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION ) )
        return( false );
    if( jmp == NULL )
        return( false );
    cl = _Class( jmp );
    if( cl == OC_RET ) {
        /* label followed by a return*/
        return( RetAftrLbl( back, jmp ) );
    }
    if( cl != OC_JMP )
        return( false );
    if( _ChkAttr( jmp, OC_ATTR_FAR ) )
        return( false );
    if( _Label( *instr )->ins == NULL )
        return( false );
    if( _Label( jmp )->redirect == _Label( *instr ) )
        return( false );
    if( _Label( jmp ) == _Label( *instr ) )
        return( false );
    /* jump to jump*/
    *instr = Redirect( back, *instr, jmp );
    if( *instr == NULL )
        return( false );
    cl = PrevClass( back, *instr );
    if( !_TransferClass( cl ) )
        return( true );
    /* dead code*/
    ins = *instr;
    for( ;; ) {
        if( ins == NULL )
            break;
        if( _Class( ins ) == OC_LABEL )
            break;
        if( _Class( ins ) == OC_INFO ) {
            ins = ins->ins.next;
        } else {
            ins = DelInstr( back, ins );
        }
    }
    *instr = ins;
    return( true );
}

ins_entry       *Untangle( BackRun& back, ins_entry *instr )
/*******************************************/
{
    ins_entry   *jmp;
    bool        change;

  OptBegin( back );
    for( ;; ) {
        if( instr == NULL )
            break;
        if( _Class( instr ) != OC_LABEL )
            break;
        if( !CompressLabels( back, instr ) )
            break;
        change = false;
        jmp = PrevIns( back, instr );
        change |= UnTangle1( back, jmp, &instr );
        if( instr == NULL )
            break;
        if( _Class( instr ) != OC_LABEL )
            break;
        if( !CompressLabels( back, instr ) )
            break;
        jmp = NextIns( back, instr );
        change |= UnTangle2( back, jmp, &instr );
        if( !change ) {
            break;
        }
    }
    { OptEnd( back ); return( instr ); }
}


static  ins_entry       *Redirect( BackRun& back, ins_entry *l_ins, ins_entry *j_ins )
/*********************************************************************/
// Redirect all refs to l_ins to the target of j_ins
{
    ins_entry       *ref;
    label_handle    new_lbl;
    ins_entry       *next;
    ins_entry       *new_ins;

  OptBegin( back );
    new_lbl = _Label( j_ins );
    new_ins = new_lbl->ins;
    if( UniqueLabel( back, _Label( l_ins ) ) && UniqueLabel( back, new_lbl ) ) {
        { OptEnd( back ); return( NULL ); }
    }
    for( ref = _Label( l_ins )->refs; ref != NULL; ref = next ) {
        next = _LblRef( ref );
        ChgLblRef( back, ref, new_lbl );
    }
    if( new_ins != NULL && _Class( new_ins ) != OC_DEAD ) {
        Untangle( back, new_ins );
    }
    if( _Class( l_ins ) == OC_DEAD ) {
        /* got deleted*/
        { OptEnd( back ); return( ValidIns( back, l_ins ) ); }
    }
    if( new_ins == NULL
     || _Class( new_ins ) == OC_DEAD
     || _ChkAttr( l_ins, OC_ATTR_SHORT )
     || _TstStatus( _Label( l_ins ), REDIRECTION ) ) {
         { OptEnd( back ); return( NextIns( back, l_ins ) ); }
    } else {
         { OptEnd( back ); return( AliasLabels( back, l_ins, new_lbl->ins ) ); }
    }
}


void    OptPush( BackRun& back )
/*********************/
{
    ins_entry   *ins;

  OptBegin( back );
    ins = back.optdata.LastIns;
    for( ;; ) {
        back.optdata.InsDelete = false;
        switch( _Class( ins ) ) {
        case OC_INFO:
            MultiLineNums( back, ins );
            break;
        case OC_LABEL:
            Untangle( back, ins );
            if( !back.optdata.InsDelete ) {
                TraceCommon( back, ins );
            }
            break;
        case OC_CALL:
            if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
                CallRet( back, ins );
            }
            break;
        case OC_JMP:
            Untangle( back, PrevIns( back, ins ) );
            if( !back.optdata.InsDelete ) {
                ComCode( back, ins );
                JmpRet( back, ins );
            }
            break;
        case OC_RET:
            if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
                RetAftrLbl( back, ins );
                RetAftrCall( back, ins );
            }
            if( !back.optdata.InsDelete ) {
                ComTail( back, back.optdata.RetList, ins );
            }
            break;
        case OC_NORET:
            if( !back.optdata.InsDelete ) {
                ComTail( back, back.optdata.NoRetList, ins );
            }
            break;
        }
        if( !back.optdata.InsDelete )
            break;      /* nothing happened*/
        FreePendingDeletes(back);
        /* find the last interesting queue entry and try some more*/
        ins = back.optdata.LastIns;
        for( ;; ) {
            if( ins == NULL )
                { OptEnd( back ); return; }
            if( _Class( ins ) != OC_INFO )
                break;
            ins = ins->ins.prev;
        }
    }
  OptEnd( back );
}


void    OptPull( BackRun& back )
/*********************/
{
    oc_class    ins_class;

  OptBegin( back );
    for( ;; ) {
        back.optdata.InsDelete = false;
        ins_class = _Class( back.optdata.FirstIns );
        switch( ins_class ) {
        case OC_LABEL:
            Untangle( back, back.optdata.FirstIns );
            if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) && !back.optdata.InsDelete ) {
                CloneCode( back, _Label( back.optdata.FirstIns ) );
            }
            break;
        case OC_CALL:
            if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
                CallRet( back, back.optdata.FirstIns );
            }
            break;
        case OC_JMP:
            if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
                IsolatedCode( back, back.optdata.FirstIns );
                if( !back.optdata.InsDelete ) {
                    StraightenCode( back, back.optdata.FirstIns );
                    JmpRet( back, back.optdata.FirstIns );
                    if( !back.optdata.InsDelete ) {
                        CheckStraightenCode( back, NextIns( back, back.optdata.FirstIns ) );
                        if( !back.optdata.InsDelete ) {
                            CloneCode( back, _Label( back.optdata.FirstIns ) );
                        }
                    }
                }
            }
            break;
        case OC_JMPI:
            if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
                IsolatedCode( back, back.optdata.FirstIns );
                if( !back.optdata.InsDelete ) {
                    CheckStraightenCode( back, NextIns( back, back.optdata.FirstIns ) );
                }
            }
            break;
        case OC_JCOND:
            Untangle( back, NextIns( back, back.optdata.FirstIns ) );
            break;
        case OC_RET:
        case OC_NORET:
            if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
                IsolatedCode( back, back.optdata.FirstIns );
                if( !back.optdata.InsDelete ) {
                    CheckStraightenCode( back, NextIns( back, back.optdata.FirstIns ) );
                }
            }
            break;
        }
        FreePendingDeletes(back);
        if( back.optdata.FirstIns == NULL )
            { OptEnd( back ); return; }
        if( !back.optdata.InsDelete ) {
            break;
        }
    }
    switch( ins_class ) {
    case OC_LREF:
    case OC_CALL:
        _SetStatus( _Label( back.optdata.FirstIns ), KEEPLABEL );
        break;
    case OC_JCOND:
    case OC_JMP:
        if( !_ChkAttr( back.optdata.FirstIns, OC_ATTR_FAR ) ) {
            SetBranches(back);
        }
        _SetStatus( _Label( back.optdata.FirstIns ), KEEPLABEL );
        break;
    }
  OptEnd( back );
}
