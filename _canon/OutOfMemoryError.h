#ifndef OUTOFMEMORYERROR_H
#define OUTOFMEMORYERROR_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for the external Java type OutOfMemoryError.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "Error.h"

class OutOfMemoryError : public Error {
public:
	OutOfMemoryError(...) {}
	virtual ~OutOfMemoryError() {}
};
#endif // OUTOFMEMORYERROR_H
