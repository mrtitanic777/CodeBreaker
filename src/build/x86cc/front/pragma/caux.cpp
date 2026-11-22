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
* Description:  Management of #pragma aux information.
*
****************************************************************************/


#include "cvars.h"
#include "pdefn2.h"
#include "caux.h"
#include "frontrun.hpp"


aux_entry *AuxLookup( FrontRun& front, const char *name )
{
    aux_entry   *ent;

    for( ent = front.pragdefn.AuxList; ent != NULL; ent = ent->next ) {
        if( strcmp( ent->name, name ) == 0 ) {
            break;
        }
    }
    return( ent );
}


static void FreeAuxInfo( FrontRun& front, aux_info *info )
{
    if( info->code != NULL ) {
        CMemFree( front, info->code );
        info->code = NULL;
    }
    if( !IsAuxParmsBuiltIn( front, info->parms ) ) {
        CMemFree( front, info->parms );
        info->parms = NULL;
    }
#if _CPU == 370
    if( info->linkage != &DefaultLinkage && info->linkage != &OSLinkage ) {
        if( info->linkage != NULL ) {
            CMemFree( info->linkage );
            info->linkage = NULL;
        }
    }
#endif
    if( info->objname != NULL ) {
        CMemFree( front, info->objname );
        info->objname = NULL;
    }
}

void PragmaAuxInit( FrontRun& front )
{
    CdeclInfo   = WatcallInfo;
    PascalInfo  = WatcallInfo;
    SyscallInfo = WatcallInfo;
    StdcallInfo = WatcallInfo;
    OptlinkInfo = WatcallInfo;
    FortranInfo = WatcallInfo;
    FastcallInfo= WatcallInfo;

#if _INTEL_CPU
    AuxInfoInit( front, front.cvars.CompFlags.use_stdcall_at_number );
#endif

    SetDefaultAuxInfo(front);
}

void PragmaAuxFini( FrontRun& front )
{
    void    *junk;

    while( front.pragdefn.AuxList != NULL ) {
        junk = front.pragdefn.AuxList;
        if( front.pragdefn.AuxList->info != NULL ) {
            if( front.pragdefn.AuxList->info->use > 1 ) {
                front.pragdefn.AuxList->info->use--;
            } else {
                FreeAuxInfo( front, front.pragdefn.AuxList->info );
                if( !IsAuxInfoBuiltIn( front, front.pragdefn.AuxList->info ) ) {
                    CMemFree( front, front.pragdefn.AuxList->info );
                }
            }
        }
        front.pragdefn.AuxList = front.pragdefn.AuxList->next;
        CMemFree( front, junk );
    }

    FreeAuxInfo( front, &DefaultInfo );
    FreeAuxInfo( front, &WatcallInfo );
    FreeAuxInfo( front, &CdeclInfo );
    FreeAuxInfo( front, &PascalInfo );
    FreeAuxInfo( front, &SyscallInfo );
    FreeAuxInfo( front, &OptlinkInfo );
    FreeAuxInfo( front, &FortranInfo );
    FreeAuxInfo( front, &StdcallInfo );
    FreeAuxInfo( front, &FastcallInfo );
#if _CPU == 386
    FreeAuxInfo( front, &Far16CdeclInfo );
    FreeAuxInfo( front, &Far16PascalInfo );
#endif
}
