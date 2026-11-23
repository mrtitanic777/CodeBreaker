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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "cvars.h"
#include <stdarg.h>
#include <errno.h>
#include "wio.h"
#include "exeelf.h"
#include "browsio.h"
#include "dwarfid.h"
#include "jmpbuf.h"
#include "frontrun.hpp"


#define C_DWARF_BUFSIZE 4096

typedef struct c_dw_section {
    unsigned        bufcount;
    char            **bufptrs;
    dw_out_offset   offset;
    dw_out_offset   length;
} C_DW_SECTION;


/*
 * -- code to generate ELF output ------------------------------------------
 *
 * note: the pre-initialized fields in these structures assume the following
 *       layout in the file
 *              elf_header
 *              string_table
 *              .debug_abbrev
 *              .debug_info
 *              .WATCOM_references
 *              .debug_line
 *              .debug_macinfo
 *              section_header_index0
 *              section_header_string_table
 *              section_header_template( .debug_abbrev )
 *              section_header_template( .debug_info )
 *              section_header_template( .WATCOM_references )
 *              section_header_template( .debug_line )
 *              section_header_template( .debug_macinfo )
 */

#define STR_NAME0           "\0"
#define STR_SHSTRTAB        ".shstrtab\0"
#define STR_DBG_ABBREV      ".debug_abbrev\0"
#define STR_DBG_INFO        ".debug_info\0"
#define STR_DBG_REF         ".WATCOM_references\0"
#define STR_DBG_LINE        ".debug_line\0"
#define STR_DBG_MACINFO     ".debug_macinfo\0"
#define OFF_STR_NAME0       (0)
#define OFF_STR_SHSTRTAB    (OFF_STR_NAME0 + sizeof( STR_NAME0 ) - 1)
#define OFF_STR_DBG_ABBREV  (OFF_STR_SHSTRTAB + sizeof( STR_SHSTRTAB ) - 1)
#define OFF_STR_DBG_INFO    (OFF_STR_DBG_ABBREV + sizeof( STR_DBG_ABBREV ) - 1)
#define OFF_STR_DBG_REF     (OFF_STR_DBG_INFO + sizeof( STR_DBG_INFO ) - 1)
#define OFF_STR_DBG_LINE    (OFF_STR_DBG_REF + sizeof( STR_DBG_REF ) - 1)
#define OFF_STR_DBG_MACINFO (OFF_STR_DBG_LINE + sizeof( STR_DBG_LINE ) - 1)
#define OFF_STR_MAX         (OFF_STR_DBG_MACINFO + sizeof( STR_DBG_MACINFO ) - 1)


static Elf32_Shdr const section_header_index0 = {
    0, SHT_NULL, 0, 0, 0, 0, SHN_UNDEF, 0, 0, 0
};


namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct BrowsioState {
    uint_32 relocValues[DW_W_MAX];
    C_DW_SECTION dw_sections[DW_DEBUG_MAX];
    Elf32_Ehdr elf_header = {
        { ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3, ELFCLASS32, ELFDATA2LSB, EV_CURRENT },
        ET_DYN,
        EM_386,
        EV_CURRENT,
        0,
        0,
        0,/* <offset of section table>=sizeof(Elf32_Ehdr)+sizeof(string_table)+sizes of 5 sections */
        0,
        sizeof( Elf32_Ehdr ),
        sizeof( Elf32_Phdr ),
        0,
        sizeof( Elf32_Shdr ),
        7,
        1
    };
    char string_table[OFF_STR_MAX + 1] = {
        STR_NAME0
        STR_SHSTRTAB
        STR_DBG_ABBREV
        STR_DBG_INFO
        STR_DBG_REF
        STR_DBG_LINE
        STR_DBG_MACINFO
    };
    unsigned string_table_offsets[5] = {
        OFF_STR_DBG_ABBREV,
        OFF_STR_DBG_INFO,
        OFF_STR_DBG_REF,
        OFF_STR_DBG_LINE,
        OFF_STR_DBG_MACINFO
    };
    Elf32_Shdr section_header_string_table = {
        OFF_STR_SHSTRTAB,
        SHT_STRTAB,
        0,
        0,
        sizeof( Elf32_Ehdr ),
        sizeof( string_table ),
        SHN_UNDEF,
        0,
        0,
        0
    };
    Elf32_Shdr section_header_template = {
        0, /* <index of name in string section> */
        SHT_PROGBITS,
        0,
        0,
        0, /* <offset of section in file> */
        0, /* <size of section> */
        SHN_UNDEF,
        0,
        0,
        0
    };
};
namespace {

}  // namespace



