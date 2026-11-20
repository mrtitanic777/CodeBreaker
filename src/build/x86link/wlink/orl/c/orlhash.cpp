/****************************************************************************
*
*                            Open Watcom Project
*
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


#include <cctype>
#include <cstdlib>
#include <cstring>
#include <string_view>
#include "orllevel.h"
#include "orlhash.h"

#include "clibext.h"


static_assert( sizeof( orl_hash_key ) == sizeof( pointer_uint ) );

namespace {

template <class T>
T *hash_alloc( orl_hash_table table, size_t size )
{
    return static_cast<T *>( ORL_PTR_ALLOC( table, size ) );
}

void hash_free( orl_hash_table table, void *p )
{
    ORL_PTR_FREE( table, p );
}

bool number_cmp( orl_hash_key n1, orl_hash_key n2 )
{
    return n1.u.number == n2.u.number;
}

bool string_cmp( orl_hash_key p1, orl_hash_key p2 )
{
    return std::strcmp( p1.u.string, p2.u.string ) == 0;
}

bool string_cmp_ignorecase( orl_hash_key p1, orl_hash_key p2 )
{
    return stricmp( p1.u.string, p2.u.string ) == 0;
}

// the hash of the PJW family wlink has always used: 4 bits of shift per byte, the top nibble folded back in
orl_hash_value hash_encode( orl_hash_value size, std::string_view text, bool ignorecase )
{
    unsigned_32     h = 0;

    for( const char ch : text ) {
        int     c = static_cast<unsigned char>( ch );

        if( ignorecase )
            c = std::toupper( c );
        h = ( h << 4 ) + c;
        const unsigned_32   g = h & 0xf0000000;

        if( g != 0 ) {
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
    }
    return h % size;
}

orl_hash_value number_hash( orl_hash_value size, orl_hash_key key )
{
    return hash_encode( size, std::string_view( reinterpret_cast<const char *>( &key.u.number ), sizeof( key.u.number ) ), false );
}

orl_hash_value string_hash( orl_hash_value size, orl_hash_key key )
{
    return hash_encode( size, key.u.string, false );
}

orl_hash_value string_hash_ignorecase( orl_hash_value size, orl_hash_key key )
{
    return hash_encode( size, key.u.string, true );
}

}  // namespace

orl_return ORLHashTableInsert( orl_hash_table hash_table, orl_hash_key key, orl_hash_data data )
{
    orl_hash_data_entry     new_data_entry = hash_alloc<ORL_STRUCT( orl_hash_data_entry )>( hash_table, ORL_STRUCT_SIZEOF( orl_hash_data_entry ) );

    if( new_data_entry == nullptr )
        return ORL_OUT_OF_MEMORY;
    new_data_entry->data = data;
    new_data_entry->next = nullptr;
    const orl_hash_value    hash_value = hash_table->hash_func( hash_table->size, key );

    for( orl_hash_entry hash_entry = hash_table->table[hash_value]; hash_entry != nullptr; hash_entry = hash_entry->next ) {
        if( hash_table->compare_func( hash_entry->key, key ) ) {
            // same key: the data goes at the end of its chain
            orl_hash_data_entry     last_data_entry = hash_entry->data_entry;

            while( last_data_entry->next != nullptr ) {
                last_data_entry = last_data_entry->next;
            }
            last_data_entry->next = new_data_entry;
            return ORL_OKAY;
        }
    }
    orl_hash_entry  new_hash_entry = hash_alloc<ORL_STRUCT( orl_hash_entry )>( hash_table, ORL_STRUCT_SIZEOF( orl_hash_entry ) );

    if( new_hash_entry == nullptr )
        return ORL_OUT_OF_MEMORY;
    new_hash_entry->key = key;
    new_hash_entry->data_entry = new_data_entry;
    new_hash_entry->next = hash_table->table[hash_value];
    hash_table->table[hash_value] = new_hash_entry;
    return ORL_OKAY;
}

orl_hash_data_entry ORLHashTableQuery( orl_hash_table hash_table, orl_hash_key key )
{
    const orl_hash_value    hash_value = hash_table->hash_func( hash_table->size, key );

    for( orl_hash_entry hash_entry = hash_table->table[hash_value]; hash_entry != nullptr; hash_entry = hash_entry->next ) {
        if( hash_table->compare_func( hash_entry->key, key ) ) {
            return hash_entry->data_entry;
        }
    }
    return nullptr;
}

orl_hash_table ORLHashTableCreate( orl_funcs *funcs, orl_hash_value size, orl_hash_table_type type )
{
    orl_hash_table  hash_table = static_cast<orl_hash_table>( ORL_CLI_ALLOC( funcs, ORL_STRUCT_SIZEOF( orl_hash_table ) ) );

    if( hash_table == nullptr )
        return nullptr;
    hash_table->table = static_cast<orl_hash_entry *>( ORL_CLI_ALLOC( funcs, size * sizeof( orl_hash_entry ) ) );
    if( hash_table->table == nullptr ) {
        ORL_CLI_FREE( funcs, hash_table );
        return nullptr;
    }
    hash_table->size = size;
    hash_table->funcs = funcs;
    switch( type ) {
    case ORL_HASH_NUMBER:
        hash_table->hash_func = number_hash;
        hash_table->compare_func = number_cmp;
        break;
    case ORL_HASH_STRING:
        hash_table->hash_func = string_hash;
        hash_table->compare_func = string_cmp;
        break;
    case ORL_HASH_STRING_IGNORECASE:
    default:
        hash_table->hash_func = string_hash_ignorecase;
        hash_table->compare_func = string_cmp_ignorecase;
        break;
    }
    for( orl_hash_value i = 0; i < size; i++ ) {
        hash_table->table[i] = nullptr;
    }
    return hash_table;
}

void ORLHashTableFree( orl_hash_table hash_table )
{
    for( orl_hash_value i = 0; i < hash_table->size; i++ ) {
        orl_hash_entry  next_hash_entry;

        for( orl_hash_entry hash_entry = hash_table->table[i]; hash_entry != nullptr; hash_entry = next_hash_entry ) {
            next_hash_entry = hash_entry->next;
            orl_hash_data_entry     next_data_entry;

            for( orl_hash_data_entry data_entry = hash_entry->data_entry; data_entry != nullptr; data_entry = next_data_entry ) {
                next_data_entry = data_entry->next;
                hash_free( hash_table, data_entry );
            }
            hash_free( hash_table, hash_entry );
        }
    }
    hash_free( hash_table, hash_table->table );
    hash_free( hash_table, hash_table );
}
