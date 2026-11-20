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
* Description:  Entry points for ORL routines.
*
****************************************************************************/


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "orl.h"
#include "orllevel.h"
#include "orlentry.h"
#include "orlflhnd.h"
#include "exedos.h"
#include "pcobj.h"
#include "exedos.h"
#include "i64.h"


namespace {

using lcl_orl = LCL_SYM( orl_handle );
using lcl_file = LCL_SYM( orl_file_handle );

// the handles ORL hands out are const pointers to opaque types; each format reads its own through a pointer it may
// write, and the first member of every one is the file format
template <class To>
To as_handle( const void *p )
{
    return static_cast<To>( const_cast<void *>( p ) );
}

lcl_orl lcl( orl_handle h )
{
    return as_handle<lcl_orl>( h );
}

lcl_file lcl( orl_file_handle h )
{
    return as_handle<lcl_file>( h );
}

orl_file_format format_of( orl_sec_handle h )
{
    return as_handle<LCL_SYM( orl_sec_handle )>( h )->type;
}

orl_file_format format_of( orl_symbol_handle h )
{
    return as_handle<LCL_SYM( orl_symbol_handle )>( h )->type;
}

orl_file_format format_of( orl_group_handle h )
{
    return as_handle<LCL_SYM( orl_group_handle )>( h )->type;
}

// What each entry point does is one thing per format: the ELF, COFF and OMF routines are given as three callables, `none`
// is what a handle of no known format (ORL_UNRECOGNIZED_FORMAT) gets.
template <class R, class E, class C, class O>
R by_format( orl_file_format fmt, R none, E elf, C coff, O omf )
{
    switch( fmt ) {
    case ORL_ELF:
        return elf();
    case ORL_COFF:
        return coff();
    case ORL_OMF:
        return omf();
    default:    // ORL_UNRECOGNIZED_FORMAT
        break;
    }
    return none;
}

// a format's handle given out as ORL's own
template <class To, class From>
To handle_out( From p )
{
    return reinterpret_cast<To>( p );
}

}  // namespace

orl_handle ORLENTRY ORLInit( orl_funcs *funcs )
/*********************************************/
{
    orl_handle  orl_hnd = static_cast<orl_handle>( ORL_CLI_ALLOC( funcs, sizeof( ORL_LCL_STRUCT( orl_handle ) ) ) );

    if( orl_hnd != nullptr ) {
        lcl_orl     h = lcl( orl_hnd );

        h->error = ORL_OKAY;
        h->funcs = *funcs;
        h->elf_hnd = ElfInit( &h->funcs );
        if( h->elf_hnd == nullptr ) {
            ORL_CLI_FREE( funcs, h );
            return nullptr;
        }
        h->coff_hnd = CoffInit( &h->funcs );
        if( h->coff_hnd == nullptr ) {
            ORL_CLI_FREE( funcs, h );
            return nullptr;
        }
        h->omf_hnd = OmfInit( &h->funcs );
        if( h->omf_hnd == nullptr ) {
            ORL_CLI_FREE( funcs, h );
            return nullptr;
        }
        h->first_file_hnd = nullptr;
    }
    return orl_hnd;
}

orl_return ORLENTRY ORLGetError( orl_handle orl_hnd )
/***************************************************/
{
    return lcl( orl_hnd )->error;
}

orl_return ORLENTRY ORLFini( orl_handle orl_hnd )
/***********************************************/
{
    lcl_orl         h = lcl( orl_hnd );
    orl_return      return_val;

    if( ( return_val = ElfFini( h->elf_hnd ) ) != ORL_OKAY )
        return return_val;
    if( ( return_val = CoffFini( h->coff_hnd ) ) != ORL_OKAY )
        return return_val;
    if( ( return_val = OmfFini( h->omf_hnd ) ) != ORL_OKAY )
        return return_val;
    while( h->first_file_hnd != nullptr ) {
        return_val = ORLRemoveFileLinks( h->first_file_hnd );
        if( return_val != ORL_OKAY ) {
            return return_val;
        }
    }
    ORL_FUNCS_FREE( h, h );
    return ORL_OKAY;
}

