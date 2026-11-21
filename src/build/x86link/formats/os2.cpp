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
* Description:  Utilities for processing creation of NE format files,
*               used by 16-bit OS/2 and Windows.
*
****************************************************************************/


#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include "wio.h"
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/base/ring.h"
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/fixups/reloc.h"
#include "exeflat.h"
#include "exepe.h"
#include "exedos.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/symbols/impexp.h"
#include "wres.h"
#include "rcstrblk.h"
#include "rcstr.h"
#include "build/x86link/formats/dosstub.h"
#include "build/x86link/formats/os2.h"
#include "posixfp.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


#ifdef _OS2

namespace {

constexpr int       NE_ORDID = 0x8000;
constexpr unsigned  STUB_ALIGN = 16U;

struct FullResourceRecord {
    FullResourceRecord      *Next;
    FullResourceRecord      *Prev;
    resource_record         Info;
};

struct FullTypeRecord {
    FullTypeRecord          *Next;
    FullTypeRecord          *Prev;
    FullResourceRecord      *Head;
    FullResourceRecord      *Tail;
    resource_type_record    Info;
};

struct ExeResDir {
    unsigned_16         ResShiftCount;
    unsigned_16         NumTypes;
    unsigned_16         NumResources;
    unsigned_16         TableSize;
    FullTypeRecord      *Head;
    FullTypeRecord      *Tail;
};

struct ResTable {
    ExeResDir       Dir;
    StringsBlock    Str;
};

}   // namespace


static void ReadNameTable( f_handle the_file )
/*********************************************
 * Read a name table & set export ordinal value accordingly.
 */
{
    unsigned_8          len_u8;
    unsigned_16         ordinal;
    bool                cmpcase;
    const char          *fname;

    fname = wlink().FmtData.u.os2fam.old_lib_name;
    cmpcase = ( (wlink().LinkFlags & LF_CASE_FLAG) != 0 );
    for( ;; ) {
        QRead( the_file, &len_u8, sizeof( len_u8 ), fname );
        if( len_u8 == 0 )
            break;
        QRead( the_file, wlink().TokBuff, len_u8, fname );
        QRead( the_file, &ordinal, sizeof( ordinal ), fname );
        if( ordinal == 0 )
            continue;
        wlink().TokBuff[len_u8] = '\0';
        CheckExport( wlink().TokBuff, ordinal, cmpcase );
    }
}

static void ReadOldLib( void )
/*****************************
 * Read an old DLL & match ordinals of exports in it with exports in this.
 */
{
    f_handle    the_file;
    union {
        dos_exe_header  dos;
        os2_exe_header  os2;
        os2_flat_header os2f;
        pe_exe_header   pe;
    }           head;
    char        *fname;
    std::vector<pe_object>  objects;
    pe_object   *currobj;
    unsigned_32 ne_header_off;
    unsigned_16 signature;

    fname = wlink().FmtData.u.os2fam.old_lib_name;
    the_file = QOpenR( fname );
    QRead( the_file, &head, sizeof( dos_exe_header ), fname );
    if( head.dos.signature != EXESIGN_DOS
      || !NE_HEADER_FOLLOWS( head.dos.reloc_offset ) ) {
        LnkMsg( WRN + MSG_INV_OLD_DLL, nullptr );
    } else {
        QSeek( the_file, NE_HEADER_OFFSET, fname );
        QRead( the_file, &ne_header_off, sizeof( ne_header_off ), fname );
        QSeek( the_file, ne_header_off, fname );
        QRead( the_file, &signature, sizeof( signature ), fname );
        QSeek( the_file, ne_header_off, fname );
        if( signature == EXESIGN_NE ) {
            QRead( the_file, &head.os2, sizeof( head.os2 ), fname );
            QSeek( the_file, ne_header_off + head.os2.resident_off, fname );
            ReadNameTable( the_file );
            QSeek( the_file, head.os2.nonres_off, fname );
            ReadNameTable( the_file );
        } else if( signature == EXESIGN_LE
          || signature == EXESIGN_LX ) {
            QRead( the_file, &head.os2f, sizeof( head.os2f ), fname );
            if( head.os2f.resname_off != 0 ) {
                QSeek( the_file, ne_header_off + head.os2f.resname_off, fname );
                ReadNameTable( the_file );
            }
            if( head.os2f.nonres_off != 0 ) {
                QSeek( the_file, head.os2f.nonres_off, fname );
                ReadNameTable( the_file );
            }
        } else if( signature == EXESIGN_PE ) {
            unsigned    num_objects;

            QRead( the_file, &head.pe, PE_HDR_SIZE, fname );
            QRead( the_file, reinterpret_cast<char *>( &head.pe ) + PE_HDR_SIZE, PE_OPT_SIZE( head.pe ), fname );
            num_objects = head.pe.fheader.num_objects;
            objects.resize( num_objects );
            QRead( the_file, objects.data(), num_objects * sizeof( pe_object ), fname );
            currobj = objects.data();
            for( ; num_objects > 0; --num_objects ) {
                if( currobj->rva == PE_DIRECTORY( head.pe, PE_TBL_EXPORT ).rva ) {
                    QSeek( the_file, currobj->physical_offset, fname );
                    PE_DIRECTORY( head.pe, PE_TBL_EXPORT ).rva -= currobj->physical_offset;
                    ReadPEExportTable( the_file, &PE_DIRECTORY( head.pe, PE_TBL_EXPORT ) );
                    break;
                }
                currobj++;
            }
            if( num_objects == 0 ) {
                LnkMsg( WRN + MSG_INV_OLD_DLL, nullptr );
            }
        } else {
            LnkMsg( WRN+MSG_INV_OLD_DLL, nullptr );
        }
    }
    QClose( the_file, fname );
    MemFree( fname );
    wlink().FmtData.u.os2fam.old_lib_name = nullptr;
}

