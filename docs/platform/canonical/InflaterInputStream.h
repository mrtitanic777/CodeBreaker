#ifndef INFLATERINPUTSTREAM_H
#define INFLATERINPUTSTREAM_H
#include <vector>
#include "InputStream.h"
#include "Inflate.h"

// Canonical java.util.zip.InflaterInputStream -- zlib (RFC-1950) decompress of the wrapped stream, up front.
// [360] NO XDK EQUIVALENT -- hand-implemented: puff.c zlibInflate (Inflate.cpp). Used by the RegionFile chunk
// read path (version-2 = zlib, the default). FIXED BUG region-chunk-no-decompress: the stub returned nothing.
// (Second ctor arg is a java.util.zip.Inflater* in Java; ignored here -- we always zlib-inflate.)
class InflaterInputStream : public InputStream {
	std::vector<unsigned char> _out;
	int _pos;
	void _init(InputStream* in) {
		std::vector<unsigned char> comp;
		if (in) { unsigned char t[4096]; int n;
			while ((n = in->read(t, 0, (int)sizeof(t))) > 0) comp.insert(comp.end(), t, t + n); }
		if (!comp.empty()) {
			int cap = (int)comp.size() * 8 + 4096;
			for (int tries = 0; tries < 10; ++tries) {
				_out.resize(cap);
				int m = zlibInflate(&comp[0], (int)comp.size(), &_out[0], cap);
				if (m >= 0) { _out.resize(m); break; }
				cap *= 2; _out.clear();
			}
		}
	}
public:
	InflaterInputStream(InputStream* in) : _pos(0) { _init(in); }
	template<class INF> InflaterInputStream(InputStream* in, INF*) : _pos(0) { _init(in); }
	int read() { return (_pos < (int)_out.size()) ? _out[_pos++] : -1; }
	int read(unsigned char* b, int off, int len) {
		int n = 0; while (n < len && _pos < (int)_out.size()) b[off + n++] = _out[_pos++];
		return (n > 0) ? n : -1;
	}
	int available() { return (int)_out.size() - _pos; }
	void close() {}
};
#endif // INFLATERINPUTSTREAM_H
