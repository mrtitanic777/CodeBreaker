// ImageIO on the PC (see ImageIO.reference.h): a PNG decoder -- 8-bit greyscale, RGB, palette, grey+alpha
// and RGBA, non-interlaced, every scanline filter -- over the layer's inflater.
#include "stdafx.h"
#include "ImageIO.h"
#include "Inflate.h"
#include "URL.h"
#include "File.h"
#include <vector>
#include <cstring>
#include <cstdio>

InputStream* J2X_getResourceAsStream(const char* name);   // the runtime's resource loader

namespace {

unsigned be32(const unsigned char* p) { return ((unsigned)p[0] << 24) | ((unsigned)p[1] << 16) | ((unsigned)p[2] << 8) | p[3]; }

int paeth(int a, int b, int c) {
	int p = a + b - c, pa = p > a ? p - a : a - p, pb = p > b ? p - b : b - p, pc = p > c ? p - c : c - p;
	return (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c);
}

std::vector<unsigned char> slurp(InputStream* in) {
	std::vector<unsigned char> bytes;
	if (!in) return bytes;
	unsigned char buf[8192];
	for (;;) {
		int n = in->read(buf, 0, (int)sizeof(buf));
		if (n <= 0) break;
		bytes.insert(bytes.end(), buf, buf + n);
	}
	return bytes;
}

} // namespace

