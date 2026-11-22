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


/*
 * RISC platforms are likely to take a big hit for misaligned accesses.
 * Memory blocks are ordinarily aligned to sizeof( unsigned ) which is
 * OK for many platforms, but notably on Alpha we need 8-byte alignment
 * to make sure doubles won't get us in trouble.
 * NB: MEM_ALIGN must be at least int-sized
 */

#if defined( __AXP__ )
    #define MEM_ALIGN   8
#else
    #define MEM_ALIGN   sizeof( void * )
#endif

#define ALLOC_FLAG      1

#define BLK_HDR_SIZE    __ROUND_UP_SIZE( sizeof( mem_blk ), MEM_ALIGN )
#define BLK_SIZE(s)     (BLK_HDR_SIZE + (s))
#define BLK_DATA(m,x)   ((char *)(m) + BLK_HDR_SIZE + (x))

#define MCB_HDR_SIZE    __ROUND_UP_SIZE( sizeof( MCB ), MEM_ALIGN )
#define MCB_SIZE(s)     (MCB_HDR_SIZE + (s))
#define MCB_DATA(m,x)   ((char *)(m) + MCB_HDR_SIZE + (x))
#define MCB_FROM_C(p)   ((MCB *)((char *)(p) - MCB_HDR_SIZE))
#define MCB_NEXT(x)     ((MCB *)((char *)(x) + ((x)->size & ~ALLOC_FLAG)))

/*
 * Size of permanent area. Needs to be reasonably big to satisfy
 * large allocation requests. Must by multiple of 0x20
 */
#define MAX_PERM_SIZE   0x00100000

#define PERM_BLK_END    (0 | ALLOC_FLAG)

enum cmem_kind {
    CMEM_PERM,
    CMEM_MEM,
    CMEM_NONE,
};

typedef struct  mem_block {
    size_t              size;   /* length of stg */
    struct mem_block    *prev;  /* pointer to previous free memory block */
    struct mem_block    *next;  /* pointer to next     free memory block */
} MCB;

typedef struct mem_blk {
    struct mem_blk      *next;
    char                *ptr;   // old permanent area block pointer
    size_t              size;   // old permanent area block size
} mem_blk;

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
struct CmemmgrState {
    char *PermPtr;   /* first free byte in permanent area block */
    size_t PermSize;   /* total size of permanent area block */
    size_t PermAvail;   /* # of bytes available in permanent area block */
    MCB CFreeList;
    mem_blk *Blks;
};

}  // namespace

static inline CmemmgrState& cmemmgr_state() { return cbrun_part<CmemmgrState>(); }


static void InitPermArea( void )
/******************************/
{
    cmemmgr_state().Blks = NULL;
    cmemmgr_state().PermAvail = 0;
    cmemmgr_state().PermPtr = NULL;
    cmemmgr_state().PermSize = 0;
}

static void FiniPermArea( void )
/******************************/
{
    mem_blk *curr;
    mem_blk *next;

    for( curr = cmemmgr_state().Blks; curr != NULL; curr = next ) {
        next = curr->next;
        free( curr );
    }
    cmemmgr_state().Blks = NULL;
}

