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
* Description:  PROCFILES   : process object file.
*
****************************************************************************/

#include <string.h>
#include "linkstd.h"
#include "link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "library.h"
#include "ar.h"
#include "objnode.h"
#include "obj2supp.h"
#include "fileio.h"
#include "objpass1.h"
#include "overlays.h"
#include "objio.h"
#include "distrib.h"
#include "symtrace.h"
#include "objcache.h"
#include "specials.h"
#include "objorl.h"
#include "objomf.h"
#include "loadfile.h"
#include "dbgall.h"
#include "objfree.h"
#include "cmdline.h"
#include "impexp.h"
#include "strtab.h"
#include "carve.h"
#include "permdata.h"
#include "ring.h"
#include "procfile.h"
#include "hash.h"
#include "loadpe.h"


static void BadSkip( const file_list *file, unsigned long *loc )
/**************************************************************/
{
    /* unused parameters */ (void)file; (void)loc;

    BadObjFormat();
}

struct ProcessEntry {
    void (*SkipObj)( const file_list *, unsigned long * );
    unsigned long (*Pass1)( void );
};

static constexpr ProcessEntry Process[] = {
    #define pick(en,skip,pass1) { skip, pass1 },
    #include "objfmts.h"
    #undef pick
};

void SetupFakeModule( void )
/*********************************/
{
    if( wlink().FmtData.type & MK_PE ) {
        wlink().FakeModule = NewModEntry();
        wlink().FakeModule->flags_dbi = DBI_ALL;
        wlink().FakeModule->flags_mod = MOD_LAST_SEG | MOD_NEED_PASS_2;
        wlink().FakeModule->flags_fmt = FILE_FMT_PE_XFER;
        wlink().FakeModule->name.u.ptr = AddStringStringTable( &wlink().PermStrings, LinkerModule );
        DBIInitModule( wlink().FakeModule );
    }
}

void LinkFakeModule( void )
/********************************/
{
    if( wlink().FmtData.type & MK_PE ) {
        // Unlike other modules, fake module goes to the beginning of the list
        wlink().FakeModule->u.next = wlink().LibModules;
        wlink().LibModules = wlink().FakeModule;
    }
}

static void CheckNewFile( mod_entry *mod, file_list *file,
                          int AlwaysCheckUsingDate)
/*********************************************************/
{
    time_t      modtime;

    if( (wlink().LinkFlags & LF_GOT_CHGD_FILES) == 0
      || AlwaysCheckUsingDate ) {
        if( QModTime( file->infile->name.u.ptr, &modtime )
          || modtime > mod->modtime ) {
            file->flags_file |= STAT_HAS_CHANGED;
        }
    } else {
        if( FindHTableElem( wlink().Root->modFilesHashed, file->infile->name.u.ptr ) ) {
            file->flags_file |= STAT_HAS_CHANGED;
        }
    }
}

static void SetStartAddr( void )
/******************************/
{
    mod_entry   *mod;

    if( wlink().StartInfo.user_specd
      || !wlink().StartInfo.from_inc )
        return;
    mod = wlink().StartInfo.mod;
    if( mod == nullptr )
        return;
    if( (mod->flags_mod & MOD_KILL)
      || (mod->u1.source->flags_file & STAT_HAS_CHANGED) ) {
        ClearStartAddr();
    }
}

static void SetupModule( mod_entry **mod, file_list *file )
/*********************************************************/
{
    char        *fname;
    mod_entry   *currmod;

    currmod = *mod;

    CheckNewFile( currmod, file, 0 );
    fname = currmod->u1.fname.u.ptr;
    for( ;; ) {
        currmod->u1.source = file;
        currmod = currmod->u.next;
        if( currmod == nullptr )
            break;
        if( currmod->u1.fname.u.ptr != fname ) {
            break;
        }
    }
    *mod = currmod;
}

static void DoIncLibDefs( void )
/******************************/
{
    libnamelist         *lib;

    for( lib = wlink().SavedDefLibs; lib != nullptr; lib = lib->next ) {
        AddObjLib( lib->name, lib->priority );
    }
}

