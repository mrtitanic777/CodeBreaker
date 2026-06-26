#include "stdafx.h"
// Auto-generated from EntitySplashFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySplashFX.h"
#include "Common.h"
#include "World.h"

EntitySplashFX::EntitySplashFX()
{
}

EntitySplashFX::EntitySplashFX(World* var1, float var2, float var3, float var4)
	: EntityRainFX(var1, var2, var3, var4)
{
		particleGravity = 0.04F;
		++P;
}

EntitySplashFX::~EntitySplashFX() {
	// No dynamic resources
}
