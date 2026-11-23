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
* Description:  Command line arguments scanning routines.
*
****************************************************************************/


#include <ctype.h>
#include <stdlib.h>
#include "bool.h"
#include "cmdscan.h"
#include "frontrun.hpp"




namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CmdscanState: cmdscan_state.h) */

}  // namespace



char const *CmdScanLineInit(    // INITIALIZE FOR COMMAND SCANNING
    FrontRun& front, char const *cmd_line )      // - new command line
{                               // RETURN OLD COMMAND-LINE SCAN ADDRESS
    char const *old;

    old = front.cmdscan.cmd.curr_ptr;
    front.cmdscan.cmd.curr_ptr = cmd_line;
    front.cmdscan.cmd.switch_ptr = NULL;
    front.cmdscan.cmd.unix_mode = false;
    return( old );
}


char const *CmdScanAddr(        // RETURN COMMAND-LINE SCAN ADDRESS
    FrontRun& front )
{
    return( front.cmdscan.cmd.curr_ptr );
}


int CmdScanChar(                // SCAN THE NEXT CHARACTER
    FrontRun& front )
{
    return( *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr++) );
}


bool CmdScanBufferEnd(          // TEST IF END OF BUFFER
    FrontRun& front )
{
    return( *front.cmdscan.cmd.curr_ptr == '\0' );
}


bool CmdScanSwEnd(              // TEST IF END OF SWITCH
    FrontRun& front )
{
    int c;                     // - current character

    c = *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr);
    if( c == '\0' ) {
        return( true );
    }
    if( isspace( c ) ) {
        return( true );
    }
    if( CmdScanSwitchChar( c ) ) {
        return( true );
    }
    return( false );
}


int CmdScanLowerChar(           // SCAN THE NEXT CHARACTER, IN LOWER CASE
    FrontRun& front )
{
    int c;                     // - character scanned

    c = CmdScanChar(front);
    c = tolower( c );
    return( c );
}


int CmdPeekChar(                // PEEK AT NEXT CHARACTER, IN LOWER CASE
    FrontRun& front )
{
    int c;                     // - character scanned

    c = CmdScanLowerChar(front);
    CmdScanUngetChar(front);
    return( c );
}


bool CmdRecogLowerChar(         // RECOGNIZE A LOWER CASE CHARACTER
    FrontRun& front, int recog )                 // - character to be recognized
{
    bool retn;                  // - true ==> got it

    if( recog == CmdScanLowerChar(front) ) {
        retn = true;
    } else {
        CmdScanUngetChar(front);
        retn = false;
    }
    return( retn );
}


bool CmdRecogChar(              // RECOGNIZE A CHARACTER
    FrontRun& front, int recog )                 // - character to be recognized
{
    bool retn;                  // - true ==> got it

    if( recog == CmdScanChar(front) ) {
        retn = true;
    } else {
        CmdScanUngetChar(front);
        retn = false;
    }
    return( retn );
}


bool CmdReRecogChar(            // RE-RECOGNIZE LAST CHARACTER
    FrontRun& front, int recog )                 // - character to be recognized
{
    bool retn;                  // - true ==> got it

    CmdScanUngetChar(front);
    if( recog == CmdScanChar(front) ) {
        retn = true;
    } else {
        retn = false;
    }
    return( retn );
}


bool CmdRecogEquals(            // SKIP EQUALCHAR IN COMMAND LINE
    FrontRun& front )
{
    switch( CmdPeekChar(front) ) {
    case '=':
    case '#':
        CmdScanChar(front);
        return( true );
        break;
    }
    return( false );
}

bool CmdPathDelim(              // SKIP EQUALCHAR # or ' ' IN COMMAND LINE
    FrontRun& front )
{
    switch( CmdPeekChar(front) ) {
    case ' ':
        CmdScanSkipWhiteSpace(front);
        return( true );
    case '=':
    case '#':
        CmdScanChar(front);
        return( true );
    }
    return( false );
}

void CmdScanSwitchBegin(        // REMEMBER START OF SWITCH
    FrontRun& front )
{
    front.cmdscan.cmd.switch_ptr = front.cmdscan.cmd.curr_ptr - 1;
}




