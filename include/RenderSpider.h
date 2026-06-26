#ifndef RENDERSPIDER_H
#define RENDERSPIDER_H

// Auto-generated from RenderSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;

class RenderSpider : public RenderLiving {
public:
	RenderSpider();
	virtual ~RenderSpider();


protected:
	virtual float getDeathMaxRotation(EntityLiving* var1);
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);

};

#endif // RENDERSPIDER_H