#ifndef ENTITYRAINFX_H
#define ENTITYRAINFX_H

// Auto-generated from EntityRainFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityFX.h"

class Tessellator;
class World;

class EntityRainFX : public EntityFX {
public:
	EntityRainFX(World* var1, double var2, double var4, double var6);
	virtual ~EntityRainFX();

	virtual void renderParticle(Tessellator* var1, float var2, float var3, float var4, float var5, float var6, float var7);
	virtual void onUpdate();

	static void initStatics();
};

#endif // ENTITYRAINFX_H