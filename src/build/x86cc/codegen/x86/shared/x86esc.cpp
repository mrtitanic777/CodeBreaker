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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "addrname.h"
#include "symdbg.h"
#include "system.h"
#include "model.h"
#include "jumps.h"
#include "zoiks.h"
#include "x86objd.h"
#include "objout.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "dbsyms.h"
#include "objio.h"
#include "optmain.h"
#include "opttell.h"
#include "intrface.h"
#include "x86obj.h"
#include "jumpsout.h"
#include "x86esc.h"
#include "encode.h"
#include "pccode.h"
#include "pcencode.h"
#include "x86enc.h"
#include "x86nopli.h"
#include "feprotos.h"
#include "backrun.hpp"


#define OC_INFO_NOT_DEBUG   static_cast<oc_class>(OC_INFO_SELECT)

static void     DoRelocRef( BackRun& back, cg_sym_handle sym, cg_class class_, segment_id segid, offset val, escape_class esc_attr );
static void     OutShortDisp( BackRun& back, label_handle lbl );
static void     OutCodeDisp( BackRun& back, label_handle lbl, fix_class f, bool rel, oc_class class_ );

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct X86escState {
    oc_class SaveDbgOc = static_cast<oc_class>(OC_INFO_NOT_DEBUG);
    pointer SaveDbgPtr;
    offset LastUnique = ADDR_UNKNOWN;
};
namespace {

}  // namespace



bool    CodeHasAbsPatch( oc_entry *code )
/***************************************/
{
    byte        *curr;
    byte        *final;

    curr = code->data;
    final = curr + code->hdr.reclen - offsetof( oc_entry, data );
    while( curr < final ) {
        if( *curr++ == ESC ) {
            if( *curr++ == ESC_ABS ) {
                return( true );
            }
        }
    }
    return( false );
}


void    DoAbsPatch( BackRun& back, abspatch_handle *handle, int len )
/****************************************************/
{
    EmitByte( back, ESC );
    EmitByte( back, ESC_ABS );
    if( len == 1 ) {
        EmitByte( back, BYTE_PATCH );
    } else if( len == 2 ) {
        EmitByte( back, WORD_PATCH );
    } else { /*4*/
        EmitByte( back, LONG_PATCH );
    }
    *handle = NewAbsPatch(back);
    EmitPtr( back, *handle );
}


void  DoFESymRef( BackRun& back, cg_sym_handle sym, cg_class class_, offset val, fe_fixup_types fixup_type )
/******************************************************************************************/
{
    fe_attr             attr;
    escape_class        esc_attr;

    esc_attr = static_cast<escape_class>(0);
    switch( fixup_type ) {
    case FE_FIX_BASE:
        val = 0;
        esc_attr |= ESCA_BASE;  /* base segment relocation */
        break;
    case FE_FIX_SELF:
        esc_attr |= ESCA_SELF;  /* self relative offset relocation */
        break;
    }
    if( val != 0 ) {
        esc_attr |= ESCA_OFST;  /* offset follows*/
    }
    if( class_ == CG_FE ) {
        attr = FEAttr( *back.front, sym );
        if( ((esc_attr & ESCA_BASE) == 0)
          && IsTargetModel( back, CGSW_X86_WINDOWS )
          && (attr & FE_PROC)
          && (static_cast<call_class_target>(reinterpret_cast<pointer_uint>(FindAuxInfoSym( back, sym, FEINF_CALL_CLASS_TARGET ))) & FECALL_X86_FAR_CALL) ) {
            esc_attr |= ESCA_LDOF;
        }
        if( UseImportForm( back, attr ) ) {
            EmitByte( back, ESC );
            EmitByte( back, ESC_IMP | esc_attr );
            EmitPtr( back, sym );
            if( esc_attr & ESCA_OFST ) {
                EmitOffset( back, val );
            }
        } else if( attr & FE_GLOBAL ) {
            DoRelocRef( back, sym, CG_FE, AskSegID( back, sym, CG_FE ), val, esc_attr );
        } else {
            // handle mismatch Fix it!
            DoRelocRef( back, reinterpret_cast<cg_sym_handle>(AskForSymLabel( back, sym, CG_FE )), CG_LBL, AskSegID( back, sym, CG_FE ), val, esc_attr );
        }
    } else {                                /* CG_TBL, CG_LBL or CG_BCK*/
        DoRelocRef( back, sym, class_, AskSegID( back, sym, class_ ), val, esc_attr );
    }
}


