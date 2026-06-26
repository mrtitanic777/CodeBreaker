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

	virtual void func_4013_a(ModelBase* var1);
	virtual void a(EntityLiving* var1, double var2, double var4, double var6, float var8, float var9);
	virtual void doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9);

public: // was protected
	ModelBase* m_e;
	ModelBase* m_field_6332_f;

	virtual float func_167_c(EntityLiving* var1, float var2);
	virtual float func_170_d(EntityLiving* var1, float var2);
	virtual void func_6331_b(EntityLiving* var1, float var2);
	virtual bool func_166_a(EntityLiving* var1, int var2);
	virtual float func_172_a(EntityLiving* var1);
	virtual int func_173_a(EntityLiving* var1, float var2, float var3);
	virtual void func_6330_a(EntityLiving* var1, float var2);

};

#endif // RENDERLIVING_H