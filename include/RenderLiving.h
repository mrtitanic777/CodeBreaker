#ifndef RENDERLIVING_H
#define RENDERLIVING_H

// Auto-generated from RenderLiving.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "Render.h"

class Entity;
class EntityLiving;
class ModelBase;

class RenderLiving : public Render {
public:
	RenderLiving();
	RenderLiving(ModelBase* var1, float var2);
	virtual ~RenderLiving();

	virtual void setRenderPassModel(ModelBase* var1);
	virtual void doRender(Entity* var1, float var2, float var3, float var4, float var5, float var6);

protected:
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);
	virtual float getDeathMaxRotation(EntityLiving* var1);
	virtual int getColorMultiplier(EntityLiving* var1, float var2, float var3);
	virtual void preRenderCallback(EntityLiving* var1, float var2);

private:
	ModelBase* m_mainModel;
	ModelBase* m_e;

};

#endif // RENDERLIVING_H