/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  COFF symbol table and relocations processing.
*
****************************************************************************/


#include <bitset>
#include <cstring>
#include <string>
#include <string_view>
#include "cofflwlv.h"
#include "cofforl.h"
#include "orlhash.h"

#include "clibext.h"


namespace {

// whether a symbol has one of the names COFF's debug records have
bool name_is( const char *name, const char *text )
{
    return std::strcmp( name, text ) == 0;
}

// whether the symbol's type says it is a function
bool is_function( const coff_symbol *symbol )
{
    const unsigned_16   type = _CoffComplexType( symbol->type );

    return ( type & COFF_IMAGE_SYM_DTYPE_FUNCTION ) != 0;
}

}  // namespace

orl_return CoffCreateSymbolHandles( coff_file_handle file_hnd )
{
    if( file_hnd->num_symbols == 0 ) {
        file_hnd->symbol_handles = nullptr;
        return ORL_OKAY;
    }
    file_hnd->symbol_handles = static_cast<coff_symbol_handle>( _ClientAlloc( file_hnd, ORL_STRUCT_SIZEOF( coff_symbol_handle ) * file_hnd->num_symbols ) );
    if( file_hnd->symbol_handles == nullptr )
        return ORL_OUT_OF_MEMORY;
    coff_quantity   prev = 0;

    for( coff_quantity i = 0; i < file_hnd->num_symbols; i++ ) {
        coff_symbol_handle  current = file_hnd->symbol_handles + i;

        current->file_format = ORL_COFF;
        current->coff_file_hnd = file_hnd;
        current->has_bf = false;
        current->symbol = reinterpret_cast<coff_symbol *>( file_hnd->symbol_table->contents + sizeof( coff_symbol ) * i );
        if( current->symbol->name.u.non_name.zeros == 0 ) {
            current->name = reinterpret_cast<char *>( file_hnd->string_table->contents + current->symbol->name.u.non_name.offset - sizeof( coff_sec_size ) );
            current->name_alloced = false;
        } else {
            // a name of the eight characters the symbol has room for has no terminator there: it is copied to get one
            const char  *name_string = current->symbol->name.u.name_string;

            if( strnlen( name_string, COFF_SYM_NAME_LEN ) == COFF_SYM_NAME_LEN ) {
                current->name = static_cast<char *>( _ClientAlloc( file_hnd, COFF_SYM_NAME_LEN + 1 ) );
                memcpy( current->name, name_string, COFF_SYM_NAME_LEN );
                current->name[COFF_SYM_NAME_LEN] = '\0';
                current->name_alloced = true;
            } else {
                current->name = current->symbol->name.u.name_string;
                current->name_alloced = false;
            }
        }
        if( name_is( current->name, ".bf" ) ) {
            if( current->symbol->num_aux >= 1 ) {
                file_hnd->symbol_handles[prev].has_bf = true;
            }
        }
        coff_sec_handle     sechdl = nullptr;

        current->type = static_cast<orl_symbol_type>( 0 );
        switch( current->symbol->sec_num ) {
        case COFF_IMAGE_SYM_DEBUG:
            current->type |= ORL_SYM_TYPE_DEBUG;
            current->binding = ORL_SYM_BINDING_NONE;
            break;
        case COFF_IMAGE_SYM_ABSOLUTE:
            current->type |= ORL_SYM_TYPE_ABSOLUTE;
            current->binding = ORL_SYM_BINDING_NONE; // ?
            break;
        case COFF_IMAGE_SYM_UNDEFINED:
            if( current->symbol->value == 0 ) {
                current->type |= ORL_SYM_TYPE_UNDEFINED;
            } else {
                current->type |= ORL_SYM_TYPE_COMMON;
            }
            break;
        default:
            current->type |= ORL_SYM_TYPE_DEFINED;
            sechdl = file_hnd->orig_sec_hnd[current->symbol->sec_num - 1];
            if( sechdl->flags & ORL_SEC_FLAG_COMDAT ) {
                current->type |= ORL_SYM_CDAT_MASK;
            }
            break;
        }
        switch( current->symbol->storage_class ) {
        case COFF_IMAGE_SYM_CLASS_EXTERNAL:
        case COFF_IMAGE_SYM_CLASS_LABEL:
        case COFF_IMAGE_SYM_CLASS_UNDEFINED_LABEL:
        case COFF_IMAGE_SYM_CLASS_WEAK_EXTERNAL:
            if( current->symbol->storage_class == COFF_IMAGE_SYM_CLASS_LABEL ) {
                current->binding = ORL_SYM_BINDING_LOCAL;
            } else if( ( current->symbol->storage_class == COFF_IMAGE_SYM_CLASS_EXTERNAL
                      || current->symbol->storage_class == COFF_IMAGE_SYM_CLASS_WEAK_EXTERNAL )
                    && current->symbol->sec_num == COFF_IMAGE_SYM_UNDEFINED
                    && current->symbol->value == 0
                    && current->symbol->num_aux == 1 ) {
                const coff_sym_weak *weak = reinterpret_cast<const coff_sym_weak *>( current->symbol + 1 );

                switch( weak->characteristics ) {
                case COFF_IMAGE_WEAK_EXTERN_SEARCH_NOLIBRARY:
                    current->binding = ORL_SYM_BINDING_WEAK;
                    break;
                case COFF_IMAGE_WEAK_EXTERN_SEARCH_LIBRARY:
                    current->binding = ORL_SYM_BINDING_LAZY;
                    break;
                case COFF_IMAGE_WEAK_EXTERN_SEARCH_ALIAS:
                    current->binding = ORL_SYM_BINDING_ALIAS;
                    break;
                }
            } else {
                current->binding = ORL_SYM_BINDING_GLOBAL;
            }
            if( is_function( current->symbol ) ) {
                current->type |= ORL_SYM_TYPE_FUNCTION;
            } else {
                current->type |= ORL_SYM_TYPE_OBJECT;
            }
            break;
        case COFF_IMAGE_SYM_CLASS_STATIC:
            current->binding = ORL_SYM_BINDING_LOCAL;
            if( current->symbol->num_aux == 0 ) {
                if( sechdl != nullptr && strcmp( sechdl->name, current->name ) == 0 ) {
                    current->type |= ORL_SYM_TYPE_SECTION;
                } else if( is_function( current->symbol ) ) {
                    current->type |= ORL_SYM_TYPE_FUNCTION;
                } else {
                    current->type |= ORL_SYM_TYPE_OBJECT;
                }
            } else if( current->symbol->num_aux == 1 && ( current->type & ORL_SYM_CDAT_MASK ) ) {
                const coff_sym_section  *aux = reinterpret_cast<const coff_sym_section *>( current->symbol + 1 );

                current->type |= ORL_SYM_TYPE_SECTION;
                current->type &= ~ORL_SYM_CDAT_MASK;
                current->type = static_cast<orl_symbol_type>( current->type | ( ( aux->selection << ORL_SYM_CDAT_SHIFT ) & ORL_SYM_CDAT_MASK ) );
            } else if( is_function( current->symbol ) ) {
                current->type |= ORL_SYM_TYPE_FUNCTION;
            }
            break;
        case COFF_IMAGE_SYM_CLASS_FUNCTION:
            // The .bf, .lf and .ef symbols are not regular symbols
            // and their values in particular must not be interpreted
            // as offsets/addresses.
            if( name_is( current->name, ".bf" )
                || name_is( current->name, ".lf" )
                || name_is( current->name, ".ef" ) ) {
                current->binding = ORL_SYM_BINDING_NONE;
            } else {
                current->binding = ORL_SYM_BINDING_LOCAL;
            }
            current->type |= ORL_SYM_TYPE_FUNC_INFO;
            break;
        case COFF_IMAGE_SYM_CLASS_FILE:
            current->binding = ORL_SYM_BINDING_LOCAL;
            current->type |= ORL_SYM_TYPE_FILE;
            break;
        }
        prev = i;
        i += current->symbol->num_aux;
    }
    return ORL_OKAY;
}

