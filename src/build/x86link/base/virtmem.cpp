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
* Description:  Virtual memory support for linker.
*
****************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <cstddef>
#include <vector>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/base/virtmem.h"
#include "build/x86link/link/run_state.hpp"
#if defined( USE_VIRTMEM )
    #include "walloca.h"
    #include "build/x86link/base/newmem.h"
    #include "ostype.h"
    #include "build/x86link/base/spillio.h"
    #include "build/x86link/formats/output.h"
    #include "build/x86link/base/fileio.h"
#endif


namespace {

#if defined( USE_VIRTMEM )

/* flags used in the virtual memory structure */
enum class VirtFlags : unsigned {
    None  = 0x00,
    InMem = 0x01,           /* virtual memory block is in RAM */
    Huge  = 0x02            /* virtual memory block is a huge block */
};

constexpr VirtFlags operator|( VirtFlags a, VirtFlags b )
{
    return static_cast<VirtFlags>( static_cast<unsigned>( a ) | static_cast<unsigned>( b ) );
}

constexpr VirtFlags operator&( VirtFlags a, VirtFlags b )
{
    return static_cast<VirtFlags>( static_cast<unsigned>( a ) & static_cast<unsigned>( b ) );
}

constexpr VirtFlags operator~( VirtFlags a )
{
    return static_cast<VirtFlags>( ~static_cast<unsigned>( a ) );
}

constexpr VirtFlags &operator|=( VirtFlags &a, VirtFlags b )
{
    return a = a | b;
}

constexpr VirtFlags &operator&=( VirtFlags &a, VirtFlags b )
{
    return a = a & b;
}

constexpr bool has( VirtFlags flags, VirtFlags f )
{
    return ( flags & f ) != VirtFlags::None;
}

/* this is for allocating very large memory requests (i.e. > 1 megabyte).
   virtual memory locations 0x80000000 and above are split into 1 megabyte
   pages.

   offset into subpage         subpage #  page #       big-page indicator
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   15 bits                   | 5 bits  | 11 bits             |1|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
low-order                           high-order

each page pointer in the huge_table structure points to an array of 32
spill addresses, each of which points to a 32K-byte subpage of virtual memory.
*/

/*
 * the following structure is for "huge" virtual memory allocation
 * first two fields are shared with seg_table
 */
struct huge_table {
    huge_table          *next;          /* next entry to swap. */
    VirtFlags           flags;
    unsigned_8          numthere;
    unsigned_8          numswapped;
    unsigned_16         sizelast;
    spilladdr           *page;
};

constexpr unsigned long HUGE_OFFSET_SHIFT   = 20;
constexpr unsigned long MAX_BIGNODE_SIZE    = 1UL << HUGE_OFFSET_SHIFT;
constexpr unsigned long HUGE_OFFSET_MASK    = MAX_BIGNODE_SIZE - 1;
constexpr unsigned long HUGE_BIT_MASK       = 0x7FFFFFFFUL;
constexpr unsigned long HUGE_PAGE           = 0x80000000UL;
constexpr unsigned      HUGE_INITIAL_ALLOC  = 2;
constexpr unsigned      HUGE_LIMIT          = 2048;     /* max number of huge pages */
constexpr unsigned      HUGE_SUBPAGE_SHIFT  = 15;
constexpr unsigned      HUGE_SUBPAGE_SIZE   = 1U << HUGE_SUBPAGE_SHIFT;
constexpr unsigned      HUGE_SUBPAGE_MASK   = HUGE_SUBPAGE_SIZE - 1;
constexpr unsigned      HUGE_NUM_SUBPAGES   = 32;

/*
 * the following structure is for "normal" virtual memory allocation
 * first two fields are shared with huge_table
 */
struct seg_table {
    seg_table           *next;          /* next entry to swap out. */
    VirtFlags           flags;
    unsigned_16         size;
    spilladdr           loc;
};

/* the seg tables are referenced by an array of pointers, something like
SegTab --> 0 1 2 3 4 5 6 ...        <-- array elements are pointers
           | | | | | | |
           v v v v v v v
           1 1 1 1 1 1 1    <-  array elements are seg_tables
           2 2 2 2 2 2 2   these arrays are referred to as "branches",
           3 3 3 3 3 3 3   and the individual elements are called "leaves".

in other words, it is effectively a dynamically allocated 2 dimensional array.
All virtual memory locations from 0 - 0x7FFFFFFF are split into 4K pages.

a virtual memory address is split into three parts:
offset                   leaf id   branch id
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   12 bits             | 4 bits|  15 bits                    |0|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 low order word                 high order word.
the leaf id chooses which element of the branch is the correct seg_table.
 0 means that the bit is always zero.

we start with branch id 1 so an address of zero can be illegal
branch id 0 is not used

*/

constexpr unsigned      OFFSET_SHIFT        = 12;
constexpr unsigned      MAX_NODE_SIZE       = 1U << OFFSET_SHIFT;
constexpr unsigned      MAX_LEAFS           = 16;       /* maximum # of leafs per branch */
constexpr unsigned      SEG_LIMIT           = 32767;    /* maximum # of branches (leafs * 16) */

constexpr unsigned      TINY_BLOCK_CUTOFF   = 256;

/* a virtual address of the "normal" kind: the high order word is the branch,
   the low order word holds the leaf and the offset */
constexpr unsigned_16 addr_high( virt_mem stg )
{
    return static_cast<unsigned_16>( stg >> 16 );
}

constexpr unsigned_16 addr_low( virt_mem stg )
{
    return static_cast<unsigned_16>( stg & 0xFFFF );
}

#else

constexpr std::size_t TINY_ALLOC_CUTOFF = _2K;

struct vmemblock {
    vmemblock           *next;
    vmemblock           *prev;
    char                mem[1];
};

#endif

// what the virtual memory keeps for one run of the linker
struct VirtState {
#if defined( USE_VIRTMEM )
    huge_table      *HugeTab;
    unsigned        NumHuge;
    unsigned        NextHuge;
    seg_table       **SegTab;
    unsigned        NumBranches;
    /* start with branch # 1 so an address of zero can be illegal. */
    unsigned        CurrBranch;
    unsigned        NextLeaf;       /* next leaf # to be allocated. */
    seg_table       *NextSwap;      /* next entry to swap out. */
    unsigned        TinyLeft;
    virt_mem        TinyAddr;
#else
    vmemblock       *VMemBlocks;
#endif
};

VirtState &virt_state()
{
    return cb_wlink::RunState::part<VirtState>();
}

#if defined( USE_VIRTMEM )

/* find the node for an address */
seg_table *node_of( const VirtState &vs, virt_mem stg )
{
    return &vs.SegTab[addr_high( stg )][addr_low( stg ) >> OFFSET_SHIFT];
}

huge_table *bignode_of( VirtState &vs, virt_mem stg )
{
    return &vs.HugeTab[( stg & HUGE_BIT_MASK ) >> HUGE_OFFSET_SHIFT];
}

unsigned subpage_num( virt_mem stg )
{
    return ( stg & HUGE_OFFSET_MASK ) >> HUGE_SUBPAGE_SHIFT;
}

void GetMoreBranches( VirtState &vs )
/**********************************
 * make a larger array to hold branch pointers in.
 */
{
    seg_table       **branches;
    unsigned        alloc_size;

    alloc_size = vs.NumBranches * sizeof( seg_table * );
    vs.NumBranches = vs.NumBranches * 2;      /* double the # of pointers. */
    if( vs.NumBranches > SEG_LIMIT ) {
        LnkMsg( FTL+MSG_NO_VIRT_MEM, nullptr );
    }
    branches = static_cast<seg_table **>( MemAllocSafe( alloc_size * 2 ) );
    memcpy( branches, vs.SegTab, alloc_size );
    memset( reinterpret_cast<char *>( branches ) + alloc_size, 0, alloc_size ); /* null pointers */
    MemFree( vs.SegTab );
    vs.SegTab = branches;
}

virt_mem GetStg( VirtState &vs, virt_mem_size amt )
{
    seg_table               *seg_entry;
    unsigned                alloc_size;
    virt_mem                vmem;

    if( vs.NextLeaf >= MAX_LEAFS ) {
        vs.NextLeaf = 0;
        vs.CurrBranch++;
        if( vs.CurrBranch >= vs.NumBranches ) {
            GetMoreBranches( vs );
        }
        alloc_size = sizeof( seg_table ) * MAX_LEAFS;
        seg_entry = static_cast<seg_table *>( PermAlloc( alloc_size ) );
        memset( seg_entry, 0, alloc_size ); /* set all flags false. */
        vs.SegTab[vs.CurrBranch] = seg_entry;
    } else {
        seg_entry = &vs.SegTab[vs.CurrBranch][vs.NextLeaf];
    }
    seg_entry->size = static_cast<unsigned_16>( amt );
    vmem = ( static_cast<virt_mem>( static_cast<unsigned_16>( vs.CurrBranch ) ) << 16 )
         | static_cast<unsigned_16>( vs.NextLeaf << OFFSET_SHIFT );
    DEBUG((DBG_VIRTMEM, "virt %h amt %x", vmem, amt ));
    vs.NextLeaf++;
    return( vmem );
}

virt_mem GetBigStg( VirtState &vs, virt_mem_size size )
{
    unsigned        alloc_size;
    huge_table      *newtab;
    huge_table      *huge_entry;
    virt_mem        vmem;

    if( vs.NextHuge >= vs.NumHuge ) {
        alloc_size = vs.NumHuge * sizeof( huge_table );
        vs.NumHuge *= 2;
        if( vs.NumHuge > HUGE_LIMIT ) {
            LnkMsg( FTL+MSG_NO_VIRT_MEM, nullptr );
        }
        newtab = static_cast<huge_table *>( MemAllocSafe( alloc_size * 2 ) );
        memcpy( newtab, vs.HugeTab, alloc_size );
        memset( reinterpret_cast<char *>( newtab ) + alloc_size, 0, alloc_size );
        MemFree( vs.HugeTab );
        vs.HugeTab = newtab;
    }
    huge_entry = &vs.HugeTab[vs.NextHuge];
    vmem = static_cast<virt_mem>( ( static_cast<unsigned long>( vs.NextHuge ) << HUGE_OFFSET_SHIFT ) | HUGE_PAGE );
    huge_entry->numthere = static_cast<unsigned_8>( ( size >> HUGE_SUBPAGE_SHIFT ) + 1 );
    huge_entry->sizelast = static_cast<unsigned_16>( size & HUGE_SUBPAGE_MASK );
    if( huge_entry->sizelast == 0 ) {
        huge_entry->numthere--;
        huge_entry->sizelast = HUGE_SUBPAGE_SIZE;
    }
    huge_entry->flags = VirtFlags::Huge;
    DEBUG((DBG_VIRTMEM, "huge virt %h amt %h", vmem, size ));
    vs.NextHuge++;
    return( vmem );
}

/* the two ways of getting storage for a piece of at most `limit` bytes */
enum class StgKind { Normal, Big };

virt_mem GetPiece( VirtState &vs, StgKind kind, virt_mem_size amt )
{
    return( kind == StgKind::Big ? GetBigStg( vs, amt ) : GetStg( vs, amt ) );
}

virt_mem DoAllocStg( VirtState &vs, StgKind kind, virt_mem_size size, virt_mem_size limit )
{
    virt_mem    ret;
    bool        gotaddr;

    ret = 0;
    gotaddr = false;
    for( ; size > limit; size -= limit ) {
        if( !gotaddr ) {
            gotaddr = true;
            ret = GetPiece( vs, kind, limit );
        } else {
            GetPiece( vs, kind, limit );
        }
    }
    if( !gotaddr ) {
        ret = GetPiece( vs, kind, size );
    } else {
        GetPiece( vs, kind, size );
    }
    return( ret );
}

virt_mem AllocTinyStg( VirtState &vs, unsigned size )
{
    virt_mem    retval;

    if( vs.TinyLeft < size ) {
        vs.TinyAddr = DoAllocStg( vs, StgKind::Normal, MAX_NODE_SIZE, MAX_NODE_SIZE );
        vs.TinyLeft = MAX_NODE_SIZE;
    }
    retval = vs.TinyAddr;
    vs.TinyAddr += size;
    vs.TinyLeft -= size;
    return( retval );
}

void AllocVMNode( VirtState &vs, seg_table *node )
/*****************************************
 * allocate a regular 4K page
 */
{
    void        *mem;

    mem = MemAlloc( node->size );
    if( mem == nullptr ) {
        node->loc.u.spill = SpillAlloc( node->size );
        node->loc.spilled = true;
    } else {
        node->loc.u.addr = static_cast<char *>( mem );
        node->loc.spilled = false;
        AppendList( reinterpret_cast<void **>( &vs.NextSwap ), node );
    }
}

void AllocHugeVMNode( VirtState &vs, huge_table *node )
/**********************************************
 * allocate a huge page
 */
{
    void        *mem;
    int         index;
    spilladdr   *page;
    bool        nomem;

    node->page = static_cast<spilladdr *>( PermAlloc( HUGE_NUM_SUBPAGES * sizeof( spilladdr ) ) );
    page = node->page;
    index = node->numthere - 1;         /* first try to allocate the last bit */
    mem = MemAlloc( node->sizelast );   /* on the end of the page. */
    if( mem == nullptr ) {
        nomem = true;
        node->flags &= ~VirtFlags::InMem;
        node->numswapped = node->numthere;
        page[index].u.spill = SpillAlloc( node->sizelast );
    } else {
        nomem = false;
        node->flags |= VirtFlags::InMem;
        page[index].u.addr = static_cast<char *>( mem );
        AppendList( reinterpret_cast<void **>( &vs.NextSwap ), node );
    }
    page[index].spilled = nomem;
    /*
     * now allocate all of the subpages, starting at the end and working backwards
     * so that they can be swapped from the start forwards.
     */
    while( index-- > 0 ) {
        if( nomem ) {
            page[index].u.spill = SpillAlloc( HUGE_SUBPAGE_SIZE );
        } else {
            mem = MemAlloc( HUGE_SUBPAGE_SIZE );
            if( mem == nullptr ) {
                nomem = true;
                node->numswapped = static_cast<unsigned_8>( index + 1 );
                page[index].u.spill = SpillAlloc( HUGE_SUBPAGE_SIZE );
            } else {
                page[index].u.addr = static_cast<char *>( mem );
            }
        }
        page[index].spilled = nomem;
    }
}

using NodeFn = bool (*)( void *, spilladdr, size_t, size_t );

bool ScanNodes( virt_mem mem, void *info, virt_mem_size len, NodeFn rtn )
/********************************************************************
 * go through the virtual memory nodes, reading or writing data
 */
{
    VirtState       &vs = virt_state();
    virt_mem_size   end_off;
    size_t          off;
    seg_table       *node;
    huge_table      *bignode;
    spilladdr       *page;
    size_t          currlen;
    size_t          amt;
    unsigned        subpage;
    virt_mem        stg;
    bool            retval;

    stg = mem;
    if( stg & HUGE_PAGE ) {  /* it is a huge page */
        off = stg & HUGE_SUBPAGE_MASK;
        for( ;; ) {
            bignode = bignode_of( vs, stg );
            page = bignode->page;
            subpage = subpage_num( stg );
            if( subpage == bignode->numthere - 1U ) {
                currlen = bignode->sizelast;
            } else {
                currlen = HUGE_SUBPAGE_SIZE;
            }
            end_off = off + len;
            if( page == nullptr ) {
                AllocHugeVMNode( vs, bignode );
                page = bignode->page;
            }
            if( end_off <= currlen )
                break;
            amt = currlen - off;
            if( !rtn( info, page[subpage], off, amt ) )
                return( false );
            len -= amt;
            stg += amt;
            info = static_cast<char *>( info ) + amt;
            off = 0;
        }
        retval = rtn( info, page[subpage], off, len );
    } else {
        off = addr_low( stg ) & ( MAX_NODE_SIZE - 1 );
        for( ;; ) {
            node = node_of( vs, stg );
            end_off = off + len;
            if( !node->loc.spilled && node->loc.u.addr == nullptr ) {
                AllocVMNode( vs, node );
            }
            if( end_off <= node->size )
                break;
            amt = node->size - off;
            if( !rtn( info, node->loc, off, amt ) ) {
                return( false );
            }
            len -= amt;
            stg += amt;
            info = static_cast<char *>( info ) + amt;
            off = 0;
        }
        retval = rtn( info, node->loc, off, len );
    }
    return( retval );
}

bool LoadInfo( void *info, spilladdr loc, size_t off, size_t len )
/************************************************************************
 * copy data to info from the memory or spillfile referenced by node & off
 */
{
    if( len ) {
        if( loc.spilled ) {
            SpillRead( loc.u.spill, off, info, len );
        } else {
            memcpy( info, loc.u.addr + off, len );
        }
    }
    return( true );
}

bool SaveInfo( void *info, spilladdr loc, size_t off, size_t len )
/************************************************************************
 * copy data at info to the memory or spillfile referenced by node & off
 */
{
    if( len ) {
        DEBUG((DBG_VIRTMEM, "saving %d bytes (offset %x) to %d.%h", len, off, loc.spilled, loc.u.spill ));
        if( loc.spilled ) {
            SpillWrite( loc.u.spill, off, info, len );
        } else {
            memcpy( loc.u.addr + off, info, len );
        }
    }
    return( true );
}

bool CompareBlock( void *info, spilladdr loc, size_t off, size_t len )
/****************************************************************************
 * compare data at info to the memory or spillfile referenced by node & off
 */
{
    if( len == 0 )
        return( true );
    if( loc.spilled ) {
        std::vector<char>   buf( len );

        SpillRead( loc.u.spill, off, buf.data(), len );
        return( memcmp( buf.data(), info, len ) == 0 );
    }
    return( memcmp( loc.u.addr + off, info, len ) == 0 );
}

bool OutInfo( void *dummy, spilladdr loc, size_t off, size_t len )
/************************************************************************
 * copy data in memory or spillfile referenced by node & off to LoadFile
 */
{
    size_t  amt;

    /* unused parameters */ (void)dummy;

    if( len ) {
        DEBUG((DBG_VIRTMEM, "writing %d bytes (offset %x) to %d.%h", len, off, loc.spilled, loc.u.spill ));
        if( loc.spilled ) {
            for( ; len != 0; len -= amt ) {
                amt = len;
                if( amt > wlink().TokSize )
                    amt = wlink().TokSize;
                SpillRead( loc.u.spill, off, wlink().TokBuff, amt );
                WriteLoad( wlink().TokBuff, amt );
                off += amt;
            }
        } else {
            WriteLoad( loc.u.addr + off, len );
        }
    }
    return( true );
}

bool NullInfo( void *dummy, spilladdr loc, size_t off, size_t len )
/*************************************************************************
 * write nulls to the location referenced by node and off.
 */
{
    /* unused parameters */ (void)dummy;

    if( len ) {
        if( loc.spilled ) {
            SpillNull( loc.u.spill, off, len );
        } else {
            memset( loc.u.addr + off, 0, len );
        }
    }
    return( true );
}

#endif

} // namespace

