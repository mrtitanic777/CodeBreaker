#ifndef DATAINPUTSTREAM_H
#define DATAINPUTSTREAM_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.io.DataInputStream.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "DataInput.h"

class DataInputStream : public DataInput {
public:
	DataInputStream(...) {}
	virtual ~DataInputStream() {}
	virtual void close(...) {}
	virtual void reset(...) {}
	virtual void mark(...) {}
	virtual int read(...) { return 0; }
	virtual int available(...) { return 0; }
	virtual long long skip(...) { return 0LL; }
	virtual int skipBytes(...) { return 0; }
};
#endif // DATAINPUTSTREAM_H
