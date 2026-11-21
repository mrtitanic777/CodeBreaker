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
* Description:  Utilities for wlink specific parts of objpass2.
*
****************************************************************************/

#include <algorithm>
#include <array>
#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/fixups/reloc.h"
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/map/map.h"
#include "exeflat.h"
#include "exeqnx.h"
#include "exeelf.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/formats/os2.h"
#include "build/x86link/formats/novell.h"
#include "build/x86link/formats/qnx.h"
#include "build/x86link/formats/elf.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/debuginfo/dbgcv.h"
#include "build/x86link/fixups/toc.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/link/run_state.hpp"


static constexpr unsigned_32 TOC_RESTORE_INSTRUCTION = 0x804b0004;

static signed_16 GetS16( const byte *p )
{
    return( *reinterpret_cast<const signed_16 *>( p ) );
}

#define FIX_POINTER_MASK            (FIX_BASE | FIX_HIGH | FIX_OFFSET_MASK)

struct fix_relo_data {
    byte            *data;
    offset          value;      /* value at location being patched */
    addr_type       loc_addr;
    addr_type       target_addr;
    fix_type        type;
    fix_fpp_type    fpp_type;
    boolbit         additive    : 1;
    boolbit         done        : 1;
    boolbit         imported    : 1;
    boolbit         os2_selfrel : 1;
};

struct fixupf_t {
    /*
     * members must be in this order
     * first two members are shared
     */
    void            *target;
    fix_type        flags;
    offset          off;
    void            *frame;
    unsigned_8      data[];
};

struct fixup_t {
    /*
     * members must be in this order
     * first two members are shared
     */
    void            *target;
    fix_type        flags;
    offset          off;
    unsigned_8      data[];
};

struct sdata_t {
    /*
     * members must be in this order
     * first two members are shared
     */
    segdata         *sdata;
    fix_type        flags;
};

struct save_fixup {
    union {
        fixupf_t    fixupf;
        fixup_t     fixup;
        sdata_t     sdata;
        unsigned_8  buff[40];   /* must be enought for 4 * 64-bit (32 bytes) */
    } u;
};

struct base_reloc {
    unsigned        rel_size;       /* actual size of reloc item */
    unsigned        fix_size;       /* size of field being fixed up */
    offset          fix_off;        /* start addr of field being fixed */
    reloc_item      item;
};

namespace {

/* what obj2supp keeps from one fixup to the next, within one run */
struct Obj2SuppState {
    offset      LastOptimized = 0;      // offset last optimized.
    fix_type    LastOptType{};
    segdata     *LastSegData = nullptr;
    offset      FixupOverflow = 0;
};

Obj2SuppState &State()
{
    return( cb_wlink::RunState::part<Obj2SuppState>() );
}

} // namespace

void ResetObj2Supp( void )
/************************/
{
    Obj2SuppState &st = State();

    st.FixupOverflow = 0;
    st.LastOptType   = static_cast<fix_type>(0);
    st.LastOptimized = 0xFFFFFFFF;
}

static void DCERelocRef(Link& link,  symbol *sym, bool isdata )
/*************************************************/
{
    if( isdata ) {
        if( (sym->info & SYM_1_VF_REFS_DONE) == 0 ) {
            DataRef(link,  sym );
        }
    } else {
        if( link.CurrRec.seg->ispdata
          || link.CurrRec.seg->isreldata ) {
            if( !IsTocSym( sym ) ) {       // make sure that if symbol goes,
                AddSymSegEdge(link,  sym, link.CurrRec.seg ); // .pdata and .reldata go too
            }
        } else {
            AddEdge(link,  link.CurrRec.seg, sym );
        }
    }
}

