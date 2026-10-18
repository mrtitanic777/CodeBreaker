#ifndef FILEREADER_H
#define FILEREADER_H
// Canonical java.io.FileReader -- REAL char reader over a file (was a no-op stub AND not a Reader subclass,
// so `new BufferedReader(new FileReader(optionsFile))` couldn't even bind -> options.txt never loaded). Now
// a Reader subclass that slurps the file (CreateFile/ReadFile) and hands out bytes-as-chars, -1 at EOF.
#include <string>
#include "JavaString.h"
#include "Reader.h"
#include "File.h"

class FileReader : public Reader {
	unsigned char* _data;
	int _size;
	int _pos;
	void _load(const char* p) {
		HANDLE h = CreateFile(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE) return;
		DWORD sz = GetFileSize(h, NULL);
		if (sz != 0 && sz != (DWORD)0xFFFFFFFF) {
			_data = new unsigned char[sz];
			DWORD r = 0; ReadFile(h, _data, sz, &r, NULL); _size = (int)r;
		}
		CloseHandle(h);
	}
public:
	FileReader(...) : _data(0), _size(0), _pos(0) {}
	FileReader(File* f, ...) : _data(0), _size(0), _pos(0) { if (f) _load(f->path.c_str()); }
	FileReader(const JavaString& p, ...) : _data(0), _size(0), _pos(0) { _load(((std::string)p).c_str()); }
	virtual ~FileReader() { if (_data) delete[] _data; }
	virtual int read(...) { return (_pos < _size) ? (int)_data[_pos++] : -1; }
	virtual bool ready(...) { return _pos < _size; }
	virtual void close(...) {}
};
#endif // FILEREADER_H
