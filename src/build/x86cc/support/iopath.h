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
* Description:  Host file system conventions macros.
*
****************************************************************************/


#pragma once
 #define DIR_SEP                '\\'
 #define DRIVE_SEP              ':'
 #define PATH_LIST_SEP          ';'
inline bool IsDirSep( char c ) { return ((c) == DIR_SEP || (c) == '/'); }
inline bool IsPathSep( char c ) { return (IsDirSep( c ) || c == DRIVE_SEP); }
 // '\foo.txt' is absolute, so is 'c:\foo.txt', but 'c:foo.txt' is not
inline bool HasPath( const char *p ) { return (IsDirSep( (p)[0] ) || (p)[0] != '\0' && (p)[1] == DRIVE_SEP); }
inline bool IsPathListSep( char c ) { return ((c) == PATH_LIST_SEP); }
inline bool IsIncludeListSep( char c ) { return IsPathListSep( c ); }
