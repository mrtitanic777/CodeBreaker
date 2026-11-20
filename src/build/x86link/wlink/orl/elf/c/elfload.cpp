/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2025-2026 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Top level ELF loading code, parses ELF headers.
*
****************************************************************************/


#include "elfload.h"
#include "elforl.h"
#include "i64.h"

#include <algorithm>
#include <bit>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <vector>


// fixme: finish making ELF SPECIFIC (see next fixme)

namespace {

// Puts the fields of a header read from the file into host byte order, the file's byte order
// being big or little endian. One of the two branches is always compiled out for a given host.
class ByteOrder {
public:
    explicit ByteOrder( elf_file_handle elf_file_hnd )
        : big( ( elf_file_hnd->flags & ORL_FILE_FLAG_BIG_ENDIAN ) != 0 ) {}

    template<class T>
    void operator()( T &field ) const
    {
        if constexpr( requires { field.u._64; } ) {
            if( big ) {
                SCONV_BE_64( field );
            } else {
                SCONV_LE_64( field );
            }
        } else if constexpr( sizeof( T ) == 2 ) {
            if( big ) {
                CONV_BE_16( field );
            } else {
                CONV_LE_16( field );
            }
        } else {
            static_assert( sizeof( T ) == 4 );
            if( big ) {
                CONV_BE_32( field );
            } else {
                CONV_LE_32( field );
            }
        }
    }

private:
    bool    big;
};

// What the loader takes out of a file header, whichever its class.
struct HeaderFields {
    elf_file_offset shoff;
    elf_half        shnum;
    elf_half        ehsize;
    elf_half        shstrndx;
};

// What it takes out of a section header, whichever its class.
struct SectionFields {
    elf_index       name;
    elf_sec_size    size;
    elf_sec_base    base;
    elf_file_offset file_offset;
    elf_sec_size    entsize;
    unsigned_32     align;
    unsigned_32     flags;
    elf_word        type;
    elf_index       link;
    elf_index       info;
};

void fix_ehdr_byte_order( elf_file_handle elf_file_hnd, Elf32_Ehdr *e_hdr )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_hdr->e_type );
    fix( e_hdr->e_machine );
    fix( e_hdr->e_version );
    fix( e_hdr->e_entry );
    fix( e_hdr->e_phoff );
    fix( e_hdr->e_shoff );
    fix( e_hdr->e_flags );
    fix( e_hdr->e_ehsize );
    fix( e_hdr->e_phentsize );
    fix( e_hdr->e_phnum );
    fix( e_hdr->e_shentsize );
    fix( e_hdr->e_shnum );
    fix( e_hdr->e_shstrndx );
}

void fix_ehdr64_byte_order( elf_file_handle elf_file_hnd, Elf64_Ehdr *e_hdr )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_hdr->e_type );
    fix( e_hdr->e_machine );
    fix( e_hdr->e_version );
    fix( e_hdr->e_entry );
    fix( e_hdr->e_phoff );
    fix( e_hdr->e_shoff );
    fix( e_hdr->e_flags );
    fix( e_hdr->e_ehsize );
    fix( e_hdr->e_phentsize );
    fix( e_hdr->e_phnum );
    fix( e_hdr->e_shentsize );
    fix( e_hdr->e_shnum );
    fix( e_hdr->e_shstrndx );
}

void fix_shdr_byte_order( elf_file_handle elf_file_hnd, Elf32_Shdr *e_shdr )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_shdr->sh_name );
    fix( e_shdr->sh_type );
    fix( e_shdr->sh_flags );
    fix( e_shdr->sh_addr );
    fix( e_shdr->sh_offset );
    fix( e_shdr->sh_size );
    fix( e_shdr->sh_link );
    fix( e_shdr->sh_info );
    fix( e_shdr->sh_addralign );
    fix( e_shdr->sh_entsize );
}

