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
* Description:  C compiler top level driver module and file I/O.
*
****************************************************************************/


#include "cvars.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "iopath.h"
#include "scan.h"
#include "jmpbuf.h"
#include "cgdefs.h"
#include "feprotos.h"
#include "swchar.h"
#include "cfeinfo.h"
#include "pathlist.h"
#include "cmacadd.h"
#include "pathgrp2.h"
#include "cscanbuf.h"

#include "clibint.h"
#include "clibext.h"
#include "frontrun.hpp"


 #define C_PATH         "..\\c\\"
 #define H_PATH         "..\\h\\"
 #define OBJ_EXT        "obj"
#define DEF_EXT         "def"
#define ERR_EXT         "err"
#define MBR_EXT         "mbr"
#define C_EXT           "c"
#define CPP_EXT         "i"
#define DEP_EXT         "d"

#define NULLDEV         "NUL"

#define STDIN_NAME      "stdin"
#define NULLDEV_NAME    "nulldev"


void FrontEndInit( FrontRun& front, bool reuse )
/******************************
 * Do the once only things
 ******************************/
{
    front.cvars.GlobalCompFlags.cc_reuse = reuse;
    front.cvars.GlobalCompFlags.cc_first_use = true;
    front.cvars.GlobalCompFlags.ignore_environment = false;
    front.cvars.GlobalCompFlags.ignore_current_dir = false;
    front.cvars.GlobalCompFlags.ide_cmd_line_has_files = false;
    front.cvars.GlobalCompFlags.ide_console_output = false;
    front.cvars.GlobalCompFlags.progress_messages = false;
//    GlobalCompFlags.dll_active = false;
}

void FrontEndFini( FrontRun& front )
/************************
 * Fini the once only
 ************************/
{
    front.cvars.GlobalCompFlags.cc_reuse = false;
    front.cvars.GlobalCompFlags.cc_first_use = true;
}

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CcmainState: ccmain_state.h) */

}  // namespace


static void initGlobals( FrontRun& front )
{
    InitStats(front);
    front.ccmain.IsStdIn = false;
    front.ccmain.IsNullDev = false;
    front.cvars.FNames = NULL;
    front.cvars.RDirNames = NULL;
    front.cvars.IAliasNames = NULL;
    front.cvars.SrcFiles = NULL;
    front.cvars.ErrFile = NULL;
    front.cvars.DefFile = NULL;
    front.cvars.CppFile = NULL;
    front.cvars.DepFile = NULL;
    front.cvars.IncPathList = static_cast<char *>(CMemAlloc( front, 1 ));
    front.cvars.SegmentNum = FIRST_PRIVATE_SEGMENT;
    InitBuffer( front, BUF_SIZE );
    InitTokenBuf( front, BUF_SIZE );
    front.ccmain.FNameBuf = static_cast<char *>(CMemAlloc( front, _MAX_PATH ));
    InitIncFile(front);
    InitErrLoc(front);
}

static void finiGlobals( FrontRun& front )
{
    FiniBuffer(front);
}

static bool ParseCmdLine( FrontRun& front, char **cmdline )
{
    char        *cmd;

    cmd = const_cast<char *>((cmdline == NULL) ?  "" : cmdline[0]);  // from cpp.c
    if( cmd == NULL ) { // argv == 1
        cmd = const_cast<char *>("");
    }
    while( *cmd == ' ' )
        ++cmd;
    if( *cmd == '?' || *cmd == '\0' ) {
        CUsage(front);
        return( false );
    }
    GenCOptions( front, cmdline );
    return( true );
}

static void OpenDepFile( FrontRun& front )
{
    char        *name;

    if( front.cvars.CompFlags.generate_auto_depend ) {
        name = DepFileName(front);
        if( name != NULL ) {
            front.cvars.DepFile = front.held->fopen( name, "w" );
            if( front.cvars.DepFile != NULL ) {
                setvbuf( front.cvars.DepFile, static_cast<char *>(CPermAlloc( front, 1024 )), _IOFBF, 1024 );
            }
        }
    }
}

char *ForceSlash( char *name, char slash )
{
    char    *save;

    save = name;
    if( slash != '\0' && name != NULL ) {
        while( name[0] != '\0' ) {
            if( name[0] == '\\' || name[0] == '/' )
                name[0] = slash;
            name++;
        }
    }
    return( save );
}

FNAMEPTR NextDependency( FrontRun& front, FNAMEPTR curr )
{
    if( curr == NULL ) {
        curr = front.cvars.FNames;
    } else {
        curr = curr->next;
    }
    for( ; curr != NULL; curr = curr->next ) {
        if( curr->rwflag && !SrcFileInRDir( front, curr ) ) {
            break;
        }
    }
    return( curr );
}


static void DelErrFile(FrontRun& front)
{
    char        *name;

    name = ErrFileName(front);
    if( name != NULL ) {
        remove( name );
    }
}

static void DelDepFile( FrontRun& front )
{
    char        *name;

    name = DepFileName(front);
    if( name != NULL ) {
        remove( name );
    }
}

static char *createFileName( FrontRun& front, const char *template_, const char *ext, bool forceext )
{
    pgroup2     pg;
    bool        use_defaults;
    const char  *module_name;

    use_defaults = ( template_ == NULL );
    if( use_defaults )
        template_ = front.cvars.WholeFName;
    _splitpath2( template_, pg.buffer, &pg.drive, &pg.dir, &pg.fname, &pg.ext );
    if( use_defaults ) {
        /*
         * default object file goes in current directory
         */
        pg.drive = const_cast<char *>("");
        pg.dir = const_cast<char *>("");
    }
    if( !forceext && pg.ext[0] != '\0' && !use_defaults ) {
        ext = pg.ext;
    }
    module_name = pg.fname;
    if( module_name[0] == '\0' || module_name[0] == '*' ) {
        module_name = FEModuleName(front);
    }
    _makepath( front.ccmain.FNameBuf, pg.drive, pg.dir, module_name, ext );
    return( front.ccmain.FNameBuf );
}

