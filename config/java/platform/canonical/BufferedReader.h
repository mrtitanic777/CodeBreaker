#ifndef BUFFEREDREADER_H
#define BUFFEREDREADER_H
// Canonical java.io.BufferedReader -- REAL line reader over a Reader (was a no-op stub whose readLine()
// returned "" forever). Every `while ((line = br.readLine()) != null)` loop (splashes.txt, options, lang
// fallbacks) got no data. readLine() returns each line; at EOF it returns "" which the value model treats
// as null (JavaString == null iff empty), so the loop terminates correctly.
#include <string>
#include "JavaString.h"
#include "Reader.h"
#include "NullPointerException.h"

class BufferedReader {
	Reader* _in;
public:
	BufferedReader(...) : _in(0) {}
	BufferedReader(Reader* r, ...) : _in(r) { if (!r) throw new NullPointerException(); }   // (reader) or (reader, bufSize); null throws, as Java
	virtual ~BufferedReader() {}
	virtual void close(...) { if (_in) _in->close(); }
	virtual int read(...) { return _in ? _in->read() : -1; }
	virtual bool ready(...) { return _in != 0; }
	// One line, sans the terminator. EOF -> "" (== null in the value model -> the read loop ends).
	virtual JavaString readLine(...) {
		if (!_in) return JavaString();
		std::string line;
		int c;
		bool any = false;
		while ((c = _in->read()) != -1) {
			any = true;
			if (c == '\n') break;
			if (c != '\r') line += (char)c;
		}
		return any ? JavaString(line) : JavaString();   // no chars read -> EOF -> null
	}
};
#endif // BUFFEREDREADER_H
