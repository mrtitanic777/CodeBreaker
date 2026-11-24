#pragma once
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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "cgswitch.h"


#ifndef CB_STATE_MODEL_H
#define CB_STATE_MODEL_H

/* the variables of model.h, one set for each run of the compiler (run/cbrun.hpp) */
struct ModelState {
    cg_switches Model;
    cg_target_switches TargetModel;
    cg_target_switches SaveTargetModel;
    proc_revision CGProcessorVersion;
};

#endif

/* what the run's code generation model says. They read `back.model`, so their bodies are in backrun.hpp, after BackRun. */
struct BackRun;
inline bool IsModel( const BackRun& back, cg_switches mdl );
inline bool IsntModel( const BackRun& back, cg_switches mdl );
inline bool IsTargetModel( const BackRun& back, cg_target_switches mdl );
inline bool IsntTargetModel( const BackRun& back, cg_target_switches mdl );
inline bool CPULevel( const BackRun& back, proc_revision level );
inline bool FPULevel( const BackRun& back, proc_revision level );
inline bool IsEmulation( const BackRun& back );
