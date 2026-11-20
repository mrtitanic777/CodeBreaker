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
* Description:  Utilities for creation of PE (Win32) executable files.
*
****************************************************************************/


#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "linkstd.h"
#include "loados2.h"
#include "exepe.h"
#include "exedos.h"
#include "reloc.h"
#include "specials.h"
#include "pcobj.h"
#include "virtmem.h"
#include "objnode.h"
#include "loadfile.h"
#include "objcalc.h"
#include "fileio.h"
#include "dbgcomm.h"
#include "dbgall.h"
#include "dbgcv.h"
#include "objpass1.h"
#include "ring.h"
#include "strtab.h"
#include "carve.h"
#include "permdata.h"
#include "loadpe.h"
#include "reserr.h"
#include "impexp.h"
#include "toc.h"
#include "objstrip.h"
#include "wressetr.h"
#include "_rcrtns.h"
#include "pass2l1.h"
#include "exerespe.h"
#include "sharedio.h"
#include "rctypes.h"
#include "exeutil.h"
#include "newmem.h"
#include "dosstub.h"
#include "posixfp.h"
#include "implcoff.h"

#include "clibext.h"
#include "run_state.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>


#ifdef _OS2

namespace {

constexpr unsigned MINIMUM_SEG_SHIFT = 2;       /* Corresponds to 2^2 == 4 bytes */
constexpr unsigned DEFAULT_SEG_SHIFT = 9;       /* Corresponds to 2^9 == 512 bytes */

constexpr unsigned STUB_ALIGN = 8U;             /* for PE format */

constexpr std::size_t PE32_OPT_SIZE = sizeof( pe32_opt_header );
constexpr std::size_t PE64_OPT_SIZE = sizeof( pe64_opt_header );
constexpr std::size_t PE32_SIZE = PE_HDR_SIZE + PE32_OPT_SIZE;
constexpr std::size_t PE64_SIZE = PE_HDR_SIZE + PE64_OPT_SIZE;

/* RDOS OS default major/minor version numbers */
constexpr unsigned PE_RDOS_OS_MAJOR = 8;
constexpr unsigned PE_RDOS_OS_MINOR = 8;
/* RDOS Subsystem default major/minor version numbers */
constexpr unsigned PE_RDOS_SS_MAJOR = 1;
constexpr unsigned PE_RDOS_SS_MINOR = 0;

constexpr char TRANSFER_SEGNAME[] = "TRANSFER CODE";

struct local_import {
    local_import        *next;
    symbol              *iatsym;
    symbol              *locsym;
};

struct IDataInfo {
    offset      ilt_off;
    offset      eof_ilt_off;
    offset      iat_off;
    offset      mod_name_off;
    offset      hint_off;
    offset      total_size;
    segdata     *sdata;
};

/* what this file keeps for one run of the linker */
struct PEState {
    module_import       *PEImpList = nullptr;
    unsigned            NumMods = 0;
    segdata             *XFerSegData = nullptr;
    local_import        *PELocalImpList = nullptr;
    unsigned            NumLocalImports = 0;
    IDataInfo           IData{};
};

PEState &state()
{
    return cb_wlink::RunState::part<PEState>();
}

}  // namespace

#define WALK_IMPORT_SYMBOLS(sym) \
    for( (sym) = HeadSym; (sym) != nullptr; (sym) = (sym)->next ) \
        if( IS_SYM_IMPORTED(sym) && (sym)->p.import_dll != nullptr \
            /*&& ((sym)->info & SYM_DEAD) == 0 */)

static offset CalcIDataSize( void )
/*********************************/
{
    PEState         &st = state();
    module_import   *mod;
    import_name     *imp;
    unsigned        iatsize;
    unsigned        size;

    iatsize = ( NumImports + st.NumMods ) * sizeof( pe_va );
    if( 0 == iatsize ) {
        return( 0 );
    }
    st.IData.ilt_off = ( st.NumMods + 1 ) * sizeof( pe_import_directory );
    st.IData.eof_ilt_off = st.IData.ilt_off + iatsize;
    st.IData.iat_off = st.IData.eof_ilt_off + TocSize;
    st.IData.mod_name_off = st.IData.iat_off + iatsize;
    size = 0;
    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        size += mod->mod->len + 1;
    }
    st.IData.hint_off = st.IData.mod_name_off + size;
    size = st.IData.hint_off;
    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        for( imp = mod->imports; imp != nullptr; imp = imp->next ) {
            if( imp->imp != nullptr ) {
                size = __ROUND_UP_SIZE_EVEN( size );
                size += imp->imp->len + sizeof( unsigned_16 ) + sizeof( unsigned_8 );
            }
        }
    }
    st.IData.total_size = size;
    return( st.IData.total_size );
}

void ResetLoadPE( void )
/**********************/
{
    PEState         &st = state();
    st.PEImpList = nullptr;
    st.XFerSegData = nullptr;
    st.NumMods = 0;
    NumImports = 0;
    memset( &st.IData, 0, sizeof( st.IData ) );
}

static offset CalcIATAbsOffset( void )
/************************************/
{
    PEState         &st = state();
    return( IDataGroup->linear + FmtData.base + st.IData.iat_off );
}

static void CalcImpOff( dll_sym_info *dll, offset *off )
/******************************************************/
{
    if( dll != nullptr) { // if not end of mod marker
        SET_SYM_ADDR( dll->iatsym, *off, 0 );
    }
    *off += sizeof( pe_va );
}

static void XFerReloc( offset off, group_entry *group, unsigned type, unsigned data )
/***********************************************************************************/
{
    reloc_item      reloc;
    size_t          size;

    size = sizeof( pe_reloc_item );
    reloc.pe = (( off + group->linear ) & OSF_PAGE_MASK) | type;
    if( type == PE_FIX_HIGHADJ ) {
        size = sizeof( high_pe_reloc_item );
        reloc.hpe.low_off = data;
    }
    WriteReloc( group, off, &reloc, size );
}

static int GetTransferGlueSize( void )
/************************************/
{
    switch( LinkState & LS_HAVE_MACHTYPE_MASK ) {
    case LS_HAVE_ALPHA_CODE:    return( sizeof( CoffImportAxpText ) );
    case LS_HAVE_MIPS_CODE:     return( sizeof( CoffImportMipsText ) ); // TODO
    case LS_HAVE_PPC_CODE:      return( sizeof( CoffImportPpcText ) );
    case LS_HAVE_X64_CODE:      return( sizeof( CoffImportX64Text ) ); // TODO
    case LS_HAVE_X86_CODE:      return( sizeof( CoffImportX86Text ) );
    default:                    DbgAssert( 0 ); return( 0 );
    }
}

static void *GetTransferGlueCode( void )
/**************************************/
{
    switch( LinkState & LS_HAVE_MACHTYPE_MASK ) {
    case LS_HAVE_ALPHA_CODE:    return( CoffImportAxpText );
    case LS_HAVE_MIPS_CODE:     return( CoffImportMipsText ); // TODO
    case LS_HAVE_PPC_CODE:      return( CoffImportPpcText );
    case LS_HAVE_X64_CODE:      return( CoffImportX64Text ); // TODO
    case LS_HAVE_X86_CODE:      return( CoffImportX86Text );
    default:                    DbgAssert( 0 ); return( nullptr );
    }
}

offset FindIATSymAbsOff( symbol *sym )
/************************************/
{
    dll_sym_info    *dll;

    dll = sym->p.import_dll;
    DbgAssert( IS_SYM_IMPORTED( sym ) && dll != nullptr );
    return( dll->iatsym->addr.off );
}

offset FindSymPosInTocv( symbol *sym )
/************************************/
{
    PEState         &st = state();
    return( FindIATSymAbsOff( sym ) - IDataGroup->linear - FmtData.base - TocShift - st.IData.eof_ilt_off );
}

