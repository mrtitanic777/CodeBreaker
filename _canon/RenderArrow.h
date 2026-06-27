#ifndef RENDERARROW_H
#define RENDERARROW_H

// Auto-generated from RenderArrow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityArrow;

class RenderArrow : public Render {
public:
	virtual ~RenderArrow();

	virtual void a(EntityArrow* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERARROW_H