#ifndef RENDERFALLINGSAND_H
#define RENDERFALLINGSAND_H

// Auto-generated from RenderFallingSand.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityFallingSand;
class RenderBlocks;

class RenderFallingSand : public Render {
public:
	RenderFallingSand();
	virtual ~RenderFallingSand();

	RenderBlocks* renderBlocks;

	virtual void a(EntityFallingSand* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERFALLINGSAND_H