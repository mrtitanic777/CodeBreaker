#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H
// java.io.DataOutput — big-endian writer into a growable byte buffer.
// Faithful to Java's DataOutputStream semantics (used by NBT save).
#include <string>
#include <vector>

class DataOutput {
public:
    std::vector<unsigned char> buffer;

    void writeByte(int v)    { buffer.push_back((unsigned char)(v & 0xFF)); }
    void writeBoolean(bool v){ writeByte(v ? 1 : 0); }
    void writeShort(int v)   { writeByte(v >> 8); writeByte(v); }
    void writeChar(int v)    { writeShort(v); }
    void writeInt(int v)     { writeByte(v >> 24); writeByte(v >> 16); writeByte(v >> 8); writeByte(v); }
    void writeLong(long long v) { for (int i = 56; i >= 0; i -= 8) writeByte((int)(v >> i)); }
    void writeFloat(float v)    { int i;       memcpy(&i, &v, 4); writeInt(i); }
    void writeDouble(double v)  { long long l; memcpy(&l, &v, 8); writeLong(l); }
    void write(const unsigned char* d, int len) { for (int i = 0; i < len; ++i) buffer.push_back(d[i]); }
    // Java DataOutput.writeUTF: 2-byte length prefix, then modified-UTF8 bytes.
    void writeUTF(const std::string& s) {
        writeShort((int)s.size());
        for (size_t i = 0; i < s.size(); ++i) buffer.push_back((unsigned char)s[i]);
    }
};
#endif // DATAOUTPUT_H
