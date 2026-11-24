/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Support for 64-bit integers (no native 64-bit type required).
*
****************************************************************************/


#ifndef _H__I64_
#define _H__I64_

#include "watcom.h"

extern "C" {










/* The FetchTrunc macros grab an 8/16/32-bit value from memory assuming
 * that the value is stored as a 64-bit integer. This is required for
 * big endian systems where the value is at different memory address
 * depending on its size.
 */


/* Note about the FetchNative macros: These assume that the value is stored
 * in memory as a non-64bit type, starting at the lowest address. That is,
 * it must be accessed as the first entry of the appropriate array (u._16
 * array for 16-bit value etc.) *regardless* of host endianness.
 */

    #define Init64Val(h,l)  {(l),(h)}
// set 64-bit from low and high part

// is the U64 a valid U32?
// is the U64 a positive I32?
// is the U64 a positive I64?
// is the I64 a I32?



extern int      U64Cnv10( unsigned_64 *res, char c );
extern int      U64Cnv8( unsigned_64 *res, char c );
extern int      U64Cnv2( unsigned_64 *res, char c );
extern int      U64Cnv16( unsigned_64 *res, char c );

extern void     U64Neg( unsigned_64 *res, const unsigned_64 *a );
extern void     U64Add( unsigned_64 *res, const unsigned_64 *a, const unsigned_64 *b );
extern void     U64Sub( unsigned_64 *res, const unsigned_64 *a, const unsigned_64 *b );
extern void     U64Mul( unsigned_64 *res, const unsigned_64 *a, const unsigned_64 *b );
extern void     U64Div( const unsigned_64 *a, const unsigned_64 *b, unsigned_64 *div, unsigned_64 *rem );
extern void     I64Div( const signed_64 *a, const signed_64 *b, signed_64 *div, signed_64 *rem );
extern void     U64AddI32( unsigned_64 *res, signed_32 i );
extern int      U64Cmp( const unsigned_64 *a, const unsigned_64 *b );
extern int      I64Cmp( const signed_64 *a, const signed_64 *b );
extern void     I64ShiftR( signed_64 *res, const signed_64 *a, unsigned shift );
extern void     U64ShiftR( unsigned_64 *res, const unsigned_64 *a, unsigned shift );
extern void     U64ShiftL( unsigned_64 *res, const unsigned_64 *a, unsigned shift );
extern void     U64Shift( unsigned_64 *res, const unsigned_64 *a, int shift );

}


