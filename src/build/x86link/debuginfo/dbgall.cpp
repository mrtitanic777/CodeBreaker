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
* Description:  General routines for producing debugging information in
*               load file.
*
****************************************************************************/


#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/link/specials.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/base/virtmem.h"
#include "exeelf.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/debuginfo/dbgcv.h"
#include "build/x86link/debuginfo/dbghll.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/debuginfo/dbgdwarf.h"
#include "build/x86link/debuginfo/dbgwat.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/formats/novell.h"

#include "clibext.h"


typedef struct {
    FunctionRef<line_walk_fn> cbfn;
} line_walk_data;


void ResetDBI(Link& link)
/*******************/
{
    link.SymFileName = nullptr;
    link.DBIGroups = nullptr;
    // wlink.exe starts each link with this 0 (only a PE link sets it): the DEBUG_TYPE_MISC block of any other format
    // ends with it (special_purpose), and a PE link earlier in the same process had left its file offset here
    link.CVDebugDirEntryPos = 0;
}

void DBIInit(Link& link)
/******************/
// called just after command file parsing
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIInit( link.Root );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVInit();
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfInit();
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllInit();
    }
}

void DBISectInit(Link& link,  section *sect )
/*******************************/
// called when a section created in command file parsing
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIInit( sect );
    }
}

void DBIInitModule(Link& link,  mod_entry *obj )
/**********************************/
// called before pass 1 is done on the module
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIInitModule(link,  obj );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfInitModule( obj );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVInitModule( obj );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllInitModule( obj );
    }
}

void DBIP1Source(Link& link,  const byte *buff, const byte *endbuff )
/*******************************************************/
{
    byte            major;
    byte            minor;

    major = *buff++;
    minor = *buff++;
    if( endbuff <= buff ) {
        BadObject(link);
        return;
    }
    link.ObjFormat |= OBJ_FMT_DEBUG_COMENT;
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIP1Source(link,  major, minor, reinterpret_cast<const char *>( buff ), endbuff - buff );
    }
}

section *DBIGetSect(Link& link,  const char *clname )
/***************************************/
{
    if( ( stricmp( clname, _MSTypeClass ) == 0 )
      || ( stricmp( clname, _MSLocalClass ) == 0 )
      || stricmp( clname, _HllLineClass ) == 0) {
        return( link.CurrSect );
    } else if( stricmp( clname, _DwarfClass ) == 0 ) {
        return( link.Root );
    }
    return( nullptr );
}

void DBIColClass( class_entry *class_ )
/************************************/
{
    if( stricmp( class_->name.u.ptr, _DwarfClass ) == 0 ) {
        class_->flags |= CLASS_DWARF;
    } else if( stricmp( class_->name.u.ptr, _MSTypeClass ) == 0 ) {
        class_->flags |= CLASS_MS_TYPE;
    } else if( stricmp( class_->name.u.ptr, _MSLocalClass ) == 0 ) {
        class_->flags |= CLASS_MS_LOCAL;
    } else if( stricmp( class_->name.u.ptr, _HllLineClass ) == 0 ) {
        class_->flags |= CLASS_HLL_LINE;
    }
}

unsigned_16 DBIColSeg(Link& link,  class_entry *class_ )
/*****************************************/
{
    switch( class_->flags & CLASS_DEBUG_INFO ) {
    case CLASS_DWARF:
        if( link.CurrMod->flags_dbi & DBI_TYPE ) {
            link.CurrMod->flags_mod |= MOD_DBI_SEEN;
        }
        return( DWARF_DEBUG_OTHER );       // assume other until later.
    case CLASS_MS_TYPE:
        return( MS_TYPE );
    case CLASS_MS_LOCAL:
        return( MS_LOCAL );
    case CLASS_HLL_LINE:
        return( HLL_LINE );
    }
    return( NOT_DEBUGGING_INFO );
}

void DBIP1ModuleScanned(Link& link)
/*****************************/
// called in pass 1 when finished looking at a module
// if some segdefs have been delayed due to distributing libraries, this
// will be called twice (once when regular pass 1 is done, once when all
// segdefs processed
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIP1ModuleScanned(link);
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfP1ModuleScanned();
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVP1ModuleScanned();
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllP1ModuleScanned();
    }
}