static void CFatal( const char *msg )
{
    printf( "%s\n", msg );
}

static void mywrite( FILE *fp, const void *data, size_t len )
{
    size_t wroteSize;

    wroteSize = fwrite( data, 1, len, fp );
    if( wroteSize < len ) {
        puts( strerror( errno ) );
        CFatal( "error on write" );
    }
}

/*
 * output DWARF sections in following order
 * .debug_abbrev
 * .debug_info
 * .WATCOM_reference
 * .debug_line
 * .debug_macinfo
 */
static const dw_sectnum inSect[] = {
    DW_DEBUG_ABBREV,
    DW_DEBUG_INFO,
    DW_DEBUG_REF,
    DW_DEBUG_LINE,
    DW_DEBUG_MACINFO
};

#define SECTION_COUNT   ArraySize( inSect )

static int createBrowseFile( FrontRun& front, FILE *browseFile )
{
    char            *ptr;
    size_t          bufsize;
    int             fileNum;
    unsigned        bufnum;
    dw_out_offset   sectionSize;
    dw_out_offset   sectionOffset[SECTION_COUNT];

    /*
     * calculate sections data size
     */
    front.browsio->elf_header.e_shoff = sizeof( Elf32_Ehdr ) + sizeof( front.browsio->string_table );
    for( fileNum = 0; fileNum < SECTION_COUNT; fileNum++ ) {
        front.browsio->elf_header.e_shoff += front.browsio->dw_sections[inSect[fileNum]].length;
    }
    /*
     * write elf header
     */
    mywrite( browseFile, &front.browsio->elf_header, sizeof( front.browsio->elf_header ) );
    /*
     * write string table
     */
    mywrite( browseFile, front.browsio->string_table, sizeof( front.browsio->string_table ) );
    /*
     * calculate each of the sections, tracking offset
     * write each of the sections
     */
    sectionOffset[0] = sizeof( front.browsio->elf_header ) + sizeof( front.browsio->string_table );
    for( fileNum = 0; fileNum < SECTION_COUNT; fileNum++ ) {
        if( fileNum > 0 )
            sectionOffset[fileNum] = sectionOffset[fileNum - 1] + front.browsio->dw_sections[inSect[fileNum - 1]].length;
        bufsize = C_DWARF_BUFSIZE;
        bufnum = 0;
        for( sectionSize = front.browsio->dw_sections[inSect[fileNum]].length; sectionSize > 0; sectionSize -= bufsize ) {
            if( bufsize > sectionSize )
                bufsize = sectionSize;
            ptr = front.browsio->dw_sections[inSect[fileNum]].bufptrs[bufnum];
            mywrite( browseFile, ptr, bufsize );
            CMemFree( front, ptr );
            ++bufnum;
        }
        if( front.browsio->dw_sections[inSect[fileNum]].bufptrs != NULL ) {
            CMemFree( front, front.browsio->dw_sections[inSect[fileNum]].bufptrs );
        }
    }
    /*
     * write section_header_index0
     */
    mywrite( browseFile, &section_header_index0, sizeof( section_header_index0 ) );
    /*
     * write section_header_string_table
     */
    mywrite( browseFile, &front.browsio->section_header_string_table, sizeof( front.browsio->section_header_string_table ) );
    /*
     * write rest of section headers
     */
    for( fileNum = 0; fileNum < SECTION_COUNT; fileNum++ ) {
        front.browsio->section_header_template.sh_name     = front.browsio->string_table_offsets[fileNum];
        front.browsio->section_header_template.sh_offset   = sectionOffset[fileNum];
        front.browsio->section_header_template.sh_size     = front.browsio->dw_sections[inSect[fileNum]].length;
        mywrite( browseFile, &front.browsio->section_header_template, sizeof( front.browsio->section_header_template ) );
    }
    return( 0 );
}

