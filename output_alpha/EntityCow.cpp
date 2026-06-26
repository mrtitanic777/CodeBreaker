#include "stdafx.h"
// Auto-generated from EntityCow.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityCow.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Item.h"
#include "ItemStack.h"
#include "EntityAnimals.h"
#include "InventoryPlayer.h"

EntityCow::EntityCow()
	: m_unusedBoolean(false)
{
}

EntityCow::EntityCow(World* var1)
	: EntityAnimals(var1)
	, m_unusedBoolean(false)
{
		texture = "/mob/cow.png";
		setSize(0.9F, 1.3F);
}

EntityCow::~EntityCow() {
	// No dynamic resources
}

void EntityCow::writeEntityToNBT(NBTTagCompound* var1) {
		EntityAnimals::writeEntityToNBT(var1);
}

void EntityCow::readEntityFromNBT(NBTTagCompound* var1) {
		EntityAnimals::readEntityFromNBT(var1);
}

const std::string& EntityCow::getLivingSound() {
		static const std::string _ret = "mob.cow"; return _ret;
}

const std::string& EntityCow::getHurtSound() {
		static const std::string _ret = "mob.cowhurt"; return _ret;
}

const std::string& EntityCow::getDeathSound() {
		static const std::string _ret = "mob.cowhurt"; return _ret;
}

float EntityCow::getSoundVolume() {
		return 0.4F;
}

int EntityCow::getDropItemId() {
		return Item::s_leather->shiftedIndex;
}

bool EntityCow::interact(EntityPlayer* var1) {
		ItemStack* var2 = var1->m_inventory->getCurrentItem();
		if(var2 != NULL && var2->itemID == Item::s_bucketEmpty->shiftedIndex) {
			var1->m_inventory->setInventorySlotContents(var1->m_inventory->m_currentItem, new ItemStack(Item::s_bucketMilk));
			return true;
		} else {
			return false;
		}
}
