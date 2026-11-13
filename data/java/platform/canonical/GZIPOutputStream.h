#ifndef GZIPOUTPUTSTREAM_H
#define GZIPOUTPUTSTREAM_H
#include <vector>
#include "OutputStream.h"
#include "Inflate.h"

// Canonical java.util.zip.GZIPOutputStream -- buffers everything written, gzip-wraps it on finish()/close(), and
// forwards the gzip stream to the wrapped OutputStream. [360] NO XDK EQUIVALENT -- hand-implemented:
// gzipDeflateStored (Inflate.cpp) -- a valid gzip stream built from STORED (uncompressed) DEFLATE blocks + correct
// CRC32/ISIZE, which java.util.zip.GZIPInputStream (and our GZIPInputStream) accept. No Huffman encoder needed;
// output is larger than real deflate but level.dat/player.dat are tiny. FIXED BUG world-never-persists: the stub
// dropped every byte, so CompressedStreamTools wrote nothing to disk.
class GZIPOutputStream : public OutputStream {
	OutputStream* _out;
	std::vector<unsigned char> _buf;
	bool _flushed;
	void _emit() {
		if (_flushed) return;
		_flushed = true;
		if (!_out || _buf.empty()) return;
		int cap = (int)_buf.size() + 5 * ((int)_buf.size() / 65535 + 1) + 32;   // worst-case STORED gzip size
		std::vector<unsigned char> gz(cap);
		int m = gzipDeflateStored(&_buf[0], (int)_buf.size(), &gz[0], cap);
		if (m > 0) _out->write(&gz[0], m);
		std::vector<unsigned char>().swap(_buf);   // FIXED BUG save-nogc-heap-leak: FREE the buffer's CAPACITY (clear() keeps it). On the no-GC heap the stream object leaks, so a mere clear() would keep the whole serialized level.dat alive forever -> heap fragmentation -> BaseHeap::Alloc failed after a few autosaves.
	}
public:
	GZIPOutputStream(OutputStream* out, int /*size*/ = 0) : _out(out), _flushed(false) {}
	void write(int b) { _buf.push_back((unsigned char)(b & 0xFF)); }
	void write(const unsigned char* b, int len) { _buf.insert(_buf.end(), b, b + len); }
	void flush() {}                                  // gzip must emit as one stream -> defer to finish/close
	void finish() { _emit(); if (_out) _out->flush(); }
	void close() { _emit(); if (_out) _out->close(); }
};
#endif // GZIPOUTPUTSTREAM_H
