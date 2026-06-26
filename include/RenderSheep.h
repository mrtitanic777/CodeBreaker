#ifndef RENDERSHEEP_H
#define RENDERSHEEP_H

// Auto-generated from RenderSheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class ModelBase;

class RenderSheep : public RenderLiving {
public:
	RenderSheep();
	RenderSheep(ModelBase* var1, ModelBase* var2, float var3);
	virtual ~RenderSheep();


protected:
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);

};

#endif // RENDERSHEEP_H