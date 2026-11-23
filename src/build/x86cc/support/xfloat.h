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
* Description:  C/C++ run-time library floating-point definitions.
*
****************************************************************************/


#ifndef _XFLOAT_H_INCLUDED
#define _XFLOAT_H_INCLUDED

#include <stddef.h>     // for wchar_t
#include <float.h>      // for LDBL_DIG

extern "C" {


typedef unsigned int    u4;
typedef int             i4;

typedef struct {                // This layout matches Intel 8087
    union {
        double  value;          // - double value
        u4      word[2];        // - so we can access bits
    } u;
} long_double;

typedef struct {                // Layout of IEEE 754 double (FD)
    union {
        double  value;          // - double value
        u4      word[2];        // - so we can access bits
    } u;
} float_double;

typedef struct {                // Layout of IEEE 754 single (FS)
    union {
        float   value;          // - double value
        u4      word;           // - so we can access bits
    } u;
} float_single;

enum    ldcvt_flags {
    FPCVT_NONE          = 0x0000,
    FPCVT_E_FMT         = 0x0001,   // 'E' format
    FPCVT_F_FMT         = 0x0002,   // 'F' format
    FPCVT_G_FMT         = 0x0004,   // 'G' format
    FPCVT_F_CVT         = 0x0008,   // __cvt routine format rules
    FPCVT_F_DOT         = 0x0010,   // always put '.' in result
    FPCVT_LONG_DOUBLE   = 0x0020,   // number is true long double
    FPCVT_NO_TRUNC      = 0x0040,   // always provide ndigits in buffer
    FPCVT_IN_CAPS       = 0x0080,   // alpha characters are output uppercased
    FPCVT_IS_INF_NAN    = 0x0100,   // number is inf/nan (output flag)
    FPCVT_A_FMT         = 0x0200,   // 'A' format
};

typedef struct cvt_info {
    int         ndigits;        // INPUT: number of digits
    int         scale;          // INPUT: FORTRAN scale factor
    int         flags;          // INPUT/OUTPUT: flags (see ldcvt_flags)
    int         expchar;        // INPUT: exponent character to use
    int         expwidth;       // INPUT/OUTPUT: number of exponent digits
    int         sign;           // OUTPUT: 0 => +ve; otherwise -ve
    int         decimal_place;  // OUTPUT: position of '.'
    int         n1;             // OUTPUT: number of leading characters
    int         nz1;            // OUTPUT: followed by this many '0's
    int         n2;             // OUTPUT: followed by these characters
    int         nz2;            // OUTPUT: followed by this many '0's
} CVT_INFO;

/*
 * Depending on the target, some functions expect near pointer arguments
 * to be pointing into the stack segment, while in other cases they must
 * point into the data segment.
 */
typedef long_double                                     *ld_stk_ptr;
typedef double                                          *dbl_stk_ptr;
typedef float                                           *flt_stk_ptr;
typedef char                                            *buf_stk_ptr;
typedef void                                            *i8_stk_ptr;
typedef void                                            *u8_stk_ptr;

extern  int     __LDClass( long_double * );
extern  void    __ZBuf2LD( buf_stk_ptr, ld_stk_ptr );
extern  void    __ZXBuf2LD( buf_stk_ptr buf, ld_stk_ptr ld, int *exponent );
extern  void    _LDScale10x( ld_stk_ptr, int );


/*
 * define number of significant digits for long double numbers (80-bit)
 * it will be defined in float.h as soon as OW support long double
 * used in mathlib/c/ldcvt.c
 */

/*
 * floating point conversion buffer length definition
 * is defined in lib_misc/h/cvtbuf.h
 * used by various floating point conversion routines
 * used in clib/startup/c/cvtbuf.c, lib_misc/h/thread.h
 * and mathlib/c/efcvt.c
 * it must be equal maximum FP precision ( LDBL_DIG )
 * hold lib_misc/h/cvtbuf.h in sync with LDBL_DIG
 */
};
#endif
