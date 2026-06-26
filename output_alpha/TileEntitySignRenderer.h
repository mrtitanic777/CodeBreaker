#ifndef TILEENTITYSIGNRENDERER_H
#define TILEENTITYSIGNRENDERER_H

// Auto-generated from TileEntitySignRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntitySpecialRenderer.h"

class SignModel;
class TileEntity;
class TileEntitySign;

class TileEntitySignRenderer : public TileEntitySpecialRenderer {
public:
	TileEntitySignRenderer();
	virtual ~TileEntitySignRenderer();

	virtual void a(TileEntitySign* var1, double var2, double var4, double var6, float var8);
	virtual void renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8);

public: // was private
	SignModel* m_signModel;

};

#endif // TILEENTITYSIGNRENDERER_H