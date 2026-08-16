#ifndef DATAINPUTSTREAM_H
#define DATAINPUTSTREAM_H
#include <vector>
#include "JArray.h"
#include "InputStream.h"
#include "DataInput.h"

// Canonical java.io.DataInputStream -- the DataInput byte codec plus a ctor that moves bytes from the wrapped
// stream. Two modes: FILE (drain the whole stream up front, then parse) and STREAM (network: pull on demand,
// because a socket never EOFs). FIXED BUG net-datainputstream-drains-socket: the 1-arg ctor drains until EOF,
// which never comes on a socket -> hang; NetworkManager uses the 2-arg streaming ctor.

class DataInputStream : public DataInput {
	std::vector<unsigned char> _owned;
	InputStream* _src;                 // STREAM mode: the socket source (may be NULL if the connect failed)
	bool _streaming;                   // true -> network stream mode (read on demand); false -> file/drained mode
	unsigned char _rbuf[4096]; int _rlen; int _rpos;
public:
	// FILE mode: drain the whole stream up front, then parse the buffer.
	DataInputStream(InputStream* in) : _src(0), _streaming(false), _rlen(0), _rpos(0) {
		if (in) { unsigned char tmp[4096]; int n;
			while ((n = in->read(tmp, 0, (int)sizeof(tmp))) > 0) _owned.insert(_owned.end(), tmp, tmp + n); }
		data = _owned.empty() ? (const unsigned char*)0 : &_owned[0];
		length = (int)_owned.size(); pos = 0;
	}
	// STREAM mode (network): DO NOT drain (a socket never EOFs) -- pull bytes on demand. The bool selects it.
	DataInputStream(InputStream* in, bool /*streaming*/) : _src(in), _streaming(true), _rlen(0), _rpos(0) {
		data = 0; length = 0; pos = 0;
	}
	virtual int readUnsignedByte() {
		if (_streaming) {                              // network: refill from the socket (blocking recv)
			if (_src == 0) return -1;                  // dead/failed socket -> EOF (caller stops; NOT the file
			                                           // path's 0, which would spin forever building empty packets)
			if (_rpos >= _rlen) { _rlen = _src->read(_rbuf, 0, (int)sizeof(_rbuf)); _rpos = 0;
				if (_rlen <= 0) { _rlen = 0; return -1; } }   // <=0 = peer closed/error; -1 signals end of stream
			return _rbuf[_rpos++];
		}
		return (pos < length) ? data[pos++] : 0;       // file mode (base behavior)
	}
	int read() { return readUnsignedByte(); }
	// java.io.DataInputStream.read(byte[]) -> count read, or -1 at EOF (used by the resource fetcher)
	int read(JArray<unsigned char>& b) {
		int n = 0; for (; n < b.length && pos < length; ++n) b[n] = data[pos++];
		return (n > 0) ? n : (b.length == 0 ? 0 : -1);
	}
	void close() {}
};
#endif // DATAINPUTSTREAM_H
