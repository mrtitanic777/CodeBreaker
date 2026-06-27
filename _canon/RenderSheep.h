#ifndef RENDERSHEEP_H
#define RENDERSHEEP_H

// Auto-generated from RenderSheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityLiving;
class EntitySheep;
class ModelBase;

class RenderSheep : public RenderLiving {
public:
	RenderSheep(ModelBase* var1, ModelBase* var2, float var3);
	virtual ~RenderSheep();

	virtual bool a(EntitySheep* var1, int var2);
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);

	static void initStatics();
};

#endif // RENDERSHEEP_H