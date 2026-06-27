#ifndef ENTITYSMOKEFX_H
#define ENTITYSMOKEFX_H

// Auto-generated from EntitySmokeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Tessellator;
class World;

class EntitySmokeFX : public EntityFX {
public:
	EntitySmokeFX(World* var1, double var2, double var4, double var6);
	EntitySmokeFX(World* var1, double var2, double var4, double var6, float var8);
	virtual ~EntitySmokeFX();

	float baseParticleScale;

	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();

	static void initStatics();
};

#endif // ENTITYSMOKEFX_H