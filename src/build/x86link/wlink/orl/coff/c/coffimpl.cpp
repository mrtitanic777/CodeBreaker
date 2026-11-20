/**************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Emulate long format of import library members in memory.
*               It is derived from short format of the import library members
*               in source .LIB file for each entry.
*               Emulated data is saved into coff_file_hnd structure.
*
****************************************************************************/


#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#include "coffimpl.h"
#include "cofforl.h"
#include "coffimpc.h"
#include "roundmac.h"
#include "implcoff.h"

#include "clibext.h"


/*
 * note before you use this for anything make sure that the values below
 * are large enough for your purposes (particularly the num sections and symbols.
 * the number of sections and symbols are fixed and suffice for import libraries)
 */

namespace {

constexpr unsigned  MAX_NUM_COFF_LIB_SECTIONS = 8;
constexpr unsigned  MAX_NUM_COFF_LIB_SYMBOLS = 32;

// The long form of an import library member is kept in the file handle's implib_data: a header with the length of the
// whole (this header included) and the position reading is at, and after it the member's bytes.
constexpr std::size_t   IMPLIB_HDR_SIZE = sizeof( std::size_t ) + sizeof( long );

class ImportLibData {
public:
    explicit ImportLibData( coff_file_handle coff_file_hnd ) : file_( coff_file_hnd ) {}

    std::size_t length() const
    {
        std::size_t     len;

        std::memcpy( &len, file_->implib_data, sizeof( len ) );
        return len;
    }
    void set_length( std::size_t len ) const
    {
        std::memcpy( file_->implib_data, &len, sizeof( len ) );
    }
    long position() const
    {
        long    pos;

        std::memcpy( &pos, file_->implib_data + sizeof( std::size_t ), sizeof( pos ) );
        return pos;
    }
    void set_position( long pos ) const
    {
        std::memcpy( file_->implib_data + sizeof( std::size_t ), &pos, sizeof( pos ) );
    }
    char *data() const
    {
        return file_->implib_data + IMPLIB_HDR_SIZE;
    }

private:
    coff_file_handle    file_;
};

// the member being made, before it is written out
struct CoffLibFile {
    coff_file_header                                        header{};
    std::array<coff_section_header, MAX_NUM_COFF_LIB_SECTIONS>  sections{};
    std::array<coff_symbol, MAX_NUM_COFF_LIB_SYMBOLS>       symbols{};
    std::string                                             strings;        // the string table, without its size
    coff_file_handle                                        coff_file_hnd = nullptr;

