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
* Description:  Utilities for handling imports and exports.
*
****************************************************************************/


#include <string.h>
#include <ctype.h>
#include <string>
#include <string_view>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/link/specials.h"
#include "build/x86link/base/fileio.h"
#include "exeflat.h"
#include "exepe.h"
#include "exedos.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/formats/os2.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/symbols/impexp.h"

#include "clibext.h"


static entry_export *FindPlace( entry_export *exp )
/*************************************************/
// finds the correct place to put exp to keep the export list sorted.
{
    entry_export    *place;
    entry_export    *prev;
    entry_export    *ret;

    ret = nullptr;
    prev = nullptr;
    place = exp->next;
    if( place != nullptr && place->ordinal <= exp->ordinal ) {
        for( ;; ) {
            if( place->ordinal == exp->ordinal ) {
                LnkMsg( WRN + MSG_DUP_EXP_ORDINAL, nullptr );
                exp->ordinal = 0;       // if duplicate, assign a new one later
                break;
            }
            if( place->ordinal > exp->ordinal ) {
                ret = exp->next;        // note: this can't happen 1st time
                exp->next = place;
                prev->next = exp;
                break;
            }
            if( place->next == nullptr ) { // no more entries, so put on the
                ret = exp->next;        // end, then break the loop.
                place->next = exp;
                exp->next = nullptr;
                break;
            }
            prev = place;
            place = place->next;
        }
    }
    return( ret );
}

static entry_export *FreeAnExport(Link& link,  entry_export *exp )
/****************************************************/
{
    entry_export    *next;

    MemFree( exp->impname );
    next = exp->next;
    CarveFree( link.CarveExportInfo, exp );
    return( next );
}

void FreeExportList(Link& link)
/*************************/
{
    entry_export    *exp;

    if( (link.LinkFlags & LF_INC_LINK_FLAG) == 0 ) {
        for( exp = link.FmtData.u.os2fam.exports; exp != nullptr; ) {
            exp = FreeAnExport(link,  exp );
        }
    }
}

void AddToExportList(Link& link,  entry_export *exp )
/***************************************/
{
    entry_export        **owner;
    entry_export        **place;
    entry_export        *curr;
    size_t              len;
    size_t              currlen;

    place = nullptr;
    len = strlen( exp->name.u.ptr );
    for( owner = &link.FmtData.u.os2fam.exports; (curr = *owner) != nullptr; owner = &curr->next ) {
        currlen = strlen( curr->name.u.ptr );
        if( currlen == len && (*CmpNRtn)( curr->name.u.ptr, exp->name.u.ptr, len ) == 0 ) {
            if( link.ObjFileFormat != FILE_FMT_INCREMENTAL ) {
                if( !IS_SYM_COMDAT( exp->sym ) ) {
                    LnkMsg( WRN+LOC+MSG_DUP_EXP_NAME, "s", curr->name.u.ptr );
                }
                FreeAnExport(link,  exp );
            }
            return;
        }
        if( place == nullptr ) {
            if( exp->ordinal == 0 ) {
                if( curr->ordinal != 0 ) {
                    place = owner;
                }
            } else if( curr->ordinal == exp->ordinal ) {
                LnkMsg( WRN+MSG_DUP_EXP_ORDINAL, nullptr );
                exp->ordinal = 0;    // if duplicate, assign a new one later
                place = &link.FmtData.u.os2fam.exports;
            } else if( curr->ordinal > exp->ordinal ) {
                place = owner;
            }
        }
    }
    if( IS_SYM_VF_REF( exp->sym ) ) {
        ClearRefInfo( exp->sym );
    }
    exp->sym->e.export_ = exp;
    exp->sym->info |= SYM_EXPORTED;
    if( place == nullptr )
        place = owner;
    DEBUG(( DBG_NEW, "%s", exp->name ));
    exp->next = *place;
    *place = exp;
}

