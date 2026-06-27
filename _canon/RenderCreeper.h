#ifndef RENDERCREEPER_H
#define RENDERCREEPER_H

// Auto-generated from RenderCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "RenderLiving.h"

class EntityCreeper;
class EntityLiving;

class RenderCreeper : public RenderLiving {
public:
	RenderCreeper();
	virtual ~RenderCreeper();

	virtual void a(EntityCreeper* var1, float var2);
	virtual int a(EntityCreeper* var1, float var2, float var3);
	virtual void preRenderCallback(EntityLiving* var1, float var2);
	virtual int getColorMultiplier(EntityLiving* var1, float var2, float var3);

	static void initStatics();
};

#endif // RENDERCREEPER_H