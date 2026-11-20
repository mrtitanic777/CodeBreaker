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
* Description:  Routines for producing DWARF debugging information.
*
****************************************************************************/


#include <setjmp.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstring>
#include <setjmp.h>
#include <string>
#include <string_view>
#include <vector>
#include "walloca.h"
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "objcalc.h"
#include "ring.h"
#include "exeelf.h"
#include "strtab.h"
#include "tistrail.h"
#include "dwarf.h"
#include "dw.h"
#include "loadfile.h"
#include "virtmem.h"
#include "objnode.h"
#include "dbgcomm.h"
#include "dbgdwarf.h"
#include "fileio.h"
#include "loadelf.h"
#include "specials.h"
#include "run_state.hpp"

#include "clibext.h"

#if 0      // disable this feature for now, to have compatibility with OW 1.9
#define WLINK_PRODUCER  DWARF_WATCOM_PRODUCER " WL"
#else
#define WLINK_PRODUCER  DWARF_WATCOM_PRODUCER
#endif

// seen through objstruc.h as DWARFMODINFO, so these stay at global scope

struct dwarfsect {
    virt_mem_ptr    u;
    virt_mem_size   size;
};

struct dwarfmodinfo {
    dwarfsect   pubsym;
    dwarfsect   dasi;
    dwarfsect   arange;
};

namespace {

// this used to hold information about linker generated debugging information.

struct sectioninfo {
    virt_mem        vm_ptr = 0;     // virt. mem. block containing genned info.
    virt_mem_size   size = 0;       // size of linker generated info.
    virt_mem_size   start = 0;      // start of linker generated info - compiler stuff 1st
    const char      *name = nullptr;
};

enum class Sect : unsigned {
    info,       // assumed to be in the same order as segment flags!
    abbrev,
    line,
    aranges,
};

constexpr std::size_t NUM_SECTIONS = 4;

constexpr std::size_t idx( Sect s )
{
    return static_cast<std::size_t>( s );
}

// what dbgdwarf.cpp keeps for one run of the linker
struct DwarfState {
    class_entry                             *dbiClass = nullptr;    // Assume there is only one!
    std::array<sectioninfo, NUM_SECTIONS>   sections{ {
        { 0, 0, 0, ".debug_info" },
        { 0, 0, 0, ".debug_abbrev" },
        { 0, 0, 0, ".debug_line" },
        { 0, 0, 0, ".debug_aranges" }
    } };

    sectioninfo &sect( Sect s ) { return sections[idx( s )]; }
};

DwarfState &dwarfState()
{
    return cb_wlink::RunState::part<DwarfState>();
}

constexpr std::string_view  PRODUCER = WLINK_PRODUCER;      // the literal's NUL follows the view

// die information

#include "pushpck1.h"

struct symbol_die {
    unsigned_8  abbrev_code;
    offset      off;
    unsigned_8  isexternal;
//    char      name[1];
};

struct symbol_seg {
    unsigned_8  len_u8;
    unsigned_8  loc_op;
    unsigned_16 seg;
};

// the abbrev codes used in the linker's autogenned stuff.

constexpr unsigned_8 NULL_ABBREV_CODE       = 0;
constexpr unsigned_8 COMPUNIT_ABBREV_CODE   = 1;
constexpr unsigned_8 CU_NOLINE_ABBREV_CODE  = 2;
constexpr unsigned_8 LABEL_ABBREV_CODE      = 3;
constexpr unsigned_8 VARIABLE_ABBREV_CODE   = 4;

constexpr auto SegmentedStandardAbbrevs = std::to_array<char>( {
    COMPUNIT_ABBREV_CODE,
    DW_TAG_compile_unit,
    DW_CHILDREN_yes,
    DW_AT_name,         DW_FORM_string,
    DW_AT_producer,     DW_FORM_string,
    DW_AT_stmt_list,    DW_FORM_ref_addr,
    0,                  0,
    CU_NOLINE_ABBREV_CODE,
    DW_TAG_compile_unit,
    DW_CHILDREN_yes,
    DW_AT_name,         DW_FORM_string,
    DW_AT_producer,     DW_FORM_string,
    0,                  0,
    LABEL_ABBREV_CODE,
    DW_TAG_label,
    DW_CHILDREN_no,
    DW_AT_low_pc,       DW_FORM_addr,
    DW_AT_external,     DW_FORM_flag,
    DW_AT_segment,      DW_FORM_block1,
    DW_AT_name,         DW_FORM_string,
    0,                  0,
    VARIABLE_ABBREV_CODE,
    DW_TAG_variable,
    DW_CHILDREN_no,
    DW_AT_low_pc,       DW_FORM_addr,
    DW_AT_external,     DW_FORM_flag,
    DW_AT_segment,      DW_FORM_block1,
    DW_AT_name,         DW_FORM_string,
    0,                  0
} );

constexpr auto FlatStandardAbbrevs = std::to_array<char>( {
    COMPUNIT_ABBREV_CODE,
    DW_TAG_compile_unit,
    DW_CHILDREN_yes,
    DW_AT_name,         DW_FORM_string,
    DW_AT_producer,     DW_FORM_string,
    DW_AT_stmt_list,    DW_FORM_ref_addr,
    0,                  0,
    CU_NOLINE_ABBREV_CODE,
    DW_TAG_compile_unit,
    DW_CHILDREN_yes,
    DW_AT_name,         DW_FORM_string,
    DW_AT_producer,     DW_FORM_string,
    0,                  0,
    LABEL_ABBREV_CODE,
    DW_TAG_label,
    DW_CHILDREN_no,
    DW_AT_low_pc,       DW_FORM_addr,
    DW_AT_external,     DW_FORM_flag,
    DW_AT_name,         DW_FORM_string,
    0,                  0,
    VARIABLE_ABBREV_CODE,
    DW_TAG_variable,
    DW_CHILDREN_no,
    DW_AT_low_pc,       DW_FORM_addr,
    DW_AT_external,     DW_FORM_flag,
    DW_AT_name,         DW_FORM_string,
    0,                  0
} );

#include "poppck.h"

constexpr std::array<unsigned_8, 9> STD_OPCODE_LENGTHS{ 0, 1, 1, 1, 1, 0, 0, 0, 0 };

inline std::size_t nameLength( const char *s )
{
    return std::string_view( s ).size();
}

}

