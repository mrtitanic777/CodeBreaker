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
* Description:  Object file processing routines specific to OMF.
*
****************************************************************************/


#include <string.h>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/link/specials.h"
#include "pcobj.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/symbols/wcomdef.h"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/objects/omfreloc.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/objects/objomf.h"
#include "build/x86link/link/run_state.hpp"


namespace {

constexpr unsigned_8 DLL_RELOC_NAME     = 0;
constexpr unsigned_8 EXPDEF_ORDINAL     = 0x80;

struct ObjOMFState {
    unsigned        HllLineNumOffset = 0;   // next offset in the $$LINES segment
};

inline ObjOMFState &st() { return cb_wlink::RunState::part<ObjOMFState>(); }

}

extern const byte OMFAlignTab[] = {0,0,1,4,8,2,12};   // (a constant)

void ResetObjOMF(Link& link)
/**********************/
{
    link.ObjBuff = nullptr;
    link.EOObjRec = nullptr;
}

static unsigned long ProcObj(Link& link,  const file_list *file, unsigned long loc, void (*procrtn)( byte ) )
/************************************************************************************************
 * Process an object file.
 */
{
    omf_record          *rec;
    byte                cmd;
    unsigned            len;

    link.RecNum = 0;
    do {
        link.ObjFormat &= ~OBJ_FMT_MS_386;   /* assume not a Microsoft 386 .obj file */
        rec = static_cast<omf_record *>( CacheRead(link,  file, loc, sizeof( omf_record ) ) );
        if( rec == nullptr ) {
            EarlyEOF(link);
            break;
        }
        loc += sizeof( omf_record );
        len = rec->length;
        cmd = rec->command;
        if( procrtn != nullptr ) {
            link.RecNum++;
            link.ObjBuff = static_cast<byte *>( CacheRead(link,  file, loc, len ) );
            if( link.ObjBuff == nullptr ) {
                EarlyEOF(link);
                break;
            }
            link.EOObjRec = link.ObjBuff + len - 1;       /* 1 for the checksum. */
            (*procrtn)( cmd );
            if( link.ObjBuff > link.EOObjRec ) {
                BadObject(link);
                break;
            }
        }
        loc += len;
    } while( cmd != CMD_MODEND && cmd != CMD_MODE32 );
    return( loc );
}

static void CheckUninit( void *_seg, void *dummy )
/************************************************/
{
    segnode *snode = static_cast<segnode *>( _seg );

    /* unused parameters */ static_cast<void>( dummy );

    if( (snode->info & SEGINF_LXDATA_SEEN) == 0 ) {
        snode->entry->isuninit = true;
        if( snode->entry->u1.vm_ptr ) {
            ReleaseInfo( snode->entry->u1.vm_ptr );
            snode->entry->u1.vm_ptr = 0;
        }
    }
}

bool IsOMF(Link& link,  const file_list *file, unsigned long loc )
/****************************************************/
{
    byte        *rec;

    rec = static_cast<byte *>( CacheRead(link,  file, loc, sizeof( unsigned_8 ) ) );
    return( rec != nullptr && ( *rec == CMD_THEADR || *rec == CMD_LHEADR ) );
}

char *GetOMFName(Link& link,  const file_list *file, unsigned long *loc )
/***********************************************************/
{
    omf_record  *rec;
    char        *name;
    unsigned    len;

    rec = static_cast<omf_record *>( CacheRead(link,  file, *loc, sizeof( omf_record ) ) );
    if( rec == nullptr )
        return( nullptr );
    *loc += sizeof( omf_record );
    len = rec->length;
    name = static_cast<char *>( CacheRead(link,  file, *loc, rec->length ) );
    *loc += len;
    if( name == nullptr )
        return( nullptr );
    len = *(unsigned char *)name;        /* get actual name length */
    return( MemToStringSafe(link,  name + 1, len ) );
}

void OMFSkipObj( const file_list *file, unsigned long *loc )
/**********************************************************/
{
    Link& link = wlink();
    *loc = ProcObj(link,  file, *loc, nullptr );
}

static void ProcTHEADR(Link& link)
/****************************/
{
    int     sym_len;

    switch( link.CurrMod->omfdbg ) {
    case OMF_DBG_CODEVIEW:
        sym_len = *link.ObjBuff++;
        if( sym_len == 0 ) {
            BadObject(link);
        }
//        CurrMod->name.u.ptr = MemToStringSafe( ObjBuff, sym_len );
// need to fix to handle separated module name and
// source file name by multiple THEADR records
// now not used, need to resolve backward compatibility
// with modules created by older Open Watcom or WATCOM tools
        break;
    }
}

static void ProcExportKeyword(Link& link)
/***********************************/
{
    unsigned_8  flags;
    length_name intname;
    length_name expname;
    ordinal_t   ordinal;

    flags = *link.ObjBuff++;
    expname.len = *link.ObjBuff++;
    expname.name = reinterpret_cast<char *>( link.ObjBuff );
    link.ObjBuff += expname.len;
    intname.len = *link.ObjBuff++;
    intname.name = reinterpret_cast<char *>( link.ObjBuff );
    link.ObjBuff += intname.len;
    ordinal = 0;
    if( flags & EXPDEF_ORDINAL ) {
        ordinal = MGET_LE_U16_UN( link.ObjBuff );
    }
    HandleExport(link,  &expname, &intname, flags, ordinal );
}