namespace ImageIO {

BufferedImage* decode(const unsigned char* d, unsigned size) {
	static const unsigned char sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
	if (size < 8 || std::memcmp(d, sig, 8) != 0) return NULL;
	unsigned w = 0, h = 0, pos = 8;
	int depth = 0, ctype = 0, interlace = 0;
	std::vector<unsigned char> idat, plte, trns;
	while (pos + 8 <= size) {
		unsigned len = be32(d + pos);
		const unsigned char* type = d + pos + 4;
		const unsigned char* data = d + pos + 8;
		if (pos + 12 + len > size) break;
		if (!std::memcmp(type, "IHDR", 4) && len >= 13) {
			w = be32(data); h = be32(data + 4); depth = data[8]; ctype = data[9]; interlace = data[12];
		} else if (!std::memcmp(type, "PLTE", 4)) {
			plte.assign(data, data + len);
		} else if (!std::memcmp(type, "tRNS", 4)) {
			trns.assign(data, data + len);
		} else if (!std::memcmp(type, "IDAT", 4)) {
			idat.insert(idat.end(), data, data + len);
		} else if (!std::memcmp(type, "IEND", 4)) {
			break;
		}
		pos += 12 + len;
	}
	if (!w || !h || depth != 8 || interlace || idat.empty()) {
		std::fprintf(stderr, "[imageio] unsupported PNG (%ux%u depth %d type %d interlace %d)\n", w, h, depth, ctype, interlace);
		return NULL;
	}
	int ch = ctype == 0 ? 1 : ctype == 2 ? 3 : ctype == 3 ? 1 : ctype == 4 ? 2 : ctype == 6 ? 4 : 0;
	if (!ch) return NULL;
	unsigned stride = w * ch;
	std::vector<unsigned char> raw((size_t)(stride + 1) * h);
	if (zlibInflate(&idat[0], (int)idat.size(), &raw[0], (int)raw.size()) != (int)raw.size()) {
		std::fprintf(stderr, "[imageio] PNG inflate failed\n");
		return NULL;
	}
	std::vector<unsigned char> px((size_t)stride * h);
	for (unsigned y = 0; y < h; ++y) {                   // undo the scanline filters
		const unsigned char* src = &raw[(size_t)y * (stride + 1)];
		int f = src[0];
		++src;
		unsigned char* row = &px[(size_t)y * stride];
		const unsigned char* up = y ? row - stride : 0;
		for (unsigned i = 0; i < stride; ++i) {
			int a = i >= (unsigned)ch ? row[i - ch] : 0, b = up ? up[i] : 0, c = (up && i >= (unsigned)ch) ? up[i - ch] : 0;
			int v = src[i];
			switch (f) {
			case 1: v += a; break;
			case 2: v += b; break;
			case 3: v += (a + b) >> 1; break;
			case 4: v += paeth(a, b, c); break;
			default: break;
			}
			row[i] = (unsigned char)v;
		}
	}
	BufferedImage* img = new BufferedImage((int)w, (int)h);
	for (unsigned i = 0, n = w * h; i < n; ++i) {
		const unsigned char* p = &px[(size_t)i * ch];
		unsigned r, g, b, a = 255;
		if (ctype == 0) { r = g = b = p[0]; }
		else if (ctype == 2) { r = p[0]; g = p[1]; b = p[2]; }
		else if (ctype == 3) {
			unsigned k = p[0];
			r = 3 * k + 2 < plte.size() ? plte[3 * k] : 0;
			g = 3 * k + 2 < plte.size() ? plte[3 * k + 1] : 0;
			b = 3 * k + 2 < plte.size() ? plte[3 * k + 2] : 0;
			a = k < trns.size() ? trns[k] : 255;
		}
		else if (ctype == 4) { r = g = b = p[0]; a = p[1]; }
		else { r = p[0]; g = p[1]; b = p[2]; a = p[3]; }
		img->pixels[(int)i] = (int)((a << 24) | (r << 16) | (g << 8) | b);
	}
	return img;
}

BufferedImage* read(InputStream* in) {
	std::vector<unsigned char> bytes = slurp(in);
	return bytes.empty() ? NULL : decode(&bytes[0], (unsigned)bytes.size());
}

BufferedImage* read(URL* u) {
	return u ? read(J2X_getResourceAsStream(u->j2xResourcePath().c_str())) : NULL;
}

BufferedImage* read(File* f) {
	if (!f) return NULL;
	FILE* fp = std::fopen(f->path.c_str(), "rb");
	if (!fp) return NULL;
	std::vector<unsigned char> bytes;
	unsigned char buf[8192];
	size_t n;
	while ((n = std::fread(buf, 1, sizeof(buf), fp)) > 0) bytes.insert(bytes.end(), buf, buf + n);
	std::fclose(fp);
	return bytes.empty() ? NULL : decode(&bytes[0], (unsigned)bytes.size());
}

namespace {
unsigned crc32(const unsigned char* p, size_t n, unsigned c = 0xFFFFFFFFu) {
	static unsigned table[256];
	static bool init = false;
	if (!init) {
		for (unsigned i = 0; i < 256; ++i) {
			unsigned v = i;
			for (int k = 0; k < 8; ++k) v = (v & 1) ? 0xEDB88320u ^ (v >> 1) : v >> 1;
			table[i] = v;
		}
		init = true;
	}
	for (size_t i = 0; i < n; ++i) c = table[(c ^ p[i]) & 0xFF] ^ (c >> 8);
	return c;
}
void put32(std::vector<unsigned char>& o, unsigned v) {
	o.push_back((unsigned char)(v >> 24)); o.push_back((unsigned char)(v >> 16));
	o.push_back((unsigned char)(v >> 8)); o.push_back((unsigned char)v);
}
void chunk(std::vector<unsigned char>& o, const char* type, const std::vector<unsigned char>& d) {
	put32(o, (unsigned)d.size());
	size_t start = o.size();
	o.insert(o.end(), type, type + 4);
	o.insert(o.end(), d.begin(), d.end());
	put32(o, crc32(&o[start], o.size() - start) ^ 0xFFFFFFFFu);
}
}

// PNG, RGBA 8-bit, stored (uncompressed) deflate blocks -- a valid file every decoder reads
bool write(BufferedImage* img, const std::string& format, File* f) {
	if (!img || !f || img->w <= 0 || img->h <= 0) return false;
	(void)format;
	std::vector<unsigned char> raw;
	raw.reserve((size_t)(img->w * 4 + 1) * img->h);
	for (int y = 0; y < img->h; ++y) {
		raw.push_back(0);
		for (int x = 0; x < img->w; ++x) {
			unsigned v = (unsigned)img->pixels[y * img->w + x];
			unsigned a = v >> 24;
			if (a == 0) a = 255;                                   // TYPE_INT_RGB: no alpha -> opaque
			raw.push_back((unsigned char)(v >> 16)); raw.push_back((unsigned char)(v >> 8));
			raw.push_back((unsigned char)v); raw.push_back((unsigned char)a);
		}
	}
	std::vector<unsigned char> z(raw.size() + 5 * (raw.size() / 65535 + 1) + 16);
	int zn = zlibDeflateStored(&raw[0], (int)raw.size(), &z[0], (int)z.size());
	if (zn < 0) return false;
	z.resize((size_t)zn);
	std::vector<unsigned char> out;
	static const unsigned char sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
	out.insert(out.end(), sig, sig + 8);
	std::vector<unsigned char> ihdr;
	put32(ihdr, (unsigned)img->w); put32(ihdr, (unsigned)img->h);
	ihdr.push_back(8); ihdr.push_back(6); ihdr.push_back(0); ihdr.push_back(0); ihdr.push_back(0);
	chunk(out, "IHDR", ihdr);
	chunk(out, "IDAT", z);
	chunk(out, "IEND", std::vector<unsigned char>());
	FILE* fp = std::fopen(f->path.c_str(), "wb");
	if (!fp) return false;
	bool ok = std::fwrite(&out[0], 1, out.size(), fp) == out.size();
	std::fclose(fp);
	return ok;
}

} // namespace ImageIO
