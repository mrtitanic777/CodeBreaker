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
* Description:  Linker message output.
*
****************************************************************************/

#include <array>
#include <charconv>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string_view>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/directives/state.hpp"   /* CodeBreaker: where the directives being read are */
#include "build/x86link/base/fileio.h"
#include "build/x86link/driver/ideentry.h"
#include "build/x86link/map/map.h"
#include "build/x86link/formats/output.h"
#include "demangle.h"
#include "rcerrors.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


const char *MsgStrings[] = {
    #define pick( name, string ) string,
    #include "build/x86link/link/wlbanner.h"
    #undef pick
};

#define MSG_ARRAY_SIZE ((MSG_MAX_ERR_MSG_NUM / 8) + 1)



namespace {

/* what the message being made knows: where it was found, the arguments it was handed */
struct MsgState {
    const char      *loc_file = nullptr;
    const char      *loc_mem = nullptr;
    unsigned        loc_rec = 0;
    MSG_ARG_LIST    arg_info{};
    char            *curr_sym_name = nullptr;
};

MsgState &state()
{
    return cb_wlink::RunState::part<MsgState>();
}

constexpr std::string_view hexchar = "0123456789abcdef";

bool use_arg_info( const MsgState &st )
{
    return st.arg_info.index >= 0;
}

constexpr bool is_vowel( int c )
{
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

}  // namespace

void ResetMsg(Link& link)
/**************************/
{
    MsgState    &st = state();

    st.loc_file = nullptr;
    st.loc_mem = nullptr;
    st.loc_rec = 0;
    st.arg_info.index = -1;
    memset( link.MsgFlags, 0xFF, MSG_ARRAY_SIZE );
}

static size_t MakeExeTypeString(Link& link,  char *buff, size_t max )
/*******************************************************/
/* make up the "an OS/2 executable" string. */
{
    std::array<char, RESOURCE_MAX_SIZE> rc_buff;
    size_t                              len;
    const char                          *str;
    size_t                              num;

    if( max <= 3 )
        return( 0 );
    len = 1;
    *buff++ = 'a';
    if( link.FmtData.osname != nullptr ) {
        str = link.FmtData.osname;
    } else {
        unsigned    format = link.FmtData.type;

        for( ;; ) {
            num = log2_32( format );
            format &= ~( 1U << num );
            if( format == 0 ) {
                break;
            }
        }
        Msg_Get( MSG_FILE_TYPES_0 + static_cast<int>( num ), rc_buff.data() );
        str = rc_buff.data();
    }
    if( is_vowel( tolower( static_cast<unsigned char>( *str ) ) ) ) {
        *buff++ = 'n';
        len++;
    }
    *buff++ = ' ';
    num = strlen( str );
    len += num + 2;
    if( len > max )
        return( len - ( num + 2 ) );
    memcpy( buff, str, num );
    buff += num;
    *buff++ = ' ';
    Msg_Get( link.FmtData.dll ? MSG_CREATE_TYPE_DLL : MSG_CREATE_TYPE_EXE, rc_buff.data() );
    str = rc_buff.data();
    num = strlen( str );
    len += num;
    if( len > max )
        return( len - num );
    memcpy( buff, str, num + 1 );       /* +1 for the nullchar */
    return( len );
}

size_t FmtStr(Link& link,  char *buff, size_t len, const char *fmt, ... )
/***********************************************************/
{
    va_list args;
    size_t  size;

    va_start( args, fmt );
    size = DoFmtStr(link,  buff, len, fmt, args );
    va_end( args );
    return( size );
}

static size_t fmtAddr(Link& link,  char *dest, size_t len, addr_type *addr, bool offs_32 )
/****************************************************************************/
{
#if defined( _PHARLAP ) || defined( _RAW )
    /* only flat offset */
    if( link.FmtData.type & MK_FLAT_OFFS ) {
        return( FmtStr(link,  dest, len, "%h", addr->off ) );
    }
#endif
#ifdef _QNX
    if( link.FmtData.type & MK_QNX_FLAT) {
        return( FmtStr(link,  dest, len, "%h", FindLinearAddr(link,  addr ) ) );
    }
#endif
#if defined( _ELF ) || defined( _OS2 )
    if( link.FmtData.type & (MK_ELF | MK_PE) ) {
        return( FmtStr(link,  dest, len, "%h", FindLinearAddr2(link,  addr ) ) );
    }
#endif
#ifdef _NOVELL
    if( link.FmtData.type & MK_ID_SPLIT ) {
        if( addr->seg == CODE_SEGMENT ) {
            return( FmtStr(link,  dest, len, "CODE:%h", addr->off ) );
        } else {
            return( FmtStr(link,  dest, len, "DATA:%h", addr->off ) );
        }
    }
#endif
    /* segmented formats 16:16 or 16:32 */
    if( !offs_32
      && (link.FmtData.type & (MK_DOS | MK_NE | MK_DOS16M | MK_QNX_16 | MK_RDOS_16)) ) {
        return( FmtStr(link,  dest, len, "%x:%x", addr->seg, static_cast<unsigned short>( addr->off ) ) );
    } else {
        return( FmtStr(link,  dest, len, "%x:%h", addr->seg, addr->off ) );
    }
}

size_t DoFmtStr(Link& link,  char *buff, size_t len, const char *src, va_list args )
/***********************************************************************
 * quick vsprintf routine
 * assumptions - format string does not end in '%'
 *             - only use of '%' is as follows
 *                  %s  : string
 *                  %tn : n character string (%ns)
 *                  %c  : character
 *                  %x  : 4 digit hex number (%4x)
 *                  %h  : 8 digit hex number (%8x)
 *                  %d  : decimal
 *                  %l  : long decimal
 *                  %a  : address ( %x:%x or 32 bit, depends on format)
 *                  %A  : address ( %x:%h or 32 bit, depends on format)
 *                  %S  : symbol name
 *                  %f  : an executable format name
 ************************************************************************/
{
    MsgState        &st = state();
    char            ch;
    char            *dest;
    char            *str;
    unsigned_16     num;
    unsigned_32     num2;
    size_t          size;
    addr_type       *addr;
    int             temp;

    dest = buff;
    for( ;; ) {
        ch = *src++;
        if( ch == '\0'
          || len == 1 )
            break;
        if( ch != '%' ) {
            *dest++ = ch;
            len--;
        } else {
            ch = *src++;
            switch( ch ) {
            case 'S' :
                if( use_arg_info( st ) ) {
                    str = st.arg_info.arg[st.arg_info.index].symb->name.u.ptr;
                    st.arg_info.index++;
                } else {
                    str = va_arg( args, symbol * )->name.u.ptr;
                }
                if( (link.LinkFlags & LF_DONT_UNMANGLE) == 0 ) {
                    size = __demangle_l( str, 0, dest, len );
                    if( size > ( len - 1 ) )
                        size = len - 1;
                    st.curr_sym_name = dest;
                } else {
                    size = strlen( str );
                    if( size > len )
                        size = len;
                    memcpy( dest, str, size );
                    st.curr_sym_name = str;
                }
                len -= size;
                dest += size;
                break;
            case 's' :
                if( use_arg_info( st ) ) {
                    str = st.arg_info.arg[st.arg_info.index].string;
                    st.arg_info.index++;
                } else {
                    str = va_arg( args, char * );
                }
                size = strlen( str );
                if( size > len )
                    size = len;
                memcpy( dest, str, size );
                len -= size;
                dest += size;
                break;
            case 't' :
                str = va_arg( args, char * );
                num = static_cast<unsigned_16>( *src++ - '0' );
                num = static_cast<unsigned_16>( num * 10 + *src++ - '0' );
                if( num > len )
                    num = static_cast<unsigned_16>( len );
                for( ; (*str != '\0') && (num > 0); --num ) {
                    *dest++ = *str++;
                }
                while( num-- > 0 ) {
                    *dest++ = ' ';
                }
                len -= num;     /* (sic: num has wrapped to 0xFFFF here, as in wlink) */
                break;
            case 'c' :
                *dest++ = static_cast<char>( va_arg( args, int ) );
                len--;
                break;
            case 'x' :
                if( use_arg_info( st ) ) {
                    num = st.arg_info.arg[st.arg_info.index].int_16;
                    st.arg_info.index++;
                } else {
                    num = static_cast<unsigned_16>( va_arg( args, unsigned int ) );
                }
                if( len < 4 )
                    return( dest - buff );    //NOTE: premature return
                dest += 4;
                len -= 4;
                str = dest;
                for( int i = 4; i > 0; i-- ) {
                    *--str = hexchar[num & 0x0f];
                    num >>= 4;
                }
                break;
            case 'h' :
                num2 = va_arg( args, unsigned_32 );
                if( len < 8 )
                    return( dest - buff );     //NOTE: premature return
                dest += 8;
                len -= 8;
                str = dest;
                for( int i = 8; i > 0; i-- ) {
                    *--str = hexchar[num2 & 0x0f];
                    num2 >>= 4;
                }
                break;
            case 'd' :
                if( len < 5 )
                    return( dest - buff );    // NOTE: premature return
                if( use_arg_info( st ) ) {
                    num = st.arg_info.arg[st.arg_info.index].int_16;
                    st.arg_info.index++;
                } else {
                    num = static_cast<unsigned_16>( va_arg( args, unsigned int ) );
                }
                size = static_cast<size_t>( std::to_chars( dest, dest + len, static_cast<unsigned>( num ) ).ptr - dest );
                dest[size] = '\0';      /* (sprintf ended the number so; the text may stop here) */
                dest += size;
                len -= size;
                break;
            case 'l' :
                if( len < 10 )
                    return( dest - buff );   //NOTE: premature return
                if( use_arg_info( st ) ) {
                    num2 = st.arg_info.arg[st.arg_info.index].int_32;
                    st.arg_info.index++;
                } else {
                    num2 = va_arg( args, unsigned_32 );
                }
                size = static_cast<size_t>( std::to_chars( dest, dest + len, static_cast<unsigned long>( num2 ) ).ptr - dest );
                dest[size] = '\0';
                dest += size;
                len -= size;
                break;
            case 'a':
            case 'A':
                if( use_arg_info( st ) ) {
                    addr = st.arg_info.arg[st.arg_info.index].address;
                    st.arg_info.index++;
                } else {
                    addr = va_arg( args, addr_type * );
                }
                temp = st.arg_info.index;
                st.arg_info.index = -1;
                size = fmtAddr(link,  dest, len, addr, ( ch == 'A' ) );
                dest += size;
                len -= size;
                st.arg_info.index = temp;
                break;
            case 'f':
                size = MakeExeTypeString(link,  dest, len );
                dest += size;
                len -= size;
                break;
            }
        }
    }
    st.arg_info.index = -1;
    *dest = '\0';
    return( dest - buff );
}

void Locator( const char *filename, const char *mem, unsigned rec )
/*****************************************************************/
{
    MsgState    &st = state();

    st.loc_file = filename;
    st.loc_mem = mem;
    st.loc_rec = rec;
}

static void LocateFile(Link& link,  unsigned num )
/************************************/
{
    unsigned    rec;

    if( num & LOC ) {
        if( num & (LOC_REC & ~LOC) ) {
            rec = link.RecNum;
        } else {
            rec = 0;
        }
        if( link.CurrMod == nullptr ) {
            Locator( DirectiveSourceName(), nullptr, 0 );
         } else {
            Locator( link.CurrMod->u1.source->infile->name.u.ptr, link.CurrMod->name.u.ptr, rec );
        }
    }
}

unsigned CalcMsgNum( unsigned num )
/*********************************/
// map the internal enum onto the value that the user sees.
{
    unsigned    class_;

    class_ = (num & CLASS_MSK) >> NUM_SHIFT;
    class_ = (class_ + 1) / 2;
    return( class_ * 1000 + (num & NUM_MSK) );
}

size_t GetMsgPrefix(Link& link,  unsigned num, char *buff, size_t max_len )
/*************************************************************/
{
    size_t                              msgprefixlen;
    unsigned                            class_;
    std::array<char, RESOURCE_MAX_SIZE> rc_buff;

    msgprefixlen = 0;
    *buff = '\0';
    class_ = num & CLASS_MSK;
    if( class_ >= (WRN & CLASS_MSK) ) {
        if( class_ == (WRN & CLASS_MSK) ) {
            Msg_Get( MSG_WARNING, rc_buff.data() );
        } else {
            Msg_Get( MSG_ERROR, rc_buff.data() );
        }
        msgprefixlen = FmtStr(link,  buff, max_len, rc_buff.data(), CalcMsgNum( num ) );
    }
    return( msgprefixlen );
}

static void MessageFini(Link& link,  unsigned num, char *buff, size_t len )
/*************************************************************/
{
    unsigned    class_;
    char        *sym_name = state().curr_sym_name;

    class_ = num & CLASS_MSK;
    if( class_ >= (ERR & CLASS_MSK) ) {
        link.LinkState |= LS_LINK_ERROR;
    }
    if( num & OUT_TERM ) {
        if( (link.LinkFlags & LF_QUIET_FLAG) == 0 ) {
            WLPrtBanner(link);
            WriteStdOutInfo( buff, num, sym_name );
        } else if( class_ != (INF & CLASS_MSK) ) {
            WriteStdOutInfo( buff, num, sym_name );
        }
    }
    if( num & OUT_MAP ) {
        if( link.MapFile != nullptr ) {
            WriteMapLnkMsgCallback(link,  num, buff, len );
        }
    }
    if( class_ == (FTL & CLASS_MSK) ) {
        Suicide();
    }
    /* yells are counted as errors for limits */
    if( ( class_ == (YELL & CLASS_MSK) )
      || ( class_ >= (MILD_ERR & CLASS_MSK) ) ) {
        if( link.LinkFlags & LF_MAX_ERRORS_FLAG ) {
            link.MaxErrors--;
            if( link.MaxErrors == 0 ) {
                LnkMsg( FTL+MSG_TOO_MANY_ERRORS, nullptr );
            }
        }
    }
}

static void FileOrder( char *rc_buff, int which_file )
/****************************************************/
{
    MsgState    &st = state();

    switch( which_file ) {
    case 1:
        Msg_Do_Put_Args( rc_buff, &st.arg_info, "s", st.loc_file );
        break;
    case 2:
        Msg_Do_Put_Args( rc_buff, &st.arg_info, "s", st.loc_mem );
        break;
    case 3:
        Msg_Do_Put_Args( rc_buff, &st.arg_info, "12", st.loc_file, st.loc_mem );
        break;
    case 4:
        Msg_Do_Put_Args( rc_buff, &st.arg_info, "d", st.loc_rec );
        break;
    case 5:
        Msg_Do_Put_Args( rc_buff, &st.arg_info, "sd", st.loc_file, st.loc_rec );
        break;
    case 6:
        Msg_Do_Put_Args( rc_buff, &st.arg_info, "sd", st.loc_mem, st.loc_rec );
        break;
    case 7:
        Msg_Do_Put_Args( rc_buff, &st.arg_info, "12d", st.loc_file, st.loc_mem, st.loc_rec );
        break;
    }
}

void LnkMsg(
    unsigned    num,    // A message number + control flags
    const char  *types, // Conversion qualifiers
    ... )               // Arguments to interpolate into message
/**************************************************
 * report a linker message
 *
 * num   selects a message containing substitutions; both printf and %digit
 * types is either NULL or the order of interpolated arguments.
 */
{
    Link& link = wlink();
    va_list                             args;
    int                                 which_file = 0;
    size_t                              len;
    std::array<char, RESOURCE_MAX_SIZE> rc_buff;
    std::array<char, MAX_MSG_SIZE>      buff;
    MsgState                            &st = state();

    if( !TestBit( link.MsgFlags, num & NUM_MSK ) )
        return;
    st.curr_sym_name = nullptr;
    LocateFile(link,  num );
    len = 0;
    if( st.loc_file != nullptr ) {
        which_file += 1;
    }
    if( st.loc_mem != nullptr ) {
        which_file += 2;
    }
    if( st.loc_rec != 0 ) {
        which_file += 4;
    }
    if( which_file != 0 ) {
        if( DirectiveSourceKind() == DIRECTIVE_SOURCE_SYSTEM ) {
            Msg_Get( MSG_SYS_BLK, rc_buff.data() );
            which_file = 1;
        } else if( DirectiveSourceKind() == DIRECTIVE_SOURCE_ENVIRONMENT ) {
            Msg_Get( MSG_ENVIRON, rc_buff.data() );
            which_file = 1;
        } else {
            Msg_Get( MSG_FILE_REC_NAME_0 + which_file - 1, rc_buff.data() );
        }
        FileOrder( rc_buff.data(), which_file );
        len += FmtStr(link,  buff.data() + len, MAX_MSG_SIZE - len, rc_buff.data() );
        if( num & LINE ) {
            if( DirectiveSourceKind() == DIRECTIVE_SOURCE_OTHER ) {
                Msg_Get( MSG_LINE, rc_buff.data() );
                Msg_Do_Put_Args( rc_buff.data(), &st.arg_info, "d", DirectiveLine() );
                len += FmtStr(link,  buff.data() + len, MAX_MSG_SIZE - len, rc_buff.data() );
            }
        }
        st.loc_file = nullptr;
        st.loc_mem = nullptr;
        st.loc_rec = 0;
    }

    Msg_Get( num & NUM_MSK, rc_buff.data() );
    va_start( args, types );
    Msg_Put_Args( rc_buff.data(), &st.arg_info, types, args );
    va_end( args );
    len += FmtStr(link,  buff.data() + len, MAX_MSG_SIZE - len, rc_buff.data() );
    MessageFini(link,  num, buff.data(), len );
}

#ifdef _OS2
static void HandleRcMsg(Link& link,  unsigned num, va_list args )
/***************************************************/
/* getting an error message from resource compiler code */
{
    size_t                              len;
    std::array<char, RESOURCE_MAX_SIZE> rc_buff;
    std::array<char, MAX_MSG_SIZE>      buff;

    num |= ERR;
    len = 0;
    state().curr_sym_name = nullptr;
    Msg_Get( num & NUM_MSK, rc_buff.data() );
    len += DoFmtStr(link,  buff.data() + len, MAX_MSG_SIZE - len, rc_buff.data(), args );
    MessageFini(link,  num, buff.data(), len );
}

void RcWarning( unsigned num, ... )
/****************************************/
{
    Link& link = wlink();
    va_list args;

    va_start( args, num );
    HandleRcMsg(link,  num, args );
    va_end( args );
}

void RcError( unsigned num, ... )
/**************************************/
{
    Link& link = wlink();
    va_list args;

    va_start( args, num );
    HandleRcMsg(link,  num, args );
    va_end( args );
}

bool RcIOError( RcStatus ret, const char *srcname, const char *dstname, int err )
{
    switch( ret ) {
    case RS_OK:
    case RS_PARAM_ERROR:
        return( false );
    case RS_OPEN_ERROR:
        RcError( ERR_CANT_OPEN_FILE, srcname, strerror( err ) );
        break;
    case RS_BAD_FILE_FMT:
        RcError( ERR_NOT_VALID_EXE, srcname );
        break;
    case RS_READ_ERROR:
        RcError( ERR_READING_FILE, srcname, strerror( err ) );
        break;
    case RS_READ_ERROR_EXE:
        RcError( ERR_READING_EXE, srcname, strerror( err ) );
        break;
    case RS_READ_ERROR_TMP:
        RcError( ERR_READING_TMP, srcname, strerror( err ) );
        break;
    case RS_READ_INCMPLT:
        RcError( ERR_UNEXPECTED_EOF, srcname );
        break;
    case RS_WRITE_ERROR:
        RcError( ERR_WRITTING_FILE, dstname, strerror( err ) );
        break;
    case RS_WRITE_ERROR_RES:
        RcError( ERR_WRITTING_RES, dstname, strerror( err ) );
        break;
    case RS_WRITE_ERROR_TMP:
        RcError( ERR_WRITTING_TMP, dstname, strerror( err ) );
        break;
    default:
        RcError( ERR_INTERNAL, INTERR_UNKNOWN_RCSTATUS );
        break;
    }
    return( true );
}
#endif

void WLPrtBanner(Link& link)
/***********************
 * print the banner, if it hasn't already been printed
 */
{
    if( !link.BannerPrinted ) {
        link.BannerPrinted = true;
        for( int id : { PRODUCT, COPYRIGHT, COPYRIGHT2, TRADEMARK, TRADEMARK2 } ) {
            WriteStdOutInfo( MsgStrings[id], BANNER, nullptr );
        }
    }
}

bool SkipSymbol(Link& link,  symbol * sym )
/************************************/
{
    mangled_type art;

    if( (sym->info & SYM_STATIC)
      && (link.MapFlags & MAP_STATICS) == 0 )
        return( true );
    art = __is_mangled_internal( sym->name.u.ptr, strlen( sym->name.u.ptr ) );
    return( (link.MapFlags & MAP_ARTIFICIAL) == 0 && art == __MANGLED_INTERNAL );
}

static int SymAlphaOrder( const void *a, const void *b, bool unmangle )
/********************************************************/
{
    const symbol    *left = *static_cast<symbol *const *>( a );
    const symbol    *right = *static_cast<symbol *const *>( b );
    const char      *leftname;
    const char      *rightname;
    size_t          leftsize;
    size_t          rightsize;
    int             result;

    if( unmangle ) {
        __unmangled_name( left->name.u.ptr, 0, &leftname, &leftsize );
        __unmangled_name( right->name.u.ptr, 0, &rightname, &rightsize );
    } else {
        leftname = left->name.u.ptr;
        rightname = right->name.u.ptr;
        leftsize = strlen( leftname );
        rightsize = strlen( rightname );
    }
    if( leftsize < rightsize ) {
        result = strnicmp( leftname, rightname, leftsize );
        if( result == 0 ) {
            result = -1;  // since leftsize < rightsize;
        }
    } else {
        result = strnicmp( leftname, rightname, rightsize );
        if( result == 0 ) {
            if( leftsize > rightsize ) {
                result = 1;
            }
        }
    }
    return( result );
}

int SymAlphaCompare( const void *a, const void *b )
{
    return( SymAlphaOrder( a, b, false ) );
}

int SymAlphaCompareUnmangled( const void *a, const void *b )
{
    return( SymAlphaOrder( a, b, true ) );
}
