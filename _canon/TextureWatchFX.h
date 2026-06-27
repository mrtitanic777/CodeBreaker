#ifndef TEXTUREWATCHFX_H
#define TEXTUREWATCHFX_H

// Auto-generated from TextureWatchFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class Minecraft;

class TextureWatchFX : public TextureFX {
public:
	TextureWatchFX(Minecraft* var1);
	virtual ~TextureWatchFX();

	Minecraft* mc;
	JArray<int > itemPixels;
	JArray<int > dialPixels;
	double rotation;
	double rotationVelocity;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXTUREWATCHFX_H