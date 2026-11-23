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
* Description:  Type conversion folding, tree demotion.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "addrname.h"
#include "tree.h"
#include "treeconv.h"
#include "treeprot.h"
#include "treefold.h"
#include "zoiks.h"
#include "utils.h"
#include "makeaddr.h"
#include "procdef.h"
#include "typemap.h"
#include "backrun.hpp"


static  bool    DemoteTree( BackRun& back, tn name, const type_def *tipe, bool just_test ) {
/***************************************************************************/

    const type_def  *frum;
    bool            demote_this_node;
    bool            can_demote;

    frum = name->tipe;
    can_demote = false;
    demote_this_node = false;
    if( TypeClass( back, frum ) <= I4 ) {
        switch( name->class_ ) {
        case TN_UNARY: /* go left*/
            switch( name->u1.t.op ) {
            case O_UMINUS:
            case O_COMPLEMENT:
            case O_CONVERT:
            case O_ROUND:
                can_demote = DemoteTree( back, name->u.left, tipe, just_test );
                demote_this_node = true;
                break;
            case O_POINTS:
                can_demote = true;
                demote_this_node = true;
                break;
            }
            break;
        case TN_BINARY: /* go left, right*/
            switch( name->u1.t.op ) {
            case O_CONVERT:
                 /* Based pointer junk */
                 break;
            case O_DIV:
            case O_MOD:
                if( name->u.left->tipe->length > tipe->length ||
                    name->u1.t.rite->tipe->length > tipe->length )
                    break;
                /* fall throught */
            case O_TIMES:
            case O_AND:
            case O_OR:
            case O_XOR:
            case O_LSHIFT:
            case O_PLUS:
            case O_MINUS:
                if( name->tipe->refno == TY_HUGE_POINTER )
                    break;
                if( name->u.left->tipe->refno == TY_HUGE_POINTER )
                    break;
                if( name->u1.t.rite->tipe->refno == TY_HUGE_POINTER )
                    break;
                can_demote = DemoteTree( back, name->u.left, tipe, just_test );
                if( can_demote ) {
                    can_demote = DemoteTree( back, name->u1.t.rite, tipe, just_test );
                }
                demote_this_node = true;
                break;
            }
            break;
        case TN_COMMA:
            can_demote = DemoteTree( back, name->u1.t.rite, tipe, just_test );
            break;
        case TN_SIDE_EFFECT:
            can_demote = DemoteTree( back, name->u.left, tipe, just_test );
            break;
        case TN_FLOW_OUT:
        case TN_CONS:
            can_demote = true;
            demote_this_node = true;
            break;
        }
        if( !just_test && demote_this_node && frum->length > tipe->length ) {
            name->flags |= TF_DEMOTED;
            name->tipe = tipe;
        }
    }
    return( can_demote );
}


void    TGDemote( BackRun& back, tn name, const type_def *tipe )
/***********************************************/
{
    if( DemoteTree( back, name, tipe, true ) ) {
        DemoteTree( back, name, tipe, false );
    }
}


tn      FoldCnvRnd( BackRun& back, cg_op opcode, tn name, const type_def *to_tipe )
/**************************************************************/
{
    tn              new_tn;
    float_handle    cf;
    float_handle    junk;

    if( name->class_ == TN_CONS ) {
        if( name->tipe->refno == TY_DEFAULT ) {
            cf = CFCopy( back, &back.cfloat.cgh, name->u.name->c.u.cfval );
        } else {
            cf = CnvCFToType( back, name->u.name->c.u.cfval, name->tipe );
        }
        if( to_tipe->attr & TYPE_FLOAT ) {
            new_tn = TGConst( back, cf, to_tipe );
        } else if( opcode == O_CONVERT ) {
            junk = cf;
            cf = CFTrunc( back, &back.cfloat.cgh, cf );
            CFFree( back, &back.cfloat.cgh, junk );
            if( to_tipe->refno != TY_DEFAULT ) {
                junk = cf;
                cf = CnvCFToType( back, cf, to_tipe );
                CFFree( back, &back.cfloat.cgh, junk );
            }
            new_tn = TGConst( back, cf, to_tipe );
        } else if( opcode == O_ROUND ) {
            junk = cf;
            cf = CFRound( back, &back.cfloat.cgh, cf );
            CFFree( back, &back.cfloat.cgh, junk );
            if( to_tipe->refno != TY_DEFAULT ) {
                junk = cf;
                cf = CnvCFToType( back, cf, to_tipe );
                CFFree( back, &back.cfloat.cgh, junk );
            }
            new_tn = TGConst( back, cf, to_tipe );
        } else {
            new_tn = NULL;
            Zoiks( back, ZOIKS_078 ); /* Not supposed to get here, ever */
        }
        BurnTree( back, name );
    } else {
        TGDemote( back, name, to_tipe );
        new_tn = NULL;
    }
    return( new_tn );
}