static void dw_write( BackRun& back, dw_sectnum sect, const void *block, size_t len )
/********************************************************************/
{
    FrontRun& front = front_of( back );
    unsigned            bufnum;
    unsigned            endbufnum;
    char                **newbufptrs;
    size_t              bufsize;
    char                *bufptr;

    bufnum = front.browsio->dw_sections[sect].offset / C_DWARF_BUFSIZE;
    endbufnum = ( front.browsio->dw_sections[sect].offset + len ) / C_DWARF_BUFSIZE;
    if( endbufnum >= front.browsio->dw_sections[sect].bufcount ) {
        newbufptrs = (char **)CMemAlloc( front, ( endbufnum + 1 ) * sizeof( char * ) );
        if( front.browsio->dw_sections[sect].bufptrs != NULL ) {
            memcpy( newbufptrs, front.browsio->dw_sections[sect].bufptrs, front.browsio->dw_sections[sect].bufcount * sizeof( char * ) );
            CMemFree( front, front.browsio->dw_sections[sect].bufptrs );
        }
        front.browsio->dw_sections[sect].bufptrs = newbufptrs;
        front.browsio->dw_sections[sect].bufcount = endbufnum + 1;
    }
    bufsize = C_DWARF_BUFSIZE - ( front.browsio->dw_sections[sect].offset % C_DWARF_BUFSIZE );
    front.browsio->dw_sections[sect].offset += len;
    if( front.browsio->dw_sections[sect].length < front.browsio->dw_sections[sect].offset ) {
        front.browsio->dw_sections[sect].length = front.browsio->dw_sections[sect].offset;
    }
    while( len != 0 ) {
        bufptr = front.browsio->dw_sections[sect].bufptrs[bufnum];
        if( bufptr == NULL ) {
            bufptr = (char *)CMemAlloc( front, C_DWARF_BUFSIZE );
            front.browsio->dw_sections[sect].bufptrs[bufnum] = bufptr;
        }
        bufptr += C_DWARF_BUFSIZE - bufsize;
        if( bufsize > len  )
            bufsize = len;
        memcpy( bufptr, block, bufsize );
        block = (char *)block + bufsize;
        len -= bufsize;
        ++bufnum;                       /* advance to next buffer */
        bufsize = C_DWARF_BUFSIZE;      /* buffer is full size */
    }
}

static dw_out_offset dw_tell( BackRun& back, dw_sectnum sect )
/*********************************************/
{
    FrontRun& front = front_of( back );
    return( front.browsio->dw_sections[sect].offset );
}

static void dw_reloc( BackRun& back, dw_sectnum sect, dw_reloc_type reloc_type, ... )
/********************************************************************/
{
    FrontRun& front = front_of( back );
    va_list         args;
    dw_targ_addr    targ_data;
    dw_targ_seg     seg_data;
    uint_32         u32_data;
    dw_sectnum      sect_no;
    SYMPTR          sym;

    va_start( args, reloc_type );
    switch( reloc_type ) {
    case DW_W_LABEL:
    case DW_W_DEFAULT_FUNCTION:
    case DW_W_ARANGE_ADDR:
    case DW_W_LOW_PC:
        u32_data = 0;   /* NOTE: assumes little-endian byte order */
        dw_write( back, sect, &u32_data, TARGET_NEAR_POINTER );
        break;
    case DW_W_HIGH_PC:
        u32_data = 1;   /* NOTE: assumes little-endian byte order */
        dw_write( back, sect, &u32_data, TARGET_NEAR_POINTER );
        break;
    case DW_W_UNIT_SIZE:
        u32_data = 1;
        dw_write( back, sect, &u32_data, sizeof( u32_data ) );
        break;
    case DW_W_STATIC:
        sym = va_arg( args, SYMPTR );
        targ_data = 0;
        dw_write( back, sect, &targ_data, sizeof( targ_data ) );
        break;
    case DW_W_SEGMENT:
        sym = va_arg( args, SYMPTR );
        seg_data = SymSegId( front, sym );
        dw_write( back, sect, &seg_data, sizeof( seg_data ) );
        break;
    case DW_W_SECTION_POS:
        sect_no = static_cast<dw_sectnum>( va_arg( args, int ) );   /* (an enum passed through ... arrives as int) */
        u32_data = dw_tell( back, sect_no );
        dw_write( back, sect, &u32_data, sizeof( u32_data ) );
        break;
    default:
        break;
    }
    va_end( args );
}

