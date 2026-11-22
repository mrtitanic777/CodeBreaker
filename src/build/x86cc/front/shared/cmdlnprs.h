/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023-2024 The Open Watcom Contributors. All Rights Reserved.
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


#ifndef __CMDLNPRS_H__
#define __CMDLNPRS_H__

#ifdef __cplusplus
extern "C" {
#endif

// Standard Command-Line Parsing Routines

void OPT_CLEAN_NUMBER           // CLEAN UP NUMBERS
    ( FrontRun& front, OPT_NUMBER **h )          // - list
;
void OPT_CLEAN_STRING           // CLEAN UP STRINGS
    ( FrontRun& front, OPT_STRING **h )          // - list
;
bool OPT_END( FrontRun& front )            // DETECT END OF CHAIN
;
bool OPT_GET_ID                 // PARSE: ID
    ( FrontRun& front, OPT_STRING **p )          // - target
;
bool OPT_GET_ID_OPT             // PARSE: OPTIONAL ID
    ( FrontRun& front, OPT_STRING **p )          // - target
;
int OPT_GET_LOWER               // GET CHAR IN LOWERCASE
    ( FrontRun& front )
;
bool OPT_GET_NUMBER             // PARSE: #
    ( FrontRun& front, unsigned *p )             // - target
;
bool OPT_GET_NUMBER_DEFAULT(    // PARSE: OPTIONAL # WITH A DEFAULT VALUE
    FrontRun& front, unsigned *p,                // - target
    unsigned default_value )    // - default value
;
bool OPT_GET_NUMBER_MULTIPLE    // PARSE: OPTION #
    ( FrontRun& front, OPT_NUMBER **h )          // - target
;
bool OPT_GET_PATH               // PARSE: PATH
    ( FrontRun& front, OPT_STRING **p )          // - target
;
bool OPT_GET_PATH_OPT           // PARSE: OPTIONAL PATH
    ( FrontRun& front, OPT_STRING **p )          // - target
;
bool OPT_GET_FILE               // PARSE: FILE
    ( FrontRun& front, OPT_STRING **p )          // - target
;
bool OPT_GET_FILE_OPT           // PARSE: OPTIONAL FILE
    ( FrontRun& front, OPT_STRING **p )          // - target
;
bool OPT_GET_CHAR               // PARSE: CHAR
    ( FrontRun& front, int *c )                  // - target
;
bool OPT_GET_CHAR_OPT           // PARSE: OPTIONAL CHAR
    ( FrontRun& front, int *c )                  // - target
;
bool OPT_GET_OPTION             // PARSE: OPTION TEXT
    ( FrontRun& front, OPT_STRING **p )          // - target
;
bool OPT_RECOG                  // RECOGNIZE CHAR
    ( FrontRun& front, int c )                   // - to be recog'ed
;
bool OPT_RERECOG                // RE-RECOGNIZE LAST CHAR
    ( FrontRun& front, int c )                   // - to be recog'ed
;
bool OPT_RECOG_LOWER            // RECOGNIZE LOWERCASE CHAR
    ( FrontRun& front, int c )                   // - to be recog'ed
;
void OPT_UNGET                  // UNGET A CHARACTER
    ( FrontRun& front )
;

// The following are required to be supplied by the front end

void BadCmdLineChar             // BAD CHAR DETECTED
    ( FrontRun& front )
;
void BadCmdLineId               // BAD ID DETECTED
    ( FrontRun& front )
;
void BadCmdLineNumber           // BAD NUMBER DETECTED
    ( FrontRun& front )
;
void BadCmdLinePath             // BAD PATH DETECTED
    ( FrontRun& front )
;
void BadCmdLineFile             // BAD FILE DETECTED
    ( FrontRun& front )
;
void BadCmdLineOption           // BAD TEXT DETECTED
    ( FrontRun& front )
;

#ifdef __cplusplus
};
#endif

#endif // __CMDLNPRS_H__