#if defined( USE_VIRTMEM )

void ReadInfo( virt_mem stg, void *buf, virt_mem_size len )
/**********************************************************
 * copy data into info from the memory or spillfile referenced by stg
 */
{
    ScanNodes( stg, buf, len, LoadInfo );
}

void PutInfo( virt_mem stg, const void *info, virt_mem_size len )
/****************************************************************
 * copy data at info to the memory or spillfile referenced by stg
 */
{
    ScanNodes( stg, const_cast<void *>( info ), len, SaveInfo );
}

void CopyInfo( virt_mem a, virt_mem b, size_t len )
/*************************************************/
{
    void        *buf;

    buf = MemAllocSafe( len );
    ReadInfo( b, buf, len );
    PutInfo( a, buf, len );
    MemFree( buf );
}

bool CompareInfo( virt_mem stg, const void *info, virt_mem_size len )
/*******************************************************************/
{
    return( ScanNodes( stg, const_cast<void *>( info ), len, CompareBlock ) );
}

void WriteInfoLoad( virt_mem stg, virt_mem_size len )
/****************************************************
 * copy data in memory or spillfile referenced by stg to LoadFile
 */
{
    ScanNodes( stg, nullptr, len, OutInfo );
}

void PutInfoNulls( virt_mem stg, virt_mem_size len )
/***************************************************
 * copy NULLS in memory or spillfile referenced by stg
 */
{
    ScanNodes( stg, nullptr, len, NullInfo );
}

