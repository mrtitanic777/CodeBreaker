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


#pragma once
extern  void    BuffStart( BackRun& back, temp_buff *temp, uint def );
extern  void    BuffEnd( BackRun& back, segment_id segid );
extern  uint    BuffLoc( BackRun& back );
extern  void    BuffPatch( BackRun& back, byte val, uint loc );
extern  void    BuffByte( BackRun& back, byte b );
extern  void    BuffWord( BackRun& back, uint w );
extern  void    BuffDWord( BackRun& back, uint_32 w );
extern  void    BuffOffset( BackRun& back, offset w );
extern  void    BuffValue( BackRun& back, uint_32 val, uint class_ );
extern  void    BuffRelocatable( BackRun& back, pointer ptr, fixup_kind type, offset off );
extern  void    BuffBack( BackRun& back, pointer back_handle, int off );
extern  void    BuffAddr( BackRun& back, pointer sym );
extern  void    BuffForward( BackRun& back, dbg_patch *dpatch );
extern  void    BuffWSLString( BackRun& back, const char *str );
extern  void    BuffString( BackRun& back, uint len, const char *str );
extern  void    BuffIndex( BackRun& back, uint tipe );
