// FileIO.h — java.io bridge (File / streams / DataInput-Output / getResourceAsStream) for the
// Xbox 360 j2x port, backed by Win32/XDK CreateFile/ReadFile/WriteFile + a mounted STFS save drive.
// [Step 5]
//
// REFERENCE IMPLEMENTATION for the Xbox-project integration. It REPLACES the converter's no-op
// java.io stubs (j2x/runtime.py: File.h, InputStream.h, OutputStream.h, FileInputStream.h,
// FileOutputStream.h, DataInput.h, DataOutput.h, DataInputStream.h, DataOutputStream.h,
// BufferedReader.h -- all return 0/false/self today) AND the Class::getResourceAsStream member
// (Class.h, currently `return 0`). It is the same bridge as the OLD port's JavaIO.h /
// DataInput.h / DataOutput.h (../MinecraftAlpha/), modernized for the NEW converter's call shapes:
//   * a REAL 3-arg InputStream::read(unsigned char* buf, int off, int len)  -- ImageIO.reference.cpp
//     calls in->read(buf, 0, len); the old port only had read(b, len).
//   * DataInput.readShort/Int/Long/Float/Double/UTF are BYTE-AT-A-TIME BIG-ENDIAN (NBT-faithful).
//   * Class::getResourceAsStream(name) resolves "/terrain.png" / "/gui/gui.png" -> game:\Content\...
//     and returns a real FileInputStream (the dependency Step 3 flagged for ImageIO::read).
//
// NOT compiled by the j2x build (which only syntax-checks the no-op stubs). Lives in the Xbox project
// against the XDK. Grounded in JavaIO.h, DataInput.h, DataOutput.h, Save360.cpp and
// docs/platform/04-textures-resources-fileio.md.
//
// !!! BIG-ENDIAN HAZARD (Xbox 360 is big-endian PowerPC) !!!
// DataInput/DataOutput assemble multi-byte values BYTE-AT-A-TIME so the wire format is identical on
// PPC and on a little-endian dev host -- this is the whole reason NBT round-trips correctly. NEVER
// "optimize" a reader to `memcpy(&i32, ptr, 4)` or `*(int*)ptr` over a serialized buffer: that is
// byte-swapped on the 360 vs the Java/PC NBT format. Floats/doubles are safe ONLY because the
// int/long is assembled big-endian FIRST and then bit-cast. (This is the opposite of NIOBuffers.h,
// whose RAW vertex scratch IS native-order on purpose -- different data, different rule.)

#ifndef FILEIO_H
#define FILEIO_H

#include "stdafx.h"          // xtl.h (CreateFile/ReadFile/WriteFile/GetFileAttributes/...)
#include <string>
#include <vector>

// === java.io.InputStream / OutputStream =======================================
class InputStream {
public:
	virtual ~InputStream() {}
	virtual int read() { return -1; }                       // next byte (0..255) or -1 at EOF
	// 3-arg form the NEW code uses (ImageIO::read does in->read(buf,0,len)). Returns count or -1@EOF.
	virtual int read(unsigned char* b, int off, int len) {
		int n = 0;
		for (; n < len; ++n) { int c = read(); if (c < 0) break; b[off + n] = (unsigned char)c; }
		return (n > 0) ? n : (len == 0 ? 0 : -1);
	}
	// 2-arg legacy form (off==0) kept for the old DataInputStream drain loop.
	virtual int read(unsigned char* b, int len) { return read(b, 0, len); }
	virtual int available() { return 0; }
	virtual long long skip(long long) { return 0; }
	virtual void close() {}
};

class OutputStream {
public:
	virtual ~OutputStream() {}
	virtual void write(int b) { (void)b; }
	virtual void write(const unsigned char* b, int len) { for (int i = 0; i < len; ++i) write((int)b[i]); }
	virtual void flush() {}
	virtual void close() {}
};

// === java.io.File =============================================================
// Win32-backed. Adds (vs the OLD JavaIO.h) length()/renameTo()/getName()/getParent()/listFiles-lite
// the NEW faithful chunk saver expects. mkdirs() is made RECURSIVE here (the old single-level
// CreateDirectory was a documented pitfall -- ChunkLoader needs <x36>\<z36> nesting).
class File {
public:
	std::string path;
	File() {}
	File(const std::string& p) : path(p) {}
	File(const std::string& parent, const std::string& child) { _join(parent, child); }
	File(File* parent, const std::string& child) { _join(parent ? parent->path : std::string(), child); }

	std::string getPath() const { return path; }
	std::string getAbsolutePath() const { return path; }
	std::string toString() const { return path; }
	std::string getName() const {
		std::string::size_type s = path.find_last_of("\\/:");
		return (s == std::string::npos) ? path : path.substr(s + 1);
	}
	std::string getParent() const {
		std::string::size_type s = path.find_last_of("\\/");
		return (s == std::string::npos) ? std::string() : path.substr(0, s);
	}
	File* getParentFile() const { std::string p = getParent(); return p.empty() ? 0 : new File(p); }

