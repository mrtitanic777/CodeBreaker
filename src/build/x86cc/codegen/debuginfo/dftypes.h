/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Prototypes for dftypes.c
*
****************************************************************************/


#pragma once
extern  dbg_type        DFFtnType( BackRun& back, const char *name, dbg_ftn_type tipe );
extern  dbg_type        DFScalar( BackRun& back, const char *name, cg_type tipe );
extern  dbg_type        DFScope( const char *name );
extern  char const      *DFScopeName( dbg_type scope );
extern  void            DFDumpName( BackRun& back, dbg_name name, dbg_type tipe );
extern  void            DFBackRefType( BackRun& back, dbg_name name, dbg_type tipe );
extern  dbg_type        DFCharBlock( BackRun& back, uint_32 len );
extern  dbg_type        DFCharBlockNamed( BackRun& back, const char *name, uint_32 len );
extern  dbg_type        DFIndCharBlock( BackRun& back, back_handle len, cg_type len_type, int off );
extern  dbg_type        DFLocCharBlock( BackRun& back, dbg_loc loc, cg_type len_type );
extern  dbg_type        DFFtnArray( BackRun& back, back_handle dims, cg_type lo_bound_tipe, cg_type num_elems_tipe, int off, dbg_type base );
extern  dbg_type        DFArray( BackRun& back, dbg_type idx, dbg_type base );
extern  dbg_type        DFIntArray( BackRun& back, uint_32 hi, dbg_type base );
extern  dbg_type        DFEndArray( BackRun& back, dbg_array ar );
extern  dbg_type        DFSubRange( int_32 lo, int_32 hi, dbg_type base );
extern  dbg_type        DFDereference( BackRun& back, cg_type ptr_type, dbg_type base );
extern  dbg_type        DFPtr( BackRun& back, cg_type ptr_type, dbg_type base );
extern  dbg_type        DFBasedPtr( BackRun& back, cg_type ptr_type, dbg_type base, dbg_loc loc_segment );
extern  void            DFBegStruct( BackRun& back, dbg_struct st );
extern  dbg_type        DFEndStruct( BackRun& back, dbg_struct st );
extern  dbg_type        DFEndEnum( BackRun& back, dbg_enum en );
extern  dbg_type        DFEndProc( BackRun& back, dbg_proc pr );
extern  dw_loc_id       DBGLoc2DFCont( BackRun& back, dbg_loc loc, dw_loc_id df_locid );
extern  dw_loc_handle   DBGLocBase2DF( BackRun& back, dbg_loc loc_seg );
extern  dw_loc_handle   DBGLoc2DF( BackRun& back, dbg_loc loc );

