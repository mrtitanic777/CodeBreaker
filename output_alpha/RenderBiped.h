#ifndef RENDERBIPED_H
#define RENDERBIPED_H

// Auto-generated from RenderBiped.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderLiving.h"

class EntityLiving;
class ModelBiped;

class RenderBiped : public RenderLiving {
public:
	RenderBiped();
	RenderBiped(ModelBiped* var1, float var2);
	virtual ~RenderBiped();


public: // was protected
	ModelBiped* m_field_4013_a;

	virtual void func_6331_b(EntityLiving* var1, float var2);

};

#endif // RENDERBIPED_H