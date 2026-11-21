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
* Description:  Search library files.
*
****************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <string>
#include "walloca.h"
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "pcobj.h"
#include "build/x86link/symbols/library.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/objects/procfile.h"
#include "ar.h"
#include "omfhash.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


static constexpr unsigned PAGES_IN_CACHE = 0x40U;

struct omf_dict_entry {
    void                **cache;        /* for extra memory store of dictionary */
    long                start;          /* recno of start of dictionary         */
    unsigned            pages;          /* number of pages in dictionary        */
    long                rec_length;     /* record alignment of obj recs         */
    unsigned_8          *buffer;
};

struct ar_dict_entry {
    unsigned_32         *filepostab;
    unsigned_16         *offsettab;
    char                **symbtab;
    unsigned_32         num_entries;
};

union dict_entry {
    omf_dict_entry      o;
    ar_dict_entry       a;
};

static void BadLibrary(Link& link,  file_list *file )
/***************************************/
{
    file->infile->status |= INSTAT_IOERR;
    MemFree( file->u.dict );
    file->u.dict = nullptr;
    Locator( file->infile->name.u.ptr, nullptr, 0 );
    LnkMsg(link,  ERR+MSG_LIB_FILE_ATTR, nullptr );
}

static int ReadOMFDict(Link& link,  file_list *file, unsigned_8 *header, bool makedict )
/**************************************************************************/
{
    omf_dict_entry  *omf_dict;
    unsigned        reclength;

    header += sizeof( unsigned_8 );
    reclength = MGET_LE_U16_UN( header ) + 3;
    if( makedict ) {
        if( file->u.dict == nullptr ) {
            file->u.dict = static_cast<dict_entry *>( MemAllocSafe(link,  sizeof( dict_entry ) ) );
        }
        omf_dict = &file->u.dict->o;
        omf_dict->cache = nullptr;
        header += sizeof( unsigned_16 );
        omf_dict->start = MGET_LE_U32_UN( header );
        header += sizeof( unsigned_32 );
        omf_dict->pages = MGET_LE_U16_UN( header );
        header += sizeof( unsigned_16 );
        if( omf_dict->start == 0
          || omf_dict->pages == 0
          || ( omf_dict->start + omf_dict->pages * DIC_REC_SIZE ) > file->infile->len ) {
            BadLibrary(link,  file );
            return( -1 );
        }
        omf_dict->rec_length = reclength;
    }
    return( reclength );
}

using index_type = unsigned;

namespace {
struct LibState {
    char    **d_symbtab = nullptr;      /* pointer to AR dictionary structures */
};
}

static int ARComp( const void *index1, const void *index2 )
/*********************************************************/
{
    char **d_symbtab = cb_wlink::RunState::part<LibState>().d_symbtab;

    return( (*CmpRtn)( d_symbtab[*static_cast<const index_type *>( index1 )], d_symbtab[*static_cast<const index_type *>( index2 )] ) );
}

