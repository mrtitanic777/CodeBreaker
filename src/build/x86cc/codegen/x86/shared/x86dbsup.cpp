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
#include "coderep.h"
#include "procdef.h"
#include "model.h"
#include "zoiks.h"
#include "types.h"
#include "x86objd.h"
#include "objout.h"
#include "x86data.h"
#include "x86dbsup.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct X86dbsupState: x86dbsup_state.h) */

}  // namespace



void    BuffStart( BackRun& back, temp_buff *temp, uint def )
/********************************************/
{
    back.x86dbsup.CurrBuff = temp;
    temp->buff[1] = def;
    temp->index = 2;
    temp->fix_idx = 0;
}


void    BuffEnd( BackRun& back, segment_id segid )
/*********************************/
{
    byte                *buff;
    const type_def      *ptr_type;
    dbg_patch           *dpatch;
    uint                i;
    uint                last;
    uint                size;

    ptr_type = TypeAddress( back, TY_LONG_POINTER );
    { segment_id old_segid = ChangeOP( back, segid );
        back.x86dbsup.CurrBuff->buff[0] = back.x86dbsup.CurrBuff->index;
        buff = back.x86dbsup.CurrBuff->buff;
        last = 0;
        for( i = 0; i < back.x86dbsup.CurrBuff->fix_idx; ++i ) {
            size = back.x86dbsup.CurrBuff->fix[i].pos - last;
            last = back.x86dbsup.CurrBuff->fix[i].pos;
            DataBytes( back, size, buff );
            buff += size;
            switch( back.x86dbsup.CurrBuff->fix[i].type ) {
            case FIX_SYMBOL:
                FEPtr( back, static_cast<cg_sym_handle>(back.x86dbsup.CurrBuff->fix[i].p), ptr_type, 0 );
                last += 2 + WORD_SIZE;
                buff += 2 + WORD_SIZE;
                break;
            case FIX_BACKHANDLE:
                DoBigBckPtr( back, static_cast<back_handle>(back.x86dbsup.CurrBuff->fix[i].p), *reinterpret_cast<offset *>(buff) );
                last += 2 + WORD_SIZE;
                buff += 2 + WORD_SIZE;
                break;
            case FIX_FORWARD:
                dpatch = static_cast<dbg_patch *>(back.x86dbsup.CurrBuff->fix[i].p);
                dpatch->segid = segid;
                dpatch->offset = AskLocation(back);
                break;
            }
        }
        DataBytes( back, back.x86dbsup.CurrBuff->index - last, buff );
    SetOP( back, old_segid ); }
}


uint    BuffLoc( BackRun& back )
/*********************/
{
    return( back.x86dbsup.CurrBuff->index );
}


void    BuffPatch( BackRun& back, byte val, uint loc )
/*************************************/
{
    back.x86dbsup.CurrBuff->buff[loc] = val;
}


void    BuffByte( BackRun& back, byte b )
/************************/
{
    if( back.x86dbsup.CurrBuff->index < DB_BUFF_SIZE ) {
        back.x86dbsup.CurrBuff->buff[back.x86dbsup.CurrBuff->index++] = b;
        if( back.x86dbsup.CurrBuff->index >= DB_BUFF_SIZE ) {
            Zoiks( back, ZOIKS_005 );
        }
    }
}


void    BuffWord( BackRun& back, uint w )
/************************/
{
    BuffByte( back, w & 0xff );
    BuffByte( back, w >> 8 );
}


void    BuffDWord( BackRun& back, uint_32 w )
/****************************/
{
    BuffWord( back, w & 0xffff );
    BuffWord( back, w >> 16 );
}


void    BuffOffset( BackRun& back, offset w )
/****************************/
{
#if _TARG_INTEGER == 16
    BuffWord( back, w );
#else
    BuffDWord( back, w );
#endif
}


void    BuffValue( BackRun& back, uint_32 val, uint class_ )
/******************************************/
{
    switch( class_ ) {
    case 0:
        BuffByte( back, val );
        break;
    case 1:
        BuffWord( back, val );
        break;
    case 2:
        BuffWord( back, val & 0xffff );
        BuffWord( back, val >> 16 );
        break;
    }
}


void    BuffRelocatable( BackRun& back, pointer ptr, fixup_kind type, offset off )
/*****************************************************************/
{
    back.x86dbsup.CurrBuff->fix[back.x86dbsup.CurrBuff->fix_idx].pos = back.x86dbsup.CurrBuff->index;
    back.x86dbsup.CurrBuff->fix[back.x86dbsup.CurrBuff->fix_idx].p = ptr;
    back.x86dbsup.CurrBuff->fix[back.x86dbsup.CurrBuff->fix_idx].type = type;
    if( type == FIX_FORWARD ) {
        BuffWord( back, 0 );
    } else {
        BuffOffset( back, off );
        back.x86dbsup.CurrBuff->index += 2;
    }
    back.x86dbsup.CurrBuff->fix_idx++;
}


void    BuffBack( BackRun& back, pointer back_handle, int off )
/***************************************/
{
    BuffRelocatable( back, back_handle, FIX_BACKHANDLE, off );
}


void    BuffAddr( BackRun& back, pointer sym )
/*****************************/
{
    BuffRelocatable( back, sym, FIX_SYMBOL, 0 );
}


void    BuffForward( BackRun& back, dbg_patch *dpatch )
/**************************************/
{
    BuffRelocatable( back, dpatch, FIX_FORWARD, 0 );
}


void    BuffWSLString( BackRun& back, const char *str )
/**************************************/
{
    while( *str != NULLCHAR ) {
        BuffByte( back, *str );
        ++str;
    }
}


void    BuffString( BackRun& back, uint len, const char *str )
/*********************************************/
{
    while( len > 0 ) {
        BuffByte( back, *str );
        ++str;
        --len;
    }
}


void    BuffIndex( BackRun& back, uint tipe )
/****************************/
{
    if( tipe < 0x80 ) {
        BuffByte( back, tipe );
    } else {
        BuffByte( back, 0x80 | (tipe >> 8) );
        BuffByte( back, tipe & 0xff );
    }
}
