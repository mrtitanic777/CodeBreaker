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
* Description:  Declaration of non-standard functions and macros used by bootstrap
*
****************************************************************************/


#pragma once
    /*
     * clibext.h:
     * This file contains defines and prototypes of functions that are present
     * in Watcom's CLIB but not in many other C libraries
     */
    #include <climits>
    #include <cstdlib>
    #include <cstdio>
    #include <cstdarg>
    #include <sys/types.h>  /* for off_t */

    #define _WCNORETURN
    #define __near
    #define __based(x)

 #if defined( _MSC_VER )
    #define S_ISDIR(x) (((x)&_S_IFMT)==_S_IFDIR)
    #define S_ISREG(x) (((x)&_S_IFMT)==_S_IFREG)
    #define S_ISCHR(x) (((x)&_S_IFMT)==_S_IFCHR)
    #define FNM_PATHNAME    0x02
    #define NAME_MAX FILENAME_MAX
    #define PATH_MAX FILENAME_MAX
 #endif

    #define _MAX_PATH2 (_MAX_PATH + 3)

    #define __Strtold(s,ld,endptr) ((*(double *)(ld))=strtod(s,endptr))

// #ifndef __cplusplus
//  #ifndef max
//    #define max(x,y) (((x)>(y))?(x):(y))
//  #endif
//  #ifndef min
//    #define min(x,y) (((x)<(y))?(x):(y))
//  #endif
// #endif

    extern "C" {

 #if defined( _MSC_VER )

    typedef struct find_t {
        char            reserved[21];       /* reserved for use by DOS    */
        char            attrib;             /* attribute byte for file    */
        unsigned short  wr_time;            /* time of last write to file */
        unsigned short  wr_date;            /* date of last write to file */
        unsigned long   size;               /* length of file in bytes    */
        char            name[NAME_MAX+1];   /* null-terminated filename   */
    } find_t;
    typedef struct dirent {
        char            d_dta[21];          /* disk transfer area */
        char            d_attr;             /* file's attribute */
        unsigned short  d_time;             /* file's time */
        unsigned short  d_date;             /* file's date */
        long            d_size;             /* file's size */
        unsigned short  d_ino;              /* serial number (not used) */
        char            d_first;            /* flag for 1st time */
        char            *d_openpath;        /* path specified to opendir */
        char            d_name[NAME_MAX+1]; /* file's name */
    } dirent;
    typedef struct dirent   DIR;

    typedef __int64 ssize_t;
    typedef int     mode_t;

 #endif

    #include "_pathgr2.h"

 #if defined( _MSC_VER )

    extern int      setenv( SupportRun& support, const char *name, const char *newvalue, int overwrite );

    extern DIR              *opendir( SupportRun& support, const char *dirname );
    extern struct dirent    *readdir( DIR *dirp );
    extern int              closedir( SupportRun& support, DIR *dirp );

    /* Globals used and set by getopt() */


 #endif

    extern void     _splitpath2( const char *inp, char *outp, char **drive, char **dir, char **fn, char **ext );
    extern int      _bgetcmd( SupportRun& support, char *buffer, int len );
    extern char     *_cmdname( SupportRun& support, char *name );
    extern char     *get_dllname( SupportRun& support, char *buf, int len );

}


#ifndef CB_STATE_CLIBEXT_H
#define CB_STATE_CLIBEXT_H

/* the variables of clibext.h, one set for each run of the compiler (run/cbrun.hpp) */
struct ClibextState {
#if defined(_MSC_VER)
    int optind = 1;   /* current argv[] index */
#endif
};

#endif

/*
 * this enable to use new OW 2.0 ...dir() functions
 * with OW 1.9 compiler
 * OW 2.0 uses new dirent structure
 */
    #define DIRENTXX        dirent
    #define DIRXX           DIR
    #define OPENDIRXX       opendir
    #define READDIRXX       readdir
    #define CLOSEDIRXX      closedir

