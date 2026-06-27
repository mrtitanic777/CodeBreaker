#ifndef PRINTWRITER_H
#define PRINTWRITER_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.io.PrintWriter.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class PrintWriter {
public:
	PrintWriter(...) {}
	virtual ~PrintWriter() {}
	virtual void flush(...) {}
	virtual void close(...) {}
	virtual bool checkError(...) { return false; }
	template<class T> void println(const T&) {}
	void println() {}
	template<class T> void print(const T&) {}
	void print() {}
	template<class T> void write(const T&) {}
};
#endif // PRINTWRITER_H