static bool checkPEMachine( const unsigned char *buffer )
/*******************************************************/
{
    unsigned_16     machine;

    std::memcpy( &machine, buffer, sizeof( machine ) );
    switch( machine ) {
    case COFF_IMAGE_FILE_MACHINE_I860:
    case COFF_IMAGE_FILE_MACHINE_I386A:
    case COFF_IMAGE_FILE_MACHINE_I386:
    case COFF_IMAGE_FILE_MACHINE_R3000:
    case COFF_IMAGE_FILE_MACHINE_R4000:
    case COFF_IMAGE_FILE_MACHINE_ALPHA:
    case COFF_IMAGE_FILE_MACHINE_POWERPC:
    case COFF_IMAGE_FILE_MACHINE_AMD64:
        return true;
    }
    return false;
}

static int checkPE( lcl_orl h, orl_io_handle io_hnd )
/***************************************************/
// (the stream is moved to the PE header and back: 1 when it holds a PE image, -1 when moving back failed)
{
    auto    read = [&]( size_t len ) { return static_cast<unsigned char *>( ORL_FUNCS_READ( h, io_hnd, len ) ); };
    auto    seek = [&]( long pos ) { return ORL_FUNCS_SEEK( h, io_hnd, pos, SEEK_CUR ); };
    int     rc = 0;
    long    pos1 = NE_HEADER_OFFSET - 4;

    if( seek( pos1 ) == 0 ) {
        const unsigned char     *buffer = read( 0x4 );

        if( buffer != nullptr ) {
            unsigned_32     ne_header_off;

            pos1 += 4;
            std::memcpy( &ne_header_off, buffer, sizeof( ne_header_off ) );
            long    pos2 = static_cast<long>( ne_header_off ) - pos1 - 4;

            if( seek( pos2 ) == 0 ) {
                buffer = read( 4 );
                if( buffer != nullptr ) {
                    pos2 += 4;
                    rc = ( buffer[0] == 'P' && buffer[1] == 'E' && buffer[2] == '\0' && buffer[3] == '\0' );
                    if( rc ) {
                        rc = 0;
                        buffer = read( 4 );
                        if( buffer != nullptr ) {
                            pos2 += 4;
                            rc = checkPEMachine( buffer );
                        }
                    }
                }
                if( seek( SEEK_POSBACK( pos2 ) ) ) {
                    rc = -1;
                }
            }
        }
        if( seek( SEEK_POSBACK( pos1 ) ) ) {
            rc = -1;
        }
    }
    return rc;
}

static int checkOMF( lcl_orl h, orl_io_handle io_hnd, const unsigned char *magic )
/*********************************************************************************
 * See if this is the start of an OMF object file
 * the first record must be a THEADR or LHEADR record and we check that
 * it is valid, if it is then we assume that this is an OMF object file.
 */
{
    int     rc = 0;

    if( ( magic[0] == CMD_THEADR
      || magic[0] == CMD_LHEADR )
      && ( magic[1] + ( static_cast<int>( magic[2] ) << 8 ) - magic[3] - 2 ) == 0 ) {
        /*
         * This looks good so far, we must now check the record
         */
        int                     len = magic[3] + 1;
        const unsigned char     *buffer = static_cast<unsigned char *>( ORL_FUNCS_READ( h, io_hnd, len ) );

        if( buffer != nullptr ) {
            const int   pos = len;
            /*
             * Go on to check record checksum
             */
            unsigned char       chksum = magic[0] + magic[1] + magic[2] + magic[3];
            const unsigned char *p = buffer;

            while( len ) {
                chksum += *p;
                len--;
                p++;
            }
            p--;
            if( chksum == 0 ) {
                /*
                 * this is the correct OMF record at the beginning of the object
                 */
                rc = 1;
            } else if( *p == 0 ) {
                /*
                 * some compilers doesn't use checksum for OMF records
                 * then checksum value is 0
                 */
                rc = 1;
            }
            if( ORL_FUNCS_SEEK( h, io_hnd, SEEK_POSBACK( pos ), SEEK_CUR ) ) {
                rc = -1;
            }
        }
    }
    return rc;
}

