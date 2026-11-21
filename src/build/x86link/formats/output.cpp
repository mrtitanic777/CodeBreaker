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
* Description:  Utilities for processing creation of load files.
*
****************************************************************************/


#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <array>
#include <string>
#include <string_view>
#if !defined( __UNIX__ ) || defined(__WATCOMC__)
    #include <process.h>
#endif
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/base/ring.h"
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/map/map.h"
#include "build/x86link/objects/omfreloc.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/formats/os2.h"
#include "build/x86link/formats/dos.h"
#include "build/x86link/formats/phar.h"
#include "build/x86link/formats/novell.h"
#include "build/x86link/formats/dos16m.h"
#include "build/x86link/formats/qnx.h"
#include "build/x86link/formats/elf.h"
#include "build/x86link/formats/rdos.h"
#include "build/x86link/formats/raw.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/driver/ideentry.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


#define IMPLIB_BUFSIZE  _4K

namespace {

// the import library being collected for this run (its memory is wlink's: MemAlloc'd, freed by BuildImpLib)
struct implibinfo {
    f_handle    handle = NIL_FHANDLE;
    char        *fname = nullptr;
    char        *buffer = nullptr;
    size_t      bufsize = 0;
    char        *module_name = nullptr;
    size_t      module_name_len = 0;
    bool        didone = false;
    bool        temp = false;       // fname is the temporary file wlib reads (not `option impfile`'s): deleted when done
};

// what loadfile.cpp keeps for one run of the linker
struct LoadFileState {
    implibinfo  implib;
    std::string entry;          // the text of one import library entry, kept to spare an allocation each time
};

struct grpwriteinfo {
    unsigned_32 grp_start = 0;
    unsigned_32 seg_start = 0;
    group_entry *last_group = nullptr;      // used only for copy classes
    bool        repos = false;
    bool        copy = false;
};

using writebuffer_fn = void *(void *, const void *, size_t);

LoadFileState &loadFileState()
{
    return cb_wlink::RunState::part<LoadFileState>();
}

}


// the "`" will be an "a" when processed.
static constexpr std::string_view TEMPFNAME = "IMP02112.xx`";

static f_handle openTempFile( char **fname )
/******************************************/
{
    const char  *ptr;
    std::string path;
    size_t      last;
    size_t      tlen;
    f_handle    fhdl;

    ptr = GetEnvString( "TMP" );
    if( ptr == nullptr )
        ptr = GetEnvString( "TMPDIR" );
    if( ptr != nullptr ) {
        path = ptr;
        if( path.empty() || !IS_PATH_SEP( path.back() ) ) {
            path += DIR_SEP;
        }
    }
    path += TEMPFNAME;
    last = path.size() - 1;             // the extension character that is changed until the name is free
    *fname = MemStrdupSafe( path.c_str() );
    tlen = 0;
    for( ;; ) {
        if( tlen >= 26 ) {
            LnkMsg( FTL+MSG_TMP_ALREADY_EXISTS, nullptr );
        }
        (*fname)[last] += 1;            // change temp file extension
        fhdl = TempFileOpen( *fname );
        if( fhdl == NIL_FHANDLE )
            break;
        QClose( fhdl, *fname );
        ++tlen;
    }
    return( QOpenRW( *fname ) );
}

static void SetupImpLib( void )
/*****************************/
{
    implibinfo  &ImpLib = loadFileState().implib;
    const char  *fname;
    size_t      namelen;
    unsigned    i;

    ImpLib.bufsize = 0;
    ImpLib.handle = NIL_FHANDLE;
    ImpLib.buffer = nullptr;
    ImpLib.module_name = nullptr;
    ImpLib.didone = false;
    if( wlink().FmtData.make_implib ) {
        if( wlink().FmtData.make_impfile ) {
            ImpLib.fname = MemStrdupSafe( wlink().FmtData.implibname );
            ImpLib.handle = QOpenRW( ImpLib.fname );
        } else {
            ImpLib.handle = openTempFile( &ImpLib.fname );
            ImpLib.temp = ( ImpLib.handle != NIL_FHANDLE );
        }
        if( ImpLib.handle != NIL_FHANDLE ) {
            ImpLib.buffer = static_cast<char *>( MemAllocSafe( IMPLIB_BUFSIZE ) );
            /*
             * for NE format (16-bit OS/2 and Windows 3.x)
             * - get base file name only
             * - uppercase name
             * for LE and LX format (OS/2 + Windows VxD + 32-bit DOS)
             * - get base file name with extension
             * - uppercase name
             * for PE format
             * - get base file name with extension
             * - preserves the case of name
             */
            fname = GetBaseName( wlink().Root->outfile->fname, 0, &namelen );
            if( wlink().FmtData.type & MK_NE ) {
                if( namelen > 8 ) {
                    namelen = 8;
                }
            } else {
                /*
                 * use base name + extension
                 */
                namelen = strlen( fname );
            }
            ImpLib.module_name_len = namelen;
            ImpLib.module_name = static_cast<char *>( MemAllocSafe( namelen ) );
            if( wlink().FmtData.type & (MK_NE | MK_OS2_FLAT | MK_WIN_VXD) ) {
                /*
                 * upper-case module name
                 */
                for( i = 0; i < namelen; i++ ) {
                    ImpLib.module_name[i] = static_cast<char>( toupper( static_cast<unsigned char>( fname[i] ) ) );
                }
            } else { /* FmtData.type & MK_PE */
                memcpy( ImpLib.module_name, fname, namelen );
            }
        }
    }
}

#if defined( __UNIX__ )
#define CVPACK_EXE "cvpack"
#else
#define CVPACK_EXE "cvpack.exe"
#endif

