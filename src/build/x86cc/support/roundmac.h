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
* Description:  Definition used for memory size and rounding
*
****************************************************************************/


#pragma once
#define _1K             0x00000400U
#define _32K            0x00008000U
#define _64K            0x00010000U


/*
 * these are for size which is any value
 */

/*
 * these are for size which is power of 2
 */
#define __ROUND_DOWN_SIZE(__x,__amt)                ((__x)&(~((__amt)-1)))
#define __ROUND_UP_SIZE(__x,__amt)                  (((__x)+((__amt)-1))&(~((__amt)-1)))

#define __ROUND_DOWN_SIZE_EVEN(__x)                 __ROUND_DOWN_SIZE(__x,2)
#define __ROUND_UP_SIZE_EVEN(__x)                   __ROUND_UP_SIZE(__x,2)