static void TraceFixup(Link& link,  fix_type type, target_spec *target )
/**********************************************************/
{
#ifdef _EXE
    bool        isovldata;          // true if data and overlaying.
    bool        overlay;            // true if doing overlays;

    overlay = ( (link.FmtData.type & MK_OVERLAYS) && !link.FmtData.u.dos.noindirect );
    if( (link.LinkFlags & LF_STRIP_CODE)
      || overlay ) {
        isovldata = ( (link.CurrRec.seg->u.leader->info & SEGINF_OVERLAYED) == 0 );
        if( link.ObjFormat & OBJ_FMT_UNSAFE_FIXUPP ) {
            isovldata = true;
        }
#else
    /* unused parameters */ (void)type;

    if( link.LinkFlags & LF_STRIP_CODE ) {
#endif
        if( target->type == FIX_TARGET_SEG ) {
            if( link.LinkFlags & LF_STRIP_CODE ) {
                if( target->u.sdata->iscode
                  && target->u.sdata != link.CurrRec.seg ) {
                    RefSeg( target->u.sdata );
                }
            }
        } else if( target->type == FIX_TARGET_EXT ) {
            if( link.LinkFlags & LF_STRIP_CODE ) {
                DCERelocRef(link,  target->u.sym, !link.CurrRec.seg->iscode );
            }
#ifdef _EXE
            if( (type & FIX_REL) == 0
              && overlay ) {
                switch( type & FIX_POINTER_MASK ) {
                case FIX_OFFSET_16:
                    if( link.FmtData.u.dos.ovl_short ) {
                        OvlIndirectCall(link,  target->u.sym );
                    }
                    break;
                case FIX_BASE:
                    OvlIndirectCall(link,  target->u.sym );
                    break;
                case FIX_BASE_OFFSET_16:
                    if( isovldata ) {
                        OvlIndirectCall(link,  target->u.sym );
                    }
                    break;
                }
            }  /* end if( notrelative && overlay ) */
#endif
        } /* end if( a symbol/segment ) */
    }
}

void RelocStartMod( void )
/*******************************/
{
    State().LastSegData = nullptr;
}

static constexpr std::array<byte, 7> OffsetSizes = { 0, 1, 2, 4, 4, 4, 4 };

static unsigned CalcFixupSize( fix_type fixtype )
/***********************************************/
{
    unsigned value;

    value = OffsetSizes[FIX_GET_OFFSET( fixtype )];
    if( fixtype & FIX_BASE ) {
        value += sizeof( unsigned_16 );
    }
    return( value );
}

static unsigned CalcAddendSize( fix_type fixtype )
/************************************************/
{
    if( fixtype & FIX_ADDEND_ZERO ) {
        return( 0 );
    } else if( (fixtype & (FIX_OFFSET_MASK | FIX_BASE)) == FIX_BASE_OFFSET_32 ) {
        return( 2 * sizeof( unsigned_32 ) );
    }
    return( sizeof( unsigned_32 ) );
}

static unsigned CalcSavedFixSize( fix_type fixtype )
/**************************************************/
{
    unsigned    retval;

    if( fixtype & FIX_CHANGE_SEG ) {
        retval = sizeof( sdata_t );
    } else if( FRAME_HAS_DATA( FIX_GET_FRAME( fixtype ) ) ) {
        retval = sizeof( fixupf_t ) + CalcAddendSize( fixtype );
    } else {
        retval = sizeof( fixup_t ) + CalcAddendSize( fixtype );
    }
    return( __ROUND_UP_SIZE( retval, sizeof( void  * ) ) );
}

static void UpdateFramePtr(Link& link,  frame_spec *frame )
/*********************************************/
/* do any necessary work to get frame pointing to the right place */
{
    switch( frame->type ) {
    case FIX_FRAME_GRP:
        if( link.CurrMod->flags_fmt == FILE_FMT_INCREMENTAL ) {
            DbgAssert( frame->u.val != 0 );
            frame->u.group = link.IncGroups[frame->u.val - 1];
        }
        break;
    case FIX_FRAME_EXT:
        frame->u.sym = UnaliasSym(link,  ST_FIND, frame->u.sym );
        DbgAssert( frame->u.sym != NULL );
        break;
    }
}

static void UpdateTargetPtr(Link& link,  target_spec *target )
/************************************************/
/* do any necessary work to get target pointing to the right place */
{
    switch( target->type ) {
    case FIX_TARGET_GRP:
        if( link.CurrMod->flags_fmt == FILE_FMT_INCREMENTAL ) {
            DbgAssert( target->u.val != 0 );
            target->u.group = link.IncGroups[target->u.val - 1];
        }
        break;
    case FIX_TARGET_EXT:
        target->u.sym = UnaliasSym(link,  ST_FIND, target->u.sym );
        DbgAssert( target->u.sym != NULL );
        break;
    }
}

static void GetFrameAddr(Link& link,  frame_spec *frame, addr_type *frame_addr,
                          addr_type *target_addr, offset off )
/*****************************************************************/
{
    switch( frame->type ) {
    case FIX_FRAME_SEG:
        if( frame->u.sdata == nullptr ) {
            LnkMsg(link,  ERR+LOC+MSG_FIXUP_MISSING_THREAD, "dA", frame->type, target_addr );
            break;
        }
        *frame_addr = frame->u.sdata->u.leader->seg_addr;
        frame_addr->off += frame->u.sdata->a.delta;
        break;
    case FIX_FRAME_GRP:
        *frame_addr = frame->u.group->addr;
        break;
    case FIX_FRAME_EXT:
        *frame_addr = frame->u.sym->addr;
        break;
    case FIX_FRAME_ABS:
        frame_addr->seg = frame->u.abs;
        break;
    case FIX_FRAME_LOC:
        *frame_addr = link.CurrRec.addr;
        frame_addr->off += off;
        break;
    case FIX_FRAME_TARG:
    case FIX_FRAME_FLAT:
        *frame_addr = *target_addr;
        break;
    }
}

static void GetTargetAddr( target_spec *target, addr_type *target_addr )
/**********************************************************************/
{
    switch( target->type ) {
    case FIX_TARGET_SEG:
        *target_addr = target->u.sdata->u.leader->seg_addr;
        target_addr->off += target->u.sdata->a.delta;
        break;
    case FIX_TARGET_GRP:
        *target_addr = target->u.group->addr;
        break;
    case FIX_TARGET_EXT:
        *target_addr = target->u.sym->addr;
        break;
    case FIX_TARGET_ABS:
        target_addr->seg = target->u.abs;
        break;
    }
}

static void MapFramePtr(Link& link,  frame_spec *frame, void **targ )
/*******************************************************/
{
    switch( frame->type ) {
    case FIX_FRAME_SEG:
        *targ = CarveGetIndex( link.CarveSegData, *targ );
        break;
    case FIX_FRAME_GRP:
        *targ = reinterpret_cast<void *>(static_cast<pointer_uint>(frame->u.group->num));
        break;
    case FIX_FRAME_EXT:
        *targ = CarveGetIndex( link.CarveSymbol, *targ );
        break;
    }
}

static void MapTargetPtr(Link& link,  target_spec *target, void **targ )
/**********************************************************/
{
    switch( target->type ) {
    case FIX_TARGET_SEG:
        *targ = CarveGetIndex( link.CarveSegData, *targ );
        break;
    case FIX_TARGET_GRP:
        *targ = reinterpret_cast<void *>(static_cast<pointer_uint>(target->u.group->num));
        break;
    case FIX_TARGET_EXT:
        *targ = CarveGetIndex( link.CarveSymbol, *targ );
        break;
    }
}

static segdata *GetTargetSegData( target_spec *target )
/*****************************************************/
{
    segdata     *sdata;
    seg_leader  *leader;

    sdata = nullptr;
    switch( target->type ) {
    case FIX_TARGET_SEG:
        sdata = target->u.sdata;
        break;
    case FIX_TARGET_GRP:
        leader = static_cast<seg_leader *>(Ring2First( target->u.group->leaders ));
        if( leader != nullptr ) {
            sdata = static_cast<segdata *>(Ring2First( leader->pieces ));
        }
        break;
    case FIX_TARGET_EXT:
        sdata = target->u.sym->p.seg;
        break;
    }
    return( sdata );
}

static bool IsReadOnly( segdata *sdata )
/**************************************/
{
    return( sdata->iscode
        || ( sdata->u.leader->group != nullptr )
        && (sdata->u.leader->group->segflags & SEG_READ_ONLY) );
}

static void CheckRWData(Link& link,  target_spec *target, addr_type *loc_addr )
/*****************************************************************/
{
    symbol sym;

    if( (link.FmtData.type & MK_WIN_NE)
      && link.FmtData.u.os2fam.chk_seg_relocs
      && IsReadOnly( State().LastSegData ) ) {
        if( !IS_SYM_IMPORTED( target->u.sym )
          && !IsReadOnly( GetTargetSegData( target ) ) ) {
            if( !IS_DBG_INFO( link.CurrRec.seg->u.leader ) ) {
                if( target->type == FIX_TARGET_SEG ) {
                    sym.name.u.ptr = target->u.sdata->u.leader->segname.u.ptr;
                    LnkMsg(link,  WRN+LOC+MSG_RELOC_TO_RWDATA_SEG, "aS", loc_addr, &sym );
                } else if( target->type == FIX_TARGET_EXT ) {
                    LnkMsg(link,  WRN+LOC+MSG_RELOC_TO_RWDATA_SEG, "aS", loc_addr, target->u.sym );
                }
            }
        }
    }
}

static bool IsTargAbsolute( target_spec *target )
/***********************************************/
{
    return( ( target->type == FIX_TARGET_SEG ) && target->u.sdata->isabs
        || ( target->type == FIX_TARGET_EXT ) && (target->u.sym->info & SYM_ABSOLUTE) );
}

static void FixFrameValue(Link& link,  frame_type type, void **targ )
/*******************************************************/
{
    switch( type ) {
    case FIX_FRAME_SEG:
        *targ = CarveMapIndex( link.CarveSegData, *targ );
        break;
    case FIX_FRAME_EXT:
        *targ = CarveMapIndex( link.CarveSymbol, *targ );
        break;
    }
}

static void FixTargetValue(Link& link,  target_type type, void **targ )
/*********************************************************/
{
    switch( type ) {
    case FIX_TARGET_SEG:
        *targ = CarveMapIndex( link.CarveSegData, *targ );
        break;
    case FIX_TARGET_EXT:
        *targ = CarveMapIndex( link.CarveSymbol, *targ );
        break;
    }
}

size_t RelocMarkSyms(Link& link,  void *_save )
/********************************/
{
    save_fixup  *save = static_cast<save_fixup *>(_save);
    frame_type  frame;
    target_type target;
    symbol      *sym;
    fix_type    fixtype = save->u.sdata.flags;

    if( fixtype & FIX_CHANGE_SEG ) {
        save->u.sdata.sdata = static_cast<segdata *>(CarveMapIndex( link.CarveSegData, save->u.sdata.sdata ));
    } else {
        frame = static_cast<frame_type>(FIX_GET_FRAME( fixtype ));
        if( FRAME_HAS_DATA( frame ) ) {
            FixFrameValue(link,  frame, &save->u.fixupf.frame );
        }
        target = static_cast<target_type>(FIX_GET_TARGET( fixtype ));
        FixTargetValue(link,  target, &save->u.fixup.target );
        if( target == FIX_TARGET_EXT ) {
            sym = static_cast<symbol *>(save->u.fixup.target);
            sym->info |= SYM_RELOC_REFD;
            sym = UnaliasSym(link,  ST_FIND, sym );
            sym->info |= SYM_RELOC_REFD;
        }
    }
    return( CalcSavedFixSize( fixtype ) );
}

static bool MemIsZero( const unsigned_8 *mem, unsigned size )
/***********************************************************/
{
    return( std::all_of( mem, mem + size, []( unsigned_8 b ) { return( b == 0 ); } ) );
}

static void PatchOffset(Link& link,  fix_relo_data *fix, offset val, bool isdelta )
/*********************************************************************/
/* patch offsets into the loaded object */
{
    byte        *code;
    offset      oldval;
    offset      &FixupOverflow = State().FixupOverflow;

    FixupOverflow = 0;
    if( val == 0 )
        return;
    code = fix->data;
    if( fix->type & FIX_SHIFT ) {
        if( val & 3 ) {
            LnkMsg(link,  WRN+LOC+MSG_REL_NOT_ALIGNED, "a", &fix->loc_addr );
        }
        val >>= 2;
    }
    switch( fix->type & FIX_OFFSET_MASK ) {
    case FIX_OFFSET_8:
        if( fix->type & FIX_HIGH ) {
            val >>= 8;
        }
        MPUT_8( code, MGET_U8( code ) + val );
        break;
    case FIX_OFFSET_16:
        if( fix->type & FIX_HIGH ) {
            val >>= 16;
        }
        /* fall through */
    case FIX_NO_OFFSET:         // used for FIX_BASE
        if( fix->type & FIX_SIGNED ) {
            oldval = val;
            val += GetS16( code );
            if( isdelta ) {
                if( static_cast<soffset>(val) > 0x7FFF ) {
                    val = val & 0xFFFF;
                    if( val > 0x7FFF ) {
                        FixupOverflow = 1;
                        val -= 0x10000;
                    }
                }
            } else {
                if( ( oldval >> 16 ) < ( val >> 16 ) ) {
                    FixupOverflow = 1;
                } else if( (fix->type & FIX_SIGNED)
                  && ( oldval >> 16 ) == ( val >> 16 )
                  && ( static_cast<signed_16>( val & 0xFFFF ) < 0 ) ) {
                    FixupOverflow = 1;
                }
            }
        } else {
            val += MGET_U16_UN( code );
        }
        MPUT_16_UN( code, val );
        break;
    case FIX_OFFSET_21:
        oldval = MGET_U32_UN( code );
        val += oldval;
        oldval &= 0xFFE00000;
        val &= 0x001FFFFF;
        MPUT_32_UN( code, oldval | val );
        break;
    case FIX_OFFSET_26:     // Processing is the same, except FIX_OFFSET_26
    case FIX_OFFSET_24:     // uses FIX_SHIFT (it's really a 28-bit offset)
        oldval = MGET_U32_UN( code );
        val += oldval;
        oldval &= 0xFC000000;
        val &= 0x03FFFFFF;
        MPUT_32_UN( code, oldval | val );
        break;
    case FIX_OFFSET_32:
        oldval = MGET_U32_UN( code );
        MPUT_32_UN( code, oldval + val );
        break;
    default:
        LnkMsg(link,  ERR+LOC+MSG_BAD_RELOC_TYPE, nullptr );
    }
}

void StoreFixup(Link& link,  offset off, fix_type type, frame_spec *frame, target_spec *target, offset addend )
/*************************************************************************************************/
{
    save_fixup      save;
    fix_relo_data   fix;
    unsigned        size;
    fix_type        fixtype;
    unsigned_8      *data;
    Obj2SuppState   &st = State();

    if( st.LastSegData != link.CurrRec.seg ) {
        DbgAssert( link.CurrRec.seg != NULL );
        st.LastSegData = link.CurrRec.seg;
        save.u.sdata.flags = FIX_CHANGE_SEG;
        save.u.sdata.sdata = link.CurrRec.seg;
        PermSaveFixup(link,  save.u.buff, __ROUND_UP_SIZE( sizeof( sdata_t ), sizeof( void * ) ) );
    }
    fixtype = static_cast<fix_type>(type | FIX_SET_TARGET( target->type ) | FIX_SET_FRAME( frame->type ));
    save.u.fixup.off = off + link.CurrRec.obj_offset;
    save.u.fixup.target = target->u.ptr;
    if( link.ObjFormat & OBJ_FMT_UNSAFE_FIXUPP ) {
        fixtype |= FIX_UNSAFE;
    }
    if( FRAME_HAS_DATA( frame->type ) ) {
        save.u.fixupf.frame = frame->u.ptr;
        data = save.u.fixupf.data;
    } else {
        data = save.u.fixup.data;
    }
    size = CalcFixupSize( type );
    if( link.CurrRec.data != nullptr ) {
        std::memcpy( data, link.CurrRec.data + off, size );
    } else {
        ReadInfo( link.CurrRec.seg->u1.vm_ptr + save.u.fixup.off, data, size );
    }
    fix.type = type;
    fix.data = data;
    if( (type & (FIX_OFFSET_MASK | FIX_HIGH)) == FIX_HIGH_OFFSET_16 ) {
        addend += st.FixupOverflow << 16;
    }
    PatchOffset(link,  &fix, addend, true );
    if( MemIsZero( data, size ) ) {
        fixtype |= FIX_ADDEND_ZERO;
    }
    save.u.fixup.flags = fixtype;
    PermSaveFixup(link,  save.u.buff, CalcSavedFixSize( fixtype ) );
    TraceFixup(link,  fixtype, target );
    if( link.CurrRec.data != nullptr ) {
        std::memcpy( link.CurrRec.data + off, data, size );
    } else {
        PutInfo( link.CurrRec.seg->u1.vm_ptr + save.u.fixup.off, data, size );
    }
}

size_t IncSaveRelocs(Link& link,  void *_save )
/*********************************/
{
    save_fixup  *save = static_cast<save_fixup *>(_save);
    segdata     *sdata;
    target_spec target;
    size_t      fixsize;
    unsigned    datasize;
    unsigned_8  *data;
    fix_type    fixtype = save->u.sdata.flags;
    segdata     *&LastSegData = State().LastSegData;

    fixsize = CalcSavedFixSize( fixtype );
    if( fixtype & FIX_CHANGE_SEG ) {
        sdata = save->u.sdata.sdata;
        if( !sdata->isdead ) {
            LastSegData = sdata;
        } else {
            LastSegData = nullptr;
        }
    } else if( LastSegData != nullptr ) {
        target.type = static_cast<target_type>(FIX_GET_TARGET( fixtype ));
        target.u.ptr = save->u.fixup.target;
        datasize = CalcFixupSize( fixtype );
        if( fixtype & FIX_ADDEND_ZERO ) {
            PutInfoNulls( LastSegData->u1.vm_ptr + save->u.fixup.off, datasize );
        } else {
            if( FRAME_HAS_DATA( FIX_GET_FRAME( fixtype ) ) ) {
                data = save->u.fixupf.data;
            } else {
                data = save->u.fixup.data;
            }
            PutInfo( LastSegData->u1.vm_ptr + save->u.fixup.off, data, datasize );
        }
        TraceFixup(link,  fixtype, &target );
    }
    PermSaveFixup(link,  save, fixsize );
    return( fixsize );
}

static void DumpReloc(Link& link,  base_reloc *breloc )
/*****************************************/
{
    WriteReloc(link,  link.CurrRec.seg->u.leader->group, breloc->fix_off, &breloc->item, breloc->rel_size );
#ifdef _OS2
    if( link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD) ) {
        if( ( OSF_PAGE_SIZE - (breloc->fix_off & OSF_PAGE_MASK) ) < breloc->fix_size ) {
            /*
             * stupid relocation has been split across two
             * pages, have to duplicate the entry
             */
            breloc->item.os2f.r32_soff -= OSF_PAGE_SIZE;
            WriteReloc(link,  link.CurrRec.seg->u.leader->group, breloc->fix_off + OSF_PAGE_SIZE, &breloc->item, breloc->rel_size );
        }
    }
#endif
}