static void DoCVPack( void )
/**************************/
{
#if !defined( __UNIX__ ) || defined(__WATCOMC__)
    int         retval;
    const char  *name;

    if( (wlink().LinkFlags & LF_CVPACK_FLAG)
      && (wlink().LinkState & LS_LINK_ERROR) == 0 ) {
        if( wlink().SymFileName != nullptr ) {
            name = wlink().SymFileName;
        } else {
            name = wlink().Root->outfile->fname;
        }
        retval = static_cast<int>( spawnlp( P_WAIT, CVPACK_EXE, CVPACK_EXE, "/nologo",
                          name, nullptr ) );
        if( retval == -1 ) {
            PrintIOError( ERR+MSG_CANT_EXECUTE, "12", CVPACK_EXE );
        }
    }
#endif
}

static void OpenOutFiles( void )
/******************************/
{
    outfilelist   *fnode;

    for( fnode = wlink().OutFiles; fnode != nullptr; fnode = fnode->next ) {
        OpenBuffFile( fnode );
    }
}

static void CloseOutFiles( void )
/*******************************/
{
    outfilelist     *fnode;

    for( fnode = wlink().OutFiles; fnode != nullptr; fnode = fnode->next ) {
        if( fnode->handle != NIL_FHANDLE ) {
            CloseBuffFile( fnode );
        }
    }
}

void ResetLoadFile( void )
/************************/
{
    ClearStartAddr();
}

void CleanLoadFile( void )
/************************/
/* CodeBreaker: the import library's temporary file goes whatever became of the link. wlink left it when the link had an
 * error or stopped (BuildImpLib is not reached, or returns first): in one process, after 26 such links every name
 * IMP02112.xx? was taken and each link with `option implib` failed ("cannot open temporary file"). */
{
    implibinfo  &ImpLib = loadFileState().implib;

    if( ImpLib.temp ) {
        if( ImpLib.handle != NIL_FHANDLE ) {
            QClose( ImpLib.handle, ImpLib.fname );
            ImpLib.handle = NIL_FHANDLE;
        }
        QDelete( ImpLib.fname );
        MemFree( ImpLib.fname );
        ImpLib.fname = nullptr;
        ImpLib.temp = false;
    }
}

void InitLoadFile( void )
/************************
 * open the file, and write out header info
 */
{
    DEBUG(( DBG_OLD, "InitLoadFile()" ));
    LnkMsg( INF+MSG_CREATE_EXE, "f" );
}

static void finiLoad( void )
/**************************/
{
    /*******************************************************************
     * This must be first of all
     */
#ifdef _RAW
    if( wlink().FmtData.output_raw ) {
        BinOutput();    // apply to all formats and override native output
        return;
    } else if( wlink().FmtData.output_hex ) {
        HexOutput();    // apply to all formats and override native output
        return;
    } else if( wlink().FmtData.type & MK_RAW ) {
        FiniRawLoadFile();
        return;
    }
#endif
    /*******************************************************************/
#ifdef _EXE
    if( wlink().FmtData.type & MK_DOS ) {
        FiniDOSLoadFile();
        return;
    }
#endif
#ifdef _OS2
  #if 0
    if( (wlink().FmtData.type & (MK_NE | MK_OS2_FLAT))
      && (wlink().LinkState & LS_HAVE_PPC_CODE) ) {
        // development temporarly on hold:
        FiniELFLoadFile();
        return;
    }
  #endif
    if( wlink().FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD) ) {
        FiniOS2FlatLoadFile();
        return;
    } else if( wlink().FmtData.type & MK_PE ) {
        FiniPELoadFile();
        return;
    } else if( wlink().FmtData.type & MK_NE ) {
        FiniOS2LoadFile();
        return;
    }
#endif
#ifdef _PHARLAP
    if( wlink().FmtData.type & MK_PHAR_LAP ) {
        FiniPharLapLoadFile();
        return;
    }
#endif
#ifdef _NOVELL
    if( wlink().FmtData.type & MK_NOVELL ) {
        FiniNovellLoadFile();
        return;
    }
#endif
#ifdef _DOS16M
    if( wlink().FmtData.type & MK_DOS16M ) {
        Fini16MLoadFile();
        return;
    }
#endif
#ifdef _QNX
    if( wlink().FmtData.type & MK_QNX ) {
        FiniQNXLoadFile();
        return;
    }
#endif
#ifdef _ELF
    if( wlink().FmtData.type & MK_ELF ) {
        FiniELFLoadFile();
        return;
    }
#endif
#ifdef _RDOS
    if( wlink().FmtData.type & MK_RDOS ) {
        FiniRdosLoadFile();
        return;
    }
#endif
}

void FiniLoadFile( void )
/************************
 * terminate writing of load file
 */
{
    wlink().CurrSect = wlink().Root;
    FreeSavedRelocs();
    OpenOutFiles();
    SetupImpLib();
    finiLoad();
    CloseOutFiles();
    DoCVPack();
}

static seg_leader *FindStack( section *sect )
/*******************************************/
{
    class_entry *class_;
    seg_leader  *seg;

    for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
        if( class_->flags & CLASS_STACK ) {
            return static_cast<seg_leader *>( RingFirst( class_->segs ) );
        }
    }
    if( (wlink().LinkState & LS_DOSSEG_FLAG) == 0 ) {
        seg = nullptr;
        for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
            while( (seg = static_cast<seg_leader *>( RingStep( class_->segs, seg ) )) != nullptr ) {
                if( seg->combine == COMBINE_STACK ) {
                    return( seg );
                }
            }
        }
    }
    return( nullptr );
}

