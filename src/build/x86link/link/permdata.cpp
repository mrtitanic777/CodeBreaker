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
* Description:  Routines for making some linker data permanent.
*
****************************************************************************/


#include <string.h>
#include <stdio.h>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/base/strtab.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/link/permdata.h"

#include "clibext.h"
#include "build/x86link/link/run_state.hpp"
#include <array>


namespace {

// what the file kept in statics: the stored relocations and the data read from the incremental file
struct PermState {
    stringtable     StoredRelocs;
    char            *ReadRelocs;
    size_t          SizeRelocs;
    char            *OldExe;
    char            *OldSymFile;
    void            *AltDefData;
    char            *IncStrTab;
};

PermState &perm_state()
{
    return cb_wlink::RunState::part<PermState>();
}

}  // namespace

static constexpr size_t SEG_CARVE_SIZE   = _2K;
static constexpr size_t MOD_CARVE_SIZE   = _4K + _1K;
static constexpr size_t SDATA_CARVE_SIZE = _16K;
static constexpr size_t SYM_CARVE_SIZE   = _32K;

void ResetPermData( void )
/************************/
{
    PermState &st = perm_state();

    wlink().IncFileName = nullptr;
    st.IncStrTab = nullptr;
    st.ReadRelocs = nullptr;
    st.OldExe = nullptr;
    st.AltDefData = nullptr;
    st.OldSymFile = nullptr;
    wlink().IncGroupDefs = nullptr;
    wlink().IncGroups = nullptr;
    wlink().SavedUserLibs = nullptr;
    wlink().SavedDefLibs = nullptr;
    wlink().CarveClass = CarveCreate( sizeof( class_entry ), 20 * sizeof( class_entry ) );
    wlink().CarveGroup = CarveCreate( sizeof( group_entry ), 20 * sizeof( group_entry ) );
    wlink().CarveDLLInfo = CarveCreate( sizeof( dll_sym_info ), 100 * sizeof( dll_sym_info ) );
    wlink().CarveExportInfo = CarveCreate( sizeof( entry_export ), 20 * sizeof( entry_export ) );
    wlink().CarveLeader = CarveCreate( sizeof( seg_leader ), SEG_CARVE_SIZE );
    wlink().CarveModEntry = CarveCreate( sizeof( mod_entry ), MOD_CARVE_SIZE );
    wlink().CarveSegData = CarveCreate( sizeof( segdata ), SDATA_CARVE_SIZE );
    wlink().CarveSymbol = CarveCreate( sizeof( symbol ), SYM_CARVE_SIZE );
    InitStringTable( &wlink().PermStrings, true );
    InitStringTable( &wlink().PrefixStrings, true );
    InitStringTable( &st.StoredRelocs, false );
}


static void MarkDLLInfo( void *dll )
/**********************************/
{
    static_cast<dll_sym_info *>(dll)->isfree = true;
}

static void MarkExportInfo( void *exp )
/*************************************/
{
    static_cast<entry_export *>(exp)->isfree = true;
}

static void MarkModEntry( void *mod )
/***********************************/
{
    static_cast<mod_entry *>(mod)->flags_mod |= MOD_IS_FREE;
}

static void MarkSegData( void *sdata )
/************************************/
{
    static_cast<segdata *>(sdata)->isfree = true;
}

static void MarkSymbol( void *sym )
/*********************************/
{
    static_cast<symbol *>(sym)->info |= SYM_IS_FREE;
}

static size_t GetString( perm_write_info *info, const char *str )
/***************************************************************/
{
    return( AddStringStringTableOffs( &info->strtab, str ) );
}

static void DoWritePermFile( perm_write_info *info, const char *data, unsigned len, bool isvmem )
/***********************************************************************************************/
{
    unsigned modpos;
    unsigned adjust;

    modpos = info->currpos % MAX_HEADROOM;
    info->currpos += len;
    while( modpos + len >= MAX_HEADROOM ) {
        adjust = MAX_HEADROOM - modpos;
        if( !isvmem ) {
            memcpy( wlink().TokBuff + modpos, data, adjust );
        } else {
            ReadInfo( reinterpret_cast<virt_mem>( const_cast<char *>( data ) ), wlink().TokBuff + modpos, adjust );
        }
        QWrite( info->incfhdl, wlink().TokBuff, MAX_HEADROOM, wlink().IncFileName );
        data += adjust;
        len -= adjust;
        modpos = 0;
    }
    if( len > 0 ) {
        if( !isvmem ) {
            memcpy( wlink().TokBuff + modpos, data, len );
        } else {
            ReadInfo( reinterpret_cast<virt_mem>( const_cast<char *>( data ) ), wlink().TokBuff + modpos, len );
        }
    }
}

static void VMemWritePermFile( perm_write_info *info, virt_mem data, unsigned len )
/*********************************************************************************/
{
    DoWritePermFile( info, reinterpret_cast<const char *>( data ), len, true );
}

static void U32WritePermFile( perm_write_info *info, unsigned_32 data )
/*********************************************************************/
{
    DoWritePermFile( info, reinterpret_cast<const char *>( &data ), sizeof( data ), false );
}

static void BufWritePermFile( perm_write_info *info, void *data, unsigned len )
/*****************************************************************************/
{
    DoWritePermFile( info, static_cast<const char *>( data ), len, false );
}

