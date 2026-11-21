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
* Description:  Map file formatting routines
*
****************************************************************************/


#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/map/map.h"
#include "dwarf.h"
#include "build/x86link/base/virtmem.h"
#include "exeelf.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/debuginfo/dbgdwarf.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


namespace {

/*
 * size of map file buffer
 */
constexpr std::size_t MAP_BUFFER_SIZE   = 16 * SECTOR_SIZE;
constexpr std::size_t MAP_LINEBUFF_SIZE = 4096;

struct line_state_info {
    unsigned_32         address;
    unsigned            file;
    unsigned_32         line;
    unsigned_32         column;
    unsigned_16         segment;
    unsigned_16         col;
    boolbit             is_stmt         : 1;
    boolbit             is_32           : 1;
    boolbit             has_seg         : 1;
    boolbit             basic_block     : 1;
    boolbit             end_sequence    : 1;
};

struct symrecinfo {
    symrecinfo          *next;
    symbol              *sym;
    mod_entry           *mod;
};

struct seg_info {
    unsigned    idx;
    seg_leader  *seg;
};

// what the map writer keeps for one run of the linker
struct MapState {
    symrecinfo                              *UndefList;
    symrecinfo                              *SymTraceList;
    std::size_t                             MapCol;
    time_t                                  StartT;
    clock_t                                 ClockTicks;
    bool                                    Absolute_Seg;
    unsigned long                           NumMapSyms;
    std::array<char, MAP_LINEBUFF_SIZE>     MapLineBuff;
};

MapState &map_state()
{
    return cb_wlink::RunState::part<MapState>();
}

constexpr std::string_view Blanks = "                                      ";

// two decimal digits of a number, appended to the text
void PutDec( std::string &out, unsigned num )
{
    out.push_back( static_cast<char>( (( num / 10 ) % 10) + '0' ) );
    out.push_back( static_cast<char>( (num % 10) + '0' ) );
}

const char *as_text( const unsigned_8 *p )
{
    return reinterpret_cast<const char *>( p );
}

} // namespace

void ResetWriteMapIO( void )
/**************************/
{
    MapState &st = map_state();

    wlink().MapFlags = static_cast<mapflag>( 0 );
    wlink().MapFName = nullptr;
    wlink().MapFile = nullptr;
    st.UndefList = nullptr;
    st.SymTraceList = nullptr;
}

static void WriteMap( const char *buffer, size_t len )
/*****************************************************
 * buffering the write to the map file
 */
{
    if( len > 0 ) {
        if( fwrite( buffer, len, 1, wlink().MapFile ) == 0 ) {
            LnkMsg( ERR+MSG_IO_PROBLEM, "12", wlink().MapFName, strerror( errno ) );
        }
    }
}

static void WriteMapNL( void )
/****************************/
{
    WriteMap( "\n", 1 );
    map_state().MapCol = 0;
}

static void WriteMapString( const char *buffer )
/**********************************************/
{
    WriteMap( buffer, strlen( buffer ) );
}

static void WriteMapMsgPrintf( int msgid, ... )
/*********************************************/
{
    MapState                        &st = map_state();
    std::array<char, RESOURCE_MAX_SIZE> format;
    va_list                         args;
    size_t                          len;

    Msg_Get( msgid, format.data() );
    va_start( args, msgid );
    len = DoFmtStr( st.MapLineBuff.data(), MAP_LINEBUFF_SIZE, format.data(), args );
    va_end( args );
    WriteMap( st.MapLineBuff.data(), len );
    WriteMapNL();
}

static void WriteMapMsg( int msgid )
/**********************************/
{
    MapState    &st = map_state();

    Msg_Get( msgid, st.MapLineBuff.data() );
    WriteMap( st.MapLineBuff.data(), strlen( st.MapLineBuff.data() ) );
    WriteMapNL();
}

static void WriteMapPrintf( const char *format, ... )
/***************************************************/
{
    MapState    &st = map_state();
    va_list     args;
    size_t      len;

    va_start( args, format );
    len = DoFmtStr( st.MapLineBuff.data(), MAP_LINEBUFF_SIZE, format, args );
    va_end( args );
    WriteMap( st.MapLineBuff.data(), len );
    WriteMapNL();
}

static void WriteMapBox( int msgid )
/**********************************/
{
    std::array<char, RESOURCE_MAX_SIZE> msg_buff;

    Msg_Get( msgid, msg_buff.data() );
    WriteMapNL();
    WriteMapNL();
    // '+', then one '-' for each character of the message but the first and last, then '+'
    std::string box( std::max<std::size_t>( strlen( msg_buff.data() ), 2 ), '-' );
    box.front() = '+';
    box.back() = '+';
    WriteMapPrintf( "%t24%s", "", box.c_str() );
    WriteMapPrintf( "%t24%s", "", msg_buff.data() );
    WriteMapPrintf( "%t24%s", "", box.c_str() );
    WriteMapNL();
}