static seg_leader *StackSegment( void )
/**************************************
 * Find stack segment.
 */
{
    seg_leader  *seg;

    seg = FindStack( wlink().Root );
    if( seg == nullptr ) {
#ifdef _EXE
        if( wlink().FmtData.type & MK_OVERLAYS ) {
            seg = FindStack( wlink().NonSect );
        }
#endif
    }
    return( seg );
}

void GetStkAddr( void )
/**********************
 * Find the address of the stack
 */
{
    if( (wlink().FmtData.type & MK_NOVELL) == 0
      && !wlink().FmtData.dll ) {
        if( wlink().StackSegPtr != nullptr ) {
            wlink().StackAddr.seg = wlink().StackSegPtr->seg_addr.seg;
            wlink().StackAddr.off = wlink().StackSegPtr->seg_addr.off + wlink().StackSegPtr->size;
        } else {
#ifdef _OS2
            if( (wlink().FmtData.type & MK_WIN_NE)
              && (wlink().LinkFlags & LF_STK_SIZE_FLAG) ) {
                PhoneyStack();
            } else {
#endif
                if( (wlink().FmtData.type & (MK_COM | MK_PE | MK_QNX | MK_ELF | MK_RDOS)) == 0 ) {
                    LnkMsg( WRN+MSG_STACK_NOT_FOUND, nullptr );
                    wlink().StackAddr.seg = 0;
                    wlink().StackAddr.off = 0;
                }
#ifdef _OS2
            }
#endif
        }
    }
}

static class_entry *LocateBSSClass( void )
/****************************************/
{
    class_entry *class_;
    section     *sect;

    sect = (wlink().Root->areas == nullptr) ? wlink().Root : wlink().NonSect;
    for( class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
        if( stricmp( class_->name.u.ptr, BSSClassName ) == 0 ) {
            return( class_ );
        }
    }
    return( nullptr );
}

static void DefABSSSym( const char *name )
/****************************************/
{
   symbol          *sym;

    sym = RefISymbol( name );
    if( (sym->info & SYM_DEFINED) == 0
      || (sym->info & SYM_LINK_GEN) ) {
        sym->info |= SYM_DEFINED | SYM_LINK_GEN;
#ifdef _EXE
        if( wlink().FmtData.type & MK_OVERLAYS ) {
            sym->u.d.ovlstate |= OVL_NO_VECTOR | OVL_FORCE;
        }
#endif
        SET_ADDR_UNDEFINED( sym->addr );
    }
 }

void DefBSSSyms( void )
/*********************/
{
    DefABSSSym( BSSStartSym );
    DefABSSSym( BSS_StartSym );
    DefABSSSym( BSSEndSym );
    DefABSSSym( BSS_EndSym );
}

static bool CompSymPtr( void *sym, void *chk )
/********************************************/
{
    return( chk == sym );
}

static void CheckBSSInStart( symbol *sym, const char *name )
/***********************************************************
 * It's OK to define _edata if:
 *      1) the DOSSEG flag is not set
 *              or
 *      2) the definition occurs in the module containing the
 *          start addresses
 */
{
    symbol      *chk;

    chk = nullptr;
    if( wlink().StartInfo.mod != nullptr ) {
        chk = static_cast<symbol *>( Ring2Lookup( wlink().StartInfo.mod->publist, CompSymPtr, sym ) );
    }
    if( chk == nullptr ) {
        LnkMsg( ERR+MSG_RESERVED_SYM_DEFINED, "s", name );
    }
}

static void DefBSSStartSize( const char *name, class_entry *class_ )
/******************************************************************
 * set the value of an start symbol, and see if it has been defined
 */
{
    symbol      *sym;
    seg_leader  *seg;

    sym = FindISymbol( name );
    if( IS_ADDR_UNDEFINED( sym->addr ) ) {
        /* if the symbol was defined internally */
        seg = static_cast<seg_leader *>( RingFirst( class_->segs ) );
        sym->p.seg = static_cast<segdata *>( RingFirst( seg->pieces ) );
        sym->addr = seg->seg_addr;
        ConvertToFrame( &sym->addr, seg->group->addr.seg, ( (seg->info & SEGINF_USE_32) == 0 ) );
    } else if( wlink().LinkState & LS_DOSSEG_FLAG ) {
        CheckBSSInStart( sym, name );
    }
}

static void DefBSSEndSize( const char *name, class_entry *class_ )
/****************************************************************
 * set the value of an end symbol, and see if it has been defined
 */
{
    symbol      *sym;
    seg_leader  *seg;

    sym = FindISymbol( name );
    if( IS_ADDR_UNDEFINED( sym->addr ) ) {
        /*
         * if the symbol was defined internally
         * find last segment in BSS class
         */
        seg = static_cast<seg_leader *>( RingLast( class_->segs ) );
        /* set end of BSS class */
        sym->p.seg = static_cast<segdata *>( RingLast( seg->pieces ) );
        SET_SYM_ADDR( sym, seg->seg_addr.off + seg->size, seg->seg_addr.seg );
        ConvertToFrame( &sym->addr, seg->group->addr.seg, ( (seg->info & SEGINF_USE_32) == 0 ) );
    } else if( wlink().LinkState & LS_DOSSEG_FLAG ) {
        CheckBSSInStart( sym, name );
    }
}

void GetBSSSize( void )
/**********************
 * Find size of BSS segment, and set the special symbols
 */
{
    class_entry *class_;

    class_ = LocateBSSClass();
    if( class_ != nullptr ) {
        /* set start of BSS class */
        DefBSSStartSize( BSSStartSym, class_ );
        DefBSSStartSize( BSS_StartSym, class_ );
        DefBSSEndSize( BSSEndSym, class_ );
        DefBSSEndSize( BSS_EndSym, class_ );
    }
}

