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
* Description:  Routines for creating ELF executable images.
*
****************************************************************************/


/*
-----------
Historical note - the ELF output support in wlink was initially developed
for IBM's OS/2 for PowerPC which used ELF, hence references to OS/2.
-----------

Layout of OS/2 ELF Executable:

----------------------------------------------------------------------------
Elf header
----------------------------------------------------------------------------
Program headers:
----------------------------------------------------------------------------
Groups data
----------------------------------------------------------------------------
Various symbol tables
----------------------------------------------------------------------------
Debug data (for dwarf)
----------------------------------------------------------------------------
Section headers:    - Empty unused section
                    - Section for each group in Groups
                    - Various symbol tables sections
                    - Relocations
----------------------------------------------------------------------------
String table for sections
----------------------------------------------------------------------------
*/

#include <string.h>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "exeelf.h"
#include "build/x86link/formats/elf.h"
#include "build/x86link/formats/elf2.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/debuginfo/dbgcomm.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/debuginfo/dbgdwarf.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/link/run_state.hpp"
#include <string>


#ifdef _ELF

namespace {

// What this file keeps for one link (fresh at each run's start: the program header count was a static that a DLL link
// set and no later link cleared).
struct ElfState {
    stringtable     SymStrTab;
    ElfSymTable     *ElfSymTab = nullptr;
    int             NumPhdr = 0;    /* just for now untill dynamic objects supported */
};

ElfState& state()
{
    return cb_wlink::RunState::part<ElfState>();
}

/* Put debugging info into section WITHIN the file instead of appending a
 * separate elf file at the end */
bool injectElfDebug(Link& link)
{
    return link.SymFileName == nullptr && (link.LinkFlags & LF_DWARF_DBI_FLAG) != 0;
}

}  // namespace

static void InitSections(Link& link,  ElfHdr *hdr )
/*************************************/
{
    unsigned        num;
    group_entry     *group;

    num = 1;
    memset( &hdr->i, 0, sizeof( hdr->i ) );
    hdr->i.secstr = num++;
    hdr->eh.e_shstrndx = hdr->i.secstr;
    hdr->i.grpbase = num;
    hdr->i.grpnum = link.NumGroups;
    if( link.FmtData.dgroupsplitseg != nullptr ) {
        hdr->i.grpnum++;
        if( link.StackSegPtr != nullptr ) {
            hdr->i.grpnum++;
        }
    }
    num += hdr->i.grpnum;
    hdr->i.relbase = num;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->g.reloclist != nullptr ) {
            hdr->i.relnum++;
        }
    }
    num += hdr->i.relnum;
    hdr->i.symtab = num++;
    hdr->i.symstr = num++;
    hdr->i.symhash = num++;
    if( injectElfDebug(link) ) {
        hdr->i.dbgnum = DwarfCountDebugSections(link);
    }
    hdr->i.dbgbegin = num;
    num += hdr->i.dbgnum;
    num += link.FmtData.u.elf.extrasects;
    hdr->eh.e_shnum = num;
    hdr->sh_size = sizeof( Elf32_Shdr ) * hdr->eh.e_shnum;
    hdr->sh = static_cast<Elf32_Shdr *>( MemAllocSafe(link,  hdr->sh_size ) );
    memset( hdr->sh, 0, hdr->sh_size );
    hdr->sh[hdr->i.symtab].sh_name = AddSecName(link,  hdr, ".symtab" );
}