static bool WriteLeaderName( void *_leader, void *info )
/******************************************************/
{
    seg_leader *leader = static_cast<seg_leader *>(_leader);

    U32WritePermFile( static_cast<perm_write_info *>(info), leader->class_->name.u.offs );
    U32WritePermFile( static_cast<perm_write_info *>(info), leader->segname.u.offs );
    return( false );
}

static unsigned WriteGroupsList( perm_write_info *info )
/******************************************************/
{
    group_entry *group;
    unsigned    num;

    num = 0;
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( !group->isautogrp
          && group->leaders != nullptr ) {
            num++;
            U32WritePermFile( info, static_cast<unsigned_32>( Ring2Count( group->leaders ) ) );
            group->sym->name.u.offs = GetString( info, group->sym->name.u.ptr );
            U32WritePermFile( info, group->sym->name.u.offs );
            Ring2Lookup( group->leaders, WriteLeaderName, info );
        }
    }
    return( num );
}

static bool CheckFree( bool isfree, perm_write_info *info )
/*********************************************************/
{
    if( isfree ) {
        U32WritePermFile( info, CARVE_INVALID_INDEX );
    }
    return( isfree );
}

static void WriteDLLInfo( void *_dll, void *info )
/************************************************/
{
    dll_sym_info *dll = static_cast<dll_sym_info *>(_dll);

    if( !CheckFree( dll->isfree, static_cast<perm_write_info *>(info) ) ) {
        dll->m.modname.u.ptr = dll->m.modnum->name.u.ptr;
        if( !dll->isordinal ) {
            dll->u.entname.u.ptr = dll->u.entry->name.u.ptr;
        }
        BufWritePermFile( static_cast<perm_write_info *>(info), dll, offsetof( dll_sym_info, iatsym ) );
    }
}

static void WriteExportInfo( void *_exp, void *info )
/***************************************************/
{
    entry_export *exp = static_cast<entry_export *>(_exp);

    if( !CheckFree( exp->isfree, static_cast<perm_write_info *>(info) ) ) {
        exp->next = static_cast<entry_export *>(CarveGetIndex( wlink().CarveExportInfo, exp->next ));
        if( exp->name.u.ptr != nullptr ) {
            exp->name.u.offs = GetString( static_cast<perm_write_info *>(info), exp->name.u.ptr );
        }
        BufWritePermFile( static_cast<perm_write_info *>(info), exp, offsetof( entry_export, sym ) );
    }
}

static void FixSymAddr( void *_sym )
/**********************************/
{
    symbol *sym = static_cast<symbol *>(_sym);

    if( !IS_SYM_IMPORTED( sym )
      && (sym->info & SYM_DEAD) == 0
      && sym->addr.off > 0
      && sym->p.seg != nullptr ) {
        sym->addr.off -= sym->p.seg->u.leader->seg_addr.off;
        sym->addr.off -= sym->p.seg->a.delta;
    }
}

static void PrepModEntry( void *_mod, void *info )
/************************************************/
{
    mod_entry *mod = static_cast<mod_entry *>(_mod);

    if( mod->flags_mod & MOD_IS_FREE ) {
        *reinterpret_cast<unsigned_32 *>(mod) = CARVE_INVALID_INDEX;
        return;
    }
    Ring2Walk( mod->publist, FixSymAddr );
    mod->u.next = static_cast<MOD_ENTRY *>(CarveGetIndex( wlink().CarveModEntry, mod->u.next ));
    mod->name.u.offs = GetString( static_cast<perm_write_info *>(info), mod->name.u.ptr );
    mod->publist = static_cast<symbol *>(CarveGetIndex( wlink().CarveSymbol, mod->publist ));
    mod->segs = static_cast<SEGDATA *>(CarveGetIndex( wlink().CarveSegData, mod->segs ));
    mod->flags_mod &= ~MOD_CLEAR_ON_INC;
    if( mod->u1.source != nullptr ) {
        mod->u1.fname = mod->u1.source->infile->name;
    }
}

static void PrepSegData( void *_sdata, void *info )
/*************************************************/
{
    segdata *sdata = static_cast<segdata *>(_sdata);

    /* unused parameters */ (void)info;

    if( sdata->isfree ) {
        *reinterpret_cast<unsigned_32 *>(sdata) = CARVE_INVALID_INDEX;
        return;
    }
    sdata->next = static_cast<SEGDATA *>(CarveGetIndex( wlink().CarveSegData, sdata->next ));  // not used
    sdata->next_by_mod = static_cast<SEGDATA *>(CarveGetIndex( wlink().CarveSegData, sdata->next_by_mod ));
    sdata->o.clname.u.ptr = sdata->u.leader->class_->name.u.ptr;
    sdata->u.name.u.ptr = sdata->u.leader->segname.u.ptr;
    sdata->vm_data = 0;
}

