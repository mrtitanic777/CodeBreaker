#include "stdafx.h"
// Auto-generated from EntityMob.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMob.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityCreature.h"

EntityMob::EntityMob()
	: attackStrength(2)
{
}

EntityMob::EntityMob(World* var1)
	: EntityCreature(var1)
	, attackStrength(2)
{
		health = 20;
}

EntityMob::~EntityMob() {
	// No dynamic resources
}

void EntityMob::onLivingUpdate() {
		float var1 = getEntityBrightness(1.0F);
		if(var1 > 0.5F) {
			entityAge += 2;
		}

		EntityCreature::onLivingUpdate();
}

void EntityMob::onEntityUpdate() {
		EntityCreature::onEntityUpdate();
		if(worldObj->m_difficultySetting == 0) {
			setEntityDead();
		}

}

Entity* EntityMob::findPlayerToAttack() {
		float var1 = worldObj->m_playerEntity->getDistanceSqToEntity(this);
		return var1 < 256.0F ? worldObj->m_playerEntity : NULL;
}

bool EntityMob::attackEntityFrom(Entity* var1, int var2) {
		if(EntityCreature::attackEntityFrom(var1, var2)) {
			if(var1 != this) {
				m_playerToAttack = var1;
			}

			return true;
		} else {
			return false;
		}
}

void EntityMob::attackEntity(Entity* var1, float var2) {
		if((double)var2 < 2.5f && var1->boundingBox.maxY > boundingBox.minY && var1->boundingBox.minY < boundingBox.maxY) {
			attackTime = 20;
			var1->attackEntityFrom(this, attackStrength);
		}

}

float EntityMob::getBlockPathWeight(int var1, int var2, int var3) {
		return 0.5F - worldObj->getLightBrightness(var1, var2, var3);
}

void EntityMob::writeEntityToNBT(NBTTagCompound* var1) {
		EntityCreature::writeEntityToNBT(var1);
}

void EntityMob::readEntityFromNBT(NBTTagCompound* var1) {
		EntityCreature::readEntityFromNBT(var1);
}

const std::string& EntityMob::getEntityString() {
		return "Monster";
}

bool EntityMob::getCanSpawnHere(float var1, float var2, float var3) {
		unsigned char var4 = worldObj->getBlockLightValue((int)var1, (int)var2, (int)var3);
		return var4 <= rand->nextInt(8) && EntityCreature::getCanSpawnHere(var1, var2, var3);
}
