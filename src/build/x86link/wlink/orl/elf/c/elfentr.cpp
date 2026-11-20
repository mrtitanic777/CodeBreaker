/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2025      The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "elfentr.h"
#include "elfload.h"
#include "elfflhn.h"
#include "elflwlv.h"
#include "elforl.h"
#include "orlhash.h"
#include "i64.h"

#include <cstring>


namespace {

// The relocation section of a program-bits section, its relocations built on first use.
// Returns ORL_OKAY with reloc_sec_hnd set, or the value the caller returns right away.
orl_return reloc_section_of( elf_sec_handle elf_sec_hnd, elf_sec_handle &reloc_sec_hnd )
{
    if( elf_sec_hnd->type != ORL_SEC_TYPE_PROG_BITS )
        return( ORL_ERROR );
    reloc_sec_hnd = elf_sec_hnd->assoc.normal.reloc_sec;
    if( reloc_sec_hnd == nullptr )
        return( ORL_FALSE );
    if( reloc_sec_hnd->assoc.reloc.relocs == nullptr ) {
        orl_return return_val = ElfCreateRelocs( elf_sec_hnd, reloc_sec_hnd );

        if( return_val != ORL_OKAY ) {
            return( return_val );
        }
    }
    return( ORL_OKAY );
}

// Hands the relocations of a relocation section to return_func (only those at one offset when only_offset is
// given). True when a callback stopped the scan: result is then its value. Otherwise result is the last
// callback's value (ORL_OKAY), or ORL_FALSE when none was called.
bool scan_relocs( elf_sec_handle reloc_sec_hnd, const elf_sec_offset *only_offset, orl_reloc_return_func return_func,
                  orl_return &result )
{
    orl_reloc       reloc = reloc_sec_hnd->assoc.reloc.relocs;

    result = ORL_FALSE;
    for( unsigned index = 0; index < U64Low( reloc_sec_hnd->size ); index += U64Low( reloc_sec_hnd->entsize ) ) {
        if( only_offset == nullptr || reloc->offset == *only_offset ) {
            result = return_func( reloc );
            if( result != ORL_OKAY ) {
                return( true );
            }
        }
        reloc++;
    }
    return( false );
}

}  // namespace

elf_handle ELFENTRY ElfInit( orl_funcs *funcs )
{
    elf_handle      elf_hnd = static_cast<elf_handle>( ORL_CLI_ALLOC( funcs, ORL_STRUCT_SIZEOF( elf_handle ) ) );

    if( elf_hnd != nullptr ) {
        elf_hnd->funcs = funcs;
        elf_hnd->first_file_hnd = nullptr;
    }
    return( elf_hnd );
}

orl_return ELFENTRY ElfFini( elf_handle elf_hnd )
{
    while( elf_hnd->first_file_hnd != nullptr ) {
        orl_return return_val = ElfRemoveFileLinks( elf_hnd->first_file_hnd );

        if( return_val != ORL_OKAY ) {
            return( return_val );
        }
    }
    ORL_PTR_FREE( elf_hnd, elf_hnd );
    return( ORL_OKAY );
}

orl_return ELFENTRY ElfFileInit( elf_handle elf_hnd, orl_io_handle io_hnd, elf_file_handle *pefh )
{
    elf_file_handle     elf_file_hnd;
    orl_return          return_val;

    elf_file_hnd = static_cast<elf_file_handle>( ORL_PTR_ALLOC( elf_hnd, ORL_STRUCT_SIZEOF( elf_file_handle ) ) );
    if( elf_file_hnd == nullptr ) {
        return( ORL_OUT_OF_MEMORY );
    }
    elf_file_hnd->sec_handles = nullptr;
    elf_file_hnd->io_hnd = io_hnd;
    elf_file_hnd->sec_name_hash_table = nullptr;
    ElfAddFileLinks( elf_hnd, elf_file_hnd );
    return_val = ElfLoadFileStructure( elf_file_hnd );
    if( return_val != ORL_OKAY ) {
        ElfRemoveFileLinks( elf_file_hnd );
        elf_file_hnd = nullptr;
    }
    *pefh = elf_file_hnd;
    return( return_val );
}