static void PrepSymbol( void *_sym, void *info )
/**********************************************/
{
    symbol      *sym = static_cast<symbol *>( _sym );
    char        *save;
    symbol      *mainsym;

    if( sym->info & SYM_IS_FREE ) {
        *reinterpret_cast<unsigned_32 *>(sym) = CARVE_INVALID_INDEX;
        return;
    }
    sym->hash = static_cast<symbol *>(CarveGetIndex( wlink().CarveSymbol, sym->hash ));
    sym->next = static_cast<symbol *>(CarveGetIndex( wlink().CarveSymbol, sym->next ));
    sym->next_by_publist = static_cast<symbol *>(CarveGetIndex( wlink().CarveSymbol, sym->next_by_publist ));
    if( sym->info & SYM_IS_ALTDEF ) {
        mainsym = sym->e.mainsym;
        if( (mainsym->info & SYM_NAME_XLATED) == 0 ) {
            mainsym->name.u.offs = GetString( static_cast<perm_write_info *>(info), mainsym->name.u.ptr );
            mainsym->info |= SYM_NAME_XLATED;
        }
        sym->name = mainsym->name;
    } else if( (sym->info & SYM_NAME_XLATED) == 0 ) {
        sym->name.u.offs = GetString( static_cast<perm_write_info *>(info), sym->name.u.ptr );
        sym->info |= SYM_NAME_XLATED;
    }
    sym->mod = static_cast<mod_entry *>(CarveGetIndex( wlink().CarveModEntry, sym->mod ));
    if( IS_SYM_ALIAS( sym ) ) {
        if( sym->info & SYM_FREE_ALIAS ) {
            save = sym->p.alias.u.ptr;
            sym->p.alias.u.offs = GetString( static_cast<perm_write_info *>(info), sym->p.alias.u.ptr );
            MemFree( save );
        } else {
            sym->p.alias.u.offs = 0;
        }
    } else if( IS_SYM_IMPORTED( sym ) ) {
        if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE) ) {
            sym->p.import_dll = static_cast<dll_sym_info *>(CarveGetIndex( wlink().CarveDLLInfo, sym->p.import_dll ));
        }
    } else if( (sym->info & SYM_IS_ALTDEF) == 0
      || IS_SYM_COMDAT( sym ) ) {
        sym->p.seg = static_cast<segdata *>(CarveGetIndex( wlink().CarveSegData, sym->p.seg ));
        sym->u.altdefs = static_cast<symbol *>(CarveGetIndex( wlink().CarveSymbol, sym->u.altdefs ));
    }
    if( sym->info & SYM_EXPORTED ) {
        if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
            sym->e.export_ = CarveGetIndex( wlink().CarveExportInfo, sym->e.export_ );
        }
    } else if( sym->e.def != nullptr ) {
        sym->e.def = static_cast<symbol *>(CarveGetIndex( wlink().CarveSymbol, sym->e.def ));
    }
}

static void PrepNameTable( obj_name_list *list, perm_write_info *info )
/*********************************************************************/
{
    for( ; list != nullptr; list = list->next ) {
        list->name.u.offs = AddStringStringTableOffs( &info->strtab, list->name.u.ptr );
    }
}

static void PrepFileList( perm_write_info *info )
/***********************************************/
{
    infilelist  *infile;
    std::array<char, PATH_MAX>  new_name;

    for( infile = wlink().CachedFiles; infile != nullptr; infile = infile->next ) {
        MakeFileName( infile, new_name.data() );
        infile->name.u.offs = GetString( info, new_name.data() );
    }
}

static bool PrepLeaders( void *_leader, void *info )
/**************************************************/
{
    seg_leader *leader = static_cast<seg_leader *>(_leader);

    leader->segname.u.offs = GetString( static_cast<perm_write_info *>(info), leader->segname.u.ptr );
    return( false );
}

static void PrepClasses( perm_write_info *info )
/**********************************************/
{
    class_entry *class_;

    for( class_ = wlink().Root->classes; class_ != nullptr; class_ = class_->next ) {
        class_->name.u.offs = GetString( info, class_->name.u.ptr );
        RingLookup( class_->segs, PrepLeaders, info );
    }
}

void WritePermFile( perm_write_info *info, const void *data, size_t len )
/***********************************************************************/
{
    QWrite( info->incfhdl, data, len, wlink().IncFileName );
}

static void FlushPermBuf( perm_write_info *info )
/***********************************************/
{
    size_t      modpos;
    size_t      adjust;

    modpos = info->currpos % MAX_HEADROOM;
    if( modpos == 0 )
        return;
    adjust = SECTOR_SIZE - ( info->currpos % SECTOR_SIZE );
    if( adjust != SECTOR_SIZE ) {
        memset( wlink().TokBuff + modpos, 0, adjust );
        info->currpos += adjust;
        modpos += adjust;
    }
    QWrite( info->incfhdl, wlink().TokBuff, modpos, wlink().IncFileName );
}

static void WriteStringBlock( void *info, const char *data, size_t size )
/***********************************************************************/
{
    QWrite( static_cast<perm_write_info *>( info )->incfhdl, data, size, wlink().IncFileName );
}

static void FiniStringBlock( stringtable *strtab, size_t *size, void *info,
                                            write_strtable_fn *writefn )
/************************************************************************/
{
    size_t      rawsize;

    rawsize = GetStringTableSize( strtab );
    *size = __ROUND_UP_SIZE_SECTOR( rawsize );
    if( *size != rawsize ) {
        ZeroStringTable( strtab, *size - rawsize );
    }
    WriteStringTable( strtab, writefn, info );
    FiniStringTable( strtab );
}

static void DumpBlock( carve_t carver, void *block, void *_info )
/***************************************************************/
{
    perm_write_info *info = static_cast<perm_write_info *>(_info);
    size_t          size;

    size = CarveBlockSize( carver );
    CarveBlockScan( carver, block, info->prepfn, info );
    QWrite( info->incfhdl, CarveBlockData( block ), size, wlink().IncFileName );
}

