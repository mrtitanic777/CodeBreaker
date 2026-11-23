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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#pragma once
#include "regsetop.h"
#include "vergen.h"
#include "funits.h"

#define C           0x01u /*  constant */
#define R           0x02u /*  register */
#define M           0x04u /*  memory */
#define U           0x08u /*  unknown whether reg or memory */
#define ANY         (C | R | M | U)
#define OP__MUL     0x10u

#define RESULT_MUL  1u
#define OP1_MUL     OP__MUL
#define OP2_MUL     (OP1_MUL * OP__MUL)
#define OTHER_MUL   (OP2_MUL * OP__MUL)

#define BOTH_EQ     0
#define NO_CC       0
#define MASK_CC     (SETS_CC)

typedef enum {
    C_R         = C,
    R_R         = R,
    M_R         = M,
    U_R         = U,
    C_1         = (C * OP1_MUL),
    R_1         = (R * OP1_MUL),
    M_1         = (M * OP1_MUL),
    U_1         = (U * OP1_MUL),
    C_2         = (C * OP2_MUL),
    R_2         = (R * OP2_MUL),
    M_2         = (M * OP2_MUL),
    U_2         = (U * OP2_MUL),

    EQ_R1       = (R * OTHER_MUL),
    EQ_R2       = (C * OTHER_MUL),
    NONE        = (EQ_R1 | EQ_R2),
    SETS_SC     = (M * OTHER_MUL),
    PRESERVE    = (U * OTHER_MUL),
    SETS_CC     = (SETS_SC | PRESERVE),

} operand_types;

inline constexpr unsigned int BinPattern( unsigned int op1, unsigned int op2, unsigned int res, int match )
{
    return ( (op1)*OP1_MUL | (op2)*OP2_MUL | (res)*RESULT_MUL | (match) );
}
inline constexpr unsigned int BinPattern( unsigned int op1, unsigned int op2, unsigned int res, operand_types match )
{
    return ( (op1)*OP1_MUL | (op2)*OP2_MUL | (res)*RESULT_MUL | (match) );
}

inline constexpr unsigned int BinPatternSC( unsigned int op1, unsigned int op2, unsigned int res, operand_types match )
{
    return ( (op1)*OP1_MUL | (op2)*OP2_MUL | (res)*RESULT_MUL | (match) | SETS_SC );
}

inline constexpr unsigned int BinPatternPP( unsigned int op1, unsigned int op2, unsigned int res, operand_types match )
{
    return ( (op1)*OP1_MUL | (op2)*OP2_MUL | (res)*RESULT_MUL | (match) | PRESERVE );
}

inline constexpr unsigned int BinPatternCC( unsigned int op1, unsigned int op2, unsigned int res, operand_types match )
{
    return ( (op1)*OP1_MUL | (op2)*OP2_MUL | (res)*RESULT_MUL | (match) | SETS_CC );
}

inline constexpr unsigned int SidePattern( unsigned int op1, unsigned int op2 )
{
    return ( (op1)*OP1_MUL | (op2)*OP2_MUL | ANY*RESULT_MUL | NONE );
}

inline constexpr unsigned int SidePatternCC( unsigned int op1, unsigned int op2 )
{
    return ( (op1)*OP1_MUL | (op2)*OP2_MUL | ANY*RESULT_MUL | NONE | SETS_CC );
}



inline constexpr unsigned int UnPattern( unsigned int op, unsigned int res, operand_types match )
{
    return ( (op)*OP1_MUL | ANY*OP2_MUL | (res)*RESULT_MUL | (match) );
}



inline constexpr unsigned int UnPatternPP( unsigned int op, unsigned int res, operand_types match )
{
    return ( (op)*OP1_MUL | ANY*OP2_MUL | (res)*RESULT_MUL | (match) | PRESERVE );
}


#define _OE(t,v,r,g,f) { static_cast<operand_types>( t ), v, g, r, f }

typedef struct opcode_entry {
        operand_types           op_type;
        vertype                 verify;         /*  verification routine */
        gentype                 generate;       /*  generation routine */
        op_reg_set_index        reg_set;
        futype                  func_unit;      /*  functional unit info */
} opcode_entry;
