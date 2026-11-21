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

void ResetObjOMF( void )
/**********************/
{
    wlink().ObjBuff = nullptr;
    wlink().EOObjRec = nullptr;
}

static unsigned long ProcObj( const file_list *file, unsigned long loc, void (*procrtn)( byte ) )
/************************************************************************************************
 * Process an object file.
 */
{
    omf_record          *rec;
    byte                cmd;
    unsigned            len;

    wlink().RecNum = 0;
    do {
        wlink().ObjFormat &= ~OBJ_FMT_MS_386;   /* assume not a Microsoft 386 .obj file */
        rec = static_cast<omf_record *>( CacheRead( file, loc, sizeof( omf_record ) ) );
        if( rec == nullptr ) {
            EarlyEOF();
            break;
        }
        loc += sizeof( omf_record );
        len = rec->length;
        cmd = rec->command;
        if( procrtn != nullptr ) {
            wlink().RecNum++;
            wlink().ObjBuff = static_cast<byte *>( CacheRead( file, loc, len ) );
            if( wlink().ObjBuff == nullptr ) {
                EarlyEOF();
                break;
            }
            wlink().EOObjRec = wlink().ObjBuff + len - 1;       /* 1 for the checksum. */
            (*procrtn)( cmd );
            if( wlink().ObjBuff > wlink().EOObjRec ) {
                BadObject();
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

bool IsOMF( const file_list *file, unsigned long loc )
/****************************************************/
{
    byte        *rec;

    rec = static_cast<byte *>( CacheRead( file, loc, sizeof( unsigned_8 ) ) );
    return( rec != nullptr && ( *rec == CMD_THEADR || *rec == CMD_LHEADR ) );
}

char *GetOMFName( const file_list *file, unsigned long *loc )
/***********************************************************/
{
    omf_record  *rec;
    char        *name;
    unsigned    len;

    rec = static_cast<omf_record *>( CacheRead( file, *loc, sizeof( omf_record ) ) );
    if( rec == nullptr )
        return( nullptr );
    *loc += sizeof( omf_record );
    len = rec->length;
    name = static_cast<char *>( CacheRead( file, *loc, rec->length ) );
    *loc += len;
    if( name == nullptr )
        return( nullptr );
    len = *(unsigned char *)name;        /* get actual name length */
    return( MemToStringSafe( name + 1, len ) );
}

void OMFSkipObj( const file_list *file, unsigned long *loc )
/**********************************************************/
{
    *loc = ProcObj( file, *loc, nullptr );
}

static void ProcTHEADR( void )
/****************************/
{
    int     sym_len;

    switch( wlink().CurrMod->omfdbg ) {
    case OMF_DBG_CODEVIEW:
        sym_len = *wlink().ObjBuff++;
        if( sym_len == 0 ) {
            BadObject();
        }
//        CurrMod->name.u.ptr = MemToStringSafe( ObjBuff, sym_len );
// need to fix to handle separated module name and
// source file name by multiple THEADR records
// now not used, need to resolve backward compatibility
// with modules created by older Open Watcom or WATCOM tools
        break;
    }
}

static void ProcExportKeyword( void )
/***********************************/
{
    unsigned_8  flags;
    length_name intname;
    length_name expname;
    ordinal_t   ordinal;

    flags = *wlink().ObjBuff++;
    expname.len = *wlink().ObjBuff++;
    expname.name = reinterpret_cast<char *>( wlink().ObjBuff );
    wlink().ObjBuff += expname.len;
    intname.len = *wlink().ObjBuff++;
    intname.name = reinterpret_cast<char *>( wlink().ObjBuff );
    wlink().ObjBuff += intname.len;
    ordinal = 0;
    if( flags & EXPDEF_ORDINAL ) {
        ordinal = MGET_LE_U16_UN( wlink().ObjBuff );
    }
    HandleExport( &expname, &intname, flags, ordinal );
}

static void ProcImportKeyword( void )
/***********************************/
{
    length_name intname;
    length_name modname;
    unsigned_8  info;

    info = *wlink().ObjBuff++;
    intname.len = *wlink().ObjBuff++;
    intname.name = reinterpret_cast<char *>( wlink().ObjBuff );
    wlink().ObjBuff += intname.len;
    modname.len = *wlink().ObjBuff++;
    modname.name = reinterpret_cast<char *>( wlink().ObjBuff );
    wlink().ObjBuff += modname.len;
    if( info == DLL_RELOC_NAME ) {
        if( *wlink().ObjBuff == 0 ) {   /* use internal name */
            HandleImport( &intname, &modname, &intname, IMP_BY_NAME );
        } else {
            length_name     extname;

            extname.len = *wlink().ObjBuff++;
            extname.name = reinterpret_cast<char *>( wlink().ObjBuff );
            wlink().ObjBuff += extname.len;
            HandleImport( &intname, &modname, &extname, IMP_BY_NAME );
        }
    } else {
        HandleImport(&intname, &modname, nullptr, MGET_LE_U16_UN( wlink().ObjBuff ) );
    }
}


static void DoMSOMF( void )
/**************************
 * Figure out debug info type and handle it accordingly later.
 */
{
    if( wlink().ObjBuff == wlink().EOObjRec ) {
        wlink().CurrMod->omfdbg = OMF_DBG_CODEVIEW;    /* Assume MS style */
    } else {
        unsigned_8 version;

        version = *wlink().ObjBuff++;
        if( wlink().ObjBuff[0] == 'C'
          && wlink().ObjBuff[1] == 'V' ) {
            wlink().CurrMod->omfdbg = OMF_DBG_CODEVIEW;
        } else if( wlink().ObjBuff[0] == 'H'
          && wlink().ObjBuff[1] == 'L' ) {
            switch( version ) {
            case 3:
                wlink().CurrMod->omfdbg = OMF_DBG_HLL_03;
                break;
            case 4:
                wlink().CurrMod->omfdbg = OMF_DBG_HLL_04;
                break;
            case 6:
                wlink().CurrMod->omfdbg = OMF_DBG_HLL_06;
                break;
            default:
                wlink().CurrMod->omfdbg = OMF_DBG_UNKNOWN;
                break;
            }
        } else {
            wlink().CurrMod->omfdbg = OMF_DBG_UNKNOWN;
        }
    }
}

static void ProcAlias( void )
/****************************
 * process a symbol alias directive
 */
{
    char        *alias;
    size_t      aliaslen;
    size_t      targetlen;
    symbol      *sym;

    for( ; wlink().ObjBuff < wlink().EOObjRec; wlink().ObjBuff += targetlen ) {
        aliaslen = *wlink().ObjBuff++;
        alias = reinterpret_cast<char *>( wlink().ObjBuff );
        wlink().ObjBuff += aliaslen;
        targetlen = *wlink().ObjBuff++;
        sym = SymOp( ST_FIND | ST_NOALIAS, alias, aliaslen );
        if( sym == nullptr
          || (sym->info & SYM_DEFINED) == 0 ) {
            MakeSymAlias( alias, aliaslen, reinterpret_cast<const char *>( wlink().ObjBuff ), targetlen );
        }
    }
}

static void ProcModuleEnd( void )
/*******************************/
{
    byte        frame;
    byte        target;
    unsigned    targetidx;
    segnode     *snode;
    extnode     *ext;
    bool        hasdisp;

    if( wlink().StartInfo.user_specd )
        return;
    target = *wlink().ObjBuff++;
    if( target & 0x40 ) {
        if( wlink().ObjBuff >= wlink().EOObjRec )
            return;
        target = *wlink().ObjBuff++;
        frame = (target >> 4) & 0x7;
        hasdisp = ( (target & 0x4) == 0 );
        target &= 0x3;
        if( frame <= 2 ) {              /* frame requires an index */
            SkipIdx();
        }
        targetidx = GetIdx();
        switch( target ) {
        case TARGET_SEG:
            if( wlink().StartInfo.type != START_UNDEFED ) {
                LnkMsg( MILD_ERR+LOC+MSG_MULT_START_ADDRS, "12", wlink().StartInfo.mod->u1.source->infile->name, wlink().StartInfo.mod->name );
                return;                 /* <-------- NOTE: premature return */
            }
            snode = static_cast<segnode *>( FindNode( wlink().SegNodes, targetidx ) );
            wlink().StartInfo.type = START_IS_SDATA;
            wlink().StartInfo.targ.sdata= snode->entry;
            if( (snode->info & SEGINF_CODE)
              && (wlink().LinkFlags & LF_STRIP_CODE) ) {
                RefSeg( snode->entry );
            }
            wlink().StartInfo.mod = wlink().CurrMod;
            break;
        case TARGET_EXT:
            ext = static_cast<extnode *>( FindNode( wlink().ExtNodes, targetidx ) );
            SetStartSym( ext->entry->name.u.ptr );
            break;
        case TARGET_ABS:
        case TARGET_GRP:
            BadObject();        /* no one does these, right??? */
            break;
        }
        if( hasdisp ) {
            if( wlink().ObjFormat & OBJ_FMT_32BIT_REC ) {
                wlink().StartInfo.off = MGET_LE_U32_UN( wlink().ObjBuff );
            } else {
                wlink().StartInfo.off = MGET_LE_U16_UN( wlink().ObjBuff );
            }
        }
    }
}

static void AddNames( void )
/***************************
 * Process NAMES record
 */
{
    int                 name_len;
    list_of_names       **entry;

    DEBUG(( DBG_OLD, "AddNames()" ));
    for( ; wlink().ObjBuff < wlink().EOObjRec; wlink().ObjBuff += name_len ) {
        name_len = *wlink().ObjBuff++;
        entry = static_cast<list_of_names **>( AllocNode( wlink().NameNodes ) );
        *entry = MakeListName( reinterpret_cast<char *>( wlink().ObjBuff ), name_len );
    }
}

static void ProcSegDef( void )
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
    sdata = AllocSegData();
    acbp = *wlink().ObjBuff++;
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
        sdata->frame = MGET_LE_U16_UN( wlink().ObjBuff );
        wlink().ObjBuff += sizeof( unsigned_16 ) + 1;
        break;
    case ALIGN_LTRELOC:
        /*
         * in 32 bit object files, ALIGN_LTRELOC is actually ALIGN_4KPAGE
         */
        if( (wlink().ObjFormat & OBJ_FMT_32BIT_REC) == 0
          && (wlink().FmtData.type & MK_RAW) == 0 ) {
            sdata->align = OMFAlignTab[ALIGN_PARA];
            /*
             * step over ltldat, max_seg_len, grp_offs fields
             */
            wlink().ObjBuff += 5;
        }
        break;
    }
    if( wlink().ObjFormat & OBJ_FMT_32BIT_REC ) {
        if( acbp & 2 ) {
            BadObject();            /* we can't handle 4 GB segments properly */
            return;
        }
        sdata->length = MGET_LE_U32_UN( wlink().ObjBuff );
        wlink().ObjBuff += sizeof( unsigned_32 );
    } else {
        if( acbp & 2 ) {
            sdata->length = 65536;  /* 64k segment */
        } else {
            sdata->length = MGET_LE_U16_UN( wlink().ObjBuff );
        }
        wlink().ObjBuff += sizeof( unsigned_16 );
    }
    name = FindName( GetIdx() );
    sdata->u.name.u.ptr = name->name;
    clname = FindName( GetIdx() );
    if( wlink().ObjFormat & OBJ_FMT_EASY_OMF ) {
        SkipIdx();                          /* skip overlay name index */
        sdata->bits = BITS_32;
        if( wlink().ObjBuff < wlink().EOObjRec ) {          /* the optional attribute field present */
            if( (*wlink().ObjBuff & 0x4) == 0 ) {   /* if USE32 bit not set */
                sdata->bits = BITS_16;
            }
        }
    } else if( acbp & 1 ) {
        sdata->bits = BITS_32;
    } else {
        sdata->bits = BITS_16;
    }
    sdata->iscode = IsCodeClass( clname->name, strlen( clname->name ) );
    snode = static_cast<segnode *>( AllocNode( wlink().SegNodes ) );
    snode->entry = sdata;
    AllocateSegment( snode, clname->name );
}

static void DefineGroup( void )
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

    grp_name = FindName( GetIdx() );
    DEBUG(( DBG_OLD, "DefineGroup() - %s", grp_name->name ));
    anchor = wlink().ObjBuff;
    num_segs = 0;
    while( wlink().ObjBuff < wlink().EOObjRec ) {
        if( *wlink().ObjBuff++ != GRP_SEGIDX ) {
            BadObject();
            return;
        }
        SkipIdx();  /* skip segment index */
        ++num_segs;
    }
    newnode = static_cast<grpnode *>( AllocNode( wlink().GrpNodes ) );
    group = SearchGroups( grp_name->name );
    if( group == nullptr ) {
        if( num_segs == 0 ) {
            newnode->entry = nullptr;
            return;                     /* NOTE: premature return! */
        }
        group = AllocGroup( grp_name->name, &wlink().Groups );
    }
    newnode->entry = group;
    wlink().ObjBuff = anchor;
    while( wlink().ObjBuff < wlink().EOObjRec ) {
        wlink().ObjBuff++;
        snode = static_cast<segnode *>( FindNode( wlink().SegNodes, GetIdx() ) );
        AddToGroup( group, snode->entry->u.leader );
    }
}

static void ProcPubdef( bool static_sym )
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
    SkipIdx();
    segidx = GetIdx();
    if( segidx != 0 ) {
        snode = static_cast<segnode *>( FindNode( wlink().SegNodes, segidx ) );
        frame = 0;
    } else {
        snode = nullptr;
        frame = MGET_LE_U16_UN( wlink().ObjBuff );
        wlink().ObjBuff += sizeof( unsigned_16 );
    }
    DEBUG(( DBG_OLD, "segidx = %d", segidx ));
    symop = ST_CREATE_DEFINE_NOALIAS;
    if( static_sym ) {
        symop |= ST_STATIC;
    }
    while( wlink().ObjBuff < wlink().EOObjRec ) {
        sym_len = *wlink().ObjBuff++;
        if( sym_len == 0 ) {
            BadObject();
        }
        sym_name = reinterpret_cast<char *>( wlink().ObjBuff );
        wlink().ObjBuff += sym_len;
        if( wlink().ObjFormat & OBJ_FMT_32BIT_REC ) {
            off = MGET_LE_U32_UN( wlink().ObjBuff );
            wlink().ObjBuff += sizeof( unsigned_32 );
        } else {
            off = MGET_LE_U16_UN( wlink().ObjBuff );
            wlink().ObjBuff += sizeof( unsigned_16 );
        }
        sym = SymOp( symop, sym_name, sym_len );
        DefineSymbol( sym, snode, off, frame );
        SkipIdx();   /* skip type index */
    }
}

