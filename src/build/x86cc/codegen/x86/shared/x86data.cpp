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
#include "system.h"
#include "jumps.h"
#include "zoiks.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "x86objd.h"
#include "data.h"
#include "utils.h"
#include "objout.h"
#include "namelist.h"
#include "objio.h"
#include "optmain.h"
#include "opttell.h"
#include "x86enc2.h"
#include "encode.h"
#include "x86obj.h"
#include "x86data.h"
#include "feprotos.h"
#include "backrun.hpp"


static void DoLblPtr( BackRun& back, label_handle lbl, segment_id segid, fix_class class_, offset plus );


void    DataAlign( BackRun& back, uint_32 align )
/********************************/
{
    offset      curr_loc;
    uint        modulus;

    curr_loc = AskLocation(back);
    modulus = curr_loc % align;
    if( modulus != 0 ) {
        if( !AskSegIsBlank( back, AskOP(back) ) ) {
            IterBytes( back, (offset)( align - modulus ), 0 );
        } else {
            IncLocation( back, align - modulus );
        }
    }
}

void    DataBytes( BackRun& back, unsigned len, const void *src )
/************************************************/
{
    if( len != 0 ) {
        TellOptimizerByPassed(back);
        SetUpObj( back, true );
        OutDBytes( back, len, static_cast<const byte *>(src) );
        TellByPassOver(back);
    }
}


void    DataShort( BackRun& back, uint_16 val )
/******************************/
{
    TellOptimizerByPassed(back);
    SetUpObj( back, true );
    OutDataShort( back, val );
    TellByPassOver(back);
}

void    DataLong( BackRun& back, uint_32 val )
/*****************************/
{
    TellOptimizerByPassed(back);
    SetUpObj( back, true );
    OutDataLong( back, val );
    TellByPassOver(back);
}


void    IterBytes( BackRun& back, offset len, byte pat )
/***************************************/
{
    TellOptimizerByPassed(back);
    SetUpObj( back, true );
    OutIBytes( back, pat, len );
    TellByPassOver(back);
}


void    DoBigBckPtr( BackRun& back, back_handle bck, offset off )
/*************************************************
 * Careful! Make sure a DGLabel has been done first!
 */
{
    TellOptimizerByPassed(back);
    DoLblPtr( back, bck->lbl, bck->segid, F_PTR, off );
    TellByPassOver(back);
}

static void     DoLblPtr( BackRun& back, label_handle lbl, segment_id segid, fix_class class_, offset plus )
/******************************************************************************************/
{
    SetUpObj( back, true );
    TellKeepLabel( back, lbl );
    OutReloc( back, segid, class_, false );
    OutLblPatch( back, lbl, class_, plus );
}


void    DoBigLblPtr( BackRun& back, cg_sym_handle sym )
/**************************************/
{
    TellOptimizerByPassed(back);
    DoLblPtr( back, FEBack( *back.front, sym )->lbl, FESegID( *back.front, sym ), F_PTR, 0 );
    TellByPassOver(back);
}


static  void    DoImpPtr( BackRun& back, cg_sym_handle sym, fix_class class_, offset plus )
/*************************************************************************/
{
    SetUpObj( back, true );
    OutImport( back, sym, class_, false );
    if( F_CLASS( class_ ) == F_BASE ) {
        _OutFarSeg( 0 );
    } else {
        _OutFarOff( plus );
        if( F_CLASS( class_ ) == F_PTR ) {
            _OutFarSeg( 0 );
        }
    }
}

void    BackImpPtr( BackRun& back, const char *nm, back_handle bck, offset plus )
/****************************************************************/
{
    fix_class const class_ = F_OFFSET;

    SetUpObj( back, true );
    OutBckImport( back, nm, bck, class_ );
    if( class_ == F_BASE ) {
        _OutFarSeg( 0 );
    } else {
        _OutFarOff( plus );
        if( class_ == F_PTR ) {
            _OutFarSeg( 0 );
        }
    }
}

void    OutLblPatch( BackRun& back, label_handle lbl, fix_class class_, offset plus )
/*******************************************************************/
{
    offset      val;

    val = AskAddress( back, lbl );
    if( val == ADDR_UNKNOWN ) {
        OutPatch( back, lbl, ADD_PATCH | _OFFSET_PATCH );
        val = 0;
    }
    if( F_CLASS( class_ ) == F_BASE ) {
        _OutFarSeg( 0 );
    } else {
        if( F_CLASS( class_ ) == F_BIG_OFFSET ) {
            OutDataLong( back, val + plus );
        } else {
            _OutFarOff( val + plus );
        }
        if( F_CLASS( class_ ) == F_PTR ) {
            _OutFarSeg( 0 );
        }
    }
}