void    DoSymRef( BackRun& back, name *opnd, offset val, bool base )
/***************************************************/
{
    DoFESymRef( back, static_cast<cg_sym_handle>(opnd->v.symbol), opnd->m.memory_type, val, base ? FE_FIX_BASE : FE_FIX_OFF );
}


void    DoSegRef( BackRun& back, segment_id segid )
/**********************************/
{
    EmitByte( back, ESC );
    EmitByte( back, ESC_REL | ESCA_BASE | ESCA_OFST );
    EmitSegId( back, segid );
    EmitOffset( back, 0 );
}

static void     DoRelocRef( BackRun& back, cg_sym_handle sym, cg_class class_,
                    segment_id segid, offset val, escape_class esc_attr )
/***********************************************************************/
{
    offset              addr;
    label_handle        lbl;

    if( esc_attr & ESCA_BASE ) {                /* don't need offset*/
        EmitByte( back, ESC );
        EmitByte( back, ESC_REL | esc_attr | ESCA_OFST );
        EmitSegId( back, segid );
        EmitOffset( back, 0 );
    } else {
        lbl = AskForSymLabel( back, sym, class_ );
        addr = AskAddress( back, lbl );
        if( (addr != ADDR_UNKNOWN) && !AskIfCommonLabel( back, lbl ) ) {
            EmitByte( back, ESC );
            EmitByte( back, ESC_REL | esc_attr | ESCA_OFST );
            EmitSegId( back, segid );
            val += addr;
            EmitOffset( back, val );
        } else if( class_ == CG_FE ) {
            EmitByte( back, ESC );
            EmitByte( back, ESC_SYM | esc_attr );     /* patch may be needed*/
            EmitPtr( back, sym );
            if( esc_attr & ESCA_OFST ) {
                EmitOffset( back, val );
            }
        } else {
            DoLblRef( back, lbl, segid, val, esc_attr );
        }
    }
}

void    DoLblRef( BackRun& back, label_handle lbl, segment_id segid,
                    offset val, escape_class esc_attr )
/*****************************************************/
{
    EmitByte( back, ESC );
    EmitByte( back, ESC_LBL | esc_attr );
    EmitSegId( back, segid );
    EmitPtr( back, lbl );
    if( esc_attr & ESCA_OFST ) {
        EmitOffset( back, val );
    }
}

static void SendBytes( BackRun& back, const byte *ptr, unsigned len )
/****************************************************/
{
    if( len != 0 ) {
        OutDBytes( back, len, ptr );
    }
}

static  void    DumpSavedDebug( BackRun& back )
/******************************/
{
    switch( back.x86esc->SaveDbgOc ) {
    case OC_INFO_DBG_RTN_BEG:
        DbgRtnBeg( back, static_cast<dbg_rtn *>(back.x86esc->SaveDbgPtr), AskLocation(back) );
        break;
    case OC_INFO_DBG_BLK_BEG:
        DbgBlkBeg( back, static_cast<dbg_block *>(back.x86esc->SaveDbgPtr), AskLocation(back) );
        break;
    case OC_INFO_DBG_EPI_BEG:
        DbgEpiBeg( back, static_cast<dbg_rtn *>(back.x86esc->SaveDbgPtr), AskLocation(back) );
        break;
    }
    back.x86esc->SaveDbgOc = OC_INFO_NOT_DEBUG;
}

