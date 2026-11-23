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


#include "_cgstd.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "coderep.h"
#include "procdef.h"
#include "model.h"
#include "x86objd.h"
#include "zoiks.h"
#include "cgaux.h"
#include "dw.h"
#include "dwarf.h"
#include "dfdbg.h"
#include "dfsupp.h"
#include "objout.h"
#include "x86obj.h"
#include "regset.h"
#include "rgtbl.h"
#include "dbsyms.h"
#include "dfsyms.h"
#include "x86data.h"
#include "x86regn.h"
#include "cgprotos.h"
#include "backrun.hpp"


struct dbg_seg_names {
    const char      *seg_name;
    const char      *class_name;
};

static const struct dbg_seg_names DwarfSegNames[DW_DEBUG_MAX] = {
    { ".debug_info",        "DWARF" },
    { ".debug_pubnames",    "DWARF" },
    { ".debug_aranges",     "DWARF" },
    { ".debug_line",        "DWARF" },
    { ".debug_loc",         "DWARF" },
    { ".debug_abbrev",      "DWARF" },
    { ".debug_macinfo",     "DWARF" },
    { ".debug_str",         "DWARF" },
    { ".WATCOM_references", "DWARF" }
};


void    DFDefSegs( BackRun& back )
/***********************/
{
    if( IsModel( back, CGSW_GEN_DBG_LOCALS )
      || IsModel( back, CGSW_GEN_DBG_TYPES ) ) {
        dw_sectnum  i;

        for( i = static_cast<dw_sectnum>(0); i < DW_DEBUG_MAX; ++i ) {
            DFSetSection( back, i, NULL, DbgSegDef( back, DwarfSegNames[i].seg_name, DwarfSegNames[i].class_name, SEG_COMB_NORMAL + SEG_USE_32 ) );
        }
    } else if( IsModel( back, CGSW_GEN_DBG_NUMBERS ) ) {
        DFSetSection( back, DW_DEBUG_LINE, NULL, DbgSegDef( back, DwarfSegNames[DW_DEBUG_LINE].seg_name, DwarfSegNames[DW_DEBUG_LINE].class_name, SEG_COMB_NORMAL + SEG_USE_32 ) );
    }
}

void   DFOutReg( BackRun& back, dw_loc_id locid, name *reg )
/*******************************************/
{
    hw_reg_set  hw_reg;
    hw_reg_set  hw_low;
    dw_regs     dw_reg;

    hw_reg = reg->r.reg;
#if _TARGET & _TARG_8086
    if( HW_CEqual( hw_reg, HW_ABCD ) ) {
        DWLocReg( back, back.dfsyms.Client, locid, DW_REG_DX );
        DWLocPiece( back, back.dfsyms.Client, locid, WD );
        DWLocReg( back, back.dfsyms.Client, locid, DW_REG_CX );
        DWLocPiece( back, back.dfsyms.Client, locid, WD );
        DWLocReg( back, back.dfsyms.Client, locid, DW_REG_BX );
        DWLocPiece( back, back.dfsyms.Client, locid, WD );
        DWLocReg( back, back.dfsyms.Client, locid, DW_REG_AX );
        DWLocPiece( back, back.dfsyms.Client, locid, WD );
        return;
    }
#endif
#if _TARGET & _TARG_8086
    hw_low = Low32Reg( hw_reg );
#elif _TARGET & _TARG_80386
    hw_low = Low64Reg( hw_reg );
#endif
    if( HW_CEqual( hw_low, HW_EMPTY ) ) {
        dw_reg = RegTransDW( back, hw_reg );
        DWLocReg( back, back.dfsyms.Client, locid, dw_reg );
   } else {
        dw_reg =  RegTransDW( back, hw_low );
        DWLocReg( back, back.dfsyms.Client, locid, dw_reg );
        DWLocPiece( back, back.dfsyms.Client, locid, WD );
        HW_TurnOff( hw_reg, hw_low );
        dw_reg = RegTransDW( back, hw_reg );
        DWLocReg( back, back.dfsyms.Client, locid, dw_reg );
        DWLocPiece( back, back.dfsyms.Client, locid, WD );
    }
}

void   DFOutRegInd( BackRun& back, dw_loc_id locid, name *reg )
/**********************************************/
{
    dw_regs     dw_reg;

    dw_reg = RegTransDW( back, reg->r.reg );
    DWLocOp( back, back.dfsyms.Client, locid, DW_LOC_breg, dw_reg, 0 );
}

uint DFStkReg( BackRun& back )
/*******************/
{
    dw_regs    dw_reg;
    hw_reg_set stk;

    stk = StackReg();
    dw_reg = RegTransDW( back, stk );
    return( dw_reg );
}

uint DFDisplayReg( BackRun& back )
/***********************/
{
    dw_regs    dw_reg;
    hw_reg_set dsp;

    dsp = DisplayReg(back);
    dw_reg = RegTransDW( back, dsp );
    return( dw_reg );
}

void DFAbbrevRef( BackRun& back )
/**********************/
{
    back_handle bck;

    bck = BENewBack( back, NULL );
    BackImpPtr( back, ABBREV_NAME, bck, 0 );
    BEFreeBack( back, bck );
}

void DFAbbrevDef( BackRun& back )
/**********************/
{
    OutBckExport( back, ABBREV_NAME, true );
}
