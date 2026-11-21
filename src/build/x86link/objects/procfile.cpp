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
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/symbols/library.h"
#include "ar.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/layout/distrib.h"
#include "build/x86link/symbols/symtrace.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/objects/objorl.h"
#include "build/x86link/objects/objomf.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/objects/procfile.h"
#include "build/x86link/base/hash.h"
#include "build/x86link/formats/pe.h"


static void BadSkip( Link& link, const file_list *file, unsigned long *loc )
/**************************************************************/
{
    /* unused parameters */ (void)file; (void)loc;

    BadObjFormat( link );
}

struct ProcessEntry {
    void (*SkipObj)( Link& link, const file_list *, unsigned long * );
    unsigned long (*Pass1)( Link& link );
};

static constexpr ProcessEntry Process[] = {
    #define pick(en,skip,pass1) { skip, pass1 },
    #include "build/x86link/objects/objfmts.h"
    #undef pick
};

void SetupFakeModule(Link& link)
/*********************************/
{
    if( link.FmtData.type & MK_PE ) {
        link.FakeModule = NewModEntry(link);
        link.FakeModule->flags_dbi = DBI_ALL;
        link.FakeModule->flags_mod = MOD_LAST_SEG | MOD_NEED_PASS_2;
        link.FakeModule->flags_fmt = FILE_FMT_PE_XFER;
        link.FakeModule->name.u.ptr = AddStringStringTable( &link.PermStrings, LinkerModule );
        DBIInitModule(link,  link.FakeModule );
    }
}

void LinkFakeModule(Link& link)
/********************************/
{
    if( link.FmtData.type & MK_PE ) {
        // Unlike other modules, fake module goes to the beginning of the list
        link.FakeModule->u.next = link.LibModules;
        link.LibModules = link.FakeModule;
    }
}

static void CheckNewFile(Link& link,  mod_entry *mod, file_list *file,
                          int AlwaysCheckUsingDate)
/*********************************************************/
{
    time_t      modtime;

    if( (link.LinkFlags & LF_GOT_CHGD_FILES) == 0
      || AlwaysCheckUsingDate ) {
        if( QModTime( file->infile->name.u.ptr, &modtime )
          || modtime > mod->modtime ) {
            file->flags_file |= STAT_HAS_CHANGED;
        }
    } else {
        if( FindHTableElem( link.Root->modFilesHashed, file->infile->name.u.ptr ) ) {
            file->flags_file |= STAT_HAS_CHANGED;
        }
    }
}

static void SetStartAddr(Link& link)
/******************************/
{
    mod_entry   *mod;

    if( link.StartInfo.user_specd
      || !link.StartInfo.from_inc )
        return;
    mod = link.StartInfo.mod;
    if( mod == nullptr )
        return;
    if( (mod->flags_mod & MOD_KILL)
      || (mod->u1.source->flags_file & STAT_HAS_CHANGED) ) {
        ClearStartAddr(link);
    }
}

