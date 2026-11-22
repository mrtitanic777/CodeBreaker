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


#if defined(__UNIX__)
 #define C_PATH         "../c/"
 #define H_PATH         "../h/"
 #define OBJ_EXT        "o"
#else
 #define C_PATH         "..\\c\\"
 #define H_PATH         "..\\h\\"
 #define OBJ_EXT        "obj"
#endif
#define DEF_EXT         "def"
#define ERR_EXT         "err"
#define MBR_EXT         "mbr"
#define C_EXT           "c"
#define CPP_EXT         "i"
#define DEP_EXT         "d"

#ifdef __UNIX__
#define NULLDEV         "/dev/null"
#else
#define NULLDEV         "NUL"
#endif

#define STDIN_NAME      "stdin"
#define NULLDEV_NAME    "nulldev"


void FrontEndInit( bool reuse )
/******************************
 * Do the once only things
 ******************************/
{
    cvars_state().GlobalCompFlags.cc_reuse = reuse;
    cvars_state().GlobalCompFlags.cc_first_use = true;
    cvars_state().GlobalCompFlags.ignore_environment = false;
    cvars_state().GlobalCompFlags.ignore_current_dir = false;
    cvars_state().GlobalCompFlags.ide_cmd_line_has_files = false;
    cvars_state().GlobalCompFlags.ide_console_output = false;
    cvars_state().GlobalCompFlags.progress_messages = false;
//    GlobalCompFlags.dll_active = false;
}

void FrontEndFini( void )
/************************
 * Fini the once only
 ************************/
{
    cvars_state().GlobalCompFlags.cc_reuse = false;
    cvars_state().GlobalCompFlags.cc_first_use = true;
}

namespace {

/* the statics of this file, one set for each run of the compiler */
struct CcmainState {
    bool IsStdIn;
    bool IsNullDev;
    char *FNameBuf = NULL;
    unsigned CppColumn = 0;
    unsigned CppWidth = 0;
    unsigned CommentChar = '\0';
    bool CppFirstChar = false;
};

}  // namespace

static inline CcmainState& ccmain_state() { return cbrun_part<CcmainState>(); }

static void initGlobals( void )
{
    InitStats();
    ccmain_state().IsStdIn = false;
    ccmain_state().IsNullDev = false;
    cvars_state().FNames = NULL;
    cvars_state().RDirNames = NULL;
    cvars_state().IAliasNames = NULL;
    cvars_state().SrcFiles = NULL;
    cvars_state().ErrFile = NULL;
    cvars_state().DefFile = NULL;
    cvars_state().CppFile = NULL;
    cvars_state().DepFile = NULL;
    cvars_state().IncPathList = static_cast<char *>(CMemAlloc( 1 ));
    cvars_state().SegmentNum = FIRST_PRIVATE_SEGMENT;
    InitBuffer( BUF_SIZE );
    InitTokenBuf( BUF_SIZE );
    ccmain_state().FNameBuf = static_cast<char *>(CMemAlloc( _MAX_PATH ));
    InitIncFile();
    InitErrLoc();
}

static void finiGlobals( void )
{
    FiniBuffer();
}

static bool ParseCmdLine( char **cmdline )
{
    char        *cmd;

    cmd = const_cast<char *>((cmdline == NULL) ?  "" : cmdline[0]);  // from cpp.c
    if( cmd == NULL ) { // argv == 1
        cmd = const_cast<char *>("");
    }
    while( *cmd == ' ' )
        ++cmd;
    if( *cmd == '?' || *cmd == '\0' ) {
        CUsage();
        return( false );
    }
    GenCOptions( cmdline );
    return( true );
}