orl_return ELFENTRY ElfFileFini( elf_file_handle elf_file_hnd )
{
    return( ElfRemoveFileLinks( elf_file_hnd ) );
}

orl_return ELFENTRY ElfFileScan( elf_file_handle elf_file_hnd, const char *desired, orl_sec_return_func return_func )
{
    orl_return      return_val;

    if( desired == nullptr ) {
        /* global request */
        for( elf_quantity i = 0; i < elf_file_hnd->num_sections; ++i ) {
            return_val = return_func( reinterpret_cast<orl_sec_handle>( elf_file_hnd->sec_handles[i] ) );
            if( return_val != ORL_OKAY ) {
                return( return_val );
            }
        }
    } else {
        orl_hash_key        h_key;

        if( elf_file_hnd->sec_name_hash_table == nullptr ) {
            return_val = ElfBuildSecNameHashTable( elf_file_hnd );
            if( return_val != ORL_OKAY ) {
                return( return_val );
            }
        }
        h_key.u.string = desired;
        for( orl_hash_data_entry data_entry = ORLHashTableQuery( elf_file_hnd->sec_name_hash_table, h_key );
             data_entry != nullptr; data_entry = data_entry->next ) {
            return_val = return_func( data_entry->data.u.sec_handle );
            if( return_val != ORL_OKAY ) {
                return( return_val );
            }
        }
    }
    return( ORL_OKAY );
}

orl_machine_type ELFENTRY ElfFileGetMachineType( elf_file_handle elf_file_hnd )
{
    return( elf_file_hnd->machine_type );
}

orl_file_flags ELFENTRY ElfFileGetFlags( elf_file_handle elf_file_hnd )
{
    return( elf_file_hnd->flags );
}

orl_file_type ELFENTRY ElfFileGetType( elf_file_handle elf_file_hnd )
{
    return( elf_file_hnd->type );
}

orl_file_size ELFENTRY ElfFileGetSize( elf_file_handle elf_file_hnd )
{
    return( elf_file_hnd->size );
}

elf_sec_handle ELFENTRY ElfFileGetSymbolTable( elf_file_handle elf_file_hnd )
{
    return( elf_file_hnd->symbol_table );
}

const char * ELFENTRY ElfSecGetName( elf_sec_handle elf_sec_hnd )
{
    return( elf_sec_hnd->name );
}

orl_return ELFENTRY ElfSecGetBase( elf_sec_handle elf_sec_hnd, orl_sec_base *sec_base )
{
    U64High( *sec_base ) = U64High( elf_sec_hnd->base );
    U64Low( *sec_base ) = U64Low( elf_sec_hnd->base );
    return( ORL_OKAY );
}

orl_sec_size ELFENTRY ElfSecGetSize( elf_sec_handle elf_sec_hnd )
{
    return( U64Low( elf_sec_hnd->size ) );
}

orl_sec_type ELFENTRY ElfSecGetType( elf_sec_handle elf_sec_hnd )
{
    return( elf_sec_hnd->type );
}

orl_sec_flags ELFENTRY ElfSecGetFlags( elf_sec_handle elf_sec_hnd )
{
    return( elf_sec_hnd->flags );
}

orl_sec_alignment ELFENTRY ElfSecGetAlignment( elf_sec_handle elf_sec_hnd )
{
    return( elf_sec_hnd->alignment );
}

elf_sec_handle ELFENTRY ElfSecGetStringTable( elf_sec_handle elf_sec_hnd )
{
    switch( elf_sec_hnd->type ) {
    case ORL_SEC_TYPE_SYM_TABLE:
    case ORL_SEC_TYPE_DYN_SYM_TABLE:
        return( elf_sec_hnd->assoc.sym.string_table );
    case ORL_SEC_TYPE_IMPORT:
        return( elf_sec_hnd->assoc.import.string_table );
    default:
        return( nullptr );
    }
}

