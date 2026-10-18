#ifndef BUFFEREDOUTPUTSTREAM_H
#define BUFFEREDOUTPUTSTREAM_H
#include "OutputStream.h"

// Canonical java.io.BufferedOutputStream -- passthrough wrapper. The real per-packet buffering happens upstream
// in DataOutputStream (accumulates a packet then flush()es it), so a straight forward-to-the-wrapped-stream is
// correct AND needed by the netcode: NetworkManager does
//   new DataOutputStream(new BufferedOutputStream(socket.getOutputStream(), 5120)).
// FIXED BUG buffered-out-swallows-packets: this used to be a no-op stub with no _out, so EVERY outbound byte was
// dropped -> multiplayer sent nothing (stuck on "Connecting..."). Single ctor with a default size covers the
// 1- and 2-arg forms the game emits.

class BufferedOutputStream : public OutputStream {
	OutputStream* _out;
public:
	BufferedOutputStream(OutputStream* out, int /*size*/ = 0) : _out(out) {}
	void write(int b) { if (_out) _out->write(b); }
	void write(const unsigned char* b, int len) { if (_out) _out->write(b, len); }
	void flush() { if (_out) _out->flush(); }
	void close() { if (_out) _out->close(); }
};
#endif // BUFFEREDOUTPUTSTREAM_H
