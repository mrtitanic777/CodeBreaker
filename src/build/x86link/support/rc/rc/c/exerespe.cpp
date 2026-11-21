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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <cerrno>
#include <cstring>
#include <ctime>
#include <deque>
#include "global.h"
#include "rcstrblk.h"
#include "rcstr.h"
#include "rcerrors.h"
#include "build/x86link/formats/rcrtns.h"
#include "rccore_2.h"
#include "mergedir.h"
#include "exeobj.h"
#include "exeutil.h"
#include "exerespe.h"
#include "memfuncs.h"

#include "clibext.h"


#if !defined( INSIDE_WLINK ) || defined( _OS2 )

namespace {

constexpr const char RESOURCE_OBJECT_NAME[] = ".rsrc";

// the language id of a resource: the sublanguage above the 10 bits of the primary language
constexpr int makeLangId( uint_16 primary, uint_16 sub )
{
    return ( static_cast<uint_16>( sub ) << 10 ) | static_cast<uint_16>( primary );
}

void PEResDirEntryInit( PEResDirEntry *direntry, int num_entries )
{
    direntry->Head.flags = 0;
    direntry->Head.time_stamp = static_cast<unsigned_32>( std::time( nullptr ) );
    direntry->Head.major = 0;
    direntry->Head.minor = 0;
    direntry->Head.num_name_entries = 0;
    direntry->Head.num_id_entries = 0;
    direntry->NumUnused = num_entries;
    direntry->Children = static_cast<PEResEntry *>( MemAllocSafe( num_entries * sizeof( PEResEntry ) ) );
}

void PEResDirAdd( PEResDirEntry *direntry, WResID *res_id, StringsBlock *strings )
{
    const int       entry_num = direntry->Head.num_name_entries + direntry->Head.num_id_entries;
    PEResEntry      *entry = direntry->Children + entry_num;

    if( res_id->IsName ) {
        const int_32 name_off = StringBlockFind( strings, &res_id->ID.Name );
        if( name_off == -1 ) {
            /*
             * this case should not happen
             */
            entry->Entry.id_name = PE_RESOURCE_MASK_ON | 0;
            entry->Name = nullptr;
        } else {
            /*
             * This value will be changed later when we know the size of the
             * the resource directory
             */
            entry->Entry.id_name = PE_RESOURCE_MASK_ON | name_off;
            entry->Name = strings->StringBlock + name_off;
        }
        direntry->Head.num_name_entries++;
    } else {
        entry->Entry.id_name = res_id->ID.Num;
        entry->Name = nullptr;
        direntry->Head.num_id_entries++;
    }
    direntry->NumUnused--;
}

// the entry the last PEResDirAdd made
PEResEntry *lastEntry( PEResDirEntry *direntry )
{
    return direntry->Children + direntry->Head.num_name_entries + direntry->Head.num_id_entries - 1;
}

bool PEResDirAddDir( PEResDirEntry *direntry, WResID *res_id, int num_sub_entries, StringsBlock *strings )
{
    if( direntry->NumUnused <= 0 )
        return true;

    PEResDirAdd( direntry, res_id, strings );

    PEResEntry *entry = lastEntry( direntry );
    entry->IsDirEntry = true;
    PEResDirEntryInit( &entry->u.Dir, num_sub_entries );

    return false;
}

bool PEResDirAddData( PEResDirEntry *direntry, WResID *res_id, WResDirWindow wind, StringsBlock *strings )
{
    if( direntry->NumUnused <= 0 )
        return true;

    PEResDirAdd( direntry, res_id, strings );

    PEResEntry *entry = lastEntry( direntry );
    entry->IsDirEntry = false;
    entry->u.Data.Wind = wind;
    /*
     * The Data.Entry field will be filled in as the resource is writen
     */
    return false;
}

bool AddType( PEResDir *pedir, WResTypeInfo *typeinfo )
{
    return PEResDirAddDir( &pedir->Root, &typeinfo->TypeName, typeinfo->NumResources, &pedir->String );
}

bool AddLang( PEResDir *pedir, WResDirWindow wind )
{
    WResLangInfo    *langinfo = WResGetLangInfo( wind );
    WResID          lang_id;

    /*
     * find the current type
     */
    PEResEntry *entry_type = lastEntry( &pedir->Root );
    /*
     * find the current resource
     */
    PEResEntry *entry_res = lastEntry( &entry_type->u.Dir );

    lang_id.IsName = false;
    lang_id.ID.Num = makeLangId( langinfo->lang.lang, langinfo->lang.sublang );
    return PEResDirAddData( &entry_res->u.Dir, &lang_id, wind, &pedir->String );
}

bool AddRes( PEResDir *pedir, WResDirWindow wind )
{
    WResResInfo     *resinfo = WResGetResInfo( wind );
    /*
     * find the current type
     */
    PEResEntry      *entry_type = lastEntry( &pedir->Root );

    /*
     * Add a directory level for the languages
     */
    return PEResDirAddDir( &entry_type->u.Dir, &resinfo->ResName, resinfo->NumResources, &pedir->String );
}

bool PEResDirBuild( PEResDir *pedir, WResDir dir )
{
    if( WResIsEmpty( dir ) ) {
        pedir->Root.Head.num_name_entries = 0;
        pedir->Root.Head.num_id_entries = 0;
        pedir->Root.Children = nullptr;
        pedir->ResRVA = 0;
        pedir->ResOffset = 0;
        pedir->ResSize = 0;
    } else {
        StringIDNamesBlockBuild( &pedir->String, dir, true );
        pedir->DirSize = sizeof( resource_dir_header );
        PEResDirEntryInit( &pedir->Root, dir->NumTypes );
        for( WResDirWindow wind = WResFirstResource( dir ); !WResIsEmptyWindow( wind ); wind = WResNextResource( wind, dir ) ) {
            if( WResIsFirstResOfType( wind ) ) {
                if( AddType( pedir, WResGetTypeInfo( wind ) ) ) {
                    return true;
                }
            }
            if( WResIsFirstLangOfRes( wind ) ) {
                if( AddRes( pedir, wind ) ) {
                    return true;
                }
            }
            AddLang( pedir, wind );
        }
    }
    return false;
}

/*
 * NB when a visit function returns an error this function MUST return
 *    without altering errno
 *
 * Perfroms a level order traversal of a PEResDir tree calling visit( entry )
 * at each entry
 */
template <class Visit>
RcStatus traverseTree( PEResDir *pedir, Visit &&visit )
{
    // the directories still to visit: few, so a plain queue
    std::deque<PEResDirEntry *> queue;

    queue.push_back( &pedir->Root );

    while( !queue.empty() ) {
        PEResDirEntry *direntry = queue.front();
        queue.pop_front();
        PEResEntry *entry_last = direntry->Children + direntry->Head.num_name_entries + direntry->Head.num_id_entries;
        for( PEResEntry *entry = direntry->Children; entry < entry_last; entry++ ) {
            const RcStatus ret = visit( entry );
            if( ret != RS_OK )
                return ret;
            if( entry->IsDirEntry ) {
                queue.push_back( &entry->u.Dir );
            }
        }
    }
    return RS_OK;
}

RcStatus SetEntryOffset( PEResEntry *entry, uint_32 *curr_offset )
{
    if( entry->IsDirEntry ) {
        entry->Entry.entry_rva = *curr_offset | PE_RESOURCE_MASK_ON;
        const int num_entries = entry->u.Dir.Head.num_name_entries + entry->u.Dir.Head.num_id_entries;
        *curr_offset += sizeof( resource_dir_header ) + num_entries * sizeof( resource_dir_entry );
    } else {
        entry->Entry.entry_rva = *curr_offset;
        *curr_offset += sizeof( resource_entry );
    }
    return RS_OK;
}

RcStatus AdjustNameEntry( PEResEntry *entry, const uint_32 *dir_size )
{
    if( entry->Entry.id_name & PE_RESOURCE_MASK_ON ) {
        /*
         * the id_name contains the offset into the string block
         */
        uint_32 str_offset = entry->Entry.id_name & PE_RESOURCE_MASK;
        str_offset += *dir_size;
        entry->Entry.id_name = str_offset | PE_RESOURCE_MASK_ON;
    }

    return RS_OK;
}

// qsort's comparator: the entries with a name go first, by name, then the ones with an id, by id
int ComparePEResIdName( const void *e1, const void *e2 )
{
    const PEResEntry *a = static_cast<const PEResEntry *>( e1 );
    const PEResEntry *b = static_cast<const PEResEntry *>( e2 );

    if( a->Name == nullptr ) {
        if( b->Name == nullptr ) {
            if( a->Entry.id_name > b->Entry.id_name ) {
                return 1;
            } else if( a->Entry.id_name < b->Entry.id_name ) {
                return -1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        if( b->Name == nullptr ) {
            return -1;
        } else {
            return CompareStringItems32( reinterpret_cast<const StringItem32 *>( a->Name ),
                                         reinterpret_cast<const StringItem32 *>( b->Name ) );
        }
    }
}

RcStatus SortDirEntry( PEResEntry *entry )
{
    if( entry->IsDirEntry ) {
        const int num_entries = entry->u.Dir.Head.num_name_entries + entry->u.Dir.Head.num_id_entries;
        std::qsort( entry->u.Dir.Children, num_entries, sizeof( PEResEntry ), ComparePEResIdName );
    }
    return RS_OK;
}

void CompleteTree( PEResDir *pedir )
{
    const int num_entries = pedir->Root.Head.num_name_entries + pedir->Root.Head.num_id_entries;
    /*
     * sort the entries at each level
     */
    std::qsort( pedir->Root.Children, num_entries, sizeof( PEResEntry ), ComparePEResIdName );
    traverseTree( pedir, SortDirEntry );
    /*
     * Set curr_offset to the size of the root entry
     */
    uint_32 curr_offset = sizeof( resource_dir_header ) + num_entries * sizeof( resource_dir_entry );

    traverseTree( pedir, [&curr_offset]( PEResEntry *entry ) { return SetEntryOffset( entry, &curr_offset ); } );
    pedir->DirSize = curr_offset;

    traverseTree( pedir, [&curr_offset]( PEResEntry *entry ) { return AdjustNameEntry( entry, &curr_offset ); } );
}

struct CopyResInfo {
    FILE                *to_fp;
    ExeFileInfo         *file;  /* for setting debugging offset */
    ResFileInfo         *curres;
    ResFileInfo         *errres;
};

/*
 * NB when an error occurs this function MUST return without altering errno
 */
RcStatus copyDataEntry( PEResEntry *entry, CopyResInfo *copy_info )
{
    if( !entry->IsDirEntry ) {
        ResFileInfo *info = static_cast<ResFileInfo *>( WResGetFileInfo( entry->u.Data.Wind ) );
        if( copy_info->curres == nullptr
          || copy_info->curres == info ) {
            WResLangInfo *langinfo = WResGetLangInfo( entry->u.Data.Wind );
            if( RESSEEK( info->fp, langinfo->Offset, SEEK_SET ) )
                return RS_READ_ERROR;
            const RcStatus ret = CopyExeData( info->fp, copy_info->to_fp, langinfo->Length );
            if( ret != RS_OK ) {
                copy_info->errres = info;
                return ret;
            }
            const uint_32 diff = ALIGN_VALUE( langinfo->Length, sizeof( uint_32 ) );
            if( diff > langinfo->Length ) {
                /*
                 * add the padding
                 */
                if( RcPadFile( copy_info->to_fp, static_cast<std::size_t>( diff - langinfo->Length ) ) ) {
                    return RS_WRITE_ERROR;
                }
            }
            CheckDebugOffset( copy_info->file );
        }
    }

    return RS_OK;
}

/*
 * NB when an error occurs this function MUST return without altering errno
 */
RcStatus copyPEResources( ExeFileInfo *dst, ResFileInfo *resfiles, FILE *to_fp, bool writebyfile, ResFileInfo **errres )
{
    CopyResInfo     copy_info;
    RcStatus        ret;
    uint_32         start_off = dst->u.PEInfo.Res.ResOffset + dst->u.PEInfo.Res.DirSize + dst->u.PEInfo.Res.String.StringBlockSize;

    copy_info.to_fp = to_fp;
    copy_info.errres = nullptr;
    copy_info.file = dst;       /* for tracking debugging info offset */
    start_off = ALIGN_VALUE( start_off, sizeof( uint_32 ) );

    auto copyAll = [dst, &copy_info]() {
        return traverseTree( &dst->u.PEInfo.Res, [&copy_info]( PEResEntry *entry ) { return copyDataEntry( entry, &copy_info ); } );
    };

    if( RESSEEK( to_fp, start_off, SEEK_SET ) )
        return RS_WRITE_ERROR;
    if( !writebyfile ) {
        copy_info.curres = nullptr;
        ret = copyAll();
        *errres = copy_info.errres;
    } else {
        ret = RS_OK;
        for( ; resfiles != nullptr; resfiles = resfiles->next ) {
            copy_info.curres = resfiles;
            if( resfiles->fp != nullptr ) {
                ret = copyAll();
            } else {
                ret = RS_OPEN_ERROR;
                resfiles->fp = ResOpenFileRO( resfiles->name );
                if( resfiles->fp != nullptr ) {
                    ret = copyAll();
                    RCCloseFile( &(resfiles->fp) );
                }
            }
            if( ret != RS_OK ) {
                *errres = resfiles;
                break;
            }
        }
    }
    return ret;
}

/*
 * NB when an error occurs this function MUST return without altering errno
 */
RcStatus writeDirEntry( PEResDirEntry *direntry, FILE *fp )
{
    if( RESWRITE( fp, &direntry->Head, sizeof( resource_dir_header ) ) != sizeof( resource_dir_header ) )
        return RS_WRITE_ERROR;

    for( int child_num = 0; child_num < direntry->Head.num_name_entries + direntry->Head.num_id_entries; child_num++ ) {
        if( RESWRITE( fp, direntry->Children + child_num, sizeof( resource_dir_entry ) ) != sizeof( resource_dir_entry ) ) {
            return RS_WRITE_ERROR;
        }
    }

    return RS_OK;
}

/*
 * NB when an error occurs this function MUST return without altering errno
 */
RcStatus writeDataEntry( PEResDataEntry *entry, FILE *fp )
{
    if( RESWRITE( fp, &entry->Entry, sizeof( resource_entry ) ) != sizeof( resource_entry ) )
        return RS_WRITE_ERROR;
    return RS_OK;
}

struct DataEntryCookie {
    ResFileInfo         *curfile;
    unsigned_32         *rva;
};

RcStatus setDataEntry( PEResEntry *entry, DataEntryCookie *info )
{
    if( !entry->IsDirEntry ) {
        ResFileInfo *fileinfo = static_cast<ResFileInfo *>( WResGetFileInfo( entry->u.Data.Wind ) );
        if( info->curfile == nullptr
          || info->curfile == fileinfo ) {
            WResLangInfo *langinfo = WResGetLangInfo( entry->u.Data.Wind );
            entry->u.Data.Entry.data_rva = *info->rva;
            entry->u.Data.Entry.size = langinfo->Length;
            entry->u.Data.Entry.code_page = 0;    /* should this be the UNICODE page*/
            entry->u.Data.Entry.rsvd = 0;
            *info->rva = *info->rva + ALIGN_VALUE( langinfo->Length, sizeof( uint_32 ) );
        }
    }
    return RS_OK;
}

/*
 * NB when an error occurs this function MUST return without altering errno
 */
RcStatus writeEntry( PEResEntry *entry, FILE *fp )
{
    if( entry->IsDirEntry ) {
        return writeDirEntry( &entry->u.Dir, fp );
    } else {
        return writeDataEntry( &entry->u.Data, fp );
    }
}

/*
 * NB when an error occurs this function MUST return without altering errno
 */
RcStatus writeDirectory( PEResDir *pedir, FILE *fp )
{
    if( RESSEEK( fp, pedir->ResOffset, SEEK_SET ) )
        return RS_WRITE_ERROR;
    /*
     * write the root entry header
     */
    RcStatus ret = writeDirEntry( &pedir->Root, fp );
    if( ret != RS_OK )
        return ret;

    ret = traverseTree( pedir, [fp]( PEResEntry *entry ) { return writeEntry( entry, fp ); } );
    if( ret != RS_OK )
        return ret;

    if( pedir->String.StringBlock != nullptr ) {
        if( RESWRITE( fp, pedir->String.StringBlock, pedir->String.StringBlockSize ) != pedir->String.StringBlockSize ) {
            return RS_WRITE_ERROR;
        }
    }

    return RS_OK;
}

void FreeSubDir( PEResDirEntry *direntry )
{
    const int       num_children = direntry->Head.num_id_entries + direntry->Head.num_name_entries;
    PEResEntry      *entry_last = direntry->Children + num_children;

    for( PEResEntry *entry = direntry->Children; entry < entry_last; ++entry ) {
        if( entry->IsDirEntry ) {
            FreeSubDir( &entry->u.Dir );
        }
    }

    MemFree( direntry->Children );
}

void FreePEResDir( PEResDir *pedir )
{
    FreeSubDir( &pedir->Root );
    if( pedir->String.StringBlock != nullptr )
        MemFree( pedir->String.StringBlock );
    if( pedir->String.StringList != nullptr ) {
        MemFree( pedir->String.StringList );
    }
}

} // namespace

#if !defined( INSIDE_WLINK )
bool RcPadFile( FILE *fp, size_t pad )
{
    const char  zero = 0;

    if( pad > 0 ) {
        if( RESSEEK( fp, pad - 1, SEEK_CUR ) ) {
            return true;
        }
        if( RESWRITE( fp, &zero, 1 ) != 1 )  {
            return true;
        }
    }
    return false;
}
#endif

namespace {

/*
 * NB when an error occurs this function MUST return without altering errno
 */
bool padObject( PEResDir *pedir, ExeFileInfo *dst, long size )
{
    const long pos = RESTELL( dst->fp );

    if( pos == -1 )
        return true;
    const long pad = pedir->ResOffset + size - pos;
    if( pad > 0 ) {
        RcPadFile( dst->fp, static_cast<std::size_t>( pad ) );
    }
    CheckDebugOffset( dst );
    return false;
}

void fillResourceObj( pe_object *res_obj, PEResDir *pedir, uint_32 alignment )
{
    std::strncpy( res_obj->name, RESOURCE_OBJECT_NAME, PE_OBJ_NAME_LEN );
    res_obj->virtual_size = 0;
    res_obj->rva = pedir->ResRVA;
    res_obj->physical_size = ALIGN_VALUE( pedir->ResSize, alignment );
    res_obj->physical_offset = pedir->ResOffset;
    res_obj->relocs_rva = 0;
    res_obj->linnum_rva = 0;
    res_obj->num_relocs = 0;
    res_obj->num_linnums = 0;
    res_obj->flags = PE_OBJ_INIT_DATA | PE_OBJ_READABLE;
}

void setDataOffsets( PEResDir *pedir, unsigned_32 *curr_rva, ResFileInfo *resfiles, bool writebyfile )
{
    DataEntryCookie     cookie;

    cookie.rva = curr_rva;
    auto setAll = [pedir, &cookie]() {
        traverseTree( pedir, [&cookie]( PEResEntry *entry ) { return setDataEntry( entry, &cookie ); } );
    };
    if( writebyfile ) {
        for( ; resfiles != nullptr; resfiles = resfiles->next ) {
            cookie.curfile = resfiles;
            setAll();
        }
    } else {
        cookie.curfile = nullptr;
        setAll();
    }
}

void reportDuplicateResources( WResMergeError *errs )
{
    for( WResMergeError *curerr = errs; curerr != nullptr; curerr = curerr->next ) {
        WResResInfo     *resinfo = WResGetResInfo( curerr->dstres );
        WResTypeInfo    *typeinfo = WResGetTypeInfo( curerr->dstres );
        ResFileInfo     *file1 = static_cast<ResFileInfo *>( WResGetFileInfo( curerr->dstres ) );
        ResFileInfo     *file2 = static_cast<ResFileInfo *>( WResGetFileInfo( curerr->srcres ) );
        ReportDupResource( &resinfo->ResName, &typeinfo->TypeName, file1->name, file2->name, false );
    }
}

} // namespace

/*
 * here is linker entry point for building resources in executable
 * image
 */
bool BuildPEResourceObject( ExeFileInfo *dst, ResFileInfo *resfiles, pe_object *res_obj, unsigned_32 rva, unsigned_32 offset, bool writebyfile )
{
    PEResDir            *pedir = &dst->u.PEInfo.Res;
    WResMergeError      *errs;

    MergeDirectory( resfiles, &errs );
    if( errs != nullptr ) {
        reportDuplicateResources( errs );
        WResFreeMergeErrors( errs );
        return true;
    }
    if( PEResDirBuild( pedir, resfiles->Dir ) ) {
        RcError( ERR_INTERNAL, INTERR_ERR_BUILDING_RES_DIR );
        return true;
    }
    CompleteTree( pedir );
    dst->u.PEInfo.Res.ResOffset = offset;
    dst->u.PEInfo.Res.ResRVA = rva;
    unsigned_32 curr_rva = rva + dst->u.PEInfo.Res.DirSize + dst->u.PEInfo.Res.String.StringBlockSize;
    curr_rva = ALIGN_VALUE( curr_rva, sizeof( uint_32 ) );
    setDataOffsets( pedir, &curr_rva, resfiles, writebyfile );
    RcStatus ret = writeDirectory( pedir, dst->fp );
    if( ret != RS_OK ) {
        RcError( ERR_WRITTING_FILE, dst->name, std::strerror( errno ) );
        return true;
    }
    ResFileInfo *errres = nullptr;
    ret = copyPEResources( dst, resfiles, dst->fp, writebyfile, &errres );
    /*
     * warning - the file names output in these messages could be
     *          incorrect if the -fr switch is in use
     */
    if( ret != RS_OK  ) {
        if( ret == RS_READ_ERROR ) {
            ret = RS_READ_ERROR_RES;
        }
        RcIOError( ret, errres->name, dst->name, errno );
        return true;
    }
    dst->u.PEInfo.Res.ResSize = curr_rva - rva;

    pe_exe_header *pehdr = dst->u.PEInfo.WinHead;
    fillResourceObj( res_obj, pedir, PE( *pehdr, file_align ) );
    if( padObject( pedir, dst, res_obj->physical_size ) ) {
        RcError( ERR_WRITTING_FILE, dst->name, std::strerror( errno ) );
        return true;
    }
    /*
     * set the resource element of the table in the header
     */
    PE_DIRECTORY( *pehdr, PE_TBL_RESOURCE ).rva = res_obj->rva;
    PE_DIRECTORY( *pehdr, PE_TBL_RESOURCE ).size = res_obj->physical_size;

    FreePEResDir( pedir );

    return false;
}


#if !defined( INSIDE_WLINK )
bool RcBuildPEResourceObject( ExeFileInfo *dst, ResFileInfo *resfiles )
{
    pe_exe_header       *pehdr = dst->u.PEInfo.WinHead;
    bool                error;

    if( CmdLineParms.NoResFile ) {
        PE_DIRECTORY( *pehdr, PE_TBL_RESOURCE ).rva = 0;
        PE_DIRECTORY( *pehdr, PE_TBL_RESOURCE ).size = 0;
        error = false;
    } else {
        pe_object *res_obj = dst->u.PEInfo.Objects + pehdr->fheader.num_objects - 1;
        const unsigned_32 rva = GetNextObjRVA( &dst->u.PEInfo );
        const unsigned_32 offset = GetNextObjPhysOffset( &dst->u.PEInfo );
        error = BuildPEResourceObject( dst, resfiles, res_obj, rva, offset, !Pass2Info.AllResFilesOpen );
    }
    return error;
}
#endif  /* !defined( INSIDE_WLINK ) */

#endif  /* !defined( INSIDE_WLINK ) || defined( _OS2 ) */
