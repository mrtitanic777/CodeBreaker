#ifndef RENDER_H
#define RENDER_H

// Auto-generated from Render.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

struct AxisAlignedBB;
class Entity;
class RenderManager;

class Render {
public:
	Render();
	virtual ~Render();

	virtual void doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6) = 0;
	static void renderOffsetAABB(const AxisAlignedBB& var0);
	virtual void setRenderManager(RenderManager* var1);
	virtual void renderShadow(Entity* var1, float var2, float var3, float var4, float var5);

protected:
	RenderManager* renderManager;
	float shadowSize;
	float shadowOpaque;

	virtual void loadTexture(const std::string& var1);
	virtual void loadDownloadableImageTexture(const std::string& var1, const std::string& var2);

};

#endif // RENDER_H