static void AssignOrdinals( void )
/*********************************
 * assign ordinal values to entries in the export list
 */
{
    entry_export        *exp;
    entry_export        *place;
    entry_export        *prev;
    bool                is_space;

    if( wlink().FmtData.u.os2fam.exports != nullptr ) {
        if( wlink().FmtData.u.os2fam.old_lib_name != nullptr ) {
            ReadOldLib();
        }
        prev = wlink().FmtData.u.os2fam.exports;
        place = prev->next;
        is_space = false;
        for( exp = wlink().FmtData.u.os2fam.exports; exp->ordinal == 0; exp = wlink().FmtData.u.os2fam.exports ) {
            /*
             * while still unassigned values
             * search for an unassigned value
             */
            for( ;; ) {
                if( place != nullptr ) {
                    is_space = ( ( place->ordinal - prev->ordinal ) > 1 );
                }
                if( place == nullptr
                  || is_space ) {
                    if( wlink().FmtData.u.os2fam.exports != prev ) {
                        wlink().FmtData.u.os2fam.exports = exp->next;
                        prev->next = exp;
                        exp->next = place;
                    }
                    exp->ordinal = prev->ordinal + 1;
                    /*
                     * now exp is 'previous' to place
                     */
                    prev = exp;
                    break;
                } else {
                    prev = place;
                    place = place->next;
                }
            }
        }
    }
}

static unsigned long WriteChainedRelocs( group_entry *group, unsigned long seg_start, unsigned long relocsize )
/***********************************************************************************************************
 * CodeBreaker (OPTION SELECTORFIXUPS): a selector to a segment of the module, wherever it is
 * wanted, is one record and a chain through the places -- each holds where the next is, the
 * last 0FFFFh -- as the original linker wrote them (KRNL386 follows them itself)
 */
{
    unsigned long   n, i, j, kept, at;
    unsigned_16     next;

    n = relocsize / sizeof( os2_reloc_item );
    std::vector<os2_reloc_item> items( n );
    CopyRelocList( group->g.reloclist, items.data() );
    kept = 0;
    at = PosLoad();
    for( i = 0; i < n; i++ ) {
        if( items[i].fixtype == 0xFF )      /* (taken into an earlier chain) */
            continue;
        if( items[i].reloc_type == INTERNAL_REFERENCE && items[i].fixtype == OSF_SOURCE_SEG ) {
            /* the others to the same segment, after this one, in its chain */
            for( j = i + 1; j < n; j++ ) {
                if( items[j].reloc_type == INTERNAL_REFERENCE && items[j].fixtype == OSF_SOURCE_SEG
                  && items[j].put.internal.grp_num == items[i].put.internal.grp_num ) {
                    break;
                }
            }
            {
                unsigned long   last = i;
                for( ; j < n; j++ ) {
                    if( items[j].reloc_type == INTERNAL_REFERENCE && items[j].fixtype == OSF_SOURCE_SEG
                      && items[j].put.internal.grp_num == items[i].put.internal.grp_num ) {
                        next = items[j].reloc_offset;
                        SeekLoad( seg_start + items[last].reloc_offset );
                        WriteLoad( &next, 2 );
                        items[j].fixtype = 0xFF;
                        last = j;
                    }
                }
            }
        }
        items[kept++] = items[i];
    }
    SeekLoad( at );
    WriteLoadU16( kept );
    WriteLoad( items.data(), kept * sizeof( os2_reloc_item ) );
    return( kept * sizeof( os2_reloc_item ) );
}

static unsigned long WriteOS2Relocs( group_entry *group, unsigned long seg_start )
/********************************************************
 * write all relocs associated with group to the file
 */
{
    unsigned long relocsize;
    unsigned long relocnum;

    relocsize = RelocSize( group->g.reloclist );
    relocnum = relocsize / sizeof( os2_reloc_item );
    if( relocnum == 0 )
        return( 0 );
    if( wlink().FmtData.u.os2fam.selector_fixups )
        return( WriteChainedRelocs( group, seg_start, relocsize ) );
    WriteLoadU16( relocnum );
    DumpRelocList( group->g.reloclist );
    return( relocsize );
}

static void WriteOS2Data( unsigned_32 stub_len, os2_exe_header *exe_head )
/*************************************************************************
 * copy code from extra memory to loadfile.
 */
{
    group_entry     *group;
    unsigned        group_num;
    unsigned long   off;
    segment_record  segrec;
    unsigned_32     seg_addr;
    unsigned long   relocsize;

    DEBUG(( DBG_BASE, "Writing data" ));

    group_num = 0;
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( group->totalsize == 0 )
            continue;   // DANGER DANGER DANGER <--!!!
        segrec.info = group->segflags;
        // write segment
        segrec.min = __ROUND_UP_SIZE_EVEN( group->totalsize );
        segrec.size = __ROUND_UP_SIZE_EVEN( group->size );
        if( segrec.size != 0 ) {
            off = NullAlign( 1 << wlink().FmtData.u.os2fam.segment_shift );
            seg_addr = off >> wlink().FmtData.u.os2fam.segment_shift;
            if( seg_addr > 0xffff ) {
                LnkMsg( ERR+MSG_ALIGN_TOO_SMALL, nullptr );
            }
            segrec.address = static_cast<unsigned_16>( seg_addr );
            WriteGroupLoad( group, false );
            NullAlign( 2 );         // segment must be even length
            relocsize = WriteOS2Relocs( group, off );
            if( relocsize != 0 ) {
                segrec.info |= SEG_RELOC;
            }
        } else {
            segrec.address = 0;
        }

        // write to segment table
        off = PosLoad();
        SeekLoad( exe_head->segment_off + stub_len + group_num * sizeof( segment_record ) );
        WriteLoad( &segrec, sizeof( segment_record ) );
        SeekLoad( off );
        group_num++;
    }
}

template<class Item>
static void AddLLItemAtEnd( Item *&head, Item *&tail, Item *item )
/****************************************************************/
{
    item->Next = nullptr;
    if( head == nullptr ) {
        head = tail = item;
    } else {
        item->Prev = tail;
        tail->Next = item;
        tail = item;
    }
}

static unsigned_16 findResOrTypeName( ResTable *restab, WResID *name )
/********************************************************************/
{
    unsigned_16 name_id;
    signed_32   str_offset;

    if( name->IsName ) {
        str_offset = StringBlockFind( &restab->Str, &name->ID.Name );
        if( str_offset == -1 ) {
            name_id = 0;
        } else {
            name_id = str_offset + restab->Dir.TableSize;
        }
    } else {
        name_id = name->ID.Num | NE_ORDID;
    }

    return( name_id );
}

static FullTypeRecord *addExeTypeRecord( ResTable *restab,
                            WResTypeInfo *type )
/**********************************************************/
{
    FullTypeRecord      *exe_type;

    exe_type = static_cast<FullTypeRecord *>( MemAllocSafe( sizeof( FullTypeRecord ) ) );
    exe_type->Info.reserved = 0;
    exe_type->Info.num_resources = type->NumResources;
    exe_type->Info.type = findResOrTypeName( restab, &(type->TypeName) );
    exe_type->Head = nullptr;
    exe_type->Tail = nullptr;
    exe_type->Next = nullptr;
    exe_type->Prev = nullptr;

    AddLLItemAtEnd( restab->Dir.Head, restab->Dir.Tail, exe_type );

    return( exe_type );
}

