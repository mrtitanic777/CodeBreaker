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
* Description:  C compiler global macro definition
*
****************************************************************************/


#pragma once

#if _CPU != 8086
    /*
     * enable Structured Exception Handling for all 32-bit targets
     */
    #define __SEH__
#endif

    #define FNAMECMPSTR             stricmp     /* for case insensitive file systems */

#define BUF_SIZE                    512

template <size_t N>
constexpr size_t LiteralLength( const char (&)[N] ) { return N - 1; }

#define PPOPERATOR_DEFINED          "defined"
#define PPOPERATOR_PRAGMA           "_Pragma"

inline bool IsPPOperatorDefined( const char *s ) { return (strcmp(s, PPOPERATOR_DEFINED) == 0); }
/* whether the token (its text and length) is the _Pragma operator, where the run's language has it (the bodies of these two are in frontrun.hpp) */
struct FrontRun;
CB_ALWAYS_INLINE bool IsPPOperatorPragma( const FrontRun& front, const char *s, size_t length );

template <class T, size_t N>
constexpr size_t ArraySize( T (&)[N] ) { return N; }

inline bool IsHugeData( const FrontRun& front );

#define DOS_EOF_CHAR                0x1A