static void DumpDepFile( FrontRun& front )
{
    FNAMEPTR    curr;

    OpenDepFile(front);
    if( front.cvars.DepFile != NULL ) {
        curr = NextDependency( front, NULL );
        if( curr != NULL ) {
            fprintf( front.cvars.DepFile, "%s :", ForceSlash( createFileName( front, front.cvars.DependTarget, OBJ_EXT, false ), front.cvars.DependForceSlash ) );
            fprintf( front.cvars.DepFile, " %s", ForceSlash( GetSourceDepName(front), front.cvars.DependForceSlash ) );
            for( curr = NextDependency( front, curr ); curr != NULL; curr = NextDependency( front, curr ) ) {
                fprintf( front.cvars.DepFile, " %s", ForceSlash( curr->name, front.cvars.DependForceSlash ) );
            }
            fprintf( front.cvars.DepFile, "\n" );
        }
        front.held->fclose( front.cvars.DepFile );
        front.cvars.DepFile = NULL;
    } else {
        DelDepFile(front);
    }
}

static IALIASPTR AddIAlias( FrontRun& front, const char *alias_name, const char *real_name, bool is_lib )
{
    size_t      alias_len;
    size_t      real_len;
    IALIASPTR   alias, old_alias;
    IALIASPTR   *lnk;

    for( lnk = &front.cvars.IAliasNames; (old_alias = *lnk) != NULL; lnk = &old_alias->next ) {
        if( ( old_alias->is_lib == is_lib ) && !FNAMECMPSTR( alias_name, old_alias->alias_name ) ) {
            break;
        }
    }
    alias_len = strlen( alias_name ) + 1;
    real_len = strlen( real_name ) + 1;
    alias = static_cast<IALIASPTR>(CMemAlloc( front, offsetof( ialias_list, alias_name ) + alias_len + real_len ));
    alias->next = NULL;
    alias->is_lib = is_lib;
    alias->real_name = alias->alias_name + alias_len;
    strcpy( alias->alias_name, alias_name );
    strcpy( alias->real_name, real_name );
    *lnk = alias;
    if( old_alias != NULL ) {
        /*
         * Replace old alias if it exists
         */
        alias->next = old_alias->next;
        CMemFree( front, old_alias );
    }

    return( alias );
}

static void FreeIAlias( FrontRun& front )
{
    IALIASPTR   aliaslist;

    while( (aliaslist = front.cvars.IAliasNames) != NULL ) {
        front.cvars.IAliasNames = aliaslist->next;
        CMemFree( front, aliaslist );
    }
    front.cvars.IAliasNames = NULL;
}

static const char *IncludeAlias( FrontRun& front, const char *filename, bool is_lib )
{
    IALIASPTR   alias;

    for( alias = front.cvars.IAliasNames; alias != NULL; alias = alias->next ) {
        if( ( alias->is_lib == is_lib ) && !FNAMECMPSTR( filename, alias->alias_name ) ) {
            return( alias->real_name );
        }
    }
    return( filename );
}

void PrtfFilenameErr( FrontRun& front, const char *filename, src_file_type typ, bool print_error )
{
    /* unused parameters */ (void)typ;

    if( print_error ) {
        CErr2p( front, ERR_CANT_OPEN_FILE, filename );
    }
}

static bool openForcePreInclude( FrontRun& front )
{
    bool    ok;

    ok = OpenSrcFile( front, front.cvars.ForcePreInclude, FT_HEADER_PRE );
    if( !ok )
        PrtfFilenameErr( front, front.cvars.ForcePreInclude, FT_HEADER_PRE, false );
    return( ok );
}


static void MakePgmName( FrontRun& front )
/******************************
 * open the primary source file, and return pointer to root file name
 */
{
    size_t      len;
    pgroup2     pg;

    /*
     * Get fname, if file name has no extension whack ".c" on
     * if stdin a "." then replace with "stdin" don't whack ".c"
     * If no module name make the same as fname
     */
    if( front.cvars.WholeFName[0] == '.' && front.cvars.WholeFName[1] == '\0' ) {
        front.ccmain.IsStdIn = true;
        front.ccmain.IsNullDev = false;
        CMemFree( front, front.cvars.WholeFName );
        front.cvars.WholeFName = static_cast<char *>(CMemAlloc( front, sizeof( STDIN_NAME ) ));
        strcpy( front.cvars.WholeFName, STDIN_NAME );
        pg.fname = front.cvars.WholeFName;
    } else if( FNAMECMPSTR( front.cvars.WholeFName, NULLDEV ) == 0 ) {
        front.ccmain.IsNullDev = true;
        front.ccmain.IsStdIn = false;
        CMemFree( front, front.cvars.WholeFName );
        front.cvars.WholeFName = static_cast<char *>(CMemAlloc( front, sizeof( NULLDEV_NAME ) ));
        strcpy( front.cvars.WholeFName, NULLDEV_NAME );
        pg.fname = front.cvars.WholeFName;
    } else {
        _splitpath2( front.cvars.WholeFName, pg.buffer, &pg.drive, &pg.dir, &pg.fname, &pg.ext );
        if( pg.ext[0] == '\0' ) { // no extension
            len = strlen( front.cvars.WholeFName );
            front.cvars.WholeFName = static_cast<char *>(CMemRealloc( front, front.cvars.WholeFName, len + 1 + ( sizeof( C_EXT ) - 1 ) + 1 ));
            strcat( front.cvars.WholeFName + len, "." C_EXT );
        }
    }
    front.cvars.SrcFName = CMemStrdup( front, pg.fname );
}

