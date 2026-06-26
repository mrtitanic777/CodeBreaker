#include "stdafx.h"
// Auto-generated from EntityAnimals.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityAnimals.h"
#include "Common.h"
#include "BlockGrass.h"
#include "NBTTagCompound.h"
#include "World.h"
#include "Block.h"
#include "EntityCreature.h"

EntityAnimals::EntityAnimals()
{
}

EntityAnimals::EntityAnimals(World* var1)
	: EntityCreature(var1)
{
}

EntityAnimals::~EntityAnimals() {
	// No dynamic resources
}

float EntityAnimals::func_439_a(int var1, int var2, int var3) {
		return worldObj->getBlockId(var1, var2 - 1, var3) == Block::s_grass->blockID ? 10.0F : worldObj->getLightBrightness(var1, var2, var3) - 0.5F;
}

void EntityAnimals::writeEntityToNBT(NBTTagCompound* var1) {
		EntityCreature::writeEntityToNBT(var1);
}

void EntityAnimals::readEntityFromNBT(NBTTagCompound* var1) {
		EntityCreature::readEntityFromNBT(var1);
}

bool EntityAnimals::getCanSpawnHere() {
		int var1 = (int)floor(posX);
		int var2 = (int)floor(boundingBox.minY);
		int var3 = (int)floor(posZ);
		return worldObj->getBlockId(var1, var2 - 1, var3) == Block::s_grass->blockID && worldObj->getBlockLightValue(var1, var2, var3) > 8 && EntityCreature::getCanSpawnHere();
}

int EntityAnimals::func_421_b() {
		return 120;
}
