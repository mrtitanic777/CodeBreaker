#ifndef TEXTUREWATERFX_H
#define TEXTUREWATERFX_H

// Auto-generated from TextureWaterFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class TextureWaterFX : public TextureFX {
public:
	TextureWaterFX();
	virtual ~TextureWaterFX();

	JArray<float > red;
	JArray<float > green2;
	JArray<float > blue;
	JArray<float > green;
	int tickCounter;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXTUREWATERFX_H