static void CantOpenFile( FrontRun& front, const char *name )
{
    char    msgtxt[80];
    char    msgbuf[MAX_MSG_LEN];

    CGetMsg( front, msgtxt, ERR_CANT_OPEN_FILE );
    sprintf( msgbuf, msgtxt, name );
    ConsErrMsgVerbatim( front, msgbuf );
}


void SetPPWidth( FrontRun& front, unsigned width )
{
    front.ccmain.CppWidth = width - 1;
}

static void CppPutc( FrontRun& front, int ch )
{
    int     rc;

    if( front.ccmain.CppFirstChar ) {
        front.ccmain.CppFirstChar = false;
        if( ch == '\n' ) {
            return;
        }
    }
    if( ch == '\n' ) {
        front.ccmain.CppColumn = 0;
        rc = fputc( '\n', front.cvars.CppFile );
    } else if( front.ccmain.CppColumn >= front.ccmain.CppWidth ) {
        front.ccmain.CppColumn = 1;
        if( front.ccmain.CommentChar == '\0' ) {
            if( (rc = fputc( '\\', front.cvars.CppFile )) != EOF ) {
                rc = fputc( '\n', front.cvars.CppFile );
            }
        } else if( (rc = fputc( '\n', front.cvars.CppFile )) != EOF ) {
            if( front.ccmain.CommentChar == '/' ) {
                front.ccmain.CppColumn += 2;
                if( (rc = fputc( '/', front.cvars.CppFile )) != EOF ) {
                    rc = fputc( '/', front.cvars.CppFile );
                }
            }
        }
        if( rc != EOF ) {
            rc = fputc( ch, front.cvars.CppFile );
        }
    } else {
        ++front.ccmain.CppColumn;
        rc = fputc( ch, front.cvars.CppFile );
    }
    if( rc == EOF ) {
        CloseFiles(front);       /* get rid of temp file */
        MyExit( front, 1 );        /* exit */
    }
}

void CppComment( FrontRun& front, int ch )
{
    if( front.cvars.CompFlags.cpp_keep_comments && CppPrinting(front) && front.cvars.CompFlags.cpp_output ) {
        if( ch != '\0' ) {
            if( front.ccmain.CppColumn + 2 >= front.ccmain.CppWidth )
                CppPutc( front, '\n' );
            CppPutc( front, '/' );
            CppPutc( front, ch );
        } else if( front.ccmain.CommentChar == '*' ) {
            if( front.ccmain.CppColumn + 2 >= front.ccmain.CppWidth )
                CppPutc( front, '\n' );
            CppPutc( front, '*' );
            CppPutc( front, '/' );
//        } else if( CommentChar == '/' ) {
//            CppPutc( '\n' );
        }
        front.ccmain.CommentChar = ch;
    }
}

void CppPuts( FrontRun& front, const char *s )
{
    while( *s != '\0' ) {
        CppPutc( front, *s++ );
    }
}

void CppPutsQuoted( FrontRun& front, const char *s )
{
    CppPutc( front, '"' );
    while( *s != '\0' ) {
        CppPutc( front, *s++ );
    }
    CppPutc( front, '"' );
}

static void OpenCppFile( FrontRun& front )
{
    char  *name = NULL;

    if( front.cvars.CompFlags.cpp_output_to_file ) {
        name = CppFileName(front);
        front.cvars.CppFile = front.held->fopen( name, "w" );
    } else {
        front.cvars.CppFile = stdout;
    }
    front.ccmain.CppColumn = 0;
    front.ccmain.CppFirstChar = true;
    if( front.cvars.CppFile == NULL ) {
        CantOpenFile( front, name );
        MyExit( front, 1 );
    } else {
        if( front.ccmain.CppWidth == 0 ) {
            front.ccmain.CppWidth = ~0U;
        }
        if( front.cvars.CppFile != stdout ) {     /* (stdout outlives this run: it must not keep a buffer of the run's memory) */
            setvbuf( front.cvars.CppFile, static_cast<char *>(CPermAlloc( front, 4096 )), _IOFBF, 4096 );
        }
    }
}

char *GetSourceDepName( FrontRun& front )
{
    pgroup2     pg;

    _splitpath2( front.cvars.WholeFName, pg.buffer, NULL, NULL, NULL, &pg.ext );
    return( createFileName( front, front.cvars.SrcDepName, pg.ext, false ) );
}


char *ObjFileName( FrontRun& front )
{
    return( createFileName( front, front.cvars.ObjectFileName, OBJ_EXT, false ) );
}

char *CppFileName( FrontRun& front )
{
    return( createFileName( front, front.cvars.ObjectFileName, CPP_EXT, false ) );
}

char *DepFileName( FrontRun& front )
{
    return( createFileName( front, front.cvars.DependFileName, DEP_EXT, false ) );
}

char *ErrFileName( FrontRun& front )
{
    if( front.cvars.ErrorFileName == NULL )
        return( NULL );
    return( createFileName( front, front.cvars.ErrorFileName, ERR_EXT, false ) );
}

void CppPrtChar( FrontRun& front, int c )
{
    if( CppPrinting(front) && front.cvars.CompFlags.cpp_output ) {
        CppPutc( front, c );
    }
}

