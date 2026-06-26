#ifndef ENTITYSLIMEFX_H
#define ENTITYSLIMEFX_H

// Auto-generated from EntitySlimeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityFX.h"

class Item;
class Tesselator;
class World;

class EntitySlimeFX : public EntityFX {
public:
	EntitySlimeFX();
	EntitySlimeFX(World* var1, double var2, double var4, double var6, Item* var8);
	virtual ~EntitySlimeFX();

	virtual int func_404_c();
	virtual void func_406_a(Tesselator* var1, float var2, float var3, float var4, float var5, float var6, float var7);

};

#endif // ENTITYSLIMEFX_H