static void DoLazyExtdef( bool isweak )
/**************************************
 * handle the lazy and weak extdef comments
 */
{
    extnode     *ext;
    symbol      *sym;
    unsigned    idx;

    while( wlink().ObjBuff < wlink().EOObjRec ) {
        ext = static_cast<extnode *>( FindNode( wlink().ExtNodes, GetIdx() ) );
        sym = ext->entry;
        ext->isweak = true;
        idx = GetIdx();
        ext = static_cast<extnode *>( FindNode( wlink().ExtNodes, idx ) );
        DefineLazyExtdef( sym, ext->entry, isweak );
    }
}

static void *GetVFListStart( void )
/*********************************/
{
    return( wlink().ObjBuff );
}

static void SetVFListStart( void *start )
/***************************************/
{
    wlink().ObjBuff = static_cast<byte *>( start );
}

static bool EndOfVFList( void )
/*****************************/
{
    return( wlink().ObjBuff >= wlink().EOObjRec );
}

static char *GetVFListName( void )
/*********************************/
{
    list_of_names       *lname;

    lname = FindName( GetIdx() );
    return( lname->name );
}

static void ProcVFTableRecord( bool ispure )
/*******************************************
 * process the watcom virtual function table information extension
 */
{
    extnode     *ext;
    symbol      *sym;
    vflistrtns  rtns;

    if( (wlink().LinkFlags & LF_VF_REMOVAL) == 0 )
        return;
    ext = static_cast<extnode *>( FindNode( wlink().ExtNodes, GetIdx() ) );
    sym = ext->entry;
    ext->isweak = true;
    ext = static_cast<extnode *>( FindNode( wlink().ExtNodes, GetIdx() ) );
    rtns.getstart = GetVFListStart;
    rtns.setstart = SetVFListStart;
    rtns.isend = EndOfVFList;
    rtns.getname = GetVFListName;
    DefineVFTableRecord( sym, ext->entry, ispure, &rtns );
}