static void WriteMapColPrintf( size_t col, const char *str, ... )
/***************************************************************/
{
    MapState        &st = map_state();
    va_list         args;
    size_t          num;
    size_t          len;

    num = 0;
    if( col > st.MapCol ) {
        num = col - st.MapCol;
    } else if( st.MapCol != 0 ) {
        num = 1;
    }
    st.MapCol += num;
    WriteMap( Blanks.data(), num );
    va_start( args, str );
    len = DoFmtStr( st.MapLineBuff.data(), MAP_LINEBUFF_SIZE, str, args );
    va_end( args );
    WriteMap( st.MapLineBuff.data(), len );
    st.MapCol += len;
}

static void WriteMapGroups( void )
/********************************/
{
    group_entry     *group;

    if( wlink().Groups != nullptr ) {
        WriteMapBox( MSG_MAP_BOX_GROUP );
        WriteMapMsg( MSG_MAP_TITLE_GROUP_0 );
        WriteMapMsg( MSG_MAP_TITLE_GROUP_1 );
        WriteMapNL();
        for( group = wlink().Groups; group != nullptr; group = group->next ) {
            if( !group->isautogrp ) { /* if not an autogroup */
                WriteMapColPrintf( 0, "%s", group->sym->name );
                WriteMapColPrintf( 32, "%a", &group->addr );
                WriteMapColPrintf( 53, "%h", group->totalsize );
                WriteMapNL();
            }
        }
        WriteMapNL();
    }
}

static void WriteMapAbsSeg( const seg_leader *leader )
/****************************************************/
{
    if( leader->info & SEGINF_ABSOLUTE ) {
        WriteMapColPrintf( 0, "%s", leader->segname );
        WriteMapColPrintf( 24, "%s", leader->class_->name );
        if( (wlink().FmtData.type & MK_16BIT)
          && (leader->info & SEGINF_USE_32) ) {
            WriteMapColPrintf( 40, "%A", &leader->seg_addr );
        } else {
            WriteMapColPrintf( 40, "%a", &leader->seg_addr );
        }
        WriteMapColPrintf( 60, "%h", leader->size );
        WriteMapNL();
    }
}

static void WriteMapNonAbsSeg( const seg_leader *seg )
/****************************************************/
{
    if( seg->info & SEGINF_ABSOLUTE ) {
        map_state().Absolute_Seg = true;
    } else {
        WriteMapColPrintf( 0, "%s", seg->segname );
        WriteMapColPrintf( 23, "%s", seg->class_->name );
        if( seg->group != nullptr ) {
            WriteMapColPrintf( 38, "%s", seg->group->sym->name );
        }
        if( (wlink().FmtData.type & MK_16BIT)
          && (seg->info & SEGINF_USE_32) ) {
            WriteMapColPrintf( 53, "%A", &seg->seg_addr );
        } else {
            WriteMapColPrintf( 53, "%a", &seg->seg_addr );
        }
        WriteMapColPrintf( 69, "%h", seg->size );
        WriteMapNL();
    }
}

static int cmp_seg( const void *a, const void *b )
/************************************************/
{
    const seg_info  *left = static_cast<const seg_info *>( a );
    const seg_info  *right = static_cast<const seg_info *>( b );

    if( left->seg->seg_addr.seg == right->seg->seg_addr.seg ) {
        if( left->seg->seg_addr.off == right->seg->seg_addr.off )
            return( left->idx - right->idx );
        return( left->seg->seg_addr.off - right->seg->seg_addr.off );
    }
    return( left->seg->seg_addr.seg - right->seg->seg_addr.seg );
}

static void WriteMapSectSegs( section *sect )
/********************************************
 * write segment info into map file
 */
{
    class_entry     *class_;
    size_t          count;
    seg_leader      *seg;

    if( sect->classes != nullptr ) {
        WriteMapBox( MSG_MAP_BOX_SEGMENTS );
        WriteMapMsg( MSG_MAP_TITLE_SEGMENTS_0 );
        WriteMapMsg( MSG_MAP_TITLE_SEGMENTS_1 );
        WriteMapNL();
        count = 0;
        for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
            if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) {
                count += RingCount( class_->segs );
            }
        }
        std::vector<seg_info> segs;
        segs.reserve( count );
        for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
            if( (class_->flags & CLASS_DEBUG_INFO) == 0 ) {
                for( seg = nullptr; (seg = static_cast<seg_leader *>( RingStep( class_->segs, seg ) )) != nullptr; ) {
                    segs.push_back( seg_info{ static_cast<unsigned>( segs.size() ), seg } );
                }
            }
        }
        qsort( segs.data(), segs.size(), sizeof( seg_info ), cmp_seg );
        for( const seg_info &s : segs ) {
            WriteMapNonAbsSeg( s.seg );
        }
        if( map_state().Absolute_Seg ) {
            WriteMapBox( MSG_MAP_BOX_ABS_SEG );
            WriteMapMsg( MSG_MAP_TITLE_ABS_SEG_0 );
            WriteMapMsg( MSG_MAP_TITLE_ABS_SEG_1 );
            WriteMapNL();
            for( const seg_info &s : segs ) {
                WriteMapAbsSeg( s.seg );
            }
        }
    }
}

