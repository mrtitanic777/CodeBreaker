/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2018 The Open Watcom Contributors. All Rights Reserved.
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


#include "_cgstd.h"
#include "cgmem.h"
#include "memout.h"
#include "cgsrtlst.h"
#include "memmgt.h"
#include "backrun.hpp"


inline void *&NextOf( void *list, unsigned int offset ) { return (*((void **)((char *)list + offset))); }

static  void            ShellSort( BackRun& back, void **array, unsigned length,
                                   bool (*before)(BackRun&, void *,void *) );

static  void    *BuildList( void **array, unsigned next_offset, unsigned length )
/*******************************************************************************/
{
    void        *list;
    void        **owner;

    list = NULL;
    owner = &list;
    for( ; length != 0; --length ) {
        *owner = *array;
        NextOf( *owner, next_offset ) = NULL;
        owner = &NextOf( *owner, next_offset );
        ++array;
    }
    return( list );
}


static  void            *MergeList( BackRun& back, void *list1, void *list2,
                                   unsigned next_offset,
                                   bool (*before)(BackRun&, void *,void *) ) {
/***************************************************************/

    void        *list;
    void        *next;
    void        **owner;

    list = NULL;
    owner = &list;
    while( list1 != NULL && list2 != NULL ) {
        if( (*before)( back, list1, list2 ) ) {
            *owner = list1;
            owner = &NextOf( list1, next_offset );
            next = NextOf( list1, next_offset );
            NextOf( list1, next_offset ) = NULL;
            list1 = next;
        } else {
            *owner = list2;
            owner = &NextOf( list2, next_offset );
            next = NextOf( list2, next_offset );
            NextOf( list2, next_offset ) = NULL;
            list2 = next;
        }
    }
    if( list1 != NULL ) {
        *owner = list1;
    } else if( list2 != NULL ) {
        *owner = list2;
    }
    return( list );
}


static  void            *DoSortList( BackRun& back, void *list, unsigned next_offset,
                                  bool (*before)(BackRun&, void *,void *),
                                  unsigned length ) {
/**************************************************************************/

    void        **array;
    void        **parray;
    void        *list2;
    unsigned    mid;
    int         i;

    if( list == NULL )
        return( NULL );
    array = static_cast<void **>(CGAlloc( back, length * sizeof( void * ) ));
    if( array == NULL ) {
        mid = length / 2;
        if( mid == 0 )
            return( list ); /* FATAL ERROR! */
        list2 = list;
        for( i = mid; i > 0; --i ) {
            list2 = NextOf( list2, next_offset );
        }
        list = DoSortList( back, list, next_offset, before, mid );
        list2 = DoSortList( back, list2, next_offset, before, length - mid );
        list = MergeList( back, list, list2, next_offset, before );
    } else {
        list2 = list;
        parray = array;
        for( i = length; i > 0; --i ) {
            *parray++ = list2;
            list2 = NextOf( list2, next_offset );
        }
        ShellSort( back, array, length, before );
        list = BuildList( array, next_offset, length );
        CGFree( back, array );
    }
    return( list );
}

static  void            ShellSort( BackRun& back, void **array, unsigned length,
                                   bool (*before)(BackRun&, void *,void *) ) {
/****************************************************************/


    unsigned    i;
    bool        swap;
    void        *t;
    unsigned    gap;
    unsigned    adjust;

    gap = length;
    adjust = 1;
    do {
        adjust = !adjust;
        gap = gap / 2 + adjust;
        do {
            swap = false;
            for( i=0; i<(length-gap); ++i ) {
                if( (*before)( back, array[i + gap], array[i] ) ) {
                    t = array[i];
                    array[i] = array[i + gap];
                    array[i + gap] = t;
                    swap = true;
                }
            }
        } while( swap );
    } while( gap != 1 );
}


void *SortList( BackRun& back, void *list, unsigned next_offset, bool (*before)(BackRun&, void *,void *) )
/*******************************************************************************/
{
    void                *list2;
    unsigned            length;
    mem_out_action      old_memout;

    length = 0;
    for( list2 = list; list2 != NULL; list2 = NextOf( list2, next_offset ) ) {
        ++length;
    }
    if( length > 1 ) {
        old_memout = SetMemOut( back, MO_OK );
        list = DoSortList( back, list, next_offset, before, length );
        SetMemOut( back, old_memout );
    }
    return( list );
}