void fix_shdr64_byte_order( elf_file_handle elf_file_hnd, Elf64_Shdr *e_shdr )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_shdr->sh_name );
    fix( e_shdr->sh_type );
    fix( e_shdr->sh_flags );
    fix( e_shdr->sh_addr );
    fix( e_shdr->sh_offset );
    fix( e_shdr->sh_size );
    fix( e_shdr->sh_link );
    fix( e_shdr->sh_info );
    fix( e_shdr->sh_addralign );
    fix( e_shdr->sh_entsize );
}

// This function needs to be run before anything else, because it determines
// how the rest of the data is to be interpreted
void determine_file_class( elf_file_handle elf_file_hnd, const Elf32_Ehdr *e_hdr )
{
    elf_file_hnd->flags = ORL_FILE_FLAG_NONE;
    switch( e_hdr->e_ident[EI_DATA] ) {
    case ELFLITTLEENDIAN:
        elf_file_hnd->flags |= ORL_FILE_FLAG_LITTLE_ENDIAN;
        break;
    case ELFBIGENDIAN:
        elf_file_hnd->flags |= ORL_FILE_FLAG_BIG_ENDIAN;
        break;
    }
    switch( e_hdr->e_ident[EI_CLASS] ) {
    case ELFCLASS32:
        elf_file_hnd->flags |= ORL_FILE_FLAG_32BIT_MACHINE;
        break;
    case ELFCLASS64:
        elf_file_hnd->flags |= ORL_FILE_FLAG_64BIT_MACHINE;
        break;
    }
}

struct MachineEntry {
    elf_half            e_machine;
    orl_machine_type    machine_type;
};

constexpr MachineEntry machine_types[] = {
    { EM_M32,           ORL_MACHINE_TYPE_WE_2100 },
    { EM_SPARC,         ORL_MACHINE_TYPE_SPARC },
    { EM_SPARC32PLUS,   ORL_MACHINE_TYPE_SPARCPLUS },
    { EM_386,           ORL_MACHINE_TYPE_I386 },
    { EM_X86_64,        ORL_MACHINE_TYPE_AMD64 },
    { EM_68K,           ORL_MACHINE_TYPE_M68000 },
    { EM_88K,           ORL_MACHINE_TYPE_M88000 },
    { EM_860,           ORL_MACHINE_TYPE_I860 },
    { EM_PPC,           ORL_MACHINE_TYPE_PPC601 },
    { EM_MIPS,          ORL_MACHINE_TYPE_R3000 },
    { EM_ALPHA,         ORL_MACHINE_TYPE_ALPHA },
};

struct FileTypeEntry {
    elf_half        e_type;
    orl_file_type   file_type;
};

constexpr FileTypeEntry file_types[] = {
    { ET_REL,   ORL_FILE_TYPE_OBJECT },
    { ET_EXEC,  ORL_FILE_TYPE_EXECUTABLE },
    { ET_DYN,   ORL_FILE_TYPE_SHARED_OBJECT },
    { ET_CORE,  ORL_FILE_TYPE_CORE },
};

void determine_file_specs( elf_file_handle elf_file_hnd, elf_half e_machine, elf_half e_type )
{
    const auto machine = std::find_if( std::begin( machine_types ), std::end( machine_types ),
                                       [e_machine]( const MachineEntry &e ) { return e.e_machine == e_machine; } );
    elf_file_hnd->machine_type = ( machine != std::end( machine_types ) ) ? machine->machine_type : ORL_MACHINE_TYPE_NONE;

    const auto type = std::find_if( std::begin( file_types ), std::end( file_types ),
                                    [e_type]( const FileTypeEntry &e ) { return e.e_type == e_type; } );
    elf_file_hnd->type = ( type != std::end( file_types ) ) ? type->file_type : ORL_FILE_TYPE_NONE;
// fixme:  add computer-specific (PPC) flags, if any!
}

struct SectionTypeEntry {
    elf_word        sh_type;
    orl_sec_type    type;
    orl_sec_flags   flags;      // flags a section of this type gets besides read permission
};