static size_t CheckStdCall( const char *name, size_t len )
/********************************************************/
// check to see if a name is in the stdcall _name@xx format
// this returns the total number of characters to be removed from the name
// including the beginning _
{
    const char  *teststr;
    size_t      chop;

    chop = 0;
    if( len > 3 ) {
        teststr = name + len - 1;
        if( *name == '_' && isdigit( static_cast<unsigned char>( *teststr ) ) ) {
            teststr--;
            if( *teststr == '@' ) {
                chop = 3;
            } else if( isdigit( static_cast<unsigned char>( *teststr ) ) ) {
                teststr--;
                if( *teststr == '@' ) {
                    chop = 4;
                }
            }
        }
    }
    return( chop );
}

entry_export *AllocExport(Link& link,  const char *name, size_t len )
/*******************************************************/
{
    entry_export    *exp;
    size_t          chop;

    exp = static_cast<entry_export *>( CarveAlloc( link.CarveExportInfo ) );
    exp->isresident = false;
    exp->isexported = true;
    exp->ismovable = false;
    exp->isanonymous = false;
    exp->isfree = false;
    exp->isprivate = false;
    exp->isiopl = false;
    exp->isnodata = false;
    if( name == nullptr ) {
        exp->name.u.ptr = nullptr;
    } else {
        if( (link.FmtData.type & MK_PE) && link.FmtData.u.pe.no_stdcall ) {
            chop = CheckStdCall( name, len );
            if( chop > 0 ) {
                name++;
                len -= chop;
            }
        }
        exp->name.u.ptr = AddSymbolStringTable( &link.PermStrings, name, len );
    }
    exp->impname = nullptr;
    exp->iopl_words = 0;
    exp->ordinal = 0;
    return( exp );
}

static constexpr unsigned EXPDEF_ORDINAL  = 0x80;
static constexpr unsigned EXPDEF_RESIDENT = 0x40;
static constexpr unsigned EXPDEF_IOPLMASK = 0x1F;

void MSExportKeyword(Link& link,  const length_name *expname, const length_name *intname, unsigned flags, ordinal_t ordinal )
/***************************************************************************************************************/
// Process the Microsoft Export keyword.
{
    entry_export    *exp;

    exp = AllocExport(link,  expname->name, expname->len );
    exp->iopl_words = flags & EXPDEF_IOPLMASK;
    if( flags & EXPDEF_RESIDENT )
        exp->isresident = true;
    if( intname->len != 0 ) {
        exp->sym = SymOp(link,  ST_CREATE_REFERENCE, intname->name, intname->len );
    } else {
        exp->sym = SymOp(link,  ST_CREATE_REFERENCE, expname->name, expname->len );
    }
    if( link.LinkFlags & LF_STRIP_CODE ) {
        DataRef(link,  exp->sym );    // make sure it isn't removed.
    }
    if( flags & EXPDEF_ORDINAL ) {
        exp->ordinal = ordinal;
    } else {
        exp->isresident = true; // no ordinal specd == isresident
    }
    AddToExportList(link,  exp );
}

dll_sym_info *AllocDLLInfo(Link& link)
/********************************/
{
    dll_sym_info    *dll;

    dll = static_cast<dll_sym_info *>( CarveAlloc( link.CarveDLLInfo ) );
    dll->isfree = false;
    return( dll );
}

void FreeImport(Link& link,  dll_sym_info *dll )
/**********************************/
{
    CarveFree( link.CarveDLLInfo, dll );
}

static symbol *GetIATSym(Link& link,  symbol *sym )
/*************************************/
{
    const char  *name;

    name = sym->name.u.ptr;
    if( link.LinkState & LS_HAVE_PPC_CODE ) {
        DbgAssert(name[0] == '.' && name[1] == '.');
        name += 2;  // skip '..' at the beginning of the name
    }
    std::string iatname( ImportSymPrefix, sizeof( ImportSymPrefix ) - 1 );
    iatname += name;
    return( SymOp(link,  ST_CREATE, iatname.c_str(), iatname.size() ) );
}

