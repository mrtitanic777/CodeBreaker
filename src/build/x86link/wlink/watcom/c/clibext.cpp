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

// Notes:
//   bootstrap tools use only single-byte encoding: no multi-byte or wide character support is dealt with.
//
// The functions of Watcom's C library that other C libraries lack, for the one configuration wlink is built in
// (not Open Watcom itself, not a Unix host): the plain C-library branches and, with _MSC_VER, the Windows ones.

#include <algorithm>
#include <array>
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

#if defined( _MSC_VER )
    #include <windows.h>
    #include "_dtaxxx.h"
    #include "ntext.h"
#endif
#include "wio.h"
#include "wreslang.h"

#include "clibint.h"
#include "clibext.h"
#include "run_state.hpp"

namespace {

// Path separators of DOS, OS/2 and Windows.
constexpr char PC = '\\';

constexpr bool ISPS( int c )
{
    return c == PC || c == '/';
}

// "\\server" (a UNC name)
inline bool test_unc( const char *x )
{
    return x[0] == '\\' && x[1] == '\\';
}

// "c:"
inline bool test_drive( const char *x )
{
    return std::isalpha( x[0] ) && x[1] == ':';
}

// What the file's functions keep between calls (getopt's position in the argument being parsed).
struct FileState {
    int     opt_offset = 0;         // position in currently parsed argument
    char    altoptchar = '/';       // alternate option character
    char    optchar = '\0';         // matched option char ('-' or altoptchar)
};

} // namespace

/****************************************************************************
*
* Description:  global command line parameters used by OW
*               must be setup by main
*
****************************************************************************/

char **_argv = nullptr;
int  _argc = 0;

/****************************************************************************
*
* Description:  Platform independent _splitpath2() implementation.
*
*****************************************************************************
*
* split full path name into its components
*/

namespace {

// copy srcb..srce (at most _MAX_PATH2 - 1 characters) to dst, if the caller asked for the part (pdst != nullptr)
char *pcopy( char **pdst, char *dst, const char *srcb, const char *srce )
{
    if( pdst == nullptr )
        return dst;
    *pdst = dst;
    const std::size_t len = (std::min<std::size_t>)( static_cast<std::size_t>( srce - srcb ), _MAX_PATH2 - 1 );
    std::memcpy( dst, srcb, len );
    dst[len] = '\0';
    return dst + len + 1;
}

} // namespace

void  _splitpath2( char const *inp, char *outp, char **drive, char **path, char **fn, char **ext )
{
    /*
     * take apart specification like
     *   \\disk2\fred\filename.ext for UNC names
     *   c:\fred\filename.ext for DOS, OS/2, Windows
     *
     * process node/drive/UNC specification
     */
    const char *startp = inp;
    if( test_unc( inp ) ) {
        inp += 2;
        while( *inp != '\0' && !ISPS( *inp ) && *inp != '.' ) {
            inp++;
        }
        outp = pcopy( drive, outp, startp, inp );
    } else if( test_drive( inp ) ) {
        /*
         * process drive specification
         */
        if( drive != nullptr ) {
            *drive = outp;
            *outp++ = inp[0];
            *outp++ = ':';
            *outp++ = '\0';
        }
        inp += 2;
    } else if( drive != nullptr ) {
        *drive = outp;
        *outp++ = '\0';
    }
    /*
     * process
     * \fred\filename.ext for DOS, OS/2, Windows
     * /fred/filename.ext for DOS, OS/2, Windows
     */
    const char *dotp = nullptr;
    const char *fnamep = inp;
    startp = inp;

    for( ;; ) {
        const unsigned char ch = static_cast<unsigned char>( *inp );
        if( ch == 0 )
            break;
        if( ch == '.' ) {
            dotp = inp;
            inp++;
            continue;
        }
        inp++;
        if( ISPS( ch ) ) {
            fnamep = inp;
            dotp = nullptr;
        }
    }
    outp = pcopy( path, outp, startp, fnamep );
    if( dotp == nullptr )
        dotp = inp;
    outp = pcopy( fn, outp, fnamep, dotp );
    outp = pcopy( ext, outp, dotp, inp );
}