void SetStkSize( void )
/**********************
 * Stack size calculation:
 * - DLLs have no stack, size is 0
 * - for executables, warn if stack size is tiny
 * - if stack size was given, use it directly unless target is Novell
 * - else use the actual stack segment size if it is > 512 bytes
 * - otherwise use the default stack size
 * The default stack size is 4096 bytes, but for DOS programs the
 * stack segment size in the clib is smaller, hence the complex logic.
 */
{
    wlink().StackSegPtr = StackSegment();
    if( wlink().FmtData.dll ) {
        wlink().StackSize = 0;  // DLLs don't have their own stack
    } else {
        if( wlink().LinkFlags & LF_STK_SIZE_FLAG ) {
            if( wlink().StackSize < 0x200 ) {
                LnkMsg( WRN+MSG_STACK_SMALL, "d", 0x200 );
            }
#ifdef _OS2
        } else {
            if( wlink().FmtData.type & MK_PE ) {
                wlink().StackSize = DefStackSizePE();
                wlink().LinkFlags |= LF_STK_SIZE_FLAG;
            }
#endif
        }
    }
    if( wlink().StackSegPtr != nullptr ) {
        if( wlink().LinkFlags & LF_STK_SIZE_FLAG ) {
            if( (wlink().FmtData.type & MK_NOVELL) == 0 ) {
                wlink().StackSegPtr->size = wlink().StackSize;
            }
        } else {
            if( !wlink().FmtData.dll
              && wlink().StackSegPtr->size >= 0x200 ) {
                wlink().StackSize = wlink().StackSegPtr->size;
            } else {
                wlink().StackSegPtr->size = wlink().StackSize;
            }
        }
    }
}

void ClearStartAddr( void )
/*************************/
{
    memset( &wlink().StartInfo, 0, sizeof( startinfo ) );
}

void SetStartSym( const char *name )
/**********************************/
{
    size_t      namelen;

    if( wlink().StartInfo.type != START_UNDEFED ) {
        if( wlink().StartInfo.type == START_IS_SYM ) {
            namelen = strlen( name );
            if( namelen != strlen( wlink().StartInfo.targ.sym->name.u.ptr )
              || (*CmpNRtn)( wlink().StartInfo.targ.sym->name.u.ptr, name, namelen ) != 0 ) {
                LnkMsg( MILD_ERR+LOC+MSG_MULT_START_ADDRS_BY, "12", wlink().StartInfo.targ.sym->name.u.ptr, name );
            }
        } else {
            LnkMsg( MILD_ERR+LOC+MSG_MULT_START_ADDRS, "12", wlink().StartInfo.mod->u1.source->infile->name, wlink().StartInfo.mod->name );
        }
    } else {
        wlink().StartInfo.targ.sym = RefISymbol( name );
        wlink().StartInfo.targ.sym->info |= SYM_1_DCE_REF;
        wlink().StartInfo.type = START_IS_SYM;
        wlink().StartInfo.mod = wlink().CurrMod;
        if( wlink().LinkFlags & LF_STRIP_CODE ) {
            DataRef( wlink().StartInfo.targ.sym );
        }
    }
}

void GetStartAddr( void )
/***********************/
{
    bool        addoff;
    int         deltaseg;

#ifdef _NOVELL
    if( wlink().FmtData.type & MK_NOVELL ) {
        return;
    }
#endif
    addoff = true;
    switch( wlink().StartInfo.type ) {
    case START_UNDEFED:         // NOTE: the possible fall through
        addoff = false;
        if( !wlink().FmtData.dll ) {
            if( wlink().Groups == nullptr
              || (wlink().FmtData.type & MK_ELF) ) {
                wlink().StartInfo.addr.seg = 0;
                wlink().StartInfo.addr.off = 0;
            } else {
                wlink().StartInfo.addr = wlink().Groups->addr;
            }
            LnkMsg( WRN+MSG_NO_START_ADDR, "a", &wlink().StartInfo.addr );
        }
        break;
    case START_IS_SDATA:
        wlink().StartInfo.addr = wlink().StartInfo.targ.sdata->u.leader->seg_addr;
        wlink().StartInfo.addr.off += wlink().StartInfo.targ.sdata->a.delta;
        /* if startaddr is not in first segment and segment is part of */
        /* a group, adjust seg + off relative to start of group        */
        /* instead of start of seg. This allows far call optimization to work*/
        if( (wlink().StartInfo.targ.sdata->u.leader->seg_addr.seg > 0)
          && (wlink().StartInfo.targ.sdata->u.leader->group != nullptr) ) {
            deltaseg = wlink().StartInfo.targ.sdata->u.leader->seg_addr.seg
              - wlink().StartInfo.targ.sdata->u.leader->group->addr.seg;
            if( (deltaseg > 0)
              && (deltaseg <= wlink().StartInfo.targ.sdata->u.leader->seg_addr.seg) ) {
                wlink().StartInfo.addr.seg -= deltaseg;
                wlink().StartInfo.addr.off += 16 * deltaseg - wlink().StartInfo.targ.sdata->u.leader->group->addr.off;
            }
        }
        break;
    case START_IS_SYM:
        wlink().StartInfo.addr = wlink().StartInfo.targ.sym->addr;
        break;
    }
    if( addoff ) {
        wlink().StartInfo.addr.off += wlink().StartInfo.off;
    }
}

