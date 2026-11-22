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


namespace {

/* the statics of this file, one set for each run of the compiler */
struct X86dbsupState {
    temp_buff *CurrBuff;
};

}  // namespace

static inline X86dbsupState& x86dbsup_state() { return cbrun_part<X86dbsupState>(); }


void    BuffStart( temp_buff *temp, uint def )
/********************************************/
{
    x86dbsup_state().CurrBuff = temp;
    temp->buff[1] = def;
    temp->index = 2;
    temp->fix_idx = 0;
}


void    BuffEnd( segment_id segid )
/*********************************/
{
    byte                *buff;
    const type_def      *ptr_type;
    dbg_patch           *dpatch;
    uint                i;
    uint                last;
    uint                size;

    ptr_type = TypeAddress( TY_LONG_POINTER );
    PUSH_OP( segid );
        x86dbsup_state().CurrBuff->buff[0] = x86dbsup_state().CurrBuff->index;
        buff = x86dbsup_state().CurrBuff->buff;
        last = 0;
        for( i = 0; i < x86dbsup_state().CurrBuff->fix_idx; ++i ) {
            size = x86dbsup_state().CurrBuff->fix[i].pos - last;
            last = x86dbsup_state().CurrBuff->fix[i].pos;
            DataBytes( size, buff );
            buff += size;
            switch( x86dbsup_state().CurrBuff->fix[i].type ) {
            case FIX_SYMBOL:
                FEPtr( static_cast<cg_sym_handle>(x86dbsup_state().CurrBuff->fix[i].p), ptr_type, 0 );
                last += 2 + WORD_SIZE;
                buff += 2 + WORD_SIZE;
                break;
            case FIX_BACKHANDLE:
                DoBigBckPtr( static_cast<back_handle>(x86dbsup_state().CurrBuff->fix[i].p), *(offset *)buff );
                last += 2 + WORD_SIZE;
                buff += 2 + WORD_SIZE;
                break;
            case FIX_FORWARD:
                dpatch = static_cast<dbg_patch *>(x86dbsup_state().CurrBuff->fix[i].p);
                dpatch->segid = segid;
                dpatch->offset = AskLocation();
                break;
            }
        }
        DataBytes( x86dbsup_state().CurrBuff->index - last, buff );
    POP_OP();
}


uint    BuffLoc( void )
/*********************/
{
    return( x86dbsup_state().CurrBuff->index );
}


void    BuffPatch( byte val, uint loc )
/*************************************/
{
    x86dbsup_state().CurrBuff->buff[loc] = val;
}


void    BuffByte( byte b )
/************************/
{
    if( x86dbsup_state().CurrBuff->index < DB_BUFF_SIZE ) {
        x86dbsup_state().CurrBuff->buff[x86dbsup_state().CurrBuff->index++] = b;
        if( x86dbsup_state().CurrBuff->index >= DB_BUFF_SIZE ) {
            _Zoiks( ZOIKS_005 );
        }
    }
}


void    BuffWord( uint w )
/************************/
{
    BuffByte( w & 0xff );
    BuffByte( w >> 8 );
}


void    BuffDWord( uint_32 w )
/****************************/
{
    BuffWord( w & 0xffff );
    BuffWord( w >> 16 );
}


void    BuffOffset( offset w )
/****************************/
{
#if _TARG_INTEGER == 16
    BuffWord( w );
#else
    BuffDWord( w );
#endif
}


void    BuffValue( uint_32 val, uint class_ )
/******************************************/
{
    switch( class_ ) {
    case 0:
        BuffByte( val );
        break;
    case 1:
        BuffWord( val );
        break;
    case 2:
        BuffWord( val & 0xffff );
        BuffWord( val >> 16 );
        break;
    }
}


void    BuffRelocatable( pointer ptr, fixup_kind type, offset off )
/*****************************************************************/
{
    x86dbsup_state().CurrBuff->fix[x86dbsup_state().CurrBuff->fix_idx].pos = x86dbsup_state().CurrBuff->index;
    x86dbsup_state().CurrBuff->fix[x86dbsup_state().CurrBuff->fix_idx].p = ptr;
    x86dbsup_state().CurrBuff->fix[x86dbsup_state().CurrBuff->fix_idx].type = type;
    if( type == FIX_FORWARD ) {
        BuffWord( 0 );
    } else {
        BuffOffset( off );
        x86dbsup_state().CurrBuff->index += 2;
    }
    x86dbsup_state().CurrBuff->fix_idx++;
}


void    BuffBack( pointer back, int off )
/***************************************/
{
    BuffRelocatable( back, FIX_BACKHANDLE, off );
}


void    BuffAddr( pointer sym )
/*****************************/
{
    BuffRelocatable( sym, FIX_SYMBOL, 0 );
}


void    BuffForward( dbg_patch *dpatch )
/**************************************/
{
    BuffRelocatable( dpatch, FIX_FORWARD, 0 );
}


void    BuffWSLString( const char *str )
/**************************************/
{
    while( *str != NULLCHAR ) {
        BuffByte( *str );
        ++str;
    }
}


void    BuffString( uint len, const char *str )
/*********************************************/
{
    while( len > 0 ) {
        BuffByte( *str );
        ++str;
        --len;
    }
}


void    BuffIndex( uint tipe )
/****************************/
{
    if( tipe < 0x80 ) {
        BuffByte( tipe );
    } else {
        BuffByte( 0x80 | (tipe >> 8) );
        BuffByte( tipe & 0xff );
    }
}
