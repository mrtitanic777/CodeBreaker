/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Prototypes and structures for load file routines.
*
****************************************************************************/

#pragma once   /* CodeBreaker: each header once (the directive layer includes them as C++ headers) */


typedef enum {
    START_UNDEFED,
    START_IS_SYM,
    START_IS_SDATA
} start_type;

typedef struct {
    union {
        symbol          *sym;
        segdata         *sdata;
    } targ;
    offset              off;
    mod_entry           *mod;
    addr_type           addr;
    start_type          type;
    boolbit             user_specd  : 1;
    boolbit             from_inc    : 1;
} startinfo;


extern void             InitLoadFile( void );
extern void             FiniLoadFile(Link& link);
extern void             CleanLoadFile( void );
extern void             GetStkAddr(Link& link);
extern void             DefBSSSyms(Link& link);
extern void             GetBSSSize(Link& link);
extern void             SetStkSize(Link& link);
extern void             GetStartAddr(Link& link);
extern void             ClearStartAddr(Link& link);
extern bool             CompareDosSegments(Link& link,  addr_type *, addr_type * );
extern bool             CompareOffsets( addr_type *, addr_type * );
extern bool             CompareProtSegments( addr_type *, addr_type * );
extern void             OrderGroups(Link& link,  FunctionRef<bool(addr_type *, addr_type *)> );
extern bool             WriteGroup(Link& link,  group_entry * );
extern unsigned_32      MemorySize(Link& link);
extern unsigned_32      AppendToLoadFile(Link& link,  const char * );
extern void             AddImpLibEntry(Link& link,  const char *, const char *, ordinal_t ordinal, bool by_name );
extern void             BuildImpLib(Link& link);
extern void             SetStartSym(Link& link,  const char * );
extern offset           CalcGroupSize(Link& link,  group_entry * );
extern offset           CalcSplitSize(Link& link);

extern void             FreeOutFiles(Link& link);
extern void             InitBuffFile( outfilelist *outfile, char *, bool );
extern void             OpenBuffFile(Link& link,  outfilelist * );
extern void             CloseBuffFile( outfilelist * );
extern void             SetOriginLoad(Link& link,  unsigned long );
extern void             WriteLoad3(Link& link,  void *, const char *, size_t );
extern void             WriteLoad(Link& link,  const void *, size_t );
extern void             WriteLoadU8(Link& link,  unsigned_8 data );
extern void             WriteLoadU16(Link& link,  unsigned_16 data );
extern void             WriteLoadU32(Link& link,  unsigned_32 data );
extern size_t           WriteLoadU8Name(Link& link,  const char *data, size_t len, bool ucase );
extern void             PadLoad(Link& link,  size_t );
extern void             PadBuffFile(Link& link,  outfilelist *, size_t );
extern void             SeekLoad(Link& link,  unsigned long );
extern void             SeekEndLoad(Link& link,  unsigned long );
extern unsigned long    PosLoad(Link& link);
extern void             WriteLeaderLoad(Link& link,  void * );
extern offset           WriteGroupLoad(Link& link,  group_entry *, bool );
extern unsigned_32      CopyToLoad(Link& link,  f_handle, const char * );
extern unsigned long    NullAlign(Link& link,  unsigned );
extern unsigned long    OffsetAlign(Link& link,  unsigned long, unsigned long );
extern void             ResetLoadFile(Link& link);