offset CalcGroupSize( group_entry *group )
/*****************************************
 * calculate the total memory size of a potentially split group
 */
{
    offset size;

    if(( group == wlink().DataGroup )
      && ( wlink().FmtData.dgroupsplitseg != nullptr )) {
        size = wlink().FmtData.dgroupsplitseg->seg_addr.off - group->addr.off - wlink().FmtData.bsspad;
        DbgAssert( size >= group->size );
    } else {
        size = group->totalsize;
    }
    return( size );
}

offset CalcSplitSize( void )
/***************************
 * calculate the size of the uninitialized portion of a group
 */
{
    offset size;

    if( wlink().FmtData.dgroupsplitseg == nullptr ) {
        return( 0 );
    } else {
        size = wlink().DataGroup->totalsize - (wlink().FmtData.dgroupsplitseg->seg_addr.off - wlink().DataGroup->addr.off);
        if( wlink().StackSegPtr != nullptr ) {
            size -= wlink().StackSize;
        }
        return( size );
    }
}

bool CompareDosSegments( addr_type *left, addr_type *right )
/**********************************************************/
{
    return( LESS_THAN_ADDR( *left, *right ) );
}

bool CompareOffsets( addr_type *left, addr_type *right )
/******************************************************/
{
    return( left->off < right->off );
}

bool CompareProtSegments( addr_type *left, addr_type *right )
/***********************************************************/
{
    if( left->seg == right->seg ) {
        return( left->off < right->off );
    }
    return( left->seg < right->seg );
}

void OrderGroups( bool (*lessthan)(addr_type *, addr_type *) )
/************************************************************/
{
    group_entry     *group;
    group_entry     *low_group;
    group_entry     *first_group;
    group_entry     **last_group;
    addr_type       *low_addr;
    addr_type       *group_addr;

    first_group = wlink().Groups;
    last_group = &wlink().Groups;
    while( first_group != nullptr ) {
        low_addr = &first_group->addr;
        low_group = nullptr;
        for( group = first_group; group->next != nullptr; group = group->next ) {
            group_addr = &group->next->addr;
            if( lessthan( group_addr, low_addr ) ) {
                low_addr = group_addr;
                low_group = group;
            }
        }
        if( low_group == nullptr ) {
            *last_group = first_group;
            last_group = &(first_group->next);
            first_group = *last_group;
        } else {
            *last_group = low_group->next;
            last_group = &(low_group->next->next);
            low_group->next = *last_group;
        }
    }
}

bool WriteGroup( group_entry *group )
/************************************
 * write the data for group to the loadfile
 * returns true if the file should be repositioned
 */
{
    unsigned long       file_loc;
    section             *sect;
    bool                repos;
    outfilelist         *finfo;

    repos = false;
    if( group->size != 0 ) {
        sect = group->section;
        wlink().CurrSect = sect;
        finfo = sect->outfile;
        file_loc = GROUP_FILE_LOC( group );
        if( file_loc > finfo->file_loc ) {
            PadLoad( file_loc - finfo->file_loc );
        } else if( file_loc != finfo->file_loc ) {
            SeekLoad( file_loc );
            repos = true;
        }
#ifdef _EXE
        if( wlink().FmtData.type & MK_OVERLAYS ) {
            OvlSetTableLoc( group, file_loc );
        }
#endif
        DEBUG((DBG_LOADDOS, "group %a section %d to %l in %s",
                &group->addr, sect->ovlref, file_loc, finfo->fname ));
        file_loc += WriteGroupLoad( group, repos );
        if( file_loc > finfo->file_loc ) {
            finfo->file_loc = file_loc;
        }
    }
    return( repos );
}

unsigned_32 MemorySize( void )
/*****************************
 * Compute size of image when loaded into memory.
 */
{
#ifdef _EXE
    unsigned_32         start;
    unsigned_32         end;
    unsigned_32         curr;
    section             *sect;
    areasect            *area;

    if( wlink().FmtData.type & MK_REAL_MODE ) {
        start = MK_REAL_ADDR( wlink().Root->sect_addr.seg, wlink().Root->sect_addr.off );
        end = start + wlink().Root->size;
        for( area = wlink().Root->areas; area != nullptr; area = area->next ) {
            for( sect = area->sections; sect != nullptr; sect = sect->next ) {
                curr = MK_REAL_ADDR( sect->sect_addr.seg, sect->sect_addr.off ) + sect->size;
                if( curr > end ) {
                    end = curr;
                }
            }
        }
        return( end - start );
    }
#endif
    return( wlink().Root->size );
}

unsigned_32 AppendToLoadFile( const char *name )
/**********************************************/
{
    f_handle        handle;
    unsigned_32     wrote;

    wrote = 0;
    if( name != nullptr ) {
        handle = QOpenR( name );
        wrote = CopyToLoad( handle, name );
        QClose( handle, name );
    }
    return( wrote );
}

#ifdef BOOTSTRAP
#define WLIB_NAME   "bwlib"
#else
#define WLIB_NAME   "wlib"
#endif
#if defined( DLLS_IMPLEMENTED )
#define WLIB_EXE WLIB_NAME "d.dll"
#elif defined( __UNIX__ )
#define WLIB_EXE WLIB_NAME
#else
#define WLIB_EXE WLIB_NAME ".exe"
#endif

// the wlib option that names the machine of the import library
static const char *wlibMachineOption( void )
{
    switch( wlink().LinkState & LS_HAVE_MACHTYPE_MASK ) {
    case LS_HAVE_ALPHA_CODE:
        return( "-ia" );
    case LS_HAVE_MIPS_CODE:
        return( "-im" );
    case LS_HAVE_PPC_CODE:
        return( "-ip" );
    case LS_HAVE_X64_CODE:
        return( "-i6" );
    case LS_HAVE_X86_CODE:
        return( "-ii" );
    default:
        return( "-ii" );
    }
}