static libnamelist *CalcLibBlacklist( void )
/*******************************************/
/* figure out if the user has changed any of the specified libraries, and
 * torch anything after a changed library */
{
    infilelist          *userlibs_infile;
    libnamelist         *oldlibs;

    userlibs_infile = wlink().CachedLibFiles;
    for( oldlibs = wlink().SavedUserLibs; oldlibs != nullptr; oldlibs = oldlibs->next ) {
        if( userlibs_infile == nullptr )
            return( oldlibs );
        if( FNAMECMPSTR( userlibs_infile->name.u.ptr, oldlibs->name ) != 0 )
            return( oldlibs );
        userlibs_infile = userlibs_infile->next;
    }
    return( nullptr );
}

static void CheckBlacklist( file_list *file, libnamelist *blacklist )
/*******************************************************************/
{
    size_t      length;
    size_t      length_b;
    size_t      delta;

    if( file->flags_file & STAT_HAS_CHANGED )
        return;
    length = strlen( file->infile->name.u.ptr );
    for( ; blacklist != nullptr; blacklist = blacklist->next ) {
        length_b = strlen( blacklist->name );
        if( length >= length_b ) {
            delta = length - length_b;
            if( FNAMECMPSTR( blacklist->name, file->infile->name.u.ptr + delta ) == 0 ) {
                file->flags_file |= STAT_HAS_CHANGED;
                return;
            }
        }
    }
}

static void PrepareModList( void )
/********************************/
{
    file_list   *file;
    mod_entry   *mod;
    mod_entry   *curr;
    libnamelist *blacklist;

    mod = wlink().Root->mods;
    for( file = wlink().Root->files; file != nullptr && mod != nullptr; file = file->next ) {
        if( strcmp( file->infile->name.u.ptr, mod->u1.fname.u.ptr ) == 0 ) {
            SetupModule( &mod, file );
        } else if( mod->u.next != nullptr ) {
            if( FNAMECMPSTR( file->infile->name.u.ptr, mod->u.next->u1.fname.u.ptr ) == 0 ) {
                mod->flags_mod |= MOD_KILL;
                mod = mod->u.next;
                SetupModule( &mod, file );
            }
        }
    }
    for( ; mod != nullptr; mod = mod->u.next ) {
        mod->flags_mod |= MOD_KILL;               // no match found
    }
    blacklist = CalcLibBlacklist();
    for( mod = wlink().LibModules; mod != nullptr; mod = mod->u.next ) {
        if( mod->u1.fname.u.ptr == nullptr ) {
            mod->flags_mod |= MOD_KILL;
        } else if( (mod->flags_mod & MOD_VISITED) == 0 ) {
            file = AddObjLib( mod->u1.fname.u.ptr, LIB_PRIORITY_MID );
            CheckNewFile( mod, file, 1);
            CheckBlacklist( file, blacklist );
            for( curr = mod->u.next; curr != nullptr; curr = curr->u.next ){
                if( curr->u1.fname.u.ptr == mod->u1.fname.u.ptr ) {
                    curr->u1.source = file;
                    curr->flags_mod |= MOD_VISITED;
                }
            }
            mod->u1.source = file;
        }
        mod->flags_mod &= ~MOD_VISITED;
    }
    FreeList( wlink().SavedUserLibs );
    wlink().SavedUserLibs = nullptr;
}

static void MarkDefaultSyms( void )
/*********************************/
{
    symbol *sym;

    for( sym = wlink().HeadSym; sym != nullptr; sym = sym->next ) {
        if( IS_SYM_ALIAS( sym )
          && (sym->info & SYM_WAS_LAZY) ) {
            sym->e.def->info |= SYM_RELOC_REFD;
        }
    }
}

static void MarkRelocs( mod_entry *mod )
/**************************************/
{
    IterateModRelocs( mod->relocs, mod->sizerelocs, RelocMarkSyms );
}