    void set_file( int processor, unsigned_32 time_stamp, unsigned_16 opt_hdr_size );
    void add_string( std::string_view name );
    signed_16 add_section( const char *name, unsigned_32 size, unsigned_16 num_relocs, unsigned_32 flags );
    coff_quantity add_symbol( signed_16 sec_num, std::string_view name, unsigned_32 value, unsigned_16 type,
                              unsigned_8 class_, unsigned_8 num_aux );
    coff_quantity add_sym_sec( signed_16 sec_num, unsigned_8 selection );
};

// what an import is made from
struct import_sym {
    coff_import_object_name_type    type;
    int                             processor;
    const char                      *DLLName;
    const char                      *exportedName;
    unsigned_32                     time_date_stamp;
    unsigned_16                     ordinal;
};

void CoffLibFile::set_file( int processor, unsigned_32 time_stamp, unsigned_16 opt_hdr_size )
{
    header.cpu_type = static_cast<uint_16>( processor );
    header.num_sections = 0;
    header.time_stamp = time_stamp;
    header.num_symbols = 0;
    header.opt_hdr_size = opt_hdr_size;
    header.flags = COFF_IMAGE_FILE_32BIT_MACHINE;
    strings.clear();
}

void CoffLibFile::add_string( std::string_view name )
{
    /*
     * (with the terminator character)
     */
    strings.append( name );
    strings.push_back( '\0' );
}

// the digits of the value that fit, the lowest ones, as wlink always wrote them (nothing for 0)
std::size_t format_digits( char *buf, std::size_t len, unsigned long value )
{
    if( len == 0 || value == 0 )
        return 0;
    std::string     digits = std::to_string( value );

    if( digits.size() > len ) {
        digits.erase( 0, digits.size() - len );
    }
    std::memcpy( buf, digits.data(), digits.size() );
    return digits.size();
}

signed_16 CoffLibFile::add_section( const char *name, unsigned_32 size, unsigned_16 num_relocs, unsigned_32 flags )
{
    /*
     * Sections numbering has index base 1
     */
    coff_section_header *section = sections.data() + header.num_sections++;
    const std::size_t   len = std::strlen( name );

    std::memset( section, 0, COFF_SECTION_HEADER_SIZE );
    if( len > COFF_SEC_NAME_LEN ) {
        section->name[0] = '/';
        format_digits( section->name + 1, COFF_SEC_NAME_LEN - 1, static_cast<unsigned_32>( strings.size() ) + 4 );
        add_string( std::string_view( name, len ) );
    } else {
        std::strncpy( section->name, name, COFF_SEC_NAME_LEN );
    }
    section->size = size;
    section->num_relocs = num_relocs;
    section->flags = flags;
    return header.num_sections;
}

/*
 * sec_num is 1-based
 */
coff_quantity CoffLibFile::add_symbol( signed_16 sec_num, std::string_view name, unsigned_32 value, unsigned_16 type,
                                       unsigned_8 class_, unsigned_8 num_aux )
{
    /*
     * Symbols numbering has index base 0
     */
    coff_symbol *sym = symbols.data() + header.num_symbols;

    if( name.size() > COFF_SYM_NAME_LEN ) {
        sym->name.u.non_name.zeros = 0;
        sym->name.u.non_name.offset = static_cast<unsigned_32>( strings.size() ) + 4;
        add_string( name );
    } else {
        std::memset( sym->name.u.name_string, 0, COFF_SYM_NAME_LEN );
        std::memcpy( sym->name.u.name_string, name.data(), name.size() );
    }
    sym->value = value;
    sym->sec_num = sec_num; /* 1-based, 0 is special value */
    sym->type = type;
    sym->storage_class = class_;
    sym->num_aux = num_aux;
    return header.num_symbols++;
}

/*
 * sec_num is 1-based
 */
coff_quantity CoffLibFile::add_sym_sec( signed_16 sec_num, unsigned_8 selection )
{
    /*
     * get section name
     * section number is 1-based, use 0-based index
     */
    const coff_section_header   *section = sections.data() + sec_num - 1;
    const std::string_view      name( section->name, strnlen( section->name, COFF_SEC_NAME_LEN ) );
    /*
     * add section symbol record
     */
    const coff_quantity     symb_idx = add_symbol( sec_num, name, 0x0, COFF_IMAGE_SYM_TYPE_NULL, COFF_IMAGE_SYM_CLASS_STATIC, 1 );
    /*
     * add section auxiliary record
     */
    coff_sym_section        *sym = reinterpret_cast<coff_sym_section *>( symbols.data() + header.num_symbols++ );

    sym->length = section->size;
    sym->num_relocs = section->num_relocs;
    sym->num_line_numbers = 0;
    sym->checksum = 0;
    if( selection == COFF_IMAGE_COMDAT_SELECT_ASSOCIATIVE ) {
        sym->number = sec_num;  /* 1-based, 0 is special value */
        sym->high_number = 0;
    } else {
        sym->number = 0;
        sym->high_number = 0;
    }
    sym->selection = selection;
    /*
     * return section symbol index
     */
    return symb_idx;
}

// (wlink gives the symbol its name as it was: the prefix is named to be put before it and never is)
coff_quantity add_symbol_with_prefix( CoffLibFile &c_file, signed_16 sec_num, const name_len &, const char *name,
                                      unsigned_32 value, unsigned_16 type, unsigned_8 class_, unsigned_8 num_aux )
/*********************************************************************************************************
 * sec_num is 1-based
 */
{
    return c_file.add_symbol( sec_num, name, value, type, class_, num_aux );
}

int DataImpLibInit( coff_file_handle coff_file_hnd )
{
    if( coff_file_hnd->implib_data == nullptr ) {
        coff_file_hnd->implib_data = static_cast<char *>( _ClientAlloc( coff_file_hnd, IMPLIB_HDR_SIZE ) );
        if( coff_file_hnd->implib_data == nullptr )
            return ORL_OUT_OF_MEMORY;
        const ImportLibData     implib( coff_file_hnd );

        implib.set_length( IMPLIB_HDR_SIZE );
        implib.set_position( 0 );
    }
    return ORL_OKAY;
}

int DataImpLibFini( coff_file_handle coff_file_hnd )
{
    if( coff_file_hnd->implib_data != nullptr ) {
        _ClientFree( coff_file_hnd, coff_file_hnd->implib_data );
        coff_file_hnd->implib_data = nullptr;
    }
    return ORL_OKAY;
}

int AddDataImpLib( coff_file_handle coff_file_hnd, const void *buff, std::size_t len )
{
    const ImportLibData     implib( coff_file_hnd );
    const std::size_t       old_len = implib.length();
    char                    *x = static_cast<char *>( _ClientAlloc( coff_file_hnd, old_len + len ) );

    if( x == nullptr )
        return ORL_OUT_OF_MEMORY;
    std::memcpy( x, coff_file_hnd->implib_data, old_len );
    std::memcpy( x + old_len, buff, len );
    _ClientFree( coff_file_hnd, coff_file_hnd->implib_data );
    coff_file_hnd->implib_data = x;
    implib.set_length( old_len + len );
    return ORL_OKAY;
}

void AddStringPadding( coff_file_handle coff_file_hnd, const name_len &str )
{
    const std::size_t   len = str.len + 1;

    AddDataImpLib( coff_file_hnd, str.name, len );
    if( len & 1 ) {
        AddDataImpLib( coff_file_hnd, "\0", 1 );
    }
}

void CreateCoffFileHeader( coff_file_handle coff_file_hnd, CoffLibFile &c_file )
{
    unsigned_32     d_ptr = sizeof( coff_file_header ) + c_file.header.opt_hdr_size + c_file.header.num_sections * COFF_SECTION_HEADER_SIZE;

    for( coff_quantity i = 0; i < c_file.header.num_sections; i++ ) {
        c_file.sections[i].rawdata_ptr = d_ptr;
        d_ptr += c_file.sections[i].size;
        c_file.sections[i].reloc_ptr = d_ptr;
        d_ptr += c_file.sections[i].num_relocs * COFF_RELOC_SIZE;
    }
    c_file.header.sym_table = d_ptr;
    AddDataImpLib( coff_file_hnd, &c_file.header, sizeof( coff_file_header ) );
}

void CreateCoffSections( coff_file_handle coff_file_hnd, CoffLibFile &c_file )
{
    AddDataImpLib( coff_file_hnd, c_file.sections.data(), c_file.header.num_sections * COFF_SECTION_HEADER_SIZE );
}

void CreateCoffSymbols( coff_file_handle coff_file_hnd, CoffLibFile &c_file )
{
    AddDataImpLib( coff_file_hnd, c_file.symbols.data(), c_file.header.num_symbols * COFF_SYM_SIZE );
}

void CreateCoffReloc( coff_file_handle coff_file_hnd, coff_sec_offset sec_offset, coff_quantity sym_tab_index, unsigned_16 type )
{
    /*
     * output is buffered so no point in putting COFF_RELOC struct
     */
    static_assert( sizeof( sec_offset ) + sizeof( sym_tab_index ) + sizeof( type ) == COFF_RELOC_SIZE );
    std::array<unsigned char, COFF_RELOC_SIZE>  record;

    std::memcpy( record.data(), &sec_offset, sizeof( sec_offset ) );
    std::memcpy( record.data() + sizeof( sec_offset ), &sym_tab_index, sizeof( sym_tab_index ) );
    std::memcpy( record.data() + sizeof( sec_offset ) + sizeof( sym_tab_index ), &type, sizeof( type ) );
    AddDataImpLib( coff_file_hnd, record.data(), record.size() );
}

void CreateCoffStringTable( coff_file_handle coff_file_hnd, CoffLibFile &c_file )
{
    const unsigned_32   table_size = static_cast<unsigned_32>( c_file.strings.size() ) + 4;

    AddDataImpLib( coff_file_hnd, &table_size, 4 );
    AddDataImpLib( coff_file_hnd, c_file.strings.data(), table_size - 4 );
}

// the name an import has in the import library: the exported name, as its type says
std::string getImportName( const char *src, coff_import_object_name_type type )
{
    /*
     * I got following information from Microsoft info about name type and
     * name conversion.
     *
     * Import by ordinal
     *   COFF_IMPORT_OBJECT_ORDINAL         = 0
     * Import name == public symbol name
     *   COFF_IMPORT_OBJECT_NAME            = 1
     * Import name == public symbol name skipping leading ?, @,
     * or optionally _
     *   COFF_IMPORT_OBJECT_NAME_NO_PREFIX  = 2
     * Import name == public symbol name skipping leading ?, @,
     * or optionally _ and truncating at first @
     *   COFF_IMPORT_OBJECT_NAME_UNDECORATE = 3
     *
     * Note:
     *   COFF_IMPORT_OBJECT_NAME_NO_PREFIX is used for C symbols
     *     with underscore as prefix
     *   COFF_IMPORT_OBJECT_NAME_UNDECORATE is used for __stdcall
     *     and __fastcall name mangling
     *   __stdcall uses underscore as prefix and @nn as suffix
     *   __fastcall uses @ as prefix and @nn as suffix
     *
     * this solution is simple, probably it needs improvement
     * no more information from Microsoft
     */
    std::size_t     len = 0;

    if( *src != 0 ) {
        const std::size_t   full_len = std::strlen( src );

        len = full_len;
        switch( type ) {
        case COFF_IMPORT_OBJECT_ORDINAL:
        case COFF_IMPORT_OBJECT_NAME:
            break;
        case COFF_IMPORT_OBJECT_NAME_UNDECORATE:
            /*
             * remove suffix @nn or @ if any
             */
            for( const char *end = src + full_len; end != src; ) {
                --end;
                if( *end < '0' || *end > '9' ) {
                    if( *end == '@' ) {
                        len -= src + full_len - end;
                    }
                    break;
                }
            }
            [[fallthrough]];
        case COFF_IMPORT_OBJECT_NAME_NO_PREFIX:
            /*
             * remove prefix @ or _ if any
             */
            if( ( *src == '@' ) || ( *src == '_' ) ) {
                src++;
                len--;
            }
            break;
        }
    }
    return std::string( src, len );
}

// The sections of an import: its code (the transfer through the import's address), by machine.
struct TextCode {
    const void      *code;
    std::size_t     size;
    unsigned_16     num_relocs;
    unsigned_32     align;
};

bool text_code_of( int processor, TextCode &text )
{
    switch( processor ) {
    case COFF_IMAGE_FILE_MACHINE_ALPHA:
        text = { CoffImportAxpText, sizeof( CoffImportAxpText ), 3, COFF_IMAGE_SCN_ALIGN_16BYTES };
        return true;
    case COFF_IMAGE_FILE_MACHINE_R3000:
    case COFF_IMAGE_FILE_MACHINE_R4000:
        text = { CoffImportMipsText, sizeof( CoffImportMipsText ), 3, COFF_IMAGE_SCN_ALIGN_16BYTES };
        return true;
    case COFF_IMAGE_FILE_MACHINE_POWERPC:
        text = { CoffImportPpcText, sizeof( CoffImportPpcText ), 1, COFF_IMAGE_SCN_ALIGN_4BYTES };
        return true;
    case COFF_IMAGE_FILE_MACHINE_AMD64:
        text = { CoffImportX64Text, sizeof( CoffImportX64Text ), 1, COFF_IMAGE_SCN_ALIGN_2BYTES };
        return true;
    case COFF_IMAGE_FILE_MACHINE_I386:
        text = { CoffImportX86Text, sizeof( CoffImportX86Text ), 1, COFF_IMAGE_SCN_ALIGN_2BYTES };
        return true;
    }
    return false;
}

constexpr unsigned_32   text_section_flags = COFF_IMAGE_SCN_LNK_COMDAT | COFF_IMAGE_SCN_CNT_CODE | COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_EXECUTE;
constexpr unsigned_32   data_section_flags = COFF_IMAGE_SCN_LNK_COMDAT | COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_MEM_READ | COFF_IMAGE_SCN_MEM_WRITE;

void AddDataImportTablesNamed( coff_file_handle coff_file_hnd, const import_sym &impsym, unsigned symb_hints )
{
    unsigned_16     type;

    switch( impsym.processor ) {
    case COFF_IMAGE_FILE_MACHINE_ALPHA:
        type = COFF_IMAGE_REL_ALPHA_REFLONGNB;
        break;
    case COFF_IMAGE_FILE_MACHINE_R3000:
    case COFF_IMAGE_FILE_MACHINE_R4000:
        type = COFF_IMAGE_REL_MIPS_REFWORDNB;
        break;
    case COFF_IMAGE_FILE_MACHINE_POWERPC:
        type = COFF_IMAGE_REL_PPC_ADDR32NB;
        break;
    case COFF_IMAGE_FILE_MACHINE_AMD64:
        type = COFF_IMAGE_REL_AMD64_ADDR32NB;
        break;
    case COFF_IMAGE_FILE_MACHINE_I386:
        type = COFF_IMAGE_REL_I386_DIR32NB;
        break;
    default:
        /*
         * TODO: should be handled somehow, now no reloc
         */
        type = 0;
        break;
    }
    const unsigned                  thunk_size = ( impsym.processor == COFF_IMAGE_FILE_MACHINE_AMD64 ) ? 8 : 4;
    const std::array<char, 8>       nulls{};

    /*
     * .idata$5 section data - name
     */
    AddDataImpLib( coff_file_hnd, nulls.data(), thunk_size );
    /*
     * .idata$5 relocations records
     */
    CreateCoffReloc( coff_file_hnd, 0x0000, symb_hints, type );
    /*
     * .idata$4 section data - name
     */
    AddDataImpLib( coff_file_hnd, nulls.data(), thunk_size );
    /*
     * .idata$4 relocations records
     */
    CreateCoffReloc( coff_file_hnd, 0x0000, symb_hints, type );
}

void AddDataImportTablesOrdinal( coff_file_handle coff_file_hnd, const import_sym &impsym )
{
    if( impsym.processor == COFF_IMAGE_FILE_MACHINE_AMD64 ) {
        const unsigned_64   import_table_entry = [&] {
            unsigned_64     entry;

            entry.u._64[0] = impsym.ordinal | COFF_IMAGE_ORDINAL_FLAG64;
            return entry;
        }();
        /*
         * .idata$5 section data - ordinal
         */
        AddDataImpLib( coff_file_hnd, &import_table_entry.u._64[0], 8 );
        /*
         * .idata$4 section data - ordinal
         */
        AddDataImpLib( coff_file_hnd, &import_table_entry.u._64[0], 8 );
        return;
    }
    // (the other machines have 32-bit thunks)
    const unsigned_32   entry32 = impsym.ordinal | COFF_IMAGE_ORDINAL_FLAG32;
    /*
     * .idata$5 section data - ordinal
     */
    AddDataImpLib( coff_file_hnd, &entry32, 4 );
    /*
     * .idata$4 section data - ordinal
     */
    AddDataImpLib( coff_file_hnd, &entry32, 4 );
}

int CoffCreateImport( coff_file_handle coff_file_hnd, const import_sym &impsym )
{
    CoffLibFile     c_file;

    c_file.coff_file_hnd = coff_file_hnd;

    const std::string   symbol_name = getImportName( impsym.exportedName, impsym.type );
    const name_len      DLLSymbolName{ symbol_name.size(), symbol_name.c_str() };

    unsigned    thunk_size = 4;
    unsigned_32 thunk_section_align = COFF_IMAGE_SCN_ALIGN_4BYTES;

    if( impsym.processor == COFF_IMAGE_FILE_MACHINE_AMD64 ) {
        thunk_size = 8;
        thunk_section_align = COFF_IMAGE_SCN_ALIGN_8BYTES;
    }

    c_file.set_file( impsym.processor, impsym.time_date_stamp, 0 );
    /*
     * .text section header
     */
    TextCode    text;

    if( !text_code_of( impsym.processor, text ) ) {
        return ORL_ERROR;
    }
    signed_16   sec_num = c_file.add_section( str_coff_text.name, static_cast<unsigned_32>( text.size ), text.num_relocs, text.align | text_section_flags );

    c_file.add_sym_sec( sec_num, COFF_IMAGE_COMDAT_SELECT_NODUPLICATES );
    coff_quantity   symb_name;

    if( impsym.processor == COFF_IMAGE_FILE_MACHINE_POWERPC ) {
        symb_name = add_symbol_with_prefix( c_file, sec_num, str_coff_ppc_prefix, impsym.exportedName, 0x0, COFF_IMAGE_SYM_TYPE_FUNCTION, COFF_IMAGE_SYM_CLASS_EXTERNAL, 0 );
    } else {
        symb_name = c_file.add_symbol( sec_num, impsym.exportedName, 0x0, COFF_IMAGE_SYM_TYPE_FUNCTION, COFF_IMAGE_SYM_CLASS_EXTERNAL, 0 );
    }

    const bool  is_named = ( impsym.type != COFF_IMPORT_OBJECT_ORDINAL );

    if( impsym.processor == COFF_IMAGE_FILE_MACHINE_POWERPC ) {
        /*
         * .pdata section header
         */
        sec_num = c_file.add_section( str_coff_pdata.name, sizeof( CoffImportPpcPdata ), 4, COFF_IMAGE_SCN_ALIGN_1BYTES | COFF_IMAGE_SCN_LNK_COMDAT | COFF_IMAGE_SCN_CNT_INITIALIZED_DATA | COFF_IMAGE_SCN_MEM_READ );
        c_file.add_sym_sec( sec_num, COFF_IMAGE_COMDAT_SELECT_ASSOCIATIVE );
        /*
         * .reldata section header
         */
        sec_num = c_file.add_section( str_coff_reldata.name, 8, 2, COFF_IMAGE_SCN_ALIGN_8BYTES | data_section_flags );
        if( is_named ) {
            c_file.add_sym_sec( sec_num, COFF_IMAGE_COMDAT_SELECT_NODUPLICATES );
        }
        c_file.add_symbol( sec_num, impsym.exportedName, 0x0, COFF_IMAGE_SYM_TYPE_NULL, COFF_IMAGE_SYM_CLASS_EXTERNAL, 0 );
    }

    /*
     * .idata$5 section header
     */
    sec_num = c_file.add_section( str_coff_idata5.name, thunk_size, is_named ? 1 : 0, thunk_section_align | data_section_flags );
    c_file.add_sym_sec( sec_num, COFF_IMAGE_COMDAT_SELECT_NODUPLICATES );
    const coff_quantity     symb_imp_name = add_symbol_with_prefix( c_file, sec_num, str_coff_imp_prefix, impsym.exportedName, 0x0, COFF_IMAGE_SYM_TYPE_NULL, COFF_IMAGE_SYM_CLASS_EXTERNAL, 0 );
    /*
     * .idata$4 section header
     */
    sec_num = c_file.add_section( str_coff_idata4.name, thunk_size, is_named ? 1 : 0, thunk_section_align | data_section_flags );
    c_file.add_sym_sec( sec_num, COFF_IMAGE_COMDAT_SELECT_ASSOCIATIVE );
    /*
     * .idata$6 section header
     */
    coff_quantity   symb_hints = 0;

    if( is_named ) {
        sec_num = c_file.add_section( str_coff_idata6.name, sizeof( impsym.ordinal ) + __ROUND_UP_SIZE_EVEN( DLLSymbolName.len + 1 ), 0, COFF_IMAGE_SCN_ALIGN_2BYTES | data_section_flags );
        symb_hints = c_file.add_sym_sec( sec_num, COFF_IMAGE_COMDAT_SELECT_ASSOCIATIVE );
    }
    /*
     * other symbols
     */
    coff_quantity   symb_toc = 0;

    if( impsym.processor == COFF_IMAGE_FILE_MACHINE_POWERPC ) {
        symb_toc = c_file.add_symbol( 0, str_coff_toc.name, 0x0, COFF_IMAGE_SYM_TYPE_NULL, COFF_IMAGE_SYM_CLASS_EXTERNAL, 0 );
    }
    add_symbol_with_prefix( c_file, 0, str_coff_import_descriptor, impsym.DLLName, 0x0, COFF_IMAGE_SYM_TYPE_NULL, COFF_IMAGE_SYM_CLASS_EXTERNAL, 0 );
    /*
     * COFF file header
     */
    CreateCoffFileHeader( coff_file_hnd, c_file );
    /*
     * COFF sections header
     */
    CreateCoffSections( coff_file_hnd, c_file );
    /*
     * COFF section data
     */
    /*
     * .text section data
     */
    AddDataImpLib( coff_file_hnd, text.code, text.size );
    /*
     * .text relocations records
     */
    switch( impsym.processor ) {
    case COFF_IMAGE_FILE_MACHINE_ALPHA:
        CreateCoffReloc( coff_file_hnd, 0x0000, symb_imp_name, COFF_IMAGE_REL_ALPHA_REFHI );
        CreateCoffReloc( coff_file_hnd, 0x0000, symb_name, COFF_IMAGE_REL_ALPHA_PAIR );
        CreateCoffReloc( coff_file_hnd, 0x0004, symb_imp_name, COFF_IMAGE_REL_ALPHA_REFLO );
        break;
    case COFF_IMAGE_FILE_MACHINE_R3000:
    case COFF_IMAGE_FILE_MACHINE_R4000:
        CreateCoffReloc( coff_file_hnd, 0x0000, symb_imp_name, COFF_IMAGE_REL_MIPS_REFHI );
        CreateCoffReloc( coff_file_hnd, 0x0000, symb_name, COFF_IMAGE_REL_MIPS_PAIR );
        CreateCoffReloc( coff_file_hnd, 0x0004, symb_imp_name, COFF_IMAGE_REL_MIPS_REFLO );
        break;
    case COFF_IMAGE_FILE_MACHINE_POWERPC:
        CreateCoffReloc( coff_file_hnd, 0x0000, symb_imp_name, COFF_IMAGE_REL_PPC_TOCREL14 | COFF_IMAGE_REL_PPC_TOCDEFN );
        /*
         * .pdata section data
         */
        AddDataImpLib( coff_file_hnd, CoffImportPpcPdata, sizeof( CoffImportPpcPdata ) );
        /*
         * .pdata relocations records
         */
        CreateCoffReloc( coff_file_hnd, 0x0000, symb_name, COFF_IMAGE_REL_PPC_ADDR32 );
        CreateCoffReloc( coff_file_hnd, 0x0004, symb_name, COFF_IMAGE_REL_PPC_ADDR32 );
        CreateCoffReloc( coff_file_hnd, 0x0010, symb_name, COFF_IMAGE_REL_PPC_ADDR32 );
        CreateCoffReloc( coff_file_hnd, 0x80410004, symb_name, COFF_IMAGE_REL_PPC_SECTION );
        {
            /*
             * .reldata section data
             */
            const std::array<char, 8>   nulls{};

            AddDataImpLib( coff_file_hnd, nulls.data(), 8 );
        }
        /*
         * .reldata relocations records
         */
        CreateCoffReloc( coff_file_hnd, 0x0000, symb_name, COFF_IMAGE_REL_PPC_ADDR32 );
        CreateCoffReloc( coff_file_hnd, 0x0004, symb_toc, COFF_IMAGE_REL_PPC_ADDR32 );
        break;
    case COFF_IMAGE_FILE_MACHINE_AMD64:
        CreateCoffReloc( coff_file_hnd, 0x0002, symb_imp_name, COFF_IMAGE_REL_AMD64_ADDR32NB );
        break;
    case COFF_IMAGE_FILE_MACHINE_I386:
        CreateCoffReloc( coff_file_hnd, 0x0002, symb_imp_name, COFF_IMAGE_REL_I386_DIR32 );
        break;
    }
    /*
     * .idata$5 and .idata$4 sections and relocations data
     */
    if( is_named ) {
        AddDataImportTablesNamed( coff_file_hnd, impsym, symb_hints );
    } else {
        AddDataImportTablesOrdinal( coff_file_hnd, impsym );
    }
    if( is_named ) {
        /*
         * .idata$6 section data - name
         */
        const unsigned_16   ordinal = impsym.ordinal;

        AddDataImpLib( coff_file_hnd, &ordinal, sizeof( ordinal ) );
        AddStringPadding( coff_file_hnd, DLLSymbolName );
    }
    /*
     * COFF symbol table
     */
    CreateCoffSymbols( coff_file_hnd, c_file );
    /*
     * COFF strings table
     */
    CreateCoffStringTable( coff_file_hnd, c_file );
    return ORL_OKAY;
}

}  // namespace