static void dw_seek( BackRun& back, dw_sectnum sect, dw_out_offset offset, int mode )
/********************************************************************/
{
    FrontRun& front = front_of( back );
    dw_out_offset new_offset;

    new_offset = offset;
    switch( mode ) {
    case DW_SEEK_SET:
        break;
    case DW_SEEK_CUR:
        new_offset = front.browsio->dw_sections[sect].offset + offset;
        break;
    case DW_SEEK_END:
        new_offset = front.browsio->dw_sections[sect].length + offset;
        break;
    }
    if( front.browsio->dw_sections[sect].offset != new_offset ) {
        front.browsio->dw_sections[sect].offset = new_offset;
        if( front.browsio->dw_sections[sect].length < front.browsio->dw_sections[sect].offset ) {
            front.browsio->dw_sections[sect].length = front.browsio->dw_sections[sect].offset;
        }
    }
}

static void *dw_alloc( BackRun& back, size_t size )
/**********************************/
{
    FrontRun& front = front_of( back );
    char        *p;

    p = static_cast<char *>(CMemAlloc( front, size ));
    memset( p, 0xA5, size );
    return( p );
}

static void dw_free( BackRun& back, void *ptr )
/******************************/
{
    FrontRun& front = front_of( back );
    CMemFree( front, ptr );
}

dw_client DwarfInit( FrontRun& front )
/*************************/
{
    dw_client       client;
    dw_init_info    info;
    dw_cu_info      cu;
    size_t          incsize;
    char            *inclist;
    char            *fname;
    dw_sectnum      sect;
    static const dw_funcs cli_funcs = {
        dw_reloc,
        dw_write,
        dw_seek,
        dw_tell,
        dw_alloc,
        dw_free
    };

    for( sect = static_cast<dw_sectnum>(0); sect < DW_DEBUG_MAX; ++sect ) {
        front.browsio->dw_sections[sect].bufcount  = 0;
        front.browsio->dw_sections[sect].bufptrs   = NULL;
        front.browsio->dw_sections[sect].offset    = 0;
        front.browsio->dw_sections[sect].length    = 0;
    }
    info.big_endian = ( (front.cvars.GenSwitches & CGSW_GEN_OBJ_ENDIAN_BIG) != 0 );
    info.language = DW_LANG_C89;
    info.compiler_options = DW_CM_BROWSER;
    info.producer_name = DWARF_PRODUCER_ID " V1";
    memcpy( JMPBUF_PTR( info.exception_handler ), front.cvars.Environment, sizeof( jmp_buf ) );
    info.funcs = cli_funcs;
    info.abbrev_sym = NULL;

    front.browsio->relocValues[DW_W_LOW_PC] = 0x0;
    front.browsio->relocValues[DW_W_HIGH_PC] = 0x1;
    front.browsio->relocValues[DW_W_UNIT_SIZE] = 0x1;

    client = DWInit( *front.back, &info );
    if( client == NULL ) {
        CFatal( "dwarf: error in DWInit()" );
    }
    fname = FNameFullPath( front, front.cvars.FNames );
    incsize = 0;
    inclist = NULL;
    cu.source_filename = fname;
    cu.directory       = "";
    cu.flags           = DW_CU_FLAG_CONTIGUOUS;
    cu.offset_size     = TARGET_NEAR_POINTER;
    cu.segment_size    = 0;
    cu.model           = DW_MEM_MODEL_none;
    cu.inc_list        = inclist;
    cu.inc_list_len    = incsize;
    cu.dbg_pch         = NULL;
    DWBeginCompileUnit( *front.back, client, &cu );
    CMemFree( front, inclist );
    DWDeclFile( *front.back, client, fname );
    return( client );
}

void DwarfFini( FrontRun& front, dw_client client )
/********************************/
{
    FILE    *out_file;

    DWEndCompileUnit( *front.back, client );
    DWFini( *front.back, client );

    out_file = OpenBrowseFile(front);
    if( out_file != NULL ) {
        /*
         * concatenate files
         */
        createBrowseFile( front, out_file );
        front.held->fclose( out_file );
    }
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
BrowsioState* browsio_make() { return new BrowsioState(); }
void browsio_free(BrowsioState* part) { delete part; }
