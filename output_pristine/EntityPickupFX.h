#ifndef ENTITYPICKUPFX_H
#define ENTITYPICKUPFX_H

// Auto-generated from EntityPickupFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Entity;
class EntityLiving;
class Tesselator;
class World;

class EntityPickupFX : public EntityFX {
public:
	EntityPickupFX();
	EntityPickupFX(World* var1, Entity* var2, EntityLiving* var3, float var4);
	virtual ~EntityPickupFX();

	virtual void renderParticle(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onEntityUpdate();
	virtual int getFXLayer();

private:
	Entity* m_entityToPickUp;
	EntityLiving* m_entityPickingUp;
	int m_age;
	int m_maxAge;
	float m_yOffs;

};

#endif // ENTITYPICKUPFX_H