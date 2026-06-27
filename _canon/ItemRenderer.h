#ifndef ITEMRENDERER_H
#define ITEMRENDERER_H

// Auto-generated from ItemRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class ItemStack;
class Minecraft;
class RenderBlocks;

class ItemRenderer {
public:
	ItemRenderer(Minecraft* var1);
	virtual ~ItemRenderer();

	Minecraft* mc;
	ItemStack* itemToRender;
	float equippedProgress;
	float prevEquippedProgress;
	RenderBlocks* renderBlocksInstance;

	virtual void renderItem(ItemStack* var1);
	virtual void renderItemInFirstPerson(float var1);
	virtual void renderOverlays(float var1);
	virtual void renderInsideOfBlock(float var1, int var2);
	virtual void renderWarpedTextureOverlay(float var1);
	virtual void renderFireInFirstPerson(float var1);
	virtual void updateEquippedItem();
	virtual void resetEquippedProgress();
	virtual void resetEquippedProgress2();

	static void initStatics();
};

#endif // ITEMRENDERER_H