static bool MSSkip(Link& link)
/************************/
{
    bool            iscv;
    bool            seencmt;

    if( link.ObjFileFormat != FILE_FMT_OMF ) {
        return( link.LinkFlags & LF_DWARF_DBI_FLAG );
    } else {
        iscv = ( (link.LinkFlags & (LF_CV_DBI_FLAG | LF_HLL_DBI_FLAG) ) != 0 );
        seencmt = ( (link.ObjFormat & OBJ_FMT_DEBUG_COMENT) != 0 );
        return( (iscv ^ seencmt) == 0 || (link.LinkFlags & LF_DWARF_DBI_FLAG) );
    }
}

bool DBISkip(Link& link,  seg_leader *seg )
/*****************************/
// returns true we should skip processing this segment because we are
// ignoring debugging information
{
    switch( seg->dbgtype ) {
    case MS_TYPE:
        return( (link.CurrMod->flags_dbi & DBI_TYPE) == 0 || MSSkip(link) );
    case MS_LOCAL:
        return( (link.CurrMod->flags_dbi & DBI_LOCAL) == 0 || MSSkip(link) );
    case HLL_LINE:
        return( (link.CurrMod->flags_dbi & DBI_LINE) == 0 || MSSkip(link) );
    case NOT_DEBUGGING_INFO:
        return( false );
    default:
        return( (link.CurrMod->flags_dbi & DBI_TYPE) == 0 || (link.LinkFlags & LF_DWARF_DBI_FLAG) == 0 );
    }
}

bool DBINoReloc( seg_leader *seg )
/********************************/
// called to see if we should handle a relocation specially.
{
    return( IS_DBG_INFO( seg ) );
}

static void AddNovGlobals( mod_entry *mod )
/*****************************************/
{
    Link& link = wlink();
#ifdef _NOVELL
    Ring2Walk( mod->publist, bind_link( link, NovDBIAddGlobal ) );
#else
    /* unused parameters */ (void)mod;
#endif
}

void DBIPreAddrCalc(Link& link)
/*************************/
{
    void (*modptr)( mod_entry * );
    void (*segptr)( seg_leader * );

    if( link.LinkFlags & LF_NOVELL_DBI_FLAG ) {
        WalkMods(link,  AddNovGlobals );
    }
    if( (link.LinkFlags & LF_ANY_DBI_FLAG) == 0 )
        return;
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        modptr = ODBIP1ModuleFinished;
        segptr = ODBIAddAddrInfo;
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        modptr = DwarfP1ModuleFinished;
        segptr = DwarfAddAddrInfo;
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        modptr = HllP1ModuleFinished;
        segptr = HllAddAddrInfo;
    } else {
        modptr = CVP1ModuleFinished;
        segptr = CVAddAddrInfo;
    }
    WalkMods(link,  modptr );
    WalkLeaders(link,  segptr );
    if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        WalkMods(link,  bind_link( link, DwarfStoreAddrInfo ) );
    }
}

void DBIAddrInfoScan(Link& link,  seg_leader *seg,
                         FunctionRef<void( segdata *, void * )> initfn,
                         FunctionRef<void( segdata *, offset, offset, void *, bool )> addfn,
                         void *cookie )
/********************************************************************************/
{
    segdata         *prev;
    segdata         *curr;
    offset          size;
    bool            isnewmod;

    if( IS_DBG_INFO( seg ) )
        return;
    if( link.FmtData.dll
      || (link.FmtData.type & MK_PE) ) {
        if( seg->class_->flags & (CLASS_STACK | CLASS_IDATA) )
            return;
        if( seg->combine == COMBINE_STACK ) {
            if( (link.LinkState & LS_DOSSEG_FLAG) == 0 ) {
                return;
            }
        }
    }
    prev = static_cast<segdata *>( RingStep( seg->pieces, nullptr ) );
    for( ;; ) {
        if( prev == nullptr )
            return;
        if( !prev->isdead )
            break;
        prev = static_cast<segdata *>( RingStep( seg->pieces, prev ) );
    }
    initfn( prev, cookie );
    size = 0;
    for( curr = prev; (curr = static_cast<segdata *>( RingStep( seg->pieces, curr ) )) != nullptr; ) {
        if( !curr->isdead ) {
            size += curr->a.delta - prev->a.delta;
            isnewmod = ( ( curr->o.mod != prev->o.mod ) && ( size != 0 ) );
            addfn( prev, curr->a.delta, size, cookie, isnewmod );
            if( isnewmod ) {
                size = 0;
            }
            prev = curr;
        }
    }
    size += prev->length;
    addfn( prev, 0, size, cookie, size != 0 );
}

