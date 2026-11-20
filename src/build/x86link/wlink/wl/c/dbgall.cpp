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
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "specials.h"
#include "fileio.h"
#include "virtmem.h"
#include "exeelf.h"
#include "objcalc.h"
#include "loadfile.h"
#include "objio.h"
#include "dbgcomm.h"
#include "dbgall.h"
#include "dbgcv.h"
#include "dbghll.h"
#include "objfree.h"
#include "overlays.h"
#include "dbgdwarf.h"
#include "dbgwat.h"
#include "ring.h"
#include "loadnov.h"

#include "clibext.h"


typedef struct {
    line_walk_fn *cbfn;
} line_walk_data;


void ResetDBI( void )
/*******************/
{
    wlink().SymFileName = nullptr;
    wlink().DBIGroups = nullptr;
    // wlink.exe starts each link with this 0 (only a PE link sets it): the DEBUG_TYPE_MISC block of any other format
    // ends with it (special_purpose), and a PE link earlier in the same process had left its file offset here
    wlink().CVDebugDirEntryPos = 0;
}

void DBIInit( void )
/******************/
// called just after command file parsing
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIInit( wlink().Root );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVInit();
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfInit();
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllInit();
    }
}

void DBISectInit( section *sect )
/*******************************/
// called when a section created in command file parsing
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIInit( sect );
    }
}

void DBIInitModule( mod_entry *obj )
/**********************************/
// called before pass 1 is done on the module
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIInitModule( obj );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfInitModule( obj );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVInitModule( obj );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllInitModule( obj );
    }
}

void DBIP1Source( const byte *buff, const byte *endbuff )
/*******************************************************/
{
    byte            major;
    byte            minor;

    major = *buff++;
    minor = *buff++;
    if( endbuff <= buff ) {
        BadObject();
        return;
    }
    wlink().ObjFormat |= OBJ_FMT_DEBUG_COMENT;
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIP1Source( major, minor, reinterpret_cast<const char *>( buff ), endbuff - buff );
    }
}

