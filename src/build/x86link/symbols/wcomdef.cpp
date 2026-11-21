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
* Description: COMDEF -- support for Microsoft COMDEFs
*
****************************************************************************/



#include <cstddef>
#include <cstdlib>
#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/base/virtmem.h"
#include "pcobj.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/objects/objomf.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/objects/omfreloc.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/symbols/wcomdef.h"
#include "build/x86link/link/run_state.hpp"


namespace {

/* these are handy constants used when processing comdat records */
constexpr unsigned CDAT_ALLOC_MASK      = 0xF;
constexpr unsigned CDAT_CONTINUATION    = 0x1;
constexpr unsigned CDAT_ITERATED        = 0x2;
constexpr unsigned CDAT_STATIC          = 0x4;
constexpr unsigned CDAT_SELECT_MASK     = 0xF0;
constexpr unsigned CDAT_SELECT_SHIFT    = 12;

// The communal definitions in progress and the blocks given back for reuse.
struct ComdefState {
    comdat_info     *CDatList;
    comdat_info     *FreedInfos;
    comdat_piece    *FreedPieces;
};

ComdefState &state()
{
    return cb_wlink::RunState::part<ComdefState>();
}

}  // namespace


void ResetComdef( void )
/**********************/
{
    state() = ComdefState{};
}

static comdat_info *AllocCDatInfo( void )
/***************************************/
{
    ComdefState     &st = state();
    comdat_info     *info;

    if( st.FreedInfos != nullptr ) {
        info = st.FreedInfos;
        st.FreedInfos = info->next;
    } else {
        info = static_cast<comdat_info *>( Pass1Alloc( sizeof( comdat_info ) ) );
    }
    info->pieces = nullptr;
    return( info );
}

static void FreeCDatInfo( comdat_info *info )
/*******************************************/
{
    ComdefState     &st = state();

    info->next = st.FreedInfos;
    st.FreedInfos = info;
}

static comdat_piece *AllocCDatPiece( void )
/*****************************************/
{
    ComdefState     &st = state();
    comdat_piece    *piece;

    if( st.FreedPieces != nullptr ) {
        piece = st.FreedPieces;
        st.FreedPieces = piece->next;
    } else {
        piece = static_cast<comdat_piece *>( Pass1Alloc( sizeof( comdat_piece ) ) );
    }
    std::memset( piece, 0, sizeof( comdat_piece ) );
    return( piece );
}

static void FreeCDatPiece( void *piec )
/*************************************/
{
    ComdefState     &st = state();
    comdat_piece    *piece = static_cast<comdat_piece *>( piec );

    if( piece->free_data ) {
        MemFree( piece->data );
    }
    piece->next = st.FreedPieces;
    st.FreedPieces = piece;
}

static unsigned_32 GetLeaf(Link& link)
/********************************/
{
    unsigned char       leaf;
    unsigned_32         value;

    value = 0;
    leaf = *link.ObjBuff++;
    if( leaf <= COMDEF_LEAF_SIZE ) {
        value = leaf;
    } else if( leaf == COMDEF_LEAF_2 ) {
        value = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 );
    } else if( leaf == COMDEF_LEAF_3 ) {
        value = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 );
        value += static_cast<unsigned_32>( *link.ObjBuff++ ) << 16;
    } else if( leaf == COMDEF_LEAF_4 ) {
        value = MGET_LE_U32_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_32 );
    }
    return( value );
}

static byte COMDEF_bits(Link& link)
/*****************************/
{
    SEGDATA *segs = link.CurrMod->segs;
    SEGDATA *seg = nullptr;

    for( ;; ) {
        seg = static_cast<SEGDATA *>( Ring2Step( segs, seg ) );
        if( seg == nullptr )
            break;
        // none of these are generated for Dwarf debug info so
        // we should not get confused when we are 16-bit
        if( seg->isuninit
          || seg->iscdat
          || seg->iscode ) {
            return( seg->bits );
        }
    }
    return( (link.ObjFormat & OBJ_FMT_32BIT_REC) ? BITS_32 : BITS_16 );
}

void ProcComdef(Link& link,  bool isstatic )
/******************************/
{
    byte                bits;
    char *              sym_name;
    unsigned_8          sym_len;
    byte                kind;
    unsigned_32         size;
    symbol *            sym;
    extnode *           ext;
    sym_flags           symop;

    bits = COMDEF_bits(link);
    symop = ST_CREATE_REFERENCE;
    if( isstatic ) {
        symop |= ST_STATIC;
    }
    while( link.ObjBuff < link.EOObjRec ) {
        sym_len = *link.ObjBuff++;
        sym_name = reinterpret_cast<char *>( link.ObjBuff );
        link.ObjBuff += sym_len;
        SkipIdx(link);
        kind = *link.ObjBuff++;
        size = GetLeaf(link);
        if( kind == COMDEF_FAR ) {
            size *= GetLeaf(link);
        }
        sym = SymOp(link,  symop, sym_name, sym_len );
        sym = MakeCommunalSym(link,  sym, size, kind == COMDEF_FAR, bits );
        ext = static_cast<extnode *>( AllocNode( link.ExtNodes ) );
        ext->entry = sym;
        ext->ovlref = 0;
    }
}

