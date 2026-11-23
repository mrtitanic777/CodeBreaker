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
* Description:  Special 386 pointer types.
*
****************************************************************************/


#include "_cgstd.h"
#include "types.h"
#include "model.h"
#include "backrun.hpp"


/*                      refno           length  attributes */
/*                      ======          ======  ========== */

//const type_def THugeCP = { TY_HUGE_CODE_PTR, 6,      TYPE_POINTER + TYPE_CODE };
const type_def TLongCP = { TY_LONG_CODE_PTR,  6,      static_cast<type_attr>(TYPE_POINTER + TYPE_CODE) };
const type_def TNearCP = { TY_NEAR_CODE_PTR,  4,      static_cast<type_attr>(TYPE_POINTER + TYPE_CODE) };
const type_def THugeP  = { TY_HUGE_POINTER,   6,      TYPE_POINTER };
const type_def TLongP  = { TY_LONG_POINTER,   6,      TYPE_POINTER };
const type_def TNearP  = { TY_NEAR_POINTER,   4,      TYPE_POINTER };

void    TargTypeInit( BackRun& back )
/**************************/
{
    TypeAlias( back, TY_UNSIGNED, TY_UINT_4 );
    TypeAlias( back, TY_INTEGER, TY_INT_4 );

    back.types.PTInteger = TypeAddress( back, TY_INT_4 );
    back.types.PTUnsigned = TypeAddress( back, TY_UINT_4 );

    if( IsTargetModel( back, CGSW_X86_BIG_CODE ) ) {
        TypeAlias( back, TY_CODE_PTR, TY_LONG_CODE_PTR );
        back.types.PTCodePointer = TypeAddress( back, TY_LONG_CODE_PTR );
    } else {
        TypeAlias( back, TY_CODE_PTR, TY_NEAR_CODE_PTR );
        back.types.PTCodePointer = TypeAddress( back, TY_NEAR_CODE_PTR );
    }
    if( IsTargetModel( back, CGSW_X86_BIG_DATA ) ) {
        TypeAlias( back, TY_POINTER, TY_LONG_POINTER );
        back.types.PTPointer = TypeAddress( back, TY_LONG_POINTER );
    } else {
        TypeAlias( back, TY_POINTER, TY_NEAR_POINTER );
        back.types.PTPointer = TypeAddress( back, TY_NEAR_POINTER );
    }
    TypeAlias( back, TY_NEAR_INTEGER, TY_INT_4 );
    TypeAlias( back, TY_LONG_INTEGER, TY_INT_4 );
    TypeAlias( back, TY_HUGE_INTEGER, TY_INT_4 );
}
