#ifndef RENDER_H
#define RENDER_H

// Auto-generated from Render.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

struct AxisAlignedBB;
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

	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) = 0;
	static void renderOffsetAABB(AxisAlignedBB var0, double var1, double var3, double var5);
	static void renderAABB(AxisAlignedBB var0);
	virtual void setRenderManager(RenderManager* var1);
	virtual void doRenderShadowAndFire(Entity* var1, double var2, double var4, double var6, float var8, float var9);
	virtual FontRenderer* getFontRendererFromRenderManager();

public: // was protected
	RenderManager* renderManager;
	float m_field_9246_c;
	float m_field_194_c;

	virtual void loadTexture(const std::string& var1);
	virtual void func_140_a(const std::string& var1, const std::string& var2);

public: // was private
	ModelBase* m_unusedModelBiped;
	RenderBlocks* m_unusedRenderBlocks;

	void renderEntityOnFire(Entity* var1, double var2, double var4, double var6, float var8);
	void renderShadow(Entity* var1, double var2, double var4, double var6, float var8, float var9);
	World* getWorldFromRenderManager();
	void renderShadowOnBlock(Block* var1, double var2, double var4, double var6, int var8, int var9, int var10, float var11, float var12, double var13, double var15, double var17);

};

#endif // RENDER_H