static void ExecWlib( void )
/**************************/
{
    implibinfo  &ImpLib = loadFileState().implib;
#if defined( DLLS_IMPLEMENTED )
    std::string cmdline;

    cmdline = "-c -b -n -q -pa ";
    cmdline += wlibMachineOption();
    cmdline += " \"";
    cmdline += wlink().FmtData.implibname;
    cmdline += "\" @\"";
    cmdline += ImpLib.fname;
    cmdline += '"';
    if( ExecDLLPgm( WLIB_EXE, cmdline.c_str() ) ) {
        PrintIOError( ERR+MSG_CANT_EXECUTE, "12", WLIB_EXE );
    }
#else
    std::string atfname;
    int         retval;

    atfname = '@';
    atfname += ImpLib.fname;
    retval = static_cast<int>( spawnlp( P_WAIT, WLIB_EXE, WLIB_EXE, "-c", "-b", "-n", "-q", "-pa",
                  wlibMachineOption(), wlink().FmtData.implibname, atfname.c_str(), nullptr ) );
    if( retval == -1 ) {
        PrintIOError( ERR+MSG_CANT_EXECUTE, "12", WLIB_EXE );
    }
#endif
}

static void FlushImpBuffer( void )
/********************************/
{
    implibinfo  &ImpLib = loadFileState().implib;

    QWrite( ImpLib.handle, ImpLib.buffer, ImpLib.bufsize, ImpLib.fname );
}

void BuildImpLib( void )
/**********************/
{
    implibinfo  &ImpLib = loadFileState().implib;

    if( wlink().LinkState & LS_LINK_ERROR )
        return;
    if( wlink().FmtData.make_implib ) {
        if( ImpLib.handle != NIL_FHANDLE ) {
            if( ImpLib.bufsize > 0 ) {
                FlushImpBuffer();
            }
            QClose( ImpLib.handle, ImpLib.fname );
            ImpLib.handle = NIL_FHANDLE;
            if( !wlink().FmtData.make_impfile ) {
                if( ImpLib.didone ) {
                    ExecWlib();
                }
                QDelete( ImpLib.fname );
                ImpLib.temp = false;
            }
            MemFree( ImpLib.buffer );
            MemFree( ImpLib.module_name );
        }
        MemFree( ImpLib.fname );
        ImpLib.fname = nullptr;
        MemFree( wlink().FmtData.implibname );
    }
}

static void BufImpWrite( const char *buffer, size_t len )
/*******************************************************/
{
    implibinfo  &ImpLib = loadFileState().implib;
    size_t      diff;

    if( ImpLib.bufsize + len >= IMPLIB_BUFSIZE ) {
        diff = ImpLib.bufsize + len - IMPLIB_BUFSIZE;
        memcpy( ImpLib.buffer + ImpLib.bufsize , buffer, IMPLIB_BUFSIZE - ImpLib.bufsize );
        ImpLib.bufsize = IMPLIB_BUFSIZE;
        FlushImpBuffer();
        ImpLib.bufsize = diff;
        if( diff > 0 ) {
            memcpy( ImpLib.buffer, buffer + len - diff, diff );
        }
    } else {
        memcpy( ImpLib.buffer + ImpLib.bufsize, buffer, len );
        ImpLib.bufsize += len;
    }
}

void AddImpLibEntry( const char *intname, const char *extname, ordinal_t ordinal, bool by_name )
/**********************************************************************************************/
{
    LoadFileState   &state = loadFileState();
    implibinfo      &ImpLib = state.implib;

    if( wlink().FmtData.make_implib
      && ImpLib.handle != NIL_FHANDLE ) {
        std::string &buff = state.entry;

        ImpLib.didone = true;
        buff.assign( "++'" );
        buff += intname;
        buff += "'.'";
        buff.append( ImpLib.module_name, ImpLib.module_name_len );
        buff += "'.";
        if( by_name ) {
            buff += ".'";
            buff += extname;
            buff += '\'';
        } else {
            buff += std::to_string( static_cast<long>( ordinal ) );
        }
#if !defined( __UNIX__ )
        buff += '\r';
#endif
        buff += '\n';
        BufImpWrite( buff.data(), buff.size() );
    }
}

void WriteLoad3( void *dummy, const char *buff, size_t size )
/************************************************************
 * write a buffer out to the load file (useful as a callback)
 */
{
    /* unused parameters */ (void)dummy;

    WriteLoad( buff, size );
}

unsigned_32 CopyToLoad( f_handle handle, const char *name )
/*********************************************************/
{
    size_t          amt_read;
    unsigned_32     wrote;

    wrote = 0;
    amt_read = QRead( handle, wlink().TokBuff, wlink().TokSize, name );
    while( amt_read != 0 && amt_read != IOERROR ) {
        WriteLoad( wlink().TokBuff, amt_read );
        wrote += amt_read;
        amt_read = QRead( handle, wlink().TokBuff, wlink().TokSize, name );
    }
    return( wrote );
}

unsigned long NullAlign( unsigned align )
/****************************************
 * align loadfile -- assumed power of two alignment
 */
{
    unsigned long   off;
    size_t          pad;

    off = PosLoad();
    pad = __ROUND_UP_SIZE( off, align ) - off;
    PadLoad( pad );
    return( off + pad );
}

unsigned long OffsetAlign( unsigned long off, unsigned long align )
/******************************************************************
 * align loadfile -- assumed power of two alignment
 */
{
    size_t          pad;

    pad = __ROUND_UP_SIZE( off, align ) - off;
    PadLoad( pad );
    return( off + pad );
}