static void GenPETransferTable( void )
/************************************/
{
    PEState         &st = state();
    offset          off;
    offset          base;
    symbol          *sym;
    void*           data;
    size_t          datalen;
    group_entry     *group;
    local_import    *loc_imp;
    pe_va           addr;

    if( st.XFerSegData == nullptr )
        return;
    group = st.XFerSegData->u.leader->group;
    base = st.XFerSegData->u.leader->seg_addr.off + st.XFerSegData->a.delta;
    if( IDataGroup != nullptr ) {
        datalen = GetTransferGlueSize();
        data = GetTransferGlueCode();
        WALK_IMPORT_SYMBOLS( sym ) {
            switch( LinkState & LS_HAVE_MACHTYPE_MASK ) {
            case LS_HAVE_ALPHA_CODE:
              {
                offset dest = FindIATSymAbsOff( sym );
                CoffImportAxpText[0] |= ( dest >> 16 );     // hioff
                CoffImportAxpText[1] |= ( dest & 0xFFFF );  // looff
                if( LinkState & LS_MAKE_RELOCS ) {
                    if( (FmtData.objalign & 0xFFFF) == 0 ) {
                        XFerReloc( sym->addr.off + 0, group, PE_FIX_HIGH, 0 );
                    } else {
                        XFerReloc( sym->addr.off + 4, group, PE_FIX_LOW, 0 );
                        XFerReloc( sym->addr.off + 0, group, PE_FIX_HIGHADJ, CoffImportAxpText[0] );
                    }
                }
              }
                break;
            case LS_HAVE_MIPS_CODE:
              {
                offset dest = FindIATSymAbsOff( sym );
                CoffImportMipsText[0] |= ( dest >> 16 );     // hioff
                CoffImportMipsText[1] |= ( dest & 0xFFFF );  // looff
                if( LinkState & LS_MAKE_RELOCS ) {
                    if( (FmtData.objalign & 0xFFFF) == 0 ) {
                        XFerReloc( sym->addr.off + 0, group, PE_FIX_HIGH, 0 );
                    } else {
                        XFerReloc( sym->addr.off + 4, group, PE_FIX_LOW, 0 );
                        XFerReloc( sym->addr.off + 0, group, PE_FIX_HIGHADJ, CoffImportMipsText[0] );
                    }
                }
              }
                break;
            case LS_HAVE_PPC_CODE:
                CoffImportPpcText[0] = (CoffImportPpcText[0] & 0xffff0000) | (FindSymPosInTocv( sym ) & 0x0000ffff);
                break;
            case LS_HAVE_X64_CODE:
                // TODO
              {
                offset dest = FindIATSymAbsOff( sym );
                reinterpret_cast<unsigned_64 *>( CoffImportX64Text + 2 )->u._32[0] = dest;
                reinterpret_cast<unsigned_64 *>( CoffImportX64Text + 2 )->u._32[1] = 0;
                if( LinkState & LS_MAKE_RELOCS ) {
                    XFerReloc( sym->addr.off + 2, group, PE_FIX_HIGHLOW, 0 );
                }
              }
                break;
            case LS_HAVE_X86_CODE:
              {
                offset dest = FindIATSymAbsOff( sym );

                MPUT_32_UN( CoffImportX86Text + 2, dest );
                if( LinkState & LS_MAKE_RELOCS ) {
                    XFerReloc( sym->addr.off + 2, group, PE_FIX_HIGHLOW, 0 );
                }
              }
                break;
            default:
                break;
            }
            off = sym->addr.off - base;
            PutInfo( st.XFerSegData->u1.vm_ptr + off, data, datalen );
        }
    }
    /* dump the local addresses table */
    for( loc_imp = st.PELocalImpList; loc_imp != nullptr; loc_imp = loc_imp->next ) {
        off = loc_imp->iatsym->addr.off - base;
        addr = FindLinearAddr( &loc_imp->locsym->addr ) + FmtData.base;
        PutInfo( st.XFerSegData->u1.vm_ptr + off, &addr, sizeof( addr ) );
    }
    if( LinkState & LS_MAKE_RELOCS ) {
        for( loc_imp = st.PELocalImpList; loc_imp != nullptr; loc_imp = loc_imp->next ) {
            XFerReloc( loc_imp->iatsym->addr.off, group, PE_FIX_HIGHLOW, 0 );
        }
    }
    group->size = group->totalsize;
}

unsigned DefStackSizePE( void )
/*****************************/
{
    if( FmtData.dll ) {
        return( PE_DLL_DEF_STACK_SIZE );
    }
    return( PE_EXE_DEF_STACK_SIZE );
}

static unsigned WriteDataPages( pe_exe_header *pehdr, pe_object *object, unsigned file_align )
/*********************************************************************************************
 * write the enumerated data pages
 */
{
    group_entry     *group;
    char            *name;
    unsigned        linear;
    seg_leader      *leader;
    unsigned        size_v;
    unsigned        size_ph;
    unsigned        code_base;
    unsigned        data_base;
    unsigned        entry_rva;
    unsigned        init_data_size;
    unsigned        code_size;

    linear = 0;
    entry_rva = 0;
    init_data_size = 0;
    code_size = 0;
    code_base = 0xFFFFFFFFUL;
    data_base = 0xFFFFFFFFUL;
    for( group = Groups; group != nullptr; group = group->next) {
        if( group->totalsize == 0 )
            continue;   // DANGER DANGER DANGER <--!!!
        name = group->sym->name.u.ptr;
        if( name == nullptr
          || name[0] == 0 ) {
            leader = static_cast<seg_leader *>( Ring2First( group->leaders ) );
            name = leader->segname.u.ptr;
            if( name == nullptr ) {
                name = const_cast<char *>( "" );
            }
        }
        strncpy( object->name, name, PE_OBJ_NAME_LEN );
        size_ph = CalcGroupSize( group );
        if( group == DataGroup
          && FmtData.dgroupsplitseg != nullptr ) {
            size_v = group->totalsize;
            if( StackSegPtr != nullptr ) {
                size_v -= StackSize;
            }
        } else {
            size_v = size_ph;
        }
        linear = __ROUND_UP_SIZE( size_v, FmtData.objalign );
        linear += group->linear;
        if( StartInfo.addr.seg == group->addr.seg ) {
            entry_rva = group->linear + StartInfo.addr.off;
        }
        object->rva = group->linear;
        /*
        //  Why weren't we filling in this field? MS do!
        */
        object->virtual_size = size_v;
        object->physical_size = __ROUND_UP_SIZE( size_ph, file_align );

        object->flags = 0;
        /* segflags are in OS/2 V1.x format, we have to translate them
            into the appropriate PE bits */
        if( group->segflags & SEG_DATA ) {
            object->flags |= PE_OBJ_INIT_DATA | PE_OBJ_READABLE;
            if( (group->segflags & SEG_READ_ONLY) == 0 ) {
                object->flags |= PE_OBJ_WRITABLE;
            }
            init_data_size += object->physical_size;
            if( object->rva < data_base ) {
                data_base = object->rva;
            }
        } else {
            object->flags |= PE_OBJ_CODE | PE_OBJ_EXECUTABLE;
            if( (group->segflags & SEG_READ_ONLY) == 0 ) {
                object->flags |= PE_OBJ_READABLE;
            }
            if( group->segflags & SEG_NOPAGE) {
                object->flags |= PE_OBJ_NOT_PAGABLE;
            }
            code_size += object->physical_size;
            if( object->rva < code_base ) {
                code_base = object->rva;
            }
        }
        if( group->segflags & SEG_PURE ) {
            object->flags |= PE_OBJ_SHARED;
        }
        if( size_ph != 0 ) {
            object->physical_offset = NullAlign( file_align );
            WriteGroupLoad( group, false );
            PadLoad( size_ph - group->size );
        }
        ++object;
    }
    if( code_base == 0xFFFFFFFFUL ) {
        code_base = 0;
    }
    if( data_base == 0xFFFFFFFFUL ) {
        data_base = 0;
    }
    PE( *pehdr, code_base ) = code_base;
    PE( *pehdr, entry_rva ) = entry_rva;
    PE( *pehdr, init_data_size ) = init_data_size;
    PE( *pehdr, code_size ) = code_size;
    if( (LinkState & LS_HAVE_X64_CODE) == 0 ) {
        PE32( *pehdr ).data_base = data_base;
    }
    return( linear );
}

static void WalkImportsMods( void (*action)(dll_sym_info *, offset *),
                                                        offset *cookie )
/**********************************************************************/
{
    PEState         &st = state();
    module_import   *mod;
    import_name     *imp;

    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        for( imp = mod->imports; imp != nullptr; imp = imp->next ) {
            action( imp->dll, cookie );
        }
        action( nullptr, cookie );
    }
}

static void WriteIAT( virt_mem buf, offset linear )
/*************************************************/
{
    PEState         &st = state();
    module_import   *mod;
    import_name     *imp;
    pe_va           iat;
    offset          pos;
    offset          hint_rva ;

    pos = 0;
    hint_rva = st.IData.hint_off + linear;
    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        for( imp = mod->imports; imp != nullptr; imp = imp->next ) {
            if( imp->imp != nullptr ) {
                hint_rva = __ROUND_UP_SIZE_EVEN( hint_rva );
                iat = PE_IMPORT_BY_NAME | hint_rva;
                hint_rva += imp->imp->len + ( sizeof( unsigned_16 ) + sizeof( unsigned_8 ) );
            } else {
                iat = PE_IMPORT_BY_ORDINAL | imp->dll->u.ordinal;
            }
            PutInfo( buf + pos, &iat, sizeof( iat ) );
            pos += sizeof( iat );
        }
        /* NULL entry marks end of list */
        PutInfoNulls( buf + pos, sizeof( iat ) );
        pos += sizeof( iat );
    }
}