	bool exists() const { return GetFileAttributes(path.c_str()) != ((DWORD)0xFFFFFFFF); }
	bool isDirectory() const {
		DWORD a = GetFileAttributes(path.c_str());
		return a != ((DWORD)0xFFFFFFFF) && (a & FILE_ATTRIBUTE_DIRECTORY) != 0;
	}
	bool isFile() const {
		DWORD a = GetFileAttributes(path.c_str());
		return a != ((DWORD)0xFFFFFFFF) && (a & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}
	bool mkdir() const { return CreateDirectory(path.c_str(), NULL) != 0 || GetLastError() == ERROR_ALREADY_EXISTS; }
	bool mkdirs() const { return _mkdirsRec(path); }        // RECURSIVE (fixes old pitfall #3)
	bool deleteFile() const { return DeleteFile(path.c_str()) != 0; }
	bool renameTo(File* dst) const { return dst && MoveFile(path.c_str(), dst->path.c_str()) != 0; }
	long long length() const {
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (!GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fad)) return 0;
		return ((long long)fad.nFileSizeHigh << 32) | (long long)fad.nFileSizeLow;
	}
private:
	void _join(const std::string& parent, const std::string& child) {
		path = parent;
		if (!path.empty()) { char last = path[path.size() - 1]; if (last != '\\' && last != ':') path += '\\'; }
		path += child;
		for (std::string::size_type i = 0; i < path.size(); ++i) if (path[i] == '/') path[i] = '\\';
	}
	static bool _mkdirsRec(const std::string& p) {
		if (p.empty()) return false;
		DWORD a = GetFileAttributes(p.c_str());
		if (a != ((DWORD)0xFFFFFFFF) && (a & FILE_ATTRIBUTE_DIRECTORY)) return true;
		std::string::size_type s = p.find_last_of('\\');
		if (s != std::string::npos && s > 0 && p[s - 1] != ':') _mkdirsRec(p.substr(0, s));   // parent first
		return CreateDirectory(p.c_str(), NULL) != 0 || GetLastError() == ERROR_ALREADY_EXISTS;
	}
};

// === FileInputStream / FileOutputStream (CreateFile on the mounted drive) ======
// Eager slurp on open (chunk-sized data; same as the old port). Works on game:\ (read-only assets)
// and Save:\ (STFS save mount) once Save360_Init has mounted the container.
class FileInputStream : public InputStream {
	std::vector<unsigned char> _buf;
	size_t _pos;
	void _load(const char* p);
public:
	FileInputStream(const std::string& p) : _pos(0) { _load(p.c_str()); }
	FileInputStream(File* f) : _pos(0) { _load(f ? f->path.c_str() : ""); }
	int read() { return (_pos < _buf.size()) ? (int)_buf[_pos++] : -1; }
	int read(unsigned char* b, int off, int len) {
		int n = 0; while (n < len && _pos < _buf.size()) b[off + n++] = _buf[_pos++];
		return (n > 0) ? n : (len == 0 ? 0 : -1);
	}
	int read(unsigned char* b, int len) { return read(b, 0, len); }
	int available() { return (int)(_buf.size() - _pos); }
	void close() {}
};

// Write-on-close (RAM buffer -> CreateFile(CREATE_ALWAYS)+WriteFile in close()). A missing close()
// or a crash before it LOSES the file -- callers must close(), and the STFS layer must
// Save360_Flush()/XContentFlush() to make it durable (see 05b note).
class FileOutputStream : public OutputStream {
	std::string _path;
	std::vector<unsigned char> _buf;
public:
	FileOutputStream(const std::string& p) : _path(p) {}
	FileOutputStream(File* f) : _path(f ? f->path : std::string()) {}
	void write(int b) { _buf.push_back((unsigned char)(b & 0xFF)); }
	void write(const unsigned char* b, int len) { _buf.insert(_buf.end(), b, b + len); }
	void close();
};

// === DataInput / DataOutput — BIG-ENDIAN byte codec (NBT) ======================
// Verbatim from the old port (DataInput.h / DataOutput.h). The byte-at-a-time shifts are LOAD-BEARING
// on PPC -- see the hazard banner above. Do not "simplify".
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
	std::string readUTF() {                                  // 2-byte length prefix + modified-UTF8
		int n = readUnsignedShort(); std::string s; s.reserve(n);
		for (int i = 0; i < n; ++i) s.push_back((char)readUnsignedByte());
		return s;
	}
};