orl_file_format ORLFileIdentify( orl_handle orl_hnd, orl_io_handle io_hnd )
/*************************************************************************/
{
    lcl_orl             h = lcl( orl_hnd );
    orl_file_format     ret_format = ORL_UNRECOGNIZED_FORMAT;
    const unsigned char *magic = static_cast<unsigned char *>( ORL_FUNCS_READ( h, io_hnd, 4 ) );
    int                 rc = 0;

    if( magic != nullptr ) {
        if( magic[0] == 0x7f && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F' ) {
            /*
             * check ELF object
             */
            ret_format = ORL_ELF;
        } else if( magic[0] == 0 && magic[1] == 0 && magic[2] == 0xFF && magic[3] == 0xFF ) {
            /*
             * check COFF import library
             */
            ret_format = ORL_COFF;
        } else if( ( rc = checkOMF( h, io_hnd, magic ) ) != 0 ) {
            /*
             * check OMF object image
             */
            if( rc > 0 ) {
                ret_format = ORL_OMF;
            }
        } else if( checkPEMachine( magic ) ) {
            /*
             * check COFF object image
             */
            ret_format = ORL_COFF;
        } else if( magic[0] == 'M' && magic[1] == 'Z' && ( rc = checkPE( h, io_hnd ) ) != 0 ) {
            /*
             * check PE executable image
             */
            if( rc > 0 ) {
                ret_format = ORL_COFF;
            }
        }
        /*
         * seek to beginning of object before return
         */
        if( ORL_FUNCS_SEEK( h, io_hnd, SEEK_POSBACK( 4 ), SEEK_CUR ) ) {
            ret_format = ORL_UNRECOGNIZED_FORMAT;
        }
    }
    return ret_format;
}

orl_file_handle ORLENTRY ORLFileInit( orl_handle orl_hnd, orl_io_handle io_hnd, orl_file_format type )
{
    lcl_orl     h = lcl( orl_hnd );

    switch( type ) {
    case ORL_ELF:
    case ORL_COFF:
    case ORL_OMF: {
        lcl_file    fh = static_cast<lcl_file>( ORL_FUNCS_ALLOC( h, sizeof( ORL_LCL_STRUCT( orl_file_handle ) ) ) );

        if( fh == nullptr ) {
            h->error = ORL_OUT_OF_MEMORY;
            break;
        }
        fh->type = type;
        switch( type ) {
        case ORL_ELF:
            h->error = ElfFileInit( h->elf_hnd, io_hnd, &fh->file_hnd.elf );
            break;
        case ORL_COFF:
            h->error = CoffFileInit( h->coff_hnd, io_hnd, &fh->file_hnd.coff );
            break;
        case ORL_OMF:
            h->error = OmfFileInit( h->omf_hnd, io_hnd, &fh->file_hnd.omf );
            break;
        default:
            break;
        }
        if( h->error != ORL_OKAY ) {
            ORL_FUNCS_FREE( h, fh );
            break;
        }
        ORLAddFileLinks( h, fh );
        return handle_out<orl_file_handle>( fh );
    }
    default:    //ORL_UNRECOGNIZED_FORMAT
        break;
    }
    return nullptr;
}

orl_return ORLENTRY ORLFileFini( orl_file_handle orl_file_hnd )
{
    lcl_file        fh = lcl( orl_file_hnd );
    orl_return      return_val;

    switch( fh->type ) {
    case ORL_ELF:
        return_val = ElfFileFini( fh->file_hnd.elf );
        break;
    case ORL_COFF:
        return_val = CoffFileFini( fh->file_hnd.coff );
        break;
    case ORL_OMF:
        return_val = OmfFileFini( fh->file_hnd.omf );
        break;
    default:    // ORL_UNRECOGNIZED_FORMAT
        return ORL_ERROR;
    }

    if( return_val != ORL_OKAY )
        return return_val;
    return ORLRemoveFileLinks( fh );
}


orl_rva ORLENTRY ORLExportTableRVA( orl_file_handle orl_file_hnd )
{
    orl_rva     rva = 0;

    if( lcl( orl_file_hnd )->type == ORL_COFF ) {
        rva = CoffExportTableRVA( lcl( orl_file_hnd )->file_hnd.coff );
    }
    return rva;
}

