#include "stdafx.h"
// Auto-generated from EntitySheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySheep.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "EntityItem.h"
#include "EntityLiving.h"
#include "EntityAnimals.h"

EntitySheep::EntitySheep()
	: m_sheared(false)
{
}

EntitySheep::EntitySheep(World* var1)
	: EntityAnimals(var1)
	, m_sheared(false)
{
		texture = "/mob/sheep.png";
		setSize(0.9F, 1.3F);
}

EntitySheep::~EntitySheep() {
	// No dynamic resources
}

bool EntitySheep::canAttackEntity(Entity* var1, int var2) {
		if(!worldObj->m_multiplayerWorld && !m_sheared && dynamic_cast<EntityLiving*>(var1) != NULL) {
			m_sheared = true;
			int var3 = 1 + rand->nextInt(3);

			for(int var4 = 0; var4 < var3; ++var4) {
				EntityItem* var5 = dropItemWithOffset(Block::s_cloth->blockID, 1, 1.0F);
				var5->motionY += (double)(rand->nextFloat() * 0.05F);
				var5->motionX += (double)((rand->nextFloat() - rand->nextFloat()) * 0.1F);
				var5->motionZ += (double)((rand->nextFloat() - rand->nextFloat()) * 0.1F);
			}
		}

		return EntityAnimals::canAttackEntity(var1, var2);
}

void EntitySheep::writeEntityToNBT(NBTTagCompound* var1) {
		EntityAnimals::writeEntityToNBT(var1);
		var1->setBoolean("Sheared", m_sheared);
}

void EntitySheep::readEntityFromNBT(NBTTagCompound* var1) {
		EntityAnimals::readEntityFromNBT(var1);
		m_sheared = var1->getBoolean("Sheared");
}

const std::string& EntitySheep::getLivingSound() {
		static const std::string _ret = "mob.sheep"; return _ret;
}

const std::string& EntitySheep::getHurtSound() {
		static const std::string _ret = "mob.sheep"; return _ret;
}

const std::string& EntitySheep::getDeathSound() {
		static const std::string _ret = "mob.sheep"; return _ret;
}
