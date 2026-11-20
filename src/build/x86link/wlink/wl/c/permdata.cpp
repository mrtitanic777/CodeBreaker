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
#include "linkstd.h"
#include "strtab.h"
#include "dbgcomm.h"
#include "dbgall.h"
#include "carve.h"
#include "reloc.h"
#include "fileio.h"
#include "virtmem.h"
#include "impexp.h"
#include "loadfile.h"
#include "objio.h"
#include "ring.h"
#include "objcalc.h"
#include "permdata.h"

#include "clibext.h"
#include "run_state.hpp"
#include <array>

stringtable             PermStrings;
stringtable             PrefixStrings;  /* these are NetWare prefix strings of which there could possibly be several */
carve_t                 CarveLeader;
carve_t                 CarveModEntry;
carve_t                 CarveSymbol;
carve_t                 CarveSegData;
carve_t                 CarveClass;
carve_t                 CarveGroup;
carve_t                 CarveDLLInfo;
carve_t                 CarveExportInfo;
char                    *IncFileName;
incgroupdef             *IncGroupDefs;
group_entry             **IncGroups;
libnamelist             *SavedUserLibs;
libnamelist             *SavedDefLibs;

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

    IncFileName = nullptr;
    st.IncStrTab = nullptr;
    st.ReadRelocs = nullptr;
    st.OldExe = nullptr;
    st.AltDefData = nullptr;
    st.OldSymFile = nullptr;
    IncGroupDefs = nullptr;
    IncGroups = nullptr;
    SavedUserLibs = nullptr;
    SavedDefLibs = nullptr;
    CarveClass = CarveCreate( sizeof( class_entry ), 20 * sizeof( class_entry ) );
    CarveGroup = CarveCreate( sizeof( group_entry ), 20 * sizeof( group_entry ) );
    CarveDLLInfo = CarveCreate( sizeof( dll_sym_info ), 100 * sizeof( dll_sym_info ) );
    CarveExportInfo = CarveCreate( sizeof( entry_export ), 20 * sizeof( entry_export ) );
    CarveLeader = CarveCreate( sizeof( seg_leader ), SEG_CARVE_SIZE );
    CarveModEntry = CarveCreate( sizeof( mod_entry ), MOD_CARVE_SIZE );
    CarveSegData = CarveCreate( sizeof( segdata ), SDATA_CARVE_SIZE );
    CarveSymbol = CarveCreate( sizeof( symbol ), SYM_CARVE_SIZE );
    InitStringTable( &PermStrings, true );
    InitStringTable( &PrefixStrings, true );
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
            memcpy( TokBuff + modpos, data, adjust );
        } else {
            ReadInfo( reinterpret_cast<virt_mem>( const_cast<char *>( data ) ), TokBuff + modpos, adjust );
        }
        QWrite( info->incfhdl, TokBuff, MAX_HEADROOM, IncFileName );
        data += adjust;
        len -= adjust;
        modpos = 0;
    }
    if( len > 0 ) {
        if( !isvmem ) {
            memcpy( TokBuff + modpos, data, len );
        } else {
            ReadInfo( reinterpret_cast<virt_mem>( const_cast<char *>( data ) ), TokBuff + modpos, len );
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
    for( group = Groups; group != nullptr; group = group->next ) {
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
        exp->next = static_cast<entry_export *>(CarveGetIndex( CarveExportInfo, exp->next ));
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
    mod->u.next = static_cast<MOD_ENTRY *>(CarveGetIndex( CarveModEntry, mod->u.next ));
    mod->name.u.offs = GetString( static_cast<perm_write_info *>(info), mod->name.u.ptr );
    mod->publist = static_cast<symbol *>(CarveGetIndex( CarveSymbol, mod->publist ));
    mod->segs = static_cast<SEGDATA *>(CarveGetIndex( CarveSegData, mod->segs ));
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
    sdata->next = static_cast<SEGDATA *>(CarveGetIndex( CarveSegData, sdata->next ));  // not used
    sdata->next_by_mod = static_cast<SEGDATA *>(CarveGetIndex( CarveSegData, sdata->next_by_mod ));
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
    sym->hash = static_cast<symbol *>(CarveGetIndex( CarveSymbol, sym->hash ));
    sym->next = static_cast<symbol *>(CarveGetIndex( CarveSymbol, sym->next ));
    sym->next_by_publist = static_cast<symbol *>(CarveGetIndex( CarveSymbol, sym->next_by_publist ));
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
    sym->mod = static_cast<mod_entry *>(CarveGetIndex( CarveModEntry, sym->mod ));
    if( IS_SYM_ALIAS( sym ) ) {
        if( sym->info & SYM_FREE_ALIAS ) {
            save = sym->p.alias.u.ptr;
            sym->p.alias.u.offs = GetString( static_cast<perm_write_info *>(info), sym->p.alias.u.ptr );
            MemFree( save );
        } else {
            sym->p.alias.u.offs = 0;
        }
    } else if( IS_SYM_IMPORTED( sym ) ) {
        if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE) ) {
            sym->p.import_dll = static_cast<dll_sym_info *>(CarveGetIndex( CarveDLLInfo, sym->p.import_dll ));
        }
    } else if( (sym->info & SYM_IS_ALTDEF) == 0
      || IS_SYM_COMDAT( sym ) ) {
        sym->p.seg = static_cast<segdata *>(CarveGetIndex( CarveSegData, sym->p.seg ));
        sym->u.altdefs = static_cast<symbol *>(CarveGetIndex( CarveSymbol, sym->u.altdefs ));
    }
    if( sym->info & SYM_EXPORTED ) {
        if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
            sym->e.export_ = CarveGetIndex( CarveExportInfo, sym->e.export_ );
        }
    } else if( sym->e.def != nullptr ) {
        sym->e.def = static_cast<symbol *>(CarveGetIndex( CarveSymbol, sym->e.def ));
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

    for( infile = CachedFiles; infile != nullptr; infile = infile->next ) {
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

    for( class_ = Root->classes; class_ != nullptr; class_ = class_->next ) {
        class_->name.u.offs = GetString( info, class_->name.u.ptr );
        RingLookup( class_->segs, PrepLeaders, info );
    }
}

void WritePermFile( perm_write_info *info, const void *data, size_t len )
/***********************************************************************/
{
    QWrite( info->incfhdl, data, len, IncFileName );
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
        memset( TokBuff + modpos, 0, adjust );
        info->currpos += adjust;
        modpos += adjust;
    }
    QWrite( info->incfhdl, TokBuff, modpos, IncFileName );
}

