#ifndef RENDERMINECART_H
#define RENDERMINECART_H

// Auto-generated from RenderMinecart.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityMinecart;
class ModelBase;

class RenderMinecart : public Render {
public:
	RenderMinecart();
	virtual ~RenderMinecart();

	ModelBase* a;

	virtual void a_(EntityMinecart* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();

private:
	// owns heap; copying disabled so a copy cannot double-free (Rule of Three)
	RenderMinecart(const RenderMinecart&);
	RenderMinecart& operator=(const RenderMinecart&);
};

#endif // RENDERMINECART_H