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
* Description:  Emit ELF object.
*
****************************************************************************/


#include "owlpriv.h"
#include "owreloc.h"
#include "backrun.hpp"

#define MAX_SECTION_NAME        128

#define ELF_UNDEF_INDEX         0
#define ELF_STRING_INDEX        1       /* index of string table section */
#define ELF_SYMBOL_INDEX        2       /* index of symbol table section */
#define FIRST_USER_SECTION      3

inline int OWLIndexToELFIndex( int x ) { return ( (x) + FIRST_USER_SECTION ); }
inline unsigned int OWLIndexToELFIndex( unsigned int x ) { return ( (x) + FIRST_USER_SECTION ); }

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct OwelfState {
    bool useRela;   // Use .rela or .rel
};
namespace {

}  // namespace



static unsigned numSections( owl_file_handle file )
/*************************************************/
{
    return( OWLIndexToELFIndex( file->next_index ) );
}

static unsigned numSymbols( owl_file_handle file )
/************************************************/
{
    return( file->symbol_table->num_global_symbols +
                file->symbol_table->num_local_symbols );
}

static Elf32_Half const machineTypes[] = {
    #define OWL_CPU(c,e,o) e,
    OWL_CPUS
    #undef OWL_CPU
};

static void writeFileHeader( BackRun& back, owl_file_handle file )
/*************************************************/
{
    Elf32_Ehdr          header;

    memset( &header.e_ident[0], 0, sizeof( header.e_ident ) );
    header.e_ident[EI_MAG0] = ELFMAG0;
    header.e_ident[EI_MAG1] = ELFMAG1;
    header.e_ident[EI_MAG2] = ELFMAG2;
    header.e_ident[EI_MAG3] = ELFMAG3;
    header.e_ident[EI_CLASS] = ELFCLASS32;
    header.e_ident[EI_DATA] = ELFDATA2LSB;
    header.e_ident[EI_VERSION] = EV_CURRENT;
    header.e_type = ET_REL;
    header.e_machine = machineTypes[file->info->cpu];
    header.e_version = EV_CURRENT;
    header.e_entry = 0;                 // fixme - should allow client to set this
    header.e_phoff = 0;
    header.e_shoff = sizeof( header );  // follows immediately after the file header
    header.e_flags = 0;                 // FIXME - get right stuff from IBM buttheads
    header.e_ehsize = sizeof( header );
    header.e_phentsize = sizeof( Elf32_Phdr );
    header.e_phnum = 0;
    header.e_shentsize = sizeof( Elf32_Shdr );
    header.e_shnum = numSections( file );
    header.e_shstrndx = ELF_STRING_INDEX;
    ClientWrite( back, file, reinterpret_cast<const char *>(&header), sizeof( header ) );
}

static void prepareStringTable( BackRun& back, owl_file_handle file, elf_special_section *str_sect )
/************************************************************************************
 * Before we can use indices for owl_string_handles, we
 * have to write the string table into a temporary buffer
 */
{
    str_sect->length = OWLStringTableSize( file->string_table );
    str_sect->buffer = static_cast<char *>(ClientAlloc( back, file,  str_sect->length ));
    OWLStringEmit( file->string_table, str_sect->buffer );
}

/*
 * must correspond to owl_sym_linkage in owl.h
 */
static Elf32_Half const elfBinding[] = {
    STB_GLOBAL,     /* OWL_SYM_UNDEFINED    */
    STB_LOCAL,      /* OWL_SYM_FUNCTION     */
    STB_LOCAL,      /* OWL_SYM_STATIC       */
    STB_GLOBAL,     /* OWL_SYM_GLOBAL       */
    STB_WEAK,       /* OWL_SYM_WEAK         */
};

/*
 * must correspond to owl_sym_type in owl.h
 */
static Elf32_Half const elfType[] = {
    STT_FUNC,       /* OWL_TYPE_FUNCTION    */
    STT_OBJECT,     /* OWL_TYPE_OBJECT      */
    STT_SECTION,    /* OWL_TYPE_SECTION     internal - should not be used by client */
    STT_FILE,       /* OWL_TYPE_FILE        internal - ditto */
};

