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
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/link/specials.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/map/map.h"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/directives/cmdnov.h"
#include "build/x86link/directives/cmdelf.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/formats/pe.h"

#include "clibext.h"
#include "build/x86link/link/run_state.hpp"


#define MAX_SEGMENT     0x10000

namespace {

struct Pass1State {
    seg_leader      *LastCodeSeg = nullptr;     // last code segment in current module
    int             segment_ord = 0;
};

inline Pass1State &st() { return cb_wlink::RunState::part<Pass1State>(); }

}

void ResetObjPass1(Link& link)
/************************/
{
    link.ObjFileFormat = static_cast<file_format>(0);
    link.ObjFormat = static_cast<obj_format>(0);
}

void P1Start( void )
/******************/
{
    Pass1State &s = st();
    s.LastCodeSeg = nullptr;
    s.segment_ord = 0;
}

static void DoSavedImport(Link& link,  symbol *sym )
/**************************************/
{
    dll_sym_info        *dll;
    length_name         modname;
    length_name         extname;

    if( link.FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE) ) {
        dll = sym->p.import_dll;
        sym->p.import_dll = nullptr;
        sym->info &= ~SYM_DEFINED;
        modname.name = dll->m.modname.u.ptr;
        modname.len = strlen( modname.name );
        if( dll->isordinal ) {
            MSImportKeyword(link,  sym, &modname, nullptr, dll->u.ordinal );
        } else {
            extname.name = dll->u.entname.u.ptr;
            extname.len = strlen( extname.name );
            MSImportKeyword(link,  sym, &modname, &extname, IMP_BY_NAME );
        }
        FreeImport(link,  dll );
    }
}

static void DoSavedExport(Link& link,  symbol *sym )
/**************************************/
{
    /* unused parameters */ static_cast<void>(sym);

#ifdef _OS2
    if( link.FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        entry_export    *exp;

        exp = static_cast<entry_export *>(sym->e.export_);
        exp->sym = sym;
        exp->impname = nullptr;
        AddToExportList(link,  exp );
    }
#endif
#ifdef _NOVELL
    if( link.FmtData.type & MK_NOVELL ) {
        AddNameTable(link,  sym->name.u.ptr, strlen( sym->name.u.ptr ), true, &link.FmtData.u.nov.exp.export_ );
    }
#endif
#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        AddNameTable(link,  sym->name.u.ptr, strlen( sym->name.u.ptr ), true, &link.FmtData.u.elf.exp.export_ );
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

void InfoCDatAltDef(Link& link,  comdat_info *info )
/**************************************/
{
    symbol      *mainsym;

    mainsym = info->sym;
    info->sym = AddAltDef(link,  mainsym, SYM_COMDAT );
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

static void AddCDatAltDef(Link& link,  segdata *sdata, symbol *sym, unsigned_8 *data,
                           sym_info flags )
/***********************************************************************/
{
    comdat_info     info;
    comdat_piece    piece;

    if( link.LinkFlags & LF_INC_LINK_FLAG ) {
        info.sdata = sdata;
        info.sym = sym;
        info.flags = flags;
        info.pieces = nullptr;
        piece.data = data;
        piece.length = sdata->length;
        piece.free_data = false;
        RingAppend( &info.pieces, &piece );
        InfoCDatAltDef(link,  &info );
    }
}

static void DoIncSymbol(Link& link,  void *_sym )
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
        mainsym = SymOp(link,  symop, sym->name.u.ptr, strlen( sym->name.u.ptr ) );
        if( IS_SYM_NICOMDEF( sym ) ) {
            MakeCommunalSym(link,  mainsym, sym->p.cdefsize, (sym->info & SYM_FAR_COMMUNAL) != 0, IS_SYM_COMM32( sym ) ? BITS_32 : BITS_16 );
        } else if( IS_SYM_COMDAT( sym ) ) {
            if( sym->info & SYM_HAS_DATA ) {
                data = GetAltdefContents( sym->p.seg );
            } else {
                data = GetSegContents( sym->p.seg, sym->p.seg->u1.vm_offs );
            }
            sym->p.seg->isdead = false;
            DefineComdat(link,  sym->p.seg, mainsym, sym->addr.off, sym->info & SYM_CDAT_SEL_MASK, static_cast<unsigned_8 *>(data) );
        } else if( (mainsym->info & SYM_DEFINED) == 0 )  {
            DoSavedImport(link,  sym );       // FIXME can lose defs here.
        }
        CarveFree( link.CarveSymbol, sym );
    } else {
        if( !IS_SYM_COMDAT( sym ) ) {
            Ring2Append( &link.CurrMod->publist, sym );
        }
        if( sym->info & SYM_DEAD )
            return;
        if( IS_SYM_IMPORTED( sym ) ) {
            DoSavedImport(link,  sym );
        } else if( IS_SYM_COMDAT( sym ) ) {
            DefineComdat(link,  sym->p.seg, sym, sym->addr.off, sym->info & SYM_CDAT_SEL_MASK, static_cast<unsigned_8 *>(GetSegContents(sym->p.seg, sym->p.seg->u1.vm_offs)) );
        }
        if( sym->info & SYM_EXPORTED ) {
            DoSavedExport(link,  sym );
        }
    }
}

