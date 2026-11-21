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
* Description:  Routines specific processing relocations in OMF.
*
****************************************************************************/


#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/base/newmem.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/fixups/reloc.h"
#include <pcobj.h>
#include <string.h>
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/objects/objomf.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/objects/omfreloc.h"
#include "build/x86link/link/run_state.hpp"


struct bakpat_list {
    bakpat_list         *next;
    unsigned_16         len;
    segdata             *sdata;
    byte                loctype;
    bool                is32bit;
    char                data[1];
};

namespace {

constexpr unsigned MAX_THREADS = 4;

struct OMFRelocState {
    bakpat_list     *BakPats = nullptr;
    frame_spec      FrameThreads[MAX_THREADS] = {};
    target_spec     TargThreads[MAX_THREADS] = {};
};

inline OMFRelocState &st() { return cb_wlink::RunState::part<OMFRelocState>(); }

}

void ResetOMFReloc( void )
/************************/
{
    st().BakPats = nullptr;
}

static void GetFrame(Link& link,  unsigned method, frame_spec *frame )
/********************************************************/
/* Get frame for fixup. */
{
    extnode     *ext;
    grpnode     *group;
    segnode     *snode;

    switch( method ) {
    case FRAME_SEG:
        snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
        frame->u.sdata = snode->entry;
        frame->type = FIX_FRAME_SEG;
        break;
    case FRAME_GRP:
        group = static_cast<grpnode *>( FindNode( link.GrpNodes, GetIdx(link) ) );
        if( group->entry == nullptr ) {
            frame->type = FIX_FRAME_FLAT;
        } else {
            frame->u.group = group->entry;
            frame->type = FIX_FRAME_GRP;
        }
        break;
    case FRAME_EXT:
        ext = static_cast<extnode *>( FindNode( link.ExtNodes, GetIdx(link) ) );
        if( IS_SYM_IMPORTED( ext->entry ) ) {
            frame->type = FIX_FRAME_TARG;
        } else {
            frame->u.sym = ext->entry;
            frame->type = FIX_FRAME_EXT;
        }
        break;
    case FRAME_ABS:
        frame->u.abs = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 );
        frame->type = FIX_FRAME_ABS;
        break;
    case FRAME_LOC:
        frame->type = FIX_FRAME_LOC;
        break;
    case FRAME_TARG:
        frame->type = FIX_FRAME_TARG;
        break;
    default:
        BadObject(link);
    }
}

static void GetTarget(Link& link,  unsigned method, target_spec *target )
/***********************************************************/
{
    extnode             *ext;
    grpnode             *group;
    segnode             *snode;

    switch( method ) {
    case TARGET_SEG:
        snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
        target->u.sdata = snode->entry;
        target->type = FIX_TARGET_SEG;
        break;
    case TARGET_GRP:
        group = static_cast<grpnode *>( FindNode( link.GrpNodes, GetIdx(link) ) );
        target->u.group = group->entry;
        target->type = FIX_TARGET_GRP;
        break;
    case TARGET_EXT:
        ext = static_cast<extnode *>( FindNode( link.ExtNodes, GetIdx(link) ) );
        target->u.sym = ext->entry;
        target->type = FIX_TARGET_EXT;
        break;
    case TARGET_ABS:
        target->u.abs = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 );
        target->type = FIX_TARGET_ABS;
        break;
    }
}

void DoRelocs(Link& link)
/**************************/
/* Process FIXUP records. */
{
    OMFRelocState &state = st();
    fix_type    fixtype;
    unsigned    typ;
    offset      location;
    offset      addend;
    frame_spec  frame;
    target_spec target;

    if( link.ObjFormat & OBJ_FMT_IGNORE_FIXUPP )
        return;
    if( link.ObjFormat & OBJ_FMT_IS_LIDATA ) {
        LnkMsg(link,  WRN+LOC_REC+MSG_REL_IN_LIDATA, nullptr );
        return;
    }
    do {
        typ = *link.ObjBuff++;
        if( typ & FIXUPP_FIXUP ) {
            /*
             * fixup
             */
            fixtype = (fix_type)(0);
            switch( (typ >> 2) & 0x0F ) {
            case LOC_OFFSET_LO:
                fixtype = FIX_OFFSET_8;
                break;
            case LOC_OFFSET:
                fixtype = FIX_OFFSET_16;
                break;
            case LOC_BASE:
                fixtype = FIX_BASE;
                break;
            case LOC_BASE_OFFSET:
                fixtype = FIX_BASE_OFFSET_16;
                break;
            case LOC_OFFSET_HI:
                fixtype = FIX_HIGH_OFFSET_8;
                break;
            case LOC_OFFSET_LOADER:
                if( link.ObjFormat & OBJ_FMT_EASY_OMF ) {
                    fixtype = FIX_OFFSET_32;                    /* Pharlap only */
                } else {
                    fixtype = FIX_OFFSET_16 | FIX_LOADER_RES;   /* OMF standard */
                }
                break;
            case LOC_PHARLAP_BASE_OFFSET_32:
                if( link.ObjFormat & OBJ_FMT_EASY_OMF ) {
                    fixtype = FIX_BASE_OFFSET_32;               /* Pharlap only */
                }
                break;
            case LOC_OFFSET_32:
                fixtype = FIX_OFFSET_32;
                break;
            case LOC_BASE_OFFSET_32:
                fixtype = FIX_BASE_OFFSET_32;
                break;
            case LOC_OFFSET_LOADER_32:
                fixtype = FIX_OFFSET_32; //| FIX_LOADER_RES;
                break;
            default:
                break;
            }
            if( fixtype && (typ & FIXUPP_MBIT) == 0 ) {
                fixtype |= FIX_REL;
            }
            location = ((typ & 3) << 8) + *link.ObjBuff++;
            typ = *link.ObjBuff++;
            /*
             * frame processing
             */
            if( typ & FIXDAT_FTHREAD ) {
                frame = state.FrameThreads[( typ >> 4 ) & 3];
            } else {
                GetFrame(link,  ( typ >> 4 ) & 7, &frame );
            }
            /*
             * target processing
             */
            if( typ & FIXDAT_TTHREAD ) {
                target = state.TargThreads[typ & 3];
            } else {
                GetTarget(link,  typ & 3, &target );
            }
            /*
             * target addend processing
             */
            addend = 0;
            if( (typ & FIXDAT_PBIT) == 0 ) {
                if( link.ObjFormat & OBJ_FMT_32BIT_REC ) {
                    addend = MGET_LE_U32_UN( link.ObjBuff );
                    link.ObjBuff += sizeof( unsigned_32 );
                } else {
                    addend = MGET_LE_U16_UN( link.ObjBuff );
                    link.ObjBuff += sizeof( unsigned_16 );
                }
            }
            StoreFixup(link,  location, fixtype, &frame, &target, addend );
        } else {
            /*
             * thread
             */
            if( typ & 0x40 ) {
                /*  frame */
                GetFrame(link,  (typ >> 2) & 7, &state.FrameThreads[typ & 3] );
            } else {
                /*  target */
                GetTarget(link,  (typ >> 2) & 3, &state.TargThreads[typ & 3] );
            }
        }
    } while( link.ObjBuff < link.EOObjRec );
}

