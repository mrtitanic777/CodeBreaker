#ifndef RENDERLIVING_H
#define RENDERLIVING_H

// Auto-generated from RenderLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "Render.h"

class Entity;
class EntityLiving;
class ModelBase;

class RenderLiving : public Render {
public:
	RenderLiving(ModelBase* var1, float var2);
	virtual ~RenderLiving();

	ModelBase* e;
	ModelBase* renderPassModel;

	virtual void setRenderPassModel(ModelBase* var1);
	virtual void a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9);
	virtual float getSwingProgress(EntityLiving* var1, float var2);
	virtual float handleRotationFloat(EntityLiving* var1, float var2);
	virtual void renderEquippedItems(EntityLiving* var1, float var2);
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);
	virtual float getDeathMaxRotation(EntityLiving* var1);
	virtual int getColorMultiplier(EntityLiving* var1, float var2, float var3);
	virtual void preRenderCallback(EntityLiving* var1, float var2);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

	static void initStatics();
};

#endif // RENDERLIVING_H