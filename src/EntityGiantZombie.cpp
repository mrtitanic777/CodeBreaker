#include "stdafx.h"
// Auto-generated from EntityGiantZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityGiantZombie.h"
#include "Common.h"
#include "World.h"
#include <string>
#include "EntityMob.h"
#include "Zombie.h"

EntityGiantZombie::EntityGiantZombie()
{
}

EntityGiantZombie::EntityGiantZombie(World* var1)
	: EntityMob(var1)
{
		texture = "/mob/zombie.png";
		moveSpeed = 0.5F;
		attackStrength = 50;
		health *= 10;
		yOffset *= 6.0F;
		setSize(width * 6.0F, height * 6.0F);
}

EntityGiantZombie::~EntityGiantZombie() {
	// No dynamic resources
}

float EntityGiantZombie::getBlockPathWeight(int var1, int var2, int var3) {
		return worldObj->getLightBrightness(var1, var2, var3) - 0.5F;
}

const std::string& EntityGiantZombie::getEntityString() {
		return "Giant";
}
