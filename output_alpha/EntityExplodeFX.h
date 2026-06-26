#ifndef ENTITYEXPLODEFX_H
#define ENTITYEXPLODEFX_H

// Auto-generated from EntityExplodeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Tesselator;
class World;

class EntityExplodeFX : public EntityFX {
public:
	EntityExplodeFX();
	EntityExplodeFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntityExplodeFX();

	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();

};

#endif // ENTITYEXPLODEFX_H