#include "stdafx.h"
// Auto-generated from EntitySheep.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntitySheep.h"
#include "Common.h"
#include "Entity.h"
#include "NBTTagCompound.h"
#include "World.h"
#include <string>
#include "EntityAnimal.h"
#include "EntityItem.h"

EntitySheep::EntitySheep()
	: m_sheared(false)
{
}

EntitySheep::EntitySheep(World* var1)
	: EntityAnimal(var1)
	, m_sheared(false)
{
		texture = "/mob/sheep.png";
		setSize(0.9F, 1.3F);
}

EntitySheep::~EntitySheep() {
	// No dynamic resources
}

bool EntitySheep::attackEntityFrom(Entity* var1, int var2) {
		if(!m_sheared && dynamic_cast<EntityLiving*>(var1) != NULL) {
			m_sheared = true;
			int var3 = 1 + rand->nextInt(3);

			for(int var4 = 0; var4 < var3; ++var4) {
				EntityItem var5 = entityDropItem(Block.clothDarkGray.blockID + rand->nextInt(3), 1, 1.0F);
				var5.motionY += rand->nextFloat() * 0.05F;
				var5.motionX += (rand->nextFloat() - rand->nextFloat()) * 0.1F;
				var5.motionZ += (rand->nextFloat() - rand->nextFloat()) * 0.1F;
			}
		}

		return EntityAnimal::attackEntityFrom(var1, var2);
}

void EntitySheep::writeEntityToNBT(NBTTagCompound* var1) {
		EntityAnimal::writeEntityToNBT(var1);
		var1->setBoolean("Sheared", m_sheared);
}

void EntitySheep::readEntityFromNBT(NBTTagCompound* var1) {
		EntityAnimal::readEntityFromNBT(var1);
		m_sheared = var1->getBoolean("Sheared");
}

const std::string& EntitySheep::getEntityString() {
		return "Sheep";
}

const std::string& EntitySheep::getLivingSound() {
		return "mob.sheep";
}

const std::string& EntitySheep::getHurtSound() {
		return "mob.sheep";
}

const std::string& EntitySheep::getDeathSound() {
		return "mob.sheep";
}