orl_return CoffBuildSecNameHashTable( coff_file_handle coff_file_hnd )
{
    coff_file_hnd->sec_name_hash_table = ORLHashTableCreate( coff_file_hnd->coff_hnd->funcs, SEC_NAME_HASH_TABLE_SIZE, ORL_HASH_STRING_IGNORECASE );
    if( coff_file_hnd->sec_name_hash_table == nullptr ) {
        return ORL_OUT_OF_MEMORY;
    }
    for( coff_quantity i = 0; i < coff_file_hnd->num_sections; ++i ) {
        orl_hash_key    h_key;
        orl_hash_data   h_data;

        h_key.u.string = coff_file_hnd->coff_sec_hnd[i]->name;
        h_data.u.sec_handle = reinterpret_cast<orl_sec_handle>( coff_file_hnd->coff_sec_hnd[i] );
        const orl_return    return_val = ORLHashTableInsert( coff_file_hnd->sec_name_hash_table, h_key, h_data );

        if( return_val != ORL_OKAY ) {
            return return_val;
        }
    }
    return ORL_OKAY;
}

namespace {

// the relocation types of each machine, in ORL's
orl_reloc_type alpha_reloc_type( coff_reloc_type coff_type )
{
    switch( coff_type ) {
    case COFF_IMAGE_REL_ALPHA_ABSOLUTE:
        return ORL_RELOC_TYPE_ABSOLUTE;
    case COFF_IMAGE_REL_ALPHA_REFLONG:
    case COFF_IMAGE_REL_ALPHA_REFQUAD:
        return ORL_RELOC_TYPE_WORD_32;
    case COFF_IMAGE_REL_ALPHA_BRADDR:
        return ORL_RELOC_TYPE_REL_21_SH;
    case COFF_IMAGE_REL_ALPHA_REFHI:
    case COFF_IMAGE_REL_ALPHA_INLINE_REFLONG:
        return ORL_RELOC_TYPE_HALF_HI;
    case COFF_IMAGE_REL_ALPHA_REFLO:
        return ORL_RELOC_TYPE_HALF_LO;
    case COFF_IMAGE_REL_ALPHA_PAIR:
        return ORL_RELOC_TYPE_PAIR;
    case COFF_IMAGE_REL_ALPHA_SECTION:
        return ORL_RELOC_TYPE_SEGMENT;
    case COFF_IMAGE_REL_ALPHA_SECREL:
        return ORL_RELOC_TYPE_SEC_REL;
    case COFF_IMAGE_REL_ALPHA_REFLONGNB:
        return ORL_RELOC_TYPE_WORD_32_NB;
    default:
        break;
    }
    return ORL_RELOC_TYPE_NONE;
}

orl_reloc_type i386_reloc_type( coff_reloc_type coff_type )
{
    switch( coff_type ) {
    case COFF_IMAGE_REL_I386_ABSOLUTE:
        return ORL_RELOC_TYPE_ABSOLUTE;
    case COFF_IMAGE_REL_I386_DIR16:
        return ORL_RELOC_TYPE_WORD_16;
    case COFF_IMAGE_REL_I386_REL16:
        return ORL_RELOC_TYPE_REL_16;
    case COFF_IMAGE_REL_I386_DIR32:
        return ORL_RELOC_TYPE_WORD_32;
    case COFF_IMAGE_REL_I386_DIR32NB:
        return ORL_RELOC_TYPE_WORD_32_NB;
    case COFF_IMAGE_REL_I386_REL32:
        return ORL_RELOC_TYPE_JUMP;
    case COFF_IMAGE_REL_I386_SECTION:
        return ORL_RELOC_TYPE_SECTION;
    case COFF_IMAGE_REL_I386_SECREL:
        return ORL_RELOC_TYPE_SEC_REL;
    default:
        break;
    }
    return ORL_RELOC_TYPE_NONE;
}

orl_reloc_type amd64_reloc_type( coff_reloc_type coff_type )
{
    switch( coff_type ) {
    case COFF_IMAGE_REL_AMD64_ABSOLUTE:
        return ORL_RELOC_TYPE_ABSOLUTE;
    case COFF_IMAGE_REL_AMD64_REL32:              // 32-Bit PC-relative offset
        return ORL_RELOC_TYPE_REL_32;
    case COFF_IMAGE_REL_AMD64_ADDR32:
        return ORL_RELOC_TYPE_WORD_32;
    case COFF_IMAGE_REL_AMD64_ADDR32NB:
        return ORL_RELOC_TYPE_WORD_32_NB;
    case COFF_IMAGE_REL_AMD64_ADDR64:
        return ORL_RELOC_TYPE_WORD_64;
    case COFF_IMAGE_REL_AMD64_REL32_1:
        return ORL_RELOC_TYPE_REL_32_ADJ1;
    case COFF_IMAGE_REL_AMD64_REL32_2:
        return ORL_RELOC_TYPE_REL_32_ADJ2;
    case COFF_IMAGE_REL_AMD64_REL32_3:
        return ORL_RELOC_TYPE_REL_32_ADJ3;
    case COFF_IMAGE_REL_AMD64_REL32_4:
        return ORL_RELOC_TYPE_REL_32_ADJ4;
    case COFF_IMAGE_REL_AMD64_REL32_5:
        return ORL_RELOC_TYPE_REL_32_ADJ5;
    case COFF_IMAGE_REL_AMD64_SECREL:
        return ORL_RELOC_TYPE_SEC_REL;
    default:
        break;
    }
    return ORL_RELOC_TYPE_NONE;
}

orl_reloc_type ppc_reloc_type( coff_reloc_type coff_type )
{
    switch( coff_type & COFF_IMAGE_REL_PPC_TYPEMASK ) {
    case COFF_IMAGE_REL_PPC_ABSOLUTE: // NOP
        return ORL_RELOC_TYPE_ABSOLUTE;
    case COFF_IMAGE_REL_PPC_ADDR64:   // 64-bit address
        return ORL_RELOC_TYPE_WORD_64;
    case COFF_IMAGE_REL_PPC_ADDR32:   // 32-bit address
        return ORL_RELOC_TYPE_WORD_32;
    case COFF_IMAGE_REL_PPC_ADDR24:   // 26-bit address, shifted left 2 (branch absolute)
        return ORL_RELOC_TYPE_WORD_24;
    case COFF_IMAGE_REL_PPC_ADDR16:   // 16-bit address
        return ORL_RELOC_TYPE_WORD_16;
    case COFF_IMAGE_REL_PPC_ADDR14:   // 16-bit address, shifted left 2 (load doubleword)
        return ORL_RELOC_TYPE_WORD_14;
    case COFF_IMAGE_REL_PPC_REL24:    // 26-bit PC-relative offset, shifted left 2 (branch relative)
        return ORL_RELOC_TYPE_REL_24;
    case COFF_IMAGE_REL_PPC_REL14:    // 16-bit PC-relative offset, shifted left 2 (br cond relative)
        return ORL_RELOC_TYPE_REL_14;
    case COFF_IMAGE_REL_PPC_TOCREL16: // 16-bit offset from TOC base
        if( coff_type & COFF_IMAGE_REL_PPC_TOCDEFN ) {
            return ORL_RELOC_TYPE_TOCVREL_16;
        }
        return ORL_RELOC_TYPE_TOCREL_16;
    case COFF_IMAGE_REL_PPC_TOCREL14: // 14-bit offset from TOC base, shifted left 2 (load doubleword)
        if( coff_type & COFF_IMAGE_REL_PPC_TOCDEFN ) {
            return ORL_RELOC_TYPE_TOCVREL_14;
        }
        return ORL_RELOC_TYPE_TOCREL_14;
    case COFF_IMAGE_REL_PPC_SECREL:
        return ORL_RELOC_TYPE_SEC_REL;
    case COFF_IMAGE_REL_PPC_SECTION:
        return ORL_RELOC_TYPE_SEGMENT;
    case COFF_IMAGE_REL_PPC_ADDR32NB:  // 32-bit addr w/o image base
        return ORL_RELOC_TYPE_WORD_32_NB;
    case COFF_IMAGE_REL_PPC_IFGLUE: // Substitute TOC restore instruction iff symbol is glue code
        return ORL_RELOC_TYPE_IFGLUE;
    case COFF_IMAGE_REL_PPC_IMGLUE:
        return ORL_RELOC_TYPE_IMGLUE;
    default:
        break;
    }
    return ORL_RELOC_TYPE_NONE;
}

orl_reloc_type mips_reloc_type( coff_reloc_type coff_type )
{
    switch( coff_type ) {
    case COFF_IMAGE_REL_MIPS_ABSOLUTE:   // NOP
        return ORL_RELOC_TYPE_ABSOLUTE;
    case COFF_IMAGE_REL_MIPS_REFWORD:    // 32-bit address
        return ORL_RELOC_TYPE_WORD_32;
    case COFF_IMAGE_REL_MIPS_JMPADDR:    // 26-bit absolute address (j/jal)
        return ORL_RELOC_TYPE_WORD_26;
    case COFF_IMAGE_REL_MIPS_REFHI:
        return ORL_RELOC_TYPE_HALF_HI;
    case COFF_IMAGE_REL_MIPS_REFLO:
        return ORL_RELOC_TYPE_HALF_LO;
    case COFF_IMAGE_REL_MIPS_PAIR:
        return ORL_RELOC_TYPE_PAIR;
    case COFF_IMAGE_REL_MIPS_GPREL:      // 16-bit offset from GP register
    case COFF_IMAGE_REL_MIPS_LITERAL:
        return ORL_RELOC_TYPE_TOCREL_16;
    case COFF_IMAGE_REL_MIPS_SECREL:
        return ORL_RELOC_TYPE_SEC_REL;
    case COFF_IMAGE_REL_MIPS_SECTION:
        return ORL_RELOC_TYPE_SEGMENT;
    case COFF_IMAGE_REL_MIPS_REFWORDNB:  // 32-bit addr w/o image base
        return ORL_RELOC_TYPE_WORD_32_NB;
    default:
        break;
    }
    return ORL_RELOC_TYPE_NONE;
}

}  // namespace