static void InitReloc(Link& link,  base_reloc *breloc )
/*****************************************/
{
    breloc->rel_size = link.FmtRelocSize;
}

static unsigned FindGroupIdx(Link& link,  segment seg )
/******************************************/
{
    group_entry *group;
    unsigned    index;

    index = 1;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->addr.seg == seg ) {
            return( index );
        }
        index++;
    }
    return( 0 );
}

#ifdef _QNX
static void MakeQNXFloatReloc(Link& link,  fix_relo_data *fix )
/*************************************************/
{
    qnx_reloc_item  qnx_reloc;

    if( link.FmtData.u.qnx.gen_seg_relocs ) {
        qnx_reloc.reloc_offset = fix->loc_addr.off | ( static_cast<unsigned_32>(fix->fpp_type) << 28 );
        qnx_reloc.segment = ToQNXIndex( fix->loc_addr.seg );
        WriteQNXFloatReloc(link,  &qnx_reloc );
    }
}
#endif


#ifdef _OS2

static constexpr std::array<byte, 6> OS2OffsetFixTypeMap = {{
    OSF_SOURCE_UNDEFINED,
    OSF_SOURCE_BYTE,
    OSF_SOURCE_OFF_16,
    OSF_SOURCE_UNDEFINED,
    OSF_SOURCE_OFF_32,
    OSF_SOURCE_UNDEFINED
}};

static constexpr std::array<byte, 6> OS2SegmentedFixTypeMap = {{
    OSF_SOURCE_SEG,
    OSF_SOURCE_UNDEFINED,
    OSF_SOURCE_PTR_32,
    OSF_SOURCE_UNDEFINED,
    OSF_SOURCE_PTR_48,
    OSF_SOURCE_UNDEFINED
}};

static unsigned MapOS2FixType( fix_type type )
/********************************************/
{
    unsigned    off;

    off = FIX_GET_OFFSET( type );
    if( type & FIX_BASE ) {
        return( OS2SegmentedFixTypeMap[off] );
    }
    return( OS2OffsetFixTypeMap[off] );
}

static void MakeWindowsFloatReloc(Link& link,  fix_relo_data *fix )
/*****************************************************/
{
    base_reloc      breloc;
    os2_reloc_item  *os2item;

    InitReloc(link,  &breloc );
    breloc.fix_off = 0;
    os2item = &breloc.item.os2;
    os2item->fixtype = MapOS2FixType( fix->type );
    os2item->reloc_offset = fix->loc_addr.off - link.CurrRec.seg->u.leader->group->addr.off;
    os2item->reloc_type = OSFIXUP | ADDITIVE;
    os2item->put.fltpt = fix->fpp_type;
    DumpReloc(link,  &breloc );
}
#endif

static offset GetSegOff( segdata *sdata )
/***************************************/
{
    return( sdata->a.delta + sdata->u.leader->seg_addr.off );
}

static void CheckPartialRange(Link& link,  fix_relo_data *fix, offset off,
                               unsigned_32 mask, unsigned_32 topbit )
/*******************************************************************/
{
    unsigned_32 utemp;
    signed_32   temp;

    utemp = MGET_U32_UN( fix->data ) & mask;
    if( utemp & topbit ) {      // it is negative
        utemp |= ~mask; // do a bogus sign extension
    }
    temp = utemp;
    if( fix->type & FIX_SHIFT ) {
        temp += off / 4;
    } else {
        temp += off;
    }
    if( ( temp < -static_cast<signed_32>(topbit) )
      || ( temp >= static_cast<signed_32>(topbit) ) ) {
        LnkMsg(link,  ERR+LOC+MSG_FIXUP_OFF_RANGE, "a", &fix->loc_addr );
    }
}

static bool CheckSpecials(Link& link,  fix_relo_data *fix, target_spec *target )
/******************************************************************/
{
    offset      off;
    offset      pos;
    unsigned    fixsize;
    group_entry *group;
    segdata     *sdata;
    fix_type    special;

#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        if( (fix->type & FIX_REL) == 0 ) {
            return( false );
        }
  #if 0
    XXX: this is not the right thing to do for elf-i386
        if( fix->loc_addr.seg != fix->target_addr.seg ) {
            return( false );
        }
  #endif
    }
#endif
#if defined( _QNX ) || defined( _OS2 )
    if( ( fix->fpp_type != FPP_NONE ) ) {
  #ifdef _QNX
        if( link.FmtData.type & MK_QNX ) {
            if( fix->fpp_type != FPP_IGNORE ) {
                MakeQNXFloatReloc(link,  fix );
            }
            return( true );
        }
  #endif
  #ifdef _OS2
        if( link.FmtData.type & MK_WIN_NE ) {
            if( fix->fpp_type != FPP_IGNORE ) {
                MakeWindowsFloatReloc(link,  fix );
            }
            return( true );
        }
  #endif
    }