static void OpenDepFile( void )
{
    char        *name;

    if( cvars_state().CompFlags.generate_auto_depend ) {
        name = DepFileName();
        if( name != NULL ) {
            cvars_state().DepFile = fopen( name, "w" );
            if( cvars_state().DepFile != NULL ) {
                setvbuf( cvars_state().DepFile, static_cast<char *>(CPermAlloc( 1024 )), _IOFBF, 1024 );
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

FNAMEPTR NextDependency( FNAMEPTR curr )
{
    if( curr == NULL ) {
        curr = cvars_state().FNames;
    } else {
        curr = curr->next;
    }
    for( ; curr != NULL; curr = curr->next ) {
        if( curr->rwflag && !SrcFileInRDir( curr ) ) {
            break;
        }
    }
    return( curr );
}


static void DelErrFile(void)
{
    char        *name;

    name = ErrFileName();
    if( name != NULL ) {
        remove( name );
    }
}

static void DelDepFile( void )
{
    char        *name;

    name = DepFileName();
    if( name != NULL ) {
        remove( name );
    }
}

static char *createFileName( const char *template_, const char *ext, bool forceext )
{
    pgroup2     pg;
    bool        use_defaults;
    const char  *module_name;

    use_defaults = ( template_ == NULL );
    if( use_defaults )
        template_ = cvars_state().WholeFName;
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
        module_name = FEModuleName();
    }
    _makepath( ccmain_state().FNameBuf, pg.drive, pg.dir, module_name, ext );
    return( ccmain_state().FNameBuf );
}

static void DumpDepFile( void )
{
    FNAMEPTR    curr;

    OpenDepFile();
    if( cvars_state().DepFile != NULL ) {
        curr = NextDependency( NULL );
        if( curr != NULL ) {
            fprintf( cvars_state().DepFile, "%s :", ForceSlash( createFileName( cvars_state().DependTarget, OBJ_EXT, false ), cvars_state().DependForceSlash ) );
            fprintf( cvars_state().DepFile, " %s", ForceSlash( GetSourceDepName(), cvars_state().DependForceSlash ) );
            for( curr = NextDependency( curr ); curr != NULL; curr = NextDependency( curr ) ) {
                fprintf( cvars_state().DepFile, " %s", ForceSlash( curr->name, cvars_state().DependForceSlash ) );
            }
            fprintf( cvars_state().DepFile, "\n" );
        }
        fclose( cvars_state().DepFile );
        cvars_state().DepFile = NULL;
    } else {
        DelDepFile();
    }
}

static IALIASPTR AddIAlias( const char *alias_name, const char *real_name, bool is_lib )
{
    size_t      alias_len;
    size_t      real_len;
    IALIASPTR   alias, old_alias;
    IALIASPTR   *lnk;

    for( lnk = &cvars_state().IAliasNames; (old_alias = *lnk) != NULL; lnk = &old_alias->next ) {
        if( ( old_alias->is_lib == is_lib ) && !FNAMECMPSTR( alias_name, old_alias->alias_name ) ) {
            break;
        }
    }
    alias_len = strlen( alias_name ) + 1;
    real_len = strlen( real_name ) + 1;
    alias = static_cast<IALIASPTR>(CMemAlloc( offsetof( ialias_list, alias_name ) + alias_len + real_len ));
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
        CMemFree( old_alias );
    }

    return( alias );
}

static void FreeIAlias( void )
{
    IALIASPTR   aliaslist;

    while( (aliaslist = cvars_state().IAliasNames) != NULL ) {
        cvars_state().IAliasNames = aliaslist->next;
        CMemFree( aliaslist );
    }
    cvars_state().IAliasNames = NULL;
}

static const char *IncludeAlias( const char *filename, bool is_lib )
{
    IALIASPTR   alias;

    for( alias = cvars_state().IAliasNames; alias != NULL; alias = alias->next ) {
        if( ( alias->is_lib == is_lib ) && !FNAMECMPSTR( filename, alias->alias_name ) ) {
            return( alias->real_name );
        }
    }
    return( filename );
}

void PrtfFilenameErr( const char *filename, src_file_type typ, bool print_error )
{
    /* unused parameters */ (void)typ;

    if( print_error ) {
        CErr2p( ERR_CANT_OPEN_FILE, filename );
    }
}

static bool openForcePreInclude( void )
{
    bool    ok;

    ok = OpenSrcFile( cvars_state().ForcePreInclude, FT_HEADER_PRE );
    if( !ok )
        PrtfFilenameErr( cvars_state().ForcePreInclude, FT_HEADER_PRE, false );
    return( ok );
}


static void MakePgmName( void )
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
    if( cvars_state().WholeFName[0] == '.' && cvars_state().WholeFName[1] == '\0' ) {
        ccmain_state().IsStdIn = true;
        ccmain_state().IsNullDev = false;
        CMemFree( cvars_state().WholeFName );
        cvars_state().WholeFName = static_cast<char *>(CMemAlloc( sizeof( STDIN_NAME ) ));
        strcpy( cvars_state().WholeFName, STDIN_NAME );
        pg.fname = cvars_state().WholeFName;
    } else if( FNAMECMPSTR( cvars_state().WholeFName, NULLDEV ) == 0 ) {
        ccmain_state().IsNullDev = true;
        ccmain_state().IsStdIn = false;
        CMemFree( cvars_state().WholeFName );
        cvars_state().WholeFName = static_cast<char *>(CMemAlloc( sizeof( NULLDEV_NAME ) ));
        strcpy( cvars_state().WholeFName, NULLDEV_NAME );
        pg.fname = cvars_state().WholeFName;
    } else {
        _splitpath2( cvars_state().WholeFName, pg.buffer, &pg.drive, &pg.dir, &pg.fname, &pg.ext );
        if( pg.ext[0] == '\0' ) { // no extension
            len = strlen( cvars_state().WholeFName );
            cvars_state().WholeFName = static_cast<char *>(CMemRealloc( cvars_state().WholeFName, len + 1 + ( sizeof( C_EXT ) - 1 ) + 1 ));
            strcat( cvars_state().WholeFName + len, "." C_EXT );
        }
    }
    cvars_state().SrcFName = CMemStrdup( pg.fname );
}

static void CantOpenFile( const char *name )
{
    char    msgtxt[80];
    char    msgbuf[MAX_MSG_LEN];

    CGetMsg( msgtxt, ERR_CANT_OPEN_FILE );
    sprintf( msgbuf, msgtxt, name );
    ConsErrMsgVerbatim( msgbuf );
}


void SetPPWidth( unsigned width )
{
    ccmain_state().CppWidth = width - 1;
}

static void CppPutc( int ch )
{
    int     rc;

    if( ccmain_state().CppFirstChar ) {
        ccmain_state().CppFirstChar = false;
        if( ch == '\n' ) {
            return;
        }
    }
    if( ch == '\n' ) {
        ccmain_state().CppColumn = 0;
        rc = fputc( '\n', cvars_state().CppFile );
    } else if( ccmain_state().CppColumn >= ccmain_state().CppWidth ) {
        ccmain_state().CppColumn = 1;
        if( ccmain_state().CommentChar == '\0' ) {
            if( (rc = fputc( '\\', cvars_state().CppFile )) != EOF ) {
                rc = fputc( '\n', cvars_state().CppFile );
            }
        } else if( (rc = fputc( '\n', cvars_state().CppFile )) != EOF ) {
            if( ccmain_state().CommentChar == '/' ) {
                ccmain_state().CppColumn += 2;
                if( (rc = fputc( '/', cvars_state().CppFile )) != EOF ) {
                    rc = fputc( '/', cvars_state().CppFile );
                }
            }
        }
        if( rc != EOF ) {
            rc = fputc( ch, cvars_state().CppFile );
        }
    } else {
        ++ccmain_state().CppColumn;
        rc = fputc( ch, cvars_state().CppFile );
    }
    if( rc == EOF ) {
        CloseFiles();       /* get rid of temp file */
        MyExit( 1 );        /* exit */
    }
}

void CppComment( int ch )
{
    if( cvars_state().CompFlags.cpp_keep_comments && CppPrinting() && cvars_state().CompFlags.cpp_output ) {
        if( ch != '\0' ) {
            if( ccmain_state().CppColumn + 2 >= ccmain_state().CppWidth )
                CppPutc( '\n' );
            CppPutc( '/' );
            CppPutc( ch );
        } else if( ccmain_state().CommentChar == '*' ) {
            if( ccmain_state().CppColumn + 2 >= ccmain_state().CppWidth )
                CppPutc( '\n' );
            CppPutc( '*' );
            CppPutc( '/' );
//        } else if( CommentChar == '/' ) {
//            CppPutc( '\n' );
        }
        ccmain_state().CommentChar = ch;
    }
}

void CppPuts( const char *s )
{
    while( *s != '\0' ) {
        CppPutc( *s++ );
    }
}

void CppPutsQuoted( const char *s )
{
    CppPutc( '"' );
    while( *s != '\0' ) {
        CppPutc( *s++ );
    }
    CppPutc( '"' );
}

static void OpenCppFile( void )
{
    char  *name = NULL;

    if( cvars_state().CompFlags.cpp_output_to_file ) {
        name = CppFileName();
        cvars_state().CppFile = fopen( name, "w" );
    } else {
        cvars_state().CppFile = stdout;
    }
    ccmain_state().CppColumn = 0;
    ccmain_state().CppFirstChar = true;
    if( cvars_state().CppFile == NULL ) {
        CantOpenFile( name );
        MyExit( 1 );
    } else {
        if( ccmain_state().CppWidth == 0 ) {
            ccmain_state().CppWidth = ~0U;
        }
        if( cvars_state().CppFile != stdout ) {     /* (stdout outlives this run: it must not keep a buffer of the run's memory) */
            setvbuf( cvars_state().CppFile, static_cast<char *>(CPermAlloc( 4096 )), _IOFBF, 4096 );
        }
    }
}

char *GetSourceDepName( void )
{
    pgroup2     pg;

    _splitpath2( cvars_state().WholeFName, pg.buffer, NULL, NULL, NULL, &pg.ext );
    return( createFileName( cvars_state().SrcDepName, pg.ext, false ) );
}


char *ObjFileName( void )
{
    return( createFileName( cvars_state().ObjectFileName, OBJ_EXT, false ) );
}

char *CppFileName( void )
{
    return( createFileName( cvars_state().ObjectFileName, CPP_EXT, false ) );
}

char *DepFileName( void )
{
    return( createFileName( cvars_state().DependFileName, DEP_EXT, false ) );
}

char *ErrFileName( void )
{
    if( cvars_state().ErrorFileName == NULL )
        return( NULL );
    return( createFileName( cvars_state().ErrorFileName, ERR_EXT, false ) );
}

void CppPrtChar( int c )
{
    if( CppPrinting() && cvars_state().CompFlags.cpp_output ) {
        CppPutc( c );
    }
}

void OpenDefFile( void )
{
    pgroup2     pg;
    char        name[_MAX_PATH];

    if( cvars_state().DefFName == NULL ) {
        _splitpath2( cvars_state().SrcFName, pg.buffer, NULL, NULL, &pg.fname, NULL );
        _makepath( name, NULL, NULL, pg.fname, DEF_EXT );
        cvars_state().DefFile = fopen( name, "w" );
    } else {
        cvars_state().DefFile = fopen( cvars_state().DefFName, "w" );
    }
    if( cvars_state().DefFile != NULL ) {
        setvbuf( cvars_state().DefFile, static_cast<char *>(CPermAlloc( 1024 )), _IOFBF, 1024 );
    }
}

FILE *OpenBrowseFile( void )
{
    pgroup2     pg;
    char        name[_MAX_PATH];
    FILE        *mbr_file;

    if( cvars_state().CompFlags.cpp_output_to_file ) {
        strcpy( name, createFileName( cvars_state().ObjectFileName, MBR_EXT, true ) );
    } else {
        _splitpath2( cvars_state().SrcFName, pg.buffer, NULL, NULL, &pg.fname, NULL );
        _makepath( name, NULL, NULL, pg.fname, MBR_EXT );
    }
    mbr_file = fopen( name, "wb" );
    if( mbr_file == NULL ) {
        CantOpenFile( name );
    }
    return( mbr_file );
}

void CloseSrcFile( FCB *fcb )
{
    if( fcb->fp != NULL && fcb->fp != stdin )
        fclose( fcb->fp );
    CloseFCB( fcb );
}

static bool FreeSrcFP( void )
/***************************/
{
    FCB     *fcb;
    FCB     *nextfcb;
    bool    ret;

    ret = false;
    for( fcb = cvars_state().SrcFiles; fcb != NULL; fcb = nextfcb ) {
        nextfcb = fcb->prev_file;
        if( nextfcb == NULL || nextfcb->fp == NULL ) {
            break;
        }
    }
    if( fcb != NULL && fcb->fp != NULL ) {
        fcb->rseekpos = ftell( fcb->fp );
        if( fcb->fp != NULL && fcb->fp != stdin )
            fclose( fcb->fp );
        fcb->fp = NULL;
        ret = true;
    }
    return( ret );
}


static FNAMEPTR FindFlist( char const *filename )
/************************************************
 * find a flist
 */
{
    FNAMEPTR    flist;

    for( flist = cvars_state().FNames; flist != NULL; flist = flist->next ) {
        if( FNAMECMPSTR( filename, flist->name ) == 0 ) {
            break;
        }
    }
    return( flist );
}

static bool IsFNameOnce( char const *filename )
{
    FNAMEPTR    flist;
    char        fullfilename[_MAX_PATH];
    char        *fullpath;

    fullpath = SrcFullPath( filename, fullfilename, sizeof( fullfilename ) );
    if( fullpath == NULL )
        return( false );
    for( flist = cvars_state().FNames; flist != NULL; flist = flist->next ) {
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

static bool TryOpen( const char *path, pgroup2 *ff, src_file_type typ )
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
    if( IsFNameOnce( filename ) ) {
        return( true );
    }
    for( ; (fp = fopen( filename, "rb" )) == NULL; ) {
        if( errno != ENOMEM && errno != ENFILE && errno != EMFILE )
            break;
        if( !FreeSrcFP() ) {        // try closing an include file
            break;
        }
    }
    if( fp == NULL )
        return( false );
#if 0
    if( CompFlags.use_precompiled_header ) {
        CompFlags.use_precompiled_header = false;
        if( UsePreCompiledHeader( filename ) ) {
            fclose( fp );
            return( true );
        }
    }
#endif
    if( OpenFCB( fp, filename, typ ) ) {
        if( cvars_state().CompFlags.cpp_mode ) {
            if( cvars_state().CppFile == NULL )
                OpenCppFile();
            CppEmitPoundLine( 1, filename, true );
            ccmain_state().CppFirstChar = true;
        }
        return( true );
    }
    fclose( fp );
    return( false );
}

FNAMEPTR AddFlist( char const *filename )
{
    FNAMEPTR    flist;
    FNAMEPTR    *lnk;
    unsigned    index;
    size_t      len1;
    size_t      len2;
    struct stat statbuf;

    index = 0;
    for( lnk = &cvars_state().FNames; (flist = *lnk) != NULL; lnk = &flist->next ) {
        if( FNAMECMPSTR( filename, flist->name ) == 0 )
            break;
        index++;
    }
    if( flist == NULL ) {
        len1 = strlen( filename ) + 1;
        if( HAS_PATH( filename ) || cvars_state().DependHeaderPath == NULL ) {
            len2 = 0;
            flist = (FNAMEPTR)CMemAlloc( offsetof( fname_list, name ) + len1 );
        } else {
            len2 = strlen( cvars_state().DependHeaderPath );
            flist = (FNAMEPTR)CMemAlloc( offsetof( fname_list, name ) + len2 + len1 );
            strcpy( flist->name, cvars_state().DependHeaderPath );
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

FNAMEPTR FileIndexToFName( unsigned file_index )
{
    FNAMEPTR    flist;

    for( flist = cvars_state().FNames; flist != NULL; flist = flist->next ) {
        if( flist->index == file_index ) {
            break;
        }
    }
    return( flist );
}

char *FNameFullPath( FNAMEPTR flist )
{
    char   fullbuff[2 * _MAX_PATH];
    char   *fullpath;

    if( flist->fullpath == NULL ) {
        fullpath = SrcFullPath( flist->name, fullbuff, sizeof( fullbuff ) );
        if( fullpath != NULL ) {
            fullpath = CMemStrdup( fullpath );
            flist->fullpath = fullpath;
        } else {
            fullpath = flist->name;
        }
    } else {
        fullpath = flist->fullpath;
    }
    return( fullpath );
}

char *FileIndexToCorrectName( unsigned file_index )
{
    FNAMEPTR    flist;
    char        *name;

    if( NULL == (flist = FileIndexToFName( file_index )) )
        return( NULL );
    if( cvars_state().CompFlags.ef_switch_used ) {
        name = FNameFullPath( flist );
    } else {
        name = flist->name;
    }
    return( name );
}

void FreeFNames( void )
{
    FNAMEPTR    flist;

    while( (flist = cvars_state().FNames) != NULL ) {
        cvars_state().FNames = flist->next;
        if( flist->fullpath != NULL ) {
            CMemFree( flist->fullpath );
        }
        CMemFree( flist );
    }
}

void AddIncFile( INCFILE *ifile )
{
    INCFILE     *ifilep;

    ifile->nextfile = NULL;
    if( cvars_state().IncFileList == NULL ) {
        cvars_state().IncFileList = ifile;
    } else {
        ifilep = cvars_state().IncFileList;
        while( ifilep->nextfile != NULL )
            ifilep = ifilep->nextfile;
        ifilep->nextfile = ifile;
    }
}

void AddIncFileList( const char *filename )
{
    INCFILE     *ifile;

    ifile = (INCFILE *)CMemAlloc( offsetof( INCFILE, filename ) + strlen( filename ) + 1 );
    strcpy( ifile->filename, filename );
    AddIncFile( ifile );
}

void FreeIncFileList( void )
{
    INCFILE *ilist;

    while( (ilist = cvars_state().IncFileList) != NULL ) {
        cvars_state().IncFileList = ilist->nextfile;
        CMemFree( ilist );
    }
}

static RDIRPTR AddRDir( const char *path )
{
    RDIRPTR     dirlist;
    RDIRPTR     *lnk;

    for( lnk = &cvars_state().RDirNames; (dirlist = *lnk) != NULL; lnk = &dirlist->next ) {
        if( stricmp( path, dirlist->name ) == 0 ) {
            break;
        }
    }
    if( dirlist == NULL ) {
        dirlist = (RDIRPTR)CMemAlloc( offsetof( rdir_list, name ) + strlen( path ) + 1 );
        dirlist->next = NULL;
        strcpy( dirlist->name, path );
        *lnk = dirlist;
    }
    return( dirlist );
}

void FreeRDir( void )
{
    RDIRPTR    dirlist;

    while( (dirlist = cvars_state().RDirNames) != NULL ) {
        cvars_state().RDirNames = dirlist->next;
        CMemFree( dirlist );
    }
}

void SrcFileReadOnlyDir( char const *dirs )
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
        AddRDir( full );
    }
}

bool SrcFileInRDir( FNAMEPTR flist )
{
    RDIRPTR     dirlist;
    bool        read_only;      // - true ==> file is in read-only directory
    char        *fullpath;      // - full path

    read_only = false;
    fullpath = FNameFullPath( flist );
    for( dirlist = cvars_state().RDirNames; dirlist != NULL; dirlist = dirlist->next ) {
        if( strnicmp( dirlist->name, fullpath, strlen( dirlist->name ) ) == 0 ) {
            read_only = true;
            break;
        }
    }
    return( read_only );
}

void SrcFileReadOnlyFile( char const *file )
{
    FNAMEPTR    flist;

    if( file == NULL ) {
        flist = cvars_state().SrcFiles->src_flist;
    } else {
        flist= FindFlist( file );
    }
    if( flist  != NULL ) {
        flist->rwflag = false;
    }
}

void SrcFileIncludeAlias( const char *alias_name, const char *real_name, bool is_lib )
{
    AddIAlias( alias_name, real_name, is_lib );
}

void SetSrcFNameOnce( void )
{
    cvars_state().SrcFiles->src_flist->once = true;
}

static void ParseInit( void )
{
    const char  *force;

    ScanInit();
    CTypeInit();
    MacroInit();
    SymInit();
    SpcSymInit();
    StringInit();
    InitDataQuads();
    ExprInit();
    StmtInit();
    SegInit();
    force = FEGetEnv( "FORCE" );
    if( force != NULL ) {
        cvars_state().ForceInclude = CMemStrdup( force );
    }
    cvars_state().ForcePreInclude = CMemStrdup( "_preincl.h" );
}

static bool OpenPgmFile( void )
{
    if( ccmain_state().IsStdIn ) {
        if( OpenFCB( stdin, STDIN_NAME, FT_SRC ) ) {
            if( cvars_state().CompFlags.cpp_mode ) {
                if( cvars_state().CppFile == NULL )
                    OpenCppFile();
                CppEmitPoundLine( 1, STDIN_NAME, true );
                ccmain_state().CppFirstChar = true;
            }
            cvars_state().MainSrcFile = cvars_state().SrcFiles;
            return( true );
        }
        return( false );
    }
    if( ccmain_state().IsNullDev ) {
        if( OpenFCB( NULL, NULLDEV_NAME, FT_SRC ) ) {
            if( cvars_state().CompFlags.cpp_mode ) {
                if( cvars_state().CppFile == NULL )
                    OpenCppFile();
                CppEmitPoundLine( 1, NULLDEV_NAME, true );
                ccmain_state().CppFirstChar = true;
            }
            cvars_state().MainSrcFile = cvars_state().SrcFiles;
            return( true );
        }
        return( false );
    }
    if( !OpenSrcFile( cvars_state().WholeFName, FT_SRC ) ) {
        CantOpenFile( cvars_state().WholeFName );
        CSuicide();
        return( false );
    }
#if _CPU == 370
    SrcFiles->column = Column;
    SrcFiles->trunc = Trunc;
#endif
    cvars_state().MainSrcFile = cvars_state().SrcFiles;
    return( true );
}


static void Parse( void )
{
    EmitInit();
    /*
     * The first token in a file should be #include if a user wants to
     * use pre-compiled headers. The following call to NextToken() to
     * get the very first token of the file will load the pre-compiled
     * header if the user requested such and it is a #include directive.
     */
    if( cvars_state().ForceInclude != NULL ) {
        if( !OpenSrcFile( cvars_state().ForceInclude, FT_HEADER_FORCED ) ) {
            PrtfFilenameErr( cvars_state().ForceInclude, FT_HEADER_FORCED, true );
        }
    }
    if( cvars_state().ForcePreInclude != NULL ) {
        openForcePreInclude();
    }
    cvars_state().CompFlags.ok_to_use_precompiled_hdr = true;
    NextToken();
    /*
     * If we didn't get a #include with the above call to NextToken()
     * it's too late to use pre-compiled header now.
     */
    cvars_state().CompFlags.ok_to_use_precompiled_hdr = false;
    ParsePgm();
    if( cvars_state().DefFile != NULL ) {
        fclose( cvars_state().DefFile );
        cvars_state().DefFile = NULL;
    }
    CheckCallParms();
    EndBlock();     /* end of block 0 */
    MacroFini();
}

static void CPP_Parse( void )
{
    if( cvars_state().ForceInclude != NULL ) {
        CppPrtChar( '\n' );
        if( !OpenSrcFile( cvars_state().ForceInclude, FT_HEADER_FORCED ) ) {
            PrtfFilenameErr( cvars_state().ForceInclude, FT_HEADER_FORCED, true );
        }
    }
    cvars_state().CurToken = T_NULL;
    while( cvars_state().CurToken != T_EOF ) {
        cvars_state().CurToken = GetNextToken();
        CppPrtToken( cvars_state().CurToken );
    }
    MacroFini();
}

static void DoCCompile( char **cmdline )
/**************************************/
{
    jmp_buf     env;

    cvars_state().Environment = JMPBUF_PTR( env );
    if( setjmp( env ) ) {           /* if fatal error has occurred */
        EmitAbort();                /* abort code generator */
        CPragmaFini();
        CloseFiles();
        FreeFNames();
        FreeRDir();
        FreeIAlias();
        cvars_state().ErrCount = 1;
        MyExit( 1 );
    }
    ParseInit();
    if( ParseCmdLine( cmdline ) ) {
        if( cvars_state().CompFlags.cpp_dump_macros ) {
            if( cvars_state().CppFile == NULL )
                OpenCppFile();
            DumpAllMacros();
            MacroFini();
        } else if( cvars_state().WholeFName == NULL ) {
            CErr1( ERR_FILENAME_REQUIRED );
            return;
        } else {
            MakePgmName();
            DelErrFile();               /* delete old error file */
            CPragmaInit();              /* memory model is known now */
#if _CPU == 370
            ParseAuxFile();
#endif
            if( cvars_state().CompFlags.cpp_mode ) {
                cvars_state().PrintWhiteSpace = true;
                if( cvars_state().ForcePreInclude != NULL ) {
                    if( cvars_state().CppFile == NULL )
                        OpenCppFile();
                    cvars_state().CompFlags.cpp_output = false;
                    if( openForcePreInclude() ) {
                        cvars_state().CurToken = T_NULL;
                        while( cvars_state().CurToken != T_EOF ) {
                            cvars_state().CurToken = GetNextToken();
                        }
                    }
                    cvars_state().CompFlags.cpp_output = true;
                }
                OpenPgmFile();
                CPP_Parse();
                if( !cvars_state().CompFlags.quiet_mode ) {
                    PrintStats();
                }
                if( cvars_state().CompFlags.warnings_cause_bad_exit ) {
                    cvars_state().ErrCount += cvars_state().WngCount;
                }
            } else {
                OpenPgmFile();
                MacroAddComp(); // Add any compile time only macros
                Parse();
                if( !cvars_state().CompFlags.quiet_mode ) {
                    PrintStats();
                }
                if( cvars_state().CompFlags.warnings_cause_bad_exit ) {
                    cvars_state().ErrCount += cvars_state().WngCount;
                }
                if( ( cvars_state().ErrCount == 0 ) && ( !cvars_state().CompFlags.check_syntax ) ) {
                    if( cvars_state().CompFlags.emit_browser_info ) {
                        DwarfBrowseEmit();
                    }
                    FreeMacroSegments();
                    DoCompile();
                } else {
                    FreeMacroSegments();
                }
            }
            if( cvars_state().ErrCount == 0 ) {
                DumpDepFile();
            } else {
                DelDepFile();
            }
            SymFini();
            CPragmaFini();
        }
    } else {
        cvars_state().ErrCount = 1;
    }
    CloseFiles();
    FreeFNames();
    FreeRDir();
    FreeIAlias();
    FreeIncFileList();
}

static bool try_open_file( const char *path, pgroup2 *fp, pgroup2 *fa, src_file_type typ )
{
    bool    ok;
    char    save_chr_fname;
    char    save_chr_ext;

    /*
     * try to open regular name
     */
    ok = TryOpen( path, fp, typ );
    if( ok ) {
        return( ok );
    }
    if( fa != NULL ) {
        /*
         * try to open alias name if defined
         */
        ok = TryOpen( path, fa, typ );
        if( ok ) {
            return( ok );
        }
    }
    if( cvars_state().CompFlags.check_truncated_fnames ) {
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
            ok = TryOpen( path, fp, typ );
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

static bool doOpenSrcFile( pgroup2 *fp, pgroup2 *fa, src_file_type typ )
{
    char        *s;
    char        *p;
    char        try_[_MAX_PATH];
    FCB         *fcb;
    char        c;
    pgroup2     fd;

    if( typ == FT_SRC ) {
        if( try_open_file( "", fp, NULL, typ ) )
            return( true );
        if( !cvars_state().CompFlags.ignore_default_dirs ) {
            if( try_open_file( C_PATH, fp, NULL, typ ) ) {
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
        if( try_open_file( "", fp, fa, typ ) ) {
            return( true );
        }
    } else {
        switch( typ ) {
        case FT_HEADER_PRE:
        case FT_LIBRARY:
            break;
        case FT_HEADER:
        case FT_HEADER_FORCED:
            if( cvars_state().CompFlags.ignore_default_dirs ) {
                try_[0] = '\0';
                /*
                 * physical file name must be used, not logical
                 */
                _splitpath2( cvars_state().SrcFiles->src_flist->name, fd.buffer, &fd.drive, &fd.dir, NULL, NULL );
                _makepath( try_, fd.drive, fd.dir, NULL, NULL );
                if( try_open_file( try_, fp, fa, typ ) ) {
                    return( true );
                }
            } else {
                /*
                 * try current directory
                 */
                if( !cvars_state().GlobalCompFlags.ignore_current_dir ) {
                    if( try_open_file( "", fp, fa, typ ) ) {
                        return( true );
                    }
                }
                for( fcb = cvars_state().SrcFiles; fcb!= NULL; fcb = fcb->prev_file ) {
                    /*
                     * physical file name must be used, not logical
                     */
                    _splitpath2( fcb->src_flist->name, fd.buffer, &fd.drive, &fd.dir, NULL, NULL );
                    _makepath( try_, fd.drive, fd.dir, NULL, NULL );
                    if( try_open_file( try_, fp, fa, typ ) ) {
                        return( true );
                    }
                }
            }
            break;
        }
        s = cvars_state().IncPathList;
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
            if( try_open_file( fd.buffer, fp, fa, typ ) ) {
                return( true );
            }
        }
        switch( typ ) {
        case FT_HEADER_PRE:
        case FT_LIBRARY:
            break;
        case FT_HEADER:
        case FT_HEADER_FORCED:
            if( !cvars_state().CompFlags.ignore_default_dirs ) {
                /*
                 * try current ../h directory
                 */
                if( try_open_file( H_PATH, fp, fa, typ ) ) {
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
#if defined( __UNIX__ )
        if( c == '\\' )
            *dir = '/';
#else
        if( c == '/' )
            *dir = '\\';
#endif
        dir++;
    }
}

bool OpenSrcFile( const char *filename, src_file_type typ )
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
        alias_filename = IncludeAlias( filename, ( typ == FT_LIBRARY || typ == FT_HEADER_PRE ) );
        if( alias_filename != NULL ) {
            _splitpath2( alias_filename, fa.buffer, &fa.drive, &fa.dir, &fa.fname, &fa.ext );
            normalizeSep( fa.dir );
            fap = &fa;
        }
        break;
    }
    return( doOpenSrcFile( &fp, fap, typ ) );
}


void CppEmitPoundLine( unsigned line_num, const char *filename, bool newline )
{
    char    buf[30];
    char    c;

    if( cvars_state().CompFlags.cpp_line_wanted && CppPrinting() && cvars_state().CompFlags.cpp_output ) {
        sprintf( buf, "#line %u \"", line_num );
        CppPuts( buf );
        while( (c = *filename++) != '\0' ) {
            if( c == '\\' )
                c = '/';
            CppPutc( c );
        }
        CppPutc( '"' );
        if( newline ) {
            CppPutc( '\n' );
        }
    }
}

bool CppPrinting( void )
{
    return( cvars_state().SkipLevel == cvars_state().NestLevel );
}

void CppPrtToken( TOKEN token )
{
    if( CppPrinting() && cvars_state().CompFlags.cpp_output ) {
        switch( token ) {
        case T_BAD_CHAR:
        case T_BAD_TOKEN:
        case T_ID:
        case T_CONSTANT:
            CppPuts( cvars_state().Buffer );
            break;
        case T_STRING:
            if( cvars_state().CompFlags.wide_char_string )
                CppPutc( 'L' );
            CppPutsQuoted( cvars_state().Buffer );
            break;
        case T_EOF:
        case T_NULL:
            break;
        case T_WHITE_SPACE:
            if( cvars_state().PrintWhiteSpace || cvars_state().CompFlags.in_pragma ) {
                CppPuts( TokenString[token] );
            } else {
                cvars_state().PrintWhiteSpace = true; //Toggle
            }
            break;
        default:
            CppPuts( TokenString[token] );
        }
    }
}


TOKEN GetNextToken( void )
{
    TOKEN   token;

    if( cvars_state().MacroPtr != NULL ) {
        token = GetMacroToken();
    } else {
        while( cvars_state().CurrChar != LCHR_EOF ) {
            if( (scan_state().CharSet[cvars_state().CurrChar] & C_WS) == 0 )
                break;
            if( cvars_state().CurrChar != '\r' )
                CppPrtChar( cvars_state().CurrChar );
            cvars_state().NextChar();
        }
        token = ScanToken();
    }
    return( token );
}

void CloseFiles( void )
{
    if( cvars_state().CppFile != NULL ) {
        fflush( cvars_state().CppFile );
        if( ferror( cvars_state().CppFile ) ) {
            char    msgtxt[80];
            char    msgbuf[MAX_MSG_LEN];

            /*
             * issue message
             */
            CGetMsg( msgtxt, ERR_FATAL_ERROR );
            sprintf( msgbuf, msgtxt, strerror( errno ) );
            NoteMsg( msgbuf );
        }
        if( cvars_state().CppFile != stdout ) {     /* (stdout is the program's, and stays open after this run) */
            fclose( cvars_state().CppFile );
        }
        cvars_state().CppFile = NULL;
    }
    if( cvars_state().ErrFile != NULL ) {
        fclose( cvars_state().ErrFile );
        cvars_state().ErrFile = NULL;
        if( ! cvars_state().CompFlags.errfile_written ) {
            DelErrFile();
        }
    }
    if( cvars_state().DefFile != NULL ) {
        fclose( cvars_state().DefFile );
        cvars_state().DefFile = NULL;
    }
}


bool FrontEnd( char **cmdline )
{
#if defined(__WATCOMC__) && defined( _M_IX86 )
    /*
     * set to 0 in case 8087 is present
     */
    _real87 = 0;
    _8087 = 0;
#endif

    InitGlobalVars();
    CMemInit();
    InitMsg();
    InitPurge();
    initGlobals();
    DoCCompile( cmdline );
    finiGlobals();
    PurgeMemory();
    FiniMsg();
    CMemFini();
    cvars_state().GlobalCompFlags.cc_first_use = false;
    return( cvars_state().ErrCount != 0 );
}