elf_sec_handle ELFENTRY ElfSecGetSymbolTable( elf_sec_handle elf_sec_hnd )
{
    switch( elf_sec_hnd->type ) {
    case ORL_SEC_TYPE_RELOCS:
    case ORL_SEC_TYPE_RELOCS_EXPADD:
        return( elf_sec_hnd->assoc.reloc.symbol_table );
    case ORL_SEC_TYPE_EXPORT:
        return( elf_sec_hnd->assoc.export_.symbol_table );
    default:
        return( nullptr );
    }
}

elf_sec_handle ELFENTRY ElfSecGetRelocTable( elf_sec_handle elf_sec_hnd )
{
    switch( elf_sec_hnd->type ) {
    case ORL_SEC_TYPE_RELOCS:
    case ORL_SEC_TYPE_RELOCS_EXPADD:
    case ORL_SEC_TYPE_SYM_TABLE:
    case ORL_SEC_TYPE_DYN_SYM_TABLE:
    case ORL_SEC_TYPE_IMPORT:
    case ORL_SEC_TYPE_EXPORT:
        return( nullptr );
    default:
        return( elf_sec_hnd->assoc.normal.reloc_sec );
    }
}

orl_return ELFENTRY ElfSecGetContents( elf_sec_handle elf_sec_hnd, unsigned char **buffer )
{
    if( elf_sec_hnd->contents != nullptr ) {
        *buffer = elf_sec_hnd->contents;
        return( ORL_OKAY );
    }
    return( ORL_ERROR );
}

orl_return ELFENTRY ElfSecQueryReloc( elf_sec_handle elf_sec_hnd, elf_sec_offset sec_offset, orl_reloc_return_func return_func )
{
    elf_sec_handle  reloc_sec_hnd;
    orl_return      return_val = reloc_section_of( elf_sec_hnd, reloc_sec_hnd );

    if( return_val != ORL_OKAY )
        return( return_val );
    scan_relocs( reloc_sec_hnd, &sec_offset, return_func, return_val );
    return( return_val );
}

orl_return ELFENTRY ElfSecScanReloc( elf_sec_handle elf_sec_hnd, orl_reloc_return_func return_func )
{
    elf_sec_handle  reloc_sec_hnd;
    orl_return      return_val = reloc_section_of( elf_sec_hnd, reloc_sec_hnd );

    if( return_val != ORL_OKAY )
        return( return_val );
    if( scan_relocs( reloc_sec_hnd, nullptr, return_func, return_val ) )
        return( return_val );
    return( ORL_TRUE );
}

// One of these should be implimented O(1), but the section handles
// get reordered in ElfLoad.
orl_table_index ELFENTRY ElfCvtSecHdlToIdx( elf_sec_handle elf_sec_hnd )
{
    elf_file_handle     fhdl = elf_sec_hnd->elf_file_hnd;
    elf_quantity        limit = fhdl->num_sections;

    for( elf_quantity index = 0; index < limit; index++ ) {
        if( fhdl->sec_handles[index] == elf_sec_hnd ) {
            return( fhdl->sec_handles[index]->index );
        }
    }
    return( 0 );
}

elf_sec_handle ELFENTRY ElfCvtIdxToSecHdl( elf_file_handle fhdl, orl_table_index idx )
{
    elf_quantity        limit = fhdl->num_sections;

    for( elf_quantity index = 0; index < limit; index++ ) {
        if( fhdl->sec_handles[index]->index == idx ) {
            return( fhdl->sec_handles[index] );
        }
    }
    return( nullptr );
}

orl_return ELFENTRY ElfRelocSecScan( elf_sec_handle elf_sec_hnd, orl_reloc_return_func return_func )
{
    orl_return      return_val;

    if( elf_sec_hnd->type != ORL_SEC_TYPE_RELOCS && elf_sec_hnd->type != ORL_SEC_TYPE_RELOCS_EXPADD )
        return( ORL_ERROR );
    if( elf_sec_hnd->assoc.reloc.relocs == nullptr ) {
        return_val = ElfCreateRelocs( elf_sec_hnd->assoc.reloc.orig_sec, elf_sec_hnd );
        if( return_val != ORL_OKAY ) {
            return( return_val );
        }
    }
    if( scan_relocs( elf_sec_hnd, nullptr, return_func, return_val ) )
        return( return_val );
    return( ORL_TRUE );
}

