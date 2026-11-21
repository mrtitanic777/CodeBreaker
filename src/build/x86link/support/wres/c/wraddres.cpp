/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2026      The Open Watcom Contributors. All Rights Reserved.
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
#include "wresrtns.h"


namespace {

/* the bytes an ID takes: its flag and its number, or its flag, length and name */
size_t WResIDSize( const WResID *id )
{
    if( id->IsName ) {
        return( offsetof( WResID, ID.Name.Name ) + id->ID.Name.NumChars );
    }
    return( offsetof( WResID, ID.Num ) + sizeof( id->ID.Num ) );
}

WResTypeNode *newTypeNode( const WResID *type_id )
{
    const size_t    id_size = WResIDSize( type_id );
    auto            *typenode = static_cast<WResTypeNode *>( WRESALLOC( offsetof( WResTypeNode, Info.TypeName ) + id_size ) );

    if( typenode != nullptr ) {
        typenode->Next = nullptr;
        typenode->Prev = nullptr;
        typenode->Head = nullptr;
        typenode->Tail = nullptr;
        typenode->Info.NumResources = 0;
        std::memcpy( &(typenode->Info.TypeName), type_id, id_size );
    } else {
        WRES_ERROR( WRS_MALLOC_FAILED );
    }
    return( typenode );
}

WResLangNode *newLangNode( uint_16 memflags, uint_32 offset, uint_32 length, const WResLangType *lang, void *fileinfo )
{
    auto *langnode = static_cast<WResLangNode *>( WRESALLOC( sizeof( WResLangNode ) ) );

    if( langnode == nullptr ) {
        WRES_ERROR( WRS_MALLOC_FAILED );
    } else {
        langnode->Next = nullptr;
        langnode->Prev = nullptr;
        langnode->data = nullptr;
        langnode->fileInfo = fileinfo;
        langnode->Info.MemoryFlags = memflags;
        langnode->Info.Offset = offset;
        langnode->Info.Length = length;
        if( lang == nullptr ) {
            langnode->Info.lang.lang = DEF_LANG;
            langnode->Info.lang.sublang = DEF_SUBLANG;
        } else {
            langnode->Info.lang = *lang;
        }
    }
    return( langnode );
}

WResResNode *newResNode( const WResID *res_id )
{
    const size_t    id_size = WResIDSize( res_id );
    auto            *resnode = static_cast<WResResNode *>( WRESALLOC( offsetof( WResResNode, Info.ResName ) + id_size ) );

    if( resnode == nullptr ) {
        WRES_ERROR( WRS_MALLOC_FAILED );
    } else {
        resnode->Next = nullptr;
        resnode->Prev = nullptr;
        resnode->Head = nullptr;
        resnode->Tail = nullptr;
        resnode->Info.NumResources = 0;
        std::memcpy( &(resnode->Info.ResName), res_id, id_size );
    }
    return( resnode );
}

} // namespace

/*
 * WResAddResource - Add the new entry to the directory. If type is NULL the
 *                   default is used.  If the entry is already in the
 *                   directory don't add anything, set duplicate true
 *                   and return an error. Return is true if any error has
 *                   occured (including duplicate entry)
 */
bool WResAddResource( const WResID *type_id, const WResID *res_id,
                    uint_16 memflags, long offset, uint_32 length,
                    WResDir dir, const WResLangType *lang,
                    bool *duplicate )
/************************************************************/
{
    WResDirWindow       wind_dup;
    const bool          rc = WResAddResource2( type_id, res_id, memflags, offset, length, dir,
                                               lang, &wind_dup, nullptr );

    if( duplicate != nullptr ) {
        *duplicate = !WResIsEmptyWindow( wind_dup );
    }
    return( rc );
}

bool WResAddResource2( const WResID *type_id, const WResID *res_id,
                    uint_16 memflags, long offset, uint_32 length,
                    WResDir dir, const WResLangType *lang,
                    WResDirWindow *wind_dup, void *fileinfo )
/************************************************************/
{
    /* set duplicate false so other errors will have it set correctly */
    if( wind_dup != nullptr ) {
        WResSetEmptyWindow( wind_dup );
    }
    WResResNode     *resnode = nullptr;
    WResTypeNode    *typenode = __FindType( type_id, dir );

    if( typenode != nullptr ) {
        /* if the type is in there already check for a duplicate resource */
        resnode = __FindRes( res_id, typenode );
        if( resnode != nullptr ) {
            WResLangNode *langnode = __FindLang( lang, resnode );

            if( langnode != nullptr ) {
                if( wind_dup != nullptr )
                    WResMakeWindow( wind_dup, typenode, resnode, langnode );
                return( WRES_ERROR( WRS_DUP_ENTRY ) );
            }
        }
    }
    if( typenode == nullptr ) {
        /* otherwise add the type to the list */
        typenode = newTypeNode( type_id );
        if( typenode == nullptr ) {
            return( true );
        }
        ResAddLLItemAtEnd( reinterpret_cast<void **>( &(dir->Head) ), reinterpret_cast<void **>( &(dir->Tail) ), typenode );
        /* adjust the count of the number of types */
        dir->NumTypes += 1;
    }
    if( resnode == nullptr ) {
        /* add the resource to the current type */
        resnode = newResNode( res_id );
        if( resnode == nullptr ) {
            return( true );
        }
        ResAddLLItemAtEnd( reinterpret_cast<void **>( &(typenode->Head) ), reinterpret_cast<void **>( &(typenode->Tail) ), resnode );
        /* adjust the counts of the number of resources */
        typenode->Info.NumResources += 1;
        dir->NumResources += 1;
    }
    WResLangNode *langnode = newLangNode( memflags, offset, length, lang, fileinfo );

    if( langnode == nullptr ) {
        return( true );
    }
    ResAddLLItemAtEnd( reinterpret_cast<void **>( &(resnode->Head) ), reinterpret_cast<void **>( &(resnode->Tail) ), langnode );
    resnode->Info.NumResources++;
    /* no error has occured */
    return( false );
}
