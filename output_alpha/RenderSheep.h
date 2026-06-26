#ifndef RENDERSHEEP_H
#define RENDERSHEEP_H

// Auto-generated from RenderSheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class EntitySheep;
class ModelBase;

class RenderSheep : public RenderLiving {
public:
	RenderSheep();
	RenderSheep(ModelBase* var1, ModelBase* var2, float var3);
	virtual ~RenderSheep();


public: // was protected
	virtual bool a(EntitySheep* var1, int var2);
	virtual bool func_166_a(EntityLiving* var1, int var2);

};

#endif // RENDERSHEEP_H