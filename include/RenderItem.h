#ifndef RENDERITEM_H
#define RENDERITEM_H

// Auto-generated from RenderItem.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class FontRenderer;
class ItemStack;
class Random;
class RenderBlocks;
class RenderEngine;
class Tesselator;

class RenderItem : public Render {
public:
	RenderItem();
	virtual ~RenderItem();

	virtual void renderItemIntoGUI(RenderEngine* var1, const ItemStack& var2, int var3, int var4);
	virtual void renderItemOverlayIntoGUI(FontRenderer* var1, const ItemStack& var2, int var3, int var4);
	virtual void doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6);

private:
	RenderBlocks* m_renderBlocks;
	Random* m_random;

	void renderQuad(Tesselator* var0, int var1, int var2, int var3, int var4, int var5);

};

#endif // RENDERITEM_H