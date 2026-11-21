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
* Description:  Carving memory manager for linker.
*
****************************************************************************/



#include <cstddef>
#include <cstdlib>

#include "build/x86link/link/linkstd.h"
#include "build/x86link/driver/ideentry.h"
#include "build/x86link/base/carve.h"


namespace {

constexpr std::size_t CARVE_ALIGN = sizeof( void * );

struct blk_t {
    blk_t           *next;
    boolbit         modified    : 1;
    unsigned        index;
};

struct free_t {
    free_t          *next;
};

// where a block's elements begin
constexpr std::size_t kBlkHeader = __ROUND_UP_SIZE( sizeof( blk_t ), CARVE_ALIGN );

inline std::size_t carve_size( unsigned s )
{
    return kBlkHeader + s;
}

inline char *carve_data( void *blk, unsigned d )
{
    return static_cast<char *>( blk ) + kBlkHeader + d;
}

}  // namespace

struct cv_t {
    free_t          *free_list;
    free_t          *insert;
    blk_t           *blk_list;
    blk_t           **blk_map;
    unsigned        elm_size;
    unsigned        elm_count;
    unsigned        blk_top;
    unsigned        blk_count;
    unsigned        blk_size;
    boolbit         size_chg : 1;
};

namespace {

// take the head of the free list (assumes it is not empty)
inline void *remove_from_free( cv_t *pcv )
{
    free_t *head = pcv->free_list;

    pcv->free_list = head->next;
    return head;
}

inline void add_to_free( free_t *&fl, void *p )
{
    free_t *node = static_cast<free_t *>( p );

    node->next = fl;
    fl = node;
}

}  // namespace


static blk_t *newBlk(Link& link,  carve_t carver )
/************************************/
{
    blk_t           *newblk;
    blk_t           **blklist;

    newblk = static_cast<blk_t *>( MemAllocSafe(link,  carve_size( carver->blk_size ) ) );
    /*
     * keep list sorted by memory address, biggest first.
     */
    for( blklist = &carver->blk_list; *blklist > newblk; blklist = &(*blklist)->next ) {}
    newblk->next = *blklist;
    *blklist = newblk;
    carver->blk_count++;
    carver->size_chg = true;
    return( newblk );
}

static void MakeFreeList( carve_t carver, blk_t *newblk, unsigned offset )
/************************************************************************/
{
    unsigned        elm_size;
    char            *top_elm;
    char            *bottom_elm;
    char            *free_elm;
    free_t          *free_list;

    elm_size = carver->elm_size;
    bottom_elm = carve_data( newblk, offset );
    top_elm = carve_data( newblk, carver->blk_top );
    free_list = carver->free_list;
    free_elm = top_elm;
    do {                         /* free_list must be maintained in order */
        free_elm -= elm_size;
        DbgZapFreed( free_elm, elm_size );
        add_to_free( free_list, free_elm );
    } while( free_elm != bottom_elm );
    carver->free_list = free_list;
}

carve_t CarveCreate(Link& link,  unsigned elm_size, unsigned blk_size )
/*********************************************************/
{
    carve_t         carver;
    unsigned        elm_count;

    if( elm_size < sizeof( free_t ) ) {
        elm_size = sizeof( free_t );
    }
    elm_size = __ROUND_UP_SIZE( elm_size, CARVE_ALIGN );
    elm_count = blk_size / elm_size;
    carver = static_cast<carve_t>( MemAllocSafe(link,  sizeof( *carver ) ) );
    carver->elm_size = elm_size;
    carver->blk_size = blk_size;
    carver->elm_count = elm_count;
    carver->blk_top = elm_count * elm_size;
    carver->blk_count = 0;
    carver->blk_list = nullptr;
    carver->free_list = nullptr;
    carver->blk_map = nullptr;
    carver->size_chg = false;
//    DbgAssert( carver->elm_size >= 2 * CARVE_ALIGN );
    DbgAssert( carver->elm_count != 0 );
    DbgVerify( carver->blk_top < _64K, "carve: size * #/block > 64k" );
    return( carver );
}