static void addExeResRecord( ResTable *restab, FullTypeRecord *type,
                            WResID *name, unsigned_16 mem_flags,
                      unsigned_16 exe_offset, unsigned_16 exe_length )
/********************************************************************/
{
    FullResourceRecord          *exe_res;

    exe_res = static_cast<FullResourceRecord *>( MemAllocSafe( sizeof( FullResourceRecord ) ) );
    exe_res->Info.offset = exe_offset;
    exe_res->Info.length = exe_length;
    exe_res->Info.flags = mem_flags;
    exe_res->Info.reserved = 0;
    exe_res->Info.name = findResOrTypeName( restab, name );
    exe_res->Next = nullptr;
    exe_res->Prev = nullptr;
    AddLLItemAtEnd( type->Head, type->Tail, exe_res );
}

static FullTypeRecord *findExeTypeRecord( ResTable *restab,
                            WResTypeInfo *type )
/***********************************************************/
{
    FullTypeRecord      *exe_type;
    StringItem16        *exe_type_name;

    for( exe_type = restab->Dir.Head; exe_type != nullptr;
                exe_type = exe_type->Next ) {
        if( type->TypeName.IsName
          && (exe_type->Info.type & NE_ORDID) == 0 ) {
            /* if they are both names */
            exe_type_name = reinterpret_cast<StringItem16 *>( static_cast<char *>( restab->Str.StringBlock ) +
                            ( exe_type->Info.type - restab->Dir.TableSize ) );
            if( exe_type_name->NumChars == type->TypeName.ID.Name.NumChars
              && strnicmp( exe_type_name->Name, type->TypeName.ID.Name.Name,
                            exe_type_name->NumChars ) == 0 ) {
                break;
            }
        } else if( !(type->TypeName.IsName)
          && (exe_type->Info.type & NE_ORDID) ) {
            /* if they are both numbers */
            if( type->TypeName.ID.Num == (exe_type->Info.type & ~NE_ORDID) ) {
                break;
            }
        }
    }
    if( exe_type == nullptr ) {              /* this is a new type */
        exe_type = addExeTypeRecord( restab, type );
    }
    return( exe_type );
} /* findExeTypeRecord */

static void FreeResTable( ResTable *restab )
/******************************************/
{
    FullTypeRecord              *type;
    FullResourceRecord          *res;

    while( (type = restab->Dir.Head) != nullptr ) {
        restab->Dir.Head = type->Next;
        while( (res = type->Head) != nullptr ) {
            type->Head = res->Next;
            MemFree( res );
        }
        MemFree( type );
    }
    restab->Dir.Tail = nullptr;
}

static void WriteResTable( ResTable *restab )
/*******************************************/
{
    FullTypeRecord      *exe_type;
    FullResourceRecord  *exe_res;

    WriteLoadU16( wlink().FmtData.u.os2fam.segment_shift );
    for( exe_type = restab->Dir.Head; exe_type != nullptr; exe_type = exe_type->Next ) {
        WriteLoad( &(exe_type->Info), sizeof( resource_type_record ) );
        for( exe_res = exe_type->Head; exe_res != nullptr; exe_res = exe_res->Next ) {
            WriteLoad( &(exe_res->Info), sizeof( resource_record ) );
        }
    }
    WriteLoadU16( 0 );
    WriteLoad( restab->Str.StringBlock, restab->Str.StringBlockSize );
}

static void CopyResData( FILE *res_fp, size_t len )
/*************************************************/
{
    std::array<char, 512> buff;

    for( ; len > buff.size(); len -= buff.size() ) {
        QRead( FP2POSIX( res_fp ), buff.data(), buff.size(), wlink().FmtData.resource );
        WriteLoad( buff.data(), buff.size() );
    }
    QRead( FP2POSIX( res_fp ), buff.data(), len, wlink().FmtData.resource );
    WriteLoad( buff.data(), len );
}

static void WriteOS2Resources( FILE *res_fp, WResDir inRes, ResTable *outRes )
/****************************************************************************/
{
    const int           shift_count = wlink().FmtData.u.os2fam.segment_shift;
    const int           align = 1 << shift_count;
    int                 outRes_off;
    WResDirWindow       wind;
    FullTypeRecord      *exe_type;
    WResResInfo         *res;
    WResLangInfo        *lang;

    if( inRes == nullptr )
        return;
    outRes_off = NullAlign( align ) >> shift_count;
    /* walk through the WRes directory */
    exe_type = nullptr;
    wind = WResFirstResource( inRes );
    while( !WResIsEmptyWindow( wind ) ) {
        lang = WResGetLangInfo( wind );

        if( WResIsFirstResOfType( wind ) ) {
            exe_type = findExeTypeRecord( outRes, WResGetTypeInfo( wind ) );
        }
        res = WResGetResInfo( wind );
        addExeResRecord( outRes, exe_type, &(res->ResName),
                        lang->MemoryFlags, outRes_off,
                        (lang->Length + align - 1) >> shift_count );
        QSeek( FP2POSIX( res_fp ), lang->Offset, wlink().FmtData.resource );
        CopyResData( res_fp, lang->Length );
        NullAlign( align );
        outRes_off += (lang->Length + align - 1) >> shift_count;

        wind = WResNextResource( wind, inRes );
    }
    NullAlign( align );
}


static unsigned long WriteTabList( obj_name_list *val, unsigned long *pcount, bool upper )
/****************************************************************************************/
{
    obj_name_list       *node;
    unsigned long       off;
    unsigned long       count;

    count = 0;
    off = 0;
    for( node = val; node != nullptr; node = node->next ) {
        off += WriteLoadU8Name( node->name.u.ptr, node->len, upper );
        ++count;
    }
    *pcount = count;
    return( off );
}

unsigned long ImportProcTable( unsigned long *count )
/***************************************************/
{
    return( WriteTabList( wlink().FmtData.u.os2fam.imp_tab_list, count, (wlink().LinkFlags & LF_CASE_FLAG) == 0 ) );
}

unsigned long ImportModTable( unsigned long *count )
/**************************************************/
{
    return( WriteTabList( wlink().FmtData.u.os2fam.mod_ref_list, count, false ) );
}