void DwarfInit( void )
/***************************/
{
    DwarfState &st = dwarfState();

    st.dbiClass = nullptr;
    for( sectioninfo &s : st.sections ) {
        s.vm_ptr = 0;
        s.size = 0;
        s.start = 0;
    }
}

void DwarfInitModule( mod_entry *mod )
/*******************************************/
{
    mod->u3.d = static_cast<DWARFMODINFO *>( _PermAlloc( sizeof( dwarfmodinfo ) ) );
    std::memset( mod->u3.d, 0, sizeof( dwarfmodinfo ) );
}

virt_mem DwarfGetLineInfo( virt_mem_size *size )
/**********************************************/
{
    sectioninfo &line = dwarfState().sect( Sect::line );

    if( line.size ) {
        *size = line.size;
        return( line.vm_ptr );
    } else {
        *size = 0;
        return( 0 );
    }
}

void DwarfP1ModuleScanned( void )
/**************************************/
{
}

static int GetStmtHeaderSize( mod_entry *mod )
/********************************************/
{
    int         len;

    len = sizeof( stmt_prologue );  // fixed size
    len += 1 + nameLength( mod->name.u.ptr ) + 1 + 3 + 1;
    return( len );
}

static void DwarfAddLines( lineinfo *info )
/*****************************************/
// calculate the amount of space needed to hold all of the line # info.
{
    ln_off_pair _WCUNALIGNED *lineptr;
    unsigned_32         dwsize;
    std::array<unsigned_8, 3 + 2 * MAX_LEB128> buff;
    dw_linenum_delta    linedelta;
    dw_addr_delta       addrdelta;
    ln_off_386          prevline;
    unsigned            size;
    unsigned            item_size;

    prevline.off = 0;
    prevline.linnum = 1;
    if( wlink().FmtData.type & MK_16BIT ) {
        dwsize = 13;    // start and end sequence with 16-bit offset
    } else {
        dwsize = 15;    // start and end sequence with 32-bit offset
    }
    if( (wlink().FmtData.type & MK_SEGMENTED) == 0 ) {
        dwsize -= 5;    // size of LNE_segment burst
    }
    lineptr = reinterpret_cast<ln_off_pair *>( info->data );
    if( info->size & LINE_IS_32BIT ) {
        item_size = sizeof( ln_off_386 );
    } else {
        item_size = sizeof( ln_off_286 );
    }
    for( size = info->size & ~LINE_IS_32BIT; size > 0; size -= item_size ) {
        if( info->size & LINE_IS_32BIT ) {
            linedelta = static_cast<signed_32>( lineptr->_386.linnum ) - prevline.linnum;
            addrdelta = lineptr->_386.off - prevline.off;
        } else {
            linedelta = static_cast<signed_32>( lineptr->_286.linnum ) - prevline.linnum;
            addrdelta = lineptr->_286.off - prevline.off;
        }
        prevline.linnum += linedelta;
        prevline.off += addrdelta;
        dwsize += DWLineGen( linedelta, addrdelta, buff.data() ) - buff.data();
        lineptr = reinterpret_cast<ln_off_pair *>( reinterpret_cast<char *>( lineptr ) + item_size );
    }
    wlink().CurrMod->u3.d->dasi.size += dwsize;
}

