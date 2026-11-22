/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023-2026 The Open Watcom Contributors. All Rights Reserved.
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


#include <ctype.h>
#include <string.h>
#include "bool.h"
#include "cmdscan.h"
#include "cmdlnprs.gh"          // required generated file
#include "cmdlnprs.h"
#include "memfuncs.h"
#include "frontrun.hpp"


static void addString           // STORE A STRING
    ( FrontRun& front, OPT_STRING **h            // - addr[ storage ]
    , char const *s             // - string
    , size_t len )              // - length
{
    OPT_STRING *value;

    value = static_cast<opt_string *>(MemAllocSafe( front, sizeof( *value ) + len ));
    strncpy( value->data, s, len );
    value->data[len] = '\0';
    value->next = *h;
    *h = value;
}


static void addNumber           // STORE A NUMBER
    ( FrontRun& front, OPT_NUMBER **h            // - addr[ storage ]
    , unsigned number )         // - number
{
    OPT_NUMBER *value;

    value = static_cast<opt_number *>(MemAllocSafe( front, sizeof( *value ) ));
    value->number = number;
    value->next = *h;
    *h = value;
}


static bool scanOffNumber       // SCAN A NUMBER
    ( FrontRun& front, unsigned *pvalue )        // - target
{
    bool number_scanned;
    unsigned value;
    int c;

    CmdRecogEquals(front);
    number_scanned = false;
    value = 0;
    for(;;) {
        c = CmdScanLowerChar(front);
        if( !isdigit( c ) ) {
            CmdScanUngetChar(front);
            break;
        }
        value *= 10;
        value += c - '0';
        number_scanned = true;
    }
    if( number_scanned ) {
        *pvalue = value;
    }
    return( number_scanned );
}


static void UnEscape        // REMOVE QUOTES ESCAPING FROM A STRING
    ( char *s )             // - the string
{
    char *d;

    // string will shrink so we can reduce in place
    for( d = s; *s != '\0'; ++s ) {
        // collapse double backslashes, only then look for escaped quotes
        if( s[0] == '\\' ) {
            if( s[1] == '\\' || s[1] == '"' ) {
                ++s;
            }
        }
        *d++ = *s;
    }
    *d = '\0';
}


void OPT_CLEAN_NUMBER           // CLEAN UP NUMBERS
    ( FrontRun& front, OPT_NUMBER **h )          // - list
{
    OPT_NUMBER *s;

    while( (s = *h) != NULL ) {
        *h = s->next;
        MemFree( front, s );
    }
}


void OPT_CLEAN_STRING           // CLEAN UP STRINGS
    ( FrontRun& front, OPT_STRING **h )          // - list
{
    OPT_STRING *s;

    while( (s = *h) != NULL ) {
        *h = s->next;
        MemFree( front, s );
    }
}


bool OPT_GET_ID                 // PARSE: ID
    ( FrontRun& front, OPT_STRING **p )          // - target
{
    size_t len;
    char const *id;

    CmdRecogEquals(front);
    len = CmdScanId( front, &id );
    if( len > 0 ) {
        addString( front, p, id, len );
        return( true );
    }
    BadCmdLineId(front);
    return( false );
}

bool OPT_GET_ID_OPT             // PARSE: OPTIONAL ID
    ( FrontRun& front, OPT_STRING **p )          // - target
{
    if( CmdRecogEquals(front) || !CmdScanSwEnd(front) ) {
        return( OPT_GET_ID( front, p ) );
    }
    return( true );
}


int OPT_GET_LOWER               // GET CHAR IN LOWERCASE
    ( FrontRun& front )
{
    return( CmdScanLowerChar(front) );
}


bool OPT_GET_NUMBER             // PARSE: #
    ( FrontRun& front, unsigned *p )             // - target
{
    unsigned value;

    if( scanOffNumber( front, &value ) ) {
        *p = value;
        return( true );
    }
    BadCmdLineNumber(front);
    return( false );
}


bool OPT_GET_NUMBER_MULTIPLE    // PARSE: OPTION #
    ( FrontRun& front, OPT_NUMBER **h )          // - target
{
    unsigned value;

    if( scanOffNumber( front, &value ) ) {
        addNumber( front, h, value );
        return( true );
    }
    BadCmdLineNumber(front);
    return( false );
}

bool OPT_GET_NUMBER_DEFAULT
    ( FrontRun& front, unsigned *p, unsigned default_value )
{
    unsigned value;

    if( scanOffNumber( front, &value ) ) {
        *p = value;
    } else {
        *p = default_value;
    }
    return( true );
}


