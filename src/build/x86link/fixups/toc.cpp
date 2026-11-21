/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Keep track of Table of Contents (TOC) for NT PPC or of the
*               Global Offset Table (GOT) for OS/2 PPC.
*
****************************************************************************/


#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/fixups/toc.h"
#include "build/x86link/base/hash.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/link/run_state.hpp"


constexpr offset kBogus = 0xa5a5a5a5;


namespace {

struct TocEntryId {
    segdata *sdata;     // If sdata == nullptr, use sym to get address
                        // else use off and sdata->addr get address
    union {
        symbol *sym;
        offset off;     // relative to sdata->addr
    } u;
};

struct TocEntry {
    TocEntryId  e;
    int         pos;
};

struct TocState {
    pHTable         Toc;
    const char      *TocName;
    symbol          *TocSym;
};

TocState& state()
{
    return cb_wlink::RunState::part<TocState>();
}

// An id with every byte defined (the whole union), so the hash and the comparison of its bytes agree.
TocEntryId make_id( segdata *sdata, symbol *sym )
{
    TocEntryId e{};
    e.sdata = sdata;
    e.u.sym = sym;
    return e;
}

TocEntryId make_id( segdata *sdata, offset off )
{
    TocEntryId e{};
    e.sdata = sdata;
    e.u.off = off;
    return e;
}

unsigned TocEntryHashFunc( void *_e, unsigned size )
{
    TocEntry *e = static_cast<TocEntry *>(_e);
    return DataHashFunc( &e->e, sizeof( e->e ), size );
}

int TocEntryCmp( const void *_e1, const void *_e2 )
{
    const TocEntry *e1 = static_cast<const TocEntry *>(_e1);
    const TocEntry *e2 = static_cast<const TocEntry *>(_e2);
    return std::memcmp( &e1->e, &e2->e, sizeof( e1->e ) );
}

void AddToToc(Link& link,  const TocEntryId &e )
{
    TocState &st = state();
    TocEntry searchEntry{};

    searchEntry.e = e;

    if( FindHTableElem( st.Toc, &searchEntry ) == nullptr ) {
        TocEntry *entry = static_cast<TocEntry *>(MemAllocSafe( sizeof( *entry ) ));
        *entry = TocEntry{};
        entry->e = e;
        entry->pos = static_cast<int>( link.TocSize );
        link.TocSize += sizeof( offset );
        AddHTableElem( st.Toc, entry );
    }
}

void ConvertTocEntryId( TocEntryId *e )
{
    if( e->sdata != nullptr ) {
        // do nothing: already in sdata/offset form
    } else {
        symbol *sym = e->u.sym;
        segdata *seg = sym->p.seg;

        if( IS_SYM_IMPORTED( sym )
          || seg == nullptr ) {
            // do not convert; keep symbol around
        } else {
            e->sdata = seg;
            e->u = {};
            e->u.off = sym->addr.off - seg->a.delta -
                       seg->u.leader->seg_addr.off;
        }
    }
}

void ConvertTocEntry( void *e )
{
    ConvertTocEntryId( &static_cast<TocEntry *>(e)->e );
}

offset OffFromToc(Link& link,  offset off )
{
    offset toff = off - link.TocShift;
    if( static_cast<signed_16>( toff ) != static_cast<signed_32>( toff ) ) {
        LnkMsg( ERR+MSG_TOC_TOO_BIG, nullptr );
    }
    return toff;
}

offset FindEntryPosInToc(Link& link,  const TocEntryId &e )
{
    TocEntry searchEntry{};
    searchEntry.e = e;
    TocEntry *entry = static_cast<TocEntry *>(FindHTableElem( state().Toc, &searchEntry ));
    if( entry != nullptr ) {
        return OffFromToc(link,  static_cast<offset>( entry->pos ) );
    }
    return kBogus; // return bogus position; likely to cause alignment exception
}

void WriteOutTokElem( void *_elem, void *buf )
{
    Link& link = wlink();
    TocEntry   *elem = static_cast<TocEntry *>(_elem);
    offset      addr;

    if( elem->e.sdata != nullptr ) {
        segdata *sdata = elem->e.sdata;
        seg_leader *leader = sdata->u.leader;
        addr = elem->e.u.off + sdata->a.delta + leader->group->linear
                        + leader->seg_addr.off +  link.FmtData.base;
    } else {
        addr = SymbolAbsAddr(link,  elem->e.u.sym );
    }
    DbgAssert( elem->pos >= 0 );
    PutInfo( (*static_cast<virt_mem *>(buf)) + elem->pos, &addr, sizeof( addr ) );
}

}  // namespace