orl_reloc_type CoffConvertRelocType( coff_file_handle coff_file_hnd, coff_reloc_type coff_type )
{
    switch( coff_file_hnd->machine_type ) {
    case ORL_MACHINE_TYPE_ALPHA:
        return alpha_reloc_type( coff_type );
    case ORL_MACHINE_TYPE_I386:
        return i386_reloc_type( coff_type );
    case ORL_MACHINE_TYPE_AMD64:
        return amd64_reloc_type( coff_type );
    case ORL_MACHINE_TYPE_PPC601:
        return ppc_reloc_type( coff_type );
    case ORL_MACHINE_TYPE_R3000:
    case ORL_MACHINE_TYPE_R4000:
        return mips_reloc_type( coff_type );
    default:
        break;
    }
    return static_cast<orl_reloc_type>( 0 );
}

orl_return CoffCreateRelocs( coff_sec_handle orig_sec, coff_sec_handle reloc_sec )
{
    if( reloc_sec->coff_file_hnd->symbol_handles == nullptr ) {
        const orl_return    return_val = CoffCreateSymbolHandles( reloc_sec->coff_file_hnd );

        if( return_val != ORL_OKAY ) {
            return return_val;
        }
    }
    const unsigned  num_relocs = reloc_sec->size / sizeof( coff_reloc );

    reloc_sec->assoc.reloc.num_relocs = num_relocs;
    ORL_STRUCT( orl_reloc ) *orel = static_cast<ORL_STRUCT( orl_reloc ) *>( _ClientSecAlloc( reloc_sec, ORL_STRUCT_SIZEOF( orl_reloc ) * num_relocs ) );

    reloc_sec->assoc.reloc.relocs = orel;
    if( orel == nullptr )
        return ORL_OUT_OF_MEMORY;
    const coff_reloc ORLUNALIGNED   *irel = reinterpret_cast<const coff_reloc *>( reloc_sec->contents );
    coff_file_handle                file_hnd = reloc_sec->coff_file_hnd;

    for( unsigned i = 0; i < num_relocs; i++ ) {
        orel->section = reinterpret_cast<orl_sec_handle>( orig_sec );
        orel->frame = nullptr;
        orel->addend = 0;
        if( file_hnd->machine_type == ORL_MACHINE_TYPE_ALPHA && irel->type == COFF_IMAGE_REL_ALPHA_MATCH ) {
            // (the first relocation of a table being one of these reads the one before it)
            const ORL_STRUCT( orl_reloc )   *prev_orel = orel - 1;

            orel->type = ORL_RELOC_TYPE_HALF_LO;
            orel->symbol = prev_orel->symbol;
            orel->offset = prev_orel->offset + irel->sym_tab_index;
        } else {
            orel->type = CoffConvertRelocType( file_hnd, irel->type );
            if( orel->type == ORL_RELOC_TYPE_PAIR ) {
                orel->symbol = nullptr;
            } else {
                orel->symbol = reinterpret_cast<orl_symbol_handle>( file_hnd->symbol_handles + irel->sym_tab_index );
            }
            orel->offset = irel->offset - orig_sec->hdr->offset;
        }
        irel++;
        orel++;
    }
    return ORL_OKAY;
}