static void WriteMapMemoryMapHead( void )
/***************************************/
{
    WriteMapBox( MSG_MAP_BOX_MEMORY_MAP );
    WriteMapMsg( MSG_MAP_UNREF_SYM );
    WriteMapMsg( MSG_MAP_REF_LOCAL_SYM );
    if( wlink().MapFlags & MAP_STATICS ) {
        WriteMapMsg( MSG_MAP_SYM_STATIC );
    }
    WriteMapNL();
    WriteMapMsg( MSG_MAP_TITLE_MEMORY_MAP_0 );
    WriteMapMsg( MSG_MAP_TITLE_MEMORY_MAP_1 );
    WriteMapNL();
}

void WriteMapHead( section *sect )
/********************************/
{
    WriteMapGroups();
    WriteMapSectSegs( sect );
    WriteMapMemoryMapHead();
}

static void WriteMapPubModHead( void )
/************************************/
{
    std::array<char, PATH_MAX> full_name;

    if( wlink().CurrMod->u1.source == nullptr ) {
        const char *name = wlink().CurrMod->name.u.ptr;
        std::copy_n( name, strlen( name ) + 1, full_name.begin() );
    } else {
        MakeFileName( wlink().CurrMod->u1.source->infile, full_name.data() );
    }
    WriteMapMsgPrintf( MSG_MAP_DEFINING_MODULE, full_name.data(), wlink().CurrMod->name.u.ptr );
}

static void WriteMapSym( symbol * sym, char star )
/************************************************/
{
    if( sym->info & SYM_STATIC ) {
        star = 's';
    }
    if( (wlink().FmtData.type & MK_16BIT)
      && ( sym->p.seg != nullptr )
      && ( sym->p.seg->bits == BITS_32 ) ) {
        WriteMapColPrintf( 0, "%A%c", &sym->addr, star );
    } else {
        WriteMapColPrintf( 0, "%a%c", &sym->addr, star );
    }
    WriteMapColPrintf( 15, "%S", sym );
}

static void WriteMapSymAddr( symbol *sym )
/****************************************/
{
    char                star;

    if( sym->info & SYM_REFERENCED ) {
        if( IS_SYM_IMPORTED( sym )
          || ((wlink().FmtData.type & MK_ELF)
          && IsSymElfImported( sym )) ) {
            star = 'i';
        } else {
            star = ' ';
        }
    } else if( sym->info & SYM_LOCAL_REF ) {
        star = '+';
    } else {
        star = '*';
    }
    WriteMapSym( sym, star );
    WriteMapNL();
}

void WriteMapLnkMsgCallback( unsigned msgnum, const char *str, size_t len )
/**************************************************************************
 * write link message to the map file
 */
{
    std::array<char, MAX_MSG_SIZE> msgprefix;
    size_t                         msgprefixlen;

    msgprefixlen = GetMsgPrefix( msgnum, msgprefix.data(), MAX_MSG_SIZE );
    WriteMap( msgprefix.data(), msgprefixlen );
    WriteMap( str, len );
    WriteMapNL();
}

void WriteMapOvlVectHead( vect_state *VectState )
/***********************************************/
{
    vecnode         *vectnode;
    int             n;
    addr_type       addr;
    symbol          *sym;

    WriteMapNL();
    WriteMapNL();
    WriteMapSymAddr( VectState->OverlayTable );
    WriteMapSymAddr( VectState->OverlayTableEnd );
    WriteMapSymAddr( VectState->OvlVecStart );
    WriteMapSymAddr( VectState->OvlVecEnd );
    WriteMapBox( MSG_MAP_BOX_OVERLAY_VECTOR );
    n = 1;
    for( vectnode = VectState->OvlVectors; vectnode != nullptr; vectnode = vectnode->next ) {
        OvlGetVecAddr( n++, &addr );
        sym = vectnode->sym;
        WriteMapPrintf( "%a section %d : %S",
            &addr, sym->p.seg->u.leader->class_->section->ovlref, sym );
    }
}


void WriteMapOvlPubHead( section *sect )
/**************************************/
{
    WriteMapNL();
    WriteMapNL();
    WriteMapPrintf( "Overlay section %d address %a", sect->ovlref, &sect->sect_addr );
    WriteMapPrintf( "====================================" );
    WriteMapSectSegs( sect );
    WriteMapMemoryMapHead();
}

static void WriteMapModulesSegmentsHead( void )
/*********************************************/
{
    WriteMapBox( MSG_MAP_BOX_MOD_SEG );
    if( map_state().Absolute_Seg ) {
        WriteMapMsg( MSG_MAP_ABS_ADDR );
    }
    WriteMapMsg( MSG_MAP_32BIT_SEG );
    WriteMapMsg( MSG_MAP_COMDAT );
    WriteMapNL();
    WriteMapMsg( MSG_MAP_TITLE_MOD_SEG_0 );
    WriteMapMsg( MSG_MAP_TITLE_MOD_SEG_1 );
    WriteMapNL();
}

