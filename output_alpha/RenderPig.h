#ifndef RENDERPIG_H
#define RENDERPIG_H

// Auto-generated from RenderPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class EntityPig;
class ModelBase;

class RenderPig : public RenderLiving {
public:
	RenderPig();
	RenderPig(ModelBase* var1, ModelBase* var2, float var3);
	virtual ~RenderPig();


public: // was protected
	virtual bool a(EntityPig* var1, int var2);
	virtual bool func_166_a(EntityLiving* var1, int var2);

};

#endif // RENDERPIG_H