void DwarfP1ModuleFinished( mod_entry *mod )
/*************************************************/
{
    DwarfState  &st = dwarfState();

    if( MOD_NOT_DEBUGGABLE( mod ) )
        return;
    wlink().CurrMod = mod;
    if( mod->flags_mod & MOD_DBI_SEEN )
        return;
    DBILineWalk( reinterpret_cast<lineinfo *>( mod->lines ), DwarfAddLines );
    Ring2Walk( mod->publist, DBIModGlobal );
    if( wlink().FmtData.type & MK_SEGMENTED ) {
        st.sect( Sect::abbrev ).size = SegmentedStandardAbbrevs.size();
    } else {
        st.sect( Sect::abbrev ).size = FlatStandardAbbrevs.size();
    }
    /* add length of compile unit header, abrrev code, module name, producer, stmts offset and reserve one byte for end */
    mod->u3.d->pubsym.u.vm_offs = st.sect( Sect::info ).size;
    mod->u3.d->pubsym.size += sizeof( comp_unit_prologue ) + sizeof( unsigned_8 ) + nameLength( mod->name.u.ptr ) + 1 + PRODUCER.size() + 1 + 1;
    if( mod->u3.d->dasi.size > 0 )
        mod->u3.d->pubsym.size += sizeof( unsigned_32 ); // DW_AT_STMT_LIST
    st.sect( Sect::info ).size += mod->u3.d->pubsym.size;
    /* add length of lines info */
    if( mod->u3.d->dasi.size > 0 ) {
        mod->u3.d->dasi.u.vm_offs = st.sect( Sect::line ).size;
        mod->u3.d->dasi.size += GetStmtHeaderSize( mod );
        st.sect( Sect::line ).size += mod->u3.d->dasi.size;
    }
}

void DwarfStoreAddrInfo( mod_entry *mod )
/**********************************************/
{
    sectioninfo &aranges = dwarfState().sect( Sect::aranges );

    if( (mod->flags_mod & MOD_DBI_SEEN) == 0 ) {
        if( mod->u3.d->arange.size > 0 ) {
            mod->u3.d->arange.u.vm_offs = aranges.size;
            mod->u3.d->arange.size += sizeof( arange_prologue );
            if( wlink().FmtData.type & MK_SEGMENTED ) {
                mod->u3.d->arange.size += sizeof( segmented_arange_tuple );
            } else {
                mod->u3.d->arange.size += sizeof( flat_arange_tuple );
            }
            aranges.size += mod->u3.d->arange.size;
        }
    }
}