void DBIComment( void )
/*********************/
{
}

void DBIAddModule(Link& link,  mod_entry *obj, section *sect )
/************************************************/
// called just before publics have been assigned addresses between p1 & p2
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddModule( obj, sect );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfAddModule(link,  obj, sect );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddModule(link,  obj, sect );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddModule(link,  obj, sect );
    }
}

static void DBIGenLocal(Link& link,  void *sdata )
/************************************/
// called during pass 2 segment processing
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIGenLocal(link,  static_cast<segdata *>( sdata ) );
    }
}

static void DBIGenLines(Link& link,  mod_entry *mod )
/***************************************/
// called during pass 2 linnum processing
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), bind_link( link, ODBIGenLines ) );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), bind_link( link, DwarfGenLines ) );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), bind_link( link, CVGenLines ) );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), HllGenLines);
    }
}

void DBIGenModule(Link& link)
/***********************/
// called at the end of pass2 for a module
{
    if( MOD_NOT_DEBUGGABLE( link.CurrMod ) )
        return;
    if( link.LinkFlags & LF_ANY_DBI_FLAG ) {
        Ring2Walk( link.CurrMod->segs, bind_link( link, DBIGenLocal ) );
        DBIGenLines(link,  link.CurrMod );
        if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
            ODBIGenModule(link);
        } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
            DwarfGenModule(link);
        } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
            HllGenModule();
        } else {
            CVGenModule(link);
        }
    }
}

void DBIDefClass(Link& link,  class_entry *class_, unsigned_32 size )
/******************************************************/
// called during address calculation
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIDefClass(link,  class_, size );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfDefClass(link,  class_, size );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVDefClass(link,  class_, size );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllDefClass(link,  class_, size );
    }
}

void DBIAddLocal(Link& link,  seg_leader *seg, offset length )
/************************************************/
// called during pass 1 final segment processing.
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddLocal(link,  seg, length );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddLocal( seg, length );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddLocal( seg, length );
    }
}

void DBIModGlobal(Link& link,  void *_sym )
/*****************************/
{
    symbol          *sym = static_cast<symbol *>( _sym );

    if( !IS_SYM_ALIAS( sym )
      && (sym->info & SYM_DEAD) == 0 ) {
        if( IS_SYM_IMPORTED( sym )
          || ( sym->p.seg != nullptr )
          && !IS_DBG_INFO( sym->p.seg->u.leader )
          && !sym->p.seg->isabs ) {
            DBIAddGlobal(link,  sym );
        }
    }
}

void DBIAddGlobal(Link& link,  symbol *sym )
/******************************/
// called during pass 1 symbol definition
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddGlobal(link,  sym );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfAddGlobal(link,  sym );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddGlobal(link,  sym );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddGlobal(link,  sym );
    }
}

void DBIGenGlobal(Link& link,  symbol *sym, section *sect )
/*********************************************/
// called during symbol address calculation (between pass 1 & pass 2)
// also called by loadpe between passes
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIGenGlobal(link,  sym, sect );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfGenGlobal(link,  sym, sect );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVGenGlobal(link,  sym, sect );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllGenGlobal(link,  sym, sect );
    }
#ifdef _NOVELL
    if( ( (sym->info & SYM_STATIC) == 0 )
      && (link.LinkFlags & LF_NOVELL_DBI_FLAG) ) {
        NovDBIGenGlobal(link,  sym );
    }
#endif
}

void DBIAddLines(Link& link,  segdata *seg, const void *line, size_t size, bool is32bit )
/***************************************************************************/
// called during pass 1 linnum processing
{
    lineinfo        *info;

    info = static_cast<lineinfo *>( _PermAlloc( sizeof( lineinfo ) + size - 1 ) );
    info->seg = seg;
    info->size = size;
    if( is32bit )
        info->size |= LINE_IS_32BIT;
    memcpy( info->data, line, size );
    RingAppend( &link.CurrMod->lines, info );
}

