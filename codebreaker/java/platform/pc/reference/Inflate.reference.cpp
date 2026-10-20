#include "stdafx.h"
#include "Inflate.h"

// ============================================================================================
// Compact DEFLATE (RFC-1951) inflate, after Mark Adler's puff.c (public domain). Big-endian-safe
// (works purely on byte/bit math, no multi-byte loads). No heap use.
// ============================================================================================

namespace {

struct BitReader {
	const unsigned char* src;
	int len;
	int pos;        // next byte to load
	int bitbuf;     // current byte, shifted right as bits are consumed
	int bitcnt;     // bits remaining in bitbuf
	bool error;
};

static int getBit(BitReader* b) {
	if (b->bitcnt == 0) {
		if (b->pos >= b->len) { b->error = true; return 0; }
		b->bitbuf = b->src[b->pos++];
		b->bitcnt = 8;
	}
	int bit = b->bitbuf & 1;
	b->bitbuf >>= 1;
	b->bitcnt--;
	return bit;
}

// n bits, LSB first (DEFLATE order for extra bits / lengths)
static int getBits(BitReader* b, int n) {
	int v = 0;
	for (int i = 0; i < n; ++i) v |= getBit(b) << i;
	return v;
}

struct Huffman {
	short count[16];   // number of codes of each length 1..15 (index 0 unused)
	short symbol[288]; // canonical symbol order
};

// Build a canonical Huffman table from code lengths (puff "construct").
static void buildHuffman(Huffman* h, const unsigned char* lengths, int n) {
	int len, sym;
	for (len = 0; len <= 15; ++len) h->count[len] = 0;
	for (sym = 0; sym < n; ++sym) h->count[lengths[sym]]++;
	h->count[0] = 0;   // 0-length = unused symbol
	short offs[16];
	offs[1] = 0;
	for (len = 1; len < 15; ++len) offs[len + 1] = (short)(offs[len] + h->count[len]);
	for (sym = 0; sym < n; ++sym)
		if (lengths[sym] != 0) h->symbol[offs[lengths[sym]]++] = (short)sym;
}

// Decode one symbol (puff "decode"): bits read LSB-first from the stream, codes are MSB-first.
static int decodeSymbol(BitReader* b, const Huffman* h) {
	int code = 0, first = 0, index = 0;
	for (int len = 1; len <= 15; ++len) {
		code |= getBit(b);
		if (b->error) return -1;
		int count = h->count[len];
		if (code - count < first) return h->symbol[index + (code - first)];
		index += count;
		first += count;
		first <<= 1;
		code <<= 1;
	}
	return -1;   // ran out of code lengths -> corrupt
}

static const short  LEN_BASE[29]  = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
static const unsigned char LEN_EXTRA[29] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
static const short  DIST_BASE[30]  = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
static const unsigned char DIST_EXTRA[30] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

static int inflateBlocks(BitReader* b, unsigned char* out, int outCap) {
	int outPos = 0;
	int last = 0;
	while (!last) {
		last = getBit(b);
		int type = getBits(b, 2);
		if (b->error) return -1;

		if (type == 0) {
			// stored block: align to byte, then LEN (2) + NLEN (2) + raw bytes
			b->bitcnt = 0;   // discard remaining bits in the current byte (b->pos already points past it)
			if (b->pos + 4 > b->len) return -1;
			int blen = b->src[b->pos] | (b->src[b->pos + 1] << 8);
			b->pos += 4;     // skip LEN + NLEN
			if (b->pos + blen > b->len || outPos + blen > outCap) return -1;
			for (int i = 0; i < blen; ++i) out[outPos++] = b->src[b->pos++];
		}
		else if (type == 1 || type == 2) {
			Huffman lenHuff, distHuff;
			if (type == 1) {
				// fixed Huffman (RFC-1951 3.2.6)
				unsigned char ll[288], dl[30];
				int i = 0;
				for (; i < 144; ++i) ll[i] = 8;
				for (; i < 256; ++i) ll[i] = 9;
				for (; i < 280; ++i) ll[i] = 7;
				for (; i < 288; ++i) ll[i] = 8;
				for (i = 0; i < 30; ++i) dl[i] = 5;
				buildHuffman(&lenHuff, ll, 288);
				buildHuffman(&distHuff, dl, 30);
			} else {
				// dynamic Huffman (RFC-1951 3.2.7)
				int hlit = getBits(b, 5) + 257;
				int hdist = getBits(b, 5) + 1;
				int hclen = getBits(b, 4) + 4;
				if (hlit > 286 || hdist > 30) return -1;
				static const unsigned char ORDER[19] =
					{16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
				unsigned char clLen[19];
				int i;
				for (i = 0; i < 19; ++i) clLen[i] = 0;
				for (i = 0; i < hclen; ++i) clLen[ORDER[i]] = (unsigned char)getBits(b, 3);
				Huffman clHuff;
				buildHuffman(&clHuff, clLen, 19);

				unsigned char lengths[288 + 30];
				int n = 0;
				while (n < hlit + hdist) {
					int sym = decodeSymbol(b, &clHuff);
					if (sym < 0) return -1;
					if (sym < 16) {
						lengths[n++] = (unsigned char)sym;
					} else if (sym == 16) {
						if (n == 0) return -1;
						int rep = getBits(b, 2) + 3;
						unsigned char prev = lengths[n - 1];
						while (rep-- && n < hlit + hdist) lengths[n++] = prev;
					} else if (sym == 17) {
						int rep = getBits(b, 3) + 3;
						while (rep-- && n < hlit + hdist) lengths[n++] = 0;
					} else { // 18
						int rep = getBits(b, 7) + 11;
						while (rep-- && n < hlit + hdist) lengths[n++] = 0;
					}
				}
				if (b->error) return -1;
				buildHuffman(&lenHuff, lengths, hlit);
				buildHuffman(&distHuff, lengths + hlit, hdist);
			}

			// inflate the codes for this block
			for (;;) {
				int sym = decodeSymbol(b, &lenHuff);
				if (sym < 0) return -1;
				if (sym == 256) break;                 // end of block
				if (sym < 256) {
					if (outPos >= outCap) return -1;
					out[outPos++] = (unsigned char)sym;
				} else {
					sym -= 257;
					if (sym >= 29) return -1;
					int length = LEN_BASE[sym] + getBits(b, LEN_EXTRA[sym]);
					int dsym = decodeSymbol(b, &distHuff);
					if (dsym < 0 || dsym >= 30) return -1;
					int dist = DIST_BASE[dsym] + getBits(b, DIST_EXTRA[dsym]);
					if (b->error || dist > outPos || outPos + length > outCap) return -1;
					for (int i = 0; i < length; ++i) {
						out[outPos] = out[outPos - dist];
						++outPos;
					}
				}
			}
		}
		else {
			return -1;   // type 3 = reserved/error
		}
	}
	return b->error ? -1 : outPos;
}

} // namespace

int rawInflate(const unsigned char* in, int inLen, unsigned char* out, int outCap) {
	if (!in || inLen <= 0 || !out || outCap <= 0) return -1;
	BitReader b;
	b.src = in; b.len = inLen; b.pos = 0; b.bitbuf = 0; b.bitcnt = 0; b.error = false;
	return inflateBlocks(&b, out, outCap);
}

int zlibInflate(const unsigned char* in, int inLen, unsigned char* out, int outCap) {
	if (!in || inLen < 2) return -1;
	// RFC-1950 zlib header: CMF, FLG. CM (low nibble of CMF) must be 8 (deflate).
	int cmf = in[0];
	int flg = in[1];
	if ((cmf & 0x0F) != 8) return -1;            // not deflate
	int header = 2;
	if (flg & 0x20) header += 4;                  // FDICT: a 4-byte preset-dictionary id follows
	if (header >= inLen) return -1;
	return rawInflate(in + header, inLen - header, out, outCap);
}

int gzipInflate(const unsigned char* in, int inLen, unsigned char* out, int outCap) {
	if (!in || inLen < 18) return -1;            // 10-byte header + >=0 deflate + 8-byte trailer
	if (in[0] != 0x1F || in[1] != 0x8B || in[2] != 8) return -1;   // magic + CM=deflate
	int flg = in[3];
	int p = 10;                                   // skip the fixed 10-byte header
	if (flg & 0x04) {                             // FEXTRA: 2-byte XLEN then XLEN bytes
		if (p + 2 > inLen) return -1;
		int xlen = in[p] | (in[p + 1] << 8);
		p += 2 + xlen;
	}
	if (flg & 0x08) { while (p < inLen && in[p] != 0) ++p; ++p; }   // FNAME: zero-terminated
	if (flg & 0x10) { while (p < inLen && in[p] != 0) ++p; ++p; }   // FCOMMENT: zero-terminated
	if (flg & 0x02) p += 2;                       // FHCRC: 2-byte header CRC
	int deflateLen = inLen - p - 8;               // trailing 8 bytes = CRC32 + ISIZE
	if (p < 0 || p >= inLen || deflateLen <= 0) return -1;
	return rawInflate(in + p, deflateLen, out, outCap);
}

// ---- gzip COMPRESSOR (stored/uncompressed DEFLATE blocks) ------------------------------------
// Produces a valid gzip stream that java.util.zip.GZIPInputStream accepts (it validates CRC32+ISIZE),
// without a real Huffman encoder: emit STORED DEFLATE blocks (no compression) + a correct gzip wrapper.
// Used by CompressedStreamTools::compress to ship tile-entity NBT (Packet59) so the server can inflate it.
static unsigned int _crc32(const unsigned char* d, int n) {
	unsigned int c = 0xFFFFFFFFu;
	for (int i = 0; i < n; ++i) {
		c ^= (unsigned int)d[i];
		for (int k = 0; k < 8; ++k) {
			unsigned int mask = (unsigned int)(0u - (c & 1u));   // 0xFFFFFFFF if low bit set, else 0
			c = (c >> 1) ^ (0xEDB88320u & mask);
		}
	}
	return c ^ 0xFFFFFFFFu;
}

// ---- REAL fixed-Huffman DEFLATE compressor (RFC-1951 3.2.6) ------------------------------------
// FIXED BUG save-stored-noncompression-slow: the old gzip/zlib "compressors" emitted STORED (uncompressed)
// blocks, so every RegionFile chunk write was ~82KB of raw NBT -> a full 289-chunk save shoved ~24MB at the
// STFS device and the autosave/pause looked like a hang. This is a proper greedy-LZ77 + fixed-Huffman
// encoder (single BFINAL block, BTYPE=01) that compresses the very repetitive block/light data ~5-15x.
// rawInflate() already decodes fixed+dynamic Huffman, so it round-trips with the existing reader. Falls
// back to STORED if the fixed-Huffman output would overflow (incompressible input can expand ~12.5%).
namespace {
	const int DEF_WSIZE = 32768, DEF_WMASK = 32767;
	const int DEF_HASHSIZE = 1 << 15, DEF_HASHMASK = DEF_HASHSIZE - 1;
	const int DEF_MINMATCH = 3, DEF_MAXMATCH = 258, DEF_MAXCHAIN = 128;
	// length/distance base+extra tables are shared with the decoder above (LEN_BASE/LEN_EXTRA/DIST_BASE/DIST_EXTRA).
	struct DefBW {                                    // DEFLATE bit writer: LSB-first packing
		unsigned char* o; int cap, p; unsigned int buf; int cnt; bool ok;
		DefBW(unsigned char* out, int c) : o(out), cap(c), p(0), buf(0), cnt(0), ok(true) {}
		void put(unsigned int v, int n) {
			buf |= (v << cnt); cnt += n;
			while (cnt >= 8) { if (p >= cap) { ok = false; return; } o[p++] = (unsigned char)(buf & 0xFF); buf >>= 8; cnt -= 8; }
		}
		void finish() { if (cnt > 0) { if (p >= cap) { ok = false; return; } o[p++] = (unsigned char)(buf & 0xFF); buf = 0; cnt = 0; } }
	};
	inline unsigned int defRev(unsigned int c, int n) { unsigned int r = 0; for (int i = 0; i < n; ++i) { r = (r << 1) | (c & 1u); c >>= 1; } return r; }
	inline void defSym(DefBW& w, int sym) {           // fixed lit/len code (MSB-first value), written reversed
		unsigned int code; int len;
		if (sym <= 143)      { code = 0x30u + sym;           len = 8; }
		else if (sym <= 255) { code = 0x190u + (sym - 144); len = 9; }
		else if (sym <= 279) { code = (unsigned int)(sym - 256); len = 7; }
		else                 { code = 0xC0u + (sym - 280); len = 8; }
		w.put(defRev(code, len), len);
	}
}

// Raw fixed-Huffman deflate body (no zlib/gzip wrapper). Returns bytes written, or -1 on overflow.
static int rawDeflateFixed(const unsigned char* in, int inLen, unsigned char* out, int outCap) {
	if (!out || inLen < 0) return -1;
	DefBW w(out, outCap);
	w.put(1, 1); w.put(1, 2);                          // BFINAL=1, BTYPE=01 (fixed Huffman)
	if (inLen == 0) { defSym(w, 256); w.finish(); return w.ok ? w.p : -1; }
	// Reused across calls: world saves run on one thread, so static scratch avoids a 256KB alloc/free per
	// chunk (289/save). Re-initialised to -1 every call, so no cross-call state leaks in.
	static int head[DEF_HASHSIZE];
	static int prev[DEF_WSIZE];
	for (int i = 0; i < DEF_HASHSIZE; ++i) head[i] = -1;
	for (int i = 0; i < DEF_WSIZE; ++i) prev[i] = -1;
	int pos = 0;
	while (pos < inLen && w.ok) {
		int bestLen = 0, bestDist = 0;
		if (pos + DEF_MINMATCH <= inLen) {
			unsigned int h = (((unsigned int)in[pos] << 10) ^ ((unsigned int)in[pos+1] << 5) ^ (unsigned int)in[pos+2]) & DEF_HASHMASK;
			int cand = head[h], chain = DEF_MAXCHAIN;
			int maxLen = inLen - pos; if (maxLen > DEF_MAXMATCH) maxLen = DEF_MAXMATCH;
			while (cand >= 0 && chain-- > 0) {
				int dist = pos - cand;
				if (dist > DEF_WSIZE) break;
				int l = 0; while (l < maxLen && in[cand + l] == in[pos + l]) ++l;
				if (l > bestLen) { bestLen = l; bestDist = dist; if (l >= maxLen) break; }
				cand = prev[cand & DEF_WMASK];
			}
		}
		int adv;
		if (bestLen >= DEF_MINMATCH) {
			int li = 0; while (li < 28 && LEN_BASE[li + 1] <= bestLen) ++li;
			defSym(w, 257 + li);
			if (LEN_EXTRA[li]) w.put((unsigned int)(bestLen - LEN_BASE[li]), LEN_EXTRA[li]);
			int di = 0; while (di < 29 && DIST_BASE[di + 1] <= bestDist) ++di;
			w.put(defRev((unsigned int)di, 5), 5);
			if (DIST_EXTRA[di]) w.put((unsigned int)(bestDist - DIST_BASE[di]), DIST_EXTRA[di]);
			adv = bestLen;
		} else {
			defSym(w, in[pos]);
			adv = 1;
		}
		for (int k = 0; k < adv; ++k) {                // insert hash for every consumed position
			if (pos + DEF_MINMATCH <= inLen) {
				unsigned int h2 = (((unsigned int)in[pos] << 10) ^ ((unsigned int)in[pos+1] << 5) ^ (unsigned int)in[pos+2]) & DEF_HASHMASK;
				prev[pos & DEF_WMASK] = head[h2]; head[h2] = pos;
			}
			++pos;
		}
	}
	defSym(w, 256);                                    // end-of-block
	w.finish();
	return w.ok ? w.p : -1;
}

// STORED (uncompressed) DEFLATE blocks -- fallback when fixed-Huffman would overflow. Returns body length or -1.
static int _storedBlocks(const unsigned char* in, int inLen, unsigned char* out, int outCap) {
	int p = 0, off = 0;
	do {
		int chunk = inLen - off; if (chunk > 65535) chunk = 65535;
		int isFinal = (off + chunk >= inLen) ? 1 : 0;
		if (p + 5 + chunk > outCap) return -1;
		out[p++] = (unsigned char)isFinal;
		out[p++] = (unsigned char)(chunk & 0xFF); out[p++] = (unsigned char)((chunk >> 8) & 0xFF);
		unsigned int nlen = (unsigned int)(~chunk);
		out[p++] = (unsigned char)(nlen & 0xFF); out[p++] = (unsigned char)((nlen >> 8) & 0xFF);
		for (int i = 0; i < chunk; ++i) out[p++] = in[off + i];
		off += chunk;
	} while (off < inLen);
	return p;
}

int gzipDeflateStored(const unsigned char* in, int inLen, unsigned char* out, int outCap) {
	if (!out || inLen < 0) return -1;
	if (10 > outCap) return -1;
	out[0] = 0x1F; out[1] = 0x8B; out[2] = 0x08; out[3] = 0x00;           // magic, CM=deflate, FLG=0
	out[4] = 0; out[5] = 0; out[6] = 0; out[7] = 0;                       // MTIME = 0
	out[8] = 0; out[9] = 0xFF;                                            // XFL=0, OS=255
	int body = rawDeflateFixed(in, inLen, out + 10, outCap - 10 - 8);     // reserve 10 header + 8 trailer
	if (body < 0) { body = _storedBlocks(in, inLen, out + 10, outCap - 10 - 8); if (body < 0) return -1; }
	int p = 10 + body;
	unsigned int crc = _crc32(in, inLen);
	out[p++] = (unsigned char)(crc & 0xFF);  out[p++] = (unsigned char)((crc >> 8) & 0xFF);
	out[p++] = (unsigned char)((crc >> 16) & 0xFF); out[p++] = (unsigned char)((crc >> 24) & 0xFF);
	unsigned int isize = (unsigned int)inLen;
	out[p++] = (unsigned char)(isize & 0xFF);  out[p++] = (unsigned char)((isize >> 8) & 0xFF);
	out[p++] = (unsigned char)((isize >> 16) & 0xFF); out[p++] = (unsigned char)((isize >> 24) & 0xFF);
	return p;
}

// Adler-32 (zlib trailer checksum). Two 16-bit sums mod 65521, packed (b<<16)|a.
static unsigned int _adler32(const unsigned char* d, int n) {
	unsigned int a = 1, b = 0;
	for (int i = 0; i < n; ++i) { a = (a + d[i]) % 65521u; b = (b + a) % 65521u; }
	return (b << 16) | a;
}

// zlib (RFC-1950) compressor using STORED (uncompressed) DEFLATE blocks: 2-byte zlib header + the same STORED
// blocks gzipDeflateStored emits + a 4-byte BIG-ENDIAN adler32. A valid zlib stream that zlibInflate (and
// java.util.zip.Inflater) accept -- used by DeflaterOutputStream for RegionFile chunk writes (version-2 = zlib).
// No Huffman encoder; output is larger than real deflate but round-trips. Worst case = inLen + 5*ceil(inLen/65535)
// + 6; size the out buffer accordingly. Returns bytes written or -1 on overflow.
int zlibDeflateStored(const unsigned char* in, int inLen, unsigned char* out, int outCap) {
	if (!out || inLen < 0) return -1;
	if (2 > outCap) return -1;
	out[0] = 0x78; out[1] = 0x01;                                        // CMF=deflate/32K window, FLG check (=0 mod 31)
	int body = rawDeflateFixed(in, inLen, out + 2, outCap - 2 - 4);      // reserve 2 header + 4 adler
	if (body < 0) { body = _storedBlocks(in, inLen, out + 2, outCap - 2 - 4); if (body < 0) return -1; }
	int p = 2 + body;
	unsigned int ad = _adler32(in, inLen);                               // BIG-ENDIAN in the zlib trailer
	out[p++] = (unsigned char)((ad >> 24) & 0xFF); out[p++] = (unsigned char)((ad >> 16) & 0xFF);
	out[p++] = (unsigned char)((ad >> 8)  & 0xFF); out[p++] = (unsigned char)(ad & 0xFF);
	return p;
}
