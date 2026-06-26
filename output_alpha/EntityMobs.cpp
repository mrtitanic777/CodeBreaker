#include "stdafx.h"
// Auto-generated from EntityMobs.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityMobs.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Block.h"
#include "EntityCreature.h"
#include "EntityPlayer.h"
#include "EnumSkyBlock.h"

EntityMobs::EntityMobs()
	: m_field_762_e(2)
{
}

EntityMobs::EntityMobs(World* var1)
	: EntityCreature(var1)
	, m_field_762_e(2)
{
		health = 20;
}

EntityMobs::~EntityMobs() {
	// No dynamic resources
}

void EntityMobs::onLivingUpdate() {
		float var1 = getEntityBrightness(1.0F);
		if(var1 > 0.5F) {
			m_field_9344_ag += 2;
		}

		EntityCreature::onLivingUpdate();
}

void EntityMobs::onUpdate() {
		EntityCreature::onUpdate();
		if(worldObj->m_difficultySetting == 0) {
			setEntityDead();
		}

}

Entity* EntityMobs::findPlayerToAttack() {
		EntityPlayer* var1 = worldObj->getClosestPlayerToEntity(this, 16.0f);
		return var1 != NULL && canEntityBeSeen(var1) ? var1 : NULL;
}

bool EntityMobs::canAttackEntity(Entity* var1, int var2) {
		if(EntityCreature::canAttackEntity(var1, var2)) {
			if(m_riddenByEntity != var1 && m_ridingEntity != var1) {
				if(var1 != this) {
					m_playerToAttack = var1;
				}

				return true;
			} else {
				return true;
			}
		} else {
			return false;
		}
}

void EntityMobs::attackEntity(Entity* var1, float var2) {
		if((double)var2 < 2.5f && var1->boundingBox.maxY > boundingBox.minY && var1->boundingBox.minY < boundingBox.maxY) {
			attackTime = 20;
			var1->canAttackEntity(this, m_field_762_e);
		}

}

float EntityMobs::func_439_a(int var1, int var2, int var3) {
		return 0.5F - worldObj->getLightBrightness(var1, var2, var3);
}

void EntityMobs::writeEntityToNBT(NBTTagCompound* var1) {
		EntityCreature::writeEntityToNBT(var1);
}

void EntityMobs::readEntityFromNBT(NBTTagCompound* var1) {
		EntityCreature::readEntityFromNBT(var1);
}

bool EntityMobs::getCanSpawnHere() {
		int var1 = (int)floor(posX);
		int var2 = (int)floor(boundingBox.minY);
		int var3 = (int)floor(posZ);
		if(worldObj->getSavedLightValue(EnumSkyBlock::s_Sky, var1, var2, var3) > rand->nextInt(32)) {
			return false;
		} else {
			int var4 = worldObj->getBlockLightValue(var1, var2, var3);
			return var4 <= rand->nextInt(8) && EntityCreature::getCanSpawnHere();
		}
}
