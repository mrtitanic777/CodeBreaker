#ifndef RENDERSNOWBALL_H
#define RENDERSNOWBALL_H

// Auto-generated from RenderSnowball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntitySnowball;

class RenderSnowball : public Render {
public:
	virtual ~RenderSnowball();

	virtual void a(EntitySnowball* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERSNOWBALL_H