#endif


void     VirtMemInit( void )
/**************************/
{
    VirtState &vs = virt_state();

#if defined( USE_VIRTMEM )
    /***********************
     * Allocate space for the branch pointers.
     */
    vs.NumHuge = HUGE_INITIAL_ALLOC;
    vs.NextHuge = 0;
    vs.NumBranches = 127;
    /*
     * start with branch # 1 so an virtual address of zero can be illegal
     * branch # 0 is not used
     */
    vs.CurrBranch = 1;
    vs.NextLeaf = 0;
    vs.NextSwap = nullptr;
    vs.TinyLeft = 0;
    vs.SegTab = static_cast<seg_table **>( MemAllocSafe( vs.NumBranches * sizeof( seg_table * ) ) );
    memset( vs.SegTab, 0, vs.NumBranches * sizeof( seg_table * ) );
    vs.SegTab[1] = static_cast<seg_table *>( PermAlloc( sizeof( seg_table ) * MAX_LEAFS ) );
    memset( vs.SegTab[1], 0, sizeof( seg_table ) * MAX_LEAFS );
    vs.HugeTab = static_cast<huge_table *>( MemAllocSafe( vs.NumHuge * sizeof( huge_table ) ) );
    memset( vs.HugeTab, 0, vs.NumHuge * sizeof( huge_table ) );
#else
    vs.VMemBlocks = nullptr;
#endif
}