static void SortARDict(Link& link,  ar_dict_entry *ar_dict )
/**********************************************/
{
    index_type      *index_tab;
    index_type      ix;
    index_type      ix_next;
    index_type      ix1;
    char            *symb_save;
    unsigned        offset_save;
    ar_dict_entry   d;

    if( ar_dict->num_entries < 2 )
        return;

    d = *ar_dict;
    /*
     * store the dictionary pointer into memory so we can fetch ar_dict in ARComp
     */
    cb_wlink::RunState::part<LibState>().d_symbtab = d.symbtab;
    /*
     * Create an index table that we will sort to match the
     * case-insensitive sort order that we want for our symbol names.
     */
    index_tab = static_cast<index_type *>( MemAllocSafe(link,  sizeof( index_type ) * d.num_entries ) );
    for( ix = 0; ix < d.num_entries; ix++ ) {
        index_tab[ix] = ix;
    }
    /*
     * Sort the index table using the corresponding symbol names
     * to determine the sort order (see ARComp() for more info).
     */
    qsort( index_tab, d.num_entries, sizeof( index_type ), ARComp );
    /*
     * Reorder the function name table (a vector of pointers to
     * symbol names) and the offset table (a vector of 16-bit offsets
     * into the file position table) (see ReadARDict() for info).
     */
    for( ix = 0; ix < d.num_entries; ++ix ) {
        /*
         * If this entry hasn't been corrected
         * then move out the entry that is present
         * so that we can correct it.
         */
        if( ix != index_tab[ix] ) {
            symb_save = d.symbtab[ix];
            if( d.offsettab == nullptr ) {
                offset_save = d.filepostab[ix];
            } else {
                offset_save = d.offsettab[ix];
            }
            /*
             * Correct all the entries in this sequence
             */
            for( ix1 = ix; (ix_next = index_tab[ix1]) != ix; ix1 = ix_next ) {
                d.symbtab[ix1] = d.symbtab[ix_next];
                if( d.offsettab == nullptr ) {
                    d.filepostab[ix1] = d.filepostab[ix_next];
                } else {
                    d.offsettab[ix1] = d.offsettab[ix_next];
                }
                index_tab[ix1] = ix1;
            }
            /*
             * Update this final entry in the sequence from the
             * values we set aside.
             */
            d.symbtab[ix1] = symb_save;
            if( d.offsettab == nullptr ) {
                d.filepostab[ix1] = offset_save;
            } else {
                d.offsettab[ix1] = offset_save;
            }
            index_tab[ix1] = ix1;
        }
    }
    MemFree( index_tab );
}

static void ReadARDictData(Link& link,  file_list *file, unsigned long *loc, unsigned size, int numdicts )
/********************************************************************************************/
{
    ar_dict_entry   *dict;
    char            *data;
    unsigned        num;
    unsigned        index;
    /*
     * a dictionary in an AR archive commonly starts with a header marked with '/ '.
     *
     * Be careful, more AR formats exists, GNU, BSD and COFF. Each handle dictionaries
     * a little bit different way.
     *
     * GNU (Linux) and BSD AR archives commonly have only one dictionary which is of the form:
     *
     * (all numbers in *big* endian format)
     * unsigned_32: number of entries (num)
     * num unsigned_32's: offset within AR file of the object file that the symbol name belongs too.
     * num zero terminated strings: the symbols themselves (unsorted).
     *
     * COFF AR archieves however use two dictionaries.
     * First one is the same as for GNU and BSD, but WLINK uses second one, more efficient dictionary:
     *
     * (all numbers in *little* endian format)
     * unsigned_32: number of files in object
     * nfiles unsigned_32's: offsets of the files within the archive
     * unsigned_32: number of entries (num)
     * num unsigned_16's: the file number that the symbol belongs to
     * num zero terminated strings: the symbols themselves (sorted case-sensitively).
     *
     * however WLINK needs to be able to parse both kinds.
     * (dict->offsettab == nullptr) means that we only know about an unsorted dictionary and
     * it will be sorted later.
     */
    dict = &file->u.dict->a;
    data = static_cast<char *>( CachePermRead(link,  file, *loc, size ) );
    if( numdicts == 1 ) {
        num = MGET_BE_U32_UN( data ); /* number of symbols */
        data += sizeof( unsigned_32 );
        dict->filepostab = reinterpret_cast<unsigned_32 *>( data );
        for( index = 0; index < num; index++ ) {
            dict->filepostab[index] = MGET_BE_U32_UN( data );
            data += sizeof( unsigned_32 );
        }
        dict->num_entries = num;
        dict->offsettab = nullptr;
        dict->symbtab = nullptr;
        if( num > 0 ) {
            dict->symbtab = static_cast<char **>( MemAllocSafe(link,  sizeof( char * ) * num ) );
        }
    } else /* if( numdicts == 2 ) */ {
        num = MGET_LE_U32_UN( data );    /* number of files */
        data += sizeof( unsigned_32 );
        dict->filepostab = reinterpret_cast<unsigned_32 *>( data ); /* first file off */
        for( index = 0; index < num; index++ ) {
            dict->filepostab[index] = MGET_LE_U32_UN( data );
            data += sizeof( unsigned_32 );
        }
        num = MGET_LE_U32_UN( data );    /* number of symbols */
        data += sizeof( unsigned_32 );
        dict->offsettab = reinterpret_cast<unsigned_16 *>( data );  /* first offset */
        for( index = 0; index < num; index++ ) {
            dict->offsettab[index] = MGET_LE_U16_UN( data );
            data += sizeof( unsigned_16 );
        }
        dict->num_entries = num;
        if( num > 0
          && dict->symbtab == nullptr ) {
            dict->symbtab = static_cast<char **>( MemAllocSafe(link,  sizeof( char * ) * num ) );
        }
    }
    for( index = 0; index < num; index++ ) {
        dict->symbtab[index] = data;
        data += strlen( data ) + 1;
    }
}