static void WriteImportInfo( void )
/*********************************/
{
    PEState         &st = state();
    pe_import_directory dir;
    unsigned_16         hint;
    virt_mem            buf;
    offset              pos;
    group_entry         *group;
    module_import       *mod;
    import_name         *imp;
    unsigned            size;
    unsigned            mod_name_rva;
    offset              linear;

    if( IDataGroup == nullptr ) {
        return;
    }
    group = IDataGroup;
    linear = group->linear;
    group->size = st.IData.total_size;
    buf = st.IData.sdata->u1.vm_ptr;
    pos = 0;
    dir.time_stamp = 0;
    dir.major = 0;
    dir.minor = 0;
    /* dump the directory table */
    size = 0;
    mod_name_rva = st.IData.mod_name_off + linear;
    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        dir.name_rva = mod_name_rva;
        dir.import_lookup_table_rva = st.IData.ilt_off + size + linear;
        dir.import_address_table_rva = st.IData.iat_off + size + linear;
        size += ( mod->num_entries + 1 ) * sizeof( pe_va );
        mod_name_rva += mod->mod->len + 1;
        PutInfo( buf+pos, &dir, sizeof( dir ) );
        pos += sizeof( dir );
    }
    PutInfoNulls( buf + pos, sizeof( dir ) );    /* NULL entry marks end of table */
    pos += sizeof( dir );
    WriteIAT( buf + st.IData.ilt_off, linear ); // Import Lookup table
    WriteToc( buf + st.IData.eof_ilt_off );
    for( pos = st.IData.eof_ilt_off; pos < st.IData.iat_off; pos += sizeof( pe_va ) ) {
        XFerReloc( pos, group, PE_FIX_HIGHLOW, 0 );
    }
    WriteIAT( buf + st.IData.iat_off, linear ); // Import Address table
    pos = st.IData.mod_name_off;            /* write the module names */
    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        int name_size = mod->mod->len + 1;
        PutInfo( buf + pos, mod->mod->name.u.ptr, name_size );
        pos += name_size;
    }
    pos = st.IData.hint_off;        /* write out the import names */
    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        hint = 1;
        for( imp = mod->imports; imp != nullptr; imp = imp->next ) {
            if( imp->imp != nullptr ) {
                if( pos & 1 ) {         /* round up */
                    PutInfoNulls( buf + pos, 1 );
                    pos++;
                }
                PutInfo( buf + pos, &hint, sizeof( hint ) );
                pos += sizeof( hint );
                size = imp->imp->len;
                PutInfo( buf + pos, imp->imp->name.u.ptr, size );
                pos += size;
                PutInfoNulls( buf + pos, 1 );
                pos++;
                hint++;
            }
        }
    }
}

static int namecmp_exp( const void *pn1, const void *pn2 )
/********************************************************/
{
    return( strcmp( (*static_cast<entry_export * const *>( pn1 ))->name.u.ptr, (*static_cast<entry_export * const *>( pn2 ))->name.u.ptr ) );
}


static unsigned WriteExportInfo( pe_object *object, unsigned file_align, pe_dir_entry *entry )
/********************************************************************************************/
{
    unsigned            size;
    pe_export_directory dir;
    const char          *name;
    size_t              namelen;
    std::vector<entry_export *> sort;
    entry_export        *exp;
    size_t              i;
    pe_va               eat;
    ordinal_t           next_ord;
    ordinal_t           high_ord = 0;
    size_t              num_entries;

    strncpy( object->name, ".edata", PE_OBJ_NAME_LEN );
    object->physical_offset = NullAlign( file_align );
    object->flags = PE_OBJ_INIT_DATA | PE_OBJ_READABLE;
    dir.flags = 0;
    dir.time_stamp = 0;
    dir.major = 0;
    dir.minor = 0;
    dir.name_rva = object->rva + sizeof( dir );
    dir.ordinal_base = FmtData.u.os2fam.exports->ordinal;
    if( FmtData.u.os2fam.module_name != nullptr ) {
        name = FmtData.u.os2fam.module_name;
    } else {
        name = GetBaseName( Root->outfile->fname, 0, &namelen );
    }
    /* GetBaseName strips the extension, which we actually need to keep!
     * Always recalculate the len including the extension.
     */
    namelen = strlen( name ) + 1;
    dir.address_table_rva = __ROUND_UP_SIZE( dir.name_rva + namelen, sizeof( pe_va ) );
    num_entries = 0;
    for( exp = FmtData.u.os2fam.exports; exp != nullptr; exp = exp->next ) {
        high_ord = exp->ordinal;
        ++num_entries;
        if( !exp->isprivate ) {
            if( exp->impname != nullptr ) {
                AddImpLibEntry( exp->impname, exp->name.u.ptr, 0, true );
            } else {
                AddImpLibEntry( exp->sym->name.u.ptr, exp->name.u.ptr, 0, true );
            }
        }
    }
    dir.num_eat_entries = high_ord - dir.ordinal_base + 1;
    dir.num_name_ptrs = num_entries;
    dir.name_ptr_table_rva = dir.address_table_rva + dir.num_eat_entries * sizeof( pe_va );
    dir.ordinal_table_rva = dir.name_ptr_table_rva + num_entries * sizeof( pe_va );
    sort.resize( num_entries );
    /* write the export directory table */
    WriteLoad( &dir, sizeof( dir ) );
    /* write the module name (includes null terminator) */
    WriteLoad( name, namelen );
    NullAlign( sizeof( pe_va ) );
    /* write the export address table */
    i = 0;
    next_ord = dir.ordinal_base;
    for( exp = FmtData.u.os2fam.exports; exp != nullptr; exp = exp->next ) {
        sort[i++] = exp;
        eat = exp->addr.off;
        if( next_ord < exp->ordinal ) {
            PadLoad( ( exp->ordinal - next_ord ) * sizeof( pe_va ) );
        }
        next_ord = exp->ordinal + 1;
        WriteLoadU32( eat );
    }
    qsort( sort.data(), num_entries, sizeof( entry_export * ), &namecmp_exp );
    /* write out the export name ptr table */
    eat = dir.ordinal_table_rva + num_entries * sizeof( unsigned_16 );
    for( i = 0; i < num_entries; ++i ) {
        exp = sort[i];
        WriteLoadU32( eat );
        eat += strlen( exp->name.u.ptr ) + 1;
    }
    /* write out the export ordinal table */
    for( i = 0; i < num_entries; ++i ) {
        WriteLoadU16( sort[i]->ordinal - dir.ordinal_base );
    }
    /* write out the export name table */
    for( i = 0; i < num_entries; ++i ) {
        exp = sort[i];
        WriteLoad( exp->name.u.ptr, strlen( exp->name.u.ptr ) + 1 );
    }
    size = eat - object->rva;
    object->physical_size = __ROUND_UP_SIZE( size, file_align );
    entry->size = size;
    entry->rva = object->rva;
    return( size );
}

static unsigned_32 WriteRelocList( reloc_info reloclist, unsigned_32 size, unsigned_32 pagerva )
/**********************************************************************************************/
{
    unsigned        pagesize;
    bool            padme;

    pagesize = RelocSize( reloclist );
    if( pagesize != 0 ) {
        padme = false;
        if( ( pagesize / sizeof( pe_reloc_item ) ) & 0x1 ) {
            pagesize += sizeof( pe_reloc_item );
            padme = true;
        }
        pagesize += 2 * sizeof( unsigned_32 );
        WriteLoadU32( pagerva );
        WriteLoadU32( pagesize );
        DumpRelocList( reloclist );
        if( padme ) {
            PadLoad( sizeof( pe_reloc_item ) );
        }
        size += pagesize;
    }
    return( size );
}

static unsigned WriteFixupInfo( pe_object *object, unsigned file_align, pe_dir_entry *entry )
/********************************************************************************************
 * dump the fixup table
 */
{
    unsigned        numpages;
    unsigned        pagerva;
    group_entry     *group;
    reloc_info      *reloclist_array;
    unsigned long   size;

    strncpy( object->name, ".reloc", PE_OBJ_NAME_LEN );
    object->physical_offset = NullAlign( file_align );
    object->flags = PE_OBJ_INIT_DATA | PE_OBJ_READABLE | PE_OBJ_DISCARDABLE;
    size = 0;
    /*
     * When using non-default object alignment, groups and pages need
     * not be in sync at all.
     */
    for( group = Groups; group != nullptr; group = group->next ) {
        reloclist_array = group->g.reloclist_array;
        if( reloclist_array != nullptr ) {
            pagerva = group->linear;
            numpages = __ROUND_UP_SIZE_TO_4K( group->size );
            while( numpages-- > 0 ) {
                size = WriteRelocList( *reloclist_array++, size, pagerva );
                pagerva += OSF_PAGE_SIZE;
            }
        }
    }
    PadLoad( sizeof( pe_fixup_header ) );
    size += sizeof( pe_fixup_header );
    object->physical_size = __ROUND_UP_SIZE( size, file_align );
    entry->size = size - sizeof( pe_fixup_header );
    entry->rva = object->rva;
    return( size );
}

