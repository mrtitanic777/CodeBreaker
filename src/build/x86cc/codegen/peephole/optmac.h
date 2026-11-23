#pragma once
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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


/* a function of the optimizer is entered with OptBegin and left with OptEnd (before a return: `{ OptEnd( back ); return( x ); }`); the bodies are in backrun.hpp */
struct BackRun;
inline void OptBegin( BackRun& back );
inline void OptEnd( BackRun& back );

#define _TransferClass(var) ((var) >= OC_JMP)
//#define _TransferClass(var) ((var) == OC_JMP || (var) == OC_JMPI || (var) == OC_RET || (var) == OC_NORET || (var) == OC_IDATA)

#define _Class(var)         static_cast<oc_class>(OC_BASE_CLASS(((ins_entry *)(var))->oc.oc_header.class_))
#define _GetAttr(var)       OC_INFO_CLASS(((ins_entry *)(var))->oc.oc_header.class_)
#define _ChkAttr(v1,v2)     ((((ins_entry *)(v1))->oc.oc_header.class_ & v2) != 0)
#define _SetAttr(v1,v2)     ((ins_entry *)(v1))->oc.oc_header.class_ |= static_cast<oc_class>( v2 )
#define _ChgClass(v1,v2)    ((ins_entry *)(v1))->oc.oc_header.class_ = static_cast<oc_class>(_GetAttr(v1) | v2)
#define _ResetClass(v1,v2)  ((ins_entry *)(v1))->oc.oc_header.class_ = v2

#define _Label(var)         (((ins_entry *)(var))->oc.oc_handle.handle)
#define _LblRef(var)        (((ins_entry *)(var))->oc.oc_handle.ref)
#define _LblLine(var)       (((ins_entry *)(var))->oc.oc_handle.line)
#define _JmpCond(var)       (((ins_entry *)(var))->oc.oc_jcond.cond)
#define _RetPop(var)        (((ins_entry *)(var))->oc.oc_ret.pops)
#define _ObjLen(var)        (((ins_entry *)(var))->oc.oc_header.objlen)
#define _InsLen(var)        (((ins_entry *)(var))->oc.oc_header.reclen)
#define _ClassInfo(var)     (((ins_entry *)(var))->oc.oc_header.class_)

/*  the next size seems to cover almost all optimizer entries that get allocated */
#define OCENTRY_SIZE        (sizeof( ins_entry ))


#ifndef CB_STATE_OPTMAC_H
#define CB_STATE_OPTMAC_H

/* the variables of optmac.h, one set for each run of the compiler (run/cbrun.hpp) */
struct OptmacState {
    int InOptimizer;
};

#endif