static void WriteMapImports( void )
/*********************************/
{
    symbol          *sym;

    for( sym = wlink().HeadSym; sym != nullptr; sym = sym->next ) {
        if( IS_SYM_IMPORTED( sym ) ) {
            if( wlink().FmtData.type & MK_NOVELL ) {
                if( sym->p.import_nov != nullptr
                  && sym->p.import_nov != DUMMY_IMPORT_PTR ) {
                    if( sym->prefix != nullptr
                      && ( sym->prefix[0] != '\0' ) ) {
                        WriteMapColPrintf( 0, "%s@%s", sym->prefix, sym->name );
                    } else {
                        WriteMapColPrintf( 0, "%s", sym->name );
                    }
                    WriteMapNL();
                }
            } else {
                if( sym->p.import_dll != nullptr ) {
                    WriteMapColPrintf( 0, "%s", sym->name );
#ifdef _OS2
                    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
                        WriteMapColPrintf( 36, "%s", sym->p.import_dll->m.modnum->name.u.ptr );
                    }
#endif
                    WriteMapNL();
                }
            }
        }
    }
}

static void WriteMapImportsHead( void )
/*************************************/
{
    WriteMapBox( MSG_MAP_BOX_IMP_SYM );
    if( wlink().FmtData.type & (MK_NOVELL | MK_ELF) ) {
        WriteMapMsg( MSG_MAP_TITLE_IMP_SYM_0 );
        WriteMapMsg( MSG_MAP_TITLE_IMP_SYM_1 );
    } else {
        WriteMapMsg( MSG_MAP_TITLE_IMP_SYM_2 );
        WriteMapMsg( MSG_MAP_TITLE_IMP_SYM_3 );
    }
    WriteMapNL();
}

static void WriteMapSegment( void *_seg )
/****************************************
 * NYI: completely broken for absolute segments
 */
{
    segdata         *seg = static_cast<segdata *>( _seg );
    char            star;
    char            bang;
    char            see;
    addr_type       addr;
    seg_leader      *leader;

    if( seg->isdead )
        return;
    leader = seg->u.leader;
    if( leader->class_->flags & CLASS_DEBUG_INFO )
        return;
    WriteMapColPrintf( 16, "%s", leader->segname );
    WriteMapColPrintf( 38, "%s", leader->class_->name );
    star = ( leader->info & SEGINF_ABSOLUTE ) ? '*' : ' ';
    bang = ( seg->bits == BITS_32 ) ? '!' : ' ';
    see = ( seg->iscdat ) ? 'c' : ' ';
    addr = leader->seg_addr;
    addr.off += seg->a.delta;
    if( (wlink().FmtData.type & MK_16BIT) && ( seg->bits == BITS_32 ) ) {
        WriteMapColPrintf( 53, "%A%c%c%c", &addr, star, see, bang );
    } else {
        WriteMapColPrintf( 53, "%a%c%c%c", &addr, star, see, bang );
    }
    WriteMapColPrintf( 70, "%h", seg->length );
    WriteMapNL();
}

static void WriteMapModuleSegments( mod_entry *mod )
/**************************************************/
{
    if( (mod->flags_mod & MOD_NEED_PASS_2) && mod->segs != nullptr ) {
        WriteMapColPrintf( 0, "%s", mod->name.u.ptr );
        if( strlen( mod->name.u.ptr ) > 15 )
            WriteMapNL();
        Ring2Walk( mod->segs, WriteMapSegment );
    }
}

void WriteMapModulesSegments( void )
/**********************************/
{
    WriteMapModulesSegmentsHead();
    WalkMods( WriteMapModuleSegments );
}

static void InitDwarfVMState( line_state_info *state, bool default_is_stmt )
/**************************************************************************/
{
    state->address = 0;
    state->segment = 0;
    state->file = 1;
    state->line = 1;
    state->column = 0;
    state->is_stmt = default_is_stmt;
    state->basic_block = false;
    state->end_sequence = false;
}

static void DumpDwarfVMState( line_state_info *state )
/****************************************************/
{
    std::array<char, 40> str;
    const char           *fmt;

    if( state->has_seg ) {
        if( state->is_32 ) {
            fmt = "%5d %04Xh:%08Xh ";
        } else {
            fmt = "%5d %04Xh:%04Xh ";
        }
        snprintf( str.data(), str.size(), fmt, state->line, state->segment, state->address );
    } else {
        if( state->is_32 ) {
            fmt = "%6d %08Xh ";
        } else {
            fmt = "%6d %04Xh ";
        }
        snprintf( str.data(), str.size(), fmt, state->line, state->address );
    }
    WriteMapString( str.data() );
    state->col++;
    if( state->col == 4 ) {
        WriteMapNL();
        state->col = 0;
    }
}

static const unsigned_8 *DecodeULEB128( const unsigned_8 *input, unsigned_32 *value )
/************************************************************************************/
{
    unsigned_32     result;
    unsigned        shift;
    unsigned_8      byte;

    result = 0;
    shift = 0;
    for( ;; ) {
        byte = *input++;
        result |= (byte & 0x7f) << shift;
        if( (byte & 0x80) == 0 )
            break;
        shift += 7;
    }
    *value = result;
    return( input );
}