static void WriteStringBlock( void *info, const char *data, size_t size )
/***********************************************************************/
{
    QWrite( static_cast<perm_write_info *>( info )->incfhdl, data, size, IncFileName );
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
    QWrite( info->incfhdl, CarveBlockData( block ), size, IncFileName );
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
    if( StartInfo.mod != nullptr
      && !StartInfo.user_specd ) {
        hdr->startmodidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( CarveModEntry, StartInfo.mod ) ) );
        if( StartInfo.type == START_IS_SDATA ) {
            hdr->flags |= INC_FLAG_START_SEG;
            hdr->startidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( CarveSegData, StartInfo.targ.sdata ) ) );
        } else {
            DbgAssert( StartInfo.type == START_IS_SYM );
            hdr->startidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( CarveSymbol, StartInfo.targ.sym ) ) );
        }
        hdr->startoff = StartInfo.off;
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
    for( sym = HeadSym; sym != nullptr; sym = sym->next ) {
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
    for( file = ObjLibFiles; file != nullptr; file = file->next ) {
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

    if( (LinkFlags & LF_INC_LINK_FLAG) == 0
      || (LinkState & LS_LINK_ERROR) )
        return;
    InitStringTable( &info.strtab, false );
    AddCharStringTable( &info.strtab, '\0' );   // make 0 idx not valid
    info.incfhdl = QOpenRW( IncFileName );
    hdr.flags = 0;
    hdr.exename = GetString( &info, Root->outfile->fname );
    QModTime( Root->outfile->fname, &hdr.exemodtime );
    if( SymFileName != nullptr ) {
        hdr.symname = GetString( &info, SymFileName );
        QModTime( SymFileName, &hdr.symmodtime );
    } else {
        hdr.symname = 0;
    }
    info.currpos = 0;
    BufWritePermFile( &info, &hdr, sizeof( inc_file_header ) ); // reserve space
    PrepClasses( &info );
    hdr.numgroups = WriteGroupsList( &info );
    hdr.numuserlibs = WriteLibList( &info, true );
    hdr.numdeflibs = WriteLibList( &info, false );
    if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        PrepNameTable( FmtData.u.os2fam.mod_ref_list, &info );
        PrepNameTable( FmtData.u.os2fam.imp_tab_list, &info );
        hdr.numdllsyms = WriteSmallCarve( CarveDLLInfo, MarkDLLInfo, WriteDLLInfo, &info );
        hdr.numexports = WriteSmallCarve( CarveExportInfo, MarkExportInfo, WriteExportInfo, &info );
    }
    FlushPermBuf( &info );
    WriteHashPointers( &info );
    hdr.hdrsize = info.currpos;
    WriteAltData( &info );
    hdr.altdefsize = info.currpos;
    PrepFileList( &info );
    info.prepfn = PrepModEntry;
    DumpBlockStruct( CarveModEntry, MarkModEntry, &hdr.mods, &info );
    info.prepfn = PrepSegData;
    DumpBlockStruct( CarveSegData, MarkSegData, &hdr.segdatas, &info );
    info.prepfn = PrepSymbol;
    DumpBlockStruct( CarveSymbol, MarkSymbol, &hdr.symbols, &info );
    FiniStringBlock( &info.strtab, &strsize, &info, WriteStringBlock );
    hdr.strtabsize = strsize;
    if( st.SizeRelocs ) {
        QWrite( info.incfhdl, st.ReadRelocs, st.SizeRelocs, IncFileName );
    }
    memcpy( hdr.signature, INC_FILE_SIG, INC_FILE_SIG_SIZE );
    hdr.rootmodidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( CarveModEntry, Root->mods ) ) );
    hdr.headsymidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( CarveSymbol, HeadSym ) ) );
    hdr.libmodidx = static_cast<carve_index>( reinterpret_cast<pointer_uint>( CarveGetIndex( CarveModEntry, LibModules ) ) );
    hdr.linkstate = static_cast<unsigned_32>( LinkState & ~LS_CLEAR_ON_INC );
    hdr.relocsize = st.SizeRelocs;
    PrepStartValue( &hdr );
    QSeek( info.incfhdl, 0, IncFileName );
    QWrite( info.incfhdl, &hdr, sizeof( inc_file_header ), IncFileName );
    QClose( info.incfhdl, IncFileName );
}

