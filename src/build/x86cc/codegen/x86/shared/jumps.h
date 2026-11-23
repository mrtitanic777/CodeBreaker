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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#pragma once
#define M_CJILONG       0x08ff
#define M_CJINEAR       0x00ff
#define B_IND_RMR_CALL  0x0010
#define B_IND_RMR_JMP   0x0020

#define M_RET           0x00c2
#define B_RET_LONG      0x0008
#define B_RET_IRET      0x000C
#define B_RET_NOPOP     0x0001

#define _OutShortD( addr )      OutDataByte( back, addr - AskLocation(back) - 1 )
#define _OutFarSeg( seg )       OutDataShort( back, seg )
#if _TARGET & _TARG_8086
    #define _OutOpndSize
    #define _OutFarD( seg, off )    OutDataShort( back, seg ); \
                                    OutDataShort( back, off )
    #define _OutFarOff( off )       OutDataShort( back, off )
    #define _OutNearD( addr )       OutDataShort( back, addr - AskLocation(back) - 2 )
    #define _OFFSET_PATCH           WORD_PATCH
    #define _NEAR_PATCH             (ADD_PATCH | WORD_PATCH)
#else
    #define _OutOpndSize            if( _IsntTargetModel( CGSW_X86_USE_32 ) ) { \
                                        OutDataByte( back, 0x66 ); \
                                    }
    #define _OutFarOff( off )       OutDataLong( back, off );
    #define _OutNearD( addr )       OutDataLong( back, (addr)-AskLocation(back)-4 );
    #define _OutFarD( seg, off )    OutDataShort( back, seg ); \
                                    OutDataLong( back, (unsigned)off );
    #define _OFFSET_PATCH           LONG_PATCH
    #define _NEAR_PATCH             (ADD_PATCH | LONG_PATCH)
#endif
#define _OutCCyp                OutDataByte( back, 0x0e ); \
                                _OutOpndSize; \
                                OutDataByte( back, 0xe8 )
#define _OutCNear               _OutOpndSize; \
                                OutDataByte( back, 0xe8 )
#define _OutJNear               _OutOpndSize; \
                                OutDataByte( back, 0xe9 )
#define _OutCFar                _OutOpndSize; \
                                OutDataByte( back, 0x9a )
#define _OutJFar                _OutOpndSize; \
                                OutDataByte( back, 0xea )
#define _OutJShort              OutDataByte( back, 0xeb )
#define _OutJCond( cond )       OutDataByte( back, 0x70 + cond )
#define _OutJCondNear( cond )   _OutOpndSize; \
                                OutDataByte( back, 0x0f ); \
                                OutDataByte( back, 0x80 + cond )

#define _SHORT_PATCH            (ADD_PATCH | BYTE_PATCH)