class DataOutput {
public:
	std::vector<unsigned char> buffer;
	void writeByte(int v)    { buffer.push_back((unsigned char)(v & 0xFF)); }
	void writeBoolean(bool v){ writeByte(v ? 1 : 0); }
	void writeShort(int v)   { writeByte(v >> 8); writeByte(v); }
	void writeChar(int v)    { writeShort(v); }
	void writeInt(int v)     { writeByte(v >> 24); writeByte(v >> 16); writeByte(v >> 8); writeByte(v); }
	void writeLong(long long v) { for (int i = 56; i >= 0; i -= 8) writeByte((int)(v >> i)); }
	void writeFloat(float v)    { int i;       memcpy(&i, &v, 4); writeInt(i); }          // bit-cast THEN BE emit
	void writeDouble(double v)  { long long l; memcpy(&l, &v, 8); writeLong(l); }
	void write(const unsigned char* d, int len) { for (int i = 0; i < len; ++i) buffer.push_back(d[i]); }
	void writeUTF(const std::string& s) {
		writeShort((int)s.size());
		for (size_t i = 0; i < s.size(); ++i) buffer.push_back((unsigned char)s[i]);
	}
};

// === DataInputStream / DataOutputStream =======================================
// These ARE the DataInput/DataOutput byte buffer (so NBTBase::read/writeTag works unchanged) plus a
// ctor/close() that moves bytes to/from the wrapped stream.
class DataInputStream : public DataInput {
	std::vector<unsigned char> _owned;
public:
	DataInputStream(InputStream* in) {
		if (in) { unsigned char tmp[4096]; int n;
			while ((n = in->read(tmp, 0, (int)sizeof(tmp))) > 0) _owned.insert(_owned.end(), tmp, tmp + n); }
		data = _owned.empty() ? (const unsigned char*)0 : &_owned[0];
		length = (int)_owned.size(); pos = 0;
	}
	int read() { return readUnsignedByte(); }
	void close() {}
};

class DataOutputStream : public DataOutput {
	OutputStream* _out;
public:
	DataOutputStream(OutputStream* out) : _out(out) {}
	int size() { return (int)buffer.size(); }
	void flush() { if (_out) _out->flush(); }
	void close() {
		if (_out) {
			if (!buffer.empty()) _out->write(&buffer[0], (int)buffer.size());
			_out->flush(); _out->close();
		}
	}
};

// === GZIP wrappers — PASS-THROUGH (raw NBT, like the reference port) ===========
// Saves are stored UNCOMPRESSED (self-consistent; incompatible with PC .dat). See 05b "raw vs gzip".
class GZIPInputStream : public InputStream {
	InputStream* _in;
public:
	GZIPInputStream(InputStream* in) : _in(in) {}
	int read() { return _in ? _in->read() : -1; }
	int read(unsigned char* b, int off, int len) { return _in ? _in->read(b, off, len) : -1; }
	void close() { if (_in) _in->close(); }
};
class GZIPOutputStream : public OutputStream {
	OutputStream* _out;
public:
	GZIPOutputStream(OutputStream* out) : _out(out) {}
	void write(int b) { if (_out) _out->write(b); }
	void write(const unsigned char* b, int len) { if (_out) _out->write(b, len); }
	void flush() { if (_out) _out->flush(); }
	void close() { if (_out) _out->close(); }
};

// === BufferedReader (options.txt) =============================================
// Real readLine() over a wrapped InputStream (drained eagerly). \n / \r\n terminated, EOF -> empty
// std::string AND ready()==false. (GameSettings can also keep the old shortcut of splitting itself.)
class BufferedReader {
	std::vector<unsigned char> _buf;
	size_t _pos;
public:
	BufferedReader(InputStream* in) : _pos(0) {
		if (in) { unsigned char t[4096]; int n;
			while ((n = in->read(t, 0, (int)sizeof(t))) > 0) _buf.insert(_buf.end(), t, t + n); }
	}
	bool ready() { return _pos < _buf.size(); }
	int  read() { return (_pos < _buf.size()) ? (int)_buf[_pos++] : -1; }
	std::string readLine() {
		if (_pos >= _buf.size()) return std::string();      // EOF (caller treats empty+!ready as null)
		std::string line;
		while (_pos < _buf.size()) {
			char c = (char)_buf[_pos++];
			if (c == '\n') break;
			if (c == '\r') { if (_pos < _buf.size() && _buf[_pos] == '\n') ++_pos; break; }
			line.push_back(c);
		}
		return line;
	}
	void close() {}
};

// === Class::getResourceAsStream — game:\Content resolution ======================
// The NEW code reaches assets via TexturePackBase::getResourceAsStream -> Class::getResourceAsStream
// (Class.h member, currently `return 0`). Wire that member to call this. `name` is a JAR-style
// resource path ("/terrain.png", "/gui/gui.png"); resolve to game:\Content\<name> (leading '/'
// dropped, '/' -> '\'). Returns a real FileInputStream (NULL only on bad name).
InputStream* J2X_getResourceAsStream(const char* name);

#endif // FILEIO_H
