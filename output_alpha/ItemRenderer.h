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

	virtual void renderItem(ItemStack* var1);
	virtual void renderItemInFirstPerson(float var1);
	virtual void renderOverlays(float var1);
	virtual void func_895_a();
	virtual void func_9449_b();
	virtual void func_9450_c();

public: // was private
	Minecraft* mc;
	ItemStack* m_field_9451_b;
	float m_field_9453_c;
	float m_field_9452_d;
	RenderBlocks* m_field_1357_e;

	void renderInsideOfBlock(float var1, int var2);
	void renderWarpedTextureOverlay(float var1);
	void renderFireInFirstPerson(float var1);

};

#endif // ITEMRENDERER_H