static void ProcImportKeyword(Link& link)
/***********************************/
{
    length_name intname;
    length_name modname;
    unsigned_8  info;

    info = *link.ObjBuff++;
    intname.len = *link.ObjBuff++;
    intname.name = reinterpret_cast<char *>( link.ObjBuff );
    link.ObjBuff += intname.len;
    modname.len = *link.ObjBuff++;
    modname.name = reinterpret_cast<char *>( link.ObjBuff );
    link.ObjBuff += modname.len;
    if( info == DLL_RELOC_NAME ) {
        if( *link.ObjBuff == 0 ) {   /* use internal name */
            HandleImport(link,  &intname, &modname, &intname, IMP_BY_NAME );
        } else {
            length_name     extname;

            extname.len = *link.ObjBuff++;
            extname.name = reinterpret_cast<char *>( link.ObjBuff );
            link.ObjBuff += extname.len;
            HandleImport(link,  &intname, &modname, &extname, IMP_BY_NAME );
        }
    } else {
        HandleImport(link, &intname, &modname, nullptr, MGET_LE_U16_UN( link.ObjBuff ) );
    }
}


static void DoMSOMF(Link& link)
/**************************
 * Figure out debug info type and handle it accordingly later.
 */
{
    if( link.ObjBuff == link.EOObjRec ) {
        link.CurrMod->omfdbg = OMF_DBG_CODEVIEW;    /* Assume MS style */
    } else {
        unsigned_8 version;

        version = *link.ObjBuff++;
        if( link.ObjBuff[0] == 'C'
          && link.ObjBuff[1] == 'V' ) {
            link.CurrMod->omfdbg = OMF_DBG_CODEVIEW;
        } else if( link.ObjBuff[0] == 'H'
          && link.ObjBuff[1] == 'L' ) {
            switch( version ) {
            case 3:
                link.CurrMod->omfdbg = OMF_DBG_HLL_03;
                break;
            case 4:
                link.CurrMod->omfdbg = OMF_DBG_HLL_04;
                break;
            case 6:
                link.CurrMod->omfdbg = OMF_DBG_HLL_06;
                break;
            default:
                link.CurrMod->omfdbg = OMF_DBG_UNKNOWN;
                break;
            }
        } else {
            link.CurrMod->omfdbg = OMF_DBG_UNKNOWN;
        }
    }
}

static void ProcAlias(Link& link)
/****************************
 * process a symbol alias directive
 */
{
    char        *alias;
    size_t      aliaslen;
    size_t      targetlen;
    symbol      *sym;

    for( ; link.ObjBuff < link.EOObjRec; link.ObjBuff += targetlen ) {
        aliaslen = *link.ObjBuff++;
        alias = reinterpret_cast<char *>( link.ObjBuff );
        link.ObjBuff += aliaslen;
        targetlen = *link.ObjBuff++;
        sym = SymOp(link,  ST_FIND | ST_NOALIAS, alias, aliaslen );
        if( sym == nullptr
          || (sym->info & SYM_DEFINED) == 0 ) {
            MakeSymAlias(link,  alias, aliaslen, reinterpret_cast<const char *>( link.ObjBuff ), targetlen );
        }
    }
}

static void ProcModuleEnd(Link& link)
/*******************************/
{
    byte        frame;
    byte        target;
    unsigned    targetidx;
    segnode     *snode;
    extnode     *ext;
    bool        hasdisp;

    if( link.StartInfo.user_specd )
        return;
    target = *link.ObjBuff++;
    if( target & 0x40 ) {
        if( link.ObjBuff >= link.EOObjRec )
            return;
        target = *link.ObjBuff++;
        frame = (target >> 4) & 0x7;
        hasdisp = ( (target & 0x4) == 0 );
        target &= 0x3;
        if( frame <= 2 ) {              /* frame requires an index */
            SkipIdx(link);
        }
        targetidx = GetIdx(link);
        switch( target ) {
        case TARGET_SEG:
            if( link.StartInfo.type != START_UNDEFED ) {
                LnkMsg( MILD_ERR+LOC+MSG_MULT_START_ADDRS, "12", link.StartInfo.mod->u1.source->infile->name, link.StartInfo.mod->name );
                return;                 /* <-------- NOTE: premature return */
            }
            snode = static_cast<segnode *>( FindNode( link.SegNodes, targetidx ) );
            link.StartInfo.type = START_IS_SDATA;
            link.StartInfo.targ.sdata= snode->entry;
            if( (snode->info & SEGINF_CODE)
              && (link.LinkFlags & LF_STRIP_CODE) ) {
                RefSeg( snode->entry );
            }
            link.StartInfo.mod = link.CurrMod;
            break;
        case TARGET_EXT:
            ext = static_cast<extnode *>( FindNode( link.ExtNodes, targetidx ) );
            SetStartSym(link,  ext->entry->name.u.ptr );
            break;
        case TARGET_ABS:
        case TARGET_GRP:
            BadObject(link);        /* no one does these, right??? */
            break;
        }
        if( hasdisp ) {
            if( link.ObjFormat & OBJ_FMT_32BIT_REC ) {
                link.StartInfo.off = MGET_LE_U32_UN( link.ObjBuff );
            } else {
                link.StartInfo.off = MGET_LE_U16_UN( link.ObjBuff );
            }
        }
    }
}