orl_linnum CoffConvertLines( coff_sec_handle hdl, orl_table_index numlines )
/**************************************************************************/
{
    coff_file_handle    fhdl = hdl->coff_file_hnd;

    if( fhdl->symbol_handles == nullptr ) {
        if( CoffCreateSymbolHandles( fhdl ) != ORL_OKAY ) {
            return nullptr;
        }
    }
    // the file's line numbers are converted in place: the table is the one the file was read into
    coff_line_num ORLUNALIGNED  *coffline = reinterpret_cast<coff_line_num *>( hdl->hdr->lineno_ptr - fhdl->initial_size + fhdl->rest_of_file_buffer );
    ORL_STRUCT( orl_linnum ) ORLUNALIGNED   *currline = reinterpret_cast<ORL_STRUCT( orl_linnum ) *>( coffline );

    if( hdl->relocs_done )
        return currline;
    ORL_STRUCT( orl_linnum ) ORLUNALIGNED   *linestart = currline;
    unsigned_32                             linebase = 0;

    while( numlines > 0 ) {
        if( coffline->line_number == 0 ) {
            coff_symbol_handle  sym = fhdl->symbol_handles + coffline->ir.symbol_table_index;

            coffline->ir.RVA = sym->symbol->value;
            if( sym->has_bf ) {
                const coff_sym_bfef *csym = reinterpret_cast<const coff_sym_bfef *>( sym->symbol + sym->symbol->num_aux + 2 );

                linebase = csym->linenum;
            }
        }
        const unsigned_32   offset = coffline->ir.RVA;

        currline->linnum = static_cast<unsigned_16>( coffline->line_number + linebase );
        currline->off = offset;
        coffline++;
        currline++;
        numlines--;
    }
    hdl->relocs_done = true;
    return linestart;
}

