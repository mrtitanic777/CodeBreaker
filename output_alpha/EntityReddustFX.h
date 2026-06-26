#ifndef ENTITYREDDUSTFX_H
#define ENTITYREDDUSTFX_H

// Auto-generated from EntityReddustFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Tesselator;
class World;

class EntityReddustFX : public EntityFX {
public:
	EntityReddustFX();
	EntityReddustFX(World* var1, double var2, double var4, double var6);
	EntityReddustFX(World* var1, double var2, double var4, double var6, float var8);
	virtual ~EntityReddustFX();

	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();

public: // was private
	float m_field_673_a;

};

#endif // ENTITYREDDUSTFX_H