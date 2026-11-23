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
* Description:  Fast memory manager used by the code generator.
*
****************************************************************************/


/****************************************************************************

This is a free list based memory manager optimized for codegen performance.
It will allocate fixed size blocks of several classes (sizes), any freed
memory blocks will be kept in a singly linked free list. If an attempt is
made to allocate memory and a free block of appropriate size exists, it will
be found extremely quickly. The memory manager allocates memory from the
system via malloc() and free(), but it could use OS functions that allocate
memory pages directly.

Memory classes/sizes are powers of two, but they might be arbitrary sizes
designed to fit typical allocation sizes. Allocations greater than certain
maximum size will go directly to the OS.

Expected usage is great number of mostly small allocs and frees. The
allocator is designed to reuse memory well, and do it fast.

Disadvantages: May waste memory in certain scenarios. No attempts are made
to coalesce free memory blocks. Since the codegen extremely rarely allocates
more than even 256KB memory, some wastage is considered acceptable.

Advantages: Speed. Memory allocation is extremely fast, with no searches if
appropriately sized block is available. Memory frees are instantaneous, with
no searching whatsoever. Did I mention speed? Memory blocks will be reused
immediately, which should produce helpful memory access patterns. There is
essentially no worst case performance scenario.

****************************************************************************/


#include "_cgstd.h"
#include <stdlib.h>
#include <stdio.h>
    #include <windows.h>
#include "roundmac.h"
#include "_cg.h"
#include "utils.h"
#include "onexit.h"
#include "memsydep.h"
#include "feprotos.h"
#include "backrun.hpp"

#define ALLOCATED       1

#define MAX_SIZE        14  /* 16384 (2 ^ 14) */
#define MIN_SIZE        4   /* 16    (2 ^ 4) */
#define MAX_CLASS       (MAX_SIZE - MIN_SIZE)

#define MEM_WORD_SIZE   16  /* Needed to keep alignment. */
#define TAG_SIZE        8


#define MIN_ALLOC       RoundUp( sizeof( frl ), MEM_WORD_SIZE )
#define MAX_ALLOC       (1 << MAX_SIZE)

typedef pointer_uint    tag;

static pointer          MemFromSys( BackRun& back, size_t );



/* Free list structure - length holds the size of memory block, which
 * is necessary for freeing memory. Note that the length field is set
 * when the block is first allocated and never changes afterwards.
 * The link member holds the address of the next free block of the
 * same size, or NULL if it's the last free block.
 */
typedef struct frl {
    tag         length;
    struct frl  *link;
} frl;

/* Memory block structure - memory is allocated from the OS in large
 * chunks (perhaps 64K, perhaps more or less than that). If there is
 * no memory in a free list, it will be allocated from a block, and
 * will end up on a free list when (if) it's freed.
 */
typedef struct mem_blk {
    struct mem_blk  *next;
    tag             free;
    tag             size;   /* This must be the last member! */
} mem_blk;

typedef struct blk_hdr {
    struct mem_blk  *block;
    tag             size;   /* This must be the last member! */
} blk_hdr;

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct CgmemmgrState {
    pointer_uint AllocSize = 0;
    pointer_uint MemorySize;
    int Initialized = 0;
    mem_blk *_Blks;
    frl *_FreeList[MAX_CLASS + 1];
    unsigned _ClassSizes[MAX_CLASS + 1];
};
namespace {

}  // namespace





static int myatoi( const char *p )
/********************************/
{
    int         i;

    i = 0;
    while( *p != '\0' ) {
        i *= 10;
        i += *p - '0';
        ++p;
    }
    return( i );
}


