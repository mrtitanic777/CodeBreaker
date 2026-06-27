#ifndef ENTITYSLIMEFX_H
#define ENTITYSLIMEFX_H

// Auto-generated from EntitySlimeFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Item;
class Tessellator;
class World;

class EntitySlimeFX : public EntityFX {
public:
	EntitySlimeFX(World* var1, double var2, double var4, double var6, Item* var8);
	virtual ~EntitySlimeFX();

	virtual int getFXLayer();
	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);

	static void initStatics();
};

#endif // ENTITYSLIMEFX_H