#ifndef ENTITYPICKUPFX_H
#define ENTITYPICKUPFX_H

// Auto-generated from EntityPickupFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Entity;
class Tessellator;
class World;

class EntityPickupFX : public EntityFX {
public:
	EntityPickupFX(World* var1, Entity* var2, Entity* var3, float var4);
	virtual ~EntityPickupFX();

	Entity* thePlayer;
	Entity* targetEntity;
	int age;
	int maxAge;
	float targetYOffset;

	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();
	virtual int getFXLayer();

	static void initStatics();
};

#endif // ENTITYPICKUPFX_H