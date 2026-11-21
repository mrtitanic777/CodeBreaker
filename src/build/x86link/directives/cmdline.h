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
* Description:  Prototypes for command parser functions
*
****************************************************************************/

#pragma once   /* CodeBreaker: each header once (the directive layer includes them as C++ headers) */


extern void             InitCmdFile( void );
extern void             SetSegMask(Link& link);
extern char             *GetNextLink( void );
extern void             DoCmdFile(Link& link,  const char * );
extern void             FreePaths(Link& link);
extern void             Burn(Link& link);
extern void             SetFormat(Link& link);
extern void             AddFmtLibPaths(Link& link);
extern bool             HintFormat(Link& link,  exe_format );
extern void             DecideFormat(Link& link);
extern void             FreeFormatStuff(Link& link);
extern void             AddCommentLib(Link& link,  const char *, size_t, lib_priority );
extern void             ExecSystem(Link& link,  const char * );
extern void             PruneSystemList( void );
extern void             BurnSystemList( void );
extern void             AddLibPaths(Link& link,  const char *, size_t, bool );
extern void             AddLibPathsToEnd(Link& link,  const char * );
extern void             AddLibPathsToEndList(Link& link,  const char *list );
extern int              DoBuffCmdParse(Link& link,  const char *cmd );
