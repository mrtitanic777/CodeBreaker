#ifndef ERROR_H
#define ERROR_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for the external Java type Error.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "Throwable.h"

class Error : public Throwable {
public:
	Error(...) {}
	virtual ~Error() {}
};
#endif // ERROR_H