/****************************************************************************
*
* Description:  Implementation of _cmdname().
*
****************************************************************************/

#if defined( _MSC_VER )

char *_cmdname( char *name )
{
    char    *pgm = nullptr;

    if( _get_pgmptr( &pgm ) != 0
      || pgm == nullptr
      || *pgm == '\0' ) {
        /* fix for buggy _get_pgmptr in some versions of Microsoft CRTL */
        std::array<char, _MAX_PATH> buff;
        DWORD   size = GetModuleFileName( nullptr, buff.data(), static_cast<DWORD>( buff.size() ) );
        if( size == buff.size() )
            size--;
        buff[size] = '\0';
        return std::strcpy( name, buff.data() );
    }
    return std::strcpy( name, pgm );
}

#else   /* others */

char *_cmdname( char *name )
{
    return std::strcpy( name, _argv[0] );
}

#endif

/****************************************************************************
*
* Description:  Implementation of _bgetcmd().
*
****************************************************************************/

#if defined( _MSC_VER )

int _bgetcmd( char *buffer, int len )
{
    if( buffer != nullptr && len > 0 )
        *buffer = '\0';

    const char *cmd = GetCommandLine();
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

    const int cmdlen = static_cast<int>( std::strlen( cmd ) );

    if( buffer == nullptr || len <= 0 )
        return cmdlen;

    len--;
    len = (std::min)( len, cmdlen );

    std::memcpy( buffer, cmd, static_cast<std::size_t>( len ) );
    buffer[len] = '\0';

    return cmdlen;
}

#else   /* others */

int (_bgetcmd)( char *buffer, int len )
{
    char    *p;
    char    **argv = &_argv[1];

    if( buffer != nullptr && len > 0 ) {
        p = buffer;
        --len;          /* reserve space for terminating NULL byte */
    } else {
        p = nullptr;
        len = 0;
    }

    /* create approximation of original command line */
    int total = 0;
    while( const char *word = *argv++ ) {
        /* account for at least one space separating arguments */
        if( total != 0 ) {
            ++total;
            if( len != 0 ) {
                --len;
                *p++ = ' ';
            }
        }

        int i = static_cast<int>( std::strlen( word ) );
        total += i;
        const bool need_quotes = std::strpbrk( word, " " ) != nullptr || i == 0;
        if( need_quotes )
            total += 2;

        if( need_quotes && len != 0 ) {
            --len;
            *p++ = '"';
        }
        if( len != 0 && i != 0 ) {
            i = (std::min)( i, len );
            std::memcpy( p, word, static_cast<std::size_t>( i ) );
            p += i;
            len -= i;
        }
        if( need_quotes && len != 0 ) {
            --len;
            *p++ = '"';
        }
    }
    if( p != nullptr ) {
        *p = '\0';  /* terminate string */
    }

    return total;
}

#endif  /* !_MSC_VER */

#if defined( _MSC_VER )

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