static unsigned long ImportNameTable( void )
/******************************************/
{
    unsigned long count;
    unsigned long size;

    PadLoad( 1 );
    size = 1;
    size += ImportProcTable( &count );
    size += ImportModTable( &count );
    return( size );
}

static unsigned long ModRefTable( void )
/***************************************
 * count total number of groups
 */
{
    obj_name_list       *node;
    obj_name_list       *inode;
    unsigned long       nodenum;
    unsigned long       off;

    if( wlink().FmtData.type & MK_NE ) {
        off = 1;
        for( inode = wlink().FmtData.u.os2fam.imp_tab_list; inode != nullptr; inode = inode->next ) {
            off += inode->len + 1;
        }
    } else {
        off = 0;
    }
    nodenum = 0;
    for( node = wlink().FmtData.u.os2fam.mod_ref_list; node != nullptr; node = node->next ) {
        WriteLoadU16( off );
        off += node->len + 1;
        nodenum++;
    }
    return( nodenum );
}

unsigned long ResNonResNameTable( bool dores )
/*********************************************
 * NOTE: this routine assumes INTEL byte ordering (in the use of namelen)
 */
{
    entry_export    *exp;
    unsigned long   size;
    const char      *name;
    size_t          len;
    size_t          i;
    std::string     external_name;

    size = 0;
    if( dores ) {
        /*
         * resident names table
         * get first item - module name
         */
        if( wlink().FmtData.u.os2fam.module_name != nullptr ) {
            name = wlink().FmtData.u.os2fam.module_name;
            len = strlen( name );
        } else {
            name = GetBaseName( wlink().Root->outfile->fname, 0, &len );
            if( wlink().FmtData.type & (MK_OS2_LE | MK_OS2_LX) ) {
                /*
                 * use base name + extension
                 */
                len = strlen( name );
            }
        }
        /*
         * for 16-bit NE and VxD format module name is truncated to 8 characters
         */
        if( (wlink().FmtData.type & MK_NE )
          && len > 8 ) {
            len = 8;
        } else if( wlink().FmtData.type & MK_WIN_VXD ) {
            /*
             * for VxD format module name is always 8 characters
             * if length > 8 then it is truncated to 8 characters
             * if length < 8 then it is expanded up to 8 characters
             *      by space character ( )
             */
            if( len < 8 ) {
                external_name.assign( 8, ' ' );
                for( i = 0; i < len; i++ ) {
                    external_name[i] = name[i];
                }
                name = external_name.c_str();
            }
            len = 8;
        }
    } else {
        /*
         * non-resident names table
         * get first idem - module description
         */
        if( wlink().FmtData.description != nullptr ) {
            name = wlink().FmtData.description;
        } else if( wlink().FmtData.type & MK_NE ) {
            name = wlink().Root->outfile->fname;
        } else {
            name = "";
        }
        len = strlen( name );
    }
    /*
     * resident name table - always write module name upper-cased
     * non-resident name table - write module description if not blank
     */
    if( dores
      || len > 0 ) {
        size += WriteLoadU8Name( name, len, dores );
        WriteLoadU16( 0 );  /* first item in table - ordinal 0 */
        size += 2;
    }
    for( exp = wlink().FmtData.u.os2fam.exports; exp != nullptr; exp = exp->next ) {
        if( !exp->isexported )
            continue;
        if( exp->isanonymous )
            continue;
        if( exp->isresident && !dores )
            continue;
        if( !exp->isresident && dores )
            continue;
        /*
         * get external name for export
         */
        name = exp->name.u.ptr;
        len = strlen( name );
        if( len > 255 )
            len = 255;
        external_name.assign( name, len );
        if( (wlink().LinkFlags & LF_CASE_FLAG) == 0 ) {
            for( i = 0; i < len; i++ ) {
                external_name[i] = static_cast<char>( toupper( static_cast<unsigned char>( external_name[i] ) ) );
            }
        }
        /*
         * write external name to name table
         */
        size += WriteLoadU8Name( external_name.c_str(), len, false );
        WriteLoadU16( exp->ordinal );
        size += 2;
        if( !exp->isprivate ) {
            if( exp->impname != nullptr ) {
                AddImpLibEntry( exp->impname, external_name.c_str(), 0, true );
            } else {
                AddImpLibEntry( exp->sym->name.u.ptr, nullptr, exp->ordinal, false );
            }
        }
    }
    if( size > 0 ) {
        WriteLoadU8( 0 ); /* name table terminating zero byte */
        ++size;
    }
    return( size );
}

/*
 * NOTE: The routine WriteFlatEntryTable in LOADFLAT.C is very similar to this
 *       one, however there are a enough differences to preclude the use
 *       of one routine to write both tables. Therefore any logic bugs that
 *       occur in this routine will likely have to be fixed in the other
 *       one as well.
 */