void ReadPermFile( perm_read_info *info, void *data, size_t len )
/***************************************************************/
{
    QRead( info->incfhdl, data, len, IncFileName );
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
        RingAppend( &IncGroupDefs, def );
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
    exp->next = static_cast<entry_export *>(CarveMapIndex( CarveExportInfo, exp->next ));
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
        CarveInsertFree( CarveModEntry, mod );
        return;
    }
    mod->u.next = static_cast<MOD_ENTRY *>(CarveMapIndex( CarveModEntry, mod->u.next ));
    mod->name.u.ptr = MapString( mod->name.u.offs );
    mod->publist = static_cast<symbol *>(CarveMapIndex( CarveSymbol, mod->publist ));
    mod->segs = static_cast<SEGDATA *>(CarveMapIndex( CarveSegData, mod->segs ));
    mod->u1.fname.u.ptr = MapString( mod->u1.fname.u.offs );
}

static void RebuildSegData( void *_sdata, void *info )
/****************************************************/
{
    segdata *sdata = static_cast<segdata *>(_sdata);

    /* unused parameters */ (void)info;

    if( *reinterpret_cast<unsigned_32 *>(sdata) == CARVE_INVALID_INDEX ) {
        CarveInsertFree( CarveSegData, sdata );
        return;
    }
    sdata->next = static_cast<SEGDATA *>(CarveMapIndex( CarveSegData, sdata->next ));  // dont use this?
    sdata->next_by_mod = static_cast<SEGDATA *>(CarveMapIndex( CarveSegData, sdata->next_by_mod ));
    sdata->u.name.u.ptr = MapString( sdata->u.name.u.offs );
    sdata->o.clname.u.ptr = MapString( sdata->o.clname.u.offs );
}