namespace {

struct CharClass {
    std::string_view    name;
    int                 mask;
};

constexpr std::array<CharClass, 11> my_wctypes = {{
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
}};

int my_wctype( std::string_view name )
{
    for( const CharClass &cc : my_wctypes ) {
        if( cc.name == name ) {
            return cc.mask;
        }
    }
    return 0;
}

int icase( int ch, int flags )
{
    if( flags & FNM_IGNORECASE ) {
        return std::tolower( ch );
    } else {
        return ch;
    }
}

/* Maximum length of character class name.
 * The longest is currently 'xdigit' (6 chars).
 */
constexpr int CCL_NAME_MAX = 8;

/* Note: Using wctype()/iswctype() may seem odd, but that way we can avoid
 * hardcoded character class lists.
 */
int sub_bracket( const char *p, int c )
{
    const char      *s = p;

    switch( *++p ) {
    case ':': {
        ++p;
        std::string sname;
        for( int i = 0; i < CCL_NAME_MAX; i++ ) {
            if( !std::isalpha( *p ) )
                break;
            sname += *p++;
        }
        if( *p++ != ':' )
            return 0;
        if( *p++ != ']' )
            return 0;
        const int type = my_wctype( sname );
        if( type ) {
            return iswctype( static_cast<wint_t>( c ), static_cast<wctype_t>( type ) ) ? static_cast<int>( p - s ) : static_cast<int>( s - p );
        }
        return 0;
    }
    case '=':
        return 0;
    case '.':
        return 0;
    default:
        return 0;
    }
}

const char *cclass_match( const char *patt, int c )
{
    bool        ok = false;
    int         lc = 0;
    int         state = 0;
    bool        invert = false;

    /* Meaning of '^' is unspecified in POSIX - consider it equal to '!' */
    if( *patt == '!'
      || *patt == '^' ) {
        invert = true;
        ++patt;
    }
    while( *patt != '\0' ) {
        if( *patt == ']' )
            return ( ok != invert ) ? patt + 1 : nullptr;

        if( *patt == '[' ) {
            const int sb = sub_bracket( patt, c );
            if( sb < 0 ) {
                patt -= sb;
                ok = false;
                continue;
            } else if( sb > 0 ) {
                patt += sb;
                ok = true;
                continue;
            }
        }

        switch( state ) {
        case 0:
            if( *patt == '\\' )
                ++patt;
            if( *patt == c )
                ok = true;
            lc = static_cast<int>( *patt++ );
            state = 1;
            break;
        case 1:
            if( *patt == '-' ) {
                state = 2;
                ++patt;
                break;
            }
            state = 0;
            break;
        case 2:
            if( *patt == '\\' )
                ++patt;
            if( lc <= c
              && c <= *patt )
                ok = true;
            ++patt;
            state = 0;
            break;
        default:
            return nullptr;
        }
    }
    return nullptr;
}

} // namespace

int   fnmatch( const char *patt, const char *s, int flags )
{
    int         c, cl;
    const char  *start = s;

    while( (c = icase( *patt++, flags )) != '\0' ) {
        switch( c ) {
        case '?':
            if( (flags & FNM_PATHNAME)
              && ISPS( *s ) )
                return FNM_NOMATCH;
            if( (flags & FNM_PERIOD)
              && *s == '.'
              && s == start )
                return FNM_NOMATCH;
            ++s;
            break;
        case '[':
            if( (flags & FNM_PATHNAME)
              && ISPS( *s ) )
                return FNM_NOMATCH;
            if( (flags & FNM_PERIOD)
              && *s == '.'
              && s == start )
                return FNM_NOMATCH;
            patt = cclass_match( patt, *s );
            if( patt == nullptr )
                return FNM_NOMATCH;
            ++s;
            break;
        case '*':
            if( *s == '\0' )
                return 0;
            if( (flags & FNM_PATHNAME)
              && ( *patt == '/' ) ) {
                while( *s != '\0' && !ISPS( *s ) )
                    ++s;
                break;
            }
            if( (flags & FNM_PERIOD)
              && *s == '.'
              && s == start )
                return FNM_NOMATCH;
            if( *patt == '\0' ) {
                /*
                 * Shortcut - don't examine every remaining character.
                 */
                if( flags & FNM_PATHNAME ) {
                    if( (flags & FNM_LEADING_DIR)
                      || std::strchr( s, '/' ) == nullptr ) {
                        return 0;
                    } else {
                        return FNM_NOMATCH;
                    }
                } else {
                    return 0;
                }
            }
            while( (cl = icase( *s, flags )) != '\0' ) {
                if( !fnmatch( patt, s, flags & ~FNM_PERIOD ) )
                    return 0;
                if( (flags & FNM_PATHNAME)
                  && ISPS( cl ) ) {
                    start = s + 1;
                    break;
                }
                ++s;
            }
            return FNM_NOMATCH;
        case '\\':
            if( (flags & FNM_NOESCAPE) == 0 ) {
                c = icase( *patt++, flags );
            }
            [[fallthrough]];
        default:
            if( ISPS( *s ) )
                start = s + 1;
            cl = icase( *s++, flags );
            if( (flags & FNM_PATHNAME)
              && cl == PC )
                cl = '/';
            if( c != cl ) {
                return FNM_NOMATCH;
            }
        }
    }
    if( (flags & FNM_LEADING_DIR)
      && ISPS( *s ) )
        return 0;
    return *s != '\0' ? FNM_NOMATCH : 0;
}