static void AddNames(Link& link)
/***************************
 * Process NAMES record
 */
{
    int                 name_len;
    list_of_names       **entry;

    DEBUG(( DBG_OLD, "AddNames()" ));
    for( ; link.ObjBuff < link.EOObjRec; link.ObjBuff += name_len ) {
        name_len = *link.ObjBuff++;
        entry = static_cast<list_of_names **>( AllocNode( link.NameNodes ) );
        *entry = MakeListName( reinterpret_cast<char *>( link.ObjBuff ), name_len );
    }
}

static void ProcSegDef(Link& link)
/*****************************
 * process a segdef record
 */
{
    segdata             *sdata;
    segnode             *snode;
    list_of_names       *clname;
    list_of_names       *name;
    byte                acbp;

    DEBUG(( DBG_OLD, "ProcSegDef()" ));
    sdata = AllocSegData(link);
    acbp = *link.ObjBuff++;
    switch( (acbp >> 2) & 7 ) {
    case COMB_INVALID:
    case COMB_BAD:
        sdata->combine = COMBINE_INVALID;
        break;
    case COMB_COMMON:
        sdata->combine = COMBINE_COMMON;
        break;
    case COMB_STACK:
        sdata->combine = COMBINE_STACK;
        break;
    default:
        sdata->combine = COMBINE_ADD;
        break;
    }
    sdata->align = OMFAlignTab[acbp >> 5];
    switch( acbp >> 5 ) {
    case ALIGN_ABS:
        sdata->isabs = true;
        sdata->frame = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 ) + 1;
        break;
    case ALIGN_LTRELOC:
        /*
         * in 32 bit object files, ALIGN_LTRELOC is actually ALIGN_4KPAGE
         */
        if( (link.ObjFormat & OBJ_FMT_32BIT_REC) == 0
          && (link.FmtData.type & MK_RAW) == 0 ) {
            sdata->align = OMFAlignTab[ALIGN_PARA];
            /*
             * step over ltldat, max_seg_len, grp_offs fields
             */
            link.ObjBuff += 5;
        }
        break;
    }
    if( link.ObjFormat & OBJ_FMT_32BIT_REC ) {
        if( acbp & 2 ) {
            BadObject(link);            /* we can't handle 4 GB segments properly */
            return;
        }
        sdata->length = MGET_LE_U32_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_32 );
    } else {
        if( acbp & 2 ) {
            sdata->length = 65536;  /* 64k segment */
        } else {
            sdata->length = MGET_LE_U16_UN( link.ObjBuff );
        }
        link.ObjBuff += sizeof( unsigned_16 );
    }
    name = FindName(link,  GetIdx(link) );
    sdata->u.name.u.ptr = name->name;
    clname = FindName(link,  GetIdx(link) );
    if( link.ObjFormat & OBJ_FMT_EASY_OMF ) {
        SkipIdx(link);                          /* skip overlay name index */
        sdata->bits = BITS_32;
        if( link.ObjBuff < link.EOObjRec ) {          /* the optional attribute field present */
            if( (*link.ObjBuff & 0x4) == 0 ) {   /* if USE32 bit not set */
                sdata->bits = BITS_16;
            }
        }
    } else if( acbp & 1 ) {
        sdata->bits = BITS_32;
    } else {
        sdata->bits = BITS_16;
    }
    sdata->iscode = IsCodeClass( clname->name, strlen( clname->name ) );
    snode = static_cast<segnode *>( AllocNode( link.SegNodes ) );
    snode->entry = sdata;
    AllocateSegment(link,  snode, clname->name );
}

static void DefineGroup(Link& link)
/******************************
 * Define a group.
 */
{
    int                 num_segs;
    byte                *anchor;
    segnode             *snode;
    list_of_names       *grp_name;
    grpnode             *newnode;
    group_entry         *group;

    grp_name = FindName(link,  GetIdx(link) );
    DEBUG(( DBG_OLD, "DefineGroup() - %s", grp_name->name ));
    anchor = link.ObjBuff;
    num_segs = 0;
    while( link.ObjBuff < link.EOObjRec ) {
        if( *link.ObjBuff++ != GRP_SEGIDX ) {
            BadObject(link);
            return;
        }
        SkipIdx(link);  /* skip segment index */
        ++num_segs;
    }
    newnode = static_cast<grpnode *>( AllocNode( link.GrpNodes ) );
    group = SearchGroups(link,  grp_name->name );
    if( group == nullptr ) {
        if( num_segs == 0 ) {
            newnode->entry = nullptr;
            return;                     /* NOTE: premature return! */
        }
        group = AllocGroup(link,  grp_name->name, &link.Groups );
    }
    newnode->entry = group;
    link.ObjBuff = anchor;
    while( link.ObjBuff < link.EOObjRec ) {
        link.ObjBuff++;
        snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
        AddToGroup(link,  group, snode->entry->u.leader );
    }
}

