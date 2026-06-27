#ifndef DATAINPUT_H
#define DATAINPUT_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.io.DataInput.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class DataInput {
public:
	DataInput(...) {}
	virtual ~DataInput() {}
	virtual void readFully(...) {}
	virtual unsigned char readByte(...) { return 0; }
	virtual int readUnsignedByte(...) { return 0; }
	virtual short readShort(...) { return 0; }
	virtual int readUnsignedShort(...) { return 0; }
	virtual int readInt(...) { return 0; }
	virtual long long readLong(...) { return 0LL; }
	virtual float readFloat(...) { return 0.0F; }
	virtual double readDouble(...) { return 0.0; }
	virtual bool readBoolean(...) { return false; }
	virtual char readChar(...) { return 0; }
	virtual std::string readLine(...) { return std::string(); }
	virtual std::string readUTF(...) { return std::string(); }
};
#endif // DATAINPUT_H
