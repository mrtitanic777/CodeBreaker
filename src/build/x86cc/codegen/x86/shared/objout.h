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


#pragma once
#include "import.h"

typedef void        *outputter_data;
typedef void        outputter_fn( BackRun&, const char *, outputter_data );

extern void         ObjInit( BackRun& back );
extern void         ObjFini( BackRun& back );
extern void         InitSegDefs( BackRun& back );
extern void         DefSegment( BackRun& back, segment_id segid, seg_attr attr, const char *str, uint align, bool use_16 );
extern bool         HaveCodeSeg( BackRun& back );
extern segment_id   AskCodeSeg( BackRun& back );
extern segment_id   AskAltCodeSeg( BackRun& back );
extern segment_id   AskBackSeg( BackRun& back );
extern segment_id   AskOP( BackRun& back );
extern segment_id   AskSegID( BackRun& back, void *hdl, cg_class class_ );
extern bool         AskSegIsBlank( BackRun& back, segment_id segid );
extern segment_id   ChangeOP( BackRun& back, segment_id segid );
extern void         SetOP( BackRun& back, segment_id segid );
extern void         FlushOP( BackRun& back, segment_id segid );
extern bool         NeedBaseSet( BackRun& back );
extern offset       AskLocation( BackRun& back );
extern long_offset  AskBigLocation( BackRun& back );
extern offset       AskMaxSize( BackRun& back );
extern long_offset  AskBigMaxSize( BackRun& back );
extern void         SetLocation( BackRun& back, offset loc );
extern void         SetBigLocation( BackRun& back, long_offset loc );
extern void         OutLabel( BackRun& back, label_handle label );
extern void         *InitPatches( BackRun& back );
extern void         AbsPatch( BackRun& back, abspatch_handle patch, offset lc );
extern void         TellObjNewProc( BackRun& back, cg_sym_handle proc );
extern void         IncLocation( BackRun& back, offset by );
extern bool         AskNameIsROM( BackRun& back, pointer h, cg_class c );
extern void         OutLineNum( BackRun& back, cg_linenum line, bool label_line );
extern bool         FreeObjCache( BackRun& back );
extern char         GetMemModel( BackRun& back );
extern void         ChkDbgSegSize( BackRun& back, offset max, bool typing );
extern bool         AskSegIsNear( BackRun& back, segment_id segid );
extern void         DoOutObjectName( BackRun& back, cg_sym_handle sym, outputter_fn *outputter, outputter_data data, import_kind kind );
extern bool         SymIsExported( BackRun& back, cg_sym_handle sym );
extern void         DoEmptyQueue( BackRun& back );
extern void         DoAlignment( BackRun& back, int len );
extern bool         CodeHasAbsPatch( oc_entry *code );

extern void         BackPtr( BackRun& back, back_handle bck, segment_id segid, offset plus, const type_def *tipe );
extern void         BackPtrBase( BackRun& back, back_handle bck, segment_id segid );
extern void         BackPtrBigOffset( BackRun& back, back_handle bck, segment_id segid, offset plus );
extern void         FEPtr( BackRun& back, cg_sym_handle sym, const type_def *tipe, offset plus );
extern void         FEPtrBaseOffset( BackRun& back, cg_sym_handle sym, offset plus );
extern void         FEPtrBase( BackRun& back, cg_sym_handle sym );

extern void         DataAlign( BackRun& back, uint_32 align );
extern void         DataBytes( BackRun& back, uint_32 len, const void *src );
extern void         DataShort( BackRun& back, uint_16 val );
extern void         DataLong( BackRun& back, uint_32 val );
extern void         IterBytes( BackRun& back, offset len, byte pat );
extern void         DataLabel( BackRun& back, label_handle lbl );
extern name         *GenFloat( BackRun& back, name *cons, type_class_def type_class );
