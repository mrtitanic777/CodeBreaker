#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H
#include <string>
#include <vector>
#include "stdafx.h"          // CreateFile / WriteFile / CloseHandle
#include "OutputStream.h"
#include "File.h"

// Canonical java.io.FileOutputStream -- write-on-close to disk. Single real definition (like OutputStream).
// [360] equivalent: CreateFile(CREATE_ALWAYS) + WriteFile (xboxkrnl.exe).
// FIXED BUG save-writes-noop: game code #included a no-op STUB FileOutputStream.h, so every disk write
// (level.dat / player.dat / chunk NBT via CompressedStreamTools) was silently dropped -> nothing persisted.
// The real impl used to live only in FileIO.h (reached by nobody), so this is the same class hoisted into its
// own canonical header. DURABILITY: bytes hit the drive on close(), but an STFS content package is not durable
// to the device until Save360_Flush()/XContentFlush() runs after a save batch. No atomic temp+rename: a crash
// mid-write corrupts that one file.
class FileOutputStream : public OutputStream {
	std::string _path;
	std::vector<unsigned char> _buf;
public:
	FileOutputStream(const std::string& p) : _path(p) {}
	FileOutputStream(File* f) : _path(f ? f->path : std::string()) {}
	void write(int b) { _buf.push_back((unsigned char)(b & 0xFF)); }
	void write(const unsigned char* b, int len) { _buf.insert(_buf.end(), b, b + len); }
	void flush() {}
	void close() {
		if (_path.empty()) return;
		HANDLE h = CreateFile(_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE) return;
		DWORD w = 0; if (!_buf.empty()) WriteFile(h, &_buf[0], (DWORD)_buf.size(), &w, NULL);
		CloseHandle(h);
		_buf.clear();
	}
};
#endif // FILEOUTPUTSTREAM_H
