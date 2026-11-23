/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include "bool.h"
#include "jmpbuf.h"
#include "idedll.h"
#include "cgdefs.h"
#include "cgmisc.h"
#include "cgcli.h"
#include "feprotos.h"

#include "clibext.h"
#include "frontrun.hpp"



//static   IDEInitInfo   Info;

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct IdeentryState: ideentry_state.h) */

}  // namespace


void ConBlip( FrontRun& front )
/*******************
 * C compiler do a blip to console
 */
{
    front.ideentry.IdeCbs->PrintWithCRLF( front.ideentry.IdeHdl, "." );
    /*
     * we are ignoring return for now
     */
}

void ConsErrMsg( FrontRun& front, cmsg_info *cinfo )
/**********************************
 * C compiler call back to do a  console print to stderr
 */
{
    IDEMsgInfo  info;
    IDEMsgSeverity severity;

    severity = 0;
    switch( cinfo->class_ ) {
    case CMSG_INFO:
        severity = IDEMSGSEV_NOTE;
        break;
    case CMSG_WARN:
        severity = IDEMSGSEV_WARNING;
        break;
    case CMSG_ERRO:
        severity = IDEMSGSEV_ERROR;
        break;
    }
    IdeMsgInit( &info, severity, cinfo->msgtxt );
    IdeMsgSetHelp( &info, "wccerrs.hlp", cinfo->msgnum );
    IdeMsgSetMsgNo( &info, cinfo->msgnum );
    if( cinfo->fname != NULL ) {
        IdeMsgSetSrcFile( &info, cinfo->fname );
    }
    if( cinfo->line != 0 ) {
        IdeMsgSetSrcLine( &info, cinfo->line );
    }
    front.ideentry.IdeCbs->PrintWithInfo( front.ideentry.IdeHdl, &info );
    /*
     * we are ignoring return for now
     */
}

void ConsErrMsgVerbatim( FrontRun& front, char const *line )
/******************************************
 * C compiler call back to a console print to stderr
 */
{
    IDEMsgInfo info;

    IdeMsgInit( &info, IDEMSGSEV_ERROR, line );
    front.ideentry.IdeCbs->PrintWithInfo( front.ideentry.IdeHdl, &info );
    /*
     * we are ignoring return for now
     */
}

void BannerMsg( FrontRun& front, char const  *line )
/**********************************
 * C compiler call back to print a banner type msg
 */
{
    IDEMsgInfo info;

    IdeMsgInit( &info, IDEMSGSEV_BANNER, line );
    front.ideentry.IdeCbs->PrintWithInfo( front.ideentry.IdeHdl, &info );
    /*
     * we are ignoring return for now
     */
}

void DebugMsg( FrontRun& front, char const  *line )
/**********************************
 * C compiler call back to print a banner type msg
 */
{
    IDEMsgInfo info;

    IdeMsgInit( &info, IDEMSGSEV_DEBUG, line );
    front.ideentry.IdeCbs->PrintWithInfo( front.ideentry.IdeHdl, &info );
    /*
     * we are ignoring return for now
     */
}

void NoteMsg( FrontRun& front, char const  *line )
/**********************************
 * C compiler call back to print a banner type msg
 */
{
    IDEMsgInfo info;

    IdeMsgInit( &info, IDEMSGSEV_NOTE_MSG, line );
    front.ideentry.IdeCbs->PrintWithInfo( front.ideentry.IdeHdl, &info );
    /*
     * we are ignoring return for now
     */
}

const char *FEGetEnv( FrontRun& front, char const *name )
/***************************************
 * get enviorment variable
 */
{
    const char *ret;

    ret = NULL;
    if( !front.cvars.GlobalCompFlags.ignore_environment ) {
        if( front.ideentry.IdeCbs->GetInfo( front.ideentry.IdeHdl, IDE_GET_ENV_VAR, reinterpret_cast<IDEGetInfoWParam>(name), reinterpret_cast<IDEGetInfoLParam>(&ret) ) ) {
            ret = NULL;
        }
    }
    return( ret );
}


void MyExit( FrontRun& front, int ret )
/********************/
{
    longjmp( *front.ideentry.FatalEnv, ret );
}

// IDE INTERFACE

unsigned IDEAPI IDEGetVersion ( void )
/*************************************
 * GET IDE VERSION
 */
{
    return IDE_CUR_DLL_VER;
}