static unsigned long WriteEntryTable( void )
/*******************************************
 * Write the entry table to the file
 */
{
    entry_export    *start;
    entry_export    *place;
    entry_export    *prev;
    ordinal_t       prevord;
    unsigned long   size;
    unsigned        gap;
    unsigned        entries;
    bundle_prefix   prefix;
    union {
        fixed_record    f;
        movable_record  m;
    }               bundle_item;

    size = 0;
    start = wlink().FmtData.u.os2fam.exports;
    if( start != nullptr ) {
        prevord = 0;
        for( place = start; place != nullptr; ) {
            gap = place->ordinal - prevord;
            if( gap > 1 ) {  // fill in gaps in ordinals.
                gap--;       // fix 'off by 1' problem.
                prefix.number = 0xFF;
                prefix.type = 0x00;   // Null bundles.
                for( ; gap > 0xFF; gap -= 0xFF ) {
                    WriteLoad( &prefix, sizeof( bundle_prefix ) );
                    size += sizeof( bundle_prefix );
                }
                prefix.number = static_cast<unsigned_8>( gap );
                WriteLoad( &prefix, sizeof( bundle_prefix ) );
                size += sizeof( bundle_prefix );
            }
            // now get a bundle of ordinals.
            entries = 1;
            prev = start = place;
            for( place = place->next; place != nullptr; place = place->next ) {
                if( entries >= 0xff )
                    break;
                if( start->ismovable ) {
                    if( !place->ismovable ) {
                        break;
                    }
                } else {
                    if( place->addr.seg != start->addr.seg ) {
                        break;
                    }
                }
                if( place->ordinal - prev->ordinal > 1 ) {
                    break;    // ordinal can't be put in this bundle.
                }
                entries++;
                prev = place;
            }
            if( start->ismovable ) {
                prefix.type = MOVABLE_ENTRY_PNT;
                bundle_item.m.reserved = 0x3fcd; /* int 0x3f */
                size += entries * sizeof( movable_record );
            } else {
                prefix.type = start->addr.seg;     // fixed segment records.
                size += entries * sizeof( fixed_record );
            }
            prevord = prev->ordinal;
            prefix.number = static_cast<unsigned_8>( entries );
            WriteLoad( &prefix, sizeof( bundle_prefix ) );
            size += sizeof( bundle_prefix );
            for( ; entries > 0; --entries ) {
                bundle_item.f.info = (start->iopl_words << IOPL_WORD_SHIFT);
                if( start->isexported ) {
                    bundle_item.f.info |= ENTRY_EXPORTED;
                    // (a constant, segment 0xFE, uses no data segment: not marked shared)
                    if( (wlink().FmtData.u.os2fam.flags & SHARABLE_DGROUP) && start->addr.seg != 0xFE && !start->isnodata ) {
                        bundle_item.f.info |= ENTRY_SHARED;
                    }
                }
                if( start->ismovable ) {
                    if( start->addr.seg > 0xFF ) {
                        LnkMsg( WRN+MSG_BAD_MOVABLE_SEG_NUM, nullptr );
                    }
                    bundle_item.m.entrynum = start->addr.seg;
                    bundle_item.m.entry = start->addr.off;
                    WriteLoad( &bundle_item, sizeof( movable_record ) );
                } else {
                    bundle_item.f.entry = start->addr.off;
                    WriteLoad( &bundle_item, sizeof( fixed_record ) );
                }
                start = start->next;
            }
        }
    }
    PadLoad( 2 );
    return( size + 2 );
}

void SetOS2SegFlags( void )
/*************************/
{
    SetSegFlags( wlink().FmtData.u.os2fam.seg_flags );
    wlink().FmtData.u.os2fam.seg_flags = nullptr;
}

constexpr auto DEF_SEG_ON = SEG_PURE|SEG_READ_ONLY|SEG_CONFORMING|SEG_MOVABLE|SEG_DISCARD|SEG_RESIDENT|SEG_CONTIGUOUS|SEG_NOPAGE;
constexpr auto DEF_SEG_OFF = SEG_PRELOAD|SEG_INVALID;

static void CheckGrpFlags( void *_leader )
/****************************************/
{
    seg_leader      *leader = static_cast<seg_leader *>( _leader );
    seg_flags_type  sflags;

    sflags = leader->segflags;
    // if any of these flags are on, turn it on for the entire group.
    leader->group->segflags |= sflags & DEF_SEG_OFF;
    // if any of these flags off, make sure they are off in the group.
    leader->group->segflags &= (sflags & DEF_SEG_ON) | ~DEF_SEG_ON;
    if( GET_SEG_PMODE_DPL( sflags ) == SEG_PMODE_DPL_2 ) {
        /* if any are level 2 then all have to be. */
        SET_SEG_PMODE_DPL( leader->group->segflags, SEG_PMODE_DPL_2 );
    }
}

void SetOS2GroupFlags( void )
/****************************
 * This goes through the groups, setting the flag word to be compatible with
 * the flag words that are specified in the segments.
 */
{
    group_entry     *group;

    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( group->totalsize == 0 )
            continue;   // DANGER DANGER DANGER <--!!!
        group->segflags |= DEF_SEG_ON;
        Ring2Walk( group->leaders, CheckGrpFlags );
        /*
         * for some insane reason, level 2 segments must be marked as movable
         */
        if( GET_SEG_PMODE_DPL( group->segflags ) == SEG_PMODE_DPL_2 ) {
            group->segflags |= SEG_MOVABLE;
        }
    }
}

void ChkOS2Exports( void )
/*************************
 * NOTE: there is a continue in this loop!
 */
{
    symbol          *sym;
    entry_export    *exp;
    group_entry     *group;
    unsigned        num_entries;

    num_entries = 0;
    // NOTE: there is a continue in this loop!
    for( exp = wlink().FmtData.u.os2fam.exports; exp != nullptr; exp = exp->next ) {
        num_entries++;
        sym = exp->sym;
        if( IS_SYM_ALIAS( sym ) ) {
            sym = UnaliasSym( ST_FIND, sym );
            if( sym == nullptr
              || (sym->info & SYM_DEFINED) == 0 ) {
                LnkMsg( ERR+MSG_EXP_SYM_NOT_FOUND, "s", exp->sym->name.u.ptr );
                continue;               // <----- DANGER weird control flow!
            } else if( exp->sym->info & SYM_WAS_LAZY ) {
                LnkMsg( WRN+MSG_EXP_SYM_NOT_FOUND, "s", exp->sym->name.u.ptr );
            }
            // Keep the import name. If an alias is exported, we want the
            // alias name in the import lib, not the substitute name
            if( exp->impname == nullptr ) {
                exp->impname = MemStrdupSafe( exp->sym->name.u.ptr );
            }

            exp->sym = sym;
        }
        if( (sym->info & SYM_DEFINED) == 0 ) {
            LnkMsg( ERR+MSG_EXP_SYM_NOT_FOUND, "s", sym->name );
        } else {
            exp->addr = sym->addr;
            if( sym->p.seg == nullptr
              || IS_SYM_IMPORTED( sym ) ) {
                if( wlink().FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD) ) {
                    // MN: Create a forwarder - add a special flag?
                    // Currently WriteFlatEntryTable() in loadflat.c will
                    // recognize a forwarder by segment == 0xFFFF
                } else if( (wlink().FmtData.type & MK_NE)
                  && !IS_SYM_IMPORTED( sym ) ) {
                    // An absolute symbol (an EQU made public) is exported as a
                    // constant: the NE entry table's segment number 0xFE. This
                    // is how KERNEL exports __AHINCR, __0040H and the like.
                    exp->addr.seg = 0xFE;
                    exp->addr.off = sym->addr.off;
                } else {
                    LnkMsg( ERR+MSG_CANT_EXPORT_ABSOLUTE, "S", sym );
                }
            } else {
                group = sym->p.seg->u.leader->group;
                if( wlink().FmtData.type & (MK_OS2_FLAT | MK_WIN_VXD) ) {
                    exp->addr.off -= group->addr.off;
                    if( GET_SEG_PMODE_DPL( group->segflags ) == SEG_PMODE_DPL_2 ) {
                        exp->isiopl = true; // Conforming or not doesn't matter!
                        if( exp->addr.off > 65535 ) {
                            // Call gates are 16-bit only
                            LnkMsg( ERR+LOC+MSG_BAD_TARG_OFF, "a", &exp->addr );
                        }
                    }
                } else if( wlink().FmtData.type & MK_PE ) {
                    exp->addr.off += (group->linear - group->addr.off);
                }
                if( group->segflags & SEG_MOVABLE ) {
                    exp->ismovable = true;
                }
            }
        }
    }   // NOTE: there is a continue in this loop!
    AssignOrdinals();    /* make sure all exports have ordinals */
    if( (wlink().FmtData.type & MK_WIN_VXD)
      && ( num_entries != 1 ) ) {
        LnkMsg( FTL+MSG_VXD_INCORRECT_EXPORT, nullptr );
    }
}