constexpr SectionTypeEntry section_types[] = {
    { SHT_PROGBITS, ORL_SEC_TYPE_PROG_BITS,     ORL_SEC_FLAG_INITIALIZED_DATA },
    { SHT_SYMTAB,   ORL_SEC_TYPE_SYM_TABLE,     ORL_SEC_FLAG_NONE },
    { SHT_STRTAB,   ORL_SEC_TYPE_STR_TABLE,     ORL_SEC_FLAG_NONE },
    { SHT_RELA,     ORL_SEC_TYPE_RELOCS_EXPADD, ORL_SEC_FLAG_NONE },
    { SHT_HASH,     ORL_SEC_TYPE_HASH,          ORL_SEC_FLAG_NONE },
    { SHT_DYNAMIC,  ORL_SEC_TYPE_DYNAMIC,       ORL_SEC_FLAG_NONE },
    { SHT_NOTE,     ORL_SEC_TYPE_NOTE,          ORL_SEC_FLAG_NONE },
    { SHT_NOBITS,   ORL_SEC_TYPE_NO_BITS,       ORL_SEC_FLAG_UNINITIALIZED_DATA },
    { SHT_REL,      ORL_SEC_TYPE_RELOCS,        ORL_SEC_FLAG_NONE },
    { SHT_DYNSYM,   ORL_SEC_TYPE_DYN_SYM_TABLE, ORL_SEC_FLAG_NONE },
    { SHT_OS,       ORL_SEC_TYPE_PROG_BITS,     ORL_SEC_FLAG_NONE },
    { SHT_OS_O,     ORL_SEC_TYPE_PROG_BITS,     ORL_SEC_FLAG_NONE },
    { SHT_IMPORTS,  ORL_SEC_TYPE_IMPORT,        ORL_SEC_FLAG_NONE },
    { SHT_IMPORTS_O, ORL_SEC_TYPE_IMPORT,       ORL_SEC_FLAG_NONE },
    { SHT_EXPORTS,  ORL_SEC_TYPE_EXPORT,        ORL_SEC_FLAG_NONE },
    { SHT_EXPORTS_O, ORL_SEC_TYPE_EXPORT,       ORL_SEC_FLAG_NONE },
    { SHT_RES,      ORL_SEC_TYPE_PROG_BITS,     ORL_SEC_FLAG_NONE },
    { SHT_RES_O,    ORL_SEC_TYPE_PROG_BITS,     ORL_SEC_FLAG_NONE },
};

void determine_section_specs( elf_sec_handle elf_sec_hnd, elf_word sh_type, unsigned_32 sh_flags, bool is64bit )
{
    const auto entry = std::find_if( std::begin( section_types ), std::end( section_types ),
                                     [sh_type]( const SectionTypeEntry &e ) { return e.sh_type == sh_type; } );

    elf_sec_hnd->flags = ORL_SEC_FLAG_READ_PERMISSION;
    if( entry != std::end( section_types ) ) {
        elf_sec_hnd->type = entry->type;
        elf_sec_hnd->flags |= entry->flags;
    } else {
        elf_sec_hnd->type = ORL_SEC_TYPE_NONE;
    }
    if( sh_flags & SHF_WRITE ) {
        elf_sec_hnd->flags |= ORL_SEC_FLAG_WRITE_PERMISSION;
    }
    if( (sh_flags & SHF_ALLOC) == 0 ) {
        elf_sec_hnd->flags |= ORL_SEC_FLAG_REMOVE;
    }
    if( sh_flags & SHF_EXECINSTR ) {
        // set execute permission also?
        elf_sec_hnd->flags |= ORL_SEC_FLAG_EXEC;
    }
    if( is64bit ) {
        elf_sec_hnd->flags |= ORL_SEC_FLAG_USE_64;
    } else {
        elf_sec_hnd->flags |= ORL_SEC_FLAG_USE_32;
    }
}