static void ReadARStringTable(Link& link,  file_list *file, unsigned long *loc, unsigned size )
/*********************************************************************************/
{
    char            *data;
    unsigned        i;

    if( file->strtab == nullptr
      && size > 0 ) {
        file->strtab = static_cast<char *>( CachePermRead(link,  file, *loc, size ) );
        data = file->strtab;
        for( i = 0; i < size; ++i ) {
            if( *data == '\n'
              || *data == '/'
              && *(data + 1) == '\n' )
                *data = '\0';
            ++data;
        }
    }
}

static bool ReadARDict(Link& link,  file_list *file, unsigned long *loc, bool makedict )
/**************************************************************************/
{
    const ar_header *ar_hdr;
    unsigned long   size;
    int             numdicts;

    numdicts = 0;
    if( makedict ) {
        if( file->u.dict == nullptr ) {
            file->u.dict = static_cast<dict_entry *>( MemAllocSafe(link,  sizeof( dict_entry ) ) );
        }
    }
    for( ;; ) {
        ar_hdr = static_cast<const ar_header *>( CacheRead(link,  file, *loc, sizeof( ar_header ) ) );
        size = GetARValue( ar_hdr->size, AR_SIZE_LEN );
        if( ar_hdr->name[0] == '/'
          && ar_hdr->name[1] == ' ' ) {
            ++numdicts;
            *loc += sizeof( ar_header );
            if( makedict )
                ReadARDictData(link,  file, loc, size, numdicts );
            *loc += __ROUND_UP_SIZE_EVEN( size );
        } else if( ar_hdr->name[0] == '/'
          && ar_hdr->name[1] == '/' ) {
            *loc += sizeof( ar_header );
            ReadARStringTable(link,  file, loc, size );
            *loc += __ROUND_UP_SIZE_EVEN( size );
        } else {
            break;         // found an actual object file
        }
    }
    if( makedict ) {
        if( numdicts == 0 ) {
            Locator( file->infile->name.u.ptr, nullptr, 0 );
            LnkMsg(link,  ERR+MSG_NO_DICT_FOUND, nullptr );
            MemFree( file->u.dict );
            file->u.dict = nullptr;
            return( false );
        }
        if( (link.LinkFlags & LF_CASE_FLAG) == 0
          || numdicts == 1 ) {
            SortARDict(link,  &file->u.dict->a );
        }
    }
    return( true );
}