static  void    CalcMemSize( BackRun& back )
/*********************************/
{
    bool            max_size_queried;
    bool            size_queried;
    pointer_uint    size_requested;
    pointer_uint    memory_available;
    char            buff[80];
    const char      *envvar;

    back.cgmemmgr->Initialized = 2;
    size_requested = 0;
    size_queried = false;
    max_size_queried = false;

    envvar = FEGetEnv( *back.front, "WCGMEMORY" );
    if( envvar != NULL ) {
        if( envvar[0] == '?' && envvar[1] == '\0' ) {
            max_size_queried = true;
        } else if( envvar[0] == '#' && envvar[1] == '\0' ) {
            size_queried = true;
        } else {
            size_requested = myatoi( envvar ) * _1K;
        }
    }
    {
        MEMORYSTATUS    info;

        GlobalMemoryStatus( &info );
        memory_available = info.dwAvailVirtual;
        if( size_requested != 0 ) {
            back.cgmemmgr->MemorySize = size_requested;
        } else {
            back.cgmemmgr->MemorySize = memory_available;
        }
    }
    if( max_size_queried || size_queried ) {
        sprintf( buff, "Maximum WCGMEMORY=%d\n", static_cast<int>( memory_available / _1K ) );
        FEMessage( *back.front, FEMSG_INFO, buff );
    }
}

static  void    memInit( BackRun& back )
/*****************************/
{
    if( back.cgmemmgr->Initialized == 0 ) {
        int i;

        for( i = 0; i <= MAX_CLASS; ++i ) {
            back.cgmemmgr->_FreeList[i]   = NULL;
            back.cgmemmgr->_ClassSizes[i] = 1 << ( MIN_SIZE + i );
        }

        back.cgmemmgr->Initialized = 1;
        back.cgmemmgr->AllocSize   = 0;
        back.cgmemmgr->_Blks       = NULL;
        CalcMemSize(back);
    }
}


static int SizeToClass( size_t amount )
/*************************************/
{
    int mem_class;

    if( amount >= MAX_ALLOC ) {
        return( MAX_CLASS );
    }
    if( amount <= ( 1 << MIN_SIZE ) ) {
        return( 0 );
    }
    --amount;
    amount >>= MIN_SIZE;
    mem_class = 0;
    for( ;; ) {
        ++mem_class;
        amount >>= 1;
        if( amount == 0 ) {
            break;
        }
    }
    return( mem_class );
}


static unsigned ClassToSize( BackRun& back, int mem_class )
/******************************************/
{
    return( back.cgmemmgr->_ClassSizes[mem_class] );
}


static pointer  GetFromFrl( BackRun& back, size_t amount, int frl_class )
/********************************************************/
{
    frl     *free;

    /* unused parameters */ (void)amount;

    free = back.cgmemmgr->_FreeList[frl_class];
    if( free != NULL ) {
        free->length |= ALLOCATED;
        back.cgmemmgr->_FreeList[frl_class] = free->link;
        return( free );
    }
    return( NULL );
}


static pointer  GetFromBlk( BackRun& back, size_t amount )
/*****************************************/
{
    mem_blk     *block;
    tag         *alloc;

    for( block = back.cgmemmgr->_Blks; block != NULL; block = block->next ) {
        if( block->free >= amount ) {
            alloc = reinterpret_cast<tag *>( reinterpret_cast<char *>(block) + block->size - block->free );
            *alloc = amount | ALLOCATED;
            block->free -= amount;
            return( ++alloc );
        }
    }
    if( MemFromSys( back, 0 ) == NULL)    // Grab one empty block
        return( NULL );
    return( GetFromBlk( back, amount ) ); // We know this will now succeed
}


pointer  CgMemAlloc( BackRun& back, size_t amount )
/**********************************/
{
    char        *chunk;
    int         mem_class;

    if( back.cgmemmgr->Initialized == 0 )
        memInit(back);
    if( amount == 0 )
        return( NULL );
    amount = RoundUp( amount + TAG_SIZE, MEM_WORD_SIZE );
    if( amount > MAX_ALLOC ) {
        return( MemFromSys( back, amount ) );
    }

    if( amount < MIN_ALLOC )
        amount = MIN_ALLOC;

    mem_class = SizeToClass( amount );
    chunk = static_cast<char *>(GetFromFrl( back, amount, mem_class ));
    if( chunk != NULL )
        return( chunk + TAG_SIZE );

    return( GetFromBlk( back, ClassToSize( back, mem_class ) ) );
}


