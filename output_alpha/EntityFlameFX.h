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
	EntityFlameFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntityFlameFX();

	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual float getEntityBrightness(float var1);
	virtual void onUpdate();

public: // was private
	float m_field_672_a;

};

#endif // ENTITYFLAMEFX_H