#ifndef ENTITYSMOKEFX_H
#define ENTITYSMOKEFX_H

// Auto-generated from EntitySmokeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Tesselator;
class World;

class EntitySmokeFX : public EntityFX {
public:
	EntitySmokeFX();
	EntitySmokeFX(World* var1, float var2, float var3, float var4);
	EntitySmokeFX(World* var1, float var2, float var3, float var4, float var5);
	virtual ~EntitySmokeFX();

	virtual void renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onEntityUpdate();

private:
	float m_smokeParticleScale;

};

#endif // ENTITYSMOKEFX_H