void    FEPtr( BackRun& back, cg_sym_handle sym, const type_def *tipe, offset plus )
/*******************************************************************/
{
    fe_attr     attr;
    fix_class   class_;

    TellOptimizerByPassed(back);
    attr = FEAttr( *back.front, sym );
    if( (attr & FE_PROC)
      && _IsTargetModel( CGSW_X86_WINDOWS )
      && ((call_class_target)(pointer_uint)FindAuxInfoSym( sym, FEINF_CALL_CLASS_TARGET ) & FECALL_X86_FAR_CALL) ) {
        class_ = F_LDR_OFFSET;
    } else {
        class_ = F_OFFSET;
    }
    if( tipe->length != WORD_SIZE ) {
        class_ = F_PTR;
    }
    if( (attr & (FE_PROC | FE_DLLIMPORT)) == (FE_PROC | FE_DLLIMPORT)
      && (tipe->attr & TYPE_CODE) ) {
        class_ |= F_ALT_DLLIMP;
    }
    if( UseImportForm( back, attr ) ) {
        DoImpPtr( back, sym, class_, plus );
    } else {
        DoLblPtr( back, FEBack( *back.front, sym )->lbl, FESegID( *back.front, sym ), class_, plus );
    }
    TellByPassOver(back);
}

void    FEPtrBaseOffset( BackRun& back, cg_sym_handle sym,  offset plus )
/********************************************************/
{
    fe_attr     attr;

    TellOptimizerByPassed(back);
    attr = FEAttr( *back.front, sym );
    if( UseImportForm( back, attr ) ) {
        DoImpPtr( back, sym, F_PTR, plus );
    } else {
        DoLblPtr( back, FEBack( *back.front, sym )->lbl, FESegID( *back.front, sym ), F_PTR, plus );
    }
    TellByPassOver(back);
}

void    FEPtrBase( BackRun& back, cg_sym_handle sym )
/************************************/
{
    fe_attr     attr;

    TellOptimizerByPassed(back);
    attr = FEAttr( *back.front, sym );
    if( UseImportForm( back, attr ) ) {
        DoImpPtr( back, sym, F_BASE, 0 );
    } else {
        DoLblPtr( back, FEBack( *back.front, sym )->lbl, FESegID( *back.front, sym ), F_BASE, 0 );
    }
    TellByPassOver(back);
}


void    BackPtr( BackRun& back, back_handle bck, segment_id segid, offset plus, const type_def *tipe )
/*************************************************************************************/
{
    TellOptimizerByPassed(back);
    if( tipe->length != WORD_SIZE ) {
        DoLblPtr( back, bck->lbl, segid, F_PTR, plus );
    } else {
        DoLblPtr( back, bck->lbl, segid, F_OFFSET, plus );
    }
    TellByPassOver(back);
}

void    BackPtrBigOffset( BackRun& back, back_handle bck, segment_id segid, offset plus )
/************************************************************************/
{
    TellOptimizerByPassed(back);
    DoLblPtr( back, bck->lbl, segid, F_BIG_OFFSET, plus );
    TellByPassOver(back);
}

void    BackPtrBase( BackRun& back, back_handle bck, segment_id segid )
/******************************************************/
{
    TellOptimizerByPassed(back);
    DoLblPtr( back, bck->lbl, segid, F_BASE, 0 );
    TellByPassOver(back);
}

static  cg_class ConstDataClass( BackRun& back )
/*************************************/
{
    if( _IsTargetModel( CGSW_X86_CONST_IN_CODE ) ) {
        return( CG_CLB );
    } else if( _IsTargetModel( CGSW_X86_FLOATING_DS ) && _IsTargetModel( CGSW_X86_FLOATING_SS ) ) {
        return( CG_CLB );
    } else {
        return( CG_LBL );
    }
}

name    *GenConstData( BackRun& back, const void *buffer, type_class_def type_class )
/********************************************************************/
{
    segment_id          segid;
    cg_class            cgclass;
    name                *result;
    label_handle        label;
    type_length         size;

    TellOptimizerByPassed(back);
    cgclass = ConstDataClass(back);
    size = back.data.TypeClassSize[type_class];
    label = AskForLabel( back, NULL );
    if( cgclass == CG_CLB ) {
        segid = AskCodeSeg(back);
    } else {
        segid = AskBackSeg(back);
    }
    PUSH_OP( segid );
        SetUpObj( back, true );
        if( cgclass == CG_CLB ) {
            GenSelEntry( back, true );
            CodeLabel( back, label, size );
            CodeBytes( back, buffer, size );
            GenSelEntry( back, false );
        } else {
            DataAlign( back, size );
            OutLabel( back, label );
            DataBytes( back, size, buffer );
        }
    POP_OP();
    TellByPassOver(back);
    result = AllocMemory( back, label, 0, cgclass, type_class );
    result->v.usage |= USE_IN_ANOTHER_BLOCK;
    return( result );
}

name    *GenFloat( BackRun& back, name *cons, type_class_def type_class )
/********************************************************/
{
    constant_defn       *floatval;
    name                *result;

    floatval = GetFloat( back, cons, type_class );
    if( floatval->label == NULL ) {
        result = GenConstData( back, &floatval->buffer, type_class );
        floatval->label = result->v.symbol;
    } else {
        result = AllocMemory( back, floatval->label, 0, ConstDataClass(back), type_class );
    }
    return( result );
}


void    DataLabel( BackRun& back, label_handle lbl )
/***********************************/
{
    TellObjNewLabel( back, AskForLblSym( back, lbl ) );
    TellOptimizerByPassed(back);
    SetUpObj( back, true );
    OutLabel( back, lbl );
    TellByPassOver(back);
}