/****************************************************************************
*
* Description:  Implementation of POSIX environment setenv / unsetenv
*
****************************************************************************/

int setenv( const char *name, const char *newvalue, int overwrite )
{
    (void)overwrite;
    std::string buff = std::string( name ) + '=' + newvalue;
    putenv( buff.data() );
    return 0;
}

int unsetenv( const char *name )
{
    std::string buff = std::string( name ) + '=';
    putenv( buff.data() );
    return 0;
}

/****************************************************************************
*
* Description:  Implementation of POSIX opendir/readdir/closedir
*
****************************************************************************/

namespace {

// dirent::d_first
enum class DirState : char {
    IsFirst  = 0,
    NotFirst = 1,
    Closed   = 2
};

constexpr unsigned OPENMODE_ACCESS_MASK   = 0x0007;
constexpr unsigned OPENMODE_SHARE_MASK    = 0x0070;

constexpr unsigned OPENMODE_ACCESS_RDONLY = 0x0000;

constexpr unsigned OPENMODE_DENY_COMPAT   = 0x0000;
constexpr unsigned OPENMODE_DENY_ALL      = 0x0010;
constexpr unsigned OPENMODE_DENY_WRITE    = 0x0020;
constexpr unsigned OPENMODE_DENY_READ     = 0x0030;
constexpr unsigned OPENMODE_DENY_NONE     = 0x0040;

constexpr DWORD NT_FIND_ATTRIBUTES_MASK = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_DIRECTORY;

inline DirState dir_state( const DIR *dirp )
{
    return static_cast<DirState>( dirp->d_first );
}

inline void set_dir_state( DIR *dirp, DirState state )
{
    dirp->d_first = static_cast<char>( state );
}

} // namespace

void __NT_timet_to_filetime( time_t t, FILETIME *ft )
{
    ULARGE_INTEGER  ulint;

    ulint.QuadPart = t * WINDOWS_TICK_PER_SEC + TICK_TO_UNIX_EPOCH;
    ft->dwLowDateTime = ulint.LowPart;
    ft->dwHighDateTime = ulint.HighPart;
}

time_t __NT_filetime_to_timet( const FILETIME *ft )
{
    ULARGE_INTEGER  ulint;

    ulint.LowPart   =   ft->dwLowDateTime;
    ulint.HighPart  =   ft->dwHighDateTime;
    return static_cast<time_t>( ( ulint.QuadPart - TICK_TO_UNIX_EPOCH ) / WINDOWS_TICK_PER_SEC );
}

void __GetNTCreateAttr( unsigned dos_attrib, LPDWORD desired_access, LPDWORD nt_attrib )
{
    if( dos_attrib & _A_RDONLY ) {
        *desired_access = GENERIC_READ;
        *nt_attrib = FILE_ATTRIBUTE_READONLY;
    } else {
        *desired_access = GENERIC_READ | GENERIC_WRITE;
        *nt_attrib = FILE_ATTRIBUTE_NORMAL;
    }
    if( dos_attrib & _A_HIDDEN ) {
        *nt_attrib |= FILE_ATTRIBUTE_HIDDEN;
    }
    if( dos_attrib & _A_SYSTEM ) {
        *nt_attrib |= FILE_ATTRIBUTE_SYSTEM;
    }
}

void __GetNTAccessAttr( unsigned rwmode, LPDWORD desired_access, LPDWORD nt_attrib )
{
    if( rwmode == O_RDWR ) {
        *desired_access = GENERIC_READ | GENERIC_WRITE;
        *nt_attrib = FILE_ATTRIBUTE_NORMAL;
    } else if( rwmode == O_WRONLY ) {
        *desired_access = GENERIC_WRITE;
        *nt_attrib = FILE_ATTRIBUTE_NORMAL;
    } else {
        *desired_access = GENERIC_READ;
        *nt_attrib = FILE_ATTRIBUTE_READONLY;
    }
}

