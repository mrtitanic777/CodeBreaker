#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H
#include <string>
#include <string.h>          // memcpy (writeFloat/writeDouble bit-cast)
#include <vector>
#include "JArray.h"

// Canonical java.io.DataOutput -- BIG-ENDIAN byte codec (NBT-faithful). Verbatim from the FileIO reference.
// See DataInput.h banner: the byte-at-a-time emit is LOAD-BEARING on the big-endian 360; do not "optimize".

class DataOutput {
public:
	std::vector<unsigned char> buffer;
	void writeByte(int v)    { buffer.push_back((unsigned char)(v & 0xFF)); }
	void writeBoolean(bool v){ writeByte(v ? 1 : 0); }
	void writeShort(int v)   { writeByte(v >> 8); writeByte(v); }
	void writeChar(int v)    { writeShort(v); }
	// java.io.DataOutput.writeChars(String): each char as 2 BE bytes (UTF-16BE). Packet::writeString uses it, so
	// the netcode won't compile without it. Accepts JavaString via its implicit std::string conversion.
	void writeChars(const std::string& s) { for (size_t i = 0; i < s.size(); ++i) writeChar((int)(unsigned char)s[i]); }
	void writeInt(int v)     { writeByte(v >> 24); writeByte(v >> 16); writeByte(v >> 8); writeByte(v); }
	void writeLong(long long v) { for (int i = 56; i >= 0; i -= 8) writeByte((int)(v >> i)); }
	void writeFloat(float v)    { int i;       memcpy(&i, &v, 4); writeInt(i); }          // bit-cast THEN BE emit
	void writeDouble(double v)  { long long l; memcpy(&l, &v, 8); writeLong(l); }
	void write(int v) { writeByte(v); }                      // java.io.DataOutput.write(int) -> one byte
	void write(const unsigned char* d, int len) { for (int i = 0; i < len; ++i) buffer.push_back(d[i]); }
	void write(const JArray<unsigned char>& b) { for (int i = 0; i < b.length; ++i) buffer.push_back((unsigned char)b[i]); }
	void write(const JArray<unsigned char>& b, int off, int len) { for (int i = 0; i < len; ++i) buffer.push_back((unsigned char)b[off + i]); }
	void writeUTF(const std::string& s) {
		writeShort((int)s.size());
		for (size_t i = 0; i < s.size(); ++i) buffer.push_back((unsigned char)s[i]);
	}
};
#endif // DATAOUTPUT_H