static void DumpBlockStruct( carve_t carver, void (*markfn)(void *),
                             carve_info *loc, perm_write_info *info )
/*******************************************************************/
{
    CarveWalkAllFree( carver, markfn );
    loc->num = CarveNumElements( carver );
    CarveWalkBlocks( carver, DumpBlock, info );
}

static unsigned WriteSmallCarve( carve_t carver, void (*markfn)(void *),
                                 void (*writefn)(void *, void *),
                                 perm_write_info *info )
/**************************************************************************/
{
    CarveWalkAllFree( carver, markfn );
    CarveWalkAll( carver, writefn, info );
    return( CarveNumElements( carver ) );
}

static void PrepStartValue( inc_file_header *hdr )
/************************************************/
{
    if( wlink().StartInfo.mod != nullptr
      && !wlink().StartInfo.user_specd ) {
        hdr->startmodidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( wlink().CarveModEntry, wlink().StartInfo.mod ) ) );
        if( wlink().StartInfo.type == START_IS_SDATA ) {
            hdr->flags |= INC_FLAG_START_SEG;
            hdr->startidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( wlink().CarveSegData, wlink().StartInfo.targ.sdata ) ) );
        } else {
            DbgAssert( wlink().StartInfo.type == START_IS_SYM );
            hdr->startidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( wlink().CarveSymbol, wlink().StartInfo.targ.sym ) ) );
        }
        hdr->startoff = wlink().StartInfo.off;
    } else {
        hdr->startmodidx = 0;
    }
}

static void WriteAltData( perm_write_info *info )
/***********************************************/
{
    symbol      *sym;
    size_t      savepos;

    info->currpos = 0;
    for( sym = wlink().HeadSym; sym != nullptr; sym = sym->next ) {
        if( (sym->info & SYM_IS_ALTDEF)
          && (sym->info & SYM_HAS_DATA) ) {
            savepos = info->currpos;
            VMemWritePermFile( info, sym->p.seg->u1.vm_ptr, sym->p.seg->length );
            sym->p.seg->u1.vm_offs = savepos;
        }
    }
    FlushPermBuf( info );
}

static unsigned_32 WriteLibList( perm_write_info *info, bool douser )
/*******************************************************************/
{
    unsigned_32 numlibs;
    file_list   *file;

    numlibs = 0;
    for( file = wlink().ObjLibFiles; file != nullptr; file = file->next ) {
        if( (((file->flags_file & STAT_USER_SPECD) != 0) ^ douser) == 0 ) {
            U32WritePermFile( info, GetString( info, file->infile->name.u.ptr ) );
            BufWritePermFile( info, &file->priority, sizeof( file->priority ) );
            numlibs++;
        }
    }
    return( numlibs );
}

void WritePermData( void )
/*******************************/
{
    PermState &st = perm_state();

    inc_file_header     hdr;
    perm_write_info     info;
    size_t              strsize;

    if( (wlink().LinkFlags & LF_INC_LINK_FLAG) == 0
      || (wlink().LinkState & LS_LINK_ERROR) )
        return;
    InitStringTable( &info.strtab, false );
    AddCharStringTable( &info.strtab, '\0' );   // make 0 idx not valid
    info.incfhdl = QOpenRW( wlink().IncFileName );
    hdr.flags = 0;
    hdr.exename = GetString( &info, wlink().Root->outfile->fname );
    QModTime( wlink().Root->outfile->fname, &hdr.exemodtime );
    if( wlink().SymFileName != nullptr ) {
        hdr.symname = GetString( &info, wlink().SymFileName );
        QModTime( wlink().SymFileName, &hdr.symmodtime );
    } else {
        hdr.symname = 0;
    }
    info.currpos = 0;
    BufWritePermFile( &info, &hdr, sizeof( inc_file_header ) ); // reserve space
    PrepClasses( &info );
    hdr.numgroups = WriteGroupsList( &info );
    hdr.numuserlibs = WriteLibList( &info, true );
    hdr.numdeflibs = WriteLibList( &info, false );
    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        PrepNameTable( wlink().FmtData.u.os2fam.mod_ref_list, &info );
        PrepNameTable( wlink().FmtData.u.os2fam.imp_tab_list, &info );
        hdr.numdllsyms = WriteSmallCarve( wlink().CarveDLLInfo, MarkDLLInfo, WriteDLLInfo, &info );
        hdr.numexports = WriteSmallCarve( wlink().CarveExportInfo, MarkExportInfo, WriteExportInfo, &info );
    }
    FlushPermBuf( &info );
    WriteHashPointers( &info );
    hdr.hdrsize = info.currpos;
    WriteAltData( &info );
    hdr.altdefsize = info.currpos;
    PrepFileList( &info );
    info.prepfn = PrepModEntry;
    DumpBlockStruct( wlink().CarveModEntry, MarkModEntry, &hdr.mods, &info );
    info.prepfn = PrepSegData;
    DumpBlockStruct( wlink().CarveSegData, MarkSegData, &hdr.segdatas, &info );
    info.prepfn = PrepSymbol;
    DumpBlockStruct( wlink().CarveSymbol, MarkSymbol, &hdr.symbols, &info );
    FiniStringBlock( &info.strtab, &strsize, &info, WriteStringBlock );
    hdr.strtabsize = strsize;
    if( st.SizeRelocs ) {
        QWrite( info.incfhdl, st.ReadRelocs, st.SizeRelocs, wlink().IncFileName );
    }
    memcpy( hdr.signature, INC_FILE_SIG, INC_FILE_SIG_SIZE );
    hdr.rootmodidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( wlink().CarveModEntry, wlink().Root->mods ) ) );
    hdr.headsymidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( wlink().CarveSymbol, wlink().HeadSym ) ) );
    hdr.libmodidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( wlink().CarveModEntry, wlink().LibModules ) ) );
    hdr.linkstate = static_cast<unsigned_32>( wlink().LinkState & ~LS_CLEAR_ON_INC );
    hdr.relocsize = st.SizeRelocs;
    PrepStartValue( &hdr );
    QSeek( info.incfhdl, 0, wlink().IncFileName );
    QWrite( info.incfhdl, &hdr, sizeof( inc_file_header ), wlink().IncFileName );
    QClose( info.incfhdl, wlink().IncFileName );
}

