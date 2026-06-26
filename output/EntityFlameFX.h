#ifndef ENTITYFLAMEFX_H
#define ENTITYFLAMEFX_H

// Auto-generated from EntityFlameFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Tesselator;
class World;

class EntityFlameFX : public EntityFX {
public:
	EntityFlameFX();
	EntityFlameFX(World* var1, float var2, float var3, float var4);
	virtual ~EntityFlameFX();

	virtual void renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual float getEntityBrightness(float var1);
	virtual void onEntityUpdate();

public: // was private
	float m_flameScale;

};

#endif // ENTITYFLAMEFX_H