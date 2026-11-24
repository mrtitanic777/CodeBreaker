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
* Description:  Common type definitions and macros widely used by Open
*               Watcom tools.
*
****************************************************************************/


#ifndef _WATCOM_H_INCLUDED_
#define _WATCOM_H_INCLUDED_


/* An equivalent of the __unaligned keyword may be necessary on RISC
 * architectures, but on x86/x64 it's useless
 */
#define _WCUNALIGNED

/* Macros for low/high end access on little-endian and big-endian machines */

    #define I64LO32 0
    #define I64HI32 1
    #define I64LO16 0
    #define I64W0   0
    #define I64W1   1
    #define I64LO8  0
    #define I64B0   0
    #define I64B1   1
    #define I64B2   2
    #define I64B3   3
    #define I64B4   4
    #define I64B5   5
    #define I64B6   6
    #define I64B7   7


/*  Macros for little-endian/big-endian conversion; These exist to simplify writing
 *  code that handles both little-endian and big-endian data on either little or big
 *  endian host platforms. Some of these macros could be implemented as inline
 *  assembler where instructions to byte swap data in registers or read/write
 *  memory access with byte swapping is available.
 *
 *  NOTE:   The SWAP_XX macros will swap data in place. If you only want to take a
 *          a copy of the data and leave the original intact, then use the SWAPNC_XX
 *          macros.
 */

    /* Macros to get little-endian data */
    /* Macros to get big-endian data */
    /* Macros to convert little-endian data in place */
    #define CONV_LE_16(w)
    #define CONV_LE_32(w)
    #define CONV_LE_64(w)
    /* Macros to convert big-endian data in place */
    /* Macros to swap byte order */

/********************************************************************
 *
 * MACROS for aligned data
 *
 ********************************************************************/


/*
 * Macros for get/put data (endian neutral)
 */




    /*
     * Macros for little-endian data (little-endian host)
     */




    /*
     * Macros for big-endian data (little-endian host)
     */






/********************************************************************
 *
 * MACROS for un-aligned data
 *
 ********************************************************************/


/*
 * Macros for get/put data (endian neutral)
 */

//#define MGET_U64_UN(p)

//#define MGET_S64_UN(p)

//#define MPUT_LE_64_UN(p,w)


    /*
     * Macros for little-endian data (little-endian host)
     */

//    #define MGET_LE_U64_UN(p)

//    #define MGET_LE_S64_UN(p)

//    #define MPUT_LE_64_UN(p,w)

    /*
     * Macros for big-endian data (little-endian host)
     */

//    #define MGET_BE_U64_UN(p)

//    #define MGET_BE_U64_UN(p)

//    #define MPUT_BE_64_UN(p,w)



/* Macros to swap byte order in 64-bit structure */

typedef unsigned            uint;

typedef unsigned char       uint_8;
typedef unsigned short      uint_16;
typedef unsigned int        uint_32;
#if defined( _MSC_VER )
typedef unsigned __int64    uint_64;
#else
typedef unsigned long long  uint_64;
#endif

typedef signed char         int_8;
typedef signed short        int_16;
typedef signed int          int_32;
#if defined( _MSC_VER )
typedef __int64             int_64;
#else
typedef long long           int_64;
#endif

#if defined( _MSC_VER )
typedef __int64             pointer_int;
typedef unsigned __int64    pointer_uint;
#else
typedef long long           pointer_int;
typedef unsigned long long  pointer_uint;
#endif

typedef unsigned char       unsigned_8;
typedef unsigned short      unsigned_16;
typedef unsigned int        unsigned_32;
typedef struct {
    union {
        unsigned_32     _32[2];
        unsigned_16     _16[4];
        unsigned_8      _8[8];
        struct {
            unsigned     : 16;
            unsigned     : 16;
            unsigned     : 16;
            unsigned     : 15;
            unsigned    v: 1;
        }       sign;
        uint_64         _64[1];
    } u;
} unsigned_64;

typedef signed char         signed_8;
typedef signed short        signed_16;
typedef signed int          signed_32;
typedef unsigned_64         signed_64;


