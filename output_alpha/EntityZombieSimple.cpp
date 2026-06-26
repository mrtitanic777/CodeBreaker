#include "stdafx.h"
// Auto-generated from EntityZombieSimple.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityZombieSimple.h"
#include "Common.h"
#include "World.h"

EntityZombieSimple::EntityZombieSimple()
{
}

EntityZombieSimple::EntityZombieSimple(World* var1)
	: EntityMobs(var1)
{
		texture = "/mob/zombie.png";
		m_field_9333_am = 0.5F;
		m_field_762_e = 50;
		health *= 10;
		yOffset *= 6.0F;
		setSize(width * 6.0F, height * 6.0F);
}

EntityZombieSimple::~EntityZombieSimple() {
	// No dynamic resources
}

float EntityZombieSimple::func_439_a(int var1, int var2, int var3) {
		return worldObj->getLightBrightness(var1, var2, var3) - 0.5F;
}
