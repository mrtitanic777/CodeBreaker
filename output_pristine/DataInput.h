#ifndef DATAINPUT_H
#define DATAINPUT_H
// java.io.DataInput — big-endian reader over an in-memory byte buffer.
// Faithful to Java's DataInputStream semantics (used by NBT load).
#include <string>
#include <cstring>

class DataInput {
public:
    const unsigned char* data;
    int length;
    int pos;

    DataInput() : data(0), length(0), pos(0) {}
    DataInput(const unsigned char* d, int len) : data(d), length(len), pos(0) {}

    int readUnsignedByte() { return (pos < length) ? data[pos++] : 0; }
    signed char readByte()  { return (signed char)readUnsignedByte(); }
    bool readBoolean()      { return readUnsignedByte() != 0; }
    short readShort()       { int h = readUnsignedByte(), l = readUnsignedByte(); return (short)((h << 8) | l); }
    int readUnsignedShort() { int h = readUnsignedByte(), l = readUnsignedByte(); return (h << 8) | l; }
    int readInt() {
        int a = readUnsignedByte(), b = readUnsignedByte(), c = readUnsignedByte(), d = readUnsignedByte();
        return (a << 24) | (b << 16) | (c << 8) | d;
    }
    long long readLong() {
        long long v = 0;
        for (int i = 0; i < 8; ++i) v = (v << 8) | (long long)readUnsignedByte();
        return v;
    }
    float readFloat()   { int i = readInt(); float f; memcpy(&f, &i, 4); return f; }
    double readDouble() { long long l = readLong(); double d; memcpy(&d, &l, 8); return d; }
    void readFully(unsigned char* dst, int len) { for (int i = 0; i < len; ++i) dst[i] = (unsigned char)readUnsignedByte(); }
    // Java DataInput.readUTF: 2-byte length prefix, then modified-UTF8 bytes.
    std::string readUTF() {
        int n = readUnsignedShort();
        std::string s;
        s.reserve(n);
        for (int i = 0; i < n; ++i) s.push_back((char)readUnsignedByte());
        return s;
    }
};
#endif // DATAINPUT_H
