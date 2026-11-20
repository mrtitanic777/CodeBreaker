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
#include <cstdlib>
#include <cstring>
#include <string_view>
#if defined( __WATCOMC__ )
    #include <malloc.h>     /* necessary for _heapshrink() */
#endif
#include "linkstd.h"
#include "fileio.h"
#include "idedll.h"
#include "idedrv.h"
#include "ideentry.h"
#include "wlink.h"
#include "cmdline.h"
#include "run_state.hpp"

#include "clibint.h"
#include "clibext.h"


namespace {

constexpr std::size_t   DIRECTIVE_SIZE = 8;

struct extra_cmd_info {
    IDEInfoType         type;
    std::string_view    directive;      // (DIRECTIVE_SIZE characters, blank padded)
    bool                retry;
};

constexpr std::array<extra_cmd_info, 4> ExtraCmds = {{
    { IDE_GET_TARGET_FILE,  "name    ",     false },
    { IDE_GET_OBJ_FILE,     "file    ",     true },
    { IDE_GET_LIB_FILE,     "lib     ",     true },
    { IDE_GET_RES_FILE,     "opt res=",     false },
}};

constexpr std::array<IDEMsgSeverity, 7> SeverityMap = {
//   IDE msg severity       Wlink msg class
    IDEMSGSEV_NOTE_MSG,     // INF
    IDEMSGSEV_NOTE_MSG,     // YELL
    IDEMSGSEV_WARNING,      // WRN
    IDEMSGSEV_ERROR,        // MILD_ERR
    IDEMSGSEV_ERROR,        // ERR
    IDEMSGSEV_ERROR,        // FTL
    IDEMSGSEV_BANNER        // BANNER
};

/* what the host (the program that loaded this "DLL") handed over */
struct IdeState {
    IDECBHdl        hdl = nullptr;
    IDEInitInfo     init_info{};
    IDECallBacks    *cbs = nullptr;
};

IdeState &ide()
{
    return cb_wlink::RunState::part<IdeState>();
}

}  // namespace

#if defined( DLLS_IMPLEMENTED )
int ExecDLLPgm( const char *pname, const char *cmdline )
/*******************************************************
 * return true if an error
 */
{
    int         retcode;
    IDEDRV      info;
    IdeState    &st = ide();

    IdeDrvInit( &info, pname, nullptr );
    IdeDrvChainCallbacks( st.cbs, &st.init_info );
    retcode = IdeDrvExecDLL( &info, cmdline );
    IdeDrvUnloadDLL( &info );
    return( retcode );
}
#endif

/* routines which are called by the linker core */

void WriteStdOut( const char *str )
/*********************************/
{
    IdeState    &st = ide();

    CheckBreak();
    if( st.cbs != nullptr ) {
        st.cbs->PrintWithCRLF( st.hdl, str );
    }
}

void WriteStdOutWithNL( const char *str )
/***************************************/
{
    IdeState    &st = ide();

    CheckBreak();
    if( st.cbs != nullptr ) {
        st.cbs->PrintMessage( st.hdl, str );
    }
}

void WriteStdOutNL( void )
/************************/
{
    IdeState    &st = ide();

    if( st.cbs != nullptr ) {
        st.cbs->PrintMessage( st.hdl, "" );
    }
}

void WriteStdOutInfo( const char *str, unsigned level, const char *symbol )
/*************************************************************************/
{
    IDEMsgInfo  info;
    unsigned    msgclass;
    IdeState    &st = ide();

    CheckBreak();
    if( st.cbs != nullptr ) {
        IdeMsgInit( &info, SeverityMap[(level & CLASS_MSK) >> NUM_SHIFT], str );
        msgclass = level & CLASS_MSK;
        if( msgclass != BANNER
          && msgclass >= (WRN & CLASS_MSK) ) {
            IdeMsgSetMsgNo( &info, CalcMsgNum( level ) );
            IdeMsgSetHelp( &info, "wlnkerrs.hlp", level & NUM_MSK );
        }
        if( symbol != nullptr ) {
            IdeMsgSetLnkSymbol( &info, symbol );
        }
        st.cbs->PrintWithInfo( st.hdl, &info );
    }
}

