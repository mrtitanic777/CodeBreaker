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
* Description:  Pass 1 of Open Watcom linker.
*
****************************************************************************/


#include <span>
#include <string.h>
#include <stdlib.h>
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "specials.h"
#include "objnode.h"
#include "objcalc.h"
#include "dbgall.h"
#include "overlays.h"
#include "mapio.h"
#include "cmdline.h"
#include "objstrip.h"
#include "cmdnov.h"
#include "cmdelf.h"
#include "impexp.h"
#include "ring.h"
#include "strtab.h"
#include "carve.h"
#include "permdata.h"
#include "virtmem.h"
#include "obj2supp.h"
#include "objpass2.h"
#include "objpass1.h"
#include "loadpe.h"

#include "clibext.h"
#include "run_state.hpp"


#define MAX_SEGMENT     0x10000

namespace {

struct Pass1State {
    seg_leader      *LastCodeSeg = nullptr;     // last code segment in current module
    int             segment_ord = 0;
};

inline Pass1State &st() { return cb_wlink::RunState::part<Pass1State>(); }

}

void ResetObjPass1( void )
/************************/
{
    wlink().ObjFileFormat = static_cast<file_format>(0);
    wlink().ObjFormat = static_cast<obj_format>(0);
}

void P1Start( void )
/******************/
{
    Pass1State &s = st();
    s.LastCodeSeg = nullptr;
    s.segment_ord = 0;
}

static void DoSavedImport( symbol *sym )
/**************************************/
{
    dll_sym_info        *dll;
    length_name         modname;
    length_name         extname;

    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE) ) {
        dll = sym->p.import_dll;
        sym->p.import_dll = nullptr;
        sym->info &= ~SYM_DEFINED;
        modname.name = dll->m.modname.u.ptr;
        modname.len = strlen( modname.name );
        if( dll->isordinal ) {
            MSImportKeyword( sym, &modname, nullptr, dll->u.ordinal );
        } else {
            extname.name = dll->u.entname.u.ptr;
            extname.len = strlen( extname.name );
            MSImportKeyword( sym, &modname, &extname, IMP_BY_NAME );
        }
        FreeImport( dll );
    }
}

static void DoSavedExport( symbol *sym )
/**************************************/
{
    /* unused parameters */ static_cast<void>(sym);

#ifdef _OS2
    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        entry_export    *exp;

        exp = static_cast<entry_export *>(sym->e.export_);
        exp->sym = sym;
        exp->impname = nullptr;
        AddToExportList( exp );
    }
#endif
#ifdef _NOVELL
    if( wlink().FmtData.type & MK_NOVELL ) {
        AddNameTable( sym->name.u.ptr, strlen( sym->name.u.ptr ), true, &wlink().FmtData.u.nov.exp.export_ );
    }
#endif
#ifdef _ELF
    if( wlink().FmtData.type & MK_ELF ) {
        AddNameTable( sym->name.u.ptr, strlen( sym->name.u.ptr ), true, &wlink().FmtData.u.elf.exp.export_ );
    }
#endif
}

static bool StoreCDatData( void *_piece, void *_loc )
/***************************************************/
{
    comdat_piece *piece = static_cast<comdat_piece *>(_piece);
    virt_mem     *loc = static_cast<virt_mem *>(_loc);

    PutInfo( *loc, piece->data, piece->length );
    *loc += piece->length;
    return( false );
}

void StoreInfoData( comdat_info *info )
/*************************************/
{
    virt_mem    temp;

    info->sdata->u1.vm_ptr = AllocStg( info->sdata->length );
    temp = info->sdata->u1.vm_ptr;
    RingLookup( info->pieces, StoreCDatData, &temp );
}

static bool CheckVMemPieceDiff( void *_piece, void *_loc )
/********************************************************/
{
    comdat_piece *piece = static_cast<comdat_piece *>(_piece);
    virt_mem     *loc = static_cast<virt_mem *>(_loc);
    bool         retval;

    retval = !CompareInfo( *loc, piece->data, piece->length );
    *loc += piece->length;
    return( retval );
}

static bool CheckMemPieceDiff( void *_piece, void *_loc )
/*******************************************************/
{
    comdat_piece *piece = static_cast<comdat_piece *>(_piece);
    char         **loc = static_cast<char **>(_loc);
    bool         retval;

    retval = memcmp( *loc, piece->data, piece->length ) != 0;
    *loc += piece->length;
    return( retval );
}

static bool CheckSameData( symbol *sym, comdat_info *info )
/*********************************************************/
{
    virt_mem        vmem;
    char            *data;
    comdat_piece    *piece;

    if( sym->mod->flags_mod & MOD_DONE_PASS_1 ) {
        vmem = sym->p.seg->u1.vm_ptr;
        piece = static_cast<comdat_piece *>(RingLookup( info->pieces, CheckVMemPieceDiff, &vmem ));
    } else {
        data = static_cast<char *>(GetSegContents( sym->p.seg, sym->p.seg->u1.vm_offs ));
        piece = static_cast<comdat_piece *>(RingLookup( info->pieces, CheckMemPieceDiff, &data ));
    }
    if( piece == nullptr ) {       // found a match
        info->sym->u.datasym = sym;
        return( true );
    }
    return( false );
}

static bool CheckAltSym( void *_sym, void *_info )
/************************************************/
{
    comdat_info *info = static_cast<comdat_info *>(_info);
    symbol      *sym = static_cast<symbol *>(_sym);

    if( sym != info->sym
      && IS_SYM_COMDAT( sym )
      && (sym->info & SYM_HAS_DATA) ) {
        return( CheckSameData( sym, info ) );
    }
    return( false );
}

