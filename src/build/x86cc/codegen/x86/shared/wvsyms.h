/****************************************************************************
*
*                            Open Watcom Project
*
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
* Description:  Prototypes for wvsyms.c
*
****************************************************************************/


#pragma once
extern  void    WVInitDbgInfo( BackRun& back );
extern  void    WVFiniDbgInfo( void );
extern  void    WVObjInitDbgInfo( BackRun& back );
extern  void    WVObjFiniDbgInfo( void );
extern  void    WVGenStatic( BackRun& back, cg_sym_handle sym, dbg_loc loc );
extern  void    WVObjectPtr( BackRun& back, cg_type ptr_type );
extern  void    WVSetBase( BackRun& back );
extern  void    WVBlkEnd( BackRun& back, dbg_block *blk, offset lc );
extern  void    WVRtnEnd( BackRun& back, dbg_rtn *rtn, offset lc );
