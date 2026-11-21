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
* Description:  Mainline for Open Watcom linker.
*
****************************************************************************/


#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#if defined( __WATCOMC__ )
    #include <malloc.h>
#endif
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/link/run_state.hpp"
#include "build/x86link/base/fileio.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/map/map.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/objects/procfile.h"
#include "build/x86link/base/spillio.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/formats/dos16m.h"
#include "build/x86link/formats/os2.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/formats/qnx.h"
#include "build/x86link/formats/novell.h"
#include "build/x86link/formats/elf.h"
#include "build/x86link/formats/rdos.h"
#include "build/x86link/formats/phar.h"
#include "build/x86link/symbols/symtrace.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/objects/objorl.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/fixups/toc.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/directives/cmdall.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/layout/salloc.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/symbols/symtab.h"
#include "build/x86link/objects/omfreloc.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/symbols/wcomdef.h"
#include "build/x86link/objects/objomf.h"
#include "build/x86link/driver/wlink.h"
#include "build/x86link/driver/ideentry.h"


#define DEF_STACK_SIZE  _4K

#if defined( DEVBUILD ) && defined( __WATCOMC__ )
/*
 *  Following symbols are specific for Open Watcom Linker generated executables
 *  and appropriate code can be used only if this program is linked by OW Linker
 */
extern char     *_edata;
extern char     *_end;
#endif

/*
 * Not sure what this is for - doesn't seem to be referenced
 */
//extern int              __nheapblk;

namespace {

struct WlinkState {
    const char  *arg_save = nullptr;
};

WlinkState &state()
{
    return cb_wlink::RunState::part<WlinkState>();
}

}  // namespace

static void PreAddrCalcFormatSpec(Link& link)
/****************************************
 * format specific routines which need to be called before address calculation
 */
{
#ifdef _OS2
    if( link.FmtData.type & (MK_PE | MK_WIN_VXD | MK_NE | MK_OS2_FLAT) ) {
  #if 0
        if( (link.FmtData.type & (MK_NE | MK_OS2_FLAT)) && (link.LinkState & LS_HAVE_PPC_CODE) ) {
            // Development temporarly on hold:
            // ChkOS2ElfData();
        } else {
            SetOS2SegFlags();
        }
  #else
        SetOS2SegFlags(link);
  #endif
    }
    if( link.FmtData.type & MK_PE ) {
        ChkPEData(link);
    }
#endif
#ifdef _NOVELL
    if( link.FmtData.type & MK_NOVELL ) {
        FindExportedSyms(link);
    }
#endif
#ifdef _PHARLAP
    if( link.FmtData.type & MK_PHAR_LAP ) {
        CheckPharLapData(link);
    }
#endif
}

static void PostAddrCalcFormatSpec(Link& link)
/*****************************************
 * format specific routines which need to be called after address calculation
 */
{
#ifdef _OS2
    if( link.FmtData.type & (MK_PE | MK_WIN_VXD | MK_NE | MK_OS2_FLAT) ) {
  #if 0
        if( (link.FmtData.type & (MK_NE | MK_OS2_FLAT)) && (link.LinkState & LS_HAVE_PPC_CODE) ) {
            // Development temporarly on hold:
            //PrepareOS2Elf();
        } else {
            SetOS2GroupFlags();
            ChkOS2Exports();
        }
  #else
        SetOS2GroupFlags(link);
        ChkOS2Exports(link);
  #endif
    }
    if( link.FmtData.type & MK_PE ) {
        AllocPETransferTable(link);
    }
#endif
#ifdef _ELF
    if( link.FmtData.type & MK_ELF ) {
        ChkElfData(link);
    }
#endif
#ifdef _QNX
    if( link.FmtData.type & MK_QNX ) {
        SetQNXSegFlags(link);
        SetQNXGroupFlags(link);
    }
#endif
}

