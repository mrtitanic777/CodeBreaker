/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Structures and functions to manipulate labels.
*
****************************************************************************/


#pragma once
#include "optopts.h"
#include "offset.h"
#include "rtclass.h"
#include "objrep.h"


inline symbol *RtIdxToSym( rt_class i ) { return ((cg_sym_handle)(pointer_uint)((i) + 1)); }
inline int SymToRtIdx( symbol *s ) { return ((rt_class)(pointer_uint)(s) - 1); }

inline int SymToImpHdl( symbol *s ) { return ((import_handle)(pointer_uint)(s)); }
inline symbol *ImpHdlToSym( int i ) { return ((cg_sym_handle)(pointer_uint)(i)); }

typedef enum {
    CODELABEL           = 0x0001,
    KEEPLABEL           = 0x0002,
    DYINGLABEL          = 0x0004,
    REACHED             = 0x0008,
    SHORTREACH          = 0x0010,
    CONDEMNED           = 0x0020,
    RUNTIME             = 0x0040,
    REDIRECTION         = 0x0080,
    UNIQUE              = 0x0100,
    COMMON_LBL          = 0x0200,
    UNREACHABLE         = 0x0400,
    OWL_OWNED           = 0x0800,
    WEIRD_PPC_ALIAS     = 0x1000,
    HAS_PPC_ALIAS       = 0x2000,
    PROCEDURE           = 0x4000,
} status_bits;

typedef struct label_def {
    label_handle            link;
    obj_patch               *patches;
    cg_sym_handle           sym;
    offset                  address;
    status_bits             status;
} label_def;

typedef struct code_lbl {
    struct label_def        lbl;
    struct code_lbl         *alias;
    struct ins_entry        *ins;
    struct ins_entry        *refs;
    struct code_lbl         *redirect;
#if _TARGET_RISC || (_TARGET & _TARG_80386)
    void                    *owl_symbol;
#endif
} code_lbl;

inline void SetStatus( code_lbl *var, status_bits stat ) { var->lbl.status |= (stat); }
inline void ClrStatus( code_lbl *var, status_bits stat ) { var->lbl.status &= ~(stat); }
inline bool TstStatus( code_lbl *var, status_bits stat ) { return ((var->lbl.status & (stat)) != 0); }


extern bool             ValidLbl( label_handle lbl );

extern label_handle     AskForNewLabel( BackRun& back );
extern label_handle     AskForRTLabel( BackRun& back, rt_class rtindex );
extern label_handle     AskForLabel( BackRun& back, cg_sym_handle sym );
extern bool             AskIfReachedLabel( BackRun& back, label_handle lbl );
extern bool             AskIfRTLabel( BackRun& back, label_handle lbl );
extern bool             AskIfUniqueLabel( BackRun& back, label_handle lbl );
extern bool             AskIfCommonLabel( BackRun& back, label_handle lbl );
extern offset           AskAddress( BackRun& back, label_handle lbl );
extern pointer          AskLblPatches( BackRun& back, label_handle lbl );
extern cg_sym_handle    AskForLblSym( BackRun& back, label_handle lbl );

extern void             AddLblDef( BackRun& back, ins_entry *instr );
extern void             DelLblDef( BackRun& back, ins_entry *instr );
extern void             AddLblRef( BackRun& back, ins_entry *instr );
extern void             DelLblRef( BackRun& back, ins_entry *instr );
extern void             ChgLblRef( BackRun& back, ins_entry *instr, label_handle new_lbl );
extern bool             UniqueLabel( BackRun& back, label_handle lbl );
extern ins_entry        *AliasLabels( BackRun& back, ins_entry *oldlbl, ins_entry *new_ins );
extern void             TryScrapLabel( BackRun& back, label_handle old );
extern void             ScrapCodeLabel( BackRun& back, label_handle lbl );