void DwarfAddModule( mod_entry *mod, section *sect )
/**************************************************/
// this generates the headers needed in the individual sections
{
    DwarfState          &st = dwarfState();
    arange_prologue     arange_hdr;
    stmt_prologue       stmt_hdr;
    comp_unit_prologue  compuhdr;
    unsigned_8          abbrev_code;
    size_t              namelen;
    unsigned_32         stmt_list;

    /* unused parameters */ (void)sect;

    if( (mod->flags_mod & MOD_DBI_SEEN) == 0 ) {
        if( mod->u3.d->arange.size > 0 ) {
            mod->u3.d->arange.u.vm_ptr = st.sect( Sect::aranges ).vm_ptr + mod->u3.d->arange.u.vm_offs;
            arange_hdr.length = mod->u3.d->arange.size - sizeof( unsigned_32 );
            arange_hdr.version = 2;
            arange_hdr.debug_offset = mod->u3.d->pubsym.u.vm_offs + st.sect( Sect::info ).start;
            arange_hdr.offset_size = sizeof( offset );
            if( wlink().FmtData.type & MK_SEGMENTED ) {
                arange_hdr.segment_size = sizeof( segment );
            } else {
                arange_hdr.segment_size = 0;
            }
//          memset( arange_hdr.padding, 0, sizeof( arange_hdr.padding ) );
            PutInfo( mod->u3.d->arange.u.vm_ptr, &arange_hdr, sizeof( arange_prologue ) );
            mod->u3.d->arange.u.vm_ptr += sizeof( arange_prologue );
        }
        mod->u3.d->pubsym.u.vm_ptr = st.sect( Sect::info ).vm_ptr + mod->u3.d->pubsym.u.vm_offs;
        compuhdr.length = mod->u3.d->pubsym.size - sizeof( unsigned_32 );
        compuhdr.version = 2;
        compuhdr.abbrev_offset = st.sect( Sect::abbrev ).start;
        compuhdr.addr_size = sizeof( offset );
        PutInfo( mod->u3.d->pubsym.u.vm_ptr, &compuhdr, sizeof( comp_unit_prologue ) );
        mod->u3.d->pubsym.u.vm_ptr += sizeof( comp_unit_prologue );
        /* output abbrev code */
        if( mod->u3.d->dasi.size > 0 ) {
            abbrev_code = COMPUNIT_ABBREV_CODE;
        } else {
            abbrev_code = CU_NOLINE_ABBREV_CODE;
        }
        PutInfo( mod->u3.d->pubsym.u.vm_ptr, &abbrev_code, sizeof( abbrev_code ) );
        mod->u3.d->pubsym.u.vm_ptr += sizeof( abbrev_code );
        /* output module name */
        namelen = nameLength( mod->name.u.ptr ) + 1;
        PutInfo( mod->u3.d->pubsym.u.vm_ptr, mod->name.u.ptr, namelen );
        mod->u3.d->pubsym.u.vm_ptr += namelen;
        /* output producer */
        PutInfo( mod->u3.d->pubsym.u.vm_ptr, PRODUCER.data(), PRODUCER.size() + 1 );
        mod->u3.d->pubsym.u.vm_ptr += PRODUCER.size() + 1;
        if( mod->u3.d->dasi.size > 0 ) {
            /* output stmts offset */
            stmt_list = mod->u3.d->dasi.u.vm_offs + st.sect( Sect::line ).start;
            PutInfo( mod->u3.d->pubsym.u.vm_ptr, &stmt_list, sizeof( unsigned_32 ) );
            mod->u3.d->pubsym.u.vm_ptr += sizeof( unsigned_32 );
            /* output stmts header */
            mod->u3.d->dasi.u.vm_ptr = st.sect( Sect::line ).vm_ptr + mod->u3.d->dasi.u.vm_offs;
            stmt_hdr.total_length = mod->u3.d->dasi.size - sizeof( unsigned_32 );
            stmt_hdr.version = 2;
            stmt_hdr.prologue_length = GetStmtHeaderSize( mod ) - offsetof( stmt_prologue, minimum_instruction_length );
            stmt_hdr.minimum_instruction_length = DW_MIN_INSTR_LENGTH;
            stmt_hdr.default_is_stmt = 1;
            stmt_hdr.line_base = DWLINE_BASE;
            stmt_hdr.line_range = DWLINE_RANGE;
            stmt_hdr.opcode_base = DWLINE_OPCODE_BASE;
            std::copy( STD_OPCODE_LENGTHS.begin(), STD_OPCODE_LENGTHS.end(), stmt_hdr.standard_opcode_lengths );
            PutInfo( mod->u3.d->dasi.u.vm_ptr, &stmt_hdr, sizeof( stmt_prologue ) );
            mod->u3.d->dasi.u.vm_ptr += sizeof( stmt_prologue );
            PutInfoNulls( mod->u3.d->dasi.u.vm_ptr, 1 );  // no include directories;
            mod->u3.d->dasi.u.vm_ptr += 1;
            // the file name, then no directory index, no time, no length
            std::string buff( namelen + 3, '\0' );
            std::memcpy( buff.data(), mod->name.u.ptr, namelen );
            PutInfo( mod->u3.d->dasi.u.vm_ptr, buff.data(), namelen + 3 );
            mod->u3.d->dasi.u.vm_ptr += namelen + 3;
            PutInfoNulls( mod->u3.d->dasi.u.vm_ptr, 1 );  // no more file names
            mod->u3.d->dasi.u.vm_ptr += 1;
        }
    }
}

void DwarfGenModule( void )
/********************************/
// write out the addr info, and write out the null die at the end of .debug_info
{
    unsigned            size;

    if( wlink().CurrMod->flags_mod & MOD_DBI_SEEN )
        return;
    if( wlink().CurrMod->u3.d->arange.size > 0 ) {
        // write out terminator arange
        if( wlink().FmtData.type & MK_SEGMENTED ) {
            size = sizeof( segmented_arange_tuple );
        } else {
            size = sizeof( flat_arange_tuple );
        }
        PutInfoNulls( wlink().CurrMod->u3.d->arange.u.vm_ptr, size );
    }
    // write out terminator pubsym
    PutInfoNulls( wlink().CurrMod->u3.d->pubsym.u.vm_ptr, 1 );
}

static void DefAClass( void *_seg )
/*********************************/
{
    DwarfState  &st = dwarfState();
    seg_leader  *seg = static_cast<seg_leader *>( _seg );

    seg->group = AllocGroup( AutoGrpName, &wlink().DBIGroups );
    seg->group->addr.seg = 0;
    if( st.sect( Sect::info ).size > 0 ) {
        for( std::size_t index = 0; index < NUM_SECTIONS; index++ ) {
            if( stricmp( seg->segname.u.ptr, st.sections[index].name ) == 0 ) {
                seg->dbgtype = index + DWARF_DEBUG_INFO;
                st.sections[index].start = seg->size;
                break;
            }
        }
    }
}