static class_entry *FindNamedClass(Link& link,  const char *name )
/****************************************************/
// NYI:  this doesn't take into account 16 & 32 bit classes with the same name.
{
    class_entry   *class_;

    for( class_ = link.Root->classes; class_ != nullptr; class_ = class_->next ) {
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

static bool DefIncGroup(Link& link,  void *_def, void *_grouptab )
/****************************************************/
{
    incgroupdef     *def = static_cast<incgroupdef *>(_def);
    group_entry     ***grouptab = static_cast<group_entry ***>(_grouptab);
    group_entry     *group;
    class_entry     *class_;
    seg_leader      *leader;

    group = GetGroup(link,  def->grpname );
    // the names are pairs: a class name, then a segment name
    std::span<const char *const> names( def->names, static_cast<size_t>( def->numsegs ) * 2 );
    for( size_t i = 0; i < names.size(); i += 2 ) {
        class_ = FindNamedClass(link,  names[i] );
        if( class_ != nullptr ) {
            leader = static_cast<seg_leader *>(RingLookup( class_->segs, CmpSegName, const_cast<char *>(names[i + 1]) ));
            if( leader != nullptr ) {
                AddToGroup(link,  group, leader );
            }
        }
    }
    **grouptab = group;
    (*grouptab)++;
    return( false );
}

void DoIncGroupDefs(Link& link)
/*************************/
{
    unsigned        numgroups;
    group_entry     **grouptab;

    numgroups = RingCount( link.IncGroupDefs );
    link.IncGroups = static_cast<group_entry **>(MemAllocSafe( sizeof( group_entry * ) * numgroups ));
    grouptab = link.IncGroups;
    RingLookup( link.IncGroupDefs, bind_link( link, DefIncGroup ), &grouptab );
    RingFree( &link.IncGroupDefs );
}

void Set64BitMode(Link& link)
/***********************/
// make sure that the executable format is a 64-bit format.
{
    link.LinkState |= LS_FMT_SEEN_64BIT;
    if( !HintFormat(link,  MK_ALLOW_64 ) ) {
        if( (link.ObjFormat & OBJ_FMT_TOLD_BITNESS) == 0 ) {
            link.ObjFormat |= OBJ_FMT_TOLD_BITNESS;
            LnkMsg( WRN+MSG_FOUND_XXBIT_OBJ, "sd",
                        link.CurrMod->u1.source->infile->name.u.ptr, 64 );
        }
    }
}

void Set32BitMode(Link& link)
/***********************/
// make sure that the executable format is a 32-bit format.
{
    link.LinkState |= LS_FMT_SEEN_32BIT;
    if( !HintFormat(link,  MK_ALLOW_32 ) ) {
        if( (link.ObjFormat & OBJ_FMT_TOLD_BITNESS) == 0 ) {
            link.ObjFormat |= OBJ_FMT_TOLD_BITNESS;
            LnkMsg( WRN+MSG_FOUND_XXBIT_OBJ, "sd",
                        link.CurrMod->u1.source->infile->name.u.ptr, 32 );
        }
    }
}

void Set16BitMode(Link& link)
/***********************/
{
    if( !HintFormat(link,  MK_ALLOW_16 ) ) {
        if( (link.ObjFormat & OBJ_FMT_TOLD_BITNESS) == 0 ) {
            link.ObjFormat |= OBJ_FMT_TOLD_BITNESS;
            LnkMsg( WRN+MSG_FOUND_XXBIT_OBJ, "sd",
                    link.CurrMod->u1.source->infile->name.u.ptr, 16 );
        }
    }
}

static void DoAllocateSegment(Link& link,  segdata *sdata, const char *clname )
/*****************************************************************/
{
    section         *sect;
    class_entry     *class_;
#ifdef _EXE
    bool            isovlclass;

    isovlclass = false;
    if( link.FmtData.type & MK_OVERLAYS ) {
        sdata->iscode = OvlCheckOvlClass(link,  clname, &isovlclass );
    }
#endif
    if( sdata->iscode ) {
        if( sdata->bits == BITS_16 ) {
            link.LinkState |= LS_HAVE_16BIT_CODE;
        }
    }
    sect = DBIGetSect(link,  clname );
    if( sect == nullptr ) {
        sect = link.CurrSect;
#ifdef _EXE
        if( (link.FmtData.type & MK_OVERLAYS)
          && !isovlclass ) {
            sect = link.NonSect;
        }
#endif
    }
    class_ = FindClass(link,  sect, clname, sdata->bits, sdata->iscode );
    AddSegment(link,  sdata, class_ );
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

void AllocateSegment(Link& link,  segnode *snode, const char *clname )
/*********************************************************
 * allocate a new segment (or new piece of a segment)
 */
{
    DoAllocateSegment(link,  snode->entry, clname );
    snode->info = snode->entry->u.leader->info;
}

unsigned long IncPass1( void )
/****************************/
{
    Link& link = wlink();
    segdata         *seglist;
    segdata         *seg;
    symbol          *publist;
    virt_mem_size   dataoff;
    size_t          relocs;

    seglist = link.CurrMod->segs;
    link.CurrMod->segs = nullptr;
    link.CurrMod->lines = nullptr;
    for( ;; ) {
        seg = static_cast<segdata *>(Ring2Pop( &seglist ));
        if( seg == nullptr )
            break;
        dataoff = seg->u1.vm_offs;
        DoAllocateSegment(link,  seg, seg->o.clname.u.ptr );
        seg->o.mod = link.CurrMod;
        if( !seg->isuninit
          && !seg->isdead
          && !seg->iscdat ) {
            PutInfo( seg->u1.vm_ptr, GetSegContents( seg, dataoff ), seg->length );
            seg->u.leader->info |= SEGINF_LXDATA_SEEN;
        }
    }
    publist = link.CurrMod->publist;
    link.CurrMod->publist = nullptr;
    Ring2Walk( publist, bind_link( link, DoIncSymbol ) );
    relocs = link.CurrMod->relocs;
    PermStartMod( link.CurrMod );    // destroys currmod->relocs
    IterateModRelocs( relocs, link.CurrMod->sizerelocs, bind_link( link, IncSaveRelocs ) );
    return( 0 );
}

#ifdef _QNX
static void CheckQNXSegMismatch(Link& link,  stateflag mask )
/***********************************************/
{
    if( (link.FmtData.type & MK_QNX)
      && (link.LinkState & mask)
      && !link.FmtData.u.qnx.seen_mismatch ) {
        LnkMsg( WRN+LOC+MSG_CANNOT_HAVE_16_AND_32, nullptr );
        link.FmtData.u.qnx.seen_mismatch = true;
    }
}
#endif

class_entry *DuplicateClass(Link& link,  class_entry *class_ )
/***********************************************/
{
    class_entry *newclass;

    newclass = static_cast<class_entry *>(CarveAlloc( link.CarveClass ));
    memcpy( newclass, class_, sizeof( class_entry ) );
    newclass->name.u.ptr = AddBufferStringTable( &link.PermStrings, class_->name.u.ptr, strlen( class_->name.u.ptr ) + 1 );
    class_->next = newclass;
    return( newclass );
}

class_entry *FindClass(Link& link,  section *sect, const char *name, byte bits, bool iscode )
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
    class_ = static_cast<class_entry *>(CarveAlloc( link.CarveClass ));
    class_->bits = bits;
    class_->name.u.ptr = AddBufferStringTable( &link.PermStrings, name, namelen + 1 );
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

static void CheckForLast(Link& link,  seg_leader *seg, class_entry *class_ )
/*************************************************************/
// check to see if this segment should be the last one in an autogroup.
{
    if( (link.CurrMod->flags_mod & MOD_LAST_SEG)
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

static void AddToLeader(Link& link,  seg_leader *seg, segdata *sdata )
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
    Ring2Append( &link.CurrMod->segs, sdata );
}

static seg_leader *MakeNewLeader(Link& link,  segdata *sdata, class_entry *class_, seg_info_type info )
/****************************************************************************************/
{
    seg_leader *leader;

    sdata->u.leader = leader = InitLeader(link,  sdata->u.name.u.ptr );
    leader->align = sdata->align;
    leader->combine = sdata->combine;
    leader->class_ = class_;
    leader->info = info;
    CheckForLast(link,  leader, class_ );
    RingAppend( &class_->segs, leader );
    RingAppend( &leader->pieces, sdata );
    Ring2Append( &link.CurrMod->segs, sdata );
    return( leader );
}

static seg_leader *FindALeader(Link& link,  segdata *sdata, class_entry *class_, seg_info_type info )
/**************************************************************************************/
{
    seg_leader  *leader;

    leader = static_cast<seg_leader *>(RingLookup( class_->segs, CheckClassName, sdata ));
    if( leader == nullptr ) {
        leader = MakeNewLeader(link,  sdata, class_, info );
    } else {
        AddToLeader(link,  leader, sdata );
    }
    return( leader );
}

seg_leader *InitLeader(Link& link,  const char *segname )
/*******************************************/
{
    seg_leader  *seg;

    seg = static_cast<seg_leader *>(CarveAlloc( link.CarveLeader ));
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
    seg->segname.u.ptr = AddStringStringTable( &link.PermStrings, segname );
    seg->dbgtype = NOT_DEBUGGING_INFO;
    seg->segflags = link.FmtData.def_seg_flags;
    return( seg );
}

void AddSegment(Link& link,  segdata *sd, class_entry *class_ )
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
        leader = MakeNewLeader(link,  sd, class_, info );
    } else {
        const char  *seg_name = sd->u.name.u.ptr;

        leader = FindALeader(link,  sd, class_, info );
        if( ( (leader->info & SEGINF_USE_32) != (info & SEGINF_USE_32) )
          && !( (link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD))
          && link.FmtData.u.os2fam.mixed1632 )
          && (link.FmtData.type & MK_RAW) == 0 ) {
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
    leader->dbgtype = DBIColSeg(link,  class_ );
    if( !IS_DBG_INFO( leader ) ) {
        if( sd->bits == BITS_32 ) {
            Set32BitMode(link);
#ifdef _QNX
            CheckQNXSegMismatch(link,  LS_HAVE_16BIT_CODE );
#endif
        } else {
            Set16BitMode(link);
#ifdef _QNX
            CheckQNXSegMismatch(link,  LS_FMT_SEEN_32BIT );
#endif
        }
    }
    if( DBISkip(link,  leader ) ) {
        sd->isdead = true;
    }
    if( sd->isabs ) {
        leader->seg_addr.off = 0;
        leader->seg_addr.seg = sd->frame;
    } else if( !sd->isdead ) {
        DBIAddLocal(link,  leader, sd->length );
    }
}

void FreeLeader(Link& link,  void *seg )
/**************************/
{
    RingWalk( static_cast<seg_leader *>(seg)->pieces, bind_link( link, FreeSegData ) );
    CarveFree( link.CarveLeader, seg );
}

static bool CmpLeaderPtr( void *a, void *b )
/******************************************/
{
    return( a == b );
}

void AddToGroup(Link& link,  group_entry *group, seg_leader *seg )
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
      && !( (link.FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD))
      && link.FmtData.u.os2fam.mixed1632 )
      && (link.FmtData.type & MK_RAW) == 0 ) {

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

void DefineSymbol(Link& link,  symbol *sym, segnode *snode, offset off, unsigned_16 frame )
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
            ReportMultiple(link,  sym, sym->name.u.ptr, name_len );
        }
    } else {
        sym_type = SYM_REGULAR;
        if( IS_SYM_IMPORTED( sym ) ) {
            sym = HashReplace(link,  sym );
#ifdef _OS2
            if( link.FmtData.type & MK_PE ) {
                if( sym->p.import_dll != nullptr ) {
                    AddPEImportLocalSym( sym, sym->p.import_dll->iatsym );
                    sym_type |= SYM_REFERENCED;
                    LnkMsg( WRN+MSG_IMPORT_LOCAL, "s", sym->name.u.ptr );
                }
            }
#endif
        } else if( IS_SYM_COMMUNAL( sym ) ) {
            sym = HashReplace(link,  sym );
            sym->p.seg = nullptr;
        }

        ClearSymUnion(link,  sym );
        SetAddPubSym( sym, sym_type, link.CurrMod, off, frame );
        sym->info &= ~SYM_1_DISTRIB;
        if( snode != nullptr ) {
            if( link.LinkFlags & LF_STRIP_CODE ) {
                DefStripSym( sym, snode->entry );
            }
            if( snode->info & SEGINF_CODE ) {
#ifdef _EXE
                if( (link.FmtData.type & MK_OVERLAYS)
                  && link.FmtData.u.dos.distribute ) {
                    if( link.LinkState & LS_SEARCHING_LIBRARIES ) {
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
            if( link.FmtData.type & MK_OVERLAYS ) {
                OvlTryDefVector(link,  sym );
            }
#endif
        } else {
            if( link.LinkFlags & LF_STRIP_CODE ) {
                CleanStripInfo( sym );
            }
            sym->info |= SYM_ABSOLUTE;
            sym->p.seg = nullptr;
        }
    }
}

static segdata *GetSegment(Link& link,  const char *seg_name, const char *class_name, const char *group_name,
                        unsigned align, unsigned comb, symbol *sym_communal )
/*****************************************************************************/
{
    section             *sect;
    class_entry         *class_;
    group_entry         *group;
    segdata             *sdata;
    seg_leader          *leader;
    byte                bits;

    sect = link.Root;
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        sect = OvlCheckOvlSection(link,  class_name );
    }
#endif
    bits = ( IS_SYM_COMM32( sym_communal ) ) ? BITS_32 : BITS_16;
    class_ = FindClass(link,  sect, class_name, bits, false );
    sdata = AllocSegData(link);
    sdata->u.name.u.ptr = const_cast<char *>( seg_name );   // (the name a segment keeps: never written through)
    sdata->bits = bits;
    sdata->align = align;
    sdata->combine = comb;
    sdata->isuninit = true;
    leader = FindALeader(link,  sdata, class_, ( bits == BITS_16 ) ? SEGINF_NONE : SEGINF_USE_32 );
    if( group_name != nullptr ) {
        /* put in appropriate group */
        group = GetGroup(link,  group_name );
        AddToGroup(link,  group, leader );
    }
    return( sdata );
}

static void NearAllocCommunal(Link& link,  symbol *sym, unsigned size )
/*********************************************************/
{
    sym->p.seg = GetSegment(link,  CommunalSegName, BSSClassName, DataGrpName,
                                     2, COMBINE_ADD, sym );
    sym->p.seg->length = size;
}

static void FarAllocCommunal(Link& link,  symbol *sym, unsigned size )
/********************************************************/
{
    segdata     *seg;
    segdata     *first;

    first = nullptr;
    for( ;; ) {
        seg = GetSegment(link,  sym->name.u.ptr, FarDataClassName, nullptr,
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

void AllocCommunal(Link& link,  symbol *sym, offset size )
/********************************************/
{
    if( link.LinkFlags & LF_STRIP_CODE ) {
        CleanStripInfo( sym );
    }
    if( sym->info & SYM_FAR_COMMUNAL ) {
        FarAllocCommunal(link,  sym, size );
    } else {
        NearAllocCommunal(link,  sym, size );
    }
}

symbol *MakeCommunalSym(Link& link,  symbol *sym, offset size, bool isfar, byte bits )
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
            sym = HashReplace(link,  sym );
        }
        ClearSymUnion(link,  sym );
        SET_SYM_TYPE( sym, symtype );
        sym->info |= SYM_DEFINED;
        if( isfar ) {
            sym->info |= SYM_FAR_COMMUNAL;
        }
        AllocCommunal(link,  sym, size );
        sym->addr.off = 0;
        sym->mod = link.CurrMod;
        Ring2Append( &link.CurrMod->publist, sym );
    } else {
        if( IS_SYM_NICOMDEF( sym ) ) {
            if( size < sym->p.seg->length )
                size = sym->p.seg->length;
            sym->p.seg->length = size;
        }
        altsym = AddAltDef(link,  sym, symtype );
        if( link.LinkFlags & LF_INC_LINK_FLAG ) {
            altsym->p.cdefsize = size;
        }
    }
    return( sym );
}

void CheckComdatSym(Link& link,  symbol *sym, sym_info flags )
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
        ReportMultiple(link,  sym, sym->name.u.ptr, strlen( sym->name.u.ptr ) );
    }
}

