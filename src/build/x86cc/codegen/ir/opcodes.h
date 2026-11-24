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
* Description:  Code generator opcodes and their classification.
*
****************************************************************************/


#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

typedef enum {
    #define PICK(e,i,d1,d2,ot,pnum,attr)  i,
    #define ONLY_INTERNAL_CGOPS
    #include "cgops.h"
    #undef ONLY_INTERNAL_CGOPS
    #undef PICK
} opcode_defs;

#define FIRST_OP                OP_NOP
#define FIRST_CSE_OP            OP_ADD
#define   FIRST_BINARY_OP       OP_ADD
#define     FIRST_BIT_OP        OP_AND
#define     LAST_BIT_OP         OP_XOR
#define   FIRST_IFUNC           OP_POW
#define     FIRST_BINARY_IFUNC  OP_POW
#define     LAST_BINARY_IFUNC   OP_FMOD
#define   LAST_BINARY_OP        OP_FMOD
#define      FIRST_UNARY_IFUNC  OP_LOG
#define      LAST_UNARY_IFUNC   OP_TANH
#define   LAST_IFUNC            OP_TANH
#define LAST_CSE_OP             OP_CONVERT
#define  FIRST_OP_WITH_LABEL    OP_BIT_TEST_TRUE
#define   FIRST_CONDITION       OP_BIT_TEST_TRUE
#define    FIRST_COMPARISON     OP_CMP_EQUAL
#define    LAST_COMPARISON      OP_CMP_GREATER_EQUAL
#define   LAST_CONDITION        OP_CMP_GREATER_EQUAL
#define  FIRST_SET_OP           OP_SET_EQUAL
#define  LAST_SET_OP            OP_SET_GREATER_EQUAL
#define LAST_OP                 OP_BLOCK


inline bool OpIsBinary( opcode_defs op ) { return ( (op) >= FIRST_BINARY_OP && (op) <= LAST_BINARY_OP ); }



inline bool OpIsCSE( opcode_defs op ) { return ( (op) >= FIRST_CSE_OP && (op) <= LAST_CSE_OP ); }

inline bool OpIsBit( opcode_defs op ) { return ( (op) >= FIRST_BIT_OP && (op) <= LAST_BIT_OP ); }

inline bool OpIsIFunc( opcode_defs op ) { return ( (op) >= FIRST_IFUNC && (op) <= LAST_IFUNC ); }



inline bool OpIsCompare( opcode_defs op ) { return ( (op) >= FIRST_COMPARISON && (op) <= LAST_COMPARISON ); }

inline bool OpIsCondition( opcode_defs op ) { return ( (op) >= FIRST_CONDITION && (op) <= LAST_CONDITION ); }

inline bool OpIsJump( opcode_defs op ) { return ( OpIsCondition( op ) || (op) == OP_SELECT ); }

inline bool OpIsCall( opcode_defs op ) { return ( (op) == OP_CALL || (op) == OP_CALL_INDIRECT ); }

inline bool OpCommutes( opcode_defs op )
{
    return ( ( (op) == OP_ADD ) ||
        ( (op) == OP_MUL ) ||
        ( (op) == OP_AND ) ||
        ( (op) == OP_OR ) ||
        ( (op) == OP_XOR ) );
}



#endif