void DwarfDefClass( class_entry *class_, unsigned_32 size )
/************************************************************/
// go through the list of dwarf segments, and make sure VM is allocated for
// all of them.
{
    /* unused parameters */ (void)size;

    if( class_->flags & CLASS_DWARF ) {
        dwarfState().dbiClass = class_;
        RingWalk( class_->segs, DefAClass );
    }
}

void DwarfAddGlobal( symbol *sym )
/********************************/
{
    wlink().CurrMod->u3.d->pubsym.size += nameLength( sym->name.u.ptr ) + sizeof( symbol_die ) + 1;
    if( wlink().FmtData.type & MK_SEGMENTED ) {
        wlink().CurrMod->u3.d->pubsym.size += sizeof( symbol_seg );
    }
}

static offset GetLinearGroupOffset( group_entry *group )
/******************************************************/
{
    /* gives the absolute offset for ELF, relative for PE and QNX_FLAT;
       perhaps PE and QNX_FLAT should be absolute as well if the
       DWARF format is changed */

    offset      off;

    off = 0;
    if( group != nullptr ) {
        off = group->linear - group->addr.off;
        if( wlink().FmtData.type & MK_ELF ) {
            off += wlink().FmtData.base;
        } else {
            off -= wlink().Groups->linear;
        }
    }
    return( off );
}

void DwarfGenGlobal( symbol *sym, section *sect )
/******************************************************/
{
    symbol_die  die;
    symbol_seg  symseg;
    virt_mem    vmem_addr;
    size_t      len;

    /* unused parameters */ (void)sect;

    if( (wlink().CurrMod->flags_mod & MOD_DBI_SEEN) == 0 ) {
        if( ( sym->p.seg == nullptr )
          || ( sym->p.seg->iscode ) ) {
            die.abbrev_code = LABEL_ABBREV_CODE;
        } else {
            die.abbrev_code = VARIABLE_ABBREV_CODE;
        }
        vmem_addr = wlink().CurrMod->u3.d->pubsym.u.vm_ptr;
        die.off = sym->addr.off;
        if( wlink().FmtData.type & (MK_PE | MK_QNX_FLAT | MK_ELF) ) {
            die.off += GetLinearGroupOffset( sym->p.seg->u.leader->group );
        }
        die.isexternal = ( (sym->info & SYM_STATIC) == 0 );
        PutInfo( vmem_addr, &die, sizeof( symbol_die ) );
        vmem_addr += sizeof( symbol_die );
        if( wlink().FmtData.type & MK_SEGMENTED ) {
            symseg.len_u8 = 3;
            symseg.loc_op = DW_OP_const2u;
            symseg.seg = sym->addr.seg;
            PutInfo( vmem_addr, &symseg, sizeof( symbol_seg ) );
            vmem_addr += sizeof( symbol_seg );
        }
        len = nameLength( sym->name.u.ptr ) + 1;
        PutInfo( vmem_addr, sym->name.u.ptr, len );
        wlink().CurrMod->u3.d->pubsym.u.vm_ptr = vmem_addr + len;
    }
}

