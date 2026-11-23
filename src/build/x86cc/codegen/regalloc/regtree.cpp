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
* Description:  Register tree manipulation.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "confldef.h"
#include "regset.h"
#include "freelist.h"
#include "zoiks.h"
#include "rgtbl.h"
#include "conflict.h"
#include "regtree.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct RegtreeState: regtree_state.h) */

}  // namespace



static bool HasSegRegs( reg_tree *tree )
{
    hw_reg_set  *regs;

    regs = tree->regs;
    if( regs != NULL ) {
        for( ; !HW_CEqual( *regs, HW_EMPTY ); ++regs ) {
            if( HW_COvlap( *regs, HW_SEGS ) ) {
                return( true );
            }
        }
    }
    return( false );
}


static  reg_tree        *AllocRegTree( BackRun& back )
/*******************************************/
{
    reg_tree    *tree;

    tree = static_cast<reg_tree *>(AllocFrl( back, &back.regtree.TreeFrl, sizeof( reg_tree ) ));
    return( tree );
}


static  void    FreeRegTree( BackRun& back, reg_tree *tree )
/*******************************************/
{
    FrlFreeSize( back, &back.regtree.TreeFrl, (pointer *)tree, sizeof( reg_tree ) );
}


static  hw_reg_set      *AllocRegSet( BackRun& back )
/******************************************/
{
    int         i;
    hw_reg_set  *regs;
    hw_reg_set  *curr;

    regs = static_cast<hw_reg_set *>(AllocFrl( back, &back.regtree.RegFrl, REGSET_SIZE ));
    for( curr = regs, i = REG_COUNT; i > 0; --i, ++curr ) {
        HW_CAsgn( *curr, HW_EMPTY );
    }
    return( regs );
}


static  void    FreeRegSet( BackRun& back, hw_reg_set *regs )
/********************************************/
{
    FrlFreeSize( back, &back.regtree.RegFrl, (pointer *)regs, REGSET_SIZE );
}


bool    RegTreeFrlFree( BackRun& back )
/****************************/
{
    bool        freed;

    freed = FrlFreeAll( back, &back.regtree.TreeFrl, sizeof( reg_tree ) );
    freed |= FrlFreeAll( back, &back.regtree.RegFrl, REGSET_SIZE );
    return( freed );
}


static  void    CheckBigPointer( BackRun& back, reg_tree *tree )
/***********************************************/
{
/* this routine is a sickening concession to the foolish design of the 8086
   (may all intel designers rot in hell forever, amen)

   This routine says, If the high part only wants word registers, then
   we'd better let the whole part have any pairs with segment
   registers in them. It also says don't let the LOW part of anything
   get a segment register
*/

    if( tree->lo != NULL && HasSegRegs( tree->lo ) ) {
        tree->lo->idx = RL_NUMBER_OF_SETS;
        FreeRegSet( back, tree->lo->regs );
        tree->lo->regs = NULL;
    }
    if( tree->hi != NULL && tree->hi->idx == RL_WORD && HasSegRegs( tree ) ) {
        tree->hi->idx = RL_NUMBER_OF_SETS;
        FreeRegSet( back, tree->hi->regs );
        tree->hi->regs = NULL;
    }
}


static  reg_tree        *NewTree( BackRun& back )
/**************************************/
{
    reg_tree    *tree;

    tree = AllocRegTree(back);
    tree->alt = NULL;
    tree->lo = NULL;
    tree->hi = NULL;
    tree->regs = NULL;
    tree->has_name = false;
    tree->idx = RL_NUMBER_OF_SETS;
    tree->temp = NULL;
    return( tree );
}


static  void    BuildPossible( BackRun& back, reg_tree *tree )
/*********************************************/
{
    const hw_reg_set    *src;
    hw_reg_set          *dst;

    if( tree != NULL ) {
        BuildPossible( back, tree->lo );
        BuildPossible( back, tree->hi );
        if( tree->idx != RL_NUMBER_OF_SETS ) {
            tree->regs = AllocRegSet(back);
            dst = tree->regs;
            for( src = RegSetList(back, tree->idx); !HW_CEqual( *src, HW_EMPTY ); ++src ) {
                *dst++ = *src;
            }
            *dst = *src;
        }
    }
}


static  void    SimpleTree( BackRun& back, conflict_node *conf )
/***********************************************/
{
    reg_tree    *tree;
    name        *temp;

    tree = NewTree(back);
    temp = conf->name;
    tree->temp = temp;
    tree->size = temp->n.size;
    tree->offset = temp->v.offset;
    tree->idx = conf->possible;
    BuildPossible( back, tree );
    conf->tree = tree;
}