static void RebuildSymbol( void *_sym, void *info )
/*************************************************/
{
    symbol *sym = static_cast<symbol *>(_sym);

    /* unused parameters */ (void)info;

    if( *reinterpret_cast<unsigned_32 *>(sym) == CARVE_INVALID_INDEX ) {
        CarveInsertFree( CarveSymbol, sym );
        return;
    }
    sym->hash = static_cast<symbol *>(CarveMapIndex( CarveSymbol, sym->hash ));
    sym->next = static_cast<symbol *>(CarveMapIndex( CarveSymbol, sym->next ));
    sym->next_by_publist = static_cast<symbol *>(CarveMapIndex( CarveSymbol, sym->next_by_publist ));
    sym->name.u.ptr = MapString( sym->name.u.offs );
    sym->info &= ~SYM_CLEAR_ON_INC;
    sym->mod = static_cast<mod_entry *>(CarveMapIndex( CarveModEntry, sym->mod ));
    if( IS_SYM_ALIAS( sym ) ) {
        sym->p.alias.u.ptr = MapString( sym->p.alias.u.offs );
    } else if( IS_SYM_IMPORTED( sym ) ) {
        if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE) ) {
            sym->p.import_dll = static_cast<dll_sym_info *>(CarveMapIndex( CarveDLLInfo, sym->p.import_dll ));
        }
    } else if( (sym->info & SYM_IS_ALTDEF) == 0
      || IS_SYM_COMDAT( sym ) ) {
        sym->p.seg = static_cast<segdata *>(CarveMapIndex( CarveSegData, sym->p.seg ));
        sym->u.altdefs = static_cast<symbol *>(CarveMapIndex( CarveSymbol, sym->u.altdefs ));
    }
    if( sym->info & SYM_EXPORTED ) {
        if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
            sym->e.export_ = CarveMapIndex( CarveExportInfo, sym->e.export_ );
        }
    } else if( sym->e.def != nullptr ) {
        sym->e.def = static_cast<symbol *>(CarveMapIndex( CarveSymbol, sym->e.def ));
    }
}

static void ReadBlockInfo( carve_t carver, void *blk, void *info )
/****************************************************************/
{
    QRead( static_cast<perm_read_info *>( info )->incfhdl, CarveBlockData( blk ), CarveBlockSize( carver ), IncFileName );
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
    CarvePurge( CarveGroup );
    CarvePurge( CarveModEntry );
    CarvePurge( CarveSegData );
    CarvePurge( CarveSymbol );
    if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        CarvePurge( CarveDLLInfo );
        CarvePurge( CarveExportInfo );
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
        StartInfo.from_inc = true;
        StartInfo.mod = static_cast<mod_entry *>( CarveMapIndex( CarveModEntry, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->startmodidx ) ) ) );
        if( hdr->flags & INC_FLAG_START_SEG ) {
            StartInfo.type = START_IS_SDATA;
            StartInfo.targ.sdata = static_cast<segdata *>( CarveMapIndex( CarveSegData, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->startidx ) ) ) );
        } else {
            StartInfo.type = START_IS_SYM;
            StartInfo.targ.sym = static_cast<symbol *>( CarveMapIndex( CarveSymbol, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->startidx ) ) ) );
        }
        StartInfo.off = hdr->startoff;
    }
}

