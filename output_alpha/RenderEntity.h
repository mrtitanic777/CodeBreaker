#ifndef RENDERENTITY_H
#define RENDERENTITY_H

// Auto-generated from RenderEntity.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;

class RenderEntity : public Render {
public:
	RenderEntity();
	virtual ~RenderEntity();

	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

};

#endif // RENDERENTITY_H