static bool WriteSegData( void *_sdata, void *_info )
/***************************************************/
{
    segdata         *sdata = static_cast<segdata *>( _sdata );
    grpwriteinfo    *info = static_cast<grpwriteinfo *>( _info );
    unsigned long   newpos;
    unsigned long   oldpos;

    if( !sdata->isuninit
      && !sdata->isdead
      && ( sdata->length > 0 ) ) {
        newpos = info->seg_start + sdata->a.delta;
        if( info->repos ) {
            SeekLoad( newpos );
        } else {
            oldpos = PosLoad();
            DbgAssert( newpos >= oldpos );
            PadLoad( newpos - oldpos );
        }
        if( sdata->vm_data == 0 ) {
            sdata->vm_data = sdata->u1.vm_ptr;
        }
        if( !info->copy ) {
            sdata->u1.vm_offs = newpos;   // for incremental linking
        }
        WriteInfoLoad( sdata->vm_data, sdata->length );
    }
    return( false );
}

static void DoWriteLeader( seg_leader *seg, grpwriteinfo *info )
/**************************************************************/
{
    RingLookup( seg->pieces, WriteSegData, info );
}

void WriteLeaderLoad( void *seg )
/*******************************/
{
    grpwriteinfo    info;

    info.seg_start = PosLoad();
    DoWriteLeader( static_cast<seg_leader *>( seg ), &info );
}

static bool DoGroupLeader( void *_seg, void *_info )
/**************************************************/
{
    seg_leader      *seg = static_cast<seg_leader *>( _seg );
    grpwriteinfo    *info = static_cast<grpwriteinfo *>( _info );

    // If class or sector should not be output, skip it
    if( EMIT_CLASS( seg->class_ )
      && EMIT_SEG( seg ) ) {
        info->seg_start = info->grp_start + SEG_GROUP_DELTA( seg );
        DoWriteLeader( seg, info );
    }
    return( false );
}

static bool DoDupGroupLeader( void *seg, void *_info )
/*****************************************************
 * Substitute groups generally are sourced from NO_EMIT classes,
 * As copies, they need to be output, so ignore their MOEMIT flag here
 */
{
    grpwriteinfo    *info = static_cast<grpwriteinfo *>( _info );

    info->seg_start = info->grp_start + SEG_GROUP_DELTA( static_cast<seg_leader *>( seg ) );
    DoWriteLeader( static_cast<seg_leader *>( seg ), info );
    return( false );
}

static bool WriteCopyGroups( void *_seg, void *_info )
/*****************************************************
 * This is called by the outer level iteration looking for classes
 * that have more than one group in them
 */
{
    seg_leader      *seg = static_cast<seg_leader *>( _seg );
    grpwriteinfo    *info = static_cast<grpwriteinfo *>( _info );

    if( info->last_group != seg->group ) {   // Only interate new groups
        info->last_group = seg->group;
        // Check each initialized segment in group
        Ring2Lookup( seg->group->leaders, DoDupGroupLeader, info );
        info->grp_start += seg->group->totalsize;
    }
    return( false );
}

offset  WriteGroupLoad( group_entry *group, bool repos )
/******************************************************/
{
    grpwriteinfo     info;
    class_entry      *class_;
    unsigned_32      grp_start;

    grp_start = PosLoad();
    info.repos = repos;
    info.grp_start = grp_start;
    // If group is a copy group, substitute source group(s) here
    class_ = group->leaders->class_;
    if( class_->flags & CLASS_COPY ) {
        info.copy = true;
        info.last_group = nullptr; // so it will use the first group
        RingLookup( class_->DupClass->segs->group->leaders, WriteCopyGroups, &info );
    } else {
        info.copy = false;
        Ring2Lookup( group->leaders, DoGroupLeader, &info );
    }
    return( PosLoad() - grp_start );
}

void FreeOutFiles( void )
/***********************/
{
    outfilelist     *fnode;

    CloseOutFiles();
    for( fnode = wlink().OutFiles; fnode != nullptr; fnode = wlink().OutFiles ) {
        if( wlink().LinkState & LS_LINK_ERROR ) {
            QDelete( fnode->fname );
        }
        MemFree( fnode->fname );
        wlink().OutFiles = fnode->next;
        MemFree( fnode );
    }
}

static void *SetToFillChar( void *dest, const void *data, size_t size )
/*********************************************************************/
{
    memset( dest, wlink().FmtData.FillChar, size );
    return( const_cast<void *>( data ) );
}

static void *CopyData( void *dest, const void *data, size_t size )
/****************************************************************/
{
    return( memcpy( dest, data, size ) );
}

#define BUFF_BLOCK_SIZE _16K

static void WriteBuffer( const char *data, size_t len, outfilelist *outfile, writebuffer_fn *rtn )
/************************************************************************************************/
{
    size_t   modpos;
    size_t   adjust;

    modpos = outfile->bufpos % BUFF_BLOCK_SIZE;
    outfile->bufpos += len;
    while( modpos + len >= BUFF_BLOCK_SIZE ) {
        adjust = BUFF_BLOCK_SIZE - modpos;
        rtn( outfile->buffer + modpos, data, adjust );
        QWrite( outfile->handle, outfile->buffer, BUFF_BLOCK_SIZE, outfile->fname );
        if( data != nullptr )
            data += adjust;
        len -= adjust;
        modpos = 0;
    }
    if( len > 0 ) {
        rtn( outfile->buffer + modpos, data, len );
    }
}

