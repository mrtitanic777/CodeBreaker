#ifndef RENDERZOMBIESIMPLE_H
#define RENDERZOMBIESIMPLE_H

// Auto-generated from RenderZombieSimple.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityLiving;
class EntityZombieSimple;
class ModelBase;

class RenderZombieSimple : public RenderLiving {
public:
	RenderZombieSimple(ModelBase* var1, float var2, float var3);
	virtual ~RenderZombieSimple();

	float scale;

	virtual void a(EntityZombieSimple* var1, float var2);
	virtual void preRenderCallback(EntityLiving* var1, float var2);

	static void initStatics();
};

#endif // RENDERZOMBIESIMPLE_H