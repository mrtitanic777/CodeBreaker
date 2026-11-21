/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2011-2013 The Open Watcom Contributors. All Rights Reserved.
*
* =========================================================================
*
* Description:  OMF library dictionary hash function
*
****************************************************************************/

#include <cstddef>
#include "omfhash.h"

namespace {

// 16-bit rotates; the original macros worked in int and every use truncated the result to 16 bits.
constexpr unsigned short rotl16( unsigned short a, unsigned b )
{
    return static_cast<unsigned short>( ( a << b ) | ( a >> ( 16 - b ) ) );
}

constexpr unsigned short rotr16( unsigned short a, unsigned b )
{
    return static_cast<unsigned short>( ( a << ( 16 - b ) ) | ( a >> b ) );
}

} // namespace

void omflib_hash( const char *name, size_t len, hash_entry *hash, unsigned num_blocks )
{
    unsigned short      count = static_cast<unsigned short>( len );
    const unsigned char *leftptr = reinterpret_cast<const unsigned char *>( name );
    const unsigned char *rightptr = leftptr + count;
    unsigned short      block = count | 0x20;
    unsigned short      blockd = 0;
    unsigned short      bucket = 0;
    unsigned short      bucketd = count | 0x20;

    for( ;; ) {
        --rightptr;
        /* zap to lower case (sort of) */
        unsigned short curr = *rightptr | 0x20;
        blockd = curr ^ rotl16( blockd, 2 );
        bucket = curr ^ rotr16( bucket, 2 );
        if( --count == 0 ) {
            break;
        }
        curr = *leftptr | 0x20;
        ++leftptr;
        block = curr ^ rotl16( block, 2 );
        bucketd = curr ^ rotr16( bucketd, 2 );
    }
    hash->bucket = bucket % NUM_BUCKETS;
    bucketd %= NUM_BUCKETS;
    if( bucketd == 0 ) {
        bucketd = 1;
    }
    hash->bucketd = bucketd;
    hash->block = block % num_blocks;
    blockd %= num_blocks;
    if( blockd == 0 ) {
        blockd = 1;
    }
    hash->blockd = blockd;
}