IDEBool IDEAPI IDEInitDLL
    ( IDECBHdl      idehdl          // - handle for this instantiation
    , IDECallBacks  *idecbs         // - call backs into IDE
    , IDEDllHdl     *dllhdl ) {     // - uninitialized info
/************************
 * DLL INITIALIZATION
 ************************/
    RunSet* run = cbrun_begin( static_cast<RunSet*>( *dllhdl ) );   // (the run the host kept from its last compile, or a new one: the handle the host passes to the other entry points)
    FrontRun& front = *run->front;

    front.ideentry.IdeHdl = idehdl;
    front.ideentry.IdeCbs = idecbs;
    *dllhdl = run;
    FrontEndInit( front, true );
    return( false );
}

void IDEAPI IDEFreeHeap( void )
/*************************************
 * Cleanup Heap
 */
{
}

void IDEAPI IDEFiniDLL( IDEDllHdl hdl )
/**************************************
 * DLL COMPLETION
 */
{
    RunSet* run = static_cast<RunSet*>( hdl );

    FrontEndFini( *run->front );
    cbrun_end( run );   // (the run is the host's still: its next compile starts with it)
}

void IDEAPI IDEFreeRun( IDEDllHdl hdl )
/**************************************
 * the host has no more compiles
 */
{
    cbrun_free( static_cast<RunSet*>( hdl ) );
}


static void getFrontEndArgv( FrontRun& front, char **argv, char *infile, char *outfile )
{
    char    *p;

    if( !front.cvars.GlobalCompFlags.ide_cmd_line_has_files ) {
        infile[0] = '\0';
        front.ideentry.IdeCbs->GetInfo( front.ideentry.IdeHdl, IDE_GET_SOURCE_FILE, static_cast<IDEGetInfoWParam>(NULL), reinterpret_cast<IDEGetInfoLParam>(infile) );
        *argv++ = infile;
        outfile[0] = '\0';
        p = outfile + 4;
        if( !front.ideentry.IdeCbs->GetInfo( front.ideentry.IdeHdl, IDE_GET_TARGET_FILE, static_cast<IDEGetInfoWParam>(NULL), reinterpret_cast<IDEGetInfoLParam>(p) ) ) {
            outfile[0] = '-';
            outfile[1] = 'f';
            outfile[2] = 'o';
            outfile[3] = '=';
        }
        *argv++ = outfile;
        *argv = NULL;
    }
}


int IDEAPI IDERunYourSelf       // COMPILE A PROGRAM
    ( IDEDllHdl hdl             // - handle for this instantiation
    , const char* opts          // - options
    , IDEBool* fatal_error )    // - addr[fatality indication]
/****************************
 * Do a compile of a file
 */
{
    jmp_buf     env;
    char        infile[_MAX_PATH];      // - input file name
    char        outfile[4 + _MAX_PATH]; // - output file name (need room for "-fo=")
    char        *argv[4];
    int         ret;

    FrontRun& front = *static_cast<RunSet*>( hdl )->front;

    TBreak(*front.back);   // clear any pending IDEStopRunning's
    *fatal_error = false;
    front.ideentry.FatalEnv = JMPBUF_PTR( env );
    /*
     * initialize argv array
     */
    argv[0] = const_cast<char *>(opts);
    argv[1] = NULL;
    getFrontEndArgv( front, argv + 1, infile, outfile );
    if( setjmp( env ) != 0 ) {  /* if fatal error has occurred */
        *fatal_error = true;
        ret = true;
    } else {
        ret = FrontEnd( front, argv );
    }
    return( ret );
}


void IDEAPI IDEStopRunning( IDEDllHdl hdl )
{
    CauseTBreak( *static_cast<RunSet*>( hdl )->back );
}


IDEBool IDEAPI IDEPassInitInfo( IDEDllHdl hdl, IDEInitInfo *info )
/****************************************************************/
{
    FrontRun& front = *static_cast<RunSet*>( hdl )->front;

//    DbgVerify( hdl == CompInfo.dll_handle, "PassInitInfo -- handle mismatch" );
    if( info->ver < 2 ) {
        return( true );
    }
    if( info->ignore_env ) {
        front.cvars.GlobalCompFlags.ignore_environment = true;
        front.cvars.GlobalCompFlags.ignore_current_dir = true;
    }
    if( info->cmd_line_has_files ) {
        front.cvars.GlobalCompFlags.ide_cmd_line_has_files = true;
    }
    if( info->ver > 2 ) {
        if( info->console_output ) {
            front.cvars.GlobalCompFlags.ide_console_output = true;
        }
        if( info->ver > 3 ) {
            if( info->progress_messages ) {
                front.cvars.GlobalCompFlags.progress_messages = true;
            }
        }
    }
    return( false );
}
