#ifndef BUFFEREDIMAGE_H
#define BUFFEREDIMAGE_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for java.awt.image.BufferedImage.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
class Graphics;

class BufferedImage {
public:
	BufferedImage(...) {}
	virtual ~BufferedImage() {}
	virtual void setRGB(...) {}
	virtual void flush(...) {}
	virtual int getWidth(...) { return 0; }
	virtual int getHeight(...) { return 0; }
	virtual int getRGB(...) { return 0; }
	virtual void* getData(...) { return 0; }
	virtual Graphics* getGraphics(...) { return 0; }
	virtual Graphics* createGraphics(...) { return 0; }
};
#endif // BUFFEREDIMAGE_H