void MSImportKeyword(Link& link,  symbol *sym, const length_name *modname, const length_name *extname, ordinal_t ordinal )
/************************************************************************************************************/
/* process the MS import keyword definition */
{
    dll_sym_info    *dll;

    if( (sym->info & SYM_DEFINED) == 0 ) {
        sym->info |= SYM_DEFINED | SYM_1_DCE_REF;
        if( link.LinkFlags & LF_STRIP_CODE ) {
            DefStripImpSym( sym );
        }
        SET_SYM_TYPE( sym, SYM_IMPORTED );
        dll = AllocDLLInfo(link);
        sym->p.import_dll = dll;
        if( link.FmtData.type & MK_PE ) {
            dll->iatsym = GetIATSym(link,  sym );
            dll->iatsym->info |= SYM_DEFINED | SYM_1_DCE_REF;
            SET_SYM_TYPE( dll->iatsym, SYM_IMPORTED );
            dll->iatsym->p.import_dll = nullptr;
        }
        dll->m.modnum = AddNameTable(link,  modname->name, modname->len, true, &link.FmtData.u.os2fam.mod_ref_list );
        if( ordinal == IMP_BY_NAME ) {
            dll->isordinal = false;
            dll->u.entry = AddNameTable(link,  extname->name, extname->len, false, &link.FmtData.u.os2fam.imp_tab_list );
        } else {
            dll->isordinal = true;
            dll->u.ordinal = ordinal;
        }
    }
}

void KillDependantSyms(Link& link,  symbol *sym )
/******************************************/
{
    if( link.FmtData.type & MK_PE ) {
        sym = GetIATSym(link,  sym );
        sym->info |= SYM_KILL;
    }
}

void CheckExport(Link& link,  const char *name, ordinal_t ordinal, bool cmpcase )
/*******************************************************************/
/* check if the name is exported and hasn't been assigned a value, and if so,
 * give it the specified value */
{
    entry_export    *place;
    entry_export    *prev;
    int             (*rtn)(const char *,const char *);

    DEBUG(( DBG_OLD, "Oldlib export %s ordinal %l", name, ordinal ));
    rtn = ( cmpcase ) ? strcmp : stricmp;
    prev = nullptr;
    for( place = link.FmtData.u.os2fam.exports; place != nullptr; place = place->next ) {
        if( rtn( place->name.u.ptr, name ) == 0 ) {
            if( place->ordinal == 0 ) {
                place->ordinal = ordinal;
                place = FindPlace( place );
                if( place != nullptr ) {
                    if( prev == nullptr ) {
                        link.FmtData.u.os2fam.exports = place;
                    } else {
                        prev->next = place;
                    }
                }
            }
            break;
        }
        prev = place;
    }
}

ordinal_t FindEntryOrdinal(Link& link,  addr_type addr, group_entry *group )
/**************************************************************/
{
    ordinal_t       max_ord;
    entry_export    **owner;
    entry_export    *exp;

    max_ord = 0;
    for( owner = &link.FmtData.u.os2fam.exports; (exp = *owner) != nullptr; owner = &exp->next ) {
        if( addr.seg == exp->addr.seg && addr.off == exp->addr.off ) {
            return( exp->ordinal );
        }
        if( exp->ordinal >= max_ord ) {
            max_ord = exp->ordinal;
        }
    }
    exp = AllocExport(link,  nullptr, 0 );
    exp->sym = nullptr;
    exp->isexported = false;
    exp->ordinal = max_ord + 1;
    if( group->segflags & SEG_MOVABLE )
        exp->ismovable = true;
    exp->next = nullptr;
    exp->addr = addr;
    *owner = exp;
    return( exp->ordinal );
}

bool IsSymElfImported( symbol *sym )
/**********************************/
{
    return( IS_SYM_IMPORTED( sym ) );
}

bool IsSymElfExported(Link& link,  symbol *sym )
/**********************************/
{
    return( link.FmtData.u.elf.exportallsyms || (sym->info & SYM_EXPORTED) );
}

bool IsSymElfImpExp(Link& link,  symbol *sym )
/********************************/
{
    return( IsSymElfImported( sym ) || IsSymElfExported(link,  sym ) );
}