// The section handles (and their arrays) made before an allocation failed.
void free_elf_sec_handles( elf_file_handle elf_file_hnd, elf_quantity num_alloced )
{
    if( elf_file_hnd->sec_handles != nullptr ) {
        for( elf_quantity i = 0; i < num_alloced; i++ ) {
            _ClientFree( elf_file_hnd, elf_file_hnd->sec_handles[i] );
        }
        _ClientFree( elf_file_hnd, elf_file_hnd->sec_handles );
    } else {
        for( elf_quantity i = 0; i < num_alloced; i++ ) {
            _ClientFree( elf_file_hnd, elf_file_hnd->orig_sec_handles[i] );
        }
    }
    _ClientFree( elf_file_hnd, elf_file_hnd->orig_sec_handles );
}

// The log2 of a section alignment the format can express (a power of two up to 256; 0 and 1 are both 0).
// False for any other value.
bool alignment_exponent( unsigned_32 sh_align, orl_sec_alignment &alignment )
{
    if( sh_align <= 1 ) {
        alignment = 0;
    } else if( sh_align <= 256 && std::has_single_bit( sh_align ) ) {
        alignment = static_cast<orl_sec_alignment>( std::countr_zero( sh_align ) );
    } else {
        return( false );
    }
    return( true );
}

// Reads the header at s_hdr (byte order fixed in place) whichever its class.
SectionFields read_section_header( elf_file_handle elf_file_hnd, char *s_hdr, bool is64bit )
{
    SectionFields   f;

    if( is64bit ) {
        Elf64_Shdr  *s_hdr64 = reinterpret_cast<Elf64_Shdr *>( s_hdr );

        fix_shdr64_byte_order( elf_file_hnd, s_hdr64 );
        f.name = s_hdr64->sh_name;
        U64High( f.size ) = U64High( s_hdr64->sh_size );
        U64Low( f.size ) = U64Low( s_hdr64->sh_size );
        U64High( f.base ) = U64High( s_hdr64->sh_addr );
        U64Low( f.base ) = U64Low( s_hdr64->sh_addr );
        U64High( f.file_offset ) = U64High( s_hdr64->sh_offset );
        U64Low( f.file_offset ) = U64Low( s_hdr64->sh_offset );
        U64High( f.entsize ) = U64High( s_hdr64->sh_entsize );
        U64Low( f.entsize ) = U64Low( s_hdr64->sh_entsize );
        f.align = U64Low( s_hdr64->sh_addralign );
        f.flags = U64Low( s_hdr64->sh_flags );
        f.type = s_hdr64->sh_type;
        f.link = s_hdr64->sh_link;
        f.info = s_hdr64->sh_info;
    } else {
        Elf32_Shdr  *s_hdr32 = reinterpret_cast<Elf32_Shdr *>( s_hdr );

        fix_shdr_byte_order( elf_file_hnd, s_hdr32 );
        f.name = s_hdr32->sh_name;
        Set64ValU32( f.size, s_hdr32->sh_size );
        Set64ValU32( f.base, s_hdr32->sh_addr );
        Set64ValU32( f.file_offset, s_hdr32->sh_offset );
        Set64ValU32( f.entsize, s_hdr32->sh_entsize );
        f.align = s_hdr32->sh_addralign;
        f.flags = s_hdr32->sh_flags;
        f.type = s_hdr32->sh_type;
        f.link = s_hdr32->sh_link;
        f.info = s_hdr32->sh_info;
    }
    return( f );
}

constexpr elf_index no_section = static_cast<elf_index>( -1 );