#endif
    special = fix->type & FIX_SPECIAL_MASK;
    if( ( special == FIX_TOC )
      || ( special == FIX_TOCV ) ) {
        pos = 0;
        if( special == FIX_TOCV ) {
            DbgAssert( target->type == FIX_TARGET_EXT );
#ifdef _OS2
            pos = FindSymPosInTocv(link,  target->u.sym );
#endif
        } else {
            if( target->type  == FIX_TARGET_EXT ) {
                pos = FindSymPosInToc(link,  target->u.sym );
            } else {
                sdata = GetTargetSegData( target );
                off = fix->target_addr.off - GetSegOff( sdata );
                pos = FindSdataOffPosInToc(link,  sdata, off );
            }
        }
        DbgVerify( ( pos % 4 ) == 0, "symbol not in toc" );
        if( fix->type & FIX_OFFSET_16 ) {
            MPUT_16_UN( fix->data, pos );
        } else {
            MPUT_32_UN( fix->data, pos );
        }
        return( true );
    } else if( special == FIX_IFGLUE ) {
        if( ( target->type == FIX_TARGET_EXT )
          && IS_SYM_IMPORTED( target->u.sym ) ) {
            MPUT_32_UN( fix->data, TOC_RESTORE_INSTRUCTION );
        }
        return( true );
    }
    if( (fix->type & FIX_REL) == 0 )
        return( false );
#ifdef _OS2
    if( link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD) ) {
        /*
         * OS/2 V2 relative fixups to imported items or other objects
         * require special handling
         */
        if( fix->imported )
            return( false );
        if( fix->loc_addr.seg != fix->target_addr.seg ) {
            return( false );
        }
    }
#endif
    if( (fix->type & FIX_ABS)
      && (link.FmtData.type & MK_QNX) == 0 ) {
        LnkMsg(link,  ERR+LOC+MSG_BAD_ABS_FIXUP, "a", &fix->loc_addr );
        return( true );
    }
    if( fix->type & FIX_BASE ) {
        LnkMsg(link,  ERR+LOC+MSG_BAD_REL_FIXUP, "a", &fix->loc_addr );
        return( true );
    }
    if( fix->imported ) {
#ifdef _OS2
        if( link.FmtData.type & MK_NE ) {  // can not get at a DLL relatively
            LnkMsg(link,  ERR+LOC+MSG_DLL_IN_REL_RELOC, "a", &fix->loc_addr );
            return( true );
        }
#endif
#ifdef _ELF
        if( link.FmtData.type & MK_ELF ) {
            return( false );
        }
#endif
#ifdef _NOVELL
        if( link.FmtData.type & MK_NOVELL ) {
            if( ( (fix->type & FIX_OFFSET_MASK) != FIX_OFFSET_32 )
              || (fix->type & FIX_BASE) ) {
                LnkMsg(link,  ERR+LOC+MSG_BAD_IMP_REL_RELOC, "a", &fix->loc_addr );
            } else {                           // true == isrelative.
                AddNovImpReloc(link,  target->u.sym, fix->loc_addr.off, true,
                                fix->loc_addr.seg == DATA_SEGMENT );
                /*
                 * I don't know why the novell linker does this, but it does.
                 */
                PatchOffset(link,  fix, static_cast<offset>(-4), true );
            }
        }
#endif
        return( true );
    }
    /* XXX: MK_ELF must not be included for non-i386 */
    if( link.FmtData.type & (MK_PROT_MODE & ~(MK_OS2_FLAT | MK_WIN_VXD | MK_PE | MK_ELF)) ) {
        if( ( fix->loc_addr.seg != fix->target_addr.seg )
          && (fix->type & FIX_ABS) == 0 ) {
            //must have same file segment.
            if( link.FmtData.type & MK_ID_SPLIT ) {
                LnkMsg(link,  ERR+LOC+MSG_NOV_NO_CODE_DATA_RELOC, "a", &fix->loc_addr );
            } else {
                LnkMsg(link,  ERR+LOC+MSG_REL_NOT_SAME_SEG, "a", &fix->loc_addr );
            }
            return( true );
        }
    }
    if( link.FmtData.type & MK_LINEARIZE ) {
        group = FindGroup(link,  fix->target_addr.seg );
        if( group == nullptr ) {           // can happen for IAT symbols.
            off = 0;
        } else {
            off = group->linear;
        }
        group = FindGroup(link,  fix->loc_addr.seg );
        if( group != nullptr ) {
            off -= group->linear;
        }
        off += fix->target_addr.off - fix->loc_addr.off;
    } else if( link.FmtData.type & (MK_32BIT | MK_QNX) ) {
        off = fix->target_addr.off - fix->loc_addr.off;
        if( fix->type & FIX_ABS ) {
            off -= FindGroup(link,  fix->loc_addr.seg )->linear;
        }
#ifdef _DOS16M
    } else if( link.FmtData.type & MK_DOS16M ) {
        off = SUB_16M_ADDR( fix->target_addr, fix->loc_addr );
#endif
    } else {
        off = SUB_REAL_ADDR( fix->target_addr, fix->loc_addr );
    }
    fixsize = CalcFixupSize( fix->type );
    if( (fix->type & FIX_NOADJ) == 0 ) {
        off -= fixsize;
    }
    if( fix->type == FIX_OFFSET_16 ) {
        if( off + ( fix->loc_addr.off + fixsize ) >= 0x10000 ) {
            LnkMsg(link,  ERR+LOC+MSG_FIXUP_OFF_RANGE, "a", &fix->loc_addr );
        }
    } else if( fix->type == FIX_OFFSET_21 ) {
        CheckPartialRange(link,  fix, off, 0x001FFFFF, 0x00100000 );
    } else if( fix->type == FIX_OFFSET_24 ) {   // NYI: repair PPC damage here
        CheckPartialRange(link,  fix, off, 0x03FFFFFF, 0x02000000 );
    }
    PatchOffset(link,  fix, off, true );
    fix->os2_selfrel = (link.FmtData.type & MK_OS2_LX) && link.FmtData.u.os2fam.gen_rel_relocs;
    return( !( fix->os2_selfrel || (fix->type & FIX_ABS) ) );
}

static offset FindRealAddr(Link& link,  fix_relo_data *fix )
/**********************************************/
{
    group_entry *group;
    offset      off;
    bool        dbiflat;

    off = fix->target_addr.off;
    dbiflat = DBINoReloc( link.CurrRec.seg->u.leader );
    if ( link.CurrRec.seg->u.leader->dbgtype == HLL_LINE ) { // FIXME! DBINoReloc() abuse here?
        dbiflat = false;
    }
    if( (fix->type & FIX_ABS)
      || dbiflat
      && (link.CurrMod->flags_mod & MOD_FLATTEN_DBI) == 0 ) {
        return( off );
    }
    if( (link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD | MK_LINEARIZE | MK_QNX_FLAT))
      && (fix->type & FIX_SEC_REL) == 0 ) {
        if( (link.FmtData.type & (MK_OS2_LE | MK_WIN_VXD))
          && !dbiflat )
            return( off );
        /*
         * put the correct value in so that internal fixups
         * don't have to be applied if everything loads in the
         * right spot.
        */
        group = FindGroup(link,  fix->target_addr.seg );
        if( group == nullptr )                  // can happen with dbi relocs
            return( off );
        off += ( group->linear - group->addr.off );
        if( link.FmtData.type & MK_LINEARIZE ) {
            if( (fix->type & FIX_NO_BASE) == 0 ) {
                off += link.FmtData.base;
            }
            /*
             * want dbi addresses relative to 0, but not for ELF
             * (perhaps absolute for others too, later)
             */
            if( dbiflat
              && (link.FmtData.type & MK_ELF) == 0 ) {
                off -= link.Groups->linear;
            }
        }
        if( dbiflat
          && (link.FmtData.type & MK_ELF) == 0 ) {
            off -= link.FmtData.base;
        }
    }
    return( off );
}

