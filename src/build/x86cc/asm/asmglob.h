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
* Description:  WASM main header file contains globals and limits
*
****************************************************************************/


#ifndef _ASMGLOB_H_INCLUDED
#define _ASMGLOB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <setjmp.h>
#include "watcom.h"
#include "bool.h"
#include "asmconst.h"
#include "asmerr.h"
#include "asmins.h"
#include "asmdefs.h"



#define EMPTY                   -2

/* these come back from the jmp() routine as jump_flags */
#define SCRAP_INSTRUCTION       3
#define INDIRECT_JUMP           4

#define CMPSIM(s,c)     strcmp( s, c )
#define CMPSIMBEG(sb,c) strncmp( sb, c, strlen( c ) )
#define CMPSIMEND(se,c) strcmp( se - strlen( c ), c )

#define CMPLIT(s,c)     strcmp( s, c )
#define CMPLITBEG(sb,c) strncmp( sb, c, sizeof( c ) - 1 )
#define CMPLITEND(se,c) strcmp( se - ( sizeof( c ) - 1 ), c )

#define CATSTR(s,c,l)   strncpy( s, c, l ) + l

#define BIT_012                 0x07
#define BIT_345                 0x38
#define BIT_67                  0xC0
#define NOT_BIT_345             0xC7
#define NOT_BIT_67              0x3F

#define BYTE_1                  1
#define BYTE_2                  2
#define BYTE_4                  4
#define BYTE_6                  6
#define BYTE_8                  8
#define BYTE_10                 10

#define IS_VALID_ID_CHAR_FIRST(ch)  (isalpha(ch) || ch=='_' || ch=='@' || ch=='$' || ch=='?')
#define IS_VALID_ID_CHAR(ch)        (isalnum(ch) || ch=='_' || ch=='@' || ch=='$' || ch=='?')

enum fpe {
    DO_FP_EMULATION,
    NO_FP_EMULATION,
    NO_FP_ALLOWED
};

typedef struct sw_data {
    int     fpt;
} sw_data;


/* the variables of asmglob.h, one set for each run of the compiler (run/cbrun.hpp) */
struct AsmglobState {
    sw_data SWData;
};


extern const asm_ins    ASMI86FAR *get_instruction( const char *string );

#endif
