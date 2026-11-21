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


#include "build/x86link/link/linkstd.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/base/carve.h"


/* wlink's rings: a circular singly-linked list whose header points at the LAST element (the last one's next is the
   first). ring2.cpp includes this file with PARAM2 defined, for rings whose elements have one pointer before the
   link (Ring2...), so the one set of routines serves both. */

#ifdef PARAM2
#define RINGNAME( name ) Ring2##name
#else
#define RINGNAME( name ) Ring##name
#endif

namespace {

struct RingNode                 // model of a ring element
{
#ifdef PARAM2
    void        *filler;
#endif
    RingNode    *next;          // - points to next
};

// The two ways of going around a ring, once, starting at the first element.
// (the plain one assumes the ring is not modified; the safe one reads the next element before it calls)

template <class Fn>
inline void ForEachInRing( void *hdr, Fn &&fn )
{
    RingNode    *curr = static_cast<RingNode *>( hdr );

    if( curr != nullptr ) {
        RingNode    *const last = curr;

        do {
            curr = curr->next;
            fn( curr );
        } while( curr != last );
    }
}

template <class Fn>
inline void ForEachInRingSafe( void *hdr, Fn &&fn )
{
    RingNode    *curr = static_cast<RingNode *>( hdr );

    if( curr != nullptr ) {
        RingNode    *const last = curr;
        RingNode    *next = last->next;

        do {
            curr = next;
            next = curr->next;
            fn( curr );
        } while( curr != last );
    }
}

#ifdef DEVBUILD
void verifyNotInRing( RingNode *ring, RingNode *elt )
{
    ForEachInRing( ring, [elt]( RingNode *curr ) {
        if( curr == elt ) {
            LnkFatal( "trying to insert element twice into a ring" );
        }
    } );
}
#else
#define verifyNotInRing( h, r )
#endif

} // namespace


void RINGNAME(Append) (         // APPEND ELEMENT TO RING
    void *hdr,                  // - addr( ring header )
    void *element )             // - element to be appended
{
    RingNode **rhdr = static_cast<RingNode **>( hdr );          // - ring header
    RingNode *relement = static_cast<RingNode *>( element );    // - ring element
    RingNode *lelement;                                         // - last ring element, before appending

    verifyNotInRing( *rhdr, relement );
    lelement = *rhdr;
    if( lelement == nullptr ) {
        relement->next = relement;
    } else {
        relement->next = lelement->next;
        lelement->next = relement;
    }
    *rhdr = relement;
}


void* RINGNAME(Promote) (       // PROMOTE ELEMENT TO START OF RING
    void *hdr,                  // - addr( ring header )
    void *elt,                  // - element to be promoted
    void *prv )                 // - element just before element
{
    RingNode **rhdr = static_cast<RingNode **>( hdr );
    RingNode *prev = static_cast<RingNode *>( prv );
    RingNode *element = static_cast<RingNode *>( elt );
    RingNode *last = *rhdr;

    if( prev == nullptr
      || last == prev ) {
        /* already at front */
        return( element );
    }
    if( last != element ) {
        /* delete */
        prev->next = element->next;
        /* insert at front */
        element->next = last->next;
        last->next = element;
    } else {
        /* last element in ring; rotate */
        last = prev;
    }
    *rhdr = last;
    return( element );
}


void RINGNAME(Insert) (         // INSERT ELEMENT INTO RING
    void *hdr,                  // - addr( ring header )
    void *element,              // - element to be inserted
    void *insert )              // - insertion point (or NULL for start)
{
    RingNode **rhdr = static_cast<RingNode **>( hdr );          // - ring header
    RingNode *relement = static_cast<RingNode *>( element );    // - ring element, to be inserted
    RingNode *ielement = static_cast<RingNode *>( insert );     // - ring element, insertion point
    RingNode *lelement;                                         // - last ring element, before appending

    verifyNotInRing( *rhdr, relement );
    lelement = *rhdr;
    if( ( lelement == nullptr )
      || ( lelement == ielement ) ) {
        RINGNAME(Append)( hdr, element );
    } else if( ielement == nullptr ) {  // insert at start of ring
        relement->next = lelement->next;
        lelement->next = relement;
    } else {
        relement->next = ielement->next;
        ielement->next = relement;
    }
}


void RINGNAME(Walk) (           // TRAVERSE RING
    void *hdr,                  // - ring header
    ringwalk_fn *rtn )          // - traversal routine
{
    ForEachInRingSafe( hdr, [rtn]( RingNode *relement ) {
        (*rtn)( relement );
    } );
}


void * RINGNAME(Pred)(          // FIND PREVIOUS ELEMENT IN A RING
    void *hdr,                  // - ring header
    void *element )             // - element
{
    RingNode *rhdr = static_cast<RingNode *>( hdr );    // - ring header
    RingNode *pred;                                     // - previous element
    RingNode *next;                                     // - next element

    if( rhdr == nullptr ) {
        pred = nullptr;
    } else {
        for( pred = rhdr; ; ) {
            next = pred->next;
            if( element == next )
                break;
            pred = next;
            if( pred == rhdr ) {
                pred = nullptr;
                break;
            }
        }
    }
    return( pred );
}

void *RINGNAME(PruneWithPrev) ( // PRUNE ELEMENT FROM A RING (PREV ELT AVAILABLE)
    void *hdr,                  // - addr( ring header )
    void *element,              // - element to be pruned
    void *prv )                 // - element just before element
{
    RingNode **rhdr = static_cast<RingNode **>( hdr );          // - addr( ring header )
    RingNode *relement = static_cast<RingNode *>( element );    // - element to be pruned
    RingNode *prev = static_cast<RingNode *>( prv );            // - previous element

    if( prev == nullptr ) {
        prev = *rhdr;
    }
    prev->next = relement->next;
    if( prev == relement ) {
        *rhdr = nullptr;
    } else {
        if( *rhdr == relement ) {
            *rhdr = prev;
        }
    }
    relement->next = nullptr;
    return( relement );
}


