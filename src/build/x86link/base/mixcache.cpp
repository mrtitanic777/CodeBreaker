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
  MIXCACHE - object file caching routines which do both full file caching
                and paged caching.
*/

#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/base/fileio.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/link/run_state.hpp"


constexpr unsigned long kCachePageSize = _8K;

namespace {

struct CacheState {
    bool    Multipage;
};

CacheState& state()
{
    return cb_wlink::RunState::part<CacheState>();
}

// figure out the number of cache blocks necessary
unsigned NumCacheBlocks( unsigned long len )
{
    unsigned numblocks = static_cast<unsigned>( len / kCachePageSize );
    if( (len % kCachePageSize) != 0 ) {
        numblocks++;
    }
    return numblocks;
}

// the page table of a paged cache: one pointer to a page (or nullptr) for each page of the file
char **page_table( const infilelist *infile )
{
    return static_cast<char **>( infile->cache );
}

bool DumpFileCache( infilelist *infile, bool nuke )
{
    bool blockfreed = false;
    unsigned savenum;

    if( nuke ) {
        savenum = UINT_MAX;
    } else {
        savenum = static_cast<unsigned>( infile->currpos / kCachePageSize );
    }
    if( infile->cache != nullptr ) {
        unsigned num = NumCacheBlocks( infile->len );
        char **blocklist = page_table( infile );
        for( unsigned index = 0; index < num; index++ ) {
            if( index != savenum
              && blocklist[index] != nullptr ) {
                MemFree( blocklist[index] );
                blocklist[index] = nullptr;
                blockfreed = true;
            }
        }
    }
    return blockfreed;
}

}  // namespace

bool CacheOpen(Link& link,  const file_list *file )
/*************************************/
{
    if( file == nullptr )
        return true;
    infilelist *infile = file->infile;
    if( infile->status & INSTAT_IOERR )
        return false;
    if( DoObjOpen(link,  infile ) ) {
        infile->status |= INSTAT_IN_USE;
    } else {
        infile->status |= INSTAT_IOERR;
        return false;
    }
    if( infile->len == 0 ) {
        infile->len = QFileSize( infile->handle );
        if( infile->len == 0 ) {
            LnkMsg(link,  ERR+MSG_BAD_OBJECT, "s", infile->name );
            infile->status |= INSTAT_IOERR;
            return false;
        }
    }
    if( (infile->status & INSTAT_SET_CACHE) == 0 ) {
        if( link.LinkFlags & LF_CACHE_FLAG ) {
            infile->status |= INSTAT_FULL_CACHE;
        } else if( link.LinkFlags & LF_NOCACHE_FLAG ) {
            infile->status |= INSTAT_PAGE_CACHE;
        } else {
            if( infile->status & INSTAT_LIBRARY ) {
                infile->status |= INSTAT_PAGE_CACHE;
            } else {
                infile->status |= INSTAT_FULL_CACHE;
            }
        }
    }
    if( infile->cache == nullptr ) {
        if( infile->status & INSTAT_FULL_CACHE ) {
            infile->cache = MemAllocSafe(link,  infile->len );
            if( infile->currpos != 0 ) {
                QLSeek(link,  infile->handle, 0, SEEK_SET, infile->name.u.ptr );
            }
            QRead(link,  infile->handle, infile->cache, infile->len, infile->name.u.ptr );
            infile->currpos = infile->len;
        } else {
            unsigned numblocks = NumCacheBlocks( infile->len );
            infile->cache = Pass1Alloc(link,  numblocks * sizeof( char * ) );
            std::fill_n( page_table( infile ), numblocks, nullptr );
        }
    }
    return true;
}

void CacheClose(Link& link,  const file_list *file, unsigned pass )
/*****************************************************/
{
    if( file == nullptr )
        return;
    infilelist *infile = file->infile;
//    if( infile->handle == NIL_FHANDLE )
//        return;
    infile->status &= ~INSTAT_IN_USE;
    switch( pass ) {
    case 1: /* first pass */
        {
            bool nukecache = ( (infile->status & INSTAT_LIBRARY) == 0 );
            if( infile->status & INSTAT_FULL_CACHE ) {
                if( nukecache ) {
                    FreeObjCache( file );
                }
            } else {
                DumpFileCache( infile, nukecache );   // do not cache .obj's
            }
        }
        break;
    case 3: /* freeing structure */
        FreeObjCache( file );
        if( infile->handle != NIL_FHANDLE ) {
            QClose(link,  infile->handle, infile->name.u.ptr );
            infile->handle = NIL_FHANDLE;
        }
        break;
    }
}