static void ProcVFReference( void )
/**********************************
 * process a vftable reference record
 */
{
    extnode             *ext;
    segnode             *snode;
    symbol              *sym;
    list_of_names       *lname;
    unsigned            index;

    if( (wlink().LinkFlags & LF_VF_REMOVAL) == 0 )
        return;
    index = GetIdx();
    if( index == 0 ) {
        LnkMsg( WRN+LOC+MSG_NOT_COMPILED_VF_ELIM, nullptr );
        return;
    }
    ext = static_cast<extnode *>( FindNode( wlink().ExtNodes, index ) );
    if( wlink().LinkFlags & LF_STRIP_CODE ) {
        if( *wlink().ObjBuff == 0 ) {   /* it is a comdat index */
            wlink().ObjBuff++;
            lname = FindName( GetIdx() );
            sym = FindISymbol( lname->name );
            if( sym == nullptr ) {
                sym = MakeWeakExtdef( lname->name, nullptr );
            }
            DefineVFReference( sym, ext->entry, true );
        } else {                /* it's a seg idx */
            snode = static_cast<segnode *>( FindNode( wlink().SegNodes, GetIdx() ) );
            DefineVFReference( snode, ext->entry, false );
        }
    }
    if( (ext->entry->info & SYM_DEFINED) == 0 ) {
        ext->entry->info |= SYM_1_VF_MARKED;
    }
}