orl_return ORLENTRY ORLFileScan( orl_file_handle orl_file_hnd, const char *desired, orl_sec_return_func return_func )
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_return>( fh->type, ORL_ERROR,
        [&] { return ElfFileScan( fh->file_hnd.elf, desired, return_func ); },
        [&] { return CoffFileScan( fh->file_hnd.coff, desired, return_func ); },
        [&] { return OmfFileScan( fh->file_hnd.omf, desired, return_func ); } );
}

orl_machine_type ORLENTRY ORLFileGetMachineType( orl_file_handle orl_file_hnd )
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_machine_type>( fh->type, static_cast<orl_machine_type>( 0 ),
        [&] { return ElfFileGetMachineType( fh->file_hnd.elf ); },
        [&] { return CoffFileGetMachineType( fh->file_hnd.coff ); },
        [&] { return OmfFileGetMachineType( fh->file_hnd.omf ); } );
}

orl_file_flags ORLENTRY ORLFileGetFlags( orl_file_handle orl_file_hnd )
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_file_flags>( fh->type, static_cast<orl_file_flags>( 0 ),
        [&] { return ElfFileGetFlags( fh->file_hnd.elf ); },
        [&] { return CoffFileGetFlags( fh->file_hnd.coff ); },
        [&] { return OmfFileGetFlags( fh->file_hnd.omf ); } );
}

orl_file_size ORLENTRY ORLFileGetSize( orl_file_handle orl_file_hnd )
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_file_size>( fh->type, 0,
        [&] { return ElfFileGetSize( fh->file_hnd.elf ); },
        [&] { return CoffFileGetSize( fh->file_hnd.coff ); },
        [&] { return OmfFileGetSize( fh->file_hnd.omf ); } );
}

orl_file_type ORLENTRY ORLFileGetType( orl_file_handle orl_file_hnd )
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_file_type>( fh->type, static_cast<orl_file_type>( 0 ),
        [&] { return ElfFileGetType( fh->file_hnd.elf ); },
        [&] { return CoffFileGetType( fh->file_hnd.coff ); },
        [&] { return OmfFileGetType( fh->file_hnd.omf ); } );
}

orl_file_format ORLENTRY ORLFileGetFormat( orl_file_handle orl_file_hnd )
{
    return lcl( orl_file_hnd )->type;
}

orl_sec_handle ORLENTRY ORLFileGetSymbolTable( orl_file_handle orl_file_hnd )
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_sec_handle>( fh->type, nullptr,
        [&] { return handle_out<orl_sec_handle>( ElfFileGetSymbolTable( fh->file_hnd.elf ) ); },
        [&] { return handle_out<orl_sec_handle>( CoffFileGetSymbolTable( fh->file_hnd.coff ) ); },
        [&] { return handle_out<orl_sec_handle>( OmfFileGetSymbolTable( fh->file_hnd.omf ) ); } );
}