static void PatchData(Link& link,  fix_relo_data *fix )
/*****************************************/
{
    byte        *data;
    segment     segval;
    bool        isdbi;

    data = fix->data;
    fix->additive = false;
    if( fix->imported
      && (link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD)) ) {
        fix->value = fix->target_addr.off;
    } else {
        fix->value = 0;
    }
    switch( fix->type & FIX_OFFSET_MASK ) {
    case FIX_OFFSET_8:
        fix->value += MGET_U8( data );
        break;
    case FIX_OFFSET_16:
        fix->value += MGET_U16_UN( data );
        break;
    case FIX_OFFSET_32:
        fix->value += MGET_U32_UN( data );
        break;
    }
    /*
     * Catch the ELF FIX_NOADJ cases CheckSpecials didn't handle.
     * NB: Other fixup types besides LX imports likely need the same treatment.
     */
    if( (fix->type & FIX_NOADJ)
      && fix->imported
      && (link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD)) ) {
        fix->value += CalcFixupSize( fix->type );
        fix->type  &= FIX_NOADJ;    /* This flag isn't interesting anymore. */
    }
    if( fix->value != 0 )
        fix->additive = true;
    if( fix->type & FIX_REL )
        return;
    isdbi = DBINoReloc( link.CurrRec.seg->u.leader );
    if( fix->imported ) {
        if( isdbi ) {
            fix->target_addr.off = 0;      // crazy person has imports in the dbi
            fix->target_addr.seg = 0;
        } else {
            return;
        }
    }
    if( (fix->type & FIX_ABS)
      && (link.FmtData.type & (MK_QNX | MK_DOS16M)) == 0
      || isdbi ) {
        fix->done = true;
    }
    if( (fix->type & FIX_BASE) == 0 ) {     // it's offset only
        if( !( (link.FmtData.type & MK_WIN_NE)
          && (fix->type & FIX_LOADER_RES) ) ) {
            PatchOffset(link,  fix, FindRealAddr(link,  fix ), false );
            if( (link.FmtData.type & (MK_ELF | MK_QNX | MK_PE | MK_OS2_FLAT | MK_WIN_VXD | MK_NOVELL | MK_PHAR_REX | MK_RAW)) == 0
              || (link.FmtData.type & MK_OS2_LX)
              && !link.FmtData.u.os2fam.gen_int_relocs ) {
                fix->done = true;
            }
        }
    } else {    // its a seg reloc and maybe an offset as well.
        if( (fix->type & FIX_OFFSET_MASK) != FIX_NO_OFFSET ) {
            if( !fix->done
              && (link.FmtData.type & (MK_NE | MK_OS2_FLAT | MK_WIN_VXD)) )
                return;
            PatchOffset(link,  fix, FindRealAddr(link,  fix ), false );
            data += OffsetSizes[FIX_GET_OFFSET( fix->type )];
        }
        if( link.FmtData.type & MK_PROT_MODE ) {
            MPUT_16_UN( data, 0 );
        }
#if defined( _DOS16M ) || defined( _PHARLAP )
        if( link.FmtData.type & (MK_DOS16M | MK_PHAR_MULTISEG) ) {
            MPUT_16_UN( data, fix->target_addr.seg );
            return;
        }
#endif
#ifdef _RDOS
        if( link.FmtData.type & MK_RDOS ) {
            segval = fix->target_addr.seg;
            if( segval == link.FmtData.u.rdos.code_seg ) {
                segval = link.FmtData.u.rdos.code_sel;
            } else if( segval == link.FmtData.u.rdos.data_seg ) {
                segval = link.FmtData.u.rdos.data_sel;
            } else {
                LnkMsg(link,  ERR+LOC+MSG_BAD_RELOC_TYPE, nullptr );
            }
            if( segval == 0 ) {
                LnkMsg(link,  ERR+LOC+MSG_BAD_RELOC_TYPE, nullptr );
            }
            MPUT_16_UN( data, segval );
            return;
        }
#endif
        if( fix->done
          || (link.FmtData.type & (MK_QNX | MK_DOS)) ) {
            if( isdbi
              && (link.LinkFlags & (LF_CV_DBI_FLAG | LF_HLL_DBI_FLAG)) ) {    // FIXME
                segval = FindGroupIdx(link,  fix->target_addr.seg );
            } else if( fix->type & FIX_ABS ) {
                /* MASM 5.1 stuffs abs seg length in displacement; ignore it like LINK. */
                segval = fix->target_addr.seg;
            } else {
                segval = MGET_U16_UN( data ) + fix->target_addr.seg;
            }
            MPUT_16_UN( data, segval );
        }
    }
}

#define FAR_CALL_ID     0x9A
#define FAR_JMP_ID      0xEA
#define NOP_ID          0x90
#define SS_OVERRIDE     0x36
#define CS_OVERRIDE     0x3e
#define PUSHCS_ID       0x0e
#define NEAR_CALL_ID    0xe8
#define NEAR_JMP_ID     0xE9
#define MOV_AXAX_ID     0xC089

static bool FarCallOpt(Link& link,  fix_relo_data *fix )
/******************************************/
{
    byte        *code;
    unsigned_16 temp16 = 0;
    unsigned_32 temp32 = 0;
    byte        instruction;
    bool        is32bit;
    Obj2SuppState &st = State();

    /*
     * the sequence of instructions NOP  PUSH CS (near)CALL offset is much
     * faster than (far)CALL segment:offset. So whenever a far call ends up in
     * the same segment as the thing that it is calling, replace the far call
     * with the near call sequence. Note this can potentially wreck jump tables,
     * so rely on the code generator to say when it is safe to do so.
     * this also positions the call so that it returns on an even address
     * to make things go slightly faster. This uses segment overrides rather than
     * NOP's where possible since they are faster, and if there are two far
     * calls in a row, it detects this and uses segment overrides on both the
     * push and the call to avoid the pair of NOP's
     * this also replaces far JMP's with (near) JMP MOV AX,AX
     */

    // optimization is valid only for Intel CPU
    if( link.LinkState & (LS_HAVE_MACHTYPE_MASK & ~LS_HAVE_X86_CODE) )
        return( false );
    if( fix->type & FIX_UNSAFE )
        return( false );
    if( fix->imported )
        return( false );
    if( fix->type & FIX_ABS )
        return( false );
    if( (fix->type & FIX_POINTER_MASK) == FIX_BASE_OFFSET_16 ) {
        is32bit = false;
    } else if( (fix->type & FIX_POINTER_MASK) == FIX_BASE_OFFSET_32 ) {
        is32bit = true;
    } else {
        return( false );
    }
    if( fix->data == link.CurrRec.data )  // can we assume this?
        return( false );
    if( fix->loc_addr.seg != fix->target_addr.seg )
        return( false );
    if( !(link.CurrRec.seg->canfarcall
      || link.CurrRec.seg->iscode) )
        return( false );
    code = fix->data - 1;
    instruction = MGET_U8( code );
    if( ( instruction == FAR_CALL_ID )
      || ( instruction == FAR_JMP_ID ) ) {
        fix->done = true;
        if( is32bit ) {
            temp32 = MGET_U32_UN( code + 1 );
        } else {
            temp16 = MGET_U16_UN( code + 1 );
        }
        if( link.FmtData.type & (MK_NE | MK_OS2_FLAT | MK_WIN_VXD) ) {
            if( is32bit ) {
                temp32 += fix->target_addr.off;     // haven't done this for OS/2
            } else {
                temp16 += fix->target_addr.off;     // haven't done this for OS/2
            }
        }
        if( instruction == FAR_JMP_ID ) {
            MPUT_8( code, NEAR_JMP_ID );
            if( is32bit ) {
                temp32 -= fix->loc_addr.off + 4;   //fix the offset
                MPUT_32_UN( code + 1, temp32 );
                MPUT_16_UN( code + 5, MOV_AXAX_ID );
            } else {
                temp16 -= fix->loc_addr.off + 2;   //fix the offset
                MPUT_16_UN( code + 1, temp16 );
                MPUT_16_UN( code + 3, MOV_AXAX_ID );
            }
        } else {
            MPUT_8( code, PUSHCS_ID );
            if( is32bit ) {
                temp32 -= fix->loc_addr.off + 6; //fix the offset
            } else {
                temp16 -= fix->loc_addr.off + 4; //fix the offset
            }
            if( fix->loc_addr.off & 0x1 ) {  // odd address, so put nop last
                MPUT_8( code + 1, NEAR_CALL_ID );
                if( is32bit ) {
                    MPUT_8( code + 6, NOP_ID );
                    temp32 += 1;            // adjust for different call loc.
                } else {
                    MPUT_8( code + 4, NOP_ID );
                    temp16 += 1;            // adjust for different call loc.
                }
                code -= 1;
                st.LastOptimized = fix->loc_addr.off;
                st.LastOptType = fix->type & FIX_POINTER_MASK;
            } else {
                if( ( st.LastOptType == FIX_BASE_OFFSET_16 )
                  && ( fix->loc_addr.off >= 5 )
                  && ( st.LastOptimized == fix->loc_addr.off - 5 )
                  || ( st.LastOptType == FIX_BASE_OFFSET_32 )
                  && ( fix->loc_addr.off >= 7 )
                  && ( st.LastOptimized == fix->loc_addr.off - 7 ) ) {
                    MPUT_8( code - 1, SS_OVERRIDE );
                }
                MPUT_8( code + 1, CS_OVERRIDE );
                MPUT_8( code + 2, NEAR_CALL_ID );
            }
            if( is32bit ) {
                MPUT_32_UN( code + 3, temp32 );
            } else {
                MPUT_16_UN( code + 3, temp16 );
            }
        }
        return( true );
    }
    return( false );
}

static void MakeBase( fix_relo_data *fix )
/****************************************/
{
    unsigned size;

    if( fix->type & FIX_BASE ) {
        size = OffsetSizes[FIX_GET_OFFSET( fix->type )];
        fix->loc_addr.off += size;
    }
}

#ifdef _OS2
static ordinal_t ChkOS2IntEntry(Link& link,  group_entry *group, segdata *seg,
                                target_spec *target, fix_relo_data *fix )
