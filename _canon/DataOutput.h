#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.io.DataOutput.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class DataOutput {
public:
	DataOutput(...) {}
	virtual ~DataOutput() {}
	virtual void write(...) {}
	virtual void writeByte(...) {}
	virtual void writeShort(...) {}
	virtual void writeInt(...) {}
	virtual void writeLong(...) {}
	virtual void writeFloat(...) {}
	virtual void writeDouble(...) {}
	virtual void writeBoolean(...) {}
	virtual void writeChar(...) {}
	virtual void writeUTF(...) {}
	virtual void writeBytes(...) {}
	virtual void writeChars(...) {}
};
#endif // DATAOUTPUT_H