void InfoCDatAltDef( comdat_info *info )
/**************************************/
{
    symbol      *mainsym;

    mainsym = info->sym;
    info->sym = AddAltDef( mainsym, SYM_COMDAT );
    info->sym->p.seg = info->sdata;
    info->sym->info |= info->flags & SYM_CDAT_SEL_MASK;
    if( !CheckSameData( mainsym, info ) ) {
        mainsym = static_cast<symbol *>(RingLookup( mainsym->u.altdefs, CheckAltSym, info ));
        if( mainsym == nullptr ) {
            StoreInfoData( info );
            info->sym->info |= SYM_HAS_DATA;
        }
    }
}

static void AddCDatAltDef( segdata *sdata, symbol *sym, unsigned_8 *data,
                           sym_info flags )
/***********************************************************************/
{
    comdat_info     info;
    comdat_piece    piece;

    if( wlink().LinkFlags & LF_INC_LINK_FLAG ) {
        info.sdata = sdata;
        info.sym = sym;
        info.flags = flags;
        info.pieces = nullptr;
        piece.data = data;
        piece.length = sdata->length;
        piece.free_data = false;
        RingAppend( &info.pieces, &piece );
        InfoCDatAltDef( &info );
    }
}

static void DoIncSymbol( void *_sym )
/***********************************/
{
    symbol      *sym = static_cast<symbol *>(_sym);
    symbol      *mainsym;
    void        *data;
    sym_flags   symop;

    if( sym->info & SYM_IS_ALTDEF ) {
        symop = ST_CREATE_REFERENCE;
        if( sym->info & SYM_STATIC ) {
            symop |= ST_STATIC;
        }
        mainsym = SymOp( symop, sym->name.u.ptr, strlen( sym->name.u.ptr ) );
        if( IS_SYM_NICOMDEF( sym ) ) {
            MakeCommunalSym( mainsym, sym->p.cdefsize, (sym->info & SYM_FAR_COMMUNAL) != 0, IS_SYM_COMM32( sym ) ? BITS_32 : BITS_16 );
        } else if( IS_SYM_COMDAT( sym ) ) {
            if( sym->info & SYM_HAS_DATA ) {
                data = GetAltdefContents( sym->p.seg );
            } else {
                data = GetSegContents( sym->p.seg, sym->p.seg->u1.vm_offs );
            }
            sym->p.seg->isdead = false;
            DefineComdat( sym->p.seg, mainsym, sym->addr.off, sym->info & SYM_CDAT_SEL_MASK, static_cast<unsigned_8 *>(data) );
        } else if( (mainsym->info & SYM_DEFINED) == 0 )  {
            DoSavedImport( sym );       // FIXME can lose defs here.
        }
        CarveFree( wlink().CarveSymbol, sym );
    } else {
        if( !IS_SYM_COMDAT( sym ) ) {
            Ring2Append( &wlink().CurrMod->publist, sym );
        }
        if( sym->info & SYM_DEAD )
            return;
        if( IS_SYM_IMPORTED( sym ) ) {
            DoSavedImport( sym );
        } else if( IS_SYM_COMDAT( sym ) ) {
            DefineComdat( sym->p.seg, sym, sym->addr.off, sym->info & SYM_CDAT_SEL_MASK, static_cast<unsigned_8 *>(GetSegContents(sym->p.seg, sym->p.seg->u1.vm_offs)) );
        }
        if( sym->info & SYM_EXPORTED ) {
            DoSavedExport( sym );
        }
    }
}

static class_entry *FindNamedClass( const char *name )
/****************************************************/
// NYI:  this doesn't take into account 16 & 32 bit classes with the same name.
{
    class_entry   *class_;

    for( class_ = wlink().Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( stricmp( class_->name.u.ptr, name ) == 0 ) {
            return( class_ );
        }
    }
    return( nullptr );
}

static bool CmpSegName( void *leader, void *name )
/************************************************/
{
    return( stricmp( static_cast<seg_leader *>(leader)->segname.u.ptr, static_cast<const char *>(name) ) == 0 );
}

static bool DefIncGroup( void *_def, void *_grouptab )
/****************************************************/
{
    incgroupdef     *def = static_cast<incgroupdef *>(_def);
    group_entry     ***grouptab = static_cast<group_entry ***>(_grouptab);
    group_entry     *group;
    class_entry     *class_;
    seg_leader      *leader;

    group = GetGroup( def->grpname );
    // the names are pairs: a class name, then a segment name
    std::span<const char *const> names( def->names, static_cast<size_t>( def->numsegs ) * 2 );
    for( size_t i = 0; i < names.size(); i += 2 ) {
        class_ = FindNamedClass( names[i] );
        if( class_ != nullptr ) {
            leader = static_cast<seg_leader *>(RingLookup( class_->segs, CmpSegName, const_cast<char *>(names[i + 1]) ));
            if( leader != nullptr ) {
                AddToGroup( group, leader );
            }
        }
    }
    **grouptab = group;
    (*grouptab)++;
    return( false );
}

void DoIncGroupDefs( void )
/*************************/
{
    unsigned        numgroups;
    group_entry     **grouptab;

    numgroups = RingCount( wlink().IncGroupDefs );
    wlink().IncGroups = static_cast<group_entry **>(MemAllocSafe( sizeof( group_entry * ) * numgroups ));
    grouptab = wlink().IncGroups;
    RingLookup( wlink().IncGroupDefs, DefIncGroup, &grouptab );
    RingFree( &wlink().IncGroupDefs );
}

void Set64BitMode( void )
/***********************/
// make sure that the executable format is a 64-bit format.
{
    wlink().LinkState |= LS_FMT_SEEN_64BIT;
    if( !HintFormat( MK_ALLOW_64 ) ) {
        if( (wlink().ObjFormat & OBJ_FMT_TOLD_BITNESS) == 0 ) {
            wlink().ObjFormat |= OBJ_FMT_TOLD_BITNESS;
            LnkMsg( WRN+MSG_FOUND_XXBIT_OBJ, "sd",
                        wlink().CurrMod->u1.source->infile->name.u.ptr, 64 );
        }
    }
}

