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


/*
 *  STRTAB : routines for creating string tables.
 *
*/

#include <cstddef>
#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/ring.h"


namespace {

constexpr std::size_t STR_BLOCK_SIZE = _4K;
constexpr std::size_t START_UNDEF = static_cast<std::size_t>( -1 );

} // namespace

/* a block of the table: a ring element, then the bytes (the header's STRINGBLOCK) */
struct stringblock {
    stringblock *next;
    size_t      size;
    char        data[STR_BLOCK_SIZE];
};

namespace {

struct strblkparam {
    FunctionRef<write_strtable_fn> fn;
    void                *info;
};

/* the low bit of currbase says the table does not split an item over two blocks */
inline bool TableNoSplitItems( const stringtable *t )
{
    return( (t->currbase & 1) != 0 );
}

inline size_t GetTableLastBlkStart( const stringtable *t )
{
    return( t->currbase & ~static_cast<size_t>( 1 ) );
}

inline stringblock *GetTableLastBlk( const stringtable *t )
{
    return( static_cast<stringblock *>( RingLast( t->data ) ) );
}

stringblock *AllocNewBlock(Link& link,  stringtable *strtab )
/***********************************************/
{
    stringblock *blk;

    blk = static_cast<stringblock *>( MemAllocSafe(link,  sizeof( stringblock ) ) );
    blk->next = nullptr;
    RingAppend( &strtab->data, blk );
    blk->size = 0;
    return( blk );
}

char *AddToStringTable(Link& link,  stringtable *strtab, const void *data, size_t len, bool addnullchar, size_t *offs )
/*********************************************************************************************************/
{
    stringblock *blk;
    size_t      diff;
    char        *dest;
    size_t      start;

    if( addnullchar )
        ++len;
    if( TableNoSplitItems( strtab ) && len > STR_BLOCK_SIZE ) {
        LnkMsg(link,  ERR+MSG_SYMBOL_NAME_TOO_LONG, "s", data );
        len = STR_BLOCK_SIZE;
    }
    start = START_UNDEF;
    for( blk = GetTableLastBlk( strtab ); blk->size + len > STR_BLOCK_SIZE; blk = AllocNewBlock(link,  strtab ) ) {
        if( start == START_UNDEF ) {
            start = GetTableLastBlkStart( strtab ) + (TableNoSplitItems( strtab ) ? STR_BLOCK_SIZE : blk->size);
        }
        diff = STR_BLOCK_SIZE - blk->size;
        if( diff != 0 ) {
            if( TableNoSplitItems( strtab ) ) {   // then don't split
                memset( blk->data + blk->size, 0, diff );
            } else {
                memcpy( blk->data + blk->size, data, diff );
                len -= diff;
                data = static_cast<const char *>( data ) + diff;
            }
        }
        blk->size = STR_BLOCK_SIZE;
        strtab->currbase += STR_BLOCK_SIZE;
    }
    if( offs != nullptr ) {
        if( start == START_UNDEF )
            start = GetTableLastBlkStart( strtab ) + blk->size;
        *offs = start;
    }
    dest = blk->data + blk->size;
    blk->size += len;
    if( addnullchar )
        dest[--len] = '\0';
    memcpy( dest, data, len );
    return( dest );
}

bool WriteStringData( void *_blk, void *_param )
/**********************************************/
{
    const stringblock   *blk = static_cast<stringblock *>( _blk );
    const strblkparam   *param = static_cast<strblkparam *>( _param );

    param->fn( param->info, blk->data, blk->size );
    return( false );
}

} // namespace

void InitStringTable(Link& link,  stringtable *strtab, bool dontsplit )
/*********************************************************/
{
    strtab->data = nullptr;
    strtab->currbase = ( dontsplit ) ? 1 : 0;
    AllocNewBlock(link,  strtab );
}

void AddCharStringTable(Link& link,  stringtable *strtab, char data )
/*******************************************************/
{
    AddToStringTable(link,  strtab, &data, sizeof( char ), false, nullptr );
}

char *AddStringStringTable(Link& link,  stringtable *strtab, const char *data )
/*****************************************************************/
{
    return( AddToStringTable(link,  strtab, data, strlen( data ) + 1, false, nullptr ) );
}

size_t AddStringStringTableOffs(Link& link,  stringtable *strtab, const char *data )
/**********************************************************************/
{
    size_t  offs;

    AddToStringTable(link,  strtab, data, strlen( data ) + 1, false, &offs );
    return( offs );
}

char *AddBufferStringTable(Link& link,  stringtable *strtab, const void *data, size_t len )
/*****************************************************************************/
{
    return( AddToStringTable(link,  strtab, data, len, false, nullptr ) );
}

char *AddSymbolStringTable(Link& link,  stringtable *strtab, const char *data, size_t len )
/*****************************************************************************/
{
    return( AddToStringTable(link,  strtab, data, len, true, nullptr ) );
}

void ZeroStringTable( stringtable *strtab, size_t len )
/*****************************************************/
{
    stringblock *blk;

    blk = GetTableLastBlk( strtab );
    DbgAssert( blk->size + len <= STR_BLOCK_SIZE );
    memset( blk->data + blk->size, 0, len );
    blk->size += len;
}

void WriteStringTable( stringtable *strtab, FunctionRef<write_strtable_fn> fn, void *info )
/*****************************************************************************/
{
    strblkparam param{ fn, info };

    RingLookup( strtab->data, WriteStringData, &param );
}

void FiniStringTable( stringtable *strtab )
/*****************************************/
{
    RingFree( &strtab->data );
}

size_t GetStringTableSize( stringtable *strtab )
/**********************************************/
{
    return( GetTableLastBlkStart( strtab ) + GetTableLastBlk( strtab )->size );
}