static void emitElfSymbol( owl_symbol_info *symbol, Elf32_Sym *elf_sym )
/**********************************************************************/
{
    elf_sym->st_name = OWLStringOffset( symbol->name );
    elf_sym->st_value = symbol->offset;
    elf_sym->st_size = 0;
    elf_sym->st_info = Elf32StInfo( elfBinding[symbol->linkage], elfType[symbol->type] );
    elf_sym->st_other = 0;
    elf_sym->st_shndx = SHN_UNDEF;
    if( symbol->section != NULL ) {
        elf_sym->st_shndx = OWLIndexToELFIndex( symbol->section->index );
    }
}

static void emitBogusSymbol( Elf32_Sym *elf_sym ) 
/***********************************************/
{
    elf_sym->st_name = 0;
    elf_sym->st_value = 0;
    elf_sym->st_size = 0;
    elf_sym->st_info = 0;
    elf_sym->st_other = 0;
    elf_sym->st_shndx = SHN_UNDEF;
}

static void prepareSymbolTable( BackRun& back, owl_file_handle file, elf_special_section *sym_sect )
/************************************************************************************
 * Same as for prepareStringTable, but with symbols instead of strings. Since symbol
 * table entries are format-specific, we don't have a handy OWLSymbol call to do all
 * our work for us, so we just run the table converting everything
 */
{
    owl_symbol_info     *sym;
    unsigned            next_local_index;
    unsigned            next_global_index;
    Elf32_Sym           *elf_syms;

    sym_sect->length = ( numSymbols( file ) + 1 ) * sizeof( Elf32_Sym );
    sym_sect->buffer = static_cast<char *>(ClientAlloc( back, file, sym_sect->length ));
    elf_syms = reinterpret_cast<Elf32_Sym *>(sym_sect->buffer);
    emitBogusSymbol( elf_syms );
    next_local_index = 1;
    next_global_index = file->symbol_table->num_global_symbols + file->symbol_table->num_local_symbols;
    for( sym = file->symbol_table->head; sym != NULL; sym = sym->next ) {
        if( sym->flags & OWL_SYM_DEAD )
            continue;
        if( !(OwlLinkageGlobal( sym->linkage )) ) {
            sym->index = next_local_index++;
        } else {
            sym->index = next_global_index--;
        }
        emitElfSymbol( sym, &elf_syms[sym->index] );
    }
    assert( ( next_global_index + 1 ) == next_local_index );
    assert( next_local_index == ( file->symbol_table->num_local_symbols + 1 ) );
}

static void initSectionHeader( Elf32_Shdr *header, Elf32_Word type, Elf32_Word flags )
/************************************************************************************/
{
    header->sh_name = 0;
    header->sh_type = type;
    header->sh_flags = flags;
    header->sh_addr = 0;
    header->sh_offset = 0;
    header->sh_size = 0;
    header->sh_link = SHN_UNDEF;
    header->sh_info  = 0;
    header->sh_addralign = 0;
    header->sh_entsize = 0;
}

static void formatBogusUndefHeader( owl_file_handle file, Elf32_Shdr *header )
/****************************************************************************/
{
    /* unused parameters */ (void)file;

    initSectionHeader( header, SHT_NULL, 0 );
}

static void formatStringTableHeader( owl_file_handle file, Elf32_Shdr *header )
/*****************************************************************************/
{
    initSectionHeader( header, SHT_STRTAB, 0 );
    header->sh_name = OWLStringOffset( file->x.elf.string_table.name );
    header->sh_offset = file->x.elf.next_section;
    header->sh_size = file->x.elf.string_table.length;
    file->x.elf.next_section += file->x.elf.string_table.length;
}

static void formatSymbolTableHeader( owl_file_handle file, Elf32_Shdr *header )
/*****************************************************************************/
{
    elf_special_section *sym_tab;

    sym_tab = &file->x.elf.symbol_table;
    initSectionHeader( header, SHT_SYMTAB, 0 );
    header->sh_name = OWLStringOffset( sym_tab->name );
    header->sh_link = ELF_STRING_INDEX;
    header->sh_info = file->symbol_table->num_local_symbols + 1;
    header->sh_entsize = sizeof( Elf32_Sym );
    header->sh_offset = file->x.elf.next_section;
    header->sh_size = sym_tab->length;
    file->x.elf.next_section += sym_tab->length;
}