static  reg_tree        *BuildTree( BackRun& back, name *alias, name *master,
                                    type_length offset, type_length size,
                                    conflict_node *conf )
/************************************************************************/
{
    reg_tree    *tree;
    name        *temp;
    bool        have_lo;
    bool        have_hi;
    type_length losize;
    type_length hisize;
    type_length midpoint;

    tree = NewTree(back);
    tree->offset = offset;
    tree->size = size;
    if( alias != NULL ) {
        tree->temp = alias;
        tree->has_name = true;
        alias->t.temp_flags |= VISITED;
        temp = alias->t.alias;
        while( temp != alias ) {
            if( temp->v.offset == offset && temp->n.size == size ) {
                tree->alt = temp; /* signed vs. unsigned*/
                temp->t.temp_flags |= VISITED;
                break;
            }
            temp = temp->t.alias;
        }
    }
    if( tree->alt == NULL ) {
        if( tree->temp != NULL ) {
            tree->idx = GetPossibleForTemp( conf, tree->temp );
        }
    } else {
        tree->idx = RegIntersect( GetPossibleForTemp( conf, tree->temp ),
                                  GetPossibleForTemp( conf, tree->alt  ) );
    }
    if( size == 6 ) { /* this is harmlessly specific to 80386 big pointers */
        losize = 4;
        hisize = 2;
    } else {
        losize = size / 2;
        hisize = size / 2;
    }
    midpoint = offset + losize;
    if( losize != 0 ) {
        have_lo = false;
        have_hi = false;
        temp = master->t.alias;
        while( temp != master ) {
            if( !have_lo && temp->v.offset == offset && temp->n.size == losize ) {
                tree->lo = BuildTree( back, temp, master, offset, losize, conf );
                have_lo = true;
            } else if( !have_hi && temp->v.offset == midpoint && temp->n.size == hisize ) {
                tree->hi = BuildTree( back, temp, master, midpoint, hisize, conf );
                have_hi = true;
            }
            temp = temp->t.alias;
        }
        if( !have_lo ) {
            tree->lo = BuildTree( back, NULL, master, offset, losize, conf );
        }
        if( !have_hi ) {
            tree->hi = BuildTree( back, NULL, master, midpoint, hisize, conf );
        }
        if( tree->hi->has_name ) {
            tree->has_name = true;
        }
        if( tree->lo->has_name ) {
            tree->has_name = true;
        }
    }
    return( tree );
}


void    BurnRegTree( BackRun& back, reg_tree *tree )
/***********************************/
{
    if( tree != NULL ) {
        BurnRegTree( back, tree->lo );
        BurnRegTree( back, tree->hi );
        if( tree->regs != NULL ) {
            FreeRegSet( back, tree->regs );
        }
        FreeRegTree( back, tree );
    }
}


static  void    TrimTree( BackRun& back, reg_tree *tree )
/****************************************/
{
    if( tree->lo != NULL ) {
        if( !tree->lo->has_name ) {
            BurnRegTree( back, tree->lo );
            tree->lo = NULL;
        } else {
            TrimTree( back, tree->lo );
        }
    }
    if( tree->hi != NULL ) {
        if( !tree->hi->has_name ) {
            BurnRegTree( back, tree->hi );
            tree->hi = NULL;
        } else {
            TrimTree( back, tree->hi );
        }
    }
}


static  reg_tree        *CheckTree( BackRun& back, reg_tree *tree )
/**************************************************/
{
    name        *alias;
    name        *temp;

    alias = tree->temp;
    temp = alias;
    for( ;; ) {
        temp = temp->t.alias;
        if( ( temp->t.temp_flags & VISITED ) == 0 ) {
            BurnRegTree( back, tree );
            tree = NULL;
            break;
        }
        if( temp == alias ) {
            break;
        }
    }
    return( tree );
}


static  void    CompressSets( reg_tree *tree )
/********************************************/
{
    hw_reg_set  *src;
    hw_reg_set  *dst;
    int         i;

    if( tree != NULL ) {
        CompressSets( tree->lo );
        CompressSets( tree->hi );
        if( tree->regs != NULL ) {
            dst = tree->regs;
            for( src = dst, i = REG_COUNT; i > 0; --i, ++src ) {
                if( !HW_CEqual( *src, HW_EMPTY ) ) {
                    *dst++ = *src;
                }
            }
            for( ; dst != src; ++dst ) {
                HW_CAsgn( *dst, HW_EMPTY );
            }
        }
    }
}