const char * ORLENTRY ORLSecGetName( orl_sec_handle orl_sec_hnd )
{
    return by_format<const char *>( format_of( orl_sec_hnd ), nullptr,
        [&] { return ElfSecGetName( as_handle<elf_sec_handle>( orl_sec_hnd ) ); },
        [&] { return CoffSecGetName( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfSecGetName( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_return ORLENTRY ORLSecGetBase( orl_sec_handle orl_sec_hnd, orl_sec_base *sec_base )
{
    if( sec_base != nullptr ) {
        switch( format_of( orl_sec_hnd ) ) {
        case ORL_ELF:
            return ElfSecGetBase( as_handle<elf_sec_handle>( orl_sec_hnd ), sec_base );
        case ORL_COFF:
            Set64ValU32( *sec_base, CoffSecGetBase( as_handle<coff_sec_handle>( orl_sec_hnd ) ) );
            return ORL_OKAY;
        case ORL_OMF:
            Set64ValU32( *sec_base, OmfSecGetBase( as_handle<omf_sec_handle>( orl_sec_hnd ) ) );
            return ORL_OKAY;
        default:    // ORL_UNRECOGNIZED_FORMAT
            break;
        }
        Set64ValZero( *sec_base );
    }
    return ORL_ERROR;
}

orl_sec_size ORLENTRY ORLSecGetSize( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_sec_size>( format_of( orl_sec_hnd ), 0,
        [&] { return ElfSecGetSize( as_handle<elf_sec_handle>( orl_sec_hnd ) ); },
        [&] { return CoffSecGetSize( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfSecGetSize( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_sec_type ORLENTRY ORLSecGetType( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_sec_type>( format_of( orl_sec_hnd ), static_cast<orl_sec_type>( 0 ),
        [&] { return ElfSecGetType( as_handle<elf_sec_handle>( orl_sec_hnd ) ); },
        [&] { return CoffSecGetType( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfSecGetType( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_sec_alignment ORLENTRY ORLSecGetAlignment( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_sec_alignment>( format_of( orl_sec_hnd ), 0,
        [&] { return ElfSecGetAlignment( as_handle<elf_sec_handle>( orl_sec_hnd ) ); },
        [&] { return CoffSecGetAlignment( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfSecGetAlignment( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_sec_flags ORLENTRY ORLSecGetFlags( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_sec_flags>( format_of( orl_sec_hnd ), static_cast<orl_sec_flags>( 0 ),
        [&] { return ElfSecGetFlags( as_handle<elf_sec_handle>( orl_sec_hnd ) ); },
        [&] { return CoffSecGetFlags( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfSecGetFlags( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_sec_handle ORLENTRY ORLSecGetStringTable( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_sec_handle>( format_of( orl_sec_hnd ), nullptr,
        [&] { return handle_out<orl_sec_handle>( ElfSecGetStringTable( as_handle<elf_sec_handle>( orl_sec_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( CoffSecGetStringTable( as_handle<coff_sec_handle>( orl_sec_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( OmfSecGetStringTable( as_handle<omf_sec_handle>( orl_sec_hnd ) ) ); } );
}

orl_sec_handle ORLENTRY ORLSecGetSymbolTable( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_sec_handle>( format_of( orl_sec_hnd ), nullptr,
        [&] { return handle_out<orl_sec_handle>( ElfSecGetSymbolTable( as_handle<elf_sec_handle>( orl_sec_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( CoffSecGetSymbolTable( as_handle<coff_sec_handle>( orl_sec_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( OmfSecGetSymbolTable( as_handle<omf_sec_handle>( orl_sec_hnd ) ) ); } );
}

orl_sec_handle ORLENTRY ORLSecGetRelocTable( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_sec_handle>( format_of( orl_sec_hnd ), nullptr,
        [&] { return handle_out<orl_sec_handle>( ElfSecGetRelocTable( as_handle<elf_sec_handle>( orl_sec_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( CoffSecGetRelocTable( as_handle<coff_sec_handle>( orl_sec_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( OmfSecGetRelocTable( as_handle<omf_sec_handle>( orl_sec_hnd ) ) ); } );
}

orl_linnum ORLENTRY ORLSecGetLines( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_linnum>( format_of( orl_sec_hnd ), nullptr,
        []() -> orl_linnum { return nullptr; },
        [&] { return CoffSecGetLines( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfSecGetLines( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_table_index ORLENTRY ORLSecGetNumLines( orl_sec_handle orl_sec_hnd )
{
    return by_format<orl_table_index>( format_of( orl_sec_hnd ), 0,
        []() -> orl_table_index { return 0; },
        [&] { return CoffSecGetNumLines( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfSecGetNumLines( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_sec_offset ORLENTRY ORLSecGetOffset( orl_sec_handle orl_sec_hnd )
{
    if( format_of( orl_sec_hnd ) == ORL_COFF ) {
        return CoffSecGetOffset( as_handle<coff_sec_handle>( orl_sec_hnd ) );
    }
    return 0;
}

const char * ORLENTRY ORLSecGetClassName( orl_sec_handle orl_sec_hnd )
{
    if( format_of( orl_sec_hnd ) == ORL_OMF ) {
        return OmfSecGetClassName( as_handle<omf_sec_handle>( orl_sec_hnd ) );
    }
    return nullptr;
}

orl_sec_combine ORLENTRY ORLSecGetCombine( orl_sec_handle orl_sec_hnd )
{
    if( format_of( orl_sec_hnd ) == ORL_OMF ) {
        return OmfSecGetCombine( as_handle<omf_sec_handle>( orl_sec_hnd ) );
    }
    return ORL_SEC_COMBINE_NONE;
}

orl_sec_frame ORLENTRY ORLSecGetAbsFrame( orl_sec_handle orl_sec_hnd )
{
    if( format_of( orl_sec_hnd ) == ORL_OMF ) {
        return OmfSecGetAbsFrame( as_handle<omf_sec_handle>( orl_sec_hnd ) );
    }
    return ORL_SEC_NO_ABS_FRAME;
}

orl_sec_handle ORLENTRY ORLSecGetAssociated( orl_sec_handle orl_sec_hnd )
{
    if( format_of( orl_sec_hnd ) == ORL_OMF ) {
        return OmfSecGetAssociated( as_handle<omf_sec_handle>( orl_sec_hnd ) );
    }
    return nullptr;
}

orl_group_handle ORLENTRY ORLSecGetGroup( orl_sec_handle orl_sec_hnd )
{
    if( format_of( orl_sec_hnd ) == ORL_OMF ) {
        return OmfSecGetGroup( as_handle<omf_sec_handle>( orl_sec_hnd ) );
    }
    return nullptr;
}

orl_return ORLENTRY ORLSecGetContents( orl_sec_handle orl_sec_hnd, unsigned char **buffer )
{
    return by_format<orl_return>( format_of( orl_sec_hnd ), ORL_ERROR,
        [&] { return ElfSecGetContents( as_handle<elf_sec_handle>( orl_sec_hnd ), buffer ); },
        [&] { return CoffSecGetContents( as_handle<coff_sec_handle>( orl_sec_hnd ), buffer ); },
        [&] { return OmfSecGetContents( as_handle<omf_sec_handle>( orl_sec_hnd ), buffer ); } );
}

orl_return ORLENTRY ORLSecQueryReloc( orl_sec_handle orl_sec_hnd, orl_sec_offset sec_offset, orl_reloc_return_func return_func )
{
    return by_format<orl_return>( format_of( orl_sec_hnd ), ORL_ERROR,
        [&] { return ElfSecQueryReloc( as_handle<elf_sec_handle>( orl_sec_hnd ), sec_offset, return_func ); },
        [&] { return CoffSecQueryReloc( as_handle<coff_sec_handle>( orl_sec_hnd ), sec_offset, return_func ); },
        [&] { return OmfSecQueryReloc( as_handle<omf_sec_handle>( orl_sec_hnd ), sec_offset, return_func ); } );
}

orl_return ORLENTRY ORLSecScanReloc( orl_sec_handle orl_sec_hnd, orl_reloc_return_func return_func )
{
    return by_format<orl_return>( format_of( orl_sec_hnd ), ORL_ERROR,
        [&] { return ElfSecScanReloc( as_handle<elf_sec_handle>( orl_sec_hnd ), return_func ); },
        [&] { return CoffSecScanReloc( as_handle<coff_sec_handle>( orl_sec_hnd ), return_func ); },
        [&] { return OmfSecScanReloc( as_handle<omf_sec_handle>( orl_sec_hnd ), return_func ); } );
}


orl_table_index ORLENTRY ORLCvtSecHdlToIdx( orl_sec_handle orl_sec_hnd )
/**********************************************************************/
{
    return by_format<orl_table_index>( format_of( orl_sec_hnd ), 0,
        [&] { return ElfCvtSecHdlToIdx( as_handle<elf_sec_handle>( orl_sec_hnd ) ); },
        [&] { return CoffCvtSecHdlToIdx( as_handle<coff_sec_handle>( orl_sec_hnd ) ); },
        [&] { return OmfCvtSecHdlToIdx( as_handle<omf_sec_handle>( orl_sec_hnd ) ); } );
}

orl_sec_handle ORLENTRY ORLCvtIdxToSecHdl( orl_file_handle orl_file_hnd, orl_table_index idx )
/********************************************************************************************/
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_sec_handle>( fh->type, nullptr,
        [&] { return handle_out<orl_sec_handle>( ElfCvtIdxToSecHdl( fh->file_hnd.elf, idx ) ); },
        [&] { return handle_out<orl_sec_handle>( CoffCvtIdxToSecHdl( fh->file_hnd.coff, idx ) ); },
        [&] { return handle_out<orl_sec_handle>( OmfCvtIdxToSecHdl( fh->file_hnd.omf, idx ) ); } );
}

orl_return ORLENTRY ORLRelocSecScan( orl_sec_handle orl_sec_hnd, orl_reloc_return_func return_func )
{
    return by_format<orl_return>( format_of( orl_sec_hnd ), ORL_ERROR,
        [&] { return ElfRelocSecScan( as_handle<elf_sec_handle>( orl_sec_hnd ), return_func ); },
        [&] { return CoffRelocSecScan( as_handle<coff_sec_handle>( orl_sec_hnd ), return_func ); },
        [&] { return OmfRelocSecScan( as_handle<omf_sec_handle>( orl_sec_hnd ), return_func ); } );
}

orl_return ORLENTRY ORLSymbolSecScan( orl_sec_handle orl_sec_hnd, orl_symbol_return_func return_func )
{
    return by_format<orl_return>( format_of( orl_sec_hnd ), ORL_ERROR,
        [&] { return ElfSymbolSecScan( as_handle<elf_sec_handle>( orl_sec_hnd ), return_func ); },
        [&] { return CoffSymbolSecScan( as_handle<coff_sec_handle>( orl_sec_hnd ), return_func ); },
        [&] { return OmfSymbolSecScan( as_handle<omf_sec_handle>( orl_sec_hnd ), return_func ); } );
}

orl_return ORLENTRY ORLNoteSecScan( orl_sec_handle orl_sec_hnd, orl_note_callbacks *cbs, void *cookie )
/*****************************************************************************************************/
{
    return by_format<orl_return>( format_of( orl_sec_hnd ), ORL_ERROR,
        [&] { return ElfNoteSecScan( as_handle<elf_sec_handle>( orl_sec_hnd ), cbs, cookie ); },
        [&] { return CoffNoteSecScan( as_handle<coff_sec_handle>( orl_sec_hnd ), cbs, cookie ); },
        [&] { return OmfNoteSecScan( as_handle<omf_sec_handle>( orl_sec_hnd ), cbs, cookie ); } );
}


const char * ORLENTRY ORLSymbolGetName( orl_symbol_handle orl_symbol_hnd )
{
    return by_format<const char *>( format_of( orl_symbol_hnd ), nullptr,
        [&] { return ElfSymbolGetName( as_handle<elf_symbol_handle>( orl_symbol_hnd ) ); },
        [&] { return CoffSymbolGetName( as_handle<coff_symbol_handle>( orl_symbol_hnd ) ); },
        [&] { return OmfSymbolGetName( as_handle<omf_symbol_handle>( orl_symbol_hnd ) ); } );
}

orl_return ORLENTRY ORLSymbolGetValue( orl_symbol_handle orl_symbol_hnd, orl_symbol_value *sym_value )
{
    if( sym_value != nullptr ) {
        switch( format_of( orl_symbol_hnd ) ) {
        case ORL_ELF:
            return ElfSymbolGetValue( as_handle<elf_symbol_handle>( orl_symbol_hnd ), sym_value );
        case ORL_COFF:
            Set64ValU32( *sym_value, CoffSymbolGetValue( as_handle<coff_symbol_handle>( orl_symbol_hnd ) ) );
            return ORL_OKAY;
        case ORL_OMF:
            Set64ValU32( *sym_value, OmfSymbolGetValue( as_handle<omf_symbol_handle>( orl_symbol_hnd ) ) );
            return ORL_OKAY;
        default:     //ORL_UNRECOGNIZED_FORMAT
            break;
        }
        Set64ValZero( *sym_value );
    }
    return ORL_ERROR;
}

orl_symbol_binding ORLENTRY ORLSymbolGetBinding( orl_symbol_handle orl_symbol_hnd )
{
    return by_format<orl_symbol_binding>( format_of( orl_symbol_hnd ), static_cast<orl_symbol_binding>( 0 ),
        [&] { return ElfSymbolGetBinding( as_handle<elf_symbol_handle>( orl_symbol_hnd ) ); },
        [&] { return CoffSymbolGetBinding( as_handle<coff_symbol_handle>( orl_symbol_hnd ) ); },
        [&] { return OmfSymbolGetBinding( as_handle<omf_symbol_handle>( orl_symbol_hnd ) ); } );
}

orl_symbol_type ORLENTRY ORLSymbolGetType( orl_symbol_handle orl_symbol_hnd )
{
    return by_format<orl_symbol_type>( format_of( orl_symbol_hnd ), static_cast<orl_symbol_type>( 0 ),
        [&] { return ElfSymbolGetType( as_handle<elf_symbol_handle>( orl_symbol_hnd ) ); },
        [&] { return CoffSymbolGetType( as_handle<coff_symbol_handle>( orl_symbol_hnd ) ); },
        [&] { return OmfSymbolGetType( as_handle<omf_symbol_handle>( orl_symbol_hnd ) ); } );
}

unsigned char ORLENTRY ORLSymbolGetRawInfo( orl_symbol_handle orl_symbol_hnd )
{
    return by_format<unsigned char>( format_of( orl_symbol_hnd ), 0,
        [&] { return ElfSymbolGetRawInfo( as_handle<elf_symbol_handle>( orl_symbol_hnd ) ); },
        []() -> unsigned char { return 0; },   // no COFF equivilent
        [&] { return OmfSymbolGetRawInfo( as_handle<omf_symbol_handle>( orl_symbol_hnd ) ); } );
}

orl_sec_handle ORLENTRY ORLSymbolGetSecHandle( orl_symbol_handle orl_symbol_hnd )
{
    return by_format<orl_sec_handle>( format_of( orl_symbol_hnd ), nullptr,
        [&] { return handle_out<orl_sec_handle>( ElfSymbolGetSecHandle( as_handle<elf_symbol_handle>( orl_symbol_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( CoffSymbolGetSecHandle( as_handle<coff_symbol_handle>( orl_symbol_hnd ) ) ); },
        [&] { return handle_out<orl_sec_handle>( OmfSymbolGetSecHandle( as_handle<omf_symbol_handle>( orl_symbol_hnd ) ) ); } );
}

orl_symbol_handle ORLENTRY ORLSymbolGetAssociated( orl_symbol_handle orl_symbol_hnd )
{
    return by_format<orl_symbol_handle>( format_of( orl_symbol_hnd ), nullptr,
        [&] { return handle_out<orl_symbol_handle>( ElfSymbolGetAssociated( as_handle<elf_symbol_handle>( orl_symbol_hnd ) ) ); },
        [&] { return handle_out<orl_symbol_handle>( CoffSymbolGetAssociated( as_handle<coff_symbol_handle>( orl_symbol_hnd ) ) ); },
        []() -> orl_symbol_handle { return nullptr; } );   // NYI: call to an OMF func. here.
}

orl_return ORLENTRY ORLGroupsScan( orl_file_handle orl_file_hnd, orl_group_return_func return_func )
{
    lcl_file    fh = lcl( orl_file_hnd );

    return by_format<orl_return>( fh->type, ORL_ERROR,
        []() -> orl_return { return ORL_OKAY; },
        []() -> orl_return { return ORL_OKAY; },
        [&] { return OmfGroupsScan( fh->file_hnd.omf, return_func ); } );
}

const char * ORLENTRY ORLGroupName( orl_group_handle orl_group_hnd )
{
    if( format_of( orl_group_hnd ) == ORL_OMF ) {
        return OmfGroupName( as_handle<omf_grp_handle>( orl_group_hnd ) );
    }
    return nullptr;
}

orl_table_index ORLENTRY ORLGroupSize( orl_group_handle orl_group_hnd )
{
    if( format_of( orl_group_hnd ) == ORL_OMF ) {
        return OmfGroupSize( as_handle<omf_grp_handle>( orl_group_hnd ) );
    }
    return 0;
}

const char * ORLENTRY ORLGroupMember( orl_group_handle orl_group_hnd, orl_table_index idx )
{
    if( format_of( orl_group_hnd ) == ORL_OMF ) {
        return OmfGroupMember( as_handle<omf_grp_handle>( orl_group_hnd ), idx );
    }
    return nullptr;
}
