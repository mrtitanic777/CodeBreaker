/* zcrc32.c -- zlib's crc32 for the zlib copy native/pyrt links (native/decompiler/core/zlib has no
   crc32.c): the reflected CRC-32 (polynomial 0xEDB88320) zlib, gzip, zip and binascii use, a byte at a
   time over a table made on first use. */
#include "zlib.h"

static unsigned long crc_table[256];
static int crc_ready = 0;

static void make_table(void) {
    for (unsigned long i = 0; i < 256; i++) {
        unsigned long c = i;
        for (int k = 0; k < 8; k++) c = c & 1 ? 0xEDB88320UL ^ (c >> 1) : c >> 1;
        crc_table[i] = c;
    }
    crc_ready = 1;
}

uLong ZEXPORT crc32_z(uLong crc, const Bytef *buf, z_size_t len) {
    if (buf == Z_NULL) return 0UL;
    if (!crc_ready) make_table();
    crc = (~crc) & 0xFFFFFFFFUL;
    while (len--) crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
    return (~crc) & 0xFFFFFFFFUL;
}

uLong ZEXPORT crc32(uLong crc, const Bytef *buf, uInt len) { return crc32_z(crc, buf, len); }

const z_crc_t FAR * ZEXPORT get_crc_table(void) {
    if (!crc_ready) make_table();
    return (const z_crc_t FAR *)crc_table;
}