static void addMCBtoFreeList( MCB *mcb )
/**************************************/
{
    MCB     *mcb_prev;
    MCB     *mcb_next;

    mcb->size &= ~ALLOC_FLAG;
    mcb_prev = &cmemmgr_state().CFreeList;
    for( ;; ) {         /* insert in sorted order */
        mcb_next = mcb_prev;
        mcb_prev = mcb_prev->prev;
        if( mcb_prev == &cmemmgr_state().CFreeList )
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

static void AllocPermArea( void )
/*******************************/
{
    for( cmemmgr_state().PermSize = MAX_PERM_SIZE; cmemmgr_state().PermSize != 0; cmemmgr_state().PermSize -= 0x20 ) {
        mem_blk *blk;
        blk = static_cast<mem_blk *>(calloc( 1, BLK_SIZE( cmemmgr_state().PermSize + sizeof( blk->size ) ) ));
        if( blk != NULL ) {
            blk->next = cmemmgr_state().Blks;
            cmemmgr_state().Blks = blk;
            blk->ptr = cmemmgr_state().PermPtr;
            blk->size = cmemmgr_state().PermSize;
            ((MCB *)BLK_DATA( blk, cmemmgr_state().PermSize ))->size = PERM_BLK_END;    /* null length tag */
            cmemmgr_state().PermPtr = BLK_DATA( blk, 0 );
            cmemmgr_state().PermAvail = cmemmgr_state().PermSize;
            return;
        }
    }
    cmemmgr_state().PermPtr = NULL;
    cmemmgr_state().PermAvail = cmemmgr_state().PermSize = 0;
}


void CMemInit( void )
/*******************/
{
    InitPermArea();
    cmemmgr_state().CFreeList.size = 0;
    cmemmgr_state().CFreeList.next = &cmemmgr_state().CFreeList;
    cmemmgr_state().CFreeList.prev = &cmemmgr_state().CFreeList;
}

void CMemFini( void )
/*******************/
{
    cmemmgr_state().CFreeList.size = 0;
    cmemmgr_state().CFreeList.next = &cmemmgr_state().CFreeList;
    cmemmgr_state().CFreeList.prev = &cmemmgr_state().CFreeList;
    FiniPermArea();
}

static void Ccoalesce( MCB *mcb1 )
/********************************/
{
    MCB *mcb2;

    for( ;; ) {
        mcb2 = MCB_NEXT( mcb1 );
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

static void *CFastAlloc( size_t size )
/************************************/
{
    size_t      mcb_size;
    MCB         *mcb;

    size = MCB_SIZE( size );
    if( size < sizeof( MCB ) )
        size = sizeof( MCB );
    mcb_size = _RoundUp( size, MEM_ALIGN );
    /*
     * search free list before getting memory from permanent area
     */
    for( mcb = cmemmgr_state().CFreeList.prev; mcb != &cmemmgr_state().CFreeList; mcb = mcb->prev ) {
        Ccoalesce( mcb );
        if( mcb->size >= mcb_size ) {
            if( mcb->size - mcb_size > MCB_SIZE( 0 ) ) {
                /*
                 * block is big enough to split it
                 */
                mcb->size -= mcb_size;
                mcb = MCB_NEXT( mcb );
                mcb->size = mcb_size;
            } else {
                removeMCBfromFreeList( mcb );
            }
            mcb->size |= ALLOC_FLAG;      /* indicate block allocated */
            return( MCB_DATA( mcb, 0 ) );
        }
    }
    if( mcb_size > cmemmgr_state().PermAvail )
        return( NULL );
    cmemmgr_state().PermAvail -= mcb_size;
    mcb = (MCB *)( cmemmgr_state().PermPtr + cmemmgr_state().PermAvail );
    mcb->size = mcb_size | ALLOC_FLAG;
    return( MCB_DATA( mcb, 0 ) );
}


void *CMemAlloc( size_t size )
/****************************/
{
    void    *p;

    p = CFastAlloc( size );
    if( p == NULL ) {
        AllocPermArea();            /* allocate another permanent area block */
        p = CFastAlloc( size );
        if( p == NULL ) {
            CErr1( ERR_OUT_OF_MEMORY );
            CSuicide();
        }
    }
    /*
     * make sure pointer is properly aligned
     */
    assert( ((unsigned)(pointer_uint)p & (MEM_ALIGN - 1)) == 0 );

    return( memset( p, 0, size ) );
}

char *ToStringDup( const char *buf, size_t len )
/**********************************************/
{
    char    *str;

    str = static_cast<char *>(CMemAlloc( len + 1 ));
    if( str != NULL ) {
        memcpy( str, buf, len );
        str[len] = '\0';
    }
    return( str );
}

char *CMemStrdup( const char *buf )
/*********************************/
{
    if( buf != NULL ) {
        return( strcpy( static_cast<char *>(CMemAlloc( strlen( buf ) + 1 )), buf ) );
    }
    return( NULL );
}

void *CMemRealloc( void *old_p, size_t size )
/*******************************************/
{
    void            *p;
    size_t          old_size;
    MCB             *mcb;

    if( old_p == NULL )
        return( CMemAlloc( size ) );

    mcb = MCB_FROM_C( old_p );
    old_size = ((mcb)->size & ~ALLOC_FLAG) - MCB_SIZE( 0 );
    if( size > old_size ) {
        p = CMemAlloc( size );
        memcpy( p, old_p, old_size );
        CMemFree( old_p );
    } else {
        /*
         * the current block is big enough -- nothing to do (very lazy realloc)
         */
        p = old_p;
    }
    return( p );
}

static enum cmem_kind CMemKind( const char *p )
/*********************************************/
{
    char            *ptr;
    size_t          size;
    mem_blk         *blk;

    ptr = cmemmgr_state().PermPtr;
    size = cmemmgr_state().PermSize;
    for( blk = cmemmgr_state().Blks; blk != NULL; blk = blk->next ) {
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
            if( p < BLK_DATA( blk, size ) ) {
                return( CMEM_MEM );
            }
        }
        ptr = blk->ptr;
        size = blk->size;
    }
    return( CMEM_NONE );
}

void CMemFree( void *p )
/**********************/
{
    MCB         *mcb;

    /*
     * Should try and get rid of these error cases
     *
     * ignore precompiled header memory block
     */
    if( p != NULL && ( cvars_state().PCH_Start == NULL || (char *)p < cvars_state().PCH_Start || (char *)p >= cvars_state().PCH_End ) ) {
        if( CMemKind( static_cast<const char *>(p) ) == CMEM_MEM ) {
            mcb = MCB_FROM_C( p );
            if( (char *)mcb == cmemmgr_state().PermPtr + cmemmgr_state().PermAvail ) {
                cmemmgr_state().PermAvail += mcb->size & ~ALLOC_FLAG;
                mcb = MCB_NEXT( mcb );
                if( (mcb->size & ALLOC_FLAG) == 0 ) {
                    if( (char *)mcb == cmemmgr_state().PermPtr + cmemmgr_state().PermAvail ) {
                        cmemmgr_state().PermAvail += mcb->size;
                        removeMCBfromFreeList( mcb );
                    }
                }
            } else {
                addMCBtoFreeList( mcb );
                Ccoalesce( mcb );
            }
        }
    }
}


void *CPermAlloc( size_t size )
/*****************************/
{
    void    *p;

    size = _RoundUp( size, MEM_ALIGN );
    if( size > cmemmgr_state().PermAvail ) {
        if( size <= MAX_PERM_SIZE ) {
            /*
             * allocate another permanent area block
             */
            AllocPermArea();
        }
        if( size > cmemmgr_state().PermAvail ) {
            CErr1( ERR_OUT_OF_MEMORY );
            CSuicide();
        }
    }
    cmemmgr_state().PermAvail -= size;
    p = cmemmgr_state().PermPtr;
    cmemmgr_state().PermPtr += size;
    return( memset( p, 0, size ) );
}


void *FEmalloc( size_t size )
/***************************/
{
    void    *p;

    p = calloc( 1, size );   /* (zeroed: see run/cbrun.hpp) */
    if( p == NULL ) {
        CErr1( ERR_OUT_OF_MEMORY );
        CSuicide();
    }
    return( p );
}


void FEfree( void *p )
/********************/
{
    if( p != NULL ) {
        free( p );
    }
}


int FEMoreMem( size_t size )
/**************************/
{
    /* unused parameters */ (void)size;

    return( 0 );
}
