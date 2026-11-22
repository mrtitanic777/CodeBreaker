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


#include "cvars.h"
#include <stddef.h>
#include "cmacadd.h"
#include "frontrun.hpp"


#define MACRO_ALIGN         sizeof( void * )

#define macroSizeAlign(x)   _RoundUp( (x), MACRO_ALIGN )

         /* contains actual pointer to block of memory */



namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CmacaddState: cmacadd_state.h) */

}  // namespace



static void *MacroAllocateInSeg( FrontRun& front, size_t size )
{
    void *retn;

    retn = front.cmacadd.MacroOffset;
    size = macroSizeAlign( size );
    front.cmacadd.MacroOffset += size;
    front.cmacadd.MacroSegmentLimit -= size;
    return( retn );
}

static void AllocMacroSegment( FrontRun& front, size_t minimum )
{
    struct macro_seg_list *msl;
    size_t amount;

    amount = _RoundUp( minimum + 2, 0x8000 );
    front.cmacadd.MacroSegment = static_cast<MACADDR_T>(FEmalloc( front, amount ));
    front.cmacadd.MacroOffset = front.cmacadd.MacroSegment;
    front.cmacadd.MacroSegmentLimit = amount - 2;
    if( front.cmacadd.MacroSegment == NULL ) {
        CErr1( front, ERR_OUT_OF_MACRO_MEMORY );
        CSuicide(front);
    }
    msl = static_cast<struct macro_seg_list *>(CMemAlloc( front, sizeof( struct macro_seg_list ) ));
    msl->macro_seg = front.cmacadd.MacroSegment;
    msl->next = front.cmacadd.MacSegList;
    front.cmacadd.MacSegList = msl;
}

void InitMacroSegments( FrontRun& front )
{
    front.cmacadd.MacSegList = NULL;
}

void FreeMacroSegments( FrontRun& front )
{
    struct macro_seg_list *msl;

    for( ; (msl = front.cmacadd.MacSegList) != NULL; ) {
        FEfree( front, (void *)(msl->macro_seg) );
        front.cmacadd.MacSegList = msl->next;
        CMemFree( front, msl );
    }
}


static void MacroCopy( const void *mptr, MACADDR_T offset, size_t amount )
{
    memcpy( offset, mptr, amount );
}

static void MacroReallocOverflow( FrontRun& front, size_t amount_needed, size_t amount_used )
{
    MACADDR_T old_offset;

    amount_needed = macroSizeAlign( amount_needed );
    if( amount_needed > front.cmacadd.MacroSegmentLimit ) {
        old_offset = front.cmacadd.MacroOffset;
        AllocMacroSegment( front, amount_needed );
        if( amount_used > 0 ) {
            MacroCopy( old_offset, front.cmacadd.MacroOffset, amount_used );
        }
    }
}

static MEPTR *MacroLkUp( FrontRun& front, mac_hash_idx hash, const char *name )
{
    MEPTR       mentry;
    MEPTR       *lnk;

    for( lnk = &front.cvars.MacHash[hash]; (mentry = *lnk) != NULL; lnk = &mentry->next_macro ) {
        if( strcmp( mentry->macro_name, name ) == 0 ) {
            break;
        }
    }
    return( lnk );
}

