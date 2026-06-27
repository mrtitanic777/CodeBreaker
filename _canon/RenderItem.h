#ifndef RENDERITEM_H
#define RENDERITEM_H

// Auto-generated from RenderItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityItem;
class FontRenderer;
class ItemStack;
class Random;
class RenderBlocks;
class RenderEngine;
class Tessellator;

class RenderItem : public Render {
public:
	RenderItem();
	virtual ~RenderItem();

	RenderBlocks* renderBlocks;
	Random* random;

	virtual void a(EntityItem* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void renderItemIntoGUI(FontRenderer* var1, RenderEngine* var2, ItemStack* var3, int var4, int var5);
	virtual void renderItemOverlayIntoGUI(FontRenderer* var1, RenderEngine* var2, ItemStack* var3, int var4, int var5);
	virtual void renderQuad(Tessellator* var1, int var2, int var3, int var4, int var5, int var6);
	virtual void renderTexturedQuad(int var1, int var2, int var3, int var4, int var5, int var6);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERITEM_H