void SetComdatSym(Link& link,  symbol *sym, segdata *sdata )
/**********************************************/
{
    if( link.LinkFlags & LF_STRIP_CODE ) {
        if( sdata->iscode ) {
            DefStripSym( sym, sdata );
        }
    }
    sym->info |= SYM_DEFINED;
    Ring2Append( &link.CurrMod->publist, sym );
    sym->addr.off = 0;
    sym->mod = link.CurrMod;
    SET_SYM_TYPE( sym, SYM_COMDAT );
    sym->p.seg = sdata;
}

void DefineComdat(Link& link,  segdata *sdata, symbol *sym, offset value,
                          sym_info select, unsigned_8 *data )
/***********************************************************/
{
    if( IS_SYM_REGULAR( sym )
      && (sym->info & SYM_DEFINED) ) {
        AddCDatAltDef(link,  sdata, sym, data, select );
        sdata->isdead = true;
        return;
    }
    if( sym->mod != nullptr
      && sym->mod != link.CurrMod ) {
        CheckComdatSym(link,  sym, select );
        AddCDatAltDef(link,  sdata, sym, data, select );
        sdata->isdead = true;
    } else {
        if( (sym->info & SYM_DEFINED)
          && !IS_SYM_COMDAT( sym ) ) {
            sym = HashReplace(link,  sym );
        }
        ClearSymUnion(link,  sym );
        sym->info |= select;
        SetComdatSym(link,  sym, sdata );
        sym->addr.off += value;
        sdata->u1.vm_ptr = AllocStg( sdata->length );

        if( nullptr == data ) {
            PutInfoNulls( sdata->u1.vm_ptr, sdata->length );
        } else {
            PutInfo( sdata->u1.vm_ptr, data, sdata->length );
        }
    }
}

