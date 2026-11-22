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


namespace {

/* the statics of this file, one set for each run of the compiler */
struct CscanbufState {
    size_t BufferSize;
};

}  // namespace

static inline CscanbufState& cscanbuf_state() { return cbrun_part<CscanbufState>(); }


static void EnlargeBuffer( size_t size )
/**************************************/
{
    char    *newBuffer;

//    size += 32;     /* Buffer size margin */
    if( size > cscanbuf_state().BufferSize ) {
        size = _RoundUp( size, BUF_SIZE );
        newBuffer = static_cast<char *>(CMemAlloc( size ));
        memcpy( newBuffer, cvars_state().Buffer, cscanbuf_state().BufferSize );
        CMemFree( (void *)cvars_state().Buffer );
        cvars_state().Buffer = newBuffer;
        cscanbuf_state().BufferSize = size;
    }
}

void WriteBufferChar( int c )
/***************************/
{
    EnlargeBuffer( cvars_state().TokenLen + 1 );
    cvars_state().Buffer[cvars_state().TokenLen++] = (char)c;
}

int WriteBufferCharNextChar( int c )
/**********************************/
{
    EnlargeBuffer( cvars_state().TokenLen + 1 );
    cvars_state().Buffer[cvars_state().TokenLen++] = (char)c;
    return( cvars_state().NextChar() );
}

void WriteBufferNullChar( void )
/******************************/
{
    EnlargeBuffer( cvars_state().TokenLen + 1 );
    cvars_state().Buffer[cvars_state().TokenLen] = '\0';
}

void WriteBufferString( const char *s )
/*************************************/
{
    char    c;

    EnlargeBuffer( cvars_state().TokenLen + strlen( s ) + 1 );
    while( (c = *s++) != '\0' ) {
        cvars_state().Buffer[cvars_state().TokenLen++] = c;
    }
    cvars_state().Buffer[cvars_state().TokenLen] = '\0';
}

void WriteBufferEscStr( const char **src, bool quote )
/****************************************************/
{
    const char  *p;
    char        c;

    p = *src;
    while( (c = *p++) != '\0' ) {
        EnlargeBuffer( cvars_state().TokenLen + 1 );
        if( c == '\\' || quote && c == '"' ) {
            cvars_state().Buffer[cvars_state().TokenLen++] = '\\';
            EnlargeBuffer( cvars_state().TokenLen + 1 );
        }
        cvars_state().Buffer[cvars_state().TokenLen++] = c;
    }
    *src = p;
    EnlargeBuffer( cvars_state().TokenLen + 1 );
    cvars_state().Buffer[cvars_state().TokenLen] = '\0';
}

void InitBuffer( size_t size )
/****************************/
{
    cvars_state().Buffer = static_cast<char *>(CMemAlloc( size ));
    cscanbuf_state().BufferSize = size;
}

void FiniBuffer( void )
/*********************/
{
    CMemFree( cvars_state().Buffer );
    cvars_state().Buffer = NULL;
    cscanbuf_state().BufferSize = 0;
}