void OpenDefFile( FrontRun& front )
{
    pgroup2     pg;
    char        name[_MAX_PATH];

    if( front.cvars.DefFName == NULL ) {
        _splitpath2( front.cvars.SrcFName, pg.buffer, NULL, NULL, &pg.fname, NULL );
        _makepath( name, NULL, NULL, pg.fname, DEF_EXT );
        front.cvars.DefFile = front.held->fopen( name, "w" );
    } else {
        front.cvars.DefFile = front.held->fopen( front.cvars.DefFName, "w" );
    }
    if( front.cvars.DefFile != NULL ) {
        setvbuf( front.cvars.DefFile, static_cast<char *>(CPermAlloc( front, 1024 )), _IOFBF, 1024 );
    }
}

FILE *OpenBrowseFile( FrontRun& front )
{
    pgroup2     pg;
    char        name[_MAX_PATH];
    FILE        *mbr_file;

    if( front.cvars.CompFlags.cpp_output_to_file ) {
        strcpy( name, createFileName( front, front.cvars.ObjectFileName, MBR_EXT, true ) );
    } else {
        _splitpath2( front.cvars.SrcFName, pg.buffer, NULL, NULL, &pg.fname, NULL );
        _makepath( name, NULL, NULL, pg.fname, MBR_EXT );
    }
    mbr_file = front.held->fopen( name, "wb" );
    if( mbr_file == NULL ) {
        CantOpenFile( front, name );
    }
    return( mbr_file );
}

void CloseSrcFile( FrontRun& front, FCB *fcb )
{
    if( fcb->fp != NULL && fcb->fp != stdin )
        front.held->fclose( fcb->fp );
    CloseFCB( front, fcb );
}

static bool FreeSrcFP( FrontRun& front )
/***************************/
{
    FCB     *fcb;
    FCB     *nextfcb;
    bool    ret;

    ret = false;
    for( fcb = front.cvars.SrcFiles; fcb != NULL; fcb = nextfcb ) {
        nextfcb = fcb->prev_file;
        if( nextfcb == NULL || nextfcb->fp == NULL ) {
            break;
        }
    }
    if( fcb != NULL && fcb->fp != NULL ) {
        fcb->rseekpos = ftell( fcb->fp );
        if( fcb->fp != NULL && fcb->fp != stdin )
            front.held->fclose( fcb->fp );
        fcb->fp = NULL;
        ret = true;
    }
    return( ret );
}


static FNAMEPTR FindFlist( FrontRun& front, char const *filename )
/************************************************
 * find a flist
 */
{
    FNAMEPTR    flist;

    for( flist = front.cvars.FNames; flist != NULL; flist = flist->next ) {
        if( FNAMECMPSTR( filename, flist->name ) == 0 ) {
            break;
        }
    }
    return( flist );
}

static bool IsFNameOnce( FrontRun& front, char const *filename )
{
    FNAMEPTR    flist;
    char        fullfilename[_MAX_PATH];
    char        *fullpath;

    fullpath = SrcFullPath( filename, fullfilename, sizeof( fullfilename ) );
    if( fullpath == NULL )
        return( false );
    for( flist = front.cvars.FNames; flist != NULL; flist = flist->next ) {
        if( flist->fullpath != NULL ) {
            if( FNAMECMPSTR( fullpath, flist->fullpath ) == 0 ) {
                break;
            }
        }
    }
    if( flist == NULL )
        return( false );
    return( flist->once );
}

static bool TryOpen( FrontRun& front, const char *path, pgroup2 *ff, src_file_type typ )
{
    FILE        *fp;
    char        filename[_MAX_PATH];
    char        *p;
    pgroup2     fd;

    _splitpath2( path, fd.buffer, &fd.drive, &fd.dir, NULL, NULL );
    if( ff->drive[0] != '\0' && fd.drive[0] != '\0' ) {
        return( false );
    }
    /*
     * concatenate fd.dir + sep + fp.dir
     */
    p = fd.dir + strlen( fd.dir );
    if( fd.dir[0] != '\0' ) {
        if( !IS_PATH_SEP( p[-1] ) ) {
            *p++ = DIR_SEP;
        }
    }
    strcpy( p, ff->dir );
    _makepath( filename, ( fd.drive[0] == '\0' ) ? ff->drive : fd.drive, fd.dir, ff->fname, ff->ext );
    if( IsFNameOnce( front, filename ) ) {
        return( true );
    }
    for( ; (fp = front.held->fopen( filename, "rb" )) == NULL; ) {
        if( errno != ENOMEM && errno != ENFILE && errno != EMFILE )
            break;
        if( !FreeSrcFP(front) ) {        // try closing an include file
            break;
        }
    }
    if( fp == NULL )
        return( false );
    if( OpenFCB( front, fp, filename, typ ) ) {
        if( front.cvars.CompFlags.cpp_mode ) {
            if( front.cvars.CppFile == NULL )
                OpenCppFile(front);
            CppEmitPoundLine( front, 1, filename, true );
            front.ccmain.CppFirstChar = true;
        }
        return( true );
    }
    front.held->fclose( fp );
    return( false );
}

FNAMEPTR AddFlist( FrontRun& front, char const *filename )
{
    FNAMEPTR    flist;
    FNAMEPTR    *lnk;
    unsigned    index;
    size_t      len1;
    size_t      len2;
    struct stat statbuf;

    index = 0;
    for( lnk = &front.cvars.FNames; (flist = *lnk) != NULL; lnk = &flist->next ) {
        if( FNAMECMPSTR( filename, flist->name ) == 0 )
            break;
        index++;
    }
    if( flist == NULL ) {
        len1 = strlen( filename ) + 1;
        if( HAS_PATH( filename ) || front.cvars.DependHeaderPath == NULL ) {
            len2 = 0;
            flist = (FNAMEPTR)CMemAlloc( front, offsetof( fname_list, name ) + len1 );
        } else {
            len2 = strlen( front.cvars.DependHeaderPath );
            flist = (FNAMEPTR)CMemAlloc( front, offsetof( fname_list, name ) + len2 + len1 );
            strcpy( flist->name, front.cvars.DependHeaderPath );
        }
        strcpy( flist->name + len2, filename );
        *lnk = flist;
        flist->next = NULL;
        flist->index = index;
        flist->index_db = DBFILE_INVALID;
        flist->rwflag = true;
        flist->once = false;
        flist->fullpath = NULL;
        flist->mtime = 0;
        if( stat( filename, &statbuf ) == 0 ) {
            flist->mtime = statbuf.st_mtime;
        }
    }
    return( flist );
}

