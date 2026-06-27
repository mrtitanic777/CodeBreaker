#ifndef RENDERSPIDER_H
#define RENDERSPIDER_H

// Auto-generated from RenderSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityLiving;
class EntitySpider;

class RenderSpider : public RenderLiving {
public:
	RenderSpider();
	virtual ~RenderSpider();

	virtual float a(EntitySpider* var1);
	virtual bool a(EntitySpider* var1, int var2);
	virtual float getDeathMaxRotation(EntityLiving* var1);
	virtual bool shouldRenderPass(EntityLiving* var1, int var2);

	static void initStatics();
};

#endif // RENDERSPIDER_H