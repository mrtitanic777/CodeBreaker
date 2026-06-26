#ifndef RENDERPAINTING_H
#define RENDERPAINTING_H

// Auto-generated from RenderPainting.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class EntityPainting;
class Random;

class RenderPainting : public Render {
public:
	RenderPainting();
	virtual ~RenderPainting();

	virtual void a(EntityPainting* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

public: // was private
	Random* rand;

	void func_159_a(EntityPainting* var1, int var2, int var3, int var4, int var5);
	void func_160_a(EntityPainting* var1, float var2, float var3);

};

#endif // RENDERPAINTING_H