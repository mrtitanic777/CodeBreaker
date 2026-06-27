#ifndef RENDER_H
#define RENDER_H

// Auto-generated from Render.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class AxisAlignedBB;
class Block;
class Entity;
class FontRenderer;
class ModelBase;
class RenderBlocks;
class RenderManager;
class World;

class Render {
public:
	Render();
	virtual ~Render();

	RenderManager* renderManager;
	ModelBase* unusedModelBiped;
	RenderBlocks* unusedRenderBlocks;
	float shadowSize;
	float shadowOpaque;

	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) = 0;
	virtual void loadTexture(const JavaString& var1);
	virtual void loadDownloadableImageTexture(const JavaString& var1, const JavaString& var2);
	virtual void renderEntityOnFire(Entity* var1, double var2, double var4, double var6, float var8);
	virtual void renderShadow(Entity* var1, double var2, double var4, double var6, float var8, float var9);
	virtual World* getWorldFromRenderManager();
	virtual void renderShadowOnBlock(Block* var1, double var2, double var4, double var6, int var8, int var9, int var10, float var11, float var12, double var13, double var15, double var17);
	static void renderOffsetAABB(AxisAlignedBB* var0, double var1, double var3, double var5);
	static void renderAABB(AxisAlignedBB* var0);
	virtual void setRenderManager(RenderManager* var1);
	virtual void doRenderShadowAndFire(Entity* var1, double var2, double var4, double var6, float var8, float var9);
	virtual FontRenderer* getFontRendererFromRenderManager();

	static void initStatics();
};

#endif // RENDER_H