void DwarfGenLines( lineinfo *info )
/**********************************/
{
    ln_off_pair _WCUNALIGNED *lineptr;
    unsigned            dwsize;
    dw_linenum_delta    linedelta;
    dw_addr_delta       addrdelta;
    virt_mem            vmem_addr;
    ln_off_386          prevline;
    offset              off;
    std::array<unsigned_8, 3 + 2 * MAX_LEB128> buff;
    unsigned            size;
    segdata             *seg;
    unsigned            item_size;

    if( wlink().CurrMod->flags_mod & MOD_DBI_SEEN )
        return;

    seg = info->seg;
    prevline.off = 0;
    prevline.linnum = 1;
    vmem_addr = wlink().CurrMod->u3.d->dasi.u.vm_ptr;
    if( wlink().FmtData.type & MK_16BIT ) {
        dwsize = 3;
    } else {
        dwsize = 5;
    }
    buff[0] = 0;        // extended opcode
    buff[1] = dwsize;
    buff[2] = DW_LNE_set_address;
    if( wlink().FmtData.type & MK_16BIT ) {
        MPUT_16_UN( buff.data() + 3, seg->a.delta + seg->u.leader->seg_addr.off );
    } else {
        off = seg->a.delta + seg->u.leader->seg_addr.off;
        if( wlink().FmtData.type & (MK_PE | MK_QNX_FLAT | MK_ELF) ) {
            off += GetLinearGroupOffset( seg->u.leader->group );
        }
        MPUT_32_UN( buff.data() + 3, off );
    }
    dwsize += 2;
    PutInfo( vmem_addr, buff.data(), dwsize );
    vmem_addr += dwsize;
    if( wlink().FmtData.type & MK_SEGMENTED ) {
        buff[1] = 3;
        buff[2] = DW_LNE_WATCOM_set_segment_OLD;
//        buff[2] = DW_LNE_WATCOM_set_segment;
        MPUT_16_UN( buff.data() + 3, seg->u.leader->seg_addr.seg );
        PutInfo( vmem_addr, buff.data(), 5 );
        vmem_addr += 5;
    }
    if( info->size & LINE_IS_32BIT ) {
        item_size = sizeof( ln_off_386 );
    } else {
        item_size = sizeof( ln_off_286 );
    }
    lineptr = reinterpret_cast<ln_off_pair *>( info->data );
    for( size = info->size & ~LINE_IS_32BIT; size > 0; size -= item_size ) {
        if( info->size & LINE_IS_32BIT ) {
            linedelta = static_cast<signed_32>( lineptr->_386.linnum ) - prevline.linnum;
            addrdelta = lineptr->_386.off - prevline.off;
        } else {
            linedelta = static_cast<signed_32>( lineptr->_286.linnum ) - prevline.linnum;
            addrdelta = lineptr->_286.off - prevline.off;
        }
        prevline.linnum += linedelta;
        prevline.off += addrdelta;
        dwsize = DWLineGen( linedelta, addrdelta, buff.data() ) - buff.data();
        PutInfo( vmem_addr, buff.data(), dwsize );
        vmem_addr += dwsize;
        lineptr = reinterpret_cast<ln_off_pair *>( reinterpret_cast<char *>( lineptr ) + item_size );
    }
    buff[0] = 0;        // extended opcode
    buff[1] = 1;        // size 1
    buff[2] = DW_LNE_end_sequence;
    PutInfo( vmem_addr, buff.data(), 3 );
    wlink().CurrMod->u3.d->dasi.u.vm_ptr = vmem_addr + 3;
}

static void DwarfAddAddrInit( segdata *sdata, void *cookie )
/**********************************************************/
{
    /* unused parameters */ (void)sdata; (void)cookie;
}

static void DwarfAddAddrAdd( segdata *sdata, offset delta, offset size,
                             void *cookie, bool isnewmod )
/********************************************************/
{
    /* unused parameters */ (void)delta; (void)size; (void)cookie;

    if( isnewmod
      && (sdata->o.mod->flags_mod & MOD_DBI_SEEN) == 0 ) {
        if( wlink().FmtData.type & MK_SEGMENTED ) {
            sdata->o.mod->u3.d->arange.size += sizeof( segmented_arange_tuple );
        } else {
            sdata->o.mod->u3.d->arange.size += sizeof( flat_arange_tuple );
        }
    }
}

void DwarfAddAddrInfo( seg_leader *seg )
/*********************************************/
{
    DBIAddrInfoScan( seg, DwarfAddAddrInit, DwarfAddAddrAdd, nullptr );
}

static offset GetNewAddrOffset( segdata *sdata, offset delta )
/************************************************************/
{
    offset      off;

    off = sdata->u.leader->seg_addr.off + delta;
    if( wlink().FmtData.type & (MK_PE | MK_QNX_FLAT | MK_ELF) ) {
        off += GetLinearGroupOffset( sdata->u.leader->group );
    }
    return( off );
}

static void DwarfGenAddrInit( segdata *sdata, void *_tuple )
/**********************************************************/
{
    arange_tuple *tuple = static_cast<arange_tuple *>( _tuple );

    if( wlink().FmtData.type & MK_SEGMENTED ) {
        tuple->s.v2.segment = sdata->u.leader->seg_addr.seg;
        tuple->s.v2.offset = sdata->u.leader->seg_addr.off + sdata->a.delta;
    } else {
        tuple->f.offset = GetNewAddrOffset( sdata, sdata->a.delta );
    }
}

static void DwarfGenAddrAdd( segdata *sdata, offset delta, offset size,
                             void *_tuple, bool isnewmod )
