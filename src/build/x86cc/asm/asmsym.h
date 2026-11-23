/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Assmebler symbols internal structures and definitions.
*
****************************************************************************/


#ifndef _ASMSYM_H_
#define _ASMSYM_H_

#include "asminlin.h"

typedef enum {
    MT_EMPTY,
    MT_BYTE,
    MT_WORD,
    MT_DWORD,
    MT_QWORD,
    MT_FWORD,
    MT_TBYTE,
    MT_OWORD,
    MT_SHORT,
    MT_NEAR,
    MT_FAR,
    MT_PTR,
    MT_ERROR
} memtype;


typedef struct asm_sym {
    struct asm_sym  *next;
    char            *name;

    long            addr;
    memtype         mem_type;
    sym_state       state;
    asmfixup        *fixup;
} asm_sym;

extern asm_sym_handle   AsmLookup( AsmRun& assembler, const char *name );
extern asm_sym_handle   AsmGetSymbol( const char *name );
extern void             FreeASym( AsmRun& assembler, asm_sym_handle sym );


#endif
