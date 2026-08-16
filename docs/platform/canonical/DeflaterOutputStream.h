#ifndef DEFLATEROUTPUTSTREAM_H
#define DEFLATEROUTPUTSTREAM_H
#include <vector>
#include "OutputStream.h"
#include "Inflate.h"

// Canonical java.util.zip.DeflaterOutputStream -- buffers writes, zlib-compresses on finish()/close(), forwards
// to the wrapped OutputStream. [360] NO XDK EQUIVALENT -- hand-implemented: zlibDeflateStored (Inflate.cpp), a
// valid zlib stream (2-byte header + STORED blocks + adler32) that InflaterInputStream/java.util.zip.Inflater
// read back. Used by the RegionFile chunk-write path (version-2 = zlib). FIXED BUG chunk-never-persists: the
// stub dropped every byte, so chunk saves wrote nothing (and could never round-trip on load).
class DeflaterOutputStream : public OutputStream {
	OutputStream* _out;
	std::vector<unsigned char> _buf;
	bool _flushed;
	void _emit() {
		if (_flushed) return;
		_flushed = true;
		if (!_out || _buf.empty()) return;
		int cap = (int)_buf.size() + 5 * ((int)_buf.size() / 65535 + 1) + 8;   // worst-case zlib STORED size
		std::vector<unsigned char> z(cap);
		int m = zlibDeflateStored(&_buf[0], (int)_buf.size(), &z[0], cap);
		if (m > 0) _out->write(&z[0], m);
		std::vector<unsigned char>().swap(_buf);   // FIXED BUG save-nogc-heap-leak: FREE the buffer's CAPACITY (clear() keeps it). The chunk-save path (RegionFile, version-2 zlib) runs this PER CHUNK -- ~80KB of uncompressed chunk each -> ~268 chunks/save. On the no-GC heap the stream leaks, so keeping capacity = tens of MB leaked per autosave -> BaseHeap::Alloc failed.
	}
public:
	DeflaterOutputStream(OutputStream* out) : _out(out), _flushed(false) {}
	template<class D> DeflaterOutputStream(OutputStream* out, D*) : _out(out), _flushed(false) {}   // (out, Deflater)
	void write(int b) { _buf.push_back((unsigned char)(b & 0xFF)); }
	void write(const unsigned char* b, int len) { _buf.insert(_buf.end(), b, b + len); }
	void flush() {}                                  // deflate must emit one stream -> defer to finish/close
	void finish() { _emit(); if (_out) _out->flush(); }
	void close() { _emit(); if (_out) _out->close(); }
};
#endif // DEFLATEROUTPUTSTREAM_H