void Set32BitMode( void )
/***********************/
// make sure that the executable format is a 32-bit format.
{
    wlink().LinkState |= LS_FMT_SEEN_32BIT;
    if( !HintFormat( MK_ALLOW_32 ) ) {
        if( (wlink().ObjFormat & OBJ_FMT_TOLD_BITNESS) == 0 ) {
            wlink().ObjFormat |= OBJ_FMT_TOLD_BITNESS;
            LnkMsg( WRN+MSG_FOUND_XXBIT_OBJ, "sd",
                        wlink().CurrMod->u1.source->infile->name.u.ptr, 32 );
        }
    }
}

void Set16BitMode( void )
/***********************/
{
    if( !HintFormat( MK_ALLOW_16 ) ) {
        if( (wlink().ObjFormat & OBJ_FMT_TOLD_BITNESS) == 0 ) {
            wlink().ObjFormat |= OBJ_FMT_TOLD_BITNESS;
            LnkMsg( WRN+MSG_FOUND_XXBIT_OBJ, "sd",
                    wlink().CurrMod->u1.source->infile->name.u.ptr, 16 );
        }
    }
}

static void DoAllocateSegment( segdata *sdata, const char *clname )
/*****************************************************************/
{
    section         *sect;
    class_entry     *class_;
#ifdef _EXE
    bool            isovlclass;

    isovlclass = false;
    if( wlink().FmtData.type & MK_OVERLAYS ) {
        sdata->iscode = OvlCheckOvlClass( clname, &isovlclass );
    }
#endif
    if( sdata->iscode ) {
        if( sdata->bits == BITS_16 ) {
            wlink().LinkState |= LS_HAVE_16BIT_CODE;
        }
    }
    sect = DBIGetSect( clname );
    if( sect == nullptr ) {
        sect = wlink().CurrSect;
#ifdef _EXE
        if( (wlink().FmtData.type & MK_OVERLAYS)
          && !isovlclass ) {
            sect = wlink().NonSect;
        }
#endif
    }
    class_ = FindClass( sect, clname, sdata->bits, sdata->iscode );
    AddSegment( sdata, class_ );
#ifdef _EXE
    if( isovlclass ) {
        sdata->u.leader->info |= SEGINF_OVERLAYED;
    }
#endif
    if( !sdata->isdead
      && !sdata->isuninit
      && !sdata->iscdat ) {
        sdata->u1.vm_ptr = AllocStg( sdata->length );
    }
}

void AllocateSegment( segnode *snode, const char *clname )
/*********************************************************
 * allocate a new segment (or new piece of a segment)
 */
{
    DoAllocateSegment( snode->entry, clname );
    snode->info = snode->entry->u.leader->info;
}

unsigned long IncPass1( void )
/****************************/
{
    segdata         *seglist;
    segdata         *seg;
    symbol          *publist;
    virt_mem_size   dataoff;
    size_t          relocs;

    seglist = wlink().CurrMod->segs;
    wlink().CurrMod->segs = nullptr;
    wlink().CurrMod->lines = nullptr;
    for( ;; ) {
        seg = static_cast<segdata *>(Ring2Pop( &seglist ));
        if( seg == nullptr )
            break;
        dataoff = seg->u1.vm_offs;
        DoAllocateSegment( seg, seg->o.clname.u.ptr );
        seg->o.mod = wlink().CurrMod;
        if( !seg->isuninit
          && !seg->isdead
          && !seg->iscdat ) {
            PutInfo( seg->u1.vm_ptr, GetSegContents( seg, dataoff ), seg->length );
            seg->u.leader->info |= SEGINF_LXDATA_SEEN;
        }
    }
    publist = wlink().CurrMod->publist;
    wlink().CurrMod->publist = nullptr;
    Ring2Walk( publist, DoIncSymbol );
    relocs = wlink().CurrMod->relocs;
    PermStartMod( wlink().CurrMod );    // destroys currmod->relocs
    IterateModRelocs( relocs, wlink().CurrMod->sizerelocs, IncSaveRelocs );
    return( 0 );
}

#ifdef _QNX
static void CheckQNXSegMismatch( stateflag mask )
/***********************************************/
{
    if( (wlink().FmtData.type & MK_QNX)
      && (wlink().LinkState & mask)
      && !wlink().FmtData.u.qnx.seen_mismatch ) {
        LnkMsg( WRN+LOC+MSG_CANNOT_HAVE_16_AND_32, nullptr );
        wlink().FmtData.u.qnx.seen_mismatch = true;
    }
}
#endif

class_entry *DuplicateClass( class_entry *class_ )
/***********************************************/
{
    class_entry *newclass;

    newclass = static_cast<class_entry *>(CarveAlloc( wlink().CarveClass ));
    memcpy( newclass, class_, sizeof( class_entry ) );
    newclass->name.u.ptr = AddBufferStringTable( &wlink().PermStrings, class_->name.u.ptr, strlen( class_->name.u.ptr ) + 1 );
    class_->next = newclass;
    return( newclass );
}

class_entry *FindClass( section *sect, const char *name, byte bits, bool iscode )
/*******************************************************************************/
{
    class_entry     *class_;
    class_entry     *lastclass;
    size_t          namelen;

    lastclass = sect->classes;
    for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
        if( stricmp( class_->name.u.ptr, name ) == 0
          && ( class_->bits == bits ) ) {
            return( class_ );
        }
        lastclass = class_;
    }
    namelen = strlen( name );
    class_ = static_cast<class_entry *>(CarveAlloc( wlink().CarveClass ));
    class_->bits = bits;
    class_->name.u.ptr = AddBufferStringTable( &wlink().PermStrings, name, namelen + 1 );
    class_->segs = nullptr;
    class_->section = sect;
    class_->next = nullptr;
    class_->flags = ( iscode ) ? CLASS_CODE : CLASS_NONE;
    if( IsConstClass( name, namelen ) ) {
        class_->flags |= CLASS_READ_ONLY;
    }
    if( IsStackClass( name, namelen ) ) {
        class_->flags |= CLASS_STACK;
    }
    DBIColClass( class_ );
    if( lastclass == nullptr ) {
        sect->classes = class_;
    } else {
        lastclass->next = class_;
    }
    return( class_ );
}