#ifdef DEVBUILD
void CarveVerifyAllGone( carve_t carver, const char *node_name )
/**************************************************************/
{
    free_t          *check;
    blk_t           *block;
    char            *compare;
    char            buff[80];
    bool            some_unfreed;

    some_unfreed = false;
    for( block = carver->blk_list; block != nullptr; block = block->next ) {
        compare = carve_data( block, carver->blk_top );
        do {
            compare -= carver->elm_size;
            /* verify every block has been freed */
            for( check = carver->free_list; check != nullptr; check = check->next ) {
                if( compare == reinterpret_cast<char *>( check ) ) {
                    break;
                }
            }
            if( check == nullptr ) {
                if( ! some_unfreed ) {
                    FmtStr( buff, 80, "carve %s unfreed:", node_name );
                    WriteStdOut( buff );
                    some_unfreed = true;
                }
                FmtStr( buff, 80, " %h", compare );
                WriteStdOut( buff );
            }
        } while( compare != carve_data( block, 0 ) );
    }
    if( some_unfreed ) {
        WriteStdOutNL();
    }
}
#endif

void CarveDestroy( carve_t carver )
/*********************************/
{
    blk_t           *cur;
    blk_t           *next;

    if( carver != nullptr ) {
        if( carver->blk_map != nullptr ) {
            MemFree( carver->blk_map );
        }
        for( cur = carver->blk_list; cur != nullptr; cur = next ) {
            next = cur->next;
            MemFree( cur );
        }
        MemFree( carver );
    }
}

void *CarveAlloc(Link& link,  carve_t carver )
/********************************/
{
    void            *p;

    if( carver->free_list == nullptr ) {
        MakeFreeList( carver, newBlk(link,  carver ), 0 );
    }
    p = remove_from_free( carver );
    DbgZapAlloc( p, carver->elm_size );
    return( p );
}

void *CarveZeroAlloc(Link& link,  carve_t carver )
/************************************/
{
    void            **p;

    if( carver->free_list == nullptr ) {
        MakeFreeList( carver, newBlk(link,  carver ), 0 );
    }
    p = static_cast<void **>( remove_from_free( carver ) );
    DbgAssert( ( carver->elm_size / sizeof( *p ) ) <= 16 );
    /*
     * clear the element's words (wlink's carved elements are at most 16 words;
     * a larger one was left as it was)
     */
    const unsigned  words = carver->elm_size / sizeof( *p );

    if( words <= 16 ) {
        for( unsigned i = 0; i < words; ++i ) {
            p[i] = nullptr;
        }
    }
    return( p );
}


#ifdef DEVBUILD
static void CarveDebugFree( carve_t carver, void *elm )
{
    free_t          *check;
    blk_t           *block;
    char            *compare;
    char            *start;
    unsigned        esize;

    /* make sure object hasn't been freed before */
    for( check = carver->free_list; check != nullptr; check = check->next ) {
        if( elm == static_cast<void *>( check ) ) {
            LnkFatal( "carve: freed object was previously freed" );
        }
    }
    /* make sure object is from this carve allocator */
    for( block = carver->blk_list; block != nullptr; block = block->next ) {
        start = carve_data( block, 0 );
        compare = start + carver->blk_top;
#if ! ( defined(__COMPACT__) || defined(__LARGE__) )
        /* quick check */
        if( static_cast<char *>( elm ) < start
          || static_cast<char *>( elm ) > compare ) {
            continue;
        }
#endif
        esize = carver->elm_size;
        for( ;; ) {
            if( compare == start )
                break;
            compare -= esize;
            if( static_cast<char *>( elm ) == compare ) {
                break;
            }
        }
        if( static_cast<char *>( elm ) == compare ) {
            break;
        }
    }
    if( block == nullptr ) {
        LnkFatal( "carve: freed object was never allocated" );
    }
    DbgZapFreed( elm, carver->elm_size );
}
#else
#define CarveDebugFree( carver, elm )
#endif

void CarveFree( carve_t carver, void *elm )
/*****************************************/
{
    if( elm == nullptr ) {
        return;
    }
    CarveDebugFree( carver, elm );
    add_to_free( carver->free_list, elm );
}

void *CarveGetIndex( carve_t carver, void *elm )
/***********************************************
 * note this assumes carve block list sorted by size, biggest first
 */
{
    blk_t           *block;
    unsigned        block_index;

    if( elm == nullptr ) {
        return( reinterpret_cast<void *>( static_cast<pointer_uint>( CARVE_NULL_INDEX ) ) );
    }
    block_index = carver->blk_count;
    for( block = carver->blk_list; elm < static_cast<void *>( block ); block = block->next ) {
        --block_index;
    }
    DbgAssert( block != nullptr );
    return( reinterpret_cast<void *>( static_cast<pointer_uint>( MK_INDEX( block_index, static_cast<char *>( elm ) - carve_data( block, 0 ) ) ) ) );
}

