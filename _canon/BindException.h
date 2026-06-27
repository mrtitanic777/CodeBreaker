#ifndef BINDEXCEPTION_H
#define BINDEXCEPTION_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for the external Java type BindException.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "SocketException.h"

class BindException : public SocketException {
public:
	BindException(...) {}
	virtual ~BindException() {}
};
#endif // BINDEXCEPTION_H