static void CheckForLast( seg_leader *seg, class_entry *class_ )
/*************************************************************/
// check to see if this segment should be the last one in an autogroup.
{
    if( (wlink().CurrMod->flags_mod & MOD_LAST_SEG)
      && (class_->flags & CLASS_CODE) ) {
        seg_leader *&last = st().LastCodeSeg;
        if( last != nullptr ) {             // more than one code seg
            last->info &= ~SEGINF_LAST_SEGMENT; // so don't end at previous
        }
        last = seg;
        seg->info |= SEGINF_LAST_SEGMENT;
    }
}

static bool CheckClassName( void *_seg, void *_sdata )
/****************************************************/
{
    seg_leader  *seg = static_cast<seg_leader *>(_seg);
    segdata     *sdata = static_cast<segdata *>(_sdata);

    return( stricmp( seg->segname.u.ptr, sdata->u.name.u.ptr ) == 0 && seg->combine != COMBINE_INVALID );
}

static void AddToLeader( seg_leader *seg, segdata *sdata )
/********************************************************/
{
    segdata     *first;
    offset      length;

    sdata->u.leader = seg;
    if( sdata->combine == COMBINE_COMMON ) {
        first = static_cast<segdata *>(RingFirst( seg->pieces ));
        length = first->length;
        if( length < sdata->length )
            length = sdata->length;
        if( first->isuninit ) {
            first->isdead = true;
            RingPush( &seg->pieces, sdata );
            sdata->length = length;
        } else {
            sdata->isdead = true;
            RingAppend( &seg->pieces, sdata );
            first->length = length;
        }
    } else {    // it must be COMBINE_ADD or COMBINE_STACK
        RingAppend( &seg->pieces, sdata );
    }
    Ring2Append( &wlink().CurrMod->segs, sdata );
}

static seg_leader *MakeNewLeader( segdata *sdata, class_entry *class_, seg_info_type info )
/****************************************************************************************/
{
    seg_leader *leader;

    sdata->u.leader = leader = InitLeader( sdata->u.name.u.ptr );
    leader->align = sdata->align;
    leader->combine = sdata->combine;
    leader->class_ = class_;
    leader->info = info;
    CheckForLast( leader, class_ );
    RingAppend( &class_->segs, leader );
    RingAppend( &leader->pieces, sdata );
    Ring2Append( &wlink().CurrMod->segs, sdata );
    return( leader );
}

static seg_leader *FindALeader( segdata *sdata, class_entry *class_, seg_info_type info )
/**************************************************************************************/
{
    seg_leader  *leader;

    leader = static_cast<seg_leader *>(RingLookup( class_->segs, CheckClassName, sdata ));
    if( leader == nullptr ) {
        leader = MakeNewLeader( sdata, class_, info );
    } else {
        AddToLeader( leader, sdata );
    }
    return( leader );
}

seg_leader *InitLeader( const char *segname )
/*******************************************/
{
    seg_leader  *seg;

    seg = static_cast<seg_leader *>(CarveAlloc( wlink().CarveLeader ));
    seg->next = nullptr;
    seg->next_by_group = nullptr;
    seg->pieces = nullptr;
    seg->class_ = nullptr;
    seg->ord = st().segment_ord++;
    seg->size = 0;
    seg->vsize = 0;
    seg->num = 0;
    SET_ADDR_UNDEFINED( seg->seg_addr );
    seg->group = nullptr;
    seg->info = SEGINF_NONE;
    seg->segname.u.ptr = AddStringStringTable( &wlink().PermStrings, segname );
    seg->dbgtype = NOT_DEBUGGING_INFO;
    seg->segflags = wlink().FmtData.def_seg_flags;
    return( seg );
}

void AddSegment( segdata *sd, class_entry *class_ )
/************************************************/
/* Add a segment to the segment list for an object file */
{
    seg_info_type   info;
    seg_leader      *leader;

    DEBUG((DBG_OLD,"- adding segment %s, class %s",sd->u.name.u.ptr, class_->name.u.ptr ));
    DEBUG(( DBG_OLD, "- - size = %h, comb = %x, alignment = %x",
                      sd->length, sd->combine, sd->align ));
    info = ( sd->bits == BITS_16 ) ? SEGINF_NONE : SEGINF_USE_32;
    if( class_->flags & CLASS_CODE ) {
        info |= SEGINF_CODE;
    }
    if( sd->isabs ) {
        info |= SEGINF_ABSOLUTE;
        sd->isdefd = true;
    }
    if( sd->isabs
      || sd->combine == COMBINE_INVALID ) {
        leader = MakeNewLeader( sd, class_, info );
    } else {
        const char  *seg_name = sd->u.name.u.ptr;

        leader = FindALeader( sd, class_, info );
        if( ( (leader->info & SEGINF_USE_32) != (info & SEGINF_USE_32) )
          && !( (wlink().FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD))
          && wlink().FmtData.u.os2fam.mixed1632 )
          && (wlink().FmtData.type & MK_RAW) == 0 ) {
            const char  *segname_16;
            const char  *segname_32;

            if( info & SEGINF_USE_32 ) {
                segname_16 = leader->segname.u.ptr;
                segname_32 = seg_name;
            } else {
                segname_16 = seg_name;
                segname_32 = leader->segname.u.ptr;
            }
            LnkMsg( ERR+MSG_CANT_COMBINE_32_AND_16, "12", segname_32, segname_16 );
        }
    }
    leader->dbgtype = DBIColSeg( class_ );
    if( !IS_DBG_INFO( leader ) ) {
        if( sd->bits == BITS_32 ) {
            Set32BitMode();
#ifdef _QNX
            CheckQNXSegMismatch( LS_HAVE_16BIT_CODE );
#endif
        } else {
            Set16BitMode();
#ifdef _QNX
            CheckQNXSegMismatch( LS_FMT_SEEN_32BIT );
#endif
        }
    }
    if( DBISkip( leader ) ) {
        sd->isdead = true;
    }
    if( sd->isabs ) {
        leader->seg_addr.off = 0;
        leader->seg_addr.seg = sd->frame;
    } else if( !sd->isdead ) {
        DBIAddLocal( leader, sd->length );
    }
}

