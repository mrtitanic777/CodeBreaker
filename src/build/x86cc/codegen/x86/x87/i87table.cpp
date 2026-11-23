/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2021 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Reduction tables for x87 instructions.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "model.h"
#include "regset.h"
#include "i87table.h"


const opcode_entry    Move87S[] = {
/***********************************/
/*           from  to    eq            verify          reg                gen             fu  */
_OE( UnPatternPP(  R|M|U,R|M|U,EQ_R1 ),      V_NO,           RG_,               G_NO,           FU_NO ),
_OE( UnPatternPP(  M,    M,    NONE  ),      V_SAME_LOCN,    RG_,               G_NO,           FU_NO ),
#if _TARGET & _TARG_8086
_OE( UnPatternPP(  M,    M,    NONE  ),      V_SAME_TYPE,    RG_DOUBLE,         R_SPLITUNARY,   FU_NO ),
_OE( UnPatternPP(  M|C,  R,    NONE  ),      V_NO,           RG_DOUBLE,         R_SPLITUNARY,   FU_NO ),
_OE( UnPatternPP(  R,    M,    NONE  ),      V_NO,           RG_DOUBLE,         R_SPLITUNARY,   FU_NO ),
_OE( UnPatternPP(  C,    M,    NONE  ),      V_SAME_TYPE,    RG_DOUBLE,         R_SPLITUNARY,   FU_NO ),
#else
_OE( UnPatternPP(  M,    M,    NONE  ),      V_SAME_TYPE,    RG_DBL,            R_MOVOP1REG,    FU_NO ),
_OE( UnPatternPP(  M|C,  R,    NONE  ),      V_NO,           RG_DBL,            R_EXT_PUSHC,    FU_NO ),
_OE( UnPatternPP(  R|C,  M,    NONE  ),      V_NO,           RG_DBL,            R_EXT_PUSHC,    FU_NO ),
#endif
_OE( UnPatternPP(  U,    M,    NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    M,    NONE ),       V_NO,           RG_ST0_STI,        G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  M|C,  U,    NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  M,    R,    NONE ),       V_NO,           RG_STI_ST0,        G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    ANY,  NONE ),       V_NO,           RG_DBL_8087,       R_MOVOP1MEM,    FU_NO ),
_OE( UnPatternPP(  ANY,  R,    NONE ),       V_NO,           RG_8087_DBL,       R_MOVRESMEM,    FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_OP1ZERO,      RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_OP1ONE,       RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_NO,           RG_8087,           R_FORCEOP1CMEM, FU_NO ),
_OE( UnPatternPP(  M,    R,    NONE ),       V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    M,    NONE ),       V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R|M,  ANY,  NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    R,    NONE ),       V_NO,           RG_ST0_STI,        G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  ANY,  ANY,  NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};

const opcode_entry    Move87D[] = {
/***********************************/
/*           from  to    eq            verify          reg                gen             fu  */
_OE( UnPatternPP(  R|M|U,R|M|U,EQ_R1 ),      V_NO,           RG_,               G_NO,           FU_NO ),
_OE( UnPatternPP(  M,    M,    NONE  ),      V_SAME_LOCN,    RG_,               G_NO,           FU_NO ),
#if _TARGET & _TARG_8086
_OE( UnPatternPP(  M,    M,    NONE  ),      V_SAME_TYPE,    RG_,               R_SPLIT8,       FU_NO ),
_OE( UnPatternPP(  M|C,  R,    NONE  ),      V_NO,           RG_8,              R_SPLIT8,       FU_NO ),
_OE( UnPatternPP(  R,    M,    NONE  ),      V_NO,           RG_8,              R_SPLIT8,       FU_NO ),
_OE( UnPatternPP(  C,    M,    NONE  ),      V_SAME_TYPE,    RG_8,              R_SPLIT8,       FU_NO ),
#else
_OE( UnPatternPP(  M,    M,    NONE  ),      V_SAME_TYPE,    RG_,               R_SPLITMOVE,    FU_NO ),
_OE( UnPatternPP(  M|C,  R,    NONE  ),      V_NO,           RG_8,              R_SPLITMOVE,    FU_NO ),
_OE( UnPatternPP(  R,    M,    NONE  ),      V_NO,           RG_8,              R_SPLITMOVE,    FU_NO ),
_OE( UnPatternPP(  C,    M,    NONE  ),      V_SAME_TYPE,    RG_8,              R_SPLITMOVE,    FU_NO ),
#endif
_OE( UnPatternPP(  U,    M,    NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    M,    NONE ),       V_NO,           RG_ST0_STI,        G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  M|C,  U,    NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  M,    R,    NONE ),       V_NO,           RG_STI_ST0,        G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    ANY,  NONE ),       V_NO,           RG_8_8087,         R_MOVOP1MEM,    FU_NO ),
_OE( UnPatternPP(  ANY,  R,    NONE ),       V_NO,           RG_8087_8,         R_MOVRESMEM,    FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_OP1ZERO,      RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_OP1ONE,       RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_NO,           RG_8087,           R_FORCEOP1CMEM, FU_NO ),
_OE( UnPatternPP(  M,    R,    NONE ),       V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    M,    NONE ),       V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R|M,  ANY,  NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  R,    R,    NONE ),       V_NO,           RG_ST0_STI,        G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  ANY,  ANY,  NONE ),       V_NO,           RG_8087_NEED_MOVE, G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};