/***********************************************************************/
{
    ordinal_t   int_ordinal = 0;    // 0 = no crazy stuff

    // Fixups to an IOPL (Ring 2) code segment require special
    // handling in LX/LE executables. However, conforming IOPL
    // segments do not need that and references between IOPL
    // segments don't either.
    if( GET_SEG_PMODE_DPL( group->segflags ) != GET_SEG_PMODE_DPL( seg->u.leader->group->segflags ) ) {
        if( GET_SEG_PMODE_DPL( group->segflags ) == SEG_PMODE_DPL_2
          && !(group->segflags & SEG_CONFORMING) ) {

            // The target has to be in the entry table, otherwise we can't
            // produce the required call gate fixup.
            if( (target->type & FIX_TARGET_EXT)
              && (target->u.sym->info & SYM_EXPORTED) ) {
                symbol          *sym = target->u.sym;
                entry_export    *exp = static_cast<entry_export *>(sym->e.export_);

                int_ordinal = exp->ordinal;
                DbgAssert( int_ordinal );   // Better not be zero!
            } else {
                // TODO: This is a placeholder error message. If the target is not
                // explicitly exported, the linker should automatically force it to
                // be exported and then this case cannot occur.
                LnkMsg(link,  WRN+MSG_INTERNAL, "s", "IOPL entry is not exported!" );
                LnkMsg(link,  LOC + ERR + MSG_INVALID_FLAT_RELOC, "a", &fix->loc_addr );
            }
        } else {
            // This might not produce functioning results but
            // we don't flag this case.
        }
    }

    return( int_ordinal );
}
#endif

static bool formatBaseReloc(Link& link,  fix_relo_data *fix, target_spec *target, segdata *seg, base_reloc *breloc )
/******************************************************************************************************/
{
    addr_type       target_addr;
    group_entry     *group;
    fix_type        ftype;

    /* unused parameters */ (void)target;

    ftype = fix->type & (FIX_OFFSET_MASK | FIX_BASE);
    target_addr = fix->target_addr;
#ifdef _OS2
    if( link.FmtData.type & MK_PE ) {
        unsigned_32 reltype;
        bool        save;
        offset      off;

        save = true;
        off = fix->loc_addr.off + seg->u.leader->group->linear;
        if( fix->type & FIX_HIGH ) {
            reltype = PE_FIX_HIGHADJ;   // NYI: can be high when objalign == 0x10000
            breloc->rel_size = sizeof( high_pe_reloc_item );
            breloc->item.hpe.low_off = static_cast<unsigned_16>(target_addr.off);
        } else if( (link.LinkState & LS_HAVE_MACHTYPE_MASK) == LS_HAVE_MIPS_CODE
          && ftype == FIX_OFFSET_26 ) {
            reltype = PE_FIX_MIPSJMP;
        } else if( ftype == FIX_OFFSET_16 ) {
            if( (link.FmtData.objalign & 0xFFFF) == 0 ) {
                save = false;
            }
            reltype = PE_FIX_LOW;
        } else {
            reltype = PE_FIX_HIGHLOW;
        }
        breloc->item.pe = ( off & OSF_PAGE_MASK ) | reltype;
        return( save );
    } else if( link.FmtData.type & MK_NE ) {
        os2_reloc_item  *os2item;
        offset          off;
        byte            *fixptr;
        dll_sym_info    *dll;

        off = fix->loc_addr.off;
        fixptr = fix->data;
        os2item = &(breloc->item.os2);
        os2item->fixtype = MapOS2FixType( fix->type );
        os2item->reloc_offset = off - seg->u.leader->group->addr.off;
        if( fix->imported ) {
            dll = target->u.sym->p.import_dll;
            if( dll->isordinal ) {
                os2item->reloc_type = IMPORTED_ORDINAL;
                os2item->put.ordinal.ord_num = dll->u.ordinal;
                os2item->put.ordinal.modref_idx = dll->m.modnum->num;
            } else {
                os2item->reloc_type = IMPORTED_NAME;
                os2item->put.name.impnam_off = dll->u.entry->num;
                os2item->put.name.modref_idx = dll->m.modnum->num;
            }
        } else {
            os2item->reloc_type = INTERNAL_REFERENCE;
            os2item->put.internal.rsrvd = 0;
            for( group = link.Groups; group != nullptr; group = group->next ) {
                if( group->addr.seg == target_addr.seg ) {
                    break;
                }
            }
            if( link.FmtData.u.os2fam.selector_fixups ) {
                /* CodeBreaker: by the segment's number; a far pointer's offset written in */
                os2item->put.internal.grp_num = target_addr.seg;
                os2item->put.internal.off = 0;
                if( os2item->fixtype == OSF_SOURCE_PTR_32 && !fix->additive ) {
                    MPUT_16_UN( fixptr, target_addr.off );
                    fixptr += 2;
                    os2item->reloc_offset += 2;
                    os2item->fixtype = OSF_SOURCE_SEG;
                } else if( os2item->fixtype != OSF_SOURCE_SEG ) {
                    os2item->put.internal.off = target_addr.off;
                }
            } else if( ( group != nullptr )
              && (group->segflags & SEG_MOVABLE) ) {
                os2item->put.internal.grp_num = MOVABLE_ENTRY_PNT;
                os2item->put.internal.off = FindEntryOrdinal(link,  target_addr, group );
            } else {
                os2item->put.internal.grp_num = target_addr.seg;
                os2item->put.internal.off = target_addr.off;
            }
        }
        if( fix->additive ) {
            os2item->reloc_type |= ADDITIVE;
        } else {
            MPUT_16_UN( fixptr, 0xffff );          // end of chain indicator
        }
        return( true );
    } else if( link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD) )  {
        segdata         *targseg;
        byte            flags;
        byte            fixtype;
        byte            *fixptr;
        dll_sym_info    *dll;
        bool            freedll;
        ordinal_t       int_ordinal;

        fixptr = breloc->item.os2f.u.buff;
        freedll = false;
        int_ordinal = 0;
        dll = nullptr;
        if( fix->type & FIX_REL ) {
            fixtype = OSF_SOURCE_OFF_32_REL;
            if( fix->os2_selfrel
              && (target->type & FIX_TARGET_EXT) ) {
                freedll = FindOS2ExportSym(link,  target->u.sym, &dll );
                if( dll != nullptr ) {
                    fix->imported = true;
                }
            }
        } else {
            fixtype = MapOS2FixType( fix->type );
        }
        if( IS_SYM_IMPORTED( target->u.sym ) ) {
            targseg = nullptr;
        } else {
            targseg = GetTargetSegData( target );
        }
        if( ( targseg != nullptr )
          && ( targseg->bits == BITS_16 ) ) {
            switch( fixtype ) {
            case OSF_SOURCE_SEG:        // 16-bit selector
            case OSF_SOURCE_PTR_48:     // 16:32 pointer
                if( FIX_GET_FRAME( fix->type ) == FIX_FRAME_FLAT ) {
                    break;
                }
                /* fall through */
            case OSF_SOURCE_PTR_32:     // 16:16 pointer
                fixtype |= OSF_SFLAG_FIXUP_TO_ALIAS;  // YET more fall through
                /* fall through */
            case OSF_SOURCE_OFF_16:     // 16-bit offset
                for( group = link.Groups; group != nullptr; group = group->next ) {
                    if( group->addr.seg == target_addr.seg ) {
                        break;
                    }
                }
                if( group != nullptr ) {
                    group->u.os2flags |= OS2_SEG_16_ALIAS;
                    int_ordinal = ChkOS2IntEntry(link,  group, seg, target, fix );
                }
                break;
            }
        }
        // ALWAYS set the alias flag for 16:16 pointers!
        if( fixtype == OSF_SOURCE_PTR_32 )
            fixtype |= OSF_SFLAG_FIXUP_TO_ALIAS;

        flags = 0;
        breloc->item.os2f.nr_stype = fixtype;
        breloc->item.os2f.nr_flags = flags;
        group = seg->u.leader->group;
        breloc->item.os2f.r32_soff = ( fix->loc_addr.off - group->addr.off ) & OSF_PAGE_MASK;
        if( int_ordinal ) {
            // Fixups to an IOPL segment must be done through a call gate
            // and not directly. There is a special fixup type just for this.
            flags = OSF_TARGET_INT_VIA_ENTRY;
            if( int_ordinal > 0xFF ) {
                MPUT_16_UN( fixptr, int_ordinal );
                fixptr += 2;
            } else {
                flags |= OSF_TFLAG_ORDINAL_8BIT;
                MPUT_8( fixptr, int_ordinal );
                fixptr += 1;
            }
        } else if( fix->imported ) {
            if( dll == nullptr ) {
                dll = target->u.sym->p.import_dll;
            }
            if( dll->isordinal ) {
                flags = OSF_TARGET_EXT_ORD;
            } else {
                flags = OSF_TARGET_EXT_NAME;
            }
            if( dll->m.modnum == nullptr ) {
                MPUT_8( fixptr, 0 );
                fixptr += 1;
            } else if( dll->m.modnum->num > 0xFF ) {
                flags |= OSF_TFLAG_OBJ_MOD_16BIT;
                MPUT_16_UN( fixptr, dll->m.modnum->num );
                fixptr += 2;
            } else {
                MPUT_8( fixptr, dll->m.modnum->num );
                fixptr += 1;
            }
            if( dll->isordinal ) {
                if( dll->u.ordinal > 0xFF ) {
                    MPUT_16_UN( fixptr, dll->u.ordinal );
                    fixptr += 2;
                } else {
                    flags |= OSF_TFLAG_ORDINAL_8BIT;
                    MPUT_8( fixptr, dll->u.ordinal );
                    fixptr += 1;
                }
            } else if( dll->u.entry->num > 0xFFFF ) {
                flags |= OSF_TFLAG_OFF_32BIT;
                MPUT_32_UN( fixptr, dll->u.entry->num );
                fixptr += 4;
            } else {
                MPUT_16_UN( fixptr, dll->u.entry->num );
                fixptr += 2;
            }
            if( fix->additive ) {
                flags |= OSF_TFLAG_ADDITIVE_VAL;
                switch( fix->type & FIX_OFFSET_MASK ) {
                case FIX_OFFSET_8:
                    MPUT_8( fix->data, 0 );
                    break;
                case FIX_OFFSET_16:
                    MPUT_16_UN( fix->data, 0 );
                    break;
                case FIX_OFFSET_32:
                    MPUT_32_UN( fix->data, 0 );
                    break;
                }
                if( fix->value > 0x7fff ) {
                    flags |= OSF_TFLAG_ADD_32BIT;
                    MPUT_32_UN( fixptr, fix->value );
                    fixptr += 4;
                } else {
                    MPUT_16_UN( fixptr, fix->value );
                    fixptr += 2;
                }
            }
        } else {
            if( !fix->os2_selfrel ) {
                target_addr.off += fix->value;
            }
            flags = OSF_TARGET_INTERNAL;
            if( target_addr.seg > 0xFF ) {
                flags |= OSF_TFLAG_OBJ_MOD_16BIT;
                MPUT_16_UN( fixptr, target_addr.seg );
                fixptr += 2;
            } else {
                MPUT_8( fixptr, target_addr.seg );
                fixptr += 1;
            }
            group = FindGroup(link,  target_addr.seg );
            target_addr.off -= group->addr.off;
            if( ftype != FIX_BASE ) {
                if( target_addr.off > 0xFFFF ) {
                    flags |= OSF_TFLAG_OFF_32BIT;
                    MPUT_32_UN( fixptr, target_addr.off );
                    fixptr += 4;
                } else {
                    MPUT_16_UN( fixptr, target_addr.off );
                    fixptr += 2;
                }
            }
        }
        if( freedll )
            MemFree( dll );
        breloc->rel_size = static_cast<unsigned>(fixptr - reinterpret_cast<byte *>(&breloc->item));
        breloc->item.os2f.nr_flags = flags;
        return( true );
    }
