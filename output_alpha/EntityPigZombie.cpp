#include "stdafx.h"
// Auto-generated from EntityPigZombie.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPigZombie.h"
#include "Common.h"
#include "Entity.h"
#include "ItemStack.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityPlayer.h"
#include "EntityZombie.h"
#include "Item.h"
#include "Zombie.h"

ItemStack* EntityPigZombie::s_defaultHeldItem = new ItemStack(Item::s_swordGold, 1);

EntityPigZombie::EntityPigZombie()
	: m_angerLevel(0)
	, m_randomSoundDelay(0)
{
}

EntityPigZombie::EntityPigZombie(World* var1)
	: EntityZombie(var1)
	, m_angerLevel(0)
	, m_randomSoundDelay(0)
{
		texture = "/mob/pigzombie.png";
		m_field_9333_am = 0.5F;
		m_field_762_e = 5;
		m_isImmuneToFire = true;
}

EntityPigZombie::~EntityPigZombie() {
	// No dynamic resources
}

void EntityPigZombie::onUpdate() {
		m_field_9333_am = m_playerToAttack != NULL ? 0.95F : 0.5F;
		if(m_randomSoundDelay > 0 && --m_randomSoundDelay == 0) {
			worldObj->playSoundAtEntity(this, "mob.zombiepig.zpigangry", getSoundVolume() * 2.0F, ((rand->nextFloat() - rand->nextFloat()) * 0.2F + 1.0F) * 1.8F);
		}

		EntityZombie::onUpdate();
}

bool EntityPigZombie::getCanSpawnHere() {
		return worldObj->m_difficultySetting > 0 && worldObj->checkIfAABBIsClear(boundingBox) && worldObj->getCollidingBoundingBoxes(this, boundingBox).size() == 0 && !worldObj->getIsAnyLiquid(boundingBox);
}

void EntityPigZombie::writeEntityToNBT(NBTTagCompound* var1) {
		EntityZombie::writeEntityToNBT(var1);
		var1->setShort("Anger", (short)m_angerLevel);
}

void EntityPigZombie::readEntityFromNBT(NBTTagCompound* var1) {
		EntityZombie::readEntityFromNBT(var1);
		m_angerLevel = var1->getShort("Anger");
}

Entity* EntityPigZombie::findPlayerToAttack() {
		return m_angerLevel == 0 ? NULL : EntityZombie::findPlayerToAttack();
}

void EntityPigZombie::onLivingUpdate() {
		EntityZombie::onLivingUpdate();
}

bool EntityPigZombie::canAttackEntity(Entity* var1, int var2) {
		if(dynamic_cast<EntityPlayer*>(var1) != NULL) {
			std::vector<void*> var3 = worldObj->getEntitiesWithinAABBExcludingEntity(this, boundingBox.expands(32.0f, 32.0f, 32.0f));

			for(size_t var4 = 0; var4 < var3.size(); ++var4) {
				Entity* var5 = (Entity*)var3[var4];
				if(dynamic_cast<EntityPigZombie*>(var5) != NULL) {
					EntityPigZombie* var6 = (EntityPigZombie*)var5;
					var6->becomeAngryAt(var1);
				}
			}

			becomeAngryAt(var1);
		}

		return EntityZombie::canAttackEntity(var1, var2);
}

void EntityPigZombie::becomeAngryAt(Entity* var1) {
		m_playerToAttack = var1;
		m_angerLevel = 400 + rand->nextInt(400);
		m_randomSoundDelay = rand->nextInt(40);
}

const std::string& EntityPigZombie::getLivingSound() {
		static const std::string _ret = "mob.zombiepig.zpig"; return _ret;
}

const std::string& EntityPigZombie::getHurtSound() {
		static const std::string _ret = "mob.zombiepig.zpighurt"; return _ret;
}

const std::string& EntityPigZombie::getDeathSound() {
		static const std::string _ret = "mob.zombiepig.zpigdeath"; return _ret;
}

int EntityPigZombie::getDropItemId() {
		return Item::s_porkCooked->shiftedIndex;
}

ItemStack* EntityPigZombie::getHeldItem() {
		return s_defaultHeldItem;
}
