#ifndef DATAOUTPUTSTREAM_H
#define DATAOUTPUTSTREAM_H
#include "OutputStream.h"
#include "DataOutput.h"

// Canonical java.io.DataOutputStream -- the DataOutput byte buffer plus a flush()/close() that PUSHES the
// accumulated bytes to the wrapped stream. Network code flushes per packet so packets actually send mid-session.

class DataOutputStream : public DataOutput {
	OutputStream* _out;
public:
	DataOutputStream(OutputStream* out) : _out(out) {}
	int size() { return (int)buffer.size(); }
	void flush() {
		if (_out) {
			if (!buffer.empty()) { _out->write(&buffer[0], (int)buffer.size()); buffer.clear(); }
			_out->flush();
		}
	}
	void close() {
		if (_out) { flush(); _out->close(); }
		std::vector<unsigned char>().swap(buffer);   // FIXED BUG save-nogc-heap-leak: release the DataOutput byte buffer's CAPACITY. This holds the FULL serialized NBT (level.dat, or ~80KB per chunk on the RegionFile save path); on the no-GC heap the stream object leaks, so without this the whole buffer stays resident -> heap fragmentation crash after a few autosaves.
	}
};
#endif // DATAOUTPUTSTREAM_H