int CheckLibraryType(Link& link,  file_list *file, unsigned long *loc, bool makedict )
/************************************************************************/
{
    unsigned_8      *header;
    int             reclength;

    reclength = 0;
    header = static_cast<unsigned_8 *>( CacheRead(link,  file, *loc, sizeof( omf_lib_header ) ) );
    if( header[0] == 0xf0
      && header[1] == 0x01 ) {
        /*
         * COFF object for PPC
         */
    } else if( header[0] == LIB_HEADER_REC ) {   // reading from a library
        file->flags_file |= STAT_OMF_LIB;
        reclength = ReadOMFDict(link,  file, header, makedict );
        if( reclength < 0 ) {
            return( -1 );
        }
        *loc += __ROUND_UP_SIZE( sizeof( omf_lib_header ), reclength );
    } else if( memcmp( header, AR_IDENT, AR_IDENT_LEN ) == 0 ) {
        file->flags_file |= STAT_AR_LIB;
        reclength = 2;
        *loc += AR_IDENT_LEN;
        if( !ReadARDict(link,  file, loc, makedict ) ) {
            return( -1 );
        }
    }
    return( reclength );
}

static void FreeDictCache( void **cache, unsigned buckets )
/*********************************************************/
{
    while( buckets-- > 0 ) {
        MemFree( cache[buckets] );
    }
    MemFree( cache );
}

static void **AllocDict(Link& link,  unsigned num_buckets, unsigned residue )
/****************************************************************
 * allocate a chunk of dict memory, down from the top
 */
{
    void            **cache;
    unsigned        bucket;

    cache = static_cast<void **>( MemAlloc(link,  sizeof( void * ) * ( num_buckets + 1 ) ) );
    if( cache == nullptr )
        return( nullptr );
    for( bucket = 0; bucket < num_buckets; ++bucket ) {
        cache[bucket] = MemAlloc(link,  DIC_REC_SIZE * PAGES_IN_CACHE );
        if( cache[bucket] == nullptr ) {
            FreeDictCache( cache, bucket );
            return( nullptr );
        }
    }

    if( residue != 0 ) {
        cache[bucket] = MemAlloc(link,  residue * DIC_REC_SIZE );
        if( cache[bucket] == nullptr ) {
            FreeDictCache( cache, bucket );
            return( nullptr );
        }
    } else {
       cache[bucket] = nullptr;
    }
    return( cache );
}

static void SetDict(Link& link,  file_list *file, unsigned dict_page )
/*********************************************************
 * set lib->buffer to the dict_page th page in lib 's dictionary
 */
{
    unsigned        pages;
    unsigned        num_buckets;
    unsigned        residue;
    unsigned        bucket;
    long            off;
    long            dictoff;
    omf_dict_entry  *dict;

    dict = &file->u.dict->o;
    if( dict->cache == nullptr ) {
        pages = dict->pages;
        num_buckets = pages / PAGES_IN_CACHE;
        residue = pages - num_buckets * PAGES_IN_CACHE;
        dict->cache = AllocDict(link,  num_buckets, residue );
        if( dict->cache != nullptr ) {
            QSeek(link,  file->infile->handle, dict->start, file->infile->name.u.ptr );
            for( bucket = 0; bucket < num_buckets; ++bucket ) {
                QRead(link,  file->infile->handle, dict->cache[bucket], DIC_REC_SIZE * PAGES_IN_CACHE, file->infile->name.u.ptr );
            }
            QRead(link,  file->infile->handle, dict->cache[bucket], DIC_REC_SIZE * residue, file->infile->name.u.ptr );
            file->infile->currpos = dict->start + DIC_REC_SIZE * ( residue + PAGES_IN_CACHE * num_buckets );
        }
    }
    if( dict->cache == nullptr ) {
        off = dict_page * DIC_REC_SIZE;
        dictoff = dict->start + off;
        dict->buffer = reinterpret_cast<unsigned_8 *>( link.TokBuff );
        QSeek(link,  file->infile->handle, dictoff, file->infile->name.u.ptr );
        QRead(link,  file->infile->handle, dict->buffer, DIC_REC_SIZE, file->infile->name.u.ptr );
        file->infile->currpos = dictoff + DIC_REC_SIZE;
    } else {
        bucket = dict_page / PAGES_IN_CACHE;
        residue = dict_page - bucket * PAGES_IN_CACHE;
        dict->buffer = static_cast<unsigned_8 *>( dict->cache[bucket] ) + residue * DIC_REC_SIZE;
    }
}

