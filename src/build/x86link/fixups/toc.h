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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/

#pragma once   /* CodeBreaker: each header once (the directive layer includes them as C++ headers) */


extern void             InitToc(Link& link);
extern void             CleanToc( void );
extern void             AddSymToToc(Link& link,  symbol * );
extern void             AddSdataOffToToc(Link& link,  segdata *, offset );
extern void             PrepareToc(Link& link);
extern offset           FindSdataOffPosInToc(Link& link,  segdata *, offset );
extern offset           FindIATSymAbsOff( symbol * );
extern offset           FindSymPosInToc(Link& link,  symbol * );
extern offset           FindSymPosInTocv(Link& link,  symbol * );
extern unsigned long    GetTocSize( void );
extern void             WriteToc(Link& link,  virt_mem );
extern void             SetTocAddr(Link& link,  offset , group_entry * );
extern void             CheckIfTocSym( symbol * );
extern bool             IsTocSym( symbol * );
extern void             ResetToc(Link& link);

