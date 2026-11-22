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
* Description:  ASCII encoding - table of character types.
*               Latin-1 character set
*
****************************************************************************/


#include "scan.h"
#include "ctokens.h"


/* C++ has TAB and CR set to C_XW */

/* The following table is ASCII dependent. */


extern const unsigned char TokValue[] = {      /* ASCII */

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* 00 - 0F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* 10 - 1F */

    0,                                  /* 20 SP */
    T_EXCLAMATION,                      /* 21 !  */
    0,                                  /* 22 "  */
    T_SHARP,                            /* 23 #  */
    0,                                  /* 24 $  */
    T_PERCENT,                          /* 25 %  */
    T_AND,                              /* 26 &  */
    0,                                  /* 27 '  */
    T_LEFT_PAREN,                       /* 28 (  */
    T_RIGHT_PAREN,                      /* 29 )  */
    T_TIMES,                            /* 2A *  */
    T_PLUS,                             /* 2B +  */
    T_COMMA,                            /* 2C ,  */
    T_MINUS,                            /* 2D -  */
    T_DOT,                              /* 2E .  */
    T_DIV,                              /* 2F /  */
    W_0,                                /* 30 0  */
    W_1,                                /* 31 1  */
    W_2,                                /* 32 2  */
    W_3,                                /* 33 3  */
    W_4,                                /* 34 4  */
    W_5,                                /* 35 5  */
    W_6,                                /* 36 6  */
    W_7,                                /* 37 7  */
    W_8,                                /* 38 8  */
    W_9,                                /* 39 9  */
    T_COLON,                            /* 3A :  */
    T_SEMI_COLON,                       /* 3B ;  */
    T_LT,                               /* 3C <  */
    T_EQUAL,                            /* 3D =  */
    T_GT,                               /* 3E >  */
    T_QUESTION,                         /* 3F ?  */
    T_ATSIGN,                           /* 40 @  */

    /* character weights used for hashing function */

    W_A,                                /* A */
    W_B,                                /* B */
    W_C,                                /* C */
    W_D,                                /* D */
    W_E,                                /* E */
    W_F,                                /* F */
    W_G,                                /* G */
    W_H,                                /* H */
    W_I,                                /* I */
    W_J,                                /* J */
    W_K,                                /* K */
    W_L,                                /* L */
    W_M,                                /* M */
    W_N,                                /* N */
    W_O,                                /* O */
    W_P,                                /* P */
    W_Q,                                /* Q */
    W_R,                                /* R */
    W_S,                                /* S */
    W_T,                                /* T */
    W_U,                                /* U */
    W_V,                                /* V */
    W_W,                                /* W */
    W_X,                                /* X */
    W_Y,                                /* Y */
    W_Z,                                /* Z */

    T_LEFT_BRACKET,                     /* 5B [  */
    0,                                  /* 5C \  */
    T_RIGHT_BRACKET,                    /* 5D ]  */
    T_XOR,                              /* 5E ^  */
    W__,                                /* 5F _  */
    0,                                  /* 60 `  */

    /* character weights used for hashing function */

    W_a,                                /* a */
    W_b,                                /* b */
    W_c,                                /* c */
    W_d,                                /* d */
    W_e,                                /* e */
    W_f,                                /* f */
    W_g,                                /* g */
    W_h,                                /* h */
    W_i,                                /* i */
    W_j,                                /* j */
    W_k,                                /* k */
    W_l,                                /* l */
    W_m,                                /* m */
    W_n,                                /* n */
    W_o,                                /* o */
    W_p,                                /* p */
    W_q,                                /* q */
    W_r,                                /* r */
    W_s,                                /* s */
    W_t,                                /* t */
    W_u,                                /* u */
    W_v,                                /* v */
    W_w,                                /* w */
    W_x,                                /* x */
    W_y,                                /* y */
    W_z,                                /* z */

    T_LEFT_BRACE,                       /* 7B {  */
    T_OR,                               /* 7C |  */
    T_RIGHT_BRACE,                      /* 7D }  */
    T_TILDE,                            /* 7E ~  */
    0,                                  /* 7F DEL*/

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* 80 - 8F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* 90 - 9F */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* A0 - AF */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* B0 - BF */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* C0 - CF */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* D0 - DF */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    /* E0 - EF */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0     /* F0 - FF */
};