static  bool    PartIntersect( BackRun& back, reg_tree *part,
                               reg_tree *whole,
                               hw_reg_set (*rtn)( BackRun&, hw_reg_set ) )
/****************************************************************/
{
    bool        change;
    int         i;
    int         j;
    hw_reg_set  *src;
    hw_reg_set  *dst;
    hw_reg_set  curr;
    hw_reg_set  tmp;

    change = false;
    if( whole->idx != RL_NUMBER_OF_SETS ) {
        if( part->idx == RL_NUMBER_OF_SETS ) {

            /* build a register for part consisting of the Hi/Lo parts of whole*/

            part->regs = AllocRegSet(back);
            part->idx = RL_;
            src = whole->regs;
            dst = part->regs;
            for( i = REG_COUNT; i > 0; --i ) {
                *dst++ = rtn( back, *src++ );
            }
        }

        /* check that all Hi/Lo parts of whole are contained in part*/

        src = whole->regs;
        for( i = REG_COUNT; i > 0; --i ) {
            if( !HW_CEqual( *src, HW_EMPTY ) ) {
                curr = rtn( back, *src );
                if( !HW_CEqual( curr, HW_EMPTY ) ) {
                    dst = part->regs;
                    for( j = REG_COUNT; j > 0; --j ) {
                        if( HW_Equal( *dst, curr ) ) {
                            break;
                        }
                        ++dst;
                    }
                    if( j == 0 ) {
                        HW_CAsgn( *src, HW_EMPTY );
                        change = true;
                    }
                } else {
                    HW_CAsgn( *src, HW_EMPTY );
                    change = true;
                }
            }
            ++src;
        }

        /* check that each reg in part is a Hi/Lo part in whole*/

        src = part->regs;
        for( i = REG_COUNT; i > 0; --i ) {
            curr = *src;
            if( !HW_CEqual( curr, HW_EMPTY ) ) {
                dst = whole->regs;
                for( j = REG_COUNT; j > 0; --j ) {
                    if( !HW_CEqual( *dst, HW_EMPTY ) ) {
                        tmp = rtn( back, *dst );
                        if( HW_Equal( curr, tmp ) ) {
                            break;
                        }
                    }
                    ++dst;
                }
                if( j == 0 ) {
                    HW_CAsgn( *src, HW_EMPTY );
                    change = true;
                }
            }
            ++src;
        }

    }
    return( change );
}


static  bool    Intersect( BackRun& back, reg_tree *tree )
/*****************************************/
{
    bool        change;

    change = false;
    if( tree->hi != NULL ) {
        change |= PartIntersect( back, tree->hi, tree, &HighOffsetReg );
        change |= Intersect( back, tree->hi );
    }
    if( tree->lo != NULL ) {
        change |= PartIntersect( back, tree->lo, tree, &LowOffsetReg );
        change |= Intersect( back, tree->lo );
    }
    return( change );
}


void    BurnNameTree( BackRun& back, reg_tree *tree )
/************************************/
{
    BurnRegTree( back, tree );
}


void    RegTreeInit( BackRun& back )
/*************************/
{
    InitFrl( &back.regtree.RegFrl );
    InitFrl( &back.regtree.TreeFrl );
}


void    BuildNameTree( BackRun& back, conflict_node *conf )
/******************************************/
{
    name        *temp;
    reg_tree    *tree;

    temp = conf->name;
    if( temp->n.class_ == N_TEMP && temp->t.alias != temp ) {
        tree = BuildTree( back, temp, temp, temp->v.offset, temp->n.size, conf );
        tree = CheckTree( back, tree );
        if( tree != NULL ) {
            TrimTree( back, tree );
        }
    } else {
        tree = NULL;
    }
    conf->tree = tree;
}


void    BuildRegTree( BackRun& back, conflict_node *conf )
/*****************************************/
{
    name        *temp;
    reg_tree    *tree;

    temp = conf->name;
    if( temp->n.class_ == N_TEMP && temp->t.alias != temp ) {
        tree = BuildTree( back, temp, temp, temp->v.offset, temp->n.size, conf );
        tree = CheckTree( back, tree );
        if( tree != NULL ) {
            BuildPossible( back, tree );
            TrimTree( back, tree );
            CheckBigPointer( back, tree );
            while( Intersect( back, tree ) ) {
            }
            CompressSets( tree );
        }
        conf->tree = tree;
    } else {
        SimpleTree( back, conf );
    }
}