static void UseSymbols( bool static_sym, bool iscextdef )
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
    while( wlink().ObjBuff < wlink().EOObjRec ) {
        if( iscextdef ) {
            lnptr = FindName( GetIdx() );
            sym = RefISymbol( lnptr->name );
        } else {
            sym_len = *wlink().ObjBuff++;
            if( sym_len == 0 ) {
                BadObject();
            }
            sym = SymOp( symop, reinterpret_cast<char *>( wlink().ObjBuff ), sym_len );
            wlink().ObjBuff += sym_len;
        }
        newnode = static_cast<extnode *>( AllocNode( wlink().ExtNodes ) );
        newnode->entry = sym;
        newnode->isweak = false;
        DefineReference( sym );
        SkipIdx();/*  skip type index */
    }
}

void SkipIdx( void )
/*******************
 * skip the index
 */
{
    if( *wlink().ObjBuff++ & IS_2_BYTES ) {
        wlink().ObjBuff++;
    }
}

unsigned_16 GetIdx( void )
/*************************
 * Get an index.
 */
{
    unsigned_16 index;

    index = *wlink().ObjBuff++;
    if( index & IS_2_BYTES ) {
        index = (index & 0x7f) * 256 + *wlink().ObjBuff++;
    }
    return( index );
}

list_of_names *FindName( unsigned_16 index )
/*******************************************
 * Find name of specified index.
 */
{
    return( *static_cast<list_of_names **>( FindNode( wlink().NameNodes, index ) ) );
}

