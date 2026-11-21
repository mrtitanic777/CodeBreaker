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
* Description:  IDE messages formating functions
*
****************************************************************************/

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>

#include "idedll.h"
#include "build/x86link/link/run_state.hpp"

namespace {

// What the formatter keeps for a run: the name of the contextual file (only cleared by IdeMsgStartDll now;
// the code that set it is switched off).
struct FileState {
    std::array<char, 144> contextual_file{};
};

// Indexed by IDEMsgSeverity, in the order of __IdeMsgSeverity: WARNING, ERROR, NOTE, BANNER, DEBUG, NOTE_MSG.
constexpr std::array<const char *, 6> text_severity = {
    "Warning!", "Error!", "Note!", "", "Debug!", ""
};

constexpr std::array<const char *, 6> text_msg_no = {
    "W", "E", "N", "B", "D", "N"
};

static_assert( text_severity.size() == text_msg_no.size() );
static_assert( IDEMSGSEV_WARNING == 0 && IDEMSGSEV_NOTE_MSG == 5, "severity tables follow __IdeMsgSeverity" );

// The message under construction, in the caller's buffer.
class Construction {
public:
    Construction( char *buffer, std::size_t bsize ) : buffer_( buffer ), bsize_( bsize ) {}

    void text( std::string_view s )
    {
        const std::size_t left = bsize_ - used_;
        if( left == 0 )
            return;
        const std::size_t n = std::min( s.size(), left - 1 );
        char *tgt = &buffer_[used_];
        tgt[n] = '\0';
        if( n != 0 ) {
            used_ += n;
            std::memcpy( tgt, s.data(), n );
        }
    }

    // a blank, if not at the start
    void blank()
    {
        if( used_ != 0 )
            text( " " );
    }

    // the number as 3 digits at least
    void number3( unsigned number )
    {
        const std::string s = std::to_string( number + ( number > 999 ? 10000 : 1000 ) );
        text( std::string_view( s ).substr( 1 ) );
    }

    void number( unsigned n )
    {
        text( std::to_string( n ) );
    }

    void severity( const IDEMsgInfo *info )
    {
        blank();
        text( text_severity[info->severity] );
    }

    void msg_number( const IDEMsgInfo *info )
    {
        if( IDE_HAS_MSG_NUM( info ) ) {
            blank();
            if( IDE_HAS_MSG_GRP( info ) && info->msg_group[0] != '\0' ) {
                text( info->msg_group );
                text( "-" );
                number3( info->msg_no & 255 );
            } else {
                text( text_msg_no[info->severity] );
                number3( info->msg_no );
            }
            text( ":" );
        }
    }

private:
    char        *buffer_;
    std::size_t bsize_;
    std::size_t used_ = 0;
};

} // namespace


// The "displayer" parameter is no longer required
//
void IdeMsgFormat               // FORMAT A MESSAGE
    ( IDECBHdl handle           // - handle for requestor
    , IDEMsgInfo const * info   // - message information
    , char * buffer             // - buffer
    , size_t bsize              // - buffer size
    , IDEMsgInfoFn displayer )  // - display function
{
    /* unused parameters */ (void)handle; (void)displayer;

    Construction ct( buffer, bsize );
    assert( info != nullptr );
    assert( buffer != nullptr );
    assert( bsize != 0 );
    assert( info->severity < text_severity.size() );
    if( IDE_IS_READABLE( info ) ) {
        // It is the responsibility of the processor to put out the file and include-chain messages.
        switch( info->severity ) {
          case IDEMSGSEV_ERROR :
          case IDEMSGSEV_WARNING :
          case IDEMSGSEV_DEBUG :
            if( IDE_HAS_SRC_LINE( info ) ) {
                ct.text( "(" );
                ct.number( static_cast<unsigned>( info->src_line ) );
                if( IDE_HAS_SRC_COL( info ) ) {
                    ct.text( "," );
                    ct.number( static_cast<unsigned>( info->src_col ) );
                }
                ct.text( "):" );
            }
            ct.severity( info );
            ct.msg_number( info );
            break;
          case IDEMSGSEV_NOTE :
            ct.text( " " );
            break;
        }
    } else {
        switch( info->severity ) {
          case IDEMSGSEV_ERROR :
          case IDEMSGSEV_WARNING :
          case IDEMSGSEV_NOTE :
          case IDEMSGSEV_DEBUG :
            if( IDE_HAS_SRC_FILE( info ) ) {
                ct.text( info->src_file );
                if( IDE_HAS_SRC_LINE( info ) ) {
                    ct.text( "(" );
                    ct.number( static_cast<unsigned>( info->src_line ) );
                    ct.text( "):" );
                }
            }
            ct.severity( info );
            ct.msg_number( info );
            if( IDE_HAS_SRC_COL( info ) ) {
                ct.text( " col(" );
                ct.number( static_cast<unsigned>( info->src_col ) );
                ct.text( ")" );
            }
            break;
        }
    }
    ct.blank();
    ct.text( info->msg );
}


void IdeMsgStartDll             // START OF FORMATING FOR A DLL (EACH TIME)
    ( void )
{
    cb_wlink::RunState::part<FileState>().contextual_file[0] = '\0';
}
