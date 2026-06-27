#ifndef TILEENTITYSPECIALRENDERER_H
#define TILEENTITYSPECIALRENDERER_H

// Auto-generated from TileEntitySpecialRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class FontRenderer;
class TileEntity;
class TileEntityRenderer;

class TileEntitySpecialRenderer {
public:
	virtual ~TileEntitySpecialRenderer();

	TileEntityRenderer* tileEntityRenderer;

	virtual void renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8) = 0;
	virtual void bindTextureByName(const JavaString& var1);
	virtual void setTileEntityRenderer(TileEntityRenderer* var1);
	virtual FontRenderer* getFontRenderer();

	static void initStatics();
};

#endif // TILEENTITYSPECIALRENDERER_H