void DoAlignment( BackRun& back, int len )
/*************************/
{
    const byte          *ptr;
    int                 nop;
    int                 i;
    unsigned            save_line;

    save_line = SavePendingLine( back, 0 );
    if( IsTargetModel( back, CGSW_X86_MSC8 ) ) {
        /* Microsoft C 8 pads with NOP, a byte at a time (-zmsc) */
        static const byte nop = 0x90;
        for( ; len > 0; --len )
            SendBytes( back, &nop, 1 );
        SavePendingLine( back, save_line );
        return;
    }
    /* get the size of the largest NOP pattern  */
    i = 0;
    for( nop = NopLists[i][0]; len > nop; len -= nop ) {
        SendBytes( back, &NopLists[i][1], nop );
        i = !i;
    }
    /* find the correct size NOP pattern to use */
    for( ptr = &NopLists[i][1]; nop != len; --nop )
        ptr += nop;
    SendBytes( back, ptr, len );
    SavePendingLine( back, save_line );
}


static  void    ExpandCJ( BackRun& back, any_oc *oc )
/************************************/
{
    oc_class            class_;
    fix_class           f;
    bool                rel;
    obj_length          objlen;

    class_ = oc->oc_header.class_;
    objlen = oc->oc_header.objlen;
    if( OcBaseClass( class_ ) == OC_JCOND ) {
        if( objlen == OptInsSize( back, OC_JCOND, OC_DEST_NEAR ) ) {
            if( CPULevel( back, CPU_386 ) ) {
                OutJCondNear( back, oc->oc_jcond.cond );
            } else {
                OutJCond( back, ReverseCondition( oc->oc_jcond.cond ) );
                OutDataByte( back, OptInsSize( back, OC_JMP, OC_DEST_NEAR ) );
                OutJNear( back );
            }
            OutCodeDisp( back, oc->oc_jcond.handle, F_OFFSET, true, class_ );
        } else {
            OutJCond( back, oc->oc_jcond.cond );
            OutShortDisp( back, oc->oc_jcond.handle );
        }
    } else if( OcBaseClass( class_ ) == OC_JMP && objlen == OptInsSize( back, OC_JMP, OC_DEST_SHORT ) ) {
        OutJShort( back );
        OutShortDisp( back, oc->oc_handle.handle );
    } else {
        if( class_ & OC_ATTR_FAR ) {
            f = F_PTR;
            rel = false;
            if( OcBaseClass( class_ ) == OC_CALL ) {
                if( objlen == OptInsSize( back, OC_CALL, OC_DEST_CHEAP ) ) {
                    f = F_OFFSET;
                    rel = true;
                    class_ &= ~ OC_ATTR_FAR;
                    OutCCyp( back );
                } else {
                    OutCFar( back );
                }
            } else {
                OutJFar( back );
            }
        } else {
            f = F_OFFSET;
            rel = true;
            if( OcBaseClass( class_ ) == OC_CALL ) {
                OutCNear( back );
            } else {
                OutJNear( back );
            }
        }
        OutCodeDisp( back, oc->oc_handle.handle, f, rel, class_ );
    }
}


static  void    OutShortDisp( BackRun& back, label_handle lbl ) {
/************************************************/

    offset              addr;

    addr = AskAddress( back, lbl );
    if( addr == ADDR_UNKNOWN ) {
        OutPatch( back, lbl, _SHORT_PATCH );
        addr = 0;
    }
    OutShortD( back, addr );
}


static  void    OutCodeDisp( BackRun& back, label_handle lbl, fix_class f,
                             bool rel, oc_class class_ ) {
/********************************************************/

    offset              addr;
    cg_sym_handle       sym;

    sym = AskForLblSym( back, lbl );
    if( AskIfRTLabel( back, lbl ) ) {
        OutRTImport( back, static_cast<rt_class>(SymToRtIdx( sym )), f );
        if( class_ & OC_ATTR_FAR ) {
            OutFarD( back, 0, 0 );
        } else {
            OutFarOff( back, 0 );
        }
    } else if( AskIfCommonLabel( back, lbl ) ) {
        OutSpecialCommon( back, SymToImpHdl( sym ), f, rel );
        OutFarOff( back, 0 );
    } else if( sym != NULL && UseImportForm( back, FEAttr( *back.front, sym ) ) ) { /* 90-05-22 */
        OutImport( back, sym, f, rel );
        if( class_ & OC_ATTR_FAR ) {
            OutFarD( back, 0, 0 );
        } else {
            OutFarOff( back, 0 );
        }
    } else {                /* patch, to be done later*/
        addr = AskAddress( back, lbl );
        if( ( class_ & OC_ATTR_FAR ) == 0 ) {
            if( addr == ADDR_UNKNOWN ) {
                OutPatch( back, lbl, _NEAR_PATCH );
                addr = 0;
            }
            OutNearD( back, addr );
        } else {
            if( addr == ADDR_UNKNOWN ) {
                OutPatch( back, lbl, _OFFSET_PATCH );
            }
            OutFarOff( back, addr );
            OutReloc( back, AskSegID( back, sym, CG_FE ), F_BASE, false );
            OutFarSeg( back, 0 );
        }
    }
}


