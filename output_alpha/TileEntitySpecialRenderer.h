#ifndef TILEENTITYSPECIALRENDERER_H
#define TILEENTITYSPECIALRENDERER_H

// Auto-generated from TileEntitySpecialRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class FontRenderer;
class TileEntity;
class TileEntityRenderer;

class TileEntitySpecialRenderer {
public:
	TileEntitySpecialRenderer();
	virtual ~TileEntitySpecialRenderer();

	virtual void renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8) = 0;
	virtual void setTileEntityRenderer(TileEntityRenderer* var1);
	virtual FontRenderer* getFontRenderer();

public: // was protected
	TileEntityRenderer* m_tileEntityRenderer;

	virtual void bindTextureByName(const std::string& var1);

};

#endif // TILEENTITYSPECIALRENDERER_H