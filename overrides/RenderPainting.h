#ifndef RENDERPAINTING_H
#define RENDERPAINTING_H

// Auto-generated from RenderPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityPainting;
class Random;

class RenderPainting : public Render {
public:
	virtual ~RenderPainting();

	Random* rand;

	virtual void a(EntityPainting* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void getBlockPathWeight(EntityPainting* var1, int var2, int var3, int var4, int var5);
	virtual void setLightmap(EntityPainting* var1, float var2, float var3);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);
};

#endif // RENDERPAINTING_H