static  label_handle ExpandObj( BackRun& back, byte *cur, int explen )
/*****************************************************/
{
    byte                *fini;
    escape_class        esc_class;
    label_handle        lbl;
    cg_sym_handle       sym;
    offset              val = 0;
    segment_id          segid;
    fix_class           class_;
    bool                rel;
    unsigned            len;
    abs_patch           *apatch;

    lbl = NULL;
    fini = cur + explen;
    while( cur < fini ) {
        len = 0;
        while( cur[len] != ESC ) {
            ++len;
            if( cur + len >= fini ) {
                break;
            }
        }
        if( len != 0 ) {
            OutDBytes( back, len, cur );
            cur += len;
            if( cur >= fini ) {
                break;
            }
        }
        cur++;
        esc_class = static_cast<escape_class>(*cur++);
        if( esc_class == ESC ) {
            OutDataByte( back, ESC );
            continue;
        }
        if( esc_class & ESCA_BASE ) {
            class_ = F_BASE;
        } else if( esc_class & ESCA_LDOF ) {
            class_ = F_LDR_OFFSET;
        } else {
            class_ = F_OFFSET;
        }
        if( esc_class & ESCA_SELF ) {
            rel = true;
        } else {
            rel = false;
        }
        switch( esc_class & ~ESCA_MASK ) {
        case ESC_REL:
            memcpy( &segid, cur, sizeof( segment_id ) );
            cur += sizeof( segment_id );
            OutReloc( back, segid, class_, rel );
            val = 0;
            break;
        case ESC_SYM:       /* never BASE*/
            memcpy( &sym, cur, sizeof( pointer ) );
            cur += sizeof( pointer );
            lbl = AskForSymLabel( back, sym, CG_FE );
            val = AskAddress( back, lbl );
            if( val == ADDR_UNKNOWN ) {
                if( class_ == F_OFFSET || class_ == F_LDR_OFFSET ) {
                    OutPatch( back, lbl, ADD_PATCH | _OFFSET_PATCH );
                } else {
                    OutPatch( back, lbl, ADD_PATCH | WORD_PATCH );
                }
                val = 0;
            }
            OutReloc( back, AskSegID( back, sym, CG_FE ), class_, rel );
            break;
        case ESC_LBL:       /* never BASE*/
            memcpy( &segid, cur, sizeof( segment_id ) );
            cur += sizeof( segment_id );
            memcpy( &lbl, cur, sizeof( pointer ) );
            cur += sizeof( pointer );
            if( AskIfRTLabel( back, lbl ) ) {
                OutRTImportRel( back, static_cast<rt_class>(SymToRtIdx( AskForLblSym( back, lbl ) )), F_OFFSET, false );
                val = 0;
            } else {
                if( AskIfCommonLabel( back, lbl ) ) {
                    OutSpecialCommon( back, SymToImpHdl( AskForLblSym( back, lbl ) ), class_, rel );
                } else {
                    OutReloc( back, segid, class_, rel );
                }
                val = AskAddress( back, lbl );
                if( val == ADDR_UNKNOWN ) {
                    if( class_ == F_OFFSET || class_ == F_LDR_OFFSET ) {
                        OutPatch( back, lbl, ADD_PATCH | _OFFSET_PATCH );
                    } else {
                        OutPatch( back, lbl, ADD_PATCH | WORD_PATCH );
                    }
                    val = 0;
                }
            }
            break;
        case ESC_IMP:
            memcpy( &sym, cur, sizeof( pointer ) );
            cur += sizeof( pointer );
            OutImport( back, sym, class_, rel );
            val = 0;
            break;
        case ESC_ABS:
            val = *cur++;
            memcpy( &apatch, cur, sizeof( pointer ) );
            cur += sizeof( pointer );
            OutAbsPatch( back, apatch, static_cast<patch_attr>(val) );
            continue;
        case ESC_FUN:
            OutFPPatch( back, static_cast<fp_patches>(*cur) );
            cur++;
            continue;
        default:
            Zoiks( back, ZOIKS_038 );
            break;
        }
        if( esc_class & ESCA_OFST ) {
            offset  tmp;

            memcpy( &tmp, cur, sizeof( offset ) );
            val += tmp;
            cur += sizeof( offset );
        }
        if( class_ == F_BASE ) {
            OutFarSeg( back, val );
        } else if( class_ == F_OFFSET || class_ == F_LDR_OFFSET ) {
            OutFarOff( back, val );      /* offset */
        } else if( class_ == F_PTR ) {
            OutFarOff( back, val );
            OutFarSeg( back, 0 );        /* segment */
        }
    }
    return( lbl );
}

