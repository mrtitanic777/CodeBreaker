#ifndef JAVAIO_H
#define JAVAIO_H
//------------------------------------------------------------------------------
// JavaIO.h - minimal java.io bridge for the converted port, backed by Win32/Xbox
// file I/O on a mounted drive (see Save360 — the STFS container is mounted as a
// virtual filesystem, after which CreateFile works on e.g. "Save:\\level.dat").
//
// The converted NBT writer/reader use DataOutput/DataInput (growable byte buffers).
// DataOutputStream/DataInputStream simply ARE those buffers, with a close()/ctor
// that moves the bytes to/from the underlying stream. GZIP wrappers are pass-through
// (self-consistent: we read our own saves; the reference port also stores raw NBT).
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "DataInput.h"
#include "DataOutput.h"
#include <string>
#include <vector>

// --- java.io.InputStream / OutputStream ---------------------------------------
class InputStream {
public:
    virtual ~InputStream() {}
    virtual int read() { return -1; }                       // next byte, or -1 at EOF
    virtual int read(unsigned char* b, int len) {
        int n = 0;
        for (; n < len; ++n) { int c = read(); if (c < 0) break; b[n] = (unsigned char)c; }
        return (n > 0) ? n : (len == 0 ? 0 : -1);
    }
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

// --- java.io.File -------------------------------------------------------------
class File {
public:
    std::string path;
    File() {}
    File(const std::string& p) : path(p) {}
    File(const std::string& parent, const std::string& child) { _join(parent, child); }
    File(File* parent, const std::string& child) { _join(parent ? parent->path : std::string(), child); }
    const std::string& getPath() const { return path; }
    bool exists() const { return GetFileAttributes(path.c_str()) != ((DWORD)0xFFFFFFFF); }
    bool mkdirs() const { return CreateDirectory(path.c_str(), NULL) != 0 || GetLastError() == ERROR_ALREADY_EXISTS; }
    bool deleteFile() const { return DeleteFile(path.c_str()) != 0; }
private:
    void _join(const std::string& parent, const std::string& child) {
        path = parent;
        if (!path.empty()) { char last = path[path.size() - 1]; if (last != '\\' && last != ':') path += '\\'; }
        path += child;
    }
};

// --- FileInputStream / FileOutputStream (CreateFile on the mounted drive) ------
class FileInputStream : public InputStream {
    std::vector<unsigned char> _buf;
    size_t _pos;
    void _load(const char* p) {
        HANDLE h = CreateFile(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h == INVALID_HANDLE_VALUE) return;
        DWORD sz = GetFileSize(h, NULL);
        if (sz != 0 && sz != ((DWORD)0xFFFFFFFF)) {
            _buf.resize(sz);
            DWORD r = 0; ReadFile(h, &_buf[0], sz, &r, NULL); _buf.resize(r);
        }
        CloseHandle(h);
    }
public:
    FileInputStream(const std::string& p) : _pos(0) { _load(p.c_str()); }
    FileInputStream(File* f) : _pos(0) { _load(f ? f->path.c_str() : ""); }
    int read() { return (_pos < _buf.size()) ? (int)_buf[_pos++] : -1; }
    int read(unsigned char* b, int len) {
        int n = 0; while (n < len && _pos < _buf.size()) b[n++] = _buf[_pos++];
        return (n > 0) ? n : -1;
    }
};

class FileOutputStream : public OutputStream {
    std::string _path;
    std::vector<unsigned char> _buf;
public:
    FileOutputStream(const std::string& p) : _path(p) {}
    FileOutputStream(File* f) : _path(f ? f->path : std::string()) {}
    void write(int b) { _buf.push_back((unsigned char)(b & 0xFF)); }
    void write(const unsigned char* b, int len) { _buf.insert(_buf.end(), b, b + len); }
    void close() {
        HANDLE h = CreateFile(_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h == INVALID_HANDLE_VALUE) return;
        DWORD w = 0; if (!_buf.empty()) WriteFile(h, &_buf[0], (DWORD)_buf.size(), &w, NULL);
        CloseHandle(h);
        _buf.clear();
    }
};

// --- GZIP wrappers (PASS-THROUGH — raw NBT, like the reference port) -----------
class GZIPInputStream : public InputStream {
    InputStream* _in;
public:
    GZIPInputStream(InputStream* in) : _in(in) {}
    int read() { return _in ? _in->read() : -1; }
    int read(unsigned char* b, int len) { return _in ? _in->read(b, len) : -1; }
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

// --- DataInputStream / DataOutputStream ---------------------------------------
// These ARE the NBT byte-buffer (DataInput/DataOutput) so NBTBase::read/writeTag
// works unchanged; they bridge that buffer to/from the underlying stream.
class DataInputStream : public DataInput {
    std::vector<unsigned char> _owned;
public:
    DataInputStream(InputStream* in) {
        if (in) {
            unsigned char tmp[4096]; int n;
            while ((n = in->read(tmp, (int)sizeof(tmp))) > 0) _owned.insert(_owned.end(), tmp, tmp + n);
        }
        data = _owned.empty() ? (const unsigned char*)0 : &_owned[0];
        length = (int)_owned.size();
        pos = 0;
    }
    void close() {}
};

class DataOutputStream : public DataOutput {
    OutputStream* _out;
public:
    DataOutputStream(OutputStream* out) : _out(out) {}
    void close() {
        if (_out) {
            if (!buffer.empty()) _out->write(&buffer[0], (int)buffer.size());
            _out->flush();
            _out->close();
        }
    }
};

#endif // JAVAIO_H
