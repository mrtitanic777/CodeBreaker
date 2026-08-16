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
// gzip COMPRESSOR using STORED (uncompressed) DEFLATE blocks + correct CRC32/ISIZE -- a valid gzip stream
// java.util.zip.GZIPInputStream accepts (no Huffman encoder needed). Returns bytes written or -1 on overflow.
// Worst-case output size = inLen + 5*ceil(inLen/65535) + 18; size the out buffer accordingly.
int gzipDeflateStored(const unsigned char* in, int inLen, unsigned char* out, int outCap);
// zlib-wrapped (RFC 1950) STORED DEFLATE + adler32 -- java.util.zip.Inflater / zlibInflate accept it. Used by
// DeflaterOutputStream for RegionFile chunk writes (version-2 = zlib). Worst-case out = inLen + 5*ceil(inLen/65535) + 6.
int zlibDeflateStored(const unsigned char* in, int inLen, unsigned char* out, int outCap);

#endif // INFLATE_H
