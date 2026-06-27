#ifndef RENDERPIG_H
#define RENDERPIG_H

// Auto-generated from RenderPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityLiving;
class EntityPig;
class ModelBase;

class RenderPig : public RenderLiving {
public:
	RenderPig(ModelBase* var1, ModelBase* var2, float var3);
	virtual ~RenderPig();

	virtual bool a(EntityPig* var1, int var2);
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);

	static void initStatics();
};

#endif // RENDERPIG_H