void DefineLazyExtdef(Link& link,  symbol *sym, symbol *defsym, bool isweak )
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
            if( link.LinkFlags & LF_STRIP_CODE ) {
                DataRef(link,  sym->e.def );  // default must not be removed
            }
        }
    }
}

static symbol **GetVFList(Link& link,  symbol *defsym, symbol *mainsym, bool generate,
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
        condsym = FindISymbol(link,  name );
        if( condsym == nullptr ) {
            condsym = MakeWeakExtdef(link,  name, defsym );
        } else if( (condsym->info & SYM_DEFINED)
          && !IS_SYM_COMMUNAL( condsym ) ) {
            generate = false;
            if( mainsym == nullptr ) {
                break;
            }
        }
        if( (link.LinkFlags & LF_STRIP_CODE)
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
            *symlist = FindISymbol(link,  name );
            symlist++;
        }
        *symlist = nullptr;
    }
    return( liststart );
}

static void DefineVirtualFunction(Link& link,  symbol *sym, symbol *defsym, bool ispure,
                                   vflistrtns *rtns )
/***************************************************************************/
// change sym into a virtual function reference
{
    symbol      **symlist;

    symlist = GetVFList(link,  defsym, sym, true, rtns );
    sym->info |= SYM_1_VF_REFS_DONE;
    if( symlist != nullptr ) {
        sym->e.vfdata = symlist;
        if( ispure ) {
            SET_SYM_TYPE( sym, SYM_PURE_REF );
        } else {
            SET_SYM_TYPE( sym, SYM_VF_REF );
        }
    } else {                    // might still need this if eliminated
        if( (link.LinkFlags & LF_STRIP_CODE)
          && (sym->info & SYM_EXPORTED) == 0 ) {
            sym->e.def = defsym;
        }
    }
    if( link.LinkFlags & LF_STRIP_CODE ) {
        DataRef(link,  defsym );
    }
}

