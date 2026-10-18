#ifndef BYTEARRAYINPUTSTREAM_H
#define BYTEARRAYINPUTSTREAM_H
#include "InputStream.h"
#include "JArray.h"

// Canonical java.io.ByteArrayInputStream -- reads from an in-memory byte array. [360] NO XDK EQUIVALENT --
// hand-implemented: pure C++. FIXED BUG chunk-read-empty: the stub returned nothing, so the RegionFile chunk
// read path (RandomAccessFile raw bytes -> ByteArrayInputStream -> InflaterInputStream -> NBT) fed the
// decompressor an empty buffer -> loaded chunks came back as air.
class ByteArrayInputStream : public InputStream {
	JArray<unsigned char> _buf;
	int _pos;
	int _end;
public:
	ByteArrayInputStream(JArray<unsigned char> b) : _buf(b), _pos(0), _end(b.length) {}
	ByteArrayInputStream(JArray<unsigned char> b, int off, int len) : _buf(b), _pos(off), _end(off + len) {}
	int read() { return (_pos < _end) ? _buf[_pos++] : -1; }
	int read(unsigned char* b, int off, int len) {
		int n = 0; while (n < len && _pos < _end) b[off + n++] = _buf[_pos++];
		return (n > 0) ? n : -1;
	}
	int available() { return _end - _pos; }
	long long skip(long long k) { int a = _end - _pos; if (k > a) k = a; _pos += (int)k; return k; }
	void close() {}
};
#endif // BYTEARRAYINPUTSTREAM_H
