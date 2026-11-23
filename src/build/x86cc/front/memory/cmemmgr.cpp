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
* Description:  C compiler memory management routines.
*
****************************************************************************/


#include "cvars.h"
#include "cgdefs.h"
#include "feprotos.h"
#include "roundmac.h"
#include "frontrun.hpp"


/*
 * RISC platforms are likely to take a big hit for misaligned accesses.
 * Memory blocks are ordinarily aligned to sizeof( unsigned ) which is
 * OK for many platforms, but notably on Alpha we need 8-byte alignment
 * to make sure doubles won't get us in trouble.
 * NB: MEM_ALIGN must be at least int-sized
 */

    #define MEM_ALIGN   sizeof( void * )

#define ALLOC_FLAG      1

#define BLK_HDR_SIZE    RoundUpSize( sizeof( mem_blk ), MEM_ALIGN )
inline unsigned long long BlkSize( unsigned long long s ) { return (BLK_HDR_SIZE + (s)); }
inline char *BlkData( mem_blk *m, size_t x ) { return ((char *)(m) + BLK_HDR_SIZE + (x)); }

#define MCB_HDR_SIZE    RoundUpSize( sizeof( MCB ), MEM_ALIGN )
inline unsigned long long McbSize( size_t s ) { return (MCB_HDR_SIZE + (s)); }
inline char *McbData( mem_block *m, int x ) { return ((char *)(m) + MCB_HDR_SIZE + (x)); }
inline mem_block *McbFromC( void *p ) { return ((MCB *)((char *)(p) - MCB_HDR_SIZE)); }
inline mem_block *McbNext( mem_block *x ) { return ((MCB *)((char *)(x) + ((x)->size & ~ALLOC_FLAG))); }

/*
 * Size of permanent area. Needs to be reasonably big to satisfy
 * large allocation requests. Must by multiple of 0x20
 */
#define MAX_PERM_SIZE   0x00100000

#define PERM_BLK_END    (0 | ALLOC_FLAG)







/*
 *  variables used:
 *      char *PermPtr;          first free byte in permanent area block
 *      size_t PermSize;        total size of permanent area block
 *      size_t PermAvail;       number of bytes available in permanent area block
 *
 *  Permanent memory is allocated from low to high.
 *  Memory allocated using CPermAlloc is never given back.
 *  Temporary allocations are made at the high end of permanent memory.
*/


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CmemmgrState: cmemmgr_state.h) */

}  // namespace



static void InitPermArea( FrontRun& front )
/******************************/
{
    front.cmemmgr.Blks = NULL;
    front.cmemmgr.PermAvail = 0;
    front.cmemmgr.PermPtr = NULL;
    front.cmemmgr.PermSize = 0;
}

static void FiniPermArea( FrontRun& front )
/******************************/
{
    mem_blk *curr;
    mem_blk *next;

    for( curr = front.cmemmgr.Blks; curr != NULL; curr = next ) {
        next = curr->next;
        front.held->free( curr );
    }
    front.cmemmgr.Blks = NULL;
}

static void addMCBtoFreeList( FrontRun& front, MCB *mcb )
/**************************************/
{
    MCB     *mcb_prev;
    MCB     *mcb_next;

    mcb->size &= ~ALLOC_FLAG;
    mcb_prev = &front.cmemmgr.CFreeList;
    for( ;; ) {         /* insert in sorted order */
        mcb_next = mcb_prev;
        mcb_prev = mcb_prev->prev;
        if( mcb_prev == &front.cmemmgr.CFreeList )
            break;
        if( mcb_prev < mcb ) {
            break;
        }
    }
    mcb_next->prev = mcb;
    mcb_prev->next = mcb;
    mcb->prev = mcb_prev;
    mcb->next = mcb_next;
}

static void removeMCBfromFreeList( MCB *mcb )
/*******************************************/
{
    MCB     *mcb_prev;
    MCB     *mcb_next;

    mcb_next = mcb->next;
    mcb_prev = mcb->prev;
    mcb_prev->next = mcb_next;
    mcb_next->prev = mcb_prev;
}