FNAMEPTR FileIndexToFName( FrontRun& front, unsigned file_index )
{
    FNAMEPTR    flist;

    for( flist = front.cvars.FNames; flist != NULL; flist = flist->next ) {
        if( flist->index == file_index ) {
            break;
        }
    }
    return( flist );
}

char *FNameFullPath( FrontRun& front, FNAMEPTR flist )
{
    char   fullbuff[2 * _MAX_PATH];
    char   *fullpath;

    if( flist->fullpath == NULL ) {
        fullpath = SrcFullPath( flist->name, fullbuff, sizeof( fullbuff ) );
        if( fullpath != NULL ) {
            fullpath = CMemStrdup( front, fullpath );
            flist->fullpath = fullpath;
        } else {
            fullpath = flist->name;
        }
    } else {
        fullpath = flist->fullpath;
    }
    return( fullpath );
}

char *FileIndexToCorrectName( FrontRun& front, unsigned file_index )
{
    FNAMEPTR    flist;
    char        *name;

    if( NULL == (flist = FileIndexToFName( front, file_index )) )
        return( NULL );
    if( front.cvars.CompFlags.ef_switch_used ) {
        name = FNameFullPath( front, flist );
    } else {
        name = flist->name;
    }
    return( name );
}

void FreeFNames( FrontRun& front )
{
    FNAMEPTR    flist;

    while( (flist = front.cvars.FNames) != NULL ) {
        front.cvars.FNames = flist->next;
        if( flist->fullpath != NULL ) {
            CMemFree( front, flist->fullpath );
        }
        CMemFree( front, flist );
    }
}

void AddIncFile( FrontRun& front, INCFILE *ifile )
{
    INCFILE     *ifilep;

    ifile->nextfile = NULL;
    if( front.cvars.IncFileList == NULL ) {
        front.cvars.IncFileList = ifile;
    } else {
        ifilep = front.cvars.IncFileList;
        while( ifilep->nextfile != NULL )
            ifilep = ifilep->nextfile;
        ifilep->nextfile = ifile;
    }
}

void AddIncFileList( FrontRun& front, const char *filename )
{
    INCFILE     *ifile;

    ifile = (INCFILE *)CMemAlloc( front, offsetof( INCFILE, filename ) + strlen( filename ) + 1 );
    strcpy( ifile->filename, filename );
    AddIncFile( front, ifile );
}

void FreeIncFileList( FrontRun& front )
{
    INCFILE *ilist;

    while( (ilist = front.cvars.IncFileList) != NULL ) {
        front.cvars.IncFileList = ilist->nextfile;
        CMemFree( front, ilist );
    }
}

static RDIRPTR AddRDir( FrontRun& front, const char *path )
{
    RDIRPTR     dirlist;
    RDIRPTR     *lnk;

    for( lnk = &front.cvars.RDirNames; (dirlist = *lnk) != NULL; lnk = &dirlist->next ) {
        if( stricmp( path, dirlist->name ) == 0 ) {
            break;
        }
    }
    if( dirlist == NULL ) {
        dirlist = (RDIRPTR)CMemAlloc( front, offsetof( rdir_list, name ) + strlen( path ) + 1 );
        dirlist->next = NULL;
        strcpy( dirlist->name, path );
        *lnk = dirlist;
    }
    return( dirlist );
}

void FreeRDir( FrontRun& front )
{
    RDIRPTR    dirlist;

    while( (dirlist = front.cvars.RDirNames) != NULL ) {
        front.cvars.RDirNames = dirlist->next;
        CMemFree( front, dirlist );
    }
}

void SrcFileReadOnlyDir( FrontRun& front, char const *dirs )
/******************************************
 * add dir to ro set
 */
{
    char    *full;              // - full path
    char    path[_MAX_PATH];    // - used to extract directory
    char    buff[_MAX_PATH];    // - expanded path for directory

    while( *dirs != '\0' ) {
        char *p = path;
        dirs = GetPathElement( dirs, NULL, &p );
        *p = '\0';
        full = SrcFullPath( path, buff, sizeof( buff ) );
        AddRDir( front, full );
    }
}

bool SrcFileInRDir( FrontRun& front, FNAMEPTR flist )
{
    RDIRPTR     dirlist;
    bool        read_only;      // - true ==> file is in read-only directory
    char        *fullpath;      // - full path

    read_only = false;
    fullpath = FNameFullPath( front, flist );
    for( dirlist = front.cvars.RDirNames; dirlist != NULL; dirlist = dirlist->next ) {
        if( strnicmp( dirlist->name, fullpath, strlen( dirlist->name ) ) == 0 ) {
            read_only = true;
            break;
        }
    }
    return( read_only );
}

void SrcFileReadOnlyFile( FrontRun& front, char const *file )
{
    FNAMEPTR    flist;

    if( file == NULL ) {
        flist = front.cvars.SrcFiles->src_flist;
    } else {
        flist= FindFlist( front, file );
    }
    if( flist  != NULL ) {
        flist->rwflag = false;
    }
}

void SrcFileIncludeAlias( FrontRun& front, const char *alias_name, const char *real_name, bool is_lib )
{
    AddIAlias( front, alias_name, real_name, is_lib );
}