void FreeLeader( void *seg )
/**************************/
{
    RingWalk( static_cast<seg_leader *>(seg)->pieces, FreeSegData );
    CarveFree( wlink().CarveLeader, seg );
}

static bool CmpLeaderPtr( void *a, void *b )
/******************************************/
{
    return( a == b );
}

void AddToGroup( group_entry *group, seg_leader *seg )
/****************************************************/
{
    if( Ring2Lookup( group->leaders, CmpLeaderPtr, seg ) )
        return;
    if( seg->group != nullptr
      && seg->group != group ) {
        LnkMsg( ERR+LOC+MSG_SEG_IN_TWO_GROUPS, "123", seg->segname.u.ptr,
                                   seg->group->sym->name.u.ptr, group->sym->name.u.ptr );
        return;
    }
    if( ( group->leaders != nullptr )
      && ( (group->leaders->info & SEGINF_USE_32) != (seg->info & SEGINF_USE_32) )
      && !( (wlink().FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD))
      && wlink().FmtData.u.os2fam.mixed1632 )
      && (wlink().FmtData.type & MK_RAW) == 0 ) {

        const char  *segname_16;
        const char  *segname_32;

        if( seg->info & SEGINF_USE_32 ) {
            segname_16 = group->leaders->segname.u.ptr;
            segname_32 = seg->segname.u.ptr;
        } else {
            segname_16 = seg->segname.u.ptr;
            segname_32 = group->leaders->segname.u.ptr;
        }
        LnkMsg( ERR+MSG_CANT_COMBINE_32_AND_16, "12", segname_32, segname_16 );
    }
    seg->group = group;
    Ring2Append( &group->leaders, seg );
}

void SetAddPubSym( symbol *sym, sym_info type, mod_entry *mod, offset off, unsigned_16 frame )
/********************************************************************************************/
{
    sym->mod = mod;
    SET_SYM_TYPE( sym, type );
    SET_SYM_ADDR( sym, off, frame );
    Ring2Append( &mod->publist, sym );
}

void DefineSymbol( symbol *sym, segnode *snode, offset off, unsigned_16 frame )
/*****************************************************************************/
// do the object file independent public symbol definition.
{
    size_t          name_len;
    bool            frame_ok;
    sym_info        sym_type;

    if( snode != nullptr ) {
        frame = 0;
    }
    name_len = strlen( sym->name.u.ptr );
    if( !IS_ADDR_UNDEFINED( sym->addr )
      && !IS_SYM_COMMUNAL( sym ) ) {
        if( snode != nullptr
          && sym->p.seg != nullptr ) {
            frame_ok = (sym->p.seg->u.leader == snode->entry->u.leader);
            if( sym->p.seg->u.leader->combine != COMBINE_COMMON ) {
                frame_ok = false;
            }
        } else if( sym->p.seg != nullptr ) {
            frame_ok = false;
        } else if( frame != sym->addr.seg ) {
            frame_ok = false;
        } else {
            frame_ok = true;
        }
        if( !(frame_ok
          && off == sym->addr.off) ) {
            ReportMultiple( sym, sym->name.u.ptr, name_len );
        }
    } else {
        sym_type = SYM_REGULAR;
        if( IS_SYM_IMPORTED( sym ) ) {
            sym = HashReplace( sym );
#ifdef _OS2
            if( wlink().FmtData.type & MK_PE ) {
                if( sym->p.import_dll != nullptr ) {
                    AddPEImportLocalSym( sym, sym->p.import_dll->iatsym );
                    sym_type |= SYM_REFERENCED;
                    LnkMsg( WRN+MSG_IMPORT_LOCAL, "s", sym->name.u.ptr );
                }
            }
#endif
        } else if( IS_SYM_COMMUNAL( sym ) ) {
            sym = HashReplace( sym );
            sym->p.seg = nullptr;
        }

        ClearSymUnion( sym );
        SetAddPubSym( sym, sym_type, wlink().CurrMod, off, frame );
        sym->info &= ~SYM_1_DISTRIB;
        if( snode != nullptr ) {
            if( wlink().LinkFlags & LF_STRIP_CODE ) {
                DefStripSym( sym, snode->entry );
            }
            if( snode->info & SEGINF_CODE ) {
#ifdef _EXE
                if( (wlink().FmtData.type & MK_OVERLAYS)
                  && wlink().FmtData.u.dos.distribute ) {
                    if( wlink().LinkState & LS_SEARCHING_LIBRARIES ) {
                        sym->info |= SYM_1_DISTRIB;
                    }
                }
#endif
            }
            sym->p.seg = snode->entry;
            if( sym->p.seg->isabs ) {
                sym->info |= SYM_ABSOLUTE;
            }
#ifdef _EXE
            if( wlink().FmtData.type & MK_OVERLAYS ) {
                OvlTryDefVector( sym );
            }
#endif
        } else {
            if( wlink().LinkFlags & LF_STRIP_CODE ) {
                CleanStripInfo( sym );
            }
            sym->info |= SYM_ABSOLUTE;
            sym->p.seg = nullptr;
        }
    }
}

static segdata *GetSegment( const char *seg_name, const char *class_name, const char *group_name,
                        unsigned align, unsigned comb, symbol *sym_communal )