orl_return load_elf_sec_handles( elf_file_handle elf_file_hnd, elf_index *name_index, char *s_hdr )
{
    const elf_quantity  num_sections = elf_file_hnd->num_sections;
    // the sections each one refers to (a symbol table's string table, a relocation section's
    // section and symbol table), as indexes until all are made
    std::vector<elf_index>  associated_index( num_sections );
    std::vector<elf_index>  associated2_index( num_sections );
    const bool              is64bit = ( (elf_file_hnd->flags & ORL_FILE_FLAG_64BIT_MACHINE) != 0 );

    elf_file_hnd->symbol_table = nullptr;
    elf_file_hnd->sec_handles = nullptr;
    elf_file_hnd->orig_sec_handles = static_cast<elf_sec_handle *>( _ClientAlloc( elf_file_hnd, sizeof( elf_sec_handle ) * num_sections ) );
    if( elf_file_hnd->orig_sec_handles == nullptr ) {
        return( ORL_OUT_OF_MEMORY );
    }
    s_hdr += elf_file_hnd->shentsize; // skip over index 0
    for( elf_quantity i = 0; i < num_sections; ++i ) {
        elf_sec_handle elf_sec_hnd = static_cast<elf_sec_handle>( _ClientAlloc( elf_file_hnd, ORL_STRUCT_SIZEOF( elf_sec_handle ) ) );

        if( elf_sec_hnd == nullptr ) {
            free_elf_sec_handles( elf_file_hnd, i );
            _ClientFree( elf_file_hnd, elf_file_hnd->orig_sec_handles );
            return( ORL_OUT_OF_MEMORY );
        }
        elf_file_hnd->orig_sec_handles[i] = elf_sec_hnd;
        elf_sec_hnd->file_format = ORL_ELF;
        elf_sec_hnd->elf_file_hnd = elf_file_hnd;
        elf_sec_hnd->index = i + 1;

        const SectionFields f = read_section_header( elf_file_hnd, s_hdr, is64bit );

        name_index[i] = f.name;
        elf_sec_hnd->size = f.size;
        elf_sec_hnd->base = f.base;
        elf_sec_hnd->file_offset = f.file_offset;
        elf_sec_hnd->entsize = f.entsize;
        alignment_exponent( f.align, elf_sec_hnd->alignment );
        elf_sec_hnd->contents = nullptr;
        std::memset( &(elf_sec_hnd->assoc), '\0', sizeof( elf_sec_hnd->assoc ) );
        determine_section_specs( elf_sec_hnd, f.type, f.flags, is64bit );
        switch( elf_sec_hnd->type ) {
        case ORL_SEC_TYPE_SYM_TABLE:
            elf_file_hnd->symbol_table = elf_sec_hnd;
            /* fall through */
        case ORL_SEC_TYPE_DYN_SYM_TABLE:
        case ORL_SEC_TYPE_IMPORT:
        case ORL_SEC_TYPE_EXPORT:
            associated_index[i] = f.link - 1;
            break;
        case ORL_SEC_TYPE_RELOCS:
        case ORL_SEC_TYPE_RELOCS_EXPADD:
            // Certain funky toolchains produce two reloc sections for each
            // section containing relocations (both .rel and .rela) and one of
            // them is empty. We have to ignore the empty one!
            if( U64isntZero( elf_sec_hnd->size ) ) {
                associated_index[i] = f.info - 1;
                associated2_index[i] = f.link - 1;
            } else {
                associated_index[i] = no_section;
                associated2_index[i] = no_section;
            }
            break;
        default:
            break;
        }
        s_hdr += elf_file_hnd->shentsize;
    }
    for( elf_quantity i = 0; i < num_sections; ++i ) {
        elf_sec_handle elf_sec_hnd = elf_file_hnd->orig_sec_handles[i];

        switch( elf_sec_hnd->type ) {
        case ORL_SEC_TYPE_SYM_TABLE:
        case ORL_SEC_TYPE_DYN_SYM_TABLE:
            elf_sec_hnd->assoc.sym.string_table = elf_file_hnd->orig_sec_handles[associated_index[i]];
            break;
        case ORL_SEC_TYPE_IMPORT:
            elf_sec_hnd->assoc.import.string_table = elf_file_hnd->orig_sec_handles[associated_index[i]];
            break;
        case ORL_SEC_TYPE_EXPORT:
            elf_sec_hnd->assoc.export_.symbol_table = elf_file_hnd->orig_sec_handles[associated_index[i]];
            break;
        case ORL_SEC_TYPE_RELOCS:
        case ORL_SEC_TYPE_RELOCS_EXPADD:
            elf_sec_hnd->assoc.reloc.symbol_table = elf_file_hnd->orig_sec_handles[associated2_index[i]];
            // some silly people create reloc sections with no associated section
            if( associated_index[i] != no_section ) {
                elf_sec_hnd->assoc.reloc.orig_sec = elf_file_hnd->orig_sec_handles[associated_index[i]];
                elf_file_hnd->orig_sec_handles[associated_index[i]]->assoc.normal.reloc_sec = elf_sec_hnd;
            }
            break;
        default:
            break;
        }
    }
    elf_file_hnd->sec_handles = static_cast<elf_sec_handle *>( _ClientAlloc( elf_file_hnd, sizeof( elf_sec_handle ) * num_sections ) );
    if( elf_file_hnd->sec_handles == nullptr ) {
        _ClientFree( elf_file_hnd, elf_file_hnd->orig_sec_handles );
        return( ORL_OUT_OF_MEMORY );
    }
    std::memcpy( elf_file_hnd->sec_handles, elf_file_hnd->orig_sec_handles, sizeof( elf_sec_handle ) * num_sections );
    return( ORL_OKAY );
}

