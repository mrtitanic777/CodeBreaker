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
* Description:  Prototypes for front end callbacks.
*
****************************************************************************/


CGCALLBACKDEF( FESegID,         segment_id,     (FrontRun& front, cg_sym_handle) )
CGCALLBACKDEF( FEBack,          back_handle,    (FrontRun& front, cg_sym_handle) )
CGCALLBACKDEF( FEAttr,          fe_attr,        (FrontRun& front, cg_sym_handle) )
CGCALLBACKDEF( FEAuxInfo,       pointer,        (FrontRun& front, pointer,aux_class) )
CGCALLBACKDEF( FEName,          cchar_ptr,      (FrontRun& front, cg_sym_handle) )
CGCALLBACKDEF( FEModuleName,    cchar_ptr,      (FrontRun& front) )
CGCALLBACKDEF( FEMessage,       void,           (FrontRun& front, fe_msg,pointer) )
CGCALLBACKDEF( FEMoreMem,       int,            (size_t) )
CGCALLBACKDEF( FETrue,          int,            (void) )
CGCALLBACKDEF( FEGenProc,       void,           (FrontRun& front, cg_sym_handle,call_handle) )
CGCALLBACKDEF( FELexLevel,      int,            (cg_sym_handle) )
CGCALLBACKDEF( FEDbgType,       dbg_type,       (FrontRun& front, cg_sym_handle) )
CGCALLBACKDEF( FEDbgRetType,    dbg_type,       (FrontRun& front, cg_sym_handle) )
CGCALLBACKDEF( FEStackChk,      int,            (FrontRun& front, cg_sym_handle) )
CGCALLBACKDEF( FEGetEnv,        cchar_ptr,      (FrontRun& front, cchar_ptr) )
CGCALLBACKDEF( FEExtName,       cchar_ptr,      (FrontRun& front, cg_sym_handle,int) )
CGCALLBACKDEF( FEParmType,      cg_type,        (FrontRun& front, cg_sym_handle,cg_sym_handle,cg_type) )
