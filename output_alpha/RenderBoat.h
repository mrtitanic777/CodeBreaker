#ifndef RENDERBOAT_H
#define RENDERBOAT_H

// Auto-generated from RenderBoat.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class EntityBoat;
class ModelBase;

class RenderBoat : public Render {
public:
	RenderBoat();
	virtual ~RenderBoat();

	virtual void a(EntityBoat* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

public: // was protected
	ModelBase* m_field_198_d;

};

#endif // RENDERBOAT_H