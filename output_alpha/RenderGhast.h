#ifndef RENDERGHAST_H
#define RENDERGHAST_H

// Auto-generated from RenderGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityGhast;
class EntityLiving;

class RenderGhast : public RenderLiving {
public:
	RenderGhast();
	virtual ~RenderGhast();


public: // was protected
	virtual void a(EntityGhast* var1, float var2);
	virtual void func_6330_a(EntityLiving* var1, float var2);

};

#endif // RENDERGHAST_H