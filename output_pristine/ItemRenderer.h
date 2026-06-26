#ifndef ITEMRENDERER_H
#define ITEMRENDERER_H

// Auto-generated from ItemRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

class ItemStack;
class Minecraft;
class RenderBlocks;

class ItemRenderer {
public:
	ItemRenderer();
	ItemRenderer(Minecraft* var1);
	virtual ~ItemRenderer();

	virtual void renderItemInFirstPerson(float var1);
	virtual void renderOverlays(float var1);
	virtual void updateEquippedItem();
	virtual void equipAnimationSpeed();
	virtual void equippedItemRender();
	virtual void resetEquippedProgress();

private:
	Minecraft* mc;
	ItemStack* m_itemToRender;
	float m_equippedProgress;
	float m_prevEquippedProgress;
	int swingProgress;
	bool m_itemSwingState;
	RenderBlocks* m_renderBlocksInstance;

};

#endif // ITEMRENDERER_H