void ReadPermData( void )
/******************************/
{
    PermState &st = perm_state();

    perm_read_info      info;
    inc_file_header     *hdr;

    info.incfhdl = QObjOpen( IncFileName );
    if( info.incfhdl == NIL_FHANDLE )
        return;
    info.buffer = static_cast<char *>( MemAllocSafe( SECTOR_SIZE ) );
    QRead( info.incfhdl, info.buffer, SECTOR_SIZE, IncFileName );
    hdr = reinterpret_cast<inc_file_header *>( info.buffer );
    if( memcmp( hdr->signature, INC_FILE_SIG, INC_FILE_SIG_SIZE ) != 0 ) {
        LnkMsg( WRN+MSG_INV_INC_FILE, nullptr );
        MemFree( info.buffer );
        QClose( info.incfhdl, IncFileName );
        return;
    }
    if( hdr->hdrsize > SECTOR_SIZE ) {
        info.buffer = static_cast<char *>( MemReallocSafe( info.buffer, hdr->hdrsize ) );
        hdr = reinterpret_cast<inc_file_header *>( info.buffer );   /* in case realloc moved it*/
        QRead( info.incfhdl, info.buffer + SECTOR_SIZE, hdr->hdrsize - SECTOR_SIZE, IncFileName );
    }
    info.currpos = sizeof( inc_file_header );
    CarveRestart( CarveModEntry, hdr->mods.num );
    CarveRestart( CarveSegData, hdr->segdatas.num );
    CarveRestart( CarveSymbol, hdr->symbols.num );
    if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        CarveRestart( CarveDLLInfo, hdr->numdllsyms );
        CarveRestart( CarveExportInfo, hdr->numexports );
    }
    ReadHashPointers( &info );
    if( hdr->altdefsize > 0 ) {
        st.AltDefData = MemAllocSafe( hdr->altdefsize );
        QRead( info.incfhdl, st.AltDefData, hdr->altdefsize, IncFileName );
    }
    CarveWalkBlocks( CarveModEntry, ReadBlockInfo, &info );
    CarveWalkBlocks( CarveSegData, ReadBlockInfo, &info );
    CarveWalkBlocks( CarveSymbol, ReadBlockInfo, &info );
    st.IncStrTab = static_cast<char *>( MemAllocSafe( hdr->strtabsize ) );
    QRead( info.incfhdl, st.IncStrTab, hdr->strtabsize, IncFileName );
    st.ReadRelocs = nullptr;
    if( hdr->relocsize ) {
        st.ReadRelocs = static_cast<char *>( MemAllocSafe( hdr->relocsize ) );
        QRead( info.incfhdl, st.ReadRelocs, hdr->relocsize, IncFileName );
    }
    QClose( info.incfhdl, IncFileName );
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
    ReadLibList( hdr->numuserlibs, &SavedUserLibs, &info );
    ReadLibList( hdr->numdeflibs, &SavedDefLibs, &info );
    if( FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        RebuildSmallCarve( CarveDLLInfo, hdr->numdllsyms, RebuildDLLInfo, &info );
        RebuildSmallCarve( CarveExportInfo, hdr->numexports, RebuildExportInfo, &info );
    }
    CarveWalkAll( CarveModEntry, RebuildModEntry, &info );
    CarveWalkAll( CarveSegData, RebuildSegData, &info );
    CarveWalkAll( CarveSymbol, RebuildSymbol, &info );
    Root->mods = static_cast<MOD_ENTRY *>( CarveMapIndex( CarveModEntry, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->rootmodidx ) ) ) );
    HeadSym = static_cast<symbol *>( CarveMapIndex( CarveSymbol, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->headsymidx ) ) ) );
    LibModules = static_cast<mod_entry *>( CarveMapIndex( CarveModEntry, reinterpret_cast<void *>( static_cast<pointer_uint>( hdr->libmodidx ) ) ) );
    LinkState = static_cast<stateflag>( hdr->linkstate ) | LS_GOT_PREV_STRUCTS | (LinkState & LS_CLEAR_ON_INC);
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

    if( (LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        MemFree( st.ReadRelocs );
        st.ReadRelocs = nullptr;
    }
}

void CleanPermData( void )
/*******************************/
{
    PermState &st = perm_state();

#ifdef DEVBUILD
    if( (LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        CarveVerifyAllGone( CarveLeader, "seg_leader" );
        CarveVerifyAllGone( CarveModEntry, "mod_entry" );
        CarveVerifyAllGone( CarveDLLInfo, "dll_sym_info" );
        CarveVerifyAllGone( CarveExportInfo, "entry_export" );
        CarveVerifyAllGone( CarveSymbol, "symbol" );
        CarveVerifyAllGone( CarveSegData, "segdata" );
        CarveVerifyAllGone( CarveClass, "class_entry" );
        CarveVerifyAllGone( CarveGroup, "group_entry" );
    }
#endif
    if( LinkState & LS_LINK_ERROR ) {
        QDelete( IncFileName );
    }
    CarveDestroy( CarveLeader );
    CarveDestroy( CarveModEntry );
    CarveDestroy( CarveDLLInfo );
    CarveDestroy( CarveExportInfo );
    CarveDestroy( CarveSymbol );
    CarveDestroy( CarveSegData );
    CarveDestroy( CarveClass );
    CarveDestroy( CarveGroup );
    FiniStringTable( &PrefixStrings);
    FiniStringTable( &PermStrings );
    FiniStringTable( &st.StoredRelocs );
    MemFree( IncFileName );
    MemFree( st.IncStrTab );
    MemFree( st.ReadRelocs );
    MemFree( st.OldExe );
    MemFree( st.OldSymFile );
    MemFree( st.AltDefData );
    RingFree( &IncGroupDefs );
    MemFree( IncGroups );
    FreeList( SavedUserLibs );
    FreeList( SavedDefLibs );
}