void    OutputOC( BackRun& back, any_oc *oc, any_oc *next_lbl )
/**********************************************/
{
    label_handle    lbl;
    cg_sym_handle   sym;
    oc_class        base;
    int             len;
    offset          lc;
    byte            *ptr;

    base = static_cast<oc_class>(OcBaseClass( oc->oc_header.class_ ));
    if( base == OC_NORET )
        return;
    if( base != OC_LABEL ) {
        DumpSavedDebug(back);
    }
    SetUpObj( back, false );
    switch( base ) {
    case OC_CODE:
    case OC_DATA:
        ExpandObj( back, oc->oc_entry.data, oc->oc_entry.hdr.reclen - offsetof( oc_entry, data ) );
        break;
    case OC_IDATA:
        if( next_lbl != NULL ) { /* cause next_lbl to need no alignment */
            len = -( AskLocation(back) + oc->oc_entry.hdr.objlen );
            len &= next_lbl->oc_entry.hdr.objlen;
            DoAlignment( back, len );
        }
        OutSelect( back, true );
        SendBytes( back, oc->oc_entry.data, oc->oc_entry.hdr.objlen );
        OutSelect( back, false );
        break;
    case OC_BDATA:
        SendBytes( back, oc->oc_entry.data, oc->oc_entry.hdr.objlen );
        break;
    case OC_LABEL:
        /* figure out number of bytes to pad */
        lc = AskLocation(back);
        len = -lc & oc->oc_handle.hdr.objlen;
        if( AskIfUniqueLabel( back, oc->oc_handle.handle ) ) {
            if( (lc == back.x86esc->LastUnique) && (len == 0) ) {
                /* Two unique labels have ended up next to each other.
                   Pad out to next label alignment boundry. */
                len = oc->oc_handle.hdr.objlen + 1;
            }
            back.x86esc->LastUnique = lc + len;
        }
        DoAlignment( back, len );
        OutLabel( back, oc->oc_handle.handle );           /* do patches*/
        DbgSetBase(back);
        DumpSavedDebug(back);
        break;
    case OC_LREF:
        lbl = oc->oc_handle.handle;
        sym = AskForLblSym( back, lbl );
        if( AskIfRTLabel( back, lbl ) ) {
            OutRTImport( back, static_cast<rt_class>(SymToRtIdx( sym )), F_OFFSET );
            lc = 0;
        } else if( AskIfCommonLabel( back, lbl ) ) {
            OutSpecialCommon( back, SymToImpHdl( sym ), F_OFFSET, false );
            lc = 0;
        } else if( sym != NULL && UseImportForm( back, FEAttr( *back.front, sym ) ) ) {
            OutImport( back, sym, F_OFFSET, false );
            lc = 0;
        } else {
            OutReloc( back, AskOP(back), F_OFFSET, false );
            lc = AskAddress( back, lbl );
            if( lc == ADDR_UNKNOWN ) {
                OutPatch( back, lbl, ADD_PATCH | _OFFSET_PATCH );
                lc = 0;
            }
        }
        OutFarOff( back, lc );
        break;
    case OC_JMPI:
    case OC_CALLI:
    case OC_JCONDI:
        ptr = oc->oc_entry.data;
        len = 0;
        while( ptr[len] != M_CJINEAR ) {
            ++len; /* skip over top of any prefixes */
        }
        ++len;
        OutOpndSize( back );
        ExpandObj( back, ptr, len );
        ptr += len;

    /* Need to put bits in 2nd byte of instruction distinguishing call vs.*/
    /* jump -- this is because the optimizer may change*/
    /* calls to jumps ...*/

        /* ASSUME RMR BYTE IS NOT DOUBLED BECAUSE OF ESCAPES*/
        if( base == OC_CALLI ) {
            *ptr |= B_IND_RMR_CALL;
        } else {
            *ptr |= B_IND_RMR_JMP;
        }
        OutDataByte( back, *ptr++ );
        lbl = ExpandObj( back, ptr, oc->oc_entry.hdr.reclen - offsetof( oc_entry, data ) - 1 - len );
        if( lbl != NULL && base == OC_JMPI ) {
            TellKeepLabel( back, lbl ); /* make sure label comes out*/
            GenKillLabel( back, lbl );  /* but kill it when it does*/
        }
        break;
    case OC_CALL:
    case OC_JMP:
    case OC_JCOND:
        ExpandCJ( back, oc );
        break;
    case OC_RET:
        OutOpndSize( back );
        base = oc->oc_header.class_;
        len = M_RET;
        if( base & OC_ATTR_IRET ) {
            len |= B_RET_IRET;
        }
        if( base & OC_ATTR_FAR ) {
            len |= B_RET_LONG;
        }
        if( (base & OC_ATTR_POP) == 0 ) {
            len |= B_RET_NOPOP;
        }
        OutDataByte( back, len );
        if( base & OC_ATTR_POP ) {
            OutDataShort( back, oc->oc_ret.pops );
        }
        break;
    case OC_INFO:
        base = static_cast<oc_class>(OcInfoClass( oc->oc_header.class_ ));
        switch( base ) {
        case OC_INFO_LINE:
            OutLineNum( back, oc->oc_linenum.line, oc->oc_linenum.label_line );
            break;
        case OC_INFO_LDONE:
            TellScrapLabel( back, oc->oc_handle.handle );
            break;
        case OC_INFO_DEAD_JMP:
            Zoiks( back, ZOIKS_036 );
            break;
        case OC_INFO_DBG_RTN_BEG:
        case OC_INFO_DBG_BLK_BEG:
        case OC_INFO_DBG_EPI_BEG:
            back.x86esc->SaveDbgOc = base;
            back.x86esc->SaveDbgPtr = oc->oc_debug.ptr;
            break;
        case OC_INFO_DBG_BLK_END:
            DbgBlkEnd( back, static_cast<dbg_block *>(oc->oc_debug.ptr), AskLocation(back) );
            break;
        case OC_INFO_DBG_PRO_END:
            DbgProEnd( back, static_cast<dbg_rtn *>(oc->oc_debug.ptr), AskLocation(back) );
            break;
        case OC_INFO_DBG_RTN_END:
            DbgRtnEnd( back, static_cast<dbg_rtn *>(oc->oc_debug.ptr), AskLocation(back) );
            OutLineNum( back, 0, false ); /* Kill pending line number */
            break;
        case OC_INFO_SELECT:
            OutSelect( back, oc->oc_select.starts );
            break;
        }
        break;
    default :
        Zoiks( back, ZOIKS_036 );
        break;
    }
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
X86escState* x86esc_make() { return new X86escState(); }
void x86esc_free(X86escState* part) { delete part; }
