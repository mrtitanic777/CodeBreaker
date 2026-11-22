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
#include "coderep.h"
#include "cgmem.h"
#include "data.h"
#include "addrfold.h"
#include "makeins.h"
#include "display.h"
#include "objout.h"
#include "makeaddr.h"
#include "namelist.h"
#include "makeblk.h"
#include "rgtbl.h"
#include "backrun.hpp"


typedef struct frame_patch {
        struct frame_patch      *next;
        abspatch_handle         patch;
} frame_patch;

static  name    *DisplayField( BackRun& back, level_depth level )
/************************************************/
{
    name        *reg;

    reg = AllocRegName( back, DisplayReg(back) );
    return( AllocIndex( back, reg, NULL, (-2) * level, reg->n.type_class ) );
}


name    *MakeDisplay( BackRun& back, name *op, level_depth level )
/*************************************************/
{
    name        *temp;
    name        *reg;

    reg = AllocRegName( back, DisplayReg(back) );
    temp = AllocTemp( back, U2 );
    AddIns( back, MakeMove( back, DisplayField( back, level ), temp, reg->n.type_class ) );
    op = AllocIndex( back, temp, NULL, op->t.location, op->n.type_class );
    return( op );
}


void    BigGoto( BackRun& back, level_depth level )
/**********************************/
{
    name        *reg;

    if( level != 0 ) {
        reg = AllocRegName( back, DisplayReg(back) );
        AddIns( back, MakeMove( back, DisplayField( back, level ), reg, reg->n.type_class ) );
    }
}


void    BigLabel( BackRun& back )
/**********************/
{
    instruction *ins;
    name        *bp;
    name        *sp;

    if( back.data.CurrProc->lex_level > 0 ) {
        bp = AllocRegName( back, DisplayReg(back) );
        sp = AllocRegName( back, StackReg() );
        ins = MakeUnary( back, OP_LA,
                          AllocIndex( back, bp, NULL, -1, bp->n.type_class ),
                          sp, sp->n.type_class );
    } else {
        ins = MakeNop(back);
    }
    ins->zap = (register_name *)AllocRegName( back, AllCacheRegs(back) );
    ins->flags.u.nop_flags |= NOP_ZAP_INFO;
    AddIns( back, ins );
}

bool    AskIsFrameIndex( BackRun& back, name *op )
/*********************************/
{
    name        *bp;

    bp = AllocRegName( back, DisplayReg(back) );
    return( op == AllocIndex( back, bp, NULL, -1, bp->n.type_class ) );
}


abspatch_handle *NextFramePatch( BackRun& back )
/*************************************/
{
    frame_patch *temp;

    temp = static_cast<frame_patch *>(CGAlloc( back, sizeof( frame_patch ) ));
    temp->next = static_cast<struct frame_patch *>(back.data.CurrProc->frame_index);
    back.data.CurrProc->frame_index = temp;
    return( &temp->patch );
}


void    PatchBigLabels( BackRun& back, offset lc )
/*********************************/
{
    frame_patch *frame;
    frame_patch *next;

    for( frame = static_cast<frame_patch *>(back.data.CurrProc->frame_index); frame != NULL; frame = next ) {
        next = frame->next;
        AbsPatch( back, frame->patch, lc );
        CGFree( back, frame );
    }
    back.data.CurrProc->frame_index = NULL;
}


an      PassProcParm( BackRun& back, an rtn )
/****************************/
{
    name        *op;
    name        *reg;

    op = AllocTemp( back, XX );
    op->n.size = back.data.TypePtr->length + SizeDisplayReg();
    reg = AllocRegName( back, DisplayReg(back) );
    AddIns( back, MakeMove( back, GenIns( back, rtn ),
            TempOffset( back, op, 0, back.data.ClassPointer ),
            back.data.ClassPointer ) );
    AddIns( back, MakeMove( back, reg, TempOffset( back, op, back.data.TypePtr->length,
                                       reg->n.type_class ),
                      reg->n.type_class ) );
    return( AddrName( back, op, back.data.TypeProcParm ) );
}


void    SaveDisplay( BackRun& back, opcode_defs opcode )
/***************************************/
{
    name        *reg;

    reg = AllocRegName( back, DisplayReg(back) );
    AddIns( back, MakeUnary( back, opcode, reg, NULL, reg->n.type_class ) );
}


void    SetDisplay( BackRun& back, name *temp )
/******************************/
{
    name        *reg;

    reg = AllocRegName( back, DisplayReg(back) );
    AddIns( back, MakeMove( back, TempOffset( back, temp, back.data.TypePtr->length,
                                  reg->n.type_class ),
                      reg, reg->n.type_class ) );
}