unsigned DBICalcLineQty( lineinfo *info )
/***************************************/
{
    unsigned        size;

    size = info->size & ~LINE_IS_32BIT;
    if( info->size & LINE_IS_32BIT ) {
        size /= sizeof( ln_off_386 );
    } else {
        size /= sizeof( ln_off_286 );
    }
    return( size );
}

static bool DoLineWalk( void *info, void *line_walk_cb )
/******************************************************/
{
    lineinfo        *li = static_cast<lineinfo *>( info );

    if( !li->seg->isdead ) {
        static_cast<line_walk_data *>( line_walk_cb )->cbfn( li );
    }
    return( false );
}

void DBILineWalk( lineinfo *lines, FunctionRef<line_walk_fn> cbfn )
/*****************************************************/
{
    line_walk_data  line_walk_cb;

    line_walk_cb.cbfn = cbfn;
    RingLookup( lines, DoLineWalk, &line_walk_cb );
}

virt_mem DBIAlloc( virt_mem_size size )
/*************************************/
// handy virtual memory allocation routine used inside the debug info generators
{
    if( size == 0 )
        return( 0 );
    return( AllocStg( size ) );
}

void DBIAddrStart(Link& link)
/***********************/
// called after address calculation is done.
{
#ifdef _NOVELL
    if( link.LinkFlags & LF_NOVELL_DBI_FLAG ) {
        NovDBIAddrStart();
    }
#endif
    if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddrStart(link);
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddrStart(link);
    }
    WalkAllSects(link,  bind_link( link, DBIAddrSectStart ) );
}

void DBIAddrSectStart(Link& link,  section *sect )
/************************************/
// called for each section after address calculation is done.
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddrSectStart(link,  sect );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfAddrSectStart(link,  sect );
    }
}

void DBIP2Start(Link& link,  section *sect )
/******************************/
// called for each section just before pass 2 starts
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIP2Start(link,  sect );
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        SectWalkClass(link,  sect, bind_link( link, DwarfGenAddrInfo ) );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        SectWalkClass(link,  sect, bind_link( link, CVGenAddrInfo ) );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        SectWalkClass(link,  sect, bind_link( link, HllGenAddrInfo ) );
    }
}

void DBIFini(Link& link,  section *sect )
/***************************/
// called after pass 2 is finished, but before load file generation
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIFini(link,  sect );
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVFini(link,  sect );
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllFini( sect );
    }
}

void DBISectCleanup(Link& link,  section *sect )
/**********************************/
// called when burning down the house
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBISectCleanup( sect );
    }
}

void DBICleanup(Link& link)
/*********************/
// called when burning down the house
{
    FreeGroups(link,  link.DBIGroups );
}

static void do_DBIWrite(Link& link)
/*****************************/
{
    if( link.LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIWrite(link);
    } else if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfWrite(link);
    } else if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        CVWrite(link);
    } else if( link.LinkFlags & LF_HLL_DBI_FLAG ) {
        HllWrite(link);
    }
}

void DBIWrite(Link& link)
/*******************/
// called during load file generation.  It is assumed that the loadfile is
// positioned to the right spot.
{
    outfilelist     symfile;
    outfilelist     *save;

    if( (link.LinkFlags & LF_ANY_DBI_FLAG) == 0 )
        return;
    if( link.LinkFlags & LF_CV_DBI_FLAG ) {
        // write DEBUG_TYPE_MISC: name of file containing the debug info
        if( link.SymFileName != nullptr ) {
            CVWriteDebugTypeMisc(link,  link.SymFileName );
        } else {
            CVWriteDebugTypeMisc(link,  link.Root->outfile->fname );
        }
    }
    if( link.SymFileName != nullptr ) {
        save = nullptr;
        InitBuffFile( &symfile, link.SymFileName, false );
        OpenBuffFile(link,  &symfile );
        save = link.Root->outfile;
        link.Root->outfile = &symfile;
        do_DBIWrite(link);
        CloseBuffFile( &symfile );
        link.Root->outfile = save;
    } else {
        do_DBIWrite(link);
    }
}
