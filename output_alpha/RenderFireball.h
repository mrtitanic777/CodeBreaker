#ifndef RENDERFIREBALL_H
#define RENDERFIREBALL_H

// Auto-generated from RenderFireball.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class EntityFireball;

class RenderFireball : public Render {
public:
	RenderFireball();
	virtual ~RenderFireball();

	virtual void a(EntityFireball* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

};

#endif // RENDERFIREBALL_H