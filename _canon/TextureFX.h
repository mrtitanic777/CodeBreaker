#ifndef TEXTUREFX_H
#define TEXTUREFX_H

// Auto-generated from TextureFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class RenderEngine;

class TextureFX {
public:
	TextureFX(int var1);
	virtual ~TextureFX();

	JArray<unsigned char > imageData;
	int iconIndex;
	bool anaglyphEnabled;
	int bindTextureId;
	int tileSize;
	int tileImage;

	virtual void onTick();
	virtual void bindImage(RenderEngine* var1);

	static void initStatics();
};

#endif // TEXTUREFX_H