static void KillASym( void *_sym )
/********************************/
{
    symbol *sym = static_cast<symbol *>( _sym );

    sym->info |= SYM_KILL;
    if( IS_SYM_IMPORTED( sym ) ) {
        KillDependantSyms( sym );
    }
}

static void KillSyms( mod_entry *mod )
/************************************/
{
    Ring2Walk( mod->publist, KillASym );
}

static void SetAltDefData( void *_sym )
/*************************************/
{
    symbol *sym = static_cast<symbol *>( _sym );

    if( (sym->info & SYM_IS_ALTDEF)
      && IS_SYM_COMDAT( sym )
      && (sym->info & SYM_HAS_DATA) == 0 ) {
        sym->p.seg->u1.vm_ptr = sym->e.mainsym->p.seg->u1.vm_ptr;
    }
}

static void FixModAltDefs( mod_entry *mod )
/*****************************************/
{
    Ring2Walk( mod->publist, SetAltDefData );
}

static void IncIterateMods( mod_entry *mod, void (*proc_fn)(mod_entry *), bool dochanged )
/****************************************************************************************/
{
    bool haschanged;

    for( ; mod != nullptr; mod = mod->u.next ) {
        haschanged = (mod->flags_mod & MOD_KILL) || (mod->u1.source->flags_file & STAT_HAS_CHANGED);
        if( haschanged == dochanged ) {
            proc_fn( mod );
        }
    }
}

static void AddToModList( mod_entry *mod )
/****************************************/
{
    if( wlink().CurrMod == nullptr ) {
        wlink().CurrSect->mods = mod;
    } else {
        wlink().CurrMod->u.next = mod;
    }
    mod->u.next = nullptr;
    wlink().CurrMod = mod;
}

static void SavedPass1( mod_entry *mod )
/**************************************/
{
    wlink().ObjFileFormat = FILE_FMT_INCREMENTAL;
    mod->flags_fmt = FILE_FMT_INCREMENTAL;
    AddToModList( mod );
    ObjPass1();
}

static void FreeModSegments( mod_entry *mod )
/*******************************************/
{
    mod->publist = nullptr;
    Ring2CarveFree( wlink().CarveSegData, &mod->segs );
}

static member_list *FindMember( file_list *file, char *name )
/***********************************************************/
// see if name is in the member list of list
{
    member_list         **member;
    member_list         *found;

    found = nullptr;
    for( member = &file->u.member; *member != nullptr; member = &(*member)->next ) {
        if( ModNameCompare( name, (*member)->name ) ) {
            found = *member;
            *member = (*member)->next;      // remove it from the list.
            break;
        }
    }
    return( found );
}

static bool EndOfLib( file_list *file, unsigned long loc )
/********************************************************/
{
    unsigned_8 *id;

    if( file->flags_file & STAT_OMF_LIB ) {
        id = static_cast<unsigned_8 *>( CacheRead( file, loc, sizeof( unsigned_8 ) ) );
        return( *id == LIB_TRAILER_REC );
    } else {
        return( false );
    }
}

