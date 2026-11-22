/****************************************************************************
*
*                            Open Watcom Project
*
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
* Description:  Expression tree walker.
*
****************************************************************************/


#include "cvars.h"
#include "frontrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct TreewalkState: treewalk_state.h) */

}  // namespace



int WalkExprTree( FrontRun& front, TREEPTR p,
                void (*operand)( FrontRun&, TREEPTR ),
                void (*prefix_operator)( FrontRun&, TREEPTR ),
                void (*infix_operator)( FrontRun&, TREEPTR ),
                void (*postfix_operator)( FrontRun&, TREEPTR ) )
{
    TREEPTR     parent;
    TREEPTR     temp;

    front.treewalk.NodeTimeStamp = 0;
    parent = NULL;
    for( ;; ) {
        for( ;; ) {
            if( p->left == 0 && p->right == 0 ) break;
            ++front.treewalk.NodeTimeStamp;
            (*prefix_operator)( front, p );
            if( p->left == 0 ) break;
            temp = p;
            p = p->left;
            temp->left = parent;
            temp->visit = true;
            parent = temp;
        }
        if( parent == NULL ) {
            ++front.treewalk.NodeTimeStamp;
            (*prefix_operator)( front, p );
        } else if( p->right == NULL ) {
            ++front.treewalk.NodeTimeStamp;
            (*operand)( front, p );
            goto move_up_to_parent;
        }
        for( ;; ) {
            ++front.treewalk.NodeTimeStamp;
            (*infix_operator)( front, p );     /* - perform infix operation */
            if( p->right != NULL ) {    /* - if present */
                temp = p;
                p = p->right;           /* - - get right */
                temp->right = parent;
                parent = temp;
                break;                  /* - - get out to visit right */
            }                           /* - endif */
do_postfix_call:
            ++front.treewalk.NodeTimeStamp;
            (*postfix_operator)( front, p );   /* perform postfix operation */
            if( parent == NULL )        /* quit if at root node */
                return( 0 );
move_up_to_parent:
            if( parent->visit ) {
                parent->visit = false;
                temp = parent;
                parent = temp->left;
                temp->left = p;
                p = temp;
            } else {
                temp = parent;
                parent = temp->right;
                temp->right = p;
                p = temp;
                // already visited left and right
                goto do_postfix_call;
            }
        }
    }
}

void NoOp( FrontRun& front, TREEPTR node )
{
    /* unused parameters */ (void)node;
}
