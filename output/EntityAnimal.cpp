#include "stdafx.h"
// Auto-generated from EntityAnimal.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimal.h"
#include "Common.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Block.h"
#include "EntityCreature.h"
#include "BlockGrass.h"

EntityAnimal::EntityAnimal()
{
}

EntityAnimal::EntityAnimal(World* var1)
	: EntityCreature(var1)
{
}

EntityAnimal::~EntityAnimal() {
	// No dynamic resources
}

float EntityAnimal::getBlockPathWeight(int var1, int var2, int var3) {
		return worldObj->getBlockId(var1, var2 - 1, var3) == Block::s_grass->blockID ? 10.0F : worldObj->getLightBrightness(var1, var2, var3) - 0.5F;
}

void EntityAnimal::writeEntityToNBT(NBTTagCompound* var1) {
		EntityCreature::writeEntityToNBT(var1);
}

void EntityAnimal::readEntityFromNBT(NBTTagCompound* var1) {
		EntityCreature::readEntityFromNBT(var1);
}

bool EntityAnimal::getCanSpawnHere(float var1, float var2, float var3) {
		return worldObj->getBlockLightValue((int)var1, (int)var2, (int)var3) > 8 && EntityCreature::getCanSpawnHere(var1, var2, var3);
}