static const unsigned_8 *DecodeSLEB128( const unsigned_8 *input, signed_32 *value )
/**********************************************************************************/
{
    signed_32       result;
    unsigned        shift;
    unsigned_8      byte;

    result = 0;
    shift = 0;
    for( ;; ) {
        byte = *input++;
        result |= (byte & 0x7f) << shift;
        shift += 7;
        if( (byte & 0x80) == 0 ) {
            break;
        }
    }
    if( ( shift < 32 )
      && (byte & 0x40) ) {
        result |= - ( 1 << shift );
    }
    *value = result;
    return( input );
}

static const unsigned_8 *SkipLEB128( const unsigned_8 *input )
/************************************************************/
{
    unsigned_8      byte;

    do {
        byte = *input++;
    } while( byte & 0x80 );
    return( input );
}

static void WriteMapLines( void )
/*******************************/
{
    virt_mem                    input_vm;
    virt_mem_size               length;
    const unsigned_8            *p;
    int                         opcode_base;
    int                         i;
    unsigned                    u;
    const unsigned_8            *name;
    unsigned_32                 op_len;
    unsigned_32                 tmp;
    signed_32                   itmp;
    int                         line_range;
    int                         line_base;
    bool                        default_is_stmt;
    line_state_info             state;
    unsigned                    min_instr;
    unsigned_32                 unit_length;
    const unsigned_8            *unit_base;
    int                         value;
    dw_lne                      value_lne;
    dw_lns                      value_lns;

    input_vm = DwarfGetLineInfo( &length );
    if( input_vm == 0
      || length == 0 )
       return;

    std::vector<unsigned_8> input_buff( length );
    ReadInfo( input_vm, input_buff.data(), length );
    const unsigned_8 *input = input_buff.data();
    const std::ptrdiff_t end = static_cast<std::ptrdiff_t>( length );

    for( p = input; p - input < end; ) {
        state.col = 0;
        state.is_32 = false;
        state.has_seg = false;
        unit_length = MGET_U32_UN( p );
        p += sizeof( unsigned_32 );
        unit_base = p;

        p += sizeof( unsigned_16 );
        p += sizeof( unsigned_32 );
        min_instr = *p;
        p += 1;

        default_is_stmt = ( *p != 0 );
        p += 1;

        line_base = MGET_S8( p );
        p += 1;

        line_range = MGET_U8( p );
        p += 1;

        opcode_base = *p;
        p += 1;

        std::vector<unsigned> opcode_lengths( opcode_base > 1 ? static_cast<std::size_t>( opcode_base - 1 ) : 0 );
        for( i = 0; i < opcode_base - 1; ++i ) {
            opcode_lengths[i] = *p++;
        }

        if( p - input >= end ) {
            return;
        }

        while( *p != 0 ) {
            p += strlen( as_text( p ) ) + 1;
            if( p - input >= end ) {
                return;
            }
        }
        p++;
        while( *p != 0 ) {
            WriteMapNL();
            WriteMapPrintf( "Line numbers for %s", as_text( p ) );
            WriteMapNL();
            p += strlen( as_text( p ) ) + 1;
            p = SkipLEB128( p );
            p = SkipLEB128( p );
            p = SkipLEB128( p );
            if( p - input >= end ) {
                return;
            }
        }

        p++;
        InitDwarfVMState( &state, default_is_stmt );
        while( p - unit_base < static_cast<std::ptrdiff_t>( unit_length ) ) {
            value_lns = static_cast<dw_lns>( *p++ );
            if( value_lns == 0 ) {
                p = DecodeULEB128( p, &op_len );
                value_lne = static_cast<dw_lne>( *p++ );
                --op_len;
                switch( value_lne ) {
                case DW_LNE_end_sequence:
                    state.end_sequence = true;
                    InitDwarfVMState( &state, default_is_stmt );
                    p += op_len;
                    break;
                case DW_LNE_set_address:
                    if( op_len == 4 ) {
                        tmp = MGET_U32_UN( p );
                        state.is_32 = true;
                    } else if( op_len == 2 ) {
                        tmp = MGET_U16_UN( p );
                        state.is_32 = false;
                    } else {
                        tmp = 0xffffffff;
                    }
                    state.address = tmp;
                    p += op_len;
                    break;
                case DW_LNE_WATCOM_set_segment_OLD:
                case DW_LNE_WATCOM_set_segment:
                    state.has_seg = true;
                    if( op_len == 4 ) {
                        tmp = MGET_U32_UN( p );
                    } else if( op_len == 2 ) {
                        tmp = MGET_U16_UN( p );
                    } else {
                        tmp = 0xffffffff;
                    }
                    state.segment = tmp;
                    p += op_len;
                    break;
                case DW_LNE_define_file:
                    name = p;
                    p += strlen( as_text( p ) ) + 1;
                    p = SkipLEB128( p );
                    p = SkipLEB128( p );
                    p = SkipLEB128( p );
                    WriteMapPrintf( "File:  %s", as_text( name ) );
                    break;
                default:
                    p += op_len;
                    break;
                }
            } else if( value_lns < opcode_base ) {
                switch( value_lns ) {
                case DW_LNS_copy:
                    DumpDwarfVMState( &state );
                    state.basic_block = false;
                    break;
                case DW_LNS_advance_pc:
                    p = DecodeULEB128( p, &tmp );
                    state.address += tmp * min_instr;
                    break;
                case DW_LNS_advance_line:
                    p = DecodeSLEB128( p, &itmp );
                    state.line += itmp;
                    break;
                case DW_LNS_set_file:
                    p = DecodeULEB128( p, &tmp );
                    state.file = tmp;
                    break;
                case DW_LNS_set_column:
                    p = DecodeULEB128( p, &tmp );
                    state.column = tmp;
                    break;
                case DW_LNS_negate_stmt:
                    state.is_stmt = !state.is_stmt;
                    break;
                case DW_LNS_set_basic_block:
                    state.basic_block = true;
                    break;
                case DW_LNS_const_add_pc:
                    value = 255 - opcode_base;
                    state.address += ( value / line_range ) * min_instr;
                    break;
                case DW_LNS_fixed_advance_pc:
                    tmp = MGET_U16_UN( p );
                    p += sizeof( unsigned_16 );
                    state.address += tmp;
                    break;
                default:
                    for( u = opcode_lengths[value_lns - 1]; u > 0 ; --u ) {
                        p = SkipLEB128( p );
                    }
                }
            } else {
                value = value_lns - opcode_base;
                state.line += line_base + ( value % line_range );
                state.address += ( value / line_range ) * min_instr;
                DumpDwarfVMState( &state );
                state.basic_block = false;
            }
        }
        WriteMapNL();
    }
}

