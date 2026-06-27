#ifndef THROWABLE_H
#define THROWABLE_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for the external Java type Throwable.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class Throwable {
public:
	Throwable(...) {}
	virtual ~Throwable() {}
	virtual void printStackTrace(...) {}
	virtual std::string getMessage(...) { return std::string(); }
	virtual std::string getLocalizedMessage(...) { return std::string(); }
	virtual std::string toString(...) { return std::string(); }
};
#endif // THROWABLE_H
