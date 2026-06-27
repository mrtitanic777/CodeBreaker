#ifndef TEXTURELAVAFX_H
#define TEXTURELAVAFX_H

// Auto-generated from TextureLavaFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class TextureLavaFX : public TextureFX {
public:
	TextureLavaFX();
	virtual ~TextureLavaFX();

	JArray<float > lavaBufferA;
	JArray<float > lavaBufferB;
	JArray<float > lavaHeat;
	JArray<float > lavaHeatSpeed;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXTURELAVAFX_H