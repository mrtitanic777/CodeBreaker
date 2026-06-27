#ifndef ENTITYREDDUSTFX_H
#define ENTITYREDDUSTFX_H

// Auto-generated from EntityReddustFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Tessellator;
class World;

class EntityReddustFX : public EntityFX {
public:
	EntityReddustFX(World* var1, double var2, double var4, double var6);
	EntityReddustFX(World* var1, double var2, double var4, double var6, float var8);
	virtual ~EntityReddustFX();

	float baseParticleScale;

	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();

	static void initStatics();
};

#endif // ENTITYREDDUSTFX_H