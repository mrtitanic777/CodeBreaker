#ifndef TEXTUREFLAMESFX_H
#define TEXTUREFLAMESFX_H

// Auto-generated from TextureFlamesFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class TextureFlamesFX : public TextureFX {
public:
	TextureFlamesFX(int var1);
	virtual ~TextureFlamesFX();

	JArray<float > flameBufferA;
	JArray<float > flameBufferB;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXTUREFLAMESFX_H