#include "stdafx.h"
// Auto-generated from EntityZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityZombie.h"
#include "Common.h"
#include "World.h"
#include <string>
#include "EntityMob.h"
#include "Zombie.h"

EntityZombie::EntityZombie()
{
}

EntityZombie::EntityZombie(World* var1)
	: EntityMob(var1)
{
		texture = "/mob/zombie.png";
		moveSpeed = 0.5F;
		attackStrength = 5;
}

EntityZombie::~EntityZombie() {
	// No dynamic resources
}

void EntityZombie::onLivingUpdate() {
		float var1 = getEntityBrightness(1.0F);
		if(var1 > 0.5F && worldObj->canBlockSeeTheSky((int)posX, (int)posY, (int)posZ) && rand->nextFloat() * 40.0F < (var1 - 0.5F) * 2.0F) {
			fire = 300;
		}

		EntityMob::onLivingUpdate();
}

const std::string& EntityZombie::getEntityString() {
		return "Zombie";
}
