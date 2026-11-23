/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2018 The Open Watcom Contributors. All Rights Reserved.
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


#pragma once
#include "cgmem.h"
#include "_cg.h"
#include "bckdef.h"
#include "ocentry.h"
#include "optlbl.h"
#include "optq.h"
#include "optclass.h"
#include "optsave.h"
#include "optmac.h"

inline bool TransferClass( oc_class var ) { return ((var) >= OC_JMP); }

inline oc_class OcClass( ins_entry *var ) { return static_cast<oc_class>(OcBaseClass(((ins_entry *)(var))->oc.oc_header.class_)); }

inline int GetAttr( ins_entry *var ) { return OcInfoClass(((ins_entry *)(var))->oc.oc_header.class_); }

inline bool ChkAttr( ins_entry *v1, oc_class v2 ) { return ((((ins_entry *)(v1))->oc.oc_header.class_ & v2) != 0); }

inline void SetAttr( ins_entry *v1, int v2 )
{
    ((ins_entry *)(v1))->oc.oc_header.class_ |= static_cast<oc_class>( v2 );
}

inline void ChgClass( ins_entry *v1, oc_class v2 )
{
    ((ins_entry *)(v1))->oc.oc_header.class_ = static_cast<oc_class>(GetAttr(v1) | v2);
}

inline void ResetClass( ins_entry *v1, oc_class v2 ) { ((ins_entry *)(v1))->oc.oc_header.class_ = v2; }

inline code_lbl *&LabelOf( ins_entry *var ) { return (((ins_entry *)(var))->oc.oc_handle.handle); }

inline ins_entry *&LblRef( ins_entry *var ) { return (((ins_entry *)(var))->oc.oc_handle.ref); }

inline unsigned char &JmpCond( ins_entry *var ) { return (((ins_entry *)(var))->oc.oc_jcond.cond); }

inline unsigned int &RetPop( ins_entry *var ) { return (((ins_entry *)(var))->oc.oc_ret.pops); }

inline unsigned char &ObjLen( ins_entry *var ) { return (((ins_entry *)(var))->oc.oc_header.objlen); }

inline unsigned char &InsLen( ins_entry *var ) { return (((ins_entry *)(var))->oc.oc_header.reclen); }

inline oc_class &ClassInfo( ins_entry *var ) { return (((ins_entry *)(var))->oc.oc_header.class_); }
#include "optdata.h"
#include "hwreg.h"
#include "typclass.h"
#include "blocknum.h"
#include "name.h"
#include "opcodes.h"