bool OPT_GET_FILE               // PARSE: FILE NAME
    ( FrontRun& front, OPT_STRING **p )          // - target
{
    size_t len;
    char const *fname;
    bool quoted;

    CmdRecogEquals(front);
    len = CmdScanFilename( front, &fname, &quoted );
    if( len > 0 ) {
        addString( front, p, fname, len );
        if( quoted ) {
            UnEscape( (*p)->data );
        }
        return( true );
    }
    BadCmdLineFile(front);
    return( false );
}

bool OPT_GET_FILE_OPT           // PARSE: OPTIONAL FILE NAME
    ( FrontRun& front, OPT_STRING **p )          // - target
{
    size_t len;
    char const *fname;
    bool quoted;

    // handle leading option char specially
    if( CmdRecogEquals(front) || !CmdScanSwEnd(front) ) {
        // specified an '=' so accept -this-is-a-file-name.fil or /tmp/ack.tmp
        len = CmdScanFilename( front, &fname, &quoted );
        if( len > 0 ) {
            addString( front, p, fname, len );
            if( quoted ) {
                UnEscape( (*p)->data );
            }
        } else {
            OPT_CLEAN_STRING( front, p );
        }
    }
    return( true );
}


bool OPT_GET_PATH               // PARSE: PATH
    ( FrontRun& front, OPT_STRING **p )          // - target
{
    size_t len;
    char const *path;
    bool quoted;

//    CmdPathDelim();
    CmdRecogEquals(front);
    len = CmdScanFilename( front, &path, &quoted );
    if( len > 0 ) {
        addString( front, p, path, len );
        if( quoted ) {
            UnEscape( (*p)->data );
        }
        return( true );
    }
    BadCmdLinePath(front);
    return( false );
}

bool OPT_GET_PATH_OPT           // PARSE: OPTIONAL PATH
    ( FrontRun& front, OPT_STRING **p )          // - target
{
    size_t len;
    char const *fname;
    bool quoted;

//    if( CmdPathDelim() || !CmdScanSwEnd() ) {
    if( CmdRecogEquals(front) || !CmdScanSwEnd(front) ) {
        // specified an '=' so accept -this-is-a-path-name.fil or /tmp/ack.tmp
        len = CmdScanFilename( front, &fname, &quoted );
        if( len > 0 ) {
            addString( front, p, fname, len );
            if( quoted ) {
                UnEscape( (*p)->data );
            }
        } else {
            OPT_CLEAN_STRING( front, p );
        }
    }
    return( true );
}


bool OPT_GET_OPTION             // PARSE: OPTION TEXT
    ( FrontRun& front, OPT_STRING **p )          // - target
{
    size_t len;
    char const *option;
    bool quoted;

    CmdRecogEquals(front);
    len = CmdScanOption( front, &option, &quoted );
    if( len > 0 ) {
        addString( front, p, option, len );
        if( quoted ) {
            UnEscape( (*p)->data );
        }
        return( true );
    }
    BadCmdLineOption(front);
    return( false );
}


bool OPT_GET_CHAR               // PARSE: CHAR
    ( FrontRun& front, int *p )                  // - target
{
    int c;

    if( !CmdScanSwEnd(front) ) {
        CmdRecogEquals(front);
        if( !CmdScanSwEnd(front) ) {
            c = CmdScanChar(front);
            if( isprint( c ) ) {
                *p = c;
                return( true );
            }
        }
    }
    BadCmdLineChar(front);
    return( false );
}

bool OPT_GET_CHAR_OPT           // PARSE: OPTIONAL CHAR
    ( FrontRun& front, int *p )                  // - target
{
    if( CmdRecogEquals(front) || !CmdScanSwEnd(front) ) {
        return OPT_GET_CHAR( front, p );
    }
    return( true );
}


bool OPT_RECOG                  // RECOGNIZE CHAR
    ( FrontRun& front, int c )                   // - to be recog'ed
{
    return( CmdRecogChar( front, c ) );
}

bool OPT_RERECOG                // RE-RECOGNIZE LAST CHAR
    ( FrontRun& front, int c )                   // - to be recog'ed
{
    return( CmdReRecogChar( front, c ) );
}

bool OPT_RECOG_LOWER            // RECOGNIZE LOWERCASE CHAR
    ( FrontRun& front, int c )                   // - to be recog'ed
{
    return( CmdRecogLowerChar( front, c ) );
}

void OPT_UNGET                  // UNGET A CHARACTER
    ( FrontRun& front )
{
    CmdScanUngetChar(front);
}

bool OPT_END( FrontRun& front )            // DETECT END OF CHAIN
{
    return( CmdScanSwEnd(front) );
}
