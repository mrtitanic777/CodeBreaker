#ifndef PRINTWRITER_H
#define PRINTWRITER_H
// Canonical java.io.PrintWriter -- REAL formatting writer over a Writer (was a no-op stub, so saveOptions'
// 20+ println() calls wrote nothing). The game builds each line with JCat first, so println() is always
// called with a string-like arg -> a JavaString overload covers it (NO template, to avoid the JCat->ostream
// ambiguity). print/println forward to the wrapped Writer.
#include <cstdio>
#include <string>
#include "JavaString.h"
#include "Writer.h"

class PrintWriter {
	Writer* _out;
	void _w(const JavaString& s) { if (_out) _out->write(s); }
public:
	PrintWriter(...) : _out(0) {}
	PrintWriter(Writer* w, ...) : _out(w) {}           // (writer) / (writer, autoFlush)
	virtual ~PrintWriter() {}

	void print(const JavaString& s) { _w(s); }
	void print(int v)       { char b[32]; sprintf(b, "%d", v); _w(JavaString(b)); }
	void print(long long v) { char b[32]; sprintf(b, "%lld", v); _w(JavaString(b)); }
	void print(...) {}

	void println(const JavaString& s) { _w(s); _w(JavaString("\n")); }
	void println(int v)       { print(v); _w(JavaString("\n")); }
	void println(long long v) { print(v); _w(JavaString("\n")); }
	void println(...) { _w(JavaString("\n")); }

	void write(const JavaString& s) { _w(s); }
	void write(...) {}
	void flush(...) { if (_out) _out->flush(); }
	void close(...) { if (_out) { _out->flush(); _out->close(); } }
};
#endif // PRINTWRITER_H