static void ResetMisc(Link& link)
/****************************
 * Linker support initialization.
 */
{
    link.LinkFlags = LF_REDEFS_OK | LF_FAR_CALLS_FLAG;
    link.LinkState = LS_MAKE_RELOCS;
    link.AbsGroups = nullptr;
    link.DataGroup = nullptr;
    link.IDataGroup = nullptr;
    link.OutFiles = nullptr;
    link.ObjLibFiles = nullptr;
    link.LibModules = nullptr;
    link.Groups = nullptr;
    SET_ADDR_UNDEFINED( link.CurrLoc );
    link.CurrMod = nullptr;
    link.StackSize = DEF_STACK_SIZE;
    /* CodeBreaker: the stack address of the link before stayed (a .COM link after an EXE then warned W1087) */
    link.StackAddr.seg = 0;
    link.StackAddr.off = 0;
    ResetSym(link);
}

static void DoDefaultSystem(Link& link)
/**********************************
 * first hint about format being 32-bit vs. 16-bit (might distinguish between
 * os/2 v1 & os/2 v2), and if that doesn't decide it, haul in the default
 * system block
 */
{
    if( (link.LinkState & LS_FMT_DECIDED) == 0 ) {
        if( link.LinkState & LS_FMT_SEEN_64BIT ) {
            HintFormat(link,  MK_64BIT );
        } else if( link.LinkState & LS_FMT_SEEN_32BIT ) {
            HintFormat(link,  MK_32BIT );
        } else {
            HintFormat(link,  MK_16BIT | MK_QNX );
        }
        if( (link.LinkState & LS_FMT_DECIDED) == 0 ) {
            if( link.LinkState & LS_FMT_SPECIFIED ) {
                LnkMsg( FTL+MSG_AMBIG_FORMAT, nullptr );
            }
            if( link.LinkState & LS_FMT_SEEN_64BIT ) {
                ExecSystem(link,  "64bit" );
            } else if( link.LinkState & LS_FMT_SEEN_32BIT ) {
                ExecSystem(link,  "386" );
            } else {
                ExecSystem(link,  "286" ); /* no 386 obj's after this */
            }
        }
    }
}

static void FindLibPaths(Link& link)
/******************************/
{
    AddFmtLibPaths(link);
    if( link.LinkState & LS_FMT_SEEN_64BIT ) {
        AddLibPathsToEnd(link,  GetEnvString( "LIBX64" ) );
    } else if( link.LinkState & LS_FMT_SEEN_32BIT ) {
        AddLibPathsToEnd(link,  GetEnvString( "LIB386" ) );
    } else {
        AddLibPathsToEnd(link,  GetEnvString( "LIB286" ) );
        /*
         * If we haven't seen a 386 object file by this time, we're
         * not going to.
         */
        HintFormat(link,  MK_16BIT );
    }
    AddLibPathsToEndList(link,  link.LibPath );
}

static void ResetSubSystems(Link& link)
/*********************************/
{
    ResetPermData(link);
    ResetMsg(link);
    VirtMemInit();
    ResetMisc(link);
    ResetWriteMapIO(link);
    ResetDBI(link);
    ResetCmdAll(link);
#ifdef _EXE
    ResetOverlaySupp(link);
#endif
    ResetComdef();
#ifdef _NOVELL
    ResetLoadNov();
#endif
#ifdef _OS2
    ResetLoadPE(link);
#endif
    ResetObj2Supp();
    ResetObjIO(link);
    ResetObjOMF(link);
    ResetObjPass1(link);
    ResetObjStrip();
    ResetOMFReloc();
    ResetReloc(link);
    ResetSymTrace();
    ResetLoadFile(link);
    ResetAddr(link);
    ResetToc(link);
    link.Root = NewSection();
}

void InitSubSystems( void )
/**************************/
{
#if defined( DEVBUILD ) && defined( __WATCOMC__ )
    memset( _edata, 0xA5, _end - _edata );      // don't rely on BSS == 0
#endif
    Link& link = cb_wlink::RunState::begin_run();   /* CodeBreaker: the link's state, new: nothing an earlier run in the process kept stays */
    LnkMemInit();
    LnkFilesInit();
    InitMsg(link);
    InitNodes(link);
    InitTokBuff(link);
    InitSpillFile();
    InitSym();
    InitORLObj();
    InitCmdFile();
}

