/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  ELF format low level (symbols and relocations) processing.
*
****************************************************************************/


#include <assert.h>
#include "elflwlv.h"
#include "elforl.h"
#include "orlhash.h"
#include "i64.h"

#include "clibext.h"

#include <bitset>
#include <cstring>
#include <string>


namespace {

// Puts the fields of a symbol or relocation entry read from the file into host byte order, the file's
// byte order being big or little endian. One of the two branches is always compiled out for a given host.
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

void fix_sym_byte_order( elf_file_handle elf_file_hnd, Elf32_Sym *e_sym )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_sym->st_name );
    fix( e_sym->st_value );
    fix( e_sym->st_size );
    fix( e_sym->st_shndx );
}

void fix_sym64_byte_order( elf_file_handle elf_file_hnd, Elf64_Sym *e_sym )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_sym->st_name );
    fix( e_sym->st_value );
    fix( e_sym->st_size );
    fix( e_sym->st_shndx );
}

void fix_rel_byte_order( elf_file_handle elf_file_hnd, Elf32_Rel *e_rel )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_rel->r_offset );
    fix( e_rel->r_info );
}

void fix_rela_byte_order( elf_file_handle elf_file_hnd, Elf32_Rela *e_rela )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_rela->r_offset );
    fix( e_rela->r_info );
    fix( e_rela->r_addend );
}

void fix_rel64_byte_order( elf_file_handle elf_file_hnd, Elf64_Rel *e_rel )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_rel->r_offset );
    fix( e_rel->r_info );
}

void fix_rela64_byte_order( elf_file_handle elf_file_hnd, Elf64_Rela *e_rela )
{
    const ByteOrder fix( elf_file_hnd );

    fix( e_rela->r_offset );
    fix( e_rela->r_info );
    fix( e_rela->r_addend );
}

orl_symbol_binding symbol_binding( unsigned_8 info )
{
    switch( ELF32_ST_BIND( info ) ) {
    case STB_LOCAL:
        return( ORL_SYM_BINDING_LOCAL );
    case STB_GLOBAL:
        return( ORL_SYM_BINDING_GLOBAL );
    case STB_WEAK:
        return( ORL_SYM_BINDING_WEAK );
    default:
        return( ORL_SYM_BINDING_NONE ); // other?
    }
}

orl_symbol_type symbol_type( unsigned_8 info, elf_half shndx )
{
    orl_symbol_type     type;

    switch( ELF32_ST_TYPE( info ) ) {
    case STT_OBJECT:
        type = ORL_SYM_TYPE_OBJECT;
        break;
    case STT_FUNC:
        type = ORL_SYM_TYPE_FUNCTION;
        break;
    case STT_SECTION:
        type = ORL_SYM_TYPE_SECTION;
        break;
    case STT_FILE:
        type = ORL_SYM_TYPE_FILE;
        break;
    default:
        type = ORL_SYM_TYPE_NOTYPE;
        break;
    }
    switch( shndx ) {
    case SHN_ABS:
        type |= ORL_SYM_TYPE_ABSOLUTE;
        break;
    case SHN_COMMON:
        type |= ORL_SYM_TYPE_COMMON;
        break;
    case SHN_UNDEF:
        type |= ORL_SYM_TYPE_UNDEFINED;
        break;
    }
    return( type );
}

// These stay switches: the compiler makes jump tables of them, and ElfConvertRelocType runs for every relocation.