/*********************************************************************/
{
    arange_tuple *tuple = static_cast<arange_tuple *>( _tuple );
    mod_entry * mod;
    unsigned    tup_size;

    if( !isnewmod )
        return;
    mod = sdata->o.mod;
    if( (mod->flags_mod & MOD_DBI_SEEN) == 0 ) {
        if( wlink().FmtData.type & MK_SEGMENTED ) {
            tuple->s.v2.length = size;
            tup_size = sizeof( segmented_arange_tuple );
            if( sdata->u.leader->class_->flags & CLASS_STACK ) {
                tuple->s.v2.length = wlink().StackSize;
            } else if( sdata->u.leader->combine == COMBINE_STACK ) {
                if( (wlink().LinkState & LS_DOSSEG_FLAG) == 0 ) {
                    tuple->s.v2.length = wlink().StackSize;
                }
            }
        } else {
            tuple->f.length = size;
            tup_size = sizeof( flat_arange_tuple );
            if( sdata->u.leader->class_->flags & CLASS_STACK ) {
                tuple->f.length = wlink().StackSize;
            } else if( sdata->u.leader->combine == COMBINE_STACK ) {
                if( (wlink().LinkState & LS_DOSSEG_FLAG) == 0 ) {
                    tuple->f.length = wlink().StackSize;
                }
            }
        }
        PutInfo( mod->u3.d->arange.u.vm_ptr, tuple, tup_size );
        mod->u3.d->arange.u.vm_ptr += tup_size;
    }
    if( wlink().FmtData.type & MK_SEGMENTED ) {
        tuple->s.v2.offset = sdata->u.leader->seg_addr.off + delta;
    } else {
        tuple->f.offset = GetNewAddrOffset( sdata, delta );
    }
}

void DwarfGenAddrInfo( seg_leader *seg )
/*********************************************/
{
    arange_tuple tuple;

    DBIAddrInfoScan( seg, DwarfGenAddrInit, DwarfGenAddrAdd, &tuple );
}

void DwarfAddrSectStart( section *sect )
/*********************************************/
// This allocates space for the dwarf information
{
    DwarfState  &st = dwarfState();

    if( ( sect == wlink().Root )
      && ( st.sect( Sect::abbrev ).size > 0 ) ) {
        for( std::size_t index = 0; index < NUM_SECTIONS; index++ ) {
            if( index != idx( Sect::abbrev ) ) {
                st.sections[index].vm_ptr = DBIAlloc( st.sections[index].size );
            }
        }
    }
}

static void FillHeader( Elf32_Shdr *hdr, const char *name, stringtable *strtab,
                        unsigned_32 curr_off )
/***********************************************************************/
{
    hdr->sh_name = AddStringStringTableOffs( strtab, name );
    hdr->sh_type = SHT_PROGBITS;
    hdr->sh_flags = 0;
    hdr->sh_addr = 0;
    hdr->sh_offset = curr_off;
    hdr->sh_link = SHN_UNDEF;
    hdr->sh_info = 0;
    hdr->sh_addralign = 0;
    hdr->sh_entsize = 0;
}

static void WriteDwarfSect( unsigned addidx, unsigned_32 size )
/*************************************************************/
{
    if( addidx == idx( Sect::abbrev ) ) {
        if( wlink().FmtData.type & MK_SEGMENTED ) {
            WriteLoad( SegmentedStandardAbbrevs.data(), size );
        } else {
            WriteLoad( FlatStandardAbbrevs.data(), size );
        }
    } else {
        WriteInfoLoad( dwarfState().sections[addidx].vm_ptr, size );
    }
}

static unsigned_32 WriteELFSections( unsigned_32 file_off, unsigned_32 curr_off,
                                     Elf32_Shdr *hdr, stringtable *strtab )
/******************************************************************************/
// write out all of the individual section in the debugging information
{
    DwarfState  &st = dwarfState();
    seg_leader *seg;
    unsigned    addidx;
    unsigned_32 addsize;

    if( st.dbiClass != nullptr ) {
        for( seg = nullptr; (seg = static_cast<seg_leader *>( RingStep( st.dbiClass->segs, seg ) )) != nullptr; ) {
            addsize = 0;
            addidx = seg->dbgtype - DWARF_DEBUG_INFO;
            if( addidx < NUM_SECTIONS ) {
                addsize = st.sections[addidx].size;
            }
            if( seg->size != 0 ) {
                FillHeader( hdr, seg->segname.u.ptr, strtab, curr_off );
                hdr->sh_size = seg->size + addsize;
                WriteLeaderLoad( seg );
                if( addsize != 0 ) {
                    WriteDwarfSect( addidx, addsize );
                }
                curr_off += hdr->sh_size;
                file_off += hdr->sh_size;
                hdr++;
            }
        }
    }
    for( addidx = 0; addidx < NUM_SECTIONS; addidx++ ) {
        if( ( st.sections[addidx].size != 0 )
          && ( st.sections[addidx].start == 0 ) ) {
            FillHeader( hdr, st.sections[addidx].name, strtab, curr_off );
            hdr->sh_size = st.sections[addidx].size;
            WriteDwarfSect( addidx, hdr->sh_size );
            curr_off += hdr->sh_size;
            hdr++;
        }
    }
    return( curr_off );
}

