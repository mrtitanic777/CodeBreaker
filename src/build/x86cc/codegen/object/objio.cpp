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
#include "backrun.hpp"


#define INVALID_HANDLE  NULL

#define BOUNDARY        64
#define LIMIT           0x7FFFFFFF
#define BIGGEST         0x7FFFFFFF
#define MASK            0x80000000U

#define IOBUFSIZE       8192






namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct ObjioState: objio_state.h) */

}  // namespace



static buf *NewBuffer( BackRun& back )
/***************************/
{
    buf     *newbuf;

    newbuf = static_cast<buf *>(CGAlloc( back, sizeof( buf ) ));
    newbuf->buf = static_cast<char *>(CGAlloc( back, IOBUFSIZE ));
    newbuf->bufptr = newbuf->buf;
    newbuf->bytes_left = IOBUFSIZE;
    newbuf->bytes_written = 0;
    newbuf->nextbuf = NULL;
    return( newbuf );
}


static  void    CloseStream( BackRun& back, handle h )
/*************************************/
{
    back.held->fclose( h );
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


static  void    ObjError( BackRun& back, int errcode )
/*************************************/
{
    FatalError( back, strerror( errcode ) );
}


static  handle  CreateStream( BackRun& back, const char *name )
/**********************************************/
{
    handle      retc;

    retc = back.held->fopen( name, "wb" );
    if( retc == INVALID_HANDLE ) {
        ObjError( back, errno );
    }
    return( retc );
}


bool    CGOpenf( BackRun& back )
/*********************/
{
    back.objio.ObjFile = CreateStream( back, static_cast<const char *>(FEAuxInfo( *back.front, NULL, FEINF_OBJECT_FILE_NAME )) );
    if( back.objio.ObjFile == INVALID_HANDLE )
        return( false );
    back.objio.BufList = NewBuffer(back);              // allocate first buffer
    back.objio.CurBuf = back.objio.BufList;
    return( true );
}


void    OpenObj( BackRun& back )
/*********************/
{
    back.objio.ObjOffset = 0;
    back.objio.NeedSeek = false;
    back.objio.EraseObj = false;
}


static  objoffset   Byte( objhandle rec )
/***************************************/
{
    return( (objoffset)rec );
}


static  objhandle   Offset( BackRun& back, objoffset offset )
/********************************************/
{
    if( offset > BIGGEST ) {
        FatalError( back, "Object file too large" );
        return( INVALID_OBJHANDLE );
    } else {
        return( (objhandle)offset );
    }
}


static  void    PutStream( BackRun& back, handle h, const byte *b, size_t len )
/**************************************************************/
{
    size_t      n;

    /* unused parameters */ (void)h;

    for( ;; ) {
        n = len;
        if( n > back.objio.CurBuf->bytes_left ) {
            n = back.objio.CurBuf->bytes_left;
        }
        memcpy( back.objio.CurBuf->bufptr, b, n );
        b += n;
        back.objio.CurBuf->bufptr += n;
        back.objio.CurBuf->bytes_left -= n;
        if( back.objio.CurBuf->nextbuf == NULL ) {         // if this is last buffer
            cleanupLastBuffer( back.objio.CurBuf );
        }
        len -= n;
        if( len == 0 )
            break;
        if( back.objio.CurBuf->nextbuf == NULL ) {
            back.objio.CurBuf->nextbuf = NewBuffer(back);
            back.objio.CurBuf = back.objio.CurBuf->nextbuf;
        } else {
            back.objio.CurBuf = back.objio.CurBuf->nextbuf;
            back.objio.CurBuf->bufptr = back.objio.CurBuf->buf;
            back.objio.CurBuf->bytes_left = IOBUFSIZE;
        }
    }
}


static  void    GetStream( BackRun& back, handle h, byte *b, size_t len )
/********************************************************/
{
    size_t           n;

    /* unused parameters */ (void)h;

    for( ;; ) {
        n = len;
        if( n > back.objio.CurBuf->bytes_left ) {
            n = back.objio.CurBuf->bytes_left;
        }
        memcpy( b, back.objio.CurBuf->bufptr, n );
        b += n;
        back.objio.CurBuf->bufptr += n;
        back.objio.CurBuf->bytes_left -= n;
        len -= n;
        if( len == 0 )
            break;
        back.objio.CurBuf = back.objio.CurBuf->nextbuf;
        if( back.objio.CurBuf == NULL ) {
            _Zoiks( ZOIKS_006 );
        }
        back.objio.CurBuf->bufptr = back.objio.CurBuf->buf;
        back.objio.CurBuf->bytes_left = IOBUFSIZE;
    }
}


static  void    SeekStream( BackRun& back, handle h, objoffset offset )
/******************************************************/
{
    buf         *pbuf;
    objoffset   n;

    /* unused parameters */ (void)h;

    pbuf = back.objio.BufList;
    n = 0;
    for( ;; ) {
        n += IOBUFSIZE;
        if( n > offset )
            break;
        if( pbuf->nextbuf == NULL ) {
            // seeking past the end of the file (extend file)
            pbuf->bytes_written = IOBUFSIZE;
            pbuf->nextbuf = NewBuffer(back);
        }
        pbuf = pbuf->nextbuf;
    }
    n = offset - (n - IOBUFSIZE);
    pbuf->bufptr = pbuf->buf + n;
    pbuf->bytes_left = IOBUFSIZE - n;
    if( pbuf->nextbuf == NULL ) {               // if this is last buffer
        cleanupLastBuffer( pbuf );
    }
    back.objio.CurBuf = pbuf;
}


objhandle       AskObjHandle( BackRun& back )
/**********************************/
{
    return( Offset( back, back.objio.ObjOffset ) );
}

void    PutObjBytes( BackRun& back, const void *buff, size_t len )
/*************************************************/
{
    if( back.objio.NeedSeek ) {
        SeekStream( back, back.objio.ObjFile, back.objio.ObjOffset );
        back.objio.NeedSeek = false;
    }
    PutStream( back, back.objio.ObjFile, static_cast<const byte *>(buff), len );
    back.objio.ObjOffset += len;
}

void    GetFromObj( BackRun& back, objhandle rec, objoffset roffset, byte *buff, size_t len )
/****************************************************************************/
{
    SeekStream( back, back.objio.ObjFile, Byte( rec ) + roffset + 3 );
    GetStream( back, back.objio.ObjFile, buff, len );
    back.objio.NeedSeek = true;
}


void    AbortObj( BackRun& back )
/**********************/
{
    back.objio.EraseObj = true;
}


static void FlushBuffers( BackRun& back, handle h )
/**********************************/
{
    buf         *pbuf;
    size_t      retc;

    for( ; (pbuf = back.objio.BufList) != NULL; ) {
        back.objio.BufList = pbuf->nextbuf;
        retc = fwrite( pbuf->buf, 1, pbuf->bytes_written, h );
        if( retc != pbuf->bytes_written ) {
            FatalError( back, "Error writing object file" );
        }
        CGFree( back, pbuf->buf );
        CGFree( back, pbuf );
    }
    back.objio.CurBuf = NULL;
}


void    CloseObj( BackRun& back )
/**********************/
{
    if( back.objio.ObjFile != INVALID_HANDLE ) {
        FlushBuffers( back, back.objio.ObjFile );
        CloseStream( back, back.objio.ObjFile );
        if( back.objio.EraseObj ) {
            EraseStream( static_cast<const char *>(FEAuxInfo( *back.front, NULL, FEINF_OBJECT_FILE_NAME )) );
        }
        back.objio.ObjFile = INVALID_HANDLE;
    }
}


void    ScratchObj( BackRun& back )
/************************/
{
    back.objio.EraseObj = true;
    CloseObj(back);
}

void SeekGetObj( BackRun& back, objhandle rec, objoffset roffset, byte *b, size_t len )
/**********************************************************************/
{
    SeekStream( back, back.objio.ObjFile, Byte( rec ) + roffset );
    GetStream( back, back.objio.ObjFile, b, len );
}

void SeekPutObj( BackRun& back, objhandle rec, objoffset roffset, const byte *b, size_t len )
/****************************************************************************/
{
    SeekStream( back, back.objio.ObjFile, Byte( rec ) + roffset );
    PutStream( back, back.objio.ObjFile, b, len );
}

void NeedSeekObj( BackRun& back )
/**************************/
{
    back.objio.NeedSeek = true;
}
