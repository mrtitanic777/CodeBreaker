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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/

#pragma once   /* CodeBreaker: each header once (the directive layer includes them as C++ headers) */


typedef struct {
    void    *(*getstart)( void );
    void    (*setstart)( void * );
    bool    (*isend)( void );
    char    *(*getname)( void );
} vflistrtns;

typedef struct cdat_piece {
    struct cdat_piece   *next;
    unsigned_8          *data;
    offset              length;
    boolbit             free_data : 1;
} comdat_piece;

typedef struct cdat_info {
    struct cdat_info    *next;
    segdata             *sdata;
    symbol              *sym;
    comdat_piece        *pieces;
    sym_info            flags;
} comdat_info;

extern void             P1Start( void );
extern unsigned long    IncPass1( void );
extern void             DoIncGroupDefs(Link& link);
extern void             Set64BitMode(Link& link);
extern void             Set32BitMode(Link& link);
extern void             Set16BitMode(Link& link);
extern void             AllocateSegment(Link& link,  segnode *, const char *clname );
extern void             AddSegment(Link& link,  segdata *, class_entry * );
extern class_entry      *DuplicateClass(Link& link,  class_entry *class_ );
extern class_entry      *FindClass(Link& link,  section *, const char *, byte bits, bool iscode );
extern seg_leader       *InitLeader(Link& link,  const char * );
extern void             FreeLeader( void * );
extern void             AddToGroup(Link& link,  group_entry *, seg_leader * );
extern void             SetAddPubSym( symbol *, sym_info, mod_entry *, offset, unsigned_16 );
extern void             DefineSymbol(Link& link,  symbol *, segnode *, offset, unsigned_16 );
extern void             AllocCommunal(Link& link,  symbol *, offset );
extern symbol           *MakeCommunalSym(Link& link,  symbol *, offset, bool, byte bits );
extern void             SetComdatSym(Link& link,  symbol *, segdata * );
extern void             StoreInfoData( comdat_info *info );
extern void             InfoCDatAltDef(Link& link,  comdat_info * );
extern void             DefineLazyExtdef(Link& link,  symbol *, symbol *, bool );
extern void             DefineVFTableRecord(Link& link,  symbol *, symbol *, bool, vflistrtns * );
extern void             DefineVFReference( void *, symbol *, bool );
extern void             DefineReference(Link& link,  symbol * );
extern void             CheckComdatSym(Link& link,  symbol *, sym_info );
extern void             DefineComdat(Link& link,  segdata *, symbol *, offset, sym_info, unsigned_8 * );
extern group_entry      *GetGroup(Link& link,  const char * );
extern group_entry      *SearchGroups(Link& link,  const char * );
extern bool             SeenDLLRecord(Link& link);
extern void             HandleImport(Link& link,  const length_name *, const length_name *, const length_name *, ordinal_t );
extern void             HandleExport(Link& link,  const length_name *, const length_name *, unsigned, ordinal_t );
extern bool             CheckVFList( symbol * );
extern void             SetCurrSeg(Link& link,  segdata *, offset, unsigned_8 * );
extern void             ResetObjPass1(Link& link);