#endif
#ifdef _NOVELL
    if( link.FmtData.type & MK_NOVELL ) {
        bool        save;
        offset      off;

        save = true;
        off = fix->loc_addr.off;
        if( fix->imported ) {
            save = false;
            AddNovImpReloc(link,  target->u.sym, off, (fix->type & FIX_REL) != 0, fix->loc_addr.seg == DATA_SEGMENT );
        } else {
            if( fix->loc_addr.seg != DATA_SEGMENT ) {
                off |= NOV_OFFSET_CODE_RELOC;
            }
            if( target_addr.seg != DATA_SEGMENT ) {
                off |= NOV_TARGET_CODE_RELOC;
            }
            breloc->item.novell.reloc_offset = off;
        }
        return( save );
    }
#endif
#ifdef _QNX
    if( link.FmtData.type & MK_QNX ) {
        if( ( ftype == FIX_OFFSET_32 )
          || ( ftype == FIX_BASE_OFFSET_32 ) ) {
            if( link.FmtData.u.qnx.gen_linear_relocs ) {
                qnx_linear_item qnxl_reloc;

                qnxl_reloc.reloc_offset = fix->loc_addr.off;
                seg = GetTargetSegData( target );
                if( seg->iscode ) {
                    qnxl_reloc.reloc_offset |= 0x80000000;
                }
                if( (fix->type & FIX_ABS)
                  && (fix->type & FIX_REL) ) {
                    qnxl_reloc.reloc_offset |= 0x40000000;
                }
                WriteQNXLinearReloc(link,  link.CurrRec.seg->u.leader->group, &qnxl_reloc );
                if( fix->type != FIX_BASE_OFFSET_32 ) {
                    return( false );
                }
                InitReloc(link,  breloc );
                breloc->fix_size = CalcFixupSize( fix->type );
                breloc->fix_off = fix->loc_addr.off;
            }
        }
        MakeBase( fix );
        breloc->item.qnx.reloc_offset = fix->loc_addr.off;
        breloc->item.qnx.segment = ToQNXIndex( fix->loc_addr.seg );
        return( true );
    }
#endif
#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        symbol      *sym;
        bool        save;
        offset      off;

        save = true;
        off = fix->loc_addr.off;
        switch( link.LinkState & LS_HAVE_MACHTYPE_MASK ) {
        case LS_HAVE_ALPHA_CODE:
            break;
        case LS_HAVE_MIPS_CODE:
            if( fix->type & FIX_HIGH ) {
                breloc->item.elf.info = R_MIPS_HI16;
                breloc->item.elf.addend = static_cast<unsigned_16>(target_addr.off);
            } else if( ftype == FIX_OFFSET_16 ) {
                breloc->item.elf.info = R_MIPS_LO16;
                if( (link.FmtData.objalign & 0xFFFF) == 0 ) {
                    save = false;
                }
            } else if( ftype == FIX_OFFSET_26 ) {
                breloc->item.elf.info = R_MIPS_26;
            } else if( ftype == FIX_OFFSET_32 ) {
                breloc->item.elf.info = R_MIPS_REL32;
            } else {
                breloc->item.elf.info = R_MIPS_REL32;
                LnkMsg(link,  LOC + ERR + MSG_INVALID_FLAT_RELOC, "a", &fix->loc_addr );
            }
            break;
        case LS_HAVE_PPC_CODE:
            if( fix->type & FIX_HIGH ) {
                breloc->item.elf.info = R_PPC_ADDR16_HI;
                breloc->item.elf.addend = static_cast<unsigned_16>(target_addr.off);
            } else if( ftype == FIX_OFFSET_16 ) {
                breloc->item.elf.info = R_PPC_ADDR16_LO;
                if( (link.FmtData.objalign & 0xFFFF) == 0 ) {
                    save = false;
                }
            } else {
                breloc->item.elf.info = R_PPC_REL32;
                LnkMsg(link,  LOC + ERR + MSG_INVALID_FLAT_RELOC, "a", &fix->loc_addr );
            }
            break;
        case LS_HAVE_X64_CODE:
            // TODO
            break;
        case LS_HAVE_X86_CODE:
            if( fix->type & FIX_REL ) {
                breloc->item.elf.info = R_386_PC32;
            } else {
                breloc->item.elf.info = R_386_32;
            }
            break;
        default:
            break;
        }
        sym = target->u.sym;
        if( IS_SYM_ALIAS( sym )
          && (sym->info & SYM_WAS_LAZY) ) {
            save = false;
        } else if( (target->type & FIX_TARGET_EXT)
          && IsSymElfImpExp(link,  sym ) ) {
            breloc->item.elf.addend = 0;
        } else {
            seg = GetTargetSegData( target );
            if( seg == nullptr ) {
                save = false;
            } else {
                group = seg->u.leader->group;
                sym = group->sym;
                breloc->item.elf.addend = target_addr.off - group->addr.off;
            }
        }
        if( save ) {
            breloc->item.elf.info |= FindElfSymIdx( sym ) << 8;
            breloc->item.elf.reloc_offset = off;
        }
        return( save );
    }
#endif
#ifdef _PHARLAP
    if( link.FmtData.type & MK_PHAR_REX ) {
        offset      off;

        off = fix->loc_addr.off;
        if( ftype == FIX_OFFSET_32 ) {
            off |= 0x80000000;
        }
        breloc->item.rex.reloc_offset = off;
        return( true );
    }
    if( link.FmtData.type & MK_PHAR_MULTISEG ) {
        MakeBase( fix );
        breloc->item.pms.offset = fix->loc_addr.off;
        group = seg->u.leader->group;
        breloc->item.pms.segment = group->addr.seg;
        return( true );
    }
#endif
#ifdef _EXE
    if( link.FmtData.type & MK_COM ) {
        LnkMsg(link,  WRN+LOC+MSG_SEG_RELOC_OUT, "a", &fix->loc_addr );
        return( false );
    }
#endif
    MakeBase( fix );
    breloc->item.dos.addr.off = fix->loc_addr.off;
    group = seg->u.leader->group;
    breloc->item.dos.addr.seg = group->addr.seg;
    if( group->section != link.Root ) {
        breloc->item.dos.addr.seg -= group->section->sect_addr.seg;
    }
    return( true );
}

