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
* Description:  Buffered POSIX style I/O.
*
****************************************************************************/


#include "_cgstd.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "cgmem.h"
#include "_cg.h"
#include "cgaux.h"
#include "bckdef.h"
#include "system.h"
#include "pcobj.h"
#include "zoiks.h"
#include "banner.h"
#include "utils.h"
#include "objio.h"
#include "onexit.h"
#include "feprotos.h"


#define INVALID_HANDLE  NULL

#define BOUNDARY        64
#define LIMIT           0x7FFFFFFF
#define BIGGEST         0x7FFFFFFF
#define MASK            0x80000000U

#define IOBUFSIZE       8192

typedef FILE            *handle;

typedef struct buf {
    struct buf  *nextbuf;
    char        *bufptr;        // current position within buffer
    char        *buf;           // start of buffer
    size_t      bytes_left;     // number of bytes remaining in buffer
    size_t      bytes_written;  // number of bytes written to buffer
} buf;


namespace {

/* the statics of this file, one set for each run of the compiler */
struct ObjioState {
    objoffset ObjOffset;
    handle ObjFile;
    bool_ NeedSeek;
    bool_ EraseObj;
    buf *BufList;   // start of list of buffers
    buf *CurBuf;   // current buffer
};

}  // namespace

static inline ObjioState& objio_state() { return cbrun_part<ObjioState>(); }


static buf *NewBuffer( void )
/***************************/
{
    buf     *newbuf;

    newbuf = static_cast<buf *>(CGAlloc( sizeof( buf ) ));
    newbuf->buf = static_cast<char *>(CGAlloc( IOBUFSIZE ));
    newbuf->bufptr = newbuf->buf;
    newbuf->bytes_left = IOBUFSIZE;
    newbuf->bytes_written = 0;
    newbuf->nextbuf = NULL;
    return( newbuf );
}


static  void    CloseStream( handle h )
/*************************************/
{
    fclose( h );
}


static  void    EraseStream( const char *name )
/*********************************************/
{
    remove( name );
}


static void cleanupLastBuffer( buf *pbuf )
{
    size_t  amt_used;

    amt_used = IOBUFSIZE - pbuf->bytes_left;
    if( pbuf->bytes_written < amt_used ) {
        pbuf->bytes_written = amt_used;
    }
}


static  void    ObjError( int errcode )
/*************************************/
{
    FatalError( strerror( errcode ) );
}


static  handle  CreateStream( const char *name )
/**********************************************/
{
    handle      retc;

    retc = fopen( name, "wb" );
    if( retc == INVALID_HANDLE ) {
        ObjError( errno );
    }
    return( retc );
}


bool_    CGOpenf( void )
/*********************/
{
    objio_state().ObjFile = CreateStream( static_cast<const char *>(FEAuxInfo( NULL, FEINF_OBJECT_FILE_NAME )) );
    if( objio_state().ObjFile == INVALID_HANDLE )
        return( false );
    objio_state().BufList = NewBuffer();              // allocate first buffer
    objio_state().CurBuf = objio_state().BufList;
    return( true );
}


void    OpenObj( void )
/*********************/
{
    objio_state().ObjOffset = 0;
    objio_state().NeedSeek = false;
    objio_state().EraseObj = false;
}


static  objoffset   Byte( objhandle rec )
/***************************************/
{
    return( (objoffset)rec );
}


static  objhandle   Offset( objoffset offset )
/********************************************/
{
    if( offset > BIGGEST ) {
        FatalError( "Object file too large" );
        return( INVALID_OBJHANDLE );
    } else {
        return( (objhandle)offset );
    }
}


static  void    PutStream( handle h, const byte *b, size_t len )
/**************************************************************/
{
    size_t      n;

    /* unused parameters */ (void)h;

    for( ;; ) {
        n = len;
        if( n > objio_state().CurBuf->bytes_left ) {
            n = objio_state().CurBuf->bytes_left;
        }
        memcpy( objio_state().CurBuf->bufptr, b, n );
        b += n;
        objio_state().CurBuf->bufptr += n;
        objio_state().CurBuf->bytes_left -= n;
        if( objio_state().CurBuf->nextbuf == NULL ) {         // if this is last buffer
            cleanupLastBuffer( objio_state().CurBuf );
        }
        len -= n;
        if( len == 0 )
            break;
        if( objio_state().CurBuf->nextbuf == NULL ) {
            objio_state().CurBuf->nextbuf = NewBuffer();
            objio_state().CurBuf = objio_state().CurBuf->nextbuf;
        } else {
            objio_state().CurBuf = objio_state().CurBuf->nextbuf;
            objio_state().CurBuf->bufptr = objio_state().CurBuf->buf;
            objio_state().CurBuf->bytes_left = IOBUFSIZE;
        }
    }
}