namespace {

// where in the first `len` characters of s is the first one of the charset (len if none)
std::size_t strncspn( const char *s, std::string_view charset, std::size_t len )
{
    std::bitset<256>    in_set;

    for( const char c : charset ) {
        in_set.set( static_cast<unsigned char>( c ) );
    }
    std::size_t     i;

    for( i = 0; i < len; i++ ) {
        if( in_set.test( static_cast<unsigned char>( s[i] ) ) ) {
            break;
        }
    }
    return i;
}

// the rest of what is being read: where it is, and how much is left of it
struct Drectve {
    const char  *contents;
    std::size_t len;

    void eat_white()
    {
        while( ( *contents == ' ' || *contents == '\t' || *contents == '=' || *contents == ',' ) && len > 0 ) {
            len--;
            contents++;
        }
    }

    // the text up to the first of the charset, which is left unread
    std::string take_until( std::string_view charset )
    {
        const std::size_t   l = strncspn( contents, charset, len );
        std::string         arg( contents, l );

        len -= l;
        contents += l;
        return arg;
    }
};

orl_return ParseExport( Drectve &text, callback_export_fn *export_fn, void *cookie )
/***********************************************************************************/
{
    const std::string   arg = text.take_until( ", \t" );

    if( export_fn == nullptr )
        return ORL_OKAY;
    return export_fn( arg.c_str(), cookie );
}


orl_return ParseDefLibEntry( Drectve &text, callback_deflib_fn *deflibentry_fn, void *cookie )
/**********************************************************************************************/
{
    orl_return  retval;

    for( ;; ) {
        const std::string   arg = text.take_until( ", \t" );

        if( deflibentry_fn == nullptr ) {
            retval = ORL_OKAY;
        } else {
            retval = deflibentry_fn( arg.c_str(), cookie );
        }
        if( retval != ORL_OKAY || *text.contents != ',' )
            break;
        text.contents++;
    }
    return retval;
}

bool starts_with_nocase( const char *text, std::string_view word )
{
    return strnicmp( text, word.data(), word.size() ) == 0;
}

}  // namespace

orl_return CoffParseDrectve( const char *contents, size_t len, orl_note_callbacks *cbs, void *cookie )
/****************************************************************************************************/
{
    Drectve     text{ contents, len };

    text.eat_white();
    while( text.len > 0 ) {
        if( *text.contents != '-' && *text.contents != '/' )
            break;   // - should be start of token
        text.contents++;
        text.len--;
        const char  *cmd = text.contents;

        // the command ends at the colon
        text.contents += strncspn( text.contents, ":", text.len );
        text.len -= static_cast<std::size_t>( text.contents - cmd );
        text.contents++;
        text.len--;
        if( starts_with_nocase( cmd, "export" ) ) {
            if( ParseExport( text, cbs->export_fn, cookie ) != ORL_OKAY ) {
                break;
            }
        } else if( starts_with_nocase( cmd, "defaultlib" ) ) {
            if( ParseDefLibEntry( text, cbs->deflib_fn, cookie ) != ORL_OKAY ) {
                break;
            }
        } else if( starts_with_nocase( cmd, "entry" ) ) {
            if( ParseDefLibEntry( text, cbs->entry_fn, cookie ) != ORL_OKAY ) {
                break;
            }
        }
        text.eat_white();
    }
    return ORL_OKAY;
}
