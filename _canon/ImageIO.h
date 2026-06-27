#ifndef IMAGEIO_H
#define IMAGEIO_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for javax.imageio.ImageIO.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
class BufferedImage;

namespace ImageIO {
	inline void write(...) {}
	inline BufferedImage* read(...) { return 0; }
}
#endif // IMAGEIO_H