static void SetHeaders(Link& link,  ElfHdr *hdr )
/***********************************/
{
    memcpy( hdr->eh.e_ident, ELF_SIGNATURE, ELF_SIGNATURE_LEN );
    hdr->eh.e_ident[EI_CLASS] = ELFCLASS32;
#ifdef __BIG_ENDIAN__
    hdr->eh.e_ident[EI_DATA] = ELFDATA2MSB;
#else
    hdr->eh.e_ident[EI_DATA] = ELFDATA2LSB;
#endif
    hdr->eh.e_ident[EI_VERSION] = EV_CURRENT;
    hdr->eh.e_ident[EI_OSABI] = link.FmtData.u.elf.abitype;
    hdr->eh.e_ident[EI_ABIVERSION] = link.FmtData.u.elf.abiversion;
    memset( &hdr->eh.e_ident[EI_PAD], 0, EI_NIDENT - EI_PAD );
    hdr->eh.e_type = ET_EXEC;
    switch( link.LinkState & LS_HAVE_MACHTYPE_MASK ) {
    case LS_HAVE_ALPHA_CODE:
        hdr->eh.e_machine = EM_ALPHA;
        break;
    case LS_HAVE_MIPS_CODE:
        hdr->eh.e_machine = EM_MIPS;
        break;
    case LS_HAVE_PPC_CODE:
        hdr->eh.e_machine = EM_PPC;
        break;
    case LS_HAVE_X64_CODE:
        hdr->eh.e_machine = EM_X86_64;
        break;
    case LS_HAVE_X86_CODE:
        hdr->eh.e_machine = EM_386;
        break;
    default:
        hdr->eh.e_machine = EM_NONE;
        break;
    }
    hdr->eh.e_version = EV_CURRENT;
    if( link.StartInfo.type == START_UNDEFED ) {
        hdr->eh.e_entry = 0;
    } else {
        hdr->eh.e_entry = FindLinearAddr2(link,  &link.StartInfo.addr );
    }
    hdr->eh.e_flags = 0;
    hdr->eh.e_ehsize = sizeof( Elf32_Ehdr );
    hdr->eh.e_phentsize = sizeof( Elf32_Phdr );
    hdr->eh.e_shentsize = sizeof( Elf32_Shdr );
    hdr->eh.e_phnum = link.NumGroups + state().NumPhdr;
    hdr->eh.e_phoff = sizeof( Elf32_Ehdr );
    hdr->ph_size = sizeof( Elf32_Phdr ) * hdr->eh.e_phnum;
    hdr->ph = static_cast<Elf32_Phdr *>( MemAllocSafe(link,  hdr->ph_size ) );
    if( state().NumPhdr ) {
        hdr->ph->p_type = PT_PHDR;
        hdr->ph->p_offset = sizeof( Elf32_Ehdr );
        hdr->ph->p_vaddr = sizeof( Elf32_Ehdr ) + link.FmtData.base;
        hdr->ph->p_paddr = 0;
        hdr->ph->p_filesz = hdr->ph_size;
        hdr->ph->p_memsz = hdr->ph_size;
        hdr->ph->p_flags = PF_R | PF_X;
        hdr->ph->p_align = 0;
    }
    InitStringTable(link,  &hdr->secstrtab, false );
    AddCharStringTable(link,  &hdr->secstrtab, '\0' );
    InitSections(link,  hdr );
    hdr->curr_off = hdr->eh.e_ehsize + hdr->ph_size;
    hdr->curr_off = __ROUND_UP_SIZE_PAGE( hdr->curr_off );
    SeekLoad(link,  hdr->curr_off );
}

unsigned GetElfHeaderSize(Link& link)
/**************************************/
{
    unsigned        size;

    size = sizeof( Elf32_Ehdr ) + sizeof( Elf32_Phdr ) * ( link.NumGroups + state().NumPhdr );
    return( __ROUND_UP_SIZE_PAGE( size ) );
}

size_t AddSecName(Link& link,  ElfHdr *hdr, const char *name )
/************************************************/
{
    return( AddStringStringTableOffs(link,  &hdr->secstrtab, name ) );
}


static void WriteSHStrings(Link& link,  ElfHdr *hdr, int str_idx, stringtable *strtab )
/*************************************************************************/
{
    Elf32_Shdr      *sh;

    sh = hdr->sh + str_idx;
    sh->sh_name = AddSecName(link,  hdr, ".shstrtab" );
    sh->sh_offset = hdr->curr_off;
    sh->sh_type = SHT_STRTAB;
    sh->sh_size = GetStringTableSize( strtab );
    hdr->curr_off += sh->sh_size;
    WriteStringTable( strtab, bind_link( link, WriteLoad3 ), nullptr );
}

static void SetGroupHeaders(Link& link,  group_entry *group, offset off, Elf32_Phdr *ph,
                             Elf32_Shdr *sh )