void PhoneyStack( void )
/***********************
 * signal that we will be making a fake stack later on.
 */
{
    wlink().FmtData.u.os2fam.flags |= PHONEY_STACK_FLAG;
}

static FILE *InitNEResources( WResDir *inRes, ResTable *outRes )
/**************************************************************/
{
    WResDir     dir;
    FILE        *res_fp;
    bool        dup_discarded;

    dir = nullptr;
    res_fp = nullptr;
    if( wlink().FmtData.resource != nullptr ) {
        res_fp = POSIX2FP( QOpenR( wlink().FmtData.resource ) );
        if( res_fp != nullptr ) {
            if( WResReadDir( res_fp, &dir, &dup_discarded ) ) {
                LnkMsg( WRN+MSG_PROBLEM_IN_RESOURCE, nullptr );
            } else {
                outRes->Dir.NumTypes = dir->NumTypes;
                outRes->Dir.NumResources = dir->NumResources;
                outRes->Dir.TableSize = outRes->Dir.NumTypes * sizeof( resource_type_record ) +
                                        outRes->Dir.NumResources * sizeof( resource_record ) +
                                        2 * sizeof( unsigned_16 );
                /* the 2 * unsigned_16 are the resource shift count and the type 0 record */
                outRes->Dir.Head = nullptr;
                outRes->Dir.Tail = nullptr;
                StringIDNamesBlockBuild( &outRes->Str, dir, false );
            }
        }
    }
    *inRes = dir;
    return( res_fp );
}

static void FiniNEResources( FILE *res_fp, WResDir inRes, ResTable *outRes )
/**************************************************************************/
{
    if( inRes != nullptr ) {
        if( outRes->Str.StringBlock != nullptr ) {
            MemFree( outRes->Str.StringBlock );
            outRes->Str.StringBlock = nullptr;
        }
        if( outRes->Str.StringList != nullptr ) {
            MemFree( outRes->Str.StringList );
            outRes->Str.StringList = nullptr;
        }
        WResFreeDir( inRes );
    }
    if( res_fp != nullptr ) {
        QClose( FP2POSIX( res_fp ), wlink().FmtData.resource );
    }
}


static unsigned_32 ComputeResourceSize( WResDir dir )
/***************************************************/
{
    unsigned_32     length;
    WResDirWindow   wind;
    WResLangInfo    *res;

    if( dir == nullptr ) {
        return( 0 );
    }
    length = 0;
    wind = WResFirstResource( dir );
    while( !WResIsEmptyWindow( wind ) ) {
        res = WResGetLangInfo( wind );
        length += res->Length;
        wind = WResNextResource( wind, dir );
    }
    return( length );
}

constexpr auto MAX_DGROUP_SIZE = _64K;