orl_return ELFENTRY ElfSymbolSecScan( elf_sec_handle elf_sec_hnd, orl_symbol_return_func return_func )
{
    orl_return              return_val;
    elf_symbol_handle       elf_symbol_hnd;

    switch( elf_sec_hnd->type ) {
    case ORL_SEC_TYPE_SYM_TABLE:
    case ORL_SEC_TYPE_DYN_SYM_TABLE:
        if( elf_sec_hnd->assoc.sym.symbols == nullptr ) {
            return_val = ElfCreateSymbolHandles( elf_sec_hnd );
            if( return_val != ORL_OKAY ) {
                return( return_val );
            }
        }
        elf_symbol_hnd = elf_sec_hnd->assoc.sym.symbols;
        break;
    default:
        return( ORL_ERROR );
    }
    for( unsigned index = 0; index < U64Low( elf_sec_hnd->size ); index += U64Low( elf_sec_hnd->entsize ) ) {
        return_val = return_func( reinterpret_cast<orl_symbol_handle>( elf_symbol_hnd ) );
        if( return_val != ORL_OKAY )
            return( return_val );
        elf_symbol_hnd++;
    }
    return( ORL_OKAY );
}

orl_return ELFENTRY ElfNoteSecScan( elf_sec_handle elf_sec_hnd, orl_note_callbacks *cbs, void *cookie )
/*****************************************************************************************************/
{
    if( elf_sec_hnd->type != ORL_SEC_TYPE_NOTE )
        return( ORL_ERROR );
    if( std::strcmp( elf_sec_hnd->name, ".drectve" ) != 0 )
        return( ORL_OKAY );
    if( U64Low( elf_sec_hnd->size ) == 0 )
        return( ORL_OKAY );
    return( ElfParseDrectve( reinterpret_cast<const char *>( elf_sec_hnd->contents ), U64Low( elf_sec_hnd->size ), cbs, cookie ) );
}

const char * ELFENTRY ElfSymbolGetName( elf_symbol_handle elf_symbol_hnd )
{
    return( elf_symbol_hnd->name );
}

orl_return ELFENTRY ElfSymbolGetValue( elf_symbol_handle elf_symbol_hnd, orl_symbol_value *sym_value )
{
    U64High( *sym_value ) = U64High( elf_symbol_hnd->value );
    U64Low( *sym_value ) = U64Low( elf_symbol_hnd->value );
    return( ORL_OKAY );
}

orl_symbol_binding ELFENTRY ElfSymbolGetBinding( elf_symbol_handle elf_symbol_hnd )
{
    return( elf_symbol_hnd->binding );
}

orl_symbol_type ELFENTRY ElfSymbolGetType( elf_symbol_handle elf_symbol_hnd )
{
    return( elf_symbol_hnd->type );
}

unsigned char ELFENTRY ElfSymbolGetRawInfo( elf_symbol_handle elf_symbol_hnd )
{
    return( elf_symbol_hnd->info );
}

elf_sec_handle ELFENTRY ElfSymbolGetSecHandle( elf_symbol_handle elf_symbol_hnd )
{
    switch( elf_symbol_hnd->shndx ) {
    case SHN_ABS:
    case SHN_COMMON:
    case SHN_UNDEF:
        return( nullptr );
    default:
        return( elf_symbol_hnd->elf_file_hnd->orig_sec_handles[elf_symbol_hnd->shndx - 1] );
    }
}

elf_symbol_handle ELFENTRY ElfSymbolGetAssociated( elf_symbol_handle elf_symbol_hnd )
{
    return( elf_symbol_hnd );
}
