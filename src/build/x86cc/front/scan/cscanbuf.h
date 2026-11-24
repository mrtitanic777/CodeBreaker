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


#pragma once
extern void     WriteBufferChar( FrontRun& front, int c );
extern void     WriteBufferNullChar( FrontRun& front );
extern int      WriteBufferCharNextChar( FrontRun& front, int c );
extern void     WriteBufferString( FrontRun& front, const char *s );
extern void     WriteBufferEscStr( FrontRun& front, const char **src, bool quote );
extern void     InitBuffer( FrontRun& front, size_t size );
extern void     FiniBuffer( FrontRun& front );