orl_reloc_type convertPPCReloc( elf_reloc_type elf_type )
{
    switch( elf_type ) {
    case R_PPC_NONE:
        return( ORL_RELOC_TYPE_ABSOLUTE );
    case R_PPC_ADDR32:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_PPC_ADDR24:
        return( ORL_RELOC_TYPE_WORD_24 );
    case R_PPC_ADDR16:
        return( ORL_RELOC_TYPE_WORD_16 );
    case R_PPC_ADDR16_LO:
        return( ORL_RELOC_TYPE_HALF_LO );
    case R_PPC_ADDR16_HI:
        return( ORL_RELOC_TYPE_HALF_HI );
    case R_PPC_ADDR16_HA:
        return( ORL_RELOC_TYPE_HALF_HA );
    case R_PPC_ADDR14:
        return( ORL_RELOC_TYPE_WORD_14 );
    case R_PPC_ADDR14_BRTAKEN:
    case R_PPC_ADDR14_BRNTAKEN:
        // fixme
        return( ORL_RELOC_TYPE_WORD_14 );
    case R_PPC_REL24:
        return( ORL_RELOC_TYPE_REL_24 );
    case R_PPC_REL14:
        return( ORL_RELOC_TYPE_REL_14 );
    case R_PPC_REL14_BRTAKEN:
    case R_PPC_REL14_BRNTAKEN:
        // fixme
        return( ORL_RELOC_TYPE_REL_14 );
    case R_PPC_GOT16:
        return( ORL_RELOC_TYPE_GOT_16 );
    case R_PPC_GOT16_LO:
        return( ORL_RELOC_TYPE_GOT_16_LO );
    case R_PPC_GOT16_HI:
        return( ORL_RELOC_TYPE_GOT_16_HI );
    case R_PPC_GOT16_HA:
        return( ORL_RELOC_TYPE_GOT_16_HA );
    case R_PPC_PLTREL24:
        return( ORL_RELOC_TYPE_PLTREL_24 );
    case R_PPC_COPY:
        return( ORL_RELOC_TYPE_NONE );
    case R_PPC_GLOB_DAT:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_PPC_JMP_SLOT:
        // fixme
        return( ORL_RELOC_TYPE_NONE );
    case R_PPC_RELATIVE:
        return( ORL_RELOC_TYPE_SEC_REL );
    case R_PPC_LOCAL24PC:
        // fixme
        return( ORL_RELOC_TYPE_REL_24 );
    case R_PPC_UADDR32:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_PPC_UADDR16:
        return( ORL_RELOC_TYPE_WORD_16 );
    case R_PPC_REL32:
        return( ORL_RELOC_TYPE_REL_32 );
    case R_PPC_PLT32:
        return( ORL_RELOC_TYPE_PLT_32 );
    case R_PPC_PLTREL32:
        return( ORL_RELOC_TYPE_PLTREL_32 );
    case R_PPC_PLT16_LO:
        return( ORL_RELOC_TYPE_PLT_16_LO );
    case R_PPC_PLT16_HI:
        return( ORL_RELOC_TYPE_PLT_16_HI );
    case R_PPC_PLT16_HA:
        return( ORL_RELOC_TYPE_PLT_16_HA );
    }
    return( ORL_RELOC_TYPE_NONE );
}

orl_reloc_type convert386Reloc( elf_reloc_type elf_type )
{
    switch( elf_type ) {
    case R_386_NONE:
    case R_386_COPY:
        return( ORL_RELOC_TYPE_ABSOLUTE );
    case R_386_GOT32:
        return( ORL_RELOC_TYPE_GOT_32 );
    case R_386_PLT32:
        return( ORL_RELOC_TYPE_PLT_32 );
    case R_386_JMP_SLOT:
        return( ORL_RELOC_TYPE_PLTREL_32 );
    case R_386_32:
    case R_386_GOTOFF:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_386_PC32:
    case R_386_GOTPC:
        return( ORL_RELOC_TYPE_REL_32_NOADJ );
    default:
        assert( 0 );
    }
    return( ORL_RELOC_TYPE_NONE );
}