static void AllocPermArea( FrontRun& front )
/*******************************/
{
    for( front.cmemmgr.PermSize = MAX_PERM_SIZE; front.cmemmgr.PermSize != 0; front.cmemmgr.PermSize -= 0x20 ) {
        mem_blk *blk;
        blk = static_cast<mem_blk *>(front.held->calloc( 1, BlkSize( front.cmemmgr.PermSize + sizeof( blk->size ) ) ));
        if( blk != NULL ) {
            blk->next = front.cmemmgr.Blks;
            front.cmemmgr.Blks = blk;
            blk->ptr = front.cmemmgr.PermPtr;
            blk->size = front.cmemmgr.PermSize;
            ((MCB *)BlkData( blk, front.cmemmgr.PermSize ))->size = PERM_BLK_END;    /* null length tag */
            front.cmemmgr.PermPtr = BlkData( blk, 0 );
            front.cmemmgr.PermAvail = front.cmemmgr.PermSize;
            return;
        }
    }
    front.cmemmgr.PermPtr = NULL;
    front.cmemmgr.PermAvail = front.cmemmgr.PermSize = 0;
}


void CMemInit( FrontRun& front )
/*******************/
{
    InitPermArea(front);
    front.cmemmgr.CFreeList.size = 0;
    front.cmemmgr.CFreeList.next = &front.cmemmgr.CFreeList;
    front.cmemmgr.CFreeList.prev = &front.cmemmgr.CFreeList;
}

void CMemFini( FrontRun& front )
/*******************/
{
    front.cmemmgr.CFreeList.size = 0;
    front.cmemmgr.CFreeList.next = &front.cmemmgr.CFreeList;
    front.cmemmgr.CFreeList.prev = &front.cmemmgr.CFreeList;
    FiniPermArea(front);
}

static void Ccoalesce( MCB *mcb1 )
/********************************/
{
    MCB *mcb2;

    for( ;; ) {
        mcb2 = McbNext( mcb1 );
        /*
         * quit if next block not free
         * or if no more blocks follow in permanet block
         */
        if( mcb2->size & ALLOC_FLAG )
            break;
        /*
         * coalesce mcb1 and mcb2 and remove mcb2 from free list
         */
        mcb1->size += mcb2->size;
        removeMCBfromFreeList( mcb2 );
    }
}

static void *CFastAlloc( FrontRun& front, size_t size )
/************************************/
{
    size_t      mcb_size;
    MCB         *mcb;

    size = McbSize( size );
    if( size < sizeof( MCB ) )
        size = sizeof( MCB );
    mcb_size = RoundUp( size, MEM_ALIGN );
    /*
     * search free list before getting memory from permanent area
     */
    for( mcb = front.cmemmgr.CFreeList.prev; mcb != &front.cmemmgr.CFreeList; mcb = mcb->prev ) {
        Ccoalesce( mcb );
        if( mcb->size >= mcb_size ) {
            if( mcb->size - mcb_size > McbSize( 0 ) ) {
                /*
                 * block is big enough to split it
                 */
                mcb->size -= mcb_size;
                mcb = McbNext( mcb );
                mcb->size = mcb_size;
            } else {
                removeMCBfromFreeList( mcb );
            }
            mcb->size |= ALLOC_FLAG;      /* indicate block allocated */
            return( McbData( mcb, 0 ) );
        }
    }
    if( mcb_size > front.cmemmgr.PermAvail )
        return( NULL );
    front.cmemmgr.PermAvail -= mcb_size;
    mcb = (MCB *)( front.cmemmgr.PermPtr + front.cmemmgr.PermAvail );
    mcb->size = mcb_size | ALLOC_FLAG;
    return( McbData( mcb, 0 ) );
}


void *CMemAlloc( FrontRun& front, size_t size )
/****************************/
{
    void    *p;

    p = CFastAlloc( front, size );
    if( p == NULL ) {
        AllocPermArea(front);            /* allocate another permanent area block */
        p = CFastAlloc( front, size );
        if( p == NULL ) {
            CErr1( front, ERR_OUT_OF_MEMORY );
            CSuicide(front);
        }
    }
    /*
     * make sure pointer is properly aligned
     */
    assert( ((unsigned)(pointer_uint)p & (MEM_ALIGN - 1)) == 0 );

    return( memset( p, 0, size ) );
}

char *ToStringDup( FrontRun& front, const char *buf, size_t len )
/**********************************************/
{
    char    *str;

    str = static_cast<char *>(CMemAlloc( front, len + 1 ));
    if( str != NULL ) {
        memcpy( str, buf, len );
        str[len] = '\0';
    }
    return( str );
}