void SetSrcFNameOnce( FrontRun& front )
{
    front.cvars.SrcFiles->src_flist->once = true;
}

static void ParseInit( FrontRun& front )
{
    const char  *force;

    ScanInit(front);
    CTypeInit(front);
    MacroInit(front);
    SymInit(front);
    SpcSymInit(front);
    StringInit(front);
    InitDataQuads(front);
    ExprInit(front);
    StmtInit(front);
    SegInit(front);
    force = FEGetEnv( front, "FORCE" );
    if( force != NULL ) {
        front.cvars.ForceInclude = CMemStrdup( front, force );
    }
    front.cvars.ForcePreInclude = CMemStrdup( front, "_preincl.h" );
}

static bool OpenPgmFile( FrontRun& front )
{
    if( front.ccmain.IsStdIn ) {
        if( OpenFCB( front, stdin, STDIN_NAME, FT_SRC ) ) {
            if( front.cvars.CompFlags.cpp_mode ) {
                if( front.cvars.CppFile == NULL )
                    OpenCppFile(front);
                CppEmitPoundLine( front, 1, STDIN_NAME, true );
                front.ccmain.CppFirstChar = true;
            }
            front.cvars.MainSrcFile = front.cvars.SrcFiles;
            return( true );
        }
        return( false );
    }
    if( front.ccmain.IsNullDev ) {
        if( OpenFCB( front, NULL, NULLDEV_NAME, FT_SRC ) ) {
            if( front.cvars.CompFlags.cpp_mode ) {
                if( front.cvars.CppFile == NULL )
                    OpenCppFile(front);
                CppEmitPoundLine( front, 1, NULLDEV_NAME, true );
                front.ccmain.CppFirstChar = true;
            }
            front.cvars.MainSrcFile = front.cvars.SrcFiles;
            return( true );
        }
        return( false );
    }
    if( !OpenSrcFile( front, front.cvars.WholeFName, FT_SRC ) ) {
        CantOpenFile( front, front.cvars.WholeFName );
        CSuicide(front);
        return( false );
    }
    front.cvars.MainSrcFile = front.cvars.SrcFiles;
    return( true );
}


static void Parse( FrontRun& front )
{
    EmitInit(front);
    /*
     * The first token in a file should be #include if a user wants to
     * use pre-compiled headers. The following call to NextToken() to
     * get the very first token of the file will load the pre-compiled
     * header if the user requested such and it is a #include directive.
     */
    if( front.cvars.ForceInclude != NULL ) {
        if( !OpenSrcFile( front, front.cvars.ForceInclude, FT_HEADER_FORCED ) ) {
            PrtfFilenameErr( front, front.cvars.ForceInclude, FT_HEADER_FORCED, true );
        }
    }
    if( front.cvars.ForcePreInclude != NULL ) {
        openForcePreInclude(front);
    }
    front.cvars.CompFlags.ok_to_use_precompiled_hdr = true;
    NextToken(front);
    /*
     * If we didn't get a #include with the above call to NextToken()
     * it's too late to use pre-compiled header now.
     */
    front.cvars.CompFlags.ok_to_use_precompiled_hdr = false;
    ParsePgm(front);
    if( front.cvars.DefFile != NULL ) {
        front.held->fclose( front.cvars.DefFile );
        front.cvars.DefFile = NULL;
    }
    CheckCallParms(front);
    EndBlock(front);     /* end of block 0 */
    MacroFini(front);
}

static void CPP_Parse( FrontRun& front )
{
    if( front.cvars.ForceInclude != NULL ) {
        CppPrtChar( front, '\n' );
        if( !OpenSrcFile( front, front.cvars.ForceInclude, FT_HEADER_FORCED ) ) {
            PrtfFilenameErr( front, front.cvars.ForceInclude, FT_HEADER_FORCED, true );
        }
    }
    front.cvars.CurToken = T_NULL;
    while( front.cvars.CurToken != T_EOF ) {
        front.cvars.CurToken = GetNextToken(front);
        CppPrtToken( front, front.cvars.CurToken );
    }
    MacroFini(front);
}

static void DoCCompile( FrontRun& front, char **cmdline )
/**************************************/
{
    jmp_buf     env;

    front.cvars.Environment = JMPBUF_PTR( env );
    if( setjmp( env ) ) {           /* if fatal error has occurred */
        EmitAbort();                /* abort code generator */
        CPragmaFini(front);
        CloseFiles(front);
        FreeFNames(front);
        FreeRDir(front);
        FreeIAlias(front);
        front.cvars.ErrCount = 1;
        MyExit( front, 1 );
    }
    ParseInit(front);
    if( ParseCmdLine( front, cmdline ) ) {
        if( front.cvars.CompFlags.cpp_dump_macros ) {
            if( front.cvars.CppFile == NULL )
                OpenCppFile(front);
            DumpAllMacros(front);
            MacroFini(front);
        } else if( front.cvars.WholeFName == NULL ) {
            CErr1( front, ERR_FILENAME_REQUIRED );
            return;
        } else {
            MakePgmName(front);
            DelErrFile(front);               /* delete old error file */
            CPragmaInit(front);              /* memory model is known now */
            if( front.cvars.CompFlags.cpp_mode ) {
                front.cvars.PrintWhiteSpace = true;
                if( front.cvars.ForcePreInclude != NULL ) {
                    if( front.cvars.CppFile == NULL )
                        OpenCppFile(front);
                    front.cvars.CompFlags.cpp_output = false;
                    if( openForcePreInclude(front) ) {
                        front.cvars.CurToken = T_NULL;
                        while( front.cvars.CurToken != T_EOF ) {
                            front.cvars.CurToken = GetNextToken(front);
                        }
                    }
                    front.cvars.CompFlags.cpp_output = true;
                }
                OpenPgmFile(front);
                CPP_Parse(front);
                if( !front.cvars.CompFlags.quiet_mode ) {
                    PrintStats(front);
                }
                if( front.cvars.CompFlags.warnings_cause_bad_exit ) {
                    front.cvars.ErrCount += front.cvars.WngCount;
                }
            } else {
                OpenPgmFile(front);
                MacroAddComp(front); // Add any compile time only macros
                Parse(front);
                if( !front.cvars.CompFlags.quiet_mode ) {
                    PrintStats(front);
                }
                if( front.cvars.CompFlags.warnings_cause_bad_exit ) {
                    front.cvars.ErrCount += front.cvars.WngCount;
                }
                if( ( front.cvars.ErrCount == 0 ) && ( !front.cvars.CompFlags.check_syntax ) ) {
                    if( front.cvars.CompFlags.emit_browser_info ) {
                        DwarfBrowseEmit(front);
                    }
                    FreeMacroSegments(front);
                    DoCompile(front);
                } else {
                    FreeMacroSegments(front);
                }
            }
            if( front.cvars.ErrCount == 0 ) {
                DumpDepFile(front);
            } else {
                DelDepFile(front);
            }
            SymFini(front);
            CPragmaFini(front);
        }
    } else {
        front.cvars.ErrCount = 1;
    }
    CloseFiles(front);
    FreeFNames(front);
    FreeRDir(front);
    FreeIAlias(front);
    FreeIncFileList(front);
}