static void ProcLinnum( void )
/*****************************
 * do some processing for the linnum record
 */
{
    segnode     *snode;
    bool        is32bit;

    SkipIdx();          /* don't need the group idx */
    snode = static_cast<segnode *>( FindNode( wlink().SegNodes, GetIdx() ) );
    if( snode->info & SEGINF_DEAD )   /* ignore dead segments */
        return;
    is32bit = ( (wlink().ObjFormat & OBJ_FMT_32BIT_REC) != 0 );
    DBIAddLines( snode->entry, wlink().ObjBuff, wlink().EOObjRec - wlink().ObjBuff, is32bit );
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

static byte *ProcIDBlock( virt_mem *dest, byte *buffer, unsigned iterate )
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
        return( wlink().EOObjRec );
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
        if( wlink().ObjFormat & OBJ_FMT_MS_386 ) {
            do {
                buffer = anchor;
                inner = count;
                do {
                    rep = MGET_LE_U32_UN( buffer );
                    buffer += sizeof( unsigned_32 );
                    buffer = ProcIDBlock( dest, buffer, rep );
                } while( --inner != 0 );
            } while( --iterate != 0 );
        } else {
            do {
                buffer = anchor;
                inner = count;
                do {
                    rep = MGET_LE_U16_UN( buffer );
                    buffer += sizeof( unsigned_16 );
                    buffer = ProcIDBlock( dest, buffer, rep );
                } while( --inner != 0 );
            } while( --iterate != 0 );
        }
    }
    return( buffer );
}

static void DoLIData( virt_mem start, byte *data, size_t size )
/**************************************************************
 * Expand logically iterated data.
 */
{
    unsigned    rep;
    byte        *end_data;

    end_data = data + size;
    while( data < end_data ) {
        if( wlink().ObjFormat & OBJ_FMT_MS_386 ) {
            rep = MGET_LE_U32_UN( data );
            data += sizeof( unsigned_32 );
        } else {
            rep = MGET_LE_U16_UN( data );
            data += sizeof( unsigned_16 );
        }
        data = ProcIDBlock( &start, data, rep );
    }
}

