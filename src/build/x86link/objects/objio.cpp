/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2025-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Object file i/o interface routines
*
****************************************************************************/


#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/driver/ideentry.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/objects/objio.h"

#include "clibext.h"
#include <array>



void ResetObjIO(Link& link)
/****************************/
{
    link.CachedFiles = nullptr;
    link.CachedLibFiles = nullptr;
}

static infilelist *AllocEntry(Link& link,  const char *name, const path_entry *path )
/***********************************************************************/
{
    infilelist  *infile;

    infile = static_cast<infilelist *>( _PermAlloc( sizeof( infilelist ) ) );
    infile->name.u.ptr = AddStringStringTable( &link.PermStrings, name );
    infile->path_list = path;
    infile->prefix = nullptr;
    infile->handle = NIL_FHANDLE;
    infile->cache = nullptr;
    infile->len = 0;
    infile->status = static_cast<infile_status>( 0 );
    return( infile );
}

infilelist *AllocFileEntry(Link& link,  const char *name, const path_entry *path )
/********************************************************************/
{
    infilelist  *infile;

    infile = AllocEntry(link,  name, path );
    infile->next = link.CachedFiles;
    link.CachedFiles = infile;
    return( infile );
}

infilelist *AllocUniqueFileEntry(Link& link,  const char *name, const path_entry *path )
/**************************************************************************/
{
    infilelist  *infile;

    for( infile = link.CachedLibFiles; infile != nullptr; infile = infile->next ) {
        if( FNAMECMPSTR( infile->name.u.ptr, name ) == 0 ) {
            return( infile );    // we found 1 with the same name.
        }
    }
    infile = AllocEntry(link,  name, path );   // didn't find one, so allocate a new 1
    if( link.CachedLibFiles == nullptr ) {      // add libraries to the end of the
        link.CachedLibFiles = infile;         // regular cached files list.
        AppendList( reinterpret_cast<void **>( &link.CachedFiles ), infile );
    } else {
        AppendList( reinterpret_cast<void **>( &link.CachedLibFiles ), infile );
    }
    return( infile );
}

bool CleanCachedHandles(Link& link)
/************************************/
{
    infilelist *infile;

    for( infile = link.CachedFiles; infile != nullptr; infile = infile->next ) {
        if( (infile->status & INSTAT_IN_USE) == 0
          && infile->handle != NIL_FHANDLE ) {
            break;
        }
    }
    if( infile == nullptr )
        return( false );
    QClose( infile->handle, infile->name.u.ptr );
    infile->handle = NIL_FHANDLE;
    return( true );
}

char *MakePath( char *fullname, const char **path_list )
/******************************************************/
{
    char        *p;
    char        c;

    p = fullname;
    if( *path_list != nullptr
      && (c = **path_list) != '\0' ) {
        do {
            ++(*path_list);
            if( IS_PATH_LIST_SEP( c ) ) {
                break;
            }
            *p++ = c;
        } while( (c = **path_list) != '\0' );
        c = p[-1];
        if( !IS_PATH_SEP( c ) ) {
            *p++ = DIR_SEP;
        }
    }
    return( p );
}

char *MakeFileName( infilelist *infile, char *fullname )
/******************************************************/
{
    const char  *path = infile->prefix;

    strcpy( MakePath( fullname, &path ), infile->name.u.ptr );
    return( const_cast<char *>( path ) );
}

bool MakeFileNameFromList( const char **path_list, char *name, char *fullname )
/*****************************************************************************/
{
    if( *path_list != nullptr
      && **path_list != '\0' ) {
        strcpy( MakePath( fullname, path_list ), name );
        return( true );
    }
    return( false );
}

static f_handle PathObjOpen( Link& link, const char *path_ptr, char *name, char *new_name, infilelist *infile )
/*************************************************************************************************/
{
    f_handle    fp;

    fp = NIL_FHANDLE;
    for( ;; ) {
        infile->prefix = path_ptr;
        if( !MakeFileNameFromList( &path_ptr, name, new_name ) ) {
            infile->prefix = nullptr;
            break;
        }
        fp = QObjOpen( link, new_name );
        if( fp != NIL_FHANDLE ) {
            break;
        }
    }
    return( fp );
}

bool DoObjOpen(Link& link,  infilelist *infile )
/**********************************/
{
    char                *name;
    f_handle            fp;
    unsigned            err;
    std::array<char, PATH_MAX>  new_name;
    const path_entry    *searchpath;

    name = infile->name.u.ptr;
    if( infile->handle != NIL_FHANDLE )
        return( true );
    infile->currpos = 0;
    if( HAS_PATH( name ) ) {   // has path defined
        infile->path_list = nullptr;
        fp = QObjOpen( link, name );
    } else if( infile->prefix != nullptr ) {     // already searched path
        MakeFileName( infile, new_name.data() );
        fp = QObjOpen( link, new_name.data() );
    } else {                                // new, no searched path
        fp = NIL_FHANDLE;
        if( (infile->status & INSTAT_LIB_SEARCH)
          || infile->path_list == nullptr ) {
            /* try in current directory */
            fp = QObjOpen( link, name );
        }
        if( fp == NIL_FHANDLE ) {
            for( searchpath = infile->path_list; searchpath != nullptr; searchpath = searchpath->next ) {
                fp = PathObjOpen( link, searchpath->name, name, new_name.data(), infile );
                if( fp != NIL_FHANDLE ) {
                    break;
                }
            }
            if( fp == NIL_FHANDLE
              && (infile->status & INSTAT_USE_LIBPATH) ) {
                if( link.LibPath != nullptr ) {
                    fp = PathObjOpen( link, link.LibPath, name, new_name.data(), infile );
                }
            }
        }
    }
    if( fp != NIL_FHANDLE ) {
        if( (infile->status & INSTAT_GOT_MODTIME) == 0 ) {
            infile->modtime = QFModTime( fp );
        }
        infile->handle = fp;
        return( true );
    } else if( (infile->status & INSTAT_NO_WARNING) == 0 ) {
        err = ( infile->status & INSTAT_OPEN_WARNING ) ? WRN+MSG_CANT_OPEN : ERR+MSG_CANT_OPEN;
        PrintIOError( err, "12", name );
        infile->prefix = nullptr;
        infile->handle = NIL_FHANDLE;
    }
    return( false );
}

void InitTokBuff(Link& link)
/*****************************/
{
    link.TokSize = MAX_HEADROOM;
    link.TokBuff = static_cast<char *>( MemAllocSafe( MAX_HEADROOM ) );
}

void FreeTokBuffs(Link& link)
/******************************/
{
    if( link.TokBuff != nullptr ) {
        MemFree( link.TokBuff );
        link.TokBuff = nullptr;
    }
}

void BadObject(Link& link)
/***************************/
{
    link.CurrMod->u1.source->infile->status |= INSTAT_IOERR;
    LnkMsg( ERR+LOC+MSG_OBJ_FILE_ATTR, nullptr );
}

void EarlyEOF(Link& link)
/**************************/
{
    link.CurrMod->u1.source->infile->status |= INSTAT_IOERR;
    Locator( link.CurrMod->u1.source->infile->name.u.ptr, nullptr, 0 );
    LnkMsg( ERR+MSG_EARLY_EOF, nullptr );
}