static bool try_open_file( FrontRun& front, const char *path, pgroup2 *fp, pgroup2 *fa, src_file_type typ )
{
    bool    ok;
    char    save_chr_fname;
    char    save_chr_ext;

    /*
     * try to open regular name
     */
    ok = TryOpen( front, path, fp, typ );
    if( ok ) {
        return( ok );
    }
    if( fa != NULL ) {
        /*
         * try to open alias name if defined
         */
        ok = TryOpen( front, path, fa, typ );
        if( ok ) {
            return( ok );
        }
    }
    if( front.cvars.CompFlags.check_truncated_fnames ) {
        save_chr_fname = '\0';
        if( strlen( fp->fname ) > 8 ) {
            save_chr_fname = fp->fname[8];
            fp->fname[8] = '\0';
        }
        save_chr_ext = '\0';
        if( strlen( fp->ext ) > 4 ) {
            save_chr_ext = fp->ext[4];
            fp->ext[4] = '\0';
        }
        if( save_chr_fname != '\0' || save_chr_ext != '\0' ) {
            /*
             * try to open truncated name if enabled
             */
            ok = TryOpen( front, path, fp, typ );
            if( !ok ) {
                if( save_chr_fname != '\0' ) {
                    fp->fname[8] = save_chr_fname;
                }
                if( save_chr_ext != '\0' ) {
                    fp->ext[4] = save_chr_ext;
                }
            }
        }
    }
    return( ok );
}

static bool doOpenSrcFile( FrontRun& front, pgroup2 *fp, pgroup2 *fa, src_file_type typ )
{
    char        *s;
    char        *p;
    char        try_[_MAX_PATH];
    FCB         *fcb;
    char        c;
    pgroup2     fd;

    if( typ == FT_SRC ) {
        if( try_open_file( front, "", fp, NULL, typ ) )
            return( true );
        if( !front.cvars.CompFlags.ignore_default_dirs ) {
            if( try_open_file( front, C_PATH, fp, NULL, typ ) ) {
                return( true );
            }
        }
        return( false );
    }
    if( fp->drive[0] != '\0' || IS_DIR_SEP( fp->dir[0] ) ) {
        /*
         * try absolute path
         * if drive letter given or path from root given
         */
        if( try_open_file( front, "", fp, fa, typ ) ) {
            return( true );
        }
    } else {
        switch( typ ) {
        case FT_HEADER_PRE:
        case FT_LIBRARY:
            break;
        case FT_HEADER:
        case FT_HEADER_FORCED:
            if( front.cvars.CompFlags.ignore_default_dirs ) {
                try_[0] = '\0';
                /*
                 * physical file name must be used, not logical
                 */
                _splitpath2( front.cvars.SrcFiles->src_flist->name, fd.buffer, &fd.drive, &fd.dir, NULL, NULL );
                _makepath( try_, fd.drive, fd.dir, NULL, NULL );
                if( try_open_file( front, try_, fp, fa, typ ) ) {
                    return( true );
                }
            } else {
                /*
                 * try current directory
                 */
                if( !front.cvars.GlobalCompFlags.ignore_current_dir ) {
                    if( try_open_file( front, "", fp, fa, typ ) ) {
                        return( true );
                    }
                }
                for( fcb = front.cvars.SrcFiles; fcb!= NULL; fcb = fcb->prev_file ) {
                    /*
                     * physical file name must be used, not logical
                     */
                    _splitpath2( fcb->src_flist->name, fd.buffer, &fd.drive, &fd.dir, NULL, NULL );
                    _makepath( try_, fd.drive, fd.dir, NULL, NULL );
                    if( try_open_file( front, try_, fp, fa, typ ) ) {
                        return( true );
                    }
                }
            }
            break;
        }
        s = front.cvars.IncPathList;
        while( (c = *s) != '\0' ) {
            p = fd.buffer;
            do {
                ++s;
                if( IS_PATH_LIST_SEP( c ) )
                    break;
                *p++ = c;
            } while( (c = *s) != '\0' );
            if( p != fd.buffer ) {
                c = p[-1];
                if( !IS_PATH_SEP( c ) ) {
                    *p++ = DIR_SEP;
                }
            }
            *p = '\0';
            if( try_open_file( front, fd.buffer, fp, fa, typ ) ) {
                return( true );
            }
        }
        switch( typ ) {
        case FT_HEADER_PRE:
        case FT_LIBRARY:
            break;
        case FT_HEADER:
        case FT_HEADER_FORCED:
            if( !front.cvars.CompFlags.ignore_default_dirs ) {
                /*
                 * try current ../h directory
                 */
                if( try_open_file( front, H_PATH, fp, fa, typ ) ) {
                    return( true );
                }
            }
            break;
        }
    }
    return( false );
}

