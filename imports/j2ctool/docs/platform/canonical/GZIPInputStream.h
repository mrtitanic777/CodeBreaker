#ifndef GZIPINPUTSTREAM_H
#define GZIPINPUTSTREAM_H
#include <vector>
#include "InputStream.h"
#include "Inflate.h"

// Canonical java.util.zip.GZIPInputStream -- drains the wrapped stream, gzip-decompresses it up front, then
// serves the plaintext. [360] NO XDK EQUIVALENT -- hand-implemented: puff.c gzipInflate (Inflate.cpp). The XDK's
// XMemDecompress is LZX, a DIFFERENT format. FIXED BUG world-load-empty-nbt: this was a no-op stub, so level.dat /
// player.dat / chunk NBT never decompressed -> world load read an empty compound.
class GZIPInputStream : public InputStream {
	std::vector<unsigned char> _out;
	int _pos;
public:
	GZIPInputStream(InputStream* in) : _pos(0) {
		std::vector<unsigned char> comp;
		if (in) { unsigned char t[4096]; int n;
			while ((n = in->read(t, 0, (int)sizeof(t))) > 0) comp.insert(comp.end(), t, t + n); }
		if (!comp.empty()) {
			int cap = (int)comp.size() * 8 + 4096;          // decompressed size unknown -> grow-and-retry
			for (int tries = 0; tries < 10; ++tries) {
				_out.resize(cap);
				int m = gzipInflate(&comp[0], (int)comp.size(), &_out[0], cap);
				if (m >= 0) { _out.resize(m); break; }       // -1 = overflow (or bad data) -> bigger buffer
				cap *= 2; _out.clear();
			}
		}
	}
	int read() { return (_pos < (int)_out.size()) ? _out[_pos++] : -1; }
	int read(unsigned char* b, int off, int len) {
		int n = 0; while (n < len && _pos < (int)_out.size()) b[off + n++] = _out[_pos++];
		return (n > 0) ? n : -1;
	}
	int available() { return (int)_out.size() - _pos; }
	void close() {}
};
#endif // GZIPINPUTSTREAM_H
