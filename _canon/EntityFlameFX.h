#ifndef ENTITYFLAMEFX_H
#define ENTITYFLAMEFX_H

// Auto-generated from EntityFlameFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Tessellator;
class World;

class EntityFlameFX : public EntityFX {
public:
	EntityFlameFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntityFlameFX();

	float initialParticleScale;

	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual float getEntityBrightness(float var1);
	virtual void onUpdate();

	static void initStatics();
};

#endif // ENTITYFLAMEFX_H