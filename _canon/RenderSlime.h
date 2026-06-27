#ifndef RENDERSLIME_H
#define RENDERSLIME_H

// Auto-generated from RenderSlime.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityLiving;
class EntitySlime;
class ModelBase;

class RenderSlime : public RenderLiving {
public:
	RenderSlime(ModelBase* var1, ModelBase* var2, float var3);
	virtual ~RenderSlime();

	ModelBase* scaleAmount;

	virtual bool a(EntitySlime* var1, int var2);
	virtual void a(EntitySlime* var1, float var2);
	virtual void preRenderCallback(EntityLiving* var1, float var2);
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);

	static void initStatics();
};

#endif // RENDERSLIME_H