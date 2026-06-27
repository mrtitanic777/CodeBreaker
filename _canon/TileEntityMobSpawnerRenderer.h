#ifndef TILEENTITYMOBSPAWNERRENDERER_H
#define TILEENTITYMOBSPAWNERRENDERER_H

// Auto-generated from TileEntityMobSpawnerRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include <map>
#include "TileEntitySpecialRenderer.h"

class TileEntity;
class TileEntityMobSpawner;

class TileEntityMobSpawnerRenderer : public TileEntitySpecialRenderer {
public:
	TileEntityMobSpawnerRenderer();
	virtual ~TileEntityMobSpawnerRenderer();

	std::map<JObject, JObject> cachedEntities;

	virtual void a(TileEntityMobSpawner* var1, double var2, double var4, double var6, float var8);
	virtual void renderTileEntityAt(TileEntity* var1, double var2, double var4, double var6, float var8);

	static void initStatics();
};

#endif // TILEENTITYMOBSPAWNERRENDERER_H