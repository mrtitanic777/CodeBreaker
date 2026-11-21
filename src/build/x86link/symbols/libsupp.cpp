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
 *  LIBSUPP   : Library support
 *
 */

#include <string.h>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "pcobj.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/layout/distrib.h"
#include "build/x86link/symbols/symtrace.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/symbols/library.h"
#include "build/x86link/objects/procfile.h"
#include "build/x86link/objects/objio.h"

#include "clibext.h"


static bool SearchAndProcLibFile(Link& link,  file_list *file, const char *name )
/*******************************************************************/
{
    mod_entry       *lp;
    mod_entry       **prev;

    if( !CacheOpen(link,  file ) )
        return( false );
    lp = SearchLib(link,  file, name );
    if( lp == nullptr ) {
        CacheClose(link,  file, 1 );
        return( false );
    }
    file->flags_file |= STAT_LIB_USED;
#ifdef _EXE
    if( (link.FmtData.type & MK_OVERLAYS) && link.FmtData.u.dos.distribute ) {
        if( file->flags_file & STAT_LIB_FIXED ) {
            lp->flags_mod |= MOD_FIXED;
        }
        DistribAddMod(link,  lp, file->ovlref );
    } else {
#endif
        for( prev = &link.LibModules; *prev != nullptr; prev = &(*prev)->u.next ) /*  find end of list */
            /* nothing */;
        *prev = lp;
#ifdef _EXE
    }
#endif
    link.CurrMod = lp;
    ObjPass1(link);
    CacheClose(link,  file, 1 );
#ifdef _EXE
    if( (link.FmtData.type & MK_OVERLAYS) && link.FmtData.u.dos.distribute ) {
        DistribFinishMod(link,  lp );
    }
#endif
    if( FindLibTrace( lp ) ) {
        TraceSymList( lp->publist );
    }
    return( true );
}

static constexpr size_t PREFIX_LEN = sizeof( ImportSymPrefix ) - 1;

bool LibFind(Link& link,  const char *name, bool old_sym )
/*********************************************/
/* Search for a file in a library */
{
    file_list   *file;
    bool        isimpsym;

    DEBUG(( DBG_OLD, "LibFind( %s )", name ));
    isimpsym = (link.FmtData.type & MK_PE) && memcmp( name, ImportSymPrefix, PREFIX_LEN ) == 0;
    for( file = link.ObjLibFiles; file != nullptr; file = file->next ) {
        if( file->infile->status & INSTAT_IOERR )
            continue;
        if( old_sym && (file->flags_file & STAT_OLD_LIB) )
            continue;
        if( SearchAndProcLibFile(link,  file, name ) )
            return( true );
        if( isimpsym ) {
            if( SearchAndProcLibFile(link,  file, name + PREFIX_LEN ) ) {
                return( true );
            }
        }
    }
    return( false );
}

bool ModNameCompare( const char *tname, const char *membname )
/************************************************************/
// check if a THEADR record name is equal to a library member name
{
    size_t      lentheadr;
    size_t      lenmember;
    const char  *namestart;

    namestart = GetBaseName( tname, 0, &lentheadr );
    lenmember = strlen( membname );
    if( lentheadr == lenmember ) {
        if( strnicmp( namestart, membname, lenmember ) == 0 ) {
            return( true );
        }
    }
    return( false );
}

file_list *AddObjLib(Link& link,  const char *name, lib_priority priority )
/*************************************************************/
{
    file_list   **owner;
    file_list   **new_owner;
    file_list   *file;

    DEBUG(( DBG_OLD, "Adding Object library name %s", name ));
    /* search for new library position in linked list */
    for( owner = &link.ObjLibFiles; (file = *owner) != nullptr; owner = &file->next ) {
        if( file->priority < priority )
            break;
        /* end search if library already exists with same or a higher priority */
        if( FNAMECMPSTR( file->infile->name.u.ptr, name ) == 0 ) {
            return( file );
        }
    }
    new_owner = owner;
    /* search for library definition with a lower priority */
    for( ; (file = *owner) != nullptr; owner = &file->next ) {
        if( FNAMECMPSTR( file->infile->name.u.ptr, name ) == 0 ) {
            /* remove library entry from linked list */
            *owner = file->next;
            break;
        }
    }
    /* if we need to add one */
    if( file == nullptr ) {
        file = AllocNewFile(link,  nullptr );
        file->infile = AllocUniqueFileEntry(link,  name, link.UsrLibPath );
        file->infile->status |= INSTAT_LIBRARY | INSTAT_OPEN_WARNING;
        link.LinkState |= LS_LIBRARIES_ADDED;
    }
    /* put it to new position and setup priority */
    file->next = *new_owner;
    *new_owner = file;
    file->priority = priority;

    return( file );
}
