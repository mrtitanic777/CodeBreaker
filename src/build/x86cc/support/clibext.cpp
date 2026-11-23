/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Implementation of non-standard functions used by bootstrap
*
****************************************************************************/


/****************************************************************************
*
* Notes:
*   botstrap tools use only single-byte encoding that no need to deal with
*   multi-byte encodings or wide character support
*
****************************************************************************/


    #include <string.h>
    #include <ctype.h>
    #include <errno.h>
  #if defined( _MSC_VER )
    #include <windows.h>
    #include "_dtaxxx.h"
    #include "ntext.h"
  #endif
    #include "wio.h"
    #include "wreslang.h"


#include "clibint.h"
#include "clibext.h"
#include "supportrun.hpp"


/****************************************************************************
*
* Description:  Platform independent macros.
*
****************************************************************************/

inline bool IsUncPath( const char *x ) { return ((x)[0] == '\\' && (x)[1] == '\\'); }
inline bool HasDrive( const char *x ) { return (isalpha( (x)[0] ) && (x)[1] == ':'); }


    #define PC '\\'
inline bool IsPathSepChar( int c ) { return ((c)==PC || (c)=='/'); }

/****************************************************************************
*
* Description:  global command line parameters used by OW
*               must be setup by main
*
****************************************************************************/

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct ClibextFileState: clibext_state.h) */

}  // namespace




/****************************************************************************
*
* Description:  Platform independent _splitpath2() implementation.
*
*****************************************************************************
*
* split full path name into its components
*
* Under Unix we will map drive to node, dir to dir, and
* filename to (filename and extension)
*          or (filename) if no extension requested.
*/

static char *pcopy( char **pdst, char *dst, const char *srcb, const char *srce )
/*============================================================================*/
{
    size_t    len;

    if( pdst == NULL )
        return( dst );
    *pdst = dst;
    len = srce - srcb;
    if( len > _MAX_PATH2 - 1 ) {
        len = _MAX_PATH2 - 1;
    }
    strncpy( dst, srcb, len );
    dst[len] = '\0';
    return( dst + len + 1 );
}

void  _splitpath2( char const *inp, char *outp, char **drive, char **path, char **fn, char **ext )
/*==============================================================================================*/
{
    const char      *dotp;
    const char      *fnamep;
    const char      *startp;
    unsigned char   ch;

    /*
     * take apart specification like
     *   //0/hd/user/fred/filename.ext for QNX
     *   \\disk2\fred\filename.ext for UNC names
     *   c:\fred\filename.ext for DOS, OS/2, Windows
     *
     * process node/drive/UNC specification
     */
    startp = inp;
    if( IsUncPath( inp ) ) {
        inp += 2;
        for( ;; ) {
            if( *inp == '\0' )
                break;
            if( IsPathSepChar( *inp ) )
                break;
            if( *inp == '.' )
                break;
            inp++;
        }
        outp = pcopy( drive, outp, startp, inp );
    } else if( HasDrive( inp ) ) {
        /*
         * process drive specification
         */
        if( drive != NULL ) {
            *drive = outp;
            *outp++ = inp[0];
            *outp++ = ':';
            *outp++ = '\0';
        }
        inp += 2;
    } else if( drive != NULL ) {
        *drive = outp;
        *outp++ = '\0';
    }
    /*
     * process
     * /user/fred/filename.ext for QNX
     * \fred\filename.ext for DOS, OS/2, Windows
     * /fred/filename.ext for DOS, OS/2, Windows
     */
    dotp = NULL;
    fnamep = inp;
    startp = inp;

    for(;;) {
        ch = *inp;
        if( ch == 0 )
            break;
        if( ch == '.' ) {
            dotp = inp;
            inp++;
            continue;
        }
        inp++;
        if( IsPathSepChar( ch ) ) {
            fnamep = inp;
            dotp = NULL;
        }
    }
    outp = pcopy( path, outp, startp, fnamep );
    if( dotp == NULL )
        dotp = inp;
    outp = pcopy( fn, outp, fnamep, dotp );
    outp = pcopy( ext, outp, dotp, inp );
}


/****************************************************************************
*
* Description:  Platform independent _makepath() implementation.
*
****************************************************************************/


/****************************************************************************
*
* Description:  Implementation of fullpath() - returns fully qualified
*               pathname of a file.
*
****************************************************************************/


/****************************************************************************
*
* Description:  Implementation of _cmdname().
*
****************************************************************************/

#if defined( _MSC_VER )

