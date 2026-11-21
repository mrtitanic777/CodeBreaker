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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/

#pragma once   /* CodeBreaker: each header once (the directive layer includes them as C++ headers) */


#ifdef _EXE


/* in ovlsupp.c */

extern void             WalkAreas( AREASECT *area, FunctionRef<walksecs_fn> );
extern void             ParmWalkAreas( AREASECT *area, FunctionRef<parmwalksecs_fn>, void * );
extern void             OvlProcPubsSect(Link& link,  section * );
extern void             OvlProcPubs(Link& link);
extern void             OvlCalc(Link& link);
extern void             FreeOverlaySupp(Link& link);
extern void             OvlDefVector( symbol * );
extern void             OvlVectorize( symbol * );
extern void             OvlTryRefVector(Link& link,  symbol * );
extern void             OvlUseVector(Link& link,  symbol *, extnode * );
extern void             OvlIndirectCall(Link& link,  symbol * );
extern void             OvlGetVecAddr(Link& link,  int, addr_type * );
extern bool             OvlCheckOvlClass(Link& link,  const char *, bool * );
extern section          *OvlCheckOvlSection(Link& link,  const char * );
extern void             OvlEmitVectors(Link& link);
extern void             OvlSetStartAddr(Link& link);
extern void             OvlPass1(Link& link);
extern void             OvlEmitTable(Link& link);
extern void             OvlPadOvlFiles(Link& link);
extern void             ResetOverlaySupp(Link& link);
extern void             OvlSetTableLoc(Link& link,  group_entry *, unsigned long );

/* in overlays.c */

extern void             OvlNumberSections(Link& link);
extern void             OvlSetSegments(Link& link);
extern void             OvlFillOutFilePtrs(Link& link);
extern void             OvlTryDefVector(Link& link,  symbol * );
extern void             OvlTryUseVector(Link& link,  symbol *, extnode * );
extern void             OvlPass2(Link& link);

#endif