static Elf32_Word sectionTypes( owl_section_type type )
/*****************************************************/
{
    if( type & OWL_SEC_ATTR_CODE )
        return( SHT_PROGBITS );
    if( type & OWL_SEC_ATTR_DATA )
        return( SHT_PROGBITS );
    if( type & OWL_SEC_ATTR_BSS )
        return( SHT_NOBITS );
    if( type & OWL_SEC_ATTR_INFO )
        return( SHT_NOTE );
    return( SHT_NULL );
}

static Elf32_Word sectionFlags( owl_section_type type )
/*****************************************************/
{
    uint_32 flags = 0;

    if( type & OWL_SEC_ATTR_CODE )
        flags |= SHF_ALLOC;
    if( type & OWL_SEC_ATTR_DATA )
        flags |= SHF_ALLOC;
    if( type & OWL_SEC_ATTR_BSS )
        flags |= SHF_ALLOC;
    if( type & OWL_SEC_ATTR_PERM_WRITE )
        flags |= SHF_WRITE;
    if( type & OWL_SEC_ATTR_PERM_EXEC )
        flags |= SHF_EXECINSTR;
    return( flags );
}

static uint_32 sectionAlignment( owl_section_info *section )
/**********************************************************/
{
    uint_32     alignment = 0;

    switch( section->align ) {
    case  1:
    case  2:
    case  4:
    case  8:
    case 16:
    case 32:
    case 64:
        alignment = section->align;
        break;
    default:
        assert( 0 );    // must be power of two
        break;
    }
    return( alignment );
}

static owl_offset sectionPadding( owl_section_handle section, owl_offset offset )
/*******************************************************************************/
{
    owl_offset          mod;
    owl_offset          padding;

    padding = 0;
    if( section->align != 0 ) {
        mod = offset % section->align;
        if( mod != 0 ) {
            padding = section->align - mod;
        }
    }
    section->x.elf.pad_amount = padding;
    return( padding );
}

static void doSectionHeader( owl_section_handle section, Elf32_Shdr *header )
/***************************************************************************/
{
    initSectionHeader( header, sectionTypes( section->type ), sectionFlags( section->type ) );
    header->sh_name = OWLStringOffset( section->name );
    header->sh_addralign = sectionAlignment( section );
    header->sh_size = section->size + sectionPadding( section, section->size );
    if( (section->type & OWL_SEC_ATTR_BSS) == 0 ) {
        header->sh_offset = section->file->x.elf.next_section;
        section->file->x.elf.next_section += header->sh_size;
    }
}

static void doSectionRelocsHeader( BackRun& back, owl_section_handle section, Elf32_Shdr *header )
/*********************************************************************************/
{
    size_t      reloc_entry_size;

    reloc_entry_size = back.owelf->useRela ? sizeof( Elf32_Rela ) : sizeof( Elf32_Rel );
    initSectionHeader( header, (back.owelf->useRela ? SHT_RELA : SHT_REL), sectionFlags( section->type ) & SHF_ALLOC );
    header->sh_name = OWLStringOffset( section->x.elf.relocs_name );
    header->sh_link = ELF_SYMBOL_INDEX;
    header->sh_info = OWLIndexToELFIndex( section->index );
    header->sh_size = reloc_entry_size * section->num_relocs;
    header->sh_entsize = reloc_entry_size;
    header->sh_offset = section->file->x.elf.next_section;
    section->file->x.elf.next_section += header->sh_size;
}

static void formatUserSectionHeaders( BackRun& back, owl_file_handle file, Elf32_Shdr *headers )
/*******************************************************************************/
{
    owl_section_handle  curr;

    for( curr = file->sections; curr != NULL; curr = curr->next ) {
        doSectionHeader( curr, &headers[OWLIndexToELFIndex( curr->index )] );
        if( curr->first_reloc != NULL ) {
            doSectionRelocsHeader( back, curr, &headers[OWLIndexToELFIndex( curr->x.elf.relocs_index )] );
        }
    }
}

