#ifndef BYTEARRAYOUTPUTSTREAM_H
#define BYTEARRAYOUTPUTSTREAM_H
#include "OutputStream.h"
#include "JArray.h"

// Canonical java.io.ByteArrayOutputStream -- an in-memory growable byte buffer. [360] NO XDK EQUIVALENT --
// hand-implemented: pure C++ (capacity-doubling JArray). FIXED BUG chunk-buffer-swallows-bytes: the stub's
// write() was a no-op, so RegionFileChunkBuffer (which extends this and writes `this->buf`/`this->count` to the
// RegionFile on close) buffered NOTHING -> chunk saves wrote empty sectors. `buf` is the backing array (may be
// larger than `count`, the used length) -- exactly the Java fields RegionFileChunkBuffer reads.
class ByteArrayOutputStream : public OutputStream {
public:
	JArray<unsigned char> buf;   // backing store (capacity >= count)
	int count;                   // used length
	ByteArrayOutputStream() : buf(32), count(0) {}
	ByteArrayOutputStream(int size) : buf(size > 0 ? size : 32), count(0) {}
	void _ensure(int need) {
		if (need <= buf.length) return;
		int cap = buf.length > 0 ? buf.length : 32;
		while (cap < need) cap *= 2;
		JArray<unsigned char> nb(cap);
		for (int i = 0; i < count; ++i) nb[i] = buf[i];
		// FIXED BUG bytearrayoutputstream-grow-leak: JArray has no destructor (shallow-copy value type, never
		// frees), so the old backing buffer leaked on every grow. Mid-write nothing else aliases `buf`, so
		// free it here. (Part of the RegionFileChunkBuffer chunk-save leak -- see close() in that class.)
		unsigned char* _old = (unsigned char*)buf.data();
		buf = nb;
		delete[] _old;
	}
	void write(int b) { _ensure(count + 1); buf[count++] = (unsigned char)(b & 0xFF); }
	void write(const unsigned char* b, int len) { _ensure(count + len); for (int i = 0; i < len; ++i) buf[count++] = b[i]; }
	void flush() {}
	void close() {}
	int size() { return count; }
	void reset() { count = 0; }
	JArray<unsigned char> toByteArray() {
		JArray<unsigned char> r(count);
		for (int i = 0; i < count; ++i) r[i] = buf[i];
		return r;
	}
};
#endif // BYTEARRAYOUTPUTSTREAM_H