/*****************************************************************************/
{
    section             *sect;
    class_entry         *class_;
    group_entry         *group;
    segdata             *sdata;
    seg_leader          *leader;
    byte                bits;

    sect = wlink().Root;
#ifdef _EXE
    if( wlink().FmtData.type & MK_OVERLAYS ) {
        sect = OvlCheckOvlSection( class_name );
    }
#endif
    bits = ( IS_SYM_COMM32( sym_communal ) ) ? BITS_32 : BITS_16;
    class_ = FindClass( sect, class_name, bits, false );
    sdata = AllocSegData();
    sdata->u.name.u.ptr = const_cast<char *>( seg_name );   // (the name a segment keeps: never written through)
    sdata->bits = bits;
    sdata->align = align;
    sdata->combine = comb;
    sdata->isuninit = true;
    leader = FindALeader( sdata, class_, ( bits == BITS_16 ) ? SEGINF_NONE : SEGINF_USE_32 );
    if( group_name != nullptr ) {
        /* put in appropriate group */
        group = GetGroup( group_name );
        AddToGroup( group, leader );
    }
    return( sdata );
}

static void NearAllocCommunal( symbol *sym, unsigned size )
/*********************************************************/
{
    sym->p.seg = GetSegment( CommunalSegName, BSSClassName, DataGrpName,
                                     2, COMBINE_ADD, sym );
    sym->p.seg->length = size;
}

static void FarAllocCommunal( symbol *sym, unsigned size )
/********************************************************/
{
    segdata     *seg;
    segdata     *first;

    first = nullptr;
    for( ;; ) {
        seg = GetSegment( sym->name.u.ptr, FarDataClassName, nullptr,
                          0, COMBINE_INVALID, sym );
        if( first == nullptr )
            first = seg;
        if( size < MAX_SEGMENT ) {
            seg->length = size;
            break;
        } else {
            seg->length = MAX_SEGMENT;
            size -= MAX_SEGMENT;
        }
    }
    sym->p.seg = first;
    sym->info |= SYM_DEFINED;
}

void AllocCommunal( symbol *sym, offset size )
/********************************************/
{
    if( wlink().LinkFlags & LF_STRIP_CODE ) {
        CleanStripInfo( sym );
    }
    if( sym->info & SYM_FAR_COMMUNAL ) {
        FarAllocCommunal( sym, size );
    } else {
        NearAllocCommunal( sym, size );
    }
}

symbol *MakeCommunalSym( symbol *sym, offset size, bool isfar, byte bits )
/************************************************************************/
{
    sym_info    symtype;
    symbol      *altsym;

    if( bits == BITS_16 ) {
        symtype = SYM_COMMUNAL_16;
    } else {
        symtype = SYM_COMMUNAL_32;
    }
    if( (sym->info & SYM_DEFINED) == 0
      || IS_SYM_IMPORTED( sym ) ) {
        if( IS_SYM_IMPORTED( sym ) ) {
            sym = HashReplace( sym );
        }
        ClearSymUnion( sym );
        SET_SYM_TYPE( sym, symtype );
        sym->info |= SYM_DEFINED;
        if( isfar ) {
            sym->info |= SYM_FAR_COMMUNAL;
        }
        AllocCommunal( sym, size );
        sym->addr.off = 0;
        sym->mod = wlink().CurrMod;
        Ring2Append( &wlink().CurrMod->publist, sym );
    } else {
        if( IS_SYM_NICOMDEF( sym ) ) {
            if( size < sym->p.seg->length )
                size = sym->p.seg->length;
            sym->p.seg->length = size;
        }
        altsym = AddAltDef( sym, symtype );
        if( wlink().LinkFlags & LF_INC_LINK_FLAG ) {
            altsym->p.cdefsize = size;
        }
    }
    return( sym );
}

void CheckComdatSym( symbol *sym, sym_info flags )
/************************************************/
// check a comdat redefinition to see if it is OK
// NYI: SYM_CDAT_SEL_SIZE, SYM_CDAT_SEL_EXACT, & SYM_CDAT_SEL_ASSOC not yet
// handled properly.  no prob. under coff, but OMF makes it very hard...
{
    sym_info    symflags;

    symflags = sym->info & SYM_CDAT_SEL_MASK;
    if( flags == SYM_CDAT_SEL_NODUP
      || symflags == SYM_CDAT_SEL_NODUP ) {
        symflags = SYM_CDAT_SEL_NODUP;
    } else {
        if( symflags < flags ) {
            symflags = flags;
        }
    }
    if( symflags == SYM_CDAT_SEL_NODUP ) {
        ReportMultiple( sym, sym->name.u.ptr, strlen( sym->name.u.ptr ) );
    }
}

void SetComdatSym( symbol *sym, segdata *sdata )
/**********************************************/
{
    if( wlink().LinkFlags & LF_STRIP_CODE ) {
        if( sdata->iscode ) {
            DefStripSym( sym, sdata );
        }
    }
    sym->info |= SYM_DEFINED;
    Ring2Append( &wlink().CurrMod->publist, sym );
    sym->addr.off = 0;
    sym->mod = wlink().CurrMod;
    SET_SYM_TYPE( sym, SYM_COMDAT );
    sym->p.seg = sdata;
}

void DefineComdat( segdata *sdata, symbol *sym, offset value,
                          sym_info select, unsigned_8 *data )
/***********************************************************/
{
    if( IS_SYM_REGULAR( sym )
      && (sym->info & SYM_DEFINED) ) {
        AddCDatAltDef( sdata, sym, data, select );
        sdata->isdead = true;
        return;
    }
    if( sym->mod != nullptr
      && sym->mod != wlink().CurrMod ) {
        CheckComdatSym( sym, select );
        AddCDatAltDef( sdata, sym, data, select );
        sdata->isdead = true;
    } else {
        if( (sym->info & SYM_DEFINED)
          && !IS_SYM_COMDAT( sym ) ) {
            sym = HashReplace( sym );
        }
        ClearSymUnion( sym );
        sym->info |= select;
        SetComdatSym( sym, sdata );
        sym->addr.off += value;
        sdata->u1.vm_ptr = AllocStg( sdata->length );

        if( nullptr == data ) {
            PutInfoNulls( sdata->u1.vm_ptr, sdata->length );
        } else {
            PutInfo( sdata->u1.vm_ptr, data, sdata->length );
        }
    }
}

