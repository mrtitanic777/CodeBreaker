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
	EntitySmokeFX(World* var1, double var2, double var4, double var6);
	EntitySmokeFX(World* var1, double var2, double var4, double var6, float var8);
	virtual ~EntitySmokeFX();

	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();

public: // was private
	float m_field_671_a;

};

#endif // ENTITYSMOKEFX_H