void ReadPermFile( perm_read_info *info, void *data, size_t len )
/***************************************************************/
{
    QRead( info->incfhdl, data, len, wlink().IncFileName );
}

static unsigned_32 BufPeekU32( perm_read_info *info )
/***************************************************/
{
    return( *reinterpret_cast<unsigned_32 *>( info->buffer + info->currpos ) );
}

static unsigned_32 BufReadU32( perm_read_info *info )
/***************************************************/
{
    unsigned_32 retval;

    retval = *reinterpret_cast<unsigned_32 *>( info->buffer + info->currpos );
    info->currpos += sizeof( unsigned_32 );
    return( retval );
}

static void BufRead( perm_read_info *info, void *data, size_t len )
/*****************************************************************/
{
    memcpy( data, info->buffer + info->currpos, len );
    info->currpos += len;
}

static char *MapString( size_t off )
/**********************************/
{
    PermState &st = perm_state();

    if( off == 0 )
        return( nullptr );
    return( st.IncStrTab + off );
}

static void ReadGroupsList( unsigned count, perm_read_info *info )
/****************************************************************/
{
    incgroupdef         *def;
    unsigned_32         size;
    const char          **p;

    while( count-- ) {
        size = BufReadU32( info );
        def = static_cast<incgroupdef *>( MemAllocSafe( sizeof( incgroupdef ) + ( size - 1 ) * 2 * sizeof( char * ) ) );
        RingAppend( &wlink().IncGroupDefs, def );
        def->numsegs = size;
        def->grpname = MapString( BufReadU32( info ) );
        p = def->names;
        while( size-- ) {
            *(p++) = MapString( BufReadU32( info ) );
            *(p++) = MapString( BufReadU32( info ) );
        }
    }
}

static void ReadLibList( unsigned count, libnamelist **head, perm_read_info *info )
/*********************************************************************************/
{
    libnamelist         *list;

    while( count-- > 0 ) {
        list = static_cast<libnamelist *>( MemAllocSafe( sizeof( libnamelist ) ) );
        list->name = MapString( BufReadU32( info ) );
        BufRead( info, &list->priority, sizeof( lib_priority ) );
        AppendList( reinterpret_cast<void **>( head ), list );
    }
}

static void RebuildDLLInfo( void *_dll, perm_read_info *info )
/************************************************************/
{
    dll_sym_info *dll = static_cast<dll_sym_info *>(_dll);

    BufRead( info, dll, offsetof( dll_sym_info, iatsym ) );
    dll->m.modname.u.ptr = MapString( dll->m.modname.u.offs );
    if( !dll->isordinal ) {
        dll->u.entname.u.ptr = MapString( dll->u.entname.u.offs );
    }
}

static void RebuildExportInfo( void *_exp, perm_read_info *info )
/***************************************************************/
{
    entry_export *exp = static_cast<entry_export *>(_exp);

    BufRead( info, exp, offsetof( entry_export, sym ) );
    exp->next = static_cast<entry_export *>(CarveMapIndex( wlink().CarveExportInfo, exp->next ));
    if( exp->name.u.offs != 0 ) {
        exp->name.u.ptr = MapString( exp->name.u.offs );
    }
    exp->impname = nullptr;
}

static void RebuildModEntry( void *_mod, void *info )
/***************************************************/
{
    mod_entry *mod = static_cast<mod_entry *>(_mod);

    /* unused parameters */ (void)info;

    if( *reinterpret_cast<unsigned_32 *>(mod) == CARVE_INVALID_INDEX ) {
        CarveInsertFree( wlink().CarveModEntry, mod );
        return;
    }
    mod->u.next = static_cast<MOD_ENTRY *>(CarveMapIndex( wlink().CarveModEntry, mod->u.next ));
    mod->name.u.ptr = MapString( mod->name.u.offs );
    mod->publist = static_cast<symbol *>(CarveMapIndex( wlink().CarveSymbol, mod->publist ));
    mod->segs = static_cast<SEGDATA *>(CarveMapIndex( wlink().CarveSegData, mod->segs ));
    mod->u1.fname.u.ptr = MapString( mod->u1.fname.u.offs );
}