orl_reloc_type convertAMD64Reloc( elf_reloc_type elf_type )
{
    switch( elf_type ) {
    case R_X86_64_NONE:
    case R_X86_64_COPY:
        return( ORL_RELOC_TYPE_ABSOLUTE );
    case R_X86_64_GOT32:
        return( ORL_RELOC_TYPE_GOT_32 );
    case R_X86_64_PLT32:
        return( ORL_RELOC_TYPE_PLT_32 );
    case R_X86_64_JUMP_SLOT:
        return( ORL_RELOC_TYPE_PLTREL_32 );
    case R_X86_64_32:
    case R_X86_64_32S:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_X86_64_64:
        return( ORL_RELOC_TYPE_WORD_64 );
    case R_X86_64_PC32:
    case R_X86_64_GOTPC32:
        return( ORL_RELOC_TYPE_REL_32_NOADJ );
    default:
        assert( 0 );
    }
    return( ORL_RELOC_TYPE_NONE );
}

orl_reloc_type convertSPARCReloc( elf_reloc_type elf_type )
{
    switch( elf_type ) {
    case R_SPARC_NONE:
    case R_SPARC_COPY:
        return( ORL_RELOC_TYPE_ABSOLUTE );
    case R_SPARC_8:
        return( ORL_RELOC_TYPE_WORD_8 );
    case R_SPARC_16:
        return( ORL_RELOC_TYPE_WORD_16 );
    case R_SPARC_32:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_SPARC_DISP8:
        return( ORL_RELOC_TYPE_REL_8 );
    case R_SPARC_DISP16:
        return( ORL_RELOC_TYPE_REL_16 );
    case R_SPARC_DISP32:
        return( ORL_RELOC_TYPE_REL_32 );
    default:
        assert( 0 );
    }
    return( ORL_RELOC_TYPE_NONE );
}

orl_reloc_type convertMIPSReloc( elf_reloc_type elf_type )
{
    switch( elf_type ) {
    case R_MIPS_NONE:
        return( ORL_RELOC_TYPE_ABSOLUTE );
    case R_MIPS_16:
        return( ORL_RELOC_TYPE_WORD_16 );
    case R_MIPS_32:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_MIPS_REL32:
        return( ORL_RELOC_TYPE_REL_32 );
    case R_MIPS_HI16:
        return( ORL_RELOC_TYPE_HALF_HI );
    case R_MIPS_LO16:
        return( ORL_RELOC_TYPE_HALF_LO );
    case R_MIPS_GOT16:
        return( ORL_RELOC_TYPE_GOT_16 );
    case R_MIPS_CALL16:
        return( ORL_RELOC_TYPE_GOT_16 );
    case R_MIPS_26:
        return( ORL_RELOC_TYPE_WORD_26 );
    case R_MIPS_PC16:
        return( ORL_RELOC_TYPE_REL_16 );
    default:
        assert( 0 );
    }
    return( ORL_RELOC_TYPE_NONE );
}

orl_reloc_type convertAlphaReloc( elf_reloc_type elf_type )
{
    switch( elf_type ) {
    case R_ALPHA_NONE:
    case R_ALPHA_COPY:
        return( ORL_RELOC_TYPE_ABSOLUTE );
    case R_ALPHA_REFLONG:
    case R_ALPHA_REFQUAD:
        return( ORL_RELOC_TYPE_WORD_32 );
    case R_ALPHA_GPREL32:
        return( ORL_RELOC_TYPE_GOT_32 );
    case R_ALPHA_GPRELHIGH:
        return( ORL_RELOC_TYPE_HALF_HI );
    case R_ALPHA_GPRELLOW:
        return( ORL_RELOC_TYPE_HALF_LO );
    default:
        assert( 0 );
    }
    return( ORL_RELOC_TYPE_NONE );
}

