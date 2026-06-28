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
