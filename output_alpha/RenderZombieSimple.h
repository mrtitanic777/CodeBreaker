#ifndef RENDERZOMBIESIMPLE_H
#define RENDERZOMBIESIMPLE_H

// Auto-generated from RenderZombieSimple.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class EntityZombieSimple;
class ModelBase;

class RenderZombieSimple : public RenderLiving {
public:
	RenderZombieSimple();
	RenderZombieSimple(ModelBase* var1, float var2, float var3);
	virtual ~RenderZombieSimple();


public: // was protected
	virtual void a(EntityZombieSimple* var1, float var2);
	virtual void func_6330_a(EntityLiving* var1, float var2);

public: // was private
	float m_field_204_f;

};

#endif // RENDERZOMBIESIMPLE_H