virt_mem AllocStg( virt_mem_size size )
/*************************************/
{
    VirtState &vs = virt_state();

#if defined( USE_VIRTMEM )
    if( size == 0 )
        return( 0 );
    size = __ROUND_UP_SIZE_EVEN( size );
    if( size <= TINY_BLOCK_CUTOFF ) {
        return( AllocTinyStg( vs, size ) );
    } else if( size >= MAX_BIGNODE_SIZE ) {
        return( DoAllocStg( vs, StgKind::Big, size, MAX_BIGNODE_SIZE ) );
    } else {
        return( DoAllocStg( vs, StgKind::Normal, size, MAX_NODE_SIZE ) );
    }
#else
    vmemblock * ptr;

    if( size == 0 )
        return( 0 );
    if( size < TINY_ALLOC_CUTOFF ) {
        ptr = static_cast<vmemblock *>( _PermAlloc( size + sizeof( vmemblock ) - 1 ) );
        ptr->next = ptr;
    } else {
        ptr = static_cast<vmemblock *>( MemAllocSafe( size + sizeof( vmemblock ) - 1 ) );
        ptr->prev = nullptr;
        ptr->next = vs.VMemBlocks;
        if( vs.VMemBlocks != nullptr ) {
            vs.VMemBlocks->prev = ptr;
        }
        vs.VMemBlocks = ptr;
    }
    return( ptr->mem );
#endif
}

