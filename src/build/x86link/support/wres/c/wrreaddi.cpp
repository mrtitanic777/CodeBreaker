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

#include <cstring>
#include "wresall.h"
#include "reserr.h"
#include "wresdefn.h"
#include "wresrtns.h"


namespace {

/* the list-linking calls take the head and tail of a node chain as void pointers */
template <typename Owner, typename Node>
void addAtEnd( Owner *owner, Node *node )
{
    ResAddLLItemAtEnd( reinterpret_cast<void **>( &(owner->Head) ), reinterpret_cast<void **>( &(owner->Tail) ), node );
}

bool readLangInfoLang( WResLangType *lang, FILE *fp )
{
    bool    error = false;

    lang->lang = ResReadUint16( &error, fp );
    if( error )
        return( true );
    lang->sublang = ResReadUint8( &error, fp );
    return( error );
}

bool readLangInfo( WResLangInfo *linfo, FILE *fp )
{
    bool    error = false;

    linfo->MemoryFlags = ResReadUint16( &error, fp );
    if( error )
        return( true );
    linfo->Offset = ResReadUint32( &error, fp );
    if( error )
        return( true );
    linfo->Length = ResReadUint32( &error, fp );
    return( error );
}

bool readLangInfoList( FILE *fp, WResResNode *resnode, void *fileinfo )
{
    for( unsigned i = 0; i < resnode->Info.NumResources; i++ ) {
        auto *langnode = static_cast<WResLangNode *>( WRESALLOC( sizeof( WResLangNode ) ) );

        if( langnode == nullptr )
            return( WRES_ERROR( WRS_MALLOC_FAILED ) );
        if( readLangInfoLang( &(langnode->Info.lang), fp ) )
            return( true );
        if( readLangInfo( &(langnode->Info), fp ) )
            return( true );
        langnode->data = nullptr;
        langnode->fileInfo = fileinfo;
        addAtEnd( resnode, langnode );
    }
    return( false );
}

bool readResList( FILE *fp, WResTypeNode *typenode, uint_16 ver, void *fileinfo )
{
    WResLangInfo    v1_linfo;

    std::memset( &v1_linfo, 0, sizeof( v1_linfo ) );
    /* loop through the list of resources of this type */
    for( int resnum = 0; resnum < typenode->Info.NumResources; resnum++ ) {
        uint_16     numres;

        /* read a resource record from disk */
        if( ver < 2 ) {
            numres = 1;
            if( readLangInfo( &v1_linfo, fp ) ) {
                return( true );
            }
        } else {
            bool    error = false;

            numres = ResReadUint16( &error, fp );
            if( error ) {
                return( true );
            }
        }
        auto *resnode = static_cast<WResResNode *>( ResReadWResID( fp, offsetof( WResResNode, Info.ResName ), ver ) );

        if( resnode == nullptr )
            return( true );
        resnode->Info.NumResources = numres;
        resnode->Head = nullptr;
        resnode->Tail = nullptr;
        if( ver < 2 ) {
            auto *langnode = static_cast<WResLangNode *>( WRESALLOC( sizeof( WResLangNode ) ) );

            if( langnode == nullptr )
                return( WRES_ERROR( WRS_MALLOC_FAILED ) );
            langnode->data = nullptr;
            langnode->fileInfo = fileinfo;
            langnode->Info.MemoryFlags = v1_linfo.MemoryFlags;
            langnode->Info.Offset = v1_linfo.Offset;
            langnode->Info.Length = v1_linfo.Length;
            langnode->Info.lang.lang = DEF_LANG;
            langnode->Info.lang.sublang = DEF_SUBLANG;
            addAtEnd( resnode, langnode );
        } else {
            if( readLangInfoList( fp, resnode, fileinfo ) ) {
                return( true );
            }
        }
        /* add the resource node to the linked list */
        addAtEnd( typenode, resnode );
    }
    return( false );
}

bool readTypeList( FILE *fp, WResDir dir, uint_16 ver, void *fileinfo )
{
    /* loop through the list of types */
    for( int typenum = 0; typenum < dir->NumTypes; typenum++ ) {
        bool    error = false;

        /* read a type record from disk */
        const uint_16   numres = ResReadUint16( &error, fp );

        if( error )
            return( true );

        auto *typenode = static_cast<WResTypeNode *>( ResReadWResID( fp, offsetof( WResTypeNode, Info.TypeName ), ver ) );

        if( typenode == nullptr )
            return( true );
        /* initialize the linked list of resources */
        typenode->Info.NumResources = numres;
        typenode->Head = nullptr;
        typenode->Tail = nullptr;
        /* add the type node to the linked list */
        addAtEnd( dir, typenode );
        /* read in the list of resources of this type */
        if( readResList( fp, typenode, ver, fileinfo ) ) {
            return( true );
        }
    }
    return( false );
}

bool readMResDir( FILE *fp, WResDir dir, bool *dup_discarded, bool iswin32, void *fileinfo )
/******************************************************************************************/
{
    MResResourceHeader      *msheader;
    WResDirWindow           wind_dup;

    if( WRESSEEK( fp, 0, SEEK_SET ) )
        return( WRES_ERROR( WRS_SEEK_FAILED ) );
    if( iswin32 ) {
        /* Read initial NULL header */
        msheader = MResReadResourceHeader( fp, iswin32 );
        if( msheader == nullptr )
            return( true );
        MResFreeResourceHeader( msheader );
    }
    if( dup_discarded != nullptr ) {
        *dup_discarded = false;
    }
    dir->TargetOS = iswin32 ? WRES_OS_WIN32 : WRES_OS_WIN16;
    /* assume that a NULL msheader is the EOF which is the only way of detecting */
    /* the end of a MS .RES file */
    bool    error = false;

    while( !error && (msheader = MResReadResourceHeader( fp, iswin32 )) != nullptr ) {
        WResID *res_id = WResIDFromNameOrOrdinal( msheader->Name );
        WResID *type_id = WResIDFromNameOrOrdinal( msheader->Type );

        error = (res_id == nullptr || type_id == nullptr);
        /* MResReadResourceHeader leaves the file at the start of the resource */
        if( !error ) {
            if( type_id->IsName
              || type_id->ID.Num != static_cast<uint_16>( RESOURCE2INT( RT_NAMETABLE ) ) ) {
                error = WResAddResource2( type_id, res_id, msheader->MemoryFlags,
                            WRESTELL( fp ), msheader->Size, dir, nullptr,
                            &wind_dup, fileinfo );
                if( error
                  && !WResIsEmptyWindow( wind_dup ) ) {
                    error = false;
                    if( dup_discarded != nullptr ) {
                        *dup_discarded = true;
                    }
                }
            }
        }
        if( !error ) {
            if( WRESSEEK( fp, msheader->Size, SEEK_CUR ) ) {
                error = WRES_ERROR( WRS_SEEK_FAILED );
            }
        }
        if( res_id != nullptr ) {
            WRESFREE( res_id );
        }
        if( type_id != nullptr ) {
            WRESFREE( type_id );
        }
        MResFreeResourceHeader( msheader );
    }
    return( error );
}

bool readResDir( FILE *fp, WResDir dir, void *fileinfo, bool *dup_discarded )
{
    WResHeader      header;
    WResExtHeader   extheader;

    /*
     * OW header is shorter then Microsoft headers
     * we can read it and use it for check file type
     */
    if( WResReadHeader( &header, fp ) )
        return( true );
    if( header.Magic[0] == WRESMAGIC0
      && header.Magic[1] == WRESMAGIC1 ) {
        /* RT_WATCOM */
        if( header.WResVer > WRESVERSION ) {
            return( WRES_ERROR( WRS_BAD_VERSION ) );
        }
        extheader.TargetOS = WRES_OS_WIN16;
        if( header.WResVer >= 1 ) {
            /*
             * read the extended header
             */
            if( WResReadExtHeader( &extheader, fp ) ) {
                return( true );
            }
        }
        /* set up the initial info for the directory and seek to it's start */
        dir->NumResources = header.NumResources;
        dir->NumTypes = header.NumTypes;
        dir->TargetOS = extheader.TargetOS;
        if( WRESSEEK( fp, header.DirOffset, SEEK_SET ) ) {
            return( WRES_ERROR( WRS_SEEK_FAILED ) );
        }
        /* read in the list of types (and the resources) */
        return( readTypeList( fp, dir, header.WResVer, fileinfo ) );
    }
    /* RT_WIN16 or RT_WIN32 */
    const bool iswin32 = ( header.Magic[0] == 0 && header.Magic[1] == 0x20 );

    return( readMResDir( fp, dir, dup_discarded, iswin32, fileinfo ) );
}

} // namespace

bool WResReadDir( FILE *fp, WResDir *dir, bool *dup_discarded )
{
    return( WResReadDir2( fp, dir, dup_discarded, nullptr ) );
}

bool WResReadDir2( FILE *fp, WResDir *pdir, bool *dup_discarded, void *fileinfo )
{
    bool        error;
    WResDir     dir;

    /*
     * var representing whether or not a duplicate dir entry was
     * discarded is set to false.
     * NOTE: duplicates are not discarded by calls to readWResDir.
     */
    if( dup_discarded != nullptr ) {
        *dup_discarded = false;
    }
    dir = WResInitDir();
    if( dir == nullptr ) {
        error = true;
    } else {
        error = readResDir( fp, dir, fileinfo, dup_discarded );
        if( error ) {
            WRESFREE( dir );
            dir = nullptr;
        }
    }
    *pdir = dir;
    return( error );
}