static void emitSectionHeaders( BackRun& back, owl_file_handle file )
/****************************************************/
{
    Elf32_Shdr          *headers;
    Elf32_Word          section_header_table_size;

    section_header_table_size = numSections( file ) * sizeof( Elf32_Shdr );
    file->x.elf.next_section = sizeof( Elf32_Ehdr ) + section_header_table_size;
    headers = static_cast<Elf32_Shdr *>(ClientAlloc( back, file, section_header_table_size ));
    formatBogusUndefHeader( file, &headers[ELF_UNDEF_INDEX] );
    formatStringTableHeader( file, &headers[ELF_STRING_INDEX] );
    formatSymbolTableHeader( file, &headers[ELF_SYMBOL_INDEX] );
    formatUserSectionHeaders( back, file, headers );
    ClientWrite( back, file, reinterpret_cast<const char *>(headers), section_header_table_size );
    ClientFree( back, file, headers );
}

static void emitSpecialSection( BackRun& back, owl_file_handle file, elf_special_section *section )
/**********************************************************************************/
{
    ClientWrite( back, file, section->buffer, section->length );
    ClientFree( back, file, section->buffer );
}

static void emitReloc( BackRun& back, owl_section_handle sec, owl_reloc_info *reloc, Elf32_Rela *elf_reloc )
/*******************************************************************************************/
{
    owl_offset          old_loc;
    unsigned_32         data;
    unsigned_32         bit_mask;
    size_t              data_size;

    elf_reloc->r_offset = reloc->location;
    elf_reloc->r_info = Elf32RInfo( reloc->symbol->index, ElfRelocType( back, reloc->type, sec->file->info->cpu ) );
    bit_mask = OWLRelocBitMask( back, sec->file, reloc );
    if( sec->file->info->cpu == OWL_CPU_X86 && reloc->type == OWL_RELOC_BRANCH_REL ) {
        /*
         * An x86 relative branch is based at the end of its four-byte
         * displacement, while R_386_PC32 uses the address of the relocation
         * field itself as P.  Supply A = -4 so S + A - P has the processor's
         * required base address.
         */
        old_loc = OWLBufferTell( sec->buffer );
        OWLBufferSeek( sec->buffer, reloc->location );
        OWLBufferRead( sec->buffer, reloc->location, reinterpret_cast<char *>(&data), sizeof( data ) );
        data = (data&~bit_mask)|(((data&bit_mask)-4)&bit_mask);
        OWLBufferWrite( back, sec->buffer, reinterpret_cast<char *>(&data), sizeof( data ) );
        OWLBufferSeek( sec->buffer, old_loc );
    }
    if( back.owelf->useRela ) {
        /*
         * dig up the embedded addend within the object and put it here
         */
        old_loc = OWLBufferTell( sec->buffer );
        OWLBufferSeek( sec->buffer, reloc->location );
        data = 0;
        data_size = OWLBufferSize( sec->buffer ) - reloc->location;
        if( data_size > 4 )
            data_size = 4;
        OWLBufferRead( sec->buffer, reloc->location, reinterpret_cast<char *>(&data), data_size );
        elf_reloc->r_addend = ( data & bit_mask );
        data &= ~bit_mask;
        OWLBufferWrite( back, sec->buffer, reinterpret_cast<char *>(&data), data_size );
        OWLBufferSeek( sec->buffer, old_loc );
    }
}

static void emitSectionPadding( BackRun& back, owl_section_info *curr )
/******************************************************/
{
    char                *buffer;
    owl_offset          padding;

    padding = curr->x.elf.pad_amount;
    if( padding != 0 ) {
        buffer = static_cast<char *>(ClientAlloc( back, curr->file, padding ));
        memset( buffer, 0, padding );
        ClientWrite( back, curr->file, buffer, padding );
        ClientFree( back, curr->file, buffer );
    }
}

