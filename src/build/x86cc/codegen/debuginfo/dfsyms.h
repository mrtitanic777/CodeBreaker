/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2018 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Prototypes for dfsyms.c
*
****************************************************************************/


#ifndef CB_STATE_DFSYMS_H
#define CB_STATE_DFSYMS_H

/* the variables of dfsyms.h, one set for each run of the compiler (run/cbrun.hpp) */
struct DfsymsState {
    dw_client Client;
};

#endif

extern  void    DFInitDbgInfo( BackRun& back );
extern  void    DFFiniDbgInfo( void );
extern  void    DFObjInitDbgInfo( BackRun& back );
extern  void    DFObjFiniDbgInfo( BackRun& back, offset );
extern  void    DFObjLineInitDbgInfo( BackRun& back );
extern  void    DFObjLineFiniDbgInfo( BackRun& back );
extern  void    DFGenStatic( BackRun& back, cg_sym_handle sym, dbg_loc loc );
extern  void    DFTypedef( BackRun& back, const char *nm, dbg_type tipe );
extern  void    DFProEnd( BackRun& back, dbg_rtn *rtn, offset lc );
extern  void    DFBlkBeg( BackRun& back, dbg_block *blk, offset lc );
extern  void    DFBlkEnd( BackRun& back, dbg_block *blk, offset lc );
extern  void    DFEpiBeg( BackRun& back, dbg_rtn *rtn, offset lc );
extern  void    DFRtnEnd( BackRun& back, dbg_rtn *rtn, offset lc );
extern  void    DFBegCCU( BackRun& back, segment_id code_segid, dw_sym_handle dbg_pch );
extern  void    DFLineNum( BackRun& back, cue_state *, offset );
extern  void    DFSegRange( BackRun& back );
extern  void    DFSymRange( BackRun& back, cg_sym_handle, offset );
extern  void    DFSetSection( BackRun& back, dw_sectnum  sect, back_handle bck, segment_id segid );
