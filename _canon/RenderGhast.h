#ifndef RENDERGHAST_H
#define RENDERGHAST_H

// Auto-generated from RenderGhast.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityGhast;
class EntityLiving;

class RenderGhast : public RenderLiving {
public:
	RenderGhast();
	virtual ~RenderGhast();

	virtual void a(EntityGhast* var1, float var2);
	virtual void preRenderCallback(EntityLiving* var1, float var2);

	static void initStatics();
};

#endif // RENDERGHAST_H