static void emitSectionData( BackRun& back, owl_file_handle file )
/*************************************************/
{
    owl_section_handle  curr;
    owl_reloc_info      *reloc;
    owl_offset          relocs_size;
    Elf32_Rela          *reloc_buffer;
    Elf32_Rela          *next_reloc;
    size_t              reloc_entry_size;

    /*
     * Depending on useRela, the last field of Elf32_Rela might be unused.
     * We pass rela ptr around because it works for rel case also.
     * So be careful: don't do a ++next_reloc etc and expect it to be correct.
     */
    reloc_entry_size = back.owelf->useRela ? sizeof( Elf32_Rela ) : sizeof( Elf32_Rel );
    for( curr = file->sections; curr != NULL; curr = curr->next ) {
        /*
         * We need to prepare the reloc data before emitting its corresponding
         * section because we might need to modify the buffer content.
         * (see emitReloc)
         */
        if( curr->first_reloc != NULL ) {
            relocs_size = reloc_entry_size * curr->num_relocs;
            reloc_buffer = static_cast<Elf32_Rela *>(ClientAlloc( back, file, relocs_size ));
            next_reloc = reloc_buffer;
            for( reloc = curr->first_reloc; reloc != NULL; reloc = reloc->next ) {
                emitReloc( back, curr, reloc, next_reloc );
                next_reloc = reinterpret_cast<Elf32_Rela *>( static_cast<unsigned>(reinterpret_cast<pointer_uint>(next_reloc)) + reloc_entry_size );
            }
        } else {
            reloc_buffer = NULL;
            relocs_size = 0;
        }
        if( !OwlSectionBSS( curr ) ) {
            OWLBufferEmit( back, curr->buffer );
            emitSectionPadding( back, curr );
        }
        if( reloc_buffer != NULL ) {
            /*
             * Now write out the prepared reloc_buffer
             */
            ClientWrite( back, file, reinterpret_cast<const char *>(reloc_buffer), relocs_size );
            ClientFree( back, file, reloc_buffer );
        }
    }
}

static void prepareRelocSections( BackRun& back, owl_file_handle file )
/*******************************************************
 * Run through the user-defined sections noting which ones have
 * relocs and allocate a section index for the section which will
 * hold those relocs - also, alloc names for them in the string table
 */
{
    owl_section_handle  curr;
    char                buffer[MAX_SECTION_NAME + 5];

    switch( file->info->cpu ) {
    case OWL_CPU_X86:
    case OWL_CPU_MIPS:    // MIPS SysV ABI supplement says so
        back.owelf->useRela = false;
        break;
    default:
        back.owelf->useRela = true; // This seems to be the common case
        break;
    }
    for( curr = file->sections; curr != NULL; curr = curr->next ) {
        curr->x.elf.pad_amount = 0;
        if( curr->first_reloc != NULL ) {
            strcpy( buffer, (back.owelf->useRela ? ".rela" : ".rel") );
            strcat( buffer, OWLStringText( curr->name ) );
            curr->x.elf.relocs_name = OWLStringAdd( back, file->string_table, buffer );
            curr->x.elf.relocs_index = file->next_index++;
        }
    }
}

static void addSpecialStrings( BackRun& back, owl_file_handle file )
/***************************************************/
{
    file->x.elf.string_table.name = OWLStringAdd( back, file->string_table, ".strtab" );
    file->x.elf.symbol_table.name = OWLStringAdd( back, file->string_table, ".symtab" );
}

void ELFFileEmit( BackRun& back, owl_file_handle file )
/**************************************/
{
    prepareRelocSections( back, file );
    writeFileHeader( back, file );
    addSpecialStrings( back, file );
    prepareStringTable( back, file, &file->x.elf.string_table );
    prepareSymbolTable( back, file, &file->x.elf.symbol_table );
    emitSectionHeaders( back, file );
    emitSpecialSection( back, file, &file->x.elf.string_table );
    emitSpecialSection( back, file, &file->x.elf.symbol_table );
    emitSectionData( back, file );
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
OwelfState* owelf_make() { return new OwelfState(); }
void owelf_free(OwelfState* part) { delete part; }
