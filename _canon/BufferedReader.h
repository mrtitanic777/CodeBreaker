#ifndef BUFFEREDREADER_H
#define BUFFEREDREADER_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.io.BufferedReader.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class BufferedReader {
public:
	BufferedReader(...) {}
	virtual ~BufferedReader() {}
	virtual void close(...) {}
	virtual std::string readLine(...) { return std::string(); }
	virtual int read(...) { return 0; }
	virtual bool ready(...) { return false; }
};
#endif // BUFFEREDREADER_H
