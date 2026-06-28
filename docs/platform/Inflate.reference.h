#ifndef INFLATE_H
#define INFLATE_H
// ============================ Compact RFC-1951 / RFC-1950 inflate =============================
// Self-contained DEFLATE (RFC-1951) decompressor + zlib (RFC-1950) wrapper. No allocations, no deps
// (the 360 SDK ships no zlib). Algorithm follows Mark Adler's public-domain puff.c. Used by
// Packet51MapChunk to decompress SMP chunk data (java.util.Inflater on the server side produces a
// zlib stream: 2-byte header + DEFLATE + adler32 trailer).
//
//   zlibInflate: input is zlib-wrapped (default java.util.Inflater).  rawInflate: bare DEFLATE.
//   Returns the number of bytes written to out (<= outCap), or -1 on error.
// =============================================================================================

int rawInflate(const unsigned char* in, int inLen, unsigned char* out, int outCap);
int zlibInflate(const unsigned char* in, int inLen, unsigned char* out, int outCap);
// gzip-wrapped DEFLATE (RFC 1952): 10-byte header (+ optional FEXTRA/FNAME/FCOMMENT/FHCRC fields) +
// DEFLATE + 8-byte CRC32/ISIZE trailer. Used for NBT (java.util.zip.GZIPInputStream / level.dat / Packet59).
int gzipInflate(const unsigned char* in, int inLen, unsigned char* out, int outCap);

#endif // INFLATE_H