void *CachePermRead(Link& link,  const file_list *file, unsigned long pos, size_t len )
/*************************************************************************/
{
    char *buf = static_cast<char *>( CacheRead(link,  file, pos, len ) );
    if( file->infile->status & INSTAT_FULL_CACHE )
        return buf;
    char *result;
    CacheState &st = state();
    if( st.Multipage ) {
        result = static_cast<char *>( MemReallocSafe(link,  buf, len ) );
        link.TokBuff = static_cast<char *>( MemAllocSafe(link,  link.TokSize ) );
        st.Multipage = false;           // indicate that last read is permanent.
    } else {
        result = static_cast<char *>( MemAllocSafe(link,  len ) );
        std::memcpy( result, buf, len );
    }
    return result;
}

void *CacheRead(Link& link,  const file_list *file, unsigned long pos, size_t len )
/**********************************************************************
 * read len bytes out of the cache.
 */
{
    infilelist *infile = file->infile;

    if( infile->status & INSTAT_FULL_CACHE ) {
        if( pos + len > infile->len )
            return nullptr;
        return static_cast<char *>( infile->cache ) + pos;
    }
    CacheState &st = state();
    st.Multipage = false;
    size_t offset = pos % kCachePageSize;
    size_t amtread = kCachePageSize - offset;
    size_t startnum = pos / kCachePageSize;
    size_t bufnum = startnum;
    char **cache = page_table( infile );
    for( ;; ) {
        if( cache[bufnum] == nullptr ) {   // make sure page is in.
            cache[bufnum] = static_cast<char *>( MemAllocSafe(link,  kCachePageSize ) );
            unsigned long newpos = static_cast<unsigned long>( bufnum ) * kCachePageSize;
            if( infile->currpos != newpos ) {
                QSeek(link,  infile->handle, newpos, infile->name.u.ptr );
            }
            infile->currpos = newpos + kCachePageSize;
            QRead(link,  infile->handle, cache[bufnum], kCachePageSize, infile->name.u.ptr );
        }
        if( amtread >= len )
            break;
        amtread += kCachePageSize;     // it spans pages.
        bufnum++;
        st.Multipage = true;
    }
    char *result;
    if( !st.Multipage ) {
        result = cache[startnum] + offset;
    } else {
        if( link.TokSize < len ) {
            link.TokSize = __ROUND_UP_SIZE_SECTOR( len );
            link.TokBuff = static_cast<char *>( MemReallocSafe(link,  link.TokBuff, link.TokSize ) );
        }
        amtread = kCachePageSize - offset;
        std::memcpy( link.TokBuff, cache[startnum] + offset, amtread );
        len -= amtread;
        result = link.TokBuff + amtread;
        for( ;; ) {
            startnum++;
            if( len <= kCachePageSize ) {
                std::memcpy( result, cache[startnum], len );
                break;
            } else {
                std::memcpy( result, cache[startnum], kCachePageSize );
                len -= kCachePageSize;
                result += kCachePageSize;
            }
        }
        result = link.TokBuff;
    }
    return result;
}

bool CacheIsPerm( void )
/**********************/
{
    return !state().Multipage;
}

bool CacheIsEnd( const file_list *file, unsigned long pos )
/*********************************************************/
{
    return pos >= file->infile->len;
}

void CacheFini( void )
/********************/
{
}

void CacheFree( const file_list *file, void *mem )
/*************************************************
 * used for disposing things allocated by CachePermRead
 */
{
    if( file->infile->status & INSTAT_PAGE_CACHE ) {
        MemFree( mem );
    }
}

void FreeObjCache( const file_list *file )
/****************************************/
{
    if( file == nullptr )
        return;
    if( file->infile->status & INSTAT_FULL_CACHE ) {
        MemFree( file->infile->cache );
    } else {
        DumpFileCache( file->infile, true );
    }
    file->infile->cache = nullptr;
}

bool DumpObjCache(Link& link)
/******************************/
// find and dump an object file cache.
{
    for( infilelist *infile = link.CachedFiles; infile != nullptr; infile = infile->next ) {
        if( infile->status & INSTAT_PAGE_CACHE ) {
            if( link.CurrMod == nullptr
              || link.CurrMod->u1.source == nullptr
              || link.CurrMod->u1.source->infile != infile ) {
                if( DumpFileCache( infile, true ) ) {
                    return true;
                }
            }
        }
    }
    return false;
}
