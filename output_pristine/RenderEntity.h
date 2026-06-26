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

	virtual void doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6);

};

#endif // RENDERENTITY_H