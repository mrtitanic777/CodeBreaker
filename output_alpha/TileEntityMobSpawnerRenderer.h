#ifndef TILEENTITYMOBSPAWNERRENDERER_H
#define TILEENTITYMOBSPAWNERRENDERER_H

// Auto-generated from TileEntityMobSpawnerRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntitySpecialRenderer.h"

class Map;
class TileEntity;
class TileEntityMobSpawner;

class TileEntityMobSpawnerRenderer : public TileEntitySpecialRenderer {
public:
	TileEntityMobSpawnerRenderer();
	virtual ~TileEntityMobSpawnerRenderer();

	virtual void a(TileEntityMobSpawner* var1, double var2, double var4, double var6, float var8);
	virtual void renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8);

public: // was private
	std::map<std::string, void*>* m_field_1412_b;

};

#endif // TILEENTITYMOBSPAWNERRENDERER_H