static void RebuildSegData( void *_sdata, void *info )
/****************************************************/
{
    segdata *sdata = static_cast<segdata *>(_sdata);

    /* unused parameters */ (void)info;

    if( *reinterpret_cast<unsigned_32 *>(sdata) == CARVE_INVALID_INDEX ) {
        CarveInsertFree( wlink().CarveSegData, sdata );
        return;
    }
    sdata->next = static_cast<SEGDATA *>(CarveMapIndex( wlink().CarveSegData, sdata->next ));  // dont use this?
    sdata->next_by_mod = static_cast<SEGDATA *>(CarveMapIndex( wlink().CarveSegData, sdata->next_by_mod ));
    sdata->u.name.u.ptr = MapString( sdata->u.name.u.offs );
    sdata->o.clname.u.ptr = MapString( sdata->o.clname.u.offs );
}

static void RebuildSymbol( void *_sym, void *info )
/*************************************************/
{
    symbol *sym = static_cast<symbol *>(_sym);

    /* unused parameters */ (void)info;

    if( *reinterpret_cast<unsigned_32 *>(sym) == CARVE_INVALID_INDEX ) {
        CarveInsertFree( wlink().CarveSymbol, sym );
        return;
    }
    sym->hash = static_cast<symbol *>(CarveMapIndex( wlink().CarveSymbol, sym->hash ));
    sym->next = static_cast<symbol *>(CarveMapIndex( wlink().CarveSymbol, sym->next ));
    sym->next_by_publist = static_cast<symbol *>(CarveMapIndex( wlink().CarveSymbol, sym->next_by_publist ));
    sym->name.u.ptr = MapString( sym->name.u.offs );
    sym->info &= ~SYM_CLEAR_ON_INC;
    sym->mod = static_cast<mod_entry *>(CarveMapIndex( wlink().CarveModEntry, sym->mod ));
    if( IS_SYM_ALIAS( sym ) ) {
        sym->p.alias.u.ptr = MapString( sym->p.alias.u.offs );
    } else if( IS_SYM_IMPORTED( sym ) ) {
        if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE) ) {
            sym->p.import_dll = static_cast<dll_sym_info *>(CarveMapIndex( wlink().CarveDLLInfo, sym->p.import_dll ));
        }
    } else if( (sym->info & SYM_IS_ALTDEF) == 0
      || IS_SYM_COMDAT( sym ) ) {
        sym->p.seg = static_cast<segdata *>(CarveMapIndex( wlink().CarveSegData, sym->p.seg ));
        sym->u.altdefs = static_cast<symbol *>(CarveMapIndex( wlink().CarveSymbol, sym->u.altdefs ));
    }
    if( sym->info & SYM_EXPORTED ) {
        if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
            sym->e.export_ = CarveMapIndex( wlink().CarveExportInfo, sym->e.export_ );
        }
    } else if( sym->e.def != nullptr ) {
        sym->e.def = static_cast<symbol *>(CarveMapIndex( wlink().CarveSymbol, sym->e.def ));
    }
}

static void ReadBlockInfo( carve_t carver, void *blk, void *info )
/****************************************************************/
{
    QRead( static_cast<perm_read_info *>( info )->incfhdl, CarveBlockData( blk ), CarveBlockSize( carver ), wlink().IncFileName );
}

static void SmallFreeCheck( void *data, void *_info )
/***************************************************/
{
    perm_read_info *info = static_cast<perm_read_info *>(_info);
    unsigned_32 freeblk;

    if( info->num > 0 ) {
        info->num--;
        freeblk = BufPeekU32( info );
        if( freeblk == CARVE_INVALID_INDEX ) {
            info->currpos += sizeof( unsigned_32 );
            CarveInsertFree( info->carver, data );
        } else {
            info->cbfn( data, info );
        }
    }
}

static void RebuildSmallCarve( carve_t carver, unsigned num,
                               void (*fn)(void *,perm_read_info *),
                               perm_read_info * info )
/*****************************************************************/
{
    info->carver = carver;
    info->num = num;
    info->cbfn = fn;
    CarveWalkAll( carver, SmallFreeCheck, info );
}

static void PurgeRead( perm_read_info *info )
/*******************************************/
{
    PermState &st = perm_state();

    ClearHashPointers();
    CarvePurge( wlink().CarveGroup );
    CarvePurge( wlink().CarveModEntry );
    CarvePurge( wlink().CarveSegData );
    CarvePurge( wlink().CarveSymbol );
    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        CarvePurge( wlink().CarveDLLInfo );
        CarvePurge( wlink().CarveExportInfo );
    }
    MemFree( info->buffer );
    MemFree( st.IncStrTab );
    MemFree( st.ReadRelocs );
    MemFree( st.AltDefData );
    st.IncStrTab = nullptr;
    st.ReadRelocs = nullptr;
    st.AltDefData = nullptr;
    if( st.OldExe != nullptr ) {
        MemFree( st.OldExe );
        st.OldExe = nullptr;
    }
}

static void ReadBinary( char **buf, unsigned_32 nameidx, time_t modtime )
/***********************************************************************/
{
    char                *fname;
    f_handle            hdl;
    size_t              size;

    fname = MapString( nameidx );
    hdl = QObjOpen( fname );
    if( hdl == NIL_FHANDLE ) {
        return;
    }
    if( QFModTime(hdl) != modtime ) {
        LnkMsg( WRN+MSG_EXE_DATE_CHANGED, "s", fname );
        return;
    }
    size = QFileSize( hdl );
    *buf = static_cast<char *>( MemAllocSafe( size ) );
    QRead( hdl, *buf, size, fname );
    QClose( hdl, fname );
}

