/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Interface to the cfloat library.
*
****************************************************************************/


#pragma once
#include <sys/types.h>
#include "watcom.h"
#include "bool.h"

/* aligned */
#define STRUCT_cfloat( x ) \
{ \
    int             exp; \
    int             len; \
    size_t          alloc; \
    signed char     sign; \
    char            mant[ x ]; \
}

#define CFLOAT_SIZE (offsetof( cfloat, mant ) + 1)

extern "C" {

typedef struct cfloat STRUCT_cfloat( 1 ) cfloat;

typedef struct cfstruct {
    void            *(*alloc)( BackRun&, size_t );
    void            (*free)( BackRun&, void * );
    void            *head;
} cfstruct;

typedef cfstruct    *cfhandle;

typedef union flt {
    /*
     * longest member (ldble) must be first for proper union initialization
     */
    double          ldble;  // FIXME - need long double support
    double          dble;
    float           sngl;
    unsigned_64     u64;
} flt;

extern  void        CFCnvTarget( cfloat *f, flt *buffer, int class_ );
extern  char        *CFCnvFS( cfloat *f, char *buffer, int maxlen );
extern  cfloat      *CFCnvSF( BackRun& back, cfhandle h, const char *str );
extern  cfloat      *CFCopy( BackRun& back, cfhandle h, const cfloat *f );
extern  cfloat      *CFTrunc( BackRun& back, cfhandle h, cfloat *f );
extern  cfloat      *CFRound( BackRun& back, cfhandle h, cfloat *f );
extern  cfloat      *CFCnvI32F( BackRun& back, cfhandle h, signed_32 data );
extern  cfloat      *CFCnvU32F( BackRun& back, cfhandle h, unsigned_32 data );
extern  cfloat      *CFCnvU64F( BackRun& back, cfhandle h, unsigned_32 lo32, unsigned_32 hi32 );
extern  cfloat      *CFCnvI64F( BackRun& back, cfhandle h, unsigned_32 lo32, unsigned_32 hi32 );
extern  cfloat      *CFCnvIF( BackRun& back, cfhandle h, int data );
extern  cfloat      *CFCnvUF( BackRun& back, cfhandle h, uint data );

extern  bool        CFIsI8( cfloat *f );
extern  bool        CFIsI16( cfloat *f );
extern  bool        CFIsI32( cfloat *f );
extern  bool        CFIsI64( cfloat *f );
extern  bool        CFIsU8( cfloat *f );
extern  bool        CFIsU16( cfloat *f );
extern  bool        CFIsU32( cfloat *f );
extern  bool        CFIsU64( cfloat *f );
extern  bool        CFIs32( cfloat *f );
extern  bool        CFIs64( cfloat *f );
extern  bool        CFIsFloat( cfloat *f );
extern  bool        CFIsDouble( cfloat *f );

extern  bool        CFIsSize( cfloat *f, uint size );
extern  bool        CFSignedSize( cfloat *f, uint size );
extern  bool        CFUnSignedSize( cfloat *f, uint size );

extern  signed_16   CFCnvF16( cfloat *f );
extern  signed_32   CFCnvF32( cfloat *f );
extern  signed_64   CFCnvF64( cfloat *f );
extern  double      CFToF( cfloat *f );

extern  cfloat      *CFAlloc( BackRun& back, cfhandle h, size_t );
extern  void        CFFree( BackRun& back, cfhandle h, cfloat *f );
extern  bool        CFFrlFree( BackRun& back, cfhandle h );

extern  cfloat      *CFMul( BackRun& back, cfhandle h, cfloat *f1, const cfloat *f2 );
extern  cfloat      *CFDiv( BackRun& back, cfhandle h, cfloat *f1, cfloat *f2 );
extern  cfloat      *CFInverse( BackRun& back, cfhandle h, cfloat *f );
extern  cfloat      *CFAdd( BackRun& back, cfhandle h, cfloat *f1, cfloat *f2 );
extern  cfloat      *CFSub( BackRun& back, cfhandle h, cfloat *f1, cfloat *f2 );
extern  void        CFNegate( cfloat *f );
extern  int         CFCompare( const cfloat *f1, const cfloat *f2 );
extern  int         CFTest( cfloat *f );
extern  int         CFExp( cfloat *f );

extern  void        CFInit( cfhandle h );
extern  void        CFFini( BackRun& back, cfhandle h );

};
