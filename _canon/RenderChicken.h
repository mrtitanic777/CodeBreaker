#ifndef RENDERCHICKEN_H
#define RENDERCHICKEN_H

// Auto-generated from RenderChicken.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class Entity;
class EntityChicken;
class EntityLiving;
class ModelBase;

class RenderChicken : public RenderLiving {
public:
	RenderChicken(ModelBase* var1, float var2);
	virtual ~RenderChicken();

	virtual void a(EntityChicken* var1, double var2, double var4, double var6, float var8, float var9);
	virtual float a(EntityChicken* var1, float var2);
	virtual float handleRotationFloat(EntityLiving* var1, float var2);
	virtual void a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERCHICKEN_H