void FiniOS2LoadFile( void )
/***************************
 * terminate writing of load file
 */
{
    os2_exe_header      exe_head;
    unsigned long       temp;
    unsigned_16         adseg;
    group_entry         *group;
    unsigned_32         stub_len;
    unsigned_32         dgroup_size;
    unsigned_32         dgroup_total;
    unsigned long       size;
    entry_export        *exp;
    unsigned long       imageguess;     // estimated length of the image
    unsigned            pad_len;
    WResDir             inRes;          // Directory of resources to read
    FILE                *res_fp;        // Handle for resources file
    ResTable            outRes;         // Resources to go out

    stub_len = WriteStubFile( STUB_ALIGN );
    temp = sizeof( os2_exe_header );
    exe_head.segment_off = temp;
    SeekLoad( stub_len + sizeof( os2_exe_header ) );
    adseg = 0;
    exe_head.segments = 0;
    dgroup_size = 0;
    imageguess = 0;
    if( wlink().DataGroup != nullptr ) {
        adseg = wlink().DataGroup->addr.seg;
        if( wlink().DataGroup->segflags & SEG_PURE ) {
            wlink().FmtData.u.os2fam.flags |= SHARABLE_DGROUP;
        }
        if( wlink().StackSegPtr != nullptr ) {
            if( wlink().DataGroup->totalsize - wlink().DataGroup->size < wlink().StackSize ) {
                wlink().StackSize = wlink().DataGroup->totalsize - wlink().DataGroup->size;
                wlink().DataGroup->totalsize = wlink().DataGroup->size;
            } else {
                wlink().DataGroup->totalsize -= wlink().StackSize;
            }
        }
        dgroup_size = wlink().DataGroup->totalsize;
    }
    for( group = wlink().Groups; group != nullptr; group = group->next ) {
        if( group->totalsize == 0 )
            continue;   // DANGER DANGER DANGER <--!!!
        imageguess += group->size;
        exe_head.segments++;
    }
    temp += exe_head.segments * sizeof( segment_record );
    res_fp = InitNEResources( &inRes, &outRes );
    exe_head.resource_off = temp;
    if( inRes != nullptr ) {
        exe_head.resource = outRes.Dir.NumResources;
        temp += outRes.Dir.TableSize;
        temp += outRes.Str.StringBlockSize;
    } else {
        exe_head.resource = 0;
    }
    exe_head.resident_off = temp;
    SeekLoad( stub_len + temp );
    /*
     * do resident names table (true value)
     */
    temp += ResNonResNameTable( true );
    exe_head.module_off = temp;
    exe_head.modrefs = ModRefTable();
    temp += exe_head.modrefs * sizeof( unsigned_16 );
    exe_head.import_off = temp;
    temp += ImportNameTable();
    exe_head.entry_off = temp;
    size = WriteEntryTable();
    exe_head.entry_size = size;
    temp += size;
    temp += stub_len;
    exe_head.nonres_off = temp;
    /*
     * do non-resident names table (false value)
     */
    exe_head.nonres_size = ResNonResNameTable( false );
    temp += exe_head.nonres_size;
    /*
     * if no segment shift specified, figure out the best one, assuming that
     * the maximum padding will happen every time.
     */
    if( wlink().FmtData.u.os2fam.segment_shift == 0 ) {
        imageguess += temp + static_cast<unsigned long>( wlink().Root->relocs ) * sizeof( os2_reloc_item )
                     + stub_len + exe_head.segments * 3;
        pad_len = log2_16( (imageguess >> 16) << 1 );
        imageguess += ((1 << pad_len) - 1) * exe_head.segments;
        imageguess += ComputeResourceSize( inRes ); // inRes may be NULL
        wlink().FmtData.u.os2fam.segment_shift = log2_16( (imageguess >> 16) << 1 );
        if( wlink().FmtData.u.os2fam.segment_shift == 0 ) {
            wlink().FmtData.u.os2fam.segment_shift = 1;     // since microsoft thinks 0 == 9
        }
    }
    exe_head.gangstart = NullAlign( 1 << wlink().FmtData.u.os2fam.segment_shift ) >> wlink().FmtData.u.os2fam.segment_shift;
    WriteOS2Data( stub_len, &exe_head );
    WriteOS2Resources( res_fp, inRes, &outRes );
    exe_head.gangstart = 0;
    exe_head.ganglength = 0;
    if( inRes != nullptr ) {
        SeekLoad( exe_head.resource_off + stub_len );
        WriteResTable( &outRes );
        FreeResTable( &outRes );
    }
    SeekEndLoad( 0 );
    FiniNEResources( res_fp, inRes, &outRes );
    DBIWrite();
    exe_head.signature = EXESIGN_NE;
    exe_head.version = 0x0105;          /* version 5.1 */
    exe_head.chk_sum = 0L;
    exe_head.info = 0;
    if( wlink().FmtData.type & MK_WIN_NE ) {
        exe_head.target = TARGET_WINDOWS;
    } else {
        exe_head.target = TARGET_OS2;
    }
    if( wlink().FmtData.u.os2fam.flags & PROTMODE_ONLY ) {
        exe_head.info |= OS2_PROT_MODE_ONLY;
    }
    if( wlink().FmtData.u.os2fam.flags & SINGLE_AUTO_DATA ) {
        exe_head.info |= OS2_SINGLE_AUTO;
    } else if( wlink().FmtData.u.os2fam.flags & MULTIPLE_AUTO_DATA ){
        exe_head.info |= OS2_MULT_AUTO;
    } else {
        adseg = 0;    // no automatic data segment.
    }
    if( wlink().FmtData.u.os2fam.flags & NO_APP_TYPE ) {
        /* (CodeBreaker: as the original's header has it) */
    } else if( (wlink().FmtData.type & MK_WIN_NE)
      || (wlink().FmtData.u.os2fam.flags & PM_APPLICATION) ) {
        exe_head.info |= OS2_PM_APP;
    } else if( wlink().FmtData.u.os2fam.flags & PM_NOT_COMPATIBLE ) {
        exe_head.info |= OS2_NOT_PM_COMPATIBLE;
    } else {
        exe_head.info |= OS2_PM_COMPATIBLE;
    }
    if( wlink().FmtData.u.os2fam.is_private_dll ) {
        exe_head.info |= WIN_PRIVATE_DLL;
    }
    if( wlink().FmtData.dll ) {
        exe_head.info |= OS2_IS_DLL;
        if( wlink().FmtData.u.os2fam.flags & INIT_INSTANCE_FLAG ) {
            exe_head.info |= OS2_INIT_INSTANCE;
        }
    }
    exe_head.adsegnum = adseg;
    exe_head.heap = wlink().FmtData.u.os2fam.heapsize;
    /*
     * the microsoft linker for windows will generate a stack even if no stack
     * segment has been explicitly specified. (as long as the user specifies a stack
     * size). Since microsoft's windows libraries rely on this, we have to mimic
     * it.
     */
    if( wlink().FmtData.u.os2fam.flags & PHONEY_STACK_FLAG ) {
        exe_head.SP = 0;
        exe_head.stacknum = adseg;
    } else {
        exe_head.SP = 0;
        //exe_head.SP = StackAddr.off;
        if( !IS_ADDR_UNDEFINED( wlink().StackAddr ) ) {
            exe_head.stacknum = wlink().StackAddr.seg;
        } else {
            exe_head.stacknum = 0;
        }
    }
    exe_head.stack = wlink().StackSize;
    if( dgroup_size + exe_head.heap > MAX_DGROUP_SIZE ) {
        LnkMsg( WRN+MSG_HEAP_TOO_BIG, nullptr );
        if( dgroup_size < MAX_DGROUP_SIZE ) {
            exe_head.heap = MAX_DGROUP_SIZE - dgroup_size;
        }
    }
    exe_head.IP = wlink().StartInfo.addr.off;
    if( wlink().StartInfo.type == START_UNDEFED ) {
        exe_head.entrynum = 0;
    } else {
        exe_head.entrynum = wlink().StartInfo.addr.seg;
    }
    exe_head.align = wlink().FmtData.u.os2fam.segment_shift;
    exe_head.movable = 0;
    for( exp = wlink().FmtData.u.os2fam.exports; exp != nullptr; exp = exp->next ) {
        if( exp->ismovable ) {
            exe_head.movable++;
        }
    }
    exe_head.otherflags = 0;
    if( wlink().FmtData.u.os2fam.flags & LONG_FILENAMES ) {
        exe_head.otherflags = OS2_LONG_FILE_NAMES;
    }
    if( wlink().FmtData.u.os2fam.flags & PROPORTIONAL_FONT ) {
        exe_head.otherflags |= WIN_CLEAN_MEMORY | WIN_PROPORTIONAL_FONT;
    } else if( wlink().FmtData.u.os2fam.flags & CLEAN_MEMORY ) {
        exe_head.otherflags |= WIN_CLEAN_MEMORY;
    }
    if( exe_head.ganglength ) {
        exe_head.otherflags |= WIN_GANGLOAD_PRESENT;
    }
    exe_head.swaparea = 0;
    if( wlink().FmtData.type & MK_WIN_NE ) {
        if( wlink().FmtData.ver_specified ) {
            exe_head.expver = (wlink().FmtData.major << 8) | (wlink().FmtData.minor & 0xFF);
        } else {
            exe_head.expver = 0x300;
        }
    } else {
        exe_head.expver = 0;
    }
    /* Check default data segment size. On OS/2, data segment + heap + stack
     * may be up to 64K. On Windows, the max is about 0xfffe. Windows may also
     * tweak the default heap/stack size so this check isn't bulletproof.
     */
    dgroup_total = dgroup_size + exe_head.stack + exe_head.heap;
    if( wlink().FmtData.type & MK_WIN_NE ) {
        if( dgroup_total > (MAX_DGROUP_SIZE - 3) ) {
            LnkMsg( FTL+MSG_DEFDATA_TOO_BIG, "l",
                    dgroup_total - MAX_DGROUP_SIZE + 3 );
        }
    } else {
        if( dgroup_total > MAX_DGROUP_SIZE ) {
            LnkMsg( FTL+MSG_DEFDATA_TOO_BIG, "l",
                    dgroup_total - MAX_DGROUP_SIZE );
        }
    }

    if( wlink().LinkState & LS_LINK_ERROR ) {
        exe_head.info |= OS2_LINK_ERROR;
    }
    SeekLoad( stub_len );
    WriteLoad( &exe_head, sizeof( os2_exe_header ) );
}