void __GetNTShareAttr( unsigned mode, LPDWORD share_mode )
{
    const unsigned share  = mode & OPENMODE_SHARE_MASK;
    const unsigned rwmode = mode & OPENMODE_ACCESS_MASK;

    switch( share ) {
    case OPENMODE_DENY_COMPAT:
        /*
         * Always allow reopening for read.  Since we don't want the same
         * file opened twice for writing, only allow the file to be opened
         * for writing hereafter if we're opening it now in read-only mode.
         *                      -- M. Hildebrand, 14-jun-96
         */
        *share_mode = FILE_SHARE_READ;  /* can always open again for read */
        if( rwmode == OPENMODE_ACCESS_RDONLY ) {
            *share_mode |= FILE_SHARE_WRITE;    /* can open again for write */
        }
        break;
    case OPENMODE_DENY_ALL:
        *share_mode = 0;
        break;
    case OPENMODE_DENY_READ:
        *share_mode = FILE_SHARE_WRITE;
        break;
    case OPENMODE_DENY_WRITE:
        *share_mode = FILE_SHARE_READ;
        break;
    case OPENMODE_DENY_NONE:
        *share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
        break;
    }
}

void __FromDOSDT( unsigned short d, unsigned short t, FILETIME *NT_stamp )
{
    FILETIME local_ft;

    DosDateTimeToFileTime( d, t, &local_ft );
    LocalFileTimeToFileTime( &local_ft, NT_stamp );
}

void __MakeDOSDT( FILETIME *NT_stamp, unsigned short *d, unsigned short *t )
{
    FILETIME local_ft;

    FileTimeToLocalFileTime( NT_stamp, &local_ft );
    FileTimeToDosDateTime( &local_ft, d, t );
}

bool __NTFindNextFileWithAttr( HANDLE osffh, unsigned nt_attrib, LPWIN32_FIND_DATA ffd )
{
    for( ;; ) {
        if( (nt_attrib | ~ffd->dwFileAttributes) & NT_FIND_ATTRIBUTES_MASK ) {
            return true;
        }
        if( FindNextFile( osffh, ffd ) == 0 ) {
            return false;
        }
    }
}

namespace {

// What a name given to opendir is.
enum class DirKind {
    Wildcard = -1,      // has wildcards: must be a file
    Unknown  = 0,       // without wildcards: maybe a file or a directory, needs a check ("\*.*" to add if a directory)
    Dot      = 1,       // ends in '.': a directory, "\*.*" to add
    Trailing = 2        // ends in a separator or ':': a directory, "*.*" to add
};

DirKind is_directory( std::string_view name )
{
    if( name.find_first_of( "*?" ) != std::string_view::npos )
        return DirKind::Wildcard;
    const unsigned char last = name.empty() ? '\0' : static_cast<unsigned char>( name.back() );
    if( ISPS( last ) || last == ':' )
        return DirKind::Trailing;
    if( last == '.' )
        return DirKind::Dot;
    return DirKind::Unknown;
}

void get_nt_dir_info( dirent *dirp, LPWIN32_FIND_DATA ffd )
{
    DTAXXX_TSTAMP_OF( dirp->d_dta ) = __NT_filetime_to_timet( &ffd->ftLastWriteTime );
    __MakeDOSDT( &ffd->ftLastWriteTime, &dirp->d_date, &dirp->d_time );
    dirp->d_attr = static_cast<char>( NT2DOSATTR( ffd->dwFileAttributes ) );
    dirp->d_size = static_cast<long>( ffd->nFileSizeLow );
    std::strncpy( dirp->d_name, ffd->cFileName, NAME_MAX );
    dirp->d_name[NAME_MAX] = 0;
}

DIR *open_dir( const char *dirname, DIR *dirp )
{
    WIN32_FIND_DATA     ffd;

    if( dir_state( dirp ) != DirState::Closed ) {
        FindClose( DTAXXX_HANDLE_OF( dirp->d_dta ) );
        set_dir_state( dirp, DirState::Closed );
    }
    HANDLE osffh = FindFirstFile( dirname, &ffd );
    if( osffh == INVALID_HANDLE_VALUE ) {
        errno = ENOENT;
        return nullptr;
    }
    DTAXXX_HANDLE_OF( dirp->d_dta ) = osffh;
    get_nt_dir_info( dirp, &ffd );
    set_dir_state( dirp, DirState::IsFirst );
    return dirp;
}

} // namespace