static void ReadStartInfo( inc_file_header *hdr )
/***********************************************/
{
    if( hdr->startmodidx != 0 ) {
        wlink().StartInfo.from_inc = true;
        wlink().StartInfo.mod = static_cast<mod_entry *>( CarveMapIndex( wlink().CarveModEntry, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->startmodidx ) ) ) );
        if( hdr->flags & INC_FLAG_START_SEG ) {
            wlink().StartInfo.type = START_IS_SDATA;
            wlink().StartInfo.targ.sdata = static_cast<segdata *>( CarveMapIndex( wlink().CarveSegData, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->startidx ) ) ) );
        } else {
            wlink().StartInfo.type = START_IS_SYM;
            wlink().StartInfo.targ.sym = static_cast<symbol *>( CarveMapIndex( wlink().CarveSymbol, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->startidx ) ) ) );
        }
        wlink().StartInfo.off = hdr->startoff;
    }
}

void ReadPermData( void )
/******************************/
{
    PermState &st = perm_state();

    perm_read_info      info;
    inc_file_header     *hdr;

    info.incfhdl = QObjOpen( wlink().IncFileName );
    if( info.incfhdl == NIL_FHANDLE )
        return;
    info.buffer = static_cast<char *>( MemAllocSafe( SECTOR_SIZE ) );
    QRead( info.incfhdl, info.buffer, SECTOR_SIZE, wlink().IncFileName );
    hdr = reinterpret_cast<inc_file_header *>( info.buffer );
    if( memcmp( hdr->signature, INC_FILE_SIG, INC_FILE_SIG_SIZE ) != 0 ) {
        LnkMsg( WRN+MSG_INV_INC_FILE, nullptr );
        MemFree( info.buffer );
        QClose( info.incfhdl, wlink().IncFileName );
        return;
    }
    if( hdr->hdrsize > SECTOR_SIZE ) {
        info.buffer = static_cast<char *>( MemReallocSafe( info.buffer, hdr->hdrsize ) );
        hdr = reinterpret_cast<inc_file_header *>( info.buffer );   /* in case realloc moved it*/
        QRead( info.incfhdl, info.buffer + SECTOR_SIZE, hdr->hdrsize - SECTOR_SIZE, wlink().IncFileName );
    }
    info.currpos = sizeof( inc_file_header );
    CarveRestart( wlink().CarveModEntry, hdr->mods.num );
    CarveRestart( wlink().CarveSegData, hdr->segdatas.num );
    CarveRestart( wlink().CarveSymbol, hdr->symbols.num );
    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        CarveRestart( wlink().CarveDLLInfo, hdr->numdllsyms );
        CarveRestart( wlink().CarveExportInfo, hdr->numexports );
    }
    ReadHashPointers( &info );
    if( hdr->altdefsize > 0 ) {
        st.AltDefData = MemAllocSafe( hdr->altdefsize );
        QRead( info.incfhdl, st.AltDefData, hdr->altdefsize, wlink().IncFileName );
    }
    CarveWalkBlocks( wlink().CarveModEntry, ReadBlockInfo, &info );
    CarveWalkBlocks( wlink().CarveSegData, ReadBlockInfo, &info );
    CarveWalkBlocks( wlink().CarveSymbol, ReadBlockInfo, &info );
    st.IncStrTab = static_cast<char *>( MemAllocSafe( hdr->strtabsize ) );
    QRead( info.incfhdl, st.IncStrTab, hdr->strtabsize, wlink().IncFileName );
    st.ReadRelocs = nullptr;
    if( hdr->relocsize ) {
        st.ReadRelocs = static_cast<char *>( MemAllocSafe( hdr->relocsize ) );
        QRead( info.incfhdl, st.ReadRelocs, hdr->relocsize, wlink().IncFileName );
    }
    QClose( info.incfhdl, wlink().IncFileName );
    ReadBinary( &st.OldExe, hdr->exename, hdr->exemodtime );
    if( st.OldExe == nullptr ) {
        PurgeRead( &info );
        return;
    }
    if( hdr->symname != 0 ) {
        ReadBinary( &st.OldSymFile, hdr->symname, hdr->symmodtime );
        if( st.OldSymFile == nullptr ) {
            PurgeRead( &info );
            return;
        }
    }
    ReadGroupsList( hdr->numgroups, &info );
    ReadLibList( hdr->numuserlibs, &wlink().SavedUserLibs, &info );
    ReadLibList( hdr->numdeflibs, &wlink().SavedDefLibs, &info );
    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        RebuildSmallCarve( wlink().CarveDLLInfo, hdr->numdllsyms, RebuildDLLInfo, &info );
        RebuildSmallCarve( wlink().CarveExportInfo, hdr->numexports, RebuildExportInfo, &info );
    }
    CarveWalkAll( wlink().CarveModEntry, RebuildModEntry, &info );
    CarveWalkAll( wlink().CarveSegData, RebuildSegData, &info );
    CarveWalkAll( wlink().CarveSymbol, RebuildSymbol, &info );
    wlink().Root->mods = static_cast<MOD_ENTRY *>( CarveMapIndex( wlink().CarveModEntry, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->rootmodidx ) ) ) );
    wlink().HeadSym = static_cast<symbol *>( CarveMapIndex( wlink().CarveSymbol, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->headsymidx ) ) ) );
    wlink().LibModules = static_cast<mod_entry *>( CarveMapIndex( wlink().CarveModEntry, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->libmodidx ) ) ) );
    wlink().LinkState = static_cast<stateflag>( hdr->linkstate ) | LS_GOT_PREV_STRUCTS | (wlink().LinkState & LS_CLEAR_ON_INC);
    ReadStartInfo( hdr );
    MemFree( info.buffer );
}