void     ReleaseInfo( virt_mem stg )
/**********************************/
{
#if defined( USE_VIRTMEM )
    /*******************************
     * can't prematurely release, but no big deal
     */

    /* unused parameters */ (void)stg;
#else
    VirtState &vs = virt_state();
    vmemblock * ptr;

    if( stg == nullptr )
        return;
    if( vs.VMemBlocks == nullptr )
        return;
    ptr = reinterpret_cast<vmemblock *>( stg - offsetof( vmemblock, mem ) );
    if( ptr->next == ptr )
        return;
    if( ptr->prev == nullptr ) {
        vs.VMemBlocks = ptr->next;
        if( vs.VMemBlocks != nullptr ) {
            vs.VMemBlocks->prev = nullptr;
        }
    } else {
        ptr->prev->next = ptr->next;
        if( ptr->next != nullptr ) {
            ptr->next->prev = ptr->prev;
        }
    }
    MemFree( ptr );
#endif
}

bool     SwapOutVirt( void )
/**************************/
{
#if defined( USE_VIRTMEM )
    /***********************
     * NOTE - this routine assumes that once something has been swapped out, it
     * will never be read back in again.
     */
    VirtState       &vs = virt_state();
    spilladdr       *spillmem;
    void            *mem;
    seg_table       *seg_entry;
    huge_table      *huge_entry;
    size_t          size;

    while( vs.NextSwap != nullptr ) {
        seg_entry = vs.NextSwap;
        vs.NextSwap = vs.NextSwap->next;
        if( has( seg_entry->flags, VirtFlags::Huge ) ) {
            huge_entry = reinterpret_cast<huge_table *>( seg_entry );
            if( has( huge_entry->flags, VirtFlags::InMem ) ) {
                spillmem = &huge_entry->page[huge_entry->numswapped];
                huge_entry->numswapped++;
                if( huge_entry->numthere == huge_entry->numswapped ) {
                    huge_entry->flags &= ~VirtFlags::InMem;
                    size = huge_entry->sizelast;
                } else {
                    size = HUGE_SUBPAGE_SIZE;
                }
                mem = spillmem->u.addr;
                spillmem->u.spill = SpillAlloc( size );
                spillmem->spilled = true;
                SpillWrite( spillmem->u.spill, 0, mem, size );
                MemFree( mem );
                DEBUG((DBG_VIRTMEM, "swapping out %h to %h", mem, spillmem->u.spill ));
                return( true );
            }
        } else {
            if( !seg_entry->loc.spilled && seg_entry->loc.u.addr != nullptr ) {
                mem = seg_entry->loc.u.addr;
                seg_entry->loc.u.spill = SpillAlloc( seg_entry->size );
                seg_entry->loc.spilled = true;
                SpillWrite( seg_entry->loc.u.spill, 0, mem, seg_entry->size );
                MemFree( mem );
                DEBUG((DBG_VIRTMEM, "swapping out %h to %h", mem, seg_entry->loc.u.spill ));
                return( true );
            }
        }
    }
#endif
    return( false );
}

