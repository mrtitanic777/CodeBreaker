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

} // namespace ImageIO