DIR *opendir( const char *dirname )
{
    DIR         tmp{};
    std::string pathname;

    tmp.d_attr = _A_SUBDIR;
    set_dir_state( &tmp, DirState::Closed );
    const DirKind kind = is_directory( dirname );
    if( kind == DirKind::Wildcard || kind == DirKind::Unknown ) {
        if( open_dir( dirname, &tmp ) == nullptr ) {
            return nullptr;
        }
    }
    if( kind != DirKind::Wildcard
      && (tmp.d_attr & _A_SUBDIR) ) {
        /* directory, add wildcard */
        pathname = dirname;
        if( kind != DirKind::Trailing ) {
            pathname += PC;
        }
        pathname += "*.*";
        if( open_dir( pathname.c_str(), &tmp ) == nullptr ) {
            return nullptr;
        }
        dirname = pathname.c_str();
    }
    // the directory handle is handed to the caller, who frees it with closedir: malloc as before
    DIR *dirp = static_cast<DIR *>( std::malloc( sizeof( *dirp ) ) );
    if( dirp == nullptr ) {
        FindClose( DTAXXX_HANDLE_OF( tmp.d_dta ) );
        errno = ENOMEM;
        return nullptr;
    }
    tmp.d_openpath = strdup( dirname );
    *dirp = tmp;
    return dirp;
}

dirent *readdir( DIR *dirp )
{
    WIN32_FIND_DATA     ffd;

    if( dirp == nullptr
      || dir_state( dirp ) == DirState::Closed )
        return nullptr;
    if( dir_state( dirp ) == DirState::IsFirst ) {
        set_dir_state( dirp, DirState::NotFirst );
    } else {
        if( FindNextFileA( DTAXXX_HANDLE_OF( dirp->d_dta ), &ffd ) == 0 ) {
            errno = ENOENT;
            return nullptr;
        }
        get_nt_dir_info( dirp, &ffd );
    }
    return dirp;
}

int closedir( DIR *dirp )
{
    if( dirp == nullptr
      || dir_state( dirp ) == DirState::Closed ) {
        errno = ERANGE;
        return -1;
    }
    if( FindClose( DTAXXX_HANDLE_OF( dirp->d_dta ) ) == 0 ) {
        return -1;
    }
    set_dir_state( dirp, DirState::Closed );
    if( dirp->d_openpath != nullptr )
        std::free( dirp->d_openpath );
    std::free( dirp );
    return 0;
}

/****************************************************************************
*
* Description:  Implementation of POSIX getopt
*
****************************************************************************/

char        *optarg;            /* pointer to option argument */
int         optind = 1;         /* current argv[] index */
int         optopt;             /* currently processed chracter */
int         opterr = 1;         /* error output control flag */

/* Error messages suggested by Single UNIX Specification */
#define NO_ARG_MSG      "%s: option requires an argument -- %c\n"
#define BAD_OPT_MSG     "%s: illegal option -- %c\n"