void *RINGNAME(Prune) (         // PRUNE ELEMENT FROM A RING
    void *hdr,                  // - addr( ring header )
    void *element )             // - element to be pruned
{
    RingNode **rhdr = static_cast<RingNode **>( hdr );          // - addr( ring header )
    RingNode *prev = static_cast<RingNode *>( RINGNAME(Pred)( *rhdr, element ) );

    return( RINGNAME(PruneWithPrev)( hdr, element, prev ) );
}


void* RINGNAME(Push) (          // INSERT ELEMENT AT START OF RING
    void *hdr,                  // - addr( ring header )
    void *element )             // - element to be pushed
{
    RingNode **rhdr = static_cast<RingNode **>( hdr );          // - addr( ring header )
    RingNode *last = *rhdr;                                     // - last element
    RingNode *relement = static_cast<RingNode *>( element );    // - element to be inserted

    verifyNotInRing( last, relement );
    if( last == nullptr ) {
        relement->next = relement;
        *rhdr = relement;
    } else {
        relement->next = last->next;
        last->next = relement;
    }
    return( relement );
}

void * RINGNAME(Last) (         // RETURN LAST ELEMENT IN THE RING
    void *hdr )                 // - ring header
{
    return( hdr );
}

void * RINGNAME(First) (        // RETURN FIRST ELEMENT IN THE RING
    void *hdr )                 // - ring header
{
    return( static_cast<RingNode *>( hdr )->next );
}

void * RINGNAME(Pop) (          // PRUNE FIRST ELEMENT IN THE RING
    void *hdr )                 // - addr( ring header )
{
    RingNode **rhdr = static_cast<RingNode **>( hdr );  // - addr( ring header )
    RingNode *first = nullptr;                          // - first element
    RingNode *last = *rhdr;                             // - last element

    if( last != nullptr ) {
        first = last->next;
        if( first == last ) {
            *rhdr = nullptr;
        } else {
            last->next = first->next;
        }
    }
    return( first );
}


void * RINGNAME(Lookup) (       // LOOKUP IN A RING
    void *hdr,                  // - ring hdr
    ringcompare_fn *rtn,        // - comparison routine
    void *comparand )           // - comparand
{
    RingNode *rhdr = static_cast<RingNode *>( hdr );    // - ring hdr
    RingNode *curr;                                     // - current element

    if( rhdr == nullptr ) {
        curr = nullptr;
    } else {
        curr = rhdr;
        for( ; ; ) {
            curr = curr->next;
            if( (*rtn)( curr, comparand ) )
                break;
            if( curr == rhdr ) {
                curr = nullptr;
                break;
            }
        }
    }
    return( curr );
}


unsigned RINGNAME(Count) (      // COUNT ELEMENTS IN A RING
    void *hdr )                 // - ring hdr
{
    unsigned count = 0;         // - number elements

    ForEachInRing( hdr, [&count]( RingNode * ) {
        ++count;
    } );
    return( count );
}

void *RINGNAME(Alloc) (         // ALLOCATE AND APPEND NEW ELEMENT
    void *hdr,                  // - addr( ring header )
    size_t size )               // - size of entry to be allocated
{
    void *new_element = MemAllocSafe( size );   // - allocated element

    RINGNAME(Append)( hdr, new_element );
    return( new_element );
}


void RINGNAME(Dealloc) (        // DE-ALLOCATE A RING ELEMENT
    void *hdr,                  // - addr( ring header )
    void *element )             // - element to be de-allocated
{
    RINGNAME(Prune)( hdr, element );
    MemFree( element );
}


void RINGNAME(Free) (           // FREE ALL ELEMENTS IN A RING
    void *hdr )                 // - addr( ring header )
{
    void *elt;

    for( ;; ) {
        /* modify ring in an atomic manner */
        elt = RINGNAME(Pop)( hdr );
        if( elt == nullptr )
            break;
        MemFree( elt );
    }
}



//************************************************************************
// NOTE:: the following use carving technology
//***********************************************************************


void* RINGNAME(CarveAlloc) (    // CARVER ALLOC AND APPEND AN ENTRY
    carve_t carver,             // - carving control
    void *hdr )                 // - addr( ring header )
{
    void *elt = CarveAlloc( carver );

    RINGNAME(Append)( hdr, elt );
    return( elt );
}


void RINGNAME(CarveFree) (      // CARVER FREE ALL ELEMENTS IN A RING
    carve_t carver,             // - carving control
    void *hdr )                 // - addr( ring header )
{
    void *elt;

    for( ;; ) {
        elt = RINGNAME(Pop)( hdr );
        if( elt == nullptr )
            break;
        CarveFree( carver, elt );
    }
}


void *RINGNAME(Step) (  // STEP ALONG ELEMENTS (NULL -> e1 -> e2 -> NULL)
    void *hdr,          // - ring header
    void *elt )         // - curr element (NULL to start)
{
    RingNode *rhdr = static_cast<RingNode *>( hdr );    // - ring hdr

    if( elt == nullptr ) {
        /* start traversal */
        if( rhdr != nullptr ) {
            elt = rhdr->next;
        }
    } else {
        RingNode *relt = static_cast<RingNode *>( elt );    // - ring element

        if( relt != rhdr ) {
            elt = relt->next;
        } else {
            elt = nullptr;
        }
    }
    return( elt );
}