/**************************************************************************/
{
    sh->sh_type = SHT_PROGBITS;
    sh->sh_flags = SHF_ALLOC;
    ph->p_flags = PF_R;
    if( group->segflags & SEG_DATA ) {
        sh->sh_flags |= SHF_WRITE;
        ph->p_flags |= PF_W;
    } else { // if code group
        sh->sh_flags |= SHF_EXECINSTR;
        ph->p_flags |= PF_X;
    }
    sh->sh_addr = ph->p_vaddr = group->linear + link.FmtData.base;
    ph->p_type = PT_LOAD;
    ph->p_filesz = sh->sh_size = group->size;
    ph->p_memsz = group->totalsize;
    if( group == link.DataGroup && link.StackSegPtr != nullptr ) {
        ph->p_memsz -= link.StackSize;
    }
    sh->sh_link = SHN_UNDEF;
    sh->sh_info = 0;
    sh->sh_addralign = 4;
    sh->sh_entsize = 0;
    ph->p_offset = sh->sh_offset = off;
    ph->p_paddr = 0;
    ph->p_align = link.FmtData.objalign;
}

static void InitBSSSect( Elf32_Shdr *sh, offset off, offset size, offset start )
/******************************************************************************/
{
    sh->sh_type = SHT_NOBITS;
    sh->sh_flags = SHF_ALLOC | SHF_WRITE;
    sh->sh_addr = start;
    sh->sh_offset = off;
    sh->sh_size = size;
    sh->sh_link = SHN_UNDEF;
    sh->sh_info = 0;
    sh->sh_addralign = 4;
    sh->sh_entsize = 0;
}

static const char *GroupSecName( group_entry *group )
/***************************************************/
{
    if( group->segflags & SEG_DATA ) {
        return( ".data" );
    } else {
        return( ".text" );
    }
}

static void WriteELFGroups(Link& link,  ElfHdr *hdr )
/***************************************/
{
    group_entry     *group;
    Elf32_Shdr      *sh;
    Elf32_Phdr      *ph;
    offset          off;
    offset          linear;

    sh = hdr->sh + hdr->i.grpbase;
    ph = hdr->ph + state().NumPhdr;
    off = hdr->curr_off;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->totalsize == 0 )
            continue;   // DANGER DANGER DANGER <--!!!
        SetGroupHeaders(link,  group, off, ph, sh );
        WriteGroupLoad(link,  group, false );
        off = OffsetAlign(link,  off + group->size, link.FmtData.objalign );
        sh->sh_name = AddSecName(link,  hdr, GroupSecName( group ) );
        sh++;
        if( group == link.DataGroup && link.FmtData.dgroupsplitseg != nullptr ) {
            sh->sh_name = AddSecName(link,  hdr, ".bss" );
            linear = ph->p_vaddr + __ROUND_UP_SIZE( ph->p_filesz, link.FmtData.objalign );
            InitBSSSect( sh, off, CalcSplitSize(link), linear );
            linear = __ROUND_UP_SIZE( linear + sh->sh_size, link.FmtData.objalign);
            sh++;
            if( link.StackSegPtr != nullptr ) {
                sh->sh_name = AddSecName(link,  hdr, ".stack" );
                InitBSSSect( sh, off, link.StackSize, linear );
                sh++;
            }
        }
        ph++;
    }
    hdr->curr_off = off;
}

static size_t AddRelocSectName(Link& link,  ElfHdr *hdr, const char *secname )
/****************************************************************/
{
    std::string     name( RelASecName );

    name += secname;
    return( AddSecName(link,  hdr, name.c_str() ) );
}


static void WriteRelocsSections(Link& link,  ElfHdr *hdr )
/********************************************/
{
    group_entry     *group;
    unsigned        currgrp;
    Elf32_Shdr      *sh;
    reloc_info      reloclist;

    currgrp = hdr->i.grpbase;
    sh = hdr->sh + hdr->i.relbase;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        reloclist = group->g.reloclist;
        if( reloclist != nullptr ) {
            sh->sh_offset = hdr->curr_off;
            sh->sh_entsize = sizeof( elf_reloc_item );
            sh->sh_type = SHT_RELA;
            sh->sh_flags = SHF_ALLOC;
            sh->sh_addr = 0;
            sh->sh_link = hdr->i.symtab;
            sh->sh_info = currgrp;
            sh->sh_addralign = 4;
            sh->sh_size = RelocSize( reloclist );
            sh->sh_name = AddRelocSectName(link,  hdr, GroupSecName( group ) );
            DumpRelocList(link,  reloclist );
            hdr->curr_off += sh->sh_size;
            sh++;
        }
        currgrp++;
    }
}