inline unsigned long SwapBytes16( unsigned long w ) { return ( (((w) & 0x000000FFUL) << 8) | (((w) & 0x0000FF00UL) >> 8) ); }
inline unsigned long SwapBytes32( unsigned long w )
{
    return ( (((w) & 0x000000FFUL) << 24) | (((w) & 0x0000FF00UL) << 8) | (((w) & 0x00FF0000UL) >> 8) | (((w) & 0xFF000000UL) >> 24) );
}
template <class T>
inline T& MemAs( void *p ) { return *static_cast<T *>(p); }
inline void PutLeU16( void *p, uint_16 w ) { MemAs<uint_16>( p ) = w; }
inline void PutLeU32( void *p, uint_32 w ) { MemAs<uint_32>( p ) = w; }
inline void PutBeU16( void *p, uint_16 w ) { MemAs<uint_16>( p ) = SwapBytes16( w ); }
inline void PutBeU32( void *p, uint_32 w ) { MemAs<uint_32>( p ) = SwapBytes32( w ); }
inline unsigned char &U8At0( void *x ) { return ((static_cast<uint_8*>(x))[0]); }
inline unsigned char &U8At1( void *x ) { return ((static_cast<uint_8*>(x))[1]); }
inline unsigned char &U8At2( void *x ) { return ((static_cast<uint_8*>(x))[2]); }
inline unsigned char &U8At3( void *x ) { return ((static_cast<uint_8*>(x))[3]); }
inline unsigned char U16Byte0( uint_16 x ) { return (static_cast<uint_8>((x))); }
inline unsigned char U16Byte1( uint_16 x ) { return (static_cast<uint_8>(((x)) >> 8)); }
inline unsigned char U32Byte0( uint_32 x ) { return (static_cast<uint_8>((x))); }
inline unsigned char U32Byte1( uint_32 x ) { return (static_cast<uint_8>(((x)) >> 8)); }
inline unsigned char U32Byte2( uint_32 x ) { return (static_cast<uint_8>(((x)) >> 16)); }
inline unsigned char U32Byte3( uint_32 x ) { return (static_cast<uint_8>(((x)) >> 24)); }
inline unsigned short U16Place0( unsigned char x ) { return (static_cast<uint_16>(x)); }
inline int U16Place1( unsigned char x ) { return ((static_cast<uint_16>(x)) << 8); }
inline unsigned int U32Place0( unsigned char x ) { return (static_cast<uint_32>(x)); }
inline unsigned int U32Place1( unsigned char x ) { return ((static_cast<uint_32>(x)) << 8); }
inline unsigned int U32Place2( unsigned char x ) { return ((static_cast<uint_32>(x)) << 16); }
inline unsigned int U32Place3( unsigned char x ) { return ((static_cast<uint_32>(x)) << 24); }
inline int GetU16Unaligned( void *p ) { return ( U16Place0(U8At0((p))) | U16Place1(U8At1((p))) ); }
inline unsigned int GetU32Unaligned( void *p )
{
    return ( U32Place0(U8At0((p))) | U32Place1(U8At1((p))) | U32Place2(U8At2((p))) | U32Place3(U8At3((p))) );
}
inline void PutU16Unaligned( void *p, uint_16 w )
{
    U8At0((p)) = U16Byte0((w));
    U8At1((p)) = U16Byte1((w));
}
inline void PutU32Unaligned( void *p, uint_32 w )
{
    U8At0((p)) = U32Byte0((w));
    U8At1((p)) = U32Byte1((w));
    U8At2((p)) = U32Byte2((w));
    U8At3((p)) = U32Byte3((w));
}
inline void PutLeU16Unaligned( void *p, unsigned short w )
{
    U8At0((p)) = U16Byte0((w));
    U8At1((p)) = U16Byte1((w));
}
inline void PutLeU32Unaligned( void *p, unsigned int w )
{
    U8At0((p)) = U32Byte0((w));
    U8At1((p)) = U32Byte1((w));
    U8At2((p)) = U32Byte2((w));
    U8At3((p)) = U32Byte3((w));
}
inline void PutBeU16Unaligned( void *p, unsigned short w )
{
    U8At0((p)) = U16Byte1((w));
    U8At1((p)) = U16Byte0((w));
}
inline void PutBeU32Unaligned( void *p, unsigned int w )
{
    U8At0((p)) = U32Byte3((w));
    U8At1((p)) = U32Byte2((w));
    U8At2((p)) = U32Byte1((w));
    U8At3((p)) = U32Byte0((w));
}

#endif

