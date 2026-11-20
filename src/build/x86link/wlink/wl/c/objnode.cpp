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
 *  OBJNODE:   "node" management routines
 *
 */

#include <string.h>
#include <array>
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "distrib.h"
#include "wcomdef.h"
#include "strtab.h"
#include "carve.h"
#include "permdata.h"
#include "objnode.h"


namespace {

constexpr unsigned NODE_ARRAY_SIZE = 256;
constexpr unsigned MAX_NUM_NODES = 128;

constexpr unsigned arrayNum( unsigned x ) { return x >> 8; }
constexpr unsigned elementNum( unsigned x ) { return x & 0xFF; }

}

struct nodearray {
    unsigned                    num;            // number of nodes inserted
    unsigned                    elsize;         // size of individual element in array.
    unsigned                    arraymax;       // the number of node arrays allocated - 1
    std::array<char *, MAX_NUM_NODES> array;    // the array.
};


namespace {

nodearray_handle MakeArray( unsigned size )
{
    nodearray_handle    nodes;

    nodes = static_cast<nodearray_handle>( MemAllocSafe( sizeof( nodearray ) ) );
    nodes->num = 0;
    nodes->elsize = size;
    nodes->arraymax = 0;
    size *= NODE_ARRAY_SIZE;
    nodes->array[0] = static_cast<char *>( MemAllocSafe( size ) );
    memset( nodes->array[0], 0, size );
    return nodes;
}

}

// initialize the node array structures.
void InitNodes( void )
/***************************/
{
    wlink().GrpNodes = MakeArray( sizeof( grpnode ) );
    wlink().SegNodes = MakeArray( sizeof( segnode ) );
    wlink().ExtNodes = MakeArray( sizeof( extnode ) );
    wlink().NameNodes = MakeArray( sizeof( list_of_names * ) );
}

namespace {

void BurnNodeArray( nodearray_handle list )
{
    unsigned    index;

    for( index = 0; index <= list->arraymax; index++ ) {
        MemFree( list->array[index] );
    }
    MemFree( list );
}

}

void BurnNodes( void )
/***************************/
{
    BurnNodeArray( wlink().GrpNodes );
    BurnNodeArray( wlink().SegNodes );
    BurnNodeArray( wlink().ExtNodes );
    BurnNodeArray( wlink().NameNodes );
}

void *FindNode( nodearray_handle list, unsigned index )
/*****************************************************/
{
    index--;            // index is base 1
    return list->array[arrayNum( index )] + elementNum( index ) * list->elsize;
}

namespace {

void AllocNewArray( nodearray_handle list )
{
    unsigned    size;

    list->arraymax++;
    size = list->elsize * NODE_ARRAY_SIZE;
    list->array[list->arraymax] = static_cast<char *>( MemAllocSafe( size ) );
    memset( list->array[list->arraymax], 0, size );
}

}

void *AllocNode( nodearray_handle list )
/**************************************/
{
    if( arrayNum( list->num ) > list->arraymax ) {
        AllocNewArray( list );
    }
    list->num++;
    return FindNode( list, list->num );
}

void *AllocNodeIdx( nodearray_handle list, unsigned index )
/*********************************************************/
{
    if( list->num < index ) {
        list->num = index;
        while( arrayNum( index - 1 ) > list->arraymax ) {
            AllocNewArray( list );
        }
    }
    return FindNode( list, index );
}

// Allocate a new object file entry structure and initialize it
mod_entry *NewModEntry( void )
/****************************/
{
    mod_entry           *entry;

    entry = static_cast<mod_entry *>( CarveZeroAlloc( wlink().CarveModEntry ) );
#ifdef _EXE
    if( wlink().LinkFlags & LF_STRIP_CODE ) {
        InitArcList( entry );
    }
#endif
    return entry;
}

void FreeModEntry( mod_entry *mod )
/****************************************/
{
    CarveFree( wlink().CarveModEntry, mod );
}

void FreeNodes( nodearray_handle nodes )
/**************************************/
{
    unsigned    index;

    for( index = 0; index <= nodes->arraymax; index++ ) {
        memset( nodes->array[index], 0, nodes->elsize * NODE_ARRAY_SIZE );
    }
    nodes->num = 0;
}