void FreeVirtMem( void )
/**********************/
{
    VirtState       &vs = virt_state();

#if defined( USE_VIRTMEM )
    unsigned        index;
    unsigned        inner;
    unsigned        branch;
    unsigned        leaf;
    seg_table       *seg_entry;
    huge_table      *huge_entry;
    spilladdr       *page;

    if( vs.SegTab != nullptr ) {
        for( branch = 0; branch < vs.NumBranches; branch++ ) {
            seg_entry = vs.SegTab[branch];
            if( seg_entry != nullptr ) {
                for( leaf = 0; leaf < MAX_LEAFS; leaf++ ) {
                    if( !seg_entry->loc.spilled ) {
                        MemFree( seg_entry->loc.u.addr );
                    }
                    seg_entry++;
                }
            }
        }
        MemFree( vs.SegTab );
    }
    if( vs.HugeTab != nullptr ) {
        for( index = 0; index < vs.NumHuge; index++ ) {
            huge_entry = &vs.HugeTab[index];
            if( huge_entry->page != nullptr ) {
                page = huge_entry->page;
                for( inner = huge_entry->numswapped; inner < huge_entry->numthere; inner++ ) {
                    MemFree( page[inner].u.addr );
                }
            }
        }
        MemFree( vs.HugeTab );
    }
#else
    FreeList( vs.VMemBlocks );
    vs.VMemBlocks = nullptr;
#endif
}