char *_cmdname( SupportRun& support, char *name )
{
    char    *pgm;
    char    buff[_MAX_PATH];
    DWORD   size;

    pgm = NULL;
    if( _get_pgmptr( &pgm ) != 0
      || pgm == NULL
      || *pgm == '\0' ) {
        /* fix for buggy _get_pgmptr in some versions of Microsoft CRTL */
        pgm = buff;
        size = GetModuleFileName( NULL, buff, sizeof( buff ) );
        if( size == sizeof( buff ) )
            size--;
        buff[size] = '\0';
    }
    return( strcpy( name, pgm ) );
}

#else   /* others */

char *_cmdname( SupportRun& support, char *name )
{
    return( strcpy( name, support.clibint._argv[0] ) );
}

#endif

/****************************************************************************
*
* Description:  Implementation of _bgetcmd().
*
****************************************************************************/

#if defined( _MSC_VER )

int _bgetcmd( SupportRun& support, char *buffer, int len )
{
    int         cmdlen;
    char        *cmd;
    char        *tmp;

    if( ( buffer != NULL )
      && ( len > 0 ) )
        *buffer = '\0';

    cmd = GetCommandLine();
    if( *cmd == '"' ) {
        cmd++;
        while( *cmd != '\0' && *cmd != '"' ) {
            cmd++;
        }
        if( *cmd != '\0' ) {
            cmd++;
        }
    } else {
        while( *cmd != '\0' && *cmd != ' ' && *cmd != '\t' ) {
            ++cmd;
        }
    }

    while( *cmd == ' ' || *cmd == '\t' )
        ++cmd;

    for( cmdlen = 0, tmp = cmd; *tmp != '\0'; ++tmp, ++cmdlen )
        ;

    if( ( buffer == NULL )
      || ( len <= 0 ) )
        return( cmdlen );

    len--;
    len = (len < cmdlen) ? len : cmdlen;

    while( len ) {
        *buffer++ = *cmd++;
        --len;
    }
    *buffer = '\0';

    return( cmdlen );
}

#else   /* others */

int (_bgetcmd)( SupportRun& support, char *buffer, int len )
{
    int     total;
    int     i;
    char    *word;
    char    *p;
    char    **argv = &support.clibint._argv[1];
    int     need_quotes;

    if( ( buffer != NULL )
      && ( len > 0 ) ) {
        p = buffer;
        --len;          /* reserve space for terminating NULL byte */
    } else {
        p = NULL;
        len = 0;
    }

    /* create approximation of original command line */
    total = 0;
    while( (word = *argv++) != NULL ) {
        /* account for at least one space separating arguments */
        if( total != 0 ) {
            ++total;
            if( len != 0 ) {
                --len;
                *p++ = ' ';
            }
        }

        i = strlen( word );
        total += i;
        need_quotes = ( strpbrk( word, " " ) != NULL || i == 0 );
        if( need_quotes )
            total += 2;

        if( need_quotes
          && len != 0 ) {
            --len;
            *p++ = '"';
        }
        if( len != 0
          && i != 0 ) {
            if( i > len )
                i = len;
            memcpy( p, word, i );
            p += i;
            len -= i;
        }
        if( need_quotes
          && len != 0 ) {
            --len;
            *p++ = '"';
        }
    }
    if( p != NULL ) {
        *p = '\0';  /* terminate string */
    }

    return( total );
}

#endif  /* !_MSC_VER */


/****************************************************************************
*
* Description:  Implementation of spawn.. functions for Unix.
*
****************************************************************************/


/****************************************************************************
*
* Description:  This function searches for the specified file in the
*               1) current directory or, failing that,
*               2) the paths listed in the specified environment variable
*               until it finds the first occurrence of the file.
*
****************************************************************************/



/****************************************************************************
*
* Description:  POSIX fnmatch.
*
*****************************************************************************
*
* Implementation note: On non-UNIX systems, backslashes in the string
* (but not in the pattern) are considered to be path separators and
* identical to forward slashes when FNM_PATHNAME is set.
*/

#if defined(_MSC_VER)

static const struct my_wctypes {
    const char  *name;
    int         mask;
} my_wctypes[] = {
    { "alnum", _ALPHA | _DIGIT },
    { "alpha", _ALPHA },
    { "cntrl", _CONTROL },
    { "digit", _DIGIT },
    { "graph", _PUNCT | _ALPHA | _DIGIT },
    { "lower", _LOWER },
    { "print", _BLANK | _PUNCT | _ALPHA | _DIGIT },
    { "punct", _PUNCT },
    { "space", _SPACE },
    { "upper", _UPPER },
    { "xdigit", _HEX }
};




/* Maximum length of character class name.
 * The longest is currently 'xdigit' (6 chars).
 */
    #define CCL_NAME_MAX    8