void FiniELFLoadFile(Link& link)
/*********************************/
{
    ElfHdr          hdr;

    if( link.FmtData.dll )
        state().NumPhdr = 1;

    SetHeaders(link,  &hdr );
#if 0
    if( (link.LinkState & LS_HAVE_PPC_CODE) && (link.FmtData.type & (MK_NE | MK_OS2_FLAT)) ) {
        // Development temporarly on hold
        // BuildOS2Imports(); // Build .got section
    }
#endif

    WriteELFGroups(link,  &hdr ); // Write out all groups
    WriteRelocsSections(link,  &hdr );        // Relocations
    if( injectElfDebug(link) ) {            // Debug info
        hdr.curr_off = DwarfWriteElf(link,  hdr.curr_off, &hdr.secstrtab, hdr.sh + hdr.i.dbgbegin );
    }
    if( state().ElfSymTab != nullptr ) {           // Symbol tables
        WriteElfSymTable(link,  state().ElfSymTab, &hdr, hdr.i.symhash, hdr.i.symtab, hdr.i.symstr );
        ZapElfSymTable( state().ElfSymTab );
    }
    if( hdr.i.symstr != 0 ) {           // String sections
        WriteSHStrings(link,  &hdr, hdr.i.symstr, &state().SymStrTab );
    }
    WriteSHStrings(link,  &hdr, hdr.i.secstr, &hdr.secstrtab );
    hdr.eh.e_shoff = hdr.curr_off;
    WriteLoad(link,  hdr.sh, hdr.sh_size );
    hdr.curr_off += hdr.sh_size;
    if( !injectElfDebug(link) ) {
        DBIWrite(link);
    }
    SeekLoad(link,  0 );
    WriteLoad(link,  &hdr.eh, sizeof( Elf32_Ehdr ) );
    WriteLoad(link,  hdr.ph, hdr.ph_size );
    MemFree( hdr.sh );
    MemFree( hdr.ph );
    FiniStringTable( &hdr.secstrtab );
    FiniStringTable( &state().SymStrTab );
    SeekLoad(link,  hdr.curr_off );
}

void ChkElfData(Link& link)
/****************************/
{
    group_entry     *group;
    symbol          *sym;

    link.NumExports = link.NumImports = 0;
    for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
        if( IsSymElfImported( sym ) ) {
            link.NumImports++;
        } else if( IsSymElfExported(link,  sym ) ) {
            if( (sym->info & SYM_DEFINED) == 0 ) {
                LnkMsg(link,  ERR+MSG_EXP_SYM_NOT_FOUND, "s", sym->name );
            }
            link.NumExports++;
        }
    }
    InitStringTable(link,  &state().SymStrTab, false );
    AddCharStringTable(link,  &state().SymStrTab, '\0' );
    state().ElfSymTab = CreateElfSymTable(link,  link.NumImports + link.NumExports + link.NumGroups, &state().SymStrTab );
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->totalsize != 0 ) {
            AddSymElfSymTable( state().ElfSymTab, group->sym );
        }
    }
    /* process local symbols */
    for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
        if( IsSymElfImpExp(link,  sym ) && (sym->info & SYM_STATIC) ) {
            AddSymElfSymTable( state().ElfSymTab, sym );
        }
    }
    /* process global symbols */
    for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
        if( IsSymElfImpExp(link,  sym ) && (sym->info & SYM_STATIC) == 0 ) {
            AddSymElfSymTable( state().ElfSymTab, sym );
        }
    }

}

int FindElfSymIdx( symbol *sym )
/******************************/
{
    return( FindSymIdxElfSymTable( state().ElfSymTab, sym ) );
}

#endif