static bool CheckSymRecList( void *_info, void *sym )
/***************************************************/
{
    symrecinfo *info = static_cast<symrecinfo *>( _info );

    return( ( sym == info->sym ) && ( wlink().CurrMod == info->mod ) );
}

static void AddSymRecList( symbol *sym, symrecinfo **head )
/*********************************************************/
{
    symrecinfo      *info;

    if( RingLookup( *head, CheckSymRecList, sym ) == nullptr ) {
        info = static_cast<symrecinfo *>( MemAllocSafe( sizeof( symrecinfo ) ) );
        info->next = nullptr;
        info->sym = sym;
        info->mod = wlink().CurrMod;
        RingAppend( head, info );
    }
}

void RecordUndefinedSym( symbol *sym )
/************************************/
{
    if( (wlink().LinkFlags & LF_UNDEFS_ARE_OK) == 0 )
        wlink().LinkState |= LS_LINK_ERROR;
    AddSymRecList( sym, &map_state().UndefList );
}

static void PrintUndefinedSym( void *_info )
/******************************************/
{
    symrecinfo  *info = static_cast<symrecinfo *>( _info );
    mod_entry   *mod;

    mod = info->mod;
    LnkMsg( YELL+MSG_UNDEF_SYM, "12S", mod->u1.source->infile->name, mod->name, info->sym );
    if( wlink().MapFile != nullptr ) {
        WriteMapColPrintf( 0, "%S", info->sym );
        WriteMapColPrintf( 32, "%s(%s)", mod->u1.source->infile->name, mod->name );
        WriteMapNL();
    }
}

void PrintUndefinedSyms( void )
/*****************************/
{
    MapState    &st = map_state();

    if( st.UndefList != nullptr ) {
        if( wlink().MapFile != nullptr ) {
            WriteMapBox( MSG_MAP_BOX_UNRES_REF );
            WriteMapMsg( MSG_MAP_TITLE_UNRES_REF_0 );
            WriteMapMsg( MSG_MAP_TITLE_UNRES_REF_1 );
            WriteMapNL();
        }
        RingWalk( st.UndefList, PrintUndefinedSym );
    }
}

void WriteMapTracedSymRecord( symbol *sym )
/*****************************************/
{
    if( sym->mod != wlink().CurrMod ) {
        AddSymRecList( sym, &map_state().SymTraceList );
    }
}

static void WriteMapTracedSym( void *_info )
/******************************************/
{
    symrecinfo  *info = static_cast<symrecinfo *>( _info );

    WriteMapLnkMsg( MSG_MOD_TRACE, "Ss", info->sym, info->mod->name );
}

void WriteMapTracedSyms( void )
/*****************************/
{
    MapState    &st = map_state();

    if( st.SymTraceList != nullptr ) {
        WriteMapBox( MSG_MAP_BOX_TRACE_SYM );
        RingWalk( st.SymTraceList, WriteMapTracedSym );
        WriteMapNL();
    }
}

static void WriteMapSize( int msgid, unsigned_32 size )
/*****************************************************/
{
    std::array<char, RESOURCE_MAX_SIZE> msg;

    Msg_Get( msgid, msg.data() );
    if( size > 0xffff ) {
        WriteMapPrintf( "%s  %h (%l.)", msg.data(), size, size );
    } else {
        WriteMapPrintf( "%s  %x (%d.)", msg.data(), static_cast<unsigned_16>( size ), static_cast<unsigned_16>( size ) );
    }
}