void *ImportLibRead( coff_file_handle coff_file_hnd, size_t len )
{
    const ImportLibData     implib( coff_file_hnd );

    implib.set_position( implib.position() + static_cast<long>( len ) );
    return implib.data() + implib.position() - len;
}

int ImportLibSeek( coff_file_handle coff_file_hnd, long pos, int where )
{
    const ImportLibData     implib( coff_file_hnd );

    if( where == SEEK_SET ) {
        implib.set_position( pos );
    } else if( where == SEEK_CUR ) {
        implib.set_position( implib.position() + pos );
    } else {
        implib.set_position( static_cast<long>( implib.length() ) - static_cast<long>( IMPLIB_HDR_SIZE ) - pos );
    }
    return 0;
}

int convert_import_library_init( coff_file_handle coff_file_hnd )
{
    /*
     * init import library data structure
     */
    if( DataImpLibInit( coff_file_hnd ) != ORL_OKAY )
        return ORL_OUT_OF_MEMORY;

    const auto  *i_hdr = reinterpret_cast<const coff_import_object_header *>( coff_file_hnd->f_hdr_buffer );
    import_sym  impsym;

    impsym.processor = i_hdr->machine;
    impsym.exportedName = static_cast<const char *>( ORL_PTR_READ( coff_file_hnd->coff_hnd, coff_file_hnd->io_hnd, i_hdr->size_of_data ) );
    impsym.DLLName = impsym.exportedName + std::strlen( impsym.exportedName ) + 1;
    impsym.time_date_stamp = i_hdr->time_date_stamp;
    impsym.type = static_cast<coff_import_object_name_type>( i_hdr->name_type );
    impsym.ordinal = i_hdr->oh.ordinal;
    return CoffCreateImport( coff_file_hnd, impsym );
}

int convert_import_library_fini( coff_file_handle coff_file_hnd )
{
    return DataImpLibFini( coff_file_hnd );
}
