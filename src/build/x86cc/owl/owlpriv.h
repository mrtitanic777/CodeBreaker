/****************************************************************************
*
*                            Open Watcom Project
*
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


#pragma once
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "bool.h"
#include "owl.h"
#include "owstring.h"
#include "owbuffer.h"
#include "owsymbol.h"
#include "owelf.h"
#include "owcoff.h"
#include "log.h"

typedef struct owl_info         owl_info;
typedef struct owl_file_info    owl_file_info;
typedef struct owl_section_info owl_section_info;
typedef struct owl_reloc_info   owl_reloc_info;
typedef struct owl_line_info    owl_line_info;

typedef uint_32                 owl_section_num;

struct owl_line_info {
    owl_offset                  addr;
    owl_line_num                line;
};

struct owl_reloc_info {
    owl_reloc_info              *next;
    owl_symbol_handle           symbol;
    owl_reloc_type              type;
    owl_offset                  location;
};

struct owl_info {
    owl_cpu                     cpu;
    owl_client_funcs            client_funcs;
    owl_file_info               *files;
};

struct owl_file_info {
    owl_file_info               *next;
    owl_handle                  info;
    owl_format                  format;
    owl_client_file             client_handle;
    owl_client_file             log;
    owl_file_type               type;
    owl_section_num             next_index;
    owl_string_table            *string_table;
    owl_symbol_table            *symbol_table;
    owl_section_info            *sections;
    owl_string_handle           name;
    unsigned                    num_symbols;
    union {
        elf_file_info           elf;            // format specific information
        coff_file_info          coff;
    } x;
};

struct owl_section_info {
    owl_section_info            *next;
    owl_file_handle             file;
    owl_string_handle           name;
    owl_section_type            type;
    owl_alignment               align;
    owl_section_num             index;          // not valid until header's written
    owl_buffer_handle           buffer;         // actual section contents
    owl_buffer_handle           linenum_buffer; // buffer for linenumber info
    owl_offset                  size;           // size of section in bytes
    owl_offset                  location;       // location next bytes to be written to
    unsigned                    num_relocs;     // number of relocs, if any
    unsigned                    num_linenums;   // number of line number records
    owl_symbol_handle           sym;            // symbol of section name
    owl_symbol_handle           comdat_sym;     // only global symbol for comdat section
    owl_section_info            *comdat_dep;    // comdat sections associate
    owl_reloc_info              *first_reloc;   // may be NULL of course
    owl_reloc_info              *last_reloc;
    union {
        elf_section_info        elf;
        coff_section_info       coff;
    } x;
};

inline void *ClientAlloc( BackRun &run, owl_file_info *a, size_t b ) { return ((a)->info->client_funcs.alloc( run, b )); }
inline void ClientFree( BackRun &run, owl_file_info *a, void *b ) { (a)->info->client_funcs.free( run, b ); }
inline int ClientWrite( BackRun &run, owl_file_info *a, const char *b, size_t c )
{
    return ((a)->info->client_funcs.write( run, (a)->client_handle, b, c ));
}


extern void OWLENTRY OWLSectionFree( BackRun& back, owl_section_handle section );

inline bool OwlLinkageGlobal( owl_sym_linkage x ) { return ( (x) == OWL_SYM_GLOBAL || (x) == OWL_SYM_WEAK || (x) == OWL_SYM_UNDEFINED ); }

inline bool OwlMetaSymbol( owl_sym_type x ) { return ( (x) == OWL_TYPE_SECTION || (x) == OWL_TYPE_FILE ); }

inline bool OwlSectionBSS( owl_section_info *x ) { return ( ( (x)->type & OWL_SEC_ATTR_BSS ) ) != 0; }

inline bool OwlSectionComdat( owl_section_info *x ) { return ( ( (x)->type & OWL_SEC_ATTR_COMDAT ) ) != 0; }
