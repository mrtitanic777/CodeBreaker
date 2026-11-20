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
* Description:  symbol/module tracing routines
*
****************************************************************************/


#include <string.h>
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "library.h"
#include "overlays.h"
#include "symtrace.h"
#include "run_state.hpp"


typedef struct trace_info {
    struct trace_info       *next;
    union {
        char                *name;  /* module or library name */
        file_list           *file;
    } u;
    char                    *membname;
    bool                    found;
} trace_info;

typedef struct sym_trace_info {
    struct sym_trace_info   *next;
    char                    *name;
} sym_trace_info;

namespace {

struct TraceState {
    trace_info      *TraceList = nullptr;
    sym_trace_info  *TraceListSym = nullptr;
};

inline TraceState &st() { return cb_wlink::RunState::part<TraceState>(); }

}

void ResetSymTrace( void )
/************************/
{
    TraceState &s = st();
    s.TraceListSym = nullptr;
    s.TraceList = nullptr;
}

static void CheckFileTrace( section *sect, void *_info )
/******************************************************/
{
    file_list       *file;
    trace_info      *info = static_cast<trace_info *>(_info);

    if( info->found )
        return;
    for( file = sect->files; file != nullptr; file = file->next ) {
        if( FNAMECMPSTR( file->infile->name.u.ptr, info->u.name ) == 0 ) {
            info->found = true;
            MemFree( info->u.name );
            file->flags_file |= STAT_TRACE_SYMS;
            return;
        }
    }
}

void AddTraceListMod( char *fname, char *membname )
/*************************************************/
{
    trace_info      *info;

    /*
     * it uses membname until tracing is finished
     * then CleanTraces free it
     */
    info = static_cast<trace_info *>(MemAllocSafe( sizeof( trace_info ) ));
    info->u.name = fname;
    info->membname = membname;
    info->found = false;        // used for matching libraries
    info->next = st().TraceList;
    st().TraceList = info;
}

void CheckTraces( void )
/**********************/
// first check for .obj files being traced, then check libraries
{
    trace_info      *info;
    trace_info      *next;
    file_list       *file;
    trace_info **   prev;
    TraceState      &s = st();

    prev = &s.TraceList;
    for( info = s.TraceList; info != nullptr; info = next ) {
        next = info->next;
        if( info->membname == nullptr ) {
            ParmWalkAllSects( CheckFileTrace, info );
            if( !info->found ) {
                LnkMsg( WRN+MSG_TRACE_OBJ_NOT_FOUND, "s", info->u.name );
                MemFree( info->u.name );
            }
            MemFree( info );
        } else {
            for( file = wlink().ObjLibFiles; file != nullptr; file = file->next ) {
                if( FNAMECMPSTR( file->infile->name.u.ptr, info->u.name ) == 0 ) {
                    info->found = true;
                    MemFree( info->u.name );
                    info->u.file = file;
                    break;
                }
            }
            *prev = info;
            prev = &info->next;
        } /* if */
    } /* for */
    *prev = nullptr;
}

void CheckLibTrace( file_list *file )
/***********************************/
{
    trace_info      *info;

    for( info = st().TraceList; info != nullptr; info = info->next ) {
        if( !info->found ) {
            if( FNAMECMPSTR( info->u.name, file->infile->name.u.ptr ) == 0 ) {
                info->found = true;
                MemFree( info->u.name );
                info->u.file = file;
                break;
            }
        }
    }
}

bool FindLibTrace( mod_entry *mod )
/*********************************/
{
    trace_info **   prev;
    trace_info      *info;

    prev = &st().TraceList;
    for( info = st().TraceList; info != nullptr; info = info->next ) {
        if( info->found && info->u.file == mod->u1.source ) {
            if( ModNameCompare( mod->name.u.ptr, info->membname ) ) {
                *prev = info->next;
                MemFree( info->membname );
                MemFree( info );
                return( true );
            }
        }
        prev = &info->next;
    }
    return( false );
}

void AddTraceListSym( char *symname )
/***********************************/
{
    sym_trace_info  *info;

    info = static_cast<sym_trace_info *>(MemAllocSafe( sizeof( sym_trace_info ) ));
    info->name = symname;
    info->next = st().TraceListSym;
    st().TraceListSym = info;
}

bool FindSymTrace( const char *symname )
/**************************************/
{
    sym_trace_info  *info;
    sym_trace_info  **prev;

    prev = &st().TraceListSym;
    for( info = st().TraceListSym; info != nullptr; info = info->next ) {
        if( strcmp( info->name, symname ) == 0 ) {
            *prev = info->next;
            MemFree( info->name );
            MemFree( info );
            return( true );
        }
        prev = &(info->next);
    }
    return( false );
}

void PrintBadTraces( void )
/*************************/
{
    sym_trace_info  *sym_info;
    trace_info      *info;

    for( sym_info = st().TraceListSym; sym_info != nullptr; sym_info = sym_info->next ) {
        LnkMsg( WRN+MSG_TRACE_SYM_NOT_FOUND, "s", sym_info->name );
    }
    for( info = st().TraceList; info != nullptr; info = info->next ) {
        if( info->found ) {
            LnkMsg( WRN+MSG_TRACE_LIB_NOT_FOUND, "12", info->u.file->infile->name.u.ptr, info->membname );
        } else {
            LnkMsg( WRN+MSG_TRACE_LIB_NOT_FOUND, "12", info->u.name, info->membname );
        }
    }
    CleanTraces();
}

void CleanTraces( void )
/**********************/
{
    TraceState  &s = st();

    while( s.TraceListSym != nullptr ) {
        sym_trace_info  *next = s.TraceListSym->next;
        MemFree( s.TraceListSym->name );
        MemFree( s.TraceListSym );
        s.TraceListSym = next;
    }
    while( s.TraceList != nullptr ) {
        trace_info      *next = s.TraceList->next;
        if( !s.TraceList->found ) {
            MemFree( s.TraceList->u.name );
        }
        MemFree( s.TraceList->membname );
        MemFree( s.TraceList );
        s.TraceList = next;
    }
}