static void ProcPubdef(Link& link,  bool static_sym )
/****************************************
 * Define symbols.
 */
{
    symbol          *sym;
    char            *sym_name;
    segnode         *snode;
    offset          off;
    size_t          sym_len;
    unsigned_16     frame;
    unsigned        segidx;
    sym_flags       symop;

    DEBUG(( DBG_OLD, "ProcPubdef" ));
    SkipIdx(link);
    segidx = GetIdx(link);
    if( segidx != 0 ) {
        snode = static_cast<segnode *>( FindNode( link.SegNodes, segidx ) );
        frame = 0;
    } else {
        snode = nullptr;
        frame = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 );
    }
    DEBUG(( DBG_OLD, "segidx = %d", segidx ));
    symop = ST_CREATE_DEFINE_NOALIAS;
    if( static_sym ) {
        symop |= ST_STATIC;
    }
    while( link.ObjBuff < link.EOObjRec ) {
        sym_len = *link.ObjBuff++;
        if( sym_len == 0 ) {
            BadObject(link);
        }
        sym_name = reinterpret_cast<char *>( link.ObjBuff );
        link.ObjBuff += sym_len;
        if( link.ObjFormat & OBJ_FMT_32BIT_REC ) {
            off = MGET_LE_U32_UN( link.ObjBuff );
            link.ObjBuff += sizeof( unsigned_32 );
        } else {
            off = MGET_LE_U16_UN( link.ObjBuff );
            link.ObjBuff += sizeof( unsigned_16 );
        }
        sym = SymOp(link,  symop, sym_name, sym_len );
        DefineSymbol(link,  sym, snode, off, frame );
        SkipIdx(link);   /* skip type index */
    }
}

static void DoLazyExtdef(Link& link,  bool isweak )
/**************************************
 * handle the lazy and weak extdef comments
 */
{
    extnode     *ext;
    symbol      *sym;
    unsigned    idx;

    while( link.ObjBuff < link.EOObjRec ) {
        ext = static_cast<extnode *>( FindNode( link.ExtNodes, GetIdx(link) ) );
        sym = ext->entry;
        ext->isweak = true;
        idx = GetIdx(link);
        ext = static_cast<extnode *>( FindNode( link.ExtNodes, idx ) );
        DefineLazyExtdef(link,  sym, ext->entry, isweak );
    }
}

static void *GetVFListStart( void )
/*********************************/
{
    Link& link = wlink();
    return( link.ObjBuff );
}

static void SetVFListStart( void *start )
/***************************************/
{
    Link& link = wlink();
    link.ObjBuff = static_cast<byte *>( start );
}

static bool EndOfVFList( void )
/*****************************/
{
    Link& link = wlink();
    return( link.ObjBuff >= link.EOObjRec );
}

static char *GetVFListName( void )
/*********************************/
{
    Link& link = wlink();
    list_of_names       *lname;

    lname = FindName(link,  GetIdx(link) );
    return( lname->name );
}

static void ProcVFTableRecord(Link& link,  bool ispure )
/*******************************************
 * process the watcom virtual function table information extension
 */
{
    extnode     *ext;
    symbol      *sym;
    vflistrtns  rtns;

    if( (link.LinkFlags & LF_VF_REMOVAL) == 0 )
        return;
    ext = static_cast<extnode *>( FindNode( link.ExtNodes, GetIdx(link) ) );
    sym = ext->entry;
    ext->isweak = true;
    ext = static_cast<extnode *>( FindNode( link.ExtNodes, GetIdx(link) ) );
    rtns.getstart = GetVFListStart;
    rtns.setstart = SetVFListStart;
    rtns.isend = EndOfVFList;
    rtns.getname = GetVFListName;
    DefineVFTableRecord(link,  sym, ext->entry, ispure, &rtns );
}

static void ProcVFReference(Link& link)
/**********************************
 * process a vftable reference record
 */
{
    extnode             *ext;
    segnode             *snode;
    symbol              *sym;
    list_of_names       *lname;
    unsigned            index;

    if( (link.LinkFlags & LF_VF_REMOVAL) == 0 )
        return;
    index = GetIdx(link);
    if( index == 0 ) {
        LnkMsg( WRN+LOC+MSG_NOT_COMPILED_VF_ELIM, nullptr );
        return;
    }
    ext = static_cast<extnode *>( FindNode( link.ExtNodes, index ) );
    if( link.LinkFlags & LF_STRIP_CODE ) {
        if( *link.ObjBuff == 0 ) {   /* it is a comdat index */
            link.ObjBuff++;
            lname = FindName(link,  GetIdx(link) );
            sym = FindISymbol(link,  lname->name );
            if( sym == nullptr ) {
                sym = MakeWeakExtdef(link,  lname->name, nullptr );
            }
            DefineVFReference( sym, ext->entry, true );
        } else {                /* it's a seg idx */
            snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
            DefineVFReference( snode, ext->entry, false );
        }
    }
    if( (ext->entry->info & SYM_DEFINED) == 0 ) {
        ext->entry->info |= SYM_1_VF_MARKED;
    }
}