void CarveWalkBlocks( carve_t carver, FunctionRef<void(carve_t, void *, void *)> cbfn, void *cookie )
/*****************************************************************************************/
{
    blk_t           *block;

    for( block = carver->blk_list; block != nullptr; block = block->next ) {
        cbfn( carver, block, cookie );
    }
}

bool CarveBlockModified( void *blk )
/**********************************/
{
    return( static_cast<blk_t *>( blk )->modified != 0 );
}

void CarveBlockScan( carve_t carver, void *blk, FunctionRef<void(void *, void *)> rtn, void *data )
/***************************************************************************************/
{
    char            *compare;
    char            *end;
    unsigned        esize;

    esize = carver->elm_size;
    compare = carve_data( blk, 0 );
    end = compare + carver->blk_top;
    do {
        rtn( compare, data );
        compare += esize;
    } while( compare != end );
}

unsigned CarveBlockSize( carve_t carver )
/***************************************/
{
    return( carver->blk_size );
}

void *CarveBlockData( void *block )
/*********************************/
{
    return( carve_data( block, 0 ) );
}

bool CarveSizeChanged( carve_t carver )
/*************************************/
{
    return( carver->size_chg != 0 );
}

unsigned CarveNumElements( carve_t carver )
/*****************************************/
{
    return( carver->blk_count * carver->elm_count );
}

void CarveWalkAllFree( carve_t carver, void (*rtn)( void * ) )
/************************************************************/
{
    free_t          *check;

    for( check = carver->free_list; check != nullptr; check = check->next ) {
#ifdef DEVBUILD
        free_t *check_next = check->next;
        (*rtn)( check );
        if( check->next != check_next ) {
            LnkFatal( "carve walk free routine destroyed links" );
        }
#else
        (*rtn)( check );
#endif
    }
}

void CarveWalkAll( carve_t carver, FunctionRef<void( void *, void * )> rtn, void *data )
/****************************************************************************/
{
    blk_t           *block;

    for( block = carver->blk_list; block != nullptr; block = block->next ) {
        CarveBlockScan( carver, block, rtn, data );
    }
}

void CarveRestart(Link& link,  carve_t carver, unsigned num )
/***********************************************/
{
    unsigned        numblks;
    unsigned        remainder;
    unsigned        index;
    blk_t           *block;

    if( num == 0 )
        return;
    numblks = (num + carver->elm_count - 1) / carver->elm_count;
    for( index = 0; index < numblks; index++ ) {
        newBlk(link,  carver );
    }
    carver->blk_map = static_cast<blk_t **>( MemAllocSafe(link,  numblks * sizeof( *carver->blk_map ) ) );
    index = numblks - 1;
    for( block = carver->blk_list; block != nullptr; block = block->next ) {
        carver->blk_map[index] = block;
        index -= 1;
    }
    remainder = num % carver->elm_count;
    if( remainder != 0 ) {
        MakeFreeList( carver, carver->blk_map[0], remainder * carver->elm_size );
    }
    carver->insert = nullptr;
}

static void CarveZapBlock( carve_t carver, void *blk, void *dummy )
/*****************************************************************/
{
    /* unused parameters */ (void)dummy;

    MakeFreeList( carver, static_cast<blk_t *>( blk ), 0 );
}

void CarvePurge( carve_t carver )
/********************************
 * clean out a carve block that had been prepared for incremental linking
 */
{
    carver->free_list = nullptr;
    CarveWalkBlocks( carver, CarveZapBlock, nullptr );
}

void CarveInsertFree( carve_t carver, void *data )
/************************************************/
{
    free_t          *freeblk;

    freeblk = static_cast<free_t *>( data );
    if( carver->insert == nullptr ) {
        freeblk->next = carver->free_list;
        carver->free_list = freeblk;
    } else {
        freeblk->next = carver->insert->next;
        carver->insert->next = freeblk;
    }
    carver->insert = freeblk;
}

void *CarveMapIndex( carve_t carver, void *_index )
/*************************************************/
{
    unsigned        index = static_cast<unsigned>( reinterpret_cast<pointer_uint>( _index ) );
    blk_t           *block;
    blk_t           **block_map;
    unsigned        block_index;
    unsigned        block_offset;

    /* given an index; find and allocate the carve element */
    if( index == CARVE_NULL_INDEX ) {
        return( nullptr );
    }
    block_index = GET_BLOCK( index );
    block_offset = GET_OFFSET( index );
    block_map = carver->blk_map;
    block = block_map[block_index - 1];
    return( carve_data( block, block_offset ) );
}
