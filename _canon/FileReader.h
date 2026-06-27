#ifndef FILEREADER_H
#define FILEREADER_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.io.FileReader.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class FileReader {
public:
	FileReader(...) {}
	virtual ~FileReader() {}
	virtual void close(...) {}
	virtual void mark(...) {}
	virtual void reset(...) {}
	virtual int read(...) { return 0; }
	virtual bool ready(...) { return false; }
};
#endif // FILEREADER_H
