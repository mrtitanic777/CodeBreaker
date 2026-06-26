#include "stdafx.h"
// Auto-generated from EntityZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityZombie.h"
#include "Common.h"
#include "World.h"
#include <string>
#include "Item.h"
#include "Zombie.h"
#include "EntityMobs.h"

EntityZombie::EntityZombie()
{
}

EntityZombie::EntityZombie(World* var1)
	: EntityMobs(var1)
{
		texture = "/mob/zombie.png";
		m_field_9333_am = 0.5F;
		m_field_762_e = 5;
}

EntityZombie::~EntityZombie() {
	// No dynamic resources
}

void EntityZombie::onLivingUpdate() {
		if(worldObj->isDaytime()) {
			float var1 = getEntityBrightness(1.0F);
			if(var1 > 0.5F && worldObj->canBlockSeeTheSky((int)floor(posX), (int)floor(posY), (int)floor(posZ)) && rand->nextFloat() * 30.0F < (var1 - 0.4F) * 2.0F) {
				fire = 300;
			}
		}

		EntityMobs::onLivingUpdate();
}

const std::string& EntityZombie::getLivingSound() {
		static const std::string _ret = "mob.zombie"; return _ret;
}

const std::string& EntityZombie::getHurtSound() {
		static const std::string _ret = "mob.zombiehurt"; return _ret;
}

const std::string& EntityZombie::getDeathSound() {
		static const std::string _ret = "mob.zombiedeath"; return _ret;
}

int EntityZombie::getDropItemId() {
		return Item::s_feather->shiftedIndex;
}
