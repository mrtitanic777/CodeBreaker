#include "stdafx.h"
// Auto-generated from EntityPig.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPig.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Item.h"
#include "EntityAnimals.h"

EntityPig::EntityPig()
	: m_rideable(false)
{
}

EntityPig::EntityPig(World* var1)
	: EntityAnimals(var1)
	, m_rideable(false)
{
		texture = "/mob/pig.png";
		setSize(0.9F, 0.9F);
		m_rideable = false;
}

EntityPig::~EntityPig() {
	// No dynamic resources
}

void EntityPig::writeEntityToNBT(NBTTagCompound* var1) {
		EntityAnimals::writeEntityToNBT(var1);
		var1->setBoolean("Saddle", m_rideable);
}

void EntityPig::readEntityFromNBT(NBTTagCompound* var1) {
		EntityAnimals::readEntityFromNBT(var1);
		m_rideable = var1->getBoolean("Saddle");
}

const std::string& EntityPig::getLivingSound() {
		static const std::string _ret = "mob.pig"; return _ret;
}

const std::string& EntityPig::getHurtSound() {
		static const std::string _ret = "mob.pig"; return _ret;
}

const std::string& EntityPig::getDeathSound() {
		static const std::string _ret = "mob.pigdeath"; return _ret;
}

bool EntityPig::interact(EntityPlayer* var1) {
		if(m_rideable) {
			var1->mountEntity(this);
			return true;
		} else {
			return false;
		}
}

int EntityPig::getDropItemId() {
		return Item::s_porkRaw->shiftedIndex;
}
