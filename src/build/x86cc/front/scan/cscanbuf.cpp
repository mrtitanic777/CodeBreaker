/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2020-2022 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
* Description:  C lexical analyzer token buffer handling.
*
****************************************************************************/


#include "cvars.h"
#include "cscanbuf.h"
#include "frontrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CscanbufState: cscanbuf_state.h) */

}  // namespace



static void GrowBuffer( FrontRun& front, size_t size )
/**************************************/
{
    char    *newBuffer;

    size = RoundUp( size, BUF_SIZE );
    newBuffer = static_cast<char *>(CMemAlloc( front, size ));
    memcpy( newBuffer, front.cvars.Buffer, front.cscanbuf.BufferSize );
    CMemFree( front, (void *)front.cvars.Buffer );
    front.cvars.Buffer = newBuffer;
    front.cscanbuf.BufferSize = size;
}

static CB_ALWAYS_INLINE void EnlargeBuffer( FrontRun& front, size_t size )
/********************************************************/
{
//    size += 32;     /* Buffer size margin */
    if( size > front.cscanbuf.BufferSize ) {
        GrowBuffer( front, size );
    }
}

void WriteBufferChar( FrontRun& front, int c )
/***************************/
{
    EnlargeBuffer( front, front.cvars.TokenLen + 1 );
    front.cvars.Buffer[front.cvars.TokenLen++] = (char)c;
}

int WriteBufferCharNextChar( FrontRun& front, int c )
/**********************************/
{
    EnlargeBuffer( front, front.cvars.TokenLen + 1 );
    front.cvars.Buffer[front.cvars.TokenLen++] = (char)c;
    return( front.cvars.NextChar(front) );
}

void WriteBufferNullChar( FrontRun& front )
/******************************/
{
    EnlargeBuffer( front, front.cvars.TokenLen + 1 );
    front.cvars.Buffer[front.cvars.TokenLen] = '\0';
}

void WriteBufferString( FrontRun& front, const char *s )
/*************************************/
{
    char    c;

    EnlargeBuffer( front, front.cvars.TokenLen + strlen( s ) + 1 );
    while( (c = *s++) != '\0' ) {
        front.cvars.Buffer[front.cvars.TokenLen++] = c;
    }
    front.cvars.Buffer[front.cvars.TokenLen] = '\0';
}

void WriteBufferEscStr( FrontRun& front, const char **src, bool quote )
/****************************************************/
{
    const char  *p;
    char        c;

    p = *src;
    while( (c = *p++) != '\0' ) {
        EnlargeBuffer( front, front.cvars.TokenLen + 1 );
        if( c == '\\' || quote && c == '"' ) {
            front.cvars.Buffer[front.cvars.TokenLen++] = '\\';
            EnlargeBuffer( front, front.cvars.TokenLen + 1 );
        }
        front.cvars.Buffer[front.cvars.TokenLen++] = c;
    }
    *src = p;
    EnlargeBuffer( front, front.cvars.TokenLen + 1 );
    front.cvars.Buffer[front.cvars.TokenLen] = '\0';
}

void InitBuffer( FrontRun& front, size_t size )
/****************************/
{
    front.cvars.Buffer = static_cast<char *>(CMemAlloc( front, size ));
    front.cscanbuf.BufferSize = size;
}

void FiniBuffer( FrontRun& front )
/*********************/
{
    CMemFree( front, front.cvars.Buffer );
    front.cvars.Buffer = NULL;
    front.cscanbuf.BufferSize = 0;
}