static void DoPass1( mod_entry *next, file_list *file )
/*****************************************************/
/* do pass 1 on the object file */
{
    member_list         *member;
    char                *membname;
    unsigned long       loc;
    unsigned long       size;
    unsigned_16         reclength;
    bool                lastmod;
    bool                ignoreobj;

    loc = 0;
    lastmod = false;
    if( CacheOpen( file ) ) {
        reclength = static_cast<unsigned_16>( CheckLibraryType( file, &loc, false ) );
        for( ;; ) { /*  there may be more than 1 object module in a file */
            member = nullptr;
            ignoreobj = false;
            if( EndOfLib( file, loc ) )
                break;
            membname = IdentifyObject( file, &loc, &size );
            if( file->flags_file & STAT_IS_LIB ) {
                if( (file->flags_file & STAT_HAS_MEMBER)
                  && file->u.member != nullptr ) {
                    member = FindMember( file, membname );
                    if( member == nullptr ) {
                        ignoreobj = true;
                    } else if( file->u.member == nullptr ) {
                        lastmod = true;
                    }
                }
            }
            if( ignoreobj ) {
                MemFree( membname );
                if( size != 0 ) {
                    loc += size;
                } else {
                    Process[wlink().ObjFileFormat].SkipObj( file, &loc );
                }
            } else {
                if( next == nullptr ) {
                    next = NewModEntry();
                }
                next->u.next = nullptr;
                next->u1.source = file;
                next->modtime = next->u1.source->infile->modtime;
                next->flags_fmt = wlink().ObjFileFormat;
                if( member != nullptr ) {
                    next->flags_dbi = member->flags_dbi;
                    next->flags_mod = member->flags_mod;
                    MemFree( member );
                }
                if( (file->flags_file & STAT_HAS_MEMBER) == 0 ) {
                    next->flags_dbi = file->flags_dbi;
                    if( file->flags_file & STAT_LAST_SEG ) {
                        next->flags_mod |= MOD_LAST_SEG;
                    }
                }
                AddToModList( next );
                next->location = loc;
                if( membname != nullptr ) {
                    next->name.u.ptr = AddStringStringTable( &wlink().PermStrings, membname );
                    MemFree( membname );
                } else {
                    next->name.u.ptr = file->infile->name.u.ptr;
                }
                loc = ObjPass1();
                if( file->flags_file & STAT_TRACE_SYMS ) {
                    TraceSymList( wlink().CurrMod->publist );
                }
                next = nullptr;
            }
            wlink().ObjFileFormat = static_cast<file_format>( 0 );
            wlink().ObjFormat = static_cast<obj_format>( 0 );
            if( file->flags_file & STAT_IS_LIB ) {      // skip library padding.
                unsigned_16 modulus;

                modulus = static_cast<unsigned_16>( loc % reclength );
                if( modulus != 0 ) {
                    loc += reclength - modulus;     // go to library boundary.
                }
            } else if( wlink().CurrMod->flags_fmt != FILE_FMT_OMF ) {
                break;          // can only concat omf.
            }
            if( lastmod
              || CacheIsEnd( file, loc ) ) {
                break;
            }
        }
        if( file->u.member != nullptr ) {
            LnkMsg( ERR+MSG_CANT_FIND_MEMBER, "12", file->infile->name.u.ptr, file->u.member->name );
        }
        CacheClose( file, 1 );
    }
    CheckStop();
}

static void ProcessMods( void )
/*****************************/
{
    mod_entry   *mod;
    mod_entry   *next;
    mod_entry   *savemod;
    file_list   *file;

    mod = wlink().Root->mods;
    wlink().Root->mods = nullptr;
    for( file = wlink().Root->files; file != nullptr && mod != nullptr; file = file->next ) {
        for( ; mod != nullptr; mod = next ) {
            next = mod->u.next;
            if( mod->flags_mod & MOD_KILL ) {
                FreeModEntry( mod );
            } else if( mod->u1.source != file ) {
                DoPass1( nullptr, file );
                break;
            } else {
                if( file->flags_file & STAT_HAS_CHANGED ) {
                    memset( mod, 0, sizeof( mod_entry ) );
                    DoPass1( mod, file );
                } else {
                    SavedPass1( mod );
                }
            }
        }
    }
    for( ; mod != nullptr; mod = next ) {
        next = mod->u.next;
        FreeModEntry( mod );
    }
    for( ; file != nullptr; file = file->next ) {
        DoPass1( nullptr, file );
    }
    savemod = wlink().Root->mods;       // pass1 routines will add new mods to this
    wlink().Root->mods = nullptr;
    wlink().CurrMod = nullptr;
    for( mod = wlink().LibModules; mod != nullptr; mod = next ) {
        next = mod->u.next;
        if( (mod->flags_mod & MOD_KILL)
          || mod->u1.source != nullptr
          && (mod->u1.source->flags_file & STAT_HAS_CHANGED) ) {
            FreeModEntry( mod );
        } else {
            SavedPass1( mod );
        }
    }
    wlink().LibModules = wlink().Root->mods;
    wlink().Root->mods = savemod;
}