/* Note: Using wctype()/iswctype() may seem odd, but that way we can avoid
 * hardcoded character class lists.
 */



#endif

/****************************************************************************
*
* Description:  Implementation of POSIX environment setenv
*
****************************************************************************/

#if defined(_MSC_VER)

int setenv( SupportRun& support, const char *name, const char *newvalue, int overwrite )
/*****************************************************************/
{
    char    *buff;
    size_t  len;

    len = strlen( name ) + strlen( newvalue ) + 16;
    buff = static_cast<char *>(support.held->malloc( len ));
    overwrite = overwrite;
    sprintf( buff, "%s=%s", name, newvalue );
    putenv( buff );
    support.held->free( buff );
    return( 0 );
}

#endif

/****************************************************************************
*
* Description:  Implementation of POSIX environment unsetenv
*
****************************************************************************/

#if defined(_MSC_VER)


#endif

/****************************************************************************
*
* Description:  Implementation of POSIX opendir/readdir/closedir
*
****************************************************************************/

#if defined(_MSC_VER) \
  || defined( __WATCOMC__ ) \
  && ( __WATCOMC__ == 1290 ) \
  && defined( __NT__ )      /* OW 1.9 fix */

    #define _DIR_ISFIRST            0
    #define _DIR_NOTFIRST           1
    #define _DIR_CLOSED             2




    #define MAKEDOSDTXX     __MakeDOSDT
    #define FROMDOSDTXX     __FromDOSDT
    #define GETNTDIRINFOXX  __GetNTDirInfo


time_t __NT_filetime_to_timet( const FILETIME *ft )
{
    ULARGE_INTEGER  ulint;

    ulint.u.LowPart   =   ft->dwLowDateTime;
    ulint.u.HighPart  =   ft->dwHighDateTime;
    return( ( ulint.QuadPart - TICK_TO_UNIX_EPOCH ) / WINDOWS_TICK_PER_SEC );
}




void FROMDOSDTXX( unsigned short d, unsigned short t, FILETIME *NT_stamp )
/************************************************************************/
{
    FILETIME local_ft;

    DosDateTimeToFileTime( d, t, &local_ft );
    LocalFileTimeToFileTime( &local_ft, NT_stamp );
}

void MAKEDOSDTXX( FILETIME *NT_stamp, unsigned short *d, unsigned short *t )
/**************************************************************************/
{
    FILETIME local_ft;

    FileTimeToLocalFileTime( NT_stamp, &local_ft );
    FileTimeToDosDateTime( &local_ft, d, t );
}

    #define NT_FIND_ATTRIBUTES_MASK (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DIRECTORY)

bool __NTFindNextFileWithAttr( HANDLE osffh, unsigned nt_attrib, LPWIN32_FIND_DATA ffd )
/**************************************************************************************/
{
    for( ;; ) {
        if( (nt_attrib | ~ffd->dwFileAttributes) & NT_FIND_ATTRIBUTES_MASK ) {
            return( true );
        }
        if( FindNextFile( osffh, ffd ) == 0 ) {
            return( false );
        }
    }
}

static int is_directory( const char *name )
/*****************************************/
{
    unsigned char    curr_ch;
    unsigned char    prev_ch;

    curr_ch = '\0';
    for(;;) {
        prev_ch = curr_ch;
        curr_ch = *name;
        if( curr_ch == '\0' ) {
            if( IsPathSepChar( prev_ch )
              || prev_ch == ':' ){
                /* directory, need add "*.*" */
                return( 2 );
            }
            if( prev_ch == '.' ){
                /* directory, need add "\\*.*" */
                return( 1 );
            }
            /* without wildcards maybe file or directory, need next check */
            /* need add "\\*.*" if directory */
            return( 0 );
        }
        if( curr_ch == '*' )
            break;
        if( curr_ch == '?' )
            break;
        name++;
    }
    /* with wildcard must be file */
    return( -1 );
}

void GETNTDIRINFOXX( struct DIRENTXX *dirp, LPWIN32_FIND_DATA ffd )
/*****************************************************************/
{
    DtaTimeOf( dirp->d_dta ) = __NT_filetime_to_timet( &ffd->ftLastWriteTime );
    MAKEDOSDTXX( &ffd->ftLastWriteTime, &dirp->d_date, &dirp->d_time );
    dirp->d_attr = NtToDosAttr( ffd->dwFileAttributes );
    dirp->d_size = ffd->nFileSizeLow;
    strncpy( dirp->d_name, ffd->cFileName, NAME_MAX );
    dirp->d_name[NAME_MAX] = 0;
}