section *DBIGetSect( const char *clname )
/***************************************/
{
    if( ( stricmp( clname, _MSTypeClass ) == 0 )
      || ( stricmp( clname, _MSLocalClass ) == 0 )
      || stricmp( clname, _HllLineClass ) == 0) {
        return( wlink().CurrSect );
    } else if( stricmp( clname, _DwarfClass ) == 0 ) {
        return( wlink().Root );
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

unsigned_16 DBIColSeg( class_entry *class_ )
/*****************************************/
{
    switch( class_->flags & CLASS_DEBUG_INFO ) {
    case CLASS_DWARF:
        if( wlink().CurrMod->flags_dbi & DBI_TYPE ) {
            wlink().CurrMod->flags_mod |= MOD_DBI_SEEN;
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

void DBIP1ModuleScanned( void )
/*****************************/
// called in pass 1 when finished looking at a module
// if some segdefs have been delayed due to distributing libraries, this
// will be called twice (once when regular pass 1 is done, once when all
// segdefs processed
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIP1ModuleScanned();
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfP1ModuleScanned();
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVP1ModuleScanned();
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllP1ModuleScanned();
    }
}

static bool MSSkip( void )
/************************/
{
    bool            iscv;
    bool            seencmt;

    if( wlink().ObjFileFormat != FILE_FMT_OMF ) {
        return( wlink().LinkFlags & LF_DWARF_DBI_FLAG );
    } else {
        iscv = ( (wlink().LinkFlags & (LF_CV_DBI_FLAG | LF_HLL_DBI_FLAG) ) != 0 );
        seencmt = ( (wlink().ObjFormat & OBJ_FMT_DEBUG_COMENT) != 0 );
        return( (iscv ^ seencmt) == 0 || (wlink().LinkFlags & LF_DWARF_DBI_FLAG) );
    }
}

bool DBISkip( seg_leader *seg )
/*****************************/
// returns true we should skip processing this segment because we are
// ignoring debugging information
{
    switch( seg->dbgtype ) {
    case MS_TYPE:
        return( (wlink().CurrMod->flags_dbi & DBI_TYPE) == 0 || MSSkip() );
    case MS_LOCAL:
        return( (wlink().CurrMod->flags_dbi & DBI_LOCAL) == 0 || MSSkip() );
    case HLL_LINE:
        return( (wlink().CurrMod->flags_dbi & DBI_LINE) == 0 || MSSkip() );
    case NOT_DEBUGGING_INFO:
        return( false );
    default:
        return( (wlink().CurrMod->flags_dbi & DBI_TYPE) == 0 || (wlink().LinkFlags & LF_DWARF_DBI_FLAG) == 0 );
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
#ifdef _NOVELL
    Ring2Walk( mod->publist, NovDBIAddGlobal );
#else
    /* unused parameters */ (void)mod;
#endif
}

void DBIPreAddrCalc( void )
/*************************/
{
    void (*modptr)( mod_entry * );
    void (*segptr)( seg_leader * );

    if( wlink().LinkFlags & LF_NOVELL_DBI_FLAG ) {
        WalkMods( AddNovGlobals );
    }
    if( (wlink().LinkFlags & LF_ANY_DBI_FLAG) == 0 )
        return;
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        modptr = ODBIP1ModuleFinished;
        segptr = ODBIAddAddrInfo;
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        modptr = DwarfP1ModuleFinished;
        segptr = DwarfAddAddrInfo;
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        modptr = HllP1ModuleFinished;
        segptr = HllAddAddrInfo;
    } else {
        modptr = CVP1ModuleFinished;
        segptr = CVAddAddrInfo;
    }
    WalkMods( modptr );
    WalkLeaders( segptr );
    if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        WalkMods( DwarfStoreAddrInfo );
    }
}

void DBIAddrInfoScan( seg_leader *seg,
                         void (*initfn)( segdata *, void * ),
                         void (*addfn)( segdata *, offset, offset, void *, bool ),
                         void *cookie )
/********************************************************************************/
{
    segdata         *prev;
    segdata         *curr;
    offset          size;
    bool            isnewmod;

    if( IS_DBG_INFO( seg ) )
        return;
    if( wlink().FmtData.dll
      || (wlink().FmtData.type & MK_PE) ) {
        if( seg->class_->flags & (CLASS_STACK | CLASS_IDATA) )
            return;
        if( seg->combine == COMBINE_STACK ) {
            if( (wlink().LinkState & LS_DOSSEG_FLAG) == 0 ) {
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

void DBIAddModule( mod_entry *obj, section *sect )
/************************************************/
// called just before publics have been assigned addresses between p1 & p2
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddModule( obj, sect );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfAddModule( obj, sect );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddModule( obj, sect );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddModule( obj, sect );
    }
}

static void DBIGenLocal( void *sdata )
/************************************/
// called during pass 2 segment processing
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIGenLocal( static_cast<segdata *>( sdata ) );
    }
}

static void DBIGenLines( mod_entry *mod )
/***************************************/
// called during pass 2 linnum processing
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), ODBIGenLines );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), DwarfGenLines );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), CVGenLines );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        DBILineWalk( static_cast<lineinfo *>( mod->lines ), HllGenLines);
    }
}

void DBIGenModule( void )
/***********************/
// called at the end of pass2 for a module
{
    if( MOD_NOT_DEBUGGABLE( wlink().CurrMod ) )
        return;
    if( wlink().LinkFlags & LF_ANY_DBI_FLAG ) {
        Ring2Walk( wlink().CurrMod->segs, DBIGenLocal );
        DBIGenLines( wlink().CurrMod );
        if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
            ODBIGenModule();
        } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
            DwarfGenModule();
        } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
            HllGenModule();
        } else {
            CVGenModule();
        }
    }
}

void DBIDefClass( class_entry *class_, unsigned_32 size )
/******************************************************/
// called during address calculation
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIDefClass( class_, size );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfDefClass( class_, size );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVDefClass( class_, size );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllDefClass( class_, size );
    }
}

void DBIAddLocal( seg_leader *seg, offset length )
/************************************************/
// called during pass 1 final segment processing.
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddLocal( seg, length );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddLocal( seg, length );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddLocal( seg, length );
    }
}

