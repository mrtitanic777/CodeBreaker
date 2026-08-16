#ifndef DATAINPUT_H
#define DATAINPUT_H
#include <string>
#include <string.h>          // memcpy (readFloat/readDouble bit-cast)
#include "JArray.h"

// Canonical java.io.DataInput -- BIG-ENDIAN byte codec (NBT-faithful). Verbatim from the FileIO reference.
// !!! The byte-at-a-time shifts are LOAD-BEARING on PPC (Xbox 360 is big-endian). NEVER memcpy/cast a multi-byte
// value over a serialized buffer -- it byte-swaps vs the Java/PC wire format. Floats/doubles are safe ONLY because
// the int/long is assembled big-endian FIRST and then bit-cast.

class DataInput {
public:
	const unsigned char* data;
	int length;
	int pos;
	DataInput() : data(0), length(0), pos(0) {}
	DataInput(const unsigned char* d, int len) : data(d), length(len), pos(0) {}

	virtual int readUnsignedByte() { return (pos < length) ? data[pos++] : 0; }   // VIRTUAL so the streaming
	// network DataInputStream can pull bytes from a socket on demand; all readInt/readShort/etc. build on this.
	signed char readByte()  { return (signed char)readUnsignedByte(); }
	bool readBoolean()      { return readUnsignedByte() != 0; }
	short readShort()       { int h = readUnsignedByte(), l = readUnsignedByte(); return (short)((h << 8) | l); }
	int readUnsignedShort() { int h = readUnsignedByte(), l = readUnsignedByte(); return (h << 8) | l; }
	char readChar()         { return (char)readUnsignedShort(); }
	int readInt() {
		int a = readUnsignedByte(), b = readUnsignedByte(), c = readUnsignedByte(), d = readUnsignedByte();
		return (a << 24) | (b << 16) | (c << 8) | d;
	}
	long long readLong() {
		long long v = 0; for (int i = 0; i < 8; ++i) v = (v << 8) | (long long)readUnsignedByte(); return v;
	}
	float readFloat()   { int i = readInt(); float f; memcpy(&f, &i, 4); return f; }     // bit-cast AFTER BE assembly
	double readDouble() { long long l = readLong(); double d; memcpy(&d, &l, 8); return d; }
	void readFully(unsigned char* dst, int len) { for (int i = 0; i < len; ++i) dst[i] = (unsigned char)readUnsignedByte(); }
	void readFully(JArray<unsigned char>& b) { for (int i = 0; i < b.length; ++i) b[i] = (unsigned char)readUnsignedByte(); }
	std::string readUTF() {                                  // 2-byte length prefix + modified-UTF8
		int n = readUnsignedShort(); std::string s; s.reserve(n);
		for (int i = 0; i < n; ++i) s.push_back((char)readUnsignedByte());
		return s;
	}
};
#endif // DATAINPUT_H