// Orders section handles by their offset in the file.
int section_compare( const void *sec1, const void *sec2 )
{
    const elf_file_offset   &offset1 = (*static_cast<const elf_sec_handle *>( sec1 ))->file_offset;
    const elf_file_offset   &offset2 = (*static_cast<const elf_sec_handle *>( sec2 ))->file_offset;

    if( U64High( offset1 ) > U64High( offset2 ) )
        return( 1 );
    if( U64High( offset1 ) < U64High( offset2 ) )
        return( -1 );
    if( U64Low( offset1 ) > U64Low( offset2 ) )
        return( 1 );
    if( U64Low( offset1 ) < U64Low( offset2 ) )
        return( -1 );
    return( 0 );
}

}  // namespace

orl_return ElfLoadFileStructure( elf_file_handle elf_file_hnd )
{
    orl_return          return_val;
    char                *s_hdr;
    Elf32_Ehdr          *e_hdr32;
    Elf64_Ehdr          *e_hdr64;
    elf_sec_handle      elf_sec_hnd;
    elf_quantity        contents_size1;
    elf_quantity        contents_size2;
    elf_quantity        sec_header_table_size;
    HeaderFields        hdr;

    e_hdr32 = static_cast<Elf32_Ehdr *>( _ClientRead( elf_file_hnd, sizeof( e_hdr32->e_ident ) ) );
    if( e_hdr32 == nullptr )
        return( ORL_ERROR );
    determine_file_class( elf_file_hnd, e_hdr32 );
    _ClientSeek( elf_file_hnd, 0, SEEK_SET );
    if( elf_file_hnd->flags & ORL_FILE_FLAG_64BIT_MACHINE ) {
        e_hdr64 = static_cast<Elf64_Ehdr *>( _ClientRead( elf_file_hnd, sizeof( Elf64_Ehdr ) ) );
        if( e_hdr64 == nullptr )
            return( ORL_ERROR );
        fix_ehdr64_byte_order( elf_file_hnd, e_hdr64 );
        U64High( hdr.shoff ) = U64High( e_hdr64->e_shoff );
        U64Low( hdr.shoff ) = U64Low( e_hdr64->e_shoff );
        hdr.shnum = e_hdr64->e_shnum;
        hdr.ehsize = e_hdr64->e_ehsize;
        hdr.shstrndx = e_hdr64->e_shstrndx;
        elf_file_hnd->shentsize = e_hdr64->e_shentsize;
        determine_file_specs( elf_file_hnd, e_hdr64->e_machine, e_hdr64->e_type );
    } else {
        e_hdr32 = static_cast<Elf32_Ehdr *>( _ClientRead( elf_file_hnd, sizeof( Elf32_Ehdr ) ) );
        if( e_hdr32 == nullptr )
            return( ORL_ERROR );
        fix_ehdr_byte_order( elf_file_hnd, e_hdr32 );
        Set64ValU32( hdr.shoff, e_hdr32->e_shoff );
        hdr.shnum = e_hdr32->e_shnum;
        hdr.ehsize = e_hdr32->e_ehsize;
        hdr.shstrndx = e_hdr32->e_shstrndx;
        elf_file_hnd->shentsize = e_hdr32->e_shentsize;
        determine_file_specs( elf_file_hnd, e_hdr32->e_machine, e_hdr32->e_type );
    }
    elf_file_hnd->num_sections = hdr.shnum - 1; // -1 to ignore shdr table index

    contents_size1 = U64Low( hdr.shoff ) - hdr.ehsize;
    sec_header_table_size = elf_file_hnd->shentsize * hdr.shnum;

    // e_ehsize might not be the same as sizeof( Elf32_Ehdr ) (different versions)
    _ClientSeek( elf_file_hnd, hdr.ehsize, SEEK_SET );

    if( contents_size1 > 0 ) {
        elf_file_hnd->contents_buffer1 = static_cast<unsigned_8 *>( _ClientRead( elf_file_hnd, contents_size1 ) );
        if( elf_file_hnd->contents_buffer1 == nullptr ) {
            return( ORL_ERROR );
        }
    } else {
        elf_file_hnd->contents_buffer1 = nullptr;
    }
    s_hdr = static_cast<char *>( _ClientRead( elf_file_hnd, sec_header_table_size ) );
    if( s_hdr == nullptr )
        return( ORL_ERROR );
    std::vector<elf_index>  name_index( elf_file_hnd->num_sections );

    return_val = load_elf_sec_handles( elf_file_hnd, name_index.data(), s_hdr );
    if( return_val != ORL_OKAY ) {
        return( return_val );
    }
    // now sort the section handles by file offset
    qsort( elf_file_hnd->sec_handles, elf_file_hnd->num_sections, sizeof( elf_sec_handle ), section_compare );

    elf_sec_hnd = elf_file_hnd->sec_handles[elf_file_hnd->num_sections - 1];
    contents_size2 = U64Low( elf_sec_hnd->file_offset ) + U64Low( elf_sec_hnd->size ) - U64Low( hdr.shoff ) - sec_header_table_size;
    if( contents_size2 > 0 ) {
        elf_file_hnd->size = U64Low( elf_sec_hnd->file_offset ) + U64Low( elf_sec_hnd->size );
        elf_file_hnd->contents_buffer2 = static_cast<unsigned_8 *>( _ClientRead( elf_file_hnd, contents_size2 ) );
        if( elf_file_hnd->contents_buffer2 == nullptr ) {
            return( ORL_ERROR );
        }
    } else {
        elf_file_hnd->size = U64Low( hdr.shoff ) + sec_header_table_size;
    }
    // determine contents pointers of all sections
    for( elf_quantity i = 0; i < elf_file_hnd->num_sections; ++i ) {
        elf_sec_hnd = elf_file_hnd->sec_handles[i];
        if( U64Low( elf_sec_hnd->size ) > 0 ) {
            if( U64Low( elf_sec_hnd->file_offset ) < U64Low( hdr.shoff ) ) {
                elf_sec_hnd->contents = elf_file_hnd->contents_buffer1 + U64Low( elf_sec_hnd->file_offset ) - hdr.ehsize;
            } else {
                elf_sec_hnd->contents = elf_file_hnd->contents_buffer2 + U64Low( elf_sec_hnd->file_offset ) - U64Low( hdr.shoff ) - sec_header_table_size;
            }
        }
    }
    // determine section names
    char *string_table = reinterpret_cast<char *>( elf_file_hnd->orig_sec_handles[hdr.shstrndx - 1]->contents );

    for( elf_quantity i = 0; i < elf_file_hnd->num_sections; ++i ) {
        elf_file_hnd->orig_sec_handles[i]->name = string_table + name_index[i];
    }
    return( ORL_OKAY );
}