void DefineVFTableRecord(Link& link,  symbol *sym, symbol *defsym, bool ispure,
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
        if( (link.LinkFlags & LF_STRIP_CODE)
          && (sym->info & (SYM_1_VF_REFS_DONE | SYM_EXPORTED)) == 0 ) {
            GetVFList(link,  defsym, sym, false, rtns );
            sym->e.def = defsym;
            sym->info |= SYM_1_VF_REFS_DONE;
            DataRef(link,  defsym );
        }
    } else if( !IS_SYM_IMPORTED( sym )
      && !IS_SYM_COMMUNAL( sym ) ) {
        if( IS_SYM_VF_REF( sym ) ) {
            if( IS_SYM_PURE_REF( sym ) ^ ispure ) {
                LnkMsg( WRN+LOC_REC+MSG_VF_PURE_MISMATCH, "S", sym );
            }
            symlist = GetVFList(link,  defsym, nullptr, true, rtns );
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
            DefineVirtualFunction(link,  sym, defsym, ispure, rtns );
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

void DefineReference(Link& link,  symbol *sym )
/*********************************/
// we have an object file reference for sym
{
#if defined( _EXE ) || defined( _NOVELL )
  #ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        OvlTryRefVector(link,  sym );
    }
  #endif
  #ifdef _NOVELL
    if( (link.FmtData.type & MK_NOVELL) ) {
        if( (link.LinkState & LS_SEARCHING_LIBRARIES)
          && IS_SYM_IMPORTED( sym )
          && (sym->info & SYM_CHECKED) ) {
            sym->info &= ~SYM_CHECKED;
            link.LinkState |= LS_LIBRARIES_ADDED;   // force another pass thru libs
        }
    }
  #endif
#else
    /* unused parameters */ static_cast<void>(sym);
#endif
}

group_entry *GetGroup(Link& link,  const char *name )
/***************************************/
/* Get group of specified name. */
{
    group_entry     *grp;

    grp = SearchGroups(link,  name );
    if( grp == nullptr ) {
        grp = AllocGroup(link,  name, &link.Groups );
    }
    return( grp );
}


group_entry *SearchGroups(Link& link,  const char *name )
/*******************************************/
/* Find group of specified name. */
{
    group_entry     *group;

    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( stricmp( group->sym->name.u.ptr, name ) == 0 ) {
            DEBUG(( DBG_OLD, "- group %s found at %x", name, group ));
            return( group );
        }
    }
    return( nullptr );
}