void WriteMapLibsUsed( void )
/***************************/
{
    file_list   *file;
    std::array<char, PATH_MAX> new_name;

    if( wlink().LinkState & LS_GENERATE_LIB_LIST ) {
        WriteMapBox( MSG_MAP_BOX_LIB_USED );
        for( file = wlink().ObjLibFiles; file != nullptr; file = file->next ) {
            if( file->flags_file & STAT_LIB_USED ) {
                MakeFileName( file->infile, new_name.data() );
                WriteMapPrintf( "%s", new_name.data() );
            }
        }
        wlink().LinkState &= ~LS_GENERATE_LIB_LIST;
    }
}

static const char *getStubName( void )
/************************************/
{
#ifdef _OS2
    /* CodeBreaker: no_stub is the OS/2 family's (FmtData.u is a union: for another format it is other data, and
       Netware's `option messages` pointer made "Stub file: none" come and go with the heap) */
    if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) ) {
        if( wlink().FmtData.u.os2fam.no_stub ) {
            return( "none" );
        }
        return( wlink().FmtData.u.os2fam.stub_file_name );
    }
#endif
#ifdef _DOS16M
    if( wlink().FmtData.type & MK_DOS16M ) {
        return( wlink().FmtData.u.d16m.stub );
    }
#endif
#ifdef _PHARLAP
    if( wlink().FmtData.type & MK_PHAR_LAP ) {
        return( wlink().FmtData.u.phar.stub );
    }
#endif
    return( nullptr );
}

static void WriteMapSizes( void )
/********************************
 * Write out code size to map file and print libraries used.
 */
{
    const char  *stubname;

    if( map_state().UndefList != nullptr ) {
        WriteMapNL();
    }
    if( wlink().FmtData.type & (MK_NOVELL | MK_NE | MK_OS2_FLAT | MK_PE) ) {
        WriteMapImportsHead();
        WriteMapImports();
    }
    WriteMapLibsUsed();
    WriteMapBox( MSG_MAP_BOX_LINKER_STAT );
    WriteMapSize( MSG_MAP_STACK_SIZE, wlink().StackSize );
    WriteMapSize( MSG_MAP_MEM_SIZE, MemorySize() );
#ifdef _EXE
    if( (wlink().FmtData.type & MK_OVERLAYS)
      && wlink().FmtData.u.dos.dynamic ) {
        WriteMapSize( MSG_MAP_OVL_SIZE, static_cast<unsigned long>( wlink().OvlAreaSize ) * 16 );
    }
#endif
    if( (wlink().FmtData.type & MK_NOVELL) == 0
      && ( !wlink().FmtData.dll || (wlink().FmtData.type & MK_PE) ) ) {
        WriteMapMsgPrintf( MSG_MAP_ENTRY_PT_ADDR, &wlink().StartInfo.addr );
    }
    stubname = getStubName();
    if( stubname != nullptr ) {
        WriteMapMsgPrintf( MSG_MAP_STUB_FILE, stubname );
    }
}

void StartTime( void )
/********************/
{
    MapState    &st = map_state();

    st.StartT = time( nullptr );
    st.ClockTicks = clock();
}

void EndTime( void )
/******************/
{
    MapState    &st = map_state();

    st.ClockTicks = clock() - st.ClockTicks;
}

void MapInit( void )
/******************/
{
    MapState            &st = map_state();
    std::string         tim;
    std::string         dat;
    struct tm           *localt;

    st.Absolute_Seg = false;
    if( wlink().MapFlags & MAP_FLAG ) {
        wlink().MapFile = fopen( wlink().MapFName, "wt" );
        if( wlink().MapFile == nullptr ) {
            wlink().MapFlags = static_cast<mapflag>( 0 );
            LnkMsg( FTL+MSG_CANT_OPEN, "12", wlink().MapFName, strerror( errno ) );
            return;
        }
        setvbuf( wlink().MapFile, nullptr, _IOFBF, MAP_BUFFER_SIZE );
        localt = localtime( &st.StartT );
        st.MapCol = 0;
        WriteMapString( MsgStrings[PRODUCT] );
        WriteMapNL();
        WriteMapString( MsgStrings[COPYRIGHT] );
        WriteMapNL();
        WriteMapString( MsgStrings[COPYRIGHT2] );
        WriteMapNL();
        PutDec( tim, localt->tm_hour );
        tim.push_back( ':' );
        PutDec( tim, localt->tm_min );
        tim.push_back( ':' );
        PutDec( tim, localt->tm_sec );

        PutDec( dat, localt->tm_year );
        dat.push_back( '/' );
        PutDec( dat, localt->tm_mon + 1 );
        dat.push_back( '/' );
        PutDec( dat, localt->tm_mday );

        WriteMapLnkMsg( MSG_CREATED_ON, "12", dat.c_str(), tim.c_str() );
    }
}