static  void    GetStream( handle h, byte *b, size_t len )
/********************************************************/
{
    size_t           n;

    /* unused parameters */ (void)h;

    for( ;; ) {
        n = len;
        if( n > objio_state().CurBuf->bytes_left ) {
            n = objio_state().CurBuf->bytes_left;
        }
        memcpy( b, objio_state().CurBuf->bufptr, n );
        b += n;
        objio_state().CurBuf->bufptr += n;
        objio_state().CurBuf->bytes_left -= n;
        len -= n;
        if( len == 0 )
            break;
        objio_state().CurBuf = objio_state().CurBuf->nextbuf;
        if( objio_state().CurBuf == NULL ) {
            _Zoiks( ZOIKS_006 );
        }
        objio_state().CurBuf->bufptr = objio_state().CurBuf->buf;
        objio_state().CurBuf->bytes_left = IOBUFSIZE;
    }
}


static  void    SeekStream( handle h, objoffset offset )
/******************************************************/
{
    buf         *pbuf;
    objoffset   n;

    /* unused parameters */ (void)h;

    pbuf = objio_state().BufList;
    n = 0;
    for( ;; ) {
        n += IOBUFSIZE;
        if( n > offset )
            break;
        if( pbuf->nextbuf == NULL ) {
            // seeking past the end of the file (extend file)
            pbuf->bytes_written = IOBUFSIZE;
            pbuf->nextbuf = NewBuffer();
        }
        pbuf = pbuf->nextbuf;
    }
    n = offset - (n - IOBUFSIZE);
    pbuf->bufptr = pbuf->buf + n;
    pbuf->bytes_left = IOBUFSIZE - n;
    if( pbuf->nextbuf == NULL ) {               // if this is last buffer
        cleanupLastBuffer( pbuf );
    }
    objio_state().CurBuf = pbuf;
}


objhandle       AskObjHandle( void )
/**********************************/
{
    return( Offset( objio_state().ObjOffset ) );
}

void    PutObjBytes( const void *buff, size_t len )
/*************************************************/
{
    if( objio_state().NeedSeek ) {
        SeekStream( objio_state().ObjFile, objio_state().ObjOffset );
        objio_state().NeedSeek = false;
    }
    PutStream( objio_state().ObjFile, static_cast<const byte *>(buff), len );
    objio_state().ObjOffset += len;
}

void    GetFromObj( objhandle rec, objoffset roffset, byte *buff, size_t len )
/****************************************************************************/
{
    SeekStream( objio_state().ObjFile, Byte( rec ) + roffset + 3 );
    GetStream( objio_state().ObjFile, buff, len );
    objio_state().NeedSeek = true;
}


void    AbortObj( void )
/**********************/
{
    objio_state().EraseObj = true;
}


static void FlushBuffers( handle h )
/**********************************/
{
    buf         *pbuf;
    size_t      retc;

    for( ; (pbuf = objio_state().BufList) != NULL; ) {
        objio_state().BufList = pbuf->nextbuf;
        retc = fwrite( pbuf->buf, 1, pbuf->bytes_written, h );
        if( retc != pbuf->bytes_written ) {
            FatalError( "Error writing object file" );
        }
        CGFree( pbuf->buf );
        CGFree( pbuf );
    }
    objio_state().CurBuf = NULL;
}


void    CloseObj( void )
/**********************/
{
    if( objio_state().ObjFile != INVALID_HANDLE ) {
        FlushBuffers( objio_state().ObjFile );
        CloseStream( objio_state().ObjFile );
        if( objio_state().EraseObj ) {
            EraseStream( static_cast<const char *>(FEAuxInfo( NULL, FEINF_OBJECT_FILE_NAME )) );
        }
        objio_state().ObjFile = INVALID_HANDLE;
    }
}


void    ScratchObj( void )
/************************/
{
    objio_state().EraseObj = true;
    CloseObj();
}

void SeekGetObj( objhandle rec, objoffset roffset, byte *b, size_t len )
/**********************************************************************/
{
    SeekStream( objio_state().ObjFile, Byte( rec ) + roffset );
    GetStream( objio_state().ObjFile, b, len );
}

void SeekPutObj( objhandle rec, objoffset roffset, const byte *b, size_t len )
/****************************************************************************/
{
    SeekStream( objio_state().ObjFile, Byte( rec ) + roffset );
    PutStream( objio_state().ObjFile, b, len );
}

void NeedSeekObj( void )
/**************************/
{
    objio_state().NeedSeek = true;
}
