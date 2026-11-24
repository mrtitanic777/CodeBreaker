/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2017-2025 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
* Description:  Windows NT FILETIME <-> time_t conversion routines
*
****************************************************************************/


#pragma once
#define WINDOWS_TICK_PER_SEC    10000000LLU
#define TICK_TO_UNIX_EPOCH      116444736000000000LLU

extern time_t   __NT_filetime_to_timet( const FILETIME *ft );
