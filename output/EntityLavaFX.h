#ifndef ENTITYLAVAFX_H
#define ENTITYLAVAFX_H

// Auto-generated from EntityLavaFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Tesselator;
class World;

class EntityLavaFX : public EntityFX {
public:
	EntityLavaFX();
	EntityLavaFX(World* var1, float var2, float var3, float var4);
	virtual ~EntityLavaFX();

	virtual float getEntityBrightness(float var1);
	virtual void renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onEntityUpdate();

public: // was private
	float m_lavaParticleScale;

};

#endif // ENTITYLAVAFX_H