static DIRXX *__opendir( const char *dirname, DIRXX *dirp )
/*********************************************************/
{
    WIN32_FIND_DATA     ffd;
    HANDLE              osffh;

    if( dirp->d_first != _DIR_CLOSED ) {
        FindClose( DtaHandleOf( dirp->d_dta ) );
        dirp->d_first = _DIR_CLOSED;
    }
    osffh = FindFirstFile( dirname, &ffd );
    if( osffh == INVALID_HANDLE_VALUE ) {
        errno = ENOENT;
        return( NULL );
    }
    DtaHandleOf( dirp->d_dta ) = osffh;
    GETNTDIRINFOXX( dirp, &ffd );
    dirp->d_first = _DIR_ISFIRST;
    return( dirp );
}

DIRXX *OPENDIRXX( SupportRun& support, const char *dirname )
/*************************************/
{
    DIRXX       tmp;
    DIRXX       *dirp;
    int         i;
    char        pathname[MAX_PATH + 6];

    memset( &tmp, 0, sizeof( tmp ) );
    tmp.d_attr = _A_SUBDIR;
    tmp.d_first = _DIR_CLOSED;
    i = is_directory( dirname );
    if( i <= 0 ) {
        if( __opendir( dirname, &tmp ) == NULL ) {
            return( NULL );
        }
    }
    if( i >= 0
      && (tmp.d_attr & _A_SUBDIR) ) {
        size_t          len;

        /* directory, add wildcard */
        len = strlen( dirname );
        memcpy( pathname, dirname, len );
        if( i < 2 ) {
            pathname[len++] = PC;
        }
        strcpy( &pathname[len], "*.*" );
        if( __opendir( pathname, &tmp ) == NULL ) {
            return( NULL );
        }
        dirname = pathname;
    }
    dirp = static_cast<DIR *>(support.held->malloc( sizeof( *dirp ) ));
    if( dirp == NULL ) {
        FindClose( DtaHandleOf( tmp.d_dta ) );
        errno = ENOMEM;
        return( NULL );
    }
    tmp.d_openpath = strdup( dirname );
    *dirp = tmp;
    return( dirp );
}

struct DIRENTXX *READDIRXX( DIRXX *dirp )
/***************************************/
{
    WIN32_FIND_DATA     ffd;

    if( dirp == NULL
      || dirp->d_first == _DIR_CLOSED )
        return( NULL );
    if( dirp->d_first == _DIR_ISFIRST ) {
        dirp->d_first = _DIR_NOTFIRST;
    } else {
        if( FindNextFileA( DtaHandleOf( dirp->d_dta ), &ffd ) == 0 ) {
            errno = ENOENT;
            return( NULL );
        }
        GETNTDIRINFOXX( dirp, &ffd );
    }
    return( dirp );
}

int CLOSEDIRXX( SupportRun& support, DIRXX *dirp )
/***************************/
{
    if( dirp == NULL
      || dirp->d_first == _DIR_CLOSED ) {
        errno = ERANGE;
        return( -1 );
    }
    if( FindClose( DtaHandleOf( dirp->d_dta ) ) == 0 ) {
        return( -1 );
    }
    dirp->d_first = _DIR_CLOSED;
    if( dirp->d_openpath != NULL )
        support.held->free( dirp->d_openpath );
    support.held->free( dirp );
    return( 0 );
}

#endif

/****************************************************************************
*
* Description:  Implementation of POSIX getopt
*
****************************************************************************/

#if defined(_MSC_VER)


    /* Error messages suggested by Single UNIX Specification */


#endif

/****************************************************************************
*
* Description:  Implementation of POSIX mkstemp
*
****************************************************************************/

#if defined(_MSC_VER)



#endif

/****************************************************************************
*
* Description:  Implementation of sleep function
*
****************************************************************************/

#if defined(_MSC_VER)


#endif /* _MSC_VER */

/****************************************************************************
*
* Description:  Determine resource language from system environment.
*
****************************************************************************/


wres_lang_id _WResLanguage( void )
{
    return static_cast<wres_lang_id>(( RLE_ENGLISH ));
}


/****************************************************************************
*
* Description:  Get DLL file name to load resources.
*
****************************************************************************/


char *get_dllname( SupportRun& support, char *buf, int len )
{
  #ifdef _MSC_VER
    HMODULE hnd = NULL;
  #endif

    *buf = '\0';
  #ifdef _MSC_VER
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)&support.clibext.optind, &hnd);
    GetModuleFileName( hnd, buf, len );
  #endif
    return( buf );
}


/****************************************************************************
*
* Description:  Implementation of _mkgmtime.
*
****************************************************************************/