MEPTR MacroDefine( FrontRun& front, size_t mlen, macro_flags mflags )
{
    MEPTR           old_mentry;
    MEPTR           *lnk;
    MEPTR           new_mentry;
    macro_flags     old_mflags;
    MEPTR           mentry;
    const char      *name;
    mac_hash_idx    hash;

    new_mentry = NULL;
    mentry = (MEPTR)front.cmacadd.MacroOffset;
    mentry->macro_len = mlen;
    name = mentry->macro_name;
    hash = CalcHashMacro( name );
    lnk = MacroLkUp( front, hash, name );
    old_mentry = *lnk;
    if( old_mentry != NULL ) {
        old_mflags = old_mentry->macro_flags;
        if( old_mflags & MFLAG_CAN_BE_REDEFINED ) {//delete old entry
            *lnk = old_mentry->next_macro;
            old_mentry = NULL;
        } else if( MacroCompare( mentry, old_mentry ) ) {
            if( MacroIsSpecial( old_mentry ) ) {
                CWarn2p( front, ERR_MACRO_DEFN_NOT_IDENTICAL, name );
            } else {
                SetDiagMacro( front, old_mentry );
                CWarn2p( front, ERR_MACRO_DEFN_NOT_IDENTICAL, name );
                SetDiagPop(front);
                *lnk = old_mentry->next_macro;
                old_mentry = NULL;
            }
        }
    }
    if( old_mentry == NULL ) {  //add new entry
        ++front.cvars.MacroCount;
        new_mentry = static_cast<MEPTR>(MacroAllocateInSeg( front, mlen ));
        new_mentry->macro_flags = front.cvars.InitialMacroFlags | mflags;
        new_mentry->next_macro = front.cvars.MacHash[hash];
        front.cvars.MacHash[hash] = new_mentry;
    }
    return( new_mentry );
}

void *PermMemAlloc( FrontRun& front, size_t amount )
{
    amount = macroSizeAlign( amount );
    if( amount > front.cmacadd.MacroSegmentLimit ) {
        AllocMacroSegment( front, amount );
    }
    return( MacroAllocateInSeg( front, amount ) );
}

bool MacroCompare( MEPTR m1, MEPTR m2 )
{
    if( m1->macro_len == m2->macro_len
      && m1->macro_defn == m2->macro_defn
      && m1->u.parm_count == m2->u.parm_count
      && memcmp( m1->macro_name, m2->macro_name, m1->macro_len - offsetof( MEDEFN, macro_name ) ) == 0 )
        return( false );
    return( true );
}

MEPTR MacroLookup( FrontRun& front, const char *name )
{
    return( *MacroLkUp( front, CalcHashMacro( name ), name ) );
}

void MacroSegmentAddChar(           // MacroSegment: ADD A CHARACTER
    FrontRun& front, size_t *mlen,                   // - data length
    char chr )                      // - character to insert
{
    size_t  clen;

    clen = *mlen;
    MacroReallocOverflow( front, clen + 1, clen );
    front.cmacadd.MacroOffset[clen] = chr;
    *mlen = clen + 1;
}

void MacroSegmentAddToken(          // MacroSegment: ADD A TOKEN
    FrontRun& front, size_t *mlen,                   // - data Length
    TOKEN token )                   // - token to be added
{
    size_t  clen;

    clen = *mlen;
    MacroReallocOverflow( front, clen + SIZE_MTOKEN, clen );
    SET_MTOKEN( front.cmacadd.MacroOffset + clen, token );
    *mlen = clen + SIZE_MTOKEN;
}


void MacroSegmentAddMem(            // MacroSegment: ADD A SEQUENCE OF BYTES
    FrontRun& front, size_t *mlen,                   // - data Length
    const char *buff,               // - bytes to be added
    size_t len )                    // - number of bytes
{
    size_t  clen;

    clen = *mlen;
    MacroReallocOverflow( front, clen + len, clen );
    memcpy( front.cmacadd.MacroOffset + clen, buff, len );
    *mlen += len;
}

MEPTR CreateMEntry( FrontRun& front, const char *name, size_t len )
{
    MEPTR   mentry;
    size_t  size;

    if( len == 0 ) {
        len = strlen( name );
    }
    size = offsetof( MEDEFN, macro_name ) + len + 1;
    MacroReallocOverflow( front, size, 0 );
    mentry = (MEPTR)front.cmacadd.MacroOffset;
    memcpy( mentry->macro_name, name, len );
    mentry->macro_name[len] = '\0';
    mentry->macro_len = size;
    mentry->macro_defn = 0;
    mentry->u.parm_count = 0;
    mentry->src_loc.fno = 0;
    mentry->src_loc.line = 0;
    mentry->src_loc.column = 0;
    return( mentry );
}