static void UseSymbols(Link& link,  bool static_sym, bool iscextdef )
/********************************************************
 * Define all external references.
 */
{
    list_of_names       *lnptr;
    size_t              sym_len;
    extnode             *newnode;
    symbol              *sym;
    sym_flags           symop;

    DEBUG(( DBG_OLD, "UseSymbols()" ));
    symop = ST_CREATE_REFERENCE;
    if( static_sym ) {
        symop |= ST_STATIC;
    }
    while( link.ObjBuff < link.EOObjRec ) {
        if( iscextdef ) {
            lnptr = FindName(link,  GetIdx(link) );
            sym = RefISymbol(link,  lnptr->name );
        } else {
            sym_len = *link.ObjBuff++;
            if( sym_len == 0 ) {
                BadObject(link);
            }
            sym = SymOp(link,  symop, reinterpret_cast<char *>( link.ObjBuff ), sym_len );
            link.ObjBuff += sym_len;
        }
        newnode = static_cast<extnode *>( AllocNode( link.ExtNodes ) );
        newnode->entry = sym;
        newnode->isweak = false;
        DefineReference(link,  sym );
        SkipIdx(link);/*  skip type index */
    }
}

void SkipIdx(Link& link)
/*******************
 * skip the index
 */
{
    if( *link.ObjBuff++ & IS_2_BYTES ) {
        link.ObjBuff++;
    }
}

unsigned_16 GetIdx(Link& link)
/*************************
 * Get an index.
 */
{
    unsigned_16 index;

    index = *link.ObjBuff++;
    if( index & IS_2_BYTES ) {
        index = (index & 0x7f) * 256 + *link.ObjBuff++;
    }
    return( index );
}

list_of_names *FindName(Link& link,  unsigned_16 index )
/*******************************************
 * Find name of specified index.
 */
{
    return( *static_cast<list_of_names **>( FindNode( link.NameNodes, index ) ) );
}

static void ProcLinnum(Link& link)
/*****************************
 * do some processing for the linnum record
 */
{
    segnode     *snode;
    bool        is32bit;

    SkipIdx(link);          /* don't need the group idx */
    snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
    if( snode->info & SEGINF_DEAD )   /* ignore dead segments */
        return;
    is32bit = ( (link.ObjFormat & OBJ_FMT_32BIT_REC) != 0 );
    DBIAddLines(link,  snode->entry, link.ObjBuff, link.EOObjRec - link.ObjBuff, is32bit );
}

static void EnumFindLinesSeg( void *_seg, void *_result_seg )
/****************************/
{
    segnode **result_snode = static_cast<segnode **>( _result_seg );
    segnode *snode = static_cast<segnode *>( _seg );

    if (    snode->entry->bits == BITS_32
        &&  !snode->entry->align
        &&  !snode->entry->iscode
        &&  !strcmp( snode->entry->u.leader->segname.u.ptr, "$$LINES" )
        /* FIXME: compare class? */) {
        *result_snode = snode;
    }
}

static byte *ProcIDBlock(Link& link,  virt_mem *dest, byte *buffer, unsigned iterate )
/*************************************************************************
 * Process logically iterated data blocks.
 */
{
    size_t          len;
    byte            *anchor;
    unsigned        count;
    unsigned        inner;
    unsigned        rep;

    if( iterate == 0 ) {  /* no iterations, so abort. */
        return( link.EOObjRec );
    }
    count = MGET_LE_U16_UN( buffer );
    buffer += sizeof( unsigned_16 );
    if( count == 0 ) {
        len = *buffer;
        ++buffer;
        do {
            PutInfo( *dest, buffer, len );
            *dest += len;
        } while( --iterate != 0 );
        buffer += len;
    } else {
        anchor = buffer;
        if( link.ObjFormat & OBJ_FMT_MS_386 ) {
            do {
                buffer = anchor;
                inner = count;
                do {
                    rep = MGET_LE_U32_UN( buffer );
                    buffer += sizeof( unsigned_32 );
                    buffer = ProcIDBlock(link,  dest, buffer, rep );
                } while( --inner != 0 );
            } while( --iterate != 0 );
        } else {
            do {
                buffer = anchor;
                inner = count;
                do {
                    rep = MGET_LE_U16_UN( buffer );
                    buffer += sizeof( unsigned_16 );
                    buffer = ProcIDBlock(link,  dest, buffer, rep );
                } while( --inner != 0 );
            } while( --iterate != 0 );
        }
    }
    return( buffer );
}

static void DoLIData(Link& link,  virt_mem start, byte *data, size_t size )
/**************************************************************
 * Expand logically iterated data.
 */
{
    unsigned    rep;
    byte        *end_data;

    end_data = data + size;
    while( data < end_data ) {
        if( link.ObjFormat & OBJ_FMT_MS_386 ) {
            rep = MGET_LE_U32_UN( data );
            data += sizeof( unsigned_32 );
        } else {
            rep = MGET_LE_U16_UN( data );
            data += sizeof( unsigned_16 );
        }
        data = ProcIDBlock(link,  &start, data, rep );
    }
}

