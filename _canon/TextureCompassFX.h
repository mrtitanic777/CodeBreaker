#ifndef TEXTURECOMPASSFX_H
#define TEXTURECOMPASSFX_H

// Auto-generated from TextureCompassFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class Minecraft;

class TextureCompassFX : public TextureFX {
public:
	TextureCompassFX(Minecraft* var1);
	virtual ~TextureCompassFX();

	Minecraft* mc;
	JArray<int > compassIconPixels;
	double compassRotation;
	double compassRotationDelta;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXTURECOMPASSFX_H