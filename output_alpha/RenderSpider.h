#ifndef RENDERSPIDER_H
#define RENDERSPIDER_H

// Auto-generated from RenderSpider.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class EntitySpider;

class RenderSpider : public RenderLiving {
public:
	RenderSpider();
	virtual ~RenderSpider();


public: // was protected
	virtual float a(EntitySpider* var1);
	virtual bool a(EntitySpider* var1, int var2);
	virtual float func_172_a(EntityLiving* var1);
	virtual bool func_166_a(EntityLiving* var1, int var2);

};

#endif // RENDERSPIDER_H