/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2016-2023 The Open Watcom Contributors. All Rights Reserved.
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
#define FAKE_NULL               ((back_handle)(pointer_uint)1)
inline unsigned long long PtrInt( bck_info *x ) { return ((pointer_uint)(x)); }
inline bool IsRealBack( bck_info *bck ) { return ((PtrInt(bck) & 1) == 0); }
inline bck_info *ToRealBack( bck_info *bck ) { return ((back_handle)(pointer_uint)(PtrInt(bck) & ~((pointer_uint)1))); }
inline bck_info *ToFakeBack( bck_info *bck ) { return ((back_handle)(pointer_uint)(PtrInt(bck) | 1)); }
