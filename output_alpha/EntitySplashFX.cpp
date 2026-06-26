#include "stdafx.h"
// Auto-generated from EntitySplashFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySplashFX.h"
#include "Common.h"
#include "World.h"

EntitySplashFX::EntitySplashFX()
{
}

EntitySplashFX::EntitySplashFX(World* var1, double var2, double var4, double var6, double var8, double var10, double var12)
	: EntityRainFX(var1, var2, var4, var6)
{
		m_field_664_h = 0.04F;
		++m_field_670_b;
		if(var10 == 0.0f && (var8 != 0.0f || var12 != 0.0f)) {
			motionX = var8;
			motionY = var10 + 0.1f;
			motionZ = var12;
		}

}

EntitySplashFX::~EntitySplashFX() {
	// No dynamic resources
}
