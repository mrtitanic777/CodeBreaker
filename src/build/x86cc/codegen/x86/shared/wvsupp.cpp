/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
#include "procdef.h"
#include "model.h"
#include "wvdbg.h"
#include "targetdb.h"
#include "regset.h"
#include "rgtbl.h"
#include "x86dbsup.h"
#include "x86regn.h"
#include "wvsupp.h"
#include "cgprotos.h"
#include "backrun.hpp"


static  uint    MultiReg( BackRun& back, register_name *reg )
/********************************************/
{
    hw_reg_set  hw_reg;
    hw_reg_set  tmp;

    hw_reg = reg->reg;
#if _TARGET & _TARG_8086
    if( HW_CEqual( hw_reg, HW_ABCD ) ) {
        BuffByte( back, WV_REG_DX );
        BuffByte( back, WV_REG_CX );
        BuffByte( back, WV_REG_BX );
        BuffByte( back, WV_REG_AX );
        return( 4 );
    }
#endif
#if _TARGET & _TARG_8086
    hw_reg = Low32Reg( hw_reg );
#elif _TARGET & _TARG_80386
    hw_reg = Low64Reg( hw_reg );
#endif
    if( HW_CEqual( hw_reg, HW_EMPTY ) ) {
        BuffByte( back, RegTransWV( back, reg->reg ) );
        return( 1 );
    } else {
        BuffByte( back, RegTransWV( back, hw_reg ) );
        tmp = reg->reg;
        HW_TurnOff( tmp, hw_reg );
        BuffByte( back, RegTransWV( back, tmp ) );
        return( 2 );
    }
}

static  void    DoLocDump( BackRun& back, dbg_loc loc )
/**************************************/
{
    int         offset;
    int         reg;
    uint        patch;

    if( loc->next != NULL ) {
        DoLocDump( back, loc->next );
    }
    switch( loc->class_ & 0xf0 ) {
    case LOC_CONSTANT:
        if( loc->class_ == LOC_MEMORY ) {
            BuffByte( back, LOC_MEMORY );
            BuffAddr( back, loc->u.fe_sym );
        } else {
            offset = loc->u.val;
            if( offset >= -128 && offset <= 127 ) {
                BuffByte( back, LOC_CONST_1 );
                BuffByte( back, offset );
           } else if( offset >= -32768 && offset <= 32767 ) {
                BuffByte( back, LOC_CONST_2 );
                BuffWord( back, offset );
            } else {
                BuffByte( back, LOC_CONST_4 );
                BuffDWord( back, offset );
            }
        }
        break;
    case LOC_BP_OFFSET:
        offset = NewBase( back, loc->u.be_sym );
        if( offset >= -128 && offset <= 127 ) {
            BuffByte( back, LOC_BP_OFFSET + BP_BYTE );
            BuffByte( back, offset );
       } else if( offset >= -32768 && offset <= 32767 ) {
            BuffByte( back, LOC_BP_OFFSET + BP_WORD );
            BuffWord( back, offset );
        } else {
            BuffByte( back, LOC_BP_OFFSET + BP_DWORD );
            BuffDWord( back, offset );
        }
        break;
    case LOC_REG:
        reg = RegTransWV( back, loc->u.be_sym->r.reg );
        if( reg > 15 ) {
            patch = BuffLoc(back);
            BuffByte( back, 0 );
            BuffPatch( back, LOC_MULTI_REG | ( MultiReg( back, &loc->u.be_sym->r ) - 1 ), patch );
        } else {
            BuffByte( back, LOC_REG | reg );
        }
        break;
    case LOC_IND_REG:
        if( HW_CEqual( loc->u.be_sym->r.reg, HW_EMPTY ) ) {
            //NYI: structured return value on the stack. Have to do something
            //       suitable. For now, output a no location.
            BuffByte( back, 0 );
        } else {
            reg = RegTransWV( back, loc->u.be_sym->r.reg );
            if( reg < 0 ) { /* register not found */
                BuffByte( back, loc->class_ + 1 ); /* assumes ..._FAR is one greater*/
                MultiReg( back, &loc->u.be_sym->r );
            } else {
                BuffByte( back, loc->class_ );
                BuffByte( back, reg );
            }
        }
        break;
    case LOC_OPER:
        BuffByte( back, loc->class_ );
        if( loc->class_ == LOC_OPER+LOP_XCHG ) {
            BuffByte( back, loc->u.stk );
        }
        break;
    default:
        BuffByte( back, 0 ); /* No location*/
        break;
    }
}

void    LocDump( BackRun& back, dbg_loc loc )
/****************************/
{
    uint        patch;

    if( loc != NULL ) {
        if( loc->next != NULL ) {
            patch = BuffLoc(back);
            BuffByte( back, 0 );
            DoLocDump( back, loc );
            BuffPatch( back, LOC_EXPR_IND | (BuffLoc(back) - patch), patch );
        } else {
            DoLocDump( back, loc );
        }
    } else {
        BuffByte( back, LOC_OPER+LOP_NOP );
    }
    DBLocFini( back, loc );
}
