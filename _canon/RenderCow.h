#ifndef RENDERCOW_H
#define RENDERCOW_H

// Auto-generated from RenderCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class Entity;
class EntityCow;
class EntityLiving;
class ModelBase;

class RenderCow : public RenderLiving {
public:
	RenderCow(ModelBase* var1, float var2);
	virtual ~RenderCow();

	virtual void a(EntityCow* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERCOW_H