inline unsigned char &U64Byte( unsigned_64 &a, int b ) { return ((a).u._8[(b)]); }
inline unsigned short &U64Word( unsigned_64 &a, int b ) { return ((a).u._16[(b)]); }
inline unsigned char &U64LowByte( unsigned_64 &a ) { return ((a).u._8[I64B0]); }
inline unsigned short &U64LowWord( unsigned_64 &a ) { return ((a).u._16[I64W0]); }
inline unsigned int &U64Low( unsigned_64 &a ) { return ((a).u._32[I64LO32]); }
inline unsigned int &U64LowLE( unsigned_64 &a ) { return ((a).u._32[0]); }
inline const unsigned char &U64HighByte( const unsigned_64 &a ) { return ((a).u._8[I64B1]); }
inline const unsigned short &U64HighWord( const unsigned_64 &a ) { return ((a).u._16[I64W1]); }
inline unsigned int &U64High( unsigned_64 &a ) { return ((a).u._32[I64HI32]); }
inline const unsigned int &U64High( const unsigned_64 &a ) { return ((a).u._32[I64HI32]); }
inline unsigned int &U64HighLE( unsigned_64 &a ) { return ((a).u._32[1]); }
inline signed char I64LowByte( const unsigned_64 &a ) { return (static_cast<signed_8>((a).u._8[I64B0])); }
inline short I64LowWord( const unsigned_64 &a ) { return (static_cast<signed_16>((a).u._16[I64W0])); }
inline int I64Low( const unsigned_64 &a ) { return (static_cast<signed_32>((a).u._32[I64LO32])); }
inline int I64High( const unsigned_64 &a ) { return (static_cast<signed_32>((a).u._32[I64HI32])); }
inline int U64CmpU32( const unsigned_64 &a, unsigned_32 b )
{
    return (((a).u._32[I64HI32])?1:(((a).u._32[I64LO32]<(b))?-1:((a).u._32[I64LO32]!=(b))));
}
inline bool I64isNeg( const unsigned_64 &a ) { return ((a).u.sign.v!=0); }
inline bool U64isZero( const unsigned_64 &a ) { return (((a).u._32[0]|(a).u._32[1])==0); }
inline bool U64isntZero( const unsigned_64 &a ) { return (((a).u._32[0]|(a).u._32[1])!=0); }
inline bool U64isEq( const unsigned_64 &a, const unsigned_64 &b ) { return (((a).u._32[0]==(b).u._32[0])&&((a).u._32[1]==(b).u._32[1])); }
inline bool U64isntEq( const unsigned_64 &a, const unsigned_64 &b ) { return (((a).u._32[0]!=(b).u._32[0])||((a).u._32[1]!=(b).u._32[1])); }
inline void U64And( unsigned_64 &r, const unsigned_64 &a, const unsigned_64 &b )
{
    (r).u._32[0] = (a).u._32[0] & (b).u._32[0];
    (r).u._32[1] = (a).u._32[1] & (b).u._32[1];
}
inline void U64AndEq( unsigned_64 &r, const unsigned_64 &b )
{
    (r).u._32[0] &= (b).u._32[0];
    (r).u._32[1] &= (b).u._32[1];
}
inline void U64Or( unsigned_64 &r, const unsigned_64 &a, const unsigned_64 &b )
{
    (r).u._32[0] = (a).u._32[0] | (b).u._32[0];
    (r).u._32[1] = (a).u._32[1] | (b).u._32[1];
}
inline void U64OrEq( unsigned_64 &r, const unsigned_64 &b )
{
    (r).u._32[0] |= (b).u._32[0];
    (r).u._32[1] |= (b).u._32[1];
}
inline void U64Xor( unsigned_64 &r, const unsigned_64 &a, const unsigned_64 &b )
{
    (r).u._32[0] = (a).u._32[0] ^ (b).u._32[0];
    (r).u._32[1] = (a).u._32[1] ^ (b).u._32[1];
}
inline void U64XorEq( unsigned_64 &r, const unsigned_64 &b )
{
    (r).u._32[0] ^= (b).u._32[0];
    (r).u._32[1] ^= (b).u._32[1];
}
inline void U64Not( unsigned_64 &r, const unsigned_64 &a )
{
    (r).u._32[0] = ~(a).u._32[0];
    (r).u._32[1] = ~(a).u._32[1];
}
inline void U64NotEq( unsigned_64 &r )
{
    (r).u._32[0] = ~(r).u._32[0];
    (r).u._32[1] = ~(r).u._32[1];
}
inline void U64NotAndEq( unsigned_64 &r, const unsigned_64 &b )
{
    (r).u._32[0] &= ~(b).u._32[0];
    (r).u._32[1] &= ~(b).u._32[1];
}
inline void U64Inc( unsigned_64 &r ) { if( ++(r).u._32[I64LO32] == 0 ) ++(r).u._32[I64HI32]; }
inline void U64Dec( unsigned_64 &r ) { if( (r).u._32[I64LO32]-- == 0 ) --(r).u._32[I64HI32]; }
inline unsigned int &U32FetchTrunc( unsigned_64 &x ) { return ((x).u._32[I64LO32]); }
inline int I32FetchTrunc( const unsigned_64 &x ) { return (static_cast<signed_32>((x).u._32[I64LO32])); }
inline unsigned char &U8FetchTrunc( unsigned_64 &x ) { return ((x).u._8[I64LO8]); }
inline void Set64Val( unsigned_64 &x, unsigned_32 l, unsigned_32 h )
{
    (x).u._32[I64LO32]=(l),(x).u._32[I64HI32]=(h);
}
inline void Set64ValZero( unsigned_64 &x ) { (x).u._32[0]=0,(x).u._32[1]=0; }
inline void Set64Val1p( unsigned_64 &x ) { (x).u._32[I64LO32]=1,(x).u._32[I64HI32]=0; }
inline void Set64Val1m( unsigned_64 &x )
{
    (x).u._32[I64LO32]=static_cast<unsigned_32>(-1),(x).u._32[I64HI32]=static_cast<unsigned_32>(-1);
}
inline void Set64ValI32( unsigned_64 &x, int v )
{
    (x).u._32[I64LO32]=(v),(x).u._32[I64HI32]=(static_cast<signed_32>((x).u._32[I64LO32])<0)?static_cast<unsigned_32>(-1):0;
}
inline void Set64ValU32( unsigned_64 &x, unsigned_32 v ) { (x).u._32[I64LO32]=(v),(x).u._32[I64HI32]=0; }
inline bool U64IsU32( const unsigned_64 &x ) { return ((x).u._32[I64HI32]==0); }
inline bool U64IsI32( const unsigned_64 &x ) { return (((x).u._32[I64HI32]==0)&&(static_cast<signed_32>((x).u._32[I64LO32])>=0)); }
inline void U64NegEq( unsigned_64 *a ) { U64Neg((a),(a)); }
inline void U64AddEq( unsigned_64 *a, unsigned_64 *b ) { U64Add((a),(a),(b)); }
inline void U64SubEq( unsigned_64 *a, unsigned_64 *b ) { U64Sub((a),(a),(b)); }
inline void U64MulEq( unsigned_64 *a, unsigned_64 *b ) { U64Mul((a),(a),(b)); }
inline void I64ShiftREq( unsigned_64 *a, unsigned b ) { I64ShiftR((a),(a),(b)); }
inline void U64ShiftREq( unsigned_64 *a, unsigned b ) { U64ShiftR((a),(a),(b)); }
inline void U64ShiftLEq( unsigned_64 *a, unsigned b ) { U64ShiftL((a),(a),(b)); }
inline void U64ShiftEq( unsigned_64 *a, int b ) { U64Shift((a),(a),(b)); }

#endif