char * GetEnvString( const char *envname )
/****************************************/
{
    char        *retval;
    IdeState    &st = ide();

    if( st.cbs == nullptr
      || st.init_info.ignore_env )
        return( nullptr );
    st.cbs->GetInfo( st.hdl, IDE_GET_ENV_VAR, reinterpret_cast<IDEGetInfoWParam>( envname ),
                     reinterpret_cast<IDEGetInfoLParam>( &retval ) );
    return( retval );
}

void GetExtraCommands( void )
/***************************/
{
    std::array<char, _MAX_PATH + DIRECTIVE_SIZE>    buff;
    char                                            *p;
    IdeState                                        &st = ide();

    if( !st.init_info.cmd_line_has_files ) {
        for( const extra_cmd_info &cmd : ExtraCmds ) {
            for( ;; ) {
                memcpy( buff.data(), cmd.directive.data(), DIRECTIVE_SIZE );
                p = buff.data() + DIRECTIVE_SIZE;
                if( st.cbs->GetInfo( st.hdl, cmd.type, 0, reinterpret_cast<IDEGetInfoLParam>( p ) ) )
                    break;
                if( DoBuffCmdParse( buff.data() ) )
                    break;
                if( !cmd.retry ) {
                    break;
                }
            }
        }
    }
}

/* routines which are called by the DLL driver */

IDEBool IDEAPI IDEPassInitInfo( IDEDllHdl hdl, IDEInitInfo *info )
/****************************************************************/
{
    /* unused parameters */ (void)hdl;

    if( info->ver < IDE_CUR_INFO_VER5 )
        return( true );
    ide().init_info = *info;
    return( false );
}

unsigned IDEAPI IDEGetVersion( void )
/***********************************/
{
    return( IDE_CUR_DLL_VER );
}

void IDEAPI IDEStopRunning( void )
/********************************/
{
    LinkState |= LS_STOP_WORKING | LS_LINK_ERROR;
}

void IDEAPI IDEFreeHeap( void )
/*****************************/
{
#if defined( __WATCOMC__ )
    _heapshrink();
#endif
}

IDEBool IDEAPI IDEInitDLL( IDECBHdl cbhdl, IDECallBacks *cb, IDEDllHdl *hdl )
/***************************************************************************/
{
    /* unused parameters */ (void)hdl;

    /*
     * InitSubSystems begins the run, and what the run's state held is made
     * new: the host's callbacks are bound again after it (wlink kept them
     * in statics, bound before it)
     */
    InitSubSystems();
    IdeState    &st = ide();

    st.hdl = cbhdl;
    st.cbs = cb;
    return( false );
}

void IDEAPI IDEFiniDLL( IDEDllHdl hdl )
/*************************************/
{
    /* unused parameters */ (void)hdl;

    FiniSubSystems();
}

int IDEAPI IDERunYourSelf( IDEDllHdl hdl, const char * opts, IDEBool *fatalerr )
/******************************************************************************/
{
    /* unused parameters */ (void)hdl;

    LinkMainLine( opts );
    *fatalerr = (LinkState & LS_LINK_ERROR) != 0;
    return( *fatalerr );
}

int IDEAPI IDERunYourSelfArgv( IDEDllHdl hdl, int argc, char **argv, IDEBool *fatalerr )
/**************************************************************************************/
{
    /* unused parameters */ (void)hdl;

#if !defined( __WATCOMC__ )
    _argc = argc;
    _argv = argv;
#else
    /* unused parameters */ (void)argc; (void)argv;
#endif

    LinkMainLine( nullptr );
    *fatalerr = (LinkState & LS_LINK_ERROR) != 0;
    return( *fatalerr );
}
