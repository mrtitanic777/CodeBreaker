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


#pragma once
typedef enum {
    LO_HALF     = 0x01,
    HI_HALF     = 0x02,
    IS_BYTE     = 0x04
} zero_bits;

/*  is the operand all zero? */
inline bool IsZeroBits( unsigned char bits ) { return ( ( (bits) & ( LO_HALF | HI_HALF ) ) == ( LO_HALF | HI_HALF ) ); }
inline bool IsZeroBits( zero_bits bits ) { return ( ( (bits) & ( LO_HALF | HI_HALF ) ) == ( LO_HALF | HI_HALF ) ); }

/*  is the low half of the operand zero? */
inline bool LoZero( zero_bits bits ) { return ( ( ( (bits) & LO_HALF ) != 0 ) && ( ( (bits) & IS_BYTE ) == 0 ) ); }

/*  is the high half of the operand zero? */
inline bool HiZero( zero_bits bits ) { return ( ( ( (bits) & HI_HALF ) != 0 ) && ( ( (bits) & IS_BYTE ) == 0 ) ); }

inline bool IsByte( zero_bits l, zero_bits r ) { return ( ( l & IS_BYTE ) || ( r & IS_BYTE ) ); }

/*  is the high part of one operand zero and the low part of the other zero */
inline bool OpposZero( zero_bits l, zero_bits r ) { return ( ( IsByte( l, r ) == 0 ) && ( ( l & r ) == 0 ) && IsZeroBits( (l) | (r) ) ); }


/*  are both high parts zero or both low parts zero? */
/*  screws up if IsZeroBits( l ) | IsZeroBits( r ) */
inline bool MatchZero( zero_bits l, zero_bits r ) { return ( LoZero( l & r ) || HiZero( l & r ) ); }
inline zero_bits ZeroHalf( zero_bits l, zero_bits r ) { return ( l & r ); }

/*  bits must be either LO_HALF or HI_HALF */
inline zero_bits OtherHalf( zero_bits bits ) { return ( ~bits & ( HI_HALF | LO_HALF ) ); }
