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
* Description:  WASM header file with global macros definition.
*
****************************************************************************/


#pragma once
#define ASMI86FAR

#define RC_ERROR                true
#define RC_OK                   false

#define MAX_TOKEN_COUNT         100     // there is no restriction for this number
#define MAX_LINE_LEN            512     // there is no restriction for this number
#define MAX_TOK_LEN             256
#define MAX_ID_LEN              247

/* max_ledata_threshold = 1024 - 6 for the header, -6 for space for fixups */
#define MAX_LEDATA_LEN          1024

#define NULLC                   '\0'

#define INVALID_IDX             (MAX_TOKEN_COUNT + 1)
inline bool IsValidIdx( unsigned char x ) { return ((x)<INVALID_IDX); }
inline bool IsInvalidIdx( unsigned char x ) { return ((x)>=INVALID_IDX); }
