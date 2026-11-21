/****************************************************************************
*
*                            Open Watcom Project
*
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
* Description:  Watcom style debugging information procedure prototypes
*
****************************************************************************/

#pragma once   /* CodeBreaker: each header once (the directive layer includes them as C++ headers) */


extern void     ODBIInit( section * );
extern void     ODBIInitModule(Link& link,  mod_entry * );
extern void     ODBIP1Source(Link& link,  byte, byte, const char *, size_t );
extern void     ODBIP1ModuleScanned(Link& link);
extern void     ODBIP1ModuleFinished( mod_entry * );
extern void     ODBIAddModule( mod_entry *, section * );
extern void     ODBIGenModule(Link& link);
extern void     ODBIAddFakeModule( unsigned );
extern void     ODBIGenFakeModule( char * );
extern void     ODBIDefClass(Link& link,  class_entry *, unsigned_32 );
extern void     ODBIAddLocal(Link& link,  seg_leader *, offset );
extern void     ODBIGenLocal(Link& link,  segdata * );
extern void     ODBIAddGlobal(Link& link,  symbol * );
extern void     ODBIGenGlobal(Link& link,  symbol *, section * );
extern void     ODBIGenLines(Link& link,  lineinfo * );
extern void     ODBIAddAddrInfo( seg_leader * );
extern void     ODBIAddrSectStart(Link& link,  section * );
extern void     ODBIP2Start(Link& link,  section * );
extern void     ODBISectCleanup( section * );
extern void     ODBIFini(Link& link,  section * );
extern void     ODBIWrite(Link& link);