void DBIModGlobal( void *_sym )
/*****************************/
{
    symbol          *sym = static_cast<symbol *>( _sym );

    if( !IS_SYM_ALIAS( sym )
      && (sym->info & SYM_DEAD) == 0 ) {
        if( IS_SYM_IMPORTED( sym )
          || ( sym->p.seg != nullptr )
          && !IS_DBG_INFO( sym->p.seg->u.leader )
          && !sym->p.seg->isabs ) {
            DBIAddGlobal( sym );
        }
    }
}

void DBIAddGlobal( symbol *sym )
/******************************/
// called during pass 1 symbol definition
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddGlobal( sym );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfAddGlobal( sym );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddGlobal( sym );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddGlobal( sym );
    }
}

void DBIGenGlobal( symbol *sym, section *sect )
/*********************************************/
// called during symbol address calculation (between pass 1 & pass 2)
// also called by loadpe between passes
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIGenGlobal( sym, sect );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfGenGlobal( sym, sect );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVGenGlobal( sym, sect );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllGenGlobal( sym, sect );
    }
#ifdef _NOVELL
    if( ( (sym->info & SYM_STATIC) == 0 )
      && (wlink().LinkFlags & LF_NOVELL_DBI_FLAG) ) {
        NovDBIGenGlobal( sym );
    }
#endif
}

void DBIAddLines( segdata *seg, const void *line, size_t size, bool is32bit )
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
    RingAppend( &wlink().CurrMod->lines, info );
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

void DBILineWalk( lineinfo *lines, line_walk_fn *cbfn )
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

void DBIAddrStart( void )
/***********************/
// called after address calculation is done.
{
#ifdef _NOVELL
    if( wlink().LinkFlags & LF_NOVELL_DBI_FLAG ) {
        NovDBIAddrStart();
    }
#endif
    if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVAddrStart();
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllAddrStart();
    }
    WalkAllSects( DBIAddrSectStart );
}

void DBIAddrSectStart( section *sect )
/************************************/
// called for each section after address calculation is done.
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIAddrSectStart( sect );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfAddrSectStart( sect );
    }
}

void DBIP2Start( section *sect )
/******************************/
// called for each section just before pass 2 starts
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIP2Start( sect );
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        SectWalkClass( sect, DwarfGenAddrInfo );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        SectWalkClass( sect, CVGenAddrInfo );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        SectWalkClass( sect, HllGenAddrInfo );
    }
}

void DBIFini( section *sect )
/***************************/
// called after pass 2 is finished, but before load file generation
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIFini( sect );
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVFini( sect );
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllFini( sect );
    }
}

void DBISectCleanup( section *sect )
/**********************************/
// called when burning down the house
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBISectCleanup( sect );
    }
}

void DBICleanup( void )
/*********************/
// called when burning down the house
{
    FreeGroups( wlink().DBIGroups );
}

static void do_DBIWrite( void )
/*****************************/
{
    if( wlink().LinkFlags & LF_OLD_DBI_FLAG ) {
        ODBIWrite();
    } else if( wlink().LinkFlags & LF_DWARF_DBI_FLAG ) {
        DwarfWrite();
    } else if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        CVWrite();
    } else if( wlink().LinkFlags & LF_HLL_DBI_FLAG ) {
        HllWrite();
    }
}

void DBIWrite( void )
/*******************/
// called during load file generation.  It is assumed that the loadfile is
// positioned to the right spot.
{
    outfilelist     symfile;
    outfilelist     *save;

    if( (wlink().LinkFlags & LF_ANY_DBI_FLAG) == 0 )
        return;
    if( wlink().LinkFlags & LF_CV_DBI_FLAG ) {
        // write DEBUG_TYPE_MISC: name of file containing the debug info
        if( wlink().SymFileName != nullptr ) {
            CVWriteDebugTypeMisc( wlink().SymFileName );
        } else {
            CVWriteDebugTypeMisc( wlink().Root->outfile->fname );
        }
    }
    if( wlink().SymFileName != nullptr ) {
        save = nullptr;
        InitBuffFile( &symfile, wlink().SymFileName, false );
        OpenBuffFile( &symfile );
        save = wlink().Root->outfile;
        wlink().Root->outfile = &symfile;
        do_DBIWrite();
        CloseBuffFile( &symfile );
        wlink().Root->outfile = save;
    } else {
        do_DBIWrite();
    }
}