static void IncLoadObjFiles( void )
/*********************************/
{
    PrepareModList();
    SetStartAddr();
    MarkDefaultSyms();
    IncIterateMods( wlink().Root->mods, MarkRelocs, false );
    IncIterateMods( wlink().LibModules, MarkRelocs, false );
    IncIterateMods( wlink().Root->mods, FixModAltDefs, false );
    IncIterateMods( wlink().LibModules, FixModAltDefs, false );
    IncIterateMods( wlink().Root->mods, KillSyms, true );
    IncIterateMods( wlink().LibModules, KillSyms, true );
    PurgeSymbols();
    IncIterateMods( wlink().Root->mods, FreeModSegments, true );
    IncIterateMods( wlink().LibModules, FreeModSegments, true );
    ProcessMods();
    DoIncGroupDefs();
    DoIncLibDefs();
}

void ProcObjFiles( void )
/******************************/
/* Perform Pass 1 on all object files */
{
    wlink().CurrMod = nullptr;
    if( wlink().LinkFlags & LF_INC_LINK_FLAG ) {
        if( (wlink().LinkFlags & LF_DWARF_DBI_FLAG) == 0
          && (wlink().LinkFlags & LF_ANY_DBI_FLAG) ) {
            LnkMsg( FTL+MSG_INC_ONLY_SUPPORTS_DWARF, nullptr );
        }
        if( wlink().LinkFlags & LF_STRIP_CODE ) {
            LnkMsg( FTL+MSG_INC_AND_DCE_NOT_ALLOWED, nullptr );
        }
        if( wlink().LinkFlags & LF_VF_REMOVAL ) {
            LnkMsg( FTL+MSG_INC_AND_VFR_NOT_ALLOWED, nullptr );
        }
    }
    LnkMsg( INF+MSG_LOADING_OBJECT, nullptr );
    if( wlink().LinkFlags & LF_STRIP_CODE ) {
        wlink().LinkState |= LS_CAN_REMOVE_SEGMENTS;
    }
    if( wlink().LinkState & LS_GOT_PREV_STRUCTS ) {
        IncLoadObjFiles();
    } else {
        LoadObjFiles( wlink().Root );
#ifdef _EXE
        if( wlink().FmtData.type & MK_OVERLAYS ) {
            OvlPass1();
        }
#endif
    }
}

void LoadObjFiles( section *sect )
/********************************/
{
    file_list   *file;

    wlink().CurrSect = sect;
    wlink().CurrMod = nullptr;
    for( file = sect->files; file != nullptr; file = file->next ) {
        DoPass1( nullptr, file );
    }
}

char *IdentifyObject( const file_list *file, unsigned long *loc, unsigned long *size )
/************************************************************************************/
{
    ar_header       *ar_hdr;
    char            *name;
    unsigned long   ar_loc;

    name = nullptr;
    *size = 0;
    ar_loc = 0;
    if( file->flags_file & STAT_AR_LIB ) {
        ar_loc = __ROUND_UP_SIZE_EVEN( *loc );     /* AR headers are word aligned. */
        ar_hdr = static_cast<ar_header *>( CacheRead( file, ar_loc, sizeof( ar_header ) ) );
        ar_loc += sizeof( ar_header );
        name = GetARName( ar_hdr, file, &ar_loc );
        *size = GetARValue( ar_hdr->size, AR_SIZE_LEN );
        *loc = ar_loc;
    }
    switch( FileTypeORL( file, *loc ) ) {
    case ORL_ELF:
        wlink().ObjFileFormat = FILE_FMT_ELF;
        break;
    case ORL_COFF:
        wlink().ObjFileFormat = FILE_FMT_COFF;
        break;
    case ORL_OMF:
        wlink().ObjFileFormat = FILE_FMT_OMF;
        if( (file->flags_file & STAT_IS_LIB) == STAT_OMF_LIB ) {
            name = GetOMFName( file, loc );
        }
        break;
    default:
        break;
    }
    return( name );
}

