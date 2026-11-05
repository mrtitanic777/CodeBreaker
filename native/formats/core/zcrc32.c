/* zcrc32.c -- zlib's crc32(), for the zlib copy formats.dll compiles (native/decompiler's carries no
   crc32.c).  Only a gzip stream checks it; MSZIP is raw deflate, so this is never on a hot path. */
#include "zlib.h"

uLong ZEXPORT crc32(uLong crc, const Bytef *buf, uInt len) {
    if (buf == Z_NULL) return 0;
    crc = ~crc & 0xFFFFFFFFUL;
    while (len--) {
        int k;
        crc ^= *buf++;
        for (k = 0; k < 8; k++) crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
    }
    return ~crc & 0xFFFFFFFFUL;
}
