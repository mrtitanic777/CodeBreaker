/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2022 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Definition of code generator targets.
*
****************************************************************************/


#ifndef _TARG_INCLUDED
#define _TARG_INCLUDED

/*  target processors (_HOST) */
#define _TARG_8086      1
#define _TARG_80386     2
#define _TARG_PPC       8
#define _TARG_AXP       16
#define _TARG_MIPS      32

#define _TARGET_INTEL   (_TARGET & (_TARG_80386 | _TARG_8086))
#define _TARGET_RISC    (_TARGET & (_TARG_PPC | _TARG_AXP | _TARG_MIPS))

/*  target character sets (_CSET) */

/*  target word ordering (_MEMORY) */

#include "deftarg.h"

#if _TARGET & _TARG_8086
    #define _TARG_INTEGER       16
    #define WD                  U2
    #define SW                  I2
    #define WORD_SIZE           2
    #define REG_SIZE            2
    #define TY_WORD             TY_UINT_2
#elif _TARGET & _TARG_80386
    #define _TARG_INTEGER       32
    #define WD                  U4
    #define SW                  I4
    #define WORD_SIZE           4
    #define REG_SIZE            4
    #define TY_WORD             TY_UINT_4
#endif

#endif