static void GetObject(Link& link,  segdata *snode, unsigned obj_offset, bool lidata )
/************************************************************************
 * Load object code.
 */
{
    size_t      size;
    virt_mem    start;

    if( snode->isdead
      || snode->isabs ) {   /* ignore dead or abs segments */
        link.ObjFormat |= OBJ_FMT_IGNORE_FIXUPP; /* and any corresponding fixupps */
        return;
    }
    link.ObjFormat &= ~(OBJ_FMT_IGNORE_FIXUPP | OBJ_FMT_IS_LIDATA);
    if( lidata ) {
        link.ObjFormat |= OBJ_FMT_IS_LIDATA;
    }
    if( link.ObjBuff != link.EOObjRec ) {
        size = link.EOObjRec - link.ObjBuff;
        start = snode->u1.vm_ptr + obj_offset;
        if( lidata ) {
            DoLIData(link,  start, link.ObjBuff, size );
        } else {
            if( size + obj_offset > snode->length ) {
                LnkMsg( FTL+LOC_REC+MSG_OBJ_FILE_ATTR, nullptr );
            }
            PutInfo( start, link.ObjBuff, size );
        }
    }
    SetCurrSeg(link,  snode, obj_offset, nullptr );
}

static void ProcLxdata(Link& link,  bool islidata )
/**************************************
 * process ledata and lidata records
 */
{
    segnode     *snode;
    unsigned    obj_offset;

    snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
    snode->entry->u.leader->info |= SEGINF_LXDATA_SEEN;
    snode->info |= SEGINF_LXDATA_SEEN;
    if( link.ObjFormat & OBJ_FMT_32BIT_REC ) {
        obj_offset = MGET_LE_U32_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_32 );
    } else {
        obj_offset = MGET_LE_U16_UN( link.ObjBuff );
        link.ObjBuff += sizeof( unsigned_16 );
    }
#ifdef DEVBUILD
    if( stricmp( snode->entry->u.leader->segname.u.ptr, "_BSS" ) == 0 ) {
        LnkMsg( ERR+LOC_REC+MSG_INTERNAL, "s", "Initialized BSS found" );
    }
#endif
    GetObject(link,  snode->entry, obj_offset, islidata );
}

static void ProcHllLinnum(Link& link)
/****************************/
// Because HLL have FIXUPs to the LINNUM records we treat it as
// a LEDATA record for a special $$LINES segment.
// The leading group & segment indexes doesn't figure in
// the 'LEDATA' - at least not for HLL v4.
// FIXME: check what the LINNUM records looks like in the other HLL versions!
{
    segnode         *snode;
    unsigned        obj_offset;

    if( (link.LinkFlags & LF_HLL_DBI_FLAG) == 0 )
        return;

    /* find the $$LINES segment, create it if not found with this module. */
    snode = nullptr;
    IterateNodelist( link.SegNodes, EnumFindLinesSeg, &snode );
    if( snode == nullptr ) {
        segdata             *sdata;
        segnode             *snode1;
        omf_record          *rec;
        unsigned            loc;
        unsigned            loc_1st_linnum;
        unsigned            len_1st_linnum;
        byte                *buf;

        sdata = AllocSegData(link);
        sdata->u.name.u.ptr = const_cast<char *>( "$$LINES" );
        sdata->combine      = COMBINE_ADD;
        sdata->align        = 0;
        sdata->bits         = BITS_32;
        sdata->iscode       = false;

        /* This is EXTEREMLY UGLY! Sorry about that.
         * But, we need to get the segment size right when defining
         * the segment if I understand the code correctly. Since
         * there is only this pass thru the object file, we have to
         * cook our own pass here so we can count up the LINNUM
         * bits.
         * We assume we're in the very first LINNUM record.
         */
        loc_1st_linnum = -1;
        len_1st_linnum = 0;
        loc = link.CurrMod->location;
        DEBUG(( DBG_HLL, "ProcHllLinnum: counting LINNUM data for %s", link.CurrMod->name ));
        do {
            rec = static_cast<omf_record *>( CacheRead(link,  link.CurrMod->u1.source, loc, sizeof( *rec ) ) );
            if (!rec) {
                break;
            }
            if ( rec->command == CMD_LINNUM32 || rec->command == CMD_LINNUM ) {
                if ( !sdata->length ) {
                    loc_1st_linnum = loc + sizeof( *rec );
                    len_1st_linnum = rec->length;
                }
                DEBUG(( DBG_HLL, "ProcHllLinnum: %d: +%h", loc, rec->length - 3 ));
                sdata->length += rec->length - 3; /* 0 grp idx, 0 seg idx, crc */
            }
            loc += rec->length + sizeof( omf_record );
        } while ( rec->command != CMD_MODEND && rec->command != CMD_MODE32 );

        /* get the current record from the cache */
        buf = static_cast<byte *>( CacheRead(link,  link.CurrMod->u1.source, loc_1st_linnum, len_1st_linnum ) );
        if ( buf != link.ObjBuff ) {
            link.EOObjRec = buf + len_1st_linnum - 1;
            link.ObjBuff = buf;
        }

        snode1 = static_cast<segnode *>( AllocNode( link.SegNodes ) );
        snode1->entry = sdata;
        AllocateSegment(link,  snode1, _HllLineClass );

        st().HllLineNumOffset = 0;
        IterateNodelist( link.SegNodes, EnumFindLinesSeg, &snode );
    }

    SkipIdx(link); // group idx (0)
    SkipIdx(link); // segment idx (ignored)

    /* Do like ProcLxData */
    obj_offset = st().HllLineNumOffset;
    DEBUG(( DBG_HLL, "ProcHllLinnum: HllLineNumOffset=%d length=%h", st().HllLineNumOffset, link.EOObjRec - link.ObjBuff ));
    st().HllLineNumOffset += link.EOObjRec - link.ObjBuff;
    snode->entry->u.leader->info |= SEGINF_LXDATA_SEEN;
    snode->info |= SEGINF_LXDATA_SEEN;
    GetObject(link,  snode->entry, obj_offset, false );
}