void PermSaveFixup( void *fix, size_t size )
/******************************************/
{
    PermState &st = perm_state();

    AddBufferStringTable( &st.StoredRelocs, fix, size );
}

void IterateModRelocs( size_t offset, size_t sizeleft, size_t (*fn)(void *) )
/***************************************************************************/
{
    PermState &st = perm_state();

    char        *fixoff;
    size_t      size;

    if( st.ReadRelocs != nullptr ) {
        fixoff = st.ReadRelocs + offset;
        while( sizeleft > 0 ) {
            size = fn( fixoff );
            fixoff += size;
            sizeleft -= size;
        }
    }
}

static void SaveRelocData( void *_curr, const char *data, size_t size )
/*********************************************************************/
{
    PermState &st = perm_state();

    char **curr = static_cast<char **>( _curr );

    if( st.SizeRelocs > 0 ) {
        if( st.ReadRelocs == nullptr ) {
            *curr = st.ReadRelocs = static_cast<char *>( MemAllocSafe( st.SizeRelocs ) );
        }
        memcpy( *curr, data, size );
        *curr += size;
    }
}

void IncP2Start( void )
/****************************/
{
    PermState &st = perm_state();

    char   *spare;

    spare = nullptr;
    MemFree( st.ReadRelocs );
    st.ReadRelocs = nullptr;
    MemFree( st.OldExe );
    st.OldExe = nullptr;
    MemFree( st.OldSymFile );
    st.OldSymFile = nullptr;
    MemFree( st.AltDefData );
    st.AltDefData = nullptr;
    FiniStringBlock( &st.StoredRelocs, &st.SizeRelocs, &spare, SaveRelocData );
}

void PermStartMod( mod_entry *mod )
/****************************************/
{
    PermState &st = perm_state();

    mod->relocs = GetStringTableSize( &st.StoredRelocs );
}

void PermEndMod( mod_entry *mod )
/**************************************/
{
    PermState &st = perm_state();

    mod->sizerelocs = GetStringTableSize( &st.StoredRelocs ) - mod->relocs;
}

void *GetSegContents( segdata *sdata, virt_mem_size off )
/*******************************************************/
{
    PermState &st = perm_state();

    if( st.OldSymFile != nullptr
      && IS_DBG_INFO( sdata->u.leader ) ) {
        return( st.OldSymFile + off );
    }
    return( st.OldExe + off );
}

void *GetAltdefContents( segdata *sdata )
/***********************************************/
{
    PermState &st = perm_state();

    return( static_cast<char *>( st.AltDefData ) + sdata->u1.vm_offs );
}

void FreeSavedRelocs( void )
/*********************************/
{
    PermState &st = perm_state();

    if( (wlink().LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        MemFree( st.ReadRelocs );
        st.ReadRelocs = nullptr;
    }
}

void CleanPermData( void )
/*******************************/
{
    PermState &st = perm_state();

#ifdef DEVBUILD
    if( (wlink().LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        CarveVerifyAllGone( wlink().CarveLeader, "seg_leader" );
        CarveVerifyAllGone( wlink().CarveModEntry, "mod_entry" );
        CarveVerifyAllGone( wlink().CarveDLLInfo, "dll_sym_info" );
        CarveVerifyAllGone( wlink().CarveExportInfo, "entry_export" );
        CarveVerifyAllGone( wlink().CarveSymbol, "symbol" );
        CarveVerifyAllGone( wlink().CarveSegData, "segdata" );
        CarveVerifyAllGone( wlink().CarveClass, "class_entry" );
        CarveVerifyAllGone( wlink().CarveGroup, "group_entry" );
    }
#endif
    if( wlink().LinkState & LS_LINK_ERROR ) {
        QDelete( wlink().IncFileName );
    }
    CarveDestroy( wlink().CarveLeader );
    CarveDestroy( wlink().CarveModEntry );
    CarveDestroy( wlink().CarveDLLInfo );
    CarveDestroy( wlink().CarveExportInfo );
    CarveDestroy( wlink().CarveSymbol );
    CarveDestroy( wlink().CarveSegData );
    CarveDestroy( wlink().CarveClass );
    CarveDestroy( wlink().CarveGroup );
    FiniStringTable( &wlink().PrefixStrings);
    FiniStringTable( &wlink().PermStrings );
    FiniStringTable( &st.StoredRelocs );
    MemFree( wlink().IncFileName );
    MemFree( st.IncStrTab );
    MemFree( st.ReadRelocs );
    MemFree( st.OldExe );
    MemFree( st.OldSymFile );
    MemFree( st.AltDefData );
    RingFree( &wlink().IncGroupDefs );
    MemFree( wlink().IncGroups );
    FreeList( wlink().SavedUserLibs );
    FreeList( wlink().SavedDefLibs );
}