static void SeekBuffer( unsigned long len, outfilelist *outfile, writebuffer_fn *rtn )
/************************************************************************************/
{
    size_t   modpos;
    size_t   adjust;

    modpos = outfile->bufpos % BUFF_BLOCK_SIZE;
    outfile->bufpos += len;
    while( modpos + len >= BUFF_BLOCK_SIZE ) {
        adjust = BUFF_BLOCK_SIZE - modpos;
        rtn( outfile->buffer + modpos, nullptr, adjust );
        QWrite( outfile->handle, outfile->buffer, BUFF_BLOCK_SIZE, outfile->fname );
        len -= adjust;
        modpos = 0;
    }
    if( len > 0 ) {
        rtn( outfile->buffer + modpos, nullptr, len );
    }
}

void PadBuffFile( outfilelist *outfile, size_t size )
/***************************************************/
/* pad out load file with zeros */
{
    if( size == 0 )
        return;
    if( outfile->buffer != nullptr ) {
        WriteBuffer( nullptr, size, outfile, SetToFillChar );
    } else {
        WriteNulls( outfile->handle, size, outfile->fname );
    }
}

void PadLoad( size_t size )
/**************************
 * pad out load file with zeros
 */
{
    PadBuffFile( wlink().CurrSect->outfile, size );
}

void WriteLoad( const void *buff, size_t size )
/**********************************************
 * write a buffer out to the load file
 */
{
    outfilelist         *outfile;

    outfile = wlink().CurrSect->outfile;
    if( outfile->buffer != nullptr ) {
        WriteBuffer( static_cast<const char *>( buff ), size, outfile, CopyData );
    } else {
        QWrite( outfile->handle, buff, size, outfile->fname );
    }
}

void WriteLoadU8( unsigned_8 data )
/*********************************/
{
    WriteLoad( &data, sizeof( data ) );
}

void WriteLoadU16( unsigned_16 data )
/***********************************/
{
    WriteLoad( &data, sizeof( data ) );
}

void WriteLoadU32( unsigned_32 data )
/***********************************/
{
    WriteLoad( &data, sizeof( data ) );
}

size_t WriteLoadU8Name( const char *data, size_t len, bool ucase )
/****************************************************************/
{
    std::array<char, 255>   buff;
    size_t                  i;

    if( len > 255 )
        len = 255;
    WriteLoadU8( static_cast<unsigned_8>( len ) );
    if( ucase ) {
        for( i = 0; i < len; ++i ) {
            buff[i] = static_cast<char>( toupper( static_cast<unsigned char>( data[i] ) ) );
        }
        data = buff.data();
    }
    WriteLoad( data, len );
    return( len + 1 );
}

static void FlushBuffFile( outfilelist *outfile )
/***********************************************/
{
    size_t  modpos;

    modpos = outfile->bufpos % BUFF_BLOCK_SIZE;
    if( modpos != 0 ) {
        QWrite( outfile->handle, outfile->buffer, modpos, outfile->fname );
    }
    MemFree( outfile->buffer );
    outfile->buffer = nullptr;
}

void SeekLoad( unsigned long offset )
/***********************************/
{
    outfilelist         *outfile;

    outfile = wlink().CurrSect->outfile;
    if( outfile->buffer != nullptr
      && ( outfile->origin + offset ) < outfile->bufpos ) {
        FlushBuffFile( outfile );
    }
    if( outfile->buffer == nullptr ) {
        QSeek( outfile->handle, outfile->origin + offset, outfile->fname );
    } else {
        SeekBuffer( outfile->origin + offset - outfile->bufpos, outfile, SetToFillChar );
    }
}

void SeekEndLoad( unsigned long offset )
/**************************************/
{
    outfilelist         *outfile;

    outfile = wlink().CurrSect->outfile;
    if( outfile->buffer != nullptr
      && offset > 0 ) {
        FlushBuffFile( outfile );
    }
    if( outfile->buffer == nullptr ) {
        QLSeek( outfile->handle, offset, LSEEK_END, outfile->fname );
    }
}

unsigned long PosLoad( void )
/***************************/
{
    if( wlink().CurrSect->outfile->buffer != nullptr ) {
        return( wlink().CurrSect->outfile->bufpos - wlink().CurrSect->outfile->origin );
    } else {
        return( QPos( wlink().CurrSect->outfile->handle ) - wlink().CurrSect->outfile->origin );
    }
}

void InitBuffFile( outfilelist *outfile, char *filename, bool executable )
/************************************************************************/
{
    outfile->fname    = filename;
    outfile->handle   = NIL_FHANDLE;
    outfile->file_loc = 0;
    outfile->bufpos   = 0;
    outfile->buffer   = nullptr;
    outfile->ovlfnoff = 0;
    outfile->is_exe   = executable;
    outfile->origin   = 0;
}

void SetOriginLoad( unsigned long origin )
/****************************************/
{
    wlink().CurrSect->outfile->origin = origin;
}

void OpenBuffFile( outfilelist *outfile )
/***************************************/
{
    if( outfile->is_exe ) {
        outfile->handle = ExeCreate( outfile->fname );
    } else {
        outfile->handle = QOpenRW( outfile->fname );
    }
    if( outfile->handle == NIL_FHANDLE ) {
        PrintIOError( FTL+MSG_CANT_OPEN_NO_REASON, "s", outfile->fname );
    }
    outfile->buffer = static_cast<char *>( MemAllocSafe( BUFF_BLOCK_SIZE ) );
}

void CloseBuffFile( outfilelist *outfile )
/****************************************/
{
    if( outfile->buffer != nullptr ) {
        FlushBuffFile( outfile );
    }
    QClose( outfile->handle, outfile->fname );
    outfile->handle = NIL_FHANDLE;
}
