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


#include "global.h"
#include <cerrno>
#include <cstring>
#include <string>
#include "rcerrors.h"
#include "depinfo.h"
#include "reserr.h"
#include "wresdefn.h"
#include "rcrtns.h"
#include "rccore.h"
#include "memfuncs.h"
#include "run_state.hpp"

#include "clibext.h"


#if !defined( INSIDE_WLINK ) || defined( _OS2 )

namespace {

constexpr unsigned MAX_OPEN_RESFILES = 6;

// the last error wres reported
struct ErrFrame {
    bool        used = false;
    WResStatus  status = WResStatus();
    int         errnum = 0;
};

// What the file keeps for a run.
struct FileState {
    ErrFrame    errFromWres;
};

ErrFrame &errFromWres()
{
    return cb_wlink::RunState::part<FileState>().errFromWres;
}

// a name or type that WResIDToStr made (to free with MemFree), or what stands for it
struct IdText {
    std::string     text;       // a number's text
    char            *owned = nullptr;
    const char      *shown = nullptr;

    IdText() = default;
    IdText( const IdText & ) = delete;
    IdText &operator=( const IdText & ) = delete;
    ~IdText()
    {
        if( owned != nullptr )
            MemFree( owned );
    }
};

} // namespace

int RCCloseFile( FILE **fp )
{
    int rc = 0;
    if( *fp != nullptr ) {
        rc = ResCloseFile( *fp );
        *fp = nullptr;
    }
    return rc;
}

bool OpenResFiles( ExtraRes *resnames, ResFileInfo **presfiles, bool *allopen, ExeType type, const char *exename )
{
    unsigned        rescnt = 0;
    bool            dup_discarded;

    // an error: the files opened so far are closed
    auto fail = [presfiles]() {
        CloseResFiles( *presfiles );
        *presfiles = nullptr;
        return false;
    };

    *allopen = true;
    *presfiles = nullptr;
    for( ; resnames != nullptr; resnames = resnames->next ) {
        ResFileInfo *resfile = static_cast<ResFileInfo *>( MemAllocSafe( sizeof( ResFileInfo ) ) );
        resfile->next = *presfiles;
        *presfiles = resfile;
        resfile->Dir = nullptr;
        resfile->name = resnames->name;
        resfile->fp = ResOpenFileRO( resfile->name );
        if( resfile->fp == nullptr ) {
            RcError( ERR_CANT_OPEN_FILE, resfile->name, LastWresErrStr() );
            return fail();
        }
        if( WResReadDir2( resfile->fp, &resfile->Dir, &dup_discarded, resfile ) ) {
            switch( LastWresStatus() ) {
            case WRS_BAD_SIG:
                RcError( ERR_INVALID_RES, resfile->name );
                break;
            case WRS_BAD_VERSION:
                RcError( ERR_BAD_RES_VER, resfile->name );
                break;
            default:
                RcError( ERR_READING_RES, resfile->name, LastWresErrStr() );
                break;
            }
            return fail();
        }
        if( rescnt >= MAX_OPEN_RESFILES ) {
            RCCloseFile( &(resfile->fp) );
            *allopen = false;
        }
        /*
         * remove the autodepend resource
         */
        WResID *res_id = WResIDFromStr( DEP_LIST_NAME );
        WResID *type_id = WResIDFromNum( DEP_LIST_TYPE );
        WResDelResource( resfile->Dir, type_id, res_id );
        WResIDFree( res_id );
        WResIDFree( type_id );

        const WResTargetOS res_os = static_cast<WResTargetOS>( resfile->Dir->TargetOS );
        switch( type ) {
        case EXE_TYPE_NE_WIN:
            if( res_os != WRES_OS_WIN16 ) {
                RcError( ERR_NONWIN_RES_TO_WIN_EXE, resfile->name, exename );
                return fail();
            }
            break;
        case EXE_TYPE_NE_OS2:
            /*
             * No way to tell MS and IBM resource files apart, and I can't find
             * a good way to figure out if this is a Watcom .res file
             */
            if( res_os != WRES_OS_WIN16
              && res_os != WRES_OS_OS2 ) {
                RcError( ERR_NONWIN_RES_TO_WIN_EXE, resfile->name, exename );
                return fail();
            }
            break;
        case EXE_TYPE_PE:
            if( res_os != WRES_OS_WIN32 ) {
                RcError( ERR_NONNT_RES_TO_NT_EXE, resfile->name, exename );
                return fail();
            }
            break;
        case EXE_TYPE_LX:
            /*
             * Same problem as with EXE_TYPE_NE_OS2
             */
            if( res_os != WRES_OS_OS2
              && res_os != WRES_OS_WIN16 ) {
                RcError( ERR_NONOS2_RES_TO_OS2_EXE, resfile->name, exename );
                return fail();
            }
            break;
        default: /* EXE_TYPE_UNKNOWN */
            break;
        }
        rescnt++;
    }
    return true;
}