void SetCurrSeg(Link& link,  segdata *seg, offset obj_offset, unsigned_8 *data )
/******************************************************************/
/* register a segment for the purposes of storing relocations */
{
    link.CurrRec.seg = seg;
    link.CurrRec.obj_offset = obj_offset;
    link.CurrRec.data = data;
}

bool SeenDLLRecord(Link& link)
/*******************************/
{
    link.LinkState |= LS_FMT_SEEN_IMPORT_CMT;
    if( !HintFormat(link,  MK_NE | MK_OS2_FLAT | MK_PE | MK_ELF | MK_NOVELL ) ) {
        LnkMsg( WRN+LOC+MSG_DLL_WITH_386, nullptr );
        return( false );    /* Not OK to process import/export records. */
    } else {
        return( true );
    }
}

void HandleImport(Link& link,  const length_name *intname, const length_name *modname,
                            const length_name *extname, ordinal_t ordinal )
/*************************************************************************/
// handle the import coment record
{
    symbol      *sym;

#ifndef _OS2
    /* unused parameters */ (void)modname; (void)extname; (void)ordinal;
#endif

    sym = SymOp(link,  ST_CREATE, intname->name, intname->len );
    if( (sym->info & SYM_DEFINED) == 0 ) {
        if( link.CurrMod != nullptr ) {
            Ring2Append( &link.CurrMod->publist, sym );
            sym->mod = link.CurrMod;
        }
#ifdef _OS2
        if( link.FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
            MSImportKeyword(link,  sym, modname, extname, ordinal );
        } else {
#endif
            SET_SYM_TYPE( sym, SYM_IMPORTED );
            sym->info |= SYM_DEFINED | SYM_1_DCE_REF;
#ifdef _NOVELL
            if( link.FmtData.type & MK_NOVELL ) {
                SetNovImportSymbol(link,  sym );
            }
#endif
#ifdef _ELF
            if( link.FmtData.type & MK_ELF ) {
                SetELFImportSymbol( sym );
            }
#endif
#ifdef _OS2
        }
#endif
    }
}

static void ExportSymbol(Link& link,  const length_name *expname )
/****************************************************/
{
    symbol      *sym;

    sym = SymOp(link,  ST_CREATE_REFERENCE, expname->name, expname->len );
    sym->info |= SYM_EXPORTED;
#ifdef _NOVELL
    if( link.FmtData.type & MK_NOVELL ) {
        AddNameTable(link,  expname->name, expname->len, true, &link.FmtData.u.nov.exp.export_ );
    }
#endif
#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        AddNameTable(link,  expname->name, expname->len, true, &link.FmtData.u.elf.exp.export_ );
    }
#endif
}

void HandleExport(Link& link,  const length_name *expname, const length_name *intname,
                                        unsigned flags, ordinal_t ordinal )
/*************************************************************************/
{
#ifndef _OS2
    /* unused parameters */ (void)intname; (void)flags; (void)ordinal;
#endif

#ifdef _OS2
    if( link.FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        MSExportKeyword(link,  expname, intname, flags, ordinal );
    } else {
#endif
        ExportSymbol(link,  expname );
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