static void StoreBakPat(Link& link,  segdata *sdata, byte loctype )
/*****************************************************/
/* store a bakpat record away for future processing. */
{
    unsigned            len;
    bakpat_list         *bkptr;

    len = link.EOObjRec - link.ObjBuff;
    bkptr = (bakpat_list *)(MemAllocSafe(link,  sizeof( bakpat_list ) + len - 1 ));
    bkptr->len = len;
    bkptr->loctype = loctype;
    bkptr->sdata = sdata;   /* We don't know the data offset yet. */
    bkptr->is32bit = (link.ObjFormat & OBJ_FMT_32BIT_REC) != 0;
    memcpy( bkptr->data, link.ObjBuff, len );
    AppendList( reinterpret_cast<void **>( &st().BakPats ), bkptr );
}

void ProcBakpat(Link& link)
/****************************/
/* store the bakpat record away for future processing */
{
    segnode             *snode;
    byte                loctype;

    snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
    if( snode->info & SEGINF_DEAD )
        return;
    loctype = *link.ObjBuff++;
    StoreBakPat(link,  snode->entry, loctype );
}

void DoBakPats( void )
/***************************/
/* go through the list of stored bakpats and apply them all */
{
    char                *data;
    bakpat_list         *bkptr;
    bakpat_list         *next;
    offset              off;
    offset              value;
    unsigned_8          value8;
    unsigned_16         value16;
    unsigned_32         value32;
    virt_mem            vmemloc;

    for( bkptr = st().BakPats; bkptr != nullptr; bkptr = next ) {
        next = bkptr->next;
        data = bkptr->data;
        off = 0;
        value = 0;
        while( bkptr->len > 0 ) {
            if( bkptr->is32bit ) {
                off = MGET_LE_U32_UN( data );
                data += sizeof( unsigned_32 );
                value = MGET_LE_U32_UN( data );
                data += sizeof( unsigned_32 );
                bkptr->len -= 2 * sizeof( unsigned_32 );
            } else {
                off = MGET_LE_U16_UN( data );
                data += sizeof( unsigned_16 );
                value = MGET_LE_U16_UN( data );
                data += sizeof( unsigned_16 );
                bkptr->len -= 2 * sizeof( unsigned_16 );
            }
            /* Now the sdata->data pointer should be valid. */
            vmemloc = bkptr->sdata->u1.vm_ptr + off;
            switch( bkptr->loctype ) {
            case 0:
                ReadInfo( vmemloc, &value8, sizeof( unsigned_8 ) );
                value8 += value;
                PutInfo( vmemloc, &value8, sizeof( unsigned_8 ) );
                break;
            case 1:
                ReadInfo( vmemloc, &value16, sizeof( unsigned_16 ) );
                value16 += value;
                PutInfo( vmemloc, &value16, sizeof( unsigned_16 ) );
                break;
            case 2:
                ReadInfo( vmemloc, &value32, sizeof( unsigned_32 ) );
                value32 += value;
                PutInfo( vmemloc, &value32, sizeof( unsigned_32 ) );
                break;
            }
        }
        MemFree( bkptr );
    }
    st().BakPats = nullptr;
}

void ProcNbkpat(Link& link)
/****************************/
/* process a named bakpat record */
{
    list_of_names       *symname;
    symbol              *sym;
    byte                loctype;

    loctype = *link.ObjBuff++;
    symname = FindName(link,  GetIdx(link) );
    sym = RefISymbol(link,  symname->name );
    if( !IS_SYM_COMDAT( sym ) )         /* can't handle these otherwise */
        return;
    if( sym->info & SYM_DEAD )
        return;
    StoreBakPat(link,  sym->p.seg, loctype );
}