static void LinkDirective(Link& link)
/*******************************/
{
    unsigned        directive;
    lib_priority    priority;
    segnode         *snode;

    directive = *link.ObjBuff++;
    switch( directive ) {
    case LDIR_DEFAULT_LIBRARY:
        if( link.ObjBuff < link.EOObjRec ) {
            priority = static_cast<lib_priority>( *link.ObjBuff++ - '0' );
            AddCommentLib(link,  reinterpret_cast<char *>( link.ObjBuff ), link.EOObjRec - link.ObjBuff, priority );
        }
        break;
    case LDIR_SOURCE_LANGUAGE:
        DBIP1Source(link,  link.ObjBuff, link.EOObjRec );
        break;
    case LDIR_VF_PURE_DEF:
        ProcVFTableRecord(link,  true );
        break;
    case LDIR_VF_TABLE_DEF:
        ProcVFTableRecord(link,  false );
        break;
    case LDIR_VF_REFERENCE:
        ProcVFReference(link);
        break;
    case LDIR_PACKDATA:
        if( (link.LinkFlags & LF_PACKDATA_FLAG) == 0 ) {
            link.PackDataLimit = MGET_LE_U32_UN( link.ObjBuff );
            link.LinkFlags |= LF_PACKDATA_FLAG;
        }
        break;
    case LDIR_OPT_FAR_CALLS:
        snode = static_cast<segnode *>( FindNode( link.SegNodes, GetIdx(link) ) );
        snode->entry->canfarcall = true;
        snode->entry->iscode = true;
        break;
    case LDIR_FLAT_ADDRS:
        link.CurrMod->flags_mod |= MOD_FLATTEN_DBI;
        break;
    case LDIR_OPT_UNSAFE:
        link.ObjFormat |= OBJ_FMT_UNSAFE_FIXUPP;
        break;
    }
}

static void Comment(Link& link)
/**************************
 * Process a comment record.
 */
{
    unsigned char       attribute;
    unsigned char       class_;
    int                 proc;
    unsigned char       which;

    attribute = *link.ObjBuff++;
    class_ = *link.ObjBuff++;

    switch( class_ ) {
    case CMT_DLL_ENTRY:
        which = *link.ObjBuff++;
        switch( which ) {
        case MOMF_IMPDEF:
        case MOMF_EXPDEF:
            if( SeenDLLRecord(link) ) {
                if( which == MOMF_EXPDEF ) {
                    ProcExportKeyword(link);
                } else {
                    ProcImportKeyword(link);
                }
            }
            break;
        case MOMF_PROT_LIB:
            if( link.FmtData.type & MK_WIN_NE ) {
                link.FmtData.u.os2fam.is_private_dll = true;
            }
            break;
        }
        break;
    case CMT_WAT_PROC_MODEL:
    case CMT_MS_PROC_MODEL:
        proc = *link.ObjBuff - '0';
        if( proc > link.FmtData.cpu_type )
            link.FmtData.cpu_type = proc;
        break;
    case CMT_DOSSEG:
        link.LinkState |= LS_DOSSEG_FLAG;
        break;
    case CMT_DEFAULT_LIBRARY:
        AddCommentLib(link,  reinterpret_cast<char *>( link.ObjBuff ), link.EOObjRec - link.ObjBuff, static_cast<lib_priority>( LIB_PRIORITY_MAX - 2 ) );
        break;
    case CMT_LINKER_DIRECTIVE:
        LinkDirective(link);
        break;
    case CMT_WKEXT:
        DoLazyExtdef(link,  true );
        break;
    case CMT_LZEXT:
        DoLazyExtdef(link,  false );
        break;
    case CMT_EASY_OMF:
        if( memcmp( link.ObjBuff, EASY_OMF_SIGNATURE, 5 ) == 0 ) {
            link.ObjFormat |= OBJ_FMT_EASY_OMF;
        }
        break;
    case CMT_SOURCE_NAME:
        DBIComment();
        break;
    case CMT_MS_OMF:
        DoMSOMF(link);
        break;
    case 0x80:      /* Code Gen used to put bytes out in wrong order */
        if( attribute == CMT_SOURCE_NAME ) {    /* no longer generated */
            DBIComment();
        } else if( attribute == CMT_LINKER_DIRECTIVE ) {  /* linker directive */
            LinkDirective(link);
        }
        break;
    }
}

