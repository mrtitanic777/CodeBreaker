#ifndef FILEWRITER_H
#define FILEWRITER_H
// Canonical java.io.FileWriter -- REAL text file writer (was a no-op stub, so options.txt was never saved).
// A Writer subclass so it binds PrintWriter(Writer*). Buffers text, commits via CreateFile(CREATE_ALWAYS)+
// WriteFile on flush/close -- the text-path mirror of the already-real FileOutputStream.
#include <string>
#include "JavaString.h"
#include "Writer.h"
#include "File.h"

class FileWriter : public Writer {
	std::string _path;
	std::string _buf;
	void _commit() {
		if (_path.empty()) return;
		HANDLE h = CreateFile(_path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE) return;
		DWORD w = 0;
		if (!_buf.empty()) WriteFile(h, _buf.data(), (DWORD)_buf.size(), &w, NULL);
		CloseHandle(h);
	}
public:
	FileWriter(...) {}
	FileWriter(File* f, ...) : _path(f ? f->path : std::string()) {}         // (file) / (file, append)
	FileWriter(const JavaString& p, ...) : _path((std::string)p) {}
	virtual ~FileWriter() {}
	virtual void write(const JavaString& s) { _buf += (std::string)s; }
	virtual void write(int c) { _buf += (char)c; }
	virtual void write(...) {}
	virtual void append(const JavaString& s) { _buf += (std::string)s; }
	virtual void flush(...) { _commit(); }
	virtual void close(...) { _commit(); }
};
#endif // FILEWRITER_H