void DefineLazyExtdef( symbol *sym, symbol *defsym, bool isweak )
/***************************************************************/
/* handle the lazy and weak extdef comments */
{
    symbol      *defaultsym;

    if( (sym->info & (SYM_DEFINED | SYM_EXPORTED)) == 0
      && !IS_SYM_IMPORTED( sym )
      && !IS_SYM_COMMUNAL( sym ) ) {
        if( IS_SYM_A_REF( sym )
          && !IS_SYM_LINK_WEAK( sym ) ) {
            if( IS_SYM_VF_REF( sym ) ) {
                defaultsym = *sym->e.vfdata;
            } else {
                defaultsym = sym->e.def;
            }
            if( defsym != defaultsym ) {
                LnkMsg( WRN+LOC_REC+MSG_LAZY_EXTDEF_MISMATCH, "S",sym );
            }
        } else if( (sym->info & SYM_OLDHAT) == 0
          || IS_SYM_LINK_WEAK( sym ) ) {
            if( isweak ) {
                SET_SYM_TYPE( sym, SYM_WEAK_REF );
            } else {
                SET_SYM_TYPE( sym, SYM_LAZY_REF );
            }
            sym->e.def = defsym;
            if( wlink().LinkFlags & LF_STRIP_CODE ) {
                DataRef( sym->e.def );  // default must not be removed
            }
        }
    }
}

static symbol **GetVFList( symbol *defsym, symbol *mainsym, bool generate,
                            vflistrtns *rtns )
/************************************************************************/
/* get the conditional symbols list from the vftable record, adding edges
 * to the dead code graph if necessary */
{
    symbol      **liststart;
    symbol      **symlist;
    symbol      *condsym;
    void        *bufstart;
    const char  *name;
    unsigned    count;

    liststart = nullptr;
    bufstart = rtns->getstart();
    count = 2;          // 1 for the default extdef, and 1 for nullptr.
    while( !rtns->isend() ) {
        name = rtns->getname();
        condsym = FindISymbol( name );
        if( condsym == nullptr ) {
            condsym = MakeWeakExtdef( name, defsym );
        } else if( (condsym->info & SYM_DEFINED)
          && !IS_SYM_COMMUNAL( condsym ) ) {
            generate = false;
            if( mainsym == nullptr ) {
                break;
            }
        }
        if( (wlink().LinkFlags & LF_STRIP_CODE)
          && mainsym != nullptr ) {
            AddSymEdge( condsym, mainsym );
        }
        count++;
    }
    if( generate ) {
        symlist = static_cast<symbol **>(MemAllocSafe( sizeof( symbol * ) * count ));
        liststart = symlist;
        *symlist = defsym;
        symlist++;
        rtns->setstart( bufstart );
        while( !rtns->isend() ) {
            name = rtns->getname();
            *symlist = FindISymbol( name );
            symlist++;
        }
        *symlist = nullptr;
    }
    return( liststart );
}

static void DefineVirtualFunction( symbol *sym, symbol *defsym, bool ispure,
                                   vflistrtns *rtns )
/***************************************************************************/
// change sym into a virtual function reference
{
    symbol      **symlist;

    symlist = GetVFList( defsym, sym, true, rtns );
    sym->info |= SYM_1_VF_REFS_DONE;
    if( symlist != nullptr ) {
        sym->e.vfdata = symlist;
        if( ispure ) {
            SET_SYM_TYPE( sym, SYM_PURE_REF );
        } else {
            SET_SYM_TYPE( sym, SYM_VF_REF );
        }
    } else {                    // might still need this if eliminated
        if( (wlink().LinkFlags & LF_STRIP_CODE)
          && (sym->info & SYM_EXPORTED) == 0 ) {
            sym->e.def = defsym;
        }
    }
    if( wlink().LinkFlags & LF_STRIP_CODE ) {
        DataRef( defsym );
    }
}

void DefineVFTableRecord( symbol *sym, symbol *defsym, bool ispure,
                                 vflistrtns *rtns )
/**************************************************************/
// process the watcom virtual function table information extension
{
    symbol      **symlist;
    symbol      **startlist;
    symbol      **oldlist;

    if( sym->info & SYM_DEFINED ) {
        /* if defined, still may have to keep track of conditional symbols
         * for dead code elimination */
        if( (wlink().LinkFlags & LF_STRIP_CODE)
          && (sym->info & (SYM_1_VF_REFS_DONE | SYM_EXPORTED)) == 0 ) {
            GetVFList( defsym, sym, false, rtns );
            sym->e.def = defsym;
            sym->info |= SYM_1_VF_REFS_DONE;
            DataRef( defsym );
        }
    } else if( !IS_SYM_IMPORTED( sym )
      && !IS_SYM_COMMUNAL( sym ) ) {
        if( IS_SYM_VF_REF( sym ) ) {
            if( IS_SYM_PURE_REF( sym ) ^ ispure ) {
                LnkMsg( WRN+LOC_REC+MSG_VF_PURE_MISMATCH, "S", sym );
            }
            symlist = GetVFList( defsym, nullptr, true, rtns );
            if( symlist == nullptr ) {
                if( !CheckVFList( sym ) ) {
                    LnkMsg( WRN+LOC_REC+MSG_VF_TABLE_MISMATCH, "S", sym );
                }
            } else {
                startlist = symlist;
                oldlist = sym->e.vfdata;
                for( ;; ) {
                    if( *oldlist != *symlist ) {
                        LnkMsg( WRN+LOC_REC+MSG_VF_TABLE_MISMATCH, "S", sym );
                        break;
                    }
                    if( *oldlist == nullptr )
                        break;
                    oldlist++;
                    symlist++;
                }
                MemFree( startlist );
            }
        } else if( IS_SYM_A_REF( sym )
          || (sym->info & SYM_OLDHAT) == 0 ) {
            DefineVirtualFunction( sym, defsym, ispure, rtns );
        }
    }
}