static unsigned OMFCompName( const char *name, const char *buff, unsigned index )
/********************************************************************************
 * Compare name.
 */
{
    size_t          len;
    unsigned        off;
    unsigned        returnval;

    returnval = 0;
    off = reinterpret_cast<const unsigned char *>( buff )[index];
    buff += off * 2;
    len = *reinterpret_cast<const unsigned char *>( buff );
    buff++;
    if( (*CmpNRtn)( buff, name, len ) == 0
      && name[len] == '\0' ) {
        returnval = MGET_LE_U16_UN( buff + len );
    }
    return( returnval );
}

static bool OMFSearchExtLib(Link& link,  file_list *file, const char *name, unsigned long *off )
/***********************************************************************************
 * Search library for specified member.
 */
{
    unsigned        num_blocks;
    unsigned        sector;
    unsigned        i;
    unsigned        j;
    hash_entry      h;
    omf_dict_entry  *dict;

    dict = &file->u.dict->o;
    num_blocks = dict->pages;

    omflib_hash( name, strlen( name ), &h, num_blocks );

    for( i = 0; i < num_blocks; ++i ) {
        SetDict(link,  file, h.block );
        for( j = 0; j < NUM_BUCKETS; ++j ) {
            if( dict->buffer[h.bucket] == LIB_NOT_FOUND ) {
                if( dict->buffer[NUM_BUCKETS] != LIB_FULL_PAGE )
                    return( false );
                break;
            }
            sector = OMFCompName( name, reinterpret_cast<const char *>( dict->buffer ), h.bucket );
            if( sector != 0 ) {
                *off = dict->rec_length * sector;
                return( true );
            }
            h.bucket += h.bucketd;
            if( h.bucket >= NUM_BUCKETS ) {
                h.bucket -= NUM_BUCKETS;
            }
        }
        h.block += h.blockd;
        if( h.block >= num_blocks ) {
            h.block -= num_blocks;
        }
    }
    return( false );
}

bool DiscardDicts(Link& link)
/************************
 * called when dictionaries forced out of dict memory
 */
{
    omf_dict_entry  *ptr;
    file_list       *file;

    ptr = nullptr;
    for( file = link.ObjLibFiles; file != nullptr; file = file->next ) {
        if( file->u.dict == nullptr )
            continue;
        if( file->flags_file & STAT_AR_LIB )
            continue;
        if( file->u.dict->o.cache == nullptr )
            continue;
        ptr = &file->u.dict->o;
    }
    if( ptr == nullptr )
        return( false ); /* no dicts in memory */
    FreeDictCache( ptr->cache, ( ptr->pages / PAGES_IN_CACHE ) + 1 );
    ptr->cache = nullptr;
    return( true );
}


void BurnLibs(Link& link)
/********************
 * let dict memory know it's no longer needed
 */
{
    file_list       *file;
    dict_entry      *dict;

    for( file = link.ObjLibFiles; file != nullptr; file = file->next ) {
        if( file->flags_file & STAT_AR_LIB ) {
            CacheFree( file, file->strtab );
            file->strtab = nullptr;
        }
        dict = file->u.dict;
        if( dict == nullptr )
            continue;
        if( file->flags_file & STAT_AR_LIB ) {
            CacheFree( file, dict->a.filepostab - 1 );
            MemFree( dict->a.symbtab );
        } else {
            if( dict->o.cache != nullptr ) {
                FreeDictCache( dict->o.cache, ( dict->o.pages / PAGES_IN_CACHE ) + 1 );
            }
        }
        MemFree( dict );
        file->u.dict = nullptr;
        FreeObjCache( file );
    }
}