void MapFini( void )
/*******************
 * Finish map processing
 */
{
    MapState    &st = map_state();
    signed_16   h;
    signed_16   m;
    signed_16   s;
    signed_16   t;
    std::string tim;

    if( wlink().MapFile != nullptr ) {

        WriteMapSizes();

        t = static_cast<unsigned_16>( st.ClockTicks % CLOCKS_PER_SEC );
        st.ClockTicks /= CLOCKS_PER_SEC;
        s = static_cast<unsigned_16>( st.ClockTicks % 60 );
        st.ClockTicks /= 60;
        m = static_cast<unsigned_16>( st.ClockTicks % 60 );
        st.ClockTicks /= 60;
        h = static_cast<unsigned_16>( st.ClockTicks );

        if( h > 0 ) {
            PutDec( tim, static_cast<unsigned>( h ) );
            tim.push_back( ':' );
        }
        PutDec( tim, static_cast<unsigned>( m ) );
        tim.push_back( ':' );
        PutDec( tim, static_cast<unsigned>( s ) );
        tim.push_back( '.' );
        PutDec( tim, static_cast<unsigned>( t ) );
        WriteMapMsgPrintf( MSG_MAP_LINK_TIME, tim.c_str() );

        if( wlink().MapFlags & MAP_LINES ) {
            WriteMapLines();
        }

        wlink().MapFlags = static_cast<mapflag>( 0 );

        if( fclose( wlink().MapFile ) ) {
            LnkMsg( ERR+MSG_IO_PROBLEM, "12", wlink().MapFName, strerror( errno ) );
        }
        wlink().MapFile = nullptr;

        if( wlink().MapFName != nullptr ) {
            MemFree( wlink().MapFName );
            wlink().MapFName = nullptr;
        }
    }
    RingFree( &st.SymTraceList );
    RingFree( &st.UndefList );
}

static int SymAddrCompare( const void *a, const void *b )
/*******************************************************/
{
    const symbol    *left = *static_cast<symbol *const *>( a );
    const symbol    *right = *static_cast<symbol *const *>( b );

    if( left->addr.seg < right->addr.seg ) {
        return( -1 );
    } else if( left->addr.seg > right->addr.seg ) {
        return( 1 );
    } else {
        if( left->addr.off < right->addr.off ) {
            return( -1 );
        } else if( left->addr.off > right->addr.off ) {
            return( 1 );
        }
    }
    return( 0 );
}

static void WriteMapSymArray( symbol **symarray, size_t num )
/***********************************************************/
{
    if( wlink().MapFlags & MAP_ALPHA ) {
        qsort( symarray, num, sizeof( symbol * ), SymAlphaCompare );
    } else {
        qsort( symarray, num, sizeof( symbol * ), SymAddrCompare );
    }
    do {
        WriteMapSymAddr( *symarray );
        symarray++;
        num--;
    } while( num > 0 );
}

void WriteMapPubStart( void )
/***************************/
{
    map_state().NumMapSyms = 0;
}

void WriteMapPubEnd( void )
/*************************/
{
    MapState    &st = map_state();
    symbol      *sym;
    bool        ok;

    if( (wlink().MapFlags & MAP_GLOBAL)
      && ( st.NumMapSyms > 0 ) ) {
        std::vector<symbol *> symarray;
        ok = ( st.NumMapSyms < ( UINT_MAX / sizeof( symbol * ) ) - 1 );
        if( ok ) {
            symarray.reserve( st.NumMapSyms );
        }
        for( sym = wlink().HeadSym; sym != nullptr; sym = sym->next ) {
            if( sym->info & SYM_MAP_GLOBAL ) {
                sym->info &= ~SYM_MAP_GLOBAL;
                if( ok ) {
                    symarray.push_back( sym );
                } else {
                    WriteMapSymAddr( sym );
                }
            }
        }
        if( !ok ) {
            LnkMsg( WRN+MSG_CANT_SORT_SYMBOLS, nullptr );
        } else {
            WriteMapSymArray( symarray.data(), st.NumMapSyms );
        }
    }
}

void WriteMapPubSortStart( pubdefinfo *info )
/*******************************************/
{
    info->symarray = nullptr;
    if( (wlink().MapFlags & MAP_SORT)
      && (wlink().MapFlags & MAP_GLOBAL) == 0
      && ( wlink().CurrMod->publist != nullptr ) ) {
        info->symarray = static_cast<symbol **>( MemAllocSafe( Ring2Count( wlink().CurrMod->publist ) * sizeof( symbol * ) ) );
    }
}

void WriteMapPubSortEnd( pubdefinfo *info )
/*****************************************/
{
    if( info->num > 0 ) {
        WriteMapSymArray( info->symarray, info->num );
    }
    if( info->symarray != nullptr ) {
        MemFree( info->symarray );
        info->symarray = nullptr;
    }
}

void WriteMapPubEntry( pubdefinfo *info, symbol *sym )
/****************************************************/
{
    if( !SkipSymbol( sym ) ) {
        if( info->first
          && (wlink().MapFlags & MAP_GLOBAL) == 0 ) {
            WriteMapPubModHead();
            info->first = false;
        }
        if( wlink().MapFlags & MAP_SORT ) {
            if( wlink().MapFlags & MAP_GLOBAL ) {
                map_state().NumMapSyms++;
                sym->info |= SYM_MAP_GLOBAL;
            } else {
                info->symarray[info->num] = sym;
                info->num++;
            }
        } else {
            WriteMapSymAddr( sym );
        }
    }
}
