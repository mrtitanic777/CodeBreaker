#ifndef INPUTSTREAMREADER_H
#define INPUTSTREAMREADER_H
// Canonical java.io.InputStreamReader -- REAL adapter over an InputStream (was a no-op stub whose read()
// returned -1, so BufferedReader over it yielded no text -> empty splash list / empty config reads). The
// charset arg is accepted and ignored (the game's resource text is ASCII/UTF-8, byte==char for our use).
#include <string>
#include "JavaString.h"
#include "Reader.h"
#include "InputStream.h"

class InputStreamReader : public Reader {
	InputStream* _in;
public:
	InputStreamReader(...) : _in(0) {}
	InputStreamReader(InputStream* in, ...) : _in(in) {}   // (stream) or (stream, charset) -- charset ignored
	virtual ~InputStreamReader() {}
	virtual int read(...) { return _in ? _in->read() : -1; }
	virtual void close(...) { if (_in) _in->close(); }
};
#endif // INPUTSTREAMREADER_H
