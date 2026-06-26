#ifndef RENDERGIANTZOMBIE_H
#define RENDERGIANTZOMBIE_H

// Auto-generated from RenderGiantZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class ModelBase;

class RenderGiantZombie : public RenderLiving {
public:
	RenderGiantZombie();
	RenderGiantZombie(ModelBase* var1, float var2, float var3);
	virtual ~RenderGiantZombie();


protected:
	virtual void preRenderCallback(EntityLiving* var1, float var2);

private:
	float m_scale;

};

#endif // RENDERGIANTZOMBIE_H