// Makes the orl relocations of a REL or RELA section (rela: with explicit addends) into reloc_sec->assoc.reloc.relocs.
orl_return create_relocs( elf_sec_handle orig_sec, elf_sec_handle reloc_sec, bool rela )
{
    const unsigned          num_relocs = U64Low( reloc_sec->size ) / U64Low( reloc_sec->entsize );
    const bool              is64bit = ( (reloc_sec->elf_file_hnd->flags & ORL_FILE_FLAG_64BIT_MACHINE) != 0 );
    const elf_symbol_handle symbols = reloc_sec->assoc.reloc.symbol_table->assoc.sym.symbols;
    orl_reloc_struct        *orel;
    unsigned char           *irel;

    orel = static_cast<orl_reloc_struct *>( _ClientSecAlloc( reloc_sec, ORL_STRUCT_SIZEOF( orl_reloc ) * num_relocs ) );
    reloc_sec->assoc.reloc.relocs = orel;
    if( orel == nullptr )
        return( ORL_OUT_OF_MEMORY );
    irel = reloc_sec->contents;
    for( unsigned i = 0; i < num_relocs; i++ ) {
        orel->section = reinterpret_cast<orl_sec_handle>( orig_sec );
        orel->frame = nullptr;
        if( !rela ) {
            orel->addend = 0;
        }
        if( is64bit ) {
            if( rela ) {
                Elf64_Rela *irela64 = reinterpret_cast<Elf64_Rela *>( irel );

                fix_rela64_byte_order( reloc_sec->elf_file_hnd, irela64 );
                orel->symbol = reinterpret_cast<orl_symbol_handle>( symbols + U64High( irela64->r_info ) );
                orel->type = ElfConvertRelocType( reloc_sec->elf_file_hnd, static_cast<elf_reloc_type>( U64Low( irela64->r_info ) ) );
                orel->offset = U64Low( irela64->r_offset );
                orel->addend = U64Low( irela64->r_addend );
            } else {
                Elf64_Rel *irel64 = reinterpret_cast<Elf64_Rel *>( irel );

                fix_rel64_byte_order( reloc_sec->elf_file_hnd, irel64 );
                orel->symbol = reinterpret_cast<orl_symbol_handle>( symbols + U64High( irel64->r_info ) );
                orel->type = ElfConvertRelocType( reloc_sec->elf_file_hnd, static_cast<elf_reloc_type>( U64Low( irel64->r_info ) ) );
                orel->offset = U64Low( irel64->r_offset );
            }
        } else {
            if( rela ) {
                Elf32_Rela *irela32 = reinterpret_cast<Elf32_Rela *>( irel );

                fix_rela_byte_order( reloc_sec->elf_file_hnd, irela32 );
                orel->symbol = reinterpret_cast<orl_symbol_handle>( symbols + ELF32_R_SYM( irela32->r_info ) );
                orel->type = ElfConvertRelocType( reloc_sec->elf_file_hnd, ELF32_R_TYPE( irela32->r_info ) );
                orel->offset = irela32->r_offset;
                orel->addend = irela32->r_addend;
            } else {
                Elf32_Rel *irel32 = reinterpret_cast<Elf32_Rel *>( irel );

                fix_rel_byte_order( reloc_sec->elf_file_hnd, irel32 );
                orel->symbol = reinterpret_cast<orl_symbol_handle>( symbols + ELF32_R_SYM( irel32->r_info ) );
                orel->type = ElfConvertRelocType( reloc_sec->elf_file_hnd, ELF32_R_TYPE( irel32->r_info ) );
                orel->offset = irel32->r_offset;
            }
        }
        irel += U64Low( reloc_sec->entsize );
        orel++;
    }
    return( ORL_OKAY );
}

// The length of the start of s that has none of the characters of charset, looking at len characters at most.
size_t strncspn( const char *s, const char *charset, size_t len )
{
    std::bitset<256>    chars;
    size_t              i;

    for( ; *charset != 0; charset++ ) {
        chars.set( static_cast<unsigned char>( *charset ) );
    }
    for( i = 0; i < len; i++ ) {
        if( chars.test( static_cast<unsigned char>( s[i] ) ) ) {
            break;
        }
    }
    return( i );
}

const char *pstrncspn( const char *s, const char *charset, size_t *len )
{
    size_t  l = strncspn( s, charset, *len );

    *len -= l;
    return( s + l );
}

void EatWhite( const char **contents, size_t *len )
/*************************************************/
{
    char    ch = **contents;

    while( (ch == ' ' || ch == '\t' || ch == '=' || ch == ',') && *len > 0 ) {
        (*len)--;
        *contents += 1;
        ch = **contents;
    }
}