static void Pass1Cmd( byte cmd )
/*******************************
 * Process an object record for pass 1
 */
{
    Link& link = wlink();
    bool isstatic;

    isstatic = false;
    switch( cmd ) {
    case CMD_THEADR:
        ProcTHEADR(link);
        break;
    case CMD_COMENT:
        Comment(link);
        break;
    case CMD_LLNAMES:
    case CMD_LNAMES:
        AddNames(link);
        break;
    case CMD_SEGD32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_SEGDEF:
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcSegDef(link);
        break;
    case CMD_STATIC_PUBD32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_STATIC_PUBDEF:
        ProcPubdef(link,  true );
        break;
    case CMD_PUBD32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_PUBDEF:
        ProcPubdef(link,  false );
        break;
    case CMD_STATIC_EXTDEF:
    case CMD_STATIC_EXTD32:
        isstatic = true;
    case CMD_EXTDEF:
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        UseSymbols(link,  isstatic, false );
        break;
    case CMD_CEXTDEF:
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        UseSymbols(link,  false, true );
        break;
    case CMD_GRPDEF:
        DefineGroup(link);
        break;
    case CMD_LINN32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LINNUM:
        switch( link.CurrMod->omfdbg ) {
        case OMF_DBG_CODEVIEW:
            ProcLinnum(link);
            break;
        case OMF_DBG_HLL_03:
        case OMF_DBG_HLL_04:
        case OMF_DBG_HLL_06:
            ProcHllLinnum(link);
            break;
        }
        break;
    case CMD_LINS32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LINSYM:
        ProcLinsym(link);
        break;
    case CMD_STATIC_COMDEF:
        isstatic = true;
    case CMD_COMDEF:
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcComdef(link,  isstatic );
        break;
    case CMD_COMD32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_COMDAT:
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcComdat(link);
        break;
    case CMD_LEDA32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LEDATA:
        ProcLxdata(link,  false );
        break;
    case CMD_LIDA32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LIDATA:
        ProcLxdata(link,  true );
        break;
    case CMD_FIXU32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_FIXUPP:        /* count the fixups for each seg_leader */
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        DoRelocs(link);
        link.ObjFormat &= ~OBJ_FMT_UNSAFE_FIXUPP;
        break;
    case CMD_MODE32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_MODEND:
        ProcModuleEnd(link);
        break;
    case CMD_ALIAS:
        ProcAlias(link);
        break;
    case CMD_BAKP32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_BAKPAT:
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcBakpat(link);
        break;
    case CMD_NBKP32:
        link.ObjFormat |= OBJ_FMT_MS_386;
    case CMD_NBKPAT:
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcNbkpat(link);
        break;
    case CMD_VERNUM:
    case CMD_VENDEXT:
    case CMD_LOCSYM:
    case CMD_TYPDEF:
    case CMD_DEBSYM:
    case CMD_BLKDEF:
    case CMD_BLKD32:
    case CMD_BLKEND:
    case CMD_BLKE32:
        /* ignore the Intel debugging records */
        break;
    case CMD_RHEADR:
    case CMD_REGINT:
    case CMD_REDATA:
    case CMD_RIDATA:
    case CMD_OVLDEF:
    case CMD_ENDREC:
    case CMD_LHEADR:
    case CMD_PEDATA:
    case CMD_PIDATA:
    case CMD_LIBHED:
    case CMD_LIBNAM:
    case CMD_LIBLOC:
    case CMD_LIBDIC:
        LnkMsg( WRN+LOC_REC+MSG_REC_NOT_DONE, "x", cmd );
        break;
    default:
        link.CurrMod->u1.source->infile->status |= INSTAT_IOERR;
        LnkMsg( ERR+LOC_REC+MSG_BAD_REC_TYPE, "x", cmd );
        break;
    }
}

unsigned long OMFPass1( void )
/*****************************
 * do pass 1 for OMF object files
 */
{
    Link& link = wlink();
    unsigned long retval;

    PermStartMod( link.CurrMod );
    if( link.LinkState & (LS_HAVE_MACHTYPE_MASK & ~LS_HAVE_X86_CODE) ) {
        LnkMsg( WRN+MSG_MACHTYPE_DIFFERENT, "s", link.CurrMod->u1.source->infile->name);
    } else {
        link.LinkState |= LS_HAVE_X86_CODE;
    }
    link.CurrMod->omfdbg = OMF_DBG_CODEVIEW; /* Assume MS style LINNUM records */
    retval = ProcObj(link,  link.CurrMod->u1.source, link.CurrMod->location, &Pass1Cmd );
    IterateNodelist( link.SegNodes, CheckUninit, nullptr );
    ResolveComdats();
    return( retval );
}
