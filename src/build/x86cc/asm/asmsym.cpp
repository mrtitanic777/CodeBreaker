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
* Description:  Assembler symbol table management.
*
****************************************************************************/


#include "asmglob.h"
#include "asmalloc.h"

#include "asmrun.hpp"



namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct AsmsymState {
    asm_sym_handle AsmSymHead;
};
namespace {

}  // namespace



static unsigned short const CvtTable[] = {
    #define ASM_TYPE(c,a)   a,
    ASM_TYPES
    #undef ASM_TYPE
};


static char *InitAsmSym( AsmRun& assembler, asm_sym_handle sym, const char *name )
/*************************************************************/
{
    void        *handle;

    sym->name = MemStrdupSafe( *assembler.front, name );
    sym->next = NULL;
    sym->fixup = NULL;
    sym->addr = 0;
    handle = AsmQuerySymbol( *assembler.front, sym->name );
    sym->state = AsmQueryState( *assembler.front, handle );
    if( sym->state == SYM_UNDEFINED ) {
        sym->mem_type = MT_EMPTY;
    } else {
        sym->mem_type = static_cast<memtype>(CvtTable[ AsmQueryType( *assembler.front, handle ) ]);
    }
    return( sym->name );
}

static asm_sym_handle AllocASym( AsmRun& assembler, const char *name )
/*************************************************/
{
    asm_sym_handle  sym;

    sym = static_cast<asm_sym_handle>(MemAllocSafe( *assembler.front, sizeof( *sym ) ));
    if( InitAsmSym( assembler, sym, name ) == NULL ) {
        MemFree( *assembler.front, sym );
        return( NULL );
    }
    return( sym );
}

static asm_sym_handle *AsmFind( AsmRun& assembler, const char *name )
/*************************************************
 * find a symbol in the symbol table, return NULL if not found
 */
{
    asm_sym_handle  *sym;

    sym = &assembler.asmsym->AsmSymHead;
    for( ; *sym != NULL; sym = &((*sym)->next) ) {
            if( strcmp( name, (*sym)->name ) == 0 ) {
                break;
            }
    }
    return( sym );
}


asm_sym_handle AsmLookup( AsmRun& assembler, const char *name )
/******************************************/
{
    asm_sym_handle  *sym_ptr;
    asm_sym_handle  sym;

    if( strlen( name ) > MAX_ID_LEN ) {
        AsmError( *assembler.front, LABEL_TOO_LONG );
        return NULL;
    }
    sym_ptr = AsmFind( assembler, name );
    sym = *sym_ptr;
    if( sym != NULL ) {
        return( sym );
    }

    sym = AllocASym( assembler, name );
    if( sym != NULL ) {
        sym->next = *sym_ptr;
        *sym_ptr = sym;

        sym->addr = assembler.asminlin->AsmCodeAddress;
    } else {
        AsmError( *assembler.front, NO_MEMORY );
    }
    return( sym );
}

void FreeASym( AsmRun& assembler, asm_sym_handle sym )
/*********************************/
{
    MemFree( *assembler.front, sym->name );
    MemFree( *assembler.front, sym );
}


void AsmSymInit( void )
/*********************/
{
}

void AsmSymFini( AsmRun& assembler )
/*********************/
{
    asm_sym_handle  sym;
    for( ; (sym = assembler.asmsym->AsmSymHead) != NULL; ) {
        assembler.asmsym->AsmSymHead = sym->next;
        FreeASym( assembler, sym );
    }
}


/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
AsmsymState* asmsym_make() { return new AsmsymState(); }
void asmsym_free(AsmsymState* part) { delete part; }
