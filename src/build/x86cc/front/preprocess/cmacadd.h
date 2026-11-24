/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2022 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Macro support routines.
*
****************************************************************************/


#ifndef _CMACADD_H
#define _CMACADD_H

extern void         *PermMemAlloc( FrontRun& front, size_t amount );
extern void         InitMacroSegments( FrontRun& front );
extern void         FreeMacroSegments( FrontRun& front );
extern MEPTR        CreateMEntry( FrontRun& front, const char *, size_t len );
extern MEPTR        MacroDefine( FrontRun& front, size_t len, macro_flags mflags );
extern bool         MacroCompare( MEPTR, MEPTR );
extern MEPTR        MacroLookup( FrontRun& front, const char * );

void MacroSegmentAddChar(       // MacroSegment: ADD A CHARACTER
    FrontRun& front, size_t *mlen,               // - data length
    char chr )                  // - character to be added
;
void MacroSegmentAddToken(      // MacroSegment: ADD A TOKEN
    FrontRun& front, size_t *mlen,               // - data length
    TOKEN token )               // - token to be added
;
void MacroSegmentAddMem(        // MacroSegment: ADD A SEQUENCE OF BYTES
    FrontRun& front, size_t *mlen,               // - data length
    const char *buff,           // - bytes to be added
    size_t len )                // - number of bytes
;
#endif
