#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

// Canonical java.io.OutputStream -- the ONE definition (FileIO.h and every *OutputStream subclass include this).
// FIXED BUG outputstream-odr (LINK C4743): FileIO.h defined a rich OutputStream (write(int)/write(ptr,len)) while
// this header was a 3-virtual variadic no-op stub -> two different vtable sizes across TUs -> the linker folded
// them and SocketOutputStream::write mis-dispatched, so the netcode sent nothing. Make this the single real
// definition (mirrors InputStream.h's canonical-header ODR fix); FileIO.h now #includes it instead of redefining.

class OutputStream {
public:
	virtual ~OutputStream() {}
	virtual void write(int b) { (void)b; }
	virtual void write(const unsigned char* b, int len) { for (int i = 0; i < len; ++i) write((int)b[i]); }
	virtual void flush() {}
	virtual void close() {}
};
#endif // OUTPUTSTREAM_H
