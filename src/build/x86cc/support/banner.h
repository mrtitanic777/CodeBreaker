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
* Description:  Open Watcom banner strings and version defines.
*
****************************************************************************/


#pragma once
#define _BETAVER            1
#define _OWURL              "https://github.com/open-watcom/open-watcom-v2#readme"

#define _DOSTR( p )         #p
#define _MACROSTR( p )      _DOSTR( p )

#define _BETASTR_           "beta"

#define _BETA_              " " _BETASTR_

#define _BANEXTRA           __DATE__ " " __TIME__ STR_BITNESS

#define banner1t(p)         "Open Watcom " p
#define banner1v(v)         "Version " v " " _BANEXTRA


#define banner21            "Copyright (c) 2002-" _MACROSTR( _CYEAR ) " The Open Watcom Contributors."
#define banner21a(year)     "Portions Copyright (c) " _DOSTR( year ) "-2002 Sybase, Inc."

#define banner2             banner21 " All Rights Reserved."
#define banner2a(year)      banner21a(year) " All Rights Reserved."

#define banner3             "Source code is available under the Sybase Open Watcom Public License."
#define banner3a            "See " _OWURL " for details."


  #define STR_BITNESS " (64-bit)"

// the following macros define the delimeters used by the resource
// compiler when concatenating strings

    #define BAN_VER_STR "2.0" _BETA_

/* these should all be _BLDVER/100.0 */
#define _WCC_VERSION_           BAN_VER_STR
/* these can be what ever they want to be */


/*
 * Java Tools
 */


/*
 * Versions of Microsoft tools with OW clones are compatible
 */