char *CMemStrdup( FrontRun& front, const char *buf )
/*********************************/
{
    if( buf != NULL ) {
        return( strcpy( static_cast<char *>(CMemAlloc( front, strlen( buf ) + 1 )), buf ) );
    }
    return( NULL );
}

void *CMemRealloc( FrontRun& front, void *old_p, size_t size )
/*******************************************/
{
    void            *p;
    size_t          old_size;
    MCB             *mcb;

    if( old_p == NULL )
        return( CMemAlloc( front, size ) );

    mcb = McbFromC( old_p );
    old_size = ((mcb)->size & ~ALLOC_FLAG) - McbSize( 0 );
    if( size > old_size ) {
        p = CMemAlloc( front, size );
        memcpy( p, old_p, old_size );
        CMemFree( front, old_p );
    } else {
        /*
         * the current block is big enough -- nothing to do (very lazy realloc)
         */
        p = old_p;
    }
    return( p );
}

static enum cmem_kind CMemKind( FrontRun& front, const char *p )
/*********************************************/
{
    char            *ptr;
    size_t          size;
    mem_blk         *blk;

    ptr = front.cmemmgr.PermPtr;
    size = front.cmemmgr.PermSize;
    for( blk = front.cmemmgr.Blks; blk != NULL; blk = blk->next ) {
        if( p > (char *)blk ) {
            /*
             * check if permanent memory (from beginning of block)
             */
            if( p < ptr ) {
                return( CMEM_PERM );
            }
            /*
             * check if dynamic memory (from end of block)
             */
            if( p < BlkData( blk, size ) ) {
                return( CMEM_MEM );
            }
        }
        ptr = blk->ptr;
        size = blk->size;
    }
    return( CMEM_NONE );
}

void CMemFree( FrontRun& front, void *p )
/**********************/
{
    MCB         *mcb;

    /*
     * Should try and get rid of these error cases
     *
     * ignore precompiled header memory block
     */
    if( p != NULL && ( front.cvars.PCH_Start == NULL || (char *)p < front.cvars.PCH_Start || (char *)p >= front.cvars.PCH_End ) ) {
        if( CMemKind( front, static_cast<const char *>(p) ) == CMEM_MEM ) {
            mcb = McbFromC( p );
            if( (char *)mcb == front.cmemmgr.PermPtr + front.cmemmgr.PermAvail ) {
                front.cmemmgr.PermAvail += mcb->size & ~ALLOC_FLAG;
                mcb = McbNext( mcb );
                if( (mcb->size & ALLOC_FLAG) == 0 ) {
                    if( (char *)mcb == front.cmemmgr.PermPtr + front.cmemmgr.PermAvail ) {
                        front.cmemmgr.PermAvail += mcb->size;
                        removeMCBfromFreeList( mcb );
                    }
                }
            } else {
                addMCBtoFreeList( front, mcb );
                Ccoalesce( mcb );
            }
        }
    }
}


void *CPermAlloc( FrontRun& front, size_t size )
/*****************************/
{
    void    *p;

    size = RoundUp( size, MEM_ALIGN );
    if( size > front.cmemmgr.PermAvail ) {
        if( size <= MAX_PERM_SIZE ) {
            /*
             * allocate another permanent area block
             */
            AllocPermArea(front);
        }
        if( size > front.cmemmgr.PermAvail ) {
            CErr1( front, ERR_OUT_OF_MEMORY );
            CSuicide(front);
        }
    }
    front.cmemmgr.PermAvail -= size;
    p = front.cmemmgr.PermPtr;
    front.cmemmgr.PermPtr += size;
    return( memset( p, 0, size ) );
}


void *FEmalloc( FrontRun& front, size_t size )
/***************************/
{
    void    *p;

    p = front.held->calloc( 1, size );   /* (zeroed: see run/cbrun.hpp) */
    if( p == NULL ) {
        CErr1( front, ERR_OUT_OF_MEMORY );
        CSuicide(front);
    }
    return( p );
}


void FEfree( FrontRun& front, void *p )
/********************/
{
    if( p != NULL ) {
        front.held->free( p );
    }
}


int FEMoreMem( size_t size )
/**************************/
{
    /* unused parameters */ (void)size;

    return( 0 );
}
