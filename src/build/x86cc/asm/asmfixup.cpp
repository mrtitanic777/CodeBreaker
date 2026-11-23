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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "asmglob.h"
#include "asmalloc.h"
#include "asmfixup.h"
#include "asminlin.h"
#include "asmrun.hpp"



asmfixup *AddFixup( AsmRun& assembler, asm_sym_handle sym, fixup_types fixup_type, fixup_options fixup_option )
/*******************************************************************************************
 * put the correct target offset into the link list when forward reference of
 * relocatable is resolved;
 */
{
    asmfixup    *fixup;

    switch( assembler.asmdefs.Code->info.token ) {
    case T_CMPS:
    case T_LODS:
    case T_MOVS:
    case T_OUTS:
    case T_INS:
    case T_SCAS:
    case T_STOS:
    case T_XLAT:
        return( NULL );
    }

    fixup = static_cast<asmfixup *>(MemAllocSafe( *assembler.front, sizeof( asmfixup ) ));
    fixup->external = false;
    fixup->fixup_loc = assembler.asminlin->AsmCodeAddress;
    fixup->name = sym->name;
    fixup->next = assembler.asminlin->FixupHead;
    assembler.asminlin->FixupHead = fixup;
    fixup->u_offset = 0;
    fixup->fixup_type = fixup_type;
    fixup->fixup_option = fixup_option;
    assembler.asmfixup.InsFixups[assembler.asmdefs.Opnd_Count] = fixup;
    return( fixup );
}


#define SkipFixup() \
    fixup->next = assembler.asminlin->FixupHead; \
    assembler.asminlin->FixupHead = fixup

static void PatchCodeBuffer( AsmRun& assembler, asmfixup *fixup, unsigned size )
/***********************************************************/
{
    long    disp;
    long    fixup_loc;

    fixup_loc = fixup->fixup_loc;
    disp = fixup->u_offset + assembler.asminlin->AsmCodeAddress - fixup->fixup_loc - size;
    for( ; size > 0; size-- ) {
        assembler.asminlin->AsmCodeBuffer[fixup_loc++] = (uint_8)disp;
        disp >>= 8;
    }
}


static bool DoPatch( AsmRun& assembler, asm_sym_handle sym, asmfixup *fixup )
/********************************************************/
{
    long                disp;
    long                max_disp;
    unsigned            size;
    if( fixup->name != sym->name ) {
        SkipFixup();
        return( RC_OK );
    }
    size = 0;
    switch( fixup->fixup_type ) {
    case FIX_RELOFF32:
        size = 2;
        /* fall through */
    case FIX_RELOFF16:
        size++;
        /* fall through */
    case FIX_RELOFF8:
        size++;
        /*
         * calculate the displacement
         */
        disp = fixup->u_offset + assembler.asminlin->AsmCodeAddress - fixup->fixup_loc - size;
        max_disp = (1UL << ((size * 8)-1)) - 1;
        if( disp > max_disp || disp < -(max_disp + 1) ) {
            AsmError( *assembler.front, JUMP_OUT_OF_RANGE );
            assembler.asminlin->FixupHead = NULL;
            return( RC_ERROR );
        } else {
            PatchCodeBuffer( assembler, fixup, size );
        }
        MemFree( *assembler.front, fixup );
        break;
    default:
        SkipFixup();
        break;
    }
    return( RC_OK );
}

bool BackPatch( AsmRun& assembler, asm_sym_handle sym )
/*****************************
 * patching for forward reference labels in Jmp/Call instructions;
 * call only when a new label appears;
 */
{
    asmfixup    *fixup;
    asmfixup    *next;

    fixup = assembler.asminlin->FixupHead;
    assembler.asminlin->FixupHead = NULL;
    for( ; fixup != NULL; fixup = next ) {
        next = fixup->next;
        if( DoPatch( assembler, sym, fixup ) ) {
            return( RC_ERROR );
        }
    }
    return( RC_OK );
}

void mark_fixupp( AsmRun& assembler, OPNDTYPE determinant, operand_idx index )
/**********************************************************
 * this routine marks the correct target offset and data record address for
 * FIXUPP record;
 */
{
    asmfixup    *fixup;

    fixup = assembler.asmfixup.InsFixups[index];
    if( fixup != NULL ) {
        fixup->fixup_loc = assembler.asminlin->AsmCodeAddress;
        fixup->u_offset = assembler.asmdefs.Code->data[index];
        assembler.asmdefs.Code->data[index] = 0;

        switch( determinant ) {
        case OP_I16:
        case OP_J32:
            switch( fixup->fixup_type ) {
            case FIX_OFF32:
                fixup->fixup_type = FIX_OFF16;
                break;
            case FIX_PTR32:
                fixup->fixup_type = FIX_PTR16;
                break;
            }
            break;
        case OP_I32:
        case OP_J48:
            switch( fixup->fixup_type ) {
            case FIX_OFF16:
                fixup->fixup_type = FIX_OFF32;
                break;
            case FIX_PTR16:
                fixup->fixup_type = FIX_PTR32;
                break;
            }
            break;
        }
    }
}


bool AddFPPatchAndFixups( AsmRun& assembler, fp_patches patch )
/******************************************/
{
    asmfixup    *fixup;

    fixup = static_cast<asmfixup *>(MemAllocSafe( *assembler.front, sizeof( asmfixup ) ));
    fixup->next = assembler.asminlin->FixupHead;
    assembler.asminlin->FixupHead = fixup;
    fixup->external = false;
    fixup->fixup_loc = assembler.asminlin->AsmCodeAddress;
    fixup->name = NULL;
    fixup->u_fppatch = patch;
    fixup->fixup_type = FIX_FPPATCH;
    fixup->fixup_option = OPTJ_NONE;
    if( patch == FPP_WAIT ) {
        AsmCodeByte( OP_NOP );
    } else {
        AsmCodeByte( OP_WAIT );
    }
    return( RC_OK );
}

void AsmFiniRelocs( AsmRun& assembler )
/************************/
{
    asmfixup    *fixup;

    while( (fixup = assembler.asminlin->FixupHead) != NULL ) {
        assembler.asminlin->FixupHead = fixup->next;
        MemFree( *assembler.front, fixup );
    }
}
