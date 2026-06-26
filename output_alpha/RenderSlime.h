#ifndef RENDERSLIME_H
#define RENDERSLIME_H

// Auto-generated from RenderSlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class EntitySlime;
class ModelBase;

class RenderSlime : public RenderLiving {
public:
	RenderSlime();
	RenderSlime(ModelBase* var1, ModelBase* var2, float var3);
	virtual ~RenderSlime();


public: // was protected
	virtual bool a(EntitySlime* var1, int var2);
	virtual void a(EntitySlime* var1, float var2);
	virtual void func_6330_a(EntityLiving* var1, float var2);
	virtual bool func_166_a(EntityLiving* var1, int var2);

public: // was private
	ModelBase* m_field_205_f;

};

#endif // RENDERSLIME_H