unsigned long ObjPass1( void )
/****************************/
/* Pass 1 of 8086 linker. */
{
    unsigned long loc;

    DEBUG(( DBG_BASE, "1 : file = %s, module = %s", wlink().CurrMod->u1.source->infile->name.u.ptr, wlink().CurrMod->name.u.ptr ));
    wlink().CurrMod->flags_mod |= MOD_DONE_PASS_1;
    SymModStart();
    DBIInitModule( wlink().CurrMod );
    RelocStartMod();
    P1Start();
    loc = Process[wlink().ObjFileFormat].Pass1();
    CollapseLazyExtdefs();
    SymModEnd();
    DBIP1ModuleScanned();
    ReleaseNames();
    PermEndMod( wlink().CurrMod );
    FreeObjInfo();
    wlink().ObjFileFormat = static_cast<file_format>( 0 );
    wlink().ObjFormat = static_cast<obj_format>( 0 );       //clear flags for processing obj file
    return( loc );
}

static bool ResolveVFExtdefs( void )
/**********************************/
/* go through the symbol table, and check if any conditional extdefs
 * should turned into real extdefs */
{
    bool        resolved;
    symbol      *sym;

    resolved = false;
    for( sym = wlink().HeadSym; sym != nullptr; sym = sym->next ) {
        if( IS_SYM_VF_REF( sym ) ) {
            resolved |= CheckVFList( sym );
        }
    }
    return( resolved );
}

void ResolveUndefined( void )
/**********************************/
{
    symbol      *sym;
    file_list   *file;
    bool        keepgoing;

    LnkMsg( INF+MSG_SEARCHING_LIBS, nullptr );
#ifdef _EXE
    if( (wlink().FmtData.type & MK_OVERLAYS)
      && wlink().FmtData.u.dos.distribute ) {
        wlink().LinkState |= LS_CAN_REMOVE_SEGMENTS;
        DistribInitMods();
    }
#endif
    wlink().CurrSect = wlink().Root;
    ResolveVFExtdefs();
    do {
        wlink().LinkState &= ~LS_LIBRARIES_ADDED;
        for( file = wlink().ObjLibFiles; file != nullptr; file = file->next ) {
            if( file->flags_file & STAT_SEEN_LIB ) {
                file->flags_file |= STAT_OLD_LIB;
            } else {
                file->flags_file |= STAT_SEEN_LIB;
            }
        }
        for( sym = wlink().HeadSym; sym != nullptr; sym = sym->next ) {
            if( ( (sym->info & SYM_DEFINED) == 0
              && !IS_SYM_WEAK_REF( sym )
              || (wlink().FmtData.type & MK_NOVELL)
              && IS_SYM_IMPORTED( sym )
              && (sym->info & (SYM_REFERENCED | SYM_LOCAL_REF)) )
              && (sym->info & SYM_IS_ALTDEF) == 0 ) {
                LibFind( sym->name.u.ptr, (sym->info & SYM_CHECKED) != 0 );
            }
            sym->info |= SYM_CHECKED;
        }
        keepgoing = ResolveVFExtdefs();
    } while( keepgoing || (wlink().LinkState & LS_LIBRARIES_ADDED) );

    BurnLibs();
    PrintBadTraces();
}

void ProcLocalImports( void )
/***************************/
{
#ifdef _OS2
    symbol  *sym;

    if( wlink().FmtData.type & MK_PE ) {
        for( sym = wlink().HeadSym; sym != nullptr; sym = sym->next ) {
            if( (sym->info & SYM_DEFINED) == 0
              && !IS_SYM_WEAK_REF( sym )
              && (sym->info & SYM_IS_ALTDEF) == 0 ) {
                ImportPELocalSym( sym );
            }
        }
    }
#endif
}

void FreeLocalImports( void )
/***************************/
{
#ifdef _OS2
    FreePELocalImports();
#endif
}
