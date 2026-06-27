#ifndef ENTITYPORTALFX_H
#define ENTITYPORTALFX_H

// Auto-generated from EntityPortalFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Tessellator;
class World;

class EntityPortalFX : public EntityFX {
public:
	EntityPortalFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntityPortalFX();

	float portalParticleScale;
	double portalPosX;
	double portalPosY;
	double portalPosZ;

	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual float getEntityBrightness(float var1);
	virtual void onUpdate();

	static void initStatics();
};

#endif // ENTITYPORTALFX_H