namespace {

void ReleaseNamelist( void *node, void * )
{
    MemFree( *static_cast<void **>( node ) );
}

void IterateNodeArray( char *narray, void (*fn)(void *, void *),
                       unsigned elsize, int limit, void *cookie )
{
    int index;

    for( index = 0; index < limit; index++ ) {
        fn( narray, cookie );
        narray += elsize;
    }
}

}

void IterateNodelist( nodearray_handle list, void (*fn)(void *, void *),
                             void *cookie )
/***********************************************************************/
{
    int index;
    int limit;

    if( list->num == 0 )
        return;
    limit = static_cast<int>( arrayNum( list->num ) );
    for( index = 0; index < limit; index++ ) {
        IterateNodeArray( list->array[index], fn, list->elsize, NODE_ARRAY_SIZE, cookie );
    }
    IterateNodeArray( list->array[limit], fn, list->elsize, static_cast<int>( elementNum( list->num ) ), cookie );
}

// Free list of names.
void ReleaseNames( void )
/******************************/
{
    IterateNodelist( wlink().NameNodes, ReleaseNamelist, nullptr );
    FreeNodes( wlink().NameNodes );
}

namespace {

void CollapseLazy( void *node, void * )
{
    extnode     *curr;

    curr = static_cast<extnode *>( node );
    if( IS_SYM_A_REF( curr->entry ) && !curr->isweak ) {
        ClearSymUnion( curr->entry );
        SET_SYM_TYPE( curr->entry, SYM_REGULAR );
        if( wlink().LinkState & LS_SEARCHING_LIBRARIES ) {
            curr->entry->info &= ~SYM_CHECKED;
            wlink().LinkState |= LS_LIBRARIES_ADDED;    // force another pass thru libs
        }
    }
}

}

void CollapseLazyExtdefs( void )
/*************************************/
{
    IterateNodelist( wlink().ExtNodes, CollapseLazy, nullptr );
}

namespace {

// the first node of the array's first `limit` nodes that `matches`, or null
template <class Match>
void *ListFindValue( char *node, unsigned limit, unsigned elsize, Match matches )
{
    unsigned            index;

    for( index = 0; index < limit; index++ ) {
        if( matches( node ) )
            return node;
        node += elsize;
    }
    return nullptr;
}

template <class Match>
void *IterateFindValue( nodearray_handle list, Match matches )
{
    unsigned    index;
    void        *retval;
    unsigned    limit;

    if( list->num == 0 )
        return nullptr;
    limit = arrayNum( list->num );
    for( index = 0; index < limit; index++ ) {
        retval = ListFindValue( list->array[index], NODE_ARRAY_SIZE, list->elsize, matches );
        if( retval ) {
            return retval;
        }
    }
    return ListFindValue( list->array[limit], elementNum( list->num ), list->elsize, matches );
}

}

extnode *FindExtHandle( orl_symbol_handle sym_handle )
/****************************************************/
{
    return static_cast<extnode *>( IterateFindValue( wlink().ExtNodes, [sym_handle]( const char *curr ) {
        return reinterpret_cast<const extnode *>( curr )->handle == sym_handle;
    } ) );
}

segdata *AllocSegData( void )
/***************************/
{
    segdata *sdata;

    sdata = static_cast<segdata *>( CarveZeroAlloc( wlink().CarveSegData ) );
    sdata->o.mod = wlink().CurrMod;
    return sdata;
}

// put a segdata on the list of free segdatas
void FreeSegData( void *sdata )
/*****************************/
{
    CarveFree( wlink().CarveSegData, sdata );
}

list_of_names *MakeListName( char *name, size_t len )
/***************************************************/
{
    list_of_names   *new_;

    new_ = static_cast<list_of_names *>( MemAllocSafe( sizeof( list_of_names ) + len ) );
    new_->next = nullptr;
    memcpy( new_->name, name, len );
    new_->name[len] = '\0';
    return new_;
}

unsigned long BadObjFormat( void )
/***************************************/
{
    LnkMsg( FTL+MSG_BAD_OBJECT, "s", wlink().CurrMod->u1.source->infile->name );
    return 0;
}