static void SetupModule(Link& link,  mod_entry **mod, file_list *file )
/*********************************************************/
{
    char        *fname;
    mod_entry   *currmod;

    currmod = *mod;

    CheckNewFile(link,  currmod, file, 0 );
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

static void DoIncLibDefs(Link& link)
/******************************/
{
    libnamelist         *lib;

    for( lib = link.SavedDefLibs; lib != nullptr; lib = lib->next ) {
        AddObjLib(link,  lib->name, lib->priority );
    }
}

static libnamelist *CalcLibBlacklist(Link& link)
/*******************************************/
/* figure out if the user has changed any of the specified libraries, and
 * torch anything after a changed library */
{
    infilelist          *userlibs_infile;
    libnamelist         *oldlibs;

    userlibs_infile = link.CachedLibFiles;
    for( oldlibs = link.SavedUserLibs; oldlibs != nullptr; oldlibs = oldlibs->next ) {
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

static void PrepareModList(Link& link)
/********************************/
{
    file_list   *file;
    mod_entry   *mod;
    mod_entry   *curr;
    libnamelist *blacklist;

    mod = link.Root->mods;
    for( file = link.Root->files; file != nullptr && mod != nullptr; file = file->next ) {
        if( strcmp( file->infile->name.u.ptr, mod->u1.fname.u.ptr ) == 0 ) {
            SetupModule(link,  &mod, file );
        } else if( mod->u.next != nullptr ) {
            if( FNAMECMPSTR( file->infile->name.u.ptr, mod->u.next->u1.fname.u.ptr ) == 0 ) {
                mod->flags_mod |= MOD_KILL;
                mod = mod->u.next;
                SetupModule(link,  &mod, file );
            }
        }
    }
    for( ; mod != nullptr; mod = mod->u.next ) {
        mod->flags_mod |= MOD_KILL;               // no match found
    }
    blacklist = CalcLibBlacklist(link);
    for( mod = link.LibModules; mod != nullptr; mod = mod->u.next ) {
        if( mod->u1.fname.u.ptr == nullptr ) {
            mod->flags_mod |= MOD_KILL;
        } else if( (mod->flags_mod & MOD_VISITED) == 0 ) {
            file = AddObjLib(link,  mod->u1.fname.u.ptr, LIB_PRIORITY_MID );
            CheckNewFile(link,  mod, file, 1);
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
    FreeList( link.SavedUserLibs );
    link.SavedUserLibs = nullptr;
}

static void MarkDefaultSyms(Link& link)
/*********************************/
{
    symbol *sym;

    for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
        if( IS_SYM_ALIAS( sym )
          && (sym->info & SYM_WAS_LAZY) ) {
            sym->e.def->info |= SYM_RELOC_REFD;
        }
    }
}

static void MarkRelocs(Link& link,  mod_entry *mod )
/**************************************/
{
    IterateModRelocs( mod->relocs, mod->sizerelocs, bind_link( link, RelocMarkSyms ) );
}

static void KillASym(Link& link,  void *_sym )
/********************************/
{
    symbol *sym = static_cast<symbol *>( _sym );

    sym->info |= SYM_KILL;
    if( IS_SYM_IMPORTED( sym ) ) {
        KillDependantSyms(link,  sym );
    }
}

static void KillSyms(Link& link,  mod_entry *mod )
/************************************/
{
    Ring2Walk( mod->publist, bind_link( link, KillASym ) );
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

static void IncIterateMods( mod_entry *mod, FunctionRef<void(mod_entry *)> proc_fn, bool dochanged )
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

static void AddToModList(Link& link,  mod_entry *mod )
/****************************************/
{
    if( link.CurrMod == nullptr ) {
        link.CurrSect->mods = mod;
    } else {
        link.CurrMod->u.next = mod;
    }
    mod->u.next = nullptr;
    link.CurrMod = mod;
}

static void SavedPass1(Link& link,  mod_entry *mod )
/**************************************/
{
    link.ObjFileFormat = FILE_FMT_INCREMENTAL;
    mod->flags_fmt = FILE_FMT_INCREMENTAL;
    AddToModList(link,  mod );
    ObjPass1(link);
}

static void FreeModSegments(Link& link,  mod_entry *mod )
/*******************************************/
{
    mod->publist = nullptr;
    Ring2CarveFree( link.CarveSegData, &mod->segs );
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

static bool EndOfLib(Link& link,  file_list *file, unsigned long loc )
/********************************************************/
{
    unsigned_8 *id;

    if( file->flags_file & STAT_OMF_LIB ) {
        id = static_cast<unsigned_8 *>( CacheRead(link,  file, loc, sizeof( unsigned_8 ) ) );
        return( *id == LIB_TRAILER_REC );
    } else {
        return( false );
    }
}

static void DoPass1(Link& link,  mod_entry *next, file_list *file )
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
    if( CacheOpen(link,  file ) ) {
        reclength = static_cast<unsigned_16>( CheckLibraryType(link,  file, &loc, false ) );
        for( ;; ) { /*  there may be more than 1 object module in a file */
            member = nullptr;
            ignoreobj = false;
            if( EndOfLib(link,  file, loc ) )
                break;
            membname = IdentifyObject(link,  file, &loc, &size );
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
                    Process[link.ObjFileFormat].SkipObj( link, file, &loc );
                }
            } else {
                if( next == nullptr ) {
                    next = NewModEntry(link);
                }
                next->u.next = nullptr;
                next->u1.source = file;
                next->modtime = next->u1.source->infile->modtime;
                next->flags_fmt = link.ObjFileFormat;
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
                AddToModList(link,  next );
                next->location = loc;
                if( membname != nullptr ) {
                    next->name.u.ptr = AddStringStringTable( &link.PermStrings, membname );
                    MemFree( membname );
                } else {
                    next->name.u.ptr = file->infile->name.u.ptr;
                }
                loc = ObjPass1(link);
                if( file->flags_file & STAT_TRACE_SYMS ) {
                    TraceSymList( link.CurrMod->publist );
                }
                next = nullptr;
            }
            link.ObjFileFormat = static_cast<file_format>( 0 );
            link.ObjFormat = static_cast<obj_format>( 0 );
            if( file->flags_file & STAT_IS_LIB ) {      // skip library padding.
                unsigned_16 modulus;

                modulus = static_cast<unsigned_16>( loc % reclength );
                if( modulus != 0 ) {
                    loc += reclength - modulus;     // go to library boundary.
                }
            } else if( link.CurrMod->flags_fmt != FILE_FMT_OMF ) {
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
    CheckStop(link);
}

static void ProcessMods(Link& link)
/*****************************/
{
    mod_entry   *mod;
    mod_entry   *next;
    mod_entry   *savemod;
    file_list   *file;

    mod = link.Root->mods;
    link.Root->mods = nullptr;
    for( file = link.Root->files; file != nullptr && mod != nullptr; file = file->next ) {
        for( ; mod != nullptr; mod = next ) {
            next = mod->u.next;
            if( mod->flags_mod & MOD_KILL ) {
                FreeModEntry(link,  mod );
            } else if( mod->u1.source != file ) {
                DoPass1(link,  nullptr, file );
                break;
            } else {
                if( file->flags_file & STAT_HAS_CHANGED ) {
                    memset( mod, 0, sizeof( mod_entry ) );
                    DoPass1(link,  mod, file );
                } else {
                    SavedPass1(link,  mod );
                }
            }
        }
    }
    for( ; mod != nullptr; mod = next ) {
        next = mod->u.next;
        FreeModEntry(link,  mod );
    }
    for( ; file != nullptr; file = file->next ) {
        DoPass1(link,  nullptr, file );
    }
    savemod = link.Root->mods;       // pass1 routines will add new mods to this
    link.Root->mods = nullptr;
    link.CurrMod = nullptr;
    for( mod = link.LibModules; mod != nullptr; mod = next ) {
        next = mod->u.next;
        if( (mod->flags_mod & MOD_KILL)
          || mod->u1.source != nullptr
          && (mod->u1.source->flags_file & STAT_HAS_CHANGED) ) {
            FreeModEntry(link,  mod );
        } else {
            SavedPass1(link,  mod );
        }
    }
    link.LibModules = link.Root->mods;
    link.Root->mods = savemod;
}

static void IncLoadObjFiles(Link& link)
/*********************************/
{
    PrepareModList(link);
    SetStartAddr(link);
    MarkDefaultSyms(link);
    IncIterateMods( link.Root->mods, bind_link( link, MarkRelocs ), false );
    IncIterateMods( link.LibModules, bind_link( link, MarkRelocs ), false );
    IncIterateMods( link.Root->mods, FixModAltDefs, false );
    IncIterateMods( link.LibModules, FixModAltDefs, false );
    IncIterateMods( link.Root->mods, bind_link( link, KillSyms ), true );
    IncIterateMods( link.LibModules, bind_link( link, KillSyms ), true );
    PurgeSymbols(link);
    IncIterateMods( link.Root->mods, bind_link( link, FreeModSegments ), true );
    IncIterateMods( link.LibModules, bind_link( link, FreeModSegments ), true );
    ProcessMods(link);
    DoIncGroupDefs(link);
    DoIncLibDefs(link);
}

void ProcObjFiles(Link& link)
/******************************/
/* Perform Pass 1 on all object files */
{
    link.CurrMod = nullptr;
    if( link.LinkFlags & LF_INC_LINK_FLAG ) {
        if( (link.LinkFlags & LF_DWARF_DBI_FLAG) == 0
          && (link.LinkFlags & LF_ANY_DBI_FLAG) ) {
            LnkMsg( FTL+MSG_INC_ONLY_SUPPORTS_DWARF, nullptr );
        }
        if( link.LinkFlags & LF_STRIP_CODE ) {
            LnkMsg( FTL+MSG_INC_AND_DCE_NOT_ALLOWED, nullptr );
        }
        if( link.LinkFlags & LF_VF_REMOVAL ) {
            LnkMsg( FTL+MSG_INC_AND_VFR_NOT_ALLOWED, nullptr );
        }
    }
    LnkMsg( INF+MSG_LOADING_OBJECT, nullptr );
    if( link.LinkFlags & LF_STRIP_CODE ) {
        link.LinkState |= LS_CAN_REMOVE_SEGMENTS;
    }
    if( link.LinkState & LS_GOT_PREV_STRUCTS ) {
        IncLoadObjFiles(link);
    } else {
        LoadObjFiles(link,  link.Root );
#ifdef _EXE
        if( link.FmtData.type & MK_OVERLAYS ) {
            OvlPass1(link);
        }
#endif
    }
}

void LoadObjFiles(Link& link,  section *sect )
/********************************/
{
    file_list   *file;

    link.CurrSect = sect;
    link.CurrMod = nullptr;
    for( file = sect->files; file != nullptr; file = file->next ) {
        DoPass1(link,  nullptr, file );
    }
}

char *IdentifyObject(Link& link,  const file_list *file, unsigned long *loc, unsigned long *size )
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
        ar_hdr = static_cast<ar_header *>( CacheRead(link,  file, ar_loc, sizeof( ar_header ) ) );
        ar_loc += sizeof( ar_header );
        name = GetARName(link,  ar_hdr, file, &ar_loc );
        *size = GetARValue( ar_hdr->size, AR_SIZE_LEN );
        *loc = ar_loc;
    }
    switch( FileTypeORL( link, file, *loc ) ) {
    case ORL_ELF:
        link.ObjFileFormat = FILE_FMT_ELF;
        break;
    case ORL_COFF:
        link.ObjFileFormat = FILE_FMT_COFF;
        break;
    case ORL_OMF:
        link.ObjFileFormat = FILE_FMT_OMF;
        if( (file->flags_file & STAT_IS_LIB) == STAT_OMF_LIB ) {
            name = GetOMFName(link,  file, loc );
        }
        break;
    default:
        break;
    }
    return( name );
}

unsigned long ObjPass1(Link& link)
/****************************/
/* Pass 1 of 8086 linker. */
{
    unsigned long loc;

    DEBUG(( DBG_BASE, "1 : file = %s, module = %s", link.CurrMod->u1.source->infile->name.u.ptr, link.CurrMod->name.u.ptr ));
    link.CurrMod->flags_mod |= MOD_DONE_PASS_1;
    SymModStart(link);
    DBIInitModule(link,  link.CurrMod );
    RelocStartMod();
    P1Start();
    loc = Process[link.ObjFileFormat].Pass1( link );
    CollapseLazyExtdefs(link);
    SymModEnd(link);
    DBIP1ModuleScanned(link);
    ReleaseNames(link);
    PermEndMod( link.CurrMod );
    FreeObjInfo(link);
    link.ObjFileFormat = static_cast<file_format>( 0 );
    link.ObjFormat = static_cast<obj_format>( 0 );       //clear flags for processing obj file
    return( loc );
}

static bool ResolveVFExtdefs(Link& link)
/**********************************/
/* go through the symbol table, and check if any conditional extdefs
 * should turned into real extdefs */
{
    bool        resolved;
    symbol      *sym;

    resolved = false;
    for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
        if( IS_SYM_VF_REF( sym ) ) {
            resolved |= CheckVFList( sym );
        }
    }
    return( resolved );
}

void ResolveUndefined(Link& link)
/**********************************/
{
    symbol      *sym;
    file_list   *file;
    bool        keepgoing;

    LnkMsg( INF+MSG_SEARCHING_LIBS, nullptr );
#ifdef _EXE
    if( (link.FmtData.type & MK_OVERLAYS)
      && link.FmtData.u.dos.distribute ) {
        link.LinkState |= LS_CAN_REMOVE_SEGMENTS;
        DistribInitMods();
    }
#endif
    link.CurrSect = link.Root;
    ResolveVFExtdefs(link);
    do {
        link.LinkState &= ~LS_LIBRARIES_ADDED;
        for( file = link.ObjLibFiles; file != nullptr; file = file->next ) {
            if( file->flags_file & STAT_SEEN_LIB ) {
                file->flags_file |= STAT_OLD_LIB;
            } else {
                file->flags_file |= STAT_SEEN_LIB;
            }
        }
        for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
            if( ( (sym->info & SYM_DEFINED) == 0
              && !IS_SYM_WEAK_REF( sym )
              || (link.FmtData.type & MK_NOVELL)
              && IS_SYM_IMPORTED( sym )
              && (sym->info & (SYM_REFERENCED | SYM_LOCAL_REF)) )
              && (sym->info & SYM_IS_ALTDEF) == 0 ) {
                LibFind(link,  sym->name.u.ptr, (sym->info & SYM_CHECKED) != 0 );
            }
            sym->info |= SYM_CHECKED;
        }
        keepgoing = ResolveVFExtdefs(link);
    } while( keepgoing || (link.LinkState & LS_LIBRARIES_ADDED) );

    BurnLibs(link);
    PrintBadTraces();
}

void ProcLocalImports(Link& link)
/***************************/
{
#ifdef _OS2
    symbol  *sym;

    if( link.FmtData.type & MK_PE ) {
        for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
            if( (sym->info & SYM_DEFINED) == 0
              && !IS_SYM_WEAK_REF( sym )
              && (sym->info & SYM_IS_ALTDEF) == 0 ) {
                ImportPELocalSym(link,  sym );
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
