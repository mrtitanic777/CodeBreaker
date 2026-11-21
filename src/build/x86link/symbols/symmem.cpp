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
* Description:  Symbol table extra memory routines.
*
****************************************************************************/


#include <cstring>
#include <cstdlib>
#include <climits>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "pcobj.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/symbols/symmem.h"
#include "build/x86link/link/run_state.hpp"


namespace {

constexpr std::size_t ALLOC_ALIGN = sizeof( void * );
constexpr std::size_t SYM_BLOCK_MIN = 32;

struct sym_block {
    sym_block   *next;       /* NOTE: this *must* be the first field */
    size_t      size;
    //    char                block[];
};

constexpr std::size_t SYM_BLOCK_HDR_SIZE = __ROUND_UP_SIZE( sizeof( sym_block ), ALLOC_ALIGN );

inline std::size_t SymBlockSize( std::size_t x )
{
    return SYM_BLOCK_HDR_SIZE + x;
}

inline void *SymBlockData( sym_block *b, std::size_t x )
{
    return reinterpret_cast<char *>( b ) + SYM_BLOCK_HDR_SIZE + x;
}

struct block_data {
    sym_block   *list;
    size_t      currbrk;
};

/* the symbol table's allocation blocks, one set for a run */
struct SymMemState {
    block_data  pass1;
    block_data  perm;
};

inline SymMemState &st()
{
    return cb_wlink::RunState::part<SymMemState>();
}

} // namespace

void ResetPermBlocks( void )
/**************************/
/* allocate memory for symbol table allocation and code */
{
    st().perm.list = nullptr;
}

void ResetPass1Blocks( void )
/***************************/
{
    st().pass1.list = nullptr;
}

static bool ShrinkBlock( block_data *block_set )
/**********************************************/
{
#ifdef __WATCOMC__
    sym_block   *block;

    if( block_set->list == nullptr )
        return( false );
    if( block_set->currbrk >= block_set->list->size )
        return( false );
    block = static_cast<sym_block *>( MemReallocSafe( block_set->list, SymBlockSize( block_set->currbrk ) ) );
    block->size = block_set->currbrk;
    /* assuming that a shrinkage will not move the block */
  #ifdef DEVBUILD
    if( block != block_set->list ) {
        LnkMsg( FTL+MSG_INTERNAL, "s", "realloc moved shrinked block!" );
    }
  #endif
    return( true );
#else
    /* There is no guarantee realloc() won't move memory - just don't do it */

    /* unused parameters */ (void)block_set;

    return( false );
#endif
}

bool PermShrink( void )
/*********************/
/* shrink down the current permanent allocation blocks */
{
    SymMemState &state = st();
    bool        ret;

    ret = ShrinkBlock( &state.perm );
    if( !ret ) {
        ret = ShrinkBlock( &state.pass1 );
    }
    return( ret );
}

static void GetNewBlock(Link& link,  block_data *block_set, size_t size )
/***********************************************************/
{
    size_t              try_;
    sym_block           *block;

    ShrinkBlock( block_set );
    try_ = _16K;     /* default block size */
    if( try_ < size )
        try_ = size;
    for( ;; ) {
        block = static_cast<sym_block *>( MemAlloc(link,  SymBlockSize( try_ ) ) );
        if( block != nullptr )
            break;
        try_ /= 2;
        if( try_ < size
          || try_ < SYM_BLOCK_MIN ) {
            LnkMsg(link,  FTL + MSG_NO_DYN_MEM, nullptr );
        }
    }
    block->size = try_;
    block->next = block_set->list;
    block_set->list = block;
    block_set->currbrk = 0;
}

static void *AllocBlock(Link& link,  size_t size, block_data *block_set )
/***********************************************************/
{
    void            *ptr;
    size_t          newbrk;

    size = __ROUND_UP_SIZE( size, ALLOC_ALIGN );
    newbrk = block_set->currbrk + size;
    if( block_set->list == nullptr ) {
        GetNewBlock(link,  block_set, size );
    } else if( newbrk > block_set->list->size ) {
        GetNewBlock(link,  block_set, size );
    }
    ptr = SymBlockData( block_set->list, block_set->currbrk );
    block_set->currbrk += size;
    return( ptr );
}

void *Pass1Alloc(Link& link,  size_t size )
/*****************************/
{
    return( AllocBlock(link,  size, &st().pass1 ) );
}

void *PermAlloc(Link& link,  size_t size )
/****************************/
/* allocate a thunk of permanently allocated memory */
{
    return( AllocBlock(link,  size, &st().perm ) );
}

void BasicInitSym( symbol *sym )
/*************************************/
{
    sym->hash = nullptr;
    sym->next = nullptr;
    sym->next_by_publist = nullptr;
    SET_ADDR_UNDEFINED( sym->addr );
    sym->mod = nullptr;
    sym->p.seg = nullptr;
    sym->info = SYM_REGULAR | SYM_IN_CURRENT;
    sym->u.altdefs = nullptr;   // this sets all union members to zero.
    sym->e.def = nullptr;       // ditto
    sym->prefix = nullptr;
}

symbol * AddSym(Link& link)
/****************************/
/* allocate and initialize a new symbol */
{
    symbol              *sym;

    sym = static_cast<symbol *>( CarveAlloc(link,  link.CarveSymbol ) );
    BasicInitSym( sym );
    if( link.LastSym != nullptr )
        link.LastSym->next = sym;
    link.LastSym = sym;
    if( link.HeadSym == nullptr )
        link.HeadSym = sym;
    return( sym );
}

void ReleasePass1Blocks( void )
/*****************************/
/* free pass1 block allocations */
{
    block_data &b = st().pass1;

    FreeList( b.list );
    b.list = nullptr;
}

void ReleasePermBlocks( void )
/****************************/
/* free memory used for symbol table allocation and code */
{
    block_data &b = st().perm;

    FreeList( b.list );
    b.list = nullptr;
}