void FreeImpNameTab( void )
/*************************/
{
    wlink().FmtData.u.os2fam.mod_ref_list = nullptr;  /* these are permalloc'd */
    wlink().FmtData.u.os2fam.imp_tab_list = nullptr;
}

unsigned_32 WriteStubFile( unsigned_32 stub_align )
/*************************************************/
{
    unsigned_32     stub_len;
    f_handle        the_file;
    dos_exe_header  dosheader;
    unsigned_32     read_len;
    size_t          amount;
    unsigned_32     reloc_size;
    unsigned_16     num_relocs;
    unsigned_32     the_reloc;
    unsigned_32     code_start;
    std::array<char, PATH_MAX> fullname;

    if( wlink().FmtData.u.os2fam.no_stub ) {
        stub_len = 0;
    } else if( wlink().FmtData.u.os2fam.stub_file_name == nullptr ) {
        stub_len = WriteDOSDefStub( stub_align );
    } else if( stricmp( wlink().FmtData.u.os2fam.stub_file_name, wlink().Root->outfile->fname ) == 0 ) {
        LnkMsg( ERR+MSG_STUB_SAME_AS_LOAD, nullptr );
        stub_len = WriteDOSDefStub( stub_align );
    } else {
        the_file = FindPath( wlink().FmtData.u.os2fam.stub_file_name, fullname.data() );
        if( the_file == NIL_FHANDLE ) {
            LnkMsg( WRN+MSG_CANT_OPEN_NO_REASON, "s", wlink().FmtData.u.os2fam.stub_file_name );
            return( WriteDOSDefStub( stub_align ) );   // NOTE: <== a return here.
        }
        MemFree( wlink().FmtData.u.os2fam.stub_file_name );
        wlink().FmtData.u.os2fam.stub_file_name = MemStrdupSafe( fullname.data() );
        QRead( the_file, &dosheader, sizeof( dos_exe_header ), wlink().FmtData.u.os2fam.stub_file_name );
        if( dosheader.signature != EXESIGN_DOS ) {
            LnkMsg( ERR + MSG_INV_STUB_FILE, nullptr );
            stub_len = WriteDOSDefStub( stub_align );
        } else if( wlink().FmtData.u.os2fam.stub_verbatim ) {
            /* CodeBreaker: every byte as it is; only the NE header's offset filled in */
            read_len = dosheader.file_size * 512ul - (-dosheader.mod_size & 0x1ff);
            QSeek( the_file, 0, wlink().FmtData.u.os2fam.stub_file_name );
            for( stub_len = 0; stub_len < read_len; stub_len += amount ) {
                amount = ( read_len - stub_len < wlink().TokSize ) ? read_len - stub_len : wlink().TokSize;
                QRead( the_file, wlink().TokBuff, amount, wlink().FmtData.u.os2fam.stub_file_name );
                WriteLoad( wlink().TokBuff, amount );
            }
            stub_len = NullAlign( stub_align );
            SeekLoad( NE_HEADER_OFFSET );
            WriteLoadU32( stub_len );
            SeekLoad( stub_len );
        } else {
            QSeek( the_file, dosheader.reloc_offset, wlink().FmtData.u.os2fam.stub_file_name );
            dosheader.reloc_offset = 0x40;
            code_start = dosheader.hdr_size * 16ul;
            read_len = dosheader.file_size * 512ul - (-dosheader.mod_size & 0x1ff) - code_start;
            // make sure reloc_size is a multiple of 16.
            reloc_size = __ROUND_UP_SIZE_PARA( dosheader.num_relocs * 4ul );
            dosheader.hdr_size = 4 + reloc_size / 16;
            stub_len = read_len + dosheader.hdr_size * 16ul;
            dosheader.file_size = ( stub_len + 511 ) / 512;  // round up.
            dosheader.mod_size = stub_len % 512;
            WriteLoad( &dosheader, sizeof( dos_exe_header ) );
            PadLoad( NE_HEADER_OFFSET - sizeof( dos_exe_header ) );
            stub_len = __ROUND_UP_SIZE( stub_len, stub_align );
            WriteLoadU32( stub_len );
            for( num_relocs = dosheader.num_relocs; num_relocs > 0; num_relocs-- ) {
                QRead( the_file, &the_reloc, sizeof( unsigned_32 ), wlink().FmtData.u.os2fam.stub_file_name );
                WriteLoadU32( the_reloc );
                reloc_size -= sizeof( unsigned_32 );
            }
            if( reloc_size != 0 ) {    // need padding
                PadLoad( reloc_size );
            }
            QSeek( the_file, code_start, wlink().FmtData.u.os2fam.stub_file_name );
            for( ; read_len > 0; read_len -= amount ) {
                if( read_len < wlink().TokSize ) {
                    amount = read_len;
                } else {
                    amount = wlink().TokSize;
                }
                QRead( the_file, wlink().TokBuff, amount, wlink().FmtData.u.os2fam.stub_file_name );
                WriteLoad( wlink().TokBuff, amount );
            }
            stub_len = NullAlign( stub_align );
        }
        QClose( the_file, wlink().FmtData.u.os2fam.stub_file_name );
    }
    return( stub_len );
}

#endif