void CloseResFiles( ResFileInfo *resfiles )
{
    ResFileInfo         *res;

    while( (res = resfiles) != nullptr ) {
        resfiles = res->next;
        WResFreeDir( res->Dir );
        RCCloseFile( &(res->fp) );
        MemFree( res );
    }
}

bool WresRecordError( WResStatus status )
{
    ErrFrame &err = errFromWres();

    err.used = true;
    err.status = status;
    err.errnum = errno;
    return true;
}

const char *LastWresErrStr( void )
{
    const ErrFrame &err = errFromWres();

    if( err.used ) {
        switch( err.status ) {
        case WRS_READ_INCOMPLETE:
            return "Unexpected end of file";
        default:
            return std::strerror( err.errnum );
        }
    } else {
        return "";
    }
}

int LastWresErr( void )
{
    const ErrFrame &err = errFromWres();

    if( err.used ) {
        return err.errnum;
    }
    return 0;
}

int LastWresStatus( void )
{
    const ErrFrame &err = errFromWres();

    if( err.used ) {
        return err.status;
    }
    return 0;
}

void SharedIOInitStatics( void )
{
    errFromWres() = ErrFrame();
}

void ReportDupResource( WResID *res_id, WResID *type_id, const char *file1, const char *file2, bool warn )
{
    IdText      type;
    IdText      name;

    if( type_id->IsName ) {
        type.owned = WResIDToStr( type_id );
        type.shown = type.owned;
    } else {
        switch( type_id->ID.Num ) {
        case RESOURCE2INT( RT_CURSOR ):
        case RESOURCE2INT( RT_GROUP_CURSOR ):
            type.shown = "cursor";
            break;
        case RESOURCE2INT( RT_BITMAP ):
            type.shown = "bitmap";
            break;
        case RESOURCE2INT( RT_ICON ):
        case RESOURCE2INT( RT_GROUP_ICON ):
            type.shown = "icon";
            break;
        case RESOURCE2INT( RT_MENU ):
            type.shown = "menu";
            break;
        case RESOURCE2INT( RT_DIALOG ):
            type.shown = "dialog";
            break;
        case RESOURCE2INT( RT_FONTDIR ):
            type.shown = "font directory";
            break;
        case RESOURCE2INT( RT_FONT ):
            type.shown = "font";
            break;
        case RESOURCE2INT( RT_ACCELERATOR ):
            type.shown = "accelerator";
            break;
        case RESOURCE2INT( RT_RCDATA ):
            type.shown = "rcdata";
            break;
        case RESOURCE2INT( RT_VERSIONINFO ):
            type.shown = "version info";
            break;
        case RESOURCE2INT( RT_MESSAGETABLE ):
            type.shown = "message table";
            break;
        case RESOURCE2INT( RT_TOOLBAR ):
            type.shown = "toolbar";
            break;
        default:
            type.text = std::to_string( static_cast<unsigned>( type_id->ID.Num ) );
            type.shown = type.text.c_str();
            break;
        }
    }

    if( res_id->IsName ) {
        name.owned = WResIDToStr( res_id );
        name.shown = name.owned;
    } else {
        name.text = std::to_string( static_cast<unsigned>( res_id->ID.Num ) );
        name.shown = name.text.c_str();
    }
    // the same message, as a warning or an error
    auto report = [warn]( unsigned errornum, auto... args ) {
        if( warn ) {
            RcWarning( errornum, args... );
        } else {
            RcError( errornum, args... );
        }
    };
    if( !type_id->IsName
      && type_id->ID.Num == RESOURCE2INT( RT_STRING ) ) {
        const unsigned strbase = ( res_id->ID.Num - 1 ) * 16;
        if( file1 != nullptr
          && file2 != nullptr ) {
            report( ERR_DUPLICATE_STR_TABLE_FILE, strbase, strbase + 15, file1, file2 );
        } else {
            report( ERR_DUPLICATE_STR_TABLE, strbase, strbase + 15 );
        }
    } else if( file1 != nullptr
      && file2 != nullptr ) {
        report( ERR_DUPLICATE_RES_FILE, type.shown, name.shown, file1, file2 );
    } else {
        report( ERR_DUPLICATE_RES, type.shown, name.shown );
    }
}

#endif