size_t CmdScanQuotedString(     // SCAN AN OPTION
    FrontRun& front, char const **option )       // - addr( option pointer )
{
    char const *str_beg;        // - start of string
    int c;
    int quote;

    quote = *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr++);
    *option = str_beg = front.cmdscan.cmd.curr_ptr;
    while( (c = *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr)) != '\0' ) {
        front.cmdscan.cmd.curr_ptr++;
        if( c == quote ) {
            str_beg++;
            break;
        }
        // '"\\"' means '\', not '\"'
        if( c == '\\' ) {
            c = *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr);
            if( c == '\\' || c == '"' ) {
                front.cmdscan.cmd.curr_ptr++;
            }
        }
    }
    return( front.cmdscan.cmd.curr_ptr - str_beg );
}

size_t CmdScanOption(           // SCAN AN OPTION
    FrontRun& front, char const **option,        // - addr( option pointer )
    bool *quoted )              // - addr( quoted )
{
    char const *str_beg;        // - start of string

    if( *front.cmdscan.cmd.curr_ptr == '"' ) {
        *quoted = true;
        return( CmdScanQuotedString( front, option ) );
    }
    *option = str_beg = front.cmdscan.cmd.curr_ptr;
    while( !CmdScanSwEnd(front) ) {
        front.cmdscan.cmd.curr_ptr++;
    }
    *quoted = false;
    return( front.cmdscan.cmd.curr_ptr - str_beg );
}

char const *CmdScanUngetChar(   // UNGET THE LAST CMD SCAN CHARACTER
    FrontRun& front )
{
    return( --front.cmdscan.cmd.curr_ptr );
}


size_t CmdScanNumber(           // SCAN A NUMBER
    FrontRun& front, unsigned *pvalue )          // - addr( return value )
{
    char const *str_beg;        // - start of string
    unsigned value;             // - base 10 value

    value = 0;
    str_beg = front.cmdscan.cmd.curr_ptr;
    for( ; isdigit( *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr) ); front.cmdscan.cmd.curr_ptr++ ) {
        value *= 10;
        value += *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr) - '0';
    }
    *pvalue = value;
    return( front.cmdscan.cmd.curr_ptr - str_beg );
}


size_t CmdScanId(               // SCAN AN IDENTIFIER
    FrontRun& front, char const **option )       // - addr( option pointer )
{
    char const *str_beg;        // - start of string

    *option = str_beg = front.cmdscan.cmd.curr_ptr;
    while( isalnum( *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr) ) || *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr) == '_' ) {
        front.cmdscan.cmd.curr_ptr++;
    }
    return( front.cmdscan.cmd.curr_ptr - str_beg );
}


size_t CmdScanFilename(         // SCAN A FILE NAME
    FrontRun& front, char const **option,        // - addr( option pointer )
    bool *quoted )              // - addr( quoted )
{
    char const *str_beg;        // - start of string
    int c;

    *option = str_beg = front.cmdscan.cmd.curr_ptr;
    if( front.cmdscan.cmd.unix_mode ) {
        while( *front.cmdscan.cmd.curr_ptr != '\0' ) {
            front.cmdscan.cmd.curr_ptr++;
        }
    } else if( *front.cmdscan.cmd.curr_ptr == '"' ) {
        *quoted = true;
        return( CmdScanQuotedString( front, option ) );
    } else {
        for( ; (c = *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr)) != '\0'; front.cmdscan.cmd.curr_ptr++ ) {
            if( isspace( c ) ) {
                break;
            }
        }
    }
    *quoted = false;
    return( front.cmdscan.cmd.curr_ptr - str_beg );
}


void CmdScanSkipWhiteSpace(     // SKIP OVER WHITE SPACES
    FrontRun& front )
{
    while( isspace( *reinterpret_cast<const unsigned char *>(front.cmdscan.cmd.curr_ptr) ) ) {
        front.cmdscan.cmd.curr_ptr++;
    }
}

bool CmdScanSwitchChar( int c )
{
    return( c == '-' || c == '/' );
}