static void CleanSubSystems(Link& link)
/*********************************/
{
    FreeOutFiles(link);
    MapFini(link);
    BurnSystemList();
    FreeList( link.UsrLibPath );
    CloseSpillFile();
    CleanTraces();
    FreePaths(link);
    FreeLocalImports();
    CleanLoadFile();
    CleanLinkStruct(link);
    FreeFormatStuff(link);
    FreeObjInfo(link);
    FreeVirtMem();
    CleanToc();
    CleanSym(link);
    CleanPermData(link);
}

void FiniSubSystems(Link& link)
/********************************/
{
    FiniLinkStruct(link);
    FiniMsg();
    FiniSym();
    LnkMemFini();
}

static void SetSegments(Link& link)
/*****************************/
// now that we know where everything is, do all the processing that has been
// postponed until now.
{
#ifdef _DOS16M
    if( link.FmtData.type & MK_DOS16M ) {
        MakeDos16PM(link);
    }
#endif
#ifdef _EXE
    OvlSetSegments(link);
#endif
}

static void set_signal( void )
{
    signal( SIGINT, &TrapBreak );   /* so we can clean up */
}

static void ignore_signal( void )
{
    signal( SIGINT, SIG_IGN );    /* we're going to clean up anyway */
}

static void DoLink(Link& link,  const char *cmdline )
/***************************************/
// cmdline is only used when we are running under watfor.
{
    set_signal();

    StartTime();
    InitEnvVars(link);
    DoCmdFile(link,  cmdline );
    CheckErr(link);
    MapInit(link);
    SetupFakeModule(link);
    ProcObjFiles(link); /* ObjPass1 */
    CheckErr(link);
    DoDefaultSystem(link);
    if( link.LinkState & LS_LIBRARIES_ADDED ) {
        FindLibPaths(link);
        link.LinkState |= LS_SEARCHING_LIBRARIES;
        ResolveUndefined(link);
        link.LinkState &= ~LS_SEARCHING_LIBRARIES;
        link.LinkState |= LS_GENERATE_LIB_LIST;
    }
    ProcLocalImports(link);
    DecideFormat(link);
    SetFormat(link);
    ConvertLazyRefs(link);
    SetSegments(link);
    CheckErr(link);
    DefBSSSyms(link);
    LinkFakeModule(link);
    PreAddrCalcFormatSpec(link);
    ReportUndefined(link);
    CheckClassOrder(link);
    CalcSegSizes(link);
    SetStkSize(link);
    AutoGroup(link);
    CalcAddresses(link);
    GetBSSSize(link);
    GetStkAddr(link);
    GetStartAddr(link);
    PostAddrCalcFormatSpec(link);
#ifdef _RDOS
    if( link.FmtData.type & MK_RDOS ) {
        GetRdosSegs(link);
    }
#endif
    CheckErr(link);
    InitLoadFile();
    ObjPass2(link);
    CheckErr(link);
    FiniLoadFile(link);
    WritePermData(link);
    BuildImpLib(link);
    FiniEnvVars(link);
    EndTime();
    MapFini(link);

    ignore_signal();
}

static void LinkMeBaby( void )
/****************************/
{
    Link& link = wlink();
    ResetSubSystems(link);
    DoLink(link,  state().arg_save );
}

void LinkMainLine(Link& link,  const char *cmds )
/***********************************/
{
    for( ;; ) {
        state().arg_save = cmds;         // bogus way to pass args to spawn
        Spawn( &LinkMeBaby );
        CleanSubSystems(link);
        cmds = GetNextLink();
        if( cmds == nullptr ) {
            break;
        }
    }
#if defined( __WATCOMC__ )
    _heapshrink();
#endif
}