static unsigned WriteDescription( pe_object *object, unsigned file_align )
/************************************************************************/
{
    size_t          desc_len;

    desc_len = strlen( FmtData.description );
    strncpy( object->name, ".desc", PE_OBJ_NAME_LEN );
    object->physical_offset = NullAlign( file_align );
    object->flags = PE_OBJ_INIT_DATA | PE_OBJ_READABLE;
    object->physical_size = __ROUND_UP_SIZE( desc_len, file_align );
    WriteLoad( FmtData.description, desc_len );
    return( desc_len );
}

bool RcPadFile( FILE *fp, size_t pad )
{
    DbgAssert( FP2POSIX( fp ) == Root->outfile->handle );

    /* unused parameters */ static_cast<void>( fp );

    PadLoad( pad );
    return( false );
}

void CheckDebugOffset( ExeFileInfo *info )
{
    /* unused parameters */ static_cast<void>( info );
}

RcStatus CopyExeData( FILE *in_fp, FILE *out_fp, unsigned length )
/*****************************************************************
 * CopyExeData function is used by resource compiler to write data to executable
 * there are two implementation one in resource compiler and second in linker
 */
{
    /* unused parameters */ static_cast<void>( out_fp );

    for( ; length > MAX_HEADROOM; length -= MAX_HEADROOM ) {
        QRead( FP2POSIX( in_fp ), TokBuff, MAX_HEADROOM, "resource file" );
        WriteLoad( TokBuff, MAX_HEADROOM );
    }
    if( length > 0 ) {
        QRead( FP2POSIX( in_fp ), TokBuff, length, "resource file" );
        WriteLoad( TokBuff, length );
    }
    return( RS_OK );
}

void DoAddResource( char *name )
/******************************/
{
    list_of_names   *info;
    unsigned        len;

    len = strlen( name );
    info = static_cast<list_of_names *>( _PermAlloc( sizeof( list_of_names ) + len ) );
    memcpy( info->name, name, len + 1 );
    info->next = FmtData.u.pe.resources;
    FmtData.u.pe.resources = info;
}

static unsigned WritePEResources( pe_exe_header *pehdr, pe_object *object, unsigned file_align )
/**********************************************************************************************/
{
    ExeFileInfo     einfo;
    ResFileInfo     *rinfo;
    bool            allopen;
    bool            status;

    memset( &einfo, 0, sizeof( einfo ) );

    if( FmtData.resource != nullptr ) {
        DoAddResource( FmtData.resource );
        FmtData.resource = nullptr;
    }
    status = OpenResFiles( reinterpret_cast<ExtraRes *>( FmtData.u.pe.resources ), &rinfo, &allopen, EXE_TYPE_PE, Root->outfile->fname );
    if( !status )               // we had a problem opening
        return( 0 );
    einfo.fp = POSIX2FP( Root->outfile->handle );
    einfo.name = Root->outfile->fname;
    einfo.u.PEInfo.WinHead = pehdr;
    einfo.Type = EXE_TYPE_PE;
    status = BuildPEResourceObject( &einfo, rinfo, object, object->rva, NullAlign( file_align ), !allopen );
    CloseResFiles( rinfo );
    return( object->physical_size );
}

static unsigned WriteDebugTable( pe_object *object, const char *symfilename,
                unsigned file_align, unsigned time_stamp, pe_dir_entry *entry )
/***********************************************************************************/
{
    debug_directory dir;
    unsigned        size;

    if( symfilename == nullptr ) {
        /* two entries */
        size = 2 * sizeof( debug_directory );
    } else {
        /* one entry */
        size = sizeof( debug_directory );
    }
    strncpy( object->name, ".rdata", PE_OBJ_NAME_LEN );
    object->physical_offset = NullAlign( file_align );
    object->flags = PE_OBJ_INIT_DATA | PE_OBJ_READABLE;
    object->physical_size = __ROUND_UP_SIZE( size, file_align );

    /* write debug dir entry for DEBUG_TYPE_MISC */
    dir.flags = 0;
    dir.time_stamp = time_stamp;
    dir.major = 0;
    dir.minor = 0;
    dir.debug_type = DEBUG_TYPE_MISC;
    dir.debug_size = sizeof( debug_misc_dbgdata );
    dir.data_rva = 0;
    dir.data_seek = object->physical_offset + object->physical_size;
    WriteLoad( &dir, sizeof( debug_directory ) );

    /* remember current file offset of this directory entry for later use */
    CVDebugDirEntryPos = PosLoad();

    if( symfilename == nullptr ) {
        /* write debug dir entry for DEBUG_TYPE_CODEVIEW */
        dir.flags = 0;
        dir.time_stamp = time_stamp;
        dir.major = 0;
        dir.minor = 0;
        dir.debug_type = DEBUG_TYPE_CODEVIEW;
        dir.debug_size = CVSize;
        dir.data_rva = 0;
        dir.data_seek = object->physical_offset + object->physical_size + sizeof( debug_misc_dbgdata );
        WriteLoad( &dir, sizeof( debug_directory ) );
    }

    entry->size = size;
    entry->rva = object->rva;
    return( size );
}

static void CheckNumRelocs( void )
/*********************************
 * don't want to generate a .reloc section if we don't have any relocs
 */
{
    group_entry     *group;
    symbol          *sym;

    if( (LinkState & LS_MAKE_RELOCS) == 0 )
        return;
    for( group = Groups; group != nullptr; group = group->next ) {
        if( group->g.reloclist_array != nullptr ) {
            return;
        }
    }
    WALK_IMPORT_SYMBOLS( sym ) {
        if( LinkState & LS_HAVE_MACHTYPE_MASK ) {
            return;
        }
    }
    LinkState &= ~LS_MAKE_RELOCS;
}

static seg_leader *SetLeaderTable( const char *name, pe_dir_entry *entry )
/************************************************************************/
{
    seg_leader      *leader;

    leader = FindSegment( Root, name );
    if( leader != nullptr ) {
        entry->rva =  leader->group->linear + SEG_GROUP_DELTA( leader );
        entry->size = leader->size;
    }
    return( leader );
}

static int CmpDesc( virt_mem a, virt_mem b )
/******************************************/
{
    unsigned_32     a32;
    unsigned_32     b32;

    GET32INFO( *reinterpret_cast<virt_mem *>( a ), a32 );
    GET32INFO( *reinterpret_cast<virt_mem *>( b ), b32 );
    return( static_cast<int>( a32 ) - static_cast<int>( b32 ) );
}

static void SwapDesc( virt_mem a, virt_mem b )
/********************************************/
{
    procedure_descriptor    tmp;

    a = *reinterpret_cast<virt_mem *>( a );
    b = *reinterpret_cast<virt_mem *>( b );
    ReadInfo( a, &tmp, sizeof( procedure_descriptor ) );
    CopyInfo( a, b, sizeof( procedure_descriptor ) );
    PutInfo( b, &tmp, sizeof( procedure_descriptor ) );
}

static bool SetPDataArray( void *_sdata, void *_array )
/*****************************************************/
{
    segdata         *sdata = static_cast<segdata *>( _sdata );
    virt_mem        **array = static_cast<virt_mem **>( _array );
    offset          size;
    virt_mem        data;

    if( !sdata->isdead ) {
        data = sdata->u1.vm_ptr;
        for( size = sdata->length; size >= sizeof( procedure_descriptor ); size -= sizeof( procedure_descriptor ) ) {
            **array = data;
            *array += 1;
            data += sizeof( procedure_descriptor );
        }
    }
    return( false );
}

static void SetMiscTableEntries( pe_exe_header *pehdr )
/*****************************************************/
{
    seg_leader      *leader;
    virt_mem        *temp;
    unsigned        numpdatas;
    symbol          *sym;

    SetLeaderTable( IDataGrpName, &PE_DIRECTORY( *pehdr, PE_TBL_IMPORT ) );
    sym = FindISymbol( TLSSym );
    if( sym != nullptr ) {
        PE_DIRECTORY( *pehdr, PE_TBL_THREAD ).rva = FindLinearAddr( &sym->addr );
        PE_DIRECTORY( *pehdr, PE_TBL_THREAD ).size = sym->p.seg->length;
    }
    leader = SetLeaderTable( CoffPDataSegName, &PE_DIRECTORY( *pehdr, PE_TBL_EXCEPTION ) );
    /* The .pdata section may end up being empty if the symbols got optimized out */
    if( leader != nullptr
      && leader->size ) {
        numpdatas = leader->size / sizeof( procedure_descriptor );
        std::vector<virt_mem> sortarray( numpdatas );
        temp = sortarray.data();
        RingLookup( leader->pieces, SetPDataArray, &temp );
        VMemQSort( reinterpret_cast<virt_mem>( sortarray.data() ), numpdatas, sizeof( virt_mem * ), SwapDesc, CmpDesc );
    }
}