static int ARCompName( const void *key, const void *vbase )
/*********************************************************/
{
    char            **base;

    base = static_cast<char **>( const_cast<void *>( vbase ) );
    return( (*CmpRtn)( static_cast<const char *>( key ), *base ) );
}

static bool ARSearchExtLib( file_list *file, const char *name, unsigned long *off )
/**********************************************************************************
 * Search AR format library for specified member.
 */
{
    char            **result;
    ar_dict_entry   *dict;
    unsigned        tabidx;

    dict = &file->u.dict->a;
    result = static_cast<char **>( bsearch( name, dict->symbtab, dict->num_entries, sizeof( char * ), ARCompName ) );
    if( result != nullptr ) {
        tabidx = result - dict->symbtab;
        if( dict->offsettab == nullptr ) {
            *off = dict->filepostab[tabidx];
        } else {
            *off = dict->filepostab[dict->offsettab[tabidx] - 1];
        }
        return( true );
    }
    return( false );
}

mod_entry *SearchLib(Link& link,  file_list *file, const char *name )
/********************************************************
 * Search the specified library file for the specified name & make a module
 */
{
    mod_entry       *obj;
    unsigned long   pos;
    unsigned long   dummy;
    bool            retval;
    char            *objname;

    pos = 0;
    if( file->u.dict == nullptr ) {
        if( CheckLibraryType(link,  file, &pos, true ) == -1 )
            return( nullptr );
        if( (file->flags_file & STAT_IS_LIB) == 0 ) {
            BadLibrary(link,  file );
            return( nullptr );
        }
    }
    if( file->flags_file & STAT_OMF_LIB ) {
        retval = OMFSearchExtLib(link,  file, name, &pos );
    } else {
        retval = ARSearchExtLib( file, name, &pos );
    }
    if( !retval )
        return( nullptr );

    /*
     *  update lib struct since we found desired object file
     */
    obj = NewModEntry(link);
    /*
     * correct module start position for modules in archive library
     */
    objname = IdentifyObject(link,  file, &pos, &dummy );
    if( objname != nullptr ) {
        obj->name.u.ptr = AddStringStringTable(link,  &link.PermStrings, objname );
        MemFree( objname );
    } else {
        obj->name.u.ptr = nullptr;
    }
    obj->location = pos;
    obj->u1.source = file;
    obj->modtime = file->infile->modtime;
    obj->flags_dbi = file->flags_dbi;
    obj->flags_mod = static_cast<module_flags>( 0 );
    obj->flags_fmt = link.ObjFileFormat;
    return( obj );
}

char *GetARName(Link& link,  const ar_header *header, const file_list *file, unsigned long *loc )
/***********************************************************************************/
{
    const char      *buf;
    char            *name;
    unsigned long   val;
    size_t          len;

    name = nullptr;
    if( header->name[0] == '/' ) {
        val = GetARValue( &header->name[1], AR_NAME_LEN - 1 );
        buf = file->strtab + val;
        len = strlen( buf );
    } else if( header->name[0] == '#'
      && header->name[1] == '1'
      && header->name[2] == '/') {
        len = GetARValue( &header->name[3], AR_NAME_LEN - 3 );
        buf = static_cast<const char *>( CacheRead(link,  file, *loc, len ) );
        *loc += len;
    } else {
        len = AR_NAME_LEN;
        buf = static_cast<const char *>( memchr( header->name, '/', len ) );
        if( buf != nullptr ) {
            len = buf - header->name;
        }
        buf = header->name;
    }
    if( len > 0 ) {
        name = MemToStringSafe(link,  buf, len );
    }
    return( name );
}

unsigned long GetARValue( const char *str, size_t max )
/******************************************************
 * get a numeric value from an ar_header
 */
{
    const std::string   text( str, max );

    return( strtoul( text.c_str(), nullptr, 10 ) );
}
