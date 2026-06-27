#ifndef TEXTUREPORTALFX_H
#define TEXTUREPORTALFX_H

// Auto-generated from TexturePortalFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "TextureFX.h"

class TexturePortalFX : public TextureFX {
public:
	TexturePortalFX();
	virtual ~TexturePortalFX();

	int portalTickCounter;
	JArray<JArray<unsigned char > > portalTextureData;

	virtual void onTick();

	static void initStatics();
};

#endif // TEXTUREPORTALFX_H