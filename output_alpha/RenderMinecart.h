#ifndef RENDERMINECART_H
#define RENDERMINECART_H

// Auto-generated from RenderMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class EntityMinecart;
class ModelBase;

class RenderMinecart : public Render {
public:
	RenderMinecart();
	virtual ~RenderMinecart();

	virtual void a(EntityMinecart* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

public: // was protected
	ModelBase* m_a;

};

#endif // RENDERMINECART_H