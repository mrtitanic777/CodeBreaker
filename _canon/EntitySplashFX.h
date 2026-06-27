#ifndef ENTITYSPLASHFX_H
#define ENTITYSPLASHFX_H

// Auto-generated from EntitySplashFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "EntityRainFX.h"

class World;

class EntitySplashFX : public EntityRainFX {
public:
	EntitySplashFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12);
	virtual ~EntitySplashFX();

	static void initStatics();
};

#endif // ENTITYSPLASHFX_H