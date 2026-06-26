#ifndef ENTITYPICKUPFX_H
#define ENTITYPICKUPFX_H

// Auto-generated from EntityPickupFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Entity;
class Tesselator;
class World;

class EntityPickupFX : public EntityFX {
public:
	EntityPickupFX();
	EntityPickupFX(World* var1, Entity* var2, Entity* var3, float var4);
	virtual ~EntityPickupFX();

	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();
	virtual int func_404_c();

public: // was private
	Entity* m_field_675_a;
	Entity* m_field_679_o;
	int m_field_678_p;
	int m_field_677_q;
	float m_field_676_r;

};

#endif // ENTITYPICKUPFX_H