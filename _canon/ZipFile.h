#ifndef ZIPFILE_H
#define ZIPFILE_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.util.zip.ZipFile.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
class InputStream;

class ZipFile {
public:
	ZipFile(...) {}
	virtual ~ZipFile() {}
	virtual void close(...) {}
	virtual void* getEntry(...) { return 0; }
	virtual InputStream* getInputStream(...) { return 0; }
	virtual int size(...) { return 0; }
};
#endif // ZIPFILE_H