static void normalizeSep( char *dir )
{
    char    c;

    while( (c = *dir) != '\0' ) {
        if( c == '/' )
            *dir = '\\';
        dir++;
    }
}

bool OpenSrcFile( FrontRun& front, const char *filename, src_file_type typ )
{
    const char  *alias_filename;
    pgroup2     fp;
    pgroup2     fa;
    pgroup2     *fap;

    _splitpath2( filename, fp.buffer, &fp.drive, &fp.dir, &fp.fname, &fp.ext );
    normalizeSep( fp.dir );
    fap = NULL;
    switch( typ ) {
    case FT_SRC:
        break;
    case FT_HEADER:
    case FT_HEADER_FORCED:
    case FT_HEADER_PRE:
    case FT_LIBRARY:
        /*
         * See if there's an alias for this filename
         */
        alias_filename = IncludeAlias( front, filename, ( typ == FT_LIBRARY || typ == FT_HEADER_PRE ) );
        if( alias_filename != NULL ) {
            _splitpath2( alias_filename, fa.buffer, &fa.drive, &fa.dir, &fa.fname, &fa.ext );
            normalizeSep( fa.dir );
            fap = &fa;
        }
        break;
    }
    return( doOpenSrcFile( front, &fp, fap, typ ) );
}


void CppEmitPoundLine( FrontRun& front, unsigned line_num, const char *filename, bool newline )
{
    char    buf[30];
    char    c;

    if( front.cvars.CompFlags.cpp_line_wanted && CppPrinting(front) && front.cvars.CompFlags.cpp_output ) {
        sprintf( buf, "#line %u \"", line_num );
        CppPuts( front, buf );
        while( (c = *filename++) != '\0' ) {
            if( c == '\\' )
                c = '/';
            CppPutc( front, c );
        }
        CppPutc( front, '"' );
        if( newline ) {
            CppPutc( front, '\n' );
        }
    }
}

bool CppPrinting( FrontRun& front )
{
    return( front.cvars.SkipLevel == front.cvars.NestLevel );
}

void CppPrtToken( FrontRun& front, TOKEN token )
{
    if( CppPrinting(front) && front.cvars.CompFlags.cpp_output ) {
        switch( token ) {
        case T_BAD_CHAR:
        case T_BAD_TOKEN:
        case T_ID:
        case T_CONSTANT:
            CppPuts( front, front.cvars.Buffer );
            break;
        case T_STRING:
            if( front.cvars.CompFlags.wide_char_string )
                CppPutc( front, 'L' );
            CppPutsQuoted( front, front.cvars.Buffer );
            break;
        case T_EOF:
        case T_NULL:
            break;
        case T_WHITE_SPACE:
            if( front.cvars.PrintWhiteSpace || front.cvars.CompFlags.in_pragma ) {
                CppPuts( front, TokenString[token] );
            } else {
                front.cvars.PrintWhiteSpace = true; //Toggle
            }
            break;
        default:
            CppPuts( front, TokenString[token] );
        }
    }
}


TOKEN GetNextToken( FrontRun& front )
{
    TOKEN   token;

    if( front.cvars.MacroPtr != NULL ) {
        token = GetMacroToken(front);
    } else {
        while( front.cvars.CurrChar != LCHR_EOF ) {
            if( (front.scan.CharSet[front.cvars.CurrChar] & C_WS) == 0 )
                break;
            if( front.cvars.CurrChar != '\r' )
                CppPrtChar( front, front.cvars.CurrChar );
            front.cvars.NextChar(front);
        }
        token = ScanToken(front);
    }
    return( token );
}

void CloseFiles( FrontRun& front )
{
    if( front.cvars.CppFile != NULL ) {
        fflush( front.cvars.CppFile );
        if( ferror( front.cvars.CppFile ) ) {
            char    msgtxt[80];
            char    msgbuf[MAX_MSG_LEN];

            /*
             * issue message
             */
            CGetMsg( front, msgtxt, ERR_FATAL_ERROR );
            sprintf( msgbuf, msgtxt, strerror( errno ) );
            NoteMsg( front, msgbuf );
        }
        if( front.cvars.CppFile != stdout ) {     /* (stdout is the program's, and stays open after this run) */
            front.held->fclose( front.cvars.CppFile );
        }
        front.cvars.CppFile = NULL;
    }
    if( front.cvars.ErrFile != NULL ) {
        front.held->fclose( front.cvars.ErrFile );
        front.cvars.ErrFile = NULL;
        if( ! front.cvars.CompFlags.errfile_written ) {
            DelErrFile(front);
        }
    }
    if( front.cvars.DefFile != NULL ) {
        front.held->fclose( front.cvars.DefFile );
        front.cvars.DefFile = NULL;
    }
}


bool FrontEnd( FrontRun& front, char **cmdline )
{

    InitGlobalVars(front);
    CMemInit(front);
    InitMsg(front);
    InitPurge();
    initGlobals(front);
    DoCCompile( front, cmdline );
    finiGlobals(front);
    PurgeMemory(front);
    FiniMsg(front);
    CMemFini(front);
    front.cvars.GlobalCompFlags.cc_first_use = false;
    return( front.cvars.ErrCount != 0 );
}