static void GetObject( segdata *snode, unsigned obj_offset, bool lidata )
/************************************************************************
 * Load object code.
 */
{
    size_t      size;
    virt_mem    start;

    if( snode->isdead
      || snode->isabs ) {   /* ignore dead or abs segments */
        wlink().ObjFormat |= OBJ_FMT_IGNORE_FIXUPP; /* and any corresponding fixupps */
        return;
    }
    wlink().ObjFormat &= ~(OBJ_FMT_IGNORE_FIXUPP | OBJ_FMT_IS_LIDATA);
    if( lidata ) {
        wlink().ObjFormat |= OBJ_FMT_IS_LIDATA;
    }
    if( wlink().ObjBuff != wlink().EOObjRec ) {
        size = wlink().EOObjRec - wlink().ObjBuff;
        start = snode->u1.vm_ptr + obj_offset;
        if( lidata ) {
            DoLIData( start, wlink().ObjBuff, size );
        } else {
            if( size + obj_offset > snode->length ) {
                LnkMsg( FTL+LOC_REC+MSG_OBJ_FILE_ATTR, nullptr );
            }
            PutInfo( start, wlink().ObjBuff, size );
        }
    }
    SetCurrSeg( snode, obj_offset, nullptr );
}

static void ProcLxdata( bool islidata )
/**************************************
 * process ledata and lidata records
 */
{
    segnode     *snode;
    unsigned    obj_offset;

    snode = static_cast<segnode *>( FindNode( wlink().SegNodes, GetIdx() ) );
    snode->entry->u.leader->info |= SEGINF_LXDATA_SEEN;
    snode->info |= SEGINF_LXDATA_SEEN;
    if( wlink().ObjFormat & OBJ_FMT_32BIT_REC ) {
        obj_offset = MGET_LE_U32_UN( wlink().ObjBuff );
        wlink().ObjBuff += sizeof( unsigned_32 );
    } else {
        obj_offset = MGET_LE_U16_UN( wlink().ObjBuff );
        wlink().ObjBuff += sizeof( unsigned_16 );
    }
#ifdef DEVBUILD
    if( stricmp( snode->entry->u.leader->segname.u.ptr, "_BSS" ) == 0 ) {
        LnkMsg( ERR+LOC_REC+MSG_INTERNAL, "s", "Initialized BSS found" );
    }
#endif
    GetObject( snode->entry, obj_offset, islidata );
}

