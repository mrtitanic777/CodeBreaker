/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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

#pragma once   /* CodeBreaker: each header once (the directive layer includes them as C++ headers) */


#define WriteMapLnkMsg(m,...)   LnkMsg( MAP + m, __VA_ARGS__ )

extern void     ResetWriteMapIO(Link& link);
extern void     MapInit(Link& link);
extern void     MapFini(Link& link);
extern void     StartTime( void );
extern void     EndTime( void );
extern void     RecordUndefinedSym(Link& link,  symbol * );
extern void     PrintUndefinedSyms(Link& link);
extern void     WriteMapHead(Link& link,  section * );
extern void     WriteMapOvlPubHead(Link& link,  section *sec );
extern void     WriteMapOvlVectHead(Link& link,  vect_state *VectState );
extern void     WriteMapModulesSegments(Link& link);
extern void     WriteMapLibsUsed(Link& link);
extern void     WriteMapLnkMsgCallback(Link& link,  unsigned msgnum, const char *str, size_t len );
extern void     WriteMapTracedSymRecord(Link& link,  symbol * );
extern void     WriteMapTracedSyms(Link& link);
extern void     WriteMapPubStart( void );
extern void     WriteMapPubEnd(Link& link);
extern void     WriteMapPubSortStart(Link& link,  pubdefinfo *info );
extern void     WriteMapPubSortEnd(Link& link,  pubdefinfo *info );
extern void     WriteMapPubEntry(Link& link,  pubdefinfo *info, symbol *sym );