// The argument at the start of *contents (up to a comma or white space), moved past.
std::string take_argument( const char **contents, size_t *len )
{
    size_t      l = strncspn( *contents, ", \t", *len );
    std::string arg( *contents, l );

    *len -= l;
    *contents += l;
    return( arg );
}

orl_return ParseExport( const char **contents, size_t *len, callback_export_fn *export_fn, void *cookie )
/*******************************************************************************************************/
{
    std::string arg = take_argument( contents, len );

    if( export_fn == nullptr )
        return( ORL_OKAY );
    return( export_fn( arg.c_str(), cookie ) );
}

orl_return ParseDefLibEntry( const char **contents, size_t *len, callback_deflib_fn *deflibentry_fn, void *cookie )
/*****************************************************************************************************************/
{
    orl_return      retval;

    for( ;; ) {
        std::string arg = take_argument( contents, len );

        if( deflibentry_fn == nullptr ) {
            retval = ORL_OKAY;
        } else {
            retval = deflibentry_fn( arg.c_str(), cookie );
        }
        if( retval != ORL_OKAY || **contents != ',' )
            break;
        (*contents)++;
    }
    return( retval );
}

}  // namespace

orl_return ElfCreateSymbolHandles( elf_sec_handle elf_sec_hnd )
{
    const unsigned      num_syms = U64Low( elf_sec_hnd->size ) / U64Low( elf_sec_hnd->entsize );
    const bool          is64bit = ( (elf_sec_hnd->elf_file_hnd->flags & ORL_FILE_FLAG_64BIT_MACHINE) != 0 );
    elf_symbol_handle   current;
    unsigned char       *current_sym;

    elf_sec_hnd->assoc.sym.symbols = static_cast<elf_symbol_handle>( _ClientSecAlloc( elf_sec_hnd, ORL_STRUCT_SIZEOF( elf_symbol_handle ) * num_syms ) );
    if( elf_sec_hnd->assoc.sym.symbols == nullptr )
        return( ORL_OUT_OF_MEMORY );
    current = elf_sec_hnd->assoc.sym.symbols;
    current_sym = elf_sec_hnd->contents;
    for( unsigned i = 0; i < num_syms; i++ ) {
        unsigned_32         st_name;

        if( is64bit ) {
            Elf64_Sym   *current_sym64 = reinterpret_cast<Elf64_Sym *>( current_sym );

            fix_sym64_byte_order( elf_sec_hnd->elf_file_hnd, current_sym64 );
            st_name = current_sym64->st_name;
            U64High( current->value ) = U64High( current_sym64->st_value );
            U64Low( current->value ) = U64Low( current_sym64->st_value );
            current->info = current_sym64->st_info;
            current->shndx = current_sym64->st_shndx;
        } else {
            Elf32_Sym   *current_sym32 = reinterpret_cast<Elf32_Sym *>( current_sym );

            fix_sym_byte_order( elf_sec_hnd->elf_file_hnd, current_sym32 );
            st_name = current_sym32->st_name;
            Set64ValU32( current->value, current_sym32->st_value );
            current->info = current_sym32->st_info;
            current->shndx = current_sym32->st_shndx;
        }
        if( st_name == 0 ) {
            current->name = nullptr;
        } else {
            current->name = reinterpret_cast<char *>( elf_sec_hnd->assoc.sym.string_table->contents + st_name );
        }
        current->file_format = ORL_ELF;
        current->elf_file_hnd = elf_sec_hnd->elf_file_hnd;
        current->binding = symbol_binding( current->info );
        current->type = symbol_type( current->info, current->shndx );
        current++;
        current_sym += U64Low( elf_sec_hnd->entsize );
    }
    return( ORL_OKAY );
}