static void ProcHllLinnum( void )
/****************************/
// Because HLL have FIXUPs to the LINNUM records we treat it as
// a LEDATA record for a special $$LINES segment.
// The leading group & segment indexes doesn't figure in
// the 'LEDATA' - at least not for HLL v4.
// FIXME: check what the LINNUM records looks like in the other HLL versions!
{
    segnode         *snode;
    unsigned        obj_offset;

    if( (wlink().LinkFlags & LF_HLL_DBI_FLAG) == 0 )
        return;

    /* find the $$LINES segment, create it if not found with this module. */
    snode = nullptr;
    IterateNodelist( wlink().SegNodes, EnumFindLinesSeg, &snode );
    if( snode == nullptr ) {
        segdata             *sdata;
        segnode             *snode1;
        omf_record          *rec;
        unsigned            loc;
        unsigned            loc_1st_linnum;
        unsigned            len_1st_linnum;
        byte                *buf;

        sdata = AllocSegData();
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
        loc = wlink().CurrMod->location;
        DEBUG(( DBG_HLL, "ProcHllLinnum: counting LINNUM data for %s", wlink().CurrMod->name ));
        do {
            rec = static_cast<omf_record *>( CacheRead( wlink().CurrMod->u1.source, loc, sizeof( *rec ) ) );
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
        buf = static_cast<byte *>( CacheRead( wlink().CurrMod->u1.source, loc_1st_linnum, len_1st_linnum ) );
        if ( buf != wlink().ObjBuff ) {
            wlink().EOObjRec = buf + len_1st_linnum - 1;
            wlink().ObjBuff = buf;
        }

        snode1 = static_cast<segnode *>( AllocNode( wlink().SegNodes ) );
        snode1->entry = sdata;
        AllocateSegment( snode1, _HllLineClass );

        st().HllLineNumOffset = 0;
        IterateNodelist( wlink().SegNodes, EnumFindLinesSeg, &snode );
    }

    SkipIdx(); // group idx (0)
    SkipIdx(); // segment idx (ignored)

    /* Do like ProcLxData */
    obj_offset = st().HllLineNumOffset;
    DEBUG(( DBG_HLL, "ProcHllLinnum: HllLineNumOffset=%d length=%h", st().HllLineNumOffset, wlink().EOObjRec - wlink().ObjBuff ));
    st().HllLineNumOffset += wlink().EOObjRec - wlink().ObjBuff;
    snode->entry->u.leader->info |= SEGINF_LXDATA_SEEN;
    snode->info |= SEGINF_LXDATA_SEEN;
    GetObject( snode->entry, obj_offset, false );
}

static void LinkDirective( void )
/*******************************/
{
    unsigned        directive;
    lib_priority    priority;
    segnode         *snode;

    directive = *wlink().ObjBuff++;
    switch( directive ) {
    case LDIR_DEFAULT_LIBRARY:
        if( wlink().ObjBuff < wlink().EOObjRec ) {
            priority = static_cast<lib_priority>( *wlink().ObjBuff++ - '0' );
            AddCommentLib( reinterpret_cast<char *>( wlink().ObjBuff ), wlink().EOObjRec - wlink().ObjBuff, priority );
        }
        break;
    case LDIR_SOURCE_LANGUAGE:
        DBIP1Source( wlink().ObjBuff, wlink().EOObjRec );
        break;
    case LDIR_VF_PURE_DEF:
        ProcVFTableRecord( true );
        break;
    case LDIR_VF_TABLE_DEF:
        ProcVFTableRecord( false );
        break;
    case LDIR_VF_REFERENCE:
        ProcVFReference();
        break;
    case LDIR_PACKDATA:
        if( (wlink().LinkFlags & LF_PACKDATA_FLAG) == 0 ) {
            wlink().PackDataLimit = MGET_LE_U32_UN( wlink().ObjBuff );
            wlink().LinkFlags |= LF_PACKDATA_FLAG;
        }
        break;
    case LDIR_OPT_FAR_CALLS:
        snode = static_cast<segnode *>( FindNode( wlink().SegNodes, GetIdx() ) );
        snode->entry->canfarcall = true;
        snode->entry->iscode = true;
        break;
    case LDIR_FLAT_ADDRS:
        wlink().CurrMod->flags_mod |= MOD_FLATTEN_DBI;
        break;
    case LDIR_OPT_UNSAFE:
        wlink().ObjFormat |= OBJ_FMT_UNSAFE_FIXUPP;
        break;
    }
}

static void Comment( void )
/**************************
 * Process a comment record.
 */
{
    unsigned char       attribute;
    unsigned char       class_;
    int                 proc;
    unsigned char       which;

    attribute = *wlink().ObjBuff++;
    class_ = *wlink().ObjBuff++;

    switch( class_ ) {
    case CMT_DLL_ENTRY:
        which = *wlink().ObjBuff++;
        switch( which ) {
        case MOMF_IMPDEF:
        case MOMF_EXPDEF:
            if( SeenDLLRecord() ) {
                if( which == MOMF_EXPDEF ) {
                    ProcExportKeyword();
                } else {
                    ProcImportKeyword();
                }
            }
            break;
        case MOMF_PROT_LIB:
            if( wlink().FmtData.type & MK_WIN_NE ) {
                wlink().FmtData.u.os2fam.is_private_dll = true;
            }
            break;
        }
        break;
    case CMT_WAT_PROC_MODEL:
    case CMT_MS_PROC_MODEL:
        proc = *wlink().ObjBuff - '0';
        if( proc > wlink().FmtData.cpu_type )
            wlink().FmtData.cpu_type = proc;
        break;
    case CMT_DOSSEG:
        wlink().LinkState |= LS_DOSSEG_FLAG;
        break;
    case CMT_DEFAULT_LIBRARY:
        AddCommentLib( reinterpret_cast<char *>( wlink().ObjBuff ), wlink().EOObjRec - wlink().ObjBuff, static_cast<lib_priority>( LIB_PRIORITY_MAX - 2 ) );
        break;
    case CMT_LINKER_DIRECTIVE:
        LinkDirective();
        break;
    case CMT_WKEXT:
        DoLazyExtdef( true );
        break;
    case CMT_LZEXT:
        DoLazyExtdef( false );
        break;
    case CMT_EASY_OMF:
        if( memcmp( wlink().ObjBuff, EASY_OMF_SIGNATURE, 5 ) == 0 ) {
            wlink().ObjFormat |= OBJ_FMT_EASY_OMF;
        }
        break;
    case CMT_SOURCE_NAME:
        DBIComment();
        break;
    case CMT_MS_OMF:
        DoMSOMF();
        break;
    case 0x80:      /* Code Gen used to put bytes out in wrong order */
        if( attribute == CMT_SOURCE_NAME ) {    /* no longer generated */
            DBIComment();
        } else if( attribute == CMT_LINKER_DIRECTIVE ) {  /* linker directive */
            LinkDirective();
        }
        break;
    }
}

static void Pass1Cmd( byte cmd )
/*******************************
 * Process an object record for pass 1
 */
{
    bool isstatic;

    isstatic = false;
    switch( cmd ) {
    case CMD_THEADR:
        ProcTHEADR();
        break;
    case CMD_COMENT:
        Comment();
        break;
    case CMD_LLNAMES:
    case CMD_LNAMES:
        AddNames();
        break;
    case CMD_SEGD32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_SEGDEF:
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcSegDef();
        break;
    case CMD_STATIC_PUBD32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_STATIC_PUBDEF:
        ProcPubdef( true );
        break;
    case CMD_PUBD32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_PUBDEF:
        ProcPubdef( false );
        break;
    case CMD_STATIC_EXTDEF:
    case CMD_STATIC_EXTD32:
        isstatic = true;
    case CMD_EXTDEF:
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        UseSymbols( isstatic, false );
        break;
    case CMD_CEXTDEF:
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        UseSymbols( false, true );
        break;
    case CMD_GRPDEF:
        DefineGroup();
        break;
    case CMD_LINN32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LINNUM:
        switch( wlink().CurrMod->omfdbg ) {
        case OMF_DBG_CODEVIEW:
            ProcLinnum();
            break;
        case OMF_DBG_HLL_03:
        case OMF_DBG_HLL_04:
        case OMF_DBG_HLL_06:
            ProcHllLinnum();
            break;
        }
        break;
    case CMD_LINS32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LINSYM:
        ProcLinsym();
        break;
    case CMD_STATIC_COMDEF:
        isstatic = true;
    case CMD_COMDEF:
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcComdef( isstatic );
        break;
    case CMD_COMD32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_COMDAT:
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcComdat();
        break;
    case CMD_LEDA32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LEDATA:
        ProcLxdata( false );
        break;
    case CMD_LIDA32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_LIDATA:
        ProcLxdata( true );
        break;
    case CMD_FIXU32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_FIXUPP:        /* count the fixups for each seg_leader */
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        DoRelocs();
        wlink().ObjFormat &= ~OBJ_FMT_UNSAFE_FIXUPP;
        break;
    case CMD_MODE32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_MODEND:
        ProcModuleEnd();
        break;
    case CMD_ALIAS:
        ProcAlias();
        break;
    case CMD_BAKP32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_BAKPAT:
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcBakpat();
        break;
    case CMD_NBKP32:
        wlink().ObjFormat |= OBJ_FMT_MS_386;
    case CMD_NBKPAT:
        wlink().CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ProcNbkpat();
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
        wlink().CurrMod->u1.source->infile->status |= INSTAT_IOERR;
        LnkMsg( ERR+LOC_REC+MSG_BAD_REC_TYPE, "x", cmd );
        break;
    }
}

unsigned long OMFPass1( void )
/*****************************
 * do pass 1 for OMF object files
 */
{
    unsigned long retval;

    PermStartMod( wlink().CurrMod );
    if( wlink().LinkState & (LS_HAVE_MACHTYPE_MASK & ~LS_HAVE_X86_CODE) ) {
        LnkMsg( WRN+MSG_MACHTYPE_DIFFERENT, "s", wlink().CurrMod->u1.source->infile->name);
    } else {
        wlink().LinkState |= LS_HAVE_X86_CODE;
    }
    wlink().CurrMod->omfdbg = OMF_DBG_CODEVIEW; /* Assume MS style LINNUM records */
    retval = ProcObj( wlink().CurrMod->u1.source, wlink().CurrMod->location, &Pass1Cmd );
    IterateNodelist( wlink().SegNodes, CheckUninit, nullptr );
    ResolveComdats();
    return( retval );
}