static unsigned FindNumObjects( void )
/************************************/
{
    unsigned        num_objects;

    num_objects = NumGroups;
    if( LinkState & LS_MAKE_RELOCS )
        ++num_objects;
    if( FmtData.u.os2fam.exports != nullptr )
        ++num_objects;
    if( LinkFlags & LF_CV_DBI_FLAG )
        ++num_objects;
    if( FmtData.description != nullptr )
        ++num_objects;
    if( FmtData.resource != nullptr
      || FmtData.u.pe.resources != nullptr )
        ++num_objects;
    return( num_objects );
}

static unsigned long CalcPEChecksum( unsigned long __wCrc, unsigned short *__pwBuffer, unsigned long __dwCount )
/**************************************************************************************************************/
{
    while( __dwCount-- > 0 ) {
        __wCrc += *__pwBuffer++;
        __wCrc = ( __wCrc & 0x0000FFFF ) + ( __wCrc >> 16 );
    }
    __wCrc = ( ( __wCrc >> 16 ) + __wCrc );
    return( __wCrc & 0x0000FFFF );
}

static unsigned get_stack_commit_size( unsigned size )
{
    if( FmtData.u.pe.stackcommit == DEF_VALUE ) {
        if( FmtData.dll ) {
            if( size > PE_DLL_DEF_STACK_COMMIT ) {
                size = PE_DLL_DEF_STACK_COMMIT;
            }
        } else {
            if( size > PE_EXE_DEF_STACK_COMMIT ) {
                size = PE_EXE_DEF_STACK_COMMIT;
            }
        }
    } else if( size > FmtData.u.pe.stackcommit ) {
        size = FmtData.u.pe.stackcommit;
    }
    return( size );
}

static unsigned get_heap_reserve_size( void )
{
    unsigned        size;

    size = FmtData.u.os2fam.heapsize;
    if( size == DEF_VALUE ) {
        if( FmtData.dll ) {
            size = PE_DLL_DEF_HEAP_SIZE;
        } else {
            size = PE_EXE_DEF_HEAP_SIZE;
        }
    }
    return( size );
}

static unsigned get_heap_commit_size( unsigned size )
{
    if( FmtData.u.pe.heapcommit == DEF_VALUE ) {
        if( FmtData.dll ) {
            if( size > PE_DLL_DEF_HEAP_COMMIT ) {
                size = PE_DLL_DEF_HEAP_COMMIT;
            }
        } else {
            if( size > PE_EXE_DEF_HEAP_COMMIT ) {
                size = PE_EXE_DEF_HEAP_COMMIT;
            }
        }
    } else if( size > FmtData.u.pe.heapcommit ) {
        size = FmtData.u.pe.heapcommit;
    }
    return( size );
}

