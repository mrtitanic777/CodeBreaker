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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "dwpriv.h"
#include "dwinfo.h"
#include "dwabbrev.h"
#include "dwmem.h"
#include "dwdecl.h"
#include "dwrefer.h"
#include "dwcliuti.h"
#include "dwdie.h"
#include "backrun.hpp"


void DW_InitDIE( dw_client cli )
{
    cli->die.tree = NULL;
}


void DW_FiniDIE( dw_client cli )
{
    /* unused parameters */ (void)cli;

    _Assert( cli->die.tree == NULL );
}


void DW_StartChildren( BackRun& back, dw_client cli )
{
    die_tree        *new_;

    /* make a new entry in the tree */
    new_ = static_cast<die_tree *>(CLIAlloc( cli, sizeof( *new_ ) ));
    new_->parent = cli->die.tree;
    cli->die.tree = new_;

    /* prepare for sibling relocation */
    new_->sibling = 0;
}


static void doTheSiblingThing( BackRun& back, dw_client cli )
{
    dw_sect_offs    sibling;
    dw_sect_offs    offset;

    sibling = cli->die.tree->sibling;
    if( sibling ) {
        /* relocate previous sibling */
        offset = InfoSectionOffset( cli );
        CLISectionSeekOffset( cli, DW_DEBUG_INFO, sibling );
        DW_Info32( back, cli, offset );
        CLISectionSeekEnd( cli, DW_DEBUG_INFO );
    }
}


void DW_EndChildren( BackRun& back, dw_client cli )
{
    doTheSiblingThing( back, cli );

    /* move up a level in the tree */
    cli->die.tree = static_cast<die_tree *>(DW_FreeLink( back, cli, cli->die.tree ));
    CLISectionWriteZeros( back, cli, DW_DEBUG_INFO, 1 );
}


void DW_StartDIE( BackRun& back, dw_client cli, abbrev_code abbrev )
{
    int         haskids;
    unsigned    abbrevnum;

    doTheSiblingThing( back, cli );

    if( abbrev & AB_START_REF ) {
        DW_StartRef( back, cli );
    }

    haskids = (abbrev & AB_SIBLING) != 0;       // ab_sibling in "always on"
    abbrev &= ~AB_ALWAYS;                       // class of attributes

    /* emit the abbreviation for this DIE */
    abbrevnum = DW_MarkAbbrevAsUsed( back, cli, &abbrev );
    DW_InfoULEB128( back, cli, abbrevnum );

    /* AT_sibling reference */
    if( haskids ) {
        cli->die.tree->sibling = InfoSectionOffset( cli );
        /* reserve space (skip over) */
        InfoSkip( cli, sizeof( dw_sect_offs ) );
    } else {
        cli->die.tree->sibling = 0;
    }

    /* AT_decl_* */
    if( abbrev & AB_DECL ) {
        DW_EmitDecl( back, cli );
    }
}


void DW_EndDIE( dw_client cli )
{
    /* unused parameters */ (void)cli;
}