static void FmtReloc(Link& link,  fix_relo_data *fix, target_spec *target )
/*************************************************************/
{
    base_reloc          breloc;
    bool                save;
    fix_type            ftype;

    if( fix->done )
        return;
    if( (fix->type & FIX_ABS)
      && (link.FmtData.type & (MK_QNX | MK_DOS16M)) == 0
      && !fix->imported )
        return;
    ftype = fix->type & (FIX_OFFSET_MASK | FIX_BASE);
    if( (link.FmtData.type & (MK_PHAR_SIMPLE | MK_PHAR_FLAT))
      || (link.FmtData.type & (MK_NOVELL | MK_ELF))
      && (link.LinkState & LS_HAVE_X86_CODE)
      && (ftype != FIX_OFFSET_32)
      || (link.FmtData.type & MK_ELF)
      && (link.LinkState & LS_HAVE_X86_CODE) == 0
      && (ftype & (FIX_BASE | FIX_OFFSET_8))
      || (link.FmtData.type & MK_PE)
      && (ftype & (FIX_BASE | FIX_OFFSET_8))
      || ((link.FmtData.type & (MK_PHAR_REX | MK_RAW))
      && (ftype != FIX_OFFSET_16)
      && (ftype != FIX_OFFSET_32)) ) {
        LnkMsg(link,  ERR+LOC+MSG_INVALID_FLAT_RELOC, "a", &fix->loc_addr );
        return;
    }
    if( (link.LinkState & LS_MAKE_RELOCS) == 0 )
        return;
#ifdef _QNX
    if( link.FmtData.type & MK_QNX ) {
        if( ftype == FIX_OFFSET_32 ) {
            if( !link.FmtData.u.qnx.gen_linear_relocs ) {
                return;
            }
        } else if( ( ftype == FIX_BASE )
          || ( ftype == FIX_BASE_OFFSET_16 ) ) {
            if( !link.FmtData.u.qnx.gen_seg_relocs ) {
                return;
            }
        } else if( ftype != FIX_BASE_OFFSET_32 ) {
            return;
        }
    }
#endif
    if( (fix->type & FIX_HIGH)
      && (link.FmtData.type & MK_PE) == 0
      && (link.FmtData.type & MK_ELF) == 0 ) {
        LnkMsg(link,  ERR+LOC+MSG_BAD_RELOC_TYPE, nullptr );
        return;
    }
    DEBUG(( DBG_OLD, "relocation record being output" ));
    InitReloc(link,  &breloc );
    breloc.fix_size = CalcFixupSize( fix->type );
    breloc.fix_off = fix->loc_addr.off;
    save = formatBaseReloc(link,  fix, target, link.CurrRec.seg, &breloc );
    if( save ) {
        DumpReloc(link,  &breloc );
    }
}

static void Relocate(Link& link,  offset off, fix_relo_data *fix, target_spec *target )
/*************************************************************************/
{
    unsigned    shift;
    unsigned    datasize;
    std::array<unsigned_8, 2 * sizeof( unsigned_32 ) + 2> addbuf;

    shift = 0;
    datasize = CalcFixupSize( fix->type );

    fix->done = false;
    fix->os2_selfrel = false;
    fix->data = addbuf.data();

    if( (link.LinkFlags & LF_FAR_CALLS_FLAG)
      && (link.LinkState & LS_HAVE_X86_CODE) ) {

        /*
         * it is necessary to copy also two bytes before reloc position to addbuf
         * because these two bytes can be changed during FAR CALL optimization
         */

        addbuf[0] = 0;
        addbuf[1] = 0;
        shift = ( off < 2 ) ? off : 2;
        datasize += shift;
        fix->data += 2;
    }
    ReadInfo( link.CurrRec.seg->u1.vm_ptr + off - shift, fix->data - shift, datasize );

    if( !CheckSpecials(link,  fix, target ) ) {
        PatchData(link,  fix );
        if( (link.LinkFlags & LF_FAR_CALLS_FLAG)
          && (link.LinkState & LS_HAVE_X86_CODE) )
            FarCallOpt(link,  fix );
        FmtReloc(link,  fix, target );
    }
    PutInfo( link.CurrRec.seg->u1.vm_ptr + off - shift, fix->data - shift, datasize );
}

static void BuildReloc(Link& link,  save_fixup *save, target_spec *target, frame_spec *frame )
/********************************************************************************/
{
    fix_relo_data   fix;
    addr_type       frame_addr;
    fix_type        fixtype = save->u.sdata.flags;
    offset          off = save->u.fixup.off;
    offset          FixupOverflow = State().FixupOverflow;

    frame_addr.off = 0;
    frame_addr.seg = 0;
    std::memset( &fix, 0, sizeof( fix_relo_data ) );        // to get all bitfields 0
    GetTargetAddr( target, &fix.target_addr );
    GetFrameAddr(link,  frame, &frame_addr, &fix.target_addr, off );
    fix.type = fixtype;
    fix.loc_addr = link.CurrRec.addr;
    fix.loc_addr.off += off;
    fix.fpp_type = FPP_NONE;

    if( target->type == FIX_TARGET_EXT ) {
        if( link.MapFile != nullptr ) {
            if( target->u.sym->info & SYM_TRACE ) {
                WriteMapTracedSymRecord(link,  target->u.sym );
            }
        }
        if( (target->u.sym->info & SYM_DEFINED) == 0 ) {
            RecordUndefinedSym(link,  target->u.sym );
            return;
        }
        fix.fpp_type = static_cast<fix_fpp_type>(GET_SYM_2_FPP( target->u.sym ));
        if( IS_SYM_IMPORTED( target->u.sym ) ) {
            if( FRAME_HAS_DATA( frame->type )
              && ( target->u.sym != frame->u.sym ) ) {
                if( link.FmtData.type & (MK_NOVELL | MK_OS2_FLAT | MK_WIN_VXD | MK_PE) ) {
                    fix.target_addr.seg = frame_addr.seg;
                    fix.target_addr.off = 0;
                } else {
                    LnkMsg(link,  ERR+LOC+MSG_FRAME_EQ_TARGET, "a", &fix.loc_addr );
                }
            }
            fix.imported = true;
        }
    }
    if( fixtype & FIX_BASE ) {
        if( link.FmtData.type & (MK_PROT_MODE & ~(MK_OS2_FLAT | MK_WIN_VXD | MK_PE)) ) {
            if( frame_addr.seg != fix.target_addr.seg ) {
                if( link.FmtData.type & MK_ID_SPLIT ) {
                    LnkMsg(link,  ERR+LOC+MSG_NOV_NO_CODE_DATA_RELOC, "a", &fix.loc_addr );
                } else {
                    LnkMsg(link,  ERR+LOC+MSG_FRAME_EQ_TARGET, "a", &fix.loc_addr );
                }
            }
        }
        if( !fix.imported ) {
            CheckRWData(link,  target, &fix.loc_addr );
        }
    }
    if( !fix.imported ) {
        if( fix.fpp_type == FPP_NONE ) {
            ConvertToFrame(link,  &fix.target_addr, frame_addr.seg, ( (fixtype & (FIX_OFFSET_8 | FIX_OFFSET_16)) != 0 ) );
        } else {
            fix.target_addr.seg = frame_addr.seg;
        }
    }
    if( (fixtype & (FIX_OFFSET_MASK | FIX_HIGH)) == FIX_HIGH_OFFSET_16 ) {
        fix.target_addr.off += FixupOverflow << 16;
    }
    if( IsTargAbsolute( target ) ) {
        fix.type |= FIX_ABS;
    }
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        if( ( target->type == FIX_TARGET_EXT )
          && ( (fix.type & FIX_REL) == 0
          || link.FmtData.u.dos.ovl_short )
          && target->u.sym->u.d.ovlref
          && ( (target->u.sym->u.d.ovlstate & OVL_VEC_MASK) == OVL_MAKE_VECTOR ) ) {
            // redirect target to appropriate vector entry
            OvlGetVecAddr(link,  target->u.sym->u.d.ovlref, &fix.target_addr );
        }
    }
#endif
#ifdef _OS2
    if( link.FmtData.type & MK_PE ) {
        if( fix.imported ) {
            /*
                Under PE, the imported symbol address is set to the
                transfer code (JMP [xxxxx]) that is generated by the linker.
            */
            fix.target_addr = target->u.sym->addr;
            fix.imported = false;
        }
    }
#endif
    Relocate(link,  off, &fix, target );
}

size_t IncExecRelocs(Link& link,  void *_save )
/*********************************/
{
    save_fixup  *save = static_cast<save_fixup *>(_save);
    segdata     *sdata;
    target_spec target;
    frame_spec  frame;
    fix_type    fixtype = save->u.sdata.flags;
    segdata     *&LastSegData = State().LastSegData;

    if( fixtype & FIX_CHANGE_SEG ) {
        sdata = save->u.sdata.sdata;
        if( link.LinkFlags & LF_INC_LINK_FLAG ) {
            save->u.sdata.sdata = static_cast<segdata *>(CarveGetIndex( link.CarveSegData, sdata ));
        }
        if( !sdata->isdead ) {
            LoadObj(link,  sdata );
            LastSegData = sdata;
        } else {
            LastSegData = nullptr;
        }
    } else {
        target.type = static_cast<target_type>(FIX_GET_TARGET( fixtype ));
        target.u.ptr = save->u.fixup.target;
        frame.type = static_cast<frame_type>(FIX_GET_FRAME( fixtype ));
        if( FRAME_HAS_DATA( frame.type ) ) {
            frame.u.ptr = save->u.fixupf.frame;
        }
        UpdateTargetPtr(link,  &target );
        UpdateFramePtr(link,  &frame );
        if( LastSegData != nullptr ) {
            BuildReloc(link,  save, &target, &frame );
        }
        if( link.LinkFlags & LF_INC_LINK_FLAG ) {
            MapTargetPtr(link,  &target, &save->u.fixup.target );
            if( FRAME_HAS_DATA( frame.type ) ) {
                MapFramePtr(link,  &frame, &save->u.fixupf.frame );
            }
        }
    }
    return( CalcSavedFixSize( fixtype ) );
}
