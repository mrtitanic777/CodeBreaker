/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  front end calling info structure and initialization
*
****************************************************************************/

#ifndef __CALLINFO_H__
#define __CALLINFO_H__


#include "cgauxcc.h"
#include "auxflags.h"

typedef struct aux_info {
        call_class          cclass;
        call_class_target   cclass_target;
        byte_seq            *code;          // also used by pre-compiled header
        hw_reg_set          *parms;         // also used by pre-compiled header
        hw_reg_set          returns;
        hw_reg_set          streturn;
        hw_reg_set          save;
        char                *objname;       // also used by pre-compiled header
        unsigned            use;            // use count
        aux_flags           flags;
        unsigned            index;          // for C pre-compiled header
} aux_info;

#define DefaultInfo      front.callinfo.BuiltinAuxInfo[0]
#define WatcallInfo      front.callinfo.BuiltinAuxInfo[1]
#define CdeclInfo        front.callinfo.BuiltinAuxInfo[2]
#define PascalInfo       front.callinfo.BuiltinAuxInfo[3]
#define FortranInfo      front.callinfo.BuiltinAuxInfo[4]
#define SyscallInfo      front.callinfo.BuiltinAuxInfo[5]
#define OptlinkInfo      front.callinfo.BuiltinAuxInfo[6]
#define StdcallInfo      front.callinfo.BuiltinAuxInfo[7]
#define FastcallInfo     front.callinfo.BuiltinAuxInfo[8]
  #if _CPU == 386
#define Far16CdeclInfo   front.callinfo.BuiltinAuxInfo[9]
#define Far16PascalInfo  front.callinfo.BuiltinAuxInfo[10]
  #endif

  #if _CPU == 386
#define MAX_BUILTIN_AUXINFO (9 + 2)
  #else
#define MAX_BUILTIN_AUXINFO 9
  #endif


extern void         AuxInfoInit( FrontRun& front, int );
extern void         SetAuxStackConventions( FrontRun& front );
extern void         SetAuxWatcallInfo( FrontRun& front );
extern void         SetDefaultAuxInfo( FrontRun& front );
extern int          IsAuxParmsBuiltIn( FrontRun& front, hw_reg_set *parms );
extern int          IsAuxInfoBuiltIn( FrontRun& front, aux_info *inf );
extern char         *VarNamePattern( FrontRun& front, aux_info *inf );


/* the variables of callinfo.h, one set for each run of the compiler (run/cbrun.hpp) */
struct CallinfoState {
    aux_info *DftCallConv;
    aux_info BuiltinAuxInfo[MAX_BUILTIN_AUXINFO];
};

#endif