void ResetToc(Link& link)
/**************************/
{
    TocState &st = state();
    st.Toc = nullptr;
    link.TocSize = 0;
    st.TocName = nullptr;
    st.TocSym = nullptr;
}

void CleanToc( void )
/**************************/
{
    ZapHTable( state().Toc, MemFree );
}

void InitToc(Link& link)
/***********************/
{
    TocState &st = state();
    st.Toc = CreateHTable( 1024, TocEntryHashFunc, TocEntryCmp, MemAllocSafe, MemFree );
    link.TocSize = 0;
    if( (link.LinkState & LS_HAVE_PPC_CODE)
      && (link.FmtData.type & MK_PE) ) {
        st.TocName = TocSymName;
        link.TocShift = 0x8000;
    } else {
        st.TocName = GotSymName;
        link.TocShift = kBogus;
        // It is an error to use TocShift until PrepareToc is called
    }
}

void CheckIfTocSym( symbol *sym )
/**************************************/
{
    TocState &st = state();
    if( st.TocName != nullptr
      && st.TocSym == nullptr ) {
        if( std::strcmp( sym->name.u.ptr, st.TocName ) == 0 ) {
            st.TocSym = sym;
        }
    }
}

bool IsTocSym( symbol *sym )
/*********************************/
{
    return state().TocSym == sym;
}

void AddSymToToc(Link& link,  symbol *sym )
/************************************/
{
    AddToToc(link,  make_id( nullptr, sym ) );
}

void AddSdataOffToToc(Link& link,  segdata *sdata, offset off )
/********************************************************/
{
    DbgAssert( sdata != nullptr );
    AddToToc(link,  make_id( sdata, off ) );
}

void PrepareToc(Link& link)
/****************************/
{
    TocState &st = state();
    if( st.Toc == nullptr )
        return;
    WalkHTable( st.Toc, ConvertTocEntry );
    RehashHTable( st.Toc );
    // (OS/2 PPC's GOT adjustment, development on hold in the original, is not carried.)
    if( st.TocSym != nullptr )  {
        st.TocSym->info |= SYM_1_DCE_REF | SYM_DEFINED;
        SetAddPubSym( st.TocSym, SYM_REGULAR, link.FakeModule, 0, 0 );
        if( link.LinkFlags & LF_STRIP_CODE ) {
            CleanStripInfo( st.TocSym );
        }
    }
}

void SetTocAddr(Link& link,  offset off, group_entry *group )
/******************************************************/
{
    TocState &st = state();
    if( st.Toc != nullptr
      && st.TocSym != nullptr ) {
        SET_SYM_ADDR( st.TocSym, off + link.TocShift, group->addr.seg );
    }
}

offset FindSdataOffPosInToc(Link& link,  segdata *sdata, offset off )
/*****************************************************************/
{
    DbgAssert( sdata != nullptr );
    return FindEntryPosInToc(link,  make_id( sdata, off ) );
}

offset FindSymPosInToc(Link& link,  symbol * sym )
/**********************************************/
{
    TocEntryId e = make_id( nullptr, sym );
    ConvertTocEntryId( &e );
    return FindEntryPosInToc(link,  e );
}

void WriteToc( virt_mem buf )
/**********************************/
{
    if( state().Toc == nullptr )
        return;
    WalkHTableCookie( state().Toc, WriteOutTokElem, &buf );
    // (OS/2 PPC's reserved GOT words, development on hold in the original, are not written.)
}