orl_return ElfBuildSecNameHashTable( elf_file_handle elf_file_hnd )
{
    orl_return      return_val;
    orl_hash_key    h_key;
    orl_hash_data   h_data;

    elf_file_hnd->sec_name_hash_table = ORLHashTableCreate( elf_file_hnd->elf_hnd->funcs, SEC_NAME_HASH_TABLE_SIZE, ORL_HASH_STRING_IGNORECASE );
    if( elf_file_hnd->sec_name_hash_table == nullptr ) {
        return( ORL_OUT_OF_MEMORY );
    }
    for( elf_quantity i = 0; i < elf_file_hnd->num_sections; ++i ) {
        h_key.u.string = elf_file_hnd->sec_handles[i]->name;
        h_data.u.sec_handle = reinterpret_cast<orl_sec_handle>( elf_file_hnd->sec_handles[i] );
        return_val = ORLHashTableInsert( elf_file_hnd->sec_name_hash_table, h_key, h_data );
        if( return_val != ORL_OKAY ) {
            return( return_val );
        }
    }
    return( ORL_OKAY );
}

orl_reloc_type ElfConvertRelocType( elf_file_handle elf_file_hnd, elf_reloc_type elf_type )
{
    switch( elf_file_hnd->machine_type ) {
    case ORL_MACHINE_TYPE_PPC601:
        return( convertPPCReloc( elf_type ) );
    case ORL_MACHINE_TYPE_I386:
        return( convert386Reloc( elf_type ) );
    case ORL_MACHINE_TYPE_AMD64:
        return( convertAMD64Reloc( elf_type ) );
    case ORL_MACHINE_TYPE_SPARC:
    case ORL_MACHINE_TYPE_SPARCPLUS:
        return( convertSPARCReloc( elf_type ) );
    case ORL_MACHINE_TYPE_R3000:
    case ORL_MACHINE_TYPE_R4000:
        return( convertMIPSReloc( elf_type ) );
    case ORL_MACHINE_TYPE_ALPHA:
        return( convertAlphaReloc( elf_type ) );
    default:
        assert( 0 );
    }
    return( ORL_RELOC_TYPE_NONE );
}

orl_return ElfCreateRelocs( elf_sec_handle orig_sec, elf_sec_handle reloc_sec )
{
    if( reloc_sec->assoc.reloc.symbol_table->assoc.sym.symbols == nullptr ) {
        orl_return return_val = ElfCreateSymbolHandles( reloc_sec->assoc.reloc.symbol_table );

        if( return_val != ORL_OKAY ) {
            return( return_val );
        }
    }
    switch( reloc_sec->type ) {
    case ORL_SEC_TYPE_RELOCS:
        return( create_relocs( orig_sec, reloc_sec, false ) );
    case ORL_SEC_TYPE_RELOCS_EXPADD:
        return( create_relocs( orig_sec, reloc_sec, true ) );
    default:
        break;
    }
    return( ORL_OKAY );
}

orl_return ElfParseDrectve( const char *contents, size_t len, orl_note_callbacks *cbs, void *cookie )
/***************************************************************************************************/
{
    const char      *cmd;

    EatWhite( &contents, &len );
    while( len > 0 ) {
        if( *contents != '-' )
            break;              // - should be start of token
        contents++; len--;
        cmd = contents;
        contents = pstrncspn( contents, ":", &len );
        if( contents == nullptr )
            break;
        contents++; len--;
        if( strnicmp( cmd, "export", 6 ) == 0 ) {
            if( ParseExport( &contents, &len, cbs->export_fn, cookie ) != ORL_OKAY ) {
                break;
            }
        } else if( strnicmp( cmd, "defaultlib", 10 ) == 0 ) {
            if( ParseDefLibEntry( &contents, &len, cbs->deflib_fn, cookie ) != ORL_OKAY ) {
                break;
            }
        } else if( strnicmp( cmd, "entry", 5 ) == 0 ) {
            if( ParseDefLibEntry( &contents, &len, cbs->entry_fn, cookie ) != ORL_OKAY ) {
                break;
            }
        }
        EatWhite( &contents, &len );
    }
    return( ORL_OKAY );
}
