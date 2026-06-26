#ifndef RENDERCREEPER_H
#define RENDERCREEPER_H

// Auto-generated from RenderCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityCreeper;
class EntityLiving;

class RenderCreeper : public RenderLiving {
public:
	RenderCreeper();
	virtual ~RenderCreeper();


public: // was protected
	virtual void a(EntityCreeper* var1, float var2);
	virtual int a(EntityCreeper* var1, float var2, float var3);
	virtual void func_6330_a(EntityLiving* var1, float var2);
	virtual int func_173_a(EntityLiving* var1, float var2, float var3);

};

#endif // RENDERCREEPER_H