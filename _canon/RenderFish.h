#ifndef RENDERFISH_H
#define RENDERFISH_H

// Auto-generated from RenderFish.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityFish;

class RenderFish : public Render {
public:
	virtual ~RenderFish();

	virtual void a(EntityFish* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERFISH_H