const opcode_entry    Un87[] = {
/***************************/
/*           from  to    eq            verify          reg                gen             fu  */
_OE( UnPatternPP(  M,    M,    EQ_R1 ),      V_NEGATE,       RG_8087_ZAP_ACC,   R_FLIPSIGN,     FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_OP1ZERO,      RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_OP1ONE,       RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_NO,           RG_8087,           R_FORCEOP1CMEM, FU_NO ),
_OE( UnPatternPP(  ANY,  ANY,  NONE ),       V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};

const opcode_entry    Un87Func[] = {
/****************************/
/*           from  to    eq            verify          reg                gen             fu  */
_OE( UnPattern(    C,    ANY,  NONE ),       V_OP1ZERO,      RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    C,    ANY,  NONE ),       V_OP1ONE,       RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    C,    ANY,  NONE ),       V_NO,           RG_8087_ZAP_ACC,   R_FORCEOP1CMEM, FU_NO ),
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};

const opcode_entry    Push87S[] = {
/*****************************/
/*           from  to    eq            verify          reg                gen             fu  */
#if _TARGET & _TARG_8086
_OE( UnPattern(    R|M|C,ANY,  NONE ),       V_NO,           RG_DBL_OR_PTR,     R_SPLITUNARY,   FU_NO ),
#else
_OE( UnPatternPP(  R,    ANY,  NONE ),       V_NO,           RG_DBL,            G_WORDR1,       FU_FOP ),
_OE( UnPatternPP(  M,    ANY,  NONE ),       V_NO,           RG_,               G_M1,           FU_FOP ),
_OE( UnPatternPP(  C,    ANY,  NONE ),       V_NO,           RG_,               G_PUSHFS,       FU_FOP ),
#endif
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};

const opcode_entry    Push87D[] = {
/*****************************/
/*           from  to    eq            verify          reg                gen             fu  */
#if _TARGET & _TARG_8086
_OE( UnPattern(    R|M|C,ANY,  NONE ),       V_NO,           RG_8,              R_SPLIT8,       FU_NO ),
#else
_OE( UnPattern(    R|M|C,ANY,  NONE ),       V_NO,           RG_8,              R_SPLITUNARY,   FU_NO ),
#endif
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( UnPattern(    ANY,  ANY,  NONE ),       V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};

const opcode_entry    Bin87[] = {
/***************************/
/*           op1   op2   res   eq      verify          reg                gen             fu  */
_OE( BinPatternPP( C,    R|M|U,ANY,  NONE ), V_OP1ONE,       RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( BinPatternPP( C,    R|M|U,ANY,  NONE ), V_OP1ZERO,      RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( BinPatternPP( C,    ANY,  ANY,  NONE ), V_NO,           RG_8087,           R_FORCEOP1CMEM, FU_NO ),
_OE( BinPatternPP( ANY,  C,    ANY,  NONE ), V_OP2ONE,       RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( BinPatternPP( ANY,  C,    ANY,  NONE ), V_OP2ZERO,      RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( BinPatternPP( ANY,  C,    ANY,  NONE ), V_NO,           RG_8087,           R_FORCEOP2CMEM, FU_NO ),
_OE( BinPatternPP( ANY,  ANY,  ANY,  NONE ), V_NO,           RG_8087,           G_UNKNOWN,      FU_NO ),
_OE( BinPattern(   ANY,  ANY,  ANY,  NONE ), V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};

const opcode_entry    Bin87Func[] = {
/*****************************/
/*           op1   op2   res   eq      verify          reg                gen             fu  */
_OE( BinPattern(   C,    R|M|U,ANY,  NONE ), V_OP1ONE,       RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( BinPattern(   C,    R|M|U,ANY,  NONE ), V_OP1ZERO,      RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( BinPattern(   C,    ANY,  ANY,  NONE ), V_NO,           RG_8087_ZAP_ACC,   R_FORCEOP1CMEM, FU_NO ),
_OE( BinPattern(   ANY,  C,    ANY,  NONE ), V_OP2ONE,       RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( BinPattern(   ANY,  C,    ANY,  NONE ), V_OP2ZERO,      RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( BinPattern(   ANY,  C,    ANY,  NONE ), V_NO,           RG_8087_ZAP_ACC,   R_FORCEOP2CMEM, FU_NO ),
_OE( BinPattern(   ANY,  ANY,  ANY,  NONE ), V_NO,           RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( BinPattern(   ANY,  ANY,  ANY,  NONE ), V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};

const opcode_entry    Cmp87[] = {
/***************************/
/*           op1   op2                 verify          reg                gen             fu  */
_OE( SidePatternCC( C,    ANY  ),             V_OP1ONE,       RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( SidePatternCC( C,    ANY  ),             V_OP1ZERO,      RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( SidePatternCC( C,    ANY  ),             V_NO,           RG_8087_ZAP_ACC,   R_FORCEOP1CMEM, FU_NO ),
_OE( SidePatternCC( M,    C    ),             V_INTCOMP,      RG_8087_ZAP_ACC,   R_INTCOMP,      FU_NO ),
_OE( SidePatternCC( ANY,  C    ),             V_OP2ONE,       RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( SidePatternCC( ANY,  C    ),             V_OP2ZERO,      RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( SidePatternCC( ANY,  C    ),             V_NO,           RG_8087_ZAP_ACC,   R_FORCEOP2CMEM, FU_NO ),
_OE( SidePatternCC( ANY,  ANY  ),             V_NO,           RG_8087_ZAP_ACC,   G_UNKNOWN,      FU_NO ),
_OE( SidePattern(  ANY,  ANY  ),             V_NO,           RG_,               G_UNKNOWN,      FU_NO ),
};