void ProcLinsym(Link& link)
/*********************/
{
    list_of_names *     symname;
    symbol *            sym;
    bool                is32bit;
    size_t              sym_len;

    link.ObjBuff++;          /* skip flags */
    symname = FindName(link,  GetIdx(link) );
    sym_len = std::strlen( symname->name );
    sym = SymOp(link,  ST_FIND | ST_STATIC, symname->name, sym_len );
    if( sym == nullptr )
        sym = SymOp(link,  ST_FIND, symname->name, sym_len );
    if( sym == nullptr ) {
        BadObject(link);
        return;
    }
    if( !IS_SYM_COMDAT( sym ) )
        return;
    is32bit = ( (link.ObjFormat & OBJ_FMT_32BIT_REC) != 0 );
    if( sym->mod == link.CurrMod
      && (sym->info & SYM_DEAD) == 0 ) {
        DBIAddLines(link,  sym->p.seg, link.ObjBuff, link.EOObjRec - link.ObjBuff, is32bit );
    }
}

static void DoAltDef(Link& link,  comdat_info *info )
/***************************************/
{
    if( (link.LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        FreeSegData(link,  info->sdata );
    } else {
        Ring2Append( &link.CurrMod->segs, info->sdata );
        info->sdata->isdead = true;
        InfoCDatAltDef(link,  info );
    }
}

static void FinishComdat(Link& link,  void *inf )
/***********************************/
{
    comdat_info *info = static_cast<comdat_info *>( inf );

    if( info->flags & SYM_DEAD ) {      // is redefined
        DoAltDef(link,  info );
    } else {
        StoreInfoData( info );
    }
    RingWalk( info->pieces, FreeCDatPiece );
    FreeCDatInfo( info );
}

void ResolveComdats(Link& link)
/*************************/
{
    RingWalk( state().CDatList, bind_link( link, FinishComdat ) );
    state().CDatList = nullptr;
}

static offset CountIDBlock(Link& link,  unsigned_8 **buffptr )
/************************************************/
/* this finds the length of an individual lidata block */
{
    offset      repeat;
    offset      size;
    unsigned_8  *buff;
    unsigned_16 count;

    buff = *buffptr;
    if( link.ObjFormat & OBJ_FMT_MS_386 ) {
        repeat = MGET_LE_U32_UN( buff );
        buff += sizeof( unsigned_32 );
    } else {
        repeat = MGET_LE_U16_UN( buff );
        buff += sizeof( unsigned_16 );
    }
    count = MGET_LE_U16_UN( buff );
    buff += sizeof( unsigned_16 );
    if( count == 0 ) {  // this is followed by actual data
        repeat *= *buff;
        buff += *buff + 1;
    } else {            // it is a series of id blocks
        size = 0;
        while( count-- > 0 ) {
            size += CountIDBlock(link,  &buff );
        }
        repeat *= size;
    }
    *buffptr = buff;
    return( repeat );
}

static offset CalcLIDataLength(Link& link)
/************************************/
/* this finds the length of the equivalent of an lidata record */
{
    offset      total;
    unsigned_8  *buff;

    total = 0;
    buff = link.ObjBuff;
    while( buff < link.EOObjRec ) {
        total += CountIDBlock(link,  &buff );
    }
    return( total );
}

static bool CompInfoSym( void *info, void *sym )
/**********************************************/
{
    return( static_cast<comdat_info *>( info )->sym == static_cast<symbol *>( sym ) );
}

#ifdef DEVBUILD
static bool CheckSameComdat( void *info, void *sym )
/**************************************************/
{
    if( static_cast<symbol *>( sym ) == static_cast<comdat_info *>( info )->sym ) {
        LnkMsg( ERR+LOC_REC+MSG_INTERNAL, "s", "duplicate comdat found" );
        LnkMsg( ERR+MSG_INTERNAL, "s", static_cast<symbol *>( sym )->name );
    }
    return( false );
}
#endif

#define ST_COMDAT (ST_CREATE_REFERENCE | ST_NOALIAS)

void ProcComdat(Link& link)
/*********************/
/* process a comdat record */
{
    comdat_info *       info;
    comdat_piece *      piece;
    offset              dataoff;
    segdata *           sdata;
    segnode *           snode;
    list_of_names *     symname;
    symbol *            sym;
    size_t              namelen;
    unsigned            attr;
    unsigned            flags;
    unsigned            align;
    unsigned            alloc;
    unsigned            segidx;
    bool                usealign;
    sym_flags           symop;

    flags = *link.ObjBuff++;
    attr = *link.ObjBuff++;
    align = *link.ObjBuff++;
    if( align == 0 ) {
        usealign = true;
    } else {
        usealign = false;
        align = OMFAlignTab[align];
    }
    if( link.ObjFormat & OBJ_FMT_32BIT_REC ) {
        dataoff = MGET_LE_U32_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_32 );
    } else {
        dataoff = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 );
    }
    SkipIdx(link);  /* not interested in the type index */
    alloc = attr & CDAT_ALLOC_MASK;
    if( alloc == 0 ) {  /* if explicit allocation */
        SkipIdx(link);                              /* get public base */
        segidx = GetIdx(link);
        if( segidx != 0 ) {
            snode = static_cast<segnode *>( FindNode( link.SegNodes, segidx ) );
            if( usealign ) {
                align = snode->entry->align;
            }
        } else {
            BadObject(link);        // do not support absolute comdats
            return;             // NOTE: premature return
        }
    } else {    // do not support linker defined comdats
        BadObject(link);
        return;
    }
    piece = AllocCDatPiece();
    symname = FindName(link,  GetIdx(link) );
    namelen = std::strlen( symname->name );
    symop = ST_COMDAT;
    if( flags & CDAT_STATIC ) {
        symop |= ST_STATIC;
    }
    sym = SymOp(link,  symop, symname->name, namelen );
    if( flags & CDAT_ITERATED ) {
        piece->length = CalcLIDataLength(link);
    } else {
        piece->length = link.EOObjRec - link.ObjBuff;
    }
    if( !CacheIsPerm() ) {
        piece->data = static_cast<unsigned_8 *>( MemAllocSafe( piece->length ) );
        std::memcpy( piece->data, link.ObjBuff, piece->length );
        piece->free_data = true;
    } else {
        piece->data = link.ObjBuff;
    }
    comdat_info *&cdat_list = state().CDatList;
    if( flags & CDAT_CONTINUATION ) {
        info = static_cast<comdat_info *>( RingLookup( cdat_list, CompInfoSym, sym ) );
        RingAppend( &info->pieces, piece );
        info->sdata->length += piece->length;
    } else {
        sdata = AllocSegData(link);
        sdata->length = piece->length;
        sdata->combine = COMBINE_ADD;
        sdata->iscdat = true;
        sdata->isabs = ( snode == nullptr );
        sdata->align = align;
        sdata->u.leader = snode->entry->u.leader;
        sdata->iscode = ( (snode->info & SEGINF_CODE) != 0 );
        sdata->bits = ( link.ObjFormat & OBJ_FMT_32BIT_REC ) ? BITS_32 : BITS_16;
        info = AllocCDatInfo();
        info->sdata = sdata;
        info->sym = sym;
        info->flags = static_cast<sym_info>( (attr & CDAT_SELECT_MASK) << CDAT_SELECT_SHIFT );
        info->flags |= SYM_DEAD;        // assume redefined
        RingAppend( &info->pieces, piece );
#ifdef DEVBUILD
        RingLookup( cdat_list, CheckSameComdat, sym );
#endif
        RingAppend( &cdat_list, info );
        if( IS_SYM_COMDAT( sym ) ) {
            CheckComdatSym(link,  sym, info->flags & ~SYM_DEAD );
        } else if( !(IS_SYM_REGULAR( sym )
          && (sym->info & SYM_DEFINED)) ) {
            if( sym->info & SYM_DEFINED ) {
                sym = HashReplace(link,  sym );
            }
            ClearSymUnion(link,  sym );
            if( alloc == 0 ) {  /* explicit */
                snode->entry->u.leader->info |= SEGINF_LXDATA_SEEN;
                RingAppend( &sdata->u.leader->pieces, sdata );
                Ring2Append( &link.CurrMod->segs, sdata );
            }
            sym->info |= (attr & CDAT_SELECT_MASK) << CDAT_SELECT_SHIFT;
            SetComdatSym(link,  sym, sdata );
            info->flags &= ~SYM_DEAD;
        }
    }
    if( (info->flags & SYM_DEAD)
      && (link.LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        link.ObjFormat |= OBJ_FMT_IGNORE_FIXUPP;
    } else {
        link.ObjFormat &= ~(OBJ_FMT_IGNORE_FIXUPP | OBJ_FMT_IS_LIDATA);
    }
    SetCurrSeg(link,  info->sdata, dataoff, piece->data );
}