void DefineVFReference( void *snode, symbol *targ, bool issym )
/***********************************************************/
{
    if( issym ) {
        AddSymEdge( static_cast<symbol *>(snode), targ );
    } else {
        segnode *node = static_cast<segnode *>(snode);
        if( node->info & SEGINF_CODE ) {
            AddEdge( node->entry, targ );
        }
    }
}

void DefineReference( symbol *sym )
/*********************************/
// we have an object file reference for sym
{
#if defined( _EXE ) || defined( _NOVELL )
  #ifdef _EXE
    if( wlink().FmtData.type & MK_OVERLAYS ) {
        OvlTryRefVector( sym );
    }
  #endif
  #ifdef _NOVELL
    if( (wlink().FmtData.type & MK_NOVELL) ) {
        if( (wlink().LinkState & LS_SEARCHING_LIBRARIES)
          && IS_SYM_IMPORTED( sym )
          && (sym->info & SYM_CHECKED) ) {
            sym->info &= ~SYM_CHECKED;
            wlink().LinkState |= LS_LIBRARIES_ADDED;   // force another pass thru libs
        }
    }
  #endif
#else
    /* unused parameters */ static_cast<void>(sym);
#endif
}

group_entry *GetGroup( const char *name )
/***************************************/
/* Get group of specified name. */
{
    group_entry     *grp;

    grp = SearchGroups( name );
    if( grp == nullptr ) {
        grp = AllocGroup( name, &wlink().Groups );
    }
    return( grp );
}


group_entry *SearchGroups( const char *name )
/*******************************************/
/* Find group of specified name. */
{
    group_entry     *group;

    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( stricmp( group->sym->name.u.ptr, name ) == 0 ) {
            DEBUG(( DBG_OLD, "- group %s found at %x", name, group ));
            return( group );
        }
    }
    return( nullptr );
}

void SetCurrSeg( segdata *seg, offset obj_offset, unsigned_8 *data )
/******************************************************************/
/* register a segment for the purposes of storing relocations */
{
    wlink().CurrRec.seg = seg;
    wlink().CurrRec.obj_offset = obj_offset;
    wlink().CurrRec.data = data;
}

bool SeenDLLRecord( void )
/*******************************/
{
    wlink().LinkState |= LS_FMT_SEEN_IMPORT_CMT;
    if( !HintFormat( MK_NE | MK_OS2_FLAT | MK_PE | MK_ELF | MK_NOVELL ) ) {
        LnkMsg( WRN+LOC+MSG_DLL_WITH_386, nullptr );
        return( false );    /* Not OK to process import/export records. */
    } else {
        return( true );
    }
}

void HandleImport( const length_name *intname, const length_name *modname,
                            const length_name *extname, ordinal_t ordinal )
/*************************************************************************/
// handle the import coment record
{
    symbol      *sym;

#ifndef _OS2
    /* unused parameters */ (void)modname; (void)extname; (void)ordinal;
#endif

    sym = SymOp( ST_CREATE, intname->name, intname->len );
    if( (sym->info & SYM_DEFINED) == 0 ) {
        if( wlink().CurrMod != nullptr ) {
            Ring2Append( &wlink().CurrMod->publist, sym );
            sym->mod = wlink().CurrMod;
        }
#ifdef _OS2
        if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
            MSImportKeyword( sym, modname, extname, ordinal );
        } else {
#endif
            SET_SYM_TYPE( sym, SYM_IMPORTED );
            sym->info |= SYM_DEFINED | SYM_1_DCE_REF;
#ifdef _NOVELL
            if( wlink().FmtData.type & MK_NOVELL ) {
                SetNovImportSymbol( sym );
            }
#endif
#ifdef _ELF
            if( wlink().FmtData.type & MK_ELF ) {
                SetELFImportSymbol( sym );
            }
#endif
#ifdef _OS2
        }
#endif
    }
}

static void ExportSymbol( const length_name *expname )
/****************************************************/
{
    symbol      *sym;

    sym = SymOp( ST_CREATE_REFERENCE, expname->name, expname->len );
    sym->info |= SYM_EXPORTED;
#ifdef _NOVELL
    if( wlink().FmtData.type & MK_NOVELL ) {
        AddNameTable( expname->name, expname->len, true, &wlink().FmtData.u.nov.exp.export_ );
    }
#endif
#ifdef _ELF
    if( wlink().FmtData.type & MK_ELF ) {
        AddNameTable( expname->name, expname->len, true, &wlink().FmtData.u.elf.exp.export_ );
    }
#endif
}

void HandleExport( const length_name *expname, const length_name *intname,
                                        unsigned flags, ordinal_t ordinal )
/*************************************************************************/
{
#ifndef _OS2
    /* unused parameters */ (void)intname; (void)flags; (void)ordinal;
#endif

#ifdef _OS2
    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        MSExportKeyword( expname, intname, flags, ordinal );
    } else {
#endif
        ExportSymbol( expname );
#ifdef _OS2
    }
#endif
}

bool CheckVFList( symbol *sym )
/*****************************/
/* see if any of the conditional symbols for this symbol are defined */
{
    symbol      **symlist;

    if( sym->info & SYM_1_VF_MARKED ) {
        ConvertVFSym( sym );
        return( true );
    }
    for( symlist = sym->e.vfdata + 1; *symlist != nullptr; ++symlist ) {
        if( (*symlist)->info & (SYM_DEFINED | SYM_1_VF_MARKED) ) {
            ConvertVFSym( sym );
            return( true );
        }
    }
    return( false );
}