int getopt( int argc, char * const argv[], const char *optstring )
{
    FileState   &st = cb_wlink::RunState::part<FileState>();

    (void)argc;
    optarg = nullptr;
    char *curr_arg = argv[optind];
    if( curr_arg == nullptr ) {
        return -1;
    }
    for( ;; ) {
        optopt = curr_arg[st.opt_offset];
        if( std::isspace( optopt ) ) {
            st.opt_offset++;
            continue;
        }
        break;
    }
    if( st.opt_offset > 1
      || optopt == '-'
      || optopt == st.altoptchar ) {
        if( st.opt_offset > 1 ) {
            optopt = curr_arg[st.opt_offset];
            if( optopt == '-'
              || optopt == st.altoptchar ) {
                st.optchar = static_cast<char>( optopt );
                st.opt_offset++;
                optopt = curr_arg[st.opt_offset];
            }
        } else {
            st.optchar = static_cast<char>( optopt );
            st.opt_offset++;
            optopt = curr_arg[st.opt_offset];
        }
        if( optopt == '\0' ) {  /* option char by itself should be */
            return -1;          /* left alone */
        }
        if( optopt == '-'
          && curr_arg[st.opt_offset + 1] == '\0' ) {
            st.opt_offset = 0;
            ++optind;
            return -1;      /* "--" POSIX end of options delimiter */
        }
        const char *ptr = std::strchr( optstring, optopt );
        if( ptr == nullptr ) {
            if( opterr
              && *optstring != ':' ) {
                std::fprintf( stderr, BAD_OPT_MSG, argv[0], optopt );
            }
            return '?';     /* unrecognized option */
        }
        if( *(ptr + 1) == ':' ) {   /* check if option requires argument */
            if( curr_arg[st.opt_offset + 1] == '\0' ) {
                if( argv[optind + 1] == nullptr ) {
                    if( *optstring == ':' ) {
                        return ':';
                    } else {
                        if( opterr ) {
                            std::fprintf( stderr, NO_ARG_MSG, argv[0], optopt );
                        }
                        return '?';
                    }
                }
                optarg = argv[optind + 1];
                ++optind;
            } else {
                optarg = &curr_arg[st.opt_offset + 1];
            }
            st.opt_offset = 0;
            ++optind;
        } else {
            st.opt_offset++;
            if( curr_arg[st.opt_offset] == '\0' ) {    /* last char in argv element */
                st.opt_offset = 0;
                ++optind;
            }
        }
        return optopt;      /* return recognized option char */
    } else {
        return -1;          /* no more options */
    }
}

/****************************************************************************
*
* Description:  Implementation of POSIX mkstemp
*
****************************************************************************/

namespace {

// where the "XXXXXX" ends the template, or nullptr if it does not
char *template_xs( char *template_str )
{
    /*** Ensure the last 6 characters form the string "XXXXXX" ***/
    const std::size_t len = std::strlen( template_str );
    if( len < 6 ) {
        return nullptr;     /* need room for six exes */
    }
    char *p = template_str + len - 6;
    if( std::strcmp( p, "XXXXXX" ) ) {
        return nullptr;
    }
    return p;
}

} // namespace

int mkstemp( char *template_str )
{
    /*** Ensure the template is valid ***/
    char *xs = template_xs( template_str );
    if( xs == nullptr ) {
        return -1;
    }

    /*** Get the process/thread ID ***/
    unsigned pid = GetCurrentProcessId();
    pid %= 100000;      /* first few digits could be repeated */

    /*** Try to build a unique filename ***/
    for( char letter = 'a'; letter <= 'z'; letter++ ) {
        std::snprintf( xs, 7, "%c%05u", letter, pid );
        if( access( template_str, F_OK ) != 0 ) {
            const int fh = open( template_str, O_RDWR | O_CREAT | O_TRUNC | O_EXCL | O_BINARY, S_IREAD | S_IWRITE );
            if( fh != -1 ) {
                return fh;          /* file successfully created */
            }
            /* EEXIST may occur in case of a race condition or if we simply
             * created that temp file earlier, and we'll try again. If however
             * the creation failed for some other reason, it will almost
             * certainly fail again no matter how many times we try. So don't.
             */
            if( errno != EEXIST ) {
                return -1;
            }
        }
    }
    return -1;
}

/****************************************************************************
*
* Description:  Implementation of sleep function
*
****************************************************************************/

unsigned sleep( unsigned time )
{
    Sleep( time * 1000UL );
    return 0;
}

#endif /* _MSC_VER */

/****************************************************************************
*
* Description:  Determine resource language from system environment.
*
****************************************************************************/

wres_lang_id _WResLanguage( void )
{
    return static_cast<wres_lang_id>( RLE_ENGLISH );
}

/****************************************************************************
*
* Description:  Get DLL file name to load resources.
*
****************************************************************************/

char *get_dllname( char *buf, int len )
{
    *buf = '\0';
#ifdef _MSC_VER
    HMODULE hnd = nullptr;
    GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCTSTR>( &optind ), &hnd );
    GetModuleFileName( hnd, buf, len );
#else
    (void)len;
#endif
    return buf;
}