void FiniPELoadFile( void )
/**************************
 * make a PE executable file
 */
{
    pe_exe_header   pehdr;
    unsigned        stub_len;
    std::vector<pe_object> objects;
    unsigned        num_objects;
    pe_object       *tbl_obj;
    unsigned        head_size;
    unsigned        file_align;
    unsigned        size;
    unsigned        image_size;

    file_align = 1UL << FmtData.u.os2fam.segment_shift;
    CheckNumRelocs();
    num_objects = FindNumObjects();
    memset( &pehdr, 0, sizeof( pehdr ) ); /* zero all header fields */
    if( LinkState & LS_HAVE_X64_CODE ) {
        pehdr.signature = EXESIGN_PE;
        head_size = PE64_SIZE;
        PE64( pehdr ).magic = 0x20b;
        pehdr.fheader.cpu_type = PE_CPU_AMD64;
        pehdr.fheader.num_objects = num_objects;
        pehdr.fheader.time_stamp = static_cast<unsigned_32>( std::time( nullptr ) );
        pehdr.fheader.opt_hdr_size = PE64_OPT_SIZE;
        pehdr.fheader.flags = PE_FLG_REVERSE_BYTE_LO | PE_FLG_32BIT_MACHINE | PE_FLG_LARGE_ADDRESS_AWARE;
        if( FmtData.u.pe.nolargeaddressaware ) {
            pehdr.fheader.flags &= ~PE_FLG_LARGE_ADDRESS_AWARE;
        }
        if( (LinkState & LS_MAKE_RELOCS) == 0 ) {
            pehdr.fheader.flags |= PE_FLG_RELOCS_STRIPPED;
        }
        if( (LinkState & LS_LINK_ERROR) == 0 ) {
            pehdr.fheader.flags |= PE_FLG_IS_EXECUTABLE;
        }
        if( FmtData.dll ) {
            pehdr.fheader.flags |= PE_FLG_LIBRARY;
            if( FmtData.u.os2fam.flags & INIT_INSTANCE_FLAG ) {
                PE64( pehdr ).dll_flags |= PE_DLL_PERPROC_INIT;
            } else if( FmtData.u.os2fam.flags & INIT_THREAD_FLAG ) {
                PE64( pehdr ).dll_flags |= PE_DLL_PERTHRD_INIT;
            }
            if( FmtData.u.os2fam.flags & TERM_INSTANCE_FLAG ) {
                PE64( pehdr ).dll_flags |= PE_DLL_PERPROC_TERM;
            } else if( FmtData.u.os2fam.flags & TERM_THREAD_FLAG ) {
                PE64( pehdr ).dll_flags |= PE_DLL_PERTHRD_TERM;
            }
        }

        if( FmtData.u.pe.lnk_specd ) {
            PE64( pehdr ).lnk_major = FmtData.u.pe.linkmajor;
            PE64( pehdr ).lnk_minor = FmtData.u.pe.linkminor;
        } else {
            PE64( pehdr ).lnk_major = PE_LNK_MAJOR;
            PE64( pehdr ).lnk_minor = PE_LNK_MINOR;
        }
        PE64( pehdr ).image_base.u._32[0] = FmtData.base;
        PE64( pehdr ).image_base.u._32[1] = 0;
        PE64( pehdr ).object_align = FmtData.objalign;

        /*
         *  I have changed this to allow programmers to control this shift. MS has 0x20 byte segments
         *  in some drivers! Who are we to argue? Never mind it's against the PE spec.
         */
        if( FmtData.u.os2fam.segment_shift < MINIMUM_SEG_SHIFT ) {
            LnkMsg( WRN+MSG_VALUE_INCORRECT, "s", "ALIGNMENT" );
            FmtData.u.os2fam.segment_shift = DEFAULT_SEG_SHIFT;
        }

        PE64( pehdr ).file_align = file_align;

        if( FmtData.u.pe.osv_specd ) {
            PE64( pehdr ).os_major = FmtData.u.pe.osmajor;
            PE64( pehdr ).os_minor = FmtData.u.pe.osminor;
        } else {
            PE64( pehdr ).os_major = PE_OS_MAJOR;
            PE64( pehdr ).os_minor = PE_OS_MINOR + 0xb;      // KLUDGE! ????
        }

        if( FmtData.ver_specified ) {
            PE64( pehdr ).user_major = FmtData.major;
            PE64( pehdr ).user_minor = FmtData.minor;
        } else {
            PE64( pehdr ).user_major = 0;
            PE64( pehdr ).user_minor = 0;
        }
        if( FmtData.u.pe.sub_specd ) {
            PE64( pehdr ).subsys_major = FmtData.u.pe.submajor;
            PE64( pehdr ).subsys_minor = FmtData.u.pe.subminor;
        } else {
            PE64( pehdr ).subsys_major = 3;
            PE64( pehdr ).subsys_minor = 0xa;
        }
        if( FmtData.u.pe.subsystem != PE_SS_UNKNOWN ) {
            PE64( pehdr ).subsystem = FmtData.u.pe.subsystem;
        } else {
            PE64( pehdr ).subsystem = PE_SS_WINDOWS_GUI;
        }
        /*
         * set stack reserved size for executable
         * zero for DLL (StackSize is already set to zero)
         */
        PE64( pehdr ).stack_reserve_size.u._32[0] = size = StackSize;
        PE64( pehdr ).stack_reserve_size.u._32[1] = 0;
        /*
         * set committed size for executable
         */
        PE64( pehdr ).stack_commit_size.u._32[0] = get_stack_commit_size( size );
        PE64( pehdr ).stack_commit_size.u._32[1] = 0;
        /*
         * set initial heap reserved size, if not setup use default value
         */
        PE64( pehdr ).heap_reserve_size.u._32[0] = size = get_heap_reserve_size();
        PE64( pehdr ).heap_reserve_size.u._32[1] = 0;
        /*
         * set initial heap commited size, if not setup use default value
         */
        PE64( pehdr ).heap_commit_size.u._32[0] = get_heap_commit_size( size );
        PE64( pehdr ).heap_commit_size.u._32[1] = 0;

        PE64( pehdr ).num_tables = PE_TBL_NUMBER;
        CurrSect = Root;
        SeekLoad( 0 );
        stub_len = WriteStubFile( STUB_ALIGN );
        objects.resize( num_objects );
        memset( objects.data(), 0, num_objects * sizeof( pe_object ) );
        /* leave space for the header and object table */
        PadLoad( head_size + num_objects * sizeof( pe_object ) );
        GenPETransferTable();
        WriteImportInfo();
        SetMiscTableEntries( &pehdr );
        image_size = WriteDataPages( &pehdr, objects.data(), file_align );
        tbl_obj = &objects[NumGroups];
        if( FmtData.u.os2fam.exports != nullptr ) {
            tbl_obj->rva = image_size;
            size = WriteExportInfo( tbl_obj, file_align, &PE_DIRECTORY( pehdr, PE_TBL_EXPORT ) );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( LinkState & LS_MAKE_RELOCS ) {
            tbl_obj->rva = image_size;
            size = WriteFixupInfo( tbl_obj, file_align, &PE_DIRECTORY( pehdr, PE_TBL_FIXUP ) );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( FmtData.description != nullptr ) {
            tbl_obj->rva = image_size;
            size = WriteDescription( tbl_obj, file_align );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( FmtData.resource != nullptr
          || FmtData.u.pe.resources != nullptr ) {
            tbl_obj->rva = image_size;
            size = WritePEResources( &pehdr, tbl_obj, file_align );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( LinkFlags & LF_CV_DBI_FLAG ) {
            tbl_obj->rva = image_size;
            size = WriteDebugTable( tbl_obj, SymFileName, file_align, pehdr.fheader.time_stamp, &PE_DIRECTORY( pehdr, PE_TBL_DEBUG ) );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        NullAlign( file_align ); /* pad out last page */
        PE64( pehdr ).image_size = image_size;
        PE64( pehdr ).headers_size = objects[0].physical_offset;
    } else {
        if( FmtData.u.pe.tnt
          || FmtData.u.pe.subsystem == PE_SS_PL_DOSSTYLE ) {
            pehdr.signature = EXESIGN_PL;
        } else {
            pehdr.signature = EXESIGN_PE;
        }
        head_size = PE32_SIZE;
        PE32( pehdr ).magic = 0x10b;
        switch( LinkState & LS_HAVE_MACHTYPE_MASK ) {
        case LS_HAVE_ALPHA_CODE:
            pehdr.fheader.cpu_type = PE_CPU_ALPHA;
            break;
        case LS_HAVE_MIPS_CODE:     // TODO
//            pehdr.fheader.cpu_type = PE_CPU_MIPS_R3000;
            pehdr.fheader.cpu_type = PE_CPU_MIPS_R4000;
            break;
        case LS_HAVE_PPC_CODE:
            pehdr.fheader.cpu_type = PE_CPU_POWERPC;
            break;
        case LS_HAVE_X86_CODE:
            pehdr.fheader.cpu_type = PE_CPU_386;
            break;
        default:
            break;
        }
        pehdr.fheader.num_objects = num_objects;
        pehdr.fheader.time_stamp = static_cast<unsigned_32>( std::time( nullptr ) );
        pehdr.fheader.opt_hdr_size = PE32_OPT_SIZE;
        pehdr.fheader.flags = PE_FLG_REVERSE_BYTE_LO | PE_FLG_32BIT_MACHINE;
        if( FmtData.u.pe.largeaddressaware ) {
            pehdr.fheader.flags |= PE_FLG_LARGE_ADDRESS_AWARE;
        }
        if( (LinkState & LS_MAKE_RELOCS) == 0 ) {
            pehdr.fheader.flags |= PE_FLG_RELOCS_STRIPPED;
        }
        if( (LinkState & LS_LINK_ERROR) == 0 ) {
            pehdr.fheader.flags |= PE_FLG_IS_EXECUTABLE;
        }
        if( FmtData.dll ) {
            pehdr.fheader.flags |= PE_FLG_LIBRARY;
            if( FmtData.u.os2fam.flags & INIT_INSTANCE_FLAG ) {
                PE32( pehdr ).dll_flags |= PE_DLL_PERPROC_INIT;
            } else if( FmtData.u.os2fam.flags & INIT_THREAD_FLAG ) {
                PE32( pehdr ).dll_flags |= PE_DLL_PERTHRD_INIT;
            }
            if( FmtData.u.os2fam.flags & TERM_INSTANCE_FLAG ) {
                PE32( pehdr ).dll_flags |= PE_DLL_PERPROC_TERM;
            } else if( FmtData.u.os2fam.flags & TERM_THREAD_FLAG ) {
                PE32( pehdr ).dll_flags |= PE_DLL_PERTHRD_TERM;
            }
        }

        if( FmtData.u.pe.lnk_specd ) {
            PE32( pehdr ).lnk_major = FmtData.u.pe.linkmajor;
            PE32( pehdr ).lnk_minor = FmtData.u.pe.linkminor;
        } else {
            PE32( pehdr ).lnk_major = PE_LNK_MAJOR;
            PE32( pehdr ).lnk_minor = PE_LNK_MINOR;
        }
        PE32( pehdr ).image_base = FmtData.base;
        PE32( pehdr ).object_align = FmtData.objalign;

        /*
         *  I have changed this to allow programmers to control this shift. MS has 0x20 byte segments
         *  in some drivers! Who are we to argue? Never mind it's against the PE spec.
         */
        if( FmtData.u.os2fam.segment_shift < MINIMUM_SEG_SHIFT ) {
            LnkMsg( WRN+MSG_VALUE_INCORRECT, "s", "ALIGNMENT" );
            FmtData.u.os2fam.segment_shift = DEFAULT_SEG_SHIFT;
        }

        file_align = 1UL << FmtData.u.os2fam.segment_shift;
        PE32( pehdr ).file_align = file_align;

        if( FmtData.u.pe.osv_specd ) {
            PE32( pehdr ).os_major = FmtData.u.pe.osmajor;
            PE32( pehdr ).os_minor = FmtData.u.pe.osminor;
        } else if( FmtData.u.pe.subsystem == PE_SS_RDOS ) {
            // RDOS default
            PE32( pehdr ).os_major = PE_RDOS_OS_MAJOR;
            PE32( pehdr ).os_minor = PE_RDOS_OS_MINOR;
        } else {
            PE32( pehdr ).os_major = PE_OS_MAJOR;
            PE32( pehdr ).os_minor = PE_OS_MINOR + 0xb;      // KLUDGE!
        }

        if( FmtData.ver_specified ) {
            PE32( pehdr ).user_major = FmtData.major;
            PE32( pehdr ).user_minor = FmtData.minor;
        } else {
            PE32( pehdr ).user_major = 0;
            PE32( pehdr ).user_minor = 0;
        }
        if( FmtData.u.pe.sub_specd ) {
            PE32( pehdr ).subsys_major = FmtData.u.pe.submajor;
            PE32( pehdr ).subsys_minor = FmtData.u.pe.subminor;
        } else if( FmtData.u.pe.subsystem == PE_SS_RDOS ) {
            // RDOS default
            PE32( pehdr ).subsys_major = PE_RDOS_SS_MAJOR;
            PE32( pehdr ).subsys_minor = PE_RDOS_SS_MINOR;
        } else {
            PE32( pehdr ).subsys_major = 3;
            PE32( pehdr ).subsys_minor = 0xa;
        }
        if( FmtData.u.pe.subsystem != PE_SS_UNKNOWN ) {
            PE32( pehdr ).subsystem = FmtData.u.pe.subsystem;
        } else {
            PE32( pehdr ).subsystem = PE_SS_WINDOWS_GUI;
        }
        /*
         * set stack reserved size for executable
         * zero for DLL (StackSize is already set to zero)
         */
        PE32( pehdr ).stack_reserve_size = size = StackSize;
        /*
         * set committed size for executable
         */
        PE32( pehdr ).stack_commit_size = get_stack_commit_size( size );
        /*
         * set initial heap reserved size, if not setup use default value
         */
        PE32( pehdr ).heap_reserve_size = size = get_heap_reserve_size();
        /*
         * set initial heap commited size, if not setup use default value
         */
        PE32( pehdr ).heap_commit_size = get_heap_commit_size( size );

        PE32( pehdr ).num_tables = PE_TBL_NUMBER;
        CurrSect = Root;
        SeekLoad( 0 );
        stub_len = WriteStubFile( STUB_ALIGN );
        objects.resize( num_objects );
        memset( objects.data(), 0, num_objects * sizeof( pe_object ) );
        /* leave space for the header and object table */
        PadLoad( head_size + num_objects * sizeof( pe_object ) );
        GenPETransferTable();
        WriteImportInfo();
        SetMiscTableEntries( &pehdr );
        image_size = WriteDataPages( &pehdr, objects.data(), file_align );
        tbl_obj = &objects[NumGroups];
        if( FmtData.u.os2fam.exports != nullptr ) {
            tbl_obj->rva = image_size;
            size = WriteExportInfo( tbl_obj, file_align, &PE_DIRECTORY( pehdr, PE_TBL_EXPORT ) );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( LinkState & LS_MAKE_RELOCS ) {
            tbl_obj->rva = image_size;
            size = WriteFixupInfo( tbl_obj, file_align, &PE_DIRECTORY( pehdr, PE_TBL_FIXUP ) );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( FmtData.description != nullptr ) {
            tbl_obj->rva = image_size;
            size = WriteDescription( tbl_obj, file_align );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( FmtData.resource != nullptr
          || FmtData.u.pe.resources != nullptr ) {
            tbl_obj->rva = image_size;
            size = WritePEResources( &pehdr, tbl_obj, file_align );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        if( LinkFlags & LF_CV_DBI_FLAG ) {
            tbl_obj->rva = image_size;
            size = WriteDebugTable( tbl_obj, SymFileName, file_align, pehdr.fheader.time_stamp, &PE_DIRECTORY( pehdr, PE_TBL_DEBUG ) );
            image_size += __ROUND_UP_SIZE( size, FmtData.objalign );
            ++tbl_obj;
        }
        NullAlign( file_align ); /* pad out last page */
        PE32( pehdr ).image_size = image_size;
        PE32( pehdr ).headers_size = objects[0].physical_offset;
    }
    DBIWrite();
    SeekLoad( stub_len );

    if( FmtData.u.pe.checksumfile ) {
        /* Ensure checksum is 0 before we calculate it */
        if( LinkState & LS_HAVE_X64_CODE ) {
            PE64( pehdr ).file_checksum = 0L;
        } else {
            PE32( pehdr ).file_checksum = 0L;
        }
    }

    WriteLoad( &pehdr, head_size );
    WriteLoad( objects.data(), num_objects * sizeof( pe_object ) );

    if( FmtData.u.pe.checksumfile ) {
        unsigned        crc = 0L;
        size_t          buffsize;
        unsigned long   currpos = 0L;
        unsigned long   totalsize = 0L;
        outfilelist     *outfile;
        std::vector<char> buffer;

        /*
         *  Checksum required. We have already written the EXE header with a NULL checksum
         *  We need to calculate the checksum over all blocks
         *  We flush the buffers by seeking back to 0, then repeatedly reading all the file back in
         *  and checksumming it. The MS checksum is completed by adding the total file size to the
         *  calculated CRC. We then add this to the header and rewrite the header.
         */
        SeekLoad( 0 ); /* Flush the buffer */
        outfile = CurrSect->outfile;
        DbgAssert( outfile->buffer == nullptr );

        totalsize = QFileSize( outfile->handle );

        constexpr std::size_t CRC_BUFF_SIZE = _16K;
        buffer.resize( CRC_BUFF_SIZE );
        {
            for( ; currpos < totalsize; currpos += buffsize ) {
                std::fill( buffer.begin(), buffer.end(), 0 );
                buffsize = QRead( outfile->handle, buffer.data(), CRC_BUFF_SIZE, outfile->fname );
                DbgAssert( buffsize == IOERROR );
                DbgAssert( (buffsize % 2) != 1 ); /* check for odd length */

                crc = CalcPEChecksum( crc, reinterpret_cast<unsigned short *>( buffer.data() ), buffsize / sizeof( unsigned short ) );
            }

            crc += totalsize;

            if( LinkState & LS_HAVE_X64_CODE ) {
                PE64( pehdr ).file_checksum = crc;
            } else {
                PE32( pehdr ).file_checksum = crc;
            }
            SeekLoad( stub_len );
            WriteLoad( &pehdr, head_size );
        }
    }

}

static unsigned getStubSize( void )
/**********************************
 * return the size of the stub file (unaligned)
 */
{
    unsigned        stub_len = 0;
    f_handle        the_file;
    dos_exe_header  dosheader;
    unsigned        read_len;
    unsigned        reloc_size;
    unsigned        code_start;
    std::array<char, PATH_MAX> fullname;

    if( FmtData.u.os2fam.no_stub ) {
        return( 0 );
    }
    stub_len = GetDOSDefStubSize();
    if( FmtData.u.os2fam.stub_file_name != nullptr
      && stricmp( FmtData.u.os2fam.stub_file_name, Root->outfile->fname ) != 0 ) {
        the_file = FindPath( FmtData.u.os2fam.stub_file_name, fullname.data() );
        if( the_file == NIL_FHANDLE ) {
            LnkMsg( WRN+MSG_CANT_OPEN_NO_REASON, "s", FmtData.u.os2fam.stub_file_name );
        } else {
            MemFree( FmtData.u.os2fam.stub_file_name );
            FmtData.u.os2fam.stub_file_name = MemStrdupSafe( fullname.data() );
            QRead( the_file, &dosheader, sizeof( dos_exe_header ), FmtData.u.os2fam.stub_file_name );
            if( dosheader.signature == EXESIGN_DOS ) {
                code_start = dosheader.hdr_size * 16U;
                read_len = dosheader.file_size * 512U - (-dosheader.mod_size & 0x1ff) - code_start;
                // make sure reloc_size is a multiple of 16.
                reloc_size = __ROUND_UP_SIZE_PARA( dosheader.num_relocs * 4U );
                dosheader.hdr_size = 4 + reloc_size / 16;
                stub_len = read_len + dosheader.hdr_size * 16U;
            }
            QClose( the_file, FmtData.u.os2fam.stub_file_name );
        }
    }
    return( stub_len );
}

unsigned long GetPEHeaderSize( void )
/***********************************/
{
    unsigned long   size;
    unsigned        num_objects;

    num_objects = FindNumObjects();
    size = __ROUND_UP_SIZE( getStubSize(), STUB_ALIGN ) + num_objects * sizeof( pe_object );
    if( LinkState & LS_HAVE_X64_CODE ) {
        size += PE64_SIZE;
    } else {
        size += PE32_SIZE;
    }
    return( __ROUND_UP_SIZE( size, FmtData.objalign ) );
}

static void ReadExports( unsigned namestart, unsigned nameend,
                         unsigned ordstart, unsigned numords,
                         ordinal_t ord_base, f_handle file, const char *fname )
/*****************************************************************************/
{
    std::vector<unsigned_16> ordbuf( numords );
    unsigned_16     *ordptr;
    char            *nameptr;

    QSeek( file, ordstart, fname );
    QRead( file, ordbuf.data(), numords * sizeof( unsigned_16 ), fname );
    QSeek( file, namestart, fname );
    QRead( file, TokBuff, nameend - namestart, fname );
    nameptr = TokBuff;
    ordptr = ordbuf.data();
    for( ; numords > 0; --numords ) {
        CheckExport( nameptr, *ordptr + ord_base, true );
        while( *nameptr != '\0' )
            nameptr++;
        nameptr++;
        ordptr++;
    }
}

void ReadPEExportTable( f_handle file, pe_dir_entry *export_dir )
/****************************************************************
 * read a PE export table, and set ordinal values accordingly.
 */
{
    pe_export_directory table;
    const char          *fname;
    unsigned            *nameptrs;
    unsigned            nameptrsize;
    unsigned            numentries;
    unsigned            entrystart;
    unsigned            *curr;
    unsigned            namestart;

    fname = FmtData.u.os2fam.old_lib_name;
    QRead( file, &table, sizeof( pe_export_directory ), fname );
    nameptrsize = table.num_name_ptrs * sizeof( unsigned_32 );
    if( nameptrsize == 0 )                      /* NOTE: <-- premature return */
        return;
    nameptrs = static_cast<unsigned *>( MemAllocSafe( nameptrsize + sizeof( unsigned_32 ) ) );
    QSeek( file, table.name_ptr_table_rva - export_dir->rva, fname );
    QRead( file, nameptrs, nameptrsize, fname );
    numentries = 1;
    entrystart = table.ordinal_table_rva - export_dir->rva;
    curr = nameptrs;
    *curr -= export_dir->rva;
    namestart = *curr++;
    for( nameptrsize -= sizeof( unsigned_32 ); nameptrsize > 0; nameptrsize -= sizeof( unsigned_32 ) ) {
        *curr -= export_dir->rva;
        if( *curr - namestart > TokSize ) {
            ReadExports( namestart, *(curr - 1), entrystart, numentries, table.ordinal_base, file, fname );
            entrystart += numentries * sizeof( unsigned_16 );
            numentries = 1;
            namestart = *(curr - 1);
        }
        numentries++;
        curr++;
    }   /* NOTE! this assumes the name table is at the end */
    ReadExports( namestart, export_dir->size + *nameptrs, entrystart, numentries, table.ordinal_base, file, fname );
    MemFree( nameptrs );
}

static void CreateIDataSection( void )
/************************************/
{
    PEState         &st = state();
    segdata         *sdata;
    class_entry     *class_;
    byte            bits;

    PrepareToc();
    if( 0 != CalcIDataSize() ) {
        IDataGroup = GetGroup( IDataGrpName );
        bits = ( LinkState & LS_HAVE_X64_CODE ) ? BITS_64 : BITS_32;
        class_ = FindClass( Root, CoffIDataClassName, bits, false );
        class_->flags |= CLASS_IDATA | CLASS_LXDATA_SEEN;
        sdata = AllocSegData();
        sdata->align = ( LinkState & LS_HAVE_X64_CODE ) ? 4 : 2;
        sdata->bits = bits;
        sdata->length = st.IData.total_size;
        sdata->u.name.u.ptr = CoffIDataSegName;
        sdata->combine = COMBINE_ADD;
        sdata->isabs = false;
        AddSegment( sdata, class_ );
        sdata->u1.vm_ptr = AllocStg( sdata->length );
        st.IData.sdata = sdata;
        AddToGroup( IDataGroup, sdata->u.leader );
    }
}

static void RegisterImport( dll_sym_info *dll )
/*********************************************/
{
    PEState         &st = state();
    module_import   *mod;
    import_name     *imp;
    import_name     *chk;
    import_name     **owner;
    obj_name_list   *os2_imp;
    int             cmp;
    unsigned        len;

    for( mod = st.PEImpList; mod != nullptr; mod = mod->next ) {
        if( mod->mod == dll->m.modnum ) {
            break;
        }
    }
    if( mod == nullptr ) {
        ++st.NumMods;
        mod = static_cast<module_import *>( _PermAlloc( sizeof( module_import ) ) );
        mod->next = st.PEImpList;
        st.PEImpList = mod;
        mod->mod = dll->m.modnum;
        mod->imports = nullptr;
        mod->num_entries = 0;
    }
    if( !dll->isordinal ) {
        os2_imp = dll->u.entry;
    } else {
        os2_imp = nullptr;
    }
    mod->num_entries++;
    imp = static_cast<import_name *>( _PermAlloc( sizeof( import_name ) ) );
    imp->dll = dll;
    imp->imp = os2_imp;
    /* keep the list sorted by name for calculating hint values */
    owner = &mod->imports;
    if( os2_imp != nullptr ) {
        for( ;; ) {
            chk = *owner;
            if( chk == nullptr )
                break;
            if( chk->imp != nullptr ) {
                len = chk->imp->len;
                if( len > os2_imp->len )
                    len = os2_imp->len;
                cmp = memcmp( chk->imp->name.u.ptr, os2_imp->name.u.ptr, len );
                if( cmp > 0 )
                    break;
                if( cmp == 0
                  && len > chk->imp->len ) {
                    break;
                }
            }
            owner = &chk->next;
        }
    } else {
        chk = *owner;
    }
    imp->next = chk;
    *owner = imp;
    ++NumImports;
}

static void CreateTransferSegment( class_entry *class_ )
{
    PEState         &st = state();
    offset          size;
    int             glue_size;
    symbol          *sym;
    segdata         *sdata;

    size = 0;
    glue_size = GetTransferGlueSize();
    WALK_IMPORT_SYMBOLS( sym ) {
        size += glue_size;
        RegisterImport( sym->p.import_dll );
        DBIAddGlobal( sym );
    }
    size += st.NumLocalImports * sizeof( pe_va );
    if( size != 0 ) {
        class_->flags |= CLASS_TRANSFER;
        sdata = AllocSegData();
        sdata->align = ( LinkState & LS_HAVE_X64_CODE ) ? 4 : 2;
        sdata->bits = ( LinkState & LS_HAVE_X64_CODE ) ? BITS_64 : BITS_32;
        sdata->length = size;
        sdata->u.name.u.ptr = const_cast<char *>( TRANSFER_SEGNAME );
        sdata->combine = COMBINE_ADD;
        sdata->isabs = false;
        AddSegment( sdata, class_ );
        sdata->u1.vm_ptr = AllocStg( sdata->length );
        st.XFerSegData = sdata;
    }
}

void ChkPEData( void )
/********************/
{
    class_entry     *class_;

    /* find the last code class in the program */
    for( class_ = Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( class_->flags & CLASS_CODE ) {
            break;
        }
    }
    if( class_ == nullptr ) { // No code -- no need to do transfer stuff
        return;
    }
    CurrMod = FakeModule;
    CreateTransferSegment( class_ );
    CreateIDataSection();
    CurrMod = nullptr;
}

void AllocPETransferTable( void )
/*******************************/
{
    PEState         &st = state();
    symbol          *sym;
    class_entry     *class_;
    group_entry     *group;
    seg_leader      *lead;
    segdata         *piece;
    segdata         *save;
    offset          off;
    segment         seg;
    int             glue_size;
    local_import    *loc_imp;

    /*
     *  Moved export check here as otherwise flags don't get propagated
     */
    if( st.XFerSegData == nullptr ) {
        return;
    }
    for( class_ = Root->classes; class_ != nullptr; class_ = class_->next ) {
        if( class_->flags & CLASS_TRANSFER ) {
            break;
        }
    }
    if( class_ == nullptr ) {
        return;
    }
    lead = static_cast<seg_leader *>( RingLast( class_->segs ) );
    piece = static_cast<segdata *>( RingLast( lead->pieces ) );
    CurrMod = FakeModule;
    group = lead->group;
    seg = group->addr.seg;
    off = group->addr.off + group->totalsize;
    // now calc addresses for imported local symbols
    for( loc_imp = st.PELocalImpList; loc_imp != nullptr; loc_imp = loc_imp->next ) {
        off -= sizeof( pe_va );
        SET_SYM_ADDR( loc_imp->iatsym, off, seg );
    }
    if( IDataGroup != nullptr ) {
        glue_size = GetTransferGlueSize();
        WALK_IMPORT_SYMBOLS( sym ) {
            off -= glue_size;
            SET_SYM_ADDR( sym, off, seg );
            save = sym->p.seg;
            sym->p.seg = piece;
            DBIGenGlobal( sym, Root );
            sym->p.seg = save;
        }
        off = CalcIATAbsOffset();   // now calc addresses for IAT symbols
        WalkImportsMods( CalcImpOff, &off );
        SetTocAddr( st.IData.eof_ilt_off, IDataGroup ); // Set toc's address.
    }
    CurrMod = nullptr;
}

static constexpr std::size_t PREFIX_LEN = sizeof( ImportSymPrefix ) - 1;

void AddPEImportLocalSym( symbol *locsym, symbol *iatsym )
/********************************************************/
{
    PEState         &st = state();
    local_import    *imp;

    imp = static_cast<local_import *>( MemAllocSafe( sizeof( local_import ) ) );
    AppendList( reinterpret_cast<void **>( &st.PELocalImpList ), imp );
    imp->iatsym = iatsym;
    imp->locsym = locsym;
    ++st.NumLocalImports;
}

bool ImportPELocalSym( symbol *iatsym )
/*************************************/
{
    const char      *name;
    symbol          *sym;

    name = iatsym->name.u.ptr;
    if( memcmp( name, ImportSymPrefix, PREFIX_LEN ) != 0 )
        return( false );
    sym = FindISymbol( name + PREFIX_LEN );
    if( sym == nullptr )
        return( false );
    if( IS_SYM_IMPORTED( sym ) )
        return( false );
    LnkMsg( WRN+MSG_IMPORT_LOCAL, "s", sym->name.u.ptr );
    iatsym->info |= SYM_DEFINED | SYM_1_DCE_REF;
    if( LinkFlags & LF_STRIP_CODE ) {
        DefStripImpSym( iatsym );
    }
    AddPEImportLocalSym( sym, iatsym );
    return( true );
}

void FreePELocalImports( void )
/*****************************/
{
    PEState         &st = state();
    FreeList( st.PELocalImpList );
    st.PELocalImpList = nullptr;
    st.NumLocalImports = 0;
}

#endif