static bool CheckDBISeg( void *seg, void *num )
/*********************************************/
{
    if( static_cast<seg_leader *>( seg )->size != 0 ) {
        ( *static_cast<int *>( num ) )++;
    }
    return( false );
}

int DwarfCountDebugSections( void )
/****************************************/
{
    DwarfState  &st = dwarfState();
    int num;

    num = 0;
    if( st.dbiClass != nullptr ) {
        RingLookup( st.dbiClass->segs, CheckDBISeg, &num );
    }
    for( const sectioninfo &s : st.sections ) {
        if( ( s.size != 0 )
          && ( s.start == 0 ) ) {
            num++;
        }
    }
#ifdef _EXE
    if( wlink().FmtData.type & MK_OVERLAYS ) {
        num++;
    }
#endif
    return( num );
}

offset DwarfWriteTrailer( offset curr_off )
/******************************************
 * write little-endian data
 */
{
    TISTrailer  trailer;

    strcpy( trailer.signature, TIS_TRAILER_SIGNATURE );
    MPUT_LE_32_UN( &trailer.vendor, TIS_TRAILER_VENDOR_TIS );
    MPUT_LE_32_UN( &trailer.type, TIS_TRAILER_TYPE_TIS_DWARF );
    MPUT_LE_32_UN( &trailer.size, curr_off + sizeof( TISTrailer ) );
    WriteLoad( &trailer, sizeof( TISTrailer ) );
    return( sizeof( TISTrailer ) );
}

offset DwarfWriteElf( offset curr_off, stringtable *strtab,
                                Elf32_Shdr *dbgsecbegin )
/*********************************************************/
{
    return( WriteELFSections( curr_off, curr_off, dbgsecbegin, strtab ) );
}

void DwarfWrite( void )
/*********************/
{
    Elf32_Ehdr          elf_header;
    Elf32_Shdr *        sh;
    size_t              shdr_size;
    unsigned_32         virt_off;
    stringtable         strtab;
    unsigned long       savepos;

    std::memcpy( elf_header.e_ident, ELF_SIGNATURE, ELF_SIGNATURE_LEN );
    elf_header.e_ident[EI_CLASS] = ELFCLASS32;
#ifdef __BIG_ENDIAN__
    elf_header.e_ident[EI_DATA] = ELFDATA2MSB;
#else
    elf_header.e_ident[EI_DATA] = ELFDATA2LSB;
#endif
    elf_header.e_ident[EI_VERSION] = EV_CURRENT;
    elf_header.e_ident[EI_OSABI] = ELFOSABI_NONE;
    elf_header.e_ident[EI_ABIVERSION] = 0;
    std::memset( &elf_header.e_ident[EI_PAD], 0, EI_NIDENT - EI_PAD );
    elf_header.e_type = ET_EXEC;
    elf_header.e_machine = EM_386;
    elf_header.e_version = EV_CURRENT;
    elf_header.e_entry = 0;
    elf_header.e_phoff = 0;
    elf_header.e_shoff = sizeof( elf_header );
    elf_header.e_flags = 0;
    elf_header.e_ehsize = sizeof( elf_header );
    elf_header.e_phentsize = sizeof( Elf32_Phdr );
    elf_header.e_phnum = 0;
    elf_header.e_shentsize = sizeof( Elf32_Shdr );
    elf_header.e_shnum = 2;     // reserve 2 for null header & string table
    elf_header.e_shnum += DwarfCountDebugSections();
    elf_header.e_shstrndx = elf_header.e_shnum - 1;     // last entry
    WriteLoad( &elf_header, sizeof( Elf32_Ehdr ) );
    shdr_size = sizeof( Elf32_Shdr ) * elf_header.e_shnum;
    std::vector<Elf32_Shdr> sect_header( elf_header.e_shnum );   // the first is the null section
    savepos = PosLoad() + shdr_size;
    SeekLoad( savepos );
    InitStringTable( &strtab, false );
    AddCharStringTable( &strtab, '\0' );
    virt_off = sizeof( Elf32_Ehdr ) + shdr_size;
    virt_off = WriteELFSections( savepos, virt_off, sect_header.data() + 1, &strtab );
    sh = sect_header.data() + elf_header.e_shstrndx;
    FillHeader( sh, ".shstrtab", &strtab, virt_off );
    sh->sh_type = SHT_STRTAB;
    sh->sh_size = GetStringTableSize( &strtab );
    WriteStringTable( &strtab, WriteLoad3, nullptr );
    FiniStringTable( &strtab );
    virt_off += sh->sh_size;
    virt_off += DwarfWriteTrailer( virt_off );
    SeekLoad( savepos - shdr_size );
    WriteLoad( sect_header.data(), shdr_size );
}