void     CgMemFree( BackRun& back, pointer p )
/*****************************/
{
    frl     *free;
    int     mem_class;
    tag     length;

    free   = reinterpret_cast<frl *>( static_cast<char *>(p) - TAG_SIZE );
    free->length &= ~ALLOCATED;
    length = free->length;
    if( length > MAX_ALLOC ) {   // This was a full block
        blk_hdr     *header;
        mem_blk     *blk;

        header = reinterpret_cast<blk_hdr *>( static_cast<char *>(p) - sizeof( blk_hdr ) );
        blk    = header->block;
        blk->free += header->size + sizeof( blk_hdr );
        blk->size += header->size + sizeof( blk_hdr );
    } else {
        mem_class  = SizeToClass( length );
        free->link = back.cgmemmgr->_FreeList[mem_class];
        back.cgmemmgr->_FreeList[mem_class] = free;
    }
}

void     MemCoalesce( void )
/**************************/
{
    return;
}


pointer_uint     MemInUse( BackRun& back )
/*******************************/
{
    if( back.cgmemmgr->Initialized == 0 )
        memInit(back);
    return( back.cgmemmgr->AllocSize );
}


pointer_uint     MemSize( BackRun& back )
/******************************/
{
    switch( back.cgmemmgr->Initialized ) {
    case 0:
        memInit(back);
    }
    return( back.cgmemmgr->MemorySize );
}


static  pointer MemFromSys( BackRun& back, size_t amount )
/*****************************************/
{
    pointer     ptr;
    blk_hdr     *allocated;
    mem_blk     *blk;
    size_t      size;

    /*
     * round up size to multiple of 64K
     */
    size = RoundUp( amount + sizeof( mem_blk ) + sizeof( blk_hdr ), _64K );
    /*
     * Need to clear memory for later assert() calls, and a compile reads what it never wrote as zero (run/cbrun.hpp)
     */
    ptr = back.held->calloc( 1, size );
    if( ptr != NULL ) {
        back.cgmemmgr->AllocSize += size;
        blk = static_cast<mem_blk *>(ptr);
        /*
         * If amount was zero, this block will be chopped up into
         * small chunks immediately. If nonzero, return pointer
         * to memory at the end of the block and use the rest for
         * small chunks. If the initial large allocation is freed,
         * it will also be reused for small blocks.
         */
        blk->next = back.cgmemmgr->_Blks;
        back.cgmemmgr->_Blks = blk;
        blk->free = size - amount - sizeof( mem_blk );
        blk->size = size;
        if( amount ) {
            blk->size -= amount + sizeof( blk_hdr );
            blk->free -= sizeof( blk_hdr );
            allocated = reinterpret_cast<blk_hdr *>(reinterpret_cast<char *>(blk) + sizeof( mem_blk ) + blk->free);
            allocated->block = blk;
            allocated->size  = amount | ALLOCATED;
            ptr = reinterpret_cast<char *>(allocated) + sizeof( blk_hdr );
        } else {
            ptr = reinterpret_cast<char *>(blk) + sizeof( mem_blk );
        }
        return( ptr );
    }
    return( NULL );
}


static  void MemToSys( BackRun& back, mem_blk *what )
/************************************/
{
    back.held->free( what );
}


void MemFini( BackRun& back )
/******************/
{
    mem_blk     *curr;
    mem_blk     *next;

    for( curr = back.cgmemmgr->_Blks; curr != NULL; curr = next ) {
        next = curr->next;
        MemToSys( back, curr );
    }
    back.cgmemmgr->Initialized = 0;
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
CgmemmgrState* cgmemmgr_make() { return new CgmemmgrState(); }
void